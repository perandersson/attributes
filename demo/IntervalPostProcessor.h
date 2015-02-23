#ifndef _INTERVAL_POST_PROCESSOR_H_
#define _INTERVAL_POST_PROCESSOR_H_

#include "../attributes/attributes.h"
#include "Component.h"
#include <iostream>

INTERCEPTS(INTERVAL)
class IntervalPostProcessor : public PostProcessor<Component>
{
public:
	virtual void PostConstruct(std::shared_ptr<TClass<Component>> clazz, const IMethod* method, Component* instance) const {
		auto value = method->FindAttribute("INTERVAL")->ValueAsFloat("value", 0.0f);

		cout << "Post constructing an instance of " << clazz->GetName() << " and method " << method->GetName() << endl;
		cout << "INTERVAL.value = " << value << endl;
	}

	virtual void PostConstruct(std::shared_ptr<TClass<Component>> clazz, Component* instance) const {
		cout << "IntervalPostProcessor Class is being instantiated: " << clazz->GetName() << endl;
	}
};


#endif
