/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_TMWA_PROTOCOL_H
#define NET_TMWA_PROTOCOL_H

#include "net/packetdefine.h"

/*********************************
 * Packets from server to client *
 *********************************/

packet(SMSG_SERVER_VERSION_RESPONSE, 0x7531);

packet(CMSG_LOGIN_REGISTER,          0x0064);

packet(SMSG_SERVER_PING,             0x007f); /**< Contains server tick */
packet(SMSG_CONNECTION_PROBLEM,      0x0081);

packet(SMSG_UPDATE_HOST,             0x0063); /**< Custom update host packet */
packet(SMSG_LOGIN_DATA,              0x0069);
packet(SMSG_LOGIN_ERROR,             0x006a);

packet(SMSG_CHAR_LOGIN,              0x006b);
packet(SMSG_CHAR_LOGIN_ERROR,        0x006c);
packet(SMSG_CHAR_CREATE_SUCCEEDED,   0x006d);

packet(SMSG_CHAR_CREATE_FAILED,      0x006e);
packet(SMSG_CHAR_DELETE_SUCCEEDED,   0x006f);
packet(SMSG_CHAR_DELETE_FAILED,      0x0070);
packet(SMSG_CHAR_MAP_INFO,           0x0071);

// Custom packet reply to password change request
packet(SMSG_CHAR_PASSWORD_RESPONSE,  0x0062);

packet(SMSG_CHAR_SWITCH_RESPONSE,    0x00b3);
packet(SMSG_CHANGE_MAP_SERVER,       0x0092);

packet(SMSG_MAP_LOGIN_SUCCESS,       0x0073); /**< Contains starting location */
packet(SMSG_MAP_QUIT_RESPONSE,       0x018b);
packet(SMSG_PLAYER_UPDATE_1,         0x01d8);
packet(SMSG_PLAYER_UPDATE_2,         0x01d9);
packet(SMSG_PLAYER_MOVE,             0x01da); /**< A nearby player moves */
packet(SMSG_PLAYER_STOP,             0x0088); /**< Stop walking, set position */
packet(SMSG_PLAYER_MOVE_TO_ATTACK,   0x0139);
packet(SMSG_PLAYER_STAT_UPDATE_1,    0x00b0);
packet(SMSG_PLAYER_STAT_UPDATE_2,    0x00b1);
packet(SMSG_PLAYER_STAT_UPDATE_3,    0x0141);
packet(SMSG_PLAYER_STAT_UPDATE_4,    0x00bc);
packet(SMSG_PLAYER_STAT_UPDATE_5,    0x00bd);
packet(SMSG_PLAYER_STAT_UPDATE_6,    0x00be);
packet(SMSG_WHO_ANSWER,              0x00c2);
packet(SMSG_PLAYER_WARP,             0x0091);
packet(SMSG_PLAYER_INVENTORY,        0x01ee);
packet(SMSG_PLAYER_INVENTORY_ADD,    0x00a0);
packet(SMSG_PLAYER_INVENTORY_REMOVE, 0x00af);
packet(SMSG_PLAYER_INVENTORY_USE,    0x01c8);
packet(SMSG_PLAYER_EQUIPMENT,        0x00a4);
packet(SMSG_PLAYER_EQUIP,            0x00aa);
packet(SMSG_PLAYER_UNEQUIP,          0x00ac);
packet(SMSG_PLAYER_ATTACK_RANGE,     0x013a);
packet(SMSG_PLAYER_ARROW_EQUIP,      0x013c);
packet(SMSG_PLAYER_ARROW_MESSAGE,    0x013b);
packet(SMSG_PLAYER_SKILLS,           0x010f);
packet(SMSG_PLAYER_SKILL_UP,         0x010e);
packet(SMSG_SKILL_FAILED,            0x0110);
packet(SMSG_SKILL_DAMAGE,            0x01de);
packet(SMSG_ITEM_USE_RESPONSE,       0x00a8);
packet(SMSG_ITEM_VISIBLE,            0x009d); /**< An item is on the floor */
packet(SMSG_ITEM_DROPPED,            0x009e); /**< An item is dropped */
packet(SMSG_ITEM_REMOVE,             0x00a1); /**< An item disappers */
packet(SMSG_BEING_VISIBLE,           0x0078);
packet(SMSG_BEING_MOVE,              0x007b); /**< A nearby monster moves */
packet(SMSG_BEING_SPAWN,             0x007c); /**< A being spawns nearby */
packet(SMSG_BEING_MOVE2,             0x0086); /**< New eAthena being moves */
packet(SMSG_BEING_REMOVE,            0x0080);
packet(SMSG_BEING_CHANGE_LOOKS,      0x00c3);
// Same as 0x00c3, but 16 bit ID
packet(SMSG_BEING_CHANGE_LOOKS2,     0x01d7);
packet(SMSG_BEING_SELFEFFECT,        0x019b);
packet(SMSG_BEING_EMOTION,           0x00c0);
packet(SMSG_BEING_ACTION,            0x008a); /**< Attack, sit, stand up, ... */
packet(SMSG_BEING_CHAT,              0x008d); /**< A being talks */
packet(CMSG_NAME_REQUEST,            0x0094);
packet(SMSG_BEING_NAME_RESPONSE,     0x0095); /**< Has to be requested */
packet(SMSG_BEING_CHANGE_DIRECTION,  0x009c);
packet(SMSG_BEING_RESURRECT,         0x0148);

packet(SMSG_PLAYER_STATUS_CHANGE,    0x0119);
packet(SMSG_PLAYER_GUILD_PARTY_INFO, 0x0195);
packet(SMSG_BEING_STATUS_CHANGE,     0x0196);

packet(SMSG_NPC_MESSAGE,             0x00b4);
packet(SMSG_NPC_NEXT,                0x00b5);
packet(SMSG_NPC_CLOSE,               0x00b6);
packet(SMSG_NPC_CHOICE,              0x00b7); /**< Display a choice */
packet(SMSG_NPC_BUY_SELL_CHOICE,     0x00c4);
packet(SMSG_NPC_BUY,                 0x00c6);
packet(SMSG_NPC_SELL,                0x00c7);
packet(SMSG_NPC_BUY_RESPONSE,        0x00ca);
packet(SMSG_NPC_SELL_RESPONSE,       0x00cb);
packet(SMSG_NPC_INT_INPUT,           0x0142); /**< Integer input */
packet(SMSG_NPC_STR_INPUT,           0x01d4); /**< String input */
packet(SMSG_PLAYER_CHAT,             0x008e); /**< Player talks */
packet(SMSG_WHISPER,                 0x0097); /**< Whisper Recieved */
packet(SMSG_WHISPER_RESPONSE,        0x0098);
packet(SMSG_GM_CHAT,                 0x009a); /**< GM announce */
packet(SMSG_WALK_RESPONSE,           0x0087);

// Receiving a request to trade
packet(SMSG_TRADE_REQUEST,           0x00e5);
packet(SMSG_TRADE_RESPONSE,          0x00e7);
packet(SMSG_TRADE_ITEM_ADD,          0x00e9);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE, 0x01b1); /**< Not standard eAthena! */
packet(SMSG_TRADE_OK,                0x00ec);
packet(SMSG_TRADE_CANCEL,            0x00ee);
packet(SMSG_TRADE_COMPLETE,          0x00f0);

packet(SMSG_PARTY_CREATE,            0x00fa);
packet(SMSG_PARTY_INFO,              0x00fb);
packet(SMSG_PARTY_INVITE_RESPONSE,   0x00fd);
packet(SMSG_PARTY_INVITED,           0x00fe);
packet(SMSG_PARTY_SETTINGS,          0x0101);
packet(SMSG_PARTY_MOVE,              0x0104);
packet(SMSG_PARTY_LEAVE,             0x0105);
packet(SMSG_PARTY_UPDATE_HP,         0x0106);
packet(SMSG_PARTY_UPDATE_COORDS,     0x0107);
packet(SMSG_PARTY_MESSAGE,           0x0109);

packet(SMSG_PLAYER_STORAGE_ITEMS,    0x01f0); /**< Item list for storage */
packet(SMSG_PLAYER_STORAGE_EQUIP,    0x00a6); /**< Equipment list for storage */
packet(SMSG_PLAYER_STORAGE_STATUS,   0x00f2); /**< Slots used and total slots */
packet(SMSG_PLAYER_STORAGE_ADD,      0x00f4); /**< Add item/equip to storage */
// Remove item/equip from storage
packet(SMSG_PLAYER_STORAGE_REMOVE,   0x00f6);
packet(SMSG_PLAYER_STORAGE_CLOSE,    0x00f8); /**< Storage access closed */

packet(SMSG_ADMIN_KICK_ACK,          0x00cd);

packet(SMSG_GUILD_CREATE_RESPONSE,   0x0167);
packet(SMSG_GUILD_POSITION_INFO,     0x016c);
packet(SMSG_GUILD_MEMBER_LOGIN,      0x016d);
packet(SMSG_GUILD_MASTER_OR_MEMBER,  0x014e);
packet(SMSG_GUILD_BASIC_INFO,        0x01b6);
packet(SMSG_GUILD_ALIANCE_INFO,      0x014c);
packet(SMSG_GUILD_MEMBER_LIST,       0x0154);
packet(SMSG_GUILD_POS_NAME_LIST,     0x0166);
packet(SMSG_GUILD_POS_INFO_LIST,     0x0160);
packet(SMSG_GUILD_POSITION_CHANGED,  0x0174);
packet(SMSG_GUILD_MEMBER_POS_CHANGE, 0x0156);
packet(SMSG_GUILD_EMBLEM_DATA,       0x0152);
packet(SMSG_GUILD_SKILL_INFO,        0x0162);
packet(SMSG_GUILD_NOTICE,            0x016f);
packet(SMSG_GUILD_INVITE,            0x016a);
packet(SMSG_GUILD_INVITE_ACK,        0x0169);
packet(SMSG_GUILD_LEAVE,             0x015a);
packet(SMSG_GUILD_EXPULSION,         0x015c);
packet(SMSG_GUILD_EXPULSION_LIST,    0x0163);
packet(SMSG_GUILD_MESSAGE,           0x017f);
packet(SMSG_GUILD_SKILL_UP,          0x010e);
packet(SMSG_GUILD_REQ_ALLIANCE,      0x0171);
packet(SMSG_GUILD_REQ_ALLIANCE_ACK,  0x0173);
packet(SMSG_GUILD_DEL_ALLIANCE,      0x0184);
packet(SMSG_GUILD_OPPOSITION_ACK,    0x0181);
packet(SMSG_GUILD_BROKEN,            0x015e);

packet(SMSG_MVP_EFFECT,              0x010c);

/**********************************
 *  Packets from client to server *
 **********************************/
packet(CMSG_SERVER_VERSION_REQUEST,  0x7530);

// Custom change password packet
packet(CMSG_CHAR_PASSWORD_CHANGE,    0x0061);
packet(CMSG_CHAR_SERVER_CONNECT,     0x0065);
packet(CMSG_CHAR_SELECT,             0x0066);
packet(CMSG_CHAR_CREATE,             0x0067);
packet(CMSG_CHAR_DELETE,             0x0068);

packet(CMSG_MAP_SERVER_CONNECT,      0x0072);
packet(CMSG_MAP_PING,                0x007e); /**< Send to server with tick */
packet(CMSG_MAP_LOADED,              0x007d);
packet(CMSG_CLIENT_QUIT,             0x018A);

packet(CMSG_CHAT_MESSAGE,            0x008c);
packet(CMSG_CHAT_WHISPER,            0x0096);
packet(CMSG_CHAT_WHO,                0x00c1);

packet(CMSG_SKILL_LEVELUP_REQUEST,   0x0112);
packet(CMSG_STAT_UPDATE_REQUEST,     0x00bb);
packet(CMSG_SKILL_USE_BEING,         0x0113);
packet(CMSG_SKILL_USE_POSITION,      0x0116);
// Variant of 0x116 with 80 char string at end (unsure of use)
packet(CMSG_SKILL_USE_POSITION_MORE, 0x0190);
packet(CMSG_SKILL_USE_MAP,           0x011b);

packet(CMSG_PLAYER_INVENTORY_USE,    0x00a7);
packet(CMSG_PLAYER_INVENTORY_DROP,   0x00a2);
packet(CMSG_PLAYER_EQUIP,            0x00a9);
packet(CMSG_PLAYER_UNEQUIP,          0x00ab);

packet(CMSG_ITEM_PICKUP,             0x009f);
packet(CMSG_PLAYER_CHANGE_DIR,       0x009b);
packet(CMSG_PLAYER_CHANGE_DEST,      0x0085);
packet(CMSG_PLAYER_CHANGE_ACT,       0x0089);
packet(CMSG_PLAYER_RESTART,          0x00b2);
packet(CMSG_PLAYER_EMOTE,            0x00bf);
packet(CMSG_PLAYER_STOP_ATTACK,      0x0118);
packet(CMSG_WHO_REQUEST,             0x00c1);

packet(CMSG_NPC_TALK,                0x0090);
packet(CMSG_NPC_NEXT_REQUEST,        0x00b9);
packet(CMSG_NPC_CLOSE,               0x0146);
packet(CMSG_NPC_LIST_CHOICE,         0x00b8);
packet(CMSG_NPC_INT_RESPONSE,        0x0143);
packet(CMSG_NPC_STR_RESPONSE,        0x01d5);
packet(CMSG_NPC_BUY_SELL_REQUEST,    0x00c5);
packet(CMSG_NPC_BUY_REQUEST,         0x00c8);
packet(CMSG_NPC_SELL_REQUEST,        0x00c9);

packet(CMSG_TRADE_REQUEST,           0x00e4);
packet(CMSG_TRADE_RESPONSE,          0x00e6);
packet(CMSG_TRADE_ITEM_ADD_REQUEST,  0x00e8);
packet(CMSG_TRADE_CANCEL_REQUEST,    0x00ed);
packet(CMSG_TRADE_ADD_COMPLETE,      0x00eb);
packet(CMSG_TRADE_OK,                0x00ef);

packet(CMSG_PARTY_CREATE,            0x00f9);
packet(CMSG_PARTY_INVITE,            0x00fc);
packet(CMSG_PARTY_INVITED,           0x00ff);
packet(CMSG_PARTY_LEAVE,             0x0100);
packet(CMSG_PARTY_SETTINGS,          0x0102);
packet(CMSG_PARTY_KICK,              0x0103);
packet(CMSG_PARTY_MESSAGE,           0x0108);

packet(CMSG_MOVE_TO_STORAGE,         0x00f3); /** Move item to storage */
packet(CMSG_MOVE_FROM_STORAGE,       0x00f5); /** Remove item from storage */
packet(CMSG_CLOSE_STORAGE,           0x00f7); /** Request storage close */

packet(CMSG_ADMIN_ANNOUNCE,          0x0099);
packet(CMSG_ADMIN_LOCAL_ANNOUNCE,    0x019C);
packet(CMSG_ADMIN_HIDE,              0x019D);
packet(CMSG_ADMIN_KICK,              0x00CC);
packet(CMSG_ADMIN_MUTE,              0x0149);

packet(CMSG_GUILD_CHECK_MASTER,      0x014d);
packet(CMSG_GUILD_REQUEST_INFO,      0x014f);
packet(CMSG_GUILD_REQUEST_EMBLEM,    0x0151);
packet(CMSG_GUILD_CHANGE_EMBLEM,     0x0153);
packet(CMSG_GUILD_CHANGE_MEMBER_POS, 0x0155);
packet(CMSG_GUILD_LEAVE,             0x0159);
packet(CMSG_GUILD_EXPULSION,         0x015b);
packet(CMSG_GUILD_BREAK,             0x015d);
packet(CMSG_GUILD_CHANGE_POS_INFO,   0x0161);
packet(CMSG_GUILD_CREATE,            0x0165);
packet(CMSG_GUILD_INVITE,            0x0168);
packet(CMSG_GUILD_INVITE_REPLY,      0x016b);
packet(CMSG_GUILD_CHANGE_NOTICE,     0x016e);
packet(CMSG_GUILD_ALLIANCE_REQUEST,  0x0170);
packet(CMSG_GUILD_ALLIANCE_REPLY,    0x0172);
packet(CMSG_GUILD_MESSAGE,           0x017e);
packet(CMSG_GUILD_OPPOSITION,        0x0180);
packet(CMSG_GUILD_ALLIANCE_DELETE,   0x0183);

packet(CMSG_SOLVE_CHAR_NAME,         0x0193);
packet(CMSG_IGNORE_NICK,             0x00cf);
packet(SMSG_SOLVE_CHAR_NAME,         0x0194);

packet(CMSG_CLIENT_DISCONNECT,       0x7532);
packet(SMSG_SKILL_CASTING,           0x013e);
packet(SMSG_SKILL_CAST_CANCEL,       0x01b9);
packet(SMSG_SKILL_NO_DAMAGE,         0x011a);

packet(SMSG_BEING_IP_RESPONSE,       0x020c);
packet(SMSG_PVP_MAP_MODE,            0x0199);
packet(SMSG_PVP_SET,                 0x019a);
packet(CMSG_IGNORE_ALL,              0x00d0);
packet(SMSG_IGNORE_ALL_RESPONSE,     0x00d2);
packet(SMSG_NPC_COMMAND,             0x0212);
packet(SMSG_QUEST_SET_VAR,           0x0214);
packet(SMSG_QUEST_PLAYER_VARS,       0x0215);
packet(SMSG_BEING_MOVE3,             0x0225);
packet(SMSG_MAP_MUSIC,               0x0227);
packet(SMSG_NPC_CHANGETITLE,         0x0228);
packet(SMSG_SCRIPT_MESSAGE,          0x0229);

#undef packet

#endif  // NET_TMWA_PROTOCOL_H
