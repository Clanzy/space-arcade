#pragma once

#include <SDL.h>

enum class GameStates {
	Start,
	Gameplay,
	Game_over,
	Max_game_states
};

class GameState {
private:
	static inline GameStates next_gamestate_ = GameStates::Max_game_states;

public:
	static GameStates NextGamestate() { return next_gamestate_; }
	static void NextGamestate(GameStates gamestate) { next_gamestate_ = gamestate; }

	virtual ~GameState() {}

	virtual void SetUp() = 0;
	virtual void Reset() = 0;
	virtual void OnEvent(SDL_Event &event) = 0;
	virtual void OnLogic() = 0;
	virtual void OnRender() = 0;
};
