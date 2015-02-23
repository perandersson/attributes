#pragma once
#include "AbstractToken.h"

#include <boost/regex.hpp>
using namespace boost;

static const regex METHOD_REGEX("([A-Z0-9\\_]{4,})\\s*\\(([a-zA-Z0-9\\,\\=\\s\\\"\\.]*)\\)\\s+(inline)*(virtual)*\\s+([A-Za-z0-9\\:\\*\\&\\s\\<\\>]+)\\s+([A-Za-z0-9]+)\\s*\\(([A-Za-z0-9\\:\\*\\&\\s\\<\\>\\,]*)\\)");

class ClassToken;
class MethodToken : public AbstractToken
{
public:
	MethodToken(const ClassToken* parent, const string& returnType, const string& name, const string& params, const string& head);
	~MethodToken();
	
	/*!
		\brief Retrieves the name of this class
	*/
	string GetName() const;

	/*!
		\brief the declared name of this class
	*/
	const string& GetDeclaredName() const { return mName; }

	/*!
		\brief Parse the head properties
	*/
	void ParseHead();

public:
	virtual void Visit(stringstream& header, stringstream& body);

private:
	const ClassToken* parent;
	const string mName;
	const string mReturnType;
	const string mParameters;
	const string mHead;
};
