#pragma once

namespace Engine
{
class Object
{
public:
	Object() = default;
	virtual ~Object() = default;

	virtual void update(float /*delta*/) = 0;
	virtual void draw() = 0;
};
} // namespace Engine
