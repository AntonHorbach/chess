#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <iostream>

#include "../TextureManager.h"
#include "text.h"

class Button {
    std::shared_ptr<SDL_Texture> buttonOFF;
    std::shared_ptr<SDL_Texture> buttonON;
    Text text;

    SDL_Renderer* renderer;

    SDL_Rect srcrect;
    SDL_Rect destrect;

    void(*action)(void);
    bool active = false;

public:
    Button();

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;

    bool init(SDL_Renderer* renderer, const char* text, int text_size,
                void(*action)(void));
    void setX_Y(int x, int y);
    void setWidthAndHeight(int width, int height);
    bool setTextures(const std::shared_ptr<SDL_Texture>& buttonON,
                    const std::shared_ptr<SDL_Texture>& button_off);

    void render();
    void handleEvents(SDL_Event* event);
};


#endif