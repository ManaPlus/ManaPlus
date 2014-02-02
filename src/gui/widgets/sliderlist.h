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

#include <guichan/actionlistener.hpp>
#include <guichan/listmodel.hpp>
#include <guichan/mouselistener.hpp>

#include "gui/widgets/container.h"

#include "localconsts.h"

class Button;
class Label;

class SliderList final : public Container,
                         public gcn::ActionListener,
                         public gcn::MouseListener
{
    public:
        SliderList(const Widget2 *const widget,
                   gcn::ListModel *const listModel);

        A_DELETE_COPY(SliderList)

        ~SliderList();

        void postInit(gcn::ActionListener *const listener,
                      const std::string &eventId);

        void updateAlpha();

        void mouseWheelMovedUp(gcn::MouseEvent& mouseEvent) override final;

        void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent) override final;

        void resize();

        void draw(gcn::Graphics *graphics) override final;

        void action(const gcn::ActionEvent &event) override final;

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
        gcn::ListModel *mListModel;
        std::string mPrevEventId;
        std::string mNextEventId;
        int mOldWidth;
        int mSelectedIndex;
};

#endif  // GUI_WIDGETS_SLIDERLIST_H
