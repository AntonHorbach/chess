#include <SDL2/SDL.h>
#include <iostream>

class Game {
    SDL_Window* window;
    SDL_DisplayMode mode;
    SDL_Renderer* renderer;

    int init();

public:
    Game();
    ~Game();

    int exec();
};