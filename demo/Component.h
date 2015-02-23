#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#define SERIALIZABLE(...)
#define EDITABLE(...)
#define INTERVAL(...)
#define ACTION(...)

SERIALIZABLE()
EDITABLE()
class Component
{
public:
	Component() : age(123) {

	}

	virtual ~Component() {

	}

	INTERVAL(value = 0.0032f)
	virtual void Update() {

	}

	int GetAge() const {
		return age;
	}

private:
	int age;
};


#endif
