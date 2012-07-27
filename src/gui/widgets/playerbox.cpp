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

#include "gui/widgets/playerbox.h"

#include "animatedsprite.h"
#include "being.h"
#include "client.h"
#include "configuration.h"
#include "graphics.h"

#include "gui/theme.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include "debug.h"

PlayerBox::PlayerBox(Being *being, std::string skin):
    mBeing(being),
    mAlpha(1.0)
{
    init(skin);
}

PlayerBox::PlayerBox(std::string skin):
    mBeing(nullptr),
    mAlpha(1.0)
{
    init(skin);
}

PlayerBox::~PlayerBox()
{
    if (Theme::instance())
        Theme::instance()->unloadRect(mBackground);

    mBeing = nullptr;
}

void PlayerBox::init(std::string skin)
{
    setFrameSize(2);

    if (Theme::instance())
    {
        if (skin.empty())
            skin = "playerbox_background.xml";
        Theme::instance()->loadRect(mBackground, skin);
    }
    else
    {
        for (int f = 0; f < 9; f ++)
            mBackground.grid[f] = nullptr;
    }
}

void PlayerBox::draw(gcn::Graphics *graphics)
{
    if (mBeing)
    {
        // Draw character
        const int bs = getFrameSize();
        const int x = getWidth() / 2 + bs - 16;
        const int y = getHeight() - bs - 32;
        mBeing->drawSpriteAt(static_cast<Graphics*>(graphics), x, y);
    }

    if (Client::getGuiAlpha() != mAlpha)
    {
        for (int a = 0; a < 9; a++)
        {
            if (mBackground.grid[a])
                mBackground.grid[a]->setAlpha(Client::getGuiAlpha());
        }
    }
}

void PlayerBox::drawFrame(gcn::Graphics *graphics)
{
    int w, h, bs;
    bs = getFrameSize();
    w = getWidth() + bs * 2;
    h = getHeight() + bs * 2;

    static_cast<Graphics*>(graphics)->drawImageRect(0, 0, w, h, mBackground);
}
