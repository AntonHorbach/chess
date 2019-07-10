#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <memory>

#include "../TextureManager.h"
#include "../../tools/tools.h"
#include "button.h"

class Menu {
    int dx, dy;
    SDL_Rect srcrect;
    SDL_Rect destrect;
    SDL_Renderer* renderer;
    std::shared_ptr<SDL_Texture> menu_texture;

    std::vector<Button> buttons;

    void normalize();

public:
    Menu();
    Menu(int buttons_num);

    bool init(SDL_Renderer* renderer, int x, int y, int width, int height,
                const std::string& path_to_menu_sprite);

    bool setButtonsTextures(const std::string& buttonOn_sprite, const std::string& buttonOFF_sprite);
    bool setMenuTexture(const std::string& path_to_sprite);

    void pushButton(Button&& button);
    void pushButtons(std::vector<Button>&& buttons);

    void render();
    void handleEvents(SDL_Event* event);
};

#endif