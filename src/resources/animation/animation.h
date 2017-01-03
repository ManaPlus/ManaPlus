/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESOURCES_ANIMATION_ANIMATION_H
#define RESOURCES_ANIMATION_ANIMATION_H

#include "resources/frame.h"
#include "resources/memorycounter.h"

#include <vector>

#include "localconsts.h"

class Image;

/**
 * An animation consists of several frames, each with their own delay and
 * offset.
 */
class Animation final : public MemoryCounter
{
    friend class AnimatedSprite;
    friend class ParticleEmitter;
    friend class SimpleAnimation;

    public:
        Animation() noexcept2;

        explicit Animation(const std::string &name) noexcept2;

        /**
         * Appends a new animation at the end of the sequence.
         */
        void addFrame(Image *const image, const int delay,
                      const int offsetX, const int offsetY,
                      const int rand) noexcept2;

        /**
         * Appends an animation terminator that states that the animation
         * should not loop.
         */
        void addTerminator(const int rand) noexcept2;

        /**
         * Returns the length of this animation in frames.
         */
        size_t getLength() const noexcept2 A_WARN_UNUSED
        { return mFrames.size(); }

        void addJump(const std::string &name, const int rand) noexcept2;

        void addLabel(const std::string &name) noexcept2;

        void addGoto(const std::string &name, const int rand) noexcept2;

        void addPause(const int delay, const int rand) noexcept2;

        void setLastFrameDelay(const int delay) noexcept2;

        typedef std::vector<Frame> Frames;
        typedef Frames::iterator FramesIter;
        typedef Frames::const_iterator FramesCIter;
        typedef Frames::reverse_iterator FramesRevIter;

#ifdef UNITTESTS
        Frames &getFrames() noexcept2
        { return mFrames; }
#endif  // UNITTESTS

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override
        { return mName; }

    protected:
        Frames mFrames;
        std::string mName;
        int mDuration;
};

#endif  // RESOURCES_ANIMATION_ANIMATION_H
