#ifndef _EDITABLE_POST_PROCESSOR_H_
#define _EDITABLE_POST_PROCESSOR_H_

#include "../attributes/attributes.h"
#include "Component.h"
#include <iostream>

INTERCEPTS(EDITABLE, SERIALIZABLE)
class EditablePostProcessor : public PostProcessor<Component>, public ClassInterceptor<Component>
{
public:
	virtual void InterceptClass(std::shared_ptr<TClass<Component>> clazz) const {
		auto description = clazz->FindAttribute("EDITABLE")->ValueAsString("description", "");
		cout << "Preparing class " << clazz->GetName() << endl;
		cout << "EDITABLE.description = " << description << endl;

	}

	virtual void PostConstruct(std::shared_ptr<TClass<Component>> clazz, Component* instance) const {
		cout << "EditablePostProcessor Class is being instantiated: " << clazz->GetName() << endl;
	}
};

#endif
