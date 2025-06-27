#include "Engine.hpp"

int main()
{
	Engine::Window *window = new Engine::Window(800, 600, "Engine Demo Scene");
	Engine::Renderer *renderer = new Engine::Renderer();
	Engine::Scene *scene = new Engine::Scene();

	Engine::Camera *camera = new Engine::Camera(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	camera->setPosition(Engine::Vector3(0, 0, 12));
	renderer->setCamera(camera);

	Engine::Controller *controller = new Engine::Controller(camera, window);

	Engine::Shader *shader = new Engine::Shader("assets/shaders/fog_vert.glsl", "assets/shaders/fog_frag.glsl");
	Engine::Shader *texturedShader = new Engine::Shader("assets/shaders/texture_vert.glsl", "assets/shaders/texture_frag.glsl");
	Engine::Shader *noFogShader = new Engine::Shader("assets/shaders/basic_vert.glsl", "assets/shaders/basic_frag.glsl");
	Engine::Shader *particleShader = new Engine::Shader("assets/shaders/particle_vert.glsl", "assets/shaders/particle_frag.glsl");

	std::string image("fallback.png");
	Engine::Texture *texture = new Engine::Texture(image);

	Engine::Material *material = new Engine::Material();
	Engine::Material *wireframeMaterial = new Engine::Material();
	Engine::Material *transparentMaterial = new Engine::Material();
	Engine::Material *pointsMaterial = new Engine::Material();
	Engine::Material *textMaterial = new Engine::Material();
	
	textMaterial->setCullBackface(false);
	wireframeMaterial->setWireframe(true);
	transparentMaterial->setTransparent(true);
	pointsMaterial->setRenderType(Engine::Material::RenderType::Points);

	Engine::Mesh *dragonMesh = Engine::OBJLoader::load("assets/models/dragon.obj");
	Engine::Mesh *triangleMesh = Engine::Mesh::WithTriangle();
	Engine::Mesh *quadMesh = Engine::Mesh::WithQuad();
	Engine::Mesh *circleMesh = Engine::Mesh::WithCircle();
	Engine::Mesh *pyramidMesh = Engine::Mesh::WithPyramid();
	Engine::Mesh *cubeMesh = Engine::Mesh::WithCube();
	Engine::Mesh *CylinderMesh = Engine::Mesh::WithCylinder();
	Engine::Mesh *pointMesh = new Engine::Mesh({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f});

	Engine::Ray *ray = new Engine::Ray(camera->getPosition(), camera->getForward());
	ray->setObjects(scene->getObjects());
	Engine::Entity *rayVis = ray->getEntityToVisualize();
	scene->addObject(rayVis);

	std::vector<Engine::Mesh *> meshList {
		triangleMesh,
		quadMesh,
		circleMesh,
		pyramidMesh,
		cubeMesh,
		CylinderMesh,
	};
	std::vector<Engine::Material *> materialList {
		transparentMaterial,
		wireframeMaterial,
		pointsMaterial,
		material,
	};
	std::vector<Engine::Shader *> shaderList {
		texturedShader,
		noFogShader,
		shader,
		particleShader,
	};

	std::vector<Engine::Entity *> rotatingEntities;

	// dragons
	for (size_t i = 0; i < 3; i++)
	{
		materialList[i]->setTexture(texture);
		Engine::Entity *entity = new Engine::Entity(dragonMesh, materialList[i], shaderList[i]);
		Engine::Vector3 vec(-4.0f + (4 * static_cast<float>(i)), 2.5f, 0.0f);
		entity->setPosition(vec);
		scene->addObject(entity);
		rotatingEntities.push_back(entity);
	}

	// grid
	for (size_t i = 0; i < meshList.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Engine::Entity *entity = new Engine::Entity(meshList[i], materialList[j], shaderList[j]);
			Engine::Vector3 vec(-5.0f + (2.0f * static_cast<float>(i)), -1.5f * static_cast<float>(j), 0.0f);
			entity->setPosition(vec);
			scene->addObject(entity);
			rotatingEntities.push_back(entity);
		}
	}

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
	scene->addObject(particleEmitter);
	rotatingEntities.push_back(particleEmitter);

	Engine::BitmapFont *font = new Engine::BitmapFont("assets/bitmap.txt");
	Engine::Text *text = new Engine::Text(font);
	text->setText("0123456789\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n!@#$%^&*-+=_(){}[]\\/|<>`,.'\";:");
	Engine::Entity *textEntity = new Engine::Entity(text, textMaterial, shader);
	textEntity->setPosition(Engine::Vector3(2, -5, 0));
	scene->addObject(textEntity);

	scene->addObject(controller);

	Engine::Clock *clock = new Engine::Clock();
	while (!window->shouldClose())
	{
		float delta = clock->getDelta();

		scene->update(delta);

		for (Engine::Entity *entity : rotatingEntities)
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
				scene->addObject(entity);
			}
		}

		renderer->draw(scene);

		window->update();
	}

	for (Engine::Entity *entity : rotatingEntities)
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
	for (Engine::Shader *shader : shaderList)
	{
		delete shader;
	}
	delete dragonMesh;
	delete controller;
	delete camera;
	delete renderer;
	delete window;
	delete clock;
	delete rayVis;
	delete font;
	delete text;
	delete textEntity;
	delete texture;

	return EXIT_SUCCESS;
}
