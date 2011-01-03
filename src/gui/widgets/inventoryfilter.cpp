/*
 *  The Mana Client
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The Mana Client.
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

#include "gui/widgets/inventoryfilter.h"

#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/radiobutton.h"

#include "log.h"

InventoryFilter::InventoryFilter(std::string group, int height, int spacing):
        HorizontContainer(height, spacing),
        mGroup(group)
{
}

void InventoryFilter::add(std::string tag)
{
    if (tag.empty())
        return;

    RadioButton *radio = new RadioButton(tag, mGroup, mCount == 0);
    radio->adjustSize();
    radio->setActionEventId("tag_" + tag);
    radio->addActionListener(this);
    HorizontContainer::add(radio);
}

void InventoryFilter::action(const gcn::ActionEvent &event)
{
    ActionListenerIterator iter;
    for (iter = mActionListeners.begin();
         iter != mActionListeners.end(); ++iter)
    {
        (*iter)->action(event);
    }
}