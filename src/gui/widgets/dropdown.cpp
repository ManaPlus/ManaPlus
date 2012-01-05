/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/dropdown.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"

#include "gui/palette.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include <algorithm>

#include "debug.h"

int DropDown::instances = 0;
Image *DropDown::buttons[2][2];
ImageRect DropDown::skin;
float DropDown::mAlpha = 1.0;

DropDown::DropDown(gcn::ListModel *listModel, gcn::ActionListener* listener,
                   std::string eventId):
    gcn::DropDown::DropDown(listModel,
                            new ScrollArea,
                            new ListBox(listModel))
{
    setFrameSize(2);

    // Initialize graphics
    if (instances == 0)
    {
        // Load the background skin

        // Get the button skin
        buttons[1][0] = Theme::getImageFromTheme("vscroll_up_default.png");
        buttons[0][0] = Theme::getImageFromTheme("vscroll_down_default.png");
        buttons[1][1] = Theme::getImageFromTheme("vscroll_up_pressed.png");
        buttons[0][1] = Theme::getImageFromTheme("vscroll_down_pressed.png");

        if (buttons[0][0])
            buttons[0][0]->setAlpha(mAlpha);
        if (buttons[0][1])
            buttons[0][1]->setAlpha(mAlpha);
        if (buttons[1][0])
            buttons[1][0]->setAlpha(mAlpha);
        if (buttons[1][1])
            buttons[1][1]->setAlpha(mAlpha);

        // get the border skin
        Image *boxBorder = Theme::getImageFromTheme("deepbox.png");
        if (boxBorder)
        {
            int gridx[4] = {0, 3, 28, 31};
            int gridy[4] = {0, 3, 28, 31};
            int a = 0, x, y;

            for (y = 0; y < 3; y++)
            {
                for (x = 0; x < 3; x++)
                {
                    skin.grid[a] = boxBorder->getSubImage(gridx[x], gridy[y],
                                                          gridx[x + 1] -
                                                          gridx[x] + 1,
                                                          gridy[y + 1] -
                                                          gridy[y] + 1);
                    if (skin.grid[a])
                        skin.grid[a]->setAlpha(mAlpha);
                    a++;
                }
            }

            boxBorder->decRef();
        }
    }

    instances++;

    mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
    mShadowColor = Theme::getThemeColor(Theme::DROPDOWN_SHADOW);
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);
}

DropDown::~DropDown()
{
    instances--;
    // Free images memory
    if (instances == 0)
    {
        if (buttons[0][0])
            buttons[0][0]->decRef();
        if (buttons[0][1])
            buttons[0][1]->decRef();
        if (buttons[1][0])
            buttons[1][0]->decRef();
        if (buttons[1][1])
            buttons[1][1]->decRef();

        for_each(skin.grid, skin.grid + 9, dtor<Image*>());
    }

    delete mScrollArea;
    mScrollArea = nullptr;
}

void DropDown::updateAlpha()
{
    float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;

        if (buttons[0][0])
            buttons[0][0]->setAlpha(mAlpha);
        if (buttons[0][1])
            buttons[0][1]->setAlpha(mAlpha);
        if (buttons[1][0])
            buttons[1][0]->setAlpha(mAlpha);
        if (buttons[1][1])
            buttons[1][1]->setAlpha(mAlpha);

        for (int a = 0; a < 9; a++)
        {
            if (skin.grid[a])
                skin.grid[a]->setAlpha(mAlpha);
        }
    }
}

void DropDown::draw(gcn::Graphics* graphics)
{
    int h;

    if (mDroppedDown)
        h = mFoldedUpHeight;
    else
        h = getHeight();

    updateAlpha();

    const int alpha = static_cast<int>(mAlpha * 255.0f);
    mHighlightColor.a = alpha;
    mShadowColor.a = alpha;

    if (mListBox->getListModel() && mListBox->getSelected() >= 0)
    {
        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(mListBox->getListModel()->getElementAt(
                           mListBox->getSelected()), 1, 0);
    }

    if (isFocused())
    {
        graphics->setColor(mHighlightColor);
        graphics->drawRectangle(gcn::Rectangle(0, 0, getWidth() - h, h));
    }

    drawButton(graphics);

    if (mDroppedDown)
    {
        drawChildren(graphics);

        // Draw two lines separating the ListBox with selected
        // element view.
        graphics->setColor(mHighlightColor);
        graphics->drawLine(0, h, getWidth(), h);
        graphics->setColor(mShadowColor);
        graphics->drawLine(0, h + 1, getWidth(), h + 1);
    }
}

void DropDown::drawFrame(gcn::Graphics *graphics)
{
    const int bs = getFrameSize();
    const int w = getWidth() + bs * 2;
    const int h = getHeight() + bs * 2;

    static_cast<Graphics*>(graphics)->drawImageRect(0, 0, w, h, skin);
}

void DropDown::drawButton(gcn::Graphics *graphics)
{
    int height = mDroppedDown ? mFoldedUpHeight : getHeight();

    if (buttons[mDroppedDown][mPushed])
    {
        static_cast<Graphics*>(graphics)->
            drawImage(buttons[mDroppedDown][mPushed],
            getWidth() - height + 2, 1);
    }
}

// -- KeyListener notifications
void DropDown::keyPressed(gcn::KeyEvent& keyEvent)
{
    if (keyEvent.isConsumed())
        return;

    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ENTER || key.getValue() == Key::SPACE)
        dropDown();
    else if (key.getValue() == Key::UP)
        setSelected(getSelected() - 1);
    else if (key.getValue() == Key::DOWN)
        setSelected(getSelected() + 1);
    else if (key.getValue() == Key::HOME)
        setSelected(0);
    else if (key.getValue() == Key::END && mListBox->getListModel())
        setSelected(mListBox->getListModel()->getNumberOfElements() - 1);
    else
        return;

    keyEvent.consume();
}

void DropDown::focusLost(const gcn::Event& event)
{
    gcn::DropDown::focusLost(event);
    releaseModalMouseInputFocus();
}

void DropDown::mousePressed(gcn::MouseEvent& mouseEvent)
{
    gcn::DropDown::mousePressed(mouseEvent);

    if (0 <= mouseEvent.getY() && mouseEvent.getY() < getHeight() &&
        mouseEvent.getX() >= 0 && mouseEvent.getX() < getWidth() &&
        mouseEvent.getButton() == gcn::MouseEvent::LEFT && mDroppedDown &&
        mouseEvent.getSource() == mListBox)
    {
        mPushed = false;
        foldUp();
        releaseModalMouseInputFocus();
        distributeActionEvent();
    }
}

void DropDown::mouseWheelMovedUp(gcn::MouseEvent& mouseEvent)
{
    setSelected(getSelected() - 1);
    mouseEvent.consume();
}

void DropDown::mouseWheelMovedDown(gcn::MouseEvent& mouseEvent)
{
    setSelected(getSelected() + 1);
    mouseEvent.consume();
}

void DropDown::setSelectedString(std::string str)
{
    gcn::ListModel *listModel = mListBox->getListModel();
    if (!listModel)
        return;

    for (int f = 0; f < listModel->getNumberOfElements(); f ++)
    {
        if (listModel->getElementAt(f) == str)
        {
            setSelected(f);
            break;
        }
    }
}

std::string DropDown::getSelectedString() const
{
    gcn::ListModel *listModel = mListBox->getListModel();
    if (!listModel)
        return "";

    return listModel->getElementAt(getSelected());
}
