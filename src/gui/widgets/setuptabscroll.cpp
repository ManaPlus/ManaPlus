/*
 *  The ManaPlus Client
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/widgets/setuptabscroll.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"
#include "gui/widgets/vertcontainer.h"

#include "debug.h"

SetupTabScroll::SetupTabScroll() :
    SetupTab()
{
    mContainer = new VertContainer(32, false, 8);
    mScroll = new ScrollArea(mContainer);
    mScroll->setOpaque(false);
    mScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);

//    LayoutHelper h(this);
//    ContainerPlacer place = h.getPlacer(0, 0);
//    place(0, 0, mScroll, 10, 10);
}

SetupTabScroll::~SetupTabScroll()
{
    mScroll = 0;

    delete mContainer;
    mContainer = 0;

    std::set<SetupItem*>::iterator it = mAllItems.begin();
    std::set<SetupItem*>::iterator it_end = mAllItems.end();
    while (it != it_end)
    {
        delete *it;
        ++ it;
    }
    mAllItems.clear();
}

void SetupTabScroll::addControl(SetupItem *widget)
{
    const std::string actionId = widget->getActionEventId();
    if (!actionId.empty())
    {
        std::map<std::string, SetupItem*>::iterator iter
            = mItems.find(actionId);
        if (iter != mItems.end())
        {
            delete (*iter).second;
            mItems.erase(iter);
        }
        mItems[widget->getActionEventId()] = widget;
    }
    mAllItems.insert(widget);
}

void SetupTabScroll::addControl(SetupItem *widget, std::string event)
{
    std::map<std::string, SetupItem*>::iterator iter
        = mItems.find(event);
    if (iter != mItems.end())
    {
        delete (*iter).second;
        mItems.erase(iter);
    }
    mItems[event] = widget;
    mAllItems.insert(widget);
}

void SetupTabScroll::apply()
{
    std::map<std::string, SetupItem*>::const_iterator iter;
    for (iter = mItems.begin(); iter != mItems.end(); ++ iter)
    {
        if ((*iter).second)
            (*iter).second->apply((*iter).first);
    }
}

void SetupTabScroll::cancel()
{
    std::map<std::string, SetupItem*>::const_iterator iter;
    for (iter = mItems.begin(); iter != mItems.end(); ++ iter)
    {
        if ((*iter).second)
            (*iter).second->cancel((*iter).first);
    }
}

void SetupTabScroll::externalUpdated()
{
    std::map<std::string, SetupItem*>::const_iterator iter;
    for (iter = mItems.begin(); iter != mItems.end(); ++ iter)
    {
        SetupItem *widget = (*iter).second;
        if (widget && !widget->isMainConfig())
            (*iter).second->externalUpdated((*iter).first);
    }
}
