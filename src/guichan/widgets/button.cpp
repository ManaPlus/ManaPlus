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

#include "guichan/widgets/button.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

#include "debug.h"

namespace gcn
{
    Button::Button() :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        gcn::FocusListener(),
        mCaption(),
        mHasMouse(false),
        mKeyPressed(false),
        mMousePressed(false),
        mAlignment(Graphics::CENTER),
        mSpacing(4)
    {
        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    Button::Button(const std::string& caption) :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        gcn::FocusListener(),
        mCaption(caption),
        mHasMouse(false),
        mKeyPressed(false),
        mMousePressed(false),
        mAlignment(Graphics::CENTER),
        mSpacing(4)
    {
        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    void Button::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    const std::string& Button::getCaption() const
    {
        return mCaption;
    }

    void Button::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Button::getAlignment() const
    {
        return mAlignment;
    }

    void Button::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
    }

    unsigned int Button::getSpacing() const
    {
      return mSpacing;
    }

    void Button::adjustSize()
    {
    }

    bool Button::isPressed() const
    {
        if (mMousePressed)
            return mHasMouse;
        else
            return mKeyPressed;
    }

    void Button::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void Button::mouseExited(MouseEvent& mouseEvent A_UNUSED)
    {
        mHasMouse = false;
    }

    void Button::mouseEntered(MouseEvent& mouseEvent A_UNUSED)
    {
        mHasMouse = true;
    }

    void Button::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void Button::focusLost(const Event& event A_UNUSED)
    {
        mMousePressed = false;
        mKeyPressed = false;
    }
}  // namespace gcn
