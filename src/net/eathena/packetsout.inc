/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

// 0
packet(CMSG_SERVER_VERSION_REQUEST,       0x7530,   0, nullptr);

packet(CMSG_LOGIN_REGISTER,               0x0064,   0, nullptr);
packet(CMSG_LOGIN_REGISTER2,              0x027c,   0, nullptr);
packet(CMSG_NAME_REQUEST,                 0x088a,   0, nullptr);

packet(CMSG_CHAR_PASSWORD_CHANGE,         0x0061,   0, nullptr);
packet(CMSG_CHAR_SERVER_CONNECT,          0x0065,   0, nullptr);
packet(CMSG_CHAR_SELECT,                  0x0066,   0, nullptr);
packet(CMSG_CHAR_CREATE,                  0x0970,   0, nullptr);
packet(CMSG_CHAR_DELETE,                  0x0068,   0, nullptr);
packet(CMSG_CHAR_CREATE_PIN,              0x08ba,   0, nullptr);
packet(CMSG_CHAR_CHECK_RENAME,            0x08fc,   0, nullptr);
packet(CMSG_CHAR_RENAME,                  0x028f,   0, nullptr);
packet(CMSG_CHAR_CHANGE_SLOT,             0x08d4,   0, nullptr);

packet(CMSG_MAP_SERVER_CONNECT,           0x089c,   0, nullptr);
packet(CMSG_MAP_PING,                     0x035f,   0, nullptr);
packet(CMSG_LOGIN_PING,                   0x0200,   0, nullptr);
packet(CMSG_CHAR_PING,                    0x0187,   0, nullptr);
packet(CMSG_MAP_LOADED,                   0x007d,   0, nullptr);
packet(CMSG_CLIENT_QUIT,                  0x018A,   0, nullptr);

packet(CMSG_CHAT_MESSAGE,                 0x00f3,   0, nullptr);
packet(CMSG_CHAT_WHISPER,                 0x0096,   0, nullptr);
packet(CMSG_CHAT_WHO,                     0x00c1,   0, nullptr);
packet(CMSG_CHAT_ROOM_JOIN,               0x00d9,   0, nullptr);
packet(CMSG_CHAT_JOIN_CHANNEL,            0x0b07,   0, nullptr);
packet(CMSG_CHAT_PART_CHANNEL,            0x0b09,   0, nullptr);
packet(CMSG_BATTLE_CHAT_MESSAGE,          0x02db,   0, nullptr);

packet(CMSG_BATTLE_REGISTER,              0x08d7,   0, nullptr);
packet(CMSG_BATTLE_REVOKE,                0x08da,   0, nullptr);
packet(CMSG_BATTLE_BEGIN_ACK,             0x08e0,   0, nullptr);
packet(CMSG_BATTLE_CHECK_STATE,           0x090a,   0, nullptr);

packet(CMSG_CREAYE_CHAT_ROOM,             0x00d5,   0, nullptr);
packet(CMSG_LEAVE_CHAT_ROOM,              0x00e3,   0, nullptr);
packet(CMSG_SET_CHAT_ROOM_OPTIONS,        0x00de,   0, nullptr);
packet(CMSG_SET_CHAT_ROOM_OWNER,          0x00e0,   0, nullptr);
packet(CMSG_KICK_FROM_CHAT_ROOM,          0x00e2,   0, nullptr);

packet(CMSG_SKILL_LEVELUP_REQUEST,        0x0112,   0, nullptr);
packet(CMSG_STAT_UPDATE_REQUEST,          0x00bb,   0, nullptr);
packet(CMSG_SKILL_USE_BEING,              0x083c,   0, nullptr);
packet(CMSG_SKILL_USE_POSITION,           0x0436,   0, nullptr);
packet(CMSG_SKILL_USE_POSITION_MORE,      0x0366,   0, nullptr);
packet(CMSG_SKILL_USE_MAP,                0x011b,   0, nullptr);

packet(CMSG_PLAYER_INVENTORY_USE,         0x0439,   0, nullptr);
packet(CMSG_PLAYER_INVENTORY_DROP,        0x0362,   0, nullptr);
packet(CMSG_PLAYER_EQUIP,                 0x0998,   0, nullptr);
packet(CMSG_PLAYER_UNEQUIP,               0x00ab,   0, nullptr);
packet(CMSG_PLAYER_USE_CARD,              0x017a,   0, nullptr);
packet(CMSG_PLAYER_INSERT_CARD,           0x017c,   0, nullptr);
packet(CMSG_PLAYER_VIEW_EQUIPMENT,        0x02d6,   0, nullptr);
packet(CMSG_PLAYER_SET_EQUIPMENT_VISIBLE, 0x02d8,   0, nullptr);
packet(CMSG_PLAYER_FAVORITE_ITEM,         0x0907,   0, nullptr);

packet(CMSG_ITEM_PICKUP,                  0x07e4,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_DIR,            0x0202,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_DEST,           0x0437,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_ACT,            0x0871,   0, nullptr);
packet(CMSG_PLAYER_RESTART,               0x00b2,   0, nullptr);
packet(CMSG_PLAYER_EMOTE,                 0x00bf,   0, nullptr);
packet(CMSG_PLAYER_STOP_ATTACK,           0x0118,   0, nullptr);
packet(CMSG_WHO_REQUEST,                  0x00c1,   0, nullptr);

packet(CMSG_NPC_TALK,                     0x0090,   0, nullptr);
packet(CMSG_NPC_NEXT_REQUEST,             0x00b9,   0, nullptr);
packet(CMSG_NPC_CLOSE,                    0x0146,   0, nullptr);
packet(CMSG_NPC_LIST_CHOICE,              0x00b8,   0, nullptr);
packet(CMSG_NPC_INT_RESPONSE,             0x0143,   0, nullptr);
packet(CMSG_NPC_STR_RESPONSE,             0x01d5,   0, nullptr);
packet(CMSG_NPC_BUY_SELL_REQUEST,         0x00c5,   0, nullptr);
packet(CMSG_NPC_BUY_REQUEST,              0x00c8,   0, nullptr);
packet(CMSG_NPC_SELL_REQUEST,             0x00c9,   0, nullptr);
packet(CMSG_NPC_MARKET_CLOSE,             0x09d8,   0, nullptr);
packet(CMSG_NPC_MARKET_BUY,               0x09d6,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_BUY,            0x0288,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_CLOSE,          0x084a,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_OPEN,           0x0844,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_REQUEST_TAB,    0x0846,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_SCHEDULE,       0x08c9,   0, nullptr);

packet(CMSG_TRADE_REQUEST,                0x00e4,   0, nullptr);
packet(CMSG_TRADE_RESPONSE,               0x00e6,   0, nullptr);
packet(CMSG_TRADE_ITEM_ADD_REQUEST,       0x00e8,   0, nullptr);
packet(CMSG_TRADE_CANCEL_REQUEST,         0x00ed,   0, nullptr);
packet(CMSG_TRADE_ADD_COMPLETE,           0x00eb,   0, nullptr);
packet(CMSG_TRADE_OK,                     0x00ef,   0, nullptr);

packet(CMSG_PARTY_CREATE,                 0x00f9,   0, nullptr);
packet(CMSG_PARTY_CREATE2,                0x01e8,   0, nullptr);
packet(CMSG_PARTY_INVITE,                 0x00fc,   0, nullptr);
packet(CMSG_PARTY_INVITE2,                0x095d,   0, nullptr);
packet(CMSG_PARTY_INVITED,                0x00ff,   0, nullptr);
packet(CMSG_PARTY_INVITED2,               0x02c7,   0, nullptr);
packet(CMSG_PARTY_LEAVE,                  0x0100,   0, nullptr);
packet(CMSG_PARTY_SETTINGS,               0x0102,   0, nullptr);
packet(CMSG_PARTY_KICK,                   0x0103,   0, nullptr);
packet(CMSG_PARTY_MESSAGE,                0x0108,   0, nullptr);
packet(CMSG_PARTY_CHANGE_LEADER,          0x07da,   0, nullptr);
packet(CMSG_PARTY_ALLOW_INVITES,          0x02c8,   0, nullptr);

packet(CMSG_MOVE_TO_STORAGE,              0x07ec,   0, nullptr);
packet(CMSG_MOVE_FROM_STORAGE,            0x085b,   0, nullptr);
packet(CMSG_CLOSE_STORAGE,                0x0193,   0, nullptr);

packet(CMSG_MOVE_TO_CART,                 0x0126,   0, nullptr);
packet(CMSG_MOVE_FROM_CART,               0x0127,   0, nullptr);
packet(CMSG_CHANGE_CART,                  0x01af,   0, nullptr);
packet(CMSG_MOVE_FROM_STORAGE_TO_CART,    0x0128,   0, nullptr);
packet(CMSG_MOVE_FROM_CART_TO_STORAGE,    0x0129,   0, nullptr);

packet(CMSG_ADMIN_ANNOUNCE,               0x0099,   0, nullptr);
packet(CMSG_ADMIN_LOCAL_ANNOUNCE,         0x019C,   0, nullptr);
packet(CMSG_ADMIN_HIDE,                   0x019D,   0, nullptr);
packet(CMSG_ADMIN_KICK,                   0x00CC,   0, nullptr);
packet(CMSG_ADMIN_KICK_ALL,               0x00ce,   0, nullptr);
packet(CMSG_ADMIN_RESET_PLAYER,           0x0197,   0, nullptr);
packet(CMSG_ADMIN_GOTO,                   0x01bb,   0, nullptr);
packet(CMSG_ADMIN_RECALL,                 0x01bd,   0, nullptr);
packet(CMSG_ADMIN_MUTE,                   0x0149,   0, nullptr);
packet(CMSG_ADMIN_MUTE_NAME,              0x0212,   0, nullptr);
packet(CMSG_ADMIN_ID_TO_LOGIN,            0x01df,   0, nullptr);
packet(CMSG_ADMIN_SET_TILE_TYPE,          0x0198,   0, nullptr);
packet(CMSG_ADMIN_UNEQUIP_ALL,            0x07f5,   0, nullptr);
packet(CMSG_ADMIN_REQUEST_STATS,          0x0213,   0, nullptr);

packet(CMSG_GUILD_CHECK_MASTER,           0x014d,   0, nullptr);
packet(CMSG_GUILD_REQUEST_INFO,           0x014f,   0, nullptr);
packet(CMSG_GUILD_REQUEST_EMBLEM,         0x0151,   0, nullptr);
packet(CMSG_GUILD_CHANGE_EMBLEM,          0x0153,   0, nullptr);
packet(CMSG_GUILD_CHANGE_MEMBER_POS,      0x0155,   0, nullptr);
packet(CMSG_GUILD_LEAVE,                  0x0159,   0, nullptr);
packet(CMSG_GUILD_EXPULSION,              0x015b,   0, nullptr);
packet(CMSG_GUILD_BREAK,                  0x015d,   0, nullptr);
packet(CMSG_GUILD_CHANGE_POS_INFO,        0x0161,   0, nullptr);
packet(CMSG_GUILD_CREATE,                 0x0165,   0, nullptr);
packet(CMSG_GUILD_INVITE,                 0x0168,   0, nullptr);
packet(CMSG_GUILD_INVITE_REPLY,           0x016b,   0, nullptr);
packet(CMSG_GUILD_CHANGE_NOTICE,          0x016e,   0, nullptr);
packet(CMSG_GUILD_ALLIANCE_REQUEST,       0x0170,   0, nullptr);
packet(CMSG_GUILD_ALLIANCE_REPLY,         0x0172,   0, nullptr);
packet(CMSG_GUILD_MESSAGE,                0x017e,   0, nullptr);
packet(CMSG_GUILD_OPPOSITION,             0x0180,   0, nullptr);
packet(CMSG_GUILD_ALLIANCE_DELETE,        0x0183,   0, nullptr);

packet(CMSG_SOLVE_CHAR_NAME,              0x0368,   0, nullptr);
packet(CMSG_IGNORE_ALL,                   0x00d0,   0, nullptr);
packet(CMSG_IGNORE_NICK,                  0x00cf,   0, nullptr);
packet(CMSG_REQUEST_IGNORE_LIST,          0x00d3,   0, nullptr);
packet(CMSG_REQUEST_RANKS,                0x097c,   0, nullptr);
packet(CMSG_SET_SHORTCUTS,                0x02ba,   0, nullptr);
packet(CMSG_SHORTCUTS_ROW_SHIFT,          0x0a01,   0, nullptr);
packet(CMSG_NPC_COMPLETE_PROGRESS_BAR,    0x02f1,   0, nullptr);
packet(CMSG_NPC_PRODUCE_MIX,              0x018e,   0, nullptr);
packet(CMSG_NPC_COOKING,                  0x025b,   0, nullptr);
packet(CMSG_NPC_REPAIR,                   0x01fd,   0, nullptr);
packet(CMSG_NPC_REFINE,                   0x0222,   0, nullptr);
packet(CMSG_NPC_IDENTIFY,                 0x0178,   0, nullptr);
packet(CMSG_NPC_SELECT_ARROW,             0x01ae,   0, nullptr);
packet(CMSG_NPC_SELECT_AUTO_SPELL,        0x01ce,   0, nullptr);
packet(CMSG_NPC_SHOP_CLOSE,               0x09d4,   0, nullptr);

packet(CMSG_PLAYER_MAPMOVE,               0x0140,   0, nullptr);
packet(CMSG_REMOVE_OPTION,                0x012a,   0, nullptr);
packet(CMSG_PLAYER_SET_MEMO,              0x011d,   0, nullptr);

packet(CMSG_PET_CATCH,                    0x019f,   0, nullptr);
packet(CMSG_PET_SEND_MESSAGE,             0x01a9,   0, nullptr);
packet(CMSG_PET_SET_NAME,                 0x01a5,   0, nullptr);
packet(CMSG_PET_SELECT_EGG,               0x01a7,   0, nullptr);
packet(CMSG_PET_MENU_ACTION,              0x01a1,   0, nullptr);
packet(CMSG_PET_TALK,                     0x0b0c,   0, nullptr);
packet(CMSG_PET_EMOTE,                    0x0b0d,   0, nullptr);
packet(CMSG_PET_MOVE_TO,                  0x0b11,   0, nullptr);
packet(CMSG_PET_DIRECTION,                0x0b12,   0, nullptr);

packet(CMSG_MERCENARY_ACTION,             0x029f,   0, nullptr);
packet(CMSG_HOMUNCULUS_SET_NAME,          0x0231,   0, nullptr);
packet(CMSG_HOMUNCULUS_MENU,              0x0361,   0, nullptr);
packet(CMSG_HOMMERC_MOVE_TO_MASTER,       0x0234,   0, nullptr);
packet(CMSG_HOMMERC_MOVE_TO,              0x0232,   0, nullptr);
packet(CMSG_HOMMERC_ATTACK,               0x0233,   0, nullptr);
packet(CMSG_HOMMERC_TALK,                 0x0b13,   0, nullptr);
packet(CMSG_HOMMERC_EMOTE,                0x0b14,   0, nullptr);
packet(CMSG_HOMMERC_DIRECTION,            0x0b15,   0, nullptr);

packet(CMSG_DORI_DORI,                    0x01e7,   0, nullptr);
packet(CMSG_EXPLOSION_SPIRITS,            0x01ed,   0, nullptr);
packet(CMSG_PVP_INFO,                     0x020f,   0, nullptr);
packet(CMSG_PLAYER_AUTO_REVIVE,           0x0292,   0, nullptr);
packet(CMSG_QUEST_ACTIVATE,               0x02b6,   0, nullptr);

packet(CMSG_MAIL_REFRESH_INBOX,           0x023f,   0, nullptr);
packet(CMSG_MAIL_READ_MESSAGE,            0x0241,   0, nullptr);
packet(CMSG_MAIL_GET_ATTACH,              0x0244,   0, nullptr);
packet(CMSG_MAIL_DELETE_MESSAGE,          0x0243,   0, nullptr);
packet(CMSG_MAIL_RETURN_MESSAGE,          0x0273,   0, nullptr);
packet(CMSG_MAIL_SET_ATTACH,              0x0247,   0, nullptr);
packet(CMSG_MAIL_RESET_ATTACH,            0x0246,   0, nullptr);
packet(CMSG_MAIL_SEND,                    0x0248,   0, nullptr);

packet(CMSG_FAMILY_ASK_FOR_CHILD,         0x01f9,   0, nullptr);
packet(CMSG_FAMILY_ASK_FOR_CHILD_REPLY,   0x01f7,   0, nullptr);

packet(CMSG_BANK_DEPOSIT,                 0x09a7,   0, nullptr);
packet(CMSG_BANK_WITHDRAW,                0x09a9,   0, nullptr);
packet(CMSG_BANK_CHECK,                   0x09ab,   0, nullptr);
packet(CMSG_BANK_OPEN,                    0x09b6,   0, nullptr);
packet(CMSG_BANK_CLOSE,                   0x09b8,   0, nullptr);

packet(CMSG_FRIENDS_ADD_PLAYER,           0x091a,   0, nullptr);
packet(CMSG_FRIENDS_REQUEST_ACK,          0x0208,   0, nullptr);
packet(CMSG_FRIENDS_DELETE_PLAYER,        0x0203,   0, nullptr);

packet(CMSG_AUCTION_CANCEL_REG,           0x024b,   0, nullptr);
packet(CMSG_AUCTION_SET_ITEM,             0x024c,   0, nullptr);
packet(CMSG_AUCTION_REGISTER,             0x024d,   0, nullptr);
packet(CMSG_AUCTION_CANCEL,               0x024e,   0, nullptr);
packet(CMSG_AUCTION_CLOSE,                0x025d,   0, nullptr);
packet(CMSG_AUCTION_BID,                  0x024f,   0, nullptr);
packet(CMSG_AUCTION_SEARCH,               0x0251,   0, nullptr);
packet(CMSG_AUCTION_BUY_SELL,             0x025c,   0, nullptr);

packet(CMSG_VENDING_CLOSE,                0x012e,   0, nullptr);
packet(CMSG_VENDING_LIST_REQ,             0x0130,   0, nullptr);
packet(CMSG_VENDING_BUY,                  0x0134,   0, nullptr);
packet(CMSG_VENDING_BUY2,                 0x0801,   0, nullptr);
packet(CMSG_VENDING_CREATE_SHOP,          0x01b2,   0, nullptr);

packet(CMSG_BUYINGSTORE_CREATE,           0x0815,   0, nullptr);
packet(CMSG_BUYINGSTORE_CLOSE,            0x0817,   0, nullptr);
packet(CMSG_BUYINGSTORE_OPEN,             0x0360,   0, nullptr);
packet(CMSG_BUYINGSTORE_SELL,             0x0811,   0, nullptr);

packet(CMSG_SEARCHSTORE_SEARCH,           0x0819,   0, nullptr);
packet(CMSG_SEARCHSTORE_NEXT_PAGE,        0x0940,   0, nullptr);
packet(CMSG_SEARCHSTORE_CLOSE,            0x083b,   0, nullptr);
packet(CMSG_SEARCHSTORE_CLICK,            0x0835,   0, nullptr);

packet(CMSG_MERGE_ITEM_ACK,               0x096e,   0, nullptr);
packet(CMSG_MERGE_ITEM_CANCEL,            0x0974,   0, nullptr);

packet(CMSG_SET_STATUS,                   0x0b0e,   0, nullptr);

packet(CMSG_ONLINE_LIST,                  0x0b0f,   0, nullptr);

// 20150805
packet(CMSG_SELECT_CART,                  0x0980,   0, nullptr);

#ifndef PACKETS_UPDATE
// 0
packet(CMSG_ALCHEMIST_RANKS,              0x0000,   0, nullptr);
packet(CMSG_BLACKSMITH_RANKS,             0x0000,   0, nullptr);
packet(CMSG_PK_RANKS,                     0x0000,   0, nullptr);
#else
// 20041108
if (packetVersion >= 20041108)
{
    packet(CMSG_ALCHEMIST_RANKS,          0x0218,   0, nullptr);
    packet(CMSG_BLACKSMITH_RANKS,         0x0217,   0, nullptr);
}
// 20050530
if (packetVersion >= 20050530)
{
    packet(CMSG_PK_RANKS,                 0x0237,   0, nullptr);
}
// 20150513
if (packetVersion >= 20150513)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,   0, nullptr);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x022d,   0, nullptr);
}
#endif

// 20150513
packet(CMSG_QUICK_IDENTIFY_ITEM,          0x0a35,   0, nullptr);
