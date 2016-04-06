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
packet(CMSG_NAME_REQUEST,                 0x088a,   6, clif->pGetCharNameRequest);

packet(CMSG_CHAR_PASSWORD_CHANGE,         0x0061,   0, nullptr);
packet(CMSG_CHAR_SERVER_CONNECT,          0x0065,   0, nullptr);
packet(CMSG_CHAR_SELECT,                  0x0066,   0, nullptr);
packet(CMSG_CHAR_CREATE,                  0x0970,   0, nullptr);
packet(CMSG_CHAR_DELETE,                  0x0068,   0, nullptr);
packet(CMSG_CHAR_CREATE_PIN,              0x08ba,   0, nullptr);
packet(CMSG_CHAR_CHECK_RENAME,            0x08fc,   0, nullptr);
packet(CMSG_CHAR_RENAME,                  0x028f,   0, nullptr);
packet(CMSG_CHAR_CHANGE_SLOT,             0x08d4,   0, nullptr);

packet(CMSG_MAP_SERVER_CONNECT,           0x089c,  19, clif->pWantToConnection);
packet(CMSG_MAP_PING,                     0x035f,   6, clif->pTickSend);
packet(CMSG_LOGIN_PING,                   0x0200,   0, nullptr);
packet(CMSG_CHAR_PING,                    0x0187,   0, nullptr);
packet(CMSG_MAP_LOADED,                   0x007d,   2, clif->pLoadEndAck);
packet(CMSG_CLIENT_QUIT,                  0x018A,   4, clif->pQuitGame);

packet(CMSG_CHAT_MESSAGE,                 0x00f3,  -1, clif->pGlobalMessage);
packet(CMSG_CHAT_WHISPER,                 0x0096,  -1, clif->pWisMessage);
packet(CMSG_CHAT_ROOM_JOIN,               0x00d9,  14, clif->pChatAddMember);
packet(CMSG_CHAT_JOIN_CHANNEL,            0x0b07,   0, nullptr);
packet(CMSG_CHAT_PART_CHANNEL,            0x0b09,   0, nullptr);
packet(CMSG_BATTLE_CHAT_MESSAGE,          0x02db,  -1, clif->pBattleChat);

packet(CMSG_BATTLE_REGISTER,              0x08d7,  28, clif->pBGQueueRegister);
packet(CMSG_BATTLE_REVOKE,                0x08da,  26, clif->pBGQueueRevokeReq);
packet(CMSG_BATTLE_BEGIN_ACK,             0x08e0,  51, clif->pBGQueueBattleBeginAck);
packet(CMSG_BATTLE_CHECK_STATE,           0x090a,  26, clif->pBGQueueCheckState);

packet(CMSG_CREAYE_CHAT_ROOM,             0x00d5,  -1, clif->pCreateChatRoom);
packet(CMSG_LEAVE_CHAT_ROOM,              0x00e3,   2, clif->pChatLeave);
packet(CMSG_SET_CHAT_ROOM_OPTIONS,        0x00de,  -1, clif->pChatRoomStatusChange);
packet(CMSG_SET_CHAT_ROOM_OWNER,          0x00e0,  30, clif->pChangeChatOwner);
packet(CMSG_KICK_FROM_CHAT_ROOM,          0x00e2,  26, clif->pKickFromChat);

packet(CMSG_SKILL_LEVELUP_REQUEST,        0x0112,   4, clif->pSkillUp);
packet(CMSG_STAT_UPDATE_REQUEST,          0x00bb,   5, clif->pStatusUp);
packet(CMSG_SKILL_USE_BEING,              0x083c,  10, clif->pUseSkillToId);
packet(CMSG_SKILL_USE_POSITION,           0x0436,  10, clif->pUseSkillToPos);
packet(CMSG_SKILL_USE_POSITION_MORE,      0x0366,  90, clif->pUseSkillToPosMoreInfo);
packet(CMSG_SKILL_USE_MAP,                0x011b,  20, clif->pUseSkillMap);

packet(CMSG_PLAYER_INVENTORY_USE,         0x0439,   8, clif->pUseItem);
packet(CMSG_PLAYER_INVENTORY_DROP,        0x0362,   6, clif->pDropItem);
packet(CMSG_PLAYER_EQUIP,                 0x0998,   8, clif->pEquipItem);
packet(CMSG_PLAYER_UNEQUIP,               0x00ab,   4, clif->pUnequipItem);
packet(CMSG_PLAYER_USE_CARD,              0x017a,   4, clif->pUseCard);
packet(CMSG_PLAYER_INSERT_CARD,           0x017c,   6, clif->pInsertCard);
packet(CMSG_PLAYER_VIEW_EQUIPMENT,        0x02d6,   6, clif->pViewPlayerEquip);
packet(CMSG_PLAYER_SET_EQUIPMENT_VISIBLE, 0x02d8,  10, clif->pEquipTick);
packet(CMSG_PLAYER_FAVORITE_ITEM,         0x0907,   5, clif->pMoveItem);

packet(CMSG_ITEM_PICKUP,                  0x07e4,   6, clif->pTakeItem);
packet(CMSG_PLAYER_CHANGE_DIR,            0x0202,   5, clif->pChangeDir);
packet(CMSG_PLAYER_CHANGE_DEST,           0x0437,   5, clif->pWalkToXY);
packet(CMSG_PLAYER_CHANGE_ACT,            0x0871,   7, clif->pActionRequest);
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
packet(CMSG_NPC_MARKET_CLOSE,             0x09d8,   2, clif->pNPCMarketClosed);
packet(CMSG_NPC_MARKET_BUY,               0x09d6,  -1, clif->pNPCMarketPurchase);
packet(CMSG_NPC_CASH_SHOP_BUY,            0x0288,  -1, clif->pcashshop_buy);
packet(CMSG_NPC_CASH_SHOP_CLOSE,          0x084a,   2, clif->pCashShopClose);
packet(CMSG_NPC_CASH_SHOP_OPEN,           0x0844,   2, clif->pCashShopOpen);
packet(CMSG_NPC_CASH_SHOP_REQUEST_TAB,    0x0846,   4, clif->pCashShopReqTab);
packet(CMSG_NPC_CASH_SHOP_SCHEDULE,       0x08c9,   2, clif->pCashShopSchedule);

packet(CMSG_TRADE_REQUEST,                0x00e4,   6, clif->pTradeRequest);
packet(CMSG_TRADE_RESPONSE,               0x00e6,   3, clif->pTradeAck);
packet(CMSG_TRADE_ITEM_ADD_REQUEST,       0x00e8,   8, clif->pTradeAddItem);
packet(CMSG_TRADE_CANCEL_REQUEST,         0x00ed,   2, clif->pTradeCancel);
packet(CMSG_TRADE_ADD_COMPLETE,           0x00eb,   2, clif->pTradeOk);
packet(CMSG_TRADE_OK,                     0x00ef,   2, clif->pTradeCommit);

packet(CMSG_PARTY_CREATE,                 0x00f9,  26, clif->pCreateParty);
packet(CMSG_PARTY_CREATE2,                0x01e8,  28, clif->pCreateParty2);
packet(CMSG_PARTY_INVITE,                 0x00fc,   6, clif->pPartyInvite);
packet(CMSG_PARTY_INVITE2,                0x095d,  26, clif->pPartyInvite2);
packet(CMSG_PARTY_INVITED,                0x00ff,  10, clif->pReplyPartyInvite);
packet(CMSG_PARTY_INVITED2,               0x02c7,   7, clif->pReplyPartyInvite2);
packet(CMSG_PARTY_LEAVE,                  0x0100,   2, clif->pLeaveParty);
packet(CMSG_PARTY_SETTINGS,               0x0102,   6, clif->pPartyChangeOption);
packet(CMSG_PARTY_KICK,                   0x0103,  30, clif->pRemovePartyMember);
packet(CMSG_PARTY_MESSAGE,                0x0108,  -1, clif->pPartyMessage);
packet(CMSG_PARTY_CHANGE_LEADER,          0x07da,   6, clif->pPartyChangeLeader);
packet(CMSG_PARTY_ALLOW_INVITES,          0x02c8,   3, clif->pPartyTick);

packet(CMSG_MOVE_TO_STORAGE,              0x07ec,   8, clif->pMoveToKafra);
packet(CMSG_MOVE_FROM_STORAGE,            0x085b,   8, clif->pMoveFromKafra);
packet(CMSG_CLOSE_STORAGE,                0x0193,   2, clif->pCloseKafra);

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

packet(CMSG_SOLVE_CHAR_NAME,              0x0368,   6, clif->pSolveCharName);
packet(CMSG_IGNORE_ALL,                   0x00d0,   3, clif->pPMIgnoreAll);
packet(CMSG_IGNORE_NICK,                  0x00cf,  27, clif->pPMIgnore);
packet(CMSG_REQUEST_IGNORE_LIST,          0x00d3,   2, clif->pPMIgnoreList);
packet(CMSG_REQUEST_RANKS,                0x097c,   4, clif->pRanklist);
packet(CMSG_SET_SHORTCUTS,                0x02ba,  11, clif->pHotkey);
packet(CMSG_SHORTCUTS_ROW_SHIFT,          0x0a01,   3, clif->pHotkeyRowShift);
packet(CMSG_NPC_COMPLETE_PROGRESS_BAR,    0x02f1,   2, clif->pProgressbar);
packet(CMSG_NPC_PRODUCE_MIX,              0x018e,  10, clif->pProduceMix);
packet(CMSG_NPC_COOKING,                  0x025b,   6, clif->pCooking);
packet(CMSG_NPC_REPAIR,                   0x01fd,  15, clif->pRepairItem);
packet(CMSG_NPC_REFINE,                   0x0222,   6, clif->pWeaponRefine);
packet(CMSG_NPC_IDENTIFY,                 0x0178,   4, clif->pItemIdentify);
packet(CMSG_NPC_SELECT_ARROW,             0x01ae,   4, clif->pSelectArrow);
packet(CMSG_NPC_SELECT_AUTO_SPELL,        0x01ce,   6, clif->pAutoSpell);
packet(CMSG_NPC_SHOP_CLOSE,               0x09d4,   2, clif->pNPCShopClosed);

packet(CMSG_PLAYER_MAPMOVE,               0x0140,  22, clif->pMapMove);
packet(CMSG_REMOVE_OPTION,                0x012a,   2, clif->pRemoveOption);
packet(CMSG_PLAYER_SET_MEMO,              0x011d,   2, clif->pRequestMemo);

packet(CMSG_PET_CATCH,                    0x019f,   6, clif->pCatchPet);
packet(CMSG_PET_SEND_MESSAGE,             0x01a9,   6, clif->pSendEmotion);
packet(CMSG_PET_SET_NAME,                 0x01a5,  26, clif->pChangePetName);
packet(CMSG_PET_SELECT_EGG,               0x01a7,   4, clif->pSelectEgg);
packet(CMSG_PET_MENU_ACTION,              0x01a1,   3, clif->pPetMenu);
packet(CMSG_PET_TALK,                     0x0b0c,   0, nullptr);
packet(CMSG_PET_EMOTE,                    0x0b0d,   0, nullptr);
packet(CMSG_PET_MOVE_TO,                  0x0b11,   0, nullptr);
packet(CMSG_PET_DIRECTION,                0x0b12,   0, nullptr);

packet(CMSG_MERCENARY_ACTION,             0x029f,   3, clif->pmercenary_action);
packet(CMSG_HOMUNCULUS_SET_NAME,          0x0231,  26, clif->pChangeHomunculusName);
packet(CMSG_HOMUNCULUS_MENU,              0x0361,   5, clif->pHomMenu);
packet(CMSG_HOMMERC_MOVE_TO_MASTER,       0x0234,   6, clif->pHomMoveToMaster);
packet(CMSG_HOMMERC_MOVE_TO,              0x0232,   9, clif->pHomMoveTo);
packet(CMSG_HOMMERC_ATTACK,               0x0233,  11, clif->pHomAttack);
packet(CMSG_HOMMERC_TALK,                 0x0b13,   0, nullptr);
packet(CMSG_HOMMERC_EMOTE,                0x0b14,   0, nullptr);
packet(CMSG_HOMMERC_DIRECTION,            0x0b15,   0, nullptr);

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

packet(CMSG_BANK_DEPOSIT,                 0x09a7,  10, clif->pBankDeposit);
packet(CMSG_BANK_WITHDRAW,                0x09a9,  10, clif->pBankWithdraw);
packet(CMSG_BANK_CHECK,                   0x09ab,   6, clif->pBankCheck);
packet(CMSG_BANK_OPEN,                    0x09b6,   6, clif->pBankOpen);
packet(CMSG_BANK_CLOSE,                   0x09b8,   6, clif->pBankClose);

packet(CMSG_FRIENDS_ADD_PLAYER,           0x091a,  26, clif->pFriendsListAdd);
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

packet(CMSG_BUYINGSTORE_CREATE,           0x0815,  -1, clif->pReqOpenBuyingStore);
packet(CMSG_BUYINGSTORE_CLOSE,            0x022d,   2, clif->pReqCloseBuyingStore);
packet(CMSG_BUYINGSTORE_OPEN,             0x0360,   6, clif->pReqClickBuyingStore);
packet(CMSG_BUYINGSTORE_SELL,             0x0811,  -1, clif->pReqTradeBuyingStore);

packet(CMSG_SEARCHSTORE_SEARCH,           0x0819,  -1, clif->pSearchStoreInfo);
packet(CMSG_SEARCHSTORE_NEXT_PAGE,        0x0940,   2, clif->pSearchStoreInfoNextPage);
packet(CMSG_SEARCHSTORE_CLOSE,            0x083b,   2, clif->pCloseSearchStoreInfo);
packet(CMSG_SEARCHSTORE_CLICK,            0x0835,   2, clif->pSearchStoreInfoNextPage);

packet(CMSG_MERGE_ITEM_ACK,               0x096e,  -1, clif->ackmergeitems);
packet(CMSG_MERGE_ITEM_CANCEL,            0x0974,   2, clif->cancelmergeitem);

packet(CMSG_SET_STATUS,                   0x0b0e,   0, nullptr);

packet(CMSG_ONLINE_LIST,                  0x0b0f,   0, nullptr);

// 20150805
packet(CMSG_SELECT_CART,                  0x0980,   7, clif->pSelectCart);

#ifndef PACKETS_UPDATE
// 0
packet(CMSG_ALCHEMIST_RANKS,              0x0000,   0, nullptr);
packet(CMSG_BLACKSMITH_RANKS,             0x0000,   0, nullptr);
packet(CMSG_PK_RANKS,                     0x0000,   0, nullptr);
#else
// 20041108
if (packetVersion >= 20041108)
{
    packet(CMSG_ALCHEMIST_RANKS,          0x0218,   2, clif->pAlchemist);
    packet(CMSG_BLACKSMITH_RANKS,         0x0217,   2, clif->pBlacksmith);
}

// 20050530
if (packetVersion >= 20050530)
{
    packet(CMSG_PK_RANKS,                 0x0237,   2, clif->pRankingPk);
}

// 20090603
if (packetVersion >= 20090603)
{
    packet(CMSG_PARTY_SETTINGS,           0x07d7,   8, clif->pPartyChangeOption);
}

// 20150513
if (packetVersion >= 20150513)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x022d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0817,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0924,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0885,   6, clif->pDropItem);
}
#endif

// 20150513
packet(CMSG_QUICK_IDENTIFY_ITEM,          0x0a35,   4, clif->pOneClick_ItemIdentify);
