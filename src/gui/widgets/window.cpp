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

#include "gui/widgets/window.h"

#include "client.h"
#include "configuration.h"
#include "dragdrop.h"
#include "graphicsvertexes.h"
#include "soundconsts.h"
#include "soundmanager.h"

#include "gui/focushandler.h"
#include "gui/font.h"
#include "gui/gui.h"
#include "gui/viewport.h"

#include "gui/widgets/layout.h"

#include "resources/cursor.h"
#include "resources/image.h"

#include "utils/delete2.h"

#include "debug.h"

const int resizeMask = 8 + 4 + 2 + 1;

int Window::windowInstances = 0;
int Window::mouseResize = 0;

Window::Window(const std::string &caption, const bool modal,
               Window *const parent, std::string skin) :
    BasicContainer2(nullptr),
    MouseListener(),
    WidgetListener(),
    mCaption(caption),
    mAlignment(Graphics::CENTER),
    mPadding(2),
    mTitleBarHeight(16),
    mMovable(true),
    mDragOffsetX(0),
    mDragOffsetY(0),
    mMoved(false),
    mSkin(nullptr),
    mDefaultX(0),
    mDefaultY(0),
    mDefaultWidth(0),
    mDefaultHeight(0),
    mRedraw(true),
    mLastRedraw(true),
    mGrip(nullptr),
    mParent(parent),
    mLayout(nullptr),
    mCloseRect(),
    mStickyRect(),
    mGripRect(),
    mWindowName("window"),
    mMinWinWidth(100),
    mMinWinHeight(40),
    mMaxWinWidth(mainGraphics->mWidth),
    mMaxWinHeight(mainGraphics->mHeight),
    mVertexes(new ImageCollection),
    mCaptionOffsetX(7),
    mCaptionOffsetY(5),
    mCaptionAlign(Graphics::LEFT),
    mTitlePadding(4),
    mGripPadding(2),
    mResizeHandles(-1),
    mOldResizeHandles(-1),
    mCaptionFont(getFont()),
    mShowTitle(true),
    mModal(modal),
    mCloseWindowButton(false),
    mDefaultVisible(false),
    mSaveVisible(false),
    mStickyButton(false),
    mSticky(false),
    mStickyButtonLock(false),
    mPlayVisibleSound(false)
{
    logger->log("Window::Window(\"%s\")", caption.c_str());

    windowInstances++;

//    mFrameSize = 1;
    addMouseListener(this);

    setFrameSize(0);
    setPadding(3);
    setTitleBarHeight(20);

    if (skin == "")
        skin = "window.xml";

    int childPalette = 1;
    // Loads the skin
    if (theme)
    {
        mSkin = theme->load(skin, "window.xml");
        if (mSkin)
        {
            setPadding(mSkin->getPadding());
            if (getOptionBool("titlebarBold"))
                mCaptionFont = boldFont;
            mTitlePadding = mSkin->getTitlePadding();
            mGripPadding = getOption("resizePadding");
            mCaptionOffsetX = getOption("captionoffsetx");
            if (!mCaptionOffsetX)
                mCaptionOffsetX = 7;
            mCaptionOffsetY = getOption("captionoffsety");
            if (!mCaptionOffsetY)
                mCaptionOffsetY = 5;
            mCaptionAlign = static_cast<Graphics::Alignment>(
                getOption("captionalign"));
            if (mCaptionAlign < Graphics::LEFT
                || mCaptionAlign > Graphics::RIGHT)
            {
                mCaptionAlign = Graphics::LEFT;
            }
            setTitleBarHeight(getOption("titlebarHeight"));
            if (!mTitleBarHeight)
                mTitleBarHeight = mCaptionFont->getHeight() + mPadding;

            mTitleBarHeight += getOption("titlebarHeightRelative");
            setPalette(getOption("palette"));
            childPalette = getOption("childPalette");
            mShowTitle = getOptionBool("showTitle", true);
        }
    }

    // Add this window to the window container
    if (windowContainer)
        windowContainer->add(this);

    if (mModal)
    {
        gui->setCursorType(Cursor::CURSOR_POINTER);
        requestModalFocus();
    }

    // Windows are invisible by default
    setVisible(false);

    addWidgetListener(this);
    mForegroundColor = getThemeColor(Theme::WINDOW);
    mForegroundColor2 = getThemeColor(Theme::WINDOW_OUTLINE);
    setPalette(childPalette);
}

Window::~Window()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());

    if (gui)
        gui->removeDragged(this);

    client->windowRemoved(this);

    saveWindowState();

    delete2(mLayout);

    while (!mWidgets.empty())
        delete mWidgets.front();

    mWidgets.clear();

    removeWidgetListener(this);
    delete2(mVertexes);

    windowInstances--;

    if (mSkin)
    {
        if (theme)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
    if (mGrip)
    {
        mGrip->decRef();
        mGrip = nullptr;
    }
}

void Window::setWindowContainer(WindowContainer *const wc)
{
    windowContainer = wc;
}

void Window::draw(Graphics *graphics)
{
    if (!mSkin)
        return;

    BLOCK_START("Window::draw")
    bool update = false;

    if (isBatchDrawRenders(openGLMode))
    {
        if (mResizeHandles != mOldResizeHandles)
        {
            mRedraw = true;
            mOldResizeHandles = mResizeHandles;
        }
        if (mRedraw)
        {
            mLastRedraw = true;
            mRedraw = false;
            update = true;
            mVertexes->clear();
            graphics->calcWindow(mVertexes,
                0, 0,
                mDimension.width,
                mDimension.height,
                mSkin->getBorder());

            // Draw Close Button
            if (mCloseWindowButton)
            {
                const Image *const button = mSkin->getCloseImage(
                    mResizeHandles == CLOSE);
                if (button)
                {
                    graphics->calcTileCollection(mVertexes,
                        button,
                        mCloseRect.x,
                        mCloseRect.y);
                }
            }
            // Draw Sticky Button
            if (mStickyButton)
            {
                const Image *const button = mSkin->getStickyImage(mSticky);
                if (button)
                {
                    graphics->calcTileCollection(mVertexes,
                        button,
                        mStickyRect.x,
                        mStickyRect.y);
                }
            }

            if (mGrip)
            {
                graphics->calcTileCollection(mVertexes,
                    mGrip,
                    mGripRect.x,
                    mGripRect.y);
            }
        }
        else
        {
            mLastRedraw = false;
        }
        graphics->drawTileCollection(mVertexes);
    }
    else
    {
        graphics->drawImageRect(0, 0,
            mDimension.width,
            mDimension.height,
            mSkin->getBorder());

        // Draw Close Button
        if (mCloseWindowButton)
        {
            const Image *const button = mSkin->getCloseImage(
                mResizeHandles == CLOSE);
            if (button)
                graphics->drawImage(button, mCloseRect.x, mCloseRect.y);
        }
        // Draw Sticky Button
        if (mStickyButton)
        {
            const Image *const button = mSkin->getStickyImage(mSticky);
            if (button)
                graphics->drawImage(button, mStickyRect.x, mStickyRect.y);
        }

        if (mGrip)
            graphics->drawImage(mGrip, mGripRect.x, mGripRect.y);
    }

    // Draw title
    if (mShowTitle)
    {
        graphics->setColorAll(mForegroundColor, mForegroundColor2);
        int x;
        switch (mCaptionAlign)
        {
            case Graphics::LEFT:
            default:
                x = mCaptionOffsetX;
                break;
            case Graphics::CENTER:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption) / 2;
                break;
            case Graphics::RIGHT:
                x = mCaptionOffsetX - mCaptionFont->getWidth(mCaption);
                break;
        }
        mCaptionFont->drawString(graphics, mCaption, x, mCaptionOffsetY);
    }

    if (update)
    {
        graphics->setRedraw(update);
        drawChildren(graphics);
        graphics->setRedraw(false);
    }
    else
    {
        drawChildren(graphics);
    }
    BLOCK_END("Window::draw")
}

void Window::setContentSize(int width, int height)
{
    width = width + 2 * mPadding;
    height = height + mPadding + mTitleBarHeight;

    if (mMinWinWidth > width)
        width = mMinWinWidth;
    else if (mMaxWinWidth < width)
        width = mMaxWinWidth;
    if (mMinWinHeight > height)
        height = mMinWinHeight;
    else if (mMaxWinHeight < height)
        height = mMaxWinHeight;

    setSize(width, height);
}

void Window::setLocationRelativeTo(const Widget *const widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(mDimension.x + (wx + (widget->getWidth()
        - mDimension.width) / 2 - x),
        mDimension.y + (wy + (widget->getHeight()
        - mDimension.height) / 2 - y));
}

void Window::setLocationHorisontallyRelativeTo(const Widget *const widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(mDimension.x + (wx + (widget->getWidth()
        - mDimension.width) / 2 - x), 0);
}

void Window::setLocationRelativeTo(const ImageRect::ImagePosition &position,
                                   int offsetX, int offsetY)
{
    if (position == ImageRect::UPPER_LEFT)
    {
    }
    else if (position == ImageRect::UPPER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
    }
    else if (position == ImageRect::UPPER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
    }
    else if (position == ImageRect::LEFT)
    {
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImageRect::CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImageRect::RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
        offsetY += (mainGraphics->mHeight - mDimension.height) / 2;
    }
    else if (position == ImageRect::LOWER_LEFT)
    {
        offsetY += mainGraphics->mHeight - mDimension.height;
    }
    else if (position == ImageRect::LOWER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - mDimension.width) / 2;
        offsetY += mainGraphics->mHeight - mDimension.height;
    }
    else if (position == ImageRect::LOWER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - mDimension.width;
        offsetY += mainGraphics->mHeight - mDimension.height;
    }

    setPosition(offsetX, offsetY);
}

void Window::setMinWidth(const int width)
{
    if (mSkin)
    {
        mMinWinWidth = width > mSkin->getMinWidth()
            ? width : mSkin->getMinWidth();
    }
    else
    {
        mMinWinWidth = width;
    }
}

void Window::setMinHeight(const int height)
{
    if (mSkin)
    {
        mMinWinHeight = height > mSkin->getMinHeight()
            ? height : mSkin->getMinHeight();
    }
    else
    {
        mMinWinHeight = height;
    }
}

void Window::setMaxWidth(const int width)
{
    mMaxWinWidth = width;
}

void Window::setMaxHeight(const int height)
{
    mMaxWinHeight = height;
}

void Window::setResizable(const bool r)
{
    if ((mGrip != nullptr) == r)
        return;

    if (mGrip)
        mGrip->decRef();
    if (r)
    {
        mGrip = Theme::getImageFromThemeXml("resize.xml", "");
        if (mGrip)
        {
            mGripRect.x = mDimension.width - mGrip->getWidth() - mGripPadding;
            mGripRect.y = mDimension.height - mGrip->getHeight()
                - mGripPadding;
        }
        else
        {
            mGripRect.x = 0;
            mGripRect.y = 0;
        }
    }
    else
    {
        mGrip = nullptr;
    }
}

void Window::widgetResized(const Event &event A_UNUSED)
{
    const Rect area = getChildrenArea();

    if (mGrip)
    {
        mGripRect.x = mDimension.width - mGrip->getWidth() - mGripPadding;
        mGripRect.y = mDimension.height - mGrip->getHeight() - mGripPadding;
    }

    if (mLayout)
    {
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
    if (mSkin)
    {
        const bool showClose = mCloseWindowButton
            && mSkin->getCloseImage(false);
        const int closePadding = getOption("closePadding");
        if (showClose)
        {
            const Image *const button = mSkin->getCloseImage(false);
            const int buttonWidth = button->getWidth();
            mCloseRect.x = mDimension.width - buttonWidth - closePadding;
            mCloseRect.y = closePadding;
            mCloseRect.width = buttonWidth;
            mCloseRect.height = button->getHeight();
        }
        if (mStickyButton)
        {
            const Image *const button = mSkin->getStickyImage(mSticky);
            if (button)
            {
                const int buttonWidth = button->getWidth();
                int x = mDimension.width - buttonWidth
                    - getOption("stickySpacing") - closePadding;

                if (showClose)
                    x -= mSkin->getCloseImage(false)->getWidth();

                mStickyRect.x = x;
                mStickyRect.y = getOption("stickyPadding");
                mStickyRect.width = buttonWidth;
                mStickyRect.height = button->getHeight();
            }
        }
    }
    else
    {
        mCloseRect.x = 0;
        mCloseRect.y = 0;
        mCloseRect.width = 0;
        mCloseRect.height = 0;
        mStickyRect.x = 0;
        mStickyRect.y = 0;
        mStickyRect.width = 0;
        mStickyRect.height = 0;
    }

    mRedraw = true;
}

void Window::widgetMoved(const Event& event A_UNUSED)
{
    mRedraw = true;
}

void Window::widgetHidden(const Event &event A_UNUSED)
{
    if (gui)
        gui->setCursorType(Cursor::CURSOR_POINTER);

    if (!mFocusHandler)
        return;

    for (WidgetListConstIterator it = mWidgets.begin();
         it != mWidgets.end(); ++ it)
    {
        if (mFocusHandler->isFocused(*it))
            mFocusHandler->focusNone();
    }
}

void Window::setCloseButton(const bool flag)
{
    mCloseWindowButton = flag;
}

bool Window::isResizable() const
{
    return mGrip;
}

void Window::setStickyButton(const bool flag)
{
    mStickyButton = flag;
}

void Window::setSticky(const bool sticky)
{
    mSticky = sticky;
    mRedraw = true;
}

void Window::setStickyButtonLock(const bool lock)
{
    mStickyButtonLock = lock;
    mStickyButton = lock;
}

void Window::setVisible(bool visible)
{
    setVisible(visible, false);
}

void Window::setVisible(const bool visible, const bool forceSticky)
{
    if (visible == mVisible)
        return;

    // Check if the window is off screen...
    if (visible)
        ensureOnScreen();
    else
        mResizeHandles = 0;

    if (mStickyButtonLock)
        BasicContainer2::setVisible(visible);
    else
        BasicContainer2::setVisible((!forceSticky && mSticky) || visible);
    if (visible)
    {
        if (mPlayVisibleSound)
            soundManager.playGuiSound(SOUND_SHOW_WINDOW);
        if (gui)
        {
            MouseEvent *const event = reinterpret_cast<MouseEvent*>(
                gui->createMouseEvent(this));
            if (event)
            {
                const int x = event->getX();
                const int y = event->getY();
                if (x >= 0 && x <= mDimension.width
                    && y >= 0 && y <= mDimension.height)
                {
                    mouseMoved(*event);
                }
                delete event;
            }
        }
    }
    else
    {
        if (mPlayVisibleSound)
            soundManager.playGuiSound(SOUND_HIDE_WINDOW);
    }
}

void Window::scheduleDelete()
{
    windowContainer->scheduleDelete(this);
}

void Window::mousePressed(MouseEvent &event)
{
    if (event.isConsumed())
        return;

    if (event.getSource() == this)
    {
        if (getParent())
            getParent()->moveToTop(this);

        mDragOffsetX = event.getX();
        mDragOffsetY = event.getY();
        mMoved = event.getY() <= static_cast<int>(mTitleBarHeight);
    }

    const unsigned int button = event.getButton();
    if (button == MouseEvent::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

        // Handle close button
        if (mCloseWindowButton && mSkin && mCloseRect.isPointInRect(x, y))
        {
            mouseResize = 0;
            mMoved = 0;
            event.consume();
            close();
            return;
        }

        // Handle sticky button
        if (mStickyButton && mSkin && mStickyRect.isPointInRect(x, y))
        {
            setSticky(!isSticky());
            mouseResize = 0;
            mMoved = 0;
            event.consume();
            return;
        }

        // Handle window resizing
        mouseResize = getResizeHandles(event) & resizeMask;
        if (mouseResize != 0)
            event.consume();
        if (canMove())
            mMoved = !mouseResize;
        else
            mMoved = false;
    }
    else if (button == MouseEvent::RIGHT)
    {
        if (viewport)
        {
            event.consume();
            viewport->showWindowPopup(this);
        }
    }
}

void Window::close()
{
    setVisible(false);
}

void Window::mouseReleased(MouseEvent &event A_UNUSED)
{
    if (mGrip && mouseResize)
    {
        mouseResize = 0;
        if (gui)
            gui->setCursorType(Cursor::CURSOR_POINTER);
    }

    mMoved = false;
}

void Window::mouseEntered(MouseEvent &event)
{
    updateResizeHandler(event);
}

void Window::mouseExited(MouseEvent &event A_UNUSED)
{
    if (mGrip && !mouseResize && gui)
        gui->setCursorType(Cursor::CURSOR_POINTER);
}

void Window::updateResizeHandler(MouseEvent &event)
{
    if (!gui)
        return;

    if (!dragDrop.isEmpty())
        return;

    mResizeHandles = getResizeHandles(event);

    // Changes the custom mouse cursor based on it's current position.
    switch (mResizeHandles & resizeMask)
    {
        case BOTTOM | RIGHT:
        case TOP | LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN_RIGHT);
            break;
        case TOP | RIGHT:
        case BOTTOM | LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN_LEFT);
            break;
        case BOTTOM:
        case TOP:
            gui->setCursorType(Cursor::CURSOR_RESIZE_DOWN);
            break;
        case RIGHT:
        case LEFT:
            gui->setCursorType(Cursor::CURSOR_RESIZE_ACROSS);
            break;
        default:
            gui->setCursorType(Cursor::CURSOR_POINTER);
            break;
    }
}

void Window::mouseMoved(MouseEvent &event)
{
    updateResizeHandler(event);
    if (viewport)
        viewport->hideBeingPopup();
}

bool Window::canMove() const
{
    return !mStickyButtonLock || !mSticky;
}

void Window::mouseDragged(MouseEvent &event)
{
    if (canMove())
    {
        if (!event.isConsumed() && event.getSource() == this)
        {
            if (isMovable() && mMoved)
            {
                setPosition(event.getX() - mDragOffsetX + getX(),
                    event.getY() - mDragOffsetY + getY());
            }

            event.consume();
        }
    }
    else
    {
        if (!event.isConsumed() && event.getSource() == this)
            event.consume();
        return;
    }

    // Keep guichan window inside screen when it may be moved
    if (isMovable() && mMoved)
    {
        setPosition(std::min(mainGraphics->mWidth - mDimension.width,
            std::max(0, mDimension.x)),
            std::min(mainGraphics->mHeight - mDimension.height,
            std::max(0, mDimension.y)));
    }

    if (mouseResize && !mMoved)
    {
        const int dx = event.getX() - mDragOffsetX;
        const int dy = event.getY() - mDragOffsetY;
        Rect newDim = getDimension();

        if (mouseResize & (TOP | BOTTOM))
        {
            const int newHeight = newDim.height
                + ((mouseResize & TOP) ? -dy : dy);
            newDim.height = std::min(mMaxWinHeight,
                std::max(mMinWinHeight, newHeight));

            if (mouseResize & TOP)
                newDim.y -= newDim.height - getHeight();
        }

        if (mouseResize & (LEFT | RIGHT))
        {
            const int newWidth = newDim.width
                + ((mouseResize & LEFT) ? -dx : dx);
            newDim.width = std::min(mMaxWinWidth,
                std::max(mMinWinWidth, newWidth));

            if (mouseResize & LEFT)
                newDim.x -= newDim.width - mDimension.width;
        }

        // Keep guichan window inside screen (supports resizing any side)
        if (newDim.x < 0)
        {
            newDim.width += newDim.x;
            newDim.x = 0;
        }
        if (newDim.y < 0)
        {
            newDim.height += newDim.y;
            newDim.y = 0;
        }
        if (newDim.x + newDim.width > mainGraphics->mWidth)
            newDim.width = mainGraphics->mWidth - newDim.x;
        if (newDim.y + newDim.height > mainGraphics->mHeight)
            newDim.height = mainGraphics->mHeight - newDim.y;

        // Update mouse offset when dragging bottom or right border
        if (mouseResize & BOTTOM)
            mDragOffsetY += newDim.height - mDimension.height;

        if (mouseResize & RIGHT)
            mDragOffsetX += newDim.width - mDimension.width;

        // Set the new window and content dimensions
        setDimension(newDim);
    }
}

void Window::setModal(const bool modal)
{
    if (mModal != modal)
    {
        mModal = modal;
        if (mModal)
        {
            if (gui)
                gui->setCursorType(Cursor::CURSOR_POINTER);
            requestModalFocus();
        }
        else
        {
            releaseModalFocus();
        }
    }
}

void Window::loadWindowState()
{
    const std::string &name = mWindowName;
    if (name.empty())
        return;

    setPosition(config.getValueInt(name + "WinX", mDefaultX),
        config.getValueInt(name + "WinY", mDefaultY));

    if (mSaveVisible)
    {
        setVisible(config.getValueBool(name
            + "Visible", mDefaultVisible));
    }

    if (mStickyButton)
    {
        setSticky(config.getValueBool(name
            + "Sticky", isSticky()));
    }

    if (mGrip)
    {
        int width = config.getValueInt(name + "WinWidth", mDefaultWidth);
        int height = config.getValueInt(name + "WinHeight", mDefaultHeight);

        if (getMinWidth() > width)
            width = getMinWidth();
        else if (getMaxWidth() < width)
            width = getMaxWidth();
        if (getMinHeight() > height)
            height = getMinHeight();
        else if (getMaxHeight() < height)
            height = getMaxHeight();

        setSize(width, height);
    }
    else
    {
        setSize(mDefaultWidth, mDefaultHeight);
    }

    // Check if the window is off screen...
    ensureOnScreen();

    if (viewport)
    {
        int width = mDimension.width;
        int height = mDimension.height;

        if (mDimension.x + width > viewport->getWidth())
            width = viewport->getWidth() - mDimension.x;
        if (mDimension.y + height > viewport->getHeight())
            height = viewport->getHeight() - mDimension.y;
        if (width < 0)
            width = 0;
        if (height < 0)
            height = 0;
        setSize(width, height);
    }
}

void Window::saveWindowState()
{
    // Saving X, Y and Width and Height for resizables in the config
    if (!mWindowName.empty() && mWindowName != "window")
    {
        config.setValue(mWindowName + "WinX", mDimension.x);
        config.setValue(mWindowName + "WinY", mDimension.y);

        if (mSaveVisible)
            config.setValue(mWindowName + "Visible", isWindowVisible());

        if (mStickyButton)
            config.setValue(mWindowName + "Sticky", isSticky());

        if (mGrip)
        {
            if (getMinWidth() > mDimension.width)
                setWidth(getMinWidth());
            else if (getMaxWidth() < mDimension.width)
                setWidth(getMaxWidth());
            if (getMinHeight() > mDimension.height)
                setHeight(getMinHeight());
            else if (getMaxHeight() < mDimension.height)
                setHeight(getMaxHeight());

            config.setValue(mWindowName + "WinWidth", mDimension.width);
            config.setValue(mWindowName + "WinHeight", mDimension.height);
        }
    }
}

void Window::setDefaultSize(const int defaultX, const int defaultY,
                            int defaultWidth, int defaultHeight)
{
    if (mMinWinWidth > defaultWidth)
        defaultWidth = mMinWinWidth;
    else if (mMaxWinWidth < defaultWidth)
        defaultWidth = mMaxWinWidth;
    if (mMinWinHeight > defaultHeight)
        defaultHeight = mMinWinHeight;
    else if (mMaxWinHeight < defaultHeight)
        defaultHeight = mMaxWinHeight;

    mDefaultX = defaultX;
    mDefaultY = defaultY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::setDefaultSize()
{
    mDefaultX = mDimension.x;
    mDefaultY = mDimension.y;
    mDefaultWidth = mDimension.width;
    mDefaultHeight = mDimension.height;
}

void Window::setDefaultSize(const int defaultWidth, const int defaultHeight,
                            const ImageRect::ImagePosition &position,
                            const int offsetX, const int offsetY)
{
    int x = 0;
    int y = 0;

    if (position == ImageRect::UPPER_LEFT)
    {
    }
    else if (position == ImageRect::UPPER_CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
    }
    else if (position == ImageRect::UPPER_RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
    }
    else if (position == ImageRect::LEFT)
    {
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImageRect::CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImageRect::RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
        y = (mainGraphics->mHeight - defaultHeight) / 2;
    }
    else if (position == ImageRect::LOWER_LEFT)
    {
        y = mainGraphics->mHeight - defaultHeight;
    }
    else if (position == ImageRect::LOWER_CENTER)
    {
        x = (mainGraphics->mWidth - defaultWidth) / 2;
        y = mainGraphics->mHeight - defaultHeight;
    }
    else if (position == ImageRect::LOWER_RIGHT)
    {
        x = mainGraphics->mWidth - defaultWidth;
        y = mainGraphics->mHeight - defaultHeight;
    }

    mDefaultX = x - offsetX;
    mDefaultY = y - offsetY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::resetToDefaultSize()
{
    setPosition(mDefaultX, mDefaultY);
    setSize(mDefaultWidth, mDefaultHeight);
    saveWindowState();
}

void Window::adjustPositionAfterResize(const int oldScreenWidth,
                                       const int oldScreenHeight)
{
    // If window was aligned to the right or bottom, keep it there
    const int rightMargin = oldScreenWidth - (mDimension.x + mDimension.width);
    const int bottomMargin = oldScreenHeight
        - (mDimension.y + mDimension.height);
    if (mDimension.x > 0 && mDimension.x > rightMargin)
        mDimension.x = mainGraphics->mWidth - rightMargin - mDimension.width;
    if (mDimension.y > 0 && mDimension.y > bottomMargin)
    {
        mDimension.y = mainGraphics->mHeight
            - bottomMargin - mDimension.height;
    }

    ensureOnScreen();
    adjustSizeToScreen();
}

void Window::adjustSizeToScreen()
{
    if (!mGrip)
        return;

    const int screenWidth = mainGraphics->mWidth;
    const int screenHeight = mainGraphics->mHeight;
    const int oldWidth = mDimension.width;
    const int oldHeight = mDimension.height;
    if (oldWidth + mDimension.x > screenWidth)
        mDimension.x = 0;
    if (oldHeight + mDimension.y > screenHeight)
        mDimension.x = 0;
    if (mDimension.width > screenWidth)
        mDimension.width = screenWidth;
    if (mDimension.height > screenHeight)
        mDimension.height = screenHeight;
    if (oldWidth != mDimension.width || oldHeight != mDimension.height)
        widgetResized(Event(this));
}

int Window::getResizeHandles(const MouseEvent &event)
{
    if (event.getX() < 0 || event.getY() < 0)
        return 0;

    int resizeHandles = 0;
    const unsigned y = event.getY();
    const unsigned x = event.getX();
    if (mCloseRect.isPointInRect(x, y))
        return CLOSE;

    if (!mStickyButtonLock || !mSticky)
    {
        if (mGrip && (y > mTitleBarHeight || (y < mPadding
            && mTitleBarHeight > mPadding)))
        {
            if (!getWindowArea().isPointInRect(x, y)
                && event.getSource() == this)
            {
                resizeHandles |= (x > mDimension.width - resizeBorderWidth)
                    ? RIGHT : (x < resizeBorderWidth) ? LEFT : 0;
                resizeHandles |= (y > mDimension.height - resizeBorderWidth)
                    ? BOTTOM : (y < resizeBorderWidth) ? TOP : 0;
            }
            if (x >= static_cast<unsigned>(mGripRect.x)
                && y >= static_cast<unsigned>(mGripRect.y))
            {
                mDragOffsetX = x;
                mDragOffsetY = y;
                resizeHandles |= BOTTOM | RIGHT;
            }
        }
    }

    return resizeHandles;
}

bool Window::isResizeAllowed(const MouseEvent &event) const
{
    const int y = event.getY();

    if (mGrip && (y > static_cast<int>(mTitleBarHeight)
        || y < static_cast<int>(mPadding)))
    {
        const int x = event.getX();

        if (!getWindowArea().isPointInRect(x, y) && event.getSource() == this)
            return true;

        if (x >= mGripRect.x && y >= mGripRect.y)
            return true;
    }

    return false;
}

int Window::getGuiAlpha() const
{
    const float alpha = std::max(client->getGuiAlpha(),
        theme->getMinimumOpacity());
    return static_cast<int>(alpha * 255.0F);
}

Layout &Window::getLayout()
{
    if (!mLayout)
        mLayout = new Layout;
    return *mLayout;
}

void Window::clearLayout()
{
    clear();

    // Recreate layout instance when one is present
    if (mLayout)
    {
        delete mLayout;
        mLayout = new Layout;
    }
}

LayoutCell &Window::place(const int x, const int y, Widget *const wg,
                          const int w, const int h)
{
    add(wg);
    return getLayout().place(wg, x, y, w, h);
}

ContainerPlacer Window::getPlacer(const int x, const int y)
{
    return ContainerPlacer(this, &getLayout().at(x, y));
}

void Window::reflowLayout(int w, int h)
{
    if (!mLayout)
        return;

    mLayout->reflow(w, h);
    delete2(mLayout);
    setContentSize(w, h);
}

void Window::redraw()
{
    if (mLayout)
    {
        const Rect area = getChildrenArea();
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
}

void Window::center()
{
    setLocationRelativeTo(getParent());
}

void Window::centerHorisontally()
{
    setLocationHorisontallyRelativeTo(getParent());
}

void Window::ensureOnScreen()
{
    // Skip when a window hasn't got any size initialized yet
    if (mDimension.width == 0 && mDimension.height == 0)
        return;

    // Check the left and bottom screen boundaries
    if (mDimension.x + mDimension.width > mainGraphics->mWidth)
        mDimension.x = mainGraphics->mWidth - mDimension.width;
    if (mDimension.y + mDimension.height > mainGraphics->mHeight)
        mDimension.y = mainGraphics->mHeight - mDimension.height;

    // But never allow the windows to disappear in to the right and top
    if (mDimension.x < 0)
        mDimension.x = 0;
    if (mDimension.y < 0)
        mDimension.y = 0;
}

Rect Window::getWindowArea() const
{
    return Rect(mPadding,
        mPadding,
        mDimension.width - mPadding * 2,
        mDimension.height - mPadding * 2);
}

int Window::getOption(const std::string &name, const int def) const
{
    if (mSkin)
    {
        const int val = mSkin->getOption(name);
        if (val)
            return val;
        return def;
    }
    return def;
}

bool Window::getOptionBool(const std::string &name, const bool def) const
{
    if (mSkin)
        return mSkin->getOption(name, def) != 0;
    return def;
}

Rect Window::getChildrenArea()
{
    return Rect(mPadding,
        mTitleBarHeight,
        mDimension.width - mPadding * 2,
        mDimension.height - mPadding - mTitleBarHeight);
}

void Window::resizeToContent()
{
    int w = 0;
    int h = 0;
    for (WidgetListConstIterator it = mWidgets.begin();
          it != mWidgets.end(); ++ it)
    {
        const Widget *const widget = *it;
        const int x = widget->getX();
        const int y = widget->getY();
        const int width = widget->getWidth();
        const int height = widget->getHeight();
        if (x + width > w)
            w = x + width;

        if (y + height > h)
            h = y + height;
    }

    setSize(w + 2 * mPadding,
        h + mPadding + mTitleBarHeight);
}

#ifdef USE_PROFILER
void Window::logic()
{
    BLOCK_START("Window::logic")
    logicChildren();
    BLOCK_END("Window::logic")
}
#endif
