#ifndef _MUTABLE_METADATA_H_
#define _MUTABLE_METADATA_H_

#include <memory>
#include <map>
#include "attributes.h"

class MutableAttribute : public IMutableAttribute
{
	typedef map<string, string> Values;

public:
	MutableAttribute(const string& name);
	virtual ~MutableAttribute();

	void AddParameters(const MutableAttribute* rhs);

public:
	virtual vector<string> GetParameters() const;
	virtual void AddParameter(const string& key, const string& value);
	virtual const string& GetName() const {
		return mName;
	}
	virtual bool Exists() const;
	virtual float ValueAsFloat(const string& key, float defaultVal) const;
	virtual string ValueAsString(const string& key, const string& defaultVal) const;

private:
	string mName;
	Values mValues;
};

class UnavailableAttribute : public IMutableAttribute
{
public:
	virtual void AddParameter(const string& name, const string& value) {}
	virtual vector<string> GetParameters() const { return vector<string>(); }
	virtual const string& GetName() const { return emptyName; }
	virtual bool Exists() const { return false; }
	virtual float ValueAsFloat(const string& key, float defaultVal) const { return defaultVal; }
	virtual string ValueAsString(const string& key, const string& defaultVal) const { return defaultVal; }
private:
	string emptyName;
};

extern UnavailableAttribute _unavailableAttribute;

#endif
