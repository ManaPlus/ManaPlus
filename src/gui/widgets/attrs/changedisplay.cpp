/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/attrs/changedisplay.h"

#include "configuration.h"

#include "being/playerinfo.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/gettext.h"

#include "debug.h"

ChangeDisplay::ChangeDisplay(const Widget2 *const widget,
                             const AttributesT id,
                             const std::string &restrict name,
                             const std::string &restrict shortName) :
    AttrDisplay(widget, id, name, shortName),
    ActionListener(),
    mNeeded(1),
    // TRANSLATORS: status window label
    mPoints(new Label(this, _("Max"))),
    // TRANSLATORS: status window label (plus sign)
    mInc(new Button(this, _("+"), "inc", BUTTON_SKIN, this))
{
    // Do the layout
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3, 1);
    place(4, 0, mValue, 2, 1);
    place(6, 0, mInc, 1, 1);
    place(7, 0, mPoints, 1, 1);
}

std::string ChangeDisplay::update()
{
    if (mNeeded > 0)
    {
        mPoints->setCaption(toString(mNeeded));
    }
    else
    {
        // TRANSLATORS: status bar label
        mPoints->setCaption(_("Max"));
    }

    mInc->setEnabled(PlayerInfo::getAttribute(Attributes::PLAYER_CHAR_POINTS)
        >= mNeeded && mNeeded > 0);

    return AttrDisplay::update();
}

void ChangeDisplay::setPointsNeeded(const int needed)
{
    mNeeded = needed;
    update();
}

void ChangeDisplay::action(const ActionEvent &event)
{
    if (event.getSource() == mInc)
    {
        int cnt = 1;
        if (config.getBoolValue("quickStats"))
        {
            cnt = mInc->getClickCount();
            if (cnt > 10)
                cnt = 10;
        }

        const int newpoints = PlayerInfo::getAttribute(
            Attributes::PLAYER_CHAR_POINTS) - cnt;
        PlayerInfo::setAttribute(Attributes::PLAYER_CHAR_POINTS,
            newpoints,
            Notify_true);

        const int newbase = PlayerInfo::getStatBase(mId) + cnt;
        PlayerInfo::setStatBase(mId, newbase, Notify_true);

        if (Net::getNetworkType() != ServerType::TMWATHENA)
        {
            playerHandler->increaseAttribute(mId, cnt);
        }
        else
        {
            for (int f = 0; f < cnt; f ++)
            {
                playerHandler->increaseAttribute(mId, 1);
                if (cnt != 1)
                    SDL_Delay(100);
            }
        }
    }
}
