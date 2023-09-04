#include "sc_game_over.hpp"

#include "entity.hpp"
#include "system_render.hpp"

#include <fstream>

namespace
{
std::fstream best_score;
int value;
} // namespace

void Sc_GameOver::SetUp()
{
	best_score.open("assets/data/best_score", std::ios::in | std::ios::out);
	best_score.seekg(0);
	best_score >> value;

	if (Entity::score > value) {
		value = Entity::score;
		best_score.seekp(0);
		best_score << value;
		best_score.flush();
	}
}

void Sc_GameOver::Reset()
{
	best_score.close();
}

void Sc_GameOver::OnEvent(SDL_Event &event)
{
}

void Sc_GameOver::OnLogic()
{
}

void Sc_GameOver::OnRender()
{
	Systems::Render::game_over_screen(value);
}

Sc_GameOver::~Sc_GameOver()
{
}
