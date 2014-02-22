/*
 *  The ManaPlus Client
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

#include "gui/widgets/sliderlist.h"

#include "gui/font.h"
#include "gui/gui.h"

#include "gui/models/listmodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"

#include "debug.h"

static const int buttonWidth = 27;
static const int buttonSpace = 30;
static const int sliderHeight = 30;

SliderList::SliderList(const Widget2 *const widget,
                       ListModel *const listModel) :
    Container(widget),
    ActionListener(),
    MouseListener(),
    mLabel(new Label(this)),
    mListModel(listModel),
    mPrevEventId(),
    mNextEventId(),
    mOldWidth(0),
    mSelectedIndex(0)
{
    setHeight(sliderHeight);
}

void SliderList::postInit(ActionListener *const listener,
                          const std::string &eventId)
{
    mPrevEventId = eventId + "_prev";
    mNextEventId = eventId + "_next";

    mButtons[0] = new Button(this, "<", mPrevEventId, this);
    mButtons[1] = new Button(this, ">", mNextEventId, this);

    add(mButtons[0]);
    add(mLabel);
    add(mButtons[1]);

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);

    updateLabel();
    addMouseListener(this);
}

SliderList::~SliderList()
{
}

void SliderList::updateAlpha()
{
    Button::updateAlpha();
}

void SliderList::mouseWheelMovedUp(MouseEvent& mouseEvent)
{
    setSelected(mSelectedIndex - 1);
    mouseEvent.consume();
}

void SliderList::mouseWheelMovedDown(MouseEvent& mouseEvent)
{
    setSelected(mSelectedIndex + 1);
    mouseEvent.consume();
}

void SliderList::resize()
{
    const int width = getWidth();

    mButtons[0]->setWidth(buttonWidth);
    mLabel->setWidth(width - buttonSpace * 2);
    mButtons[1]->setPosition(width - buttonSpace + 3, 0);
    mButtons[1]->setWidth(buttonWidth);
    updateLabel();
}

void SliderList::draw(Graphics *graphics)
{
    BLOCK_START("SliderList::draw")
    const int width = mDimension.width;
    if (mOldWidth != width)
    {
        resize();
        mOldWidth = width;
    }
    Container::draw(graphics);
    BLOCK_END("SliderList::draw")
}

void SliderList::updateLabel()
{
    if (!mListModel || mSelectedIndex < 0
        || mSelectedIndex >= mListModel->getNumberOfElements())
    {
        return;
    }

    mLabel->setCaption(mListModel->getElementAt(mSelectedIndex));
    mLabel->adjustSize();
    const int space = mDimension.width - buttonSpace * 2;
    const int labelWidth = mLabel->getWidth();
    int labelY = (mDimension.height - mLabel->getHeight()) / 2;
    if (labelY < 0)
        labelY = 0;

    if (space < 0 || space < labelWidth)
        mLabel->setPosition(buttonSpace, labelY);
    else
        mLabel->setPosition(buttonSpace + (space - labelWidth) / 2, labelY);
}

void SliderList::action(const ActionEvent &event)
{
    if (!mListModel)
        return;

    const std::string &eventId = event.getId();
    if (eventId == mPrevEventId)
    {
        mSelectedIndex --;
        if (mSelectedIndex < 0)
            mSelectedIndex = mListModel->getNumberOfElements() - 1;
    }
    else if (eventId == mNextEventId)
    {
        mSelectedIndex ++;
        if (mSelectedIndex >= mListModel->getNumberOfElements())
            mSelectedIndex = 0;
    }
    updateLabel();
    distributeActionEvent();
}

void SliderList::setSelectedString(const std::string &str)
{
    if (!mListModel)
        return;

    for (int f = 0; f < mListModel->getNumberOfElements(); f ++)
    {
        if (mListModel->getElementAt(f) == str)
        {
            setSelected(f);
            break;
        }
    }
}

std::string SliderList::getSelectedString() const
{
    if (!mListModel)
        return std::string();

    return mListModel->getElementAt(mSelectedIndex);
}

void SliderList::setSelected(const int idx)
{
    if (!mListModel)
        return;

    mSelectedIndex = idx;
    const int num = mListModel->getNumberOfElements();
    if (mSelectedIndex >= num)
        mSelectedIndex = 0;
    if (mSelectedIndex < 0)
        mSelectedIndex = num - 1;
    updateLabel();
}

void SliderList::adjustSize()
{
    setWidth(getMaxLabelWidth() + 60);
    updateLabel();
}

int SliderList::getMaxLabelWidth() const
{
    if (!mListModel || !gui)
        return 1;

    int maxWidth = 0;
    const Font *const font = getFont();

    const int num = mListModel->getNumberOfElements();
    for (int f = 0; f < num; f ++)
    {
        const int w = font->getWidth(mListModel->getElementAt(f));
        if (w > maxWidth)
            maxWidth = w;
    }

    return maxWidth;
}
