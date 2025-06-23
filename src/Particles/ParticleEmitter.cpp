#include "../Particles/ParticleEmitter.hpp"
#include "../Logging.hpp"
#include "../Math/Interpolation.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Random.hpp"

ParticleEmitter::ParticleEmitter(ParticleOptions from, ParticleOptions to, float spawnRate, Shader *shader) :
	Entity(nullptr, nullptr, shader), _from(from), _to(to), _spawnRate(spawnRate), _changed(true)
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	_positionsVBO.setData(&_positions);
	_sizesVBO.setData(&_sizes);
	_colorsVBO.setData(&_colors);

	_positionsVBO.bind(0, 3);
	_sizesVBO.bind(1, 1);
	_colorsVBO.bind(2, 3);
}

ParticleEmitter::~ParticleEmitter()
{
	glDeleteVertexArrays(1, &_VAO);
}

void ParticleEmitter::refresh()
{
	if (_changed)
	{
		_changed = false;

		_positionsVBO.refresh();
		_sizesVBO.refresh();
		_colorsVBO.refresh();
	}
}

void ParticleEmitter::update(float delta)
{
	updateParticles(delta);

	_time += delta;
	while (_time >= _spawnRate)
	{
		_time -= _spawnRate;
		addParticle();
	}

	refresh();
}

void ParticleEmitter::draw()
{
	_shader->use();

	GLint modelLoc = glGetUniformLocation(_shader->getProgramID(), "model");

	if (modelLoc != -1)
	{
		Matrix4 model = Matrix4::WithTranslation(0) * Matrix4::WithScale(1) * Matrix4::WithRotation(Quaternion());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
	}
	else
	{
		WARN("'model' uniform not found in shader.");
	}

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(_lifetimes.size()));
}

void ParticleEmitter::updateParticles(float delta)
{
	for (size_t i = 0; i < _options.size(); i++)
	{
		_lifetimes[i] -= delta;

		if (_lifetimes[i] <= 0.0f)
		{
			_positions.erase(_positions.begin() + i * 3, _positions.begin() + (i + 1) * 3);
			_sizes.erase(_sizes.begin() + i);
			_colors.erase(_colors.begin() + i * 3, _colors.begin() + (i + 1) * 3);
			_lifetimes.erase(_lifetimes.begin() + i);
			_options.erase(_options.begin() + i);

			--i;
		}
	}

	for (size_t i = 0; i < _options.size(); i++)
	{
		float lifetime = _options[i].lifetime;
		float alpha = _lifetimes[i] / lifetime;

		_colors[i * 3] = lerp(_from.color[0], _to.color[0], alpha);
		_colors[i * 3 + 1] = lerp(_from.color[1], _to.color[1], alpha);
		_colors[i * 3 + 2] = lerp(_from.color[2], _to.color[2], alpha);
		_sizes[i] = lerp(_from.size, _to.size, alpha);

		_positions[i * 3] += _options[i].velocity[0] * delta;
		_positions[i * 3 + 1] += _options[i].velocity[1] * delta;
		_positions[i * 3 + 2] += _options[i].velocity[2] * delta;

		_options[i].velocity[0] += _options[i].acceleration[0] * delta;
		_options[i].velocity[1] += _options[i].acceleration[1] * delta;
		_options[i].velocity[2] += _options[i].acceleration[2] * delta;
	}

	_changed = true;
}

void ParticleEmitter::addParticle()
{
	Matrix4 modelMatrix = computeModelMatrix();
	Vector3 position(Random::range(-1.0f, 1.0f),
					 Random::range(-1.0f, 1.0f),
					 Random::range(-1.0f, 1.0f));
	Vector3 velocity(Random::range(_from.velocity[0], _to.velocity[0]),
					 Random::range(_from.velocity[1], _to.velocity[1]),
					 Random::range(_from.velocity[2], _to.velocity[2]));
	Vector3 acceleration(Random::range(_from.acceleration[0], _to.acceleration[0]),
						 Random::range(_from.acceleration[1], _to.acceleration[1]),
						 Random::range(_from.acceleration[2], _to.acceleration[2]));

	modelMatrix.apply(position);
	velocity = _rotation.apply(velocity);
	acceleration = _rotation.apply(acceleration);

	ParticleOptions options = {
		{
			velocity.x,
			velocity.y,
			velocity.z,
		},
		{
			acceleration.x,
			acceleration.y,
			acceleration.z,
		},
		Random::range(_from.lifetime, _to.lifetime),
		_from.size,
		{
			_from.color[0],
			_from.color[1],
			_from.color[2],
		},
	};
	_options.push_back(options);

	_positions.push_back(position.x);
	_positions.push_back(position.y);
	_positions.push_back(position.z);

	_sizes.push_back(options.size);

	_colors.push_back(options.color[0]);
	_colors.push_back(options.color[1]);
	_colors.push_back(options.color[2]);

	_lifetimes.push_back(options.lifetime);

	_changed = true;
}
