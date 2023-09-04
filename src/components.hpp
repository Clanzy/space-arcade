#pragma once

#include "sdl_secure.hpp"

#include <array>

namespace Components
{

struct Position {
	float x;
	float y;
};

struct Collision {
	float w;
	float h;
};

struct Movement {
	float x = 0;
	float y = 0;
	float speed = 0;
};

struct Texture {
	SDL_Texture_S texture;
};

struct Shooting {
	int cooldown;
	int elapsed = 0;
};

struct HP {
	int hp = 1;
};

} // namespace Components
