#ifndef GAME_H
#define GAME_H

#include <ctime>

#include "SDL.h"

// Game state class. Used to indicate game states
enum class STATE { MENU, INSTRUCTIONS, GAME_WON, LEVEL1, QUIT };

class Game {
public:
	Game(const char* title, int x, int y, int width, int height, Uint32 flags);

	Game(const Game& g) = delete;
	Game(Game&& g) = delete;
	Game& operator=(const Game& g) = delete;
	Game& operator=(Game&& g) = delete;
	~Game();

	// Game loop
	void Run();

private:
	SDL_Window* window; // The game window
	SDL_Renderer* renderer; // Main renderer used for the game
	SDL_Event e; // Main event object used for event handling
	STATE state; // Game state
	int mouse_x, mouse_y; // Mouse coordinates
	clock_t old_clock, new_clock; // Clock variables used to track game time
	float delta_time; // Time passed since last frame

	// Menus and levels

	STATE MainMenu();
	STATE Instructions();
	STATE GameWon();
	STATE GameOver();
	STATE Level1();
};

#endif