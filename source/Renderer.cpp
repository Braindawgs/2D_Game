#include "Renderer.hpp"
#include <stdexcept>

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

void Renderer::Clear()
{
    // Set color to black
    SDL_SetRenderDrawColor(m_rndr, 0, 0, 0, 255);
    // Clear renderer
    SDL_RenderClear(m_rndr);
}

void Renderer::Render(int posX, int posY, SDL_Texture& texture)
{
    SDL_Rect source = {0, 0};
    SDL_QueryTexture(&texture, NULL, NULL, &source.x, &source.y);

    SDL_Rect dest{posX, posY, source.w, source.h};
    SDL_RenderCopy(m_rndr, &texture, &source, &dest);
}

void Renderer::Render(SDL_Texture& texture)
{
    Render(0, 0, texture);
}

void Renderer::Render(int posX, int posY, std::string const& txt, TTF_Font* font, SDL_Color const& textColor)
{
    auto textSurface = TTF_RenderText_Blended(font, txt.c_str(), textColor);
    auto textTexture = SDL_CreateTextureFromSurface(m_rndr, textSurface);
    SDL_FreeSurface(textSurface); // Free the surface since the texture is created

    SDL_Rect source = {0, 0, textSurface->w, textSurface->h};
    SDL_Rect dest = {posX, posY, source.w, source.h};

    SDL_RenderCopy(m_rndr, textTexture, &source, &dest);
}

void Renderer::RenderPlainRect(SDL_Rect& rect, SDL_Color const& colorscheme)
{
    SDL_SetRenderDrawColor(m_rndr, colorscheme.r, colorscheme.g, colorscheme.b, colorscheme.a);
    SDL_RenderFillRect(m_rndr, &rect);
}

void Renderer::Display()
{
    SDL_RenderPresent(m_rndr);
}