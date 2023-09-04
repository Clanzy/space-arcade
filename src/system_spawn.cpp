#include "system_spawn.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "random.hpp"
#include "textures.hpp"
#include "timer.hpp"
#include "grid.hpp"
#include "vector.hpp"
#include "settings.hpp"

namespace Systems
{
namespace Spawn
{
using namespace Components;

namespace
{

auto get_next_spawn_time() noexcept->long long { return 2000LL + Random::generate_int() % 2000; }

long long time_to_next_spawn = get_next_spawn_time();
long long time_to_next_spawn_elapsed = 0;

Components::Identifier generate_enemy_type()
{
	const int chance = Random::generate_int() % 100;

	if (chance < 10) { return Identifier::MediumEnemy; }
	if (chance < 20) { return Identifier::RapidfireEnemy; }
	if (chance < 100) { return Identifier::SmallEnemy; }
	return Identifier::MaxIdentifiers;
}

int passive_spawn()
{
	time_to_next_spawn_elapsed += Timer::DeltaTime();
	if (time_to_next_spawn_elapsed >= time_to_next_spawn) {
		const Identifier i = generate_enemy_type();
		switch (i) {
		case Identifier::SmallEnemy:
			spawn_small_enemy(Random::generate_float() *
						  (settings::initial_screen_width -
						   Entities::SmallEnemy::collision.w),
					  -Entities::SmallEnemy::collision.h);
			break;
		case Identifier::MediumEnemy:
			spawn_medium_enemy(Random::generate_float() *
						   (settings::initial_screen_width -
						    Entities::MediumEnemy::collision.w),
					   -Entities::MediumEnemy::collision.h);
			break;
		case Identifier::RapidfireEnemy:
			spawn_rapidfire_enemy(Random::generate_float() *
						      (settings::initial_screen_width -
						       Entities::RapidfireEnemy::collision.w),
					      -Entities::RapidfireEnemy::collision.h);
			break;

		default: break;
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

void insert_into_grid(int element, const Components::Position &p, const Components::Collision &c)
{
	Grid::move_element(element, p, c);
}
} // namespace

int spawn_player(float x, float y)
{
	constexpr int i = 0;
	tags[i] = Entities::Player::tag;
	identifiers[i] = Entities::Player::identifier;
	positions[i] = { x, y };
	collisions[i] = Entities::Player::collision;
	movements[i] = Entities::Player::movement;
	shootings[i] = Entities::Player::shooting;
	hps[i] = Entities::Player::hp;
	textures[i] = { Textures::player };
	insert_into_grid(i, positions[i], collisions[i]);
	return i;
}

int spawn_small_enemy(float x, float y)
{
	for (int i = 0; i < max_entities; ++i) {
		if (tags[i] == Tags::ZERO) {
			tags[i] = Entities::SmallEnemy::tag;
			identifiers[i] = Entities::SmallEnemy::identifier;
			positions[i] = { x, y };
			collisions[i] = Entities::SmallEnemy::collision;
			movements[i] = Entities::SmallEnemy::movement;
			shootings[i] = Entities::SmallEnemy::shooting;
			hps[i] = Entities::SmallEnemy::hp;
			textures[i] = { Textures::small_enemy };
			insert_into_grid(i, positions[i], collisions[i]);
			return i;
		}
	}
	throw "More than 1024 entities";
}

int spawn_medium_enemy(float x, float y)
{
	for (int i = 0; i < max_entities; ++i) {
		if (tags[i] == Tags::ZERO) {
			tags[i] = Entities::MediumEnemy::tag;
			identifiers[i] = Entities::MediumEnemy::identifier;
			positions[i] = { x, y };
			collisions[i] = Entities::MediumEnemy::collision;
			movements[i] = Entities::MediumEnemy::movement;
			shootings[i] = Entities::MediumEnemy::shooting;
			hps[i] = Entities::MediumEnemy::hp;
			textures[i] = { Textures::medium_enemy };
			insert_into_grid(i, positions[i], collisions[i]);
			return i;
		}
	}
	throw "More than 1024 entities";
}

int spawn_rapidfire_enemy(float x, float y)
{
	for (int i = 0; i < max_entities; ++i) {
		if (tags[i] == Tags::ZERO) {
			tags[i] = Entities::RapidfireEnemy::tag;
			identifiers[i] = Entities::RapidfireEnemy::identifier;
			positions[i] = { x, y };
			collisions[i] = Entities::RapidfireEnemy::collision;
			movements[i] = Entities::RapidfireEnemy::movement;
			shootings[i] = Entities::RapidfireEnemy::shooting;
			hps[i] = Entities::RapidfireEnemy::hp;
			textures[i] = { Textures::rapidfire_enemy };
			insert_into_grid(i, positions[i], collisions[i]);
			return i;
		}
	}
	throw "More than 1024 entities";
}

int spawn_enemy_projectile(float x, float y)
{
	for (int i = 0; i < max_entities; ++i) {
		if (tags[i] == Tags::ZERO) {
			tags[i] = Entities::EnemyProjectile::tag;
			identifiers[i] = Entities::EnemyProjectile::identifier;
			positions[i] = { x - Entities::EnemyProjectile::collision.w / 2.F, y };
			collisions[i] = Entities::EnemyProjectile::collision;
			movements[i] = Entities::EnemyProjectile::movement;
			textures[i] = { Textures::enemy_projectile };
			insert_into_grid(i, positions[i], collisions[i]);
			return i;
		}
	}
	throw "More than 1024 entities";
}

int spawn_player_projectile(float x, float y)
{
	for (int i = 0; i < max_entities; ++i) {
		if (tags[i] == Tags::ZERO) {
			tags[i] = Entities::PlayerProjectile::tag;
			identifiers[i] = Entities::PlayerProjectile::identifier;
			positions[i] = { x - Entities::PlayerProjectile::collision.w / 2.F,
					 y - Entities::PlayerProjectile::collision.h };
			collisions[i] = Entities::PlayerProjectile::collision;
			movements[i] = Entities::PlayerProjectile::movement;
			textures[i] = { Textures::player_projectile };
			insert_into_grid(i, positions[i], collisions[i]);
			return i;
		}
	}
	throw "More than 1024 entities";
}

void update()
{
	static constexpr tag_t desired = (Tags::COLLISION | Tags::POSITION);
	for (int i = 0; i < max_entities; ++i) {
		if (to_despawn[i]) {
			tags[i] = Tags::ZERO;
			identifiers[i] = Identifier::MaxIdentifiers;
			to_despawn[i] = false;
			if ((tags[i] & desired) == desired) { Grid::remove_element(i); }
		}
	}
	passive_spawn();
}

} // namespace Spawn

} // namespace Systems
