#pragma once

#include "../Rendering/Entity.hpp"
#include "../Scene/Camera.hpp"

#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void setCamera(Camera *camera);
	void addEntity(Entity *entity);
	void draw();

private:
	std::vector<Entity *> _entities;
	Camera *_camera;
};
