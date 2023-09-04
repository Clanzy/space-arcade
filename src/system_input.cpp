#include "system_input.hpp"

#include "numkeys.hpp"
#include "vector.hpp"
#include "entity.hpp"

#include <limits>

namespace Systems
{
namespace Input
{

void proceed_numkeys()
{
	Components::Movement movement{ 0.F, 0.F, Entity::Base::base_player_movement.speed };
	if (numkeys[SDL_SCANCODE_LEFT] || numkeys[SDL_SCANCODE_A]) {
		movement.x -= 1;
	}
	if (numkeys[SDL_SCANCODE_RIGHT] || numkeys[SDL_SCANCODE_D]) {
		movement.x += 1;
	}
	if (numkeys[SDL_SCANCODE_UP] || numkeys[SDL_SCANCODE_W]) {
		movement.y -= 1;
	}
	if (numkeys[SDL_SCANCODE_DOWN] || numkeys[SDL_SCANCODE_S]) {
		movement.y += 1;
	}
	Vector::normalize(movement.x, movement.y);

	auto &player_movement = Entity::get_movement(Entity::player_id);
	player_movement.x = movement.x;
	player_movement.y = movement.y;

	auto &player_shooting = Entity::get_shooting(Entity::player_id);
	if (numkeys[SDL_SCANCODE_SPACE]) {
		// when space is held the cd is 250 ms
		player_shooting.cooldown = 250;
	} else {
		// when space is not held the cd is ~166 minutes
		player_shooting.cooldown = 9999999;
	}
}

} // namespace Input

} // namespace Systems
