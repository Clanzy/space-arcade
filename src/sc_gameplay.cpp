#include "sc_gameplay.hpp"

#include "entity.hpp"
#include "settings.hpp"
#include "system_input.hpp"
#include "system_movement.hpp"
#include "system_render.hpp"
#include "system_script.hpp"
#include "system_shooting.hpp"
#include "timer.hpp"

namespace
{
int freeze_screen = 0;
const long long freeze_time = 3000;
long long freeze_time_elapsed = 0;
} // namespace

void Sc_Gameplay::SetUp()
{
	Entity::spawn_player({ (settings::initial_screen_width - 64.F) / 2.F,
			       settings::initial_screen_height - 64.F });
	freeze_screen = 0;
	freeze_time_elapsed = 0;
}

void Sc_Gameplay::Reset()
{
}

void Sc_Gameplay::OnEvent(SDL_Event &event)
{
}

void Sc_Gameplay::OnLogic()
{
	if (freeze_screen == 1) {
		Systems::Movement::update();
		Systems::Script::update();
		freeze_time_elapsed += Timer::DeltaTime();
		if (freeze_time_elapsed >= freeze_time) {
			NextGamestate(GameStates::Game_over);
		}
	} else {
		Systems::Input::proceed_numkeys();
		Entity::passive_spawn();
		Systems::Movement::update();
		freeze_screen = Systems::Script::update();
		Systems::Shooting::update();
	}
}

void Sc_Gameplay::OnRender()
{
	Systems::Render::gameplay();
}

Sc_Gameplay::~Sc_Gameplay()
{
	this->Reset();
}
