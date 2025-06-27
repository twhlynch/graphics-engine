#pragma once

#include "../Rendering/Entity.hpp"
#include "../Scene/Camera.hpp"
#include <vector>

namespace Engine
{
class Renderer
{
public:
	Renderer();
	~Renderer();

	void setCamera(Camera *camera);
	void addEntity(Entity *entity, bool alpha = false);
	void draw();
	void update(float delta);

private:
	std::vector<Entity *> _entities;
	std::vector<Entity *> _transparentEntities;
	Camera *_camera;
};
} // namespace Engine
