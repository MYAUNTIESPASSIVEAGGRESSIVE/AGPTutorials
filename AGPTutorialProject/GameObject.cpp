#include "GameObject.h"

GameObject::GameObject(std::string objectName, Mesh* objectMesh, Material* material)
	: name(objectName), mesh(objectMesh), material(material)
{

}