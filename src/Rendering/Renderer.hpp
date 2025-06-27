#pragma once

#include "../Scene/Camera.hpp"
#include "../Scene/Scene.hpp"

namespace Engine
{
class Renderer
{
public:
	Renderer();
	~Renderer();

	void setCamera(Camera *camera);
	void draw(Scene *scene);

private:
	Camera *_camera;
};
} // namespace Engine
