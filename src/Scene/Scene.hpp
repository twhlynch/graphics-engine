#pragma once

#include "../Scene/Object.hpp"
#include <vector>

namespace Engine
{
class Scene
{
public:
	Scene();
	~Scene();

	void addObject(Object *object);
	std::vector<Object *> *getObjects()
	{
		return &_objects;
	}

	void update(float delta);

private:
	std::vector<Object *> _objects;
};
} // namespace Engine
