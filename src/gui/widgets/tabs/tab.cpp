/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gui/widgets/tabs/tab.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "gui/gui.h"

#include "gui/widgets/label.h"
#include "gui/widgets/tabbedarea.h"

#include "resources/image.h"

#include "utils/delete2.h"

#include "debug.h"

int Tab::mInstances = 0;
float Tab::mAlpha = 1.0;

static std::string const data[Tab::TAB_COUNT] =
{
    "tab.xml",
    "tab_highlighted.xml",
    "tab_selected.xml",
    "tab_unused.xml"
};

Skin *Tab::tabImg[Tab::TAB_COUNT];

Tab::Tab(const Widget2 *const widget) :
    BasicContainer(widget),
    MouseListener(),
    WidgetListener(),
    mLabel(new Label(this)),
    mTabbedArea(nullptr),
    mTabColor(&getThemeColor(Theme::TAB)),
    mTabOutlineColor(&getThemeColor(Theme::TAB_OUTLINE)),
    mTabHighlightedColor(&getThemeColor(Theme::TAB_HIGHLIGHTED)),
    mTabHighlightedOutlineColor(&getThemeColor(
        Theme::TAB_HIGHLIGHTED_OUTLINE)),
    mTabSelectedColor(&getThemeColor(Theme::TAB_SELECTED)),
    mTabSelectedOutlineColor(&getThemeColor(Theme::TAB_SELECTED_OUTLINE)),
    mFlashColor(&getThemeColor(Theme::TAB_FLASH)),
    mFlashOutlineColor(&getThemeColor(Theme::TAB_FLASH_OUTLINE)),
    mPlayerFlashColor(&getThemeColor(Theme::TAB_PLAYER_FLASH)),
    mPlayerFlashOutlineColor(&getThemeColor(Theme::TAB_PLAYER_FLASH_OUTLINE)),
    mFlash(0),
    mVertexes(new ImageCollection),
    mImage(nullptr),
    mMode(0),
    mHasMouse(false)
{
    init();
}

Tab::~Tab()
{
    if (gui)
        gui->removeDragged(this);

    mInstances--;
    if (mInstances == 0 && theme)
    {
        for (int mode = 0; mode < TAB_COUNT; mode ++)
            theme->unload(tabImg[mode]);
    }

    delete2(mLabel);

    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
    delete2(mVertexes);
}

void Tab::init()
{
    addMouseListener(this);
    setFocusable(false);
    setFrameSize(0);
    mFlash = 0;

    addWidgetListener(this);

    if (mInstances == 0)
    {
        // Load the skin
        if (theme)
        {
            for (int mode = 0; mode < TAB_COUNT; mode ++)
                tabImg[mode] = theme->load(data[mode], "tab.xml");
        }
        updateAlpha();
    }
    mInstances++;

    add(mLabel);

    const Skin *const skin = tabImg[TAB_STANDARD];
    if (!skin)
        return;
    const int padding = skin->getPadding();

    mLabel->setPosition(padding, padding);
}

void Tab::updateAlpha()
{
    const float alpha = std::max(client->getGuiAlpha(),
        theme->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            for (int t = 0; t < TAB_COUNT; t++)
            {
                Skin *const skin = tabImg[t];
                if (skin)
                {
                    const ImageRect &rect = skin->getBorder();
                    Image *const image = rect.grid[a];
                    if (image)
                        image->setAlpha(mAlpha);
                }
            }
        }
    }
}

void Tab::draw(Graphics *graphics)
{
    BLOCK_START("Tab::draw")
    int mode = TAB_STANDARD;

    // check which type of tab to draw
    if (mTabbedArea)
    {
        if (mTabbedArea->isTabSelected(this))
        {
            mLabel->setForegroundColorAll(*mTabSelectedColor,
                *mTabSelectedOutlineColor);
            mode = TAB_SELECTED;
            // if tab is selected, it doesnt need to highlight activity
            mFlash = 0;
        }
        else if (mHasMouse)
        {
            mLabel->setForegroundColorAll(*mTabHighlightedColor,
                *mTabHighlightedOutlineColor);
            mode = TAB_HIGHLIGHTED;
        }
        else
        {
            mLabel->setForegroundColorAll(*mTabColor, *mTabOutlineColor);
        }

        switch (mFlash)
        {
            case 1:
                mLabel->setForegroundColorAll(*mFlashColor,
                    *mFlashOutlineColor);
                break;
            case 2:
                mLabel->setForegroundColorAll(*mPlayerFlashColor,
                    *mPlayerFlashOutlineColor);
                break;
            default:
                break;
        }
    }

    const Skin *const skin = tabImg[mode];
    if (!skin)
    {
        BLOCK_END("Tab::draw")
        return;
    }

    updateAlpha();

    // draw tab
    if (isBatchDrawRenders(openGLMode))
    {
        const ImageRect &rect = skin->getBorder();
        if (mRedraw || mode != mMode || graphics->getRedraw())
        {
            mMode = mode;
            mRedraw = false;
            mVertexes->clear();
            graphics->calcWindow(mVertexes,
                0, 0,
                mDimension.width, mDimension.height,
                rect);

            if (mImage)
            {
                const Skin *const skin1 = tabImg[TAB_STANDARD];
                if (skin1)
                {
                    const int padding = skin1->getPadding();
                    graphics->calcTileCollection(mVertexes,
                        mImage,
                        padding,
                        padding);
                }
            }
        }

        graphics->drawTileCollection(mVertexes);
    }
    else
    {
        graphics->drawImageRect(0, 0,
            mDimension.width, mDimension.height,
            skin->getBorder());
        if (mImage)
        {
            const Skin *const skin1 = tabImg[TAB_STANDARD];
            if (skin1)
            {
                const int padding = skin1->getPadding();
                graphics->drawImage(mImage, padding, padding);
            }
        }
    }

    drawChildren(graphics);
    BLOCK_END("Tab::draw")
}

void Tab::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::widgetMoved(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::setLabelFont(Font *const font)
{
    if (!mLabel)
        return;

    mLabel->setFont(font);
    mLabel->adjustSize();
    adjustSize();
}


void Tab::adjustSize()
{
    const Skin *const skin = tabImg[TAB_STANDARD];
    if (!skin)
        return;
    const int pad2 = skin->getPadding() * 2;

    if (mImage)
    {
        const SDL_Rect &rect = mImage->mBounds;
        setSize(rect.w + pad2, rect.h + pad2);
    }
    else
    {
        setSize(mLabel->getWidth() + pad2,
                mLabel->getHeight() + pad2);
    }

    if (mTabbedArea)
        mTabbedArea->adjustTabPositions();
}

void Tab::setTabbedArea(TabbedArea* tabbedArea)
{
    mTabbedArea = tabbedArea;
}

TabbedArea* Tab::getTabbedArea() const
{
    return mTabbedArea;
}

void Tab::setCaption(const std::string &caption)
{
    mLabel->setCaption(caption);
    mLabel->adjustSize();
    adjustSize();
}

void Tab::setImage(Image *const image)
{
    if (mImage)
        mImage->decRef();
    mImage = image;
    adjustSize();
}

const std::string &Tab::getCaption() const
{
    return mLabel->getCaption();
}

void Tab::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void Tab::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}
