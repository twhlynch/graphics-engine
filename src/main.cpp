#include "Clock.hpp"
#include "Loaders/OBJLoader.hpp"
#include "Math/Angles.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Random.hpp"
#include "Math/Ray.hpp"
#include "Math/Vector3.hpp"
#include "Particles/ParticleEmitter.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Window.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Controller.hpp"
#include "UI/BitmapFont.hpp"
#include "UI/Text.hpp"
#include <GLFW/glfw3.h>

int main()
{
	Engine::Window *window = new Engine::Window(800, 600, "Engine Demo Scene");
	Engine::Renderer *renderer = new Engine::Renderer();

	Engine::Camera *camera = new Engine::Camera(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	camera->setPosition(Engine::Vector3(0, 0, 12));

	renderer->setCamera(camera);

	Engine::Controller *controller = new Engine::Controller(camera, window);

	Engine::Shader *shader = new Engine::Shader("assets/shaders/fog_vert.glsl", "assets/shaders/fog_frag.glsl");
	Engine::Shader *texturedShader = new Engine::Shader("assets/shaders/texture_vert.glsl", "assets/shaders/texture_frag.glsl");

	std::vector<Engine::Shader *> shaders({texturedShader, shader, shader});

	std::vector<Engine::Entity *> entities;

	std::string image("fallback.png");
	Engine::Texture *texture = new Engine::Texture(image);

	std::vector<Engine::Material *> materialList = {
		(new Engine::Material()),
		(new Engine::Material()),
		(new Engine::Material()),
	};
	materialList[1]->setWireframe(true);
	materialList[2]->setRenderType(Engine::Material::RenderType::Points);

	Engine::Mesh *dragon = Engine::OBJLoader::load("assets/models/dragon.obj");
	for (size_t i = 0; i < materialList.size(); i++)
	{
		materialList[i]->setTexture(texture);
		Engine::Entity *entity = new Engine::Entity(dragon, materialList[i], shaders[i]);
		Engine::Vector3 vec(-4.0f + (4 * static_cast<float>(i)), 2.5f, 0.0f);
		entity->setPosition(vec);
		renderer->addEntity(entity, (i == 0));
		entities.push_back(entity);
	}

	std::vector<Engine::Mesh *> meshList = {
		Engine::Mesh::WithTriangle(),
		Engine::Mesh::WithQuad(),
		Engine::Mesh::WithCircle(),
		Engine::Mesh::WithPyramid(),
		Engine::Mesh::WithCube(),
		Engine::Mesh::WithCylinder(),
	};
	for (size_t i = 0; i < meshList.size(); i++)
	{
		for (size_t j = 0; j < materialList.size(); j++)
		{
			Engine::Entity *entity = new Engine::Entity(meshList[i], materialList[j], shaders[j]);
			Engine::Vector3 vec(-5.0f + (2.0f * static_cast<float>(i)), -1.5f * static_cast<float>(j), 0.0f);
			entity->setPosition(vec);
			renderer->addEntity(entity);
			entities.push_back(entity);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		Engine::Entity *entity = new Engine::Entity(meshList[4], materialList[0], shaders[0]);
		Engine::Vector3 position(Engine::randRange(-1.0f, 1.0f) + 9, Engine::randRange(-1.0f, 1.0f) + 2, Engine::randRange(-1.0f, 1.0f));
		Engine::Vector3 scale(Engine::randRange(0.2f, 0.5f), Engine::randRange(0.2f, 0.5f), Engine::randRange(0.2f, 0.5f));
		Engine::Quaternion rotation(Engine::randRange(-1.0f, 1.0f), Engine::randRange(-1.0f, 1.0f), Engine::randRange(-1.0f, 1.0f), Engine::randRange(-1.0f, 1.0f));
		rotation.normalize();
		entity->setPosition(position);
		entity->setScale(scale);
		entity->setRotation(rotation);
		renderer->addEntity(entity, true);
		entities.push_back(entity);
	}

	Engine::Ray *ray = new Engine::Ray(camera->getPosition(), camera->getForward());
	Engine::Entity *rayVis = ray->getEntityToVisualize();
	ray->setObjects(&entities);
	renderer->addEntity(rayVis);
	Engine::Mesh *pointMesh = new Engine::Mesh({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f});

	Engine::Shader *particleShader = new Engine::Shader("assets/shaders/particle_vert.glsl", "assets/shaders/particle_frag.glsl");
	Engine::ParticleEmitter *particleEmitter = new Engine::ParticleEmitter(
		Engine::ParticleOptions {
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f},
			2.0f,
			1.0f,
			{0.6f, 0.2f, 0.4f},
		},
		Engine::ParticleOptions {
			{0.0f, 0.0f, 0.8f},
			{0.0f, 0.0f, 0.0f},
			3.0f,
			3.0f,
			{0.9f, 0.1f, 0.1f},
		},
		0.0005f,
		particleShader);
	particleEmitter->setPosition(Engine::Vector3(10, -3, 0));
	particleEmitter->setRotation(Engine::Quaternion::WithAxisAngle(Engine::Vector3(1, 0, 0), Engine::radians(90)));
	particleEmitter->setScale(0.2);
	renderer->addEntity(particleEmitter);
	entities.push_back(particleEmitter);

	Engine::BitmapFont *font = new Engine::BitmapFont("assets/bitmap.txt");
	Engine::Text *text = new Engine::Text(font);
	text->setText("0123456789\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n!@#$%^&*-+=_(){}[]\\/|<>`,.'\";:");
	Engine::Material *textMaterial = new Engine::Material();
	textMaterial->setCullBackface(false);
	Engine::Entity *textEntity = new Engine::Entity(text, textMaterial, shader);
	textEntity->setPosition(Engine::Vector3(2, -5, 0));
	renderer->addEntity(textEntity);
	entities.push_back(textEntity);

	Engine::Clock *clock = new Engine::Clock();
	while (!window->shouldClose())
	{
		float delta = clock->getDelta();

		controller->update(delta);

		renderer->update(delta);

		for (Engine::Entity *entity : entities)
		{
			entity->setRotation((entity->getRotation() * Engine::Quaternion::WithAxisAngle(Engine::Vector3(0, 1, 0), delta)).normalized());
		}

		particleEmitter->setRotation((particleEmitter->getRotation() * Engine::Quaternion::WithAxisAngle(Engine::Vector3(0, 1, 0), delta * 5)).normalized());

		if (Engine::Window::isPressing(GLFW_KEY_SPACE))
		{
			ray->set(camera->getPosition(), camera->getForward());
			*rayVis = *ray->getEntityToVisualize();

			std::vector<Engine::Intersection> intersections = ray->cast();
			for (const Engine::Intersection &intersection : intersections)
			{
				Engine::Entity *entity = new Engine::Entity(pointMesh, materialList[2], rayVis->getShader());
				entity->setScale(0.1f);
				entity->setPosition(intersection.point);
				renderer->addEntity(entity, true);
			}
		}

		renderer->draw();

		window->update();
	}

	for (Engine::Entity *entity : entities)
	{
		delete entity;
	}
	for (Engine::Material *material : materialList)
	{
		delete material;
	}
	for (Engine::Mesh *mesh : meshList)
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
