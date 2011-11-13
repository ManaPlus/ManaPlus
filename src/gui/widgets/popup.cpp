/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/widgets/popup.h"

#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/windowcontainer.h"
#include "gui/widgets/window.h"

#include "resources/image.h"

#include <guichan/exception.hpp>

#include "debug.h"

Popup::Popup(const std::string &name, std::string skin):
    mPopupName(name),
    mMinWidth(100),
    mMinHeight(40),
    mMaxWidth(mainGraphics->mWidth),
    mMaxHeight(mainGraphics->mHeight),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true)
{
    logger->log("Popup::Popup(\"%s\")", name.c_str());

    if (!windowContainer)
        throw GCN_EXCEPTION("Popup::Popup(): no windowContainer set");

    addWidgetListener(this);

    setPadding(3);

    if (skin == "")
        skin = "popup.xml";

    // Loads the skin
    if (Theme::instance())
    {
        mSkin = Theme::instance()->load(skin);
        if (mSkin)
            setPadding(mSkin->getPadding());
    }
    else
    {
        mSkin = nullptr;
    }

    // Add this window to the window container
    windowContainer->add(this);

    // Popups are invisible by default
    setVisible(false);
}

Popup::~Popup()
{
    logger->log("Popup::~Popup(\"%s\")", mPopupName.c_str());

    delete mVertexes;
    mVertexes = nullptr;

    if (mSkin)
    {
        if (Theme::instance())
            Theme::instance()->unload(mSkin);
        mSkin = nullptr;
    }
}

void Popup::setWindowContainer(WindowContainer *wc)
{
    windowContainer = wc;
}

void Popup::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);

    if (mRedraw)
    {
        mRedraw = false;
        g->calcWindow(mVertexes, 0, 0, getWidth(),
            getHeight(), mSkin->getBorder());
    }

    g->drawImageRect2(mVertexes, mSkin->getBorder());

//    g->drawImageRect(0, 0, getWidth(), getHeight(), mSkin->getBorder());

    drawChildren(graphics);
}

gcn::Rectangle Popup::getChildrenArea()
{
    return gcn::Rectangle(getPadding(), 0, getWidth() - getPadding() * 2,
                          getHeight() - getPadding() * 2);
}

void Popup::setContentSize(int width, int height)
{
    width += 2 * getPadding();
    height += 2 * getPadding();

    if (getMinWidth() > width)
        width = getMinWidth();
    else if (getMaxWidth() < width)
        width = getMaxWidth();
    if (getMinHeight() > height)
        height = getMinHeight();
    else if (getMaxHeight() < height)
        height = getMaxHeight();

    setSize(width, height);
    mRedraw = true;
}

void Popup::setLocationRelativeTo(gcn::Widget *widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x),
                getY() + (wy + (widget->getHeight() - getHeight()) / 2 - y));
    mRedraw = true;
}

void Popup::setMinWidth(int width)
{
    if (mSkin)
    {
        mMinWidth = width > mSkin->getMinWidth()
            ? width : mSkin->getMinWidth();
    }
    else
    {
        mMinWidth = width;
    }
}

void Popup::setMinHeight(int height)
{
    if (mSkin)
    {
        mMinHeight = height > mSkin->getMinHeight() ?
            height : mSkin->getMinHeight();
    }
    else
    {
        mMinHeight = height;
    }
}

void Popup::setMaxWidth(int width)
{
    mMaxWidth = width;
}

void Popup::setMaxHeight(int height)
{
    mMaxHeight = height;
}

void Popup::scheduleDelete()
{
    windowContainer->scheduleDelete(this);
}

void Popup::position(int x, int y)
{
    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > mainGraphics->mWidth)
        posX = mainGraphics->mWidth - getWidth();
    if (posY + getHeight() > mainGraphics->mHeight)
        posY = y - getHeight() - distance;

    setPosition(posX, posY);
    setVisible(true);
    requestMoveToTop();
    mRedraw = true;
}

void Popup::mouseMoved(gcn::MouseEvent &event A_UNUSED)
{
    if (viewport)
        viewport->hideBeingPopup();
    mRedraw = true;
}

void Popup::hide()
{
    setVisible(false);
    mRedraw = true;
}

void Popup::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void Popup::widgetMoved(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}
