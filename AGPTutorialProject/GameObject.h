#pragma once
#include <string>
#include "Transform.h"
class GameObject
{
public:

	Transform transform;

private:
	std::string name = "GameObject";
	
public:
	std::string GetName() { return name; }

	GameObject(std::string objectName);
};

