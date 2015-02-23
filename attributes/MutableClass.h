#ifndef _MUTABLE_CLASS_H_
#define _MUTABLE_CLASS_H_

#include <memory>
#include <map>
#include <set>
#include "attributes.h"
#include "IPostProcessorDelegate.h"
#include "MutableAttributes.h"

class MutableMethod;
class MutableClassLoader;

class MutableClass : public IMutableClass
{
	typedef map<string, shared_ptr<MutableMethod>> Methods;
	typedef vector<string> InheritsFromClasseNames;
	typedef vector<const MutableClass*> InheritsFromClasses;
	typedef vector<shared_ptr<IPostProcessorDelegate>> PostProcessors;

public:
	MutableClass(const string& name, const type_info& type, IMutableConstructor* constructor, MutableClassLoader* classLoader);
	virtual ~MutableClass();

	void ConnectInheritedClasses();
	void ConnectAttributes();
	void ConnectMethods();
	void AttachPostProcessors();
	void AttachPostProcessorsForClass(const MutableClass* clazz);
	
	/*!
		\brief Retrieves all the attributes attached to this class
	*/
	set<string> CollectClassAttributes() const;
	
	/*!
		\brief Retrieves all the attributes attached to the methods of this class
	*/
	set<string> CollectMethodAttributes(const MutableMethod* method) const;

	/*!
		\brief Retrieves all the attributes this class is marked to intercept
	*/
	vector<string> InterceptAttributes() const;
	bool InterceptsClass(const MutableClass* clazz) const;
	bool InterceptsMethod(const MutableClass* clazz, const MutableMethod* method) const;
	const IAttribute* GetInterceptor() const;

	// Inheritance
	void InheritedBy(const MutableClass* clazz);
	bool IsImplementedBy(const string& className) const;
	bool Implements(const string& className) const;
	IClassInterceptor* CastToInterceptor(void* ptr) const;
	IPostProcessor* CastToProcessor(void* ptr) const;

// IClass
public:
	virtual void* NewInstance() const;
	virtual const string& GetName() const;
	virtual vector<const IMethod*> GetMethods() const;
	virtual const IMethod* GetMethod(const string& name, const string& signature) const;
	virtual const IMethod* GetMethod(const IMethod* method) const;
	virtual vector<const IAttribute*> GetAttributes() const;
	virtual const IAttribute* FindAttribute(const string& name) const;
	virtual const IAttribute* FindAttribute(const string& name, bool includeInherited) const;
	virtual const IConstructor* GetConstructor() const;

// IMutableClass
public:
	virtual void InheritsFrom(const string& name);
	virtual IMutableAttribute* RegisterAttribute(const string& name);
	virtual IMutableMethod* RegisterMethod(const string& name, const string& signature, IRawMethod* rm);

private:
	string mName;
	const type_info& mType;

	MutableAttributes mAttributes;
	IMutableConstructor* mConstructor;
	Methods mMethods;

	InheritsFromClasseNames mInheritsFrom;
	InheritsFromClasseNames mInheritedBy;
	InheritsFromClasses mInheritsFromClasses;
	InheritsFromClasses mInheritedByClasses;

	MutableClassLoader* mClassLoader;
	PostProcessors mPostProcessors;
};

#endif
