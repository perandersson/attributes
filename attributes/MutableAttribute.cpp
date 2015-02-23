#include "MutableAttribute.h"
UnavailableAttribute _unavailableAttribute;

MutableAttribute::MutableAttribute(const string& name)
: mName(name)
{
}

MutableAttribute::~MutableAttribute()
{
}

void MutableAttribute::AddParameters(const MutableAttribute* rhs)
{
	for (auto& rhsValue : rhs->mValues) {
		const auto& name = rhsValue.first;
		const auto& value = rhsValue.second;
		mValues.insert(make_pair(name, value));
	}
}

vector<string> MutableAttribute::GetParameters() const
{
	vector<string> keys;
	for (auto it = mValues.begin(), end = mValues.end(); it != end; ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

void MutableAttribute::AddParameter(const string& key, const string& value)
{
	mValues.insert(make_pair(key, value));
}

bool MutableAttribute::Exists() const
{
	return true;
}

float MutableAttribute::ValueAsFloat(const string& key, float defaultVal) const
{
	auto it = mValues.find(key);
	if (it == mValues.end()) {
		return defaultVal;
	}

	float val = defaultVal;
	sscanf_s(it->second.c_str(), "%f", &val);
	return val;
}

string MutableAttribute::ValueAsString(const string& key, const string& defaultVal) const
{
	auto it = mValues.find(key);
	if (it == mValues.end()) {
		return defaultVal;
	}

	return it->second;
}
