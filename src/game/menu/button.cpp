#include "button.h"

Button::Button() {}

bool Button::init(SDL_Renderer* renderer, const char* text, int text_size, void(*action)(void))
{
    if(text_size < 0) return false;

    this->renderer = renderer;

    this->text = Text(renderer, text, text_size, {0, 0, 0, 0});
    this->action = action;

    srcrect.x = srcrect.y = 0;

    return true;
}

int Button::getX() const {
    return destrect.x;
}

int Button::getY() const {
    return destrect.y;
}

int Button::getHeight() const {
    return destrect.h;
}

int Button::getWidth() const {
    return destrect.w;
}

void Button::setX_Y(int x, int y) {
    destrect.x = x;
    destrect.y = y;

    text.setX_Y(x + 15, y + 15);
}

void Button::setWidthAndHeight(int width, int height) {
    srcrect.h = destrect.h = height;
    srcrect.w = destrect.w = width;
}

bool Button::setTextures(const std::shared_ptr<SDL_Texture>& buttonON,
                        const std::shared_ptr<SDL_Texture>& buttonOff)
{
    if(buttonON == nullptr || buttonOff == nullptr) return false;

    this->buttonON = buttonON;
    this->buttonOFF = buttonOff;

    return true;
}

void Button::render() {
    if(active) 
        SDL_RenderCopy(renderer, buttonON.get(), &srcrect, &destrect);
    else
        SDL_RenderCopy(renderer, buttonOFF.get(), &srcrect, &destrect);

    text.render();
}

void Button::handleEvents(SDL_Event* event) {
    switch(event->type) {
    case SDL_MOUSEBUTTONDOWN:
        active = true;
        break;
    case SDL_MOUSEBUTTONUP: {
        if(action) action();
        active = false;
        break;
    }
    case SDL_MOUSEMOTION:
        break;
    }
}