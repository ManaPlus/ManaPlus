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

#include "gui/windows/cutinwindow.h"

#include "configuration.h"

#include "const/utils/timer.h"

#include "resources/sprite/animatedsprite.h"

#include "utils/delete2.h"
#include "utils/timer.h"

#include "debug.h"

CutInWindow *cutInWindow = nullptr;

CutInWindow::CutInWindow() :
    Window("", Modal_false, nullptr, "cutin.xml"),
    mImage(nullptr),
    mOldTitleBarHeight(mTitleBarHeight)
{
    setWindowName("CutIn");

    setShowTitle(false);
    setResizable(false);
    setDefaultVisible(false);
    setSaveVisible(false);
    setVisible(Visible_false);
    enableVisibleSound(false);
}

CutInWindow::~CutInWindow()
{
    delete2(mImage);
}

void CutInWindow::draw(Graphics *const graphics)
{
    Window::draw(graphics);
    draw2(graphics);
}

void CutInWindow::safeDraw(Graphics *const graphics)
{
    Window::safeDraw(graphics);
    draw2(graphics);
}

void CutInWindow::draw2(Graphics *const graphics)
{
    if (mImage != nullptr)
        mImage->drawRaw(graphics, mPadding, mTitleBarHeight);
}

void CutInWindow::show(const std::string &name,
                       const CutInT cutin)
{
    delete2(mImage);
    if (name.empty())
    {
        setVisible(Visible_false);
    }
    else
    {
        mImage = AnimatedSprite::load(
            pathJoin(paths.getStringValue("cutInsDir"), name).append(".xml"),
            0);
        if (mImage != nullptr)
        {
            mImage->update(1);
            const bool showTitle = (cutin == CutIn::MovableClose);
            if (showTitle)
                mTitleBarHeight = mOldTitleBarHeight;
            else
                mTitleBarHeight = mPadding;
            const int width = mImage->getWidth() + 2 * mPadding;
            const int height = mImage->getHeight() + mTitleBarHeight
                + mPadding;

            const int screenWidth = mainGraphics->mWidth;
            const int screenHeight = mainGraphics->mHeight;

            if (width * 2 > screenWidth ||
                height * 2 > screenHeight)
            {
                return;
            }
            const int padding = 100;
            int x = 0;
            const int y = screenHeight - height - padding;
            switch (cutin)
            {
                case CutIn::BottomRight:
                    x = screenWidth - width - padding;
                    break;
                case CutIn::BottomCenter:
                case CutIn::Movable:
                case CutIn::MovableClose:
                    x = (screenWidth - width) / 2;
                    break;
                case CutIn::BottomLeft:
                default:
                    x = padding;
                    break;
            }
            setCloseButton(false);
            setVisible(Visible_true);
            setPosition(x, y);
            setCloseButton(showTitle);
            setShowTitle(showTitle);
            setSize(width, height);
            setVisible(Visible_true);
            requestMoveToTop();
        }
    }
}

void CutInWindow::hide()
{
    delete2(mImage);
    setVisible(Visible_false);
}

void CutInWindow::logic()
{
    if (mImage != nullptr)
    {
        const int time = tick_time * MILLISECONDS_IN_A_TICK;
        mImage->update(time);
    }
}
