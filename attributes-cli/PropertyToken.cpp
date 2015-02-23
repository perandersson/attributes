#include "PropertyToken.h"
#include "ClassToken.h"

PropertyToken::PropertyToken(const ClassToken* parent, const string& type, const string& name)
: AbstractToken(), parent(parent), mType(type), mName(name)
{
}

PropertyToken::~PropertyToken()
{
}

void PropertyToken::Visit(std::stringstream& header, std::stringstream& body)
{
	const string className = parent->GetDeclaredName();
	body << "\tc->AddProperty(\"" << GetName() << "\", &" << className << "::" << mName << ");" << std::endl;
}

string PropertyToken::GetName() const
{
	return mName;
}
