/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "gui/widgets/extendedlistbox.h"

#include "client.h"
#include "configuration.h"
#include "keyevent.h"

#include "gui/palette.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "gui/widgets/extendedlistmodel.h"

#include <guichan/focushandler.hpp>
#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include <guichan/key.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

ExtendedListBox::ExtendedListBox(gcn::ListModel *listModel):
    ListBox(listModel),
    mRowHeight(13)
{
}

ExtendedListBox::~ExtendedListBox()
{
}

void ExtendedListBox::draw(gcn::Graphics *graphics)
{
    if (!mListModel)
        return;

    ExtendedListModel *model = static_cast<ExtendedListModel*>(mListModel);
    Graphics *g = static_cast<Graphics*>(graphics);

    updateAlpha();
    graphics->setFont(getFont());

    const int height = getRowHeight();
    int textPos = (height - getFont()->getHeight()) / 2;
    if (textPos < 0)
        textPos = 0;

    // Draw filled rectangle around the selected list element
    if (mSelected >= 0)
    {
        mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
        graphics->setColor(mHighlightColor);
        graphics->fillRectangle(gcn::Rectangle(0, height * mSelected,
            getWidth(), height));
    }

    // Draw the list elements
    graphics->setColor(getForegroundColor());
    for (int i = 0, y = 0; i < mListModel->getNumberOfElements();
         ++i, y += height)
    {
        const Image *image = model->getImageAt(i);
        if (!image)
        {
            graphics->drawText(mListModel->getElementAt(i), 1, y + textPos);
        }
        else
        {
            g->drawImage(image, 1, y + (height - image->getHeight()) / 2);
            graphics->drawText(mListModel->getElementAt(i),
                image->getWidth() + 2, y + textPos);
        }
    }
}

unsigned int ExtendedListBox::getRowHeight() const
{
    return mRowHeight;
}
