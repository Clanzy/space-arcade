#include "sc_start.hpp"

#include "system_render.hpp"
#include "timer.hpp"

void Sc_Start::SetUp() {}
void Sc_Start::Reset() {}
void Sc_Start::OnEvent(SDL_Event &event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN) {
		GameState::NextGamestate(GameStates::Gameplay);
	}
}
void Sc_Start::OnLogic() {}
void Sc_Start::OnRender() { Systems::Render::start_screen(); }
