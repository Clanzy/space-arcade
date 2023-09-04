#pragma once

#include <chrono>

namespace Timer
{
void Update() noexcept;
[[nodiscard]] long long Now() noexcept;
[[nodiscard]] long long DeltaTime() noexcept;
} // namespace Timer
