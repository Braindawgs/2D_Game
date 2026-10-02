#pragma once

#include <SDL2/SDL.h>
#include <cstddef>

/**
 * @brief Frame timer for sprite animations, tells which frame should be shown.
 * Knows nothing about textures, owner maps frame index to sprite rect.
 *
 */
class Animation
{
    public:
        /**
         * @brief Construct a new Animation object.
         *
         * @param frameCount Number of frames.
         * @param frameMs How long one frame is shown.
         * @param loop Start over after last frame, otherwise stay on last frame.
         */
        Animation(size_t frameCount, Uint32 frameMs, bool loop);

        /**
         * @brief Advances animation, time that isn't passed in doesn't count (pause, game over).
         *
         * @param deltaMs Time since last update.
         */
        void update(Uint32 deltaMs);

        /**
         * @brief Get the Frame index to show.
         *
         * @return size_t Frame index, 0 to frameCount - 1.
         */
        size_t getFrame() const;

        /**
         * @brief Check if non looping animation has shown all its frames.
         *
         * @return true If finished, always false for looping animation.
         */
        bool isFinished() const;

        void reset();

    private:
        size_t m_frameCount;
        Uint32 m_frameMs;
        bool m_loop;
        Uint32 m_elapsedMs = 0;
};
