/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#define PACKETSOUT_VOID

packet(CMSG_SERVER_VERSION_REQUEST,  0x7530,   0, nullptr);

packet(CMSG_LOGIN_REGISTER,          0x0064,   0, nullptr);

packet(CMSG_NAME_REQUEST,            0x0094,   0, nullptr);

packet(CMSG_CHAR_PASSWORD_CHANGE,    0x0061,   0, nullptr);
packet(CMSG_CHAR_SERVER_CONNECT,     0x0065,   0, nullptr);
packet(CMSG_CHAR_SELECT,             0x0066,   0, nullptr);
packet(CMSG_CHAR_CREATE,             0x0067,   0, nullptr);
packet(CMSG_CHAR_DELETE,             0x0068,   0, nullptr);

packet(CMSG_MAP_SERVER_CONNECT,      0x0072,   0, nullptr);
packet(CMSG_MAP_PING,                0x007e,   0, nullptr);
packet(CMSG_MAP_LOADED,              0x007d,   0, nullptr);
packet(CMSG_CLIENT_QUIT,             0x018A,   0, nullptr);

packet(CMSG_CHAT_MESSAGE,            0x008c,   0, nullptr);
packet(CMSG_CHAT_WHISPER,            0x0096,   0, nullptr);
packet(CMSG_CHAT_WHO,                0x00c1,   0, nullptr);

packet(CMSG_SKILL_LEVELUP_REQUEST,   0x0112,   0, nullptr);
packet(CMSG_STAT_UPDATE_REQUEST,     0x00bb,   0, nullptr);
packet(CMSG_SKILL_USE_BEING,         0x0113,   0, nullptr);
packet(CMSG_SKILL_USE_POSITION,      0x0116,   0, nullptr);
packet(CMSG_SKILL_USE_POSITION_MORE, 0x0190,   0, nullptr);
packet(CMSG_SKILL_USE_MAP,           0x011b,   0, nullptr);

packet(CMSG_PLAYER_INVENTORY_USE,    0x00a7,   0, nullptr);
packet(CMSG_PLAYER_INVENTORY_DROP,   0x00a2,   0, nullptr);
packet(CMSG_PLAYER_EQUIP,            0x00a9,   0, nullptr);
packet(CMSG_PLAYER_UNEQUIP,          0x00ab,   0, nullptr);

packet(CMSG_ITEM_PICKUP,             0x009f,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_DIR,       0x009b,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_DEST,      0x0085,   0, nullptr);
packet(CMSG_PLAYER_CHANGE_ACT,       0x0089,   0, nullptr);
packet(CMSG_PLAYER_RESTART,          0x00b2,   0, nullptr);
packet(CMSG_PLAYER_EMOTE,            0x00bf,   0, nullptr);
packet(CMSG_PLAYER_STOP_ATTACK,      0x0118,   0, nullptr);
packet(CMSG_WHO_REQUEST,             0x00c1,   0, nullptr);

packet(CMSG_NPC_TALK,                0x0090,   0, nullptr);
packet(CMSG_NPC_NEXT_REQUEST,        0x00b9,   0, nullptr);
packet(CMSG_NPC_CLOSE,               0x0146,   0, nullptr);
packet(CMSG_NPC_LIST_CHOICE,         0x00b8,   0, nullptr);
packet(CMSG_NPC_INT_RESPONSE,        0x0143,   0, nullptr);
packet(CMSG_NPC_STR_RESPONSE,        0x01d5,   0, nullptr);
packet(CMSG_NPC_BUY_SELL_REQUEST,    0x00c5,   0, nullptr);
packet(CMSG_NPC_BUY_REQUEST,         0x00c8,   0, nullptr);
packet(CMSG_NPC_SELL_REQUEST,        0x00c9,   0, nullptr);

packet(CMSG_TRADE_REQUEST,           0x00e4,   0, nullptr);
packet(CMSG_TRADE_RESPONSE,          0x00e6,   0, nullptr);
packet(CMSG_TRADE_ITEM_ADD_REQUEST,  0x00e8,   0, nullptr);
packet(CMSG_TRADE_CANCEL_REQUEST,    0x00ed,   0, nullptr);
packet(CMSG_TRADE_ADD_COMPLETE,      0x00eb,   0, nullptr);
packet(CMSG_TRADE_OK,                0x00ef,   0, nullptr);

packet(CMSG_PARTY_CREATE,            0x00f9,   0, nullptr);
packet(CMSG_PARTY_INVITE,            0x00fc,   0, nullptr);
packet(CMSG_PARTY_INVITED,           0x00ff,   0, nullptr);
packet(CMSG_PARTY_LEAVE,             0x0100,   0, nullptr);
packet(CMSG_PARTY_SETTINGS,          0x0102,   0, nullptr);
packet(CMSG_PARTY_KICK,              0x0103,   0, nullptr);
packet(CMSG_PARTY_MESSAGE,           0x0108,   0, nullptr);

packet(CMSG_MOVE_TO_STORAGE,         0x00f3,   0, nullptr);
packet(CMSG_MOVE_FROM_STORAGE,       0x00f5,   0, nullptr);
packet(CMSG_CLOSE_STORAGE,           0x00f7,   0, nullptr);

packet(CMSG_ADMIN_ANNOUNCE,          0x0099,   0, nullptr);
packet(CMSG_ADMIN_LOCAL_ANNOUNCE,    0x019C,   0, nullptr);
packet(CMSG_ADMIN_HIDE,              0x019D,   0, nullptr);
packet(CMSG_ADMIN_KICK,              0x00CC,   0, nullptr);
packet(CMSG_ADMIN_MUTE,              0x0149,   0, nullptr);

packet(CMSG_SOLVE_CHAR_NAME,         0x0193,   0, nullptr);
packet(CMSG_IGNORE_NICK,             0x00cf,   0, nullptr);

packet(CMSG_CLIENT_DISCONNECT,       0x7532,   0, nullptr);
packet(CMSG_IGNORE_ALL,              0x00d0,   0, nullptr);

#ifdef PACKETS_UPDATE
// condition code here
#endif
