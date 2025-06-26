#pragma once

template <typename T>
inline T lerp(T a, T b, T t)
{
	return a + ((b - a) * t);
}
