#pragma once

#include <algorithm>
#include <cmath>
#include <float.h>
#include <ostream>

namespace Engine
{
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
	Vector3(const Vector3 &other) :
		x(other.x), y(other.y), z(other.z)
	{
	}

	void swap(Vector3 &other) noexcept
	{
		std::swap(x, other.x);
		std::swap(y, other.y);
		std::swap(z, other.z);
	}

	Vector3 &operator=(Vector3 other)
	{
		this->swap(other);
		return *this;
	}

	bool operator==(const Vector3 &other) const
	{
		return std::abs(x - other.x) < FLT_EPSILON &&
			   std::abs(y - other.y) < FLT_EPSILON &&
			   std::abs(z - other.z) < FLT_EPSILON;
	}
	bool operator!=(const Vector3 &other) const
	{
		return !(*this == other);
	}

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator+(const Vector3 &other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3 &other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const Vector3 &other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}
	Vector3 operator/(const Vector3 &other) const
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	Vector3 operator+(const float n) const
	{
		return Vector3(x + n, y + n, z + n);
	}
	Vector3 operator-(const float n) const
	{
		return Vector3(x - n, y - n, z - n);
	}
	Vector3 operator*(const float n) const
	{
		return Vector3(x * n, y * n, z * n);
	}
	Vector3 operator/(const float n) const
	{
		return Vector3(x / n, y / n, z / n);
	}

	Vector3 operator+=(const Vector3 &other) const
	{
		return *this + other;
	}
	Vector3 operator-=(const Vector3 &other) const
	{
		return *this - other;
	}
	Vector3 operator*=(const Vector3 &other) const
	{
		return *this * other;
	}
	Vector3 operator/=(const Vector3 &other) const
	{
		return *this / other;
	}

	Vector3 operator+=(const float n) const
	{
		return *this + n;
	}
	Vector3 operator-=(const float n) const
	{
		return *this - n;
	}
	Vector3 operator*=(const float n) const
	{
		return *this * n;
	}
	Vector3 operator/=(const float n) const
	{
		return *this / n;
	}

	friend std::ostream &operator<<(std::ostream &out, const Vector3 &vec)
	{
		out << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return out;
	}

	float max() const
	{
		return std::max(std::max(x, y), z);
	}
	float min() const
	{
		return std::min(std::min(x, y), z);
	}

	void normalize(const float l = 1.0f)
	{
		float len = length();

		if (len <= FLT_EPSILON)
		{
			x = y = z = 0.0f;
		}
		else
		{
			float factor = l * (1.0f / len);

			x *= factor;
			y *= factor;
			z *= factor;
		}
	}
	Vector3 normalized(const float l = 1.0f) const
	{
		Vector3 vec = Vector3(*this);
		vec.normalize(l);
		return vec;
	}
	Vector3 cross(const Vector3 &other) const
	{
		return Vector3(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x));
	}
	float length() const
	{
		return std::sqrt(dot(*this));
	}
	float dot(const Vector3 &other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}

	float distance(const Vector3 &other) const
	{
		Vector3 d = *this - other;
		return d.length();
	}

	Vector3 lerp(const Vector3 &other, float factor) const
	{
		return *this * (1.0f - factor) + other * factor;
	}

	float x, y, z;
};
} // namespace Engine
