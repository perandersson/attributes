#ifndef _ACTION_POST_PROCESSOR_H_
#define _ACTION_POST_PROCESSOR_H_

#include "../attributes/attributes.h"
#include "Component.h"
#include <iostream>

INTERCEPTS(ACTION)
class ActionPostProcessor : public PostProcessor<Component>
{
public:
	virtual void PostConstruct(std::shared_ptr<TClass<Component>> clazz, const IMethod* method, Component* instance) const {
		auto action = method->FindAttribute("ACTION")->ValueAsString("action", "");
		if (action.empty())
			return;

		//RegisterAction(instance, action);

		cout << "Post constructing an instance of " << clazz->GetName() << " and method " << method->GetName() << endl;
		cout << "ACTION.action = " << action << endl;
	}
};


#endif
