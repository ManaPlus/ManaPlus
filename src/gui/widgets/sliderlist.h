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

#ifndef GUI_WIDGETS_SLIDERLIST_H
#define GUI_WIDGETS_SLIDERLIST_H

#include "listeners/actionlistener.h"
#include "listeners/mouselistener.h"

#include "gui/widgets/container.h"

#include "localconsts.h"

class Button;
class Label;
class ListModel;

class SliderList final : public Container,
                         public ActionListener,
                         public MouseListener
{
    public:
        SliderList(const Widget2 *const widget,
                   ListModel *const listModel);

        A_DELETE_COPY(SliderList)

        ~SliderList();

        void postInit(ActionListener *const listener,
                      const std::string &eventId);

        void updateAlpha();

        void mouseWheelMovedUp(MouseEvent& mouseEvent) override final;

        void mouseWheelMovedDown(MouseEvent& mouseEvent) override final;

        void resize();

        void draw(Graphics *graphics) override final;

        void action(const ActionEvent &event) override final;

        void setSelectedString(const std::string &str);

        std::string getSelectedString() const A_WARN_UNUSED;

        void setSelected(const int idx);

        void adjustSize();

        int getSelected() const A_WARN_UNUSED
        { return mSelectedIndex; }

    protected:
        void updateLabel();

        int getMaxLabelWidth() const A_WARN_UNUSED;

        Button *mButtons[2];
        Label *mLabel;
        ListModel *mListModel;
        std::string mPrevEventId;
        std::string mNextEventId;
        int mOldWidth;
        int mSelectedIndex;
};

#endif  // GUI_WIDGETS_SLIDERLIST_H
