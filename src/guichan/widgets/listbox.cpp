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

#include "guichan/widgets/listbox.hpp"

#include "guichan/basiccontainer.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/listmodel.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/selectionlistener.hpp"

#include "debug.h"

namespace gcn
{
    ListBox::ListBox() :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        mSelected(-1),
        mListModel(nullptr),
        mWrappingEnabled(false),
        mSelectionListeners()
    {
        setWidth(100);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    ListBox::ListBox(ListModel *listModel) :
        gcn::Widget(),
        gcn::MouseListener(),
        gcn::KeyListener(),
        mSelected(-1),
        mListModel(listModel),
        mWrappingEnabled(false),
        mSelectionListeners()
    {
        setWidth(100);
        adjustSize();
        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    void ListBox::draw(Graphics* graphics A_UNUSED)
    {
    }

    void ListBox::logic()
    {
    }

    int ListBox::getSelected() const
    {
        return mSelected;
    }

    void ListBox::setSelected(int selected)
    {
        if (!mListModel)
        {
            mSelected = -1;
        }
        else
        {
            if (selected < 0)
                mSelected = -1;
            else if (selected >= mListModel->getNumberOfElements())
                mSelected = mListModel->getNumberOfElements() - 1;
            else
                mSelected = selected;
        }

        Rectangle scroll;

        if (mSelected < 0)
            scroll.y = 0;
        else
            scroll.y = getRowHeight() * mSelected;

        scroll.height = getRowHeight();
        showPart(scroll);

        distributeValueChangedEvent();
    }

    void ListBox::keyPressed(KeyEvent &keyEvent A_UNUSED)
    {
    }

    void ListBox::mousePressed(MouseEvent &mouseEvent A_UNUSED)
    {
    }

    void ListBox::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            if (getSelected() > 0 )
                setSelected(getSelected() - 1);

            mouseEvent.consume();
        }
    }

    void ListBox::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            setSelected(getSelected() + 1);

            mouseEvent.consume();
        }
    }

    void ListBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ListBox::setListModel(ListModel *listModel)
    {
        mSelected = -1;
        mListModel = listModel;
        adjustSize();
    }

    ListModel* ListBox::getListModel()
    {
        return mListModel;
    }

    void ListBox::adjustSize()
    {
    }

    bool ListBox::isWrappingEnabled() const
    {
        return mWrappingEnabled;
    }

    void ListBox::setWrappingEnabled(bool wrappingEnabled)
    {
        mWrappingEnabled = wrappingEnabled;
    }

    void ListBox::addSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void ListBox::removeSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void ListBox::distributeValueChangedEvent()
    {
        for (SelectionListenerIterator iter = mSelectionListeners.begin();
             iter != mSelectionListeners.end();
             ++ iter)
        {
            SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }

    unsigned int ListBox::getRowHeight() const
    {
        return getFont()->getHeight();
    }
}  // namespace gcn
