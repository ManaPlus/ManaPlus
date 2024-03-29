/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/windowcontainer.h"

#include "gui/widgets/window.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/foreach.h"

#include "debug.h"

WindowContainer *windowContainer = nullptr;

WindowContainer::WindowContainer(const Widget2 *const widget) :
    Container(widget),
    mDeathList(),
    mDeathSet()
{
}

void WindowContainer::slowLogic()
{
    delete_all(mDeathList);
    mDeathList.clear();
    mDeathSet.clear();
}

void WindowContainer::scheduleDelete(Widget *const widget)
{
    if (widget == nullptr)
        return;

    if (mDeathSet.find(widget) == mDeathSet.end())
    {
        mDeathList.push_back(widget);
        mDeathSet.insert(widget);
    }
    else
    {
        reportAlways("double adding pointer %p for deletion in scheduleDelete",
            static_cast<void*>(widget))
    }
}

void WindowContainer::adjustAfterResize(const int oldScreenWidth,
                                        const int oldScreenHeight)
{
    FOR_EACH (WidgetListIterator, i, mWidgets)
    {
        if (Window *const window = dynamic_cast<Window*>(*i))
            window->adjustPositionAfterResize(oldScreenWidth, oldScreenHeight);
    }
}

void WindowContainer::moveWidgetAfter(Widget *const after,
                                      Widget *const widget)
{
    const WidgetListIterator widgetIter = std::find(
        mWidgets.begin(), mWidgets.end(), widget);

    if (widgetIter != mWidgets.end())
    {
        WidgetListIterator afterIter = std::find(
            mWidgets.begin(), mWidgets.end(), after);

        if (afterIter != mWidgets.end())
        {
            ++ afterIter;
            mWidgets.erase(widgetIter);
            mWidgets.insert(afterIter, widget);
        }
    }

    const WidgetListIterator widgetIter2 = std::find(
        mLogicWidgets.begin(), mLogicWidgets.end(), widget);

    if (widgetIter2 != mLogicWidgets.end())
    {
        WidgetListIterator afterIter = std::find(
            mLogicWidgets.begin(), mLogicWidgets.end(), after);

        if (afterIter != mLogicWidgets.end())
        {
            ++ afterIter;
            mLogicWidgets.erase(widgetIter2);
            mLogicWidgets.insert(afterIter, widget);
        }
    }
}

#ifdef USE_PROFILER
void WindowContainer::draw(Graphics *const graphics)
{
    BLOCK_START("WindowContainer::draw")
    Container::draw(graphics);
    BLOCK_END("WindowContainer::draw")
}
#endif  // USE_PROFILER
