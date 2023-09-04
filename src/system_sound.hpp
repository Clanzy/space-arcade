#pragma once

#include <string>

namespace Systems
{
namespace Sound
{

void load_background(std::string path);
void play_background();

void load_player_shot(std::string path);
void play_player_shot();

void load_enemy_shot(std::string path);
void play_enemy_shot();

void load_player_damaged(std::string path);
void play_player_damaged();

} // namespace Sound
} // namespace Systems
