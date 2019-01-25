/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

// 0
packet(CMSG_SERVER_VERSION_REQUEST,       0x7530,   0, nullptr);

packet(CMSG_LOGIN_REGISTER,               0x0064,   0, lclif->p->parse_CA_LOGIN);
packet(CMSG_LOGIN_REGISTER2,              0x01dd,   0, lclif->p->parse_CA_LOGIN2);
packet(CMSG_LOGIN_REGISTER3,              0x01fa,   0, lclif->p->parse_CA_LOGIN3);
packet(CMSG_LOGIN_REGISTER4,              0x027c,   0, lclif->p->parse_CA_LOGIN4);
packet(CMSG_LOGIN_REGISTER_PCBANG,        0x0277,   0, lclif->p->parse_CA_LOGIN_PCBANG);
packet(CMSG_LOGIN_REGISTER_HAN,           0x02b0,   0, lclif->p->parse_CA_LOGIN_HAN);
packet(CMSG_LOGIN_REGISTER_SSO,           0x0825,  -1, lclif->p->parse_CA_SSO_LOGIN_REQ);
packet(CMSG_LOGIN_REGISTER_KEY,           0x01db,   0, lclif->p->parse_CA_REQ_HASH);

packet(CMSG_NAME_REQUEST,                 0x0094,   6, clif->pGetCharNameRequest);

packet(CMSG_CHAR_PASSWORD_CHANGE,         0x0061,   0, nullptr);
packet(CMSG_CHAR_SERVER_CONNECT,          0x0065,   0, chr->parse_char_connect);
packet(CMSG_CHAR_SELECT,                  0x0066,   3, chr->parse_char_select);
packet(CMSG_CHAR_CREATE,                  0x0067,   0, chr->parse_make_char_0067);
packet(CMSG_CHAR_DELETE,                  0x0068,  56, chr->parse_char_delete_char_0068);
packet(CMSG_CHAR_DELETE2_REQ,             0x0827,   6, chr->parse_char_delete2_req);
packet(CMSG_CHAR_DELETE2_ACCEPT,          0x0829,  12, chr->parse_char_delete2_accept);
packet(CMSG_CHAR_DELETE2_CANCEL,          0x082b,   6, chr->parse_char_delete2_cancel);
packet(CMSG_CHAR_CREATE_PIN,              0x08ba,  10, chr->parse_char_pincode_first_pin);
packet(CMSG_CHAR_PIN_CHECK,               0x08b8,  10, chr->parse_char_pincode_check);
packet(CMSG_CHAR_PIN_CHANGE,              0x08be,  14, chr->parse_char_pincode_change);
packet(CMSG_CHAR_PIN_WINDOW,              0x08c5,   6, chr->parse_char_pincode_window);
packet(CMSG_CHAR_REQUEST_CHARS,           0x09a1,   2, chr->parse_char_request_chars);
packet(CMSG_CHAR_CHECK_RENAME,            0x08fc,  30, chr->parse_char_rename_char);
packet(CMSG_CHAR_CHECK_RENAME2,           0x028d,  34, chr->parse_char_rename_char2);
packet(CMSG_CHAR_RENAME,                  0x028f,   6, chr->parse_char_rename_char_confirm);
packet(CMSG_CHAR_CHANGE_SLOT,             0x08d4,   8, chr->parse_char_move_character);
packet(CMSG_CHAR_REQUEST_CAPTCHA,         0x07e5,   8, chr->parse_char_request_captcha);
packet(CMSG_CHAR_CHECK_CAPTCHA,           0x07e7,  32, chr->parse_char_check_captcha);

packet(CMSG_MAP_SERVER_CONNECT,           0x0072,  19, clif->pWantToConnection);
packet(CMSG_MAP_PING,                     0x007e,   6, clif->pTickSend);
packet(CMSG_LOGIN_PING,                   0x0200,   0, lclif->p->parse_CA_CONNECT_INFO_CHANGED);
packet(CMSG_LOGIN_HASH_CHECK,             0x0204,   0, lclif->p->parse_CA_EXE_HASHCHECK);

packet(CMSG_CHAR_PING,                    0x0187,  56, chr->parse_char_ping);
packet(CMSG_MAP_LOADED,                   0x007d,   2, clif->pLoadEndAck);
packet(CMSG_CLIENT_QUIT,                  0x018A,   4, clif->pQuitGame);

packet(CMSG_CHAT_MESSAGE,                 0x008c,  -1, clif->pGlobalMessage);
packet(CMSG_CHAT_WHISPER,                 0x0096,  -1, clif->pWisMessage);
packet(CMSG_CHAT_ROOM_JOIN,               0x00d9,  14, clif->pChatAddMember);
packet2(CMSG_CHAT_JOIN_CHANNEL,           0x0b07,   0, nullptr);
packet2(CMSG_CHAT_PART_CHANNEL,           0x0b09,   0, nullptr);
packet(CMSG_BATTLE_CHAT_MESSAGE,          0x02db,  -1, clif->pBattleChat);

packet(CMSG_CREAYE_CHAT_ROOM,             0x00d5,  -1, clif->pCreateChatRoom);
packet(CMSG_LEAVE_CHAT_ROOM,              0x00e3,   2, clif->pChatLeave);
packet(CMSG_SET_CHAT_ROOM_OPTIONS,        0x00de,  -1, clif->pChatRoomStatusChange);
packet(CMSG_SET_CHAT_ROOM_OWNER,          0x00e0,  30, clif->pChangeChatOwner);
packet(CMSG_KICK_FROM_CHAT_ROOM,          0x00e2,  26, clif->pKickFromChat);

packet(CMSG_SKILL_LEVELUP_REQUEST,        0x0112,   4, clif->pSkillUp);
packet(CMSG_STAT_UPDATE_REQUEST,          0x00bb,   5, clif->pStatusUp);
packet(CMSG_SKILL_USE_BEING,              0x0113,  10, clif->pUseSkillToId);
packet(CMSG_SKILL_USE_POSITION,           0x0116,  10, clif->pUseSkillToPos);
packet(CMSG_SKILL_USE_POSITION_MORE,      0x0190,  90, clif->pUseSkillToPosMoreInfo);
packet(CMSG_SKILL_USE_MAP,                0x011b,  20, clif->pUseSkillMap);

packet(CMSG_PLAYER_INVENTORY_USE,         0x0439,   8, clif->pUseItem);
packet(CMSG_PLAYER_INVENTORY_DROP,        0x00a2,   6, clif->pDropItem);
packet(CMSG_PLAYER_EQUIP,                 0x00a9,   6, clif->pEquipItem);
packet(CMSG_PLAYER_UNEQUIP,               0x00ab,   4, clif->pUnequipItem);
packet(CMSG_PLAYER_USE_CARD,              0x017a,   4, clif->pUseCard);
packet(CMSG_PLAYER_INSERT_CARD,           0x017c,   6, clif->pInsertCard);
packet(CMSG_PLAYER_VIEW_EQUIPMENT,        0x02d6,   6, clif->pViewPlayerEquip);
packet(CMSG_SET_CONFIG_OPTION,            0x02d8,  10, clif->pEquipTick);

packet(CMSG_ITEM_PICKUP,                  0x009f,   6, clif->pTakeItem);
packet(CMSG_PLAYER_CHANGE_DIR,            0x009b,   5, clif->pChangeDir);
packet(CMSG_PLAYER_CHANGE_DEST,           0x0085,   5, clif->pWalkToXY);
packet(CMSG_PLAYER_CHANGE_ACT,            0x0089,   7, clif->pActionRequest);
packet(CMSG_PLAYER_RESTART,               0x00b2,   3, clif->pRestart);
packet(CMSG_PLAYER_EMOTE,                 0x00bf,   3, clif->pEmotion);
packet(CMSG_PLAYER_STOP_ATTACK,           0x0118,   2, clif->pStopAttack);
packet(CMSG_WHO_REQUEST,                  0x00c1,   2, clif->pHowManyConnections);

packet(CMSG_NPC_TALK,                     0x0090,   7, clif->pNpcClicked);
packet(CMSG_NPC_NEXT_REQUEST,             0x00b9,   6, clif->pNpcNextClicked);
packet(CMSG_NPC_CLOSE,                    0x0146,   6, clif->pNpcCloseClicked);
packet(CMSG_NPC_LIST_CHOICE,              0x00b8,   7, clif->pNpcSelectMenu);
packet(CMSG_NPC_INT_RESPONSE,             0x0143,  10, clif->pNpcAmountInput);
packet(CMSG_NPC_STR_RESPONSE,             0x01d5,  -1, clif->pNpcStringInput);
packet(CMSG_NPC_BUY_SELL_REQUEST,         0x00c5,   7, clif->pNpcBuySellSelected);
packet(CMSG_NPC_BUY_REQUEST,              0x00c8,  -1, clif->pNpcBuyListSend);
packet(CMSG_NPC_SELL_REQUEST,             0x00c9,  -1, clif->pNpcSellListSend);

packet(CMSG_TRADE_REQUEST,                0x00e4,   6, clif->pTradeRequest);
packet(CMSG_TRADE_RESPONSE,               0x00e6,   3, clif->pTradeAck);
packet(CMSG_TRADE_ITEM_ADD_REQUEST,       0x00e8,   8, clif->pTradeAddItem);
packet(CMSG_TRADE_CANCEL_REQUEST,         0x00ed,   2, clif->pTradeCancel);
packet(CMSG_TRADE_ADD_COMPLETE,           0x00eb,   2, clif->pTradeOk);
packet(CMSG_TRADE_OK,                     0x00ef,   2, clif->pTradeCommit);

packet(CMSG_PARTY_CREATE,                 0x00f9,  26, clif->pCreateParty);
packet(CMSG_PARTY_CREATE2,                0x01e8,  28, clif->pCreateParty2);
packet(CMSG_PARTY_INVITE,                 0x00fc,   6, clif->pPartyInvite);
packet(CMSG_PARTY_INVITED,                0x00ff,  10, clif->pReplyPartyInvite);
packet(CMSG_PARTY_INVITED2,               0x02c7,   7, clif->pReplyPartyInvite2);
packet(CMSG_PARTY_LEAVE,                  0x0100,   2, clif->pLeaveParty);
packet(CMSG_PARTY_SETTINGS,               0x0102,   6, clif->pPartyChangeOption);
packet(CMSG_PARTY_KICK,                   0x0103,  30, clif->pRemovePartyMember);
packet(CMSG_PARTY_MESSAGE,                0x0108,  -1, clif->pPartyMessage);
packet(CMSG_PARTY_CHANGE_LEADER,          0x07da,   6, clif->pPartyChangeLeader);
packet(CMSG_PARTY_ALLOW_INVITES,          0x02c8,   3, clif->pPartyTick);

packet(CMSG_MOVE_TO_STORAGE,              0x00f3,   8, clif->pMoveToKafra);
packet(CMSG_MOVE_FROM_STORAGE,            0x00f5,   8, clif->pMoveFromKafra);
packet(CMSG_CLOSE_STORAGE,                0x00f7,   2, clif->pCloseKafra);

packet(CMSG_MOVE_TO_CART,                 0x0126,   8, clif->pPutItemToCart);
packet(CMSG_MOVE_FROM_CART,               0x0127,   8, clif->pGetItemFromCart);
packet(CMSG_CHANGE_CART,                  0x01af,   4, clif->pChangeCart);
packet(CMSG_MOVE_FROM_STORAGE_TO_CART,    0x0128,   8, clif->pMoveFromKafraToCart);
packet(CMSG_MOVE_FROM_CART_TO_STORAGE,    0x0129,   8, clif->pMoveToKafraFromCart);

packet(CMSG_ADMIN_ANNOUNCE,               0x0099,  -1, clif->pBroadcast);
packet(CMSG_ADMIN_LOCAL_ANNOUNCE,         0x019C,  -1, clif->pLocalBroadcast);
packet(CMSG_ADMIN_HIDE,                   0x019D,   6, clif->pGMHide);
packet(CMSG_ADMIN_KICK,                   0x00CC,   6, clif->pGMKick);
packet(CMSG_ADMIN_KICK_ALL,               0x00ce,   2, clif->pGMKickAll);
packet(CMSG_ADMIN_RESET_PLAYER,           0x0197,   4, clif->pResetChar);
packet(CMSG_ADMIN_GOTO,                   0x01bb,  26, clif->pGMShift);
packet(CMSG_ADMIN_RECALL,                 0x01bd,  26, clif->pGMRecall);
packet(CMSG_ADMIN_MUTE,                   0x0149,   9, clif->pGMReqNoChat);
packet(CMSG_ADMIN_MUTE_NAME,              0x0212,  26, clif->pGMRc);
packet(CMSG_ADMIN_ID_TO_LOGIN,            0x01df,   6, clif->pGMReqAccountName);
packet(CMSG_ADMIN_SET_TILE_TYPE,          0x0198,   8, clif->pGMChangeMapType);
packet(CMSG_ADMIN_UNEQUIP_ALL,            0x07f5,   6, clif->pGMFullStrip);
packet(CMSG_ADMIN_REQUEST_STATS,          0x0213,  26, clif->pCheck);
packet(CMSG_ADMIN_MONSTER_ITEM,           0x013f,  26, clif->pGM_Monster_Item);

packet(CMSG_GUILD_CHECK_MASTER,           0x014d,   2, clif->pGuildCheckMaster);
packet(CMSG_GUILD_REQUEST_INFO,           0x014f,   6, clif->pGuildRequestInfo);
packet(CMSG_GUILD_REQUEST_EMBLEM,         0x0151,   6, clif->pGuildRequestEmblem);
packet(CMSG_GUILD_CHANGE_EMBLEM,          0x0153,  -1, clif->pGuildChangeEmblem);
packet(CMSG_GUILD_CHANGE_MEMBER_POS,      0x0155,  -1, clif->pGuildChangeMemberPosition);
packet(CMSG_GUILD_LEAVE,                  0x0159,  54, clif->pGuildLeave);
packet(CMSG_GUILD_EXPULSION,              0x015b,  54, clif->pGuildExpulsion);
packet(CMSG_GUILD_BREAK,                  0x015d,  42, clif->pGuildBreak);
packet(CMSG_GUILD_CHANGE_POS_INFO,        0x0161,  -1, clif->pGuildChangePositionInfo);
packet(CMSG_GUILD_CREATE,                 0x0165,  30, clif->pCreateGuild);
packet(CMSG_GUILD_INVITE,                 0x0168,  14, clif->pGuildInvite);
packet(CMSG_GUILD_INVITE_REPLY,           0x016b,  10, clif->pGuildReplyInvite);
packet(CMSG_GUILD_CHANGE_NOTICE,          0x016e, 186, clif->pGuildChangeNotice);
packet(CMSG_GUILD_ALLIANCE_REQUEST,       0x0170,  14, clif->pGuildRequestAlliance);
packet(CMSG_GUILD_ALLIANCE_REPLY,         0x0172,  10, clif->pGuildReplyAlliance);
packet(CMSG_GUILD_MESSAGE,                0x017e,  -1, clif->pGuildMessage);
packet(CMSG_GUILD_OPPOSITION,             0x0180,   6, clif->pGuildOpposition);
packet(CMSG_GUILD_ALLIANCE_DELETE,        0x0183,  10, clif->pGuildDelAlliance);

packet(CMSG_SOLVE_CHAR_NAME,              0x0193,   6, clif->pSolveCharName);
packet(CMSG_IGNORE_ALL,                   0x00d0,   3, clif->pPMIgnoreAll);
packet(CMSG_IGNORE_NICK,                  0x00cf,  27, clif->pPMIgnore);
packet(CMSG_REQUEST_IGNORE_LIST,          0x00d3,   2, clif->pPMIgnoreList);
packet(CMSG_SET_SHORTCUTS,                0x02ba,  11, clif->pHotkey);
packet(CMSG_NPC_COMPLETE_PROGRESS_BAR,    0x02f1,   2, clif->pProgressbar);
packet(CMSG_NPC_PRODUCE_MIX,              0x018e,  10, clif->pProduceMix);
packet(CMSG_NPC_COOKING,                  0x025b,   6, clif->pCooking);
packet(CMSG_NPC_REPAIR,                   0x01fd,  15, clif->pRepairItem);
packet(CMSG_NPC_REFINE,                   0x0222,   6, clif->pWeaponRefine);
packet(CMSG_NPC_IDENTIFY,                 0x0178,   4, clif->pItemIdentify);
packet(CMSG_NPC_SELECT_ARROW,             0x01ae,   4, clif->pSelectArrow);
packet(CMSG_NPC_SELECT_AUTO_SPELL,        0x01ce,   6, clif->pAutoSpell);

packet(CMSG_PLAYER_MAPMOVE,               0x0140,  22, clif->pMapMove);
packet(CMSG_REMOVE_OPTION,                0x012a,   2, clif->pRemoveOption);
packet(CMSG_PLAYER_SET_MEMO,              0x011d,   2, clif->pRequestMemo);

packet(CMSG_PET_CATCH,                    0x019f,   6, clif->pCatchPet);
packet(CMSG_PET_SEND_MESSAGE,             0x01a9,   6, clif->pSendEmotion);
packet(CMSG_PET_SET_NAME,                 0x01a5,  26, clif->pChangePetName);
packet(CMSG_PET_SELECT_EGG,               0x01a7,   4, clif->pSelectEgg);
packet(CMSG_PET_MENU_ACTION,              0x01a1,   3, clif->pPetMenu);
packet2(CMSG_PET_TALK,                    0x0b0c,   0, nullptr);
packet2(CMSG_PET_EMOTE,                   0x0b0d,   0, nullptr);
packet2(CMSG_PET_MOVE_TO,                 0x0b11,   0, nullptr);
packet2(CMSG_PET_DIRECTION,               0x0b12,   0, nullptr);

packet(CMSG_MERCENARY_ACTION,             0x029f,   3, clif->pmercenary_action);
packet(CMSG_HOMUNCULUS_SET_NAME,          0x0231,  26, clif->pChangeHomunculusName);
packet(CMSG_HOMMERC_MOVE_TO_MASTER,       0x0234,   6, clif->pHomMoveToMaster);
packet(CMSG_HOMMERC_MOVE_TO,              0x0232,   9, clif->pHomMoveTo);
packet(CMSG_HOMMERC_ATTACK,               0x0233,  11, clif->pHomAttack);
packet2(CMSG_HOMMERC_TALK,                0x0b13,   0, nullptr);
packet2(CMSG_HOMMERC_EMOTE,               0x0b14,   0, nullptr);
packet2(CMSG_HOMMERC_DIRECTION,           0x0b15,   0, nullptr);

packet(CMSG_DORI_DORI,                    0x01e7,   2, clif->pNoviceDoriDori);
packet(CMSG_EXPLOSION_SPIRITS,            0x01ed,   2, clif->pNoviceExplosionSpirits);
packet(CMSG_PVP_INFO,                     0x020f,  10, clif->pPVPInfo);
packet(CMSG_PLAYER_AUTO_REVIVE,           0x0292,   2, clif->pAutoRevive);
packet(CMSG_QUEST_ACTIVATE,               0x02b6,   7, clif->pquestStateAck);

packet(CMSG_MAIL_REFRESH_INBOX,           0x023f,   2, clif->pMail_refreshinbox);
packet(CMSG_MAIL_READ_MESSAGE,            0x0241,   6, clif->pMail_read);
packet(CMSG_MAIL_GET_ATTACH,              0x0244,   6, clif->pMail_getattach);
packet(CMSG_MAIL_DELETE_MESSAGE,          0x0243,   6, clif->pMail_delete);
packet(CMSG_MAIL_RETURN_MESSAGE,          0x0273,  30, clif->pMail_return);
packet(CMSG_MAIL_SET_ATTACH,              0x0247,   8, clif->pMail_setattach);
packet(CMSG_MAIL_RESET_ATTACH,            0x0246,   4, clif->pMail_winopen);
packet(CMSG_MAIL_SEND,                    0x0248,  -1, clif->pMail_send);

packet(CMSG_FAMILY_ASK_FOR_CHILD,         0x01f9,   6, clif->pAdopt_request);
packet(CMSG_FAMILY_ASK_FOR_CHILD_REPLY,   0x01f7,  14, clif->pAdopt_reply);

packet(CMSG_FRIENDS_ADD_PLAYER,           0x0202,  26, clif->pFriendsListAdd);
packet(CMSG_FRIENDS_REQUEST_ACK,          0x0208,  14, clif->pFriendsListReply);
packet(CMSG_FRIENDS_DELETE_PLAYER,        0x0203,  10, clif->pFriendsListRemove);

packet(CMSG_AUCTION_CANCEL_REG,           0x024b,   4, clif->pAuction_cancelreg);
packet(CMSG_AUCTION_SET_ITEM,             0x024c,   8, clif->pAuction_setitem);
packet(CMSG_AUCTION_REGISTER,             0x024d,  12, clif->pAuction_register);
packet(CMSG_AUCTION_CANCEL,               0x024e,   6, clif->pAuction_cancel);
packet(CMSG_AUCTION_CLOSE,                0x025d,   6, clif->pAuction_close);
packet(CMSG_AUCTION_BID,                  0x024f,  10, clif->pAuction_bid);
packet(CMSG_AUCTION_SEARCH,               0x0251,  34, clif->pAuction_search);
packet(CMSG_AUCTION_BUY_SELL,             0x025c,   4, clif->pAuction_buysell);

packet(CMSG_VENDING_CLOSE,                0x012e,   2, clif->pCloseVending);
packet(CMSG_VENDING_LIST_REQ,             0x0130,   6, clif->pVendingListReq);
packet(CMSG_VENDING_BUY,                  0x0134,  -1, clif->pPurchaseReq);
packet(CMSG_VENDING_BUY2,                 0x0801,  -1, clif->pPurchaseReq2);
packet(CMSG_VENDING_CREATE_SHOP,          0x01b2,  -1, clif->pOpenVending);

packet(CMSG_MERGE_ITEM_ACK,               0x096e,  -1, clif->ackmergeitems);
packet(CMSG_MERGE_ITEM_CANCEL,            0x0974,   2, clif->cancelmergeitem);

packet(CMSG_REQ_REMAIN_TIME,              0x01c0,   2, clif->pReqRemainTime);

packet2(CMSG_SET_STATUS,                  0x0b0e,   0, nullptr);

packet2(CMSG_ONLINE_LIST,                 0x0b0f,   0, nullptr);

packet2(CMSG_PLAYER_INVENTORY_USE2,       0x0b26,   6, nullptr);

#ifndef PACKETS_UPDATE
// 0
packet(CMSG_ALCHEMIST_RANKS,              0x0000,   0, nullptr);
packet(CMSG_BLACKSMITH_RANKS,             0x0000,   0, nullptr);
packet(CMSG_PK_RANKS,                     0x0000,   0, nullptr);
packet(CMSG_TAEKWON_RANKS,                0x0000,   0, nullptr);
packet(CMSG_BUYINGSTORE_CLOSE,            0x0000,   0, nullptr);
packet(CMSG_BUYINGSTORE_CREATE,           0x0000,   0, nullptr);
packet(CMSG_BUYINGSTORE_SELL,             0x0000,   0, nullptr);
packet(CMSG_SEARCHSTORE_SEARCH,           0x0000,   0, nullptr);
packet(CMSG_SEARCHSTORE_CLOSE,            0x0000,   0, nullptr);
packet(CMSG_HOMUNCULUS_MENU,              0x0000,   0, nullptr);
packet(CMSG_SEARCHSTORE_CLICK,            0x0000,   0, nullptr);
packet(CMSG_BUYINGSTORE_OPEN,             0x0000,   0, nullptr);
packet(CMSG_SEARCHSTORE_NEXT_PAGE,        0x0000,   0, nullptr);
packet(CMSG_QUICK_IDENTIFY_ITEM,          0x0000,   0, nullptr);
packet(CMSG_PARTY_INVITE2,                0x0000,   0, nullptr);
packet(CMSG_SKILL_FEEL_SAVE_OK,           0x0000,   0, nullptr);
packet(CMSG_SKILL_SELECT_MENU,            0x0000,   0, nullptr);
packet(CMSG_PLAYER_LESS_EFFECTS,          0x0000,   0, nullptr);
packet(CMSG_SHORTCUTS_ROW_SHIFT,          0x0000,   0, nullptr);
packet(CMSG_NPC_SHOP_CLOSE,               0x0000,   0, nullptr);
packet(CMSG_NPC_MARKET_BUY,               0x0000,   0, nullptr);
packet(CMSG_NPC_MARKET_CLOSE,             0x0000,   0, nullptr);
packet(CMSG_BANK_DEPOSIT,                 0x0000,   0, nullptr);
packet(CMSG_BANK_WITHDRAW,                0x0000,   0, nullptr);
packet(CMSG_BANK_CHECK,                   0x0000,   0, nullptr);
packet(CMSG_BANK_OPEN,                    0x0000,   0, nullptr);
packet(CMSG_BANK_CLOSE,                   0x0000,   0, nullptr);
packet(CMSG_REQUEST_RANKS,                0x0000,   0, nullptr);
packet(CMSG_PLAYER_FAVORITE_ITEM,         0x0000,   0, nullptr);
packet(CMSG_BATTLE_REGISTER,              0x0000,   0, nullptr);
packet(CMSG_BATTLE_REVOKE,                0x0000,   0, nullptr);
packet(CMSG_BATTLE_BEGIN_ACK,             0x0000,   0, nullptr);
packet(CMSG_BATTLE_CHECK_STATE,           0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_BUY,            0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_BUY1,           0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_CLOSE,          0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_OPEN,           0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_REQUEST_TAB,    0x0000,   0, nullptr);
packet(CMSG_NPC_CASH_SHOP_SCHEDULE,       0x0000,   0, nullptr);
packet(CMSG_GUILD_INVITE2,                0x0000,   0, nullptr);
packet(CMSG_STORAGE_PASSWORD,             0x0000,   0, nullptr);
packet(CMSG_ITEM_LIST_WINDOW_SELECT,      0x0000,   0, nullptr);
packet(CMSG_SELECT_CART,                  0x0000,   0, nullptr);
packet(CMSG_ROULETTE_INFO,                0x0000,   0, nullptr);
packet(CMSG_ROULETTE_GENERATE,            0x0000,   0, nullptr);
packet(CMSG_ROULETTE_OPEN,                0x0000,   0, nullptr);
packet(CMSG_ROULETTE_CLOSE,               0x0000,   0, nullptr);
packet(CMSG_ROULETTE_RECV_ITEM,           0x0000,   0, nullptr);
packet(CMSG_ADMIN_RECALL2,                0x0000,   0, nullptr);
packet(CMSG_ADMIN_REMOVE2,                0x0000,   0, nullptr);
packet(CMSG_BOOKING_REGISTER_REQ,         0x0000,   0, nullptr);
packet(CMSG_BOOKING_SEARCH_REQ,           0x0000,   0, nullptr);
packet(CMSG_BOOKING_DELETE_REQ,           0x0000,   0, nullptr);
packet(CMSG_BOOKING_UPDATE_REQ,           0x0000,   0, nullptr);
packet(CMSG_MAIL2_CHECK_NAME,             0x0000,   0, nullptr);
packet(CMSG_MAIL2_OPEN_WRITE_MAIL,        0x0000,   0, nullptr);
packet(CMSG_MAIL2_ADD_ITEM_TO_MAIL,       0x0000,   0, nullptr);
packet(CMSG_MAIL2_REMOVE_ITEM_MAIL,       0x0000,   0, nullptr);
packet(CMSG_MAIL2_SEND_MAIL,              0x0000,   0, nullptr);
packet(CMSG_MAIL2_NEXT_PAGE,              0x0000,   0, nullptr);
packet(CMSG_MAIL2_READ_MAIL,              0x0000,   0, nullptr);
packet(CMSG_MAIL2_DELETE_MAIL,            0x0000,   0, nullptr);
packet(CMSG_MAIL2_REQUEST_MONEY,          0x0000,   0, nullptr);
packet(CMSG_MAIL2_REQUEST_ITEMS,          0x0000,   0, nullptr);
packet(CMSG_MAIL2_REFRESH_MAIL_LIST,      0x0000,   0, nullptr);
packet(CMSG_MAIL2_OPEN_MAILBOX,           0x0000,   0, nullptr);
packet(CMSG_MAIL2_CLOSE_MAILBOX,          0x0000,   0, nullptr);
packet(CMSG_MAIL2_CANCEL_WRITE_MAIL,      0x0000,   0, nullptr);
packet(CMSG_CLAN_MESSAGE,                 0x0000,   0, nullptr);
packet(CMSG_WINDOW_OPEN_REQUEST2,         0x0000,   0, nullptr);
packet(CMSG_ATTENDANCE_REWARD,            0x0000,   0, nullptr);
packet(CMSG_MAP_BLOCK_PLAY_CANCEL,        0x0000,   0, nullptr);
packet(CMSG_PRIVATE_AIRSHIP_REQUEST,      0x0000,   0, nullptr);
packet(CMSG_PLAYER_SELECT_STYLE,          0x0000,   0, nullptr);
packet(CMSG_PLAYER_SELECT_STYLE2,         0x0000,   0, nullptr);
packet(CMSG_PET_EVOLUTION,                0x0000,   0, nullptr);
packet(CMSG_PLAYER_SET_TITLE,             0x0000,   0, nullptr);
packet(CMSG_CHANGE_DRESS,                 0x0000,   0, nullptr);
packet(CMSG_INSTANCE_COMMAND,             0x0000,   0, nullptr);
packet(CMSG_ACHIEVEMENT_GET_REWARD,       0x0000,   0, nullptr);
packet(CMSG_LOGIN_MOBILE_OTP,             0x0000,   0, nullptr);
packet(CMSG_LOGIN_OTP_CODE,               0x0000,   0, nullptr);
packet(CMSG_INVENTORY_EXPAND,             0x0000,   0, nullptr);
packet(CMSG_INVENTORY_EXPAND_CONFIRM,     0x0000,   0, nullptr);
packet(CMSG_INVENTORY_EXPAND_REJECT,      0x0000,   0, nullptr);
#else
// 20040713
if (packetVersion >= 20040713)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x009b,  13, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x009f,  10, clif->pTakeItem);
}

// 20040726
if (packetVersion >= 20040726)
{
    packet(CMSG_CHAT_MESSAGE,             0x00f3,  -1, clif->pGlobalMessage);
    packet(CMSG_MAP_PING,                 0x00f7,  10, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x009f,  13, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0094,  10, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0072,  14, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0113,  16, clif->pMoveToKafra);
}

// 20040809
if (packetVersion >= 20040809)
{
    packet(CMSG_MAP_PING,                 0x00f7,  13, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x009f,  12, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0094,  13, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0072,  17, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0113,  23, clif->pMoveToKafra);
}

// 20040906
if (packetVersion >= 20040906)
{
    packet(CMSG_CHAT_MESSAGE,             0x009f,  -1, clif->pGlobalMessage);
    packet(CMSG_MAP_PING,                 0x0116,  11, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x00f3,  10, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0113,  11, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0094,  17, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x007e,  19, clif->pMoveToKafra);
}

// 20040920
if (packetVersion >= 20040920)
{
    packet(CMSG_MAP_PING,                 0x0116,  14, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x00f3,  18, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0113,  14, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0094,  19, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x007e,  25, clif->pMoveToKafra);
}

// 20041005
if (packetVersion >= 20041005)
{
    packet(CMSG_MAP_PING,                 0x0116,  10, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x00f3,  13, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0113,  10, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0094,  14, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x007e,  16, clif->pMoveToKafra);
}

// 20041025
if (packetVersion >= 20041025)
{
    packet(CMSG_MAP_PING,                 0x0116,   9, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x00f3,  15, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0113,   9, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0094,  12, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x007e,  13, clif->pMoveToKafra);
}

// 20041108
if (packetVersion >= 20041108)
{
    packet(CMSG_ALCHEMIST_RANKS,          0x0218,   2, clif->pAlchemist);
    packet(CMSG_BLACKSMITH_RANKS,         0x0217,   2, clif->pBlacksmith);
}

// 20041115
if (packetVersion >= 20041115)
{
    packet(CMSG_PLAYER_LESS_EFFECTS,      0x021d,   6, clif->pLessEffect);
}

// 20041129
if (packetVersion >= 20041129)
{
    packet(CMSG_CHAT_MESSAGE,             0x0085,  -1, clif->pGlobalMessage);
    packet(CMSG_MAP_PING,                 0x0089,   7, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x00f3,   8, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00a2,   7, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  12, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  14, clif->pMoveToKafra);
}

// 20050110
if (packetVersion >= 20050110)
{
    packet(CMSG_CHAT_MESSAGE,             0x00f3,  -1, clif->pGlobalMessage);
    packet(CMSG_CLOSE_STORAGE,            0x0193,   2, clif->pCloseKafra);
    packet(CMSG_MAP_PING,                 0x0089,   9, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  23, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,   9, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  20, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  20, clif->pMoveToKafra);
}

// 20050328
if (packetVersion >= 20050328)
{
    packet(CMSG_TAEKWON_RANKS,            0x0225,   2, clif->pTaekwon);
}

// 20050425
if (packetVersion >= 20050425)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
}

// 20050509
if (packetVersion >= 20050509)
{
    packet(CMSG_MAP_PING,                 0x0089,   8, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  11, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,   8, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  10, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  14, clif->pMoveToKafra);
}

// 20050530
if (packetVersion >= 20050530)
{
    packet(CMSG_PK_RANKS,                 0x0237,   2, clif->pRankingPk);
}

// 20050628
if (packetVersion >= 20050628)
{
    packet(CMSG_MAP_PING,                 0x0089,  13, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  17, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,  13, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  12, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  31, clif->pMoveToKafra);
}

// 20050718
if (packetVersion >= 20050718)
{
    packet(CMSG_MAP_PING,                 0x0089,   7, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  11, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,   7, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  21, clif->pMoveToKafra);
}

// 20050719
if (packetVersion >= 20050719)
{
    packet(CMSG_MAP_PING,                 0x0089,  13, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  17, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,  13, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  31, clif->pMoveToKafra);
}

// 20050817
if (packetVersion >= 20050817)
{
    packet(CMSG_SKILL_FEEL_SAVE_OK,       0x0254,   3, clif->pFeelSaveOk);
}

// 20060327
if (packetVersion >= 20060327)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  12, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  17, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  23, clif->pMoveToKafra);
}

// 20070108
if (packetVersion >= 20070108)
{
    packet(CMSG_MAP_PING,                 0x0089,  11, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  14, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,  11, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  19, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  17, clif->pMoveToKafra);
}

// 20070212
if (packetVersion >= 20070212)
{
    packet(CMSG_MAP_PING,                 0x0089,   8, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  11, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,   8, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  10, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  14, clif->pMoveToKafra);
}

// 20070227
if (packetVersion >= 20070227)
{
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_INSTANCE_COMMAND,         0x02cf,   6, clif->pMemorialDungeonCommand);
}

// 20080827
if (packetVersion >= 20080827)
{
    packet(CMSG_MAP_PING,                 0x0089,  11, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0085,  10, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x00f5,  11, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0116,  17, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0094,  19, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x00a7,   9, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x008c,  14, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0113,  25, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x00a2,  14, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x00f7,  17, clif->pMoveFromKafra);
}

// 20080910
if (packetVersion >= 20080910)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
}

// 20081210
if (packetVersion >= 20081210)
{
    packet(CMSG_SKILL_SELECT_MENU,        0x0443,   8, clif->pSkillSelectMenu);
}

// 20090520
if (packetVersion >= 20090520)
{
    packet(CMSG_MAP_BLOCK_PLAY_CANCEL,    0x0447,   2, clif->p_cz_blocking_play_cancel);
}

// 20090603
if (packetVersion >= 20090603)
{
    packet(CMSG_PARTY_SETTINGS,           0x07d7,   8, clif->pPartyChangeOption);
}

// 20091222
if (packetVersion >= 20091222)
{
    packet(CMSG_BOOKING_DELETE_REQ,       0x0806,   4, clif->pPartyBookingDeleteReq);
}

// 20091229
if (packetVersion >= 20091229)
{
    packet(CMSG_BOOKING_SEARCH_REQ,       0x0803,  14, clif->pPartyBookingSearchReq);
    packet(CMSG_BOOKING_DELETE_REQ,       0x0806,   2, clif->pPartyBookingDeleteReq);
    packet(CMSG_BOOKING_UPDATE_REQ,       0x0808,  14, clif->pPartyBookingUpdateReq);
}

// 20100303
if (packetVersion >= 20100303)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
}

// 20100420
if (packetVersion >= 20100420)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
}

// 20100601
if (packetVersion >= 20100601)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
}

// 20100608
if (packetVersion >= 20100608)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLOSE,        0x083b,   2, clif->pCloseSearchStoreInfo);
}

// 20100803
if (packetVersion >= 20100803)
{
    packet(CMSG_ADMIN_RECALL2,            0x0842,   6, clif->pGMRecall2);
    packet(CMSG_ADMIN_REMOVE2,            0x0843,   6, clif->pGMRemove2);
}

// 20101124
if (packetVersion >= 20101124)
{
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_NPC_CASH_SHOP_BUY,        0x0288,  -1, clif->pcashshop_buy);
}

// 20110614
if (packetVersion >= 20110614)
{
    packet(CMSG_NPC_CASH_SHOP_SCHEDULE,   0x08c9,   2, clif->pCashShopSchedule);
}

// 20110718
if (packetVersion >= 20110718)
{
    packet(CMSG_NPC_CASH_SHOP_CLOSE,      0x084a,   2, clif->pCashShopClose);
    packet(CMSG_NPC_CASH_SHOP_OPEN,       0x0844,   2, clif->pCashShopOpen);
    packet(CMSG_NPC_CASH_SHOP_REQUEST_TAB, 0x0846,  4, clif->pCashShopReqTab);
    packet(CMSG_NPC_CASH_SHOP_BUY1,       0x0848,  -1, clif->pCashShopBuy);
}

// 20111005
if (packetVersion >= 20111005)
{
    packet(CMSG_NAME_REQUEST,             0x088a,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x0817,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0366,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0815,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0885,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0893,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08ad,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0364,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0369,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0838,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0897,   8, clif->pMoveFromKafra);
    packet(CMSG_BATTLE_REGISTER,          0x08d7,  28, clif->pBGQueueRegister);
    packet(CMSG_BATTLE_REVOKE,            0x08da,  26, clif->pBGQueueRevokeReq);
    packet(CMSG_BATTLE_BEGIN_ACK,         0x08e0,  51, clif->pBGQueueBattleBeginAck);
    packet(CMSG_BATTLE_CHECK_STATE,       0x090a,  26, clif->pBGQueueCheckState);
}

// 20111102
if (packetVersion >= 20111102)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08ab,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0835,  19, clif->pWantToConnection);  // non PACKETVER_RE
    packet(CMSG_MAP_SERVER_CONNECT,       0x083c,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0899,   6, clif->pTickSend);          // non PACKETVER_RE
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x088b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0892,   5, clif->pWalkToXY);          // non PACKETVER_RE
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08aa,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x089e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0436,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a2,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a1,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0898,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0835,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x02c4,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0281,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0811,  -1, clif->pItemListWindowSelected);
}

// 20120307
if (packetVersion >= 20120307)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0884,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086A,  19, clif->pWantToConnection);
    packet(CMSG_CHAR_CREATE,              0x0970,  31, chr->parse_char_create_new_char);
    packet(CMSG_MAP_PING,                 0x0887,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0890,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0865,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x02c4,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x093b,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0885,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0369,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0963,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0863,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0889,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0929,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0861,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0870,  -1, clif->pItemListWindowSelected);
}

// 20120410
if (packetVersion >= 20120410)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094b,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0886,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0871,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0938,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0891,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x086c,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x0889,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0884,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089c,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a6,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0885,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_FAVORITE_ITEM,     0x0907,   5, clif->pMoveItem);
    packet(CMSG_PARTY_INVITE2,            0x091c,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0961,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0945,  -1, clif->pItemListWindowSelected);
}

// 20120418
if (packetVersion >= 20120418)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_GUILD_INVITE2,            0x0916,  26, clif->pGuildInvite2);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x08a8,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
}

// 20120502
if (packetVersion >= 20120502)
{
    packet(CMSG_SELECT_CART,              0x0980,   7, clif->pSelectCart);
}

// 20120702
if (packetVersion >= 20120702)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0363,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0364,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0960,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x089f,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089e,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a0,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0889,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0953,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x094a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085a,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0863,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0886,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0861,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0862,  10, clif->pUseSkillToId);
}

// 20120710
if (packetVersion >= 20120710)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0886,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
}

// 20120716
if (packetVersion >= 20120716)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0811,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0940,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0819,  36, clif->pStoragePassword);
    packet(CMSG_CLAN_MESSAGE,             0x098d,  -1, clif->pClanMessage);
}

// 20120925
if (packetVersion >= 20120925)
{
    packet(CMSG_PLAYER_EQUIP,             0x0998,   8, clif->pEquipItem);
}

// 20130320
if (packetVersion >= 20130320)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0363,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0897,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0933,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0438,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092e,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0881,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0886,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0898,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x088e,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0959,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0922,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094c,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086f,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0365,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0874,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x093f,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0938,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x089b,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x086d,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0947,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0868,  -1, clif->pItemListWindowSelected);
}

// 20130320
if (packetVersion >= 20130320)
{
    packet(CMSG_BANK_DEPOSIT,             0x09a7,  10, clif->pBankDeposit);
    packet(CMSG_BANK_WITHDRAW,            0x09a9,  10, clif->pBankWithdraw);
    packet(CMSG_BANK_CHECK,               0x09ab,   6, clif->pBankCheck);
}

// 20130417
if (packetVersion >= 20130417)
{
    packet(CMSG_BANK_OPEN,                0x09b6,   6, clif->pBankOpen);
    packet(CMSG_BANK_CLOSE,               0x09b8,   6, clif->pBankClose);
}

// 20130515
if (packetVersion >= 20130515)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0943,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0362,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x08a1,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0944,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0887,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0962,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ac,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0931,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0947,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x093e,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0963,  -1, clif->pItemListWindowSelected);
}

// 20130522
if (packetVersion >= 20130522)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a9,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x07ec,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0925,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x095e,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089c,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a3,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0964,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08aa,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0360,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x08a6,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a2,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0811,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0952,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0362,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087e,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0368,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0926,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0874,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x095c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0950,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x088e,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086a,  -1, clif->pItemListWindowSelected);
}

// 20130529
if (packetVersion >= 20130529)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0918,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0919,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0897,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0951,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0895,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0938,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0941,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0876,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0964,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0863,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0890,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0917,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0936,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0937,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0877,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0957,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0892,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x023b,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a7,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0869,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x08a8,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0956,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0958,  -1, clif->pItemListWindowSelected);
}

// 20130605
if (packetVersion >= 20130605)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_REQUEST_RANKS,            0x097c,   4, clif->pRanklist);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0883,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
}

// 20130612
if (packetVersion >= 20130612)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0919,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087e,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0940,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0964,  36, clif->pStoragePassword);
}

// 20130618
if (packetVersion >= 20130618)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0281,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095b,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0930,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a6,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0885,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x094f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0363,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x088e,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0944,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0889,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x096a,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0891,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0945,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0953,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0890,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0862,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x02c4,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0917,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0932,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0951,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0887,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0864,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0942,  -1, clif->pItemListWindowSelected);
}

// 20130626
if (packetVersion >= 20130626)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088c,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094d,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x088b,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0921,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0365,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08ab,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0817,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0960,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0952,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0895,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0930,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a5,  -1, clif->pItemListWindowSelected);
}

// 20130703
if (packetVersion >= 20130703)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0930,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0360,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0202,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x094a,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0873,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
}

// 20130807
if (packetVersion >= 20130807)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0887,  36, clif->pStoragePassword);
}

// 20130814
if (packetVersion >= 20130814)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0368,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x088a,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088c,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0926,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0202,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0962,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0941,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x093a,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0937,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0874,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0887,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0835,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0923,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0281,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0868,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0873,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0895,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0958,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095f,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0936,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0947,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0885,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a4,  -1, clif->pItemListWindowSelected);
}

// 20130814
if (packetVersion >= 20130814)
{
    packet(CMSG_ADMIN_MONSTER_ITEM,       0x09ce, 102, clif->pGM_Monster_Item);
}

// 20131211
if (packetVersion >= 20131211)
{
    packet(CMSG_MAIL2_CLOSE_MAILBOX,      0x09e9,   2, clif->pRodexCloseMailbox);
}

// 20131218
if (packetVersion >= 20131218)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092f,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0947,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x022d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08ab,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0811,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_NPC_SHOP_CLOSE,           0x09d4,   2, clif->pNPCShopClosed);
    packet(CMSG_NPC_MARKET_BUY,           0x09d6,  -1, clif->pNPCMarketPurchase);
    packet(CMSG_NPC_MARKET_CLOSE,         0x09d8,   2, clif->pNPCMarketClosed);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x085c,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
}
if (packetVersion >= 20131218)
{
    packet(CMSG_MAIL2_NEXT_PAGE,          0x09ee,  11, clif->pRodexNextMaillist);
    packet(CMSG_MAIL2_DELETE_MAIL,        0x09f5,  11, clif->pRodexDeleteMail);
    packet(CMSG_MAIL2_REFRESH_MAIL_LIST,  0x09ef,  11, clif->pRodexRefreshMaillist);
}

// 20131223
if (packetVersion >= 20131223)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
}

if (packetVersion >= 20131223)
{
    packet(CMSG_MAIL2_READ_MAIL,          0x09ea,  11, clif->pRodexReadMail);
}

// 20131230
if (packetVersion >= 20131230)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0365,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x087f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0949,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0969,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x02C4,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0926,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094A,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0860,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x091E,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x092A,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0369,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x0968,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0871,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089c,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x08a9,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0943,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x096a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0898,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x091d,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x022d,  -1, clif->pItemListWindowSelected);
}

/////////////////////////////////////////////////////
//                                                 //
// next is separated packets from shuffle packets. //
//                                                 //
/////////////////////////////////////////////////////


// 20131230
if (packetVersion >= 20131230)
{
    packet(CMSG_MAIL2_SEND_MAIL,          0x09ec,  -1, clif->pRodexSendMail);
}

// 20140122
if (packetVersion >= 20140122)
{
    packet(CMSG_PET_EVOLUTION,            0x09fb,  -1, clif->pPetEvolution);
}

// 20140129
if (packetVersion >= 20140129)
{
    packet(CMSG_SHORTCUTS_ROW_SHIFT,      0x0a01,   3, clif->pHotkeyRowShift);
}

// 20140212
if (packetVersion >= 20140212)
{
    packet(CMSG_MAIL2_OPEN_MAILBOX,       0x09e8,  11, clif->pRodexOpenMailbox);
}

// 20140326
if (packetVersion >= 20140326)
{
    packet(CMSG_MAIL2_REQUEST_MONEY,      0x09f1,  11, clif->pRodexRequestZeny);
    packet(CMSG_MAIL2_REQUEST_ITEMS,      0x09f3,  11, clif->pRodexRequestItems);
    packet(CMSG_MAIL2_CANCEL_WRITE_MAIL,  0x0a03,   2, clif->pRodexCancelWriteMail);
}

// 20140416
if (packetVersion >= 20140416)
{
    packet(CMSG_MAIL2_OPEN_WRITE_MAIL,    0x0a08,  26, clif->pRodexOpenWriteMail);
    packet(CMSG_MAIL2_ADD_ITEM_TO_MAIL,   0x0a04,   6, clif->pRodexAddItem);
    packet(CMSG_MAIL2_REMOVE_ITEM_MAIL,   0x0a06,   6, clif->pRodexRemoveItem);
}

// 20140423
if (packetVersion >= 20140423)
{
    packet(CMSG_MAIL2_CHECK_NAME,         0x0a13,  26, clif->pRodexCheckName);
}

// 20140605
if (packetVersion >= 20140605)
{
    packet(CMSG_ROULETTE_OPEN,            0x0a19,  -1, clif->pRouletteOpen);
    packet(CMSG_ROULETTE_INFO,            0x0a1b,   2, clif->pRouletteInfo);
    packet(CMSG_ROULETTE_CLOSE,           0x0a1d,  14, clif->pRouletteClose);
}

// 20140611
if (packetVersion >= 20140611)
{
// new packets
    packet(CMSG_ROULETTE_GENERATE,        0x0a1f,   2, clif->pRouletteGenerate);
    packet(CMSG_ROULETTE_RECV_ITEM,       0x0a21,   6, clif->pRouletteRecvItem);
    packet(CMSG_ACHIEVEMENT_GET_REWARD,   0x0a25,   6, clif->pAchievementGetReward);
// changed packets
    packet(CMSG_ROULETTE_OPEN,            0x0a19,   2, clif->pRouletteOpen);
    packet(CMSG_ROULETTE_CLOSE,           0x0a1d,   2, clif->pRouletteClose);
}

// 20140618
if (packetVersion >= 20140618)
{
    packet(CMSG_ROULETTE_RECV_ITEM,       0x0a21,   3, clif->pRouletteRecvItem);
}

// 20140903
if (packetVersion >= 20140903)
{
    packet(CMSG_PLAYER_SET_TITLE,         0x0a2e,   6, clif->pChangeTitle);
}

// 20141119
if (packetVersion >= 20141119)
{
    packet(CMSG_QUICK_IDENTIFY_ITEM,      0x0a35,   4, clif->pOneClick_ItemIdentify);
}

// 20151001
if (packetVersion >= 20151001)
{
    packet(CMSG_CHAR_CREATE,              0x0a39,  36, chr->parse_char_create_new_char);
}

// 20151104
if (packetVersion >= 20151104)
{
    packet(CMSG_PLAYER_SELECT_STYLE,      0x0a46,  14, clif->pReqStyleChange);
}

// 20160323
if (packetVersion >= 20160323)
{
    packet(CMSG_WINDOW_OPEN_REQUEST2,     0x0a68,   3, clif->pOpenUIRequest);
}

// 20160330
if (packetVersion >= 20160330)
{
    packet(CMSG_MAIL2_SEND_MAIL,          0x0a6e,  -1, clif->pRodexSendMail);
}

// 20170419
if (packetVersion >= 20170419)
{
    packet(CMSG_MAIL2_OPEN_MAILBOX,       0x0ac0,  26, clif->pRodexOpenMailbox);
    packet(CMSG_MAIL2_REFRESH_MAIL_LIST,  0x0ac1,  26, clif->pRodexRefreshMaillist);
}

// 20171214 zero
if (packetVersionZero >= 20171214)
{
    packet(CMSG_CHANGE_DRESS,             0x0ae8,   2, clif->pChangeDress);
}

// 20171220
if (packetVersion >= 20171220)
{
    packet(CMSG_CHANGE_DRESS,             0x0ae8,   2, clif->pChangeDress);
}

// zero 20180131
if (packetVersionZero >= 20180131)
{
    packet(CMSG_SKILL_USE_POSITION,       0x0af4,  11, clif->pUseSkillToPos);
}

// 20180207
if (packetVersion >= 20180207)
{
    packet(CMSG_SKILL_USE_POSITION,       0x0af4,  11, clif->pUseSkillToPos);
}

// re 20180307
if (packetVersionRe >= 20180307)
{
    packet(CMSG_ATTENDANCE_REWARD,        0x0aef,   2, clif->pAttendanceRewardRequest);
}

// re 20180321
if (packetVersionRe >= 20180321)
{
    packet(CMSG_PRIVATE_AIRSHIP_REQUEST,  0x0a49,  20, clif->pPrivateAirshipRequest);
}

// main 20180620
if (packetVersionMain >= 20180620)
{
    packet(CMSG_PRIVATE_AIRSHIP_REQUEST,  0x0a49,  20, clif->pPrivateAirshipRequest);
}

// main 20180404
if (packetVersionMain >= 20180404)
{
    packet(CMSG_ATTENDANCE_REWARD,        0x0aef,   2, clif->pAttendanceRewardRequest);
}

// zero 20180411
if (packetVersionZero >= 20180411)
{
    packet(CMSG_ATTENDANCE_REWARD,        0x0aef,   2, clif->pAttendanceRewardRequest);
}

// 20180516 main
if (packetVersionMain >= 20180516)
{
    packet(CMSG_PLAYER_SELECT_STYLE2,     0x0afc,  16, clif->pReqStyleChange2);
}

// 20180516 re
if (packetVersionRe >= 20180516)
{
    packet(CMSG_PLAYER_SELECT_STYLE2,     0x0afc,  16, clif->pReqStyleChange2);
}

// 20180523 zero
if (packetVersionZero >= 20180523)
{
    packet(CMSG_PLAYER_SELECT_STYLE2,     0x0afc,  16, clif->pReqStyleChange2);
}

// 20180704 re
if (packetVersionRe >= 20180704)
{
    packet(CMSG_NPC_PRODUCE_MIX,          0x018e,  18, clif->pProduceMix);
    packet(CMSG_NPC_SELECT_ARROW,         0x01ae,   6, clif->pSelectArrow);
    packet(CMSG_NPC_REPAIR,               0x01fd,  25, clif->pRepairItem);
    packet(CMSG_NPC_COOKING,              0x025b,   8, clif->pCooking);
    packet(CMSG_PRIVATE_AIRSHIP_REQUEST,  0x0a49,  22, clif->pPrivateAirshipRequest);
}

// 20180718 re
if (packetVersionRe >= 20180718)
{
    packet(CMSG_PLAYER_SELECT_STYLE,      0x0afc,  16, clif->pReqStyleChange);
}

// 20181031 main
if (packetVersionMain >= 20181031)
{
    packet(CMSG_INVENTORY_EXPAND,         0x0b14,   2, clif->pInventoryExpansion);
    packet(CMSG_INVENTORY_EXPAND_CONFIRM, 0x0b16,   2, clif->pInventoryExpansionConfirm);
    packet(CMSG_INVENTORY_EXPAND_REJECT,  0x0b19,   2, clif->pInventoryExpansionReject);
}

// 20181031 re
if (packetVersionRe >= 20181031)
{
    packet(CMSG_INVENTORY_EXPAND,         0x0b14,   2, clif->pInventoryExpansion);
    packet(CMSG_INVENTORY_EXPAND_CONFIRM, 0x0b16,   2, clif->pInventoryExpansionConfirm);
    packet(CMSG_INVENTORY_EXPAND_REJECT,  0x0b19,   2, clif->pInventoryExpansionReject);
}

// 20181114 main
if (packetVersionMain >= 20181114)
{
    packet(CMSG_LOGIN_MOBILE_OTP,         0x09a3,  -1, lclif->p->parse_CA_ACK_MOBILE_OTP);
    packet(CMSG_LOGIN_OTP_CODE,           0x0ad0,  11, lclif->p->parse_CA_OTP_CODE);
}

// 20181114 re
if (packetVersionRe >= 20181114)
{
    packet(CMSG_LOGIN_MOBILE_OTP,         0x09a3,  -1, lclif->p->parse_CA_ACK_MOBILE_OTP);
    packet(CMSG_LOGIN_OTP_CODE,           0x0ad0,  11, lclif->p->parse_CA_OTP_CODE);
}

// 20181114 zero
if (packetVersionZero >= 20181114)
{
    packet(CMSG_INVENTORY_EXPAND,         0x0b14,   2, clif->pInventoryExpansion);
    packet(CMSG_INVENTORY_EXPAND_CONFIRM, 0x0b16,   2, clif->pInventoryExpansionConfirm);
    packet(CMSG_INVENTORY_EXPAND_REJECT,  0x0b19,   2, clif->pInventoryExpansionReject);
}

#endif
