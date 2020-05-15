#include "menu.h"

Menu::Menu()
{
}

Menu::Menu(int buttons_num)
{
    buttons.reserve(buttons_num);
}

void Menu::normalize()
{
    for(size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i].setWidthAndHeight(destrect.w - 30, destrect.h / buttons.size() - 30);
        buttons[i].setX_Y(destrect.x + 10, destrect.h / buttons.size() * i + destrect.y + 15);
    }
}

bool Menu::init(
        SDL_Renderer* renderer,
        int x,
        int y,
        int width,
        int height,
        const std::string& path_to_menu_sprite)
{
    if(tools::less_than({x, y, width, height}, 0))
    {
        return false;
    }

    this->renderer = renderer;

    if(!setMenuTexture(path_to_menu_sprite))
    {
        std::cout << "Cant't load menu texture: " << SDL_GetError() << '\n';
        return false;
    }

    srcrect.x = srcrect.y = 0;
    destrect.x = x;
    destrect.y = y;
    destrect.h = srcrect.h = height;
    destrect.w = srcrect.w = width;

    std::swap(this->buttons, buttons);

    return true;
}

bool Menu::setButtonsTextures(
        const std::string& buttonOn_sprite, const std::string& buttonOFF_sprite)
{
    auto buttonOn = TextureManager::loadTexture(renderer, buttonOn_sprite.c_str());
    auto buttonOff = TextureManager::loadTexture(renderer, buttonOFF_sprite.c_str());

    if(buttonOff == nullptr || buttonOn == nullptr)
    {
        return false;
    }

    for(Button& button : buttons)
    {
        if(!button.setTextures(buttonOn, buttonOff))
        {
            return false;
        }
    }

    return true;
}

bool Menu::setMenuTexture(const std::string& path_to_sprite)
{
    menu_texture = TextureManager::loadTexture(renderer, path_to_sprite.c_str());

    if(menu_texture == nullptr)
    {
        return false;
    }

    return true;
}

void Menu::pushButton(Button&& button)
{
    buttons.push_back(button);
    normalize();
}

void Menu::pushButtons(std::vector<Button>&& buttons)
{
    this->buttons += buttons;
    normalize();
}

void Menu::render()
{
    SDL_RenderCopy(renderer, menu_texture.get(), &srcrect, &destrect);

    for(Button& button : buttons)
    {
        button.render();
    }
}

void Menu::handleEvents(SDL_Event* event)
{
    for(Button& button : buttons)
    {
        if(event->button.x <= button.getX() + button.getWidth()
           && event->button.x >= button.getX()
           && event->button.y <= button.getY() + button.getHeight()
           && event->button.y >= button.getY())
        {
            button.handleEvents(event);
            return;
        }
    }
}