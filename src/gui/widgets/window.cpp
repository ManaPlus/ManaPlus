/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "gui/widgets/windowcontainer.h"

#include "resources/image.h"

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include "debug.h"

int Window::instances = 0;
int Window::mouseResize = 0;

Window::Window(const std::string &caption, bool modal, Window *parent,
               std::string skin):
    gcn::Window(caption),
    mGrip(0),
    mParent(parent),
    mLayout(NULL),
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
    mVertexes(new GraphicsVertexes()),
    mRedraw(true)
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
        mSkin = Theme::instance()->load(skin);
        if (mSkin)
            setPadding(mSkin->getPadding());
    }
    else
    {
        mSkin = 0;
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
    mCaptionFont = getFont();
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
}

Window::~Window()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());

    saveWindowState();

    delete mLayout;
    mLayout = 0;

    while (!mWidgets.empty())
        delete mWidgets.front();

    mWidgets.clear();

    removeWidgetListener(this);
    delete mVertexes;
    mVertexes = 0;

    instances--;

    if (mSkin)
    {
        if (Theme::instance())
            Theme::instance()->unload(mSkin);
        mSkin = 0;
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

    Graphics *g = static_cast<Graphics*>(graphics);
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
        g->drawText(getCaption(), 7, 5, gcn::Graphics::LEFT);
    }

    // Draw Close Button
    if (mCloseButton && mSkin->getCloseImage())
    {
        Image *button = mSkin->getCloseImage();
        const int x = getWidth() - button->getWidth() - getPadding();
        g->drawImage(button, x, getPadding());
    }

    // Draw Sticky Button
    if (mStickyButton)
    {
        Image *button = mSkin->getStickyImage(mSticky);
        if (button)
        {
            int x = getWidth() - button->getWidth() - getPadding();
            if (mCloseButton && mSkin->getCloseImage())
                x -= mSkin->getCloseImage()->getWidth() + getPadding();

            g->drawImage(button, x, getPadding());
        }
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

void Window::setLocationRelativeTo(gcn::Widget *widget)
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

void Window::setLocationHorisontallyRelativeTo(gcn::Widget *widget)
{
    if (!widget)
        return;

    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x), 0);
}

void Window::setLocationRelativeTo(ImageRect::ImagePosition position,
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

void Window::setMaxWidth(int width)
{
    mMaxWinWidth = width;
}

void Window::setMaxHeight(int height)
{
    mMaxWinHeight = height;
}

void Window::setResizable(bool r)
{
    if (static_cast<bool>(mGrip) == r)
        return;

    if (r)
    {
        mGrip = new ResizeGrip;
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
        add(mGrip);
    }
    else
    {
        remove(mGrip);
        delete mGrip;
        mGrip = 0;
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

    WidgetListConstIterator it;

    if (!mFocusHandler)
        return;

    for (it = mWidgets.begin(); it != mWidgets.end(); ++ it)
    {
        if (mFocusHandler->isFocused(*it))
            mFocusHandler->focusNone();
    }
}

void Window::setCloseButton(bool flag)
{
    mCloseButton = flag;
}

bool Window::isResizable() const
{
    return mGrip;
}

void Window::setStickyButton(bool flag)
{
    mStickyButton = flag;
}

void Window::setSticky(bool sticky)
{
    mSticky = sticky;
}

void Window::setStickyButtonLock(bool lock)
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
        checkIfIsOffScreen();

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
        if (mCloseButton && mSkin)
        {
            Image *img = mSkin->getCloseImage();
            if (img)
            {
                gcn::Rectangle closeButtonRect(
                    getWidth() - img->getWidth()
                    - getPadding(), getPadding(),
                    img->getWidth(), img->getHeight());

                if (closeButtonRect.isPointInRect(x, y))
                {
                    mouseResize = 0;
                    mMoved = 0;
                    close();
                    return;
                }
            }
        }

        // Handle sticky button
        if (mStickyButton && mSkin)
        {
            Image *button = mSkin->getStickyImage(mSticky);
            if (button)
            {
                int rx = getWidth() - button->getWidth() - getPadding();
                if (mCloseButton)
                {
                    Image *img = mSkin->getCloseImage();
                    if (img)
                        rx -= img->getWidth();
                }
                gcn::Rectangle stickyButtonRect(rx, getPadding(),
                    button->getWidth(), button->getHeight());
                if (stickyButtonRect.isPointInRect(x, y))
                {
                    setSticky(!isSticky());
                    mouseResize = 0;
                    mMoved = 0;
                    return;
                }
            }
        }

        // Handle window resizing
        mouseResize = getResizeHandles(event);
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

    int resizeHandles = getResizeHandles(event);

    // Changes the custom mouse cursor based on it's current position.
    switch (resizeHandles)
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

bool Window::canMove()
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
            int newHeight = newDim.height + ((mouseResize & TOP) ? -dy : dy);
            newDim.height = std::min(mMaxWinHeight,
                                     std::max(mMinWinHeight, newHeight));

            if (mouseResize & TOP)
                newDim.y -= newDim.height - getHeight();
        }

        if (mouseResize & (LEFT | RIGHT))
        {
            int newWidth = newDim.width + ((mouseResize & LEFT) ? -dx : dx);
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
    checkIfIsOffScreen();

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
                            ImageRect::ImagePosition position,
                            int offsetX, int offsetY)
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

int Window::getResizeHandles(gcn::MouseEvent &event)
{
    if ((mStickyButtonLock && mSticky) || event.getX() < 0 || event.getY() < 0)
        return 0;

    int resizeHandles = 0;
    const unsigned y = event.getY();

    if (mGrip && (y > mTitleBarHeight || (y < getPadding()
        && mTitleBarHeight > getPadding())))
    {
        const unsigned x = event.getX();

        if (!getWindowArea().isPointInRect(x, y) && event.getSource() == this)
        {
            resizeHandles |= (x > getWidth() - resizeBorderWidth) ? RIGHT :
                              (x < resizeBorderWidth) ? LEFT : 0;
            resizeHandles |= (y > getHeight() - resizeBorderWidth) ? BOTTOM :
                              (y < resizeBorderWidth) ? TOP : 0;
        }

        if (event.getSource() == mGrip)
        {
            mDragOffsetX = x;
            mDragOffsetY = y;
            resizeHandles |= BOTTOM | RIGHT;
        }
    }

    return resizeHandles;
}

bool Window::isResizeAllowed(gcn::MouseEvent &event)
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
    float alpha = std::max(Client::getGuiAlpha(),
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

LayoutCell &Window::place(int x, int y, gcn::Widget *wg, int w, int h)
{
    add(wg);
    return getLayout().place(wg, x, y, w, h);
}

ContainerPlacer Window::getPlacer(int x, int y)
{
    return ContainerPlacer(this, &getLayout().at(x, y));
}

void Window::reflowLayout(int w, int h)
{
    if (!mLayout)
        return;

    mLayout->reflow(w, h);
    delete mLayout;
    mLayout = 0;
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

void Window::checkIfIsOffScreen(bool partially, bool entirely)
{
    // Move the window onto screen if it has become off screen
    // For instance, because of resolution change...

    // First of all, don't deal when a window hasn't got
    // any size initialized yet...
    if (getWidth() == 0 && getHeight() == 0)
        return;

    // Made partially the default behaviour
    if (!partially && !entirely)
        partially = true;

  // Keep guichan window inside screen (supports resizing any side)

    gcn::Rectangle winDimension =  getDimension();

    if (winDimension.x < 0)
    {
        winDimension.width += winDimension.x;
        winDimension.x = 0;
    }
    if (winDimension.y < 0)
    {
        winDimension.height += winDimension.y;
        winDimension.y = 0;
    }

    // Look if the window is partially off-screen limits...
    if (partially)
    {
        if (winDimension.x + winDimension.width > mainGraphics->mWidth)
            winDimension.x = mainGraphics->mWidth - winDimension.width;

        if (winDimension.y + winDimension.height > mainGraphics->mHeight)
            winDimension.y = mainGraphics->mHeight - winDimension.height;

        setDimension(winDimension);
        return;
    }

    if (entirely)
    {
        if (winDimension.x > mainGraphics->mWidth)
            winDimension.x = mainGraphics->mWidth - winDimension.width;

        if (winDimension.y > mainGraphics->mHeight)
            winDimension.y = mainGraphics->mHeight - winDimension.height;
    }
    setDimension(winDimension);
}

gcn::Rectangle Window::getWindowArea()
{
    return gcn::Rectangle(getPadding(),
                          getPadding(),
                          getWidth() - getPadding() * 2,
                          getHeight() - getPadding() * 2);
}