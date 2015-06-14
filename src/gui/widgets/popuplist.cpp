/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#include "gui/widgets/popuplist.h"

#include "gui/gui.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/scrollarea.h"

#include "debug.h"

PopupList::PopupList(DropDown *const widget,
                     ListModel *const listModel,
                     const bool extended,
                     const Modal modal) :
    Popup("PopupList", "popuplist.xml"),
    FocusListener(),
    mListModel(listModel),
    mListBox(extended ? CREATEWIDGETR(ExtendedListBox,
        widget, listModel, "extendedlistbox.xml", 0) :
        CREATEWIDGETR(ListBox,
        widget, listModel, "popuplistbox.xml")),
    mScrollArea(new ScrollArea(this, mListBox, false)),
    mDropDown(widget),
    mPressedIndex(-2),
    mModal(modal)
{
    mListBox->setMouseConsume(false);
    mScrollArea->setMouseConsume(false);
    mAllowLogic = false;
    setFocusable(true);

    mListBox->setDistributeMousePressed(true);
    mScrollArea->setPosition(mPadding, mPadding);
}

void PopupList::postInit()
{
    Popup::postInit();
    add(mScrollArea);

    if (gui)
        gui->addGlobalFocusListener(this);

    addKeyListener(mDropDown);
    addMouseListener(this);
    adjustSize();
}

PopupList::~PopupList()
{
    if (mParent)
        mParent->removeFocusListener(this);
    if (gui)
        gui->removeGlobalFocusListener(this);
    removeKeyListener(mDropDown);
}

void PopupList::show(int x, int y)
{
    int len = mListBox->getHeight() + 8;
    if (len > 250)
        len = 250;
    setContentSize(mListBox->getWidth() + 8, len);
    const int width = mDimension.width;
    const int height = mDimension.height;
    if (mainGraphics->mWidth < (x + width + 5))
        x = mainGraphics->mWidth - width;
    if (mainGraphics->mHeight < (y + height + 5))
        y = mainGraphics->mHeight - height;
    setPosition(x, y);
    setVisible(Visible_true);
    requestMoveToTop();
    if (mModal == Modal_true)
        requestModalFocus();
}

void PopupList::widgetResized(const Event &event)
{
    Popup::widgetResized(event);
    adjustSize();
}

void PopupList::setSelected(int selected)
{
    if (!mListBox)
        return;

    mListBox->setSelected(selected);
}

int PopupList::getSelected() const
{
    if (!mListBox)
        return -1;

    return mListBox->getSelected();
}

void PopupList::setListModel(ListModel *const model)
{
    if (mListBox)
        mListBox->setListModel(model);
    mListModel = model;
}

void PopupList::adjustSize()
{
    const int pad2 = 2 * mPadding;
    const int width = mDimension.width - pad2;
    mScrollArea->setWidth(width);
    mScrollArea->setHeight(mDimension.height - pad2);
    mListBox->adjustSize();
    mListBox->setWidth(width);
}

void PopupList::mousePressed(MouseEvent& event)
{
    mPressedIndex = mListBox->getSelectionByMouse(
        event.getY() + mPadding);
    event.consume();
}

void PopupList::mouseReleased(MouseEvent& event)
{
    if (mPressedIndex != mListBox->getSelectionByMouse(
        event.getY() + mPadding))
    {
        mPressedIndex = -2;
        return;
    }

    mPressedIndex = -2;
    if (event.getSource() == mScrollArea)
        return;
    if (mDropDown)
        mDropDown->updateSelection();
    setVisible(Visible_false);
    if (mModal == Modal_true)
        releaseModalFocus();
}

void PopupList::focusGained(const Event& event)
{
    const Widget *const source = event.getSource();
    if (mVisible == Visible_false ||
        source == this ||
        source == mListBox ||
        source == mScrollArea ||
        source == mDropDown)
    {
        return;
    }

    if (mDropDown)
        mDropDown->updateSelection();
    setVisible(Visible_false);
    if (mModal == Modal_true)
        releaseModalFocus();
}

void PopupList::focusLost(const Event& event A_UNUSED)
{
    if (mDropDown)
        mDropDown->updateSelection();
}
