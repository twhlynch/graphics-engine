#include "Scene.hpp"

namespace Engine
{
Scene::Scene()
{
}
Scene::~Scene()
{
}

void Scene::addObject(Object *object)
{
	_objects.push_back(object);
}

void Scene::update(float delta)
{
	for (Object *object : _objects)
	{
		object->update(delta);
	}
}
} // namespace Engine
