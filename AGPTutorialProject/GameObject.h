#pragma once
#include <string>
#include "Transform.h"

class Mesh;
class Material;
//class Texture;

class GameObject
{
public:

	Transform transform;

	Mesh* mesh;

	Material* material;
	//Texture* texture;

private:
	std::string name = "GameObject";
	
public:
	std::string GetName() { return name; }

	GameObject(std::string objectName, Mesh* objectMesh, Material* material);
};

