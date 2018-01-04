/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_NOTIFYTYPES_H
#define ENUMS_RESOURCES_NOTIFYTYPES_H

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
        UNEQUIP_FAILED,
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
        ROOM_ROLE_OWNER,
        ROOM_ERROR_FULL,
        ROOM_ERROR_WRONG_PASSWORD,
        ROOM_ERROR_KICKED,
        ROOM_ERROR_ZENY,
        ROOM_ERROR_LOW_LEVEL,
        ROOM_ERROR_HIGH_LEVEL,
        ROOM_ERROR_RACE,
        USE_ITEM_WAIT,
        MAIL_SEND_OK,
        MAIL_SEND_ERROR,
        MAIL_ATTACH_ITEM_ERROR,
        MAIL_ATTACH_MONEY_ERROR,
        MAIL_RETURN_ERROR,
        MAIL_RETURN_OK,
        MAIL_DELETE_ERROR,
        MAIL_DELETE_OK,
        MAIL_GET_ATTACH_OK,
        MAIL_GET_ATTACH_ERROR,
        MAIL_GET_ATTACH_TOO_MANY_ITEMS,
        NEW_MAIL,
        MAP_TYPE_BATTLEFIELD,
        RENTAL_TIME_LEFT,
        RENTAL_TIME_EXPIRED,
        REFINE_SUCCESS,
        REFINE_FAILURE,
        REFINE_DOWNGRADE,
        REFINE_UNKNOWN,
        CART_ADD_WEIGHT_ERROR,
        CART_ADD_COUNT_ERROR,
        BOUND_ITEM,
        SKILL_END_ALL_NEGATIVE_STATUS,
        SKILL_IMMUNITY_TO_ALL_STATUSES,
        SKILL_MAX_HP,
        SKILL_MAX_SP,
        SKILL_ALL_STATUS_PLUS_20,
        SKILL_ENCHANT_WEAPON_HOLY,
        SKILL_ENCHANT_ARMOR_HOLY,
        SKILL_DEF_PLUS_25,
        SKILL_ATTACK_PLUS_100,
        SKILL_FLEE_PLUS_50,
        SKILL_FULL_STRIP_FAILED,
        SKILL_MESSAGE_UNKNOWN,
        IGNORE_PLAYER_SUCCESS,
        IGNORE_PLAYER_FAILURE,
        IGNORE_PLAYER_TOO_MANY,
        IGNORE_PLAYER_UNKNOWN,
        UNIGNORE_PLAYER_SUCCESS,
        UNIGNORE_PLAYER_FAILURE,
        UNIGNORE_PLAYER_UNKNOWN,
        IGNORE_PLAYER_TYPE_UNKNOWN,
        PET_CATCH_PROCESS,
        DELETE_ITEM_NORMAL,
        DELETE_ITEM_SKILL_USE,
        DELETE_ITEM_FAIL_REFINE,
        DELETE_ITEM_MATERIAL_CHANGE,
        DELETE_ITEM_TO_STORAGE,
        DELETE_ITEM_TO_CART,
        DELETE_ITEM_SOLD,
        DELETE_ITEM_ANALYSIS,
        DELETE_ITEM_UNKNOWN,
        DELETE_ITEM_DROPPED,
        BEING_REMOVE_DIED,
        BEING_REMOVE_LOGGED_OUT,
        BEING_REMOVE_WARPED,
        BEING_REMOVE_TRICK_DEAD,
        BEING_REMOVE_UNKNOWN,
        DIVORCED,
        CALLED_PARTNER,
        CALLING_PARTNER,
        ADOPT_CHILD_ERROR_HAVE_BABY,
        ADOPT_CHILD_ERROR_LEVEL,
        ADOPT_CHILD_ERROR_BABY_MARRIED,
        SKILL_MEMO_SAVED,
        SKILL_MEMO_ERROR_LEVEL,
        SKILL_MEMO_ERROR_SKILL,
        BUY_TRADE_FAILED,
        VENDING_SOLD_ITEM,
        BUY_FAILED_NPC_NOT_FOUND,
        BUY_FAILED_SYSTEM_ERROR,
        BUY_FAILED_WRONG_ITEM,
        MAIL_NAME_VALIDATION_ERROR,
        MAIL_ATTACH_ITEM_WEIGHT_ERROR,
        MAIL_ATTACH_ITEM_FATAL_ERROR,
        MAIL_ATTACH_ITEM_NO_SPACE,
        MAIL_ATTACH_ITEM_NOT_TRADEABLE,
        MAIL_ATTACH_ITEM_UNKNOWN_ERROR,
        MAIL_REMOVE_ITEM_ERROR,
        MAIL_SEND_FATAL_ERROR,
        MAIL_SEND_COUNT_ERROR,
        MAIL_SEND_ITEM_ERROR,
        MAIL_SEND_RECEIVER_ERROR,
        MAIL_GET_ATTACH_FULL_ERROR,
        MAIL_GET_MONEY_OK,
        MAIL_GET_MONEY_ERROR,
        MAIL_GET_MONEY_LIMIT_ERROR,

        TYPE_END
    };
}  // namespace NotifyTypes
#endif  // ENUMS_RESOURCES_NOTIFYTYPES_H
