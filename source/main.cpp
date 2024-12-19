#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>

#include "Snek.hpp"
#include "Apples.hpp"
#include "Renderer.hpp"

// https://www.youtube.com/watch?v=3kw1-dOikMA&list=PLYmIsLVSssdIOn5J71CVBblPlXici1_2A&index=7

#define WINDOW_SIZE (1000u)
#define APPLE_COUNT (100u)

int main(int argc, char* argv[]) 
{
    SDL_Event evt;
    Snek::Player snek(WINDOW_SIZE, WINDOW_SIZE);
    Apples apples(APPLE_COUNT);

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

    Renderer rd(WINDOW_SIZE, WINDOW_SIZE);

    auto backGroundTexture = rd.loadTexture("assets/background/background_whatever.png");
    //auto appleTexture = rd.loadTexture("assets/textures/apples.png");
    apples.populateTexture(rd);
    while(running)
    {
        while(SDL_PollEvent(&evt))
        {
            if (SDL_QUIT == evt.type)
            {
                running = false;
            }

            snek.movementInput(evt);
        }

        snek.updatePosition();
        snek.updateMovement();

        if (apples.checkCollision(snek.getSnekHead()))
        {
            snek.snekChangeSize(5);
        }

        snek.checkCollisionSelf();

    
        //TODO: Scoring module, just testing, remove 
        TTF_Font* font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 24);
        if (font == nullptr) 
        {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
        }
        std::string const cScore = "Score:";
        std::string score = std::to_string(apples.appleCount());


        // Clear screen
        rd.clear();

        rd.renderFitWindow(*backGroundTexture);
        // Draw scoreboard
        rd.render(0, 0, (cScore+score), font, {255, 255, 255, 255});

        // Draw snek
        rd.renderPlainRect(snek.getSnekHead(), {0, 255, 0, 255});
        rd.renderPlainRectArray(snek.getSnekBody(), {50, 200, 50, 255}, true);
        rd.renderPlainRect(snek.getSnekTail(), {255, 255, 0, 255});
        // Draw apples
        std::for_each(apples.m_apples.begin(), apples.m_apples.end(), [&](auto& apple)
        {
            rd.renderFromSprite(apple.color.texture, apple.color.spriteX, apple.color.spriteY, 
                                apple.color.spriteW, apple.color.spriteH,
                                apple.rect.x, apple.rect.y, apple.rect.w, apple.rect.w);
        });

        // Apple textute move to somewhere
        //rd.renderFromSprite(appleTexture, 17, 125, 73, 94, 100, 100, 10, 10);
       
        rd.display();
        SDL_Delay(35);
    }

    // Cleanup and close
    SDL_Quit();

    return 0;
}

