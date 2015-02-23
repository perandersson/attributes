#include "MethodToken.h"
#include "ClassToken.h"

MethodToken::MethodToken(const ClassToken* parent, const string& returnType, const string& name, const string& params, const string& head)
: AbstractToken(), parent(parent), mReturnType(returnType), mName(name), mParameters(params), mHead(head)
{
}

MethodToken::~MethodToken()
{
}

void MethodToken::Visit(stringstream& header, stringstream& body) 
{
	const string className = parent->GetDeclaredName();
	body << TAB << TAB << "{" << endl;
	body << TAB << TAB << TAB << "IMutableMethod* m = c->RegisterMethod(\"" << GetName() << "\", \"" << mParameters << "\", cntr::for_method(&" << className << "::" << mName << "));" << endl;

	for (auto it = mMetaData.begin(),
		end = mMetaData.end(); it != end; ++it) {
		auto& key = it->first;
		auto& value = it->second;

		body << TAB << TAB << TAB << "{" << endl;
		body << TAB << TAB << TAB << TAB << "IMutableAttribute* a = m->RegisterAttribute(\"" << key << "\");" << endl;

		map<string, string>::iterator mit = value->Begin();
		map<string, string>::const_iterator mend = value->End();
		for (; mit != mend; ++mit) {
			body << TAB << TAB << TAB << TAB << "a->AddParameter(\"" << mit->first << "\", \"" << mit->second << "\");" << endl;
		}

		body << TAB << TAB << TAB << "}" << endl;
	}
	body << TAB << TAB << "}" << endl;
}

string MethodToken::GetName() const
{
	return mName;
}

void MethodToken::ParseHead()
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
