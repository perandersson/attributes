#ifndef _MUTABLE_CLASS_LOADER_H_
#define _MUTABLE_CLASS_LOADER_H_

#include <memory>
#include <map>
#include "attributes.h"

class MutableClass;

class MutableClassLoader : public IMutableClassLoader
{
	typedef map<string, shared_ptr<MutableClass>> Classes;

public:	
	virtual vector<const IClass*> GetClasses() const;
	virtual const IClass* FindClass(const string& name) const;
	virtual vector<const IClass*> FindExtendedClasses(const string& name) const;
	virtual vector<const IClass*> FindParentClasses(const string& name) const;
	virtual void PostConstruct(void* instance, IPostProcessor* processor) const;
	virtual IMutableClass* RegisterClass(const string& name, const type_info& type, IMutableConstructor* constructor);
	virtual void Build();

private:
	Classes mClasses;
};


#endif
