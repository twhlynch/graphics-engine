#include "Renderer.hpp"
#include "../Logging.hpp"
#include <glad/glad.h>

namespace Engine
{
Renderer::Renderer() :
	_camera(nullptr)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer::~Renderer() {}

void Renderer::setCamera(Camera *camera)
{
	_camera = camera;
}

void Renderer::addEntity(Entity *entity, bool alpha)
{
	if (!alpha)
	{
		_entities.push_back(entity);
	}
	else
	{
		_transparentEntities.push_back(entity);
	}
}

void Renderer::update(float delta)
{
	for (Entity *entity : _entities)
	{
		entity->update(delta);
	}

	for (Entity *entity : _transparentEntities)
	{
		entity->update(delta);
	}
}

void Renderer::draw()
{
	if (!_camera)
	{
		WARN("No camera set!");
		return;
	}

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4 view = _camera->getViewMatrix();
	Matrix4 projection = _camera->getProjectionMatrix();

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

		if (viewLoc != -1)
		{
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
		}
		else
		{
			WARN("'view' uniform not found in shader!");
		}

		if (projLoc != -1)
		{
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());
		}
		else
		{
			WARN("'projection' uniform not found in shader!");
		}

		entity->draw();
	}

	glDepthMask(GL_FALSE);

	std::sort(_transparentEntities.begin(), _transparentEntities.end(),
			  [this](const Entity *a, const Entity *b) {
				  Vector3 camPos = this->_camera->getPosition();
				  return camPos.distance(a->getPosition()) > camPos.distance(b->getPosition());
			  });

	glEnable(GL_BLEND);

	for (Entity *entity : _transparentEntities)
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

		if (viewLoc != -1)
		{
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
		}
		else
		{
			WARN("'view' uniform not found in shader!");
		}

		if (projLoc != -1)
		{
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());
		}
		else
		{
			WARN("'projection' uniform not found in shader!");
		}

		entity->draw();
	}

	glDisable(GL_BLEND);
}
} // namespace Engine
