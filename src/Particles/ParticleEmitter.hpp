#pragma once

#include "../Rendering/BufferObject.hpp"
#include "../Rendering/Entity.hpp"
#include "../Rendering/Shader.hpp"
#include <glad/glad.h>
#include <vector>

namespace Engine
{
struct ParticleOptions
{
	float velocity[3];	   // vx, vy, vz // random range
	float acceleration[3]; // ax, ay, az // random range
	float lifetime;		   // in seconds // random range
	float size;			   // particle size // lerp
	float color[3];		   // r, g, b /// lerp
};

class ParticleEmitter : public Entity
{
public:
	ParticleEmitter(ParticleOptions from, ParticleOptions to, float spawnRate, Shader *shader);
	~ParticleEmitter();

	void update(float delta) override;
	void draw() override;

	void refresh();
	void changed()
	{
		_changed = true;
	}

private:
	void addParticle();
	void updateParticles(float delta);

	ParticleOptions _from;
	ParticleOptions _to;

	float _time;
	float _spawnRate;
	bool _changed;

	std::vector<ParticleOptions> _options;

	std::vector<float> _positions;
	std::vector<float> _sizes;
	std::vector<float> _colors;

	std::vector<float> _lifetimes;

	GLuint _vao;
	BufferObject _positionsVBO, _sizesVBO, _colorsVBO;
};
} // namespace Engine
