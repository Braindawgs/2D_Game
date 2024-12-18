#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>


class Apples
{
    public:
        Apples(size_t count);
        ~Apples(){};
        
        /**
         * @brief Check if entity collided with apples.
         * 
         * @param entity Collidiong object. 
         * @return true If object colided.
         */
        bool checkCollision(SDL_Rect const& entity);

    	/**
    	 * @brief Apple count.
    	 * 
    	 * @return size_t count.
    	 */
        size_t appleCount();

        size_t m_count;
        std::vector<SDL_Rect> m_apples;

    private:

        void deleteApple(SDL_Rect& entity);
};