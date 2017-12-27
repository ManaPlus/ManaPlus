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

#include "gui/widgets/playerbox.h"

#include "settings.h"

#include "being/being.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "render/graphics.h"

#include "resources/image/image.h"

#include "debug.h"

PlayerBox::PlayerBox(Widget2 *const widget,
                     Being *const being,
                     const std::string &skin,
                     const std::string &selectedSkin) :
    Widget(widget),
    MouseListener(),
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

PlayerBox::PlayerBox(Widget2 *const widget,
                     const std::string &skin,
                     const std::string &selectedSkin) :
    Widget(widget),
    MouseListener(),
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
    if (gui != nullptr)
        gui->removeDragged(this);

    Theme::unloadRect(mBackground, 0, 8);
    Theme::unloadRect(mSelectedBackground, 0, 8);
    mBeing = nullptr;
}

void PlayerBox::init(std::string name, std::string selectedName)
{
    mAllowLogic = false;
    setFrameSize(2);
    addMouseListener(this);

    if (theme != nullptr)
    {
        if (name.empty())
            name = "playerbox.xml";
        mSkin = theme->loadSkinRect(mBackground,
            name,
            "playerbox_background.xml",
            0,
            8);
        if (mSkin != nullptr)
        {
            mDrawBackground = (mSkin->getOption("drawbackground") != 0);
            mOffsetX = mSkin->getOption("offsetX", -mapTileSize / 2);
            mOffsetY = mSkin->getOption("offsetY", -mapTileSize);
            mFrameSize = mSkin->getOption("frameSize", 2);
        }
        if (selectedName.empty())
            selectedName = "playerboxselected.xml";
        mSelectedSkin = theme->loadSkinRect(mSelectedBackground,
            selectedName,
            "playerbox_background.xml",
            0,
            8);
    }
    else
    {
        for (int f = 0; f < 9; f ++)
            mBackground.grid[f] = nullptr;
        for (int f = 0; f < 9; f ++)
            mSelectedBackground.grid[f] = nullptr;
    }
}

void PlayerBox::draw(Graphics *const graphics)
{
    BLOCK_START("PlayerBox::draw")
    if (mBeing != nullptr)
    {
        const int bs = mFrameSize;
        const int x = mDimension.width / 2 + bs + mOffsetX;
        const int y = mDimension.height - bs + mOffsetY;
        mBeing->drawBasic(graphics, x, y);
    }

    if (settings.guiAlpha != mAlpha)
    {
        const float alpha = settings.guiAlpha;
        for (int a = 0; a < 9; a++)
        {
            if (mBackground.grid[a] != nullptr)
                mBackground.grid[a]->setAlpha(alpha);
        }
    }
    BLOCK_END("PlayerBox::draw")
}

void PlayerBox::safeDraw(Graphics *const graphics)
{
    PlayerBox::draw(graphics);
}

void PlayerBox::drawFrame(Graphics *const graphics)
{
    BLOCK_START("PlayerBox::drawFrame")
    if (mDrawBackground)
    {
        const int bs = mFrameSize * 2;
        const int w = mDimension.width + bs;
        const int h = mDimension.height + bs;

        if (!mSelected)
            graphics->drawImageRect(0, 0, w, h, mBackground);
        else
            graphics->drawImageRect(0, 0, w, h, mSelectedBackground);
    }
    BLOCK_END("PlayerBox::drawFrame")
}

void PlayerBox::safeDrawFrame(Graphics *const graphics)
{
    BLOCK_START("PlayerBox::drawFrame")
    if (mDrawBackground)
    {
        const int bs = mFrameSize * 2;
        const int w = mDimension.width + bs;
        const int h = mDimension.height + bs;

        if (!mSelected)
            graphics->drawImageRect(0, 0, w, h, mBackground);
        else
            graphics->drawImageRect(0, 0, w, h, mSelectedBackground);
    }
    BLOCK_END("PlayerBox::drawFrame")
}

void PlayerBox::mouseReleased(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        if (!mActionEventId.empty())
            distributeActionEvent();
        event.consume();
    }
}
