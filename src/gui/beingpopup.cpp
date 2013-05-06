/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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
#include "playerrelations.h"
#include "units.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/label.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

BeingPopup::BeingPopup() :
    Popup("BeingPopup", "beingpopup.xml"),
    mBeingName(new Label(this, "A")),
    mBeingParty(new Label(this, "A")),
    mBeingGuild(new Label(this, "A")),
    mBeingRank(new Label(this, "A")),
    mBeingComment(new Label(this, "A"))
{
    // Being Name
    mBeingName->setFont(boldFont);
    mBeingName->setPosition(0, 0);

    const int fontHeight = mBeingName->getHeight();

    // Being's party
    mBeingParty->setPosition(0, fontHeight);
    // Being's party
    mBeingGuild->setPosition(0, 2 * fontHeight);
    mBeingRank->setPosition(0, 3 * fontHeight);
    mBeingComment->setPosition(0, 4 * fontHeight);

    mBeingParty->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));
    mBeingGuild->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));
    mBeingRank->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));
    mBeingComment->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));

    add(mBeingName);
    add(mBeingParty);
    add(mBeingGuild);
    add(mBeingRank);
    add(mBeingComment);
}

BeingPopup::~BeingPopup()
{
}

void BeingPopup::show(const int x, const int y, Being *const b)
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
        mBeingName->setForegroundColor(getThemeColor(
            Theme::PLAYER_ADVANCED));
    }
    else
    {
        mBeingName->setForegroundColor(getThemeColor(Theme::POPUP));
    }

    mBeingName->adjustSize();
    label1->setCaption("");
    label2->setCaption("");
    label3->setCaption("");
    label4->setCaption("");

    if (!(b->getPartyName().empty()))
    {
        // TRANSLATORS: being popup label
        label1->setCaption(strprintf(_("Party: %s"),
                           b->getPartyName().c_str()));
        label1->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = label2;
        label2 = label1;
        label1 = nullptr;
    }

    if (!(b->getGuildName().empty()))
    {
        // TRANSLATORS: being popup label
        label2->setCaption(strprintf(_("Guild: %s"),
                           b->getGuildName().c_str()));
        label2->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = label2;
        label2 = nullptr;
    }

    if (b->getPvpRank() > 0)
    {
        // TRANSLATORS: being popup label
        label3->setCaption(strprintf(_("Pvp rank: %u"), b->getPvpRank()));
        label3->adjustSize();
    }
    else
    {
        label4 = label3;
        label3 = nullptr;
    }

    if (!b->getComment().empty())
    {
        // TRANSLATORS: being popup label
        label4->setCaption(strprintf(_("Comment: %s"),
            b->getComment().c_str()));
        label4->adjustSize();
    }
    else
    {
        label4 = nullptr;
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

    const int height1 = getFont()->getHeight();
    int height = height1;
    if (label1)
        height += height1;
    if (label2)
        height += height1;
    if (label3)
        height += height1;
    if (label4)
        height += height1;

    setContentSize(minWidth, height);
    position(x, y);
    return;
}
