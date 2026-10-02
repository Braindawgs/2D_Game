#pragma once

#include "Snek.hpp"
#include "Apples.hpp"
#include "Renderer.hpp"

enum class GameState
{
    PLAYING,
    GAME_OVER
};

/**
 * @brief Everything that is reset when game restarts.
 *
 */
struct GameRound
{
    /**
     * @brief Creates snake, places apples and loads their textures.
     *
     * @param windowW Window width.
     * @param windowH Window height.
     * @param appleCount Number of apples.
     * @param rd Renderer.
     */
    GameRound(int windowW, int windowH, size_t appleCount, Renderer& rd)
        : snek(windowW, windowH),
          apples(appleCount, windowW, windowH, snek.getSegmentSize(), snek.getOccupiedCells())
    {
        snek.populateTexture(rd);
        apples.populateTexture(rd);
    }

    Snek::Player snek;
    Apples apples;
    unsigned int score = 0;
    GameState state = GameState::PLAYING;
};
