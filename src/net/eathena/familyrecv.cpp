/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/familyrecv.h"

#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "const/sound.h"

#include "enums/resources/notifytypes.h"

#include "gui/widgets/createwidget.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "listeners/requestadoptchildlistener.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

namespace FamilyRecv
{
    ConfirmDialog *confirmDlg = nullptr;
    RequestAdoptChildListener listener;
    BeingId mParent1 = BeingId_zero;
    BeingId mParent2 = BeingId_zero;
}  // namespace FamilyRecv

void FamilyRecv::processAskForChild(Net::MessageIn &msg)
{
    if (localPlayer == nullptr)
    {
        mParent1 = msg.readBeingId("account id who ask");
        mParent2 = msg.readBeingId("acoount id for other parent");
        msg.readString(24, "name who ask");
        return;
    }
    mParent1 = msg.readBeingId("account id who ask");
    mParent2 = msg.readBeingId("acoount id for other parent");
    const std::string name1 = msg.readString(24, "name who ask");
    const Party *const party = localPlayer->getParty();
    if (party != nullptr)
    {
        const PartyMember *const member = party->getMember(mParent2);
        if (member != nullptr)
        {
            const std::string name2 = member->getName();
            CREATEWIDGETV(confirmDlg, ConfirmDialog,
                // TRANSLATORS: adopt child message
                _("Request parents"),
                // TRANSLATORS: adopt child message
                strprintf(_("Do you accept %s and %s as parents?"),
                name1.c_str(), name2.c_str()),
                SOUND_REQUEST,
                false,
                Modal_false,
                nullptr);
            confirmDlg->addActionListener(&listener);
        }
    }
}

void FamilyRecv::processCallPartner(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    if ((localPlayer != nullptr) && name == localPlayer->getName())
    {
        NotifyManager::notify(NotifyTypes::CALLED_PARTNER);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::CALLING_PARTNER, name.c_str());
    }
}

void FamilyRecv::processDivorced(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    NotifyManager::notify(NotifyTypes::DIVORCED, name.c_str());
}

void FamilyRecv::processAskForChildReply(Net::MessageIn &msg)
{
    const int type = msg.readInt32("type");
    switch (type)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ADOPT_CHILD_ERROR_HAVE_BABY);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::ADOPT_CHILD_ERROR_LEVEL);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::ADOPT_CHILD_ERROR_BABY_MARRIED);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
}

}  // namespace EAthena
