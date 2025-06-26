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
	Window *window = new Window(800, 600, "Renderer");
	Renderer *renderer = new Renderer();

	Camera *camera = new Camera(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	camera->setPosition(Vector3(0, 0, 12));

	renderer->setCamera(camera);

	Controller *controller = new Controller(camera, window);

	Shader *shader = new Shader("assets/shaders/fog_vert.glsl", "assets/shaders/fog_frag.glsl");
	Shader *texturedShader = new Shader("assets/shaders/texture_vert.glsl", "assets/shaders/texture_frag.glsl");

	std::vector<Shader *> shaders({texturedShader, shader, shader});

	std::vector<Entity *> entities;

	std::string image("fallback.png");
	Texture *texture = new Texture(image);

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
		materialList[i]->setTexture(texture);
		Entity *entity = new Entity(dragon, materialList[i], shaders[i]);
		Vector3 vec(-4.0f + (4 * static_cast<float>(i)), 2.5f, 0.0f);
		entity->setPosition(vec);
		renderer->addEntity(entity, (i == 0));
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
			Entity *entity = new Entity(meshList[i], materialList[j], shaders[j]);
			Vector3 vec(-5.0f + (2.0f * static_cast<float>(i)), -1.5f * static_cast<float>(j), 0.0f);
			entity->setPosition(vec);
			renderer->addEntity(entity);
			entities.push_back(entity);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		Entity *entity = new Entity(meshList[4], materialList[0], shaders[0]);
		Vector3 position(randRange(-1.0f, 1.0f) + 9, randRange(-1.0f, 1.0f) + 2, randRange(-1.0f, 1.0f));
		Vector3 scale(randRange(0.2f, 0.5f), randRange(0.2f, 0.5f), randRange(0.2f, 0.5f));
		Quaternion rotation(randRange(-1.0f, 1.0f), randRange(-1.0f, 1.0f), randRange(-1.0f, 1.0f), randRange(-1.0f, 1.0f));
		rotation.normalize();
		entity->setPosition(position);
		entity->setScale(scale);
		entity->setRotation(rotation);
		renderer->addEntity(entity, true);
		entities.push_back(entity);
	}

	Ray *ray = new Ray(camera->getPosition(), camera->getForward());
	Entity *rayVis = ray->getEntityToVisualize();
	ray->setObjects(&entities);
	renderer->addEntity(rayVis);
	Mesh *pointMesh = new Mesh({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f});

	Shader *particleShader = new Shader("assets/shaders/particle_vert.glsl", "assets/shaders/particle_frag.glsl");
	ParticleEmitter *particleEmitter = new ParticleEmitter(
		ParticleOptions {
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f},
			2.0f,
			1.0f,
			{0.6f, 0.2f, 0.4f},
		},
		ParticleOptions {
			{0.0f, 0.0f, 0.8f},
			{0.0f, 0.0f, 0.0f},
			3.0f,
			3.0f,
			{0.9f, 0.1f, 0.1f},
		},
		0.0005f,
		particleShader);
	particleEmitter->setPosition(Vector3(10, -3, 0));
	particleEmitter->setRotation(Quaternion::WithAxisAngle(Vector3(1, 0, 0), radians(90)));
	particleEmitter->setScale(0.2);
	renderer->addEntity(particleEmitter);
	entities.push_back(particleEmitter);

	BitmapFont *font = new BitmapFont("assets/bitmap.txt");
	Text *text = new Text(font);
	text->setText("0123456789\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n!@#$%^&*-+=_(){}[]\\/|<>`,.'\";:");
	Material *textMaterial = new Material();
	textMaterial->setCullBackface(false);
	Entity *textEntity = new Entity(text, textMaterial, shader);
	textEntity->setPosition(Vector3(2, -5, 0));
	renderer->addEntity(textEntity);
	entities.push_back(textEntity);

	Clock *clock = new Clock();
	while (!window->shouldClose())
	{
		float delta = clock->getDelta();

		shader->refresh();

		controller->update(delta);

		renderer->update(delta);

		for (Entity *entity : entities)
		{
			entity->setRotation((entity->getRotation() * Quaternion::WithAxisAngle(Vector3(0, 1, 0), delta)).normalized());
		}

		particleEmitter->setRotation((particleEmitter->getRotation() * Quaternion::WithAxisAngle(Vector3(0, 1, 0), delta * 5)).normalized());

		if (Window::isPressing(GLFW_KEY_SPACE))
		{
			ray->set(camera->getPosition(), camera->getForward());
			*rayVis = *ray->getEntityToVisualize();

			std::vector<Intersection> intersections = ray->cast();
			for (const Intersection &intersection : intersections)
			{
				Entity *entity = new Entity(pointMesh, materialList[2], rayVis->getShader());
				entity->setScale(0.1f);
				entity->setPosition(intersection.point);
				renderer->addEntity(entity, true);
			}
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
