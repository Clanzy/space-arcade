#pragma once

namespace Systems
{
namespace Spawn
{

int spawn_player(float x, float y);
int spawn_small_enemy(float x, float y);
int spawn_medium_enemy(float x, float y);
int spawn_rapidfire_enemy(float x, float y);
int spawn_player_projectile(float x, float y);
int spawn_enemy_projectile(float x, float y);
void update();

} // namespace Spawn
} // namespace Systems
