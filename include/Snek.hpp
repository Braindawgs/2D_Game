#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <deque>
#include <vector>

#include "Renderer.hpp"
#include "Movement.hpp"
#include "Utils.hpp"

namespace Snek
{
    // Body texture index, named by direction snake entered segment and direction it left it.
    // Turns in opposite rotation use same texture, e.g. LEFT then DOWN also uses UP_RIGHT.
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
        // Direction snake was moving when it entered this segment.
        snakeDirection dir;
    };

    struct SnekHead
    {
        SDL_Rect snekHead;
        snakeDirection dir;
        spriteTexture texture;
    };

    struct SenkBody
    {
        std::deque<SnekSingleBody> snekBody;
        spriteTexture texture[5];
    };

    // Tail direction is taken from body at render time, see getSnekTailDirection().
    struct SnekTail
    {
        SDL_Rect snekTail;
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
         * @brief Get cells covered by snake (head, body and tail).
         *
         * @return std::vector<SDL_Rect> Occupied cells.
         */
        std::vector<SDL_Rect> getOccupiedCells();

        /**
         * @brief Get the Segment Size object.
         *
         * @return int Size of one snake segment, also size of one grid cell.
         */
        int getSegmentSize() const;

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

        /**
         * @brief Applies current direction and moves snake one step.
         *
         */
        void updateMovement();
        void setSpeed(int x, int y);
        void setAngle(double angl);
        double getAngle();

        void snekSetSize(unsigned int size);
        void snekChangeSize(int dsize);
        
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
         * @brief Get body texture for segment based on how snake moved through it.
         *
         * @param inDir Direction snake entered segment.
         * @param outDir Direction snake left segment.
         * @return snekCurveTexture Curve texture, NONE for straight segment.
         */
        static snekCurveTexture getSnekCurve(snakeDirection inDir, snakeDirection outDir);

        /**
         * @brief Get the Snek Tail direction.
         * Tail is drawn over last body segment, so it points where snake left that segment.
         *
         * @return snakeDirection Tail direction.
         */
        snakeDirection getSnekTailDirection() const;

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