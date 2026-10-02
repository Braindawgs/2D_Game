#pragma once

#include <SDL2/SDL.h>
#include <cstddef>

/**
 * @brief How often and how many objects spawn, e.g. 5 apples every 5 seconds, at most 150 on board.
 *
 */
struct SpawnRule
{
    Uint32 intervalMs;
    size_t count;
    // Maximum objects on board at once, 0 means no cap.
    size_t maxActive;
};

/**
 * @brief Tracks time for one spawn rule and tells when and how many objects to spawn.
 * Doesn't know what it spawns, same class can be used for apples, bombs, etc.
 *
 */
class Spawner
{
    public:
        explicit Spawner(SpawnRule const& rule);

        /**
         * @brief Advances spawn timer.
         * Only call while game is running, time that isn't passed in doesn't count (pause, game over).
         *
         * @param deltaMs Time since last update.
         * @param activeCount Objects currently on board, used for cap.
         * @return size_t Number of objects to spawn now, 0 if interval hasn't passed yet or cap is reached.
         */
        size_t update(Uint32 deltaMs, size_t activeCount);

        /**
         * @brief Restarts interval from zero.
         *
         */
        void reset();

        SpawnRule const& getRule() const;

    private:
        SpawnRule m_rule;
        Uint32 m_elapsedMs = 0;
};
