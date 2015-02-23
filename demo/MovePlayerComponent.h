#ifndef _MOVEPLAYERCOMPONENT_H_
#define _MOVEPLAYERCOMPONENT_H_

#include "Component.h"
#include <string>
using namespace std;

EDITABLE(description = "Component used to move a player", constructor = "")
class MovePlayerComponent : public Component
{
public:
	MovePlayerComponent() :name("John Doe"), blabla(100) {

	}

	~MovePlayerComponent() {

	}

	// Use the attribute declared in Component class
	//INTERVAL(value = 2.0f)
	virtual void Update() {

	}

	const string& GetName() const {
		return name;
	}

	int GetBlabla() const {
		return blabla;
	}

	ACTION(action = "MoveForward")
	void MoveForward(int key, float speed){

	}

private:
	string name;
	int blabla;
};

#endif
