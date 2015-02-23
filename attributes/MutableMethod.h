#ifndef _MUTABLE_METHOD_H_
#define _MUTABLE_METHOD_H_

#include "MutableAttributes.h"

class MutableMethod : public IMutableMethod
{
public:
	MutableMethod(const string& name, const string& signature, IRawMethod* rm);
	virtual ~MutableMethod();

	bool IsSameMethod(const MutableMethod* rhs) const;
	void CollectAttributesFromParent(const MutableMethod* method);

// IMethod
public:
	virtual const string& GetName() const;
	virtual const string& GetSignature() const;
	virtual vector<const IAttribute*> GetAttributes() const;
	virtual const IAttribute* FindAttribute(const string& name) const;
	virtual const IAttribute* FindAttribute(const string& name, bool includeParents) const;

// IMutableMethod
public:
	virtual IMutableAttribute* RegisterAttribute(const string& name);

private:
	string mName;
	string mSignature;
	IRawMethod* mRawMethod;
	MutableAttributes mAttributes;
};


#endif
