/*
 *  The ManaPlus Client
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

#include "gui/widgets/statspage.h"

#include "gui/widgets/scrollarea.h"
#include "gui/widgets/vertcontainer.h"

#include "gui/widgets/attrs/derdisplay.h"

#include "utils/foreach.h"

#include "resources/db/statdb.h"

#include "debug.h"

StatsPage::StatsPage(const Widget2 *const widget,
                     const std::string &page) :
    Container(widget),
    WidgetListener(),
    AttributeListener(),
    StatListener(),
    mAttrs(),
    mAttrCont(new VertContainer(this, 32, true, 0)),
    mAttrScroll(new ScrollArea(this, mAttrCont, Opaque_false, std::string()))
{
    addWidgetListener(this);
    setSelectable(false);

    mAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    mAttrScroll->setSelectable(false);
    mAttrCont->setSelectable(false);

    add(mAttrScroll);
    const STD_VECTOR<BasicStat> &basicStats = StatDb::getStats(page);
    FOR_EACH (STD_VECTOR<BasicStat>::const_iterator, it, basicStats)
    {
        const BasicStat &stat = *it;
        AttrDisplay *disp = new DerDisplay(this,
            stat.attr,
            stat.name,
            stat.tag);
        disp->update();
        mAttrCont->add2(disp, true, -1);
        mAttrs[stat.attr] = disp;
    }
}

void StatsPage::widgetResized(const Event &event A_UNUSED)
{
    mAttrScroll->setSize(getWidth(), getHeight());
}

void StatsPage::attributeChanged(const AttributesT id,
                                 const int64_t oldVal A_UNUSED,
                                 const int64_t newVal A_UNUSED)
{
    const Attrs::const_iterator it = mAttrs.find(id);
    if (it != mAttrs.end() && (it->second != nullptr))
        it->second->update();
}

void StatsPage::statChanged(const AttributesT id,
                            const int oldVal1 A_UNUSED,
                            const int oldVal2 A_UNUSED)
{
    const Attrs::const_iterator it = mAttrs.find(id);
    if (it != mAttrs.end() && (it->second != nullptr))
        it->second->update();
}
