/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/widgets/listbox.h"

#include "settings.h"

#include "gui/focushandler.h"
#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "gui/models/listmodel.h"

#include "listeners/selectionlistener.h"

#include "utils/foreach.h"

#include "render/graphics.h"

#include "debug.h"

float ListBox::mAlpha = 1.0;

ListBox::ListBox(const Widget2 *const widget,
                 ListModel *const listModel,
                 const std::string &skin) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mSelected(-1),
    mListModel(listModel),
    mWrappingEnabled(false),
    mSelectionListeners(),
    mHighlightColor(getThemeColor(ThemeColorId::HIGHLIGHT, 255U)),
    mForegroundSelectedColor(getThemeColor(ThemeColorId::LISTBOX_SELECTED,
        255U)),
    mForegroundSelectedColor2(getThemeColor(
        ThemeColorId::LISTBOX_SELECTED_OUTLINE, 255U)),
    mOldSelected(-1),
    mPadding(0),
    mPressedIndex(-2),
    mRowHeight(0),
    mItemPadding(1),
    mSkin(nullptr),
    mDistributeMousePressed(true),
    mCenterText(false)
{
    setWidth(100);
    setFocusable(true);
    addMouseListener(this);
    addKeyListener(this);

    mForegroundColor = getThemeColor(ThemeColorId::LISTBOX, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::LISTBOX_OUTLINE, 255U);

    if (theme != nullptr)
    {
        mSkin = theme->load(skin,
            "listbox.xml",
            true,
            theme->getThemePath());
    }

    if (mSkin != nullptr)
    {
        mPadding = mSkin->getPadding();
        mItemPadding = mSkin->getOption("itemPadding");
    }

    const Font *const font = getFont();
    mRowHeight = CAST_U32(
        font->getHeight() + 2 * mItemPadding);
}

void ListBox::postInit()
{
    adjustSize();
}

ListBox::~ListBox()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    if (theme != nullptr)
        theme->unload(mSkin);
}

void ListBox::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (mAlpha != alpha)
        mAlpha = alpha;
}

void ListBox::draw(Graphics *const graphics)
{
    if (mListModel == nullptr)
        return;

    BLOCK_START("ListBox::draw")
    updateAlpha();

    mHighlightColor.a = CAST_U32(mAlpha * 255.0F);
    graphics->setColor(mHighlightColor);
    Font *const font = getFont();
    const int rowHeight = CAST_S32(getRowHeight());
    const int width = mDimension.width;

    if (mCenterText)
    {
        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(Rect(mPadding,
                rowHeight * mSelected + mPadding,
                mDimension.width - 2 * mPadding, rowHeight));

            const std::string str = mListModel->getElementAt(mSelected);
            font->drawString(graphics,
                mForegroundSelectedColor,
                mForegroundSelectedColor2,
                str,
                (width - font->getWidth(str)) / 2,
                mSelected * rowHeight + mPadding + mItemPadding);
        }
        // Draw the list elements
        const int sz = mListModel->getNumberOfElements();
        for (int i = 0, y = mPadding + mItemPadding;
             i < sz; ++i, y += rowHeight)
        {
            if (i != mSelected)
            {
                const std::string str = mListModel->getElementAt(i);
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor2,
                    str,
                    (width - font->getWidth(str)) / 2, y);
            }
        }
    }
    else
    {
        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(Rect(mPadding,
                rowHeight * mSelected + mPadding,
                mDimension.width - 2 * mPadding, rowHeight));

            const std::string str = mListModel->getElementAt(mSelected);
            font->drawString(graphics,
                mForegroundSelectedColor,
                mForegroundSelectedColor2,
                str,
                mPadding,
                mSelected * rowHeight + mPadding + mItemPadding);
        }
        // Draw the list elements
        const int sz = mListModel->getNumberOfElements();
        for (int i = 0, y = mPadding + mItemPadding; i < sz;
             ++i, y += rowHeight)
        {
            if (i != mSelected)
            {
                const std::string str = mListModel->getElementAt(i);
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor2,
                    str,
                    mPadding, y);
            }
        }
    }
    BLOCK_END("ListBox::draw")
}

void ListBox::keyPressed(KeyEvent &event)
{
    const InputActionT action = event.getActionId();
    if (action == InputAction::GUI_SELECT)
    {
        distributeActionEvent();
        event.consume();
    }
    else if (action == InputAction::GUI_UP)
    {
        if (mSelected > 0)
        {
            setSelected(mSelected - 1);
        }
        else if (mSelected == 0 &&
                 mWrappingEnabled &&
                 getListModel() != nullptr)
        {
            setSelected(getListModel()->getNumberOfElements() - 1);
        }
        event.consume();
    }
    else if (action == InputAction::GUI_DOWN)
    {
        const int num = getListModel()->getNumberOfElements() - 1;
        if (mSelected < num)
            setSelected(mSelected + 1);
        else if (mSelected == num && mWrappingEnabled)
            setSelected(0);
        event.consume();
    }
    else if (action == InputAction::GUI_HOME)
    {
        setSelected(0);
        event.consume();
    }
    else if (action == InputAction::GUI_END && (getListModel() != nullptr))
    {
        setSelected(getListModel()->getNumberOfElements() - 1);
        event.consume();
    }
}

void ListBox::safeDraw(Graphics *const graphics)
{
    ListBox::draw(graphics);
}

// Don't do anything on scrollwheel. ScrollArea will deal with that.

void ListBox::mouseWheelMovedUp(MouseEvent &event A_UNUSED)
{
}

void ListBox::mouseWheelMovedDown(MouseEvent &event A_UNUSED)
{
}

void ListBox::mousePressed(MouseEvent &event)
{
    mPressedIndex = getSelectionByMouse(event.getY());
    if (mMouseConsume && mPressedIndex != -1)
        event.consume();
}

void ListBox::mouseReleased(MouseEvent &event)
{
    if (mPressedIndex != getSelectionByMouse(event.getY()))
        return;

    if (mDistributeMousePressed)
    {
        mouseReleased1(event);
    }
    else
    {
        switch (event.getClickCount())
        {
            case 1:
                mouseDragged(event);
                mOldSelected = mSelected;
                break;
            case 2:
                if (gui != nullptr)
                    gui->resetClickCount();
                if (mOldSelected == mSelected)
                    mouseReleased1(event);
                else
                    mouseDragged(event);
                mOldSelected = mSelected;
                break;
            default:
                mouseDragged(event);
                mOldSelected = mSelected;
                break;
        }
    }
    mPressedIndex = -2;
}

void ListBox::mouseReleased1(const MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        setSelected(std::max(0, getSelectionByMouse(event.getY())));
        distributeActionEvent();
    }
}

void ListBox::mouseDragged(MouseEvent &event)
{
    if (event.getButton() != MouseButton::LEFT || getRowHeight() == 0)
        return;

    // Make list selection update on drag, but guard against negative y
    if (getRowHeight() != 0u)
        setSelected(std::max(0, getSelectionByMouse(event.getY())));
}

void ListBox::refocus()
{
    if (mFocusHandler == nullptr)
        return;

    if (isFocusable())
        mFocusHandler->requestFocus(this);
}

void ListBox::adjustSize()
{
    BLOCK_START("ListBox::adjustSize")
    if (mListModel != nullptr)
    {
        setHeight(CAST_S32(getRowHeight()) *
            mListModel->getNumberOfElements() + 2 * mPadding);
    }
    BLOCK_END("ListBox::adjustSize")
}

void ListBox::logic()
{
    BLOCK_START("ListBox::logic")
    adjustSize();
    BLOCK_END("ListBox::logic")
}

int ListBox::getSelectionByMouse(const int y) const
{
    if (y < mPadding)
        return -1;
    return (y - mPadding) / CAST_S32(getRowHeight());
}

void ListBox::setSelected(const int selected)
{
    if (mListModel == nullptr)
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

    Rect scroll;

    if (mSelected < 0)
        scroll.y = 0;
    else
        scroll.y = CAST_S32(getRowHeight()) * mSelected;

    scroll.height = CAST_S32(getRowHeight());
    showPart(scroll);

    distributeValueChangedEvent();
}

void ListBox::setListModel(ListModel *const listModel)
{
    mSelected = -1;
    mListModel = listModel;
    adjustSize();
}

void ListBox::addSelectionListener(SelectionListener *const selectionListener)
{
    mSelectionListeners.push_back(selectionListener);
}

void ListBox::removeSelectionListener(SelectionListener *const
                                      selectionListener)
{
    mSelectionListeners.remove(selectionListener);
}

void ListBox::distributeValueChangedEvent()
{
    FOR_EACH (SelectionListenerIterator, iter, mSelectionListeners)
    {
        SelectionEvent event(this);
        (*iter)->valueChanged(event);
    }
}
