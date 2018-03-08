/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/setupbuttonitem.h"

#include "gui/widgets/button.h"
#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/vertcontainer.h"

#include "gui/widgets/tabs/setuptabscroll.h"

#include "debug.h"

SetupButtonItem::SetupButtonItem(const std::string &restrict text,
                                 const std::string &restrict description,
                                 const std::string &restrict actionEventId,
                                 SetupTabScroll *restrict const parent,
                                 const std::string &restrict eventName,
                                 ActionListener *const listener) :
    SetupItem(text, description, "", parent, eventName, MainConfig_false),
    mHorizont(nullptr),
    mButton(nullptr)
{
    mValueType = VSTR;
    mWidget = new Button(this,
        text,
        actionEventId,
        BUTTON_SKIN,
        listener);
    createControls();
}

SetupButtonItem::~SetupButtonItem()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mButton = nullptr;
}

void SetupButtonItem::save()
{
}

void SetupButtonItem::cancel(const std::string &eventName A_UNUSED)
{
}

void SetupButtonItem::externalUpdated(const std::string &eventName A_UNUSED)
{
}

void SetupButtonItem::rereadValue()
{
}

void SetupButtonItem::createControls()
{
    mHorizont = new HorizontContainer(this, 32, 2);

    // TRANSLATORS: setup item button
    mHorizont->add(mWidget);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
//    mWidget->addActionListener(this);
}

void SetupButtonItem::fromWidget()
{
}

void SetupButtonItem::toWidget()
{
}

void SetupButtonItem::action(const ActionEvent &event A_UNUSED)
{
}

void SetupButtonItem::apply(const std::string &eventName A_UNUSED)
{
}
