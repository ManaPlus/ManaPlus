/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "client.h"

#include "being/being.h"

#include "resources/image.h"

#include "debug.h"

PlayerBox::PlayerBox(Being *const being, const std::string &skin,
                     const std::string &selectedSkin) :
    Widget2(),
    ScrollArea(),
    mBeing(being),
    mAlpha(1.0),
    mBackground(),
    mSelectedBackground(),
    mSkin(nullptr),
    mSelectedSkin(nullptr),
    mOffsetX(-mapTileSize / 2),
    mOffsetY(-mapTileSize),
    mDrawBackground(false),
    mSelected(false)
{
    init(skin, selectedSkin);
}

PlayerBox::PlayerBox(const std::string &skin,
                     const std::string &selectedSkin) :
    ScrollArea(),
    mBeing(nullptr),
    mAlpha(1.0),
    mBackground(),
    mSelectedBackground(),
    mSkin(nullptr),
    mSelectedSkin(nullptr),
    mOffsetX(-mapTileSize / 2),
    mOffsetY(-mapTileSize),
    mDrawBackground(false),
    mSelected(false)
{
    init(skin, selectedSkin);
}

PlayerBox::~PlayerBox()
{
    if (gui)
        gui->removeDragged(this);

    Theme::unloadRect(mBackground);
    Theme::unloadRect(mSelectedBackground);
    mBeing = nullptr;
}

void PlayerBox::init(std::string name, std::string selectedName)
{
    setFrameSize(2);

    Theme *const theme = Theme::instance();
    if (theme)
    {
        if (name.empty())
            name = "playerbox.xml";
        mSkin = theme->loadSkinRect(mBackground,
            name, "playerbox_background.xml");
        if (mSkin)
        {
            mDrawBackground = (mSkin->getOption("drawbackground") != 0);
            mOffsetX = mSkin->getOption("offsetX", -mapTileSize / 2);
            mOffsetY = mSkin->getOption("offsetY", -mapTileSize);
            mFrameSize = mSkin->getOption("frameSize", 2);
        }
        if (selectedName.empty())
            selectedName = "playerboxselected.xml";
        mSelectedSkin = theme->loadSkinRect(mSelectedBackground,
            selectedName, "playerbox_background.xml");
    }
    else
    {
        for (int f = 0; f < 9; f ++)
            mBackground.grid[f] = nullptr;
        for (int f = 0; f < 9; f ++)
            mSelectedBackground.grid[f] = nullptr;
    }
}

void PlayerBox::draw(gcn::Graphics *graphics)
{
    BLOCK_START("PlayerBox::draw")
    if (mBeing)
    {
        const int bs = mFrameSize;
        const int x = mDimension.width / 2 + bs + mOffsetX;
        const int y = mDimension.height - bs + mOffsetY;
        mBeing->drawSpriteAt(static_cast<Graphics*>(graphics), x, y);
    }

    if (client->getGuiAlpha() != mAlpha)
    {
        const float alpha = client->getGuiAlpha();
        for (int a = 0; a < 9; a++)
        {
            if (mBackground.grid[a])
                mBackground.grid[a]->setAlpha(alpha);
        }
    }
    BLOCK_END("PlayerBox::draw")
}

void PlayerBox::drawFrame(gcn::Graphics *graphics)
{
    BLOCK_START("PlayerBox::drawFrame")
    if (mDrawBackground)
    {
        const int bs = mFrameSize * 2;
        const int w = mDimension.width + bs;
        const int h = mDimension.height + bs;

        if (!mSelected)
        {
            static_cast<Graphics*>(graphics)->drawImageRect(
                0, 0, w, h, mBackground);
        }
        else
        {
            static_cast<Graphics*>(graphics)->drawImageRect(
                0, 0, w, h, mSelectedBackground);
        }
    }
    BLOCK_END("PlayerBox::drawFrame")
}

void PlayerBox::mouseReleased(gcn::MouseEvent& event)
{
    ScrollArea::mouseReleased(event);
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mActionEventId.empty())
            distributeActionEvent();
    }
}
