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

#include "guichan/widgets/window.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/mouseinput.hpp"

#include "debug.h"

namespace gcn
{
    Window::Window() :
        Container(),
        gcn::MouseListener(),
        mCaption(),
        mAlignment(Graphics::CENTER),
        mPadding(2),
        mTitleBarHeight(16),
        mMovable(true),
        mOpaque(true),
        mDragOffsetX(0),
        mDragOffsetY(0),
        mMoved(false)
    {
        mFrameSize = 1;
        addMouseListener(this);
    }

    Window::Window(const std::string& caption) :
        Container(),
        gcn::MouseListener(),
        mCaption(caption),
        mAlignment(Graphics::CENTER),
        mPadding(2),
        mTitleBarHeight(16),
        mMovable(true),
        mOpaque(true),
        mDragOffsetX(0),
        mDragOffsetY(0),
        mMoved(false)
    {
        mFrameSize = 1;
        addMouseListener(this);
    }

    Window::~Window()
    {
    }

    void Window::setPadding(unsigned int padding)
    {
        mPadding = padding;
    }

    unsigned int Window::getPadding() const
    {
        return mPadding;
    }

    void Window::setTitleBarHeight(unsigned int height)
    {
        mTitleBarHeight = height;
    }

    unsigned int Window::getTitleBarHeight()
    {
        return mTitleBarHeight;
    }

    void Window::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    const std::string& Window::getCaption() const
    {
        return mCaption;
    }

    void Window::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Window::getAlignment() const
    {
        return mAlignment;
    }

    void Window::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getSource() != this)
            return;

        if (getParent())
            getParent()->moveToTop(this);

        mDragOffsetX = mouseEvent.getX();
        mDragOffsetY = mouseEvent.getY();

        mMoved = mouseEvent.getY() <= static_cast<int>(mTitleBarHeight);
    }

    void Window::mouseReleased(MouseEvent& mouseEvent A_UNUSED)
    {
        mMoved = false;
    }

    void Window::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed() || mouseEvent.getSource() != this)
            return;

        if (isMovable() && mMoved)
        {
            setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
                        mouseEvent.getY() - mDragOffsetY + getY());
        }

        mouseEvent.consume();
    }

    Rectangle Window::getChildrenArea()
    {
        return Rectangle(getPadding(),
                         getTitleBarHeight(),
                         getWidth() - getPadding() * 2,
                         getHeight() - getPadding() - getTitleBarHeight());
    }

    void Window::setMovable(bool movable)
    {
        mMovable = movable;
    }

    bool Window::isMovable() const
    {
        return mMovable;
    }

    void Window::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool Window::isOpaque()
    {
        return mOpaque;
    }

    void Window::resizeToContent()
    {
        int w = 0, h = 0;
        for (WidgetListConstIterator it = mWidgets.begin();
             it != mWidgets.end(); ++ it)
        {
            if ((*it)->getX() + (*it)->getWidth() > w)
                w = (*it)->getX() + (*it)->getWidth();

            if ((*it)->getY() + (*it)->getHeight() > h)
                h = (*it)->getY() + (*it)->getHeight();
        }

        setSize(w + 2* getPadding(), h + getPadding() + getTitleBarHeight());
    }
}  // namespace gcn
