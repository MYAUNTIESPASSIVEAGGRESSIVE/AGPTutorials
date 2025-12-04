#pragma once
#include <string>
#include "Transform.h"

class Mesh;
class Texture;

class GameObject
{
public:

	Transform transform;

	Mesh* mesh;

	Texture* texture;

private:
	std::string name = "GameObject";
	
public:
	std::string GetName() { return name; }

	GameObject(std::string objectName, Mesh* objectMesh, Texture* texture);
};

