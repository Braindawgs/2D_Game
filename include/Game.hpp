#pragma once

#include "Snek.hpp"
#include "Apples.hpp"
#include "Bombs.hpp"
#include "Renderer.hpp"
#include "Spawner.hpp"

enum class GameState
{
    PLAYING,
    GAME_OVER
};

/**
 * @brief All gameplay tuning in one place.
 *
 */
struct GameConfig
{
    int windowW;
    int windowH;

    size_t appleCount;
    // Segments snake grows by eating an apple.
    size_t appleGrowth;
    SpawnRule appleSpawnRule;

    SpawnRule bombSpawnRule;
    BombType bombType;
    // Segments snake loses by eating a bomb.
    size_t bombEatPenalty;
    // Segments snake loses if any part of it is in blast radius.
    size_t bombBlastPenalty;
};

/**
 * @brief Everything that is reset when game restarts.
 *
 */
struct GameRound
{
    /**
     * @brief Creates snake, places apples and loads textures.
     *
     * @param config Gameplay settings.
     * @param rd Renderer.
     */
    GameRound(GameConfig const& config, Renderer& rd);

    /**
     * @brief Advances game by one step: movement, eating, bombs and spawning.
     * Call only while playing. Sets state to GAME_OVER when snake bites itself or a bomb leaves it without body.
     *
     * @param deltaMs Time since last update.
     */
    void update(Uint32 deltaMs);

    GameConfig config;
    Snek::Player snek;
    Apples apples;
    Bombs bombs;
    Spawner appleSpawner;
    Spawner bombSpawner;
    unsigned int score = 0;
    GameState state = GameState::PLAYING;

    private:
        void explode(Blast const& blast);

        /**
         * @brief Removes segments from snake, ends game if none are left.
         * 
         * @param count Number of segments to remove.
         */
        void loseSegments(size_t count);
};
