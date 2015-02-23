#include "MutableMethod.h"
#include "MutableAttribute.h"

MutableMethod::MutableMethod(const string& name, const string& signature, IRawMethod* rm)
: mName(name), mSignature(signature), mRawMethod(rm) {

}

MutableMethod::~MutableMethod()
{

}

bool MutableMethod::IsSameMethod(const MutableMethod* rhs) const
{
	return mName == rhs->GetName() && rhs->GetSignature() == mSignature;
}

void MutableMethod::CollectAttributesFromParent(const MutableMethod* method)
{
	mAttributes.CollectAttributesFromParent(&method->mAttributes);
}

const string& MutableMethod::GetName() const
{
	return mName;
}

const string& MutableMethod::GetSignature() const
{
	return mSignature;
}

vector<const IAttribute*> MutableMethod::GetAttributes() const
{
	return mAttributes.GetAttributes();
}

const IAttribute* MutableMethod::FindAttribute(const string& name) const
{
	return FindAttribute(name, true);
}

const IAttribute* MutableMethod::FindAttribute(const string& name, bool includeParents) const
{
	const IAttribute* attribute = mAttributes.FindAttribute(name);
	if (!attribute->Exists() && includeParents) {

	}
	return attribute;
}

IMutableAttribute* MutableMethod::RegisterAttribute(const string& name)
{
	return mAttributes.RegisterAttribute(name);
}
