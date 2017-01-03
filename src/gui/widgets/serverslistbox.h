/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SERVERSLISTBOX_H
#define GUI_WIDGETS_SERVERSLISTBOX_H

#include "gui/widgets/listbox.h"

#include "gui/models/serverslistmodel.h"

#include "localconsts.h"

class ServersListBox final : public ListBox
{
    public:
        ServersListBox(const Widget2 *const widget,
                       ServersListModel *const model) :
            ListBox(widget, model, "serverslistbox.xml"),
            mNotSupportedColor(getThemeColor(
                ThemeColorId::SERVER_VERSION_NOT_SUPPORTED)),
            mNotSupportedColor2(getThemeColor(
                ThemeColorId::SERVER_VERSION_NOT_SUPPORTED_OUTLINE))
        {
            mHighlightColor = getThemeColor(ThemeColorId::HIGHLIGHT);
        }

        void draw(Graphics *const graphics) override final A_NONNULL(2)
        {
            if (!mListModel)
                return;

            ServersListModel *const model
                = static_cast<ServersListModel *const>(mListModel);

            updateAlpha();

            mHighlightColor.a = CAST_S32(mAlpha * 255.0F);
            graphics->setColor(mHighlightColor);

            const int height = getRowHeight();
            mNotSupportedColor.a = CAST_S32(mAlpha * 255.0F);

            // Draw filled rectangle around the selected list element
            if (mSelected >= 0)
            {
                graphics->fillRectangle(Rect(mPadding,
                    height * mSelected + mPadding,
                    getWidth() - 2 * mPadding,
                    height));
            }

            Font *const font1 = boldFont;
            Font *const font2 = getFont();
            const int fontHeight = font1->getHeight();
            const int pad1 = fontHeight + mPadding;
            const int pad2 = height / 4 + mPadding;
            const int width = getWidth();
            // Draw the list elements
            for (int i = 0, y = 0; i < model->getNumberOfElements();
                 ++i, y += height)
            {
                const ServerInfo &info = model->getServer(i);

                const Color *color1;
                const Color *color2;
                if (mSelected == i)
                {
                    color1 = &mForegroundSelectedColor;
                    color2 = &mForegroundSelectedColor2;
                }
                else
                {
                    color1 = &mForegroundColor;
                    color2 = &mForegroundColor2;
                }

                int top;
                int x = mPadding;

                if (!info.name.empty())
                {
                    x += font1->getWidth(info.name) + 15;
                    font1->drawString(graphics,
                        *color1,
                        *color2,
                        info.name,
                        mPadding,
                        y + mPadding);
                    top = y + pad1;
                }
                else
                {
                    top = y + pad2;
                }

                if (!info.description.empty())
                {
                    font2->drawString(graphics,
                        *color1,
                        *color2,
                        info.description,
                        x,
                        y + mPadding);
                }
                font2->drawString(graphics,
                    *color1,
                    *color2,
                    model->getElementAt(i),
                    mPadding,
                    top);

                if (info.version.first > 0)
                {
                    font2->drawString(graphics,
                        mNotSupportedColor,
                        mNotSupportedColor2,
                        info.version.second,
                        width - info.version.first - mPadding,
                        top);
                }
            }
        }

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2)
        {
            ServersListBox::draw(graphics);
        }

        unsigned int getRowHeight() const override final
        {
            return 2 * getFont()->getHeight() + 5;
        }
    private:
        Color mNotSupportedColor;
        Color mNotSupportedColor2;
};

#endif  // GUI_WIDGETS_SERVERSLISTBOX_H
