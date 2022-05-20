#include <exception>
#include <array>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Game.h"
#include "Object.h"
#include "Camera.h"
#include "Constants.h"
#include "Player.h"

Game::Game(const char* title, int x, int y, int width, int height, Uint32 flags)
	: state { STATE::MENU } {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) throw std::exception("Failed to initialise SDL");

	window = SDL_CreateWindow(title, x, y, width, height, flags);
	if (!window) throw std::exception("Failed to create window");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) throw std::exception("Failed to create renderer");
}

Game::~Game() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run() {
	while (state != STATE::QUIT) {
		switch (state) {
		case STATE::MENU: state = MainMenu(); break;
		case STATE::INSTRUCTIONS: state = Instructions(); break;
		case STATE::LEVEL1: state = Level1(); break;
		case STATE::GAME_WON: state = GameWon(); break;
		case STATE::QUIT: break;
		}
	}
}

// ----------------------------------- Menus and levels -------------------------------------------

STATE Game::MainMenu() {
	Texture background("../Images/Background.bmp", renderer);
	
	enum Buttons { Play, HowToPlay, Quit, TotalButtons };
	std::array<ObjectAndTexture, TotalButtons> buttons {
		ObjectAndTexture{ Object(380, 200, 200, 100), Texture("../Images/PlayBtn.bmp", renderer) },
		ObjectAndTexture{ Object(380, 310, 200, 100), Texture("../Images/Instructions.bmp", renderer) },
		ObjectAndTexture{ Object(380, 420, 200, 100), Texture("../Images/Quit.bmp", renderer) }
	};

	while (true) {
		// Event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return STATE::QUIT;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (buttons[Play].object.IsClicked(mouse_x, mouse_y)) return STATE::LEVEL1;
				if (buttons[HowToPlay].object.IsClicked(mouse_x, mouse_y)) return STATE::INSTRUCTIONS;
				if (buttons[Quit].object.IsClicked(mouse_x, mouse_y)) return STATE::QUIT;
			}
		}

		// Rendering
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		background.Render(renderer, nullptr, nullptr);
		for (auto& o : buttons) o.object.Render(renderer, o.texture);

		SDL_RenderPresent(renderer);
	}
}

STATE Game::Instructions() {
	Texture background("../Images/InstructionsBackground.bmp", renderer);
	ObjectAndTexture menu_btn{
		Object(380, 400, 200, 100),
		Texture("../Images/MenuBtn.bmp", renderer)
	};

	// Game loop
	while (true) {
		// Event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return STATE::QUIT;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);

				if (menu_btn.object.IsClicked(mouse_x, mouse_y))
					return STATE::MENU;
			}
		}

		// Rendering
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		background.Render(renderer, nullptr, nullptr);
		menu_btn.object.Render(renderer, menu_btn.texture);

		SDL_RenderPresent(renderer);
	}
}

STATE Game::GameWon() {
	Texture game_won_text("../Images/GameWonText.bmp", renderer, 255, 255, 255);
	SDL_Rect game_won_text_area{ 314, 50, 333, 55 };
	ObjectAndTexture menu_btn{
		Object(380, 220, 200, 100),
		Texture("../Images/MenuBtn.bmp", renderer)
	};

	while (true) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return STATE::QUIT;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (menu_btn.object.IsClicked(mouse_x, mouse_y)) return STATE::MENU;
			}
		}

		SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
		SDL_RenderClear(renderer);

		game_won_text.Render(renderer, nullptr, &game_won_text_area);
		menu_btn.object.Render(renderer, menu_btn.texture);

		SDL_RenderPresent(renderer);
	}
}

STATE Game::Level1() {
	Texture ground_tile_texture("../Images/GroundTile.bmp", renderer);
	Texture player_texture("../Images/Character.bmp", renderer);
	Texture coin_texture("../Images/Coin.bmp", renderer, 255, 255, 255);

	std::array<Object, 20> tiles;
	for (size_t i = 0; i < tiles.size(); ++i) {
		tiles[i].SetLocation(static_cast<int>(100 * i), WINDOW_HEIGHT - 100);
		tiles[i].SetSize(100, 100);
	}

	std::vector<Object> coins{
		Object(300, 360, 50, 50),
		Object(1000, 360, 50, 50),
		Object(1700, 360, 50, 50)
	};
	int coins_collected = 0;
	const int TOTAL_COINS = coins.size();

	Player player(600.0f, 1000.0f, 40, 220, 40, 80);

	Bounds level_bounds{ 0, 0, 100 * tiles.size(), WINDOW_HEIGHT };
	SDL_Rect camera{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	old_clock = clock();

	while (true) {
		// Update game variables
		new_clock = clock();
		delta_time = static_cast<float>(new_clock - old_clock) / CLOCKS_PER_SEC;
		old_clock = clock();

		// Event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return STATE::QUIT;
		}

		// Logic
		player.HandleMovement(delta_time, tiles, level_bounds);
		player.ApplyGravity(2500.0f, delta_time);
		CenterScreenOnObject(player, camera, level_bounds);

		/*
		Check if the player is colliding with any coins. If he is, delete the coin and 
		increase the amount of collected coins		
		*/
		for (auto& o : coins) {
			if (player.CollidesWith(o)) {
				o.Delete();
				++coins_collected;
			}
		}

		// Remove deleted coins from the vector
		std::vector<Object>::iterator iter;
		iter = std::remove_if(coins.begin(), coins.end(), [](const Object& o) -> bool {
			return o.IsDeleted();
			});
		if (iter != coins.end()) coins.erase(iter);

		// Go to game won screen when all coins are collected
		if (coins_collected == TOTAL_COINS) return STATE::GAME_WON;

		// Rendering
		SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
		SDL_RenderClear(renderer);

		player.Render(renderer, player_texture, camera);

		// Render only the tiles which are on the screen
		for (auto& t : tiles) {
			if (SDL_HasIntersection(&camera, &t.GetHitbox()))
				t.Render(renderer, ground_tile_texture, camera);
		}

		for (auto& o : coins)
			if (SDL_HasIntersection(&o.GetHitbox(), &camera))
				o.Render(renderer, coin_texture, camera);

		SDL_RenderPresent(renderer);
	}
}