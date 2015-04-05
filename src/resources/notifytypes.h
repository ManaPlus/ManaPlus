/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2015  The ManaPlus Developers
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

#ifndef RESOURCES_NOTIFYTYPES_H
#define RESOURCES_NOTIFYTYPES_H

#include "localconsts.h"

namespace NotifyTypes
{
    enum Type
    {
        NONE = 0,
        BUY_DONE,
        BUY_FAILED,
        BUY_FAILED_NO_MONEY,
        BUY_FAILED_OVERWEIGHT,
        BUY_FAILED_TOO_MANY_ITEMS,
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
        EQUIP_FAILED_LEVEL,
        PARTY_CREATE_FAILED,
        PARTY_CREATED,
        PARTY_LEFT,
        PARTY_LEFT_DENY,
        PARTY_KICKED,
        PARTY_KICK_DENY,
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
        PARTY_USER_LEFT_DENY,
        PARTY_USER_KICKED,
        PARTY_USER_KICK_DENY,
        PARTY_UNKNOWN_USER_MSG,
        PARTY_USER_NOT_IN_PARTY,
        MONEY_GET,
        MONEY_SPENT,
        SKILL_RAISE_ERROR,
        ARROWS_EQUIP_NEEDED,
        TRADE_FAIL_FAR_AWAY,
        TRADE_FAIL_CHAR_NOT_EXISTS,
        TRADE_CANCELLED_ERROR,
        TRADE_CANCELLED_NAME,
        TRADE_CANCELLED_BUSY,
        TRADE_ERROR_UNKNOWN,
        TRADE_ADD_PARTNER_OVER_WEIGHT,
        TRADE_ADD_PARTNER_NO_SLOTS,
        TRADE_ADD_UNTRADABLE_ITEM,
        TRADE_ADD_ERROR,
        TRADE_CANCELLED,
        TRADE_COMPLETE,
        KICK_FAIL,
        KICK_SUCCEED,
        MVP_PLAYER,
        WHISPERS_IGNORED,
        WHISPERS_IGNORE_FAILED,
        WHISPERS_UNIGNORED,
        WHISPERS_UNIGNORE_FAILED,
        SKILL_FAIL_MESSAGE,
        PVP_OFF_GVG_OFF,
        PVP_ON,
        GVG_ON,
        PVP_ON_GVG_ON,
        PVP_UNKNOWN,
        PARTY_INVITE_PARTY_SAME_ACCOUNT,
        PARTY_INVITE_PARTY_BLOCKED_INVITE,
        PARTY_INVITE_PARTY_NOT_ONLINE,
        PET_CATCH_FAILED,
        PET_CATCH_SUCCESS,
        PET_CATCH_UNKNOWN,
        MERCENARY_EXPIRED,
        MERCENARY_KILLED,
        MERCENARY_FIRED,
        MERCENARY_RUN,
        MERCENARY_UNKNOWN,
        HOMUNCULUS_FEED_OK,
        HOMUNCULUS_FEED_FAIL,
        CARD_INSERT_FAILED,
        CARD_INSERT_SUCCESS,
        BANK_DEPOSIT_FAILED,
        BANK_WITHDRAW_FAILED,
        BUYING_STORE_CREATE_FAILED,
        BUYING_STORE_CREATE_FAILED_WEIGHT,
        BUYING_STORE_CREATE_EMPTY,
        BUYING_STORE_SELL_FAILED_MONEY_LIMIT,
        BUYING_STORE_SELL_FAILED_EMPTY,
        BUYING_STORE_SELL_FAILED,
        BUYING_STORE_SELLER_SELL_FAILED_DEAL,
        BUYING_STORE_SELLER_SELL_FAILED_AMOUNT,
        BUYING_STORE_SELLER_SELL_FAILED_BALANCE,
        BUYING_STORE_SELLER_SELL_FAILED,
        SEARCH_STORE_FAILED,
        SEARCH_STORE_FAILED_NO_STORES,
        SEARCH_STORE_FAILED_MANY_RESULTS,
        SEARCH_STORE_FAILED_CANT_SEARCH_ANYMORE,
        SEARCH_STORE_FAILED_CANT_SEARCH_YET,
        SEARCH_STORE_FAILED_NO_INFORMATION,
        PET_FEED_OK,
        PET_FEED_ERROR,
        MANNER_CHANGED,
        MANNER_POSITIVE_POINTS,
        MANNER_NEGATIVE_POINTS,
        ROOM_LIMIT_EXCEEDED,
        ROOM_ALREADY_EXISTS,
        ROOM_JOINED,
        ROOM_LEAVE,
        ROOM_KICKED,

        TYPE_END
    };
}  // namespace NotifyTypes
#endif  // RESOURCES_NOTIFYTYPES_H
