#include "entity.hpp"

#include "random.hpp"
#include "settings.hpp"
#include "textures.hpp"
#include "timer.hpp"

#include <limits>

namespace Entity
{
auto fill_with_zero = []() noexcept {
	std::array<tag_t, max_entities> ret;
	ret.fill(Tags::ZERO);
	return ret;
};

auto fill_with_max = []() noexcept {
	std::array<Identifier, max_entities> ret;
	ret.fill(Identifier::MaxIdentifiers);
	return ret;
};

std::array<tag_t, max_entities> components{ fill_with_zero() };
std::array<Identifier, max_entities> identifiers{ fill_with_max() };
std::array<Components::Texture, max_entities> textures;
std::array<Components::Position, max_entities> positions;
std::array<Components::Collision, max_entities> collisions;
std::array<Components::Movement, max_entities> movements;
std::array<Components::Shooting, max_entities> shootings;
std::array<Components::HP, max_entities> hps;

// this is the pointer to free indexes of arrays of entities
// we need to adjust this pointer after each spawn eand deletion
inline entity_id spawn_id = 1;

// call this after each spawn
// this function sets ```spawn_id``` to next non ocupied index
// next spawn will fill this id with new entity data
void calculate_next_id()
{
	for (entity_id current = spawn_id + 1; current < max_entities; ++current) {
		if (identifiers[current] == Identifier::MaxIdentifiers) {
			spawn_id = current;
			return;
		}
	}
	// should never be there
	SDL_assert(true && "Number of entities is exceeded");
}

// call this after each deletion with id of deleted entity
// if it smaller we move ```spawn_id``` to this lower value
// next spawn will fill the freed id with new entity data
void calculate_next_id(entity_id current_id)
{
	if (current_id < spawn_id) {
		spawn_id = current_id;
	}
}

// general functions

bool has_components(entity_id entity, tag_t component_tags)
{
	return (components[entity] & component_tags) == component_tags;
}

bool is_entity_type(entity_id entity, Identifier identifier)
{
	return identifiers[entity] == identifier;
}

void despawn_entity(entity_id entity)
{
	components[entity] = Tags::ZERO;
	identifiers[entity] = Identifier::MaxIdentifiers;
	calculate_next_id(entity);
}

// spawn functions for each entity

using namespace Base;
int spawn_player(const Components::Position &base_player_position)
{
	components[player_id] = base_player_tags;
	identifiers[player_id] = base_player_identifier;
	positions[player_id] = base_player_position;
	collisions[player_id] = base_player_collision;
	movements[player_id] = base_player_movement;
	shootings[player_id] = base_player_shooting;
	hps[player_id] = base_player_hp;
	textures[player_id].texture = Textures::player;

	return player_id;
}

int spawn_small_enemy(const Components::Position &base_small_enemy_position)
{
	components[spawn_id] = base_small_enemy_tags;
	identifiers[spawn_id] = base_small_enemy_identifier;
	positions[spawn_id] = base_small_enemy_position;
	collisions[spawn_id] = base_small_enemy_collision;
	movements[spawn_id] = base_small_enemy_movement;
	shootings[spawn_id] = base_small_enemy_shooting;
	hps[spawn_id] = base_small_enemy_hp;
	textures[spawn_id].texture = Textures::small_enemy;

	const entity_id temp = spawn_id;
	calculate_next_id();
	return temp;
}

int spawn_medium_enemy(const Components::Position &base_medium_enemy_position)
{
	components[spawn_id] = base_medium_enemy_tags;
	identifiers[spawn_id] = base_medium_enemy_identifier;
	positions[spawn_id] = base_medium_enemy_position;
	collisions[spawn_id] = base_medium_enemy_collision;
	movements[spawn_id] = base_medium_enemy_movement;
	shootings[spawn_id] = base_medium_enemy_shooting;
	hps[spawn_id] = base_medium_enemy_hp;
	textures[spawn_id].texture = Textures::medium_enemy;

	const entity_id temp = spawn_id;
	calculate_next_id();
	return temp;
}

int spawn_rapidfire_enemy(const Components::Position &base_rapidfire_enemy_position)
{
	components[spawn_id] = base_rapidfire_enemy_tags;
	identifiers[spawn_id] = base_rapidfire_enemy_identifier;
	positions[spawn_id] = base_rapidfire_enemy_position;
	collisions[spawn_id] = base_rapidfire_enemy_collision;
	movements[spawn_id] = base_rapidfire_enemy_movement;
	shootings[spawn_id] = base_rapidfire_enemy_shooting;
	hps[spawn_id] = base_rapidfire_enemy_hp;
	textures[spawn_id].texture = Textures::rapidfire_enemy;

	const entity_id temp = spawn_id;
	calculate_next_id();
	return temp;
}

int spawn_player_projectile(const Components::Position &base_player_projectile_position)
{
	components[spawn_id] = base_player_projectile_tags;
	identifiers[spawn_id] = base_player_projectile_identifier;
	positions[spawn_id].x =
		base_player_projectile_position.x - base_player_projectile_collision.w / 2.F;
	positions[spawn_id].y =
		base_player_projectile_position.y - base_player_projectile_collision.h;
	collisions[spawn_id] = base_player_projectile_collision;
	movements[spawn_id] = base_player_projectile_movement;
	textures[spawn_id].texture = Textures::player_projectile;

	const entity_id temp = spawn_id;
	calculate_next_id();
	return temp;
}

int spawn_enemy_projectile(const Components::Position &base_enemy_projectile_position)
{
	components[spawn_id] = base_enemy_projectile_tags;
	identifiers[spawn_id] = base_enemy_projectile_identifier;
	positions[spawn_id].x =
		base_enemy_projectile_position.x - base_enemy_projectile_collision.w / 2.F;
	positions[spawn_id].y = base_enemy_projectile_position.y;
	collisions[spawn_id] = base_enemy_projectile_collision;
	movements[spawn_id] = base_enemy_projectile_movement;
	textures[spawn_id].texture = Textures::enemy_projectile;

	const entity_id temp = spawn_id;
	calculate_next_id();
	return temp;
}

// setter getter reset functions

const tag_t &get_components(entity_id entity)
{
	return components[entity];
}

const Identifier &get_identifier(entity_id entity)
{
	return identifiers[entity];
}

Components::Texture &get_texture(entity_id entity)
{
	return textures[entity];
}

Components::Position &get_position(entity_id entity)
{
	return positions[entity];
}

Components::Collision &get_collision(entity_id entity)
{
	return collisions[entity];
}

Components::Movement &get_movement(entity_id entity)
{
	return movements[entity];
}

Components::Shooting &get_shooting(entity_id entity)
{
	return shootings[entity];
}

Components::HP &get_hp(entity_id entity)
{
	return hps[entity];
}

void despawn_all()
{
	components.fill(Tags::ZERO);
	identifiers.fill(Identifier::MaxIdentifiers);
}
namespace
{

auto get_next_spawn_time() noexcept->long long
{
	return 2000LL + Random::generate_int() % 2000;
}

long long time_to_next_spawn = get_next_spawn_time();
long long time_to_next_spawn_elapsed = 0;

Identifier generate_enemy_type()
{
	const int chance = Random::generate_int() % 100;

	if (chance < 10) {
		return Identifier::MediumEnemy;
	}
	if (chance < 20) {
		return Identifier::RapidfireEnemy;
	}
	if (chance < 100) {
		return Identifier::SmallEnemy;
	}
	return Identifier::MaxIdentifiers;
}
} // namespace

int passive_spawn()
{
	time_to_next_spawn_elapsed += Timer::DeltaTime();
	if (time_to_next_spawn_elapsed >= time_to_next_spawn) {
		const Identifier i = generate_enemy_type();
		switch (i) {
		case Identifier::SmallEnemy:
			spawn_small_enemy(
				{ Random::generate_float() * (settings::initial_screen_width -
							      base_small_enemy_collision.w),
				  -base_small_enemy_collision.h });
			break;
		case Identifier::MediumEnemy:
			spawn_medium_enemy(
				{ Random::generate_float() * (settings::initial_screen_width -
							      base_medium_enemy_collision.w),
				  -base_medium_enemy_collision.h });
			break;
		case Identifier::RapidfireEnemy:
			spawn_rapidfire_enemy(
				{ Random::generate_float() * (settings::initial_screen_width -
							      base_rapidfire_enemy_collision.w),
				  -base_rapidfire_enemy_collision.h });
			break;

		default:
			break;
		}

		// with 20% chance make a double spawn
		if ((Random::generate_int() % 10) < 2) {
			time_to_next_spawn = 500;
		} else {
			time_to_next_spawn = get_next_spawn_time();
		}
		time_to_next_spawn_elapsed = 0;
		return 1;
	}
	return 0;
}

} // namespace Entity
