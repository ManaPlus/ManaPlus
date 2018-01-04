/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_WIDGETGROUP_H
#define GUI_WIDGETS_WIDGETGROUP_H

#include "gui/widgets/container.h"

#include "listeners/actionlistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

class WidgetGroup notfinal : public Container,
                             public WidgetListener,
                             public ActionListener
{
    public:
        A_DELETE_COPY(WidgetGroup)

        virtual void addButton(const std::string &restrict text,
                               const std::string &restrict tag,
                               const bool pressed);

        void action(const ActionEvent &event) override;

        virtual void addWidget(Widget *const widget,
                               const int spacing);

        void clear() override final;

        void widgetResized(const Event &event) override final A_CONST;

        virtual Widget *createWidget(const std::string &name,
                                     const bool pressed)
                                     const A_WARN_UNUSED = 0;

    protected:
        WidgetGroup(const Widget2 *const widget,
                    const std::string &group,
                    const int height,
                    const int spacing);

        int mSpacing;
        int mCount;
        std::string mGroup;

    private:
        int mLastX;
};

#endif  // GUI_WIDGETS_WIDGETGROUP_H
