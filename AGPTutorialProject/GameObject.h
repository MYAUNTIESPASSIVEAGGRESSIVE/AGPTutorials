#pragma once
#include <string>
#include "Transform.h"

class Mesh;

class GameObject
{
public:

	Transform transform;

	Mesh* mesh;

private:
	std::string name = "GameObject";
	
public:
	std::string GetName() { return name; }

	GameObject(std::string objectName, Mesh* objectMesh);
};

