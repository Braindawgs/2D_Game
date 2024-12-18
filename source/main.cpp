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

    Renderer rd(WINDOW_SIZE, WINDOW_SIZE);
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

        if (apples.checkCollision(snek.m_snekHead))
        {
            snek.changeSize(5);
        }

        snek.checkCollisionSelf();
        snek.updateBody();

    
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
        // Draw scoreboard
        rd.render(0, 0, (cScore+score), font, {255, 255, 255, 255});
        // Draw snek
        rd.renderPlainRectArray(snek.m_snekBody, {0, 200, 100, 255}, true);
        // Draw apples
        rd.renderPlainRectArray(apples.m_apples, {255, 0, 0, 255}, true);
       
        rd.display();
        SDL_Delay(35);
    }

    // Cleanup and close
    SDL_Quit();

    return 0;
}

