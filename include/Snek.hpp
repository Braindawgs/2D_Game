#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <deque>

namespace Snek
{
    class Player
    {
    public:
        Player(int windowX, int windowY) : m_snekHead{windowX/2, windowY/2, m_snekW, m_snekH} {}
        ~Player(){}

        // TODO: temp public, will be moved to main.
        // Sizes
        const int m_snekW = 10;
        const int m_snekH = 10;
        // Snek parts
        SDL_Rect m_snekHead;
        std::deque<SDL_Rect> m_snekBody;

        /**
         * @brief Size functions.
         * 
         * @param size +/- size or size to be set to.
         */
        void ChangeSize(int size);
        void SetSizeTo(int size);

        /**
         * @brief Get the Size object.
         * 
         * @return size_t size of the snake.
         */
        size_t GetSize();

        void UpdateBody();
        void CheckCollisionSelf();
    private:

        int m_size = 1;
    };


}