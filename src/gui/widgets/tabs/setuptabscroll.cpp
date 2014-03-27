/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/widgets/tabs/setuptabscroll.h"

#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"
#include "gui/widgets/vertcontainer.h"

#include "utils/delete2.h"

#include "debug.h"

SetupTabScroll::SetupTabScroll(const Widget2 *const widget) :
    SetupTab(widget),
    mContainer(new VertContainer(this, 25, false, 8)),
    mScroll(new ScrollArea(this, mContainer, false)),
    mItems(),
    mAllItems(),
    mPreferredFirstItemSize(200)
{
    mScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
}

SetupTabScroll::~SetupTabScroll()
{
    mScroll = nullptr;
    delete2(mContainer);
    removeItems();
}

void SetupTabScroll::removeItems()
{
    std::set<SetupItem*>::iterator it = mAllItems.begin();
    const std::set<SetupItem*>::iterator it_end = mAllItems.end();
    while (it != it_end)
    {
        delete *it;
        ++ it;
    }
    mAllItems.clear();

    mItems.clear();
}

void SetupTabScroll::clear()
{
    removeItems();
    mContainer->removeControls();
    mContainer->clear();
}

void SetupTabScroll::addControl(SetupItem *const widget)
{
    const std::string actionId = widget->getActionEventId();
    if (!actionId.empty())
    {
        const std::map<std::string, SetupItem*>::iterator iter
            = mItems.find(actionId);
        if (iter != mItems.end())
        {
            delete (*iter).second;
            mItems.erase(iter);
        }
        mItems[actionId] = widget;
    }
    mAllItems.insert(widget);
}

void SetupTabScroll::addControl(SetupItem *const widget,
                                const std::string &event)
{
    const std::map<std::string, SetupItem*>::iterator iter
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
    for (std::map<std::string, SetupItem*>::const_iterator
         iter = mItems.begin(), iter_end = mItems.end();
         iter != iter_end; ++ iter)
    {
        if ((*iter).second)
            (*iter).second->apply((*iter).first);
    }
}

void SetupTabScroll::cancel()
{
    for (std::map<std::string, SetupItem*>::const_iterator
         iter = mItems.begin(), iter_end = mItems.end();
         iter != iter_end; ++ iter)
    {
        if ((*iter).second)
            (*iter).second->cancel((*iter).first);
    }
}

void SetupTabScroll::externalUpdated()
{
    for (std::map<std::string, SetupItem*>::const_iterator
         iter = mItems.begin(), iter_end = mItems.end();
         iter != iter_end; ++ iter)
    {
        SetupItem *const widget = (*iter).second;
        if (widget && !widget->isMainConfig())
            widget->externalUpdated((*iter).first);
    }
}

void SetupTabScroll::externalUnloaded()
{
    for (std::map<std::string, SetupItem*>::const_iterator
         iter = mItems.begin(), iter_end = mItems.end();
         iter != iter_end; ++ iter)
    {
        SetupItem *const widget = (*iter).second;
        if (widget && !widget->isMainConfig())
            widget->externalUnloaded((*iter).first);
    }
}

void SetupTabScroll::widgetResized(const Event &event A_UNUSED)
{
    mScroll->setWidth(getWidth() - 12);
    mScroll->setHeight(getHeight() - 12 - 12);
}

void SetupTabScroll::reread(const std::string &name)
{
    SetupItem *const item = mItems[name + "Event"];
    if (item)
        item->rereadValue();
}
