#include "Attribute.h"
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;

Attribute::Attribute(const string& name)
: mName(name)
{
	
}

Attribute::~Attribute()
{

}

void Attribute::AppendValues(const string& values)
{
	vector<string> temp;
	boost::split(temp, values, boost::is_any_of(","));

	for (auto it = temp.begin(), end = temp.end(); it != end; ++it) {
		auto potential = *it;

		string name = "";
		string value = "";
		vector<string> parts;
		boost::split(parts, potential, boost::is_any_of("="));
		name = parts[0];
		boost::replace_all(name, " ", "");

		if (parts.size() == 2) {
			value = parts[1];
			if (value[0] == ' ') {
				value = value.substr(1);
			}
			boost::replace_all(value, "\"", "");
		}

		mValues.insert(make_pair(name, value));
	}
}
