/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

// very outdated packets
packet(SMSG_MAP_LOGIN_SUCCESS,             0x0073,  11, &GameRecv::processMapLogin,                     1);
packet(SMSG_BEING_FAKE_NAME,               0x0078,  54, &BeingRecv::processBeingFakeName,               1);
packet(SMSG_BEING_SPAWN,                   0x0079,  53, &BeingRecv::processBeingSpawn,                  1);
packet(SMSG_BEING_MOVE,                    0x007b,  60, &BeingRecv::processBeingMove,                   1);
packet(SMSG_BEING_SPAWN_OUTDATED,          0x007c,  41, nullptr,                                        1);
packet(SMSG_WHISPER,                       0x0097,  -1, &ChatRecv::processWhisper,                      1);
packet(SMSG_WHISPER_RESPONSE,              0x0098,   3, &ChatRecv::processWhisperResponse,              1);
packet(SMSG_ITEM_DROPPED,                  0x009e,  17, &ItemRecv::processItemDropped,                  1);
packet(SMSG_PLAYER_INVENTORY_ADD,          0x00a0,  23, &InventoryRecv::processPlayerInventoryAdd,      1);
packet(SMSG_PLAYER_INVENTORY,              0x00a3,  -1, &InventoryRecv::processPlayerInventory,         1);
packet(SMSG_PLAYER_EQUIPMENT,              0x00a4,  -1, &InventoryRecv::processPlayerEquipment,         1);
packet(SMSG_PLAYER_STORAGE_ITEMS,          0x00a5,  -1, &InventoryRecv::processPlayerStorage,           1);
packet(SMSG_PLAYER_STORAGE_EQUIP,          0x00a6,  -1, &InventoryRecv::processPlayerStorageEquip,      1);
packet(SMSG_PLAYER_EQUIP,                  0x00aa,   7, &InventoryRecv::processPlayerEquip,             1);
packet(SMSG_PLAYER_UNEQUIP,                0x00ac,   7, &InventoryRecv::processPlayerUnEquip,           1);
packet(SMSG_BEING_CHANGE_LOOKS_OUTDATED,   0x00c3,   8, nullptr,                                        1);
packet(SMSG_TRADE_REQUEST,                 0x00e5,  26, &TradeRecv::processTradeRequest,                1);
packet(SMSG_TRADE_ITEM_ADD,                0x00e9,  19, &TradeRecv::processTradeItemAdd,                1);
packet(SMSG_TRADE_RESPONSE_OUTDATED,       0x00e7,   3, nullptr,                                        0);
packet(SMSG_PLAYER_STORAGE_ADD,            0x00f4,  21, &InventoryRecv::processPlayerStorageAdd,        1);
packet(SMSG_PARTY_INVITED,                 0x00fe,  30, &PartyRecv::processPartyInvited,                1);
packet(SMSG_PLAYER_STATUS_CHANGE,          0x0119,  13, &BeingRecv::processPlayerStatusChange,          1);
packet(SMSG_SKILL_DAMAGE,                  0x0114,  31, &BeingRecv::processSkillDamage,                 1);
packet(SMSG_SKILL_ENTRY,                   0x011f,  16, &BeingRecv::processSkillEntry,                  1);
packet(SMSG_PLAYER_CART_EQUIP,             0x0122,  -1, &InventoryRecv::processPlayerCartEquip,         1);
packet(SMSG_PLAYER_CART_ITEMS,             0x0123,  -1, &InventoryRecv::processPlayerCartItems,         1);
packet(SMSG_PLAYER_CART_ADD,               0x0124,  21, &InventoryRecv::processPlayerCartAdd,           1);
packet(SMSG_VENDING_ITEMS_LIST,            0x0133,  -1, &VendingRecv::processItemsList,                 1);
packet(SMSG_VENDING_OPEN,                  0x0136,  -1, &VendingRecv::processOpen,                      1);
packet(SMSG_SKILL_CASTING,                 0x013e,  24, &BeingRecv::processSkillCasting,                1);
packet(SMSG_BEING_STATUS_CHANGE,           0x0196,   9, &BeingRecv::processBeingStatusChange,           1);
packet(SMSG_QUEST_LIST,                    0x02b1,  -1, &QuestRecv::processAddQuests,                   1);
packet(SMSG_BEING_VIEW_EQUIPMENT,          0x02d7,  -1, &BeingRecv::processBeingViewEquipment,          1);
packet(SMSG_PARTY_SETTINGS,                0x0101,   6, &PartyRecv::processPartySettings,               1);
packet(SMSG_PLAYER_HP,                     0x0106,  10, &BeingRecv::processBeingHp,                     1);
packet(SMSG_PET_STATUS,                    0x01a2,  35, &PetRecv::processPetStatus,                     1);

// fake packets for add packet name
packet(SMSG_SELECT_CART,                   0x0000,   0, nullptr,                                        0);
packet(CMSG_NAVIGATE_TO,                   0x0000,   0, nullptr,                                        0);
packet(SMSG_MERGE_ITEM,                    0x0000,   0, nullptr,                                        0);
packet(SMSG_ACK_MERGE_ITEMS,               0x0000,   0, nullptr,                                        0);
packet(SMSG_RANKS_LIST,                    0x0000,   0, nullptr,                                        0);
packet(SMSG_CHAR_CHARACTERS,               0x0000,   0, nullptr,                                        0);

// login server, unknown version
packet(SMSG_LOGIN_DATA,                    0x0069,  -1, &LoginRecv::processLoginData,                   0);
packet(SMSG_LOGIN_CODING_KEY,              0x01dc,  -1, &LoginRecv::processCondingKey,                  0);
packet(SMSG_LOGIN_ERROR,                   0x006a,  23, &Ea::LoginRecv::processLoginError,              0);

// char server, unknown version
packet(SMSG_CHAR_CHANGE_SLOT,              0x08d5,  -1, &CharServerRecv::processCharChangeSlot,         0);
packet(SMSG_CHAR_CHECK_RENAME,             0x028e,   4, &CharServerRecv::processCharCheckRename,        0);
packet(SMSG_CHAR_CREATE_FAILED,            0x006e,   3, &Ea::CharServerRecv::processCharCreateFailed,   0);
packet(SMSG_CHAR_CREATE_SUCCEEDED,         0x006d, 149, &CharServerRecv::processCharCreate,             0);
packet(SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK, 0x082a,  10, &CharServerRecv::processCharDelete2AcceptActual, 0);
packet(SMSG_CHAR_DELETE2_ACK,              0x0828,  14, &CharServerRecv::processCharDelete2Ack,         0);
packet(SMSG_CHAR_DELETE2_CANCEL_ACK,       0x082c,  10, &CharServerRecv::processCharDelete2CancelAck,   0);
packet(SMSG_CHAR_DELETE_FAILED,            0x0070,   3, &CharServerRecv::processCharDeleteFailed,       0);
packet(SMSG_CHAR_DELETE_SUCCEEDED,         0x006f,   2, &Ea::CharServerRecv::processCharDelete,         0);
packet(SMSG_CHAR_LOGIN,                    0x006b,  -1, &CharServerRecv::processCharLogin,              0);
packet(SMSG_CHAR_LOGIN2,                   0x082d,  -1, &CharServerRecv::processCharLogin2,             0);
packet(SMSG_CHAR_LOGIN_ERROR,              0x006c,   3, &Ea::CharServerRecv::processCharLoginError,     0);
packet(SMSG_CHAR_MAP_INFO,                 0x0071,  28, &CharServerRecv::processCharMapInfo,            0);
packet(SMSG_CHAR_PINCODE_STATUS,           0x08b9,  12, &CharServerRecv::processPincodeStatus,          0);
packet(SMSG_CHAR_RENAME,                   0x0290,   4, &CharServerRecv::processCharRename,             0);
packet(SMSG_MAP_NOT_FOUND,                 0x0840,  -1, &GeneralRecv::processMapNotFound,               0);

// map server, unknown versions
packet(SMSG_ADMIN_GET_LOGIN_ACK,           0x01e0,  30, &AdminRecv::processAdminGetLoginAck,            0);
packet(SMSG_ADMIN_KICK_ACK,                0x00cd,   3, &Ea::AdminRecv::processKickAck,                 0);
packet(SMSG_ADMIN_SET_TILE_TYPE,           0x0192,  24, &AdminRecv::processSetTileType,                 0);
packet(SMSG_BATTLE_BEGINS,                 0x08df,  50, &BattleGroundRecv::processBattleBegins,         0);
packet(SMSG_BATTLE_JOINED,                 0x08d9,  30, &BattleGroundRecv::processBattleJoined,         0);
packet(SMSG_BATTLE_NOTICE_DELETE,          0x08db,  27, &BattleGroundRecv::processBattleNoticeDelete,   0);
packet(SMSG_BATTLE_QUEUE_ACK,              0x08d8,  27, &BattleGroundRecv::processBattleQueueAck,       0);
packet(SMSG_BEING_ACTION,                  0x008a,  29, &Ea::BeingRecv::processBeingAction,             0);
packet(SMSG_BEING_CHANGE_DIRECTION,        0x009c,   9, &BeingRecv::processBeingChangeDirection,        0);
packet(SMSG_BEING_CHAT,                    0x008d,  -1, &ChatRecv::processBeingChat,                    0);
packet(SMSG_BEING_EMOTION,                 0x00c0,   7, &Ea::BeingRecv::processBeingEmotion,            0);
packet(SMSG_BEING_MOVE2,                   0x0086,  16, &BeingRecv::processBeingMove2,                  0);
packet(SMSG_BEING_NAME_RESPONSE,           0x0095,  30, &Ea::BeingRecv::processNameResponse,            0);
packet(SMSG_BEING_REMOVE,                  0x0080,   7, &Ea::BeingRecv::processBeingRemove,             0);
packet(SMSG_BEING_REMOVE_SKILL,            0x0120,   6, &BeingRecv::processBeingRemoveSkill,            0);
packet(SMSG_BEING_RESURRECT,               0x0148,   8, &BeingRecv::processBeingResurrect,              0);
packet(SMSG_BEING_SELFEFFECT,              0x019b,  10, &BeingRecv::processBeingSelfEffect,             0);
packet(SMSG_BEING_SLIDE,                   0x01ff,  10, &BeingRecv::processBeingSlide,                  0);
packet(SMSG_BEING_SOUND_EFFECT,            0x01d3,  35, &BeingRecv::processBeingSoundEffect,            0);
packet(SMSG_BEING_SPECIAL_EFFECT,          0x01f3,  10, &BeingRecv::processBeingSpecialEffect,          0);
packet(SMSG_BEING_STAT_UPDATE_1,           0x01ab,  12, &BeingRecv::processBeingStatUpdate1,            0);
packet(SMSG_BIND_ITEM,                     0x02d3,   4, &InventoryRecv::processBindItem,                0);
packet(SMSG_BLADE_STOP,                    0x01d1,  14, &BeingRecv::processBladeStop,                   0);
packet(SMSG_CART_INFO,                     0x0121,  14, &InventoryRecv::processCartInfo,                0);
packet(SMSG_CART_REMOVE,                   0x012b,   2, &InventoryRecv::processCartRemove,              0);
packet(SMSG_CHANGE_MAP_SERVER,             0x0092,  28, &CharServerRecv::processChangeMapServer,        0);
packet(SMSG_CHAR_BAN_CHAR_LIST,            0x020d,  -1, &CharServerRecv::processCharBanCharList,        0);
packet(SMSG_CHAR_SWITCH_RESPONSE,          0x00b3,   3, &Ea::GameRecv::processCharSwitchResponse,       0);
packet(SMSG_CHAT_DISPLAY,                  0x00d7,  -1, &ChatRecv::processChatDisplay,                  0);
packet(SMSG_CHAT_IGNORE_LIST,              0x00d4,  -1, &ChatRecv::processChatIgnoreList,               0);
packet(SMSG_CHAT_ROOM_ADD_MEMBER,          0x00dc,  28, &ChatRecv::processChatRoomAddMember,            0);
packet(SMSG_CHAT_ROOM_CREATE_ACK,          0x00d6,   3, &ChatRecv::processChatRoomCreateAck,            0);
packet(SMSG_CHAT_ROOM_DESTROY,             0x00d8,   6, &ChatRecv::processChatRoomDestroy,              0);
packet(SMSG_CHAT_ROOM_JOIN_ACK,            0x00db,  -1, &ChatRecv::processChatRoomJoinAck,              0);
packet(SMSG_CHAT_ROOM_JOIN_FAILED,         0x00da,   3, &ChatRecv::processChatRoomJoinFailed,           0);
packet(SMSG_CHAT_ROOM_LEAVE,               0x00dd,  29, &ChatRecv::processChatRoomLeave,                0);
packet(SMSG_CHAT_ROOM_ROLE_CHANGE,         0x00e1,  30, &ChatRecv::processChatRoomRoleChange,           0);
packet(SMSG_CHAT_ROOM_SETTINGS,            0x00df,  -1, &ChatRecv::processChatRoomSettings,             0);
packet(SMSG_CHAT_SILENCE,                  0x014b,  27, &ChatRecv::processChatSilence,                  0);
packet(SMSG_CHAT_TALKIE_BOX,               0x0191,  86, &ChatRecv::processChatTalkieBox,                0);
packet(SMSG_CLASS_CHANGE,                  0x01b0,  11, &BeingRecv::processClassChange,                 0);
packet(SMSG_COMBO_DELAY,                   0x01d2,  10, &BeingRecv::processComboDelay,                  0);
packet(SMSG_CONNECTION_PROBLEM,            0x0081,   3, &GeneralRecv::processConnectionProblem,         0);
packet(SMSG_FAMILY_ASK_FOR_CHILD,          0x01f6,  34, &FamilyRecv::processAskForChild,                0);
packet(SMSG_FAMILY_CALL_PARTNER,           0x01e6,  26, &FamilyRecv::processCallPartner,                0);
packet(SMSG_FAMILY_DIVORCED,               0x0205,  26, &FamilyRecv::processDivorced,                   0);
packet(SMSG_FRIENDS_DELETE_PLAYER,         0x020a,  10, &FriendsRecv::processDeletePlayer,              0);
packet(SMSG_FRIENDS_LIST,                  0x0201,  -1, &FriendsRecv::processFriendsList,               0);
packet(SMSG_FRIENDS_PLAYER_ONLINE,         0x0206,  11, &FriendsRecv::processPlayerOnline,              0);
packet(SMSG_FRIENDS_REQUEST,               0x0207,  34, &FriendsRecv::processRequest,                   0);
packet(SMSG_FRIENDS_REQUEST_ACK,           0x0209,  36, &FriendsRecv::processRequestAck,                0);
packet(SMSG_GM_CHAT,                       0x009a,  -1, &ChatRecv::processGmChat,                       0);
packet(SMSG_GM_CHAT2,                      0x01c3,  -1, &ChatRecv::processGmChat2,                      0);
packet(SMSG_GRAFFITI_VISIBLE,              0x01c9,  97, &BeingRecv::processGraffiti,                    0);
packet(SMSG_GUILD_ALIANCE_INFO,            0x014c,  -1, &GuildRecv::processGuildAlianceInfo,        0);
packet(SMSG_GUILD_ALLIANCE_ADDED_DISABLED, 0x0185,  34, nullptr,                                        0);
packet(SMSG_GUILD_BASIC_INFO,              0x01b6, 114, &GuildRecv::processGuildBasicInfo,          0);
packet(SMSG_GUILD_BROKEN,                  0x015e,   6, &GuildRecv::processGuildBroken,             0);
packet(SMSG_GUILD_CREATE_RESPONSE,         0x0167,   3, &GuildRecv::processGuildCreateResponse,     0);
packet(SMSG_GUILD_DEL_ALLIANCE,            0x0184,  10, &GuildRecv::processGuildDelAlliance,        0);
packet(SMSG_GUILD_EMBLEM,                  0x01b4,  12, &GuildRecv::processGuildEmblem,                 0);
packet(SMSG_GUILD_EMBLEM_DATA,             0x0152,  -1, &GuildRecv::processGuildEmblemData,         0);
packet(SMSG_GUILD_EXPULSION_LIST,          0x0163,  -1, &GuildRecv::processGuildExpulsionList,          0);
packet(SMSG_GUILD_INVITE,                  0x016a,  30, &GuildRecv::processGuildInvite,             0);
packet(SMSG_GUILD_INVITE_ACK,              0x0169,   3, &GuildRecv::processGuildInviteAck,          0);
packet(SMSG_GUILD_LEAVE,                   0x015a,  66, &GuildRecv::processGuildLeave,              0);
packet(SMSG_GUILD_MASTER_OR_MEMBER,        0x014e,   6, &GuildRecv::processGuildMasterOrMember,     0);
packet(SMSG_GUILD_MEMBER_LIST,             0x0154,  -1, &GuildRecv::processGuildMemberList,         0);
packet(SMSG_GUILD_MEMBER_LOGIN,            0x01f2,  20, &GuildRecv::processGuildMemberLogin,            0);
packet(SMSG_GUILD_MEMBER_POS_CHANGE,       0x0156,  -1, &GuildRecv::processGuildMemberPosChange,    0);
packet(SMSG_GUILD_MESSAGE,                 0x017f,  -1, &GuildRecv::processGuildMessage,            0);
packet(SMSG_GUILD_NOTICE,                  0x016f, 182, &GuildRecv::processGuildNotice,             0);
packet(SMSG_GUILD_OPPOSITION_ACK,          0x0181,   3, &GuildRecv::processGuildOppositionAck,      0);
packet(SMSG_GUILD_POSITION_CHANGED,        0x0174,  -1, &GuildRecv::processGuildPositionChanged,    0);
packet(SMSG_GUILD_POSITION_INFO,           0x016c,  43, &GuildRecv::processGuildPositionInfo,           0);
packet(SMSG_GUILD_POS_INFO_LIST,           0x0160,  -1, &GuildRecv::processGuildPosInfoList,        0);
packet(SMSG_GUILD_POS_NAME_LIST,           0x0166,  -1, &GuildRecv::processGuildPosNameList,        0);
packet(SMSG_GUILD_REQ_ALLIANCE,            0x0171,  30, &GuildRecv::processGuildReqAlliance,        0);
packet(SMSG_GUILD_REQ_ALLIANCE_ACK,        0x0173,   3, &GuildRecv::processGuildReqAllianceAck,     0);
packet(SMSG_GUILD_SKILL_INFO,              0x0162,  -1, &GuildRecv::processGuildSkillInfo,          0);
packet(SMSG_GUILD_UPDATE_COORDS,           0x01eb,  10, &GuildRecv::processGuildUpdateCoords,           0);
packet(SMSG_IGNORE_ALL_RESPONSE,           0x00d2,   4, &Ea::ChatRecv::processIgnoreAllResponse,        0);
packet(SMSG_IGNORE_NICK_ACK,               0x00d1,   4, &ChatRecv::processIgnoreNickAck,                0);
packet(SMSG_ITEM_MVP_DROPPED,              0x07fd,  -1, &ItemRecv::processItemMvpDropped,               0);
packet(SMSG_ITEM_REMOVE,                   0x00a1,   6, &Ea::ItemRecv::processItemRemove,               0);
packet(SMSG_ITEM_USE_RESPONSE,             0x00a8,   7, &Ea::InventoryRecv::processItemUseResponse,     0);
packet(SMSG_ITEM_VISIBLE,                  0x009d,  17, &ItemRecv::processItemVisible,                  0);
packet(SMSG_MANNER_MESSAGE,                0x014a,   6, &ChatRecv::processMannerMessage,                0);
packet(SMSG_MAP_AUTH_REFUSE,               0x0074,   3, &GameRecv::processMapAuthRefuse,                0);
packet(SMSG_MAP_QUIT_RESPONSE,             0x018b,   4, &Ea::GameRecv::processMapQuitResponse,          0);
packet(SMSG_MAP_TYPE,                      0x01d6,   4, &BeingRecv::processMapType,                     0);
packet(SMSG_MARRIAGE_PROCESS_OUTDATED,     0x01e4,   2, nullptr,                                        0);
packet(SMSG_MARRIAGE_PROPOSAL_OUTDATED,    0x01e2,  34, nullptr,                                        0);
packet(SMSG_MONSTER_INFO,                  0x018c,  29, &BeingRecv::processMonsterInfo,                 0);
packet(SMSG_MVP_ITEM,                      0x010a,   4, &ChatRecv::processMVPItem,                      0);
packet(SMSG_MVP_EXP,                       0x010b,   6, &ChatRecv::processMVPExp,                       0);
packet(SMSG_MVP_EFFECT,                    0x010c,   6, &Ea::ChatRecv::processMVPEffect,                0);
packet(SMSG_MVP_NO_ITEM,                   0x010d,   2, &ChatRecv::processMVPNoItem,                    0);
packet(SMSG_NPC_BUY,                       0x00c6,  -1, &BuySellRecv::processNpcBuy,                    0);
packet(SMSG_NPC_BUY_RESPONSE,              0x00ca,   3, &BuySellRecv::processNpcBuyResponse,            0);
packet(SMSG_NPC_BUY_SELL_CHOICE,           0x00c4,   6, &Ea::BuySellRecv::processNpcBuySellChoice,      0);
packet(SMSG_NPC_CASH_BUY,                  0x0849,  16, &CashShopRecv::processCashShopBuy,              0);
packet(SMSG_NPC_CASH_POINTS,               0x0845,  10, &CashShopRecv::processCashShopPoints,           0);
packet(SMSG_NPC_CASH_SCHEDULE,             0x08ca,  -1, &CashShopRecv::processCashShopSchedule,         0);
packet(SMSG_NPC_CASH_TAB_PRICE_LIST,       0x08c0,  -1, &CashShopRecv::processCashShopTabPriceList,     0);
packet(SMSG_NPC_CHOICE,                    0x00b7,  -1, &Ea::NpcRecv::processNpcChoice,                 0);
packet(SMSG_NPC_CLOSE,                     0x00b6,   6, &Ea::NpcRecv::processNpcClose,                  0);
packet(SMSG_NPC_CLOSE_TIMEOUT,             0x08d6,   6, &NpcRecv::processNpcCloseTimeout,               0);
packet(SMSG_NPC_CUTIN,                     0x01b3,  67, &NpcRecv::processNpcCutin,                      0);
packet(SMSG_NPC_INT_INPUT,                 0x0142,   6, &Ea::NpcRecv::processNpcIntInput,               0);
packet(SMSG_NPC_MESSAGE,                   0x00b4,  -1, &Ea::NpcRecv::processNpcMessage,                0);
packet(SMSG_NPC_NEXT,                      0x00b5,   6, &Ea::NpcRecv::processNpcNext,                   0);
packet(SMSG_NPC_SELL,                      0x00c7,  -1, &Ea::BuySellRecv::processNpcSell,               0);
packet(SMSG_NPC_SELL_RESPONSE,             0x00cb,   3, &BuySellRecv::processNpcSellResponse,           0);
packet(SMSG_NPC_SHOW_DIGIT,                0x01b1,   7, &NpcRecv::processShowDigit,                     0);
packet(SMSG_NPC_STR_INPUT,                 0x01d4,   6, &Ea::NpcRecv::processNpcStrInput,               0);
packet(SMSG_NPC_VIEWPOINT,                 0x0144,  23, &NpcRecv::processNpcViewPoint,                  0);
packet(SMSG_PARTY_CREATE,                  0x00fa,   3, &Ea::PartyRecv::processPartyCreate,             0);
packet(SMSG_PARTY_INFO,                    0x00fb,  -1, &PartyRecv::processPartyInfo,                   0);
packet(SMSG_PARTY_INVITE_RESPONSE_OUTDATED, 0x00fd, 27, nullptr,                                        0);
packet(SMSG_PARTY_LEADER,                  0x07fc,  10, &PartyRecv::processPartyLeader,                 0);
packet(SMSG_PARTY_LEAVE,                   0x0105,  31, &Ea::PartyRecv::processPartyLeave,              0);
packet(SMSG_PARTY_MEMBER_INFO,             0x01e9,  81, &PartyRecv::processPartyMemberInfo,             0);
packet(SMSG_PARTY_MESSAGE,                 0x0109,  -1, &PartyRecv::processPartyMessage,                0);
packet(SMSG_PARTY_UPDATE_COORDS,           0x0107,  10, &Ea::PartyRecv::processPartyUpdateCoords,       0);
packet(SMSG_PET_CATCH_PROCESS,             0x019e,   2, &PetRecv::processPetCatchProcess,               0);
packet(SMSG_PET_DATA,                      0x01a4,  11, &PetRecv::processPetData,                       0);
packet(SMSG_PET_EGGS_LIST,                 0x01a6,  -1, &PetRecv::processEggsList,                      0);
packet(SMSG_PET_FOOD,                      0x01a3,   5, &PetRecv::processPetFood,                       0);
packet(SMSG_PET_MESSAGE,                   0x01aa,  10, &PetRecv::processPetMessage,                    0);
packet(SMSG_PET_ROULETTE,                  0x01a0,   3, &PetRecv::processPetRoulette,                   0);
packet(SMSG_PLAYER_ADD_SKILL,              0x0111,  39, &SkillRecv::processSkillAdd,                    0);
packet(SMSG_PLAYER_ARROW_EQUIP,            0x013c,   4, &Ea::InventoryRecv::processPlayerArrowEquip,    0);
packet(SMSG_PLAYER_ARROW_MESSAGE,          0x013b,   4, &Ea::PlayerRecv::processPlayerArrowMessage,     0);
packet(SMSG_PLAYER_ATTACK_RANGE,           0x013a,   4, &Ea::InventoryRecv::processPlayerAttackRange,   0);
packet(SMSG_PLAYER_CART_ADD_ERROR,         0x012c,   3, &InventoryRecv::processCartAddError,            0);
packet(SMSG_PLAYER_CART_REMOVE,            0x0125,   8, &InventoryRecv::processPlayerCartRemove,        0);
packet(SMSG_PLAYER_CHAT,                   0x008e,  -1, &ChatRecv::processChat,                         0);
packet(SMSG_PLAYER_GUILD_PARTY_INFO,       0x0195, 102, &BeingRecv::processPlayerGuilPartyInfo,         0);
packet(SMSG_PLAYER_HEAL,                   0x013d,   6, &PlayerRecv::processPlayerHeal,                 0);
packet(SMSG_PLAYER_IDENTIFIED,             0x0179,   5, &InventoryRecv::processPlayerIdentified,        0);
packet(SMSG_PLAYER_IDENTIFY_LIST,          0x0177,  -1, &InventoryRecv::processPlayerIdentifyList,      0);
packet(SMSG_PLAYER_INSERT_CARD,            0x017d,   7, &InventoryRecv::processPlayerInsertCard,        0);
packet(SMSG_PLAYER_INVENTORY_REMOVE,       0x00af,   6, &InventoryRecv::processPlayerInventoryRemove,   0);
packet(SMSG_PLAYER_MOVE_TO_ATTACK,         0x0139,  16, &Ea::BeingRecv::processPlayerMoveToAttack,      0);
packet(SMSG_PLAYER_NOTIFY_MAPINFO,         0x0189,   4, &PlayerRecv::processNotifyMapInfo,              0);
packet(SMSG_PLAYER_REFINE,                 0x0188,   8, &InventoryRecv::processPlayerRefine,            0);
packet(SMSG_PLAYER_REPAIR_EFFECT,          0x01fe,   5, &InventoryRecv::processPlayerRepairEffect,      0);
packet(SMSG_PLAYER_REPAIR_LIST,            0x01fc,  -1, &InventoryRecv::processPlayerRepairList,        0);
packet(SMSG_PLAYER_SKILLS,                 0x010f,  -1, &SkillRecv::processPlayerSkills,                0);
packet(SMSG_PLAYER_SKILL_AUTO_SPELLS,      0x01cd,  30, &SkillRecv::processSkillAutoSpells,             0);
packet(SMSG_PLAYER_SKILL_PRODUCE_EFFECT,   0x018f,   6, &SkillRecv::processSkillProduceEffect,          0);
packet(SMSG_PLAYER_SKILL_PRODUCE_MIX_LIST, 0x018d,  -1, &SkillRecv::processSkillProduceMixList,         0);
packet(SMSG_PLAYER_SKILL_UP,               0x010e,  11, &Ea::SkillRecv::processPlayerSkillUp,           0);
packet(SMSG_PLAYER_STAT_UPDATE_1,          0x00b0,   8, &Ea::PlayerRecv::processPlayerStatUpdate1,      0);
packet(SMSG_PLAYER_STAT_UPDATE_2,          0x00b1,   8, &Ea::PlayerRecv::processPlayerStatUpdate2,      0);
packet(SMSG_PLAYER_STAT_UPDATE_3,          0x0141,  14, &Ea::PlayerRecv::processPlayerStatUpdate3,      0);
packet(SMSG_PLAYER_STAT_UPDATE_4,          0x00bc,   6, &Ea::PlayerRecv::processPlayerStatUpdate4,      0);
packet(SMSG_PLAYER_STAT_UPDATE_5,          0x00bd,  44, &PlayerRecv::processPlayerStatUpdate5,          0);
packet(SMSG_PLAYER_STAT_UPDATE_6,          0x00be,   5, &Ea::PlayerRecv::processPlayerStatUpdate6,      0);
packet(SMSG_PLAYER_STOP,                   0x0088,  10, &Ea::BeingRecv::processPlayerStop,              0);
packet(SMSG_PLAYER_STORAGE_CLOSE,          0x00f8,   2, &Ea::InventoryRecv::processPlayerStorageClose,  0);
packet(SMSG_PLAYER_STORAGE_REMOVE,         0x00f6,   8, &InventoryRecv::processPlayerStorageRemove,     0);
packet(SMSG_PLAYER_STORAGE_STATUS,         0x00f2,   6, &Ea::InventoryRecv::processPlayerStorageStatus, 0);
packet(SMSG_PLAYER_USE_CARD,               0x017b,  -1, &InventoryRecv::processPlayerUseCard,           0);
packet(SMSG_PLAYER_WARP,                   0x0091,  22, &Ea::PlayerRecv::processPlayerWarp,             0);
packet(SMSG_PVP_MAP_MODE,                  0x0199,   4, &Ea::BeingRecv::processPvpMapMode,              0);
packet(SMSG_PVP_SET,                       0x019a,  14, &BeingRecv::processPvpSet,                      0);
packet(SMSG_SCRIPT_MESSAGE,                0x08b3,  -1, &ChatRecv::processScriptMessage,                0);
packet(SMSG_SERVER_PING,                   0x007f,   6, &GameRecv::processServerTick,                   0);
packet(SMSG_SKILL_ARROW_CREATE_LIST,       0x01ad,  -1, &SkillRecv::processSkillArrowCreateList,        0);
packet(SMSG_SKILL_AUTO_CAST,               0x0147,  39, &BeingRecv::processSkillAutoCast,               0);
packet(SMSG_SKILL_CAST_CANCEL,             0x01b9,   6, &BeingRecv::processSkillCancel,                 0);
packet(SMSG_SKILL_DEVOTION_EFFECT,         0x01cf,  28, &SkillRecv::processSkillDevotionEffect,         0);
packet(SMSG_SKILL_FAILED,                  0x0110,  10, &SkillRecv::processSkillFailed,                 0);
packet(SMSG_SKILL_GROUND_DAMAGE_UNUSED,    0x0115,  35, nullptr,                                        0);
packet(SMSG_SKILL_GROUND_NO_DAMAGE,        0x0117,  18, &BeingRecv::processSkillGroundNoDamage,         0);
packet(SMSG_SKILL_MEMO_MESSAGE,            0x011e,   3, &SkillRecv::processSkillMemoMessage,            0);
packet(SMSG_SKILL_NO_DAMAGE,               0x011a,  15, &Ea::BeingRecv::processSkillNoDamage,           1);
packet(SMSG_SKILL_UNIT_UPDATE,             0x01ac,   6, &SkillRecv::processSkillUnitUpdate,             0);
packet(SMSG_SKILL_WARP_POINT,              0x011c,  68, &SkillRecv::processSkillWarpPoint,              0);
packet(SMSG_SOLVE_CHAR_NAME,               0x0194,  30, &BeingRecv::processSolveCharName,               0);
packet(SMSG_SPIRIT_BALLS,                  0x01d0,   8, &BeingRecv::processSpiritBalls,                 0);
packet(SMSG_SPIRIT_BALL_SINGLE,            0x01e1,   8, &BeingRecv::processSpiritBalls,                 0);
packet(SMSG_TRADE_CANCEL,                  0x00ee,   2, &Ea::TradeRecv::processTradeCancel,             0);
packet(SMSG_TRADE_COMPLETE,                0x00f0,   3, &Ea::TradeRecv::processTradeComplete,           0);
packet(SMSG_TRADE_ITEM_ADD_RESPONSE,       0x00ea,   5, &TradeRecv::processTradeItemAddResponse,        0);
packet(SMSG_TRADE_OK,                      0x00ec,   3, &Ea::TradeRecv::processTradeOk,                 0);
packet(SMSG_TRADE_UNDO,                    0x00f1,   2, &TradeRecv::processTradeUndo,                   0);
packet(SMSG_VENDING_BUY_ACK,               0x0135,   7, &VendingRecv::processBuyAck,                    0);
packet(SMSG_VENDING_HIDE_BOARD,            0x0132,   6, &VendingRecv::processHideBoard,                 0);
packet(SMSG_VENDING_OPEN_REQ,              0x012d,   4, &VendingRecv::processOpenReq,                   0);
packet(SMSG_VENDING_REPORT,                0x0137,   6, &VendingRecv::processReport,                    0);
packet(SMSG_VENDING_SHOW_BOARD,            0x0131,  86, &VendingRecv::processShowBoard,                 0);
packet(SMSG_WALK_RESPONSE,                 0x0087,  12, &PlayerRecv::processWalkResponse,               0);
packet(SMSG_WEDDING_EFFECT,                0x01ea,   6, &BeingRecv::processWddingEffect,                0);
packet(SMSG_WHO_ANSWER,                    0x00c2,   6, &Ea::GameRecv::processWhoAnswer,                0);
packet(SMSG_BOOKING_CANCEL_VOLUNTEER,      0x0909,   6, nullptr,                                        0);
packet(SMSG_BOOKING_ADD_FILTERING_LIST,    0x090b,  30, nullptr,                                        0);
packet(SMSG_BOOKING_SUB_FILTERING_LIST,    0x090c,  30, nullptr,                                        0);
packet(SMSG_ADD_MAP_MARKER,                0x09c1,  10, &MapRecv::processAddMapMarker,                  0);
packet(SMSG_GUILD_ONLINE_INFO,             0x016d,  14, &GuildRecv::processOnlineInfo,                  0);
packet(SMSG_FORMAT_MESSAGE_STRING,         0x02c2,  -1, &ChatRecv::processFormatMessageString,          0);

// 3
if (packetVersion >= 3)
{
    packet(SMSG_PLAYER_INVENTORY_USE,      0x01c8,  13, &Ea::InventoryRecv::processPlayerInventoryUse,  3);
    packet(SMSG_SKILL_DAMAGE,              0x01de,  33, &BeingRecv::processSkillDamage,                 3);
}

// 4
if (packetVersion >= 4)
{
    packet(SMSG_BEING_CHANGE_LOOKS2,       0x01d7,  11, &BeingRecv::processBeingChangeLook2,            4);
    packet(SMSG_BEING_VISIBLE,             0x01d8,  54, &BeingRecv::processBeingVisible,                4);
    packet(SMSG_BEING_SPAWN,               0x01d9,  53, &BeingRecv::processBeingSpawn,                  4);
    packet(SMSG_BEING_MOVE,                0x01da,  60, &BeingRecv::processBeingMove,                   4);
}

// 5
packet(SMSG_PLAYER_STORAGE_ADD,            0x01c4,  22, &InventoryRecv::processPlayerStorageAdd,        5);
packet(SMSG_PLAYER_CART_ADD,               0x01c5,  22, &InventoryRecv::processPlayerCartAdd,           5);

// 6
if (packetVersion >= 6)
{
    packet(SMSG_TRADE_REQUEST,             0x01f4,  32, &TradeRecv::processTradeRequest,                6);
    packet(SMSG_TRADE_RESPONSE,            0x01f5,   9, &TradeRecv::processTradeResponse,               6);
}

if (packetVersion >= 7)
{
    packet(SMSG_BEING_VISIBLE,             0x022a,  58, &BeingRecv::processBeingVisible,                7);
    packet(SMSG_BEING_SPAWN,               0x022b,  57, &BeingRecv::processBeingSpawn,                  7);
    packet(SMSG_BEING_MOVE,                0x022c,  65, &BeingRecv::processBeingMove,                   7);
}

// 20040816
packet(SMSG_ADMIN_ACCOUNT_STATS,           0x0214,  42, &AdminRecv::processAccountStats,                20040816);

// 20040817
packet(SMSG_PVP_INFO,                      0x0210,  22, &PlayerRecv::processPvpInfo,                    20040817);

// 20041101
packet(SMSG_PLAYER_SKILL_MESSAGE,          0x0215,   6, &PlayerRecv::processPlayerSkillMessage,         20041101);

// 20041108
packet(SMSG_FAMILY_ASK_FOR_CHILD_REPLY,    0x0216,   6, &FamilyRecv::processAskForChildReply,           20041108);
packet(SMSG_BLACKSMITH_RANKS_LIST,         0x0219, 282, &BeingRecv::processBlacksmithRanksList,         20041108);
packet(SMSG_ALCHEMIST_RANKS_LIST,          0x021a, 282, &BeingRecv::processAlchemistRanksList,          20041108);
packet(SMSG_PLAYER_FAME_BLACKSMITH,        0x021b,  10, &PlayerRecv::processPlayerFameBlacksmith,       20041108);
packet(SMSG_PLAYER_FAME_ALCHEMIST,         0x021c,  10, &PlayerRecv::processPlayerFameAlchemist,        20041108);

// 20041129
packet(SMSG_PLAYER_REFINE_LIST,            0x0221,  -1, &InventoryRecv::processPlayerRefineList,        20041129);
packet(SMSG_PLAYER_UPGRADE_MESSAGE,        0x0223,   8, &PlayerRecv::processPlayerUpgradeMessage,       20041129);

// 20050328
packet(SMSG_PLAYER_FAME_TAEKWON,           0x0224,  10, &PlayerRecv::processPlayerFameTaekwon,          20050328);
packet(SMSG_TAEKWON_RANKS_LIST,            0x0226, 282, &BeingRecv::processTaekwonRanksList,            20050328);

// 20050411
if (packetVersion >= 20050411)
{
    packet(SMSG_PLAYER_STATUS_CHANGE,      0x0229,  15, &BeingRecv::processPlayerStatusChange,          20050411);
}

// 20050523
packet(SMSG_HOMUNCULUS_DATA,               0x0230,  12, &HomunculusRecv::processHomunculusData,         20050523);

// 20050530
packet(SMSG_HOMUNCULUS_SKILLS,             0x0235,  -1, &HomunculusRecv::processHomunculusSkills,       20050530);
packet(SMSG_PK_RANKS_LIST,                 0x0238, 282, &BeingRecv::processPkRanksList,                 20050530);
packet(SMSG_HOMUNCULUS_INFO,               0x022e,  71, &HomunculusRecv::processHomunculusInfo,         20050530);

// 20050531
packet(SMSG_HOMUNCULUS_SKILL_UP,           0x0239,  11, &HomunculusRecv::processHomunculusSkillUp,      20050531);

// 20050608
packet(SMSG_HOMUNCULUS_FOOD,               0x022f,   5, &HomunculusRecv::processHomunculusFood,         20050608);
packet(SMSG_PLAYER_STORAGE_PASSWORD,       0x023a,   4, &InventoryRecv::processPlayerStoragePassword,   20050608);
packet(SMSG_PLAYER_STORAGE_PASSWORD_RESULT, 0x023c,  6, &InventoryRecv::processPlayerStoragePasswordResult, 20050608);

// 20050718
packet(SMSG_MAIL_READ_MAIL,                0x0242,  -1, &MailRecv::processReadMail,                     20050718);
packet(SMSG_MAIL_SEND_MAIL_ACK,            0x0249,   3, &MailRecv::processSendMailAck,                  20050718);
packet(SMSG_MAIL_NEW_MAIL,                 0x024a,  70, &MailRecv::processNewMail,                      20050718);
packet(SMSG_AUCTION_MESSAGE,               0x0250,   3, &AuctionRecv::processAuctionMessage,            20050718);
packet(SMSG_AUCTION_RESULTS,               0x0252,  -1, &AuctionRecv::processAuctionResults,            20050718);

// 20050801
packet(SMSG_MAIL_GET_ATTACHMENT,           0x0245,   3, &MailRecv::processGetAttachment,                20050801);

// 20050817
packet(SMSG_GLADIATOR_FEEL_REQUEST,        0x0253,   3, &BeingRecv::processGladiatorFeelRequest,        20050817);

// 20050829
packet(SMSG_MAIL_MAILS_LIST,               0x0240,  -1, &MailRecv::processMailList,                     20050829);
packet(SMSG_MAIL_SET_ATTACHMENT_ACK,       0x0255,   5, &MailRecv::processSetAttachmentAck,             20050829);
packet(SMSG_MAIL_DELETE_MAIL_ACK,          0x0257,   8, &MailRecv::processDeleteAck,                    20050829);

// 20050912
packet(SMSG_AUCTION_SET_ITEM,              0x0256,   5, &AuctionRecv::processAuctionSetItem,            20050912);

// 20051010
packet(SMSG_STARS_KILL,                    0x020e,  32, &BeingRecv::processStarsKill,                   20051010);
packet(SMSG_PLAYER_COOKING_LIST,           0x025a,  -1, &InventoryRecv::processPlayerCookingList,       20051010);

// 20051017
packet(SMSG_AUCTION_CLOSE,                 0x025d,   4, &AuctionRecv::processAuctionClose,              20051017);

// 20051024
packet(SMSG_AUCTION_OPEN_WINDOW,           0x025f,   6, &AuctionRecv::processOpenWindow,                20051024);
packet(SMSG_MAIL_OPEN_WINDOW,              0x0260,   6, &MailRecv::processMailOpen,                     20051024);

// 20060306
packet(SMSG_MAIL_RETURN,                   0x0274,   8, &MailRecv::processMailReturn,                   20060306);

// 20060424
if (packetVersion >= 20060424)
{
    packet(SMSG_MAP_ACCOUNT_ID,            0x0283,   6, &GameRecv::processMapAccountId,                 20060424);
    packet(SMSG_BEING_SPECIAL_EFFECT_NUM,  0x0284,  14, &BeingRecv::processBeingSpecialEffectNum,       20060424);
    packet(SMSG_NPC_CASH_SHOP_OPEN,        0x0287,  -1, &CashShopRecv::processCashShopOpen,             20060424);
    packet(SMSG_NPC_CASH_BUY_ACK,          0x0289,   8, &CashShopRecv::processCashShopBuyAck,           20060424);
    packet(SMSG_PLAYER_STATUS_CHANGE2,     0x028a,  18, &BeingRecv::processPlayerStatusChange2,         20060424);
    packet(SMSG_FORMAT_MESSAGE,            0x0291,   4, &ChatRecv::processFormatMessage,                20060424);
    packet(SMSG_BOSS_MAP_INFO,             0x0293,  70, &BeingRecv::processBossMapInfo,                 20060424);
    packet(SMSG_PLAYER_READ_BOOK,          0x0294,  10, &PlayerRecv::processPlayerReadBook,             20060424);
    packet(SMSG_PLAYER_ITEM_RENTAL_TIME,   0x0298,   8, &InventoryRecv::processPlayerItemRentalTime,    20060424);
    packet(SMSG_PLAYER_ITEM_RENTAL_EXPIRED, 0x0299,  6, &InventoryRecv::processPlayerItemRentalExpired, 20060424);
    packet(SMSG_MERCENARY_SKILLS,          0x029d,  -1, &MercenaryRecv::processMercenarySkills,         20060424);
    packet(SMSG_MERCENARY_UPDATE,          0x02a2,   8, &MercenaryRecv::processMercenaryUpdate,         20060424);
}

// 20061218
if (packetVersion >= 20061218)
{
    packet(SMSG_PLAYER_INVENTORY_ADD,      0x029a,  27, &InventoryRecv::processPlayerInventoryAdd,      20061218);
}

// 20070227
if (packetVersion >= 20070227)
{
    packet(SMSG_NPC_CASH_BUY_ACK,          0x0289,  12, &CashShopRecv::processCashShopBuyAck,           20070227);
    packet(SMSG_QUEST_LIST_OBJECTIVES,     0x02b2,  -1, &QuestRecv::processAddQuestsObjectives,         20070227);
    packet(SMSG_QUEST_ADD,                 0x02b3, 107, &QuestRecv::processAddQuest,                    20070227);
    packet(SMSG_QUEST_REMOVE,              0x02b4,   6, &QuestRecv::processRemoveQuest,                 20070227);
    packet(SMSG_QUEST_UPDATE_OBJECTIVES,   0x02b5,  -1, &QuestRecv::processUpdateQuestsObjectives,      20070227);
    packet(SMSG_QUEST_ACTIVATE,            0x02b7,   7, &QuestRecv::processActivateQuest,               20070227);
    packet(SMSG_ITEM_DAMAGED,              0x02bb,   8, &InventoryRecv::processItemDamaged,             20070227);
    packet(SMSG_COLOR_MESSAGE,             0x02c1,  -1, &ChatRecv::processColorChat,                    20070227);
    packet(SMSG_PARTY_INVITATION_STATS,    0x02c9,   3, &PartyRecv::processPartyInvitationStats,        20070227);
    packet(SMSG_INSTANCE_CREATE,           0x02cc,   4, &MapRecv::processInstanceCreate,                20070227);
    packet(SMSG_INSTANCE_DELETE,           0x02ce,  10, &MapRecv::processInstanceDelete,                20070227);
    packet(SMSG_PLAYER_EQUIP_TICK_ACK,     0x02d9,  10, &PlayerRecv::processPlayerEquipTickAck,         20070227);
    packet(SMSG_PLAYER_SHOW_EQUIP,         0x02da,   3, &PlayerRecv::processPlayerShowEquip,            20070227);
    packet(SMSG_BATTLE_CHAT_MESSAGE,       0x02dc,  -1, &ChatRecv::processBattleChatMessage,            20070227);
    packet(SMSG_BATTLE_EMBLEM,             0x02dd,  32, &BattleGroundRecv::processBattleEmblem,         20070227);
    packet(SMSG_BATTLE_UPDATE_SCORE,       0x02de,   6, &BattleGroundRecv::processBattleUpdateScore,    20070227);
    packet(SMSG_BATTLE_UPDATE_COORDS,      0x02df,  36, &BattleGroundRecv::processBattleUpdateCoords,   20070227);
    packet(SMSG_BATTLE_UPDATE_HP,          0x02e0,  34, &BattleGroundRecv::processBattleUpdateHp,       20070227);
}

// 20070821
packet(SMSG_PARTY_INVITE_RESPONSE,         0x02c5,  30, &PartyRecv::processPartyInviteResponse,         20070821);
packet(SMSG_PARTY_INVITED,                 0x02c6,  30, &PartyRecv::processPartyInvited,                20070821);

// 20071002
if (packetVersion >= 20071002)
{
    packet(SMSG_PLAYER_INVENTORY,          0x01ee,  -1, &InventoryRecv::processPlayerInventory,         20071002);
    packet(SMSG_PLAYER_CART_ITEMS,         0x01ef,  -1, &InventoryRecv::processPlayerCartItems,         20071002);
    packet(SMSG_PLAYER_STORAGE_ITEMS,      0x0295,  -1, &InventoryRecv::processPlayerStorage,           20071002);
    packet(SMSG_PLAYER_STORAGE_EQUIP,      0x0296,  -1, &InventoryRecv::processPlayerStorageEquip,      20071002);
    packet(SMSG_PLAYER_CART_EQUIP,         0x0297,  -1, &InventoryRecv::processPlayerCartEquip,         20071002);
    packet(SMSG_PARTY_ITEM_PICKUP,         0x02b8,  22, &PartyRecv::processPartyItemPickup,             20071002);
    packet(SMSG_PLAYER_INVENTORY_ADD,      0x02d4,  29, &InventoryRecv::processPlayerInventoryAdd,      20071002);
}

// 20071023
packet(SMSG_INSTANCE_START,                0x02cb,  65, &MapRecv::processInstanceStart,                 20071023);
packet(SMSG_INSTANCE_INFO,                 0x02cd,  71, &MapRecv::processInstanceInfo,                  20071023);

// 20071106
if (packetVersion >= 20071106)
{
    packet(SMSG_BEING_FAKE_NAME,           0x0078,  55, &BeingRecv::processBeingFakeName,               20071106);
    packet(SMSG_BEING_SPAWN_OUTDATED,      0x007c,  42, nullptr,                                        20071106);
    packet(SMSG_BEING_MOVE,                0x022c,  65, &BeingRecv::processBeingMove,                   20071106);
    packet(SMSG_MERCENARY_INFO,            0x029b,  80, &MercenaryRecv::processMercenaryInfo,           20071106);
}

// 20071113
packet(SMSG_BEING_ACTION2,                 0x02e1,  33, &BeingRecv::processBeingAction2,                20071113);

// 20080102
if (packetVersion >= 20080102)
{
    packet(SMSG_PLAYER_EQUIPMENT,          0x02d0,  -1, &InventoryRecv::processPlayerEquipment,         20080102);
    packet(SMSG_PLAYER_STORAGE_EQUIP,      0x02d1,  -1, &InventoryRecv::processPlayerStorageEquip,      20080102);
    packet(SMSG_PLAYER_CART_EQUIP,         0x02d2,  -1, &InventoryRecv::processPlayerCartEquip,         20080102);
    packet(SMSG_PLAYER_INVENTORY,          0x02e8,  -1, &InventoryRecv::processPlayerInventory,         20080102);
    packet(SMSG_PLAYER_CART_ITEMS,         0x02e9,  -1, &InventoryRecv::processPlayerCartItems,         20080102);
    packet(SMSG_PLAYER_STORAGE_ITEMS,      0x02ea,  -1, &InventoryRecv::processPlayerStorage,           20080102);
    packet(SMSG_MAP_LOGIN_SUCCESS,         0x02eb,  13, &GameRecv::processMapLogin,                     20080102);
    packet(SMSG_BEING_MOVE,                0x02ec,  67, &BeingRecv::processBeingMove,                   20080102);
    packet(SMSG_BEING_SPAWN,               0x02ed,  59, &BeingRecv::processBeingSpawn,                  20080102);
    packet(SMSG_BEING_VISIBLE,             0x02ee,  60, &BeingRecv::processBeingVisible,                20080102);
    packet(SMSG_BEING_FONT,                0x02ef,   8, &BeingRecv::processBeingFont,                   20080102);
}

// 20080318
packet(SMSG_NPC_SHOW_PROGRESS_BAR,         0x02f0,  10, &NpcRecv::processNpcShowProgressBar,            20080318);
packet(SMSG_NPC_PROGRESS_BAR_ABORT,        0x02f2,   2, &NpcRecv::processProgressBarAbort,              20080318);

// 20080827
if (packetVersion >= 20080827)
{
    packet(SMSG_BEING_SPAWN_OUTDATED,      0x007c,  44, nullptr,                                        20080827);
}

// 20081112
packet(SMSG_PLAYER_SKILL_COOLDOWN,         0x043d,   8, &SkillRecv::processSkillCoolDown,               20081112);

// 20081113
if (packetVersion >= 20081113)
{
    packet(SMSG_PLAYER_SKILL_COOLDOWN_LIST, 0x043e,  -1, &SkillRecv::processSkillCoolDownList,          20081113);
}

// 20081126
if (packetVersion >= 20081126)
{
    packet(SMSG_PET_STATUS,                0x01a2,  37, &PetRecv::processPetStatus,                     20081126);
}

// 20081210
packet(SMSG_AUTOSHADOW_SPELL_LIST,         0x0442,  -1, &PlayerRecv::processPlayerAutoShadowSpellList,  20081210);

// 20081217
packet(SMSG_BEING_MILLENIUM_SHIELD,        0x0440,  10, &BeingRecv::processBeingMilleniumShield,        20081217);
packet(SMSG_PLAYER_DELETE_SKILL,           0x0441,   4, &SkillRecv::processSkillDelete,                 20081217);

// 20090121
packet(SMSG_BEING_STATUS_CHANGE,           0x043f,  25, &BeingRecv::processBeingStatusChange,           20090121);

// 20090218
packet(SMSG_QUEST_NPC_EFFECT,              0x0446,  14, &QuestRecv::processNpcQuestEffect,              20090218);

// 20090603
packet(SMSG_PARTY_SETTINGS,                0x07d8,   8, &PartyRecv::processPartySettings,               20090603);
packet(SMSG_PLAYER_SHORTCUTS,              0x07d9, 268, &PlayerRecv::processPlayerShortcuts,            20090603);

// 20090715
packet(SMSG_PLAYER_UPDATE_SKILL,           0x07e1,  15, &SkillRecv::processSkillUpdate,                 20090715);

// 20090805
packet(SMSG_FORMAT_MESSAGE_NUMBER,         0x07e2,   8, &ChatRecv::processFormatMessageNumber,          20090805);

// 20090818
packet(SMSG_FORMAT_MESSAGE_SKILL,          0x07e6,   8, &ChatRecv::processFormatMessageSkill,           20090818);

// 20090922
packet(SMSG_SKILL_ITEM_LIST_WINDOW,        0x07e3,   6, &SkillRecv::processSkillItemListWindow,         20090922);
packet(SMSG_CHAR_CAPTCHA_NOT_SUPPORTED,    0x07e9,   5, &CharServerRecv::processCharCaptchaNotSupported, 20090922);

// 20091027
packet(SMSG_PLAYER_GET_EXP,                0x07f6,  14, &PlayerRecv::processPlayerGetExp,               20091027);

if (packetVersion >= 20091103)
{
    packet(SMSG_BEING_MOVE,                0x07f7,  -1, &BeingRecv::processBeingMove,                   20091103);
    packet(SMSG_BEING_SPAWN,               0x07f8,  -1, &BeingRecv::processBeingSpawn,                  20091103);
    packet(SMSG_BEING_VISIBLE,             0x07f9,  -1, &BeingRecv::processBeingVisible,                20091103);
}

// 20091104
if (packetVersion >= 20091104)
{
    packet(SMSG_WHISPER,                   0x0097,  -1, &ChatRecv::processWhisper,                      20091104);
}

// 20091117
packet(SMSG_PLAYER_INVENTORY_REMOVE2,      0x07fa,   8, &InventoryRecv::processPlayerInventoryRemove2,  20091117);

// 20091124
if (packetVersion >= 20091124)
{
    packet(SMSG_SKILL_CASTING,             0x07fb,  25, &BeingRecv::processSkillCasting,                20091124);
}

// 20091201
packet(SMSG_BATTLE_PLAY,                   0x07fe,  26, &BattleGroundRecv::processBattlePlay,           20091201);

// 20091222
packet(SMSG_BOOKING_REGISTER_ACK,          0x0803,   4, nullptr,                                        20091222);
packet(SMSG_BOOKING_SEARCH_ACK,            0x0805,  -1, nullptr,                                        20091222);

// 20091229
packet(SMSG_BOOKING_DELETE_ACK,            0x0807,   4, nullptr,                                        20091229);
packet(SMSG_BOOKING_INSERT_NOTIFY,         0x0809,  50, nullptr,                                        20091229);
packet(SMSG_BOOKING_UPDATE_NOTIFY,         0x080a,  18, nullptr,                                        20091229);
packet(SMSG_BOOKING_DELETE_NOTIFY,         0x080b,   6, nullptr,                                        20091229);

// 20100105
if (packetVersion >= 20100105)
{
    packet(SMSG_VENDING_ITEMS_LIST,        0x0800,  -1, &VendingRecv::processItemsList,                 20100105);
}

// 20100126
packet(SMSG_PLAYER_HP,                     0x080e,  14, &BeingRecv::processBeingHp,                     20100126);

// 20100223
packet(SMSG_TRADE_ITEM_ADD,                0x080f,  20, &TradeRecv::processTradeItemAdd,                20100223);

// 20100303
packet(SMSG_BUYINGSTORE_OPEN,              0x0810,   3, &BuyingStoreRecv::processBuyingStoreOpen,       20100303);

// 20100309
packet(SMSG_BUYINGSTORE_OWN_ITEMS,         0x0813,  -1, &BuyingStoreRecv::processBuyingStoreOwnItems,   20100309);
packet(SMSG_BUYINGSTORE_HIDE_BOARD,        0x0816,   6, &BuyingStoreRecv::processBuyingStoreHideBoard,  20100309);
packet(SMSG_BUYINGSTORE_ITEMS_LIST,        0x0818,  -1, &BuyingStoreRecv::processBuyingStoreItemsList,  20100309);
packet(SMSG_ELEMENTAL_INFO,                0x081d,  22, &ElementalRecv::processElementalInfo,           20100309);
packet(SMSG_ELEMENTAL_UPDATE_STATUS,       0x081e,   8, &ElementalRecv::processElementalUpdateStatus,   20100309);

// 20100420
packet(SMSG_BUYINGSTORE_CREATE_FAILED,     0x0812,   8, &BuyingStoreRecv::processBuyingStoreCreateFailed, 20100420);
packet(SMSG_BUYINGSTORE_SHOW_BOARD,        0x0814,  86, &BuyingStoreRecv::processBuyingStoreShowBoard,  20100420);
packet(SMSG_BUYINGSTORE_SELL_FAILED,       0x081a,   4, &BuyingStoreRecv::processBuyingStoreSellFailed, 20100420);
packet(SMSG_BUYINGSTORE_REPORT,            0x081b,  10, &BuyingStoreRecv::processBuyingStoreReport,     20100420);
packet(SMSG_BUYINGSTORE_DELETE_ITEM,       0x081c,  10, &BuyingStoreRecv::processBuyingStoreDeleteItem, 20100420);
packet(SMSG_BUYINGSTORE_SELLER_SELL_FAILED, 0x0824,   6, &BuyingStoreRecv::processBuyingStoreSellerSellFailed, 20100420);

// 20100601
packet(SMSG_SEARCHSTORE_SEARCH_ACK,        0x0836,   0, &SearchStoreRecv::processSearchAck,             20100601);
packet(SMSG_SEARCHSTORE_SEARCH_FAILED,     0x0837,   3, &SearchStoreRecv::processSearchFailed,          20100601);

// 20100608
if (packetVersion >= 20100608)
{
    packet(SMSG_SEARCHSTORE_OPEN,          0x083a,   4, &SearchStoreRecv::processSearchOpen,            20100608);
    packet(SMSG_SEARCHSTORE_CLICK_ACK,     0x083d,   6, &SearchStoreRecv::processSearchClickAck,        20100608);
}

// 20100629
if (packetVersion >= 20100629)
{
    packet(SMSG_PLAYER_EQUIPMENT,          0x02d0,  -1, &InventoryRecv::processPlayerEquipment,         20100629);
    packet(SMSG_PLAYER_STORAGE_EQUIP,      0x02d1,  -1, &InventoryRecv::processPlayerStorageEquip,      20100629);
    packet(SMSG_PLAYER_CART_EQUIP,         0x02d2,  -1, &InventoryRecv::processPlayerCartEquip,         20100629);
    packet(SMSG_PLAYER_EQUIP,              0x00aa,   9, &InventoryRecv::processPlayerEquip,             20100629);
}

// 20100701
if (packetVersion >= 20100701)
{
    packet(SMSG_SEARCHSTORE_OPEN,          0x083a,   5, &SearchStoreRecv::processSearchOpen,            20100701);
}

// 20100803
packet(SMSG_GUILD_EXPULSION,               0x0839,  66, &GuildRecv::processGuildExpulsion,              20100803);

// 20101124
if (packetVersion >= 20101124)
{
    packet(SMSG_BEING_MOVE,                0x0856,  -1, &BeingRecv::processBeingMove,                   20101124);
    packet(SMSG_BEING_VISIBLE,             0x0857,  -1, &BeingRecv::processBeingVisible,                20101124);
    packet(SMSG_BEING_SPAWN,               0x0858,  -1, &BeingRecv::processBeingSpawn,                  20101124);
    packet(SMSG_BEING_VIEW_EQUIPMENT,      0x0859,  -1, &BeingRecv::processBeingViewEquipment,          20101124);
}

// 20110718
if (packetVersion >= 20110718)
{
    packet(SMSG_SKILL_ENTRY,               0x08c7,  -1, &BeingRecv::processSkillEntry,                  20110718);
}

// 20111005
packet(SMSG_SKILL_SNAP,                    0x08d2,  10, &BeingRecv::processBeingSlide,                  20111005);

// 20111010
packet(CMSG_NAVIGATE_TO,                   0x08e2,  27, &BeingRecv::processNavigateTo,                  20111010);

// 20120000
packet(SMSG_LOGIN_ERROR2,                  0x083e,  26, &LoginRecv::processLoginError2,                 20120000);

// 20120221
if (packetVersion >= 20120221)
{
    packet(SMSG_BEING_VISIBLE,             0x0915,  -1, &BeingRecv::processBeingVisible,                20120221);
    packet(SMSG_BEING_MOVE,                0x0914,  -1, &BeingRecv::processBeingMove,                   20120221);
    packet(SMSG_BEING_SPAWN,               0x090f,  -1, &BeingRecv::processBeingSpawn,                  20120221);
}

// 20120229
if (packetVersion >= 20120229)
{
    packet(SMSG_MERGE_ITEM,                0x096d,  -1, &InventoryRecv::processMergeItem,               20120229);
    packet(SMSG_ACK_MERGE_ITEMS,           0x096f,   7, &InventoryRecv::processMergeItemResponse,       20120229);
}

// 20120410
packet(SMSG_BEING_CHARM,                   0x08cf,  10, &BeingRecv::processBeingCharm,                  20120410);
packet(SMSG_RECRUIT_REGISTER_ACK,          0x08e6,   4, nullptr,                                        20120410);
packet(SMSG_RECRUIT_SEARCH_ACK,            0x08e8,  -1, nullptr,                                        20120410);
packet(SMSG_RECRUIT_DELETE_ACK,            0x08ea,   4, nullptr,                                        20120410);
packet(SMSG_RECRUIT_INSERT_NOTIFY,         0x08ec,  73, nullptr,                                        20120410);
packet(SMSG_RECRUIT_UPDATE_NOTIFY,         0x08ed,  43, nullptr,                                        20120410);
packet(SMSG_RECRUIT_DELETE_NOTIFY,         0x08ee,   6, nullptr,                                        20120410);
packet(SMSG_BOOKING_VOLUNTEER_INFO,        0x08f2,  36, nullptr,                                        20120410);
packet(SMSG_BOOKING_REFUSE_VOLUNTEER,      0x08fa,   6, nullptr,                                        20120410);
packet(SMSG_PLAYER_FAVORITE_ITEM,          0x0908,   5, &InventoryRecv::processFavoriteItem,            20120410);
packet(SMSG_MONSTER_HP,                    0x0977,  14, &BeingRecv::processMonsterHp,                   20120410);

// 20120419
if (packetVersion >= 20120419)
{
    packet(SMSG_CHAR_CHARACTERS,           0x099d,  -1, &CharServerRecv::processCharCharacters,         20120419);
}

// 20120502
if (packetVersion >= 20120502)
{
    packet(SMSG_RANKS_LIST,                0x097d, 288, &BeingRecv::processRanksList,                   20120502);
}

// 20120618
if (packetVersion >= 20120618)
{
    packet(SMSG_BEING_STATUS_CHANGE,       0x0983,  29, &BeingRecv::processBeingStatusChange,           20120618);
}

// 20120604
if (packetVersion >= 20120604)
{
    packet(SMSG_PLAYER_SKILL_COOLDOWN_LIST, 0x0985,  -1, &SkillRecv::processSkillCoolDownList,           20120604);
}

// 20120702
if (packetVersion >= 20120702)
{
    packet(SMSG_CLAN_INFO,                 0x098a,  -1, &ClanRecv::processClanInfo,                     20120702);
}

// 20120716
if (packetVersion >= 20120716)
{
    packet(SMSG_CLAN_ONLINE_COUNT,         0x0988,   6, &ClanRecv::processClanOnlineCount,              20120716);
    packet(SMSG_CLAN_CHAT,                 0x098e,  -1, &ClanRecv::processClanChat,                     20120716);
}

// 20120925
if (packetVersion >= 20120925)
{
    packet(SMSG_PLAYER_INVENTORY_ADD,      0x0990,  31, &InventoryRecv::processPlayerInventoryAdd,      20120925);
    packet(SMSG_PLAYER_INVENTORY,          0x0991,  -1, &InventoryRecv::processPlayerInventory,         20120925);
    packet(SMSG_PLAYER_EQUIPMENT,          0x0992,  -1, &InventoryRecv::processPlayerEquipment,         20120925);
    packet(SMSG_PLAYER_CART_ITEMS,         0x0993,  -1, &InventoryRecv::processPlayerCartItems,         20120925);
    packet(SMSG_PLAYER_CART_EQUIP,         0x0994,  -1, &InventoryRecv::processPlayerCartEquip,         20120925);
    packet(SMSG_PLAYER_STORAGE_ITEMS,      0x0995,  -1, &InventoryRecv::processPlayerStorage,           20120925);
    packet(SMSG_PLAYER_STORAGE_EQUIP,      0x0996,  -1, &InventoryRecv::processPlayerStorageEquip,      20120925);
    packet(SMSG_BEING_VIEW_EQUIPMENT,      0x0997,  -1, &BeingRecv::processBeingViewEquipment,          20120925);
    packet(SMSG_PLAYER_EQUIP,              0x0999,  11, &InventoryRecv::processPlayerEquip,             20120925);
    packet(SMSG_PLAYER_UNEQUIP,            0x099a,   9, &InventoryRecv::processPlayerUnEquip,           20120925);
}

// 20130821
packet(SMSG_SHOW_PROGRESS_BAR_FREE,        0x09d1,  14, &NpcRecv::processShowProgressBarFree,           20130821);

// 20121010
packet(SMSG_MAP_TYPE_PROPERTY2,            0x099b,   8, &BeingRecv::processMapTypeProperty,             20121010);

// 20121212
if (packetVersion >= 20121212)
{
    packet(SMSG_SKILL_ENTRY,               0x099f,  -1, &BeingRecv::processSkillEntry,                  20121212);
}

// 20130000
if (packetVersion >= 20130000)
{
    packet(SMSG_ITEM_DROPPED,              0x084b,  19, &ItemRecv::processItemDropped,                  20130000);
}

// 20130710
packet(SMSG_PLAYER_RANK_POINTS,            0x097e,  12, &PlayerRecv::processPlayerRankPoints,           20130710);

// 20130724
packet(SMSG_BANK_STATUS,                   0x09a6,  12, &BankRecv::processBankStatus,                   20130724);
packet(SMSG_BANK_DEPOSIT,                  0x09a8,  16, &BankRecv::processBankDeposit,                  20130724);
packet(SMSG_BANK_WITHDRAW,                 0x09aa,  16, &BankRecv::processBankWithdraw,                 20130724);

// 20130731
if (packetVersion >= 20130731)
{
    packet(SMSG_SKILL_ENTRY,               0x09ca,  -1, &BeingRecv::processSkillEntry,                  20130731);
}

// 20131218
packet(SMSG_MAIL2_MAIL_LIST_PAGE,          0x09f0,  -1, &Mail2Recv::processMailListPage,                20131218);
packet(SMSG_MAIL2_MAIL_DELETE,             0x09f6,  11, &Mail2Recv::processMailDelete,                  20131218);

// 20131223
if (packetVersion >= 20131223)
{
    packet(SMSG_BEING_ACTION2,             0x08c8,  34, &BeingRecv::processBeingAction2,                20131223);
    packet(SMSG_NPC_MARKET_OPEN,           0x09d5,  -1, &MarketRecv::processMarketOpen,                 20131223);
    packet(SMSG_NPC_MARKET_BUY_ACK,        0x09d7,  -1, &MarketRecv::processMarketBuyAck,               20131223);
    packet(SMSG_BEING_MOVE,                0x09db,  -1, &BeingRecv::processBeingMove,                   20131223);
    packet(SMSG_BEING_SPAWN,               0x09dc,  -1, &BeingRecv::processBeingSpawn,                  20131223);
    packet(SMSG_BEING_VISIBLE,             0x09dd,  -1, &BeingRecv::processBeingVisible,                20131223);
    packet(SMSG_WHISPER_RESPONSE,          0x09df,   7, &ChatRecv::processWhisperResponse,              20131223);
    packet(SMSG_SKILL_NO_DAMAGE,           0x09cb,  17, &Ea::BeingRecv::processSkillNoDamage,           20131223);
    packet(SMSG_CLAN_LEAVE,                0x0989,   2, &ClanRecv::processClanLeave,                    20131223);
}

// 20131230
packet(SMSG_MAIL2_SEND_RESULT,             0x09ed,   3, &Mail2Recv::processSendResult,                  20131230);

// 20140409
packet(SMSG_MAIL2_REQUEST_MONEY,           0x09f2,  12, &Mail2Recv::processRequestMoney,                20140409);
packet(SMSG_MAIL2_REQUEST_ITEMS,           0x09f4,  12, &Mail2Recv::processRequestItems,                20140409);

// 20140115
packet(SMSG_MAIL2_READ_MAIL,               0x09eb,  -1, &Mail2Recv::processReadMail,                    20140115);

// 20140416
packet(SMSG_MAIL2_OPEN_NEW_MAIL_WINDOW,    0x0a12,  27, &Mail2Recv::processOpenNewMailWindow,           20140416);

// 20140521
packet(SMSG_MAIL2_REMOVE_ITEM_RESULT,      0x0a07,   9, &Mail2Recv::processRemoveItemResult,            20140521);
packet(SMSG_MAIL2_CHECK_NAME_RESULT,       0x0a14,  10, &Mail2Recv::processCheckNameResult,             20140521);

// 20140613
if (packetVersion >= 20140613)
{
    packet(SMSG_BATTLE_UPDATE_HP,          0x0a0e,  14, &BattleGroundRecv::processBattleUpdateHp,       20140613);
}

// 20141016
packet(SMSG_ROULETTE_OPEN_ACK,             0x0a1a,  23, &RouletteRecv::processRouletteOpenAck,          20141016);
packet(SMSG_ROULETTE_INFO_ACK_TYPE,        0x0a1c,  -1, &RouletteRecv::processRouletteInfoAckType,      20141016);
packet(SMSG_ROULETTE_GENERATE_ACK_TYPE,    0x0a20,  21, &RouletteRecv::processRouletteGenerateAckType,  20141016);
packet(SMSG_ROULETTE_RECV_ITEM_ACK,        0x0a22,   5, &RouletteRecv::processRouletteItemAck,          20141016);
if (packetVersion >= 20141016)
{
    packet(SMSG_VENDING_REPORT,            0x09e5,  18, &VendingRecv::processReport,                    20141016);
    packet(SMSG_BUYINGSTORE_REPORT,        0x09e6,  22, &BuyingStoreRecv::processBuyingStoreReport,     20141016);
}

// 20141022
if (packetVersion >= 20141022)
{
    packet(SMSG_QUEST_LIST,                0x097a,  -1, &QuestRecv::processAddQuests,                   20141022);
    packet(SMSG_PLAYER_SHORTCUTS,          0x0a00, 269, &PlayerRecv::processPlayerShortcuts,            20141022);
    packet(SMSG_MAP_LOGIN_SUCCESS,         0x0a18,  14, &GameRecv::processMapLogin,                     20141022);
    packet(SMSG_VENDING_OPEN_STATUS,       0x0a28,   3, &VendingRecv::processOpenStatus,                20141022);
}

// 20141112
packet(SMSG_MAIL2_ICON,                    0x09e7,   3, &Mail2Recv::processMailIcon,                    20141112);

// 20141119
packet(SMSG_MAIL2_ADD_ITEM_RESULT,         0x0a05,  53, &Mail2Recv::processAddItemResult,               20141119);

// 0
// evol packets
if (serverVersion > 0)
{
    packet2(SMSG_NPC_COMMAND,              0x0b00,  16, &Ea::NpcRecv::processNpcCommand,                0);
    packet2(SMSG_BEING_NAME_RESPONSE2,     0x0b01,  -1, &BeingRecv::processNameResponse2,               0);
    packet2(SMSG_MAP_MASK,                 0x0b02,  10, &Ea::PlayerRecv::processMapMask,                0);
    packet2(SMSG_MOB_INFO,                 0x0b03,  -1, &BeingRecv::processMobInfo,                     0);
    packet2(SMSG_BEING_MOVE3,              0x0b04,  -1, &Ea::BeingRecv::processBeingMove3,              0);
    packet2(SMSG_MAP_MUSIC,                0x0b05,  -1, &Ea::PlayerRecv::processMapMusic,               0);
    packet2(SMSG_NPC_CHANGETITLE,          0x0b06,  -1, &Ea::NpcRecv::processChangeTitle,               0);
    packet2(SMSG_CHAT_JOIN_CHANNEL,        0x0b08,  27, &ChatRecv::processJoinChannel,                  0);
    packet2(SMSG_BEING_ATTRS,              0x0b0a,  -1, &BeingRecv::processBeingAttrs,                  0);
    packet2(SMSG_NPC_AREA,                 0x0b0b,  -1, &NpcRecv::processArea,                          0);
    packet2(SMSG_ONLINE_LIST,              0x0b10,  -1, &PlayerRecv::processOnlineList,                 0);
    packet2(SMSG_PLAYER_CLIENT_COMMAND,    0x0b16,  -1, &Ea::PlayerRecv::processPlayerClientCommand,    0);
    packet2(SMSG_BEING_CHANGE_LOOKS_CARDS, 0x0b17,  19, &BeingRecv::processBeingChangeLookCards,        0);
    packet2(SMSG_ITEM_VISIBLE2,            0x0b18,  28, &ItemRecv::processItemVisible2,                 0);
    packet2(SMSG_ITEM_DROPPED2,            0x0b19,  28, &ItemRecv::processItemDropped2,                 0);
    packet2(SMSG_BATTLE_EMBLEM2,           0x0b1a,  34, &BattleGroundRecv::processBattleEmblem2,        0);
    packet2(SMSG_MAP_SET_TILES_TYPE,       0x0b1b,  34, &Ea::MapRecv::processSetTilesType,              0);
    packet2(SMSG_NPC_SKIN,                 0x0b1c,  -1, &NpcRecv::processNpcSkin,                       0);
    packet2(SMSG_PLAYER_KILLED_BY,         0x0b1d,   6, &PlayerRecv::processKilledBy,                   0);
    packet2(SMSG_SKILL_CASTING2,           0x0b1e,  -1, &BeingRecv::processSkillCasting2,               0);
    packet2(SMSG_PLAYER_ADD_SKILL2,        0x0b1f,  -1, &SkillRecv::processSkillAdd2,                   0);
    packet2(SMSG_PLAYER_UPDATE_SKILL2,     0x0b20,  -1, &SkillRecv::processSkillUpdate2,                0);
    packet2(SMSG_WALK_ERROR,               0x0b21,  10, &PlayerRecv::processWalkError,                  0);
    packet2(SMSG_HOMUNCULUS_EXP,           0x0b22,  10, &HomunculusRecv::processHomunculusExp,          0);
    packet2(SMSG_QUEST_LIST2,              0x0b23,  -1, &QuestRecv::processAddQuests2,                  0);
    packet2(SMSG_QUEST_ADD2,               0x0b24,  23, &QuestRecv::processAddQuest2,                   0);
    packet2(SMSG_PLAYER_ATTRS,             0x0b25,  -1, &PlayerRecv::processPlayerAttrs,                0);

    // char server
    packet2(SMSG_CHAR_PASSWORD_RESPONSE,   0x0062,   3, &LoginRecv::processCharPasswordResponse,        0);
}

// 20150226
if ((serverVersion >= 8 || serverVersion == 0) && packetVersion >= 20150226)
{
    packet(SMSG_VENDING_OPEN,              0x0136,  -1, &VendingRecv::processOpen,                      20150226);
    packet(SMSG_VENDING_ITEMS_LIST,        0x0800,  -1, &VendingRecv::processItemsList,                 20150226);
    packet(SMSG_TRADE_ITEM_ADD,            0x0a09,  45, &TradeRecv::processTradeItemAdd,                20150226);
    packet(SMSG_PLAYER_STORAGE_ADD,        0x0a0a,  47, &InventoryRecv::processPlayerStorageAdd,        20150226);
    packet(SMSG_PLAYER_CART_ADD,           0x0a0b,  47, &InventoryRecv::processPlayerCartAdd,           20150226);
    packet(SMSG_PLAYER_INVENTORY_ADD,      0x0a0c,  56, &InventoryRecv::processPlayerInventoryAdd,      20150226);
    packet(SMSG_PLAYER_EQUIPMENT,          0x0a0d,  -1, &InventoryRecv::processPlayerEquipment,         20150226);
    packet(SMSG_PLAYER_CART_EQUIP,         0x0a0f,  -1, &InventoryRecv::processPlayerCartEquip,         20150226);
    packet(SMSG_PLAYER_STORAGE_EQUIP,      0x0a10,  -1, &InventoryRecv::processPlayerStorageEquip,      20150226);
    packet(SMSG_BEING_VIEW_EQUIPMENT,      0x0a2d,  -1, &BeingRecv::processBeingViewEquipment,          20150226);
}

// 20150513
if (packetVersion >= 20150513)
{
    packet(SMSG_DRESS_ROOM_OPEN,           0x0a02,   4, &PlayerRecv::processDressRoomOpen,              20150513);
    packet(SMSG_BEING_VISIBLE,             0x09ff,  -1, &BeingRecv::processBeingVisible,                20150513);
    packet(SMSG_BEING_SPAWN,               0x09fe,  -1, &BeingRecv::processBeingSpawn,                  20150513);
    packet(SMSG_BEING_MOVE,                0x09fd,  -1, &BeingRecv::processBeingMove,                   20150513);
    packet(SMSG_PLAYER_HEAL,               0x0a27,   8, &PlayerRecv::processPlayerHeal,                 20150513);
    packet(SMSG_HOMUNCULUS_INFO,           0x09f7,  75, &HomunculusRecv::processHomunculusInfo,         20150513);
}

// 20150805
if (packetVersion >= 20150805)
{
    packet(SMSG_SELECT_CART,               0x097f,  -1, &InventoryRecv::processSelectCart,              20150805);
}

// 20151223
if (packetVersion >= 20151223)
{
    packet(SMSG_SKILL_SCALE,               0x0a41,  18, &SkillRecv::processSkillScale,                  20151223);
}

// 20160330
if (packetVersion >= 20160330)
{
    packet(SMSG_MAP_LOGIN_SUCCESS,         0x02eb,  13, &GameRecv::processMapLogin,                     20160330);
}

// 20160622
if (packetVersion >= 20160622)
{
    packet(SMSG_GUILD_BASIC_INFO,          0x0a84,  94, &GuildRecv::processGuildBasicInfo,              20160622);
}

// 20160921
if (packetVersion >= 20160921)
{
    packet(SMSG_PLAYER_INVENTORY_ADD,      0x0a37,  59, &InventoryRecv::processPlayerInventoryAdd,      20160921);
    packet(SMSG_VENDING_ITEMS_LIST,        0x0800,  -1, &VendingRecv::processItemsList,                 20160921);
}

// 20161026
if (packetVersion >= 20161026)
{
    packet(SMSG_GUILD_MEMBER_LIST,         0x0aa5,  -1, &GuildRecv::processGuildMemberList,             20161026);
}

// 20170315
if (packetVersion >= 20170315)
{
    packet(SMSG_LOGIN_DATA,                0x0ac4,  -1, &LoginRecv::processLoginData,                   20170315);
}

// 20160316
if (packetVersion >= 20160316)
{
    packet(SMSG_MAIL2_CHECK_NAME_RESULT,   0x0a51,  34, &Mail2Recv::processCheckNameResult,             20160316);
}

// 20160601
if (packetVersion >= 20160601)
{
    packet(SMSG_MAIL2_MAIL_LIST_PAGE,      0x0a7d,  -1, &Mail2Recv::processMailListPage,                20160601);
}

// 20170329
if (packetVersion >= 20170329)
{
    packet(SMSG_CHAR_MAP_INFO,             0x0ac5, 156, &CharServerRecv::processCharMapInfo,            20170329);
}

// 20170419
if (packetVersion >= 20170419)
{
    packet(SMSG_MAIL2_MAIL_LIST_PAGE,      0x0ac2,  -1, &Mail2Recv::processMailListPage,                20170419);
}

// 20170502
if (packetVersion >= 20170502)
{
    packet(SMSG_PARTY_MEMBER_INFO,         0x0a43,  85, &PartyRecv::processPartyMemberInfo,             20170502);
    packet(SMSG_PARTY_INFO,                0x0a44,  -1, &PartyRecv::processPartyInfo,                   20170502);
    packet(SMSG_PARTY_MEMBER_JOB_LEVEL,    0x0abd,  10, &PartyRecv::processPartyMemberJobLevel,         20170502);
}

// 20170315
if (packetVersion == 20170315)
{
    packet(SMSG_CHANGE_MAP_SERVER,         0x0ac7, 156, &CharServerRecv::processChangeMapServer,        20170315);
}

// 20170830
packet(SMSG_PLAYER_STAT_UPDATE_7,          0x0acb,  12, &PlayerRecv::processPlayerStatUpdate7,          20170830);
packet(SMSG_PLAYER_GET_EXP2,               0x0acc,  18, &PlayerRecv::processPlayerGetExp2,              20170830);

// zero 20171018
if (packets_zero == true)
{
    packet(SMSG_ITEM_DROPPED,              0x0add,  22, &ItemRecv::processItemDropped,                  20171018);
}

// 20171207
if (packetVersion >= 20171207)
{
    packet(SMSG_PARTY_MEMBER_INFO,         0x0ae4,  89, &PartyRecv::processPartyMemberInfo,             20171207);
    packet(SMSG_PARTY_INFO,                0x0ae5,  -1, &PartyRecv::processPartyInfo,                   20171207);
}

// re 20180221
if (packets_re == true)
{
if (packetVersion >= 20180221)
{
    packet(SMSG_SOLVE_CHAR_NAME,           0x0af7,  32, &BeingRecv::processSolveCharName,               20180221);
}
}

// main 20180307
if (packets_main == true)
{
if (packetVersion >= 20180307)
{
    packet(SMSG_SOLVE_CHAR_NAME,           0x0af7,  32, &BeingRecv::processSolveCharName,               20180307);
}
}

// 0
// evol always packets
packet(SMSG_SERVER_VERSION_RESPONSE,       0x7531,  -1, &LoginRecv::processServerVersion,               0);
packet(SMSG_UPDATE_HOST,                   0x0063,  -1, &Ea::LoginRecv::processUpdateHost,              0);
