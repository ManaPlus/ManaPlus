/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 * Copyright (C) 2011-2014  The ManaPlus Developers
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

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widget.hpp"

#include "guichan/actionevent.hpp"
#include "guichan/actionlistener.hpp"
#include "guichan/basiccontainer.hpp"
#include "guichan/deathlistener.hpp"
#include "guichan/event.hpp"
#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/widgetlistener.hpp"

#include "debug.h"

namespace gcn
{
    Font* Widget::mGlobalFont = nullptr;
    std::list<Widget*> Widget::mWidgets;
    std::set<Widget*> Widget::mWidgetsSet;

    Widget::Widget() :
        mMouseListeners(),
        mKeyListeners(),
        mActionListeners(),
        mDeathListeners(),
        mFocusListeners(),
        mWidgetListeners(),
        mForegroundColor(0x000000),
        mBackgroundColor(0xffffff),
        mBaseColor(0x808090),
        mSelectionColor(0xc3d9ff),
        mFocusHandler(nullptr),
        mInternalFocusHandler(nullptr),
        mParent(nullptr),
        mDimension(),
        mFrameSize(0),
        mActionEventId(),
        mFocusable(false),
        mVisible(true),
        mTabIn(true),
        mTabOut(true),
        mEnabled(true),
        mId(),
        mCurrentFont(nullptr)
    {
        mWidgets.push_back(this);
        mWidgetsSet.insert(this);
    }

    Widget::~Widget()
    {
        for (DeathListenerIterator iter = mDeathListeners.begin();
             iter != mDeathListeners.end();
             ++iter)
        {
            Event event(this);
            (*iter)->death(event);
        }

        _setFocusHandler(nullptr);

        mWidgets.remove(this);
        mWidgetsSet.erase(this);
    }

    void Widget::drawFrame(Graphics* graphics)
    {
        BLOCK_START("Widget::drawFrame")
        const Color &faceColor = getBaseColor();
        Color highlightColor = faceColor + Color(0x303030);
        Color shadowColor = faceColor - Color(0x303030);
        const int alpha = getBaseColor().a;
        const int width = getWidth() + getFrameSize() * 2 - 1;
        const int height = getHeight() + getFrameSize() * 2 - 1;
        highlightColor.a = alpha;
        shadowColor.a = alpha;

        for (unsigned int i = 0; i < getFrameSize(); ++i)
        {
            graphics->setColor(shadowColor);
            graphics->drawLine(i, i, width - i, i);
            graphics->drawLine(i, i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i, i + 1, width - i, height - i);
            graphics->drawLine(i, height - i, width - i - 1, height - i);
        }
        BLOCK_END("Widget::drawFrame")
    }

    void Widget::_setParent(Widget* parent)
    {
        mParent = parent;
    }

    void Widget::setWidth(int width)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;

        setDimension(newDimension);
    }

    void Widget::setHeight(int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.height = height;

        setDimension(newDimension);
    }

    void Widget::setX(int x)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;

        setDimension(newDimension);
    }

    void Widget::setY(int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.y = y;

        setDimension(newDimension);
    }

    void Widget::setPosition(int x, int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;
        newDimension.y = y;

        setDimension(newDimension);
    }

    void Widget::setDimension(const Rectangle& dimension)
    {
        const Rectangle oldDimension = mDimension;
        mDimension = dimension;

        if (mDimension.width != oldDimension.width
            || mDimension.height != oldDimension.height)
        {
            distributeResizedEvent();
        }

        if (mDimension.x != oldDimension.x
            || mDimension.y != oldDimension.y)
        {
            distributeMovedEvent();
        }
    }

    void Widget::setFrameSize(unsigned int frameSize)
    {
        mFrameSize = frameSize;
    }

    unsigned int Widget::getFrameSize() const
    {
        return mFrameSize;
    }

    const Rectangle& Widget::getDimension() const
    {
        return mDimension;
    }

    const std::string& Widget::getActionEventId() const
    {
        return mActionEventId;
    }

    void Widget::setActionEventId(const std::string& actionEventId)
    {
        mActionEventId = actionEventId;
    }

    bool Widget::isFocused() const
    {
        if (!mFocusHandler)
            return false;

        return (mFocusHandler->isFocused(this));
    }

    void Widget::setFocusable(bool focusable)
    {
        if (!focusable && isFocused())
        {
            mFocusHandler->focusNone();
        }

        mFocusable = focusable;
    }

    bool Widget::isFocusable() const
    {
        return mFocusable && isVisible() && isEnabled();
    }

    void Widget::requestFocus()
    {
        if (!mFocusHandler)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add "
                "the widget to the gui?).");
        }

        if (isFocusable())
            mFocusHandler->requestFocus(this);
    }

    void Widget::requestMoveToTop()
    {
        if (mParent)
            mParent->moveToTop(this);
    }

    void Widget::requestMoveToBottom()
    {
        if (mParent)
            mParent->moveToBottom(this);
    }

    void Widget::setVisible(bool visible)
    {
        if (!visible && isFocused())
            mFocusHandler->focusNone();

        if (visible)
            distributeShownEvent();
        else
            distributeHiddenEvent();

        mVisible = visible;
    }

    void Widget::setBaseColor(const Color& color)
    {
        mBaseColor = color;
    }

    const Color& Widget::getBaseColor() const
    {
        return mBaseColor;
    }

    void Widget::setForegroundColor(const Color& color)
    {
        mForegroundColor = color;
    }

    const Color& Widget::getForegroundColor() const
    {
        return mForegroundColor;
    }

    void Widget::setBackgroundColor(const Color& color)
    {
        mBackgroundColor = color;
    }

    const Color& Widget::getBackgroundColor() const
    {
        return mBackgroundColor;
    }

    void Widget::setSelectionColor(const Color& color)
    {
        mSelectionColor = color;
    }

    const Color& Widget::getSelectionColor() const
    {
        return mSelectionColor;
    }

    void Widget::_setFocusHandler(FocusHandler* focusHandler)
    {
        if (mFocusHandler)
        {
            releaseModalFocus();
            mFocusHandler->remove(this);
        }

        if (focusHandler)
            focusHandler->add(this);

        mFocusHandler = focusHandler;
    }

    FocusHandler* Widget::_getFocusHandler()
    {
        return mFocusHandler;
    }

    void Widget::addActionListener(ActionListener* actionListener)
    {
        mActionListeners.push_back(actionListener);
    }

    void Widget::removeActionListener(ActionListener* actionListener)
    {
        mActionListeners.remove(actionListener);
    }

    void Widget::addDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.push_back(deathListener);
    }

    void Widget::removeDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.remove(deathListener);
    }

    void Widget::addKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Widget::removeKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Widget::addFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.push_back(focusListener);
    }

    void Widget::removeFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.remove(focusListener);
    }

    void Widget::addMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.push_back(mouseListener);
    }

    void Widget::removeMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.remove(mouseListener);
    }

    void Widget::addWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.push_back(widgetListener);
    }

    void Widget::removeWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.remove(widgetListener);
    }

    void Widget::getAbsolutePosition(int& x, int& y) const
    {
        if (!mParent)
        {
            x = mDimension.x;
            y = mDimension.y;
            return;
        }

        int parentX;
        int parentY;

        mParent->getAbsolutePosition(parentX, parentY);

        const Rectangle &rect = mParent->getChildrenArea();
        x = parentX + mDimension.x + rect.x;
        y = parentY + mDimension.y + rect.y;
    }

    Font* Widget::getFont() const
    {
        if (!mCurrentFont)
            return mGlobalFont;
        return mCurrentFont;
    }

    void Widget::setGlobalFont(Font* font)
    {
        mGlobalFont = font;

        for (std::list<Widget*>::const_iterator iter = mWidgets.begin();
             iter != mWidgets.end(); ++iter)
        {
            if (!(*iter)->mCurrentFont)
                (*iter)->fontChanged();
        }
    }

    void Widget::setFont(Font* font)
    {
        mCurrentFont = font;
        fontChanged();
    }

    bool Widget::widgetExists(const Widget* widget)
    {
        return mWidgetsSet.find(const_cast<Widget*>(widget))
            != mWidgetsSet.end();
    }

    bool Widget::isTabInEnabled() const
    {
        return mTabIn;
    }

    void Widget::setTabInEnabled(bool enabled)
    {
        mTabIn = enabled;
    }

    bool Widget::isTabOutEnabled() const
    {
        return mTabOut;
    }

    void Widget::setTabOutEnabled(bool enabled)
    {
        mTabOut = enabled;
    }

    void Widget::setSize(int width, int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;
        newDimension.height = height;

        setDimension(newDimension);
    }

    void Widget::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool Widget::isEnabled() const
    {
        return mEnabled && isVisible();
    }

    void Widget::requestModalFocus()
    {
        if (!mFocusHandler)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add "
                "the widget to the gui?).");
        }

        mFocusHandler->requestModalFocus(this);
    }

    void Widget::requestModalMouseInputFocus()
    {
        if (!mFocusHandler)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add "
                "the widget to the gui?).");
        }

        mFocusHandler->requestModalMouseInputFocus(this);
    }

    void Widget::releaseModalFocus()
    {
        if (!mFocusHandler)
            return;

        mFocusHandler->releaseModalFocus(this);
    }

    void Widget::releaseModalMouseInputFocus()
    {
        if (!mFocusHandler)
            return;

        mFocusHandler->releaseModalMouseInputFocus(this);
    }

    bool Widget::isModalFocused() const
    {
        if (!mFocusHandler)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add "
                "the widget to the gui?).");
        }

        if (mParent)
        {
            return (mFocusHandler->getModalFocused() == this)
                || mParent->isModalFocused();
        }

        return mFocusHandler->getModalFocused() == this;
    }

    bool Widget::isModalMouseInputFocused() const
    {
        if (!mFocusHandler)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add "
                "the widget to the gui?).");
        }

        if (mParent)
        {
            return (mFocusHandler->getModalMouseInputFocused() == this)
                || mParent->isModalMouseInputFocused();
        }

        return mFocusHandler->getModalMouseInputFocused() == this;
    }

    Widget *Widget::getWidgetAt(int x A_UNUSED, int y A_UNUSED)
    {
        return nullptr;
    }

    const std::list<MouseListener*>& Widget::_getMouseListeners()
    {
        return mMouseListeners;
    }

    const std::list<KeyListener*>& Widget::_getKeyListeners()
    {
        return mKeyListeners;
    }

    const std::list<FocusListener*>& Widget::_getFocusListeners()
    {
        return mFocusListeners;
    }

    Rectangle Widget::getChildrenArea()
    {
        return Rectangle(0, 0, 0, 0);
    }

    FocusHandler* Widget::_getInternalFocusHandler()
    {
        return mInternalFocusHandler;
    }

    void Widget::setInternalFocusHandler(FocusHandler* focusHandler)
    {
        mInternalFocusHandler = focusHandler;
    }

    void Widget::setId(const std::string& id)
    {
        mId = id;
    }

    const std::string& Widget::getId()
    {
        return mId;
    }

    void Widget::distributeResizedEvent()
    {
        for (WidgetListenerIterator iter = mWidgetListeners.begin();
             iter != mWidgetListeners.end();
             ++ iter)
        {
            Event event(this);
            (*iter)->widgetResized(event);
        }
    }

    void Widget::distributeMovedEvent()
    {
        for (WidgetListenerIterator iter = mWidgetListeners.begin();
             iter != mWidgetListeners.end();
             ++ iter)
        {
            Event event(this);
            (*iter)->widgetMoved(event);
        }
    }

    void Widget::distributeHiddenEvent()
    {
        for (WidgetListenerIterator iter = mWidgetListeners.begin();
             iter != mWidgetListeners.end();
             ++ iter)
        {
            Event event(this);
            (*iter)->widgetHidden(event);
        }
    }

    void Widget::distributeActionEvent()
    {
        for (ActionListenerIterator iter = mActionListeners.begin();
             iter != mActionListeners.end();
             ++iter)
        {
            ActionEvent actionEvent(this, mActionEventId);
            (*iter)->action(actionEvent);
        }
    }

    void Widget::distributeShownEvent()
    {
        for (WidgetListenerIterator iter = mWidgetListeners.begin();
             iter != mWidgetListeners.end();
             ++iter)
        {
            Event event(this);
            (*iter)->widgetShown(event);
        }
    }

    void Widget::showPart(Rectangle rectangle)
    {
        if (mParent)
            mParent->showWidgetPart(this, rectangle);
    }
}  // namespace gcn
