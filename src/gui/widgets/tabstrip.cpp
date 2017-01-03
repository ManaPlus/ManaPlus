/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "debug.h"

TabStrip::TabStrip(const Widget2 *const widget,
                   const std::string &group,
                   const int height,
                   const int spacing) :
    WidgetGroup(widget, group, height, spacing),
    mPressFirst(true)
{
    mAllowLogic = false;
}

TabStrip::TabStrip(const Widget2 *const widget,
                   const int height,
                   const int spacing) :
    WidgetGroup(widget, "", height, spacing),
    mPressFirst(true)
{
    mAllowLogic = false;
}

Widget *TabStrip::createWidget(const std::string &text,
                               const bool pressed) const
{
    Button *const widget = new Button(this);
    widget->setStick(true);
    widget->setCaption(text);
    widget->adjustSize();
    if ((!mCount && mPressFirst) || pressed)
        widget->setPressed(true);
    widget->setTag(CAST_S32(mWidgets.size()));
    return widget;
}

void TabStrip::action(const ActionEvent &event)
{
    WidgetGroup::action(event);
    if (event.getSource())
    {
        Widget *const widget = event.getSource();
        Button *const button = static_cast<Button*>(widget);
        if (!button)
            return;
        if (button->isPressed2())
        {
            FOR_EACH (WidgetListConstIterator, iter, mWidgets)
            {
                if (*iter != widget)
                {
                    Button *const button2 = static_cast<Button*>(*iter);
                    button2->setPressed(false);
                }
            }
        }
        else
        {
            button->setPressed(true);
        }
    }
}

void TabStrip::nextTab()
{
    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
    {
        Button *button = static_cast<Button*>(*iter);
        if (button->isPressed2())
        {
            button->setPressed(false);
            ++iter;
            if (iter == mWidgets.end())
                iter = mWidgets.begin();
            button = static_cast<Button*>(*iter);
            action(ActionEvent(button, button->getActionEventId()));
            return;
        }
    }
}

void TabStrip::prevTab()
{
    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
    {
        Button *button = static_cast<Button*>(*iter);
        if (button->isPressed2())
        {
            button->setPressed(false);
            if (iter == mWidgets.begin())
                iter = mWidgets.end();
            if (iter == mWidgets.begin())
                return;
            --iter;
            button = static_cast<Button*>(*iter);
            action(ActionEvent(button, button->getActionEventId()));
            return;
        }
    }
}
