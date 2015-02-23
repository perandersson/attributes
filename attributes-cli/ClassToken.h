#pragma once
#include "AbstractToken.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
using namespace boost;


// class Name
// struct Name
static const regex CLASS_REGEX_1("((class|struct)[\\s]+([A-Za-z0-9]+))");

class ClassToken : public AbstractToken
{
public:
	ClassToken(const filesystem::path& filename, const string& name, const vector<string>& inheritsClasses, const string& head, const string& body);
	~ClassToken();

	/*!
		\brief Retrieves the name of this class
	*/
	string GetName() const;

	/*!
		\brief the declared name of this class
	*/
	const string& GetDeclaredName() const { return mName; }

	void ParseHead();

	void ParseBody();

// IToken
public:
	virtual void Visit(stringstream& header, stringstream& body);

private:
	void ParseMethods(const string& data);
	void ParseProperties(const string& data);

private:
	const filesystem::path mFileName;
	const string mName;
	const string mHead;
	const string mBody;

	vector<boost::shared_ptr<IToken>> tokens;
	vector<string> inheritsClasses;
};
