#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const char* path_to_sprite) {
    SDL_Surface* surface = IMG_Load(path_to_sprite);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}