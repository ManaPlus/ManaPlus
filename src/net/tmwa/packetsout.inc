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

packet(CMSG_SERVER_VERSION_REQUEST,  0x7530);

packet(CMSG_LOGIN_REGISTER,          0x0064);

packet(CMSG_NAME_REQUEST,            0x0094);

packet(CMSG_CHAR_PASSWORD_CHANGE,    0x0061);
packet(CMSG_CHAR_SERVER_CONNECT,     0x0065);
packet(CMSG_CHAR_SELECT,             0x0066);
packet(CMSG_CHAR_CREATE,             0x0067);
packet(CMSG_CHAR_DELETE,             0x0068);

packet(CMSG_MAP_SERVER_CONNECT,      0x0072);
packet(CMSG_MAP_PING,                0x007e);
packet(CMSG_MAP_LOADED,              0x007d);
packet(CMSG_CLIENT_QUIT,             0x018A);

packet(CMSG_CHAT_MESSAGE,            0x008c);
packet(CMSG_CHAT_WHISPER,            0x0096);
packet(CMSG_CHAT_WHO,                0x00c1);

packet(CMSG_SKILL_LEVELUP_REQUEST,   0x0112);
packet(CMSG_STAT_UPDATE_REQUEST,     0x00bb);
packet(CMSG_SKILL_USE_BEING,         0x0113);
packet(CMSG_SKILL_USE_POSITION,      0x0116);
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

packet(CMSG_MOVE_TO_STORAGE,         0x00f3);
packet(CMSG_MOVE_FROM_STORAGE,       0x00f5);
packet(CMSG_CLOSE_STORAGE,           0x00f7);

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

packet(CMSG_CLIENT_DISCONNECT,       0x7532);
packet(CMSG_IGNORE_ALL,              0x00d0);
