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

#include "gui/widgets/listbox.h"

#include "client.h"

#include "input/keydata.h"
#include "input/keyevent.h"

#include "gui/gui.h"

#include <guichan/focushandler.hpp>
#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

float ListBox::mAlpha = 1.0;

ListBox::ListBox(const Widget2 *const widget,
                 gcn::ListModel *const listModel,
                 const std::string &skin) :
    gcn::ListBox(listModel),
    Widget2(widget),
    mHighlightColor(getThemeColor(Theme::HIGHLIGHT)),
    mForegroundSelectedColor(getThemeColor(Theme::LISTBOX_SELECTED)),
    mForegroundSelectedColor2(getThemeColor(Theme::LISTBOX_SELECTED_OUTLINE)),
    mOldSelected(-1),
    mPadding(0),
    mPressedIndex(-2),
    mRowHeight(0),
    mItemPadding(1),
    mSkin(nullptr),
    mDistributeMousePressed(true),
    mCenterText(false)
{
    mForegroundColor = getThemeColor(Theme::LISTBOX);
    mForegroundColor2 = getThemeColor(Theme::LISTBOX_OUTLINE);

    Theme *const theme = Theme::instance();
    if (theme)
        mSkin = theme->load(skin, "listbox.xml");

    if (mSkin)
    {
        mPadding = mSkin->getPadding();
        mItemPadding = mSkin->getOption("itemPadding");
    }

    const gcn::Font *const font = getFont();
    if (font)
        mRowHeight = font->getHeight() + 2 * mItemPadding;
    else
        mRowHeight = 13;
}

void ListBox::postInit()
{
    adjustSize();
}

ListBox::~ListBox()
{
    if (gui)
        gui->removeDragged(this);

    if (Theme::instance())
        Theme::instance()->unload(mSkin);
}

void ListBox::updateAlpha()
{
    const float alpha = std::max(client->getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
        mAlpha = alpha;
}

void ListBox::draw(gcn::Graphics *graphics)
{
    if (!mListModel)
        return;

    BLOCK_START("ListBox::draw")
    updateAlpha();
    Graphics *const g = static_cast<Graphics*>(graphics);

    mHighlightColor.a = static_cast<int>(mAlpha * 255.0F);
    graphics->setColor(mHighlightColor);
    gcn::Font *const font = getFont();
    const int rowHeight = getRowHeight();
    const int width = mDimension.width;

    if (mCenterText)
    {
        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(mPadding,
                rowHeight * mSelected + mPadding,
                mDimension.width - 2 * mPadding, rowHeight));

            g->setColorAll(mForegroundSelectedColor,
                mForegroundSelectedColor2);
            const std::string str = mListModel->getElementAt(mSelected);
            font->drawString(graphics, str,
                (width - font->getWidth(str)) / 2,
                mSelected * rowHeight + mPadding + mItemPadding);
        }
        // Draw the list elements
        g->setColorAll(mForegroundColor, mForegroundColor2);
        const int sz = mListModel->getNumberOfElements();
        for (int i = 0, y = mPadding + mItemPadding;
             i < sz; ++i, y += rowHeight)
        {
            if (i != mSelected)
            {
                const std::string str = mListModel->getElementAt(i);
                font->drawString(graphics, str,
                    (width - font->getWidth(str)) / 2, y);
            }
        }
    }
    else
    {
        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(mPadding,
                rowHeight * mSelected + mPadding,
                mDimension.width - 2 * mPadding, rowHeight));

            g->setColorAll(mForegroundSelectedColor,
                mForegroundSelectedColor2);
            const std::string str = mListModel->getElementAt(mSelected);
            font->drawString(graphics, str, mPadding,
                mSelected * rowHeight + mPadding + mItemPadding);
        }
        // Draw the list elements
        g->setColorAll(mForegroundColor, mForegroundColor2);
        const int sz = mListModel->getNumberOfElements();
        for (int i = 0, y = mPadding + mItemPadding; i < sz;
             ++i, y += rowHeight)
        {
            if (i != mSelected)
            {
                const std::string str = mListModel->getElementAt(i);
                font->drawString(graphics, str, mPadding, y);
            }
        }
    }
    BLOCK_END("ListBox::draw")
}

void ListBox::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();
    if (action == Input::KEY_GUI_SELECT)
    {
        distributeActionEvent();
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_UP)
    {
        if (mSelected > 0)
            setSelected(mSelected - 1);
        else if (mSelected == 0 && mWrappingEnabled && getListModel())
            setSelected(getListModel()->getNumberOfElements() - 1);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_DOWN)
    {
        const int num = getListModel()->getNumberOfElements() - 1;
        if (mSelected < num)
            setSelected(mSelected + 1);
        else if (mSelected == num && mWrappingEnabled)
            setSelected(0);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_HOME)
    {
        setSelected(0);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_END && getListModel())
    {
        setSelected(getListModel()->getNumberOfElements() - 1);
        keyEvent.consume();
    }
}

// Don't do anything on scrollwheel. ScrollArea will deal with that.

void ListBox::mouseWheelMovedUp(gcn::MouseEvent &mouseEvent A_UNUSED)
{
}

void ListBox::mouseWheelMovedDown(gcn::MouseEvent &mouseEvent A_UNUSED)
{
}

void ListBox::mousePressed(gcn::MouseEvent &event)
{
    mPressedIndex = getSelectionByMouse(event.getY());
}

void ListBox::mouseReleased(gcn::MouseEvent &event)
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
                if (gui)
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

void ListBox::mouseReleased1(const gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.getButton() == gcn::MouseEvent::LEFT)
    {
        setSelected(std::max(0, getSelectionByMouse(mouseEvent.getY())));
        distributeActionEvent();
    }
}

void ListBox::mouseDragged(gcn::MouseEvent &event)
{
    if (event.getButton() != gcn::MouseEvent::LEFT || getRowHeight() == 0)
        return;

    // Make list selection update on drag, but guard against negative y
    if (getRowHeight())
        setSelected(std::max(0, getSelectionByMouse(event.getY())));
}

void ListBox::refocus()
{
    if (!mFocusHandler)
        return;

    if (isFocusable())
        mFocusHandler->requestFocus(this);
}

void ListBox::adjustSize()
{
    BLOCK_START("ListBox::adjustSize")
    if (mListModel)
    {
        setHeight(getRowHeight() * mListModel->getNumberOfElements()
            + 2 * mPadding);
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
    return (y - mPadding) / getRowHeight();
}
