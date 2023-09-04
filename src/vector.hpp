#pragma once

#include <cmath>

namespace Vector
{

// Make the length of vector equal to one
inline void normalize(float &x, float &y) noexcept
{
	const float length = std::sqrt(x * x + y * y);
	if (length != 0) {
		x /= length;
		y /= length;
	}
}

// Unused, but if there will be a need to implement rotation
// this allows to convert from vector (unnormalized or normalized)
// to angle (degrees)
inline float vector_to_angle(const float x, const float y) noexcept
{
	//
	return std::atan2(y, x) * 180 / M_PI - 90.F;
}

// Unused, but if there will be a need to implement rotation
// this allows to convert from angle (degrees) to vector (normalized)
inline void angle_to_vector(const float angle, float &x, float &y) noexcept
{
	float angle_rad = (angle + 90.F) * M_PI / 180.0;

	x = std::cos(angle_rad);
	y = std::sin(angle_rad);
}

} // namespace Vector
