/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "gui/models/extendedlistmodel.h"

#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "render/graphics.h"

#include "debug.h"

ExtendedListBox::ExtendedListBox(const Widget2 *const widget,
                                 ListModel *const listModel,
                                 const std::string &skin,
                                 const unsigned int rowHeight) :
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

void ExtendedListBox::draw(Graphics *const graphics)
{
    if (!mListModel)
        return;

    BLOCK_START("ExtendedListBox::draw")
    ExtendedListModel *const model = static_cast<ExtendedListModel* const>(
        mListModel);

    updateAlpha();
    Font *const font = getFont();

    const int height = CAST_S32(mRowHeight);
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
            const size_t strSize = str.size();
            size_t divPos = strSize / 2;
            if (divPos > 0 && CAST_U8(
                str[divPos - 1]) >= 0xc0)
            {
                divPos --;
            }
            for (size_t d = divPos; d > 10; d --)
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

    const size_t itemsSz = mListItems.size();
    const size_t selSz = mSelectedItems.size();
    int minY = -1;
    int maxY = -1;
    for (size_t f = 0; f < selSz; f ++)
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
        mHighlightColor.a = CAST_U32(mAlpha * 255.0F);
        graphics->setColor(mHighlightColor);
        graphics->fillRectangle(Rect(mPadding, minY + mPadding,
            width - pad2, maxY - minY + height));
    }

    for (size_t f = 0; f < itemsSz; ++f)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int row1 = item.row;
        if (item.image)
        {
            const Image *const image = model->getImageAt(row1);
            if (image)
            {
                graphics->drawImage(image,
                    mImagePadding,
                    item.y + (height - image->getHeight()) / 2 + mPadding);
            }
        }
    }

    for (size_t f = 0; f < itemsSz; ++f)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int row1 = item.row;
        const int y1 = item.y;
        const Image *const image = model->getImageAt(row1);
        if (!image || !item.image)
        {
            font->drawString(graphics,
                mForegroundColor, mForegroundColor2,
                item.text,
                mPadding, y1 + textPos);
        }
        else
        {
            font->drawString(graphics,
                mForegroundColor, mForegroundColor2,
                item.text,
                image->getWidth() + mImagePadding + mSpacing, y1 + textPos);
        }
    }

    for (size_t f = 0; f < selSz; ++f)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int row1 = item.row;
        const int y1 = item.y;
        const Image *const image = model->getImageAt(row1);
        if (!image || !item.image)
        {
            font->drawString(graphics,
                mForegroundSelectedColor, mForegroundSelectedColor2,
                item.text,
                mPadding, y1 + textPos);
        }
        else
        {
            font->drawString(graphics,
                mForegroundSelectedColor, mForegroundSelectedColor2,
                item.text,
                image->getWidth() + mImagePadding + mSpacing, y1 + textPos);
        }
    }

    for (size_t f = 0; f < selSz; ++f)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int row1 = item.row;
        if (item.image)
        {
            const Image *const image = model->getImageAt(row1);
            if (image)
            {
                graphics->drawImage(image,
                    mImagePadding,
                    item.y + (height - image->getHeight()) / 2 + mPadding);
            }
        }
    }

    BLOCK_END("ExtendedListBox::draw")
}

void ExtendedListBox::safeDraw(Graphics *const graphics)
{
    ExtendedListBox::draw(graphics);
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

    const int height = CAST_S32(mRowHeight);
    const size_t itemsSz = mListItems.size();
    for (size_t f = 0; f < itemsSz; f ++)
    {
        const ExtendedListBoxItem &item = mListItems[f];
        const int y2 = item.y;
        if (y2 <= y && y2 + height > y)
            return item.row;
    }

    const size_t selSz = mSelectedItems.size();
    for (size_t f = 0; f < selSz; f ++)
    {
        const ExtendedListBoxItem &item = mSelectedItems[f];
        const int y2 = item.y;
        if (y2 <= y && y2 + height > y)
            return item.row;
    }
    return 0;
}
