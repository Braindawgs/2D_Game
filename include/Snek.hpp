#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <deque>

#include "Movement.hpp"

namespace Snek
{
    class Player
    {
    public:
        Player(int windowX, int windowY) : m_snekHead{windowX/2, windowY/2, m_snekW, m_snekH} {}
        ~Player(){}

        /**
         * @brief Size functions.
         * 
         * @param size +/- size or size to be set to.
         */
        void changeSize(int size);
        void setSizeTo(int size);

        /**
         * @brief Get the Snek Head object.
         * 
         * @return SDL_Rect& Reference to snake head.
         */
        SDL_Rect& getSnekHead();

        /**
         * @brief Get the Snek Body object.
         * 
         * @return std::deque<SDL_Rect>& Reference to snake body.
         */
        std::deque<SDL_Rect>& getSnekBody();

        /**
         * @brief Get the Size object.
         * 
         * @return size_t size of the snake.
         */
        size_t getSize();


        /**
         * @brief Reads movement input.
         * 
         * @param evt SDL event.
         */
        void movementInput(SDL_Event& evt);
        
        void updateBody();
        void updatePosition();
        void checkCollisionSelf();
    private:

        // Sizes
        const int m_snekW = 10;
        const int m_snekH = 10;
        // Snek parts
        SDL_Rect m_snekHead;

        //TODO: change container maybe?
        std::deque<SDL_Rect> m_snekBody;

        int m_size = 1;
        snakeDirection m_dir = NONE;
    };
}