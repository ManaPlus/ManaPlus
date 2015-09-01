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

packet(SMSG_ADMIN_KICK_ACK,                0x00cd,   6);
packet(SMSG_BEING_ACTION,                  0x008a,  29);
packet(SMSG_BEING_CHANGE_DIRECTION,        0x009c,   9);
packet(SMSG_BEING_CHANGE_LOOKS,            0x00c3,   8);
packet(SMSG_BEING_CHANGE_LOOKS2,           0x01d7,  11);
packet(SMSG_BEING_CHAT,                    0x008d,  -1);
packet(SMSG_BEING_EMOTION,                 0x00c0,   7);
packet(SMSG_BEING_IP_RESPONSE,             0x020c,  10);
packet(SMSG_BEING_MOVE,                    0x007b,  60);
packet(SMSG_BEING_MOVE2,                   0x0086,  16);
packet(SMSG_BEING_MOVE3,                   0x0225,  -1);
packet(SMSG_BEING_NAME_RESPONSE,           0x0095,  30);
packet(SMSG_BEING_REMOVE,                  0x0080,   7);
packet(SMSG_BEING_RESURRECT,               0x0148,   8);
packet(SMSG_BEING_SELFEFFECT,              0x019b,  10);
packet(SMSG_BEING_SPAWN,                   0x007c,  41);
packet(SMSG_BEING_STATUS_CHANGE,           0x0196,   9);
packet(SMSG_BEING_VISIBLE,                 0x0078,  54);
packet(SMSG_CHANGE_MAP_SERVER,             0x0092,  28);
packet(SMSG_CHAR_CREATE_FAILED,            0x006e,   3);
packet(SMSG_CHAR_CREATE_SUCCEEDED,         0x006d, 108);
packet(SMSG_CHAR_DELETE_FAILED,            0x0070,   3);
packet(SMSG_CHAR_DELETE_SUCCEEDED,         0x006f,   2);
packet(SMSG_CHAR_LOGIN,                    0x006b,  -1);
packet(SMSG_CHAR_LOGIN_ERROR,              0x006c,   3);
packet(SMSG_CHAR_MAP_INFO,                 0x0071,  28);
packet(SMSG_CHAR_PASSWORD_RESPONSE,        0x0062,   3);
packet(SMSG_CHAR_SWITCH_RESPONSE,          0x00b3,   3);
packet(SMSG_CONNECTION_PROBLEM,            0x0081,   3);
packet(SMSG_GM_CHAT,                       0x009a,  -1);
packet(SMSG_GUILD_ALIANCE_INFO,            0x014c,  -1);
packet(SMSG_GUILD_BASIC_INFO,              0x01b6, 114);
packet(SMSG_GUILD_BROKEN,                  0x015e,   6);
packet(SMSG_GUILD_CREATE_RESPONSE,         0x0167,   3);
packet(SMSG_GUILD_DEL_ALLIANCE,            0x0184,  10);
packet(SMSG_GUILD_EMBLEM_DATA,             0x0152,  -1);
packet(SMSG_GUILD_EXPULSION,               0x015c,  90);
packet(SMSG_GUILD_EXPULSION_LIST,          0x0163,  -1);
packet(SMSG_GUILD_INVITE,                  0x016a,  30);
packet(SMSG_GUILD_INVITE_ACK,              0x0169,   3);
packet(SMSG_GUILD_LEAVE,                   0x015a,  66);
packet(SMSG_GUILD_MASTER_OR_MEMBER,        0x014e,   6);
packet(SMSG_GUILD_MEMBER_LIST,             0x0154,  -1);
packet(SMSG_GUILD_MEMBER_LOGIN,            0x016d,  14);
packet(SMSG_GUILD_MEMBER_POS_CHANGE,       0x0156,  -1);
packet(SMSG_GUILD_MESSAGE,                 0x017f,  -1);
packet(SMSG_GUILD_NOTICE,                  0x016f, 182);
packet(SMSG_GUILD_OPPOSITION_ACK,          0x0181,   3);
packet(SMSG_GUILD_POSITION_CHANGED,        0x0174,  -1);
packet(SMSG_GUILD_POSITION_INFO,           0x016c,  43);
packet(SMSG_GUILD_POS_INFO_LIST,           0x0160,  -1);
packet(SMSG_GUILD_POS_NAME_LIST,           0x0166,  -1);
packet(SMSG_GUILD_REQ_ALLIANCE,            0x0171,  30);
packet(SMSG_GUILD_REQ_ALLIANCE_ACK,        0x0173,   3);
packet(SMSG_GUILD_SKILL_INFO,              0x0162,  -1);
packet(SMSG_GUILD_SKILL_UP,                0x010e,  11);
packet(SMSG_IGNORE_ALL_RESPONSE,           0x00d2,   4);
packet(SMSG_ITEM_DROPPED,                  0x009e,  17);
packet(SMSG_ITEM_REMOVE,                   0x00a1,   6);
packet(SMSG_ITEM_USE_RESPONSE,             0x00a8,   7);
packet(SMSG_ITEM_VISIBLE,                  0x009d,  17);
packet(SMSG_LOGIN_DATA,                    0x0069,  -1);
packet(SMSG_LOGIN_ERROR,                   0x006a,  23);
packet(SMSG_MAP_LOGIN_SUCCESS,             0x0073,  11);
packet(SMSG_MAP_MUSIC,                     0x0227,  -1);
packet(SMSG_MAP_QUIT_RESPONSE,             0x018b,   4);
packet(SMSG_MVP_EFFECT,                    0x010c,   6);
packet(SMSG_NPC_BUY,                       0x00c6,  -1);
packet(SMSG_NPC_BUY_RESPONSE,              0x00ca,   3);
packet(SMSG_NPC_BUY_SELL_CHOICE,           0x00c4,   6);
packet(SMSG_NPC_CHANGETITLE,               0x0228,  -1);
packet(SMSG_NPC_CHOICE,                    0x00b7,  -1);
packet(SMSG_NPC_CLOSE,                     0x00b6,   6);
packet(SMSG_NPC_COMMAND,                   0x0212,  16);
packet(SMSG_NPC_INT_INPUT,                 0x0142,   6);
packet(SMSG_NPC_MESSAGE,                   0x00b4,  -1);
packet(SMSG_NPC_NEXT,                      0x00b5,   6);
packet(SMSG_NPC_SELL,                      0x00c7,  -1);
packet(SMSG_NPC_SELL_RESPONSE,             0x00cb,   3);
packet(SMSG_NPC_STR_INPUT,                 0x01d4,   6);
packet(SMSG_PARTY_CREATE,                  0x00fa,   3);
packet(SMSG_PARTY_INFO,                    0x00fb,  -1);
packet(SMSG_PARTY_INVITED,                 0x00fe,  30);
packet(SMSG_PARTY_INVITE_RESPONSE,         0x00fd,  27);
packet(SMSG_PARTY_LEAVE,                   0x0105,  31);
packet(SMSG_PARTY_MESSAGE,                 0x0109,  -1);
packet(SMSG_PARTY_MOVE,                    0x0104,  79);
packet(SMSG_PARTY_SETTINGS,                0x0101,   6);
packet(SMSG_PARTY_UPDATE_COORDS,           0x0107,  10);
packet(SMSG_PARTY_UPDATE_HP,               0x0106,  10);
packet(SMSG_PLAYER_ARROW_EQUIP,            0x013c,   4);
packet(SMSG_PLAYER_ARROW_MESSAGE,          0x013b,   4);
packet(SMSG_PLAYER_ATTACK_RANGE,           0x013a,   4);
packet(SMSG_PLAYER_CHAT,                   0x008e,  -1);
packet(SMSG_PLAYER_EQUIP,                  0x00aa,   7);
packet(SMSG_PLAYER_EQUIPMENT,              0x00a4,  -1);
packet(SMSG_PLAYER_GUILD_PARTY_INFO,       0x0195, 102);
packet(SMSG_PLAYER_INVENTORY,              0x01ee,  -1);
packet(SMSG_PLAYER_INVENTORY_ADD,          0x00a0,  23);
packet(SMSG_PLAYER_INVENTORY_REMOVE,       0x00af,   6);
packet(SMSG_PLAYER_INVENTORY_USE,          0x01c8,  13);
packet(SMSG_PLAYER_MOVE,                   0x01da,  60);
packet(SMSG_PLAYER_MOVE_TO_ATTACK,         0x0139,  16);
packet(SMSG_PLAYER_SKILLS,                 0x010f,  -1);
packet(SMSG_PLAYER_SKILL_UP,               0x010e,  11);
packet(SMSG_PLAYER_STATUS_CHANGE,          0x0119,  13);
packet(SMSG_PLAYER_STAT_UPDATE_1,          0x00b0,   8);
packet(SMSG_PLAYER_STAT_UPDATE_2,          0x00b1,   8);
packet(SMSG_PLAYER_STAT_UPDATE_3,          0x0141,  14);
packet(SMSG_PLAYER_STAT_UPDATE_4,          0x00bc,   6);
packet(SMSG_PLAYER_STAT_UPDATE_5,          0x00bd,  44);
packet(SMSG_PLAYER_STAT_UPDATE_6,          0x00be,   5);
packet(SMSG_PLAYER_STOP,                   0x0088,  10);
packet(SMSG_PLAYER_STORAGE_ADD,            0x00f4,  21);
packet(SMSG_PLAYER_STORAGE_CLOSE,          0x00f8,   2);
packet(SMSG_PLAYER_STORAGE_EQUIP,          0x00a6,  -1);
packet(SMSG_PLAYER_STORAGE_ITEMS,          0x01f0,  -1);
packet(SMSG_PLAYER_STORAGE_REMOVE,         0x00f6,   8);
packet(SMSG_PLAYER_STORAGE_STATUS,         0x00f2,   6);
packet(SMSG_PLAYER_UNEQUIP,                0x00ac,   7);
packet(SMSG_PLAYER_UPDATE_1,               0x01d8,  54);
packet(SMSG_PLAYER_UPDATE_2,               0x01d9,  53);
packet(SMSG_PLAYER_WARP,                   0x0091,  22);
packet(SMSG_PVP_MAP_MODE,                  0x0199,   4);
packet(SMSG_PVP_SET,                       0x019a,  14);
packet(SMSG_QUEST_PLAYER_VARS,             0x0215,  -1);
packet(SMSG_QUEST_SET_VAR,                 0x0214,   8);
packet(SMSG_SCRIPT_MESSAGE,                0x0229,  -1);
packet(SMSG_SERVER_PING,                   0x007f,   6);
packet(SMSG_SERVER_VERSION_RESPONSE,       0x7531,  10);
packet(SMSG_SKILL_CASTING,                 0x013e,  24);
packet(SMSG_SKILL_CAST_CANCEL,             0x01b9,   6);
packet(SMSG_SKILL_DAMAGE,                  0x01de,  33);
packet(SMSG_SKILL_FAILED,                  0x0110,  10);
packet(SMSG_SKILL_NO_DAMAGE,               0x011a,  15);
packet(SMSG_SOLVE_CHAR_NAME,               0x0194,  30);
packet(SMSG_TRADE_CANCEL,                  0x00ee,   2);
packet(SMSG_TRADE_COMPLETE,                0x00f0,   3);
packet(SMSG_TRADE_ITEM_ADD,                0x00e9,  19);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE,       0x01b1,   7);
packet(SMSG_TRADE_OK,                      0x00ec,   3);
packet(SMSG_TRADE_REQUEST,                 0x00e5,  26);
packet(SMSG_TRADE_RESPONSE,                0x00e7,   3);
packet(SMSG_UPDATE_HOST,                   0x0063,  -1);
packet(SMSG_WALK_RESPONSE,                 0x0087,  12);
packet(SMSG_WHISPER,                       0x0097,  -1);
packet(SMSG_WHISPER_RESPONSE,              0x0098,   3);
packet(SMSG_WHO_ANSWER,                    0x00c2,   6);
