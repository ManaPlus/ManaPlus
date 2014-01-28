/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/characterdisplay.h"

#include "units.h"

#include "gui/windows/charselectdialog.h"

#include "gui/popups/textpopup.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "utils/gettext.h"

#include <SDL_mouse.h>

#include "debug.h"

CharacterDisplay::CharacterDisplay(const Widget2 *const widget,
                                   CharSelectDialog *const charSelectDialog) :
    Container(widget),
    gcn::MouseListener(),
    gcn::WidgetListener(),
    mCharacter(nullptr),
    mPlayerBox(new PlayerBox(nullptr)),
    mName(new Label(this, "wwwwwwwwwwwwwwwwwwwwwwww")),
    mPopup(new TextPopup)
{
    mPopup->postInit();

    mPlayerBox->setActionEventId("select");
    mPlayerBox->addActionListener(charSelectDialog);
    setActionEventId("use");
    addActionListener(charSelectDialog);

    LayoutHelper h(this);
    ContainerPlacer placer = h.getPlacer(0, 0);

    placer(0, 0, mPlayerBox, 3, 5);
    placer(0, 5, mName, 3);

    update();

    mName->setAlignment(Graphics::CENTER);
    mName->adjustSize();

    if (mainGraphics->getWidth() > 800)
        setWidth(120);
    else
        setWidth(80);
    setHeight(120);
    addMouseListener(this);
    addWidgetListener(this);
}

CharacterDisplay::~CharacterDisplay()
{
    delete mPopup;
    mPopup = nullptr;
}

void CharacterDisplay::setCharacter(Net::Character *const character)
{
    if (mCharacter == character)
        return;

    mCharacter = character;
    mPlayerBox->setPlayer(character ? character->dummy : nullptr);
    update();
}

void CharacterDisplay::requestFocus()
{
}

void CharacterDisplay::setActive(const bool active A_UNUSED)
{
}

void CharacterDisplay::update()
{
    if (mCharacter)
        mName->setCaption(mCharacter->dummy->getName());
    else
        mName->setCaption("");
    const int width = mPlayerBox->getWidth();
    mName->resizeTo(width, width);

    distributeResizedEvent();
}

void CharacterDisplay::widgetHidden(const gcn::Event &event A_UNUSED)
{
    mPopup->setVisible(false);
}

void CharacterDisplay::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    mPopup->setVisible(false);
}

void CharacterDisplay::mouseMoved(gcn::MouseEvent &event A_UNUSED)
{
    if (!gui)
        return;

    int mouseX = 0;
    int mouseY = 0;
    gui->getMouseState(&mouseX, &mouseY);
    const std::string &name = mName->getCaption();
    if (!name.empty())
    {
        mPopup->show(mouseX, mouseY, name, strprintf(_("Level: %u"),
            mCharacter->data.mAttributes[PlayerInfo::LEVEL]),
            strprintf(_("Money: %s"), Units::formatCurrency(
            mCharacter->data.mAttributes[PlayerInfo::MONEY]).c_str()));
    }
    else
    {
        mPopup->setVisible(false);
    }
}

void CharacterDisplay::mousePressed(gcn::MouseEvent &event)
{
    if (event.getClickCount() == 2)
        distributeActionEvent();
}
