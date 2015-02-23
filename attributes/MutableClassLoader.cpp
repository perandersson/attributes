#include "MutableClassLoader.h"
#include "MutableClass.h"
#include "ClassPostProcessor.h"

vector<const IClass*> MutableClassLoader::GetClasses() const
{
	vector<const IClass*> result;
	Classes::const_iterator it = mClasses.begin();
	const Classes::const_iterator end = mClasses.end();
	for (; it != end; ++it) {
		const IClass* md = it->second.get();
		result.push_back(md);
	}
	return result;
}

const IClass* MutableClassLoader::FindClass(const string& name) const
{
	auto it = mClasses.find(name);
	if (it == mClasses.end())
		return nullptr;

	return it->second.get();
}

vector<const IClass*> MutableClassLoader::FindExtendedClasses(const string& name) const
{
	vector<const IClass*> result;
	Classes::const_iterator it = mClasses.begin();
	const Classes::const_iterator end = mClasses.end();
	for (; it != end; ++it) {
		const MutableClass* md = it->second.get();
		if (md->Implements(name)) {
			result.push_back(md);
		}
	}
	return result;
}

vector<const IClass*> MutableClassLoader::FindParentClasses(const string& name) const
{
	vector<const IClass*> result;
	for (auto clazzPair : mClasses) {
		auto clazz = clazzPair.second.get();
		if (clazz->IsImplementedBy(name)) {
			result.push_back(clazz);
		}
	}
	return result;
}


void MutableClassLoader::PostConstruct(void* instance, IPostProcessor* processor) const
{

}

IMutableClass* MutableClassLoader::RegisterClass(const string& name, const type_info& type, IMutableConstructor* constructor)
{
	auto it = mClasses.find(name);
	if (it != mClasses.end()) {
		throw std::exception((string("More than one class is registered with the name: ") + name).c_str());
	}

	auto clazz = shared_ptr<MutableClass>(new MutableClass(name, type, constructor, this));
	mClasses.insert(make_pair(name, clazz));
	return clazz.get();
}

void MutableClassLoader::Build()
{
	for (auto& clazz : mClasses) {
		clazz.second->ConnectInheritedClasses();
	}

	for (auto& clazz : mClasses) {
		clazz.second->ConnectAttributes();
	}

	for (auto& clazz : mClasses) {
		clazz.second->ConnectMethods();
	}

	for (auto& clazz : mClasses) {
		clazz.second->AttachPostProcessors();
	}

	for (auto& clazzPair : mClasses) {
		auto clazz = clazzPair.second.get();
		const auto processorName = string("ClassInterceptor<" + clazz->GetName() + ">");
		const auto extendedClasses = FindExtendedClasses(processorName);
		for (auto extendedClass : extendedClasses) {
			for (auto& clazzPair : mClasses) {
				auto clazz = clazzPair.second.get();
				if (static_cast<const MutableClass*>(extendedClass)->InterceptsClass(clazz)) {
					void* ptr = extendedClass->NewInstance();
					auto interceptor = static_cast<const MutableClass*>(extendedClass)->CastToInterceptor(ptr);
					interceptor->ProcessClass(clazz);
					delete interceptor;
				}
			}
		}
	}
}
