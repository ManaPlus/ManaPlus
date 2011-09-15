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

#include "guichan/widgets/dropdown.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/listmodel.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/widgets/listbox.hpp"
#include "guichan/widgets/scrollarea.hpp"

#include "debug.h"

namespace gcn
{
    DropDown::DropDown(ListModel *listModel,
                       ScrollArea *scrollArea,
                       ListBox *listBox)
    {
        setWidth(100);
        setFocusable(true);
        mDroppedDown = false;
        mPushed = false;
        mIsDragged = false;

        setInternalFocusHandler(&mInternalFocusHandler);

        mInternalScrollArea = (scrollArea == NULL);
        mInternalListBox = (listBox == NULL);

        if (mInternalScrollArea)
            mScrollArea = new ScrollArea();
        else
            mScrollArea = scrollArea;

        if (mInternalListBox)
            mListBox = new ListBox();
        else
            mListBox = listBox;

        mScrollArea->setContent(mListBox);
        add(mScrollArea);

        mListBox->addActionListener(this);
        mListBox->addSelectionListener(this);

        setListModel(listModel);

        if (mListBox->getSelected() < 0)
            mListBox->setSelected(0);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);

        adjustHeight();
    }

    DropDown::~DropDown()
    {
        if (widgetExists(mListBox))
        {
            mListBox->removeActionListener(this);
            mListBox->removeSelectionListener(this);
        }

        if (mInternalScrollArea)
        {
            delete mScrollArea;
            mScrollArea = 0;
        }

        if (mInternalListBox)
        {
            delete mListBox;
            mListBox = 0;
        }

        setInternalFocusHandler(NULL);
    }

    int DropDown::getSelected() const
    {
        return mListBox->getSelected();
    }

    void DropDown::setSelected(int selected)
    {
        if (selected >= 0)
            mListBox->setSelected(selected);
    }

    void DropDown::mousePressed(MouseEvent& mouseEvent)
    {
        // If we have a mouse press on the widget.
        if (0 <= mouseEvent.getY()
            && mouseEvent.getY() < getHeight()
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() < getWidth()
            && mouseEvent.getButton() == MouseEvent::LEFT
            && !mDroppedDown
            && mouseEvent.getSource() == this)
        {
            mPushed = true;
            dropDown();
            requestModalMouseInputFocus();
        }
        // Fold up the listbox if the upper part is clicked after fold down
        else if (0 <= mouseEvent.getY()
                 && mouseEvent.getY() < mFoldedUpHeight
                 && mouseEvent.getX() >= 0
                 && mouseEvent.getX() < getWidth()
                 && mouseEvent.getButton() == MouseEvent::LEFT
                 && mDroppedDown
                 && mouseEvent.getSource() == this)
        {
            mPushed = false;
            foldUp();
            releaseModalMouseInputFocus();
        }
        // If we have a mouse press outside the widget
        else if (0 > mouseEvent.getY()
                 || mouseEvent.getY() >= getHeight()
                 || mouseEvent.getX() < 0
                 || mouseEvent.getX() >= getWidth())
        {
            mPushed = false;
            foldUp();
        }
    }

    void DropDown::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mIsDragged)
            mPushed = false;

        // Released outside of widget. Can happen when we have modal
        // input focus.
        if ((0 > mouseEvent.getY()
            || mouseEvent.getY() >= getHeight()
            || mouseEvent.getX() < 0
            || mouseEvent.getX() >= getWidth())
            && mouseEvent.getButton() == MouseEvent::LEFT
            && isModalMouseInputFocused())
        {
            releaseModalMouseInputFocus();

            if (mIsDragged)
                foldUp();
        }
        else if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mPushed = false;
        }

        mIsDragged = false;
    }

    void DropDown::mouseDragged(MouseEvent& mouseEvent)
    {
        mIsDragged = true;

        mouseEvent.consume();
    }

    void DropDown::setListModel(ListModel *listModel)
    {
        mListBox->setListModel(listModel);

        if (mListBox->getSelected() < 0)
            mListBox->setSelected(0);

        adjustHeight();
    }

    ListModel *DropDown::getListModel()
    {
        return mListBox->getListModel();
    }

    void DropDown::adjustHeight()
    {
        if (mScrollArea == NULL)
            throw GCN_EXCEPTION("Scroll area has been deleted.");

        if (mListBox == NULL)
            throw GCN_EXCEPTION("List box has been deleted.");

        int listBoxHeight = mListBox->getHeight();

        // We add 2 for the border
        int h2 = getFont()->getHeight() + 2;

        setHeight(h2);

        // The addition/subtraction of 2 compensates for the seperation lines
        // seperating the selected element view and the scroll area.

        if (mDroppedDown && getParent())
        {
            int h = getParent()->getChildrenArea().height - getY();

            if (listBoxHeight > h - h2 - 2)
            {
                mScrollArea->setHeight(h - h2 - 2);
                setHeight(h);
            }
            else
            {
                setHeight(listBoxHeight + h2 + 2);
                mScrollArea->setHeight(listBoxHeight);
            }
        }

        mScrollArea->setWidth(getWidth());
        // Resize the ListBox to exactly fit the ScrollArea.
        mListBox->setWidth(mScrollArea->getChildrenArea().width);
        mScrollArea->setPosition(0, 0);
    }

    void DropDown::dropDown()
    {
        if (!mDroppedDown)
        {
            mDroppedDown = true;
            mFoldedUpHeight = getHeight();
            adjustHeight();

            if (getParent())
                getParent()->moveToTop(this);
        }

        mListBox->requestFocus();
    }

    void DropDown::foldUp()
    {
        if (mDroppedDown)
        {
            mDroppedDown = false;
            adjustHeight();
            mInternalFocusHandler.focusNone();
        }
    }

    void DropDown::focusLost(const Event& event A_UNUSED)
    {
        foldUp();
        mInternalFocusHandler.focusNone();
    }


    void DropDown::death(const Event& event)
    {
        if (event.getSource() == mScrollArea)
            mScrollArea = NULL;

        BasicContainer::death(event);
    }

    void DropDown::action(const ActionEvent& actionEvent A_UNUSED)
    {
        foldUp();
        releaseModalMouseInputFocus();
        distributeActionEvent();
    }

    Rectangle DropDown::getChildrenArea()
    {
        if (mDroppedDown)
        {
            // Calculate the children area (with the one pixel border in mind)
            return Rectangle(1, mFoldedUpHeight + 1, 
                getWidth() - 2, getHeight() - mFoldedUpHeight - 2);
        }

        return Rectangle();
    }

    void DropDown::setBaseColor(const Color& color)
    {
        if (mInternalScrollArea)
            mScrollArea->setBaseColor(color);

        if (mInternalListBox)
            mListBox->setBaseColor(color);

        Widget::setBaseColor(color);
    }

    void DropDown::setBackgroundColor(const Color& color)
    {
        if (mInternalScrollArea)
            mScrollArea->setBackgroundColor(color);

        if (mInternalListBox)
            mListBox->setBackgroundColor(color);

        Widget::setBackgroundColor(color);
    }

    void DropDown::setForegroundColor(const Color& color)
    {
        if (mInternalScrollArea)
            mScrollArea->setForegroundColor(color);

        if (mInternalListBox)
            mListBox->setForegroundColor(color);

        Widget::setForegroundColor(color);
    }

    void DropDown::setFont(Font *font)
    {
        if (mInternalScrollArea)
            mScrollArea->setFont(font);

        if (mInternalListBox)
            mListBox->setFont(font);

        Widget::setFont(font);
    }

    void DropDown::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == this)
        {
            mouseEvent.consume();

            if (mListBox->getSelected() > 0)
                mListBox->setSelected(mListBox->getSelected() - 1);
        }
    }

    void DropDown::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == this)
        {
            mouseEvent.consume();

            mListBox->setSelected(mListBox->getSelected() + 1);
        }
    }

    void DropDown::setSelectionColor(const Color& color)
    {
        Widget::setSelectionColor(color);

        if (mInternalListBox)
            mListBox->setSelectionColor(color);
    }

    void DropDown::valueChanged(const SelectionEvent& event A_UNUSED)
    {
        distributeValueChangedEvent();
    }

    void DropDown::addSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void DropDown::removeSelectionListener(SelectionListener* listener)
    {
        mSelectionListeners.remove(listener);
    }

    void DropDown::distributeValueChangedEvent()
    {
        SelectionListenerIterator iter;

        for (iter = mSelectionListeners.begin();
             iter != mSelectionListeners.end(); 
             ++iter)
        {
            SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }
}

