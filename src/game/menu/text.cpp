#include "text.h"

Text::Text(SDL_Renderer* renderer, const char* text, int font_size, SDL_Color color)
    :renderer(renderer), text(text), font_size(font_size), color(color)
{
    texture = TextureManager::loadTextTexture(renderer, text, font_size, color);

    srcrect.x = srcrect.y = 0;
    srcrect.h = destrect.h = font_size + 10;
    destrect.w = srcrect.w = this->text.length() * 10;
}

void Text::setX_Y(int x, int y) {
    destrect.x = x;
    destrect.y = y;
}

void Text::render() {
    SDL_RenderCopy(renderer, texture.get(), &srcrect, &destrect);
}