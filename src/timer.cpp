#include "timer.hpp"

namespace Timer
{

namespace
{

[[nodiscard]] inline long long TimeInMs() noexcept
{
	const std::chrono::time_point<std::chrono::steady_clock> now =
		std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
long long current = TimeInMs();
long long delta_time = 0;

} // namespace

void Update() noexcept
{
	auto temp = current;
	current = TimeInMs();
	delta_time = current - temp;
}
long long Now() noexcept { return current; }
long long DeltaTime() noexcept { return delta_time; }

} // namespace Timer
