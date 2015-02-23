#include "ClassToken.h"
#include "MethodToken.h"
#include "PropertyToken.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;


ClassToken::ClassToken(const filesystem::path& filename, const std::string& name, const std::vector<std::string>& inheritsClasses, const string& head, const string& body)
: AbstractToken(), mName(name), mFileName(filename), inheritsClasses(inheritsClasses), mHead(head), mBody(body)
{
}

ClassToken::~ClassToken()
{
}

void ClassToken::ParseHead()
{
	// METADATA1()
	// METADATA2(...)
	static const regex METADATA_ROWS("[\\s]+(([A-Z0-9_]{4,})[\\s]*\\(([a-zA-Z0-9,\\.=\\s\\\"]*)\\))");

	if (!mHead.empty()) {
		boost::sregex_iterator it(mHead.begin(), mHead.end(), METADATA_ROWS);
		const boost::sregex_iterator e;
		for (; it != e; ++it) {
			const boost::smatch match = *it;

			const string val = match[1];
			const string key = match[2];
			const string values = match[3];

			auto metaData = GetOrCreateMetaData(key);
			if (!values.empty())
				metaData->AppendValues(values);
		}
	}
}

void ClassToken::ParseBody()
{
	ParseMethods(mBody);
	ParseProperties(mBody);
}

void ClassToken::ParseMethods(const std::string& data)
{
	static const regex METHOD_REGEX("((virtual|inline)??\\s*(virtual|inline)??)\\s+([a-zA-Z0-9:\\*\\&\\<\\>]+)\\s([A-Za-z0-9]+)\\s*\\(([a-zA-Z0-9:\\*\\&\\<\\>,\\s]*)\\)\\s*(=\\s0;)??");

	auto start = data.begin();
	auto end = data.end();

	size_t startOffset = 0;

	boost::sregex_iterator it(data.begin(), data.end(), METHOD_REGEX);
	const boost::sregex_iterator e;
	for (; it != e; ++it) {
		boost::smatch match = *it;

		const size_t len = match.position() - startOffset;
		const string betweenMethods = data.substr(startOffset, len);
		startOffset = match.position();
		const string extra = match[1];
		const string returnType = match[4];
		const string methodName = match[5];
		const string params = match[6];
		const string virtualmark = match[7];

		if (returnType == string(":"))
			continue;
		
		boost::shared_ptr<MethodToken> token(new MethodToken(this, returnType, methodName, params, betweenMethods));
		token->ParseHead();
		tokens.push_back(token);
	}
}

void ClassToken::ParseProperties(const std::string& data)
{
	auto start = data.begin();
	auto end = data.end();

	// Read the HTML file into the string s...
	boost::sregex_iterator it(data.begin(), data.end(), PPROPERTY_REGEX);
	boost::sregex_iterator e;
	for (; it != e; ++it) {
		boost::smatch match = *it;

		const string metadata = match[1];
		const string type = match[2];
		const string name = match[3];

		boost::shared_ptr<IToken> token(new PropertyToken(this, type, name));
		//token->ParseFile(data);
		tokens.push_back(token);
	}
}

void ClassToken::Visit(stringstream& header, stringstream& body)
{
	header << "#include \"" << boost::filesystem::canonical(mFileName).string() << "\"" << endl;

	const string className = GetName();

	body << TAB << "// Class: " << className << endl;
	body << TAB << "{" << endl;
	body << TAB << TAB << "IMutableClass* c = cl->RegisterClass(\"" << className << "\", typeid(" << mName << "), cntr::for_class<" << mName << ">());" << endl;

	for (auto it = inheritsClasses.begin(), end = inheritsClasses.end(); it != end; ++it) {
		body << TAB << TAB << "c->InheritsFrom(\"" << *it << "\");" << endl;
	}

	for (auto it = mMetaData.begin(), 
		end = mMetaData.end(); it != end; ++it) {
		auto& key = it->first;
		auto& value = it->second;

		body << TAB << TAB << "{" << endl;
		body << TAB << TAB << TAB << "IMutableAttribute* a = c->RegisterAttribute(\"" << key << "\");" << endl;

		map<string, string>::iterator mit = value->Begin();
		map<string, string>::const_iterator mend = value->End();
		for (; mit != mend; ++mit) {
			body << TAB << TAB << TAB << "a->AddParameter(\"" << mit->first << "\", \"" << mit->second << "\");" << endl;
		}

		body << TAB << TAB << "}" << endl;
	}

	for (auto it = tokens.begin(), end = tokens.end(); it != end; ++it) {
		(*it)->Visit(header, body);
	}
	body << TAB << "}" << endl;
	body << endl;
}

string ClassToken::GetName() const
{
	return mName;
}
