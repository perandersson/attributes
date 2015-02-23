#include <iostream>
#include "MovePlayerComponent.h"
#include "../attributes/attributes.h"

int main() 
{
	auto classLoader = GetClassLoader();

	// 
	auto metaData = classLoader->FindClass<MovePlayerComponent>()->FindAttribute("SERIALIZABLE")->Exists();
	cout << "SERIALIZABLE = " << metaData << endl;

	// Create an inherited class using the name of the class
	auto baseComponent = classLoader->FindClass<Component>("MovePlayerComponent")->NewInstance();
	cout << "age = " << baseComponent->GetAge() << endl;
	cout << "name = " << dynamic_cast<MovePlayerComponent*>(baseComponent)->GetName() << endl;
	cout << "blabla = " << dynamic_cast<MovePlayerComponent*>(baseComponent)->GetBlabla() << endl;

	// Create MovePlayerComponent using the reflection library
	// This is required if we want to execute the post constructor
	auto component = classLoader->FindClass<MovePlayerComponent>()->NewInstance();

	auto component2 = classLoader->FindClass<Component>()->NewInstance();

	cin.get();
	

	return 0;
}
