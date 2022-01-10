/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "enums/resources/notifytypes.h"

#include "resources/notificationinfo.h"

#include "utils/gettext.h"

#include "localconsts.h"

namespace NotifyManager
{
    static const NotificationInfo notifications[NotifyTypes::TYPE_END] =
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
            N_("Unable to buy. You don't have enough money."),
            NotifyFlags::EMPTY},
        {"buy fail overweight",
            // TRANSLATORS: notification message
            N_("Unable to buy. It is too heavy."),
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
            N_("You are already in a guild."),
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
            N_("%s has been kicked from your guild."),
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
        {"unequip failed",
            // TRANSLATORS: notification message
            N_("Unable to unequip."),
            NotifyFlags::EMPTY},
        {"party create failed",
            // TRANSLATORS: notification message
            N_("Couldn't create party."),
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
            N_("You can't leave party on this map."),
            NotifyFlags::EMPTY},
        {"party kicked",
            // TRANSLATORS: notification message
            N_("You were kicked from party."),
            NotifyFlags::EMPTY},
        {"party kick deny",
            // TRANSLATORS: notification message
            N_("You can't be kicked from party on this map."),
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
            N_("%s can't be kicked from party on this map."),
            NotifyFlags::PARTY_STRING},
        {"party user kicked",
            // TRANSLATORS: notification message
            N_("%s has kicked from your party."),
            NotifyFlags::PARTY_STRING},
        {"party user kick deny",
            // TRANSLATORS: notification message
            N_("%s can't be kicked from party on this map."),
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
        {"trade cancelled busy",
            // TRANSLATORS: notification message
            N_("Trade with %s cancelled, because player is busy"),
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
            N_("Pet caught."),
            NotifyFlags::EMPTY},
        {"pet catch unknown error",
            // TRANSLATORS: notification message
            N_("Pet catch unknown error: %d."),
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
            N_("You can't feed homunculus, because you don't have any %s."),
            NotifyFlags::STRING},
        {"card insert failed",
            // TRANSLATORS: notification message
            N_("Card insert failed."),
            NotifyFlags::EMPTY},
        {"card insert success",
            // TRANSLATORS: notification message
            N_("Card inserted."),
            NotifyFlags::EMPTY},
        {"bank check failed",
            // TRANSLATORS: notification message
            N_("Bank check failed. Bank probably disabled."),
            NotifyFlags::EMPTY},
        {"bank deposit failed",
            // TRANSLATORS: notification message
            N_("Deposit failed. You probably don't have this "
               "amount of money with you right now."),
            NotifyFlags::EMPTY},
        {"bank withdraw failed",
            // TRANSLATORS: notification message
            N_("Withdraw failed. You probably don't have this "
                "amount of money in the bank right now."),
            NotifyFlags::EMPTY},
        {"buying store create failed",
            // TRANSLATORS: notification message
            N_("Buying store create failed."),
            NotifyFlags::EMPTY},
        {"buying store create failed weight",
            // TRANSLATORS: notification message
            N_("Buying store create failed. Too many weight."),
            NotifyFlags::INT},
        {"buying store create empty",
            // TRANSLATORS: notification message
            N_("Buying store create failed. No items in store."),
            NotifyFlags::EMPTY},
        {"buying store buy failed money limit",
            // TRANSLATORS: notification message
            N_("All items within the buy limit were purchased."),
            NotifyFlags::EMPTY},
        {"buying store buy failed empty",
            // TRANSLATORS: notification message
            N_("All items were purchased."),
            NotifyFlags::EMPTY},
        {"buying store buy failed",
            // TRANSLATORS: notification message
            N_("Buying item failed."),
            NotifyFlags::EMPTY},
        {"buying store sell failed deal",
            // TRANSLATORS: notification message
            N_("The deal has failed."),
            NotifyFlags::EMPTY},
        {"buying store sell failed amount",
            // TRANSLATORS: notification message
            N_("The trade failed, because the entered amount of item is "
                "higher, than the buyer is willing to buy."),
            NotifyFlags::EMPTY},
        {"buying store sell failed balance",
            // TRANSLATORS: notification message
            N_("The trade failed, because the buyer is "
                "lacking required balance."),
            NotifyFlags::EMPTY},
        {"buying store sell failed",
            // TRANSLATORS: notification message
            N_("Selling item failed."),
            NotifyFlags::EMPTY},

        {"search store failed",
            // TRANSLATORS: notification message
            N_("Items searching failed."),
            NotifyFlags::EMPTY},
        {"search store failed no stores",
            // TRANSLATORS: notification message
            N_("No matching stores were found."),
            NotifyFlags::EMPTY},
        {"search store failed many results",
            // TRANSLATORS: notification message
            N_("There are too many results. Please enter more "
                "detailed search term."),
            NotifyFlags::EMPTY},
        {"search store failed cant search anymore",
            // TRANSLATORS: notification message
            N_("You cannot search anymore."),
            NotifyFlags::EMPTY},
        {"search store failed cant search yet",
            // TRANSLATORS: notification message
            N_("You cannot search yet."),
            NotifyFlags::EMPTY},
        {"search store failed no information",
            // TRANSLATORS: notification message
            N_("No store information available."),
            NotifyFlags::EMPTY},
        {"pet feed ok",
            // TRANSLATORS: notification message
            N_("Pet feeding success."),
            NotifyFlags::EMPTY},
        {"pet feed error",
            // TRANSLATORS: notification message
            N_("Pet feeding error."),
            NotifyFlags::EMPTY},
        {"manner changed",
            // TRANSLATORS: notification message
            N_("A manner point has been successfully aligned."),
            NotifyFlags::EMPTY},
        {"manner positive points",
            // TRANSLATORS: notification message
            N_("You got positive manner points from %s."),
            NotifyFlags::STRING},
        {"manner negative points",
            // TRANSLATORS: notification message
            N_("You got negative manner points from %s."),
            NotifyFlags::STRING},
        {"chat room limit exceed",
            // TRANSLATORS: notification message
            N_("Chat room limit exceeded"),
            NotifyFlags::EMPTY},
        {"chat room already exists",
            // TRANSLATORS: notification message
            N_("Chat room already exists"),
            NotifyFlags::EMPTY},
        {"chat room joined",
            // TRANSLATORS: notification message
            N_("%s joined room."),
            NotifyFlags::STRING},
        {"chat room leave",
            // TRANSLATORS: notification message
            N_("%s left room."),
            NotifyFlags::STRING},
        {"chat room kick",
            // TRANSLATORS: notification message
            N_("%s kicked from room."),
            NotifyFlags::STRING},
        {"chat room role owner",
            // TRANSLATORS: notification message
            N_("%s role changed to room owner."),
            NotifyFlags::STRING},
        {"chat room error full",
            // TRANSLATORS: notification message
            N_("Room join failed. Room full."),
            NotifyFlags::EMPTY},
        {"chat room error password",
            // TRANSLATORS: notification message
            N_("Room join failed. Wrong password."),
            NotifyFlags::EMPTY},
        {"chat room error kicked",
            // TRANSLATORS: notification message
            N_("Room join failed. Kicked from room."),
            NotifyFlags::EMPTY},
        {"chat room error zeny",
            // TRANSLATORS: notification message
            N_("Room join failed. Not enough money."),
            NotifyFlags::EMPTY},
        {"chat room error low level",
            // TRANSLATORS: notification message
            N_("Room join failed. Too low level."),
            NotifyFlags::EMPTY},
        {"chat room error high level",
            // TRANSLATORS: notification message
            N_("Room join failed. Too high level."),
            NotifyFlags::EMPTY},
        {"chat room error race",
            // TRANSLATORS: notification message
            N_("Room join failed. Wrong race."),
            NotifyFlags::EMPTY},
        {"chat room error race",
            // TRANSLATORS: notification message
            N_("Left %d seconds until you can use item."),
            NotifyFlags::INT},
        {"mail send ok",
            // TRANSLATORS: notification message
            N_("Message successfully sent."),
            NotifyFlags::EMPTY},
        {"mail send error",
            // TRANSLATORS: notification message
            N_("Message send failed."),
            NotifyFlags::EMPTY},
        {"mail attach item error",
            // TRANSLATORS: notification message
            N_("Item attach failed."),
            NotifyFlags::EMPTY},
        {"mail attach money error",
            // TRANSLATORS: notification message
            N_("Money attach failed."),
            NotifyFlags::EMPTY},
        {"mail return error",
            // TRANSLATORS: notification message
            N_("Message return failed."),
            NotifyFlags::EMPTY},
        {"mail return ok",
            // TRANSLATORS: notification message
            N_("Message return success."),
            NotifyFlags::EMPTY},
        {"mail delete error",
            // TRANSLATORS: notification message
            N_("Message deletion failed."),
            NotifyFlags::EMPTY},
        {"mail delete ok",
            // TRANSLATORS: notification message
            N_("Message successfully deleted."),
            NotifyFlags::EMPTY},
        {"mail get attach ok",
            // TRANSLATORS: notification message
            N_("You got attach successfully."),
            NotifyFlags::EMPTY},
        {"mail get attach error",
            // TRANSLATORS: notification message
            N_("Error on getting attach successfully."),
            NotifyFlags::EMPTY},
        {"mail get attach too many items",
            // TRANSLATORS: notification message
            N_("Can't get attach. Too many items."),
            NotifyFlags::EMPTY},
        {"new mail",
            "%s",
            NotifyFlags::STRING},
        {"mail type battle field",
            // TRANSLATORS: notification message
            N_("You enter battle field."),
            NotifyFlags::EMPTY},
        {"rental time left",
            "%s",
            NotifyFlags::STRING},
        {"rental time expired",
            // TRANSLATORS: notification message
            N_("Rental time for %s expired"),
            NotifyFlags::STRING},
        {"refine success",
            // TRANSLATORS: notification message
            N_("Refine success for item %s."),
            NotifyFlags::STRING},
        {"refine failure",
            // TRANSLATORS: notification message
            N_("Refine failure for item %s."),
            NotifyFlags::STRING},
        {"refine downgrade",
            // TRANSLATORS: notification message
            N_("Refine failure. Item %s downgraded."),
            NotifyFlags::STRING},
        {"refine unknown",
            // TRANSLATORS: notification message
            N_("Refine unknown for item %s."),
            NotifyFlags::STRING},
        {"cart add weight error",
            // TRANSLATORS: notification message
            N_("You can't add item to card because weight too high."),
            NotifyFlags::EMPTY},
        {"cart add count error",
            // TRANSLATORS: notification message
            N_("You can't add item to card because too many items."),
            NotifyFlags::EMPTY},
        {"bound item",
            // TRANSLATORS: notification message
            N_("Item %s bound to you."),
            NotifyFlags::STRING},
        {"skill end all negative status",
            // TRANSLATORS: notification message
            N_("End all negative status."),
            NotifyFlags::EMPTY},
        {"skill immunity to all status",
            // TRANSLATORS: notification message
            N_("Immunity to all status."),
            NotifyFlags::EMPTY},
        {"skill max hp",
            // TRANSLATORS: notification message
            N_("Max hp +100%."),
            NotifyFlags::EMPTY},
        {"skill max sp",
            // TRANSLATORS: notification message
            N_("Max sp +100%."),
            NotifyFlags::EMPTY},
        {"skill all stats +20",
            // TRANSLATORS: notification message
            N_("All stats +20."),
            NotifyFlags::EMPTY},
        {"skill enchant weapon with holy element",
            // TRANSLATORS: notification message
            N_("Enchant weapon with holy element."),
            NotifyFlags::EMPTY},
        {"skill enchant armor with holy element",
            // TRANSLATORS: notification message
            N_("Enchant armor with holy element."),
            NotifyFlags::EMPTY},
        {"skill def +25%",
            // TRANSLATORS: notification message
            N_("Def +25%."),
            NotifyFlags::EMPTY},
        {"skill atk +100%",
            // TRANSLATORS: notification message
            N_("Atk +100%."),
            NotifyFlags::EMPTY},
        {"skill flee +50",
            // TRANSLATORS: notification message
            N_("Flee +50."),
            NotifyFlags::EMPTY},
        {"skill full strip failed",
            // TRANSLATORS: notification message
            N_("Full strip failed because of coating."),
            NotifyFlags::EMPTY},
        {"skill message unknown",
            // TRANSLATORS: notification message
            N_("Unknown skill message."),
            NotifyFlags::EMPTY},
        {"ignore player success",
            // TRANSLATORS: notification message
            N_("Player successfully ignored."),
            NotifyFlags::EMPTY},
        {"ignore player failure",
            // TRANSLATORS: notification message
            N_("Player ignore failed."),
            NotifyFlags::EMPTY},
        {"ignore player too many",
            // TRANSLATORS: notification message
            N_("Player ignore failed. Because too many ignores."),
            NotifyFlags::EMPTY},
        {"ignore player unknown",
            // TRANSLATORS: notification message
            N_("Unknown player ignore failure."),
            NotifyFlags::EMPTY},
        {"unignore player success",
            // TRANSLATORS: notification message
            N_("Player successfully unignored."),
            NotifyFlags::EMPTY},
        {"unignore player failure",
            // TRANSLATORS: notification message
            N_("Player unignore failed."),
            NotifyFlags::EMPTY},
        {"unignore player unknown",
            // TRANSLATORS: notification message
            N_("Unknown player unignore failure."),
            NotifyFlags::EMPTY},
        {"ignore unknown type",
            // TRANSLATORS: notification message
            N_("Unknown ignore type."),
            NotifyFlags::EMPTY},
        {"pet catch process",
            // TRANSLATORS: notification message
            N_("Pet catch started."),
            NotifyFlags::EMPTY},
        {"delete item normal",
            "",
            NotifyFlags::STRING},
        {"delete item skill use",
            "",
            NotifyFlags::STRING},
        {"delete item fail refine",
            "",
            NotifyFlags::STRING},
        {"delete item material change",
            "",
            NotifyFlags::STRING},
        {"delete item to storage",
            "",
            NotifyFlags::STRING},
        {"delete item to cart",
            "",
            NotifyFlags::STRING},
        {"delete item sold",
            "",
            NotifyFlags::STRING},
        {"delete item analysis",
            "",
            NotifyFlags::STRING},
        {"delete item unknown",
            "",
            NotifyFlags::STRING},
        {"delete item dropped",
            "",
            NotifyFlags::STRING},
        {"being remove died",
            // TRANSLATORS: notification message
            N_("Player %s died."),
            NotifyFlags::STRING},
        {"being remove logged out",
            // TRANSLATORS: notification message
            N_("Player %s logged out."),
            NotifyFlags::STRING},
        {"being remove warped",
            // TRANSLATORS: notification message
            N_("Player %s warped."),
            NotifyFlags::STRING},
        {"being remove trick dead",
            // TRANSLATORS: notification message
            N_("Player %s trick dead."),
            NotifyFlags::STRING},
        {"being remove unknown",
            // TRANSLATORS: notification message
            N_("Player %s unknown remove."),
            NotifyFlags::STRING},
        {"player divorced",
            // TRANSLATORS: notification message
            N_("You and %s are now divorced."),
            NotifyFlags::STRING},
        {"partner called",
            // TRANSLATORS: notification message
            N_("You were called by your partner."),
            NotifyFlags::EMPTY},
        {"partner calling",
            // TRANSLATORS: notification message
            N_("You are calling your partner, %s."),
            NotifyFlags::STRING},
        {"adopt child error have baby",
            // TRANSLATORS: notification message
            N_("Child adoption failed. You already have a baby."),
            NotifyFlags::EMPTY},
        {"adopt child error level",
            // TRANSLATORS: notification message
            N_("Child adoption failed. Your level is too low."),
            NotifyFlags::EMPTY},
        {"adopt child error baby married",
            // TRANSLATORS: notification message
            N_("Child adoption failed. This player is already "
            "married and can't be a baby."),
            NotifyFlags::EMPTY},
        {"skill memo saved",
            // TRANSLATORS: notification message
            N_("Saved location for warp skill."),
            NotifyFlags::EMPTY},
        {"skill memo error level",
            // TRANSLATORS: notification message
            N_("Error saving location. Not enough skill level."),
            NotifyFlags::EMPTY},
        {"skill memo error skill",
            // TRANSLATORS: notification message
            N_("Error saving location. You do not have warp skill."),
            NotifyFlags::EMPTY},
        {"buy trade fail",
            // TRANSLATORS: notification message
            N_("Unable to buy while trading."),
            NotifyFlags::EMPTY},
        {"vending sold item",
            ("%s"),
            NotifyFlags::STRING},
        {"buy fail npc not found",
            // TRANSLATORS: notification message
            N_("Unable to buy. Npc not found."),
            NotifyFlags::EMPTY},
        {"buy fail system error",
            // TRANSLATORS: notification message
            N_("Unable to buy. Shop system error."),
            NotifyFlags::EMPTY},
        {"buy fail wrong item",
            // TRANSLATORS: notification message
            N_("Unable to buy. Wrong items selected."),
            NotifyFlags::EMPTY},
        {"mail name validation error",
            // TRANSLATORS: notification message
            N_("Mail destination name %s is wrong."),
            NotifyFlags::STRING},
        {"mail attach item weight error",
            // TRANSLATORS: notification message
            N_("Item attach failed. Weight too big."),
            NotifyFlags::EMPTY},
        {"mail attach item fatal error",
            // TRANSLATORS: notification message
            N_("Item attach failed. Fatal error."),
            NotifyFlags::EMPTY},
        {"mail attach item no space",
            // TRANSLATORS: notification message
            N_("Item attach failed. No more space."),
            NotifyFlags::EMPTY},
        {"mail attach item non tradeable",
            // TRANSLATORS: notification message
            N_("Item attach failed. Item on tradeable."),
            NotifyFlags::EMPTY},
        {"mail attach item unknown error",
            // TRANSLATORS: notification message
            N_("Item attach failed. Unknown error."),
            NotifyFlags::EMPTY},
        {"mail remove item error",
            // TRANSLATORS: notification message
            N_("Item %s remove failed."),
            NotifyFlags::STRING},
        {"mail send fatal error",
            // TRANSLATORS: notification message
            N_("Mail send failed. Fatal error."),
            NotifyFlags::EMPTY},
        {"mail send count error",
            // TRANSLATORS: notification message
            N_("Mail send failed. Too many mails sent."),
            NotifyFlags::EMPTY},
        {"mail send item error",
            // TRANSLATORS: notification message
            N_("Mail send failed. Wrong attach found."),
            NotifyFlags::EMPTY},
        {"mail send receiver error",
            // TRANSLATORS: notification message
            N_("Mail send failed. Receiver name wrong or not checked."),
            NotifyFlags::EMPTY},
        {"mail get attach full error",
            // TRANSLATORS: notification message
            N_("Error on getting attach. No space or weight too high."),
            NotifyFlags::EMPTY},
        {"mail get money ok",
            // TRANSLATORS: notification message
            N_("You got money from mail."),
            NotifyFlags::EMPTY},
        {"mail get money error",
            // TRANSLATORS: notification message
            N_("Error on getting money attach."),
            NotifyFlags::EMPTY},
        {"mail get money limit error",
            // TRANSLATORS: notification message
            N_("Error on getting money attach. Too many money."),
            NotifyFlags::EMPTY},
    };
}  // namespace NotifyManager
#endif  // RESOURCES_NOTIFICATIONS_H
