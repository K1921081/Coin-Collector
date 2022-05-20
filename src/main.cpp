#include <iostream>

#include "SDL.h"

#include "Game.h"
#include "Constants.h"

int main(int argc, char** args)
{
    try {
        Game game("Zombie outbreak", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        game.Run();
    }
    catch (std::invalid_argument& ia) {
        std::cerr << "Invalid argument passed to function\n";
        std::cerr << ia.what() << '\n';
    }
    catch (std::runtime_error& r) {
        std::cerr << "Runtime error caught: " << r.what() << '\n';
        std::cerr << "Error: " << SDL_GetError() << '\n';
    }
    catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        std::cerr << "Error: " << SDL_GetError() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown error occured\n";
    }

    return 0;
}