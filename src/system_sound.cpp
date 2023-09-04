#include "system_sound.hpp"

#include "sdl_secure.hpp"

#include <SDL_mixer.h>

namespace Systems
{
namespace Sound
{

namespace
{
Mix_Music_U music = nullptr;
Mix_Chunk_U player_shot = nullptr;
Mix_Chunk_U enemy_shot = nullptr;
Mix_Chunk_U player_damaged = nullptr;
} // namespace

void load_background(std::string path) { music.reset(Mix_LoadMUS(path.c_str())); }
void play_background()
{
	if (Mix_PlayMusic(music.get(), 1000) < 0) { SDL_Log("%s", Mix_GetError()); };
}

void load_player_shot(std::string path) { player_shot.reset(Mix_LoadWAV(path.c_str())); }
void play_player_shot() { Mix_PlayChannel(-1, player_shot.get(), 0); }

void load_enemy_shot(std::string path) { enemy_shot.reset(Mix_LoadWAV(path.c_str())); }
void play_enemy_shot() { Mix_PlayChannel(-1, enemy_shot.get(), 0); }

void load_player_damaged(std::string path) { player_damaged.reset(Mix_LoadWAV(path.c_str())); }
void play_player_damaged() { Mix_PlayChannel(-1, player_damaged.get(), 0); }

} // namespace Sound
} // namespace Systems
