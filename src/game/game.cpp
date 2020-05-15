#include "game.h"

Game::Game(): window(nullptr), renderer(nullptr), white_player(WHITE), black_player(BLACK)
{
}

bool Game::init(const char* title, int x, int y, int width, int height, bool fullscreen)
{
    Uint32 flags = fullscreen ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN) : SDL_WINDOW_SHOWN;

    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Bad sdl init: " << SDL_GetError() << '\n';
        return false;
    }

    if(TTF_Init() != 0)
    {
        std::cout << "Bad ttf init: " << TTF_GetError() << '\n';
        return false;
    }

    window = SDL_CreateWindow(title, x, y, width, height, flags);
    if(window == nullptr)
    {
        std::cout << "The window has not been created: " << SDL_GetError() << '\n';
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr)
    {
        std::cout << "Renderer has not been created: " << SDL_GetError() << '\n';
        return false;
    }

    if(!field.init(renderer) || !white_player.init(renderer) || !black_player.init(renderer))
    {
        return false;
    }

    if(!initMenu())
    {
        return false;
    }

    isRunning = true;
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);

    return true;
}

bool Game::initMenu()
{
    if(!menu.init(renderer, 480, 0, 160, 480, "./assets/textures/menu.png"))
    {
        return false;
    }

    Button button;

    button.init(this, renderer, "New game", 18, &Game::newGame);
    menu.pushButton(std::move(button));

    button.init(this, renderer, "Surrender", 18, &Game::surrender);
    menu.pushButton(std::move(button));

    button.init(this, renderer, "Back", 18, &Game::back);
    menu.pushButton(std::move(button));

    button.init(this, renderer, "Exit", 18, &Game::exit);
    menu.pushButton(std::move(button));

    menu.setButtonsTextures("./assets/textures/buttonON.png", "./assets/textures/button.png");

    return true;
}

void Game::newGame()
{
    halfMove = 0;
    current_player = &white_player;
    another_player = &black_player;

    field.resetSquares();
    white_player.reset();
    black_player.reset();
}

void Game::surrender()
{
    std::string player(*current_player);

    std::cout << player + " resigned\n";

    current_player->setStatus(STATUS::CHECKMATE);
}

void Game::back()
{
    if(halfMove == 0)
    {
        return;
    }
    else if(halfMove < 2)
    {
        white_player.rollback();
        --halfMove;
    }
    else
    {
        another_player->rollback();
        current_player->rollback();
        halfMove -= 2;
    }
}

void Game::exit()
{
    isRunning = false;
}

bool Game::checkmate()
{
    const Figure* enemy_king = another_player->getFigure(FT::KING);

    if(enemy_king->isDead())
    {
        return true;
    }

    if(enemy_king->getAvailableMoves().size() + enemy_king->getAvailableAttacks().size() != 0)
    {
        return false;
    }

    std::vector<std::reference_wrapper<const Figure>> direct_threats;

    for(const auto& figure : current_player->getFigures())
    {
        std::vector<MOVE> actions = figure.getAvailableAttacks() + figure.getAvailableMoves();
        for(const auto&[dx, dy] : actions)
        {
            if(figure.getX() + dx == enemy_king->getX() && figure.getY() + dy == enemy_king->getY())
            {
                direct_threats.push_back(std::reference_wrapper<const Figure>(figure));
                break;
            }
        }
    }

    if(direct_threats.empty())
    {
        return false;
    }
    else if(direct_threats.size() > 1)
    {
        return true;
    }

    for(const Figure& threat : direct_threats)
    {
        for(const Figure& figure : another_player->getFigures())
        {
            if(figure.isDead())
            {
                continue;
            }

            std::vector<POS>
                    attacks = figure.getAvailableAttacks() + POS{figure.getX(), figure.getY()};

            if(std::find(std::begin(attacks), std::end(attacks), POS{threat.getX(), threat.getY()})
               != std::end(attacks))
            {
                return false;
            }

            if(threat.getType() == FT::LEFT_HORSE || threat.getType() == FT::RIGHT_HORSE)
            {
                continue;
            }

            if(tools::findMatches(threat.getX(),
                                  threat.getY(),
                                  enemy_king->getX(),
                                  enemy_king->getY(),
                                  figure.getAvailableMoves() + POS{figure.getX(), figure.getY()})
                       .size() != 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Game::pat()
{
    for(const auto& figure : current_player->getFigures())
    {
        if(!figure.isDead() && figure.getType() != FT::KING)
        {
            return false;
        }
    }

    if(current_player->getFigure(FT::KING)->getAvailableMoves().empty()
       && current_player->getFigure(FT::KING)->getAvailableAttacks().empty())
    {
        return true;
    }

    return false;
}

void Game::handleFieldEvent(SDL_Event* event)
{
    switch(event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        if(current_player->handleEvents(event))
        {
            current_player->update(another_player);

            field.changeSquares(event->button.x / (480 / 8),
                                event->button.y / (480 / 8),
                                current_player->getFigure(event->button.x, event->button.y)
                                              ->getAvailableMoves());

            field.changeSquares(event->button.x / (480 / 8),
                                event->button.y / (480 / 8),
                                current_player->getFigure(event->button.x, event->button.y)
                                              ->getAvailableAttacks(),
                                SQUARE::ATTACK_SQUARE);
        }
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        if(current_player->handleEvents(event))
        {
            if(current_player->isAttack()
               && another_player->killIfFind(event->button.x, event->button.y))
            {
                ++halfMove;
            }
            else if(!current_player->isMove())
            {
                current_player->rollback();
            }
            else
            {
                ++halfMove;
            }

            current_player->update(another_player);
            another_player->update(current_player);
        }

        field.resetSquares();
        break;
    }
    case SDL_MOUSEMOTION:
    {
        current_player->handleEvents(event);
        break;
    }
    default:
        break;
    }
}

void Game::handleEvents()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if(event.button.x >= 0 && event.button.x < 480)
        {
            if(white_player.getStatus() == STATUS::NONE && black_player.getStatus() == STATUS::NONE)
            {
                handleFieldEvent(&event);
            }
        }
        else
        {
            menu.handleEvents(&event);
        }
    }
}

void Game::update()
{
    if(white_player.getStatus() != STATUS::NONE || black_player.getStatus() != STATUS::NONE)
    {
        return;
    }
    else if(checkmate())
    {
        std::string message(*current_player);
        std::cout << message + " did checkmate.\n";

        another_player->setStatus(STATUS::CHECKMATE);
    }
    else
    {
        current_player = (halfMove % 2 == 0 ? &white_player : &black_player);
        another_player = (current_player == &white_player ? &black_player : &white_player);

        if(pat())
        {
            std::string message(*current_player);
            std::cout << "It's pat for " + message << '\n';

            current_player->setStatus(STATUS::PAT);
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);

    field.render(renderer);
    white_player.render();
    black_player.render();
    menu.render();

    SDL_RenderPresent(renderer);
}

int Game::exec()
{
    const int fps = 30;
    const int ms_for_frame = 1000 / fps;

    while(isRunning)
    {
        handleEvents();
        update();
        render();

        SDL_Delay(ms_for_frame);
    }

    return 0;
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
