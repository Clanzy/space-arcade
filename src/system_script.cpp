#include "system_script.hpp"

#include "components.hpp"
#include "settings.hpp"
#include "grid.hpp"
#include "timer.hpp"
#include "system_sound.hpp"

namespace Systems
{
namespace Script
{
using namespace Components;

namespace
{
void player_check_edges(Position &position, const Collision &collision)
{
	if (position.x < 0) {
		position.x = 0;
	} else if (position.x + collision.w > settings::initial_screen_width) {
		position.x = settings::initial_screen_width - collision.w;
	}
	if (position.y < 0) {
		position.y = 0;
	} else if (position.y + collision.h > settings::initial_screen_height) {
		position.y = settings::initial_screen_height - collision.h;
	}
}

void check_edges_and_despawn(Entity::entity_id i, const Position &position,
			     const Collision &collision)
{
	if (position.x < -collision.w) {
		Entity::despawn_entity(i);
	} else if (position.x > settings::initial_screen_width + collision.w) {
		Entity::despawn_entity(i);
	} else if (position.y < -collision.h) {
		Entity::despawn_entity(i);
	} else if (position.y > settings::initial_screen_height + collision.h) {
		Entity::despawn_entity(i);
	}
}

void stop_at_half_screen(const Position &position, const Collision &collision, Movement &movement)
{
	if (position.y + collision.h >= settings::initial_screen_height / 3.F) {
		movement.speed = 0.F;
	}
}

[[nodiscard]] bool detect_collision(const Components::Position &p1, const Components::Collision &c1,
				    const Components::Position &p2, const Components::Collision &c2)
{
	const float l1 = p1.x, t1 = p1.y, r1 = p1.x + c1.w, b1 = p1.y + c1.h;
	const float l2 = p2.x, t2 = p2.y, r2 = p2.x + c2.w, b2 = p2.y + c2.h;

	return l1 < r2 && r1 > l2 && t1 < b2 && b1 > t2;
}

int player_scripts()
{
	static bool invulnerability = false;
	static long long elapsed = 0;

	auto &player_pos = Entity::get_position(Entity::player_id);
	auto &player_col = Entity::get_collision(Entity::player_id);
	if (invulnerability) {
		elapsed += Timer::DeltaTime();
		if (elapsed >= 3000) {
			invulnerability = false;
		}

	} else {
		auto s = Grid::get_neighbour_objects(Entity::player_id);
		for (auto j : s) {
			const auto identifier = Entity::get_identifier(j);
			if (identifier == Entity::Identifier::EnemyProjectile ||
			    identifier == Entity::Identifier::SmallEnemy ||
			    identifier == Entity::Identifier::MediumEnemy ||
			    identifier == Entity::Identifier::RapidfireEnemy) {
				auto &enemy_pos = Entity::get_position(j);
				auto &enemy_col = Entity::get_collision(j);
				if (detect_collision(player_pos, player_col, enemy_pos,
						     enemy_col)) {
					auto &player_hp = Entity::get_hp(Entity::player_id);
					player_hp.hp -= 1;
					Systems::Sound::play_player_damaged();
					if (identifier == Entity::Identifier::EnemyProjectile) {
						Entity::despawn_entity(j);
					}

					if (player_hp.hp > 0) {
						invulnerability = true;
						elapsed = 0;
					} else {
						auto &player_shooting =
							Entity::get_shooting(Entity::player_id);
						auto &player_texture =
							Entity::get_texture(Entity::player_id);
						player_shooting.cooldown = 100000;
						player_texture.texture = nullptr;
						return 1;
					}
				}
			}
		}
	}
	player_check_edges(player_pos, player_col);
	return 0;
}

void small_enemy_scripts(const Entity::entity_id i)
{
	const auto &pos = Entity::get_position(i);
	const auto &col = Entity::get_collision(i);
	check_edges_and_despawn(i, pos, col);
	auto s = Grid::get_neighbour_objects(i);
	for (auto j : s) {
		const auto &idents = Entity::get_identifier(j);
		if (idents == Entity::Identifier::PlayerProjectile) {
			const auto &other_pos = Entity::get_position(i);
			const auto &other_col = Entity::get_collision(i);

			if (detect_collision(pos, col, other_pos, other_col)) {
				// despawn proj

				Entity::despawn_entity(j);

				auto &hp = Entity::get_hp(i);
				hp.hp -= 1;

				if (hp.hp <= 0) {
					Entity::despawn_entity(i);
					Entity::score += 10;
				}
			}
		}
	}
}
void medium_enemy_scripts(int i)
{
	const auto &pos = Entity::get_position(i);
	const auto &col = Entity::get_collision(i);
	check_edges_and_despawn(i, pos, col);
	auto s = Grid::get_neighbour_objects(i);
	for (auto j : s) {
		const auto idents = Entity::get_identifier(j);
		if (idents == Entity::Identifier::PlayerProjectile) {
			const auto &other_pos = Entity::get_position(i);
			const auto &other_col = Entity::get_collision(i);
			if (detect_collision(pos, col, other_pos, other_col)) {
				// despawn proj
				Entity::despawn_entity(j);

				auto &hp = Entity::get_hp(i);
				hp.hp -= 1;

				if (hp.hp <= 0) {
					Entity::despawn_entity(i);
					Entity::score += 50;
				}
			}
		}
	}
}
void rapidfire_enemy_scripts(int i)
{
	const auto &pos = Entity::get_position(i);
	const auto &col = Entity::get_collision(i);
	auto moves = Entity::get_movement(i);
	stop_at_half_screen(pos, col, moves);

	auto s = Grid::get_neighbour_objects(i);
	for (auto j : s) {
		const auto idents = Entity::get_identifier(j);
		if (idents == Entity::Identifier::PlayerProjectile) {
			const auto &other_pos = Entity::get_position(i);
			const auto &other_col = Entity::get_collision(i);
			if (detect_collision(pos, col, other_pos, other_col)) {
				// despawn proj
				Entity::despawn_entity(j);

				auto &hp = Entity::get_hp(i);
				hp.hp -= 1;

				if (hp.hp <= 0) {
					Entity::despawn_entity(i);
					Entity::score += 80;
				}
			}
		}
	}
}

} // namespace

int update()
{
	const int rc = player_scripts();
	for (Entity::entity_id i = 0; i < Entity::max_entities; ++i) {
		const auto &idents = Entity::get_identifier(i);
		switch (idents) {
		case Entity::Identifier::SmallEnemy:

			small_enemy_scripts(i);
			break;
		case Entity::Identifier::MediumEnemy:
			medium_enemy_scripts(i);
			break;
		case Entity::Identifier::RapidfireEnemy:
			rapidfire_enemy_scripts(i);
			break;
		case Entity::Identifier::EnemyProjectile:
		case Entity::Identifier::PlayerProjectile: {
			const auto &pos = Entity::get_position(i);
			const auto &col = Entity::get_collision(i);
			check_edges_and_despawn(i, pos, col);
			check_edges_and_despawn(i, pos, col);
			break;
		}
		default:
			break;
		}
	}
	return rc;
}

} // namespace Script

} // namespace Systems
