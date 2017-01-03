/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "gui/widgets/colorpage.h"

#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "gui/models/colormodel.h"

#include "render/graphics.h"

#include "debug.h"

ColorPage::ColorPage(const Widget2 *const widget,
                     ListModel *const listModel,
                     const std::string &skin) :
    ListBox(widget, listModel, skin)
{
    mItemPadding = mSkin ? mSkin->getOption("itemPadding") : 1;
    const Font *const font = getFont();
    mRowHeight = CAST_U32(font->getHeight() +
        2 * mItemPadding);
    if (mListModel)
    {
        setHeight(CAST_S32(getRowHeight()) *
            mListModel->getNumberOfElements()
            + 2 * mPadding + 20);
    }
}

ColorPage::~ColorPage()
{
}

void ColorPage::draw(Graphics *const graphics)
{
    BLOCK_START("ColorPage::draw")

    const ColorModel *const model = static_cast<ColorModel* const>(
        mListModel);

    mHighlightColor.a = CAST_U32(mAlpha * 255.0F);
    updateAlpha();
    Font *const font = getFont();

    const int rowHeight = CAST_S32(getRowHeight());
    const int width = mDimension.width;

    if (mSelected >= 0)
    {
        graphics->setColor(mHighlightColor);
        graphics->fillRectangle(Rect(mPadding,
            rowHeight * mSelected + mPadding,
            mDimension.width - 2 * mPadding, rowHeight));

        const ColorPair *const colors = model->getColorAt(mSelected);
        const std::string str = mListModel->getElementAt(mSelected);
        font->drawString(graphics,
            *colors->color1,
            *colors->color2,
            str,
            (width - font->getWidth(str)) / 2,
            mSelected * rowHeight + mPadding);
    }

    const int sz = mListModel->getNumberOfElements();
    for (int i = 0, y = mPadding; i < sz; ++i, y += rowHeight)
    {
        if (i != mSelected)
        {
            const ColorPair *const colors = model->getColorAt(i);
            const std::string str = mListModel->getElementAt(i);
            font->drawString(graphics,
                *colors->color1,
                *colors->color2,
                str,
                (width - font->getWidth(str)) / 2,
                y);
        }
    }

    BLOCK_END("ColorPage::draw")
}

void ColorPage::safeDraw(Graphics *const graphics)
{
    ColorPage::draw(graphics);
}

void ColorPage::resetAction()
{
    setSelected(-1);
}

void ColorPage::adjustSize()
{
    BLOCK_START("ColorPage::adjustSize")
    if (mListModel)
    {
        setHeight(CAST_S32(getRowHeight()) *
            mListModel->getNumberOfElements() +
            2 * mPadding + 20);
    }
    BLOCK_END("ColorPage::adjustSize")
}
