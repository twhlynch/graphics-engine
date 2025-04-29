#include "Renderer.hpp"

#include "../Logging.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() :
	_camera(nullptr)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer::~Renderer() {}

void Renderer::setCamera(Camera *camera)
{
	_camera = camera;
}

void Renderer::addEntity(Entity *entity)
{
	_entities.push_back(entity);
}

void Renderer::draw()
{
	if (!_camera)
	{
		WARN("No camera set!");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Entity *entity : _entities)
	{
		Shader *shader = entity->getShader();
		if (!shader)
		{
			WARN("Entity without shader!");
			continue;
		}

		shader->use();

		GLint viewLoc = glGetUniformLocation(shader->getProgramID(), "view");
		GLint projLoc = glGetUniformLocation(shader->getProgramID(), "projection");

		glm::mat4 view = _camera->getViewMatrix();
		glm::mat4 projection = _camera->getProjectionMatrix();

		if (viewLoc != -1)
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		else
			WARN("'view' uniform not found in shader!");

		if (projLoc != -1)
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		else
			WARN("'projection' uniform not found in shader!");

		entity->draw();
	}
}
