#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>
#include <optional>

#include "Game.hpp"
#include "Renderer.hpp"
#include "Utils.hpp"

// https://www.youtube.com/watch?v=3kw1-dOikMA&list=PLYmIsLVSssdIOn5J71CVBblPlXici1_2A&index=7

#define WINDOW_SIZE (1000u)
#define APPLE_COUNT (100u)

/**
 * @brief Checks for first press of key, ignores repeats from holding key down.
 *
 * @param evt SDL event.
 * @param key Key to check.
 * @return true If key was just pressed.
 */
static bool isKeyPressed(SDL_Event const& evt, SDL_Keycode key)
{
    return (SDL_KEYDOWN == evt.type) && (0 == evt.key.repeat) && (key == evt.key.keysym.sym);
}

static TTF_Font* openFont(int size)
{
    TTF_Font* font = TTF_OpenFont(assetPath("assets/fonts/FreeSans.ttf").c_str(), size);
    if (font == nullptr)
    {
        std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
    }
    return font;
}

int main(int argc, char* argv[])
{
    SDL_Event evt;
    bool running = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw std::runtime_error("SDL Init failed\n");
    }
    // Initialize TTF
    if (TTF_Init() != 0)
    {
        throw std::runtime_error("TTF Init failed\n");
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        throw std::runtime_error("IMG Init failed\n");
    }

    // Scoped so Renderer and its textures are destroyed before SDL_Quit.
    {
        Renderer rd(WINDOW_SIZE, WINDOW_SIZE);

        auto backGroundTexture = rd.loadTexture("assets/background/background_whatever.png");

        TTF_Font* font = openFont(24);
        TTF_Font* titleFont = openFont(72);
        SDL_Color const white = {255, 255, 255, 255};

        // Optional so restart can construct new round in place, Player can't be reassigned.
        std::optional<GameRound> round;
        round.emplace(WINDOW_SIZE, WINDOW_SIZE, APPLE_COUNT, rd);

        // Key poller.
        while(running)
        {
            bool restart = false;

            while(SDL_PollEvent(&evt))
            {
                if (SDL_QUIT == evt.type)
                {
                    running = false;
                }
                else if (GameState::PLAYING == round->state)
                {
                    //TODO: No in game event ends the round yet, Esc ends it for now.
                    if (isKeyPressed(evt, SDLK_ESCAPE))
                    {
                        round->state = GameState::GAME_OVER;
                    }
                    else
                    {
                        round->snek.movementInput(evt);
                    }
                }
                else if (isKeyPressed(evt, SDLK_r) || isKeyPressed(evt, SDLK_RETURN))
                {
                    restart = true;
                }
                else if (isKeyPressed(evt, SDLK_ESCAPE))
                {
                    running = false;
                }
            }

            if (restart)
            {
                round.emplace(WINDOW_SIZE, WINDOW_SIZE, APPLE_COUNT, rd);
            }

            auto& snek = round->snek;
            auto& apples = round->apples;

            if (GameState::PLAYING == round->state)
            {
                snek.updateMovement();

                if (apples.checkAppleCollision(snek.getSnekHead()))
                {
                    snek.snekChangeSize(5);
                    round->score++;
                }

                snek.checkCollisionSelf();
            }

            std::string const score = "Score: " + std::to_string(round->score);

            // Clear screen
            rd.clear();

            if (nullptr != backGroundTexture)
            {
                rd.renderFitWindow(*backGroundTexture);
            }

            // Draw snake
            snek.renderSnake(rd);

            // Draw apples
            //TODO: Apples rendering, move it to apple class
            std::for_each(apples.m_apples.begin(), apples.m_apples.end(), [&](auto& apple)
            {
                rd.renderFromSprite(apple.color.texture, apple.color.spriteX, apple.color.spriteY,
                                    apple.color.spriteW, apple.color.spriteH,
                                    apple.rect.x, apple.rect.y, apple.rect.w, apple.rect.w);
            });

            // Draw scoreboard, after apples so they don't cover it
            rd.render(0, 0, score, font, white);

            if (GameState::GAME_OVER == round->state)
            {
                int const center = WINDOW_SIZE / 2;
                SDL_Rect dim = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
                rd.renderPlainRect(dim, {0, 0, 0, 170});
                rd.renderTextCentered(center, center - 120, "Game over", titleFont, white);
                rd.renderTextCentered(center, center, score, font, white);
                rd.renderTextCentered(center, center + 50, "Press R to restart, Esc to quit", font, white);
            }

            rd.display();
            SDL_Delay(35);
        }

        // Cleanup and close
        if (nullptr != titleFont)
        {
            TTF_CloseFont(titleFont);
        }
        if (nullptr != font)
        {
            TTF_CloseFont(font);
        }
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
