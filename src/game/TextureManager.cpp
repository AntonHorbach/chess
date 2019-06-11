#include "TextureManager.h"

std::shared_ptr<SDL_Texture> TextureManager::make_shared_texture(SDL_Texture* texture) {
    return {texture, SDL_DestroyTexture};
}

std::shared_ptr<SDL_Texture> TextureManager::loadTexture(SDL_Renderer* renderer,
                                                        const char* path_to_sprite)
{
    SDL_Surface* surface = IMG_Load(path_to_sprite);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return make_shared_texture(texture);
}