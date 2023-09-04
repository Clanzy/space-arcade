#pragma once

#include <random>

namespace Random
{
	[[nodiscard]] float generate_float() noexcept;
	[[nodiscard]] int generate_int() noexcept;
} // namespace Random
