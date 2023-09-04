#pragma once 

#include "game_state.hpp"

class Sc_Start : public GameState {
public:
	virtual void SetUp() override;
	virtual void Reset() override;
	virtual void OnEvent(SDL_Event &event) override;
	virtual void OnLogic() override;
	virtual void OnRender() override;
};

