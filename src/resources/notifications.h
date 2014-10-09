/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef RESOURCES_NOTIFICATIONS_H
#define RESOURCES_NOTIFICATIONS_H

#include "localconsts.h"

#include "resources/notificationinfo.h"
#include "resources/notifyflags.h"

#include "utils/gettext.h"

namespace NotifyManager
{
    static const NotificationInfo notifications[] =
    {
        {"",
            "", NotifyFlags::EMPTY},
        {"buy done",
            // TRANSLATORS: notification message
            N_("Thanks for buying."),
            NotifyFlags::EMPTY},
        {"buy fail",
            // TRANSLATORS: notification message
            N_("Unable to buy."),
            NotifyFlags::EMPTY},
        {"buy fail no money",
            // TRANSLATORS: notification message
            N_("Unable to buy. You not have enought money."),
            NotifyFlags::EMPTY},
        {"buy fail overweight",
            // TRANSLATORS: notification message
            N_("Unable to buy. You overweight."),
            NotifyFlags::EMPTY},
        {"buy fail too many items",
            // TRANSLATORS: notification message
            N_("Unable to buy. You have too many items."),
            NotifyFlags::EMPTY},
        {"sell empty",
            // TRANSLATORS: notification message
            N_("Nothing to sell."),
            NotifyFlags::EMPTY},
        {"sell done",
            // TRANSLATORS: notification message
            N_("Thanks for selling."),
            NotifyFlags::EMPTY},
        {"sell fail",
            // TRANSLATORS: notification message
            N_("Unable to sell."),
            NotifyFlags::EMPTY},
        {"sell trade fail",
            // TRANSLATORS: notification message
            N_("Unable to sell while trading."),
            NotifyFlags::EMPTY},
        {"sell unsellable fail",
            // TRANSLATORS: notification message
            N_("Unable to sell unsellable item."),
            NotifyFlags::EMPTY},
        {"online users",
            // TRANSLATORS: notification message
            N_("Online users: %d"),
            NotifyFlags::INT},
        {"guild created",
            // TRANSLATORS: notification message
            N_("Guild created."),
            NotifyFlags::EMPTY},
        {"guild already",
            // TRANSLATORS: notification message
            N_("You are already in guild."),
            NotifyFlags::EMPTY},
        {"guild create fail",
            // TRANSLATORS: notification message
            N_("Emperium check failed."),
            NotifyFlags::EMPTY},
        {"guild error",
            // TRANSLATORS: notification message
            N_("Unknown server response."),
            NotifyFlags::EMPTY},
        {"guild left",
            // TRANSLATORS: notification message
            N_("You have left the guild."),
            NotifyFlags::EMPTY},
        {"guild invite fail",
            // TRANSLATORS: notification message
            N_("Could not invite user to guild."),
            NotifyFlags::GUILD},
        {"guild invite rejected",
            // TRANSLATORS: notification message
            N_("User rejected guild invite."),
            NotifyFlags::GUILD},
        {"guild invite joined",
            // TRANSLATORS: notification message
            N_("User is now part of your guild."),
            NotifyFlags::GUILD},
        {"guild invite full",
            // TRANSLATORS: notification message
            N_("Your guild is full."),
            NotifyFlags::GUILD},
        {"guild invite error",
            // TRANSLATORS: notification message
            N_("Unknown guild invite response."),
            NotifyFlags::GUILD},
        {"guild user left",
            // TRANSLATORS: notification message
            N_("%s has left your guild."),
            NotifyFlags::GUILD_STRING},
        {"guild kicked",
            // TRANSLATORS: notification message
            N_("You were kicked from guild."),
            NotifyFlags::EMPTY},
        {"guild user kicked",
            // TRANSLATORS: notification message
            N_("%s has kicked from your guild."),
            NotifyFlags::GUILD_STRING},
        {"use failed",
            // TRANSLATORS: notification message
            N_("Failed to use item."),
            NotifyFlags::EMPTY},
        {"equip failed",
            // TRANSLATORS: notification message
            N_("Unable to equip."),
            NotifyFlags::EMPTY},
        {"equip failed level",
            // TRANSLATORS: notification message
            N_("Unable to equip because you have wrong level."),
            NotifyFlags::EMPTY},
        {"party create failed",
            // TRANSLATORS: notification message
            N_("Could not create party."),
            NotifyFlags::EMPTY},
        {"party created",
            // TRANSLATORS: notification message
            N_("Party successfully created."),
            NotifyFlags::EMPTY},
        {"party left",
            // TRANSLATORS: notification message
            N_("You have left the party."),
            NotifyFlags::EMPTY},
        {"party left deny",
            // TRANSLATORS: notification message
            N_("You cant left party on this map."),
            NotifyFlags::EMPTY},
        {"party kicked",
            // TRANSLATORS: notification message
            N_("You was kicked from party."),
            NotifyFlags::EMPTY},
        {"party kick deny",
            // TRANSLATORS: notification message
            N_("You cant be kicked kicked from party on this map."),
            NotifyFlags::EMPTY},
        {"party user joined",
            // TRANSLATORS: notification message
            N_("%s has joined your party."),
            NotifyFlags::PARTY_STRING},
        {"party invite already member",
            // TRANSLATORS: notification message
            N_("%s is already a member of a party."),
            NotifyFlags::PARTY_STRING},
        {"party invite refused",
            // TRANSLATORS: notification message
            N_("%s refused your invitation."),
            NotifyFlags::PARTY_STRING},
        {"party invite done",
            // TRANSLATORS: notification message
            N_("%s is now a member of your party."),
            NotifyFlags::PARTY_STRING},
        {"party invite full",
            // TRANSLATORS: notification message
            N_("%s can't join your party because party is full."),
            NotifyFlags::PARTY_STRING},
        {"party invite error",
            // TRANSLATORS: notification message
            N_("QQQ Unknown invite response for %s."),
            NotifyFlags::PARTY_STRING},
        {"party exp sharing on",
            // TRANSLATORS: notification message
            N_("Experience sharing enabled."),
            NotifyFlags::PARTY},
        {"party exp sharing off",
            // TRANSLATORS: notification message
            N_("Experience sharing disabled."),
            NotifyFlags::PARTY},
        {"party exp sharing error",
            // TRANSLATORS: notification message
            N_("Experience sharing not possible."),
            NotifyFlags::PARTY},
        {"party item sharing on",
            // TRANSLATORS: notification message
            N_("Item sharing enabled."),
            NotifyFlags::PARTY},
        {"party item sharing off",
            // TRANSLATORS: notification message
            N_("Item sharing disabled."),
            NotifyFlags::PARTY},
        {"party item sharing error",
            // TRANSLATORS: notification message
            N_("Item sharing not possible."),
            NotifyFlags::PARTY},
        {"party user left",
            // TRANSLATORS: notification message
            N_("%s has left your party."),
            NotifyFlags::PARTY_STRING},
        {"party user left deny",
            // TRANSLATORS: notification message
            N_("%s cant be kicked from party on this map."),
            NotifyFlags::PARTY_STRING},
        {"party user kicked",
            // TRANSLATORS: notification message
            N_("%s has kicked from your party."),
            NotifyFlags::PARTY_STRING},
        {"party user kick deny",
            // TRANSLATORS: notification message
            N_("%s cant be kicked from party on this map."),
            NotifyFlags::PARTY_STRING},
        {"party unknown user msg",
            // TRANSLATORS: notification message
            N_("An unknown member tried to say: %s"),
            NotifyFlags::PARTY_STRING},
        {"party user not in party",
            // TRANSLATORS: notification message
            N_("%s is not in your party!"),
            NotifyFlags::PARTY_STRING},
        {"money get",
            // TRANSLATORS: notification message
            N_("You picked up %s."),
            NotifyFlags::STRING},
        {"money spend",
            // TRANSLATORS: notification message
            N_("You spent %s."),
            NotifyFlags::STRING},
        {"skill raise error",
            // TRANSLATORS: notification message
            N_("Cannot raise skill!"),
            NotifyFlags::EMPTY},
        {"arrow equip needed",
            // TRANSLATORS: notification message
            N_("Equip ammunition first."),
            NotifyFlags::EMPTY},
        {"trade fail far away",
            // TRANSLATORS: notification message
            N_("Trading with %s isn't possible. Trade partner is "
            "too far away."),
            NotifyFlags::STRING},
        {"trade fail chat not exists",
            // TRANSLATORS: notification message
            N_("Trading with %s isn't possible. Character doesn't exist."),
            NotifyFlags::STRING},
        {"trade cancelled error",
            // TRANSLATORS: notification message
            N_("Trade cancelled due to an unknown reason."),
            NotifyFlags::EMPTY},
        {"trade cancelled user",
            // TRANSLATORS: notification message
            N_("Trade with %s cancelled."),
            NotifyFlags::STRING},
        {"trade error unknown",
            // TRANSLATORS: notification message
            N_("Unhandled trade cancel packet with %s"),
            NotifyFlags::STRING},
        {"trade add partner over weighted",
            // TRANSLATORS: notification message
            N_("Failed adding item. Trade partner is over weighted."),
            NotifyFlags::EMPTY},
        {"trade add partned has no free slots",
            // TRANSLATORS: notification message
            N_("Failed adding item. Trade partner has no free slot."),
            NotifyFlags::EMPTY},
        {"trade add untradable item",
            // TRANSLATORS: notification message
            N_("Failed adding item. You can't trade this item."),
            NotifyFlags::EMPTY},
        {"trade add error",
            // TRANSLATORS: notification message
            N_("Failed adding item for unknown reason."),
            NotifyFlags::EMPTY},
        {"trade cancelled",
            // TRANSLATORS: notification message
            N_("Trade canceled."),
            NotifyFlags::EMPTY},
        {"trade complete",
            // TRANSLATORS: notification message
            N_("Trade completed."),
            NotifyFlags::EMPTY},
        {"kick fail",
            // TRANSLATORS: notification message
            N_("Kick failed!"),
            NotifyFlags::EMPTY},
        {"kick succeed",
            // TRANSLATORS: notification message
            N_("Kick succeeded!"),
            NotifyFlags::EMPTY},
        {"mvp player",
            // TRANSLATORS: notification message
            N_("MVP player: %s"),
            NotifyFlags::STRING},
        {"whispers ignored",
            // TRANSLATORS: notification message
            N_("All whispers ignored."),
            NotifyFlags::EMPTY},
        {"whispers ignore failed",
            // TRANSLATORS: notification message
            N_("All whispers ignore failed."),
            NotifyFlags::EMPTY},
        {"whispers unignored",
            // TRANSLATORS: notification message
            N_("All whispers unignored."),
            NotifyFlags::EMPTY},
        {"whispers unignore failed",
            // TRANSLATORS: notification message
            N_("All whispers unignore failed."),
            NotifyFlags::EMPTY},
        {"skill fail message",
            "%s",
            NotifyFlags::STRING},
        {"pvp off gvg off",
            // TRANSLATORS: notification message
            N_("pvp off, gvg off"),
            NotifyFlags::SPEECH},
        {"pvp on",
            // TRANSLATORS: notification message
            N_("pvp on"),
            NotifyFlags::SPEECH},
        {"gvg on",
            // TRANSLATORS: notification message
            N_("gvg on"),
            NotifyFlags::SPEECH},
        {"pvp on gvg on",
            // TRANSLATORS: notification message
            N_("pvp on, gvg on"),
            NotifyFlags::SPEECH},
        {"unknown pvp",
            // TRANSLATORS: notification message
            N_("unknown pvp"),
            NotifyFlags::SPEECH},
        {"party user char from account in party",
            // TRANSLATORS: notification message
            N_("Char from account %s is already in your party!"),
            NotifyFlags::PARTY_STRING},
        {"party user blocked invite",
            // TRANSLATORS: notification message
            N_("%s blocked invite!"),
            NotifyFlags::PARTY_STRING},
        {"party user not online",
            // TRANSLATORS: notification message
            N_("Char is not online!"),
            NotifyFlags::PARTY_STRING},
        {"pet catch failed",
            // TRANSLATORS: notification message
            N_("Pet catch failed."),
            NotifyFlags::EMPTY},
        {"pet catch success",
            // TRANSLATORS: notification message
            N_("Pet catched."),
            NotifyFlags::EMPTY},
        {"pet catch unknown error",
            // TRANSLATORS: notification message
            N_("Pet catch unkown error: %d."),
            NotifyFlags::INT},
        {"mercenary expired",
            // TRANSLATORS: notification message
            N_("Your mercenary duty hour is over."),
            NotifyFlags::EMPTY},
        {"mercenary killed",
            // TRANSLATORS: notification message
            N_("Your mercenary was killed."),
            NotifyFlags::EMPTY},
        {"mercenary fired",
            // TRANSLATORS: notification message
            N_("Your mercenary was fired."),
            NotifyFlags::EMPTY},
        {"mercenary run",
            // TRANSLATORS: notification message
            N_("Your mercenary run away."),
            NotifyFlags::EMPTY},
        {"mercenary unknown",
            // TRANSLATORS: notification message
            N_("Mercenary unknown state."),
            NotifyFlags::EMPTY},
        {"homunculus feed ok",
            // TRANSLATORS: notification message
            N_("You feed your homunculus."),
            NotifyFlags::EMPTY},
        {"homunculus feed failed",
            // TRANSLATORS: notification message
            N_("You cant feed homunculus, because not have %s."),
            NotifyFlags::STRING},
        {"card insert failed",
            // TRANSLATORS: notification message
            N_("Card insert failed."),
            NotifyFlags::EMPTY},
        {"card insert success",
            // TRANSLATORS: notification message
            N_("Card inserted."),
            NotifyFlags::EMPTY}
    };
}  // namespace NotifyManager
#endif  // RESOURCES_NOTIFICATIONS_H
