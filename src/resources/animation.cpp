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

#include "resources/animation.h"

#include "logger.h"

#include "utils/dtor.h"

#include "debug.h"

Animation::Animation():
    mDuration(0)
{
}

void Animation::addFrame(Image *image, int delay, int offsetX, int offsetY,
                         int rand)
{
    Frame frame
        = { image, delay, offsetX, offsetY, rand, Frame::ANIMATION, "" };
    mFrames.push_back(frame);
    mDuration += delay;
}

void Animation::addTerminator(int rand)
{
    addFrame(nullptr, 0, 0, 0, rand);
}

bool Animation::isTerminator(const Frame &candidate)
{
    return (!candidate.image && candidate.type == Frame::ANIMATION);
}

void Animation::addJump(std::string name, int rand)
{
    Frame frame = { nullptr, 0, 0, 0, rand, Frame::JUMP, name };
    mFrames.push_back(frame);
}

void Animation::addLabel(std::string name)
{
    Frame frame = { nullptr, 0, 0, 0, 100, Frame::LABEL, name };
    mFrames.push_back(frame);
}

void Animation::addGoto(std::string name, int rand)
{
    Frame frame = { nullptr, 0, 0, 0, rand, Frame::GOTO, name };
    mFrames.push_back(frame);
}

void Animation::addPause(int delay, int rand)
{
    Frame frame = { nullptr, delay, 0, 0, rand, Frame::PAUSE, "" };
    mFrames.push_back(frame);
}

void Animation::setLastFrameDelay(int delay)
{
    FramesRevIter it = mFrames.rbegin();
    FramesRevIter it_end = mFrames.rend();
    for (; it != it_end; ++ it)
    {
        if ((*it).type == Frame::ANIMATION && (*it).image)
        {
            (*it).delay = delay;
            break;
        }
    }
}
