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

packet(SMSG_ADMIN_KICK_ACK,                0x00cd,   6, &Ea::AdminRecv::processKickAck);
packet(SMSG_BEING_ACTION,                  0x008a,  29, &Ea::BeingRecv::processBeingAction);
packet(SMSG_BEING_CHANGE_DIRECTION,        0x009c,   9, &BeingRecv::processBeingChangeDirection);
packet(SMSG_BEING_CHANGE_LOOKS,            0x00c3,   8, &BeingRecv::processBeingChangeLook);
packet(SMSG_BEING_CHANGE_LOOKS2,           0x01d7,  11, &BeingRecv::processBeingChangeLook2);
packet(SMSG_BEING_CHAT,                    0x008d,  -1, &ChatRecv::processBeingChat);
packet(SMSG_BEING_EMOTION,                 0x00c0,   7, &Ea::BeingRecv::processBeingEmotion);
packet(SMSG_BEING_IP_RESPONSE,             0x020c,  10, &BeingRecv::processIpResponse);
packet(SMSG_BEING_MOVE,                    0x007b,  60, &BeingRecv::processBeingMove);
packet(SMSG_BEING_MOVE2,                   0x0086,  16, &BeingRecv::processBeingMove2);
packet(SMSG_BEING_MOVE3,                   0x0225,  -1, &Ea::BeingRecv::processBeingMove3);
packet(SMSG_BEING_NAME_RESPONSE,           0x0095,  30, &Ea::BeingRecv::processNameResponse);
packet(SMSG_BEING_REMOVE,                  0x0080,   7, &Ea::BeingRecv::processBeingRemove);
packet(SMSG_BEING_RESURRECT,               0x0148,   8, &BeingRecv::processBeingResurrect);
packet(SMSG_BEING_SELFEFFECT,              0x019b,  10, &BeingRecv::processBeingSelfEffect);
packet(SMSG_BEING_SPAWN,                   0x007c,  41, &BeingRecv::processBeingSpawn);
packet(SMSG_BEING_STATUS_CHANGE,           0x0196,   9, &BeingRecv::processBeingStatusChange);
packet(SMSG_BEING_VISIBLE,                 0x0078,  54, &BeingRecv::processBeingVisible);
packet(SMSG_CHANGE_MAP_SERVER,             0x0092,  28, &CharServerRecv::processChangeMapServer);
packet(SMSG_CHAR_CREATE_FAILED,            0x006e,   3, &Ea::CharServerRecv::processCharCreateFailed);
packet(SMSG_CHAR_CREATE_SUCCEEDED,         0x006d, 108, &CharServerRecv::processCharCreate);
packet(SMSG_CHAR_DELETE_FAILED,            0x0070,   3, &CharServerRecv::processCharDeleteFailed);
packet(SMSG_CHAR_DELETE_SUCCEEDED,         0x006f,   2, &Ea::CharServerRecv::processCharDelete);
packet(SMSG_CHAR_LOGIN,                    0x006b,  -1, &CharServerRecv::processCharLogin);
packet(SMSG_CHAR_LOGIN_ERROR,              0x006c,   3, &Ea::CharServerRecv::processCharLoginError);
packet(SMSG_CHAR_MAP_INFO,                 0x0071,  28, &CharServerRecv::processCharMapInfo);
packet(SMSG_CHAR_PASSWORD_RESPONSE,        0x0062,   3, &LoginRecv::processCharPasswordResponse);
packet(SMSG_CHAR_SWITCH_RESPONSE,          0x00b3,   3, &Ea::GameRecv::processCharSwitchResponse);
packet(SMSG_CONNECTION_PROBLEM,            0x0081,   3, &GeneralRecv::processConnectionProblem);
packet(SMSG_GM_CHAT,                       0x009a,  -1, &ChatRecv::processGmChat);
packet(SMSG_GUILD_ALIANCE_INFO,            0x014c,  -1, &Ea::GuildRecv::processGuildAlianceInfo);
packet(SMSG_GUILD_BASIC_INFO,              0x01b6, 114, &Ea::GuildRecv::processGuildBasicInfo);
packet(SMSG_GUILD_BROKEN,                  0x015e,   6, &Ea::GuildRecv::processGuildBroken);
packet(SMSG_GUILD_CREATE_RESPONSE,         0x0167,   3, &Ea::GuildRecv::processGuildCreateResponse);
packet(SMSG_GUILD_DEL_ALLIANCE,            0x0184,  10, &Ea::GuildRecv::processGuildDelAlliance);
packet(SMSG_GUILD_EMBLEM_DATA,             0x0152,  -1, &Ea::GuildRecv::processGuildEmblemData);
packet(SMSG_GUILD_EXPULSION,               0x015c,  90, &GuildRecv::processGuildExpulsion);
packet(SMSG_GUILD_EXPULSION_LIST,          0x0163,  -1, &GuildRecv::processGuildExpulsionList);
packet(SMSG_GUILD_INVITE,                  0x016a,  30, &Ea::GuildRecv::processGuildInvite);
packet(SMSG_GUILD_INVITE_ACK,              0x0169,   3, &Ea::GuildRecv::processGuildInviteAck);
packet(SMSG_GUILD_LEAVE,                   0x015a,  66, &Ea::GuildRecv::processGuildLeave);
packet(SMSG_GUILD_MASTER_OR_MEMBER,        0x014e,   6, &Ea::GuildRecv::processGuildMasterOrMember);
packet(SMSG_GUILD_MEMBER_LIST,             0x0154,  -1, &Ea::GuildRecv::processGuildMemberList);
packet(SMSG_GUILD_MEMBER_LOGIN,            0x016d,  14, &GuildRecv::processGuildMemberLogin);
packet(SMSG_GUILD_MEMBER_POS_CHANGE,       0x0156,  -1, &Ea::GuildRecv::processGuildMemberPosChange);
packet(SMSG_GUILD_MESSAGE,                 0x017f,  -1, &Ea::GuildRecv::processGuildMessage);
packet(SMSG_GUILD_NOTICE,                  0x016f, 182, &Ea::GuildRecv::processGuildNotice);
packet(SMSG_GUILD_OPPOSITION_ACK,          0x0181,   3, &Ea::GuildRecv::processGuildOppositionAck);
packet(SMSG_GUILD_POSITION_CHANGED,        0x0174,  -1, &Ea::GuildRecv::processGuildPositionChanged);
packet(SMSG_GUILD_POSITION_INFO,           0x016c,  43, &GuildRecv::processGuildPositionInfo);
packet(SMSG_GUILD_POS_INFO_LIST,           0x0160,  -1, &Ea::GuildRecv::processGuildPosInfoList);
packet(SMSG_GUILD_POS_NAME_LIST,           0x0166,  -1, &Ea::GuildRecv::processGuildPosNameList);
packet(SMSG_GUILD_REQ_ALLIANCE,            0x0171,  30, &Ea::GuildRecv::processGuildReqAlliance);
packet(SMSG_GUILD_REQ_ALLIANCE_ACK,        0x0173,   3, &Ea::GuildRecv::processGuildReqAllianceAck);
packet(SMSG_GUILD_SKILL_INFO,              0x0162,  -1, &Ea::GuildRecv::processGuildSkillInfo);
packet(SMSG_GUILD_SKILL_UP,                0x010e,  11, &Ea::GuildRecv::processGuildSkillUp);
packet(SMSG_IGNORE_ALL_RESPONSE,           0x00d2,   4, &Ea::ChatRecv::processIgnoreAllResponse);
packet(SMSG_ITEM_DROPPED,                  0x009e,  17, &ItemRecv::processItemDropped);
packet(SMSG_ITEM_REMOVE,                   0x00a1,   6, &Ea::ItemRecv::processItemRemove);
packet(SMSG_ITEM_USE_RESPONSE,             0x00a8,   7, &Ea::InventoryRecv::processItemUseResponse);
packet(SMSG_ITEM_VISIBLE,                  0x009d,  17, &ItemRecv::processItemVisible);
packet(SMSG_LOGIN_DATA,                    0x0069,  -1, &Ea::LoginRecv::processLoginData);
packet(SMSG_LOGIN_ERROR,                   0x006a,  23, &Ea::LoginRecv::processLoginError);
packet(SMSG_MAP_LOGIN_SUCCESS,             0x0073,  11, &GameRecv::processMapLogin);
packet(SMSG_MAP_MUSIC,                     0x0227,  -1, &Ea::PlayerRecv::processMapMusic);
packet(SMSG_MAP_QUIT_RESPONSE,             0x018b,   4, &Ea::GameRecv::processMapQuitResponse);
packet(SMSG_MVP_EFFECT,                    0x010c,   6, &Ea::ChatRecv::processMVPEffect);
packet(SMSG_NPC_BUY,                       0x00c6,  -1, &BuySellRecv::processNpcBuy);
packet(SMSG_NPC_BUY_RESPONSE,              0x00ca,   3, &Ea::BuySellRecv::processNpcBuyResponse);
packet(SMSG_NPC_BUY_SELL_CHOICE,           0x00c4,   6, &Ea::BuySellRecv::processNpcBuySellChoice);
packet(SMSG_NPC_CHANGETITLE,               0x0228,  -1, &Ea::NpcRecv::processChangeTitle);
packet(SMSG_NPC_CHOICE,                    0x00b7,  -1, &Ea::NpcRecv::processNpcChoice);
packet(SMSG_NPC_CLOSE,                     0x00b6,   6, &Ea::NpcRecv::processNpcClose);
packet(SMSG_NPC_COMMAND,                   0x0212,  16, &Ea::NpcRecv::processNpcCommand);
packet(SMSG_NPC_INT_INPUT,                 0x0142,   6, &Ea::NpcRecv::processNpcIntInput);
packet(SMSG_NPC_MESSAGE,                   0x00b4,  -1, &Ea::NpcRecv::processNpcMessage);
packet(SMSG_NPC_NEXT,                      0x00b5,   6, &Ea::NpcRecv::processNpcNext);
packet(SMSG_NPC_SELL,                      0x00c7,  -1, &Ea::BuySellRecv::processNpcSell);
packet(SMSG_NPC_SELL_RESPONSE,             0x00cb,   3, &BuySellRecv::processNpcSellResponse);
packet(SMSG_NPC_STR_INPUT,                 0x01d4,   6, &Ea::NpcRecv::processNpcStrInput);
packet(SMSG_PARTY_CREATE,                  0x00fa,   3, &Ea::PartyRecv::processPartyCreate);
packet(SMSG_PARTY_INFO,                    0x00fb,  -1, &PartyRecv::processPartyInfo);
packet(SMSG_PARTY_INVITED,                 0x00fe,  30, &PartyRecv::processPartyInvited);
packet(SMSG_PARTY_INVITE_RESPONSE,         0x00fd,  27, &PartyRecv::processPartyInviteResponse);
packet(SMSG_PARTY_LEAVE,                   0x0105,  31, &Ea::PartyRecv::processPartyLeave);
packet(SMSG_PARTY_MESSAGE,                 0x0109,  -1, &PartyRecv::processPartyMessage);
packet(SMSG_PARTY_MOVE,                    0x0104,  79, &PartyRecv::processPartyMove);
packet(SMSG_PARTY_SETTINGS,                0x0101,   6, &PartyRecv::processPartySettings);
packet(SMSG_PARTY_UPDATE_COORDS,           0x0107,  10, &Ea::PartyRecv::processPartyUpdateCoords);
packet(SMSG_PARTY_UPDATE_HP,               0x0106,  10, &PartyRecv::processPartyUpdateHp);
packet(SMSG_PLAYER_ARROW_EQUIP,            0x013c,   4, &Ea::InventoryRecv::processPlayerArrowEquip);
packet(SMSG_PLAYER_ARROW_MESSAGE,          0x013b,   4, &Ea::PlayerRecv::processPlayerArrowMessage);
packet(SMSG_PLAYER_ATTACK_RANGE,           0x013a,   4, &Ea::InventoryRecv::processPlayerAttackRange);
packet(SMSG_PLAYER_CHAT,                   0x008e,  -1, &ChatRecv::processChat);
packet(SMSG_PLAYER_EQUIP,                  0x00aa,   7, &InventoryRecv::processPlayerEquip);
packet(SMSG_PLAYER_EQUIPMENT,              0x00a4,  -1, &InventoryRecv::processPlayerEquipment);
packet(SMSG_PLAYER_GUILD_PARTY_INFO,       0x0195, 102, &BeingRecv::processPlayerGuilPartyInfo);
packet(SMSG_PLAYER_INVENTORY,              0x01ee,  -1, &InventoryRecv::processPlayerInventory);
packet(SMSG_PLAYER_INVENTORY_ADD,          0x00a0,  23, &InventoryRecv::processPlayerInventoryAdd);
packet(SMSG_PLAYER_INVENTORY_REMOVE,       0x00af,   6, &InventoryRecv::processPlayerInventoryRemove);
packet(SMSG_PLAYER_INVENTORY_USE,          0x01c8,  13, &Ea::InventoryRecv::processPlayerInventoryUse);
packet(SMSG_PLAYER_MOVE,                   0x01da,  60, &BeingRecv::processPlayerMove);
packet(SMSG_PLAYER_MOVE_TO_ATTACK,         0x0139,  16, &Ea::BeingRecv::processPlayerMoveToAttack);
packet(SMSG_PLAYER_SKILLS,                 0x010f,  -1, &SkillRecv::processPlayerSkills);
packet(SMSG_PLAYER_SKILL_UP,               0x010e,  11, &Ea::SkillRecv::processPlayerSkillUp);
packet(SMSG_PLAYER_STATUS_CHANGE,          0x0119,  13, &BeingRecv::processPlaterStatusChange);
packet(SMSG_PLAYER_STAT_UPDATE_1,          0x00b0,   8, &Ea::PlayerRecv::processPlayerStatUpdate1);
packet(SMSG_PLAYER_STAT_UPDATE_2,          0x00b1,   8, &Ea::PlayerRecv::processPlayerStatUpdate2);
packet(SMSG_PLAYER_STAT_UPDATE_3,          0x0141,  14, &Ea::PlayerRecv::processPlayerStatUpdate3);
packet(SMSG_PLAYER_STAT_UPDATE_4,          0x00bc,   6, &Ea::PlayerRecv::processPlayerStatUpdate4);
packet(SMSG_PLAYER_STAT_UPDATE_5,          0x00bd,  44, &PlayerRecv::processPlayerStatUpdate5);
packet(SMSG_PLAYER_STAT_UPDATE_6,          0x00be,   5, &Ea::PlayerRecv::processPlayerStatUpdate6);
packet(SMSG_PLAYER_STOP,                   0x0088,  10, &Ea::BeingRecv::processPlayerStop);
packet(SMSG_PLAYER_STORAGE_ADD,            0x00f4,  21, &InventoryRecv::processPlayerStorageAdd);
packet(SMSG_PLAYER_STORAGE_CLOSE,          0x00f8,   2, &Ea::InventoryRecv::processPlayerStorageClose);
packet(SMSG_PLAYER_STORAGE_EQUIP,          0x00a6,  -1, &InventoryRecv::processPlayerStorageEquip);
packet(SMSG_PLAYER_STORAGE_ITEMS,          0x01f0,  -1, &InventoryRecv::processPlayerStorage);
packet(SMSG_PLAYER_STORAGE_REMOVE,         0x00f6,   8, &InventoryRecv::processPlayerStorageRemove);
packet(SMSG_PLAYER_STORAGE_STATUS,         0x00f2,   6, &Ea::InventoryRecv::processPlayerStorageStatus);
packet(SMSG_PLAYER_UNEQUIP,                0x00ac,   7, &InventoryRecv::processPlayerUnEquip);
packet(SMSG_PLAYER_UPDATE_1,               0x01d8,  54, &BeingRecv::processPlayerUpdate1);
packet(SMSG_PLAYER_UPDATE_2,               0x01d9,  53, &BeingRecv::processPlayerUpdate2);
packet(SMSG_PLAYER_WARP,                   0x0091,  22, &Ea::PlayerRecv::processPlayerWarp);
packet(SMSG_PVP_MAP_MODE,                  0x0199,   4, &Ea::BeingRecv::processPvpMapMode);
packet(SMSG_PVP_SET,                       0x019a,  14, &BeingRecv::processPvpSet);
packet(SMSG_QUEST_PLAYER_VARS,             0x0215,  -1, &QuestRecv::processPlayerQuests);
packet(SMSG_QUEST_SET_VAR,                 0x0214,   8, &QuestRecv::processSetQuestVar);
packet(SMSG_SCRIPT_MESSAGE,                0x0229,  -1, &ChatRecv::processScriptMessage);
packet(SMSG_SERVER_PING,                   0x007f,   6, nullptr);
packet(SMSG_SERVER_VERSION_RESPONSE,       0x7531,  10, &LoginRecv::processServerVersion);
packet(SMSG_SKILL_CASTING,                 0x013e,  24, &BeingRecv::processSkillCasting);
packet(SMSG_SKILL_CAST_CANCEL,             0x01b9,   6, &BeingRecv::processSkillCastCancel);
packet(SMSG_SKILL_DAMAGE,                  0x01de,  33, &Ea::BeingRecv::processSkillDamage);
packet(SMSG_SKILL_FAILED,                  0x0110,  10, &SkillRecv::processSkillFailed);
packet(SMSG_SKILL_NO_DAMAGE,               0x011a,  15, &Ea::BeingRecv::processSkillNoDamage);
packet(SMSG_SOLVE_CHAR_NAME,               0x0194,  30, nullptr);
packet(SMSG_TRADE_CANCEL,                  0x00ee,   2, &Ea::TradeRecv::processTradeCancel);
packet(SMSG_TRADE_COMPLETE,                0x00f0,   3, &Ea::TradeRecv::processTradeComplete);
packet(SMSG_TRADE_ITEM_ADD,                0x00e9,  19, &TradeRecv::processTradeItemAdd);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE,       0x01b1,   7, &TradeRecv::processTradeItemAddResponse);
packet(SMSG_TRADE_OK,                      0x00ec,   3, &Ea::TradeRecv::processTradeOk);
packet(SMSG_TRADE_REQUEST,                 0x00e5,  26, &TradeRecv::processTradeRequest);
packet(SMSG_TRADE_RESPONSE,                0x00e7,   3, &TradeRecv::processTradeResponse);
packet(SMSG_UPDATE_HOST,                   0x0063,  -1, &Ea::LoginRecv::processUpdateHost);
packet(SMSG_WALK_RESPONSE,                 0x0087,  12, &PlayerRecv::processWalkResponse);
packet(SMSG_WHISPER,                       0x0097,  -1, &ChatRecv::processWhisper);
packet(SMSG_WHISPER_RESPONSE,              0x0098,   3, &ChatRecv::processWhisperResponse);
packet(SMSG_WHO_ANSWER,                    0x00c2,   6, &Ea::GameRecv::processWhoAnswer);
packet(SMSG_MAP_MASK,                      0x0226,  10, &Ea::PlayerRecv::processMapMask);
