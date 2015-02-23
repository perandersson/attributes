#pragma once
#include "AbstractToken.h"

#include <boost/regex.hpp>
using namespace boost;

static const regex PPROPERTY_REGEX("PPROPERTY\\s*\\(([a-zA-Z0-9\\,\\=\\s\\\"\\.]+)\\)\\s+([A-Za-z0-9\\:\\*\\&\\s\\<\\>]+)\\s+([A-Za-z0-9]+)\\s*\\;");

class ClassToken;
class PropertyToken : public AbstractToken
{
public:
	PropertyToken(const ClassToken* parent, const string& type, const string& name);
	~PropertyToken();
	
	/*!
		\brief Retrieves the name of this class
	*/
	string GetName() const;

	/*!
		\brief the declared name of this class
	*/
	const string& GetDeclaredName() const { return mName; }

// IToken
public:
	virtual void Visit(stringstream& header, stringstream& body);

private:
	const ClassToken* parent;
	const string mType;
	const string mName;
};
