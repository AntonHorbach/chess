#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <memory>
#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class TextureManager {
private:
    static std::map<std::string, std::shared_ptr<SDL_Texture>> textures;

    static std::shared_ptr<SDL_Texture> make_shared_texture(SDL_Texture* texture);

public:
    static std::shared_ptr<SDL_Texture> loadTexture(SDL_Renderer* renderer, const std::string& path_to_sprite);
    static std::shared_ptr<SDL_Texture> loadTextTexture(SDL_Renderer* renderer, const std::string& text,
                                                        size_t font_size, SDL_Color color);
};

#endif