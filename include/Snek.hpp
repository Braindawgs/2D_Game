#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <deque>

#include "Renderer.hpp"
#include "Movement.hpp"
#include "Utils.hpp"

namespace Snek
{
    enum snekCurveTexture
    {
        NONE = 0,
        DOWN_LEFT = 1,
        UP_LEFT = 2,
        UP_RIGHT = 3,
        DOWN_RIGHT = 4
    };

    struct SnekSingleBody
    {
        SDL_Rect snekSingleBodyPart;
        double angle;
    };

    struct SnekHead
    {
        SDL_Rect snekHead;
        double angle;
        spriteTexture texture;
    };

    struct SenkBody
    {
        std::deque<SnekSingleBody> snekBody;
        spriteTexture texture[5];
    };

    struct SnekTail
    {
        SDL_Rect snekTail;
        double angle;
        spriteTexture texture;
    };

    struct SnekParts
    {
        SnekHead snekHead;
        SenkBody snekBody;
        SnekTail snekTail;
    };

    class Player
    {
    public:
        Player(int windowX, int windowY);
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
         * @brief Get the Size object.
         * 
         * @return size_t size of the snake.
         */
        size_t getSize();

        /**
         * @brief Loads snake textures. 
         * 
         * @param rd Renderer.
         */
        void populateTexture(Renderer& rd);
        
        /**
         * @brief Reads movement input.
         * 
         * @param evt SDL event.
         */
        void movementInput(SDL_Event& evt);

        void updateMovement();
        void setSpeed(int x, int y);
        void setAngle(double angl);
        double getAngle();

        snekCurveTexture getSnekCurve(int anglePrev);

        void snekSetSize(unsigned int size);
        void snekChangeSize(int dsize);
        
        void updatePosition();
        void checkCollisionSelf();

        //TODO: Deal with this
        void renderSnake(Renderer& rd);
    private:

        // Sizes
        const int m_snekW = 10;
        const int m_snekH = 10;
        const int segmentSize = 10;

        const char* snakeTexture = "assets/textures/snake.png";
        // Snek parts
        SnekParts m_sparts;

        void growBody();
        void shrinkBody();
        void movementExec(snakeDirection const& dir);

        /**
         * @brief Get the Snek Parts textures.
         * 
         * @return spriteTexture 
         */
        spriteTexture getSnekHeadTexture() const;
        spriteTexture* getSnekBodyTexture();
        spriteTexture getSnekTailTexture() const;
        
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
        std::deque<SnekSingleBody>& getSnekBody();


        // Snek velocity.
        int m_speedX = 0;
        int m_speedY = 0;
        double m_angle = 0;

        int m_size = 1;
        snakeDirection m_dir = snakeDirection::NONE;
    };
}