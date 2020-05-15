#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <memory>

#include "../TextureManager.h"

class Text
{
    std::string text;

    SDL_Renderer* renderer;
    SDL_Color color;
    SDL_Rect srcrect;
    SDL_Rect destrect;

    int font_size;

    std::shared_ptr<SDL_Texture> texture;

public:
    Text() = default;
    Text(SDL_Renderer* renderer, const char* text, int font_size, SDL_Color color);

    void setX_Y(int x, int y);
    void render();
};

#endif