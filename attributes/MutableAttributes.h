#ifndef _MUTABLE_ATTRIBUTES_H_
#define _MUTABLE_ATTRIBUTES_H_

#include <memory>
#include <map>
#include "attributes.h"

class MutableAttribute;

struct MutableAttributeData
{
	shared_ptr<MutableAttribute> attribute;
	bool fromParent;
};

class MutableAttributes
{
	typedef map<string, shared_ptr<MutableAttributeData>> Attributes;

public:
	MutableAttributes();
	~MutableAttributes();

	/*!
		\brief Collect any attribute values not part of this attributes collection and mark them as "fromParent"
	*/
	void CollectAttributesFromParent(const MutableAttributes* attributes);

	/*!
		\brief Registers a new attribute with the given name
	*/
	MutableAttribute* RegisterAttribute(const string& name);

	/*!
		\brief Retrieves all attributes
	*/
	vector<const IAttribute*> GetAttributes() const;

	/*!
		\brief Tries to find an attribute
	*/
	const IAttribute* FindAttribute(const string& name) const;
	
	/*!
		\brief Tries to find an attribute
	*/
	const IAttribute* FindAttribute(const string& name, bool includeParent) const;

	/*!
		\brief Retrieves the names of all the attributes
	*/
	vector<string> GetAttributeNames() const;

private:
	Attributes mAttributes;
};

#endif
