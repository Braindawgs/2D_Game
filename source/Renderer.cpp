#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>
#include "Renderer.hpp"

Renderer::Renderer(int windowW, int windowH)
{
    // TODO: Add resiable flag, maybe baby.
    m_window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                windowW, windowH, 0);

    if (nullptr == m_window)
    {
        throw std::runtime_error("Failed to init window, crit error\n");
    }

    m_rndr = SDL_CreateRenderer(m_window, -1, 0);

    if (nullptr == m_window)
    {
        throw std::runtime_error("Failed to init renderer, crit error\n");
    }
}

Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
}


#if 0 // Init stuff in constructor and destroy it in dest
void Renderer::Create(int windowW, int windowH)
{
    // TODO: Add resiable flag, maybe baby.
    m_window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                windowW, windowH, 0);

    if (nullptr == m_window)
    {
        throw std::runtime_error("Failed to init window, crit error\n");
    }

    m_rndr = SDL_CreateRenderer(m_window, -1, 0);

    if (nullptr == m_window)
    {
        throw std::runtime_error("Failed to init renderer, crit error\n");
    }
}
#endif

void Renderer::clear()
{
    // Set color to black
    SDL_SetRenderDrawColor(m_rndr, 0, 0, 0, 255);
    // Clear renderer
    SDL_RenderClear(m_rndr);
}

SDL_Texture* Renderer::loadTexture(std::string const& path)
{
    auto texture = IMG_LoadTexture(m_rndr, path.c_str());

    if (nullptr == texture)
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    return texture;
}

void Renderer::render(int posX, int posY, SDL_Texture& texture)
{
    SDL_Rect source = {0, 0, 0, 0};
    SDL_QueryTexture(&texture, NULL, NULL, &source.w, &source.h);

    SDL_Rect dest{posX, posY, source.w, source.h};
    SDL_RenderCopy(m_rndr, &texture, &source, &dest);
}

void Renderer::render(SDL_Texture& texture)
{
    render(0, 0, texture);
}

void Renderer::render(int posX, int posY, int sizeW, int sizeH, SDL_Texture& texture)
{
    SDL_Rect dest{posX, posY, sizeW, sizeH};
    SDL_RenderCopy(m_rndr, &texture, nullptr, &dest);
}
 
void Renderer::render(int posX, int posY, std::string const& txt, TTF_Font* font, SDL_Color const& textColor)
{
    auto textSurface = TTF_RenderText_Blended(font, txt.c_str(), textColor);
    auto textTexture = SDL_CreateTextureFromSurface(m_rndr, textSurface);
    SDL_FreeSurface(textSurface); // Free the surface since the texture is created

    SDL_Rect source = {0, 0, textSurface->w, textSurface->h};
    SDL_Rect dest = {posX, posY, source.w, source.h};

    SDL_RenderCopy(m_rndr, textTexture, &source, &dest);
}

void Renderer::renderFromSprite(SDL_Texture* spriteSheet, int srcX, int srcY, int srcW, int srcH, 
                                int destX, int destY, int destW, int destH)
{
    SDL_Rect srcRect = {srcX, srcY, srcW, srcH};

    // Destination rectangle (where on the screen to render it)
    SDL_Rect destRect = {destX, destY, destW, destH};

    // Render the selected portion of the sprite sheet
    SDL_RenderCopy(m_rndr, spriteSheet, &srcRect, &destRect);
}

void Renderer::renderFitWindow(SDL_Texture& texture)
{
    SDL_RenderCopy(m_rndr, &texture, NULL, NULL);
}

void Renderer::renderPlainRect(SDL_Rect& rect, SDL_Color const& colorscheme)
{
    SDL_SetRenderDrawColor(m_rndr, colorscheme.r, colorscheme.g, colorscheme.b, colorscheme.a);
    SDL_RenderFillRect(m_rndr, &rect);
}

void Renderer::display()
{
    SDL_RenderPresent(m_rndr);
}