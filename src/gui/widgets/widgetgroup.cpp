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

#include "gui/widgets/widgetgroup.h"

#include "debug.h"

WidgetGroup::WidgetGroup(const Widget2 *const widget,
                         const std::string &group, const int height,
                         const int spacing) :
    Container(widget),
    gcn::WidgetListener(),
    gcn::ActionListener(),
    mSpacing(spacing),
    mCount(0),
    mGroup(group),
    mLastX(spacing)
{
    setHeight(height);
    addWidgetListener(this);
}

void WidgetGroup::addButton(const std::string &tag)
{
    addButton(tag, tag);
}

void WidgetGroup::addButton(const std::string &restrict text,
                            const std::string &restrict tag)
{
    if (text.empty() || tag.empty())
        return;

    Widget *const widget = createWidget(text);
    if (widget)
    {
        widget->setActionEventId(mActionEventId + tag);
        widget->addActionListener(this);
        add(widget, mSpacing);
    }
}

void WidgetGroup::action(const gcn::ActionEvent &event)
{
    for (ActionListenerIterator iter = mActionListeners.begin();
         iter != mActionListeners.end(); ++iter)
    {
        (*iter)->action(event);
    }
}

void WidgetGroup::add(gcn::Widget *const widget, const int spacing)
{
    if (!widget)
        return;

    Container::add(widget);
    widget->setPosition(mLastX, spacing);
    mCount++;
    mLastX += widget->getWidth() + 2 * mSpacing;
}

void WidgetGroup::clear()
{
    Container::clear();

    mCount = 0;
}

void WidgetGroup::widgetResized(const gcn::Event &event A_UNUSED)
{
}
