#include "Clock.hpp"
#include "Loaders/OBJLoader.hpp"
#include "Logging.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Ray.hpp"
#include "Math/Vector3.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Window.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Controller.hpp"

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

int main()
{
	Window *window = new Window(800, 600, "Renderer");
	Renderer *renderer = new Renderer();

	Camera *camera = new Camera(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	camera->setPosition(Vector3(0, 0, 12));

	renderer->setCamera(camera);

	Controller *controller = new Controller(camera, window);

	Shader *shader = new Shader("assets/shaders/fog_vert.glsl", "assets/shaders/fog_frag.glsl");

	std::vector<Entity *> entities;

	std::vector<Material *> materialList = {
		(new Material()),
		(new Material()),
		(new Material()),
	};
	materialList[1]->setWireframe(true);
	materialList[2]->setRenderType(Material::RenderType::Points);

	Mesh *dragon = OBJLoader::load("assets/models/dragon.obj");
	for (size_t i = 0; i < materialList.size(); i++)
	{
		Entity *entity = new Entity(dragon, materialList[i], shader);
		Vector3 vec(-4.0f + 4 * (float)i, 2.5f, 0.0f);
		entity->setPosition(vec);
		renderer->addEntity(entity);
		entities.push_back(entity);
	}

	std::vector<Mesh *> meshList = {
		Mesh::WithTriangle(),
		Mesh::WithQuad(),
		Mesh::WithCircle(),
		Mesh::WithPyramid(),
		Mesh::WithCube(),
		Mesh::WithCylinder(),
	};
	for (size_t i = 0; i < meshList.size(); i++)
	{
		for (size_t j = 0; j < materialList.size(); j++)
		{
			Entity *entity = new Entity(meshList[i], materialList[j], shader);
			Vector3 vec(-5.0f + 2.0f * (float)i, -1.5f * (float)j, 0.0f);
			entity->setPosition(vec);
			renderer->addEntity(entity);
			entities.push_back(entity);
		}
	}

	Ray *ray = new Ray(Vector3(0.0f), Vector3(45.0f, 45.0f, 45.0f));
	Entity *rayVis = ray->getEntityToVisualize();
	renderer->addEntity(rayVis);

	Clock *clock = new Clock();
	while (!window->shouldClose())
	{
		float delta = clock->getDelta();
		float currentTime = clock->getTime();

		shader->refresh();

		controller->update(delta);

		for (Entity *entity : entities)
		{
			entity->setRotation(Quaternion::WithAxisAngle(Vector3(0, 1, 0), currentTime));
		}

		renderer->draw();

		window->update();
	}

	for (Entity *entity : entities)
	{
		delete entity;
	}
	for (Material *material : materialList)
	{
		delete material;
	}
	for (Mesh *mesh : meshList)
	{
		delete mesh;
	}
	delete dragon;
	delete shader;
	delete controller;
	delete camera;
	delete renderer;
	delete window;
	delete clock;
	delete rayVis;

	return EXIT_SUCCESS;
}
