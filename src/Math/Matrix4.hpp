#pragma once

#include "Quaternion.hpp"
#include "Vector3.hpp"

#include <iostream>

class Matrix4
{
public:
	Matrix4() :
		Matrix4(0.0f)
	{
	}
	Matrix4(float n)
	{
		std::fill(std::begin(_m), std::end(_m), 0.0f);
		_m[0] = _m[5] = _m[10] = _m[15] = n;
	}

	inline float *data()
	{
		return &(_m[0]);
	}

	float &operator[](size_t index)
	{
		return _m[index];
	}

	inline Matrix4 operator*(const Matrix4 &other)
	{
		Matrix4 result(0.0f);
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				for (int k = 0; k < 4; k++)
				{
					result._m[row + col * 4] += _m[row + k * 4] * other._m[k + col * 4];
				}
			}
		}
		return result;
	}

	static Matrix4 WithLookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
	{
		const Vector3 z((target - position).normalized());
		const Vector3 x(z.cross(up).normalized());
		const Vector3 y(x.cross(z));

		Matrix4 result(1);
		result[0] = x.x;
		result[4] = x.y;
		result[8] = x.z;
		result[1] = y.x;
		result[5] = y.y;
		result[9] = y.z;
		result[2] = -z.x;
		result[6] = -z.y;
		result[10] = -z.z;
		result[12] = -x.dot(position);
		result[13] = -y.dot(position);
		result[14] = z.dot(position);
		return result;
	}

	static Matrix4 WithPerspective(float fov, float aspect, float near, float far)
	{
		const float tanHalfFov = tan(fov * 0.5f);

		Matrix4 result;
		result[0] = 1 / (aspect * tanHalfFov);
		result[5] = 1 / (tanHalfFov);
		result[10] = far / (near - far);
		result[11] = -1;
		result[14] = -(far * near) / (far - near);
		return result;
	}

	static Matrix4 WithRotation(const Quaternion &q)
	{
		float xx = q.x * q.x;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yy = q.y * q.y;
		float yz = q.y * q.z;
		float zz = q.z * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		Matrix4 result(1);
		result[0] = 1.0f - 2.0f * (yy + zz);
		result[1] = 2.0f * (xy + wz);
		result[2] = 2.0f * (xz - wy);
		result[4] = 2.0f * (xy - wz);
		result[5] = 1.0f - 2.0f * (xx + zz);
		result[6] = 2.0f * (yz + wx);
		result[8] = 2.0f * (xz + wy);
		result[9] = 2.0f * (yz - wx);
		result[10] = 1.0f - 2.0f * (xx + yy);
		return result;
	}

	static Matrix4 WithTranslation(const Vector3 &v)
	{
		Matrix4 result(1);
		result[12] = v.x;
		result[13] = v.y;
		result[14] = v.z;
		return result;
	}

	static Matrix4 WithScale(const Vector3 &v)
	{
		Matrix4 result(1);
		result[0] *= v.x;
		result[5] *= v.y;
		result[10] *= v.z;
		return result;
	}

private:
	float _m[16];
};
