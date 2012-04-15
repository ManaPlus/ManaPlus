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

#ifndef GUI_TABSTRIP_H
#define GUI_TABSTRIP_H

#include "gui/widgets/widgetgroup.h"

#include <guichan/actionevent.hpp>
#include <guichan/widget.hpp>

class TabStrip : public WidgetGroup
{
    public:
        TabStrip(std::string group, int height, int spacing = 0);

        TabStrip(int height, int spacing = 0);

        gcn::Widget *createWidget(std::string name);

        void action(const gcn::ActionEvent &event);
};

#endif
