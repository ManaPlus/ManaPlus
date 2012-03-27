/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef ANIMATION_H
#define ANIMATION_H

#include "utils/xml.h"

#include <vector>
#include <string>

class Image;

/**
 * A single frame in an animation, with a delay and an offset.
 */
struct Frame
{
    enum FrameType
    {
        ANIMATION = 0,
        JUMP,
        GOTO,
        LABEL,
        PAUSE
    };
    Image *image;
    int delay;
    int offsetX;
    int offsetY;
    int rand;
    FrameType type;
    std::string nextAction;
};

/**
 * An animation consists of several frames, each with their own delay and
 * offset.
 */
class Animation
{
    public:
        Animation();

        /**
         * Appends a new animation at the end of the sequence.
         */
        void addFrame(Image *image, int delay, int offsetX, int offsetY,
                      int rand);

        /**
         * Appends an animation terminator that states that the animation
         * should not loop.
         */
        void addTerminator(int rand);

        /**
         * Returns the frame at the specified index.
         */
        Frame *getFrame(int index)
        { return &(mFrames[index]); }

        /**
         * Returns the length of this animation in frames.
         */
        unsigned int getLength() const
        { return static_cast<unsigned>(mFrames.size()); }

        /**
         * Check for empty animation.
         */
        bool empty() const
        { return mFrames.empty(); }

        /**
         * Returns the duration of this animation.
         */
        int getDuration() const
        { return mDuration; }

        void addJump(std::string name, int rand);

        void addLabel(std::string name);

        void addGoto(std::string name, int rand);

        void addPause(int delay, int rand);

        void setLastFrameDelay(int delay);

        /**
         * Determines whether the given animation frame is a terminator.
         */
        static bool isTerminator(const Frame &phase);

    protected:
        typedef std::vector<Frame> Frames;
        typedef Frames::iterator FramesIter;
        typedef Frames::reverse_iterator FramesRevIter;

        Frames mFrames;
        int mDuration;
};

#endif
