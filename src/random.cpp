#include "random.hpp"

#include <limits>

namespace Random
{
namespace
{

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> float_dis(0.0F, 1.0F);
std::uniform_int_distribution<int> int_dis(0, std::numeric_limits<int>::max());

} // namespace

float generate_float() noexcept { return float_dis(gen); }
int generate_int() noexcept { return int_dis(gen); }

} // namespace Random
