#include "game_loop.hpp"

#include "sdl_secure.hpp"
#include "settings.hpp"
#include "timer.hpp"
#include "renderer.hpp"
#include "numkeys.hpp"
#include "textures.hpp"
#include "sc_start.hpp"
#include "sc_gameplay.hpp"
#include "sc_game_over.hpp"
#include "system_sound.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

GameLoop::GameLoop()
	: states_(static_cast<int>(GameStates::Max_game_states))
{
	constexpr int supported_images = IMG_INIT_PNG;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw std::runtime_error(std::string("SDL was not initialized: ") + SDL_GetError());
	}
	if (!(IMG_Init(supported_images) & supported_images)) {
		throw std::runtime_error(std::string("SDL_image was not initialized: ") +
					 IMG_GetError());
	}

	if (Mix_OpenAudio(88200, MIX_DEFAULT_FORMAT, 8, 4096) == -1) {
		throw std::runtime_error(std::string("SDL_mixer was not initialized: ") +
					 Mix_GetError());
	}

	main_window_ = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					settings::initial_screen_width,
					settings::initial_screen_height, 0);

	// SDL_RENDERER_PRESENTVSYNC sets an FPS ceiling for application that is equal to the refresh rate of the monitor
	renderer = SDL_CreateRenderer(main_window_.get(), -1,
				      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	numkeys = SDL_GetKeyboardState(nullptr);

	Textures::font = IMG_LoadTexture(renderer.get(), "assets/textures/font.png");
	Textures::player = IMG_LoadTexture(renderer.get(), "assets/textures/spaceship.png");
	Textures::small_enemy = IMG_LoadTexture(renderer.get(), "assets/textures/small_enemy.png");
	Textures::medium_enemy =
		IMG_LoadTexture(renderer.get(), "assets/textures/medium_enemy.png");
	Textures::rapidfire_enemy =
		IMG_LoadTexture(renderer.get(), "assets/textures/rapidfire_enemy.png");
	Textures::enemy_projectile =
		IMG_LoadTexture(renderer.get(), "assets/textures/enemy_projectile.png");
	Textures::player_projectile =
		IMG_LoadTexture(renderer.get(), "assets/textures/player_projectile.png");

	Systems::Sound::load_background("assets/sounds/background.wav");
	Systems::Sound::load_enemy_shot("assets/sounds/enemy_shot.wav");
	Systems::Sound::load_player_shot("assets/sounds/player_shot.wav");
	Systems::Sound::load_player_damaged("assets/sounds/player_damaged.wav");

	states_[static_cast<int>(GameStates::Start)] = std::make_unique<Sc_Start>();
	states_[static_cast<int>(GameStates::Gameplay)] = std::make_unique<Sc_Gameplay>();
	states_[static_cast<int>(GameStates::Game_over)] = std::make_unique<Sc_GameOver>();

	states_[static_cast<int>(current_gamestate_)]->SetUp();
}

int GameLoop::Invoke()
{
	static const auto change_gamestate = [this]() {
		if (GameState::NextGamestate() != GameStates::Max_game_states) {
			states_[static_cast<int>(current_gamestate_)]->Reset();
			current_gamestate_ = GameState::NextGamestate();
			states_[static_cast<int>(current_gamestate_)]->SetUp();
			GameState::NextGamestate(GameStates::Max_game_states);
		}
	};

	static const auto toggle_fullscreen = [this]() {
		settings::is_fullscreen = !settings::is_fullscreen;
		SDL_SetWindowFullscreen(main_window_.get(), settings::is_fullscreen ?
								    SDL_WINDOW_FULLSCREEN_DESKTOP :
								    0);
		int new_width;
		int new_height;
		SDL_GetWindowSize(main_window_.get(), &new_width, &new_height);

		settings::scale_factor_x = static_cast<float>(new_width) /
					   static_cast<float>(settings::initial_screen_width);
		settings::scale_factor_y = static_cast<float>(new_height) /
					   static_cast<float>(settings::initial_screen_height);

		settings::speed_scale_x = settings::is_fullscreen ? settings::scale_factor_x : 1.F;
		settings::speed_scale_y = settings::is_fullscreen ? settings::scale_factor_y : 1.F;
	};

	Systems::Sound::play_background();
	SDL_Event event;
	while (running_) {
		Timer::Update();
		while (SDL_PollEvent(&event) == 1) {
			if (event.type == SDL_QUIT) {
				running_ = false;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_f) { toggle_fullscreen(); }
			}
			states_[static_cast<int>(current_gamestate_)]->OnEvent(event);
		}
		states_[static_cast<int>(current_gamestate_)]->OnLogic();
		change_gamestate();
		states_[static_cast<int>(current_gamestate_)]->OnRender();
	}

	return 0;
}

GameLoop::~GameLoop()
{
	SDL_DestroyRenderer(renderer.get());
	SDL_DestroyWindow(main_window_.get());

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
