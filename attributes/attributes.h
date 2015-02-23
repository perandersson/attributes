#pragma once
#ifndef _METASOURCE_H_
#define _METASOURCE_H_

#include <cassert>
#include <vector>
#include <typeinfo>
#include <functional>
#include <string>
#include <memory>
using namespace std;

#define STRINGIFY(X) #X

class IClass;
class IMutableClass;

class IMethod;
class IMutableMethod;

class IAttribute;
class IMutableAttribute;

class IClassLoader;
class IMutableClassLoader;

class IPostProcessor;
class IClassInterceptor;

class IConstructor;
class IMutableConstructor;

class IRawMethod;

/*!
	\brief
*/
class IPostProcessor
{
public:
	virtual ~IPostProcessor() {}

	virtual void ProcessInstance(const IClass* clazz, void* ptr) const = 0;
	virtual void ProcessInstance(const IClass* clazz, const IMethod* method, void* ptr) const = 0;

};

/*!
	\brief
*/
class IClassInterceptor
{
public:
	virtual ~IClassInterceptor() {}

	/*!
		\brief 
	*/
	virtual void ProcessClass(const IClass* clazz) const = 0;
};

/*!
	\brief 
*/
class IConstructor
{
public:
	virtual ~IConstructor() {}

	/*!
		\brief Create a new instance using this constructor

		\return The memory location of the new instance
	*/
	virtual void* NewInstance() const = 0;
};

class IAttribute
{
public:
	virtual ~IAttribute() {}

	/*!
		\brief Retrieves all the parameter names
	*/
	virtual vector<string> GetParameters() const = 0;

	/*!
		\brief Checks if this attribute exists or not

		\return TRUE if this attribute exists; FALSE otherwise
	*/
	virtual bool Exists() const = 0;

	/*!
		\brief Retrieves the name of this attribute
	*/
	virtual const string& GetName() const = 0;

	/*!
		\brief Retrieves the underlying value as a floating number. 

		Returns the default value if the underlying value is not a floating number or if this attribute instance does not exists.

		\param key
		\param defaultVal
	*/
	virtual float ValueAsFloat(const string& key, float defaultVal) const = 0;
	
	/*!
		\brief Retrieves the underlying value as a string. 

		Returns the default value if the underlying value is not a string number or if this attribute instance does not exists.

		\param key
		\param defaultVal
	*/
	virtual string ValueAsString(const string& key, const string& defaultVal) const = 0;
};

class IRawMethod
{
public:
	virtual ~IRawMethod() {}
};

class IMethod
{
public:
	virtual ~IMethod() {}

public:
	virtual const string& GetName() const = 0;

	virtual const string& GetSignature() const = 0;

	virtual vector<const IAttribute*> GetAttributes() const = 0;

	virtual const IAttribute* FindAttribute(const string& name) const = 0;

	virtual const IAttribute* FindAttribute(const string& name, bool includeParents) const = 0;
};

class IClass
{
public:
	virtual ~IClass() {}

protected:

public:
	virtual void* NewInstance() const = 0;

	virtual const string& GetName() const = 0;

	virtual vector<const IMethod*> GetMethods() const = 0;

	virtual const IMethod* GetMethod(const string& name, const string& signature) const = 0;

	virtual const IMethod* GetMethod(const IMethod* method) const = 0;

	virtual vector<const IAttribute*> GetAttributes() const = 0;

	virtual const IAttribute* FindAttribute(const string& name) const = 0;

	virtual const IAttribute* FindAttribute(const string& name, bool includeInherited) const = 0;

	virtual const IConstructor* GetConstructor() const = 0;
};

/*!

*/
class IClassLoader
{
public:
	virtual ~IClassLoader() {}

public:
	/*!
		\brief
	*/
	virtual vector<const IClass*> GetClasses() const = 0;

	/*!
		\brief
	*/
	virtual const IClass* FindClass(const string& name) const = 0;

	/*!
		\brief
	*/
	virtual vector<const IClass*> FindExtendedClasses(const string& name) const = 0;
	
	/*!
		\brief
	*/
	virtual vector<const IClass*> FindParentClasses(const string& name) const = 0;

	/*!
		\brief
	*/
	virtual void PostConstruct(void* instance, IPostProcessor* processor) const = 0;
};

class IMutableConstructor : public IConstructor
{
public:
	virtual ~IMutableConstructor() {}

	virtual IClassInterceptor* CastToInterceptor(void* ptr) const = 0;
	virtual IPostProcessor* CastToProcessor(void* ptr) const = 0;
};

class IMutableAttribute : public IAttribute
{
public:
	virtual void AddParameter(const string& name, const string& value) = 0;
};

class IMutableClass : public IClass
{
public:
	virtual void InheritsFrom(const string& name) = 0;
	virtual IMutableAttribute* RegisterAttribute(const string& name) = 0;
	virtual IMutableMethod* RegisterMethod(const string& name, const string& signature, IRawMethod* rm) = 0;
};

class IMutableMethod : public IMethod {
public:
	virtual IMutableAttribute* RegisterAttribute(const string& name) = 0;
};

class IMutableClassLoader : public IClassLoader
{
public:
	virtual IMutableClass* RegisterClass(const string& name, const type_info& type, IMutableConstructor* constructor) = 0;
	virtual void Build() = 0;
};

template<class T>
class TConstructor : public IMutableConstructor
{
public:
	virtual void* NewInstance() const {
		T* ptr = new T();
		return ptr;
	}

	virtual IClassInterceptor* CastToInterceptor(void* ptr) const {
		return dynamic_cast<IClassInterceptor*>(reinterpret_cast<T*>(ptr));
	}

	virtual IPostProcessor* CastToProcessor(void* ptr) const {
		return dynamic_cast<IPostProcessor*>(reinterpret_cast<T*>(ptr));
	}
};

template<class T>
class TClass
{
public:
	TClass();
	TClass(const IClass* clazz);
	TClass(const TClass<T>& rhs);
	~TClass();

	TClass<T>& operator=(const TClass<T>& rhs);

	T* NewInstance() const;

	const TConstructor<T>* GetConstructor() const;

	const string& GetName() const;

	vector<const IMethod*> GetMethods() const;

	const IMethod* GetMethod(const string& name, const string& signature) const;

	const IMethod* GetMethod(const IMethod* method) const;

	vector<const IAttribute*> GetAttributes() const;

	const IAttribute* FindAttribute(const string& name) const;

	const IAttribute* FindAttribute(const string& name, bool includeInherited) const;

private:
	const IClass* mClass;
};

//
// Intecepts the supplied attributes. Must be placed on a class implementing {@code PostProcessor<T>} and/or {@code ClassInterceptor<T>}
#define INTERCEPTS(...)

template<class T>
class PostProcessor : public IPostProcessor
{
public:
	/*!
		\brief Method invoked when a class containing the declared attribute is created
	*/
	virtual void PostConstruct(std::shared_ptr<TClass<T>> clazz, T* instance) const {}

	/*!
		\brief Method invoked when a class containing the declared attribute is created
	*/
	virtual void PostConstruct(std::shared_ptr<TClass<T>> clazz, const IMethod* method, T* instance) const {}

// IPostProcessor
public:
	virtual void ProcessInstance(const IClass* clazz, void* ptr) const;
	virtual void ProcessInstance(const IClass* clazz, const IMethod* method, void* ptr) const;
};

template <class T>
class ClassInterceptor : public IClassInterceptor
{
public:
	/*!
		\brief Method invoked 

		This is called only once for each class
	*/
	virtual void InterceptClass(std::shared_ptr<TClass<T>> clazz) const {}

// IClassInterceptor
public:
	virtual void ProcessClass(const IClass* clazz) const;
};

template<typename M>
class TRawMethod : public IRawMethod
{
public:
	TRawMethod(M method)
	: mMethod(method) {
	}

private:
	M mMethod;
};

typedef string cntr_id;

struct cntr {
	template<typename P1>
	static cntr_id typeof() {
		return string(typeid(P1).name());
	}

	template<typename P1, typename P2>
	static cntr_id typeof() {
		return string(typeid(P1).name()) + string(typeid(P2).name());
	}

	template<typename P1, typename P2, typename P3>
	static cntr_id typeof() {
		return string(typeid(P1).name()) + string(typeid(P2).name()) + string(typeid(P3).name());
	}

	template<typename P1, typename P2, typename P3, typename P4>
	static cntr_id typeof() {
		return string(typeid(P1).name()) + string(typeid(P2).name()) + string(typeid(P3).name()) + string(typeid(P4).name());
	}

	template<class T>
	static IMutableConstructor* for_class() {
		return new TConstructor<T>();
	}

	template<typename M>
	static IRawMethod* for_method(M method) {
		return new TRawMethod<M>(method);
	}

	template<class T>
	static string class_name() {
		string name(typeid(T).name());
#ifdef _MSC_VER
		return name.substr(6);
#else
		return name;
#endif
	}
};

/*!

*/
class TClassLoader
{
public:
	TClassLoader(std::shared_ptr<const IClassLoader> classLoader) : mClassLoader(classLoader){
	}

	~TClassLoader() {
	}

	/*!
		\brief Locates the class descriptor using the template.

		This returns the class descriptor for the exact class if found.
	*/
	template<class T>
	std::shared_ptr<TClass<T>> FindClass() const {
		auto ptr = mClassLoader->FindClass(cntr::class_name<T>());
		if (ptr == nullptr)
			return std::shared_ptr<TClass<T>>();
		return std::shared_ptr<TClass<T>>(new TClass<T>(ptr));
	}
	
	/*!
		\brief Locates the class descriptor
	*/
	template<class T>
	std::shared_ptr<TClass<T>> FindClass(const string& name) const {
		auto ptr = mClassLoader->FindClass(name);
		if (ptr == nullptr)
			return std::shared_ptr<TClass<T>>();
		return std::shared_ptr<TClass<T>>(new TClass<T>(ptr));
	}

	/*!
		\brief Find all classes extending the specified class
	*/
	template<class T>
	vector<std::shared_ptr<TClass<T>>> FindExtendedClasses() const {
		vector<std::shared_ptr<TClass<T>>> v;
		auto result = mClassLoader->FindExtendedClasses(cntr::class_name<T>());
		for (auto clazz : result) {
			v.push_back(std::shared_ptr<TClass<T>>(clazz));
		}
		return v;
	}

	template<class T>
	vector<std::shared_ptr<TClass<T>>> GetClasses() const {
		vector<std::shared_ptr<TClass<T>>> v;
		auto result = mClassLoader->GetClasses();
		for (auto clazz : result) {
			v.push_back(std::shared_ptr<TClass<T>>(clazz));
		}
		return v;
	}

	/*!
		\brief Performs post processing on the supplied instance
	*/
	template<class T>
	void PostConstruct(T* instance, PostProcessor<T>* processor) const {
		mClassLoader->PostConstruct(instance, processor);
	}

private:
	std::shared_ptr<const IClassLoader> mClassLoader;
};

/*!
\brief Retrieves the class loader for the entire application
*/
extern std::shared_ptr<TClassLoader> GetClassLoader();

///////////////////////////////////////

template<class T>
TClass<T>::TClass()
: mClass(nullptr) {
}

template<class T>
TClass<T>::TClass(const IClass* clazz)
: mClass(clazz) {
}

template<class T>
TClass<T>::TClass(const TClass<T>& rhs)
: mClass(rhs.mClass) {
}

template<class T>
TClass<T>::~TClass() {
}

template<class T>
TClass<T>& TClass<T>::operator=(const TClass<T>& rhs) {
	mClass = rhs.mClass;
	return *this;
}

template<class T>
T* TClass<T>::NewInstance() const {
	void* mem = mClass->NewInstance();
	return reinterpret_cast<T*>(mem);
}

template<class T>
const TConstructor<T>* TClass<T>::GetConstructor() const
{
	return dynamic_cast<const TConstructor<T>*>(mClass->GetConstructor());
}

template<class T>
const string& TClass<T>::GetName() const {
	return mClass->GetName();
}

template<class T>
vector<const IMethod*> TClass<T>::GetMethods() const {
	return mClass->GetMethods();
}

template<class T>
const IMethod* TClass<T>::GetMethod(const string& name, const string& signature) const {
	return mClass->GetMethod(name, signature);
}

template<class T>
const IMethod* TClass<T>::GetMethod(const IMethod* method) const {
	return mClass->GetMethod(method);
}

template<class T>
vector<const IAttribute*> TClass<T>::GetAttributes() const {
	return mClass->GetAttributes();
}

template<class T>
const IAttribute* TClass<T>::FindAttribute(const string& name) const {
	return mClass->FindAttribute(name);
}

template<class T>
const IAttribute* TClass<T>::FindAttribute(const string& name, bool includeInherited) const {
	return mClass->FindAttribute(name, includeInherited);
}

template<class T>
void ClassInterceptor<T>::ProcessClass(const IClass* clazz) const {

	shared_ptr<TClass<T>> ct(new TClass<T>(clazz));
	this->InterceptClass(ct);
}

template <class T>
void PostProcessor<T>::ProcessInstance(const IClass* clazz, void* ptr) const
{
	shared_ptr<TClass<T>> ct(new TClass<T>(clazz));
	T* instance = reinterpret_cast<T*>(ptr);

	this->PostConstruct(ct, instance);
}

template <class T>
void PostProcessor<T>::ProcessInstance(const IClass* clazz, const IMethod* method, void* ptr) const
{
	shared_ptr<TClass<T>> ct(new TClass<T>(clazz));
	T* instance = reinterpret_cast<T*>(ptr);

	this->PostConstruct(ct, method, instance);
}


#endif
