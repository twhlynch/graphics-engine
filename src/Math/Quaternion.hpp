#pragma once

#include "../Math/Vector3.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cmath>
#include <ostream>

class Quaternion
{
public:
	Quaternion()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	Quaternion(const float x, const float y, const float z, const float w) :
		x(x), y(y), z(z), w(w)
	{}
	Quaternion(const Quaternion &other)
	{
		*this = other;
	}

	inline void operator=(const Quaternion &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	inline bool operator==(const Quaternion &other) const
	{
		return std::abs(x - other.x) < FLT_EPSILON &&
			   std::abs(y - other.y) < FLT_EPSILON &&
			   std::abs(z - other.z) < FLT_EPSILON &&
			   std::abs(w - other.w) < FLT_EPSILON;
	}
	inline bool operator!=(const Quaternion &other) const
	{
		return !(*this == other);
	}

	inline Quaternion operator+(const Quaternion &other) const
	{
		return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	inline Quaternion operator-(const Quaternion &other) const
	{
		return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	inline Quaternion operator*(const Quaternion &other) const
	{
		return Quaternion(
			x * other.w + y * other.z - z * other.y + w * other.x,
			-x * other.z + y * other.w + z * other.x + w * other.y,
			x * other.y - y * other.x + z * other.w + w * other.z,
			-x * other.x - y * other.y - z * other.z + w * other.w);
	}
	inline Quaternion operator/(const Quaternion &other) const
	{
		return Quaternion(
			(x * other.w - w * other.x - z * other.y + y * other.z) / (other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z),
			(y * other.w + z * other.x - w * other.y - x * other.z) / (other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z),
			(z * other.w - y * other.x + x * other.y - w * other.z) / (other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z),
			(w * other.w + x * other.x + y * other.y + z * other.z) / (other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z));
	}

	inline Quaternion operator*(const float n) const
	{
		return Quaternion(x * n, y * n, z * n, w * n);
	}
	inline Quaternion operator/(const float n) const
	{
		return Quaternion(x / n, y / n, z / n, w / n);
	}

	inline Quaternion operator+=(const Quaternion &other) const
	{
		return *this + other;
	}
	inline Quaternion operator-=(const Quaternion &other) const
	{
		return *this - other;
	}
	inline Quaternion operator*=(const Quaternion &other) const
	{
		return *this * other;
	}
	inline Quaternion operator/=(const Quaternion &other) const
	{
		return *this / other;
	}

	inline Quaternion operator*=(const float n) const
	{
		return *this * n;
	}
	inline Quaternion operator/=(const float n) const
	{
		return *this / n;
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Quaternion &quat)
	{
		out << "Quaternion(" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << ")";
		return out;
	}

	inline glm::qua<float> asGLM() const
	{
		return glm::qua<float>(w, x, y, z);
	}

	inline float max() const
	{
		return std::max(std::max(std::max(x, y), z), w);
	}
	inline float min() const
	{
		return std::min(std::min(std::min(x, y), z), w);
	}

	inline Quaternion normalize()
	{
		float len = length();

		if (len <= FLT_EPSILON)
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		else
		{
			float factor = 1.0f / len;

			x *= factor;
			y *= factor;
			z *= factor;
			w *= factor;
		}

		return *this;
	}
	inline Quaternion normalized() const
	{
		return Quaternion(*this).normalize();
	}
	inline float length() const
	{
		return std::sqrt(dot(*this));
	}
	inline float dot(const Quaternion &other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	inline Quaternion inverse()
	{
		float d = dot(*this);

		x = -x / d;
		y = -y / d;
		z = -z / d;
		w /= d;

		return *this;
	}
	inline Quaternion inversed()
	{
		return Quaternion(*this).inverse();
	}
	inline Quaternion conjugate()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
	inline Quaternion conjugated()
	{
		return Quaternion(*this).conjugate();
	}

	inline float distance(const Quaternion &other) const
	{
		Quaternion d = *this - other;
		return d.length();
	}

	inline Quaternion lerp(const Quaternion &other, float factor) const
	{
		return *this * (1.0f - factor) + other * factor;
	}

	inline Vector3 apply(Vector3 &vec) const
	{
		Vector3 u(x, y, z);
		Vector3 v = u.cross(vec) * 2.0f;
		return vec + v * w + u.cross(v);
	}

	static Quaternion WithAxisAngle(const Vector3 &axis, float radians)
	{
		float half = radians * 0.5f;
		float s = std::sin(half);
		return Quaternion(axis.x * s, axis.y * s, axis.z * s, std::cos(half));
	}
	static Quaternion WithEulerAngle(const Vector3 &euler)
	{
		float hx = euler.x * 0.5f;
		float hy = euler.y * 0.5f;
		float hz = euler.z * 0.5f;

		float cy = std::cos(hz);
		float sy = std::sin(hz);
		float cp = std::cos(hy);
		float sp = std::sin(hy);
		float cr = std::cos(hx);
		float sr = std::sin(hx);

		float srcp = sr * cp;
		float crsp = cr * sp;
		float srsp = sr * sp;
		float crcp = cr * cp;

		return Quaternion(
			srcp * cy - crsp * sy,
			crsp * cy + srcp * sy,
			crcp * sy - srsp * cy,
			crcp * cy + srsp * sy);
	}

	float x, y, z, w;
};
