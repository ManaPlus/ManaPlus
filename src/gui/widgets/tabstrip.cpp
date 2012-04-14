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

#include "gui/widgets/tabstrip.h"

#include "gui/widgets/button.h"
#include "gui/widgets/tab.h"

#include "logger.h"

#include "debug.h"

TabStrip::TabStrip(std::string group, int height, int spacing) :
    WidgetGroup(group, height, spacing)
{
}

gcn::Widget *TabStrip::createWidget(std::string text)
{
    Button *widget = new Button();
    widget->setStick(true);
    widget->setCaption(text);
    widget->adjustSize();
    if (!mCount)
        widget->setPressed(true);
    return widget;
}

void TabStrip::action(const gcn::ActionEvent &event)
{
    WidgetGroup::action(event);
    if (event.getSource())
    {
        gcn::Widget *widget = event.getSource();
        if (static_cast<Button*>(widget)->isPressed2())
        {
            WidgetListConstIterator iter;
            for (iter = mWidgets.begin(); iter != mWidgets.end(); ++ iter)
            {
                if (*iter != widget)
                {
                    Button *button = static_cast<Button*>(*iter);
                    button->setPressed(false);
                }
            }
        }
    }
}
