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

#include "guichan/widgets/checkbox.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"

#include "debug.h"

namespace gcn
{

    CheckBox::CheckBox() :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        mSelected(false),
        mCaption()
    {
        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    CheckBox::CheckBox(const std::string &caption, bool selected) :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        mSelected(selected),
        mCaption()
    {
        setCaption(caption);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    bool CheckBox::isSelected() const
    {
        return mSelected;
    }

    void CheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    const std::string &CheckBox::getCaption() const
    {
        return mCaption;
    }

    void CheckBox::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    void CheckBox::keyPressed(KeyEvent& keyEvent A_UNUSED)
    {
    }

    void CheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            toggleSelected();
        }
    }

    void CheckBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void CheckBox::adjustSize()
    {
    }

    void CheckBox::toggleSelected()
    {
        mSelected = !mSelected;
        distributeActionEvent();
    }
}  // namespace gcn
