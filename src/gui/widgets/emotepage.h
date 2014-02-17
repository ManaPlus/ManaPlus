/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_EMOTEPAGE_H
#define GUI_WIDGETS_EMOTEPAGE_H

#include "gui/widgets/widget2.h"

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "gui/base/widget.hpp"

#include "localconsts.h"

class EmotePage final : public gcn::Widget,
                        public MouseListener,
                        public WidgetListener
{
    public:
        explicit EmotePage(const Widget2 *const widget);

        A_DELETE_COPY(EmotePage)

        ~EmotePage();

        void draw(Graphics *graphics) override final;

        void mousePressed(MouseEvent &mouseEvent) override final;

        int getIndexFromGrid(const int x, const int y) const;

        void widgetResized(const Event &event A_UNUSED) override final;

        void widgetMoved(const Event &event A_UNUSED) override final;

        void resetAction();

        int getSelectedIndex() const
        { return mSelectedIndex; }

    private:
        ImageSet *mEmotes;
        ImageCollection *mVertexes;
        int mSelectedIndex;
        bool mRedraw;
};

#endif  // GUI_WIDGETS_EMOTEPAGE_H
