#include "AbstractToken.h"
#include "MethodToken.h"
#include <boost/algorithm/string.hpp>
using namespace std;

AbstractToken::AbstractToken()
{
}

AbstractToken::~AbstractToken()
{
}

void AbstractToken::AddMetaData(Attribute* metadata)
{
	mMetaData.insert(make_pair(metadata->GetName(), boost::shared_ptr<Attribute>(metadata)));
}

const Attribute* AbstractToken::GetMetaData(const string& name) const
{
	auto it = mMetaData.find(name);
	if (it == mMetaData.end())
		return nullptr;

	return it->second.get();
}

Attribute* AbstractToken::GetOrCreateMetaData(const string& name)
{
	auto it = mMetaData.find(name);
	if (it != mMetaData.end())
		return it->second.get();

	Attribute* md = new Attribute(name);
	mMetaData.insert(make_pair(name, boost::shared_ptr<Attribute>(md)));
	return md;
}
