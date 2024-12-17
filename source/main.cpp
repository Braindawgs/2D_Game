#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>

#include "Movement.hpp"
#include "Snek.hpp"
#include "Apples.hpp"

// https://www.youtube.com/watch?v=3kw1-dOikMA&list=PLYmIsLVSssdIOn5J71CVBblPlXici1_2A&index=7

#define WINDOW_SIZE (1000u)
#define APPLE_COUNT (100u)

int main(int argc, char* argv[]) 
{
    SDL_Event evt;
    Snek::Player snek(WINDOW_SIZE, WINDOW_SIZE);
    Apples apples(APPLE_COUNT);

    bool running = true;

    SnakeMove::snakeDirection dir;
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

    auto window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                WINDOW_SIZE, WINDOW_SIZE, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);

    while(running)
    {
        while(SDL_PollEvent(&evt))
        {
            if (SDL_QUIT == evt.type)
            {
                running = false;
            }

            SnakeMove::movementSelecter(evt, dir);
        }

        SnakeMove::movementExec(snek.m_snekHead, dir);


        // Collision detection with apple
#if 0
        std::for_each(apples.begin(), apples.end(), [&](auto& apple)
        {
            if ((snekHead.x == apple.x) && (snekHead.y == apple.y))
            {
                size +=10;
                apple.x = -10;
                apple.y = -10;
            }
        });
#endif
        if (apples.CheckCollision(snek.m_snekHead))
        {
            snek.ChangeSize(5);
        }

        snek.CheckCollisionSelf();
        snek.UpdateBody();


        //TODO: Scoring module, just testing, remove 
        TTF_Font* font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 24);
        if (font == nullptr) 
        {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
        }
        SDL_Color textColor = {255, 255, 255, 255}; // White color
        std::string const cScore = "Score:";
        std::string score = std::to_string(apples.AppleCount());
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, (cScore+score).c_str(), textColor);
        
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface); // Free the surface since the texture is created

        SDL_Rect scoreRect{0, 0, 80, 40};
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);
        // Render the text
        SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);




        // Draw snake
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        std::for_each(snek.m_snekBody.begin(), snek.m_snekBody.end(), [&](auto& snekBody)
        {
            SDL_RenderFillRect(renderer, &snekBody);
        });

        //Draw apples
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        std::for_each(apples.m_apples.begin(), apples.m_apples.end(), [&](auto& apple)        
        {
            SDL_RenderFillRect(renderer, &apple);
        });

        SDL_RenderPresent(renderer);
        SDL_Delay(35);
    }

    // Cleanup and close
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

