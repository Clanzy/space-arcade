#include "system_shooting.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include "random.hpp"
#include "timer.hpp"
#include "system_sound.hpp"

namespace Systems
{
namespace Shooting
{
using namespace Components;
namespace
{

void small_enemy_shoot(Components::Shooting &shooting, const Components::Position &position,
		       const Components::Collision &collision)
{
	shooting.elapsed += Timer::DeltaTime();
	if (shooting.elapsed >= shooting.cooldown) {
		Systems::Sound::play_enemy_shot();
		const float x = position.x + collision.w / 2;
		const float y = position.y + collision.h;
		const Entity::entity_id proj = Entity::spawn_enemy_projectile({ x, y });

		const auto &player_pos = Entity::get_position(Entity::player_id);
		const auto &player_col = Entity::get_collision(Entity::player_id);
		const float x2 = player_pos.x + player_col.w / 2.F;
		const float y2 = player_pos.y + player_col.h / 2.F;
		Movement m{ x2 - x, y2 - Entity::Base::base_enemy_projectile_collision.h / 2.F - y,
			    1.F };
		Vector::normalize(m.x, m.y);
		// TODO: implement normal setters instead of this weird syntax
		auto &mov = Entity::get_movement(proj);
		mov.x = m.x;
		mov.y = m.y;
		mov.speed = m.speed;

		shooting.elapsed = 0;
	}
}

void rapidfire_enemy_shoot(Components::Shooting &shooting, const Components::Position &position,
			   const Components::Collision &collision)
{
	shooting.elapsed += Timer::DeltaTime();
	if (shooting.elapsed >= shooting.cooldown) {
		Systems::Sound::play_enemy_shot();
		const float x = position.x + collision.w / 2;
		const float y = position.y + collision.h;
		const int proj = Entity::spawn_enemy_projectile({ x, y });

		Movement m{ Random::generate_float() * 4.F - 2.F, 1.F, 1.F };
		Vector::normalize(m.x, m.y);
		auto &mov = Entity::get_movement(proj);
		mov.x = m.x;
		mov.y = m.y;
		mov.speed = m.speed;

		shooting.elapsed = 0;
	}
}

void medium_enemy_shoot(Components::Shooting &shooting, const Components::Position &position,
			const Components::Collision &collision)
{
	shooting.elapsed += Timer::DeltaTime();
	if (shooting.elapsed >= shooting.cooldown) {
		Systems::Sound::play_enemy_shot();
		const float x = position.x;
		const float y = position.y + collision.h;
		const Entity::entity_id left = Entity::spawn_enemy_projectile({ x, y });
		auto &left_proj_mov = Entity::get_movement(left);
		left_proj_mov.x = -1.F;
		left_proj_mov.y = 2.2F;
		left_proj_mov.speed = 1.5F;
		Vector::normalize(left_proj_mov.x, left_proj_mov.y);

		const Entity::entity_id mid =
			Entity::spawn_enemy_projectile({ x + collision.w / 2.F, y });
		Entity::get_movement(mid).speed = 1.5F;

		const Entity::entity_id right =
			Entity::spawn_enemy_projectile({ x + collision.w, y });
		auto &right_proj_mov = Entity::get_movement(right);
		right_proj_mov.x = 1.F;
		right_proj_mov.y = 2.2F;
		right_proj_mov.speed = 1.5F;
		Vector::normalize(right_proj_mov.x, right_proj_mov.y);

		shooting.elapsed = 0;
	}
}

void player_shoot(Components::Shooting &shooting, const Components::Position &position,
		  const Components::Collision &collision)
{
	shooting.elapsed += Timer::DeltaTime();
	if (shooting.elapsed >= shooting.cooldown) {
		Systems::Sound::play_player_shot();
		const float x = position.x + collision.w / 2;
		const float y = position.y;
		Entity::spawn_player_projectile({ x, y });
		shooting.elapsed = 0;
	}
}
} // namespace

void update()
{
	static constexpr Entity::tag_t desired =
		(Entity::Tags::SHOOTING | Entity::Tags::POSITION | Entity::Tags::COLLISION);

	auto &player_shooting = Entity::get_shooting(Entity::player_id);
	const auto &player_pos = Entity::get_position(Entity::player_id);
	const auto &player_col = Entity::get_collision(Entity::player_id);

	player_shoot(player_shooting, player_pos, player_col);
	for (Entity::entity_id i = 1; i < Entity::max_entities; ++i) {
		if (Entity::has_components(i, desired)) {
			const auto idents = Entity::get_identifier(i);
			auto &shooting = Entity::get_shooting(i);
			const auto &pos = Entity::get_position(i);
			const auto &col = Entity::get_collision(i);
			switch (idents) {
			case Entity::Identifier::SmallEnemy:
				small_enemy_shoot(shooting, pos, col);
				break;
			case Entity::Identifier::MediumEnemy:
				medium_enemy_shoot(shooting, pos, col);
				break;
			case Entity::Identifier::RapidfireEnemy:
				rapidfire_enemy_shoot(shooting, pos, col);
				break;

			default:
				break;
			}
		}
	}
}
} // namespace Shooting
} // namespace Systems
