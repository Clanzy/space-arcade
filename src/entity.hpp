#pragma once

#include "components.hpp"

namespace Entity
{

// I don't know where to put it better
// it could be in an external header but
// I don't think this variable is worthy making a separate file
inline int score = 0;

using tag_t = uint32_t;

// each tag corresponds to each component from Components namespace
// you can query it with has_components() (using OR) function to detect
// if certain entity has these components
namespace Tags
{
inline constexpr tag_t ZERO = 0;
inline constexpr tag_t TEXTURE = 1;
inline constexpr tag_t POSITION = 1 << 1;
inline constexpr tag_t COLLISION = 1 << 2;
inline constexpr tag_t MOVEMENT = 1 << 3;
inline constexpr tag_t SHOOTING = 1 << 4;
inline constexpr tag_t HP = 1 << 5;
} // namespace Tags

enum class Identifier : int8_t {
	Player = 0,
	SmallEnemy,
	MediumEnemy,
	PlayerProjectile,
	EnemyProjectile,
	RapidfireEnemy,
	MaxIdentifiers
};

using entity_id = int;
inline constexpr entity_id max_entities = 1024;
inline constexpr entity_id player_id = 0;

// general functions

bool has_components(entity_id entity, tag_t component_tags);
bool is_entity_type(entity_id entity, Identifier identifier);
void despawn_entity(entity_id entity);

namespace Base
{

// base entity values

constexpr tag_t base_player_tags = Tags::POSITION | Tags::COLLISION | Tags::MOVEMENT |
				   Tags::TEXTURE | Tags::SHOOTING | Tags::HP;
constexpr Identifier base_player_identifier = Identifier::Player;
constexpr Components::Collision base_player_collision = { 64.F, 64.F };
constexpr Components::Movement base_player_movement = { 0.F, 0.F, 1.5F };
constexpr Components::Shooting base_player_shooting = { 9999999, 0 };
constexpr Components::HP base_player_hp = { 3 };

constexpr tag_t base_small_enemy_tags = Tags::TEXTURE | Tags::POSITION | Tags::COLLISION |
					Tags::MOVEMENT | Tags::SHOOTING | Tags::HP;
constexpr Identifier base_small_enemy_identifier = Identifier::SmallEnemy;
constexpr Components::Collision base_small_enemy_collision = { 64.F, 64.F };
constexpr Components::Movement base_small_enemy_movement = { 0.F, 1.F, 0.33F };
constexpr Components::Shooting base_small_enemy_shooting = { 1250, 1050 };
constexpr Components::HP base_small_enemy_hp = { 2 };

constexpr tag_t base_medium_enemy_tags = Tags::TEXTURE | Tags::POSITION | Tags::COLLISION |
					 Tags::MOVEMENT | Tags::SHOOTING | Tags::HP;
constexpr Identifier base_medium_enemy_identifier = Identifier::MediumEnemy;
constexpr Components::Collision base_medium_enemy_collision = { 96.F, 96.F };
constexpr Components::Movement base_medium_enemy_movement = { 0.F, 1.F, 0.2F };
constexpr Components::Shooting base_medium_enemy_shooting = { 2200, 2000 };
constexpr Components::HP base_medium_enemy_hp = { 6 };

constexpr tag_t base_rapidfire_enemy_tags = Tags::TEXTURE | Tags::POSITION | Tags::COLLISION |
					    Tags::MOVEMENT | Tags::SHOOTING | Tags::HP;
constexpr Identifier base_rapidfire_enemy_identifier = Identifier::RapidfireEnemy;
constexpr Components::Collision base_rapidfire_enemy_collision = { 96.F, 96.F };
constexpr Components::Movement base_rapidfire_enemy_movement = { 0.F, 1.F, 0.2F };
constexpr Components::Shooting base_rapidfire_enemy_shooting = { 400, 200 };
constexpr Components::HP base_rapidfire_enemy_hp = { 6 };

constexpr tag_t base_player_projectile_tags = Tags::TEXTURE | Tags::POSITION | Tags::COLLISION |
					      Tags::MOVEMENT;
constexpr Identifier base_player_projectile_identifier = Identifier::PlayerProjectile;
constexpr Components::Collision base_player_projectile_collision = { 16.F, 32.F };
constexpr Components::Movement base_player_projectile_movement = { 0.F, -1.F, 2.2F };

constexpr tag_t base_enemy_projectile_tags = Tags::TEXTURE | Tags::POSITION | Tags::COLLISION |
					     Tags::MOVEMENT;
constexpr Identifier base_enemy_projectile_identifier = Identifier::EnemyProjectile;
constexpr Components::Collision base_enemy_projectile_collision = { 16.F, 16.F };
constexpr Components::Movement base_enemy_projectile_movement = { 0.F, 1.F, 0.4F };

} // namespace Base

// spawn functions for each entity

int spawn_player(const Components::Position &base_player_position);
int spawn_small_enemy(const Components::Position &base_small_enemy_position);
int spawn_medium_enemy(const Components::Position &base_medium_enemy_position);
int spawn_rapidfire_enemy(const Components::Position &base_rapidfire_enemy_position);
int spawn_player_projectile(const Components::Position &base_player_projectile_position);
int spawn_enemy_projectile(const Components::Position &base_enemy_projectile_position);

// setter getter reset functions

const tag_t &get_components(entity_id entity);
const Identifier &get_identifier(entity_id entity);

// functions below don't check if component is valid
// always check get_components() before getting component
// these functions return references so they are both getters and setters

Components::Texture &get_texture(entity_id entity);
Components::Position &get_position(entity_id entity);
Components::Collision &get_collision(entity_id entity);
Components::Movement &get_movement(entity_id entity);
Components::Shooting &get_shooting(entity_id entity);
Components::HP &get_hp(entity_id entity);

// this function doesn't clear components
// it only fills component tags to zero and idintifier to MaxIdentifiers
void despawn_all();

int passive_spawn();

} // namespace Entity
