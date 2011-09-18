/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "gui/beingpopup.h"

#include "being.h"
#include "graphics.h"
#include "playerrelations.h"
#include "units.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"

#include "gui/widgets/label.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

#include "debug.h"

BeingPopup::BeingPopup() :
    Popup("BeingPopup", "beingpopup.xml")
{
    // Being Name
    mBeingName = new Label("A");
    mBeingName->setFont(boldFont);
    mBeingName->setPosition(getPadding(), getPadding());

    const int fontHeight = mBeingName->getHeight() + getPadding();

    // Being's party
    mBeingParty = new Label("A");
    mBeingParty->setPosition(getPadding(), fontHeight);

    // Being's party
    mBeingGuild = new Label("A");
    mBeingGuild->setPosition(getPadding(), 2 * fontHeight);

    mBeingRank = new Label("A");
    mBeingRank->setPosition(getPadding(), 3 * fontHeight);

    mBeingComment = new Label("A");
    mBeingComment->setPosition(getPadding(), 4 * fontHeight);

    add(mBeingName);
    add(mBeingParty);
    add(mBeingGuild);
    add(mBeingRank);
    add(mBeingComment);
}

BeingPopup::~BeingPopup()
{
}

void BeingPopup::show(int x, int y, Being *b)
{
    if (!b)
    {
        setVisible(false);
        return;
    }

    Label *label1 = mBeingParty;
    Label *label2 = mBeingGuild;
    Label *label3 = mBeingRank;
    Label *label4 = mBeingComment;

    b->updateComment();

    if (b->getType() == Being::NPC && b->getComment().empty())
    {
        setVisible(false);
        return;
    }

    mBeingName->setCaption(b->getName() + b->getGenderSignWithSpace());
    if (gui)
    {
        if (player_relations.isGoodName(b))
            mBeingName->setFont(boldFont);
        else
            mBeingName->setFont(gui->getSecureFont());
    }
    if (b->isAdvanced())
    {
        mBeingName->setForegroundColor(Theme::getThemeColor(
            Theme::PLAYER_ADVANCED));
    }
    else
    {
        mBeingName->setForegroundColor(Theme::getThemeColor(Theme::TEXT));
    }

    mBeingName->adjustSize();
    label1->setCaption("");
    label2->setCaption("");
    label3->setCaption("");
    label4->setCaption("");

    if (!(b->getPartyName().empty()))
    {
        label1->setCaption(strprintf(_("Party: %s"),
                           b->getPartyName().c_str()));
        label1->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = label2;
        label2 = label1;
        label1 = 0;
    }

    if (!(b->getGuildName().empty()))
    {
        label2->setCaption(strprintf(_("Guild: %s"),
                           b->getGuildName().c_str()));
        label2->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = label2;
        label2 = 0;
    }

    if (b->getPvpRank() > 0)
    {
        label3->setCaption(strprintf(_("Pvp rank: %d"), b->getPvpRank()));
        label3->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = 0;
    }

    if (!b->getComment().empty())
    {
        label4->setCaption(strprintf(_("Comment: %s"),
            b->getComment().c_str()));
        label4->adjustSize();
    }
    else
    {
        label4 = 0;
    }

    int minWidth = mBeingName->getWidth();
    if (label1 && label1->getWidth() > minWidth)
        minWidth = label1->getWidth();
    if (label2 && label2->getWidth() > minWidth)
        minWidth = label2->getWidth();
    if (label3 && label3->getWidth() > minWidth)
        minWidth = label3->getWidth();
    if (label4 && label4->getWidth() > minWidth)
        minWidth = label4->getWidth();

    int height = getFont()->getHeight();
    if (label1)
        height += getFont()->getHeight();
    if (label2)
        height += getFont()->getHeight();
    if (label3)
        height += getFont()->getHeight();
    if (label4)
        height += getFont()->getHeight();

    setContentSize(minWidth + 10, height + 10);

    position(x, y);
    return;
}
