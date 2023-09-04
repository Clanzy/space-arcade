#pragma once

#include "components.hpp"
#include "entity.hpp"

#include <set>
#include <memory>

// The grid *should* increase perfomance by reducing collision checking
// it divides screen in X*Y (100 in this case) rectangles. Instead of
// checking collision of one object against all other on screen,
// it allows checking collision only against its own rectangle.
//
// I didn't run benchmark testing, but it probably saves the day when
// there are lots of projectiles and slightly more costly when there are
// few projectiles.
//
// In any case it is an opportunity to show off my amazing competitive
// programming skillz! (Joke, it's really not hard of a data structure and
// almost anyone can implement it when explained what it does)
namespace Grid
{
void move_element(Entity::entity_id element, const Components::Position &p,
		  const Components::Collision &c);
void remove_element(Entity::entity_id element);
std::set<Entity::entity_id> get_neighbour_objects(const Entity::entity_id element);
} // namespace Grid
