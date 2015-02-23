#pragma once
#include "IToken.h"
#include <boost/shared_ptr.hpp>
#include <map>

class AbstractToken : public IToken
{
	typedef map<string, boost::shared_ptr<Attribute>> MetaDataMap;

public:
	AbstractToken();
	~AbstractToken();

public:
	virtual void AddMetaData(Attribute* metadata);
	virtual const Attribute* GetMetaData(const string& name) const;

protected:
	Attribute* GetOrCreateMetaData(const string& name);

protected:
	MetaDataMap mMetaData;
};
