#include "system_render.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "renderer.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "textures.hpp"

#include <string>

namespace Systems
{
namespace Render
{
using namespace Components;

namespace
{

// Stars---------------------------------------------------------------------------

struct Star {
	SDL_FRect rect;
};

constexpr int number_of_stars = 100;

std::array<Star, number_of_stars> stars{ []() {
	auto generate_star = []() {
		return Star{ { Random::generate_float() * settings::initial_screen_width,
			       Random::generate_float() * settings::initial_screen_height, 2.F,
			       2.F } };
	};
	std::array<Star, number_of_stars> ret;
	for (auto &i : ret) {
		i = generate_star();
	}
	return ret;
}() };

void render_stars()
{
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	for (const Star &star : stars) {
		if (!settings::is_fullscreen) {
			SDL_RenderFillRectF(renderer.get(), &star.rect);
		} else {
			const SDL_FRect scaled = { star.rect.x * settings::scale_factor_x,
						   star.rect.y * settings::scale_factor_y,
						   star.rect.w * settings::scale_factor_x,
						   star.rect.h * settings::scale_factor_y };
			SDL_RenderFillRectF(renderer.get(), &scaled);
		}
	}
}

// Glyphs--------------------------------------------------------------------------

constexpr int number_of_glyphs = 65;

const SDL_Texture_S &font = Textures::font;
std::array<SDL_Rect, number_of_glyphs> glyphs{ []() {
	std::array<SDL_Rect, number_of_glyphs> ret;
	int x = 0;
	int y = 0;
	for (auto &i : ret) {
		i = { x * 32, y * 32, 32, 32 };
		++x;
		if (x == 5) {
			++y;
			x = 0;
		}
	}
	return ret;
}() };

int char_to_glyph(char c)
{
	const std::string punctuation = ".,/<>(){}[]?;:'\"/!|=+_-*^%#@$";
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	}

	if (c >= 'a' && c <= 'z') {
		return c - 'a';
	}

	if (c >= '1' && c <= '9') {
		return c - '1' + 26;
	}

	if (c == '0') {
		return 35;
	}

	int i = 0;
	for (const char j : punctuation) {
		if (c == j) {
			return i + 36;
		}
		++i;
	}

	return -1;
}

void render_string(std::string str, float x, float y, float size = 32)
{
	float move = 0;
	for (const char ch : str) {
		if (iswspace(ch)) {
			move += size;
			continue;
		}
		const SDL_Rect srcrect = glyphs[char_to_glyph(ch)];
		if (settings::is_fullscreen) {
			const SDL_FRect dstrect = { (x + move) * settings::scale_factor_x,
						    y * settings::scale_factor_y,
						    size * settings::scale_factor_x,
						    size * settings::scale_factor_y };
			SDL_RenderCopyF(renderer.get(), font.get(), &srcrect, &dstrect);

		} else {
			const SDL_FRect dstrect = { x + move, y, size, size };
			SDL_RenderCopyF(renderer.get(), font.get(), &srcrect, &dstrect);
		}
		move += size;
	}
}

void render_lives(float x, float y)
{
	float move = 0;
	for (int i = 0; i < Entity::get_hp(Entity::player_id).hp; ++i) {
		if (settings::is_fullscreen) {
			const SDL_FRect dstrect = { (x + move) * settings::scale_factor_x,
						    y * settings::scale_factor_y,
						    32 * settings::scale_factor_x,
						    32 * settings::scale_factor_y };
			SDL_RenderCopyF(renderer.get(), Textures::player.get(), 0, &dstrect);

		} else {
			const SDL_FRect dstrect = { x + move, y, 32, 32 };
			SDL_RenderCopyF(renderer.get(), Textures::player.get(), 0, &dstrect);
		}
		move += 32 + 5;
	}
}

} // namespace

void gameplay()
{
	using namespace Entity;
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xff);
	SDL_RenderClear(renderer.get());
	render_stars();
	static constexpr tag_t desired = (Tags::TEXTURE | Tags::COLLISION | Tags::POSITION);
	for (tag_t entity = 0; entity < max_entities; ++entity) {
		if (has_components(entity, desired)) {
			const auto &pos = get_position(entity);
			const auto &col = get_collision(entity);
			auto &texture = get_texture(entity).texture;
			if (settings::is_fullscreen) {
				const SDL_FRect dstrect = { pos.x * settings::scale_factor_x,
							    pos.y * settings::scale_factor_y,
							    col.w * settings::scale_factor_x,
							    col.h * settings::scale_factor_y };
				SDL_RenderCopyF(renderer.get(), texture.get(), 0, &dstrect);
			} else {
				const SDL_FRect dstrect = { pos.x, pos.y, col.w, col.h };
				SDL_RenderCopyF(renderer.get(), texture.get(), 0, &dstrect);
			}
		}
	}
	render_lives(0, settings::initial_screen_height - 32 - 32);
	render_string(std::string("Score:") + std::to_string(score), 0,
		      settings::initial_screen_height - 32, 32);
	SDL_RenderPresent(renderer.get());
}

void start_screen()
{
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xff);
	SDL_RenderClear(renderer.get());
	render_stars();
	render_string("Controls:", 50, 50 + 36 * 0, 32);
	render_string("WASD or arrows for movement", 50, 50 + 36 * 1, 32);
	render_string("Space for shooting", 50, 50 + 36 * 2, 32);
	render_string("F to toggle fullscreen", 50, 50 + 36 * 3, 32);
	render_string("Press any key to continue", 50, 50 + 36 * 5, 32);
	SDL_RenderPresent(renderer.get());
}

void game_over_screen(int best_score)
{
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xff);
	SDL_RenderClear(renderer.get());
	render_stars();
	render_string("Game over", 50, 50, 32);
	render_string(std::string("Your score:") + std::to_string(Entity::score), 50, 50 + 36 * 1,
		      32);
	render_string(std::string("Best score:") + std::to_string(best_score), 50, 50 + 36 * 2, 32);
	SDL_RenderPresent(renderer.get());
}

} // namespace Render

} // namespace Systems
