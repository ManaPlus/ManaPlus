/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "utils/dtor.h"

Animation::Animation():
    mDuration(0)
{
}

void Animation::addFrame(Image *image, int delay, int offsetX, int offsetY)
{
    Frame frame = { image, delay, offsetX, offsetY, "" };
    mFrames.push_back(frame);
    mDuration += delay;
}

void Animation::addTerminator()
{
    addFrame(NULL, 0, 0, 0);
}

bool Animation::isTerminator(const Frame &candidate)
{
    return (candidate.image == NULL);
}

void Animation::addJump(std::string name)
{
    Frame frame = { 0, 0, 0, 0, name };
    mFrames.push_back(frame);
}
