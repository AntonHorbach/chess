#include "TextureManager.h"

std::map<std::string, std::shared_ptr<SDL_Texture>> TextureManager::textures;

std::shared_ptr<SDL_Texture> TextureManager::make_shared_texture(SDL_Texture* texture) {
    return {texture, SDL_DestroyTexture};
}

std::shared_ptr<SDL_Texture> TextureManager::loadTexture(SDL_Renderer* renderer,
                                                        const char* path_to_sprite)
{
    if(textures.find(path_to_sprite) != std::end(textures))
    {
        return textures[path_to_sprite];
    }

    SDL_Surface* surface = IMG_Load(path_to_sprite);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.insert({path_to_sprite, make_shared_texture(texture)});

    return textures[path_to_sprite];
}