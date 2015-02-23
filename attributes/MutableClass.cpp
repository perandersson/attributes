#include "MutableClass.h"
#include "MutableMethod.h"
#include "MutableAttribute.h"
#include "MutableClassLoader.h"
#include "MethodPostProcessor.h"
#include "ClassPostProcessor.h"
#include <algorithm>
#include <set>

static const string GetProcessorName(const string& className) {
	return string("PostProcessor<" + className + ">");
}

MutableClass::MutableClass(const string& name, const type_info& type, IMutableConstructor* constructor, MutableClassLoader* classLoader)
: mName(name), mType(type), mConstructor(constructor), mClassLoader(classLoader)
{

}

MutableClass::~MutableClass()
{

}

void MutableClass::ConnectInheritedClasses()
{
	for (auto className : mInheritsFrom) {
		auto clazz = const_cast<MutableClass*>(static_cast<const MutableClass*>(mClassLoader->FindClass(className)));
		if (clazz != nullptr) {
			mInheritsFromClasses.push_back(clazz);
			clazz->InheritedBy(this);
		}
	}
}

void MutableClass::ConnectAttributes()
{
	for (auto parent : mInheritsFromClasses) {
		mAttributes.CollectAttributesFromParent(&parent->mAttributes);
	}
}

void MutableClass::ConnectMethods()
{
	for (auto parent : mClassLoader->FindParentClasses(this->GetName())) {
		for (auto methodPair : mMethods) {
			auto method = static_cast<MutableMethod*>(methodPair.second.get());
			auto parentMethod = static_cast<const MutableMethod*>(parent->GetMethod(method));
			if (parentMethod != nullptr) {
				method->CollectAttributesFromParent(parentMethod);
			}
		}
	}
}

void MutableClass::AttachPostProcessors()
{
	const auto processorName = GetProcessorName(mName);
	const auto extendedClasses = mClassLoader->FindExtendedClasses(processorName);
	for (auto clazz : extendedClasses) {
		AttachPostProcessorsForClass(static_cast<const MutableClass*>(clazz));
		
	}

	for (auto clazz : mInheritsFromClasses) {
		const auto processorName = GetProcessorName(clazz->GetName());
		const auto extendedClasses = mClassLoader->FindExtendedClasses(processorName);
		for (auto clazz : extendedClasses) {
			AttachPostProcessorsForClass(static_cast<const MutableClass*>(clazz));
		}
	}

}

void MutableClass::AttachPostProcessorsForClass(const MutableClass* clazz)
{
	if (clazz->InterceptsClass(this)) {
		auto postProcessor = reinterpret_cast<IPostProcessor*>(clazz->NewInstance());
		auto instance = shared_ptr<IPostProcessor>(postProcessor);
		mPostProcessors.push_back(shared_ptr<IPostProcessorDelegate>(new ClassPostProcessor(instance)));
	}

	for (auto& method : mMethods) {
		auto mutableMethod = static_cast<const MutableMethod*>(method.second.get());
		if (clazz->InterceptsMethod(this, mutableMethod)) {
			auto postProcessor = reinterpret_cast<IPostProcessor*>(clazz->NewInstance());
			auto instance = shared_ptr<IPostProcessor>(postProcessor);
			mPostProcessors.push_back(shared_ptr<IPostProcessorDelegate>(new MethodPostProcessor(instance, mutableMethod)));
		}
	}
}

set<string> MutableClass::CollectClassAttributes() const
{
	set<string> result;
	for (auto attributeName : mAttributes.GetAttributeNames()) {
		result.insert(attributeName);
	}

	for (auto clazz : mInheritsFromClasses) {
		auto attributes = clazz->CollectClassAttributes();
		result.insert(attributes.begin(), attributes.end());
	}

	return result;
}

set<string> MutableClass::CollectMethodAttributes(const MutableMethod* method) const
{
	set<string> result;
	for (auto attribute : method->GetAttributes()) {
		result.insert(attribute->GetName());
	}

	for (auto clazz : mInheritsFromClasses) {
		auto inheritedMethod = clazz->GetMethod(method);
		if (inheritedMethod != nullptr) {
			auto attributes = clazz->CollectMethodAttributes(static_cast<const MutableMethod*>(inheritedMethod));
			result.insert(attributes.begin(), attributes.end());
		}
	}

	return result;
}

vector<string> MutableClass::InterceptAttributes() const
{
	auto interceptor = GetInterceptor();
	if (interceptor != nullptr) {
		return interceptor->GetParameters();
	}
	return vector<string>();
}

bool MutableClass::InterceptsClass(const MutableClass* clazz) const
{
	for (auto intercept : InterceptAttributes()) {
		set<string> attributes = clazz->CollectClassAttributes();
		for (auto attribute : attributes) {
			if (intercept == attribute) {
				return true;
			}
		}
	}
	return false;
}

bool MutableClass::InterceptsMethod(const MutableClass* clazz, const MutableMethod* method) const
{
	for (auto intercept : InterceptAttributes()) {
		set<string> attributes = clazz->CollectMethodAttributes(method);
		for (auto attribute : attributes) {
			if (intercept == attribute) {
				return true;
			}
		}
	}
	return false;
}

const IAttribute* MutableClass::GetInterceptor() const
{
	static const string INTERCEPTOR_NAME(STRINGIFY(INTERCEPTS));
	auto attributes = mAttributes.GetAttributes();
	for (auto attribute : attributes) {
		if (attribute->GetName() == INTERCEPTOR_NAME) {
			return attribute;
		}
	}
	return &_unavailableAttribute;
}

void MutableClass::InheritedBy(const MutableClass* clazz)
{
	mInheritedBy.push_back(clazz->GetName());
	mInheritedByClasses.push_back(clazz);
}

bool MutableClass::IsImplementedBy(const string& className) const
{
	for (auto clazz : mInheritedByClasses) {
		if (clazz->GetName() == className) {
			return true;
		}
	}

	return false;
}

bool MutableClass::Implements(const string& className) const
{
	for (auto clazz : mInheritsFromClasses) {
		if (clazz->GetName() == className) {
			return true;
		}

		if (clazz->Implements(className)) {
			return true;
		}
	}

	for (auto& name : mInheritsFrom) {
		if (className == name) {
			return true;
		}
	}

	return false;
}

void* MutableClass::NewInstance() const
{
	void* instance = mConstructor->NewInstance();
	for (auto postProcessor : mPostProcessors) {
		postProcessor->Process(this, instance);
	}
	return instance;
}

IClassInterceptor* MutableClass::CastToInterceptor(void* ptr) const
{
	return mConstructor->CastToInterceptor(ptr);
}

IPostProcessor* MutableClass::CastToProcessor(void* ptr) const
{
	return mConstructor->CastToProcessor(ptr);
}

const string& MutableClass::GetName() const
{
	return mName;
}

vector<const IMethod*> MutableClass::GetMethods() const
{
	vector<const IMethod*> result;
	for (auto& method : mMethods) {
		result.push_back(method.second.get());
	}
	return result;
}

const IMethod* MutableClass::GetMethod(const string& name, const string& signature) const
{
	auto it = mMethods.find(name + "+" + signature);
	if (it == mMethods.end()) {
		for (auto clazz : mInheritsFromClasses) {
			auto method = clazz->GetMethod(name, signature);
			if (method != nullptr) {
				return method;
			}
		}
		return nullptr;
	}
	return it->second.get();
}

const IMethod* MutableClass::GetMethod(const IMethod* method) const
{
	auto& name = method->GetName();
	auto& signature = method->GetSignature();
	return GetMethod(name, signature);
}

vector<const IAttribute*> MutableClass::GetAttributes() const
{
	vector<const IAttribute*> result = mAttributes.GetAttributes();
	for (auto clazz : mInheritsFromClasses) {
		auto attributes = clazz->GetAttributes();
		result.insert(result.end(), attributes.begin(), attributes.end());
	}
	return result;
}

const IAttribute* MutableClass::FindAttribute(const string& name) const
{
	return FindAttribute(name, true);
}

const IAttribute* MutableClass::FindAttribute(const string& name, bool includeInherited) const
{
	auto attribute = mAttributes.FindAttribute(name);
	if (attribute->Exists()) {
		return attribute;
	}

	if (!includeInherited) {
		return &_unavailableAttribute;
	}

	for (auto clazz : mInheritsFromClasses) {
		auto attribute = clazz->FindAttribute(name, includeInherited);
		if (attribute->Exists()) {
			return attribute;
		}
	}

	return &_unavailableAttribute;
}

void MutableClass::InheritsFrom(const string& name)
{
	mInheritsFrom.push_back(name);
}

IMutableAttribute* MutableClass::RegisterAttribute(const string& name)
{
	return mAttributes.RegisterAttribute(name);
}

IMutableMethod* MutableClass::RegisterMethod(const string& name, const string& signature, IRawMethod* rm)
{
	auto method = shared_ptr<MutableMethod>(new MutableMethod(name, signature, rm));
	mMethods.insert(make_pair(name + "+" + signature, method));
	return method.get();
}

const IConstructor* MutableClass::GetConstructor() const
{
	return mConstructor;
}
