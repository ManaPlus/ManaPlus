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

#include "gui/widgets/setupquickitem.h"

#include "gui/widgets/button.h"
#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/vertcontainer.h"

#include "gui/widgets/tabs/setuptabscroll.h"

#include "debug.h"

SetupQuickItem::SetupQuickItem(const std::string &restrict description,
                               SetupTabScroll *restrict const parent,
                               const std::string &restrict eventName,
                               const ModifierGetFuncPtr getFunc,
                               const ModifierChangeFuncPtr changeFunc) :
    SetupItem("", description, "", parent, eventName, MainConfig_false),
    GameModifierListener(),
    mHorizont(nullptr),
    mButton(nullptr),
    mLabel(nullptr),
    mGetFunc(getFunc),
    mChangeFunc(changeFunc)
{
    mValueType = VSTR;
    createControls();
}

SetupQuickItem::~SetupQuickItem()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mButton = nullptr;
}

void SetupQuickItem::save()
{
}

void SetupQuickItem::cancel(const std::string &eventName A_UNUSED)
{
}

void SetupQuickItem::externalUpdated(const std::string &eventName A_UNUSED)
{
}

void SetupQuickItem::rereadValue()
{
}

void SetupQuickItem::createControls()
{
    mHorizont = new HorizontContainer(this, 32, 2);

    mWidget = new Button(this, ">", mEventName + "_CHANGE", nullptr);
    mLabel = new Label(this, std::string());
    mLabel->setToolTip(mDescription);
    mLabel->adjustSize();

    mHorizont->add(mWidget, 5);
    mHorizont->add(mLabel);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mWidget->addActionListener(this);
    toWidget();
}

void SetupQuickItem::fromWidget()
{
}

void SetupQuickItem::toWidget()
{
    mLabel->setCaption(mGetFunc());
    mLabel->adjustSize();
}

void SetupQuickItem::action(const ActionEvent &event)
{
    if (event.getId() == mEventName + "_CHANGE")
    {
        // need change game modifier
        mChangeFunc(true);
        toWidget();
    }
}

void SetupQuickItem::apply(const std::string &eventName A_UNUSED)
{
}

void SetupQuickItem::gameModifiersChanged()
{
    toWidget();
}
