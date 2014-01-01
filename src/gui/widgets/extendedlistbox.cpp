/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "gui/widgets/extendedlistmodel.h"

#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

ExtendedListBox::ExtendedListBox(const Widget2 *const widget,
                                 gcn::ListModel *const listModel,
                                 const std::string &skin,
                                 const int rowHeight) :
    ListBox(widget, listModel, skin),
    mImagePadding(mSkin ? mSkin->getOption("imagePadding") : 0),
    mSpacing(mSkin ? mSkin->getOption("spacing") : 0),
    mHeight(0),
    mListItems(),
    mSelectedItems()
{
    if (rowHeight)
        mRowHeight = rowHeight;
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

    const int height = mRowHeight;
    const int pad2 = 2 + mPadding;
    const int width = mDimension.width;
    int textPos = (height - font->getHeight()) / 2 + mPadding;
    if (textPos < 0)
        textPos = 0;

    const int sz = mListModel->getNumberOfElements();
    mListItems.clear();
    mSelectedItems.clear();
    int y = 0;
    const int insideWidth = width - pad2;
    for (int f = 0; f < sz; f ++)
    {
        int row = f;
        bool useImage = true;
        std::string str = mListModel->getElementAt(f);
        int strWidth = font->getWidth(str) + 8;

        const Image *const image = model->getImageAt(row);
        if (image)
            strWidth += image->getWidth() + mImagePadding;

        std::vector<ExtendedListBoxItem> &list =
            row == mSelected ? mSelectedItems : mListItems;

        if (insideWidth < strWidth)
        {
            const int strSize = str.size();
            int divPos = strSize / 2;
            if (divPos > 0 && static_cast<unsigned char>(
                str[divPos - 1]) >= 0xc0)
            {
                divPos --;
            }
            for (int d = divPos; d > 10; d --)
            {
                if (str[d] == 32)
                {
                    divPos = d + 1;
                    break;
                }
            }
            list.push_back(ExtendedListBoxItem(row,
                str.substr(0, divPos), useImage, y));
            str = str.substr(divPos);
            y += height;
            useImage = false;
        }
        list.push_back(ExtendedListBoxItem(row, str, useImage, y));

        y += height;
    }
    mHeight = y + height;

    const int itemsSz = mListItems.size();
    const int selSz = mSelectedItems.size();
    int minY = -1;
    int maxY = -1;
    for (int f = 0; f < selSz; f ++)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int y1 = item.y;
        if (minY == -1)
            minY = y1;
        if (maxY < y1)
            maxY = y1;
    }

    if (minY != -1)
    {
        mHighlightColor.a = static_cast<int>(mAlpha * 255.0F);
        graphics->setColor(mHighlightColor);
        graphics->fillRectangle(gcn::Rectangle(mPadding, minY + mPadding,
            width - pad2, maxY - minY + height));
    }

    for (int f = 0; f < itemsSz; ++f)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int row1 = item.row;
        if (item.image)
        {
            const Image *const image = model->getImageAt(row1);
            if (image)
            {
                g->drawImage2(image, mImagePadding, item.y + (height
                    - image->getHeight()) / 2 + mPadding);
            }
        }
    }

    g->setColorAll(mForegroundColor, mForegroundColor2);

    for (int f = 0; f < itemsSz; ++f)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int row1 = item.row;
        const int y1 = item.y;
        const Image *const image = model->getImageAt(row1);
        if (!image || !item.image)
        {
            font->drawString(graphics, item.text, mPadding, y1 + textPos);
        }
        else
        {
            font->drawString(graphics, item.text,
                image->getWidth() + mImagePadding + mSpacing, y1 + textPos);
        }
    }

    for (int f = 0; f < selSz; ++f)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int row1 = item.row;
        if (item.image)
        {
            const Image *const image = model->getImageAt(row1);
            if (image)
            {
                g->drawImage2(image, mImagePadding, item.y + (height
                    - image->getHeight()) / 2 + mPadding);
            }
        }
    }

    g->setColorAll(mForegroundSelectedColor, mForegroundSelectedColor2);

    for (int f = 0; f < selSz; ++f)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int row1 = item.row;
        const int y1 = item.y;
        const Image *const image = model->getImageAt(row1);
        if (!image || !item.image)
        {
            font->drawString(graphics, item.text, mPadding, y1 + textPos);
        }
        else
        {
            font->drawString(graphics, item.text,
                image->getWidth() + mImagePadding + mSpacing, y1 + textPos);
        }
    }

    BLOCK_END("ExtendedListBox::draw")
}

void ExtendedListBox::adjustSize()
{
    if (mHeight)
        setHeight(mHeight + 2 * mPadding);
    else
        ListBox::adjustSize();
}

int ExtendedListBox::getSelectionByMouse(const int y) const
{
    if (mListItems.empty() && mSelectedItems.empty())
        return ListBox::getSelectionByMouse(y);

    const int height = mRowHeight;
    const int itemsSz = mListItems.size();
    for (int f = 0; f < itemsSz; f ++)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int y2 = item.y;
        if (y2 <= y && y2 + height > y)
            return item.row;
    }

    const int selSz = mSelectedItems.size();
    for (int f = 0; f < selSz; f ++)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int y2 = item.y;
        if (y2 <= y && y2 + height > y)
            return item.row;
    }
    return 0;
}
