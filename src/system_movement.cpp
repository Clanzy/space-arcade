#include "system_movement.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "settings.hpp"
#include "timer.hpp"
#include "grid.hpp"

namespace Systems
{
namespace Movement
{
void update()
{
	using namespace Entity;
	static constexpr tag_t desired = (Tags::MOVEMENT | Tags::POSITION | Tags::COLLISION);

	const float dt = static_cast<float>(Timer::DeltaTime()) / 1000.F;

	for (entity_id entity = 0; entity < max_entities; ++entity) {
		if (has_components(entity, desired)) {
			auto &pos = get_position(entity);
			const auto &mov = get_movement(entity);
			const auto &col = get_collision(entity);

			pos.x += mov.x * settings::speed_unit * dt * mov.speed;
			pos.y += mov.y * settings::speed_unit * dt * mov.speed;

			Grid::move_element(entity, pos, col);
		}
	}
}
} // namespace Movement

} // namespace Systems
