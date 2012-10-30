/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef GUI_WIDGETGROUP_H
#define GUI_WIDGETGROUP_H

#include "gui/widgets/container.h"

#include <guichan/actionlistener.hpp>
#include <guichan/widgetlistener.hpp>

#include "localconsts.h"

class WidgetGroup : public Container,
                    public gcn::WidgetListener,
                    public gcn::ActionListener
{
    public:
        WidgetGroup(const Widget2 *const widget,
                    const std::string &group, const int height,
                    const int spacing);

        A_DELETE_COPY(WidgetGroup)

        virtual void addButton(std::string tag);

        virtual void addButton(std::string text, std::string tag);

        void action(const gcn::ActionEvent &event) override;

        virtual void add(gcn::Widget *const widget,
                         const int spacing);

        virtual void clear();

        void widgetResized(const gcn::Event &event) override;

        virtual Widget *createWidget(const std::string &name)
                                     A_WARN_UNUSED = 0;

    protected:
        int mSpacing;
        int mCount;
        std::string mGroup;

    private:
        int mLastX;
};

#endif
