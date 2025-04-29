#pragma once

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <ostream>

class Vector3
{
public:
	Vector3()
	{
		x = y = z = 0.0f;
	}
	Vector3(const float n)
	{
		x = y = z = n;
	}
	Vector3(const float x, const float y, const float z) :
		x(x), y(y), z(z)
	{}
	Vector3(const Vector3 &other)
	{
		*this = other;
	}

	inline void operator=(const Vector3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	inline bool operator==(const Vector3 &other) const
	{
		return std::abs(x - other.x) < FLT_EPSILON &&
			   std::abs(y - other.y) < FLT_EPSILON &&
			   std::abs(z - other.z) < FLT_EPSILON;
	}
	inline bool operator!=(const Vector3 &other) const
	{
		return !(*this == other);
	}

	inline Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator+(const Vector3 &other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	inline Vector3 operator-(const Vector3 &other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	inline Vector3 operator*(const Vector3 &other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}
	inline Vector3 operator/(const Vector3 &other) const
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	inline Vector3 operator+(const float n) const
	{
		return Vector3(x + n, y + n, z + n);
	}
	inline Vector3 operator-(const float n) const
	{
		return Vector3(x - n, y - n, z - n);
	}
	inline Vector3 operator*(const float n) const
	{
		return Vector3(x * n, y * n, z * n);
	}
	inline Vector3 operator/(const float n) const
	{
		return Vector3(x / n, y / n, z / n);
	}

	inline Vector3 operator+=(const Vector3 &other) const
	{
		return *this + other;
	}
	inline Vector3 operator-=(const Vector3 &other) const
	{
		return *this - other;
	}
	inline Vector3 operator*=(const Vector3 &other) const
	{
		return *this * other;
	}
	inline Vector3 operator/=(const Vector3 &other) const
	{
		return *this / other;
	}

	inline Vector3 operator+=(const float n) const
	{
		return *this + n;
	}
	inline Vector3 operator-=(const float n) const
	{
		return *this - n;
	}
	inline Vector3 operator*=(const float n) const
	{
		return *this * n;
	}
	inline Vector3 operator/=(const float n) const
	{
		return *this / n;
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Vector3 &vec)
	{
		out << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return out;
	}

	inline glm::vec3 asGLM() const
	{
		return glm::vec3(x, y, z);
	}

	inline float max() const
	{
		return std::max(std::max(x, y), z);
	}
	inline float min() const
	{
		return std::min(std::min(x, y), z);
	}

	inline Vector3 normalize(const float l = 1.0f)
	{
		float len = length();

		if (len <= FLT_EPSILON)
			x = y = z = 0.0f;
		else
		{
			float factor = l * (1.0f / len);

			x *= factor;
			y *= factor;
			z *= factor;
		}

		return *this;
	}
	inline Vector3 normalized(const float l = 1.0f) const
	{
		return Vector3(*this).normalize(l);
	}
	inline Vector3 cross(const Vector3 &other) const
	{
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x);
	}
	inline float length() const
	{
		return std::sqrt(dot(*this));
	}
	inline float dot(const Vector3 &other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline float distance(const Vector3 &other) const
	{
		Vector3 d = *this - other;
		return d.length();
	}

	inline Vector3 lerp(const Vector3 &other, float factor) const
	{
		return *this * (1.0f - factor) + other * factor;
	}

	float x, y, z;
};
