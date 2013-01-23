/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "gui/sdlinput.h"

#include "gui/widgets/extendedlistmodel.h"

#include <guichan/focushandler.hpp>
#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include <guichan/key.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

ExtendedListBox::ExtendedListBox(const Widget2 *const widget,
                                 gcn::ListModel *const listModel,
                                 const std::string &skin,
                                 int rowHeight) :
    ListBox(widget, listModel, skin),
    mRowHeight(rowHeight),
    mImagePadding(mSkin ? mSkin->getOption("imagePadding") : 0),
    mSpacing(mSkin ? mSkin->getOption("spacing") : 0),
    mItemPadding(mSkin ? mSkin->getOption("itemPadding") : 1)
{
    if (!mRowHeight)
    {
        const gcn::Font *font = getFont();
        if (font)
            mRowHeight = font->getHeight() + 2 * mItemPadding;
        else
            mRowHeight = 13;
    }
}

ExtendedListBox::~ExtendedListBox()
{
}

void ExtendedListBox::draw(gcn::Graphics *graphics)
{
    if (!mListModel)
        return;

    BLOCK_START("ExtendedListBox::draw")
    ExtendedListModel *const model = static_cast<ExtendedListModel* const>(
        mListModel);
    Graphics *const g = static_cast<Graphics *const>(graphics);

    updateAlpha();
    gcn::Font *const font = getFont();

    const int height = getRowHeight();
    int textPos = (height - font->getHeight()) / 2 + mPadding;
    if (textPos < 0)
        textPos = 0;

    // Draw filled rectangle around the selected list element
    if (mSelected >= 0)
    {
        mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
        graphics->setColor(mHighlightColor);
        graphics->fillRectangle(gcn::Rectangle(mPadding,
            height * mSelected + mPadding,
            getWidth() - 2 * mPadding, height));
    }

    // Draw the list elements
    graphics->setColor(mForegroundColor);
    for (int i = 0, y = 0; i < mListModel->getNumberOfElements();
         ++i, y += height)
    {
        if (i != mSelected)
        {
            const Image *const image = model->getImageAt(i);
            if (!image)
            {
                font->drawString(graphics, mListModel->getElementAt(i),
                    mPadding, y + textPos);
            }
            else
            {
                g->drawImage(image, mImagePadding, y + (height
                    - image->getHeight()) / 2 + mPadding);
                font->drawString(graphics, mListModel->getElementAt(i),
                    image->getWidth() + mImagePadding + mSpacing, y + textPos);
            }
        }
    }
    if (mSelected >= 0)
    {
        const Image *const image = model->getImageAt(mSelected);
        if (!image)
        {
            font->drawString(graphics, mListModel->getElementAt(mSelected),
                mPadding, mSelected * height + textPos);
        }
        else
        {
            const int y = mSelected * height;
            g->drawImage(image, mImagePadding, y + (height
                - image->getHeight()) / 2 + mPadding);
            graphics->setColor(mForegroundSelectedColor);
            font->drawString(graphics, mListModel->getElementAt(mSelected),
                image->getWidth() + mImagePadding + mSpacing, y + textPos);
        }
    }
    BLOCK_END("ExtendedListBox::draw")
}
