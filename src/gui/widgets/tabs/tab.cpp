/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/tabs/tab.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "gui/widgets/label.h"
#include "gui/widgets/tabbedarea.h"

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
    gcn::BasicContainer(),
    Widget2(widget),
    gcn::MouseListener(),
    gcn::WidgetListener(),
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
    mRedraw(true),
    mHasMouse(false)
{
    init();
}

Tab::~Tab()
{
    if (gui)
        gui->removeDragged(this);

    mInstances--;
    if (mInstances == 0 && Theme::instance())
    {
        Theme *const theme = Theme::instance();
        for (int mode = 0; mode < TAB_COUNT; mode ++)
            theme->unload(tabImg[mode]);
    }

    delete mLabel;
    mLabel = nullptr;

    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
    delete mVertexes;
    mVertexes = nullptr;
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
        Theme *const theme = Theme::instance();
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
        Theme::instance()->getMinimumOpacity());

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

void Tab::draw(gcn::Graphics *graphics)
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

    Graphics *const g = static_cast<Graphics*>(graphics);

    // draw tab
    if (isBatchDrawRenders(openGLMode))
    {
        const ImageRect &rect = skin->getBorder();
        if (mRedraw || mode != mMode || g->getRedraw())
        {
            mMode = mode;
            mRedraw = false;
            mVertexes->clear();
            g->calcWindow(mVertexes, 0, 0,
                mDimension.width, mDimension.height, rect);

            if (mImage)
            {
                const Skin *const skin1 = tabImg[TAB_STANDARD];
                if (skin1)
                {
                    const int padding = skin1->getPadding();
                    g->calcTile(mVertexes, mImage, padding, padding);
                }
            }
        }

        g->drawTile(mVertexes);
    }
    else
    {
        g->drawImageRect(0, 0,
            mDimension.width, mDimension.height, skin->getBorder());
        if (mImage)
        {
            const Skin *const skin1 = tabImg[TAB_STANDARD];
            if (skin1)
            {
                const int padding = skin1->getPadding();
                g->drawImage(mImage, padding, padding);
            }
        }
    }

    drawChildren(graphics);
    BLOCK_END("Tab::draw")
}

void Tab::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::widgetMoved(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void Tab::setLabelFont(gcn::Font *const font)
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

void Tab::mouseEntered(gcn::MouseEvent& mouseEvent A_UNUSED)
{
    mHasMouse = true;
}

void Tab::mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED)
{
    mHasMouse = false;
}
