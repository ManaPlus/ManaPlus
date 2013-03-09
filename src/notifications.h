/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef NOTIFYCATIONS_H
#define NOTIFYCATIONS_H

#include "utils/gettext.h"

namespace NotifyManager
{
    enum NotifyTypes
    {
        BUY_DONE,
        BUY_FAILED,
        SELL_LIST_EMPTY,
        SOLD,
        SELL_FAILED,
        SELL_TRADE_FAILED,
        SELL_UNSELLABLE_FAILED,
        ONLINE_USERS,
        GUILD_CREATED,
        GUILD_ALREADY,
        GUILD_EMPERIUM_CHECK_FAILED,
        GUILD_ERROR,
        GUILD_LEFT,
        GUILD_INVITE_FAILED,
        GUILD_INVITE_REJECTED,
        GUILD_INVITE_JOINED,
        GUILD_INVITE_FULL,
        GUILD_INVITE_ERROR,
        GUILD_USER_LEFT,
        GUILD_KICKED,
        GUILD_USER_KICKED,
        USE_FAILED,
        EQUIP_FAILED,
        PARTY_CREATE_FAILED,
        PARTY_CREATED,
        PARTY_LEFT,
        PARTY_USER_JOINED,
        PARTY_INVITE_ALREADY_MEMBER,
        PARTY_INVITE_REFUSED,
        PARTY_INVITE_DONE,
        PARTY_INVITE_PARTY_FULL,
        PARTY_INVITE_ERROR,
        PARTY_EXP_SHARE_ON,
        PARTY_EXP_SHARE_OFF,
        PARTY_EXP_SHARE_ERROR,
        PARTY_ITEM_SHARE_ON,
        PARTY_ITEM_SHARE_OFF,
        PARTY_ITEM_SHARE_ERROR,
        PARTY_USER_LEFT,
        PARTY_UNKNOWN_USER_MSG,
        PARTY_USER_NOT_IN_PARTY,
        MONEY_GET,
        MONEY_SPENT,
        SKILL_RAISE_ERROR,
        ARROWS_EQUIP_NEEDED,

        TYPE_END
    };

    enum NotifyFlags
    {
        EMPTY,
        INT,
        STRING,
        GUILD,
        GUILD_STRING,
        PARTY,
        PARTY_STRING
    };

    struct NotificationInfo
    {
        const char *text;
        const NotifyFlags flags;
    };

    static const NotificationInfo notifications[] =
    {
        {N_("Thanks for buying."), EMPTY},
        {N_("Unable to buy."), EMPTY},
        {N_("Nothing to sell."), EMPTY},
        {N_("Thanks for selling."), EMPTY},
        {N_("Unable to sell."), EMPTY},
        {N_("Unable to sell while trading."), EMPTY},
        {N_("Unable to sell unsellable item."), EMPTY},
        {N_("Online users: %d"), INT},
        {N_("Guild created."), EMPTY},
        {N_("You are already in guild."), EMPTY},
        {N_("Emperium check failed."), EMPTY},
        {N_("Unknown server response."), EMPTY},
        {N_("You have left the guild."), EMPTY},
        {N_("Could not invite user to guild."), GUILD},
        {N_("User rejected guild invite."), GUILD},
        {N_("User is now part of your guild."), GUILD},
        {N_("Your guild is full."), GUILD},
        {N_("Unknown guild invite response."), GUILD},
        {N_("%s has left your guild."), GUILD_STRING},
        {N_("You were kicked from guild."), EMPTY},
        {N_("%s has kicked from your guild."), GUILD_STRING},
        {N_("Failed to use item."), EMPTY},
        {N_("Unable to equip."), EMPTY},
        {N_("Could not create party."), EMPTY},
        {N_("Party successfully created."), EMPTY},
        {N_("You have left the party."), EMPTY},
        {N_("%s has joined your party."), PARTY_STRING},
        {N_("%s is already a member of a party."), PARTY_STRING},
        {N_("%s refused your invitation."), PARTY_STRING},
        {N_("%s is now a member of your party."), PARTY_STRING},
        {N_("%s can't join your party because party is full."), PARTY_STRING},
        {N_("QQQ Unknown invite response for %s."), PARTY_STRING},
        {N_("Experience sharing enabled."), PARTY},
        {N_("Experience sharing disabled."), PARTY},
        {N_("Experience sharing not possible."), PARTY},
        {N_("Item sharing enabled."), PARTY},
        {N_("Item sharing disabled."), PARTY},
        {N_("Item sharing not possible."), PARTY},
        {N_("%s has left your party."), PARTY_STRING},
        {N_("An unknown member tried to say: %s"), PARTY_STRING},
        {N_("%s is not in your party!"), PARTY_STRING},
        {N_("You picked up %s."), STRING},
        {N_("You spent %s."), STRING},
        {N_("Cannot raise skill!"), EMPTY},
        {N_("Equip arrows first."), EMPTY},
    };
}
#endif
