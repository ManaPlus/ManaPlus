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

#include "gui/widgets/window.h"

#include "client.h"
#include "configuration.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/layout.h"
#include "gui/widgets/resizegrip.h"

#include "resources/image.h"

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include "debug.h"

const int resizeMask = 8 + 4 + 2 + 1;

int Window::instances = 0;
int Window::mouseResize = 0;

Window::Window(const std::string &caption, const bool modal,
               Window *const parent, std::string skin) :
    gcn::Window(caption),
    gcn::WidgetListener(),
    mSkin(nullptr),
    mGrip(nullptr),
    mParent(parent),
    mLayout(nullptr),
    mWindowName("window"),
    mShowTitle(true),
    mModal(modal),
    mCloseButton(false),
    mDefaultVisible(false),
    mSaveVisible(false),
    mStickyButton(false),
    mSticky(false),
    mStickyButtonLock(false),
    mMinWinWidth(100),
    mMinWinHeight(40),
    mMaxWinWidth(mainGraphics->mWidth),
    mMaxWinHeight(mainGraphics->mHeight),
    mDefaultX(0),
    mDefaultY(0),
    mDefaultWidth(0),
    mDefaultHeight(0),
    mVertexes(new GraphicsVertexes()),
    mCaptionOffsetX(7),
    mCaptionOffsetY(5),
    mCaptionAlign(gcn::Graphics::LEFT),
    mTitlePadding(4),
    mResizeHandles(-1),
    mRedraw(true),
    mCaptionFont(getFont())
{
    logger->log("Window::Window(\"%s\")", caption.c_str());

    if (!windowContainer)
        throw GCN_EXCEPTION("Window::Window(): no windowContainer set");

    instances++;

    setFrameSize(0);
    setPadding(3);
    setTitleBarHeight(20);

    if (skin == "")
        skin = "window.xml";

    // Loads the skin
    if (Theme::instance())
    {
        mSkin = Theme::instance()->load(skin, "window.xml");
        if (mSkin)
        {
            setPadding(mSkin->getPadding());
            setTitlePadding(mSkin->getTitlePadding());
            setTitleBarHeight(getOption("titlebarHeight"));
            mCaptionOffsetX = getOption("captionoffsetx");
            if (!mCaptionOffsetX)
                mCaptionOffsetX = 7;
            mCaptionOffsetY = getOption("captionoffsety");
            if (!mCaptionOffsetY)
                mCaptionOffsetY = 5;
            mCaptionAlign = getOption("captionalign");
            if (mCaptionAlign < gcn::Graphics::LEFT
                || mCaptionAlign > gcn::Graphics::RIGHT)
            {
                mCaptionAlign = gcn::Graphics::LEFT;
            }
        }
    }

    // Add this window to the window container
    windowContainer->add(this);

    if (mModal)
    {
        gui->setCursorType(Gui::CURSOR_POINTER);
        requestModalFocus();
    }

    // Windows are invisible by default
    setVisible(false);

    addWidgetListener(this);
    setForegroundColor(Theme::getThemeColor(Theme::WINDOW));
}

Window::~Window()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());

    saveWindowState();

    delete mLayout;
    mLayout = nullptr;

    while (!mWidgets.empty())
        delete mWidgets.front();

    mWidgets.clear();

    removeWidgetListener(this);
    delete mVertexes;
    mVertexes = nullptr;

    instances--;

    if (mSkin)
    {
        if (Theme::instance())
            Theme::instance()->unload(mSkin);
        mSkin = nullptr;
    }
}

void Window::setWindowContainer(WindowContainer *wc)
{
    windowContainer = wc;
}

void Window::draw(gcn::Graphics *graphics)
{
    if (!mSkin)
        return;

    Graphics *const g = static_cast<Graphics*>(graphics);
    bool update = false;

    if (mRedraw)
    {
        mRedraw = false;
        update = true;
        g->calcWindow(mVertexes, 0, 0, getWidth(),
            getHeight(), mSkin->getBorder());
    }

    g->drawImageRect2(mVertexes, mSkin->getBorder());

    // Draw title
    if (mShowTitle)
    {
        g->setColor(getForegroundColor());
        g->setFont(mCaptionFont);
        g->drawText(getCaption(), mCaptionOffsetX, mCaptionOffsetY,
            static_cast<gcn::Graphics::Alignment>(mCaptionAlign));
    }

    // Draw Close Button
    if (mCloseButton)
    {
        const Image *const button = mSkin->getCloseImage(
            mResizeHandles == CLOSE);
        if (button)
            g->drawImage(button, mCloseRect.x, mCloseRect.y);
    }
    // Draw Sticky Button
    if (mStickyButton)
    {
        const Image *const button = mSkin->getStickyImage(mSticky);
        if (button)
            g->drawImage(button, mStickyRect.x, mStickyRect.y);
    }

    if (update)
    {
        g->setRedraw(update);
        drawChildren(graphics);
        g->setRedraw(false);
    }
    else
    {
        drawChildren(graphics);
    }
}

void Window::setContentSize(int width, int height)
{
    width = width + 2 * getPadding();
    height = height + getPadding() + getTitleBarHeight();

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

void Window::setLocationRelativeTo(const gcn::Widget *const widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x),
                getY() + (wy + (widget->getHeight() - getHeight()) / 2 - y));
}

void Window::setLocationHorisontallyRelativeTo(const gcn::Widget *const widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x), 0);
}

void Window::setLocationRelativeTo(const ImageRect::ImagePosition position,
                                   int offsetX, int offsetY)
{
    if (position == ImageRect::UPPER_LEFT)
    {
    }
    else if (position == ImageRect::UPPER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - getWidth()) / 2;
    }
    else if (position == ImageRect::UPPER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - getWidth();
    }
    else if (position == ImageRect::LEFT)
    {
        offsetY += (mainGraphics->mHeight - getHeight()) / 2;
    }
    else if (position == ImageRect::CENTER)
    {
        offsetX += (mainGraphics->mWidth - getWidth()) / 2;
        offsetY += (mainGraphics->mHeight - getHeight()) / 2;
    }
    else if (position == ImageRect::RIGHT)
    {
        offsetX += mainGraphics->mWidth - getWidth();
        offsetY += (mainGraphics->mHeight - getHeight()) / 2;
    }
    else if (position == ImageRect::LOWER_LEFT)
    {
        offsetY += mainGraphics->mHeight - getHeight();
    }
    else if (position == ImageRect::LOWER_CENTER)
    {
        offsetX += (mainGraphics->mWidth - getWidth()) / 2;
        offsetY += mainGraphics->mHeight - getHeight();
    }
    else if (position == ImageRect::LOWER_RIGHT)
    {
        offsetX += mainGraphics->mWidth - getWidth();
        offsetY += mainGraphics->mHeight - getHeight();
    }

    setPosition(offsetX, offsetY);
}

void Window::setMinWidth(int width)
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

void Window::setMinHeight(int height)
{
    if (mSkin)
    {
        mMinWinHeight = height > mSkin->getMinHeight() ?
                        height : mSkin->getMinHeight();
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
    if (static_cast<bool>(mGrip) == r)
        return;

    if (r)
    {
        if (mGrip)
        {
            remove(mGrip);
            delete mGrip;
        }
        mGrip = new ResizeGrip;
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
        add(mGrip);
    }
    else
    {
        remove(mGrip);
        delete mGrip;
        mGrip = nullptr;
    }
}

void Window::widgetResized(const gcn::Event &event A_UNUSED)
{
    const gcn::Rectangle area = getChildrenArea();

    if (mGrip)
    {
        mGrip->setPosition(getWidth() - mGrip->getWidth() - area.x,
                           getHeight() - mGrip->getHeight() - area.y);
    }

    if (mLayout)
    {
        int w = area.width;
        int h = area.height;
        mLayout->reflow(w, h);
    }
    const bool showClose = mCloseButton && mSkin->getCloseImage(false);
    const int closePadding = getOption("closePadding");
    if (showClose)
    {
        const Image *const button = mSkin->getCloseImage(false);
        const int x = getWidth() - button->getWidth() - closePadding;
        mCloseRect.x = x;
        mCloseRect.y = closePadding;
        mCloseRect.width = button->getWidth();
        mCloseRect.height = button->getHeight();
    }
    if (mStickyButton)
    {
        const Image *const button = mSkin->getStickyImage(mSticky);
        if (button)
        {
            int x = getWidth() - button->getWidth() - closePadding;
            if (showClose)
                x -= mSkin->getCloseImage(false)->getWidth() + closePadding;

            mStickyRect.x = x;
            mStickyRect.y = closePadding;
            mStickyRect.width = button->getWidth();
            mStickyRect.height = button->getHeight();
        }
    }

    mRedraw = true;
}

void Window::widgetMoved(const gcn::Event& event A_UNUSED)
{
    mRedraw = true;
}

void Window::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (gui)
        gui->setCursorType(Gui::CURSOR_POINTER);

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
    mCloseButton = flag;
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
}

void Window::setStickyButtonLock(const bool lock)
{
    mStickyButtonLock = lock;
    mStickyButton = lock;
//    mMovable = false;
}

void Window::setVisible(bool visible)
{
    setVisible(visible, false);
}

void Window::setVisible(bool visible, bool forceSticky)
{
    if (visible == isVisible())
        return; // Nothing to do

    // Check if the window is off screen...
    if (visible)
        ensureOnScreen();

    if (isStickyButtonLock())
        gcn::Window::setVisible(visible);
    else
        gcn::Window::setVisible((!forceSticky && isSticky()) || visible);
}

void Window::scheduleDelete()
{
    windowContainer->scheduleDelete(this);
}

void Window::mousePressed(gcn::MouseEvent &event)
{
    // Let Guichan move window to top and figure out title bar drag
    gcn::Window::mousePressed(event);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

        // Handle close button
        if (mCloseButton && mSkin && mCloseRect.isPointInRect(x, y))
        {
            mouseResize = 0;
            mMoved = 0;
            close();
            return;
        }

        // Handle sticky button
        if (mStickyButton && mSkin && mStickyRect.isPointInRect(x, y))
        {
            setSticky(!isSticky());
            mouseResize = 0;
            mMoved = 0;
            return;
        }

        // Handle window resizing
        mouseResize = getResizeHandles(event) & resizeMask;
        if (canMove())
            mMoved = !mouseResize;
        else
            mMoved = false;
    }
}

void Window::close()
{
    setVisible(false);
}

void Window::mouseReleased(gcn::MouseEvent &event A_UNUSED)
{
    if (mGrip && mouseResize)
    {
        mouseResize = 0;
        if (gui)
            gui->setCursorType(Gui::CURSOR_POINTER);
    }

    // This should be the responsibility of Guichan (and is from 0.8.0 on)
    mMoved = false;
}

void Window::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mGrip && !mouseResize && gui)
        gui->setCursorType(Gui::CURSOR_POINTER);
}

void Window::mouseMoved(gcn::MouseEvent &event)
{
    if (!gui)
        return;

    mResizeHandles = getResizeHandles(event);

    // Changes the custom mouse cursor based on it's current position.
    switch (mResizeHandles & resizeMask)
    {
        case BOTTOM | RIGHT:
        case TOP | LEFT:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN_RIGHT);
            break;
        case TOP | RIGHT:
        case BOTTOM | LEFT:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN_LEFT);
            break;
        case BOTTOM:
        case TOP:
            gui->setCursorType(Gui::CURSOR_RESIZE_DOWN);
            break;
        case RIGHT:
        case LEFT:
            gui->setCursorType(Gui::CURSOR_RESIZE_ACROSS);
            break;
        default:
            gui->setCursorType(Gui::CURSOR_POINTER);
    }

    if (viewport)
        viewport->hideBeingPopup();
}

bool Window::canMove() const
{
    return !mStickyButtonLock || !mSticky;
}

void Window::mouseDragged(gcn::MouseEvent &event)
{
    if (canMove())
    {
        // Let Guichan handle title bar drag
        gcn::Window::mouseDragged(event);
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
        int newX = std::max(0, getX());
        int newY = std::max(0, getY());
        newX = std::min(mainGraphics->mWidth - getWidth(), newX);
        newY = std::min(mainGraphics->mHeight - getHeight(), newY);
        setPosition(newX, newY);
    }

    if (mouseResize && !mMoved)
    {
        const int dx = event.getX() - mDragOffsetX;
        const int dy = event.getY() - mDragOffsetY;
        gcn::Rectangle newDim = getDimension();

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
                newDim.x -= newDim.width - getWidth();
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
            mDragOffsetY += newDim.height - getHeight();

        if (mouseResize & RIGHT)
            mDragOffsetX += newDim.width - getWidth();

        // Set the new window and content dimensions
        setDimension(newDim);
    }
}

void Window::setModal(bool modal)
{
    if (mModal != modal)
    {
        mModal = modal;
        if (mModal)
        {
            if (gui)
                gui->setCursorType(Gui::CURSOR_POINTER);
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
        int width = getWidth();
        int height = getHeight();

        if (getX() + width > viewport->getWidth())
            width = viewport->getWidth() - getX();
        if (getY() + height > viewport->getHeight())
            height = viewport->getHeight() - getY();
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
        config.setValue(mWindowName + "WinX", getX());
        config.setValue(mWindowName + "WinY", getY());

        if (mSaveVisible)
            config.setValue(mWindowName + "Visible", isVisible());

        if (mStickyButton)
            config.setValue(mWindowName + "Sticky", isSticky());

        if (mGrip)
        {
            if (getMinWidth() > getWidth())
                setWidth(getMinWidth());
            else if (getMaxWidth() < getWidth())
                setWidth(getMaxWidth());
            if (getMinHeight() > getHeight())
                setHeight(getMinHeight());
            else if (getMaxHeight() < getHeight())
                setHeight(getMaxHeight());

            config.setValue(mWindowName + "WinWidth", getWidth());
            config.setValue(mWindowName + "WinHeight", getHeight());
        }
    }
}

void Window::setDefaultSize(int defaultX, int defaultY,
                            int defaultWidth, int defaultHeight)
{
    if (getMinWidth() > defaultWidth)
        defaultWidth = getMinWidth();
    else if (getMaxWidth() < defaultWidth)
        defaultWidth = getMaxWidth();
    if (getMinHeight() > defaultHeight)
        defaultHeight = getMinHeight();
    else if (getMaxHeight() < defaultHeight)
        defaultHeight = getMaxHeight();

    mDefaultX = defaultX;
    mDefaultY = defaultY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;
}

void Window::setDefaultSize()
{
    mDefaultX = getX();
    mDefaultY = getY();
    mDefaultWidth = getWidth();
    mDefaultHeight = getHeight();
}

void Window::setDefaultSize(int defaultWidth, int defaultHeight,
                            const ImageRect::ImagePosition position,
                            const int offsetX, const int offsetY)
{
    int x = 0, y = 0;

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
    gcn::Rectangle dimension = getDimension();

    // If window was aligned to the right or bottom, keep it there
    const int rightMargin = oldScreenWidth - (getX() + getWidth());
    const int bottomMargin = oldScreenHeight - (getY() + getHeight());
    if (getX() > 0 && getX() > rightMargin)
        dimension.x = mainGraphics->mWidth - rightMargin - getWidth();
    if (getY() > 0 && getY() > bottomMargin)
        dimension.y = mainGraphics->mHeight - bottomMargin - getHeight();

    setDimension(dimension);
    ensureOnScreen();
}

int Window::getResizeHandles(const gcn::MouseEvent &event)
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
        if (mGrip && (y > mTitleBarHeight || (y < getPadding()
            && mTitleBarHeight > getPadding())))
        {
            if (!getWindowArea().isPointInRect(x, y)
                && event.getSource() == this)
            {
                resizeHandles |= (x > getWidth() - resizeBorderWidth)
                    ? RIGHT : (x < resizeBorderWidth) ? LEFT : 0;
                resizeHandles |= (y > getHeight() - resizeBorderWidth)
                    ? BOTTOM : (y < resizeBorderWidth) ? TOP : 0;
            }

            if (event.getSource() == mGrip)
            {
                mDragOffsetX = x;
                mDragOffsetY = y;
                resizeHandles |= BOTTOM | RIGHT;
            }
        }
    }

    return resizeHandles;
}

bool Window::isResizeAllowed(const gcn::MouseEvent &event) const
{
    const int y = event.getY();

    if (mGrip && (y > static_cast<int>(mTitleBarHeight)
        || y < static_cast<int>(getPadding())))
    {
        const int x = event.getX();

        if (!getWindowArea().isPointInRect(x, y) && event.getSource() == this)
            return true;

        if (event.getSource() == mGrip)
            return true;
    }

    return false;
}

int Window::getGuiAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());
    return static_cast<int>(alpha * 255.0f);
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

    // Restore the resize grip
    if (mGrip)
        add(mGrip);

    // Recreate layout instance when one is present
    if (mLayout)
    {
        delete mLayout;
        mLayout = new Layout;
    }
}

LayoutCell &Window::place(const int x, const int y, gcn::Widget *const wg,
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
    delete mLayout;
    mLayout = nullptr;
    setContentSize(w, h);
}

void Window::redraw()
{
    if (mLayout)
    {
        const gcn::Rectangle area = getChildrenArea();
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
    if (getWidth() == 0 && getHeight() == 0)
        return;

    gcn::Rectangle dimension = getDimension();

    // Check the left and bottom screen boundaries
    if (dimension.x + dimension.width > mainGraphics->mWidth)
        dimension.x = mainGraphics->mWidth - dimension.width;
    if (dimension.y + dimension.height > mainGraphics->mHeight)
        dimension.y = mainGraphics->mHeight - dimension.height;

    // But never allow the windows to disappear in to the right and top
    if (dimension.x < 0)
        dimension.x = 0;
    if (dimension.y < 0)
        dimension.y = 0;

    setDimension(dimension);
}

gcn::Rectangle Window::getWindowArea() const
{
    return gcn::Rectangle(getPadding(),
                          getPadding(),
                          getWidth() - getPadding() * 2,
                          getHeight() - getPadding() * 2);
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

bool Window::getOptionBool(std::string name)
{
    if (mSkin)
        return mSkin->getOption(name) != 0;
    return 0;
}
