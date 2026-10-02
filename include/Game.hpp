#pragma once

#include "Snek.hpp"
#include "Apples.hpp"
#include "Renderer.hpp"
#include "Spawner.hpp"

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
     * @param appleCount Number of apples at start.
     * @param appleSpawnRule How often and how many apples spawn during game.
     * @param rd Renderer.
     */
    GameRound(int windowW, int windowH, size_t appleCount, SpawnRule const& appleSpawnRule, Renderer& rd)
        : snek(windowW, windowH),
          apples(appleCount, windowW, windowH, snek.getSegmentSize(), snek.getOccupiedCells()),
          appleSpawner(appleSpawnRule)
    {
        snek.populateTexture(rd);
        apples.populateTexture(rd);
    }

    Snek::Player snek;
    Apples apples;
    Spawner appleSpawner;
    unsigned int score = 0;
    GameState state = GameState::PLAYING;
};
