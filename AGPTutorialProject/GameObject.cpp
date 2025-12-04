#include "GameObject.h"

GameObject::GameObject(std::string objectName, Mesh* objectMesh, Texture* texture) 
	: name(objectName), mesh(objectMesh), texture(texture)
{

}