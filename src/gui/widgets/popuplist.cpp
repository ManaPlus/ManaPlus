/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/widgets/dropdown.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "utils/gettext.h"

#include "debug.h"

PopupList::PopupList(DropDown *const widget,
                     gcn::ListModel *const listModel):
    Popup("PopupList", "popuplist.xml"),
    gcn::FocusListener(),
    mListModel(listModel),
    mListBox(new ListBox(widget, listModel)),
    mScrollArea(new ScrollArea),
    mDropDown(widget)
{
    setFocusable(true);

    mListBox->setDistributeMousePressed(true);
    mListBox->addSelectionListener(this);
    mScrollArea->setContent(mListBox);
    add(mScrollArea);

    if (getParent())
        getParent()->addFocusListener(this);
}

void PopupList::show(int x, int y)
{
    int len = mListBox->getHeight() + 8;
    if (len > 250)
        len = 250;
    setContentSize(mListBox->getWidth() + 8, len);
    if (mainGraphics->mWidth < (x + getWidth() + 5))
        x = mainGraphics->mWidth - getWidth();
    if (mainGraphics->mHeight < (y + getHeight() + 5))
        y = mainGraphics->mHeight - getHeight();
    setPosition(x, y);
    setVisible(true);
    requestMoveToTop();
}

void PopupList::widgetResized(const gcn::Event &event)
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

void PopupList::setListModel(gcn::ListModel *model)
{
    if (mListBox)
        mListBox->setListModel(model);
    mListModel = model;
}

void PopupList::adjustSize()
{
    mScrollArea->setWidth(getWidth() - 8);
    mScrollArea->setHeight(getHeight() - 8);
    mListBox->adjustSize();
}

void PopupList::valueChanged(const gcn::SelectionEvent& event A_UNUSED)
{
    if (mDropDown)
        mDropDown->updateSelection();
    setVisible(false);
}

void PopupList::focusLost(const gcn::Event& event A_UNUSED)
{
    logger->log("lost focus");
    if (mDropDown)
        mDropDown->updateSelection();
//    setVisible(false);
}
