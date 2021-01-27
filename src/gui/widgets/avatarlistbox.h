/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_WIDGETS_AVATARLISTBOX_H
#define GUI_WIDGETS_AVATARLISTBOX_H

#include "gui/widgets/listbox.h"

class AvatarListModel;
class Image;

class AvatarListBox final : public ListBox,
                            public ConfigListener
{
    public:
        AvatarListBox(const Widget2 *const widget,
                      AvatarListModel *const model);

        A_DELETE_COPY(AvatarListBox)

        ~AvatarListBox() override final;

        /**
         * Draws the list box.
         */
        void draw(Graphics *gcnGraphics) override final A_NONNULL(2);

        void safeDraw(Graphics *gcnGraphics) override final A_NONNULL(2);

        void mousePressed(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event A_UNUSED) override final;

        void optionChanged(const std::string &value) override final;

    private:
        int mImagePadding;
        bool mShowGender;
        bool mShowLevel;

        static int instances;
        static Image *onlineIcon;
        static Image *offlineIcon;
};

#endif  // GUI_WIDGETS_AVATARLISTBOX_H
