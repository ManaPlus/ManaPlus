/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "gui/widgets/radiogroup.h"

#include "gui/widgets/radiobutton.h"

#include "debug.h"

RadioGroup::RadioGroup(const Widget2 *const widget,
                       const std::string &group, const int height,
                       const int spacing) :
    WidgetGroup(widget, group, height, spacing)
{
}

gcn::Widget *RadioGroup::createWidget(const std::string &text) const
{
    RadioButton *const widget = new RadioButton(
        this, text, mGroup, mCount == 0);
    widget->adjustSize();
    return widget;
}
