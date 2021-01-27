/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/animation/animation.h"

#include "utils/foreach.h"

#include "debug.h"

Animation::Animation() noexcept2 :
    MemoryCounter(),
    mFrames(),
    mName("animation"),
    mDuration(0)
{
}

Animation::Animation(const std::string &name) noexcept2 :
    MemoryCounter(),
    mFrames(),
    mName(name),
    mDuration(0)
{
}

void Animation::addFrame(Image *const image, const int delay,
                         const int offsetX, const int offsetY,
                         const int rand) noexcept2
{
    Frame frame
        = { image, delay, offsetX, offsetY, rand, FrameType::ANIMATION, "" };
    mFrames.push_back(frame);
    mDuration += delay;
}

void Animation::addTerminator(const int rand) noexcept2
{
    addFrame(nullptr, 0, 0, 0, rand);
}

void Animation::addJump(const std::string &name, const int rand) noexcept2
{
    const Frame frame = { nullptr, 0, 0, 0, rand, FrameType::JUMP, name };
    mFrames.push_back(frame);
}

void Animation::addLabel(const std::string &name) noexcept2
{
    const Frame frame = { nullptr, 0, 0, 0, 100, FrameType::LABEL, name };
    mFrames.push_back(frame);
}

void Animation::addGoto(const std::string &name, const int rand) noexcept2
{
    const Frame frame = { nullptr, 0, 0, 0, rand, FrameType::GOTO, name };
    mFrames.push_back(frame);
}

void Animation::addPause(const int delay, const int rand) noexcept2
{
    const Frame frame = { nullptr, delay, 0, 0, rand, FrameType::PAUSE, "" };
    mFrames.push_back(frame);
}

void Animation::setLastFrameDelay(const int delay) noexcept2
{
    for (FramesRevIter it = mFrames.rbegin(), it_end = mFrames.rend();
         it != it_end; ++ it)
    {
        if ((*it).type == FrameType::ANIMATION && ((*it).image != nullptr))
        {
            (*it).delay = delay;
            break;
        }
    }
}

int Animation::calcMemoryLocal() const
{
    int sz = sizeof(Animation);
    FOR_EACH (FramesCIter, it, mFrames)
    {
        const Frame &frame = *it;
        sz += static_cast<int>(sizeof(Frame) +
            frame.nextAction.capacity());
    }
    return sz;
}
