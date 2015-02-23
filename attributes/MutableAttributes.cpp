#include "MutableAttributes.h"
#include "MutableAttribute.h"

MutableAttributes::MutableAttributes()
{
}

MutableAttributes::~MutableAttributes()
{
}

void MutableAttributes::CollectAttributesFromParent(const MutableAttributes* attributes)
{
	for (auto& attributePair : attributes->mAttributes) {
		auto attribute = attributePair.second;
		

		auto data = shared_ptr<MutableAttributeData>(new MutableAttributeData());
		data->attribute = shared_ptr<MutableAttribute>(new MutableAttribute(attribute->attribute->GetName()));
		data->attribute->AddParameters(attribute->attribute.get());
		data->fromParent = true;
		mAttributes.insert(make_pair(attributePair.first, data));
	}
}

vector<const IAttribute*> MutableAttributes::GetAttributes() const
{
	vector<const IAttribute*> result;
	for (auto& attribute : mAttributes) {
		result.push_back(attribute.second->attribute.get());
	}
	return result;
}

MutableAttribute* MutableAttributes::RegisterAttribute(const string& name)
{
	auto it = mAttributes.find(name);
	if (it != mAttributes.end())
		return it->second->attribute.get();

	auto data = shared_ptr<MutableAttributeData>(new MutableAttributeData());
	data->attribute = shared_ptr<MutableAttribute>(new MutableAttribute(name));
	data->fromParent = false;
	mAttributes.insert(make_pair(name, data));
	return data->attribute.get();
}

const IAttribute* MutableAttributes::FindAttribute(const string& name) const
{
	return FindAttribute(name, true);
}

const IAttribute* MutableAttributes::FindAttribute(const string& name, bool includeParent) const
{
	auto it = mAttributes.find(name);
	if (it == mAttributes.end()) {
		return &_unavailableAttribute;
	}

	auto data = it->second.get();
	if (includeParent && data->fromParent)
		return data->attribute.get();
	else if (!includeParent && data->fromParent)
		return &_unavailableAttribute;

	return data->attribute.get();
}

vector<string> MutableAttributes::GetAttributeNames() const
{
	vector<string> result;
	for (auto& attribute : mAttributes) {
		result.push_back(attribute.second->attribute->GetName());
	}
	return result;
}
