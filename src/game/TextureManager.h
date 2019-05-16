#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path_to_sprite);
};

#endif