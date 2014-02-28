/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef GUI_WIDGETS_AVATARLISTBOX_H
#define GUI_WIDGETS_AVATARLISTBOX_H

#include "listeners/configlistener.h"

#include "gui/widgets/listbox.h"

#include <string>

class AvatarListModel;
class Image;

class AvatarListBox final : public ListBox,
                            public ConfigListener
{
public:
    AvatarListBox(const Widget2 *const widget,
                  AvatarListModel *const model);

    A_DELETE_COPY(AvatarListBox)

    ~AvatarListBox();

    /**
     * Draws the list box.
     */
    void draw(Graphics *gcnGraphics) override final;

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
