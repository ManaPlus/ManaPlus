/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

packet(SMSG_ADMIN_KICK_ACK,                0x00cd,   6, &Ea::AdminRecv::processKickAck,                 0);
packet(SMSG_BEING_ACTION,                  0x008a,  29, &Ea::BeingRecv::processBeingAction,             0);
packet(SMSG_BEING_CHANGE_DIRECTION,        0x009c,   9, &BeingRecv::processBeingChangeDirection,        0);
packet(SMSG_BEING_CHANGE_LOOKS,            0x00c3,   8, &BeingRecv::processBeingChangeLook,             0);
packet(SMSG_BEING_CHANGE_LOOKS2,           0x01d7,  11, &BeingRecv::processBeingChangeLook2,            0);
packet(SMSG_BEING_CHAT,                    0x008d,  -1, &ChatRecv::processBeingChat,                    0);
packet(SMSG_BEING_EMOTION,                 0x00c0,   7, &Ea::BeingRecv::processBeingEmotion,            0);
packet(SMSG_BEING_IP_RESPONSE,             0x020c,  10, &BeingRecv::processIpResponse,                  0);
packet(SMSG_BEING_MOVE,                    0x007b,  60, &BeingRecv::processBeingMove,                   0);
packet(SMSG_BEING_MOVE2,                   0x0086,  16, &BeingRecv::processBeingMove2,                  0);
packet(SMSG_BEING_MOVE3,                   0x0225,  -1, &Ea::BeingRecv::processBeingMove3,              0);
packet(SMSG_BEING_NAME_RESPONSE,           0x0095,  30, &Ea::BeingRecv::processNameResponse,            0);
packet(SMSG_BEING_REMOVE,                  0x0080,   7, &Ea::BeingRecv::processBeingRemove,             0);
packet(SMSG_BEING_RESURRECT,               0x0148,   8, &BeingRecv::processBeingResurrect,              0);
packet(SMSG_BEING_SELFEFFECT,              0x019b,  10, &BeingRecv::processBeingSelfEffect,             0);
packet(SMSG_BEING_SPAWN,                   0x007c,  41, &BeingRecv::processBeingSpawn,                  0);
packet(SMSG_BEING_STATUS_CHANGE,           0x0196,   9, &BeingRecv::processBeingStatusChange,           0);
packet(SMSG_BEING_VISIBLE,                 0x0078,  54, &BeingRecv::processBeingVisible,                0);
packet(SMSG_CHANGE_MAP_SERVER,             0x0092,  28, &CharServerRecv::processChangeMapServer,        0);
packet(SMSG_CHAR_CREATE_FAILED,            0x006e,   3, &Ea::CharServerRecv::processCharCreateFailed,   0);
packet(SMSG_CHAR_CREATE_SUCCEEDED,         0x006d, 108, &CharServerRecv::processCharCreate,             0);
packet(SMSG_CHAR_DELETE_FAILED,            0x0070,   3, &CharServerRecv::processCharDeleteFailed,       0);
packet(SMSG_CHAR_DELETE_SUCCEEDED,         0x006f,   2, &Ea::CharServerRecv::processCharDelete,         0);
packet(SMSG_CHAR_LOGIN,                    0x006b,  -1, &CharServerRecv::processCharLogin,              0);
packet(SMSG_CHAR_LOGIN_ERROR,              0x006c,   3, &Ea::CharServerRecv::processCharLoginError,     0);
packet(SMSG_CHAR_MAP_INFO,                 0x0071,  28, &CharServerRecv::processCharMapInfo,            0);
packet(SMSG_CHAR_PASSWORD_RESPONSE,        0x0062,   3, &LoginRecv::processCharPasswordResponse,        0);
packet(SMSG_CHAR_SWITCH_RESPONSE,          0x00b3,   3, &Ea::GameRecv::processCharSwitchResponse,       0);
packet(SMSG_CONNECTION_PROBLEM,            0x0081,   3, &GeneralRecv::processConnectionProblem,         0);
packet(SMSG_GM_CHAT,                       0x009a,  -1, &ChatRecv::processGmChat,                       0);
packet(SMSG_IGNORE_ALL_RESPONSE,           0x00d2,   4, &Ea::ChatRecv::processIgnoreAllResponse,        0);
packet(SMSG_ITEM_DROPPED,                  0x009e,  17, &ItemRecv::processItemDropped,                  0);
packet(SMSG_ITEM_REMOVE,                   0x00a1,   6, &Ea::ItemRecv::processItemRemove,               0);
packet(SMSG_ITEM_USE_RESPONSE,             0x00a8,   7, &Ea::InventoryRecv::processItemUseResponse,     0);
packet(SMSG_ITEM_VISIBLE,                  0x009d,  17, &ItemRecv::processItemVisible,                  0);
packet(SMSG_LOGIN_DATA,                    0x0069,  -1, &LoginRecv::processLoginData,                   0);
packet(SMSG_LOGIN_ERROR,                   0x006a,  23, &Ea::LoginRecv::processLoginError,              0);
packet(SMSG_MAP_LOGIN_SUCCESS,             0x0073,  11, &GameRecv::processMapLogin,                     0);
packet(SMSG_MAP_MUSIC,                     0x0227,  -1, &Ea::PlayerRecv::processMapMusic,               0);
packet(SMSG_MAP_QUIT_RESPONSE,             0x018b,   4, &Ea::GameRecv::processMapQuitResponse,          0);
packet(SMSG_MVP_EFFECT,                    0x010c,   6, &Ea::ChatRecv::processMVPEffect,                0);
packet(SMSG_NPC_BUY,                       0x00c6,  -1, &BuySellRecv::processNpcBuy,                    0);
packet(SMSG_NPC_BUY_RESPONSE,              0x00ca,   3, &BuySellRecv::processNpcBuyResponse,            0);
packet(SMSG_NPC_BUY_SELL_CHOICE,           0x00c4,   6, &Ea::BuySellRecv::processNpcBuySellChoice,      0);
packet(SMSG_NPC_CHANGETITLE,               0x0228,  -1, &Ea::NpcRecv::processChangeTitle,               0);
packet(SMSG_NPC_CHOICE,                    0x00b7,  -1, &Ea::NpcRecv::processNpcChoice,                 0);
packet(SMSG_NPC_CLOSE,                     0x00b6,   6, &Ea::NpcRecv::processNpcClose,                  0);
packet(SMSG_NPC_COMMAND,                   0x0212,  16, &Ea::NpcRecv::processNpcCommand,                0);
packet(SMSG_NPC_INT_INPUT,                 0x0142,   6, &Ea::NpcRecv::processNpcIntInput,               0);
packet(SMSG_NPC_MESSAGE,                   0x00b4,  -1, &Ea::NpcRecv::processNpcMessage,                0);
packet(SMSG_NPC_NEXT,                      0x00b5,   6, &Ea::NpcRecv::processNpcNext,                   0);
packet(SMSG_NPC_SELL,                      0x00c7,  -1, &Ea::BuySellRecv::processNpcSell,               0);
packet(SMSG_NPC_SELL_RESPONSE,             0x00cb,   3, &BuySellRecv::processNpcSellResponse,           0);
packet(SMSG_NPC_STR_INPUT,                 0x01d4,   6, &Ea::NpcRecv::processNpcStrInput,               0);
packet(SMSG_PARTY_CREATE,                  0x00fa,   3, &Ea::PartyRecv::processPartyCreate,             0);
packet(SMSG_PARTY_INFO,                    0x00fb,  -1, &PartyRecv::processPartyInfo,                   0);
packet(SMSG_PARTY_INVITED,                 0x00fe,  30, &PartyRecv::processPartyInvited,                0);
packet(SMSG_PARTY_INVITE_RESPONSE,         0x00fd,  27, &PartyRecv::processPartyInviteResponse,         0);
packet(SMSG_PARTY_LEAVE,                   0x0105,  31, &Ea::PartyRecv::processPartyLeave,              0);
packet(SMSG_PARTY_MESSAGE,                 0x0109,  -1, &PartyRecv::processPartyMessage,                0);
packet(SMSG_PARTY_MOVE,                    0x0104,  79, &PartyRecv::processPartyMove,                   0);
packet(SMSG_PARTY_SETTINGS,                0x0101,   6, &PartyRecv::processPartySettings,               0);
packet(SMSG_PARTY_UPDATE_COORDS,           0x0107,  10, &Ea::PartyRecv::processPartyUpdateCoords,       0);
packet(SMSG_PARTY_UPDATE_HP,               0x0106,  10, &PartyRecv::processPartyUpdateHp,               0);
packet(SMSG_PLAYER_ARROW_EQUIP,            0x013c,   4, &Ea::InventoryRecv::processPlayerArrowEquip,    0);
packet(SMSG_PLAYER_ARROW_MESSAGE,          0x013b,   4, &Ea::PlayerRecv::processPlayerArrowMessage,     0);
packet(SMSG_PLAYER_ATTACK_RANGE,           0x013a,   4, &Ea::InventoryRecv::processPlayerAttackRange,   0);
packet(SMSG_PLAYER_CHAT,                   0x008e,  -1, &ChatRecv::processChat,                         0);
packet(SMSG_PLAYER_EQUIP,                  0x00aa,   7, &InventoryRecv::processPlayerEquip,             0);
packet(SMSG_PLAYER_EQUIPMENT,              0x00a4,  -1, &InventoryRecv::processPlayerEquipment,         0);
packet(SMSG_PLAYER_GUILD_PARTY_INFO,       0x0195, 102, &BeingRecv::processPlayerGuilPartyInfo,         0);
packet(SMSG_PLAYER_INVENTORY,              0x01ee,  -1, &InventoryRecv::processPlayerInventory,         0);
packet(SMSG_PLAYER_INVENTORY_ADD,          0x00a0,  23, &InventoryRecv::processPlayerInventoryAdd,      0);
packet(SMSG_PLAYER_INVENTORY_REMOVE,       0x00af,   6, &InventoryRecv::processPlayerInventoryRemove,   0);
packet(SMSG_PLAYER_INVENTORY_USE,          0x01c8,  13, &InventoryRecv::processPlayerInventoryUse,      0);
packet(SMSG_PLAYER_MOVE,                   0x01da,  60, &BeingRecv::processPlayerMove,                  0);
packet(SMSG_PLAYER_MOVE_TO_ATTACK,         0x0139,  16, &Ea::BeingRecv::processPlayerMoveToAttack,      0);
packet(SMSG_PLAYER_SKILLS,                 0x010f,  -1, &SkillRecv::processPlayerSkills,                0);
packet(SMSG_PLAYER_SKILL_UP,               0x010e,  11, &Ea::SkillRecv::processPlayerSkillUp,           0);
packet(SMSG_PLAYER_STATUS_CHANGE,          0x0119,  13, &BeingRecv::processPlayerStatusChange,          0);
packet(SMSG_PLAYER_STAT_UPDATE_1,          0x00b0,   8, &Ea::PlayerRecv::processPlayerStatUpdate1,      0);
packet(SMSG_PLAYER_STAT_UPDATE_2,          0x00b1,   8, &Ea::PlayerRecv::processPlayerStatUpdate2,      0);
packet(SMSG_PLAYER_STAT_UPDATE_3,          0x0141,  14, &Ea::PlayerRecv::processPlayerStatUpdate3,      0);
packet(SMSG_PLAYER_STAT_UPDATE_4,          0x00bc,   6, &Ea::PlayerRecv::processPlayerStatUpdate4,      0);
packet(SMSG_PLAYER_STAT_UPDATE_5,          0x00bd,  44, &PlayerRecv::processPlayerStatUpdate5,          0);
packet(SMSG_PLAYER_STAT_UPDATE_6,          0x00be,   5, &Ea::PlayerRecv::processPlayerStatUpdate6,      0);
packet(SMSG_PLAYER_STOP,                   0x0088,  10, &Ea::BeingRecv::processPlayerStop,              0);
packet(SMSG_PLAYER_STORAGE_ADD,            0x00f4,  21, &InventoryRecv::processPlayerStorageAdd,        0);
packet(SMSG_PLAYER_STORAGE_CLOSE,          0x00f8,   2, &Ea::InventoryRecv::processPlayerStorageClose,  0);
packet(SMSG_PLAYER_STORAGE_EQUIP,          0x00a6,  -1, &InventoryRecv::processPlayerStorageEquip,      0);
packet(SMSG_PLAYER_STORAGE_ITEMS,          0x01f0,  -1, &InventoryRecv::processPlayerStorage,           0);
packet(SMSG_PLAYER_STORAGE_REMOVE,         0x00f6,   8, &InventoryRecv::processPlayerStorageRemove,     0);
packet(SMSG_PLAYER_STORAGE_STATUS,         0x00f2,   6, &Ea::InventoryRecv::processPlayerStorageStatus, 0);
packet(SMSG_PLAYER_UNEQUIP,                0x00ac,   7, &InventoryRecv::processPlayerUnEquip,           0);
packet(SMSG_PLAYER_UPDATE_1,               0x01d8,  54, &BeingRecv::processPlayerUpdate1,               0);
packet(SMSG_PLAYER_UPDATE_2,               0x01d9,  53, &BeingRecv::processPlayerUpdate2,               0);
packet(SMSG_PLAYER_WARP,                   0x0091,  22, &Ea::PlayerRecv::processPlayerWarp,             0);
packet(SMSG_PVP_MAP_MODE,                  0x0199,   4, &Ea::BeingRecv::processPvpMapMode,              0);
packet(SMSG_PVP_SET,                       0x019a,  14, &BeingRecv::processPvpSet,                      0);
packet(SMSG_QUEST_PLAYER_VARS,             0x0215,  -1, &QuestRecv::processPlayerQuests,                0);
packet(SMSG_QUEST_SET_VAR,                 0x0214,   8, &QuestRecv::processSetQuestVar,                 0);
packet(SMSG_SCRIPT_MESSAGE,                0x0229,  -1, &ChatRecv::processScriptMessage,                0);
packet(SMSG_SERVER_PING,                   0x007f,   6, nullptr,                                        0);
packet(SMSG_SERVER_VERSION_RESPONSE,       0x7531,  10, &LoginRecv::processServerVersion,               0);
packet(SMSG_SKILL_CASTING,                 0x013e,  24, &BeingRecv::processSkillCasting,                0);
packet(SMSG_SKILL_CAST_CANCEL,             0x01b9,   6, &BeingRecv::processSkillCastCancel,             0);
packet(SMSG_SKILL_DAMAGE,                  0x01de,  33, &BeingRecv::processSkillDamage,                 0);
packet(SMSG_SKILL_FAILED,                  0x0110,  10, &SkillRecv::processSkillFailed,                 0);
packet(SMSG_SKILL_NO_DAMAGE,               0x011a,  15, &Ea::BeingRecv::processSkillNoDamage,           0);
packet(SMSG_SOLVE_CHAR_NAME,               0x0194,  30, nullptr,                                        0);
packet(SMSG_TRADE_CANCEL,                  0x00ee,   2, &Ea::TradeRecv::processTradeCancel,             0);
packet(SMSG_TRADE_COMPLETE,                0x00f0,   3, &Ea::TradeRecv::processTradeComplete,           0);
packet(SMSG_TRADE_ITEM_ADD,                0x00e9,  19, &TradeRecv::processTradeItemAdd,                0);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE,       0x01b1,   7, &TradeRecv::processTradeItemAddResponse,        0);
packet(SMSG_TRADE_OK,                      0x00ec,   3, &Ea::TradeRecv::processTradeOk,                 0);
packet(SMSG_TRADE_REQUEST,                 0x00e5,  26, &TradeRecv::processTradeRequest,                0);
packet(SMSG_TRADE_RESPONSE,                0x00e7,   3, &TradeRecv::processTradeResponse,               0);
packet(SMSG_UPDATE_HOST,                   0x0063,  -1, &Ea::LoginRecv::processUpdateHost,              0);
packet(SMSG_WALK_RESPONSE,                 0x0087,  12, &PlayerRecv::processWalkResponse,               0);
packet(SMSG_WHISPER,                       0x0097,  -1, &ChatRecv::processWhisper,                      0);
packet(SMSG_WHISPER_RESPONSE,              0x0098,   3, &ChatRecv::processWhisperResponse,              0);
packet(SMSG_WHO_ANSWER,                    0x00c2,   6, &Ea::GameRecv::processWhoAnswer,                0);
packet(SMSG_MAP_MASK,                      0x0226,  10, &Ea::PlayerRecv::processMapMask,                0);
packet(SMSG_PLAYER_CLIENT_COMMAND,         0x0230,  -1, &Ea::PlayerRecv::processPlayerClientCommand,    0);
packet(SMSG_MAP_SET_TILES_TYPE,            0x0231,  34, &Ea::MapRecv::processSetTilesType,              0);
packet(SMSG_ONLINE_LIST,                   0x0211,  -1, &PlayerRecv::processOnlineList,                 0);
