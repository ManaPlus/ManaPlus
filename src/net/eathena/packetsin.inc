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

packet(SMSG_SERVER_VERSION_RESPONSE, 0x7531);

packet(SMSG_SERVER_PING,             0x007f); /**< Contains server tick */
packet(SMSG_CONNECTION_PROBLEM,      0x0081);
packet(SMSG_MAP_NOT_FOUND,           0x0840);

packet(SMSG_LOGIN_DATA,              0x0069);
packet(SMSG_LOGIN_ERROR,             0x006a);
packet(SMSG_LOGIN_ERROR2,            0x083e);
packet(SMSG_LOGIN_CODING_KEY,        0x01dc);

// unused
packet(SMSG_CHAR_LOGIN,              0x006b);
packet(SMSG_CHAR_LOGIN2,             0x082d);
packet(SMSG_CHAR_LOGIN_ERROR,        0x006c);
packet(SMSG_CHAR_CREATE_SUCCEEDED,   0x006d);
packet(SMSG_CHAR_PINCODE_STATUS,     0x08b9);
packet(SMSG_CHAR_CHECK_RENAME,       0x028e);
packet(SMSG_CHAR_RENAME,             0x0290);
packet(SMSG_CHAR_CHANGE_SLOT,        0x08d5);
packet(SMSG_CHAR_CAPTCHA_NOT_SUPPORTED, 0x07e9);
packet(SMSG_CHAR_PASSWORD_RESPONSE,  0x0062);

packet(SMSG_CHAR_CREATE_FAILED,      0x006e);
packet(SMSG_CHAR_DELETE_SUCCEEDED,   0x006f);
packet(SMSG_CHAR_DELETE_FAILED,      0x0070);
packet(SMSG_CHAR_DELETE2_ACK,        0x0828);
packet(SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK, 0x082a);
packet(SMSG_CHAR_DELETE2_CANCEL_ACK, 0x082c);
packet(SMSG_CHAR_MAP_INFO,           0x0071);
packet(SMSG_CHAR_CHARACTERS,         0x099d);
packet(SMSG_CHAR_BAN_CHAR_LIST,      0x020d);

packet(SMSG_CHAR_SWITCH_RESPONSE,    0x00b3);
packet(SMSG_CHANGE_MAP_SERVER,       0x0092);

packet(SMSG_MAP_LOGIN_SUCCESS,       0x0a18); /**< Contains starting location */
packet(SMSG_MAP_AUTH_REFUSE,         0x0074);
packet(SMSG_MAP_QUIT_RESPONSE,       0x018b);
packet(SMSG_PLAYER_STOP,             0x0088); /**< Stop walking, set position */
packet(SMSG_PLAYER_MOVE_TO_ATTACK,   0x0139);
packet(SMSG_PLAYER_STAT_UPDATE_1,    0x00b0);
packet(SMSG_PLAYER_STAT_UPDATE_2,    0x00b1);
packet(SMSG_PLAYER_STAT_UPDATE_3,    0x0141);
packet(SMSG_PLAYER_STAT_UPDATE_4,    0x00bc);
packet(SMSG_PLAYER_STAT_UPDATE_5,    0x00bd);
packet(SMSG_PLAYER_STAT_UPDATE_6,    0x00be);
packet(SMSG_PLAYER_GET_EXP,          0x07f6);
packet(SMSG_WHO_ANSWER,              0x00c2);
packet(SMSG_PLAYER_WARP,             0x0091);
packet(SMSG_PLAYER_INVENTORY,        0x0991);
packet(SMSG_PLAYER_INVENTORY_ADD,    0x0990);
packet(SMSG_PLAYER_INVENTORY_REMOVE, 0x00af);
packet(SMSG_PLAYER_INVENTORY_REMOVE2, 0x07fa);
packet(SMSG_PLAYER_INVENTORY_USE,    0x01c8);
packet(SMSG_PLAYER_IDENTIFY_LIST,    0x0177);
packet(SMSG_PLAYER_IDENTIFIED,       0x0179);
packet(SMSG_PLAYER_CART_ADD_OUTDATED, 0x0124);
packet(SMSG_PLAYER_CART_ADD,         0x01c5);
packet(SMSG_PLAYER_CART_ADD_ERROR,   0x012c);
packet(SMSG_PLAYER_CART_ITEMS,       0x0993);
packet(SMSG_PLAYER_CART_EQUIP,       0x0994);
packet(SMSG_PLAYER_CART_REMOVE,      0x0125);
packet(SMSG_PLAYER_ITEM_RENTAL_TIME, 0x0298);
packet(SMSG_PLAYER_ITEM_RENTAL_EXPIRED, 0x0299);
packet(SMSG_PLAYER_REFINE,           0x0188);
packet(SMSG_PLAYER_EQUIPMENT,        0x0992);
packet(SMSG_PLAYER_EQUIP,            0x0999);
packet(SMSG_PLAYER_UNEQUIP,          0x099a);
packet(SMSG_PLAYER_USE_CARD,         0x017b);
packet(SMSG_PLAYER_INSERT_CARD,      0x017d);
packet(SMSG_PLAYER_ATTACK_RANGE,     0x013a);
packet(SMSG_PLAYER_ARROW_EQUIP,      0x013c);
packet(SMSG_PLAYER_ARROW_MESSAGE,    0x013b);
packet(SMSG_PLAYER_SKILLS,           0x010f);
packet(SMSG_PLAYER_ADD_SKILL,        0x0111);
packet(SMSG_PLAYER_DELETE_SKILL,     0x0441);
packet(SMSG_PLAYER_UPDATE_SKILL,     0x07e1);
packet(SMSG_PLAYER_SKILL_UP,         0x010e);
packet(SMSG_PLAYER_HEAL,             0x013d);
packet(SMSG_PLAYER_SKILL_COOLDOWN,   0x043d);
packet(SMSG_PLAYER_SKILL_COOLDOWN_LIST_OUTDATED, 0x043e);
packet(SMSG_PLAYER_SKILL_COOLDOWN_LIST, 0x0985);
packet(SMSG_PLAYER_SKILL_MESSAGE,    0x0215);
packet(SMSG_PLAYER_NOTIFY_MAPINFO,   0x0189);
packet(SMSG_PLAYER_SKILL_PRODUCE_MIX_LIST, 0x018d);
packet(SMSG_PLAYER_SKILL_PRODUCE_EFFECT, 0x018f);
packet(SMSG_PLAYER_SKILL_AUTO_SPELLS, 0x01cd);
packet(SMSG_PLAYER_REPAIR_LIST,      0x01fc);
packet(SMSG_PLAYER_REPAIR_EFFECT,    0x01fe);
packet(SMSG_PLAYER_FAME_BLACKSMITH,  0x021b);
packet(SMSG_PLAYER_FAME_ALCHEMIST,   0x021c);
packet(SMSG_PLAYER_FAME_TAEKWON,     0x0224);
packet(SMSG_PLAYER_RANK_POINTS,      0x097e);
packet(SMSG_PLAYER_REFINE_LIST,      0x0221);
packet(SMSG_PLAYER_UPGRADE_MESSAGE,  0x0223);
packet(SMSG_PLAYER_COOKING_LIST,     0x025a);
packet(SMSG_PLAYER_READ_BOOK,        0x0294);
packet(SMSG_PLAYER_EQUIP_TICK_ACK,   0x02d9);
packet(SMSG_PLAYER_FAVORITE_ITEM,    0x0908);
packet(SMSG_SKILL_FAILED,            0x0110);
packet(SMSG_SKILL_DAMAGE_OUTDATED,   0x0114);
packet(SMSG_SKILL_DAMAGE,            0x01de);
packet(SMSG_SKILL_AUTO_CAST,         0x0147);
packet(SMSG_SKILL_SNAP,              0x08d2);
packet(SMSG_SKILL_WARP_POINT,        0x011c);
packet(SMSG_SKILL_MEMO_MESSAGE,      0x011e);
packet(SMSG_SKILL_UNIT_UPDATE,       0x01ac);
packet(SMSG_SKILL_ARROW_CREATE_LIST, 0x01ad);
packet(SMSG_SKILL_DEVOTION_EFFECT,   0x01cf);
packet(SMSG_ITEM_USE_RESPONSE,       0x00a8);
packet(SMSG_ITEM_VISIBLE,            0x009d); /**< An item is on the floor */
packet(SMSG_ITEM_VISIBLE2,           0x0b18);
packet(SMSG_GRAFFITI_VISIBLE,        0x01c9);
packet(SMSG_ITEM_DROPPED,            0x084b); /**< An item is dropped */
packet(SMSG_ITEM_DROPPED2,           0x0b19);
packet(SMSG_ITEM_MVP_DROPPED,        0x07fd);
packet(SMSG_ITEM_REMOVE,             0x00a1); /**< An item disappers */
packet(SMSG_BEING_VISIBLE,           0x0915);
packet(SMSG_BEING_FAKE_NAME,         0x0078);
packet(SMSG_BEING_MOVE,              0x0914); /**< A nearby monster moves */
packet(SMSG_BEING_SPAWN,             0x090f); /**< A being spawns nearby */
packet(SMSG_BEING_MOVE2,             0x0086); /**< New eAthena being moves */
packet(SMSG_BEING_MOVE3,             0x0b04);
packet(SMSG_BEING_REMOVE,            0x0080);
packet(SMSG_BEING_REMOVE_SKILL,      0x0120);
packet(SMSG_BEING_CHANGE_LOOKS_OUTDATED, 0x00c3);
packet(SMSG_BEING_CHANGE_LOOKS2,     0x01d7);
packet(SMSG_BEING_CHANGE_LOOKS_CARDS, 0x0b17);
packet(SMSG_BEING_SELFEFFECT,        0x019b);
packet(SMSG_BEING_SPECIAL_EFFECT,    0x01f3);
packet(SMSG_BEING_SPECIAL_EFFECT_NUM, 0x0284);
packet(SMSG_BEING_SOUND_EFFECT,      0x01d3);
packet(SMSG_BEING_EMOTION,           0x00c0);
packet(SMSG_BEING_ACTION,            0x008a); /**< Attack, sit, stand up, ...*/
packet(SMSG_BEING_ACTION2,           0x02e1);
packet(SMSG_BEING_CHAT,              0x008d); /**< A being talks */
packet(SMSG_BEING_SLIDE,             0x01ff);
packet(SMSG_BEING_CHARM,             0x08cf);
packet(SMSG_COLOR_MESSAGE,           0x02c1);
packet(SMSG_FORMAT_MESSAGE,          0x0291);
packet(SMSG_FORMAT_MESSAGE_NUMBER,   0x07e2);
packet(SMSG_FORMAT_MESSAGE_SKILL,    0x07e6);
packet(SMSG_BEING_NAME_RESPONSE,     0x0095); /**< Has to be requested */
packet(SMSG_BEING_NAME_RESPONSE2,    0x0b01); /**< Has to be requested */
packet(SMSG_BEING_CHANGE_DIRECTION,  0x009c);
packet(SMSG_BEING_RESURRECT,         0x0148);
packet(SMSG_BEING_STAT_UPDATE_1,     0x01ab);
packet(SMSG_BEING_ATTRS,             0x0b0a);
packet(SMSG_BEING_FONT,              0x02ef);
packet(SMSG_BEING_MILLENIUM_SHIELD,  0x0440);
packet(SMSG_BEING_VIEW_EQUIPMENT,    0x0997);
packet(SMSG_IGNORE_NICK_ACK,         0x00d1);
packet(SMSG_STARS_KILL,              0x020e);
packet(SMSG_GLADIATOR_FEEL_REQUEST,  0x0253);
packet(SMSG_BOSS_MAP_INFO,           0x0293);
packet(SMSG_ITEM_DAMAGED,            0x02bb);
packet(SMSG_AUTOSHADOW_SPELL_LIST,   0x0442);
packet(SMSG_BIND_ITEM,               0x02d3);

packet(SMSG_PLAYER_STATUS_CHANGE_OUTDATED, 0x0119);
packet(SMSG_PLAYER_STATUS_CHANGE,    0x0229);
packet(SMSG_PLAYER_STATUS_CHANGE2,   0x028a);
packet(SMSG_PLAYER_STATUS_CHANGE_NO_TICK, 0x0196);
packet(SMSG_PLAYER_GUILD_PARTY_INFO, 0x0195);
packet(SMSG_BEING_STATUS_CHANGE,     0x0983);
packet(SMSG_BEING_STATUS_CHANGE2,    0x043f);

packet(SMSG_NPC_MESSAGE,             0x00b4);
packet(SMSG_NPC_NEXT,                0x00b5);
packet(SMSG_NPC_CLOSE,               0x00b6);
packet(SMSG_NPC_CLOSE_TIMEOUT,       0x08d6);
packet(SMSG_NPC_CHOICE,              0x00b7); /**< Display a choice */
packet(SMSG_NPC_BUY_SELL_CHOICE,     0x00c4);
packet(SMSG_NPC_BUY,                 0x00c6);
packet(SMSG_NPC_SELL,                0x00c7);
packet(SMSG_NPC_BUY_RESPONSE,        0x00ca);
packet(SMSG_NPC_SELL_RESPONSE,       0x00cb);
packet(SMSG_NPC_INT_INPUT,           0x0142); /**< Integer input */
packet(SMSG_NPC_STR_INPUT,           0x01d4); /**< String input */
packet(SMSG_NPC_SHOW_PROGRESS_BAR,   0x02f0);
packet(SMSG_NPC_PROGRESS_BAR_ABORT,  0x02f2);
packet(SMSG_NPC_CHANGETITLE,         0x0b06);
packet(SMSG_NPC_MARKET_OPEN,         0x09d5);
packet(SMSG_NPC_MARKET_BUY_ACK,      0x09d7);
packet(SMSG_NPC_CASH_SHOP_OPEN,      0x0287);
packet(SMSG_NPC_CASH_BUY_ACK,        0x0289);
packet(SMSG_NPC_CASH_POINTS,         0x0845);
packet(SMSG_NPC_CASH_BUY,            0x0849);
packet(SMSG_NPC_CASH_TAB_PRICE_LIST, 0x08c0);
packet(SMSG_NPC_CASH_SCHEDULE,       0x08ca);
packet(SMSG_NPC_AREA,                0x0b0b);
packet(SMSG_NPC_SHOW_DIGIT,          0x01b1);

packet(SMSG_PLAYER_CHAT,             0x008e); /**< Player talks */
packet(SMSG_WHISPER,                 0x0097); /**< Whisper Recieved */
packet(SMSG_WHISPER_RESPONSE,        0x09df);
packet(SMSG_GM_CHAT,                 0x009a); /**< GM announce */
packet(SMSG_GM_CHAT2,                0x01c3);
packet(SMSG_WALK_RESPONSE,           0x0087);

packet(SMSG_BUYINGSTORE_OPEN,        0x0810);
packet(SMSG_BUYINGSTORE_CREATE_FAILED, 0x0812);
packet(SMSG_BUYINGSTORE_OWN_ITEMS,   0x0813);
packet(SMSG_BUYINGSTORE_SHOW_BOARD,  0x0814);
packet(SMSG_BUYINGSTORE_HIDE_BOARD,  0x0816);
packet(SMSG_BUYINGSTORE_ITEMS_LIST,  0x0818);
packet(SMSG_BUYINGSTORE_SELL_FAILED, 0x081a);
packet(SMSG_BUYINGSTORE_REPORT,      0x081b);
packet(SMSG_BUYINGSTORE_DELETE_ITEM, 0x081c);
packet(SMSG_BUYINGSTORE_SELLER_SELL_FAILED, 0x0824);

// Receiving a request to trade
packet(SMSG_TRADE_REQUEST_OUTDATED,  0x00e5);
packet(SMSG_TRADE_REQUEST,           0x01f4);
packet(SMSG_TRADE_RESPONSE_OUTDATED, 0x00e7);
packet(SMSG_TRADE_RESPONSE,          0x01f5);
packet(SMSG_TRADE_ITEM_ADD,          0x080f);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE, 0x00ea);
packet(SMSG_TRADE_OK,                0x00ec);
packet(SMSG_TRADE_CANCEL,            0x00ee);
packet(SMSG_TRADE_COMPLETE,          0x00f0);
packet(SMSG_TRADE_UNDO,              0x00f1);

packet(SMSG_PARTY_CREATE,            0x00fa);
packet(SMSG_PARTY_INFO,              0x00fb);
packet(SMSG_PARTY_INVITE_RESPONSE_OUTDATED, 0x00fd);
packet(SMSG_PARTY_INVITE_RESPONSE,   0x02c5);
packet(SMSG_PARTY_INVITED,           0x02c6);
packet(SMSG_PARTY_SETTINGS,          0x07d8);
packet(SMSG_PARTY_LEAVE,             0x0105);
packet(SMSG_PARTY_UPDATE_COORDS,     0x0107);
packet(SMSG_PARTY_MESSAGE,           0x0109);
packet(SMSG_PARTY_INVITATION_STATS,  0x02c9);
packet(SMSG_PARTY_MEMBER_INFO,       0x01e9);
packet(SMSG_PARTY_ITEM_PICKUP,       0x02b8);
packet(SMSG_PARTY_LEADER,            0x07fc);

packet(SMSG_PLAYER_STORAGE_ITEMS,    0x0995); /* Item list for storage */
packet(SMSG_PLAYER_STORAGE_EQUIP,    0x0996); /* Equipment list for storage */
packet(SMSG_PLAYER_STORAGE_STATUS,   0x00f2); /* Slots used and total slots */
packet(SMSG_PLAYER_STORAGE_ADD_OUTDATED, 0x00f4);
packet(SMSG_PLAYER_STORAGE_ADD,      0x01c4); /* Add item/equip to storage */
// Remove item/equip from storage
packet(SMSG_PLAYER_STORAGE_REMOVE,   0x00f6);
packet(SMSG_PLAYER_STORAGE_CLOSE,    0x00f8); /* Storage access closed */
packet(SMSG_PLAYER_STORAGE_PASSWORD, 0x023a);
packet(SMSG_PLAYER_STORAGE_PASSWORD_RESULT, 0x023c);

packet(SMSG_ADMIN_KICK_ACK,          0x00cd);
packet(SMSG_ADMIN_GET_LOGIN_ACK,     0x01e0);
packet(SMSG_ADMIN_SET_TILE_TYPE,     0x0192);
packet(SMSG_ADMIN_ACCOUNT_STATS,     0x0214);

packet(SMSG_GUILD_CREATE_RESPONSE,   0x0167);
packet(SMSG_GUILD_POSITION_INFO,     0x016c);
packet(SMSG_GUILD_MEMBER_LOGIN,      0x01f2);
packet(SMSG_GUILD_MASTER_OR_MEMBER,  0x014e);
packet(SMSG_GUILD_BASIC_INFO,        0x01b6);
packet(SMSG_GUILD_ALIANCE_INFO,      0x014c);
packet(SMSG_GUILD_MEMBER_LIST,       0x0154);
packet(SMSG_GUILD_POS_NAME_LIST,     0x0166);
packet(SMSG_GUILD_POS_INFO_LIST,     0x0160);
packet(SMSG_GUILD_POSITION_CHANGED,  0x0174);
packet(SMSG_GUILD_MEMBER_POS_CHANGE, 0x0156);
packet(SMSG_GUILD_EMBLEM_DATA,       0x0152);
packet(SMSG_GUILD_EMBLEM,            0x01b4);
packet(SMSG_GUILD_SKILL_INFO,        0x0162);
packet(SMSG_GUILD_NOTICE,            0x016f);
packet(SMSG_GUILD_INVITE,            0x016a);
packet(SMSG_GUILD_INVITE_ACK,        0x0169);
packet(SMSG_GUILD_LEAVE,             0x015a);
packet(SMSG_GUILD_EXPULSION,         0x0839);
packet(SMSG_GUILD_EXPULSION_LIST,    0x0163);
packet(SMSG_GUILD_MESSAGE,           0x017f);
packet(SMSG_GUILD_SKILL_UP,          0x010e);
packet(SMSG_GUILD_REQ_ALLIANCE,      0x0171);
packet(SMSG_GUILD_REQ_ALLIANCE_ACK,  0x0173);
packet(SMSG_GUILD_DEL_ALLIANCE,      0x0184);
packet(SMSG_GUILD_OPPOSITION_ACK,    0x0181);
packet(SMSG_GUILD_BROKEN,            0x015e);
packet(SMSG_GUILD_UPDATE_COORDS,     0x01eb);
packet(SMSG_GUILD_ALLIANCE_ADDED_DISABLED, 0x0185);

packet(SMSG_QUEST_ADD,               0x02b3);
packet(SMSG_QUEST_LIST_OUTDATED,     0x02b1);
packet(SMSG_QUEST_LIST,              0x097a);
packet(SMSG_QUEST_LIST_OBJECTIVES,   0x02b2);
packet(SMSG_QUEST_UPDATE_OBJECTIVES, 0x02b5);
packet(SMSG_QUEST_REMOVE,            0x02b4);
packet(SMSG_QUEST_ACTIVATE,          0x02b7);
packet(SMSG_QUEST_NPC_EFFECT,        0x0446);

packet(SMSG_MVP_ITEM,                0x010a);
packet(SMSG_MVP_EXP,                 0x010b);
packet(SMSG_MVP_EFFECT,              0x010c);
packet(SMSG_MVP_NO_ITEM,             0x010d);

packet(SMSG_RANKS_LIST,              0x097d);
packet(SMSG_BLACKSMITH_RANKS_LIST,   0x0219);
packet(SMSG_ALCHEMIST_RANKS_LIST,    0x021a);
packet(SMSG_TAEKWON_RANKS_LIST,      0x0226);
packet(SMSG_PK_RANKS_LIST,           0x0238);
packet(SMSG_MONSTER_HP,              0x0977);
packet(SMSG_MONSTER_INFO,            0x018c);
packet(SMSG_PLAYER_HP,               0x080e);
packet(SMSG_PVP_INFO,                0x0210);
packet(SMSG_SCRIPT_MESSAGE,          0x08b3);

packet(SMSG_CHAT_IGNORE_LIST,        0x00d4);
packet(SMSG_CHAT_ROOM_CREATE_ACK,    0x00d6);
packet(SMSG_CHAT_DISPLAY,            0x00d7);
packet(SMSG_CHAT_ROOM_DESTROY,       0x00d8);
packet(SMSG_CHAT_ROOM_JOIN_FAILED,   0x00da);
packet(SMSG_CHAT_ROOM_JOIN_ACK,      0x00db);
packet(SMSG_CHAT_ROOM_ADD_MEMBER,    0x00dc);
packet(SMSG_CHAT_ROOM_LEAVE,         0x00dd);
packet(SMSG_CHAT_ROOM_SETTINGS,      0x00df);
packet(SMSG_CHAT_ROOM_ROLE_CHANGE,   0x00e1);
packet(SMSG_CHAT_JOIN_CHANNEL,       0x0b08);
packet(SMSG_CHAT_TALKIE_BOX,         0x0191);

packet(SMSG_MAIL_MAILS_LIST,         0x0240);
packet(SMSG_MAIL_READ_MAIL,          0x0242);
packet(SMSG_MAIL_GET_ATTACHMENT,     0x0245);
packet(SMSG_MAIL_SEND_MAIL_ACK,      0x0249);
packet(SMSG_MAIL_NEW_MAIL,           0x024a);
packet(SMSG_MAIL_SET_ATTACHMENT_ACK, 0x0255);
packet(SMSG_MAIL_DELETE_MAIL_ACK,    0x0257);
packet(SMSG_MAIL_OPEN_WINDOW,        0x0260);
packet(SMSG_MAIL_RETURN,             0x0274);

packet(SMSG_FAMILY_ASK_FOR_CHILD,    0x01f6);
packet(SMSG_FAMILY_CALL_PARTNER,     0x01e6);
packet(SMSG_FAMILY_DIVORCED,         0x0205);
packet(SMSG_FAMILY_ASK_FOR_CHILD_REPLY, 0x0216);

packet(SMSG_PET_MESSAGE,             0x01aa);
packet(SMSG_PET_ROULETTE,            0x01a0);
packet(SMSG_PET_EGGS_LIST,           0x01a6);
packet(SMSG_PET_DATA,                0x01a4);
packet(SMSG_PET_STATUS,              0x01a2);
packet(SMSG_PET_FOOD,                0x01a3);
packet(SMSG_PET_CATCH_PROCESS,       0x019e);

packet(SMSG_BANK_STATUS,             0x09a6);
packet(SMSG_BANK_DEPOSIT,            0x09a8);
packet(SMSG_BANK_WITHDRAW,           0x09aa);

packet(SMSG_SOLVE_CHAR_NAME,         0x0194);
packet(SMSG_SKILL_CASTING,           0x07fb);
packet(SMSG_SKILL_CAST_CANCEL,       0x01b9);
packet(SMSG_SKILL_NO_DAMAGE,         0x011a);
packet(SMSG_SKILL_GROUND_NO_DAMAGE,  0x0117);
packet(SMSG_SKILL_GROUND_DAMAGE_UNUSED, 0x0115);
packet(SMSG_SKILL_ENTRY,             0x09ca);
packet(SMSG_SKILL_ITEM_LIST_WINDOW,  0x07e3);

packet(SMSG_PVP_MAP_MODE,            0x0199);
packet(SMSG_PVP_SET,                 0x019a);
packet(SMSG_MAP_TYPE_PROPERTY2,      0x099b);
packet(SMSG_MAP_TYPE,                0x01d6);
packet(SMSG_IGNORE_ALL_RESPONSE,     0x00d2);

packet(SMSG_NPC_COMMAND,             0x0B00);

packet(SMSG_MAP_ACCOUNT_ID,          0x0283);
packet(SMSG_PLAYER_SHORTCUTS,        0x0a00);
packet(SMSG_PLAYER_SHOW_EQUIP,       0x02da);
packet(SMSG_NPC_CUTIN,               0x01b3);
packet(SMSG_NPC_VIEWPOINT,           0x0144);

packet(SMSG_MERCENARY_UPDATE,        0x02a2);
packet(SMSG_MERCENARY_INFO,          0x029b);
packet(SMSG_MERCENARY_SKILLS,        0x029d);

packet(SMSG_HOMUNCULUS_SKILLS,       0x0235);
packet(SMSG_HOMUNCULUS_DATA,         0x0230);
packet(SMSG_HOMUNCULUS_INFO,         0x022e);
packet(SMSG_HOMUNCULUS_SKILL_UP,     0x0239);
packet(SMSG_HOMUNCULUS_FOOD,         0x022f);

packet(SMSG_FRIENDS_PLAYER_ONLINE,   0x0206);
packet(SMSG_FRIENDS_LIST,            0x0201);
packet(SMSG_FRIENDS_REQUEST,         0x0207);
packet(SMSG_FRIENDS_REQUEST_ACK,     0x0209);
packet(SMSG_FRIENDS_DELETE_PLAYER,   0x020a);

packet(SMSG_AUCTION_OPEN_WINDOW,     0x025f);
packet(SMSG_AUCTION_RESULTS,         0x0252);
packet(SMSG_AUCTION_SET_ITEM,        0x0256);
packet(SMSG_AUCTION_MESSAGE,         0x0250);
packet(SMSG_AUCTION_CLOSE,           0x025d);

packet(SMSG_VENDING_OPEN_REQ,        0x012d);
packet(SMSG_VENDING_SHOW_BOARD,      0x0131);
packet(SMSG_VENDING_HIDE_BOARD,      0x0132);
packet(SMSG_VENDING_ITEMS_LIST,      0x0800);
packet(SMSG_VENDING_BUY_ACK,         0x0135);
packet(SMSG_VENDING_OPEN,            0x0136);
packet(SMSG_VENDING_OPEN_STATUS,     0x0a28);
packet(SMSG_VENDING_REPORT,          0x0137);

packet(SMSG_ELEMENTAL_UPDATE_STATUS, 0x081e);
packet(SMSG_ELEMENTAL_INFO,          0x081d);

packet(SMSG_UPDATE_HOST,             0x0063);

packet(SMSG_MAP_MASK,                0x0b02);

packet(SMSG_MOB_INFO,                0x0b03);

packet(SMSG_MAP_MUSIC,               0x0b05);

packet(SMSG_PLAYER_CLIENT_COMMAND,   0x0b16);

packet(SMSG_CART_INFO,               0x0121);
packet(SMSG_CART_REMOVE,             0x012b);

packet(SMSG_SEARCHSTORE_SEARCH_ACK,  0x0836);
packet(SMSG_SEARCHSTORE_SEARCH_FAILED, 0x0837);
packet(SMSG_SEARCHSTORE_OPEN,        0x083a);
packet(SMSG_SEARCHSTORE_CLICK_ACK,   0x083d);

packet(SMSG_ONLINE_LIST,             0x0b10);

packet(SMSG_MANNER_MESSAGE,          0x014a);
packet(SMSG_CHAT_SILENCE,            0x014b);

packet(SMSG_CLASS_CHANGE,            0x01b0);

packet(SMSG_INSTANCE_START,          0x02cb);
packet(SMSG_INSTANCE_CREATE,         0x02cc);
packet(SMSG_INSTANCE_INFO,           0x02cd);
packet(SMSG_INSTANCE_DELETE,         0x02ce);

packet(SMSG_BATTLE_CHAT_MESSAGE,     0x02dc);
packet(SMSG_BATTLE_EMBLEM,           0x02dd);
packet(SMSG_BATTLE_UPDATE_SCORE,     0x02de);
packet(SMSG_BATTLE_UPDATE_COORDS,    0x02df);
packet(SMSG_BATTLE_PLAY,             0x07fe);
packet(SMSG_BATTLE_QUEUE_ACK,        0x08d8);
packet(SMSG_BATTLE_BEGINS,           0x08df);
packet(SMSG_BATTLE_NOTICE_DELETE,    0x08db);
packet(SMSG_BATTLE_JOINED,           0x08d9);

packet(SMSG_ROULETTE_OPEN_ACK,       0x0a1a);
packet(SMSG_ROULETTE_INFO_ACK_TYPE,  0x0a1c);
packet(SMSG_ROULETTE_RECV_ITEM_ACK,  0x0a22);
packet(SMSG_ROULETTE_GENERATE_ACK_TYPE, 0x0a20);

packet(SMSG_MARRIAGE_PROPOSAL_OUTDATED, 0x01e2);
packet(SMSG_MARRIAGE_PROCESS_OUTDATED, 0x01e4);

packet(SMSG_SPIRIT_BALLS,            0x01d0);
packet(SMSG_SPIRIT_BALL_SINGLE,      0x01e1);
packet(SMSG_BLADE_STOP,              0x01d1);
packet(SMSG_COMBO_DELAY,             0x01d2);
packet(SMSG_WEDDING_EFFECT,          0x01ea);
