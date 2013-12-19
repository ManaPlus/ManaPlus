/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "resources/animation.h"

#include "debug.h"

Animation::Animation() noexcept :
    mFrames(),
    mDuration(0)
{
}

void Animation::addFrame(Image *const image, const int delay,
                         const int offsetX, const int offsetY,
                         const int rand) noexcept
{
    Frame frame
        = { image, delay, offsetX, offsetY, rand, Frame::ANIMATION, "" };
    mFrames.push_back(frame);
    mDuration += delay;
}

void Animation::addTerminator(const int rand) noexcept
{
    addFrame(nullptr, 0, 0, 0, rand);
}

bool Animation::isTerminator(const Frame &candidate) noexcept
{
    return (!candidate.image && candidate.type == Frame::ANIMATION);
}

void Animation::addJump(const std::string &name, const int rand) noexcept
{
    Frame frame = { nullptr, 0, 0, 0, rand, Frame::JUMP, name };
    mFrames.push_back(frame);
}

void Animation::addLabel(const std::string &name) noexcept
{
    Frame frame = { nullptr, 0, 0, 0, 100, Frame::LABEL, name };
    mFrames.push_back(frame);
}

void Animation::addGoto(const std::string &name, const int rand) noexcept
{
    Frame frame = { nullptr, 0, 0, 0, rand, Frame::GOTO, name };
    mFrames.push_back(frame);
}

void Animation::addPause(const int delay, const int rand) noexcept
{
    Frame frame = { nullptr, delay, 0, 0, rand, Frame::PAUSE, "" };
    mFrames.push_back(frame);
}

void Animation::setLastFrameDelay(const int delay) noexcept
{
    for (FramesRevIter it = mFrames.rbegin(), it_end = mFrames.rend();
         it != it_end; ++ it)
    {
        if ((*it).type == Frame::ANIMATION && (*it).image)
        {
            (*it).delay = delay;
            break;
        }
    }
}
