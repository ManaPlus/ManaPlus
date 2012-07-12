/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/tab.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "gui/palette.h"
#include "gui/theme.h"

#include "gui/widgets/tabbedarea.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include <guichan/widgets/label.hpp>

#include "debug.h"

int Tab::mInstances = 0;
float Tab::mAlpha = 1.0;

enum
{
    TAB_STANDARD = 0, // 0
    TAB_HIGHLIGHTED,  // 1
    TAB_SELECTED,     // 2
    TAB_UNUSED,       // 3
    TAB_COUNT         // 4 - Must be last.
};

static std::string const data[TAB_COUNT] =
{
    "tab.xml",
    "tab_highlighted.xml",
    "tab_selected.xml",
    "tab_unused.xml"
};

ImageRect Tab::tabImg[TAB_COUNT];

Tab::Tab() :
    gcn::Tab(),
    mTabColor(&Theme::getThemeColor(Theme::TAB)),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mMode(0)
{
    init();
    mFlashColor = Theme::getThemeColor(Theme::TAB_FLASH);
    mPlayerFlashColor = Theme::getThemeColor(Theme::TAB_PLAYER_FLASH);
}

Tab::~Tab()
{
    mInstances--;
    if (mInstances == 0 && Theme::instance())
    {
        Theme *theme = Theme::instance();
        for (int mode = 0; mode < TAB_COUNT; mode ++)
            Theme::instance()->unloadRect(tabImg[mode]);
    }
    delete mVertexes;
    mVertexes = nullptr;
}

void Tab::init()
{
    setFocusable(false);
    setFrameSize(0);
    mFlash = 0;

    addWidgetListener(this);

    if (mInstances == 0)
    {
        // Load the skin
        if (Theme::instance())
        {
            for (int mode = 0; mode < TAB_COUNT; mode ++)
                Theme::instance()->loadRect(tabImg[mode], data[mode]);
        }
        updateAlpha();
    }
    mInstances++;
}

void Tab::updateAlpha()
{
    float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    // TODO We don't need to do this for every tab on every draw
    // Maybe use a config listener to do it as the value changes.
    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            for (int t = 0; t < TAB_COUNT; t++)
            {
                if (tabImg[t].grid[a])
                    tabImg[t].grid[a]->setAlpha(mAlpha);
            }
        }
    }
}

void Tab::draw(gcn::Graphics *graphics)
{
    int mode = TAB_STANDARD;

    // check which type of tab to draw
    if (mTabbedArea)
    {
        mLabel->setForegroundColor(*mTabColor);
        if (mTabbedArea->isTabSelected(this))
        {
            mode = TAB_SELECTED;
            // if tab is selected, it doesnt need to highlight activity
            mFlash = 0;
        }
        else if (mHasMouse)
        {
            mode = TAB_HIGHLIGHTED;
        }

        switch (mFlash)
        {
            case 1:
                mLabel->setForegroundColor(mFlashColor);
                break;
            case 2:
                mLabel->setForegroundColor(mPlayerFlashColor);
                break;
            default:
                break;
        }
    }

    updateAlpha();

    // draw tab
    if (mRedraw || mode != mMode
        || static_cast<Graphics*>(graphics)->getRedraw())
    {
        mMode = mode;
        mRedraw = false;
        static_cast<Graphics*>(graphics)->calcWindow(mVertexes, 0, 0,
            getWidth(), getHeight(), tabImg[mode]);
    }

    static_cast<Graphics*>(graphics)->drawImageRect2(mVertexes, tabImg[mode]);

//    static_cast<Graphics*>(graphics)->
//        drawImageRect(0, 0, getWidth(), getHeight(), tabImg[mode]);

    // draw label
    drawChildren(graphics);
}

void Tab::setTabColor(const gcn::Color *color)
{
    mTabColor = color;
}

void Tab::setFlash(int flash)
{
    mFlash = flash;
}

void Tab::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::widgetMoved(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::setLabelFont(gcn::Font *font)
{
    if (!mLabel)
        return;

    mLabel->setFont(font);
    mLabel->adjustSize();
    adjustSize();
}
