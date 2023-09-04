#pragma once

#include "game_state.hpp"
#include "sdl_secure.hpp"

#include <vector>
#include <memory>

class GameLoop {
private:
	bool running_ = true;
	std::vector<std::unique_ptr<GameState> > states_;
	GameStates current_gamestate_ = GameStates::Start;

	SDL_Window_U main_window_ = nullptr;

public:
	GameLoop();
	~GameLoop();
	int Invoke();
};
