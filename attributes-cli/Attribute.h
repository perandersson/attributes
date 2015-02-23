#pragma once
#include <sstream>
#include <map>
using namespace std;

static const char TAB = '\t';

class Attribute
{
	typedef map<string, string> Values;

public:
	Attribute(const string& name);
	~Attribute();

	void AppendValues(const string& values);

	/*!
		\brief Retrieves the name of the metadata
	*/
	inline const string& GetName() const {
		return mName;
	}

	inline map<string, string>::iterator Begin() {
		return mValues.begin();
	}

	inline map<string, string>::const_iterator End() {
		return mValues.end();
	}

private:
	string mName;
	Values mValues;
};
