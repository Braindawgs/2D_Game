#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "Animation.hpp"
#include "Grid.hpp"
#include "Renderer.hpp"

/**
 * @brief Bomb settings given per spawn, so different bombs can tick faster/slower or blast bigger.
 *
 */
struct BombType
{
    Uint32 fuseMs;
    // Blast radius in cells, 0 is only bomb cell.
    int blastRadius;
};

/**
 * @brief Bomb that exploded, game decides what it does to snake and apples.
 *
 */
struct Blast
{
    SDL_Rect center;
    int radiusCells;
};

class Bombs
{
    struct BombData
    {
        SDL_Rect rect;
        BombType type;
        Uint32 remainingMs;
        Animation spark;
    };

    struct ExplosionData
    {
        Blast blast;
        Animation animation;
    };

    public:
        /**
         * @brief Construct a new Bombs object.
         *
         * @param windowW Window width.
         * @param windowH Window height.
         * @param cellSize Grid cell size, same as snake segment size.
         */
        Bombs(int windowW, int windowH, int cellSize);

        /**
         * @brief Loads bomb texture.
         *
         * @param rd Renderer.
         */
        void populateTexture(Renderer& rd);

        /**
         * @brief Places new bombs on free grid cells.
         *
         * @param count Number of bombs to add.
         * @param type Fuse and blast radius of new bombs.
         * @param blocked Cells bombs must not be placed on, e.g. snake and apples.
         * @return size_t Number of bombs placed, less than count if grid is full.
         */
        size_t spawn(size_t count, BombType const& type, std::vector<SDL_Rect> const& blocked);

        /**
         * @brief Check if entity hit a bomb, hit bomb is removed without exploding.
         *
         * @param entity Colliding object.
         * @return true If entity hit a bomb.
         */
        bool checkBombCollision(SDL_Rect const& entity);

        /**
         * @brief Advances fuses and explosion animations.
         *
         * @param deltaMs Time since last update.
         * @return std::vector<Blast> Bombs that exploded during this update.
         */
        std::vector<Blast> update(Uint32 deltaMs);

        /**
         * @brief Check if cell is hit by blast.
         *
         * @param blast Blast.
         * @param cell Cell to check.
         * @return true If cell is within blast radius.
         */
        bool isInBlast(Blast const& blast, SDL_Rect const& cell) const;

        /**
         * @brief Renders ticking bombs and explosions.
         *
         * @param rd Renderer.
         */
        void render(Renderer& rd);

        /**
         * @brief Bomb count, explosions are not counted.
         *
         * @return size_t count.
         */
        size_t bombCount() const;

        /**
         * @brief Get cells covered by bombs.
         *
         * @return std::vector<SDL_Rect> Occupied cells.
         */
        std::vector<SDL_Rect> getOccupiedCells() const;

    private:
        // Sprite sheet made by tools/gen_bomb_sprite.py, keep in sync with it.
        const char* bombTexturePath = "assets/textures/bomb.png";
        static constexpr int cFrameSize = 80;
        static constexpr int cTickRow = 0;
        static constexpr int cExplosionRow = 1;
        // Tick row: 2 spark frames normal, then same 2 frames red hot.
        static constexpr size_t cSparkFrames = 2;
        static constexpr size_t cHotFrameOffset = 2;
        static constexpr size_t cExplosionFrames = 4;

        static constexpr Uint32 cSparkFrameMs = 150;
        static constexpr Uint32 cExplosionFrameMs = 90;
        // Bomb blinks red near end of fuse, faster in last moments.
        static constexpr Uint32 cWarningMs = 1500;
        static constexpr Uint32 cWarningBlinkMs = 200;
        static constexpr Uint32 cFinalWarningMs = 600;
        static constexpr Uint32 cFinalWarningBlinkMs = 80;

        Grid m_grid;
        SDL_Texture* m_texture = nullptr;
        std::vector<BombData> m_bombs;
        std::vector<ExplosionData> m_explosions;

        bool isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const;

        /**
         * @brief Check if bomb should show red hot frame now.
         *
         * @param remainingMs Time left on fuse.
         * @return true If red hot frame should be shown.
         */
        static bool isHotFrame(Uint32 remainingMs);
};
