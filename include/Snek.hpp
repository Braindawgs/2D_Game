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
        Player(int windowX, int windowY) : m_snekHead{windowX/2, windowY/2, m_snekW, m_snekH}, 
                                           m_snekTail{windowX/2, windowY/2+m_snekH, m_snekW, m_snekH} {}
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
         * @brief Get the Snek Tail object
         * 
         * @return SDL_Rect& Reference to snake tail.
         */
        SDL_Rect& getSnekTail();

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

        void updateMovement();
        void setSpeed(int x, int y);

        void snekSetSize(unsigned int size);
        void snekChangeSize(int dsize);
        
        void updatePosition();
        void checkCollisionSelf();
    private:

        // Sizes
        const int m_snekW = 10;
        const int m_snekH = 10;
        const int segmentSize = 10;

        // Snek parts
        SDL_Rect m_snekHead;
        SDL_Rect m_snekTail;
        std::deque<SDL_Rect> m_snekBody;

        void growBody();
        void shrinkBody();
        void movementExec(snakeDirection const& dir);

        // Snek velocity.
        int m_speedX = 0;
        int m_speedY = 0;

        int m_size = 1;
        snakeDirection m_dir = NONE;
    };
}