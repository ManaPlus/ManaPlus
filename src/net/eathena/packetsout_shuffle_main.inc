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

#define PACKETSOUT_SHUFFLE_MAIN_VOID

if (packets_main)
{

// 20120710
if (packetVersion == 20120710)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0202,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x02c4,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0362,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0365,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0367,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0436,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x07e4,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0811,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x085b,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0877,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0878,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087f,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088d,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a3,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a6,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a8,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08aa,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0936,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0948,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x094b,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x094e,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0953,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0956,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0958,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0960,  90, clif->pUseSkillToPosMoreInfo);
}

// 20120712
if (packetVersion == 20120712)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0202,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x02c4,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0362,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0365,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0367,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0436,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x07e4,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0811,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x085b,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0877,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0878,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087f,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088d,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a3,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a6,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a8,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08aa,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0936,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0948,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x094b,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x094e,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0953,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0956,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0958,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0960,  90, clif->pUseSkillToPosMoreInfo);
}

// 20120716
if (packetVersion == 20120716)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x089f,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20120724
if (packetVersion == 20120724)
{
//  packet(UNKNOWN,                       0x035f,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0364,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0815,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0835,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0865,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0866,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0872,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0877,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x087c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0887,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0888,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x088e,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x08a1,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a9,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x08ab,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x091c,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x091f,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0928,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0929,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0935,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x0938,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x093c,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0945,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0957,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0959,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0963,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0965,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0966,   8, clif->pMoveFromKafra);
}

// 20120801
if (packetVersion == 20120801)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0360,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0438,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085d,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x086b,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0873,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0875,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_HOMUNCULUS_MENU,          0x088b,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0890,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0895,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x089c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089d,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089f,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a2,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08aa,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ab,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0917,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x091d,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x092a,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x092e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0930,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x0934,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x093c,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0941,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0946,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0964,  12, clif->pSearchStoreInfoListItemClick);
}

// 20120808
if (packetVersion == 20120808)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0366,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0438,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0838,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085f,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088f,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x093d,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION,       0x0940,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20120814
if (packetVersion == 20120814)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x0281,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0361,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0365,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0366,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x07ec,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0815,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0838,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x085e,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0861,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0862,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0865,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0868,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x086f,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0875,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x087f,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0882,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088b,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0895,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x08a8,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091f,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x0920,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0929,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0932,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x0934,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0941,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0967,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0969,   5, clif->pChangeDir);
}

// 20120822
if (packetVersion == 20120822)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0365,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x0437,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085a,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_SELL,         0x085c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x085d,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086b,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086e,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0872,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x087c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x087d,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x088a,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0899,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089f,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x08a5,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ab,   7, clif->pActionRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0917,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0924,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_NAME_REQUEST,             0x0927,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0935,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION,       0x0937,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0940,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0943,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x094d,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094e,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0953,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0955,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0958,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095b,   6, clif->pReqClickBuyingStore);
}

// 20120830
if (packetVersion == 20120830)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x022d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0364,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PARTY_INVITE2,            0x0436,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x0802,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0835,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0838,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0861,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0872,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION,       0x0887,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0888,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0890,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0926,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0934,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0945,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x094b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x0964,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20120905
if (packetVersion == 20120905)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x022d,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0365,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0817,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x085d,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086f,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0877,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0878,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_BEING,          0x087a,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x087b,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087e,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0887,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0895,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0897,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08a0,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a6,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0917,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0918,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x091a,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0927,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093c,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x0944,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0945,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0950,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0954,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0959,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0960,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0967,   7, clif->pActionRequest);
}

// 20120911
if (packetVersion == 20120911)
{
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0819,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x0868,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x086f,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0877,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0879,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x088a,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088f,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a5,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a6,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a7,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ad,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0930,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0935,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x0948,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0949,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x094b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0958,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0959,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0962,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0963,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0966,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0968,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x096a,  36, clif->pStoragePassword);
}

// 20120919
if (packetVersion == 20120919)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0956,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20120925
if (packetVersion == 20120925)
{
//  packet(UNKNOWN,                       0x07e4,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0815,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0819,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0835,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0838,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x085d,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0873,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087a,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087e,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0887,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x088a,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x088d,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a4,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x091c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0923,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0927,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0932,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0942,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0948,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0950,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0956,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0957,   5, clif->pChangeDir);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0958,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x095e,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0963,   8, clif->pMoveFromKafra);
}

// 20120926
if (packetVersion == 20120926)
{
//  packet(UNKNOWN,                       0x07e4,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0815,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0819,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0835,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0838,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x085d,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0873,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087a,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087e,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0887,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x088a,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x088d,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a4,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x091c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0923,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0927,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0932,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0942,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0948,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0950,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0956,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0957,   5, clif->pChangeDir);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0958,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x095e,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0963,   8, clif->pMoveFromKafra);
}

// 20121010
if (packetVersion == 20121010)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0202,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0817,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0869,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x086c,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0874,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x087d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0883,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0888,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x088f,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x0891,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0896,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0898,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a1,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a6,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08a9,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0925,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x0934,   6, clif->pTickSend);
    packet(CMSG_MOVE_TO_STORAGE,          0x093c,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0942,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0946,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0947,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094c,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0957,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0958,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x0959,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x095c,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0964,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0969,  18, clif->pPartyBookingRegisterReq);
}

// 20121017
if (packetVersion == 20121017)
{
    packet(CMSG_PARTY_INVITE2,            0x023b,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0365,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0815,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0868,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086b,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0887,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0888,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0897,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089a,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a6,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0918,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0920,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0960,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0965,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20121024
if (packetVersion == 20121024)
{
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0436,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_BEING,          0x0437,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0438,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0817,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0838,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085a,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0868,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0872,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0875,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x087a,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x087e,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x0882,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0888,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0889,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x088a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x089c,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a9,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08aa,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0931,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0938,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x093a,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0942,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0947,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0954,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0955,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x095d,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095e,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0965,  19, clif->pWantToConnection);
}

// 20121031
if (packetVersion == 20121031)
{
    packet(CMSG_MAP_PING,                 0x022d,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x035f,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x085a,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0871,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0877,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0898,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x092b,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20121107
if (packetVersion == 20121107)
{
    packet(CMSG_ITEM_PICKUP,              0x0360,   6, clif->pTakeItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0361,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0365,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x085c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x0861,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0865,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x086c,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086d,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0871,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x0873,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0878,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087f,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0890,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0891,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0896,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089a,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x089b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a2,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x08ad,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0922,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0934,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x093b,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0945,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x094d,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0960,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0963,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0966,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x096a,  26, clif->pFriendsListAdd);
}

// 20121114
if (packetVersion == 20121114)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0365,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0437,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0815,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0838,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0862,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0876,   6, clif->pSolveCharName);
    packet(CMSG_NAME_REQUEST,             0x0879,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0881,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0898,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0899,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x08a9,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08aa,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ab,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091a,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0930,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x0935,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0938,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x093c,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0944,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0949,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION,       0x094a,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094b,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x095c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0965,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x096a,   8, clif->pDull);
}

// 20121121
if (packetVersion == 20121121)
{
    packet(CMSG_SKILL_USE_BEING,          0x0202,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0362,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0437,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0863,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x086a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086d,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086f,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0872,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x0875,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087e,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0880,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0892,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x089a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a7,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08aa,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x091a,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x0921,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0927,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0929,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0938,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0946,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x094d,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0950,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0952,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0954,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x0957,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0958,   6, clif->pReqClickBuyingStore);
}

// 20121128
if (packetVersion == 20121128)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0362,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0363,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x07ec,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0802,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0861,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x0867,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x086c,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x086e,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x0872,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0888,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088b,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0894,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0895,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a8,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08ad,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091b,   6, clif->pSolveCharName);
    packet(CMSG_NAME_REQUEST,             0x091c,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x091e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0921,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0922,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x092f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0931,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0945,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0947,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0950,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0953,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0954,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0960,  -1, clif->pReqTradeBuyingStore);
}

// 20121205
if (packetVersion == 20121205)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x035f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0360,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0815,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x0863,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0864,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0870,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0873,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x0875,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0878,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x0888,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0889,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x088b,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089a,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08ad,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091c,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0921,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x093a,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x093e,   8, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x093f,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x0946,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x094b,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0954,   7, clif->pActionRequest);
    packet(CMSG_PARTY_INVITE2,            0x0959,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0966,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0968,   5, clif->pChangeDir);
}

// 20121212
if (packetVersion == 20121212)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0362,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0368,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0369,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0436,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0438,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0817,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085c,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x0877,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_BEING,          0x087d,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0893,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0895,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a3,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a7,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x08a9,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x08aa,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ac,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0917,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x091a,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0920,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0932,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0947,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0951,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0953,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0957,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x095b,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0965,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x0966,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20121218
if (packetVersion == 20121218)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0362,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0363,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x083c,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0872,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087a,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x087b,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0886,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0889,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0890,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091a,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091c,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0935,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0946,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0957,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20121227
if (packetVersion == 20121227)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x0202,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x02c4,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0361,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0368,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0437,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0860,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0870,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0872,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x087a,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0886,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0888,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088e,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0890,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0894,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0896,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x08a6,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08aa,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0917,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x091f,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0920,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0932,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0933,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0940,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0943,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_NAME_REQUEST,             0x0947,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0953,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0958,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x095d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0962,  26, clif->pPartyInvite2);
}

// 20130103
if (packetVersion == 20130103)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x022d,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0436,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0940,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130107
if (packetVersion == 20130107)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x022d,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0436,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0940,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130109
if (packetVersion == 20130109)
{
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0365,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0368,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0838,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0866,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0886,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0888,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088a,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x088e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x0891,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0892,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a2,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x08aa,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08ac,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0918,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091b,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0922,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0927,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0928,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0930,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0931,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0938,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x093e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x0950,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0951,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0955,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x095e,   6, clif->pTakeItem);
}

// 20130115
if (packetVersion == 20130115)
{
//  packet(UNKNOWN,                       0x0361,   8, clif->pDull);
//  packet(UNKNOWN,                       0x07ec,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0817,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0861,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0863,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x0870,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0871,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0877,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0879,   5, clif->pChangeDir);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087c,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0888,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0894,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x0895,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x08a0,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x08a4,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0918,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0928,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092d,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x092e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0939,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x094b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x094c,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094e,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x095a,  36, clif->pStoragePassword);
}

// 20130116
if (packetVersion == 20130116)
{
//  packet(UNKNOWN,                       0x0361,   8, clif->pDull);
//  packet(UNKNOWN,                       0x07ec,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0817,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0861,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0863,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x0870,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0871,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0877,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0879,   5, clif->pChangeDir);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087c,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0888,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0894,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x0895,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x08a0,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x08a4,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0918,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0928,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092d,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x092e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0939,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x094b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x094c,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094e,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x095a,  36, clif->pStoragePassword);
}

// 20130121
if (packetVersion == 20130121)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x023b,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x035f,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0369,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x07ec,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0802,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0815,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0817,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085a,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x085d,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086c,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x0874,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0884,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x091f,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0940,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094f,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0956,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x095b,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0963,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0969,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130130
if (packetVersion == 20130130)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x035f,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0367,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0368,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0437,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x085f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0862,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x086b,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087d,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0885,   7, clif->pActionRequest);
    packet(CMSG_ITEM_PICKUP,              0x0886,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0887,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x0889,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x089a,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x08a0,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x08a4,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08a9,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091a,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0920,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0924,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x093a,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0956,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0957,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x095b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x095c,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095d,   2, clif->pSearchStoreInfoNextPage);
}

// 20130206
if (packetVersion == 20130206)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0362,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x0811,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085d,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0876,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0883,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0885,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0899,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a0,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ac,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0920,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0925,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093c,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0950,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130215
if (packetVersion == 20130215)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x023b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0360,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0366,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0802,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0886,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089e,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089f,   4, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x08ad,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0923,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0956,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x095d,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0962,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130220
if (packetVersion == 20130220)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x094b,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130227
if (packetVersion == 20130227)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x035f,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0360,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0811,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0815,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x085a,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0863,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0867,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0869,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x086e,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x087b,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0893,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0898,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a0,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x08a1,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x091b,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0924,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x092b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x092f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0936,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x0946,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0951,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095b,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x095e,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0967,   7, clif->pActionRequest);
}

// 20130306
if (packetVersion == 20130306)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x022d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0367,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0368,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0802,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x0817,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085b,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0860,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086d,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0881,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0890,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0898,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x089f,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x08a4,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x08ac,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0917,   8, clif->pMoveFromKafra);
    packet(CMSG_MOVE_TO_STORAGE,          0x091e,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0926,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0934,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0936,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x0943,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0944,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0945,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0946,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x094a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x094e,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0951,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0960,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
}

// 20130313
if (packetVersion == 20130313)
{
    packet(CMSG_PARTY_INVITE2,            0x0202,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0802,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0887,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0920,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x092d,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0947,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130320
if (packetVersion == 20130320)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0363,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0365,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0438,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085a,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x085d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0868,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x086d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086f,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0874,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0881,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0886,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x088e,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0890,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0897,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x0898,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x089b,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0922,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092e,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x0933,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0938,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x093f,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0947,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094c,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION,       0x0959,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x095a,   8, clif->pDull);
}

// 20130327
if (packetVersion == 20130327)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x02c4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0835,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x086d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0873,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0878,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0885,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_BEING,          0x088f,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0891,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0893,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x08a1,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a2,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x091f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092b,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x092d,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x092e,   4, clif->pDull);
//  packet(UNKNOWN,                       0x092f,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0933,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0938,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0939,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094a,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x094b,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094f,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0955,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0960,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0961,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0967,  -1, clif->pItemListWindowSelected);
}

// 20130403
if (packetVersion == 20130403)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x023b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0360,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0884,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x08a5,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0926,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0942,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0950,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130410
if (packetVersion == 20130410)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x0367,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0437,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0860,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0865,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x086c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0879,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087c,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x087d,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0881,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x088c,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x088d,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0890,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0891,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089c,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x089e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x08a0,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08a4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a7,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x0917,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x091d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0933,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0939,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x093d,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0944,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0945,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0955,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0960,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0967,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x096a,   6, clif->pDropItem);
}

// 20130417
if (packetVersion == 20130417)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x0202,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0811,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086b,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0875,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0929,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0962,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0969,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x096a,   4, clif->pDull);
}

// 20130424
if (packetVersion == 20130424)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0969,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130502
if (packetVersion == 20130502)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085a,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0870,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0874,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0875,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0877,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087c,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x0888,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0889,   8, clif->pDull);
//  packet(UNKNOWN,                       0x08a1,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a3,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0961,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x0967,   6, clif->pTakeItem);
}

// 20130508
if (packetVersion == 20130508)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x022d,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0436,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0868,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0878,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x088b,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0893,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0895,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0897,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x08ab,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x092d,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0944,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x094d,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0957,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x095c,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130515
if (packetVersion == 20130515)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0362,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0862,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0887,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x08a1,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08aa,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ac,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x092d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0931,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x093e,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0943,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0944,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0947,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0962,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0963,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130522
if (packetVersion == 20130522)
{
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0360,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0362,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0368,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_PING,                 0x07ec,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x0811,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0874,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087e,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x088e,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x089b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089c,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a2,   7, clif->pActionRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a3,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x08a6,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a9,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08aa,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x08ac,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0925,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0926,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x0950,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0952,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x095c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x095e,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0964,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0965,   8, clif->pDull);
}

// 20130529
if (packetVersion == 20130529)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x023b,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0863,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0869,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0874,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0876,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0877,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0888,   4, clif->pDull);
//  packet(UNKNOWN,                       0x088e,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0890,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0892,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0895,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x0897,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a7,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x08a8,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x0917,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0918,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0919,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0936,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0937,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0938,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0941,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0951,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0956,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0957,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0958,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0964,   2, clif->pReqCloseBuyingStore);
}

// 20130605
if (packetVersion == 20130605)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0883,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130612
if (packetVersion == 20130612)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087e,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0919,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0940,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0964,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130618
if (packetVersion == 20130618)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0281,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x02c4,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0363,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0862,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0864,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0878,   4, clif->pDull);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0885,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0887,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0889,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x088e,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0890,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x0891,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a6,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x08a7,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0917,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0930,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0932,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0942,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0944,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0945,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x094f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0951,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0953,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095b,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x096a,  10, clif->pUseSkillToPos);
}

// 20130626
if (packetVersion == 20130626)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0365,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0817,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0860,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x088b,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088c,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x088f,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0894,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x0895,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a5,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08ab,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0921,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0930,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094d,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0952,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0960,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130703
if (packetVersion == 20130703)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x0202,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0360,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0873,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0930,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x094a,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130710
if (packetVersion == 20130710)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0880,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130717
if (packetVersion == 20130717)
{
    packet(CMSG_MAP_PING,                 0x02c4,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x083c,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0862,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0863,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086b,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x086c,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0882,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x088a,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088c,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0897,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0898,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x089b,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x08a6,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x08a9,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08aa,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0917,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0918,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091d,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x091e,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x092f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x093b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0956,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0958,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x095b,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0960,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0967,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x096a,   2, clif->pReqCloseBuyingStore);
}

// 20130724
if (packetVersion == 20130724)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x023b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0437,   8, clif->pDull);
//  packet(UNKNOWN,                       0x085a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085e,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0860,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x0867,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0874,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0876,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0890,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0891,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0893,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x089c,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0921,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0924,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x092e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x094a,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0953,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0954,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0956,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x0958,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x095c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x095d,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0965,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0966,   5, clif->pChangeDir);
}

// 20130731
if (packetVersion == 20130731)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x022d,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0366,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0438,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0863,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0865,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0878,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x087e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0884,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0889,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x088b,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0894,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0895,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0923,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION,       0x0925,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092b,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x092d,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0935,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0941,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094c,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0956,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0957,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0958,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x095b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0962,   6, clif->pDropItem);
}

// 20130807
if (packetVersion == 20130807)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0887,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130814
if (packetVersion == 20130814)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x0202,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0281,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0368,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0815,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0835,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0868,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0873,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0874,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0885,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x0887,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x088a,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088c,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0895,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0896,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a4,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0923,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x0926,   6, clif->pTakeItem);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0936,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0937,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x093a,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0941,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x0947,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0958,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0959,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0962,  90, clif->pUseSkillToPosMoreInfo);
}

// 20130821
if (packetVersion == 20130821)
{
    packet(CMSG_SKILL_USE_POSITION,       0x0202,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x0360,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0437,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0438,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07ec,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0815,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0862,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0863,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x086e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0879,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0881,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x088d,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088f,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0892,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a6,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x08ad,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091d,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0923,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x092d,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x093e,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0947,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x0954,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095e,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0964,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0967,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x0969,  26, clif->pPartyInvite2);
}

// 20130828
if (packetVersion == 20130828)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x023b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0361,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0364,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0817,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0873,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087e,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0889,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x089e,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089f,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a3,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x08ac,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0917,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0919,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x091e,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x0923,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x0924,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0928,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0930,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0944,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0946,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x094a,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x094c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x094f,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095d,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0966,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x096a,  19, clif->pWantToConnection);
}

// 20130904
if (packetVersion == 20130904)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0361,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0815,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0835,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0838,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0937,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0940,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x0956,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20130911
if (packetVersion == 20130911)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0364,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0436,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0819,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x085c,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0868,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x086b,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086d,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x087b,   4, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0889,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0891,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a4,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091e,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x093a,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0940,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0948,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0967,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x096a,   8, clif->pMoveToKafra);
}

// 20130917
if (packetVersion == 20130917)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x035f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0811,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0838,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x083c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0863,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0868,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x086e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x086f,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x087a,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x087e,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x088a,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0895,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089d,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a0,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a2,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08a8,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0918,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091c,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x091e,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0921,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0923,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0934,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x094c,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0957,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0959,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095b,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x096a,  26, clif->pFriendsListAdd);
}

// 20130925
if (packetVersion == 20130925)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0438,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x086d,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x086e,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0875,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0876,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087d,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0885,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0887,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088f,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0890,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0940,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x0958,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x095a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x095b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x095c,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0969,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131002
if (packetVersion == 20131002)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x035f,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0437,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0940,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131008
if (packetVersion == 20131008)
{
    packet(CMSG_NAME_REQUEST,             0x0202,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0362,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x07e4,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x083c,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x085d,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085e,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x0863,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086d,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0883,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0884,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0896,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089c,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x08a9,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0917,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x091c,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0923,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0927,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x0932,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x0934,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0936,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x093c,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0947,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0952,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0954,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0956,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x095d,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0969,  -1, clif->pReqOpenBuyingStore);
}

// 20131016
if (packetVersion == 20131016)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x022d,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0281,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0361,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0364,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0366,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x07ec,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0802,   4, clif->pDull);
//  packet(UNKNOWN,                       0x085a,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x085d,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0863,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0864,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0867,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x0870,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0875,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0887,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0888,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088b,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0890,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0894,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x091a,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091e,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x091f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0927,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0946,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x095e,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0962,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0966,   5, clif->pChangeDir);
}

// 20131023
if (packetVersion == 20131023)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0366,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0438,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x0811,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0819,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0835,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085b,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0862,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086e,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0872,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_BEING,          0x0876,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x0885,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x088b,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x088f,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a7,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ab,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ac,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0919,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x091d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0923,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x0924,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0935,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0937,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0944,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094a,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x0951,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0954,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095b,   5, clif->pWalkToXY);
}

// 20131030
if (packetVersion == 20131030)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0365,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087d,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0887,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x08a2,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0925,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131106
if (packetVersion == 20131106)
{
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0363,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0369,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0436,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0802,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0817,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0866,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0867,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0870,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0875,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0887,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0891,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0893,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0894,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a2,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0919,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0935,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x093b,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0942,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0946,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0947,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0948,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0950,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0954,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x095d,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0962,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0967,  -1, clif->pReqTradeBuyingStore);
}

// 20131107
if (packetVersion == 20131107)
{
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0363,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0369,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0436,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0802,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0817,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0866,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0867,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0870,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0875,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0887,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0891,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0893,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0894,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a2,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0919,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0935,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x093b,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0942,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0946,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0947,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0948,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0950,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0954,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x095d,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0962,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0967,  -1, clif->pReqTradeBuyingStore);
}

// 20131108
if (packetVersion == 20131108)
{
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0363,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0369,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0436,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0802,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0817,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0866,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0867,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0870,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0875,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0887,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0891,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0893,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0894,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a2,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0919,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0935,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x093b,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0942,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0946,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0947,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0948,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0950,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0954,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x095d,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0962,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0967,  -1, clif->pReqTradeBuyingStore);
}

// 20131113
if (packetVersion == 20131113)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0364,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0879,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088f,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089a,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x08a8,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091c,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0920,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x0926,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0928,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0929,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x092b,   6, clif->pTakeItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092c,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x092d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x092e,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x092f,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0934,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0936,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x093c,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0949,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0950,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095d,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x095f,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0960,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0964,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0969,  -1, clif->pSearchStoreInfo);
}

// 20131120
if (packetVersion == 20131120)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0361,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0368,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x0802,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0811,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0817,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x085a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x085f,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0862,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0884,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088c,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088d,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x089e,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a1,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a8,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0917,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0926,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0927,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092b,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092d,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x093e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093f,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0940,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0950,   6, clif->pGetCharNameRequest);
    packet(CMSG_PARTY_INVITE2,            0x0953,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0955,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0956,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x095e,  18, clif->pPartyBookingRegisterReq);
}

// 20131127
if (packetVersion == 20131127)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0364,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0365,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0366,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0437,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x07e4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0881,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0884,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088d,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0894,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x089e,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0930,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093a,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_PING,                 0x0940,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x094c,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094f,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095c,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0967,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x096a,   8, clif->pMoveToKafra);
}

// 20131204
if (packetVersion == 20131204)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0365,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_PING,                 0x0366,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x085a,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0869,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x086c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0877,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0879,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x087f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0882,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a0,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a4,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x08a7,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x08ad,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x091f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0928,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x092d,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092f,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0932,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x093b,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0941,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0951,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0958,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0959,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095b,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0961,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0967,   8, clif->pMoveFromKafra);
}

// 20131211
if (packetVersion == 20131211)
{
    packet(CMSG_SKILL_USE_BEING,          0x0202,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0362,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0366,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x083c,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0865,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0873,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0881,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0886,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0892,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x08a3,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a4,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ab,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x092b,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0931,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x094c,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131218
if (packetVersion == 20131218)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x022d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0811,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085c,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x087b,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08ab,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092f,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0947,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131223
if (packetVersion == 20131223)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20131230
if (packetVersion == 20131230)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x022d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_BEING,          0x02c4,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0365,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0369,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0860,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0871,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087b,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x087e,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x087f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0898,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089c,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x08a9,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x091d,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x091e,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x091f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x0926,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_PICKUP,              0x092a,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093d,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x093e,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0943,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0949,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094a,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x0968,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0969,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x096a,  90, clif->pUseSkillToPosMoreInfo);
}

// 20140108
if (packetVersion == 20140108)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0936,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140115
if (packetVersion == 20140115)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0361,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0367,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0802,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0865,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0887,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x088a,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x088e,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a7,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x092d,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0940,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x095b,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x095d,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0965,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0966,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x096a,  18, clif->pPartyBookingRegisterReq);
}

// 20140122
if (packetVersion == 20140122)
{
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0360,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0811,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0863,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0870,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0871,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0872,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x088c,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0890,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0899,  -1, clif->pSearchStoreInfo);
    packet(CMSG_NAME_REQUEST,             0x089d,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x08a2,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08aa,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0917,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091a,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0925,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x092f,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0940,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0941,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x0942,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x094b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094c,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0950,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0952,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0955,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0957,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x095d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x095f,  -1, clif->pReqOpenBuyingStore);
}

// 20140129
if (packetVersion == 20140129)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0361,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0367,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0802,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x083c,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0884,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0885,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0889,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0921,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0924,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x092c,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x094d,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0958,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0961,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140205
if (packetVersion == 20140205)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0938,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140212
if (packetVersion == 20140212)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x02c4,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0369,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0438,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x086e,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0874,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0877,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0878,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087e,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0888,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x089d,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x089e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x08a0,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x08a1,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a7,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ad,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0919,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091b,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0928,   6, clif->pSolveCharName);
    packet(CMSG_MAP_PING,                 0x0930,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0934,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0936,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x093d,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x0944,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0952,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_PICKUP,              0x0953,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0960,   2, clif->pReqCloseBuyingStore);
}

// 20140219
if (packetVersion == 20140219)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0360,   4, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0364,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x0802,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0838,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x085c,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085d,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x085f,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0860,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0868,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x086f,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x087c,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0889,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0898,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089f,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a6,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08aa,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ac,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0921,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0927,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0939,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0946,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0949,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0953,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x095a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_PING,                 0x0961,   6, clif->pTickSend);
}

// 20140226
if (packetVersion == 20140226)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0361,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0362,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0811,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0877,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0887,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0894,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_MOVE_TO_STORAGE,          0x091a,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0921,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0931,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0941,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0962,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0964,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0969,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140305
if (packetVersion == 20140305)
{
    packet(CMSG_ITEM_PICKUP,              0x0202,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0361,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0436,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0438,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x07e4,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0815,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0878,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0934,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x095e,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140312
if (packetVersion == 20140312)
{
    packet(CMSG_SKILL_USE_BEING,          0x0202,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x023b,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0366,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x086f,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0889,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x088c,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x088e,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0891,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0894,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x089b,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089e,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x08a6,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a9,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08ad,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x091b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x091c,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x091e,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x092a,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0948,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x094a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x094b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094c,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0957,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x095d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x095e,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x0966,   8, clif->pMoveToKafra);
}

// 20140319
if (packetVersion == 20140319)
{
    packet(CMSG_ITEM_PICKUP,              0x02c4,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x07e4,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0802,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0811,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0815,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085a,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0864,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0865,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0866,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x086e,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0872,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0883,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x088e,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0890,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0893,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x089f,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a2,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0923,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0933,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0942,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_BEING,          0x0944,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0947,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x094c,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0954,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_PING,                 0x0955,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0961,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0964,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x096a,  -1, clif->pReqOpenBuyingStore);
}

// 20140326
if (packetVersion == 20140326)
{
    packet(CMSG_NAME_REQUEST,             0x0362,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0365,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x07ec,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x083c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0865,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0867,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0869,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086b,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087c,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087e,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION,       0x087f,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0887,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0898,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x08aa,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08ac,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ad,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0918,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0928,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x092a,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093d,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0942,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0945,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0946,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0956,   5, clif->pChangeDir);
    packet(CMSG_MOVE_TO_STORAGE,          0x0959,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095a,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0969,  10, clif->pUseSkillToId);
}

// 20140402
if (packetVersion == 20140402)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x023b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0360,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0364,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x07ec,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085b,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085d,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0867,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0868,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0882,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0883,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x088a,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x088c,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0890,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0896,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089a,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08ac,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x091f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0926,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0933,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x093f,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0944,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0946,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x094c,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0950,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0958,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x095c,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0965,  12, clif->pSearchStoreInfoListItemClick);
}

// 20140409
if (packetVersion == 20140409)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x0819,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085b,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0868,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x086d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0873,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0875,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x087e,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0883,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x0884,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x088a,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0890,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0893,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0896,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a2,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x08a4,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a6,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a7,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x08a9,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0918,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x092e,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0942,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0947,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x094c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095e,  19, clif->pWantToConnection);
}

// 20140416
if (packetVersion == 20140416)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x095c,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140423
if (packetVersion == 20140423)
{
//  packet(UNKNOWN,                       0x022d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x0360,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0436,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0811,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x083c,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085b,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0862,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0863,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0866,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086b,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x086f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0873,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x088b,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x0890,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0895,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0896,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0898,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x089b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x089d,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a8,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x08ad,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x091a,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0920,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_CLICK,        0x094f,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x095e,   4, clif->pDull);
}

// 20140430
if (packetVersion == 20140430)
{
    packet(CMSG_MAP_PING,                 0x023b,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x035f,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0363,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x083c,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0860,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0870,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0871,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0875,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x087f,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0884,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0899,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x08a1,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0940,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094b,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0953,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0956,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x096a,   6, clif->pSolveCharName);
}

// 20140508
if (packetVersion == 20140508)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20140508
if (packetVersion == 20140508)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x022d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x02c4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x0366,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0368,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x07e4,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0802,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0861,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0886,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089b,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0927,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0930,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0932,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0934,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094e,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x096a,  18, clif->pPartyBookingRegisterReq);
}

// 20140514
if (packetVersion == 20140514)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0437,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0817,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0865,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0867,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0868,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0876,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0877,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0885,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0886,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088a,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0895,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089a,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089c,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x08a5,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0918,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091d,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0921,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0925,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x092c,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x092f,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094e,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0958,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095f,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x0962,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0965,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x096a,   6, clif->pDropItem);
}

// 20140521
if (packetVersion == 20140521)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0869,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x088b,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088d,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089c,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ac,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x0968,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140528
if (packetVersion == 20140528)
{
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0202,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x0360,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x085f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0862,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x0872,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0875,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0877,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0879,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x088a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088f,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0894,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0896,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x089d,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x08a4,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a8,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091d,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0929,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0930,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0938,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x093a,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x093f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094b,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x095f,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0963,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0964,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0966,  -1, clif->pReqTradeBuyingStore);
}

// 20140605
if (packetVersion == 20140605)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0361,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0368,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0369,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0817,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0921,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0931,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0940,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094c,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140611
if (packetVersion == 20140611)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0364,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0438,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x07e4,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0838,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0864,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0867,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x086c,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0874,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0878,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x088c,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0891,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0893,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0894,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x08a1,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08a2,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0924,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0936,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x0941,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x094a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094f,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0950,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0951,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0952,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0957,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0958,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0963,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0965,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0969,   2, clif->pSearchStoreInfoNextPage);
}

// 20140611
if (packetVersion == 20140611)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20140612
if (packetVersion == 20140612)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0364,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0438,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x07e4,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0838,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0864,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0867,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x086c,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0874,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0878,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x088c,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0891,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0893,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0894,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x08a1,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08a2,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0924,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0936,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x0941,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x094a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094f,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0950,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0951,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0952,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0957,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0958,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0963,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0965,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0969,   2, clif->pSearchStoreInfoNextPage);
}

// 20140613
if (packetVersion == 20140613)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0364,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0438,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x07e4,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0838,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0864,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0867,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x086c,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0874,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0878,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x088c,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0891,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0893,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0894,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x08a1,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08a2,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0924,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0936,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x0941,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x094a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094f,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0950,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0951,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0952,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0957,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0958,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0963,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0965,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0969,   2, clif->pSearchStoreInfoNextPage);
}

// 20140618
if (packetVersion == 20140618)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x085d,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_PING,                 0x0860,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0861,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0878,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x0884,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0885,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0886,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0890,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x0892,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a6,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a7,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x08ac,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0917,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0929,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x0935,   6, clif->pTakeItem);
    packet(CMSG_PARTY_INVITE2,            0x0938,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0939,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093b,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0945,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0954,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0957,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x095d,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095e,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0962,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0967,   5, clif->pHomMenu);
}

// 20140625
if (packetVersion == 20140625)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x0202,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x023b,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0815,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0817,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0835,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x085a,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0861,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x086b,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0875,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x087b,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0885,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0886,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0888,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088e,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0897,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x08a1,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a2,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x091a,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0923,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0928,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0940,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0946,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0959,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0960,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0968,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0969,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140702
if (packetVersion == 20140702)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x022d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x023b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0364,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0437,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0438,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x07e4,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0815,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0819,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0835,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085a,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x086c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0887,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0892,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0895,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x08a0,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x08a2,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0925,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092c,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0933,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0940,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140709
if (packetVersion == 20140709)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0364,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0860,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0866,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0869,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0875,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_BEING,          0x0877,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0879,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x0887,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0888,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x088b,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0894,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0897,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0898,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ad,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x091a,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0925,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_POSITION,       0x092f,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0931,   6, clif->pSolveCharName);
    packet(CMSG_MAP_PING,                 0x0934,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_SELL,         0x0939,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0940,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x094d,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094e,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094f,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x095f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x0961,   6, clif->pTakeItem);
}

// 20140716
if (packetVersion == 20140716)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0362,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0811,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x085c,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x085f,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0868,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0871,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0881,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x088b,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x088d,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x088f,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0896,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089a,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x089f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a2,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x08ac,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0918,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x091f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0926,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092c,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0938,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x093b,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0947,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0952,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0958,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x0959,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0969,  -1, clif->pItemListWindowSelected);
}

// 20140723
if (packetVersion == 20140723)
{
    packet(CMSG_MAP_PING,                 0x02c4,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0436,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0819,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0838,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085f,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x086d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0888,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0891,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0896,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0898,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x089e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a2,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x08ad,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0927,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092f,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0934,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0935,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0939,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x093d,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0945,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0947,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0948,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x095f,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0960,   5, clif->pChangeDir);
}

// 20140730
if (packetVersion == 20140730)
{
    packet(CMSG_SKILL_USE_POSITION,       0x022d,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0364,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0366,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0367,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0437,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x07ec,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0802,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0815,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0817,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085e,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087d,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087f,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0889,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x088b,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x088d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x0892,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a0,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a6,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a7,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08a9,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x08ad,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x091e,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0924,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x092a,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0934,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0940,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0946,   5, clif->pHomMenu);
}

// 20140806
if (packetVersion == 20140806)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0948,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140813
if (packetVersion == 20140813)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0365,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x07e4,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0802,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0819,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x0868,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0878,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x087c,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0882,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0897,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0899,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x08a3,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08a7,   8, clif->pDull);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0967,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140814
if (packetVersion == 20140814)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0365,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x07e4,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0802,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0819,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x0868,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0878,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x087c,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0882,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0897,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0899,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x08a3,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08a7,   8, clif->pDull);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0967,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140820
if (packetVersion == 20140820)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x035f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x07ec,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0835,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0861,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0864,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0869,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x086e,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0872,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0876,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x0891,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0899,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x089a,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089b,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a3,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a7,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x092f,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0936,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x0937,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x093a,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x093e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094a,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0951,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0952,   7, clif->pActionRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x0956,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0958,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0961,  18, clif->pPartyBookingRegisterReq);
}

// 20140827
if (packetVersion == 20140827)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0943,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140903
if (packetVersion == 20140903)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088f,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0931,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0941,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0943,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0945,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20140917
if (packetVersion == 20140917)
{
    packet(CMSG_PARTY_INVITE2,            0x022d,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0364,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x0365,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0366,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0367,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0838,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0864,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x086d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0889,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0895,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x0897,   6, clif->pTickSend);
    packet(CMSG_NAME_REQUEST,             0x0898,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x089c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x08a8,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0919,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0930,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0949,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x094f,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0951,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0955,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0956,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0957,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095a,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095c,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x095e,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0966,   4, clif->pDull);
}

// 20140924
if (packetVersion == 20140924)
{
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0367,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0815,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0862,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0864,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0865,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0867,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x086d,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x086e,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0886,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0894,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0898,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x089c,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a5,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x08a7,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0918,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x091b,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0925,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0926,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0928,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_OPEN,         0x092b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0934,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0949,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0952,  26, clif->pFriendsListAdd);
}

// 20141001
if (packetVersion == 20141001)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0361,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0365,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0838,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x087c,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0884,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x0885,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x089c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089d,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ad,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x091c,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x092a,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0937,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0939,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x093f,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094b,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x0952,   5, clif->pHomMenu);
}

// 20141008
if (packetVersion == 20141008)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0942,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141015
if (packetVersion == 20141015)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x022d,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0364,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086e,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0922,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0936,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094b,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0967,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141016
if (packetVersion == 20141016)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x022d,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0364,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086e,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0922,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0936,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094b,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0967,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141022
if (packetVersion == 20141022)
{
    packet(CMSG_SKILL_USE_POSITION,       0x023b,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x0438,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0835,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0878,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087d,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0896,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0899,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08aa,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x08ab,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ad,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091a,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x092b,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x093b,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0940,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x094e,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0955,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141029
if (packetVersion == 20141029)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0369,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0437,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0940,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141105
if (packetVersion == 20141105)
{
//  packet(UNKNOWN,                       0x022d,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x035f,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0360,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x085c,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0863,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x0864,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0865,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0871,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0874,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0875,   7, clif->pActionRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0877,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0879,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0887,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0892,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0898,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a0,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a5,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a7,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ad,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091d,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x091e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x092b,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0944,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0948,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x0950,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0957,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_SELL,         0x095f,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0968,  18, clif->pPartyBookingRegisterReq);
}

// 20141112
if (packetVersion == 20141112)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0362,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0438,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x07e4,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0835,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x083c,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x085f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0863,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0869,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x086c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0871,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0885,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0886,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0887,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a0,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a1,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x08ab,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0919,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x0926,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0929,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0943,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094b,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x094c,   4, clif->pDull);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0955,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x095d,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0960,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0962,  10, clif->pUseSkillToPos);
}

// 20141119
if (packetVersion == 20141119)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0202,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0861,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0865,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0866,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0872,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0873,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0875,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087c,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0887,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0888,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0895,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x08a8,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08aa,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x0918,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0920,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0921,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x092f,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0933,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0938,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0940,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_TO_STORAGE,          0x0941,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0942,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0948,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094c,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0963,  90, clif->pUseSkillToPosMoreInfo);
}

// 20141126
if (packetVersion == 20141126)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0367,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0802,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0819,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086e,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0871,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0884,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0896,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a4,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x08ad,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0920,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0942,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x095a,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x095b,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x095f,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0965,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141203
if (packetVersion == 20141203)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0202,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x0281,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0362,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0367,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0368,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0802,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x0861,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086d,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x086e,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087b,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0880,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0889,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0898,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x089c,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a5,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08aa,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0917,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x091c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x091d,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x0928,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x092a,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0936,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0957,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0962,  10, clif->pUseSkillToId);
}

// 20141210
if (packetVersion == 20141210)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0436,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07e4,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x087b,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0885,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ac,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0917,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0927,   8, clif->pDull);
//  packet(UNKNOWN,                       0x092b,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0947,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0954,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0955,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0958,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0961,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0963,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x0967,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20141217
if (packetVersion == 20141217)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x0360,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x085b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x0860,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0875,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0878,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0879,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x0883,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0889,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x088e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x08a1,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x08a2,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x08a8,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08ab,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x08ac,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0919,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x091f,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0924,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0930,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0932,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0933,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x0936,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0939,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0940,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0944,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0945,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0948,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0960,   4, clif->pDull);
}

// 20141224
if (packetVersion == 20141224)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x0361,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0438,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0835,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x085a,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0865,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x086c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0870,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x087a,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x087b,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089a,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x089b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x08a3,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a8,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08ac,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0930,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0932,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x093a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_PING,                 0x0945,   6, clif->pTickSend);
    packet(CMSG_NAME_REQUEST,             0x0946,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0949,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094f,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0950,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0953,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0956,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x095b,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095f,   7, clif->pActionRequest);
}

// 20141231
if (packetVersion == 20141231)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x086d,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150107
if (packetVersion == 20150107)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087c,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0895,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x092d,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0943,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0947,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150114
if (packetVersion == 20150114)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0436,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x083c,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0868,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0899,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0946,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0955,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0957,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150121
if (packetVersion == 20150121)
{
//  packet(UNKNOWN,                       0x0281,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x087c,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088b,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x089d,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089e,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x08ab,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0918,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0919,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x091d,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0955,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0959,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0963,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0967,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150128
if (packetVersion == 20150128)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x023b,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0365,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0368,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0838,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0864,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x086d,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0870,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x0874,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0875,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0888,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x08ab,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x091f,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0929,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092d,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0938,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x093a,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0944,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094e,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0963,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0968,   6, clif->pDropItem);
}

// 20150129
if (packetVersion == 20150129)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x023b,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0365,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0368,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0838,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0864,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x086d,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0870,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x0874,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0875,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0888,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x08ab,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x091f,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0929,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092d,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0938,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x093a,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0944,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094e,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0963,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0968,   6, clif->pDropItem);
}

// 20150130
if (packetVersion == 20150130)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x023b,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0365,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0368,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0838,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0864,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x086d,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0870,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x0874,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0875,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0888,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x08ab,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x091f,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0929,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092d,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0938,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x093a,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0944,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094e,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0963,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0968,   6, clif->pDropItem);
}

// 20150204
if (packetVersion == 20150204)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0966,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150211
if (packetVersion == 20150211)
{
//  packet(UNKNOWN,                       0x023b,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0368,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0369,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0436,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0437,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x07e4,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0817,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0819,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0835,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0862,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0863,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0870,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0873,   5, clif->pWalkToXY);
    packet(CMSG_NAME_REQUEST,             0x087b,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x087f,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0882,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0883,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0885,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0886,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x089c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a0,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a4,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08aa,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0919,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0920,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0944,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0951,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0957,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0958,   8, clif->pMoveToKafra);
}

// 20150217
if (packetVersion == 20150217)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085b,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150225
if (packetVersion == 20150225)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x02c4,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0360,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0362,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x0436,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0817,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0867,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0896,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089b,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x089c,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0940,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0946,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0948,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x094f,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0952,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0955,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x096a,  19, clif->pWantToConnection);
}

// 20150225
if (packetVersion == 20150225)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20150226
if (packetVersion == 20150226)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x02c4,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0360,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0362,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x0436,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0817,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0867,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0896,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x089b,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x089c,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x08a4,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0940,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0946,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0948,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x094f,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0952,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0955,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x096a,  19, clif->pWantToConnection);
}

// 20150304
if (packetVersion == 20150304)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0363,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0802,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0862,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0879,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087e,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0892,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x089a,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x093a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0947,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x095d,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0960,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0961,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150311
if (packetVersion == 20150311)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x023b,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0360,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0436,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0438,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0815,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0838,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x086a,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x086c,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x087b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0883,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0886,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0888,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0896,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x08a1,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x08a3,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a5,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a6,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0928,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x092a,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x092e,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x093b,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0943,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0946,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0957,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0958,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x095b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0963,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0964,   8, clif->pMoveToKafra);
}

// 20150318
if (packetVersion == 20150318)
{
//  packet(UNKNOWN,                       0x0202,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x023b,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0281,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x07e4,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0802,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0811,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0862,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0863,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0873,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0885,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0889,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x088c,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x089c,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a4,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091d,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0920,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0927,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0928,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x0936,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x0937,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0938,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x093a,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x093c,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x094c,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0951,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0958,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0959,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0960,  18, clif->pPartyBookingRegisterReq);
}

// 20150325
if (packetVersion == 20150325)
{
    packet(CMSG_PARTY_INVITE2,            0x0202,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_BEING,          0x0365,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0438,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0802,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0819,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085d,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x087c,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087e,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0883,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x0885,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0891,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0893,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0897,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0899,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a1,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08a7,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x0919,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x092c,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0931,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0932,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0938,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0940,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0947,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094a,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0950,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x0954,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0969,   8, clif->pMoveFromKafra);
}

// 20150401
if (packetVersion == 20150401)
{
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0362,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x0367,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0437,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x083c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x086f,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0875,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x087e,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088c,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x088f,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0895,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0898,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x089c,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a5,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x091b,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0922,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0924,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0938,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0939,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x093a,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x093b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x093e,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0946,   4, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0949,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x094b,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0953,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095f,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0964,   5, clif->pChangeDir);
}

// 20150408
if (packetVersion == 20150408)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x0819,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x085a,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_BEING,          0x085c,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x085e,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0865,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0868,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x086e,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0878,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x087e,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x087f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0888,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0889,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0891,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0898,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089c,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a2,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a4,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x091b,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0922,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x092a,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0946,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x094f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0955,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0957,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0959,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0963,   2, clif->pSearchStoreInfoNextPage);
}

// 20150415
if (packetVersion == 20150415)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x0361,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0364,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0366,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0368,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0802,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x0817,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0835,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x085e,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0863,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0867,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x0868,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x0869,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x086c,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0880,   8, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x088e,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0891,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0898,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a0,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0922,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x092e,   6, clif->pSolveCharName);
    packet(CMSG_MAP_PING,                 0x093c,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0941,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0946,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094d,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0953,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x095c,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0960,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0961,  18, clif->pPartyBookingRegisterReq);
}

// 20150422
if (packetVersion == 20150422)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0955,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150429
if (packetVersion == 20150429)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0363,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0867,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086a,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0886,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x088f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0894,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0899,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089f,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x08a6,   8, clif->pDull);
//  packet(UNKNOWN,                       0x08a8,   4, clif->pDull);
//  packet(UNKNOWN,                       0x08ad,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093d,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x0943,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150507
if (packetVersion == 20150507)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x023b,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0362,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0817,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x085a,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0864,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0887,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0889,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0924,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x092e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x093b,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0941,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0942,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0953,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0955,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0958,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150513
if (packetVersion == 20150513)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x022d,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x02c4,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0363,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0817,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0864,   8, clif->pMoveFromKafra);
    packet(CMSG_MOVE_TO_STORAGE,          0x0879,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0883,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0885,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a8,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0923,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0924,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0927,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x094a,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0958,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0960,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150520
if (packetVersion == 20150520)
{
    packet(CMSG_MAP_PING,                 0x0202,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_SELL,         0x0361,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0835,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0865,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0868,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x0880,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0882,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x089c,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089e,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x08a2,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ad,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x091c,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x091d,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0924,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092b,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0931,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0936,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093d,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0940,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0945,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x094e,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x095b,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095f,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0960,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0961,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x096a,  18, clif->pPartyBookingRegisterReq);
}

// 20150527
if (packetVersion == 20150527)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x0838,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x083c,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0940,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150603
if (packetVersion == 20150603)
{
    packet(CMSG_PARTY_INVITE2,            0x0361,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0437,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0811,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0819,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0860,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0864,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0867,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086a,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0873,   7, clif->pActionRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0877,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0881,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0884,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088b,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0897,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x089d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x089e,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x08a1,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ad,  19, clif->pWantToConnection);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091b,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x092d,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x093b,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x093f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0955,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0956,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0960,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0969,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x096a,   5, clif->pWalkToXY);
}

// 20150610
if (packetVersion == 20150610)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x022d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0438,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x07e4,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0835,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0870,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0872,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0877,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x087e,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0884,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0885,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x088c,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x088d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x088f,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0897,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x08a0,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ac,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0925,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x092b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x092c,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x092e,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0932,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093e,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0940,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0946,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0949,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x0957,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0964,   5, clif->pChangeDir);
}

// 20150617
if (packetVersion == 20150617)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0360,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0362,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0363,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0365,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x07ec,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0811,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0869,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x086a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086b,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x0870,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x087a,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0886,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0894,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094e,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150618
if (packetVersion == 20150618)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0360,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0362,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0363,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0365,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x07ec,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0811,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0869,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x086a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086b,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x0870,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x087a,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0886,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0894,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094e,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150624
if (packetVersion == 20150624)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0364,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0365,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0870,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0940,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0966,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150701
if (packetVersion == 20150701)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x023b,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0281,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x07e4,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0802,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x086d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x087d,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0883,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088e,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0893,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a0,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a4,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x08a5,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x08a6,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x08ad,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x0919,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0923,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0928,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092c,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x0946,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094e,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x0954,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0956,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0958,   7, clif->pActionRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x095f,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0960,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0968,   5, clif->pChangeDir);
}

// 20150702
if (packetVersion == 20150702)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x023b,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0281,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x07e4,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0802,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x086d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x087d,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0883,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088e,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0893,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a0,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a4,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x08a5,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x08a6,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x08ad,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x0919,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0923,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0928,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092c,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x0946,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094e,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x0954,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0956,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0958,   7, clif->pActionRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x095f,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0960,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0968,   5, clif->pChangeDir);
}

// 20150708
if (packetVersion == 20150708)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x022d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x02c4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0366,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0368,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0436,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0811,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x085e,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x0872,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x087f,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0884,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089d,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a5,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x08ad,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x091f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x092a,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093c,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095b,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0962,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150715
if (packetVersion == 20150715)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x023b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0362,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0364,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0436,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x0437,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0438,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_SELL,         0x0835,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x083c,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x085c,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086f,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0873,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0879,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087c,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087f,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0886,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0895,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0896,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0897,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0899,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x089a,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08a4,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ac,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0917,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x093e,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0944,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0950,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0956,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x0961,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0965,  18, clif->pPartyBookingRegisterReq);
}

// 20150722
if (packetVersion == 20150722)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x023b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0360,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0361,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0368,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0436,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0811,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0817,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0869,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087d,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087e,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0880,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0884,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x088d,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0899,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a8,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0919,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0921,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0925,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092e,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x093c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x094c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0951,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0958,  36, clif->pStoragePassword);
}

// 20150729
if (packetVersion == 20150729)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0437,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0438,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x085b,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0860,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x086c,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x086d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x086e,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086f,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0870,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0880,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0881,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0886,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x089a,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089b,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a3,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a4,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x08ac,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08ad,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0920,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x092f,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x093a,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094f,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0955,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x095e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0961,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x096a,   2, clif->pReqCloseBuyingStore);
}

// 20150805
if (packetVersion == 20150805)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x088a,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150812
if (packetVersion == 20150812)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x087f,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150819
if (packetVersion == 20150819)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0202,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_BEING,          0x022d,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0281,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085d,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0862,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0865,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0871,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0888,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0919,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x091e,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0927,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0961,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0967,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20150826
if (packetVersion == 20150826)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0362,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0368,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0436,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x07ec,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0819,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0861,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0865,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x086b,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0870,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087b,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088b,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088d,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0890,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x0891,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a0,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a1,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x08a4,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a8,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0924,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0928,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x093b,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0945,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094f,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0951,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0959,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0964,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0968,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0969,  -1, clif->pSearchStoreInfo);
}

// 20150902
if (packetVersion == 20150902)
{
//  packet(UNKNOWN,                       0x023b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0360,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x0367,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0802,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x083c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085b,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x0863,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x086f,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087b,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x087f,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0886,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0887,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0889,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088d,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0892,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x0897,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x0899,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a9,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0923,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x0928,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x092a,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x092d,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0941,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0947,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0953,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095b,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0960,   2, clif->pSearchStoreInfoNextPage);
}

// 20150909
if (packetVersion == 20150909)
{
    packet(CMSG_PLAYER_CHANGE_ACT,        0x023b,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0360,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0361,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0365,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x0369,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0437,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0861,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0871,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x087b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x0883,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0886,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x088f,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0895,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0928,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0940,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0941,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095e,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0962,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x096a,   5, clif->pChangeDir);
}

// 20150916
if (packetVersion == 20150916)
{
    packet(CMSG_SKILL_USE_POSITION,       0x022d,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0817,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0835,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x085e,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0869,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0873,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0877,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x0881,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x089b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x089c,   6, clif->pTakeItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x089e,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_PING,                 0x08ac,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0920,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0924,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x092e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x092f,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0934,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0936,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0938,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x093e,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0941,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0942,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0948,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x095a,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0960,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0961,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0969,  19, clif->pWantToConnection);
}

// 20150923
if (packetVersion == 20150923)
{
//  packet(UNKNOWN,                       0x0361,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0366,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x07e4,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0817,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085c,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x085d,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0864,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x086f,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0870,   5, clif->pChangeDir);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0879,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0886,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_OPEN,         0x088e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0892,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0895,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x089b,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x089f,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_PING,                 0x08a0,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a2,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x08a5,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a6,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x091e,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x092b,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0930,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0936,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x093b,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0951,   7, clif->pActionRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0961,  -1, clif->pItemListWindowSelected);
}

// 20151001
if (packetVersion == 20151001)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0960,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151007
if (packetVersion == 20151007)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x0202,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0811,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0862,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x093f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x095f,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0961,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0967,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151014
if (packetVersion == 20151014)
{
//  packet(UNKNOWN,                       0x0202,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0817,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0838,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x085a,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085c,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0860,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0863,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0867,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0872,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION,       0x0874,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0881,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0883,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0884,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0889,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x088e,   8, clif->pDull);
//  packet(UNKNOWN,                       0x089a,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089f,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_PICKUP,              0x08aa,   6, clif->pTakeItem);
    packet(CMSG_PARTY_INVITE2,            0x091c,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x091d,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0930,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0934,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0944,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x094f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0956,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x095e,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x0961,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0964,  19, clif->pWantToConnection);
}

// 20151021
if (packetVersion == 20151021)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x023b,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x02c4,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x0361,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086a,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x091d,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0940,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151022
if (packetVersion == 20151022)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x023b,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x02c4,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x0361,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086a,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x091d,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0940,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151028
if (packetVersion == 20151028)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0860,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151029
if (packetVersion == 20151029)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0860,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151104
if (packetVersion == 20151104)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x023b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0360,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0363,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0437,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x07ec,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0811,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x0815,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x0886,   6, clif->pTickSend);
    packet(CMSG_NAME_REQUEST,             0x0887,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x088b,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x088d,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x08a3,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x08a5,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0928,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0939,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0940,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x0964,   6, clif->pTakeItem);
}

// 20151111
if (packetVersion == 20151111)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x02c4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0369,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0802,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0815,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x085d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0862,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0871,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0885,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089c,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0942,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x094a,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0958,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0966,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0967,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0969,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151118
if (packetVersion == 20151118)
{
    packet(CMSG_MAP_PING,                 0x022d,   6, clif->pTickSend);
    packet(CMSG_HOMUNCULUS_MENU,          0x035f,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0365,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x086b,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x088b,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ab,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0921,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0925,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x092e,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x092f,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x093c,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0943,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0957,   4, clif->pDull);
//  packet(UNKNOWN,                       0x095c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151125
if (packetVersion == 20151125)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0361,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0365,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0366,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0368,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0438,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0802,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0838,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x085e,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x085f,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0863,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0883,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0884,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0885,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x088c,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089c,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x089f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a9,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x08ad,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0920,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x092a,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x092e,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0939,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x093e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x0951,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0956,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0957,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0959,  90, clif->pUseSkillToPosMoreInfo);
}

// 20151202
if (packetVersion == 20151202)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0870,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20151209
if (packetVersion == 20151209)
{
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0365,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x07e4,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x07ec,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x0811,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0819,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x085b,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x085d,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0861,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0866,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0875,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x087f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x088e,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x088f,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x0894,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a1,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_SELL,         0x0920,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x092d,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0930,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0932,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x093b,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0948,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x094a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0956,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x095c,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0961,   7, clif->pActionRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0964,  26, clif->pFriendsListAdd);
}

// 20151216
if (packetVersion == 20151216)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x022d,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0361,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0364,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0436,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x083c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_PING,                 0x085b,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0864,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0865,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x086a,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x086e,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0870,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0874,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x0885,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x088b,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089d,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089e,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a2,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_PICKUP,              0x08a9,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091d,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0944,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0947,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0949,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x0954,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0960,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0966,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_BEING,          0x0968,  10, clif->pUseSkillToId);
}

// 20151223
if (packetVersion == 20151223)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x02c4,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0362,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0364,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0802,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0815,  -1, clif->pSearchStoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0864,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0866,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x086e,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0872,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_BEING,          0x0875,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0876,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0881,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0884,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0886,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x088d,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0890,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0891,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0898,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x08aa,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0918,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x091a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091b,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0920,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0923,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0924,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095e,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x095f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0965,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0967,  12, clif->pSearchStoreInfoListItemClick);
}

// 20151230
if (packetVersion == 20151230)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x02c4,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0366,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0436,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x07ec,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0815,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x085b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0861,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0869,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0886,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x088e,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0897,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091d,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0923,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x093a,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0949,   5, clif->pHomMenu);
    packet(CMSG_ITEM_PICKUP,              0x094e,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160106
if (packetVersion == 20160106)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x07ec,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0861,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086a,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x086c,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0878,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087f,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x0885,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0889,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x088a,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0891,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a0,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091d,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0940,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160113
if (packetVersion == 20160113)
{
    packet(CMSG_SKILL_USE_POSITION,       0x022d,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x023b,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x035f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x0815,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085b,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0864,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x086d,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0873,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0875,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0888,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x088b,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0892,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0893,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089a,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x08a0,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x08a6,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08aa,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0919,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_NAME_REQUEST,             0x091b,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0924,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0930,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0932,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x093c,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0941,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094d,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0967,  -1, clif->pReqOpenBuyingStore);
}

// 20160120
if (packetVersion == 20160120)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0865,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160127
if (packetVersion == 20160127)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085e,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0922,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x095a,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0961,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160203
if (packetVersion == 20160203)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x0202,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0361,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0436,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0437,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x07e4,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0811,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0819,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0835,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x086c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0872,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0873,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x088c,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0918,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x093e,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0940,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0947,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0954,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x095a,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x095d,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160211
if (packetVersion == 20160211)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x023b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0365,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086c,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0870,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0886,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160217
if (packetVersion == 20160217)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x0202,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x023b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0362,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0365,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x0864,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0870,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0873,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087a,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0888,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x088d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088f,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0899,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a0,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x08a9,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x08ac,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08ad,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x091d,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0920,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0926,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0941,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x094a,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x095e,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x0966,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0967,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0969,  -1, clif->pReqOpenBuyingStore);
}

// 20160224
if (packetVersion == 20160224)
{
    packet(CMSG_PARTY_INVITE2,            0x022d,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_BEING,          0x0364,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0369,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0436,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x083c,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0861,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x086b,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0884,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0885,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0888,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a9,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0920,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0929,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x092f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0936,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x0938,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x094c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0961,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160302
if (packetVersion == 20160302)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x022d,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0367,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0802,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0819,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x085b,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0864,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0865,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0867,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0868,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0873,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0875,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x087a,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x087d,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x0883,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a6,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a9,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091a,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0927,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x092d,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0945,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x094e,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0950,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0957,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x095a,   6, clif->pTickSend);
    packet(CMSG_MOVE_TO_STORAGE,          0x0960,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0961,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0967,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0968,   7, clif->pActionRequest);
}

// 20160309
if (packetVersion == 20160309)
{
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x023b,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0281,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0361,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x0819,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0838,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x083c,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x085a,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x085f,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0866,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086a,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0873,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087c,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x087e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089d,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x08a7,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x091d,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0920,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0922,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0929,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x092a,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x092e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0932,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x094f,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x0956,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x096a,   6, clif->pTakeItem);
}

// 20160316
if (packetVersion == 20160316)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160318
if (packetVersion == 20160318)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160323
if (packetVersion == 20160323)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0365,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0867,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0869,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086a,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0872,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0878,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0883,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0896,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x089a,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x091b,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0926,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0927,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0933,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x093c,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160330
if (packetVersion == 20160330)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0365,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0867,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x086d,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0878,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087f,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0889,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x088b,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x088d,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0918,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0925,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x092a,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x092c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0930,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0939,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093b,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160406
if (packetVersion == 20160406)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0364,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x07e4,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x085a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION,       0x085c,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0869,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0877,   6, clif->pTakeItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0878,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0879,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0884,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0892,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0895,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x0898,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x089b,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089e,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a1,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a9,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x08ac,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0927,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0933,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x0934,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x0940,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0949,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0953,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x095d,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x095f,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0962,   4, clif->pDull);
}

// 20160414
if (packetVersion == 20160414)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0362,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0363,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0862,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0880,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0885,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x089e,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0918,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0922,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x0927,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0931,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0934,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0945,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0953,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160420
if (packetVersion == 20160420)
{
    packet(CMSG_SEARCHSTORE_SEARCH,       0x022d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x02c4,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x035f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0819,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0864,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0870,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0872,   5, clif->pChangeDir);
    packet(CMSG_MOVE_TO_STORAGE,          0x0874,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0884,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088b,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x08a5,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x092f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0935,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x094e,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x095c,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160427
if (packetVersion == 20160427)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x0835,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0940,  -1, clif->pSearchStoreInfo);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160504
if (packetVersion == 20160504)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x0202,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0363,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0365,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x083c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085f,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x086b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x087f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0884,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x0886,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0887,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x088a,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x088d,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0890,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0893,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0898,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089d,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ad,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0918,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0921,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0922,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0924,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0940,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0941,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0948,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0952,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0969,  36, clif->pStoragePassword);
}

// 20160511
if (packetVersion == 20160511)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085e,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0894,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x089b,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0918,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0940,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160518
if (packetVersion == 20160518)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086c,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0874,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x089a,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a9,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0928,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160525
if (packetVersion == 20160525)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085a,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085e,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086a,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089c,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x091d,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x092c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0937,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0945,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x094a,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0951,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0956,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160526
if (packetVersion == 20160526)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085a,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085e,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0867,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086a,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089c,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x091d,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x092c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0937,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0945,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x094a,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0951,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0956,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160601
if (packetVersion == 20160601)
{
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0202,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0817,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0863,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0870,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088d,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x088f,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x0895,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x08a7,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ac,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0924,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x095b,   5, clif->pChangeDir);
    packet(CMSG_STORAGE_PASSWORD,         0x095f,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0961,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160608
if (packetVersion == 20160608)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x022d,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x02c4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x035f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0436,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0437,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0802,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085c,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0885,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0889,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x089b,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a6,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x093b,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094d,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0958,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x095b,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0969,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160609
if (packetVersion == 20160609)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x022d,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x02c4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x035f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0436,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0437,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x07ec,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0802,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085c,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0885,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0889,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0899,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x089b,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a6,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x093b,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x094d,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0958,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x095b,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0969,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160615
if (packetVersion == 20160615)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0281,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0363,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0369,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x083c,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0866,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0870,   4, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x087e,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x087f,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0884,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x0887,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0888,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_PING,                 0x088a,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x0891,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0898,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x093e,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0947,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0948,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0954,   5, clif->pChangeDir);
    packet(CMSG_MOVE_TO_STORAGE,          0x0957,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0958,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x095c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x095e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0961,  12, clif->pSearchStoreInfoListItemClick);
}

// 20160622
if (packetVersion == 20160622)
{
    packet(CMSG_SKILL_USE_POSITION,       0x023b,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_FROM_STORAGE,        0x035f,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0361,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x07e4,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_SELL,         0x0861,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0865,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0867,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0880,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0887,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0890,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0891,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0892,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x089a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089e,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a2,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x08a8,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_PICKUP,              0x091c,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x092d,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_BEING,          0x092f,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0936,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0937,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x093b,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0959,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0965,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0969,   6, clif->pDropItem);
}

// 20160629
if (packetVersion == 20160629)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x022d,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x035f,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0363,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0368,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085c,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x085e,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0860,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0861,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0863,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x0867,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0881,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x0885,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x088e,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x0893,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x091e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0922,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0925,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0926,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x093e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0946,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0948,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094a,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0957,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095a,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0968,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x0969,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x096a,   8, clif->pMoveToKafra);
}

// 20160630
if (packetVersion == 20160630)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x022d,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x035f,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0363,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0368,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085c,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x085e,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0860,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0861,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0863,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x0867,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0881,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x0885,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x088e,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x0893,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x091e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0922,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0925,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0926,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x093e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0946,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0948,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094a,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0957,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095a,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0968,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x0969,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x096a,   8, clif->pMoveToKafra);
}

// 20160706
if (packetVersion == 20160706)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0362,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x085f,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0860,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0869,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0884,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0886,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0892,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0899,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08a4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a5,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x08a8,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0918,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x091b,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x0924,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0926,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0927,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0929,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092d,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0939,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x093d,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0944,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0945,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x094c,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0952,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0957,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x0958,   6, clif->pTakeItem);
}

// 20160713
if (packetVersion == 20160713)
{
    packet(CMSG_STORAGE_PASSWORD,         0x022d,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0363,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0364,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0838,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0860,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0865,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0869,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0875,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x0877,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x087b,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0883,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x088d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0892,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x089a,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x089f,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a2,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a4,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x091c,   5, clif->pWalkToXY);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0921,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0922,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x092c,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0931,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0939,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0944,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0945,   7, clif->pActionRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x0947,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0957,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x095b,   8, clif->pDull);
}

// 20160720
if (packetVersion == 20160720)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0362,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0363,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0365,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x07e4,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0838,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x086d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0883,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0887,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0897,  -1, clif->pSearchStoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x089a,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x089c,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089e,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a0,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08aa,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0917,   4, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x091c,   6, clif->pTakeItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092a,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x093b,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_PING,                 0x0946,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094d,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0953,   8, clif->pDull);
//  packet(UNKNOWN,                       0x095b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0960,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0969,  26, clif->pPartyInvite2);
}

// 20160727
if (packetVersion == 20160727)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x023b,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0362,  -1, clif->pSearchStoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0363,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0436,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x07ec,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0866,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0868,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0869,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0874,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0877,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0883,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0887,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x088e,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x0891,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x089f,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x08a2,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08a4,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x08a7,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092e,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0936,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0941,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_NAME_REQUEST,             0x0946,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0949,   8, clif->pMoveFromKafra);
    packet(CMSG_MOVE_TO_STORAGE,          0x0951,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x095f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0966,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0969,  -1, clif->pItemListWindowSelected);
}

// 20160803
if (packetVersion == 20160803)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0364,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085d,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0878,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087f,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0881,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0886,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x0887,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0888,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0891,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0895,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089c,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x089e,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a1,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x091b,   6, clif->pTakeItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0929,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0930,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x0932,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0934,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0937,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x093a,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x093e,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0952,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0955,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0956,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0959,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x095a,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x096a,  -1, clif->pReqTradeBuyingStore);
}

// 20160810
if (packetVersion == 20160810)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0361,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x0819,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0838,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085e,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x085f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0860,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0875,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0879,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087a,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0885,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0888,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0890,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089d,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a9,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x091a,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x091b,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x091c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x0926,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x092b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x092d,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0935,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x0943,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x094b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0959,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095b,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0967,   8, clif->pDull);
}

// 20160817
if (packetVersion == 20160817)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x0364,   6, clif->pReqClickBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0437,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x07e4,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0811,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0819,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x083c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x085c,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0867,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SOLVE_CHAR_NAME,          0x086b,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0875,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087f,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0886,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0891,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x089c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x08a5,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x08a6,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08a9,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x08ab,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION,       0x08ac,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ad,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0929,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x092d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0930,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0939,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x093c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0957,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0960,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0963,   5, clif->pChangeDir);
}

// 20160824
if (packetVersion == 20160824)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0934,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160831
if (packetVersion == 20160831)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x022d,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0835,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0865,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086d,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0870,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x0874,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0876,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0878,   7, clif->pActionRequest);
    packet(CMSG_NAME_REQUEST,             0x087c,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a8,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x08a9,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0917,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x091b,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092c,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0938,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0946,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094a,   5, clif->pChangeDir);
    packet(CMSG_MOVE_TO_STORAGE,          0x094f,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0950,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0954,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0957,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x095e,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x0960,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0964,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x0967,  10, clif->pUseSkillToId);
}

// 20160907
if (packetVersion == 20160907)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x091c,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160913
if (packetVersion == 20160913)
{
//  packet(UNKNOWN,                       0x0361,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0817,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_NAME_REQUEST,             0x0865,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0874,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0875,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0879,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x087b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0887,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0889,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x088e,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_SELL,         0x088f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0891,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION,       0x0892,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089b,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x089c,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a5,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0935,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x093a,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0949,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x094a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0950,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0952,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0954,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0962,   4, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0963,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0968,   2, clif->pReqCloseBuyingStore);
}

// 20160921
if (packetVersion == 20160921)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x094a,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20160928
if (packetVersion == 20160928)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0202,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x035f,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0366,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0811,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0838,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0864,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0866,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086d,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0872,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0878,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0889,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0897,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089a,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x08a2,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a9,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0919,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x091e,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_BEING,          0x0927,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x092d,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0944,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094e,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0953,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0955,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x0957,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x095a,   4, clif->pDull);
}

// 20161005
if (packetVersion == 20161005)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0202,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0368,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0838,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x0863,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0886,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088e,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0891,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0892,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PARTY_INVITE2,            0x089b,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x089c,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a0,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08ac,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x08ad,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0918,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0919,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x091e,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x092b,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0931,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0932,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x093b,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0942,   6, clif->pDropItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0944,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0945,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x094d,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0952,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095b,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x0967,  10, clif->pUseSkillToId);
}

// 20161012
if (packetVersion == 20161012)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x023b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0364,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x0365,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x0369,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x07ec,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0819,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x085b,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0863,   7, clif->pActionRequest);
    packet(CMSG_ITEM_PICKUP,              0x0868,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086d,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0872,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0875,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION,       0x0880,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x0893,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a0,   5, clif->pChangeDir);
    packet(CMSG_SOLVE_CHAR_NAME,          0x092d,   6, clif->pSolveCharName);
    packet(CMSG_NAME_REQUEST,             0x0936,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0937,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0939,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0943,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0944,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0951,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x095c,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0962,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0966,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x0967,  36, clif->pStoragePassword);
}

// 20161019
if (packetVersion == 20161019)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x022d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_HOMUNCULUS_MENU,          0x0360,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0361,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0437,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0889,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0892,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0946,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0963,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20161026
if (packetVersion == 20161026)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0363,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0438,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x085a,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085f,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0861,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0862,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086a,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x086c,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x086e,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087a,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x087c,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION,       0x087f,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0886,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0891,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0894,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0898,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091a,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x091b,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0926,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x092c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x092e,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x092f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0930,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094b,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0953,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x095c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x095e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0962,   5, clif->pChangeDir);
}

// 20161102
if (packetVersion == 20161102)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x0361,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0367,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0436,   7, clif->pActionRequest);
    packet(CMSG_ITEM_PICKUP,              0x0802,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0838,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x083c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_BEING,          0x085f,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086c,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0874,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0886,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x088f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0890,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x089f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a2,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08aa,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x091b,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0922,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0925,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x092f,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0936,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0946,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0949,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0964,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x0965,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x0966,   6, clif->pTickSend);
}

// 20161103
if (packetVersion == 20161103)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x0361,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0367,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0436,   7, clif->pActionRequest);
    packet(CMSG_ITEM_PICKUP,              0x0802,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0838,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x083c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_BEING,          0x085f,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0869,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x086c,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0874,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0886,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x088f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0890,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x089f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a2,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08aa,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x091b,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0922,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0925,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x092f,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0936,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0946,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0949,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0964,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x0965,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x0966,   6, clif->pTickSend);
}

// 20161109
if (packetVersion == 20161109)
{
//  packet(UNKNOWN,                       0x02c4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0361,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0362,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x0365,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0366,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0835,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0865,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x086a,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x086d,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0870,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0876,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0881,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0891,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_PICKUP,              0x0898,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089a,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x089d,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x089f,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a7,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_SELL,         0x08ad,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0927,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0937,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x093c,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x093f,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0954,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0956,   5, clif->pChangeDir);
}

// 20161116
if (packetVersion == 20161116)
{
    packet(CMSG_MAP_PING,                 0x0368,   6, clif->pTickSend);
    packet(CMSG_MOVE_TO_STORAGE,          0x0369,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0835,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085f,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0864,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x086f,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0885,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x088b,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x088d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088f,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0890,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0892,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0893,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a1,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x08a2,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08aa,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ac,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0920,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_POSITION,       0x0925,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x092a,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0931,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x093c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x094a,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0952,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0957,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x095b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x095d,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095f,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0967,   8, clif->pDull);
}

// 20161123
if (packetVersion == 20161123)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x035f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0362,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0437,   6, clif->pTickSend);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085c,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0861,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0862,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0866,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086f,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0871,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x087f,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0880,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x0882,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x088b,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089c,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x08a9,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08aa,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x091a,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0926,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_OPEN,         0x092a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x092f,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0930,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x094d,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094f,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x095a,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x095b,   7, clif->pActionRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0962,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x096a,   6, clif->pSolveCharName);
}

// 20161130
if (packetVersion == 20161130)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x035f,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0361,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x088f,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0931,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0943,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0954,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0959,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20161207
if (packetVersion == 20161207)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x023b,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0361,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0867,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0868,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0875,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087e,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0886,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a1,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x08a2,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08ad,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0918,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091d,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0943,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x095d,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x0965,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20161214
if (packetVersion == 20161214)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x022d,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0281,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x02c4,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_HOMUNCULUS_MENU,          0x0360,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0369,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0436,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0819,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x085a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0862,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x086d,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0887,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0895,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0899,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x08a6,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x092e,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x093d,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20161221
if (packetVersion == 20161221)
{
    packet(CMSG_ITEM_PICKUP,              0x035f,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0366,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0438,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0817,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0866,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0876,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_NAME_REQUEST,             0x0881,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0884,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0885,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_SELL,         0x088c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0890,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0899,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089a,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089b,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x08aa,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x091e,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x0926,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x092c,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x092e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0930,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0943,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x0946,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0964,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0965,   5, clif->pChangeDir);
}

// 20161228
if (packetVersion == 20161228)
{
    packet(CMSG_BUYINGSTORE_SELL,         0x0362,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x085a,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x085e,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0865,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086a,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x086c,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086d,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0870,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0871,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0875,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0886,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0889,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0893,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x089f,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x08a2,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a3,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x08a5,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08ab,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08ac,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x08ad,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091c,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION,       0x0929,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092c,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x0934,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0935,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0938,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093d,   7, clif->pActionRequest);
    packet(CMSG_MAP_PING,                 0x0944,   6, clif->pTickSend);
}

// 20170104
if (packetVersion == 20170104)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0438,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x083c,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x085a,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x087f,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0896,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x091b,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0940,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170111
if (packetVersion == 20170111)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x085d,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0877,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x087f,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x088a,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a1,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x08a3,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x08a6,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x091a,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091b,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094c,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0961,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0969,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170118
if (packetVersion == 20170118)
{
    packet(CMSG_SKILL_USE_POSITION,       0x022d,  10, clif->pUseSkillToPos);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0364,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0436,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0438,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0862,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0865,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x086f,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0873,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x089e,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ad,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x091f,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0927,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0933,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0958,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0962,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x096a,   8, clif->pMoveToKafra);
}

// 20170125
if (packetVersion == 20170125)
{
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0438,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0811,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x086e,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0877,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_BEING,          0x0879,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x087b,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x087d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0881,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0884,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0894,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0895,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0898,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x089b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a5,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x091b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x091c,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0920,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0929,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION,       0x092b,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0930,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x093c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0943,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x0944,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x095c,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x0965,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0968,   2, clif->pSearchStoreInfoNextPage);
}

// 20170201
if (packetVersion == 20170201)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0815,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x085d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085e,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0875,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0879,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0881,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0884,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0885,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_STORAGE_PASSWORD,         0x0886,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088b,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x08a4,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0919,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0920,  26, clif->pPartyInvite2);
    packet(CMSG_HOMUNCULUS_MENU,          0x0938,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0940,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094c,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0966,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0969,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170208
if (packetVersion == 20170208)
{
//  packet(UNKNOWN,                       0x02c4,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0367,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x085c,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0860,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x088c,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0892,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a1,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ac,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0921,   6, clif->pDropItem);
    packet(CMSG_ITEM_PICKUP,              0x0923,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x092d,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0932,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0937,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170215
if (packetVersion == 20170215)
{
    packet(CMSG_STORAGE_PASSWORD,         0x02c4,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x035f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0811,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x083c,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085c,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0876,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087c,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087d,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x087e,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0883,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0884,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x088a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x088b,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0890,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0896,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x089b,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a2,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x08a8,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0925,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x092b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092d,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x0942,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x094e,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x095f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0962,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0969,   5, clif->pWalkToXY);
}

// 20170222
if (packetVersion == 20170222)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x0202,   5, clif->pHomMenu);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x085f,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0866,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0870,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0871,   4, clif->pDull);
//  packet(UNKNOWN,                       0x0877,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0894,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a3,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x08a8,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0937,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0939,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x0943,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x095d,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0962,   8, clif->pMoveFromKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170228
if (packetVersion == 20170228)
{
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x022d,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0360,   7, clif->pActionRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0819,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x085e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0863,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086b,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0873,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0874,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0876,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0883,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0884,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0889,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x0893,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x089e,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a0,  26, clif->pFriendsListAdd);
    packet(CMSG_STORAGE_PASSWORD,         0x08a2,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x08a6,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a7,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x091f,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x092a,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x092e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0937,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x093e,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0944,   8, clif->pMoveFromKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0947,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x0948,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x0952,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0955,  18, clif->pPartyBookingRegisterReq);
}

// 20170308
if (packetVersion == 20170308)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x087d,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170315
if (packetVersion == 20170315)
{
    packet(CMSG_SKILL_USE_POSITION,       0x02c4,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x035f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0360,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0366,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0436,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x07ec,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x085c,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0863,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086a,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0872,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x087b,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0884,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x088b,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x088d,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x088f,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_BEING,          0x0892,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x08aa,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091a,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CREATE,       0x091b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x091d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0920,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0922,  -1, clif->pItemListWindowSelected);
    packet(CMSG_NAME_REQUEST,             0x0944,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x094a,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x094e,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0950,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_STORAGE_PASSWORD,         0x0952,  36, clif->pStoragePassword);
}

// 20170322
if (packetVersion == 20170322)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x091a,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170329
if (packetVersion == 20170329)
{
    packet(CMSG_PARTY_INVITE2,            0x0281,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0363,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085d,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0888,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a8,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0917,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0926,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0929,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092e,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0937,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0939,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0949,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x095f,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170405
if (packetVersion == 20170405)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x022d,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0281,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0362,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0363,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x0369,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0835,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x085f,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0860,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x0864,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0865,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086f,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0893,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a5,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x094c,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x094f,   8, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0964,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170412
if (packetVersion == 20170412)
{
    packet(CMSG_SKILL_USE_POSITION,       0x023b,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0365,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0863,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0869,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086d,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0878,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0879,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x087b,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0890,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0893,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0898,   6, clif->pSolveCharName);
    packet(CMSG_HOMUNCULUS_MENU,          0x089a,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089c,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08a1,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091a,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091e,  19, clif->pWantToConnection);
    packet(CMSG_MAP_PING,                 0x0929,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x092e,   8, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0938,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0942,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0945,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0949,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x094f,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0952,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0959,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x095b,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095c,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x095d,  36, clif->pStoragePassword);
}

// 20170419
if (packetVersion == 20170419)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0811,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0819,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_NAME_REQUEST,             0x0838,   6, clif->pGetCharNameRequest);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085a,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085e,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x0862,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0868,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x086a,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0872,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0881,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x088f,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0897,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x0898,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x089d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x08aa,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091b,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x0920,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0922,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0930,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0931,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0935,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093a,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION,       0x093f,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0942,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_PICKUP,              0x095c,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x095d,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0963,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0965,   6, clif->pReqClickBuyingStore);
}

// 20170426
if (packetVersion == 20170426)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0281,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0866,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0887,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0899,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x089c,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a2,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x08a4,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x091f,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0927,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0940,   6, clif->pDropItem);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0958,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0963,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170502
if (packetVersion == 20170502)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0875,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0894,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089c,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x093c,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0950,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170517
if (packetVersion == 20170517)
{
//  packet(UNKNOWN,                       0x0364,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0367,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_BEING,          0x0815,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x0817,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0868,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0875,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087b,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x087d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088c,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x088d,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x0894,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0896,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x0899,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x089e,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089f,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MAP_PING,                 0x08a2,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a8,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x08aa,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x091b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0923,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x093b,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0945,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0946,   6, clif->pReqClickBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0947,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0958,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0960,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x0964,   6, clif->pTakeItem);
}

// 20170524
if (packetVersion == 20170524)
{
    packet(CMSG_PARTY_INVITE2,            0x0364,  26, clif->pPartyInvite2);
    packet(CMSG_STORAGE_PASSWORD,         0x0368,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0802,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x085e,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x085f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0860,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0864,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0866,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0868,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x086d,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0873,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0874,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_CREATE,       0x087d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0882,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x088d,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0894,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089c,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08a1,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x091e,   8, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0923,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0925,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0934,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0946,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0958,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x095a,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x095b,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0964,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0967,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0968,   6, clif->pSolveCharName);
}

// 20170531
if (packetVersion == 20170531)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_BEING,          0x0369,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x07e4,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_SELL,         0x07ec,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x0819,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x085b,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085f,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0861,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0868,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0873,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0875,  10, clif->pUseSkillToPos);
    packet(CMSG_PARTY_INVITE2,            0x0878,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x087b,   7, clif->pActionRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0885,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x088b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x088d,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0894,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x089a,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x089c,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a2,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08ac,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x08ad,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x092d,   6, clif->pGetCharNameRequest);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0933,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0937,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x0940,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0945,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0963,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0968,  -1, clif->pSearchStoreInfo);
}

// 20170607
if (packetVersion == 20170607)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0361,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0364,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x07e4,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085a,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x085e,  -1, clif->pSearchStoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0862,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0863,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0864,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0871,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0873,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0875,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0885,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088a,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x0897,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x089d,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x08a9,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08ab,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0917,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0918,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x0919,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0925,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_POSITION,       0x0927,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0931,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0934,   5, clif->pWalkToXY);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0938,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x093d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0942,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0944,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0949,  -1, clif->pReqOpenBuyingStore);
}

// 20170614
if (packetVersion == 20170614)
{
    packet(CMSG_MOVE_FROM_STORAGE,        0x023b,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0361,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0364,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0367,   6, clif->pDropItem);
    packet(CMSG_STORAGE_PASSWORD,         0x0437,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x0838,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x083c,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0860,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0865,   4, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0866,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0867,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x086b,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086c,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x0877,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0879,   8, clif->pMoveToKafra);
    packet(CMSG_SOLVE_CHAR_NAME,          0x087d,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087e,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0889,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0899,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a2,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x08ad,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x091b,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0928,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_SELL,         0x092f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0936,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0944,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0957,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0963,  12, clif->pSearchStoreInfoListItemClick);
}

// 20170621
if (packetVersion == 20170621)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0202,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0361,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0365,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0366,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x07e4,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0802,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x085d,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x087d,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x0885,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0889,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a8,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0956,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0957,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x095b,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x095c,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0961,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170628
if (packetVersion == 20170628)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0863,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170705
if (packetVersion == 20170705)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0202,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x02c4,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0879,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0886,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x088d,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x088e,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089a,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089d,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091a,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x092f,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0930,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0932,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0934,   8, clif->pMoveFromKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x094c,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170712
if (packetVersion == 20170712)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0944,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170719
if (packetVersion == 20170719)
{
    packet(CMSG_PLAYER_CHANGE_ACT,        0x022d,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0367,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0368,   4, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0369,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x07e4,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x085a,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0863,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x086e,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x087d,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0881,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0882,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0885,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x0891,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_BEING,          0x0898,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x089a,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x089d,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a6,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a8,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091b,   6, clif->pDropItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091f,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x092c,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x092e,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x092f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x093d,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x093e,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0944,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0946,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0966,   8, clif->pMoveToKafra);
}

// 20170726
if (packetVersion == 20170726)
{
    packet(CMSG_BUYINGSTORE_CREATE,       0x0363,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0366,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x0369,  10, clif->pUseSkillToId);
    packet(CMSG_PARTY_INVITE2,            0x0438,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0838,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0873,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0874,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0878,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0881,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0888,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x088e,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x08a3,   8, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x08a7,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x08aa,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_PICKUP,              0x08ab,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x08ac,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091d,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_CLICK,        0x091e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_HOMUNCULUS_MENU,          0x091f,   5, clif->pHomMenu);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0921,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_SELL,         0x0923,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0943,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x094f,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0950,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0952,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0954,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x095a,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0963,  -1, clif->pSearchStoreInfo);
}

// 20170801
if (packetVersion == 20170801)
{
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x022d,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0281,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0361,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x0362,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x087d,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x08a6,   4, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094f,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x095a,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170809
if (packetVersion == 20170809)
{
//  packet(UNKNOWN,                       0x022d,   4, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0281,   8, clif->pMoveFromKafra);
    packet(CMSG_PARTY_INVITE2,            0x0364,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0366,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0367,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x07e4,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0802,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0835,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0868,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x086e,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x086f,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION,       0x0876,  10, clif->pUseSkillToPos);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0880,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x088c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0890,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0892,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0895,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0899,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_SELL,         0x08a3,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a6,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_PICKUP,              0x0918,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0921,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x092a,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x092b,   6, clif->pTickSend);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0931,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0939,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x093b,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0940,  18, clif->pPartyBookingRegisterReq);
}

// 20170816
if (packetVersion == 20170816)
{
    packet(CMSG_NAME_REQUEST,             0x022d,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x035f,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0361,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0362,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0438,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x085a,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION,       0x0862,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0864,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x087e,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0881,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0882,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0884,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0888,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x0889,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a3,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a7,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x08a9,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x08ac,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x091c,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0921,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0925,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CREATE,       0x092c,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x093a,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x093d,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x0940,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0941,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0950,   5, clif->pChangeDir);
    packet(CMSG_MOVE_TO_STORAGE,          0x0959,   8, clif->pMoveToKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0960,  -1, clif->pSearchStoreInfo);
}

// 20170823
if (packetVersion == 20170823)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0361,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0802,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086c,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x086d,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08ac,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x095b,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170830
if (packetVersion == 20170830)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0281,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x02c4,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0363,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0364,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0860,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0865,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x086a,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0875,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0884,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0885,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x0888,  10, clif->pUseSkillToId);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0897,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0899,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x089a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_TO_STORAGE,          0x089e,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x08a2,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a8,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_PING,                 0x091e,   6, clif->pTickSend);
    packet(CMSG_HOMUNCULUS_MENU,          0x0921,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0925,   6, clif->pReqClickBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x092e,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0939,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093e,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0940,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0942,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0943,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_SELL,         0x0947,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0951,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x0959,  10, clif->pUseSkillToPos);
}

// 20170906
if (packetVersion == 20170906)
{
//  packet(UNKNOWN,                       0x0202,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0281,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x02c4,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0366,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0802,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0860,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0866,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x086c,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x087b,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a2,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a3,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x08a7,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_ITEM_PICKUP,              0x091a,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x091e,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0953,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20170913
if (packetVersion == 20170913)
{
    packet(CMSG_NAME_REQUEST,             0x0281,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x035f,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0437,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MOVE_FROM_STORAGE,        0x07e4,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0817,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0835,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x085a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0860,   6, clif->pTakeItem);
    packet(CMSG_PARTY_INVITE2,            0x0865,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0866,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x088c,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0890,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MOVE_TO_STORAGE,          0x0891,   8, clif->pMoveToKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0892,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a6,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x08a7,   4, clif->pDull);
    packet(CMSG_SKILL_USE_BEING,          0x08aa,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x08ab,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ac,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ad,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x091b,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091d,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x091e,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0920,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0923,   5, clif->pWalkToXY);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0925,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x0927,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x095a,  10, clif->pUseSkillToPos);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095c,   6, clif->pSolveCharName);
}

// 20170920
if (packetVersion == 20170920)
{
    packet(CMSG_ITEM_PICKUP,              0x0369,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0436,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_FROM_STORAGE,        0x07ec,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x085a,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0861,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_BEING,          0x0862,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0864,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0865,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x086a,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x086c,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0874,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0875,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x0889,   6, clif->pGetCharNameRequest);
    packet(CMSG_MAP_PING,                 0x088e,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089b,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x0919,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x091e,   8, clif->pDull);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0921,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0923,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x0926,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x092e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0937,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0939,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0945,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x095d,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0961,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0966,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x096a,   2, clif->pSearchStoreInfoNextPage);
}

// 20170927
if (packetVersion == 20170927)
{
    packet(CMSG_HOMUNCULUS_MENU,          0x02c4,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x035f,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0361,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0362,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0366,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_BEING,          0x085c,  10, clif->pUseSkillToId);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0873,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0875,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_SELL,         0x087d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x087e,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x088b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0899,   7, clif->pActionRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x089a,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089b,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a3,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a5,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x08a6,   8, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x08ad,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x091e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x0922,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0923,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0927,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x093b,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0942,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x0945,   6, clif->pTickSend);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_PICKUP,              0x094d,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0959,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x095a,  10, clif->pUseSkillToPos);
}

// 20171002
if (packetVersion == 20171002)
{
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x022d,   6, clif->pDropItem);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0363,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_HOMUNCULUS_MENU,          0x0438,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0885,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0897,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_SERVER_CONNECT,       0x089d,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0928,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x092d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x0934,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x093b,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x093d,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x093e,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0943,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x095f,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171011
if (packetVersion == 20171011)
{
    packet(CMSG_PLAYER_CHANGE_DEST,       0x023b,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0437,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x087b,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0882,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0950,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0954,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171018
if (packetVersion == 20171018)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0363,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_ITEM_PICKUP,              0x0436,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x086a,   4, clif->pDull);
//  packet(UNKNOWN,                       0x087a,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x087e,   5, clif->pHomMenu);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0889,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x089a,   6, clif->pDropItem);
    packet(CMSG_PARTY_INVITE2,            0x089f,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a6,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0938,  -1, clif->pItemListWindowSelected);
    packet(CMSG_STORAGE_PASSWORD,         0x0944,  36, clif->pStoragePassword);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094a,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x094f,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171025
if (packetVersion == 20171025)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x08a2,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171101
if (packetVersion == 20171101)
{
    packet(CMSG_STORAGE_PASSWORD,         0x022d,  36, clif->pStoragePassword);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0368,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0369,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0438,   6, clif->pTickSend);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0835,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x085b,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x0860,   6, clif->pGetCharNameRequest);
    packet(CMSG_SKILL_USE_BEING,          0x086c,  10, clif->pUseSkillToId);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0872,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0876,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0886,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x088e,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0890,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0895,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0899,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x089b,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x089c,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a0,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ab,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SEARCHSTORE_CLICK,        0x08ad,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MOVE_TO_STORAGE,          0x091b,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0939,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x094a,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x094d,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0952,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0957,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x095a,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0962,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0966,  10, clif->pUseSkillToPos);
}

// 20171108
if (packetVersion == 20171108)
{
    packet(CMSG_MAP_PING,                 0x0202,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0361,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SOLVE_CHAR_NAME,          0x07e4,   6, clif->pSolveCharName);
    packet(CMSG_STORAGE_PASSWORD,         0x0815,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0819,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0838,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0863,   5, clif->pWalkToXY);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0878,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x087e,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0884,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0896,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0897,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SKILL_USE_BEING,          0x08a2,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a9,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x08ad,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x091d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091f,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0940,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_TO_STORAGE,          0x0941,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x0945,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0947,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0949,  19, clif->pWantToConnection);
    packet(CMSG_PARTY_INVITE2,            0x094e,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0958,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x095a,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0963,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0965,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION,       0x0967,  10, clif->pUseSkillToPos);
}

// 20171115
if (packetVersion == 20171115)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0360,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0365,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0802,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
//  packet(UNKNOWN,                       0x086d,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x086f,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MOVE_FROM_STORAGE,        0x087e,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0883,   8, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x088b,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x0890,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x0898,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a4,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0926,   6, clif->pTakeItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0958,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x095a,  26, clif->pPartyInvite2);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171122
if (packetVersion == 20171122)
{
    packet(CMSG_SOLVE_CHAR_NAME,          0x0281,   6, clif->pSolveCharName);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x02c4,  -1, clif->pSearchStoreInfo);
//  packet(UNKNOWN,                       0x035f,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x0838,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x083c,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x085b,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0862,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0867,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_SELL,         0x0877,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0885,   8, clif->pDull);
    packet(CMSG_MAP_PING,                 0x0890,   6, clif->pTickSend);
    packet(CMSG_ITEM_PICKUP,              0x0891,   6, clif->pTakeItem);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0893,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0897,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0898,   6, clif->pDropItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x089a,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x089e,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION,       0x08a6,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_OPEN,         0x08a9,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x091e,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0920,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0923,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_STORAGE_PASSWORD,         0x0934,  36, clif->pStoragePassword);
    packet(CMSG_SKILL_USE_BEING,          0x093b,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0945,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0946,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0947,   5, clif->pWalkToXY);
    packet(CMSG_PARTY_INVITE2,            0x0962,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0968,  -1, clif->pReqOpenBuyingStore);
}

// 20171129
if (packetVersion == 20171129)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x02c4,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0361,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0363,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0365,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PARTY_INVITE2,            0x0838,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0862,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x086d,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x0876,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0878,  36, clif->pStoragePassword);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x088a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x089c,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a5,   8, clif->pMoveToKafra);
    packet(CMSG_MAP_PING,                 0x0940,   6, clif->pTickSend);
//  packet(UNKNOWN,                       0x094b,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0953,   8, clif->pMoveFromKafra);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0966,  19, clif->pWantToConnection);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171206
if (packetVersion == 20171206)
{
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0867,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x086a,   4, clif->pDull);
//  packet(UNKNOWN,                       0x086e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MAP_PING,                 0x0885,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0888,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0897,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x089d,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x08a2,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x08a4,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091d,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x0923,   8, clif->pMoveToKafra);
//  packet(UNKNOWN,                       0x092e,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0936,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0942,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0958,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x0961,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171213
if (packetVersion == 20171213)
{
    packet(CMSG_PARTY_INVITE2,            0x0202,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0802,  26, clif->pFriendsListAdd);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0860,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0881,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0890,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x091a,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0957,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171220
if (packetVersion == 20171220)
{
    packet(CMSG_MAP_SERVER_CONNECT,       0x0281,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0366,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0369,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0436,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0437,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x085e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0861,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0872,  10, clif->pUseSkillToId);
    packet(CMSG_NAME_REQUEST,             0x0873,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0880,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x0882,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0885,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION,       0x088c,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x0899,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x089e,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a7,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x091b,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x091e,   6, clif->pSolveCharName);
    packet(CMSG_MOVE_TO_STORAGE,          0x0924,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0929,   6, clif->pDropItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0933,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093e,   7, clif->pActionRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0941,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0946,   4, clif->pDull);
//  packet(UNKNOWN,                       0x094e,   8, clif->pDull);
    packet(CMSG_HOMUNCULUS_MENU,          0x0951,   5, clif->pHomMenu);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0957,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0960,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0964,  26, clif->pPartyInvite2);
}

// 20171227
if (packetVersion == 20171227)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0361,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0369,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0802,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x087d,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0888,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x088a,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x088d,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x08a0,   8, clif->pMoveToKafra);
    packet(CMSG_HOMUNCULUS_MENU,          0x08a5,   5, clif->pHomMenu);
    packet(CMSG_MAP_SERVER_CONNECT,       0x092c,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x092e,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x0938,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0945,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0946,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0969,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x096a,  18, clif->pPartyBookingRegisterReq);
}

// 20180103
if (packetVersion == 20180103)
{
    packet(CMSG_MOVE_TO_STORAGE,          0x02c4,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0363,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0811,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0865,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x086b,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x086d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0872,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x0876,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_SELL,         0x0879,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088e,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0899,  26, clif->pFriendsListAdd);
    packet(CMSG_SOLVE_CHAR_NAME,          0x089f,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a9,   5, clif->pChangeDir);
    packet(CMSG_NAME_REQUEST,             0x08ab,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08ac,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x091d,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0922,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x0926,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0927,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_FROM_STORAGE,        0x092c,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_PICKUP,              0x0935,   6, clif->pTakeItem);
    packet(CMSG_SKILL_USE_BEING,          0x0938,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0941,   5, clif->pWalkToXY);
    packet(CMSG_STORAGE_PASSWORD,         0x0946,  36, clif->pStoragePassword);
    packet(CMSG_HOMUNCULUS_MENU,          0x0948,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094e,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x095d,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x095f,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0960,  90, clif->pUseSkillToPosMoreInfo);
}

// 20180117
if (packetVersion == 20180117)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0202,   5, clif->pChangeDir);
    packet(CMSG_MAP_SERVER_CONNECT,       0x022d,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x023b,  26, clif->pFriendsListAdd);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0361,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
//  packet(UNKNOWN,                       0x0436,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x0875,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20180124
if (packetVersion == 20180124)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x0436,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0802,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085f,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0868,   4, clif->pDull);
//  packet(UNKNOWN,                       0x086a,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x087a,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0890,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0919,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0940,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x094d,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0958,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0961,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20180131
if (packetVersion == 20180131)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_MOVE_TO_STORAGE,          0x0436,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0802,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0819,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x085f,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0868,   4, clif->pDull);
//  packet(UNKNOWN,                       0x086a,  18, clif->pPartyBookingRegisterReq);
//  packet(UNKNOWN,                       0x086f,   8, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x087a,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0888,  19, clif->pWantToConnection);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0890,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0919,  26, clif->pFriendsListAdd);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0940,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0946,  -1, clif->pItemListWindowSelected);
    packet(CMSG_ITEM_PICKUP,              0x094d,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0958,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x0961,   5, clif->pHomMenu);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20180207
if (packetVersion == 20180207)
{
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0281,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x035f,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x0360,   4, clif->pDull);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
//  packet(UNKNOWN,                       0x0363,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0364,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0365,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x07e4,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x07ec,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0802,  26, clif->pPartyInvite2);
    packet(CMSG_BUYINGSTORE_SELL,         0x0811,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0870,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0881,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x092c,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x092e,   5, clif->pChangeDir);
    packet(CMSG_MAP_PING,                 0x0940,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0950,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20180213
if (packetVersion == 20180213)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0369,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0802,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0817,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_CLICK,        0x085a,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PARTY_INVITE2,            0x086f,  26, clif->pPartyInvite2);
    packet(CMSG_MAP_PING,                 0x0874,   6, clif->pTickSend);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0875,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0878,   5, clif->pChangeDir);
    packet(CMSG_BUYINGSTORE_OPEN,         0x087b,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_BEING,          0x0882,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x088c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_ITEM_PICKUP,              0x0892,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0898,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x089c,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08a3,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a5,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08a9,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x08ad,   6, clif->pSolveCharName);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0917,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0922,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0924,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0926,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0933,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0936,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_NAME_REQUEST,             0x093c,   6, clif->pGetCharNameRequest);
//  packet(UNKNOWN,                       0x0943,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_MOVE_TO_STORAGE,          0x0955,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x095a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_HOMUNCULUS_MENU,          0x0962,   5, clif->pHomMenu);
}

// 20180221
if (packetVersion == 20180221)
{
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0202,  90, clif->pUseSkillToPosMoreInfo);
//  packet(UNKNOWN,                       0x0366,   8, clif->pDull);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0436,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0838,   5, clif->pChangeDir);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0867,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x086c,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_CREATE,       0x086f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0871,   6, clif->pDropItem);
    packet(CMSG_HOMUNCULUS_MENU,          0x0876,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0879,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x087d,   5, clif->pWalkToXY);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0880,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0881,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x0883,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x088f,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0891,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0897,  19, clif->pWantToConnection);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0899,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_MAP_PING,                 0x089d,   6, clif->pTickSend);
    packet(CMSG_STORAGE_PASSWORD,         0x0917,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_FROM_STORAGE,        0x091e,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0929,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PARTY_INVITE2,            0x093d,  26, clif->pPartyInvite2);
    packet(CMSG_SKILL_USE_BEING,          0x094b,  10, clif->pUseSkillToId);
    packet(CMSG_SKILL_USE_POSITION,       0x094d,  10, clif->pUseSkillToPos);
    packet(CMSG_ITEM_PICKUP,              0x094e,   6, clif->pTakeItem);
    packet(CMSG_NAME_REQUEST,             0x0957,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0964,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x096a,   7, clif->pActionRequest);
}

// 20180307
if (packetVersion == 20180307)
{
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x035f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0437,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x07e4,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0861,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0862,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0864,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x086c,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0870,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0872,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0877,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088d,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0893,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a6,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08aa,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ab,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0917,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x0920,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0937,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0939,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x093d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0944,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0948,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0951,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0954,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0957,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0969,   7, clif->pActionRequest);
}

// 20180309
if (packetVersion == 20180309)
{
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_CREATE,       0x035f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0437,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x07e4,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0861,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0862,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0864,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x086c,   6, clif->pTickSend);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0870,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0872,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0877,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_FROM_STORAGE,        0x088d,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0893,  10, clif->pUseSkillToId);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x08a6,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08aa,  26, clif->pFriendsListAdd);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08ab,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x0917,  10, clif->pUseSkillToPos);
    packet(CMSG_MOVE_TO_STORAGE,          0x0920,   8, clif->pMoveToKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0937,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0939,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x093d,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_HOMUNCULUS_MENU,          0x0944,   5, clif->pHomMenu);
    packet(CMSG_PARTY_INVITE2,            0x0948,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0951,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0954,   6, clif->pTakeItem);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0957,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0969,   7, clif->pActionRequest);
}

// 20180314
if (packetVersion == 20180314)
{
    packet(CMSG_SEARCHSTORE_CLICK,        0x0361,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x0366,  10, clif->pUseSkillToId);
    packet(CMSG_HOMUNCULUS_MENU,          0x0369,   5, clif->pHomMenu);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0436,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x085a,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_PICKUP,              0x0862,   6, clif->pTakeItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0863,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x0868,   8, clif->pDull);
    packet(CMSG_MOVE_FROM_STORAGE,        0x086e,   8, clif->pMoveFromKafra);
    packet(CMSG_BUYINGSTORE_SELL,         0x0874,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_SERVER_CONNECT,       0x087a,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0888,   5, clif->pChangeDir);
    packet(CMSG_SKILL_USE_POSITION,       0x088a,  10, clif->pUseSkillToPos);
    packet(CMSG_STORAGE_PASSWORD,         0x088d,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0894,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x089b,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0921,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0927,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x092f,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0933,   8, clif->pMoveToKafra);
    packet(CMSG_PARTY_INVITE2,            0x0935,  26, clif->pPartyInvite2);
//  packet(UNKNOWN,                       0x0945,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094d,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x094e,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0956,   4, clif->pDull);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0959,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0962,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0967,   6, clif->pSolveCharName);
}

// 20180315
if (packetVersion == 20180315)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180321
if (packetVersion == 20180321)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180328
if (packetVersion == 20180328)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180404
if (packetVersion == 20180404)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180418
if (packetVersion == 20180418)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180425
if (packetVersion == 20180425)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180502
if (packetVersion == 20180502)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180509
if (packetVersion == 20180509)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180516
if (packetVersion == 20180516)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180523
if (packetVersion == 20180523)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180530
if (packetVersion == 20180530)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180605
if (packetVersion == 20180605)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180620
if (packetVersion == 20180620)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180621
if (packetVersion == 20180621)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180704
if (packetVersion == 20180704)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180718
if (packetVersion == 20180718)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180801
if (packetVersion == 20180801)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180808
if (packetVersion == 20180808)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180822
if (packetVersion == 20180822)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180829
if (packetVersion == 20180829)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180831
if (packetVersion == 20180831)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180912
if (packetVersion == 20180912)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20180919
if (packetVersion == 20180919)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181002
if (packetVersion == 20181002)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181017
if (packetVersion == 20181017)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181024
if (packetVersion == 20181024)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181031
if (packetVersion == 20181031)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181107
if (packetVersion == 20181107)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181114
if (packetVersion == 20181114)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  12, clif->pSearchStoreInfoListItemClick);
}

// 20181121
if (packetVersion == 20181121)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  14, clif->pSearchStoreInfoListItemClick);
}

// 20181128
if (packetVersion == 20181128)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  14, clif->pSearchStoreInfoListItemClick);
}

// 20181205
if (packetVersion == 20181205)
{
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0202,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x022d,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x023b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0281,   4, clif->pDull);
    packet(CMSG_PARTY_INVITE2,            0x02c4,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x035f,   5, clif->pWalkToXY);
    packet(CMSG_MAP_PING,                 0x0360,   6, clif->pTickSend);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0361,   5, clif->pChangeDir);
    packet(CMSG_ITEM_PICKUP,              0x0362,   6, clif->pTakeItem);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0363,   6, clif->pDropItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x0364,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0365,   8, clif->pMoveFromKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0366,  10, clif->pUseSkillToPos);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0367,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0368,   6, clif->pGetCharNameRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0369,   6, clif->pSolveCharName);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0436,  19, clif->pWantToConnection);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0437,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x0438,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x07e4,  -1, clif->pItemListWindowSelected);
//  packet(UNKNOWN,                       0x07ec,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0802,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0811,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0815,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0817,   6, clif->pReqClickBuyingStore);
    packet(CMSG_BUYINGSTORE_SELL,         0x0819,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0838,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x083c,  14, clif->pSearchStoreInfoListItemClick);
}

}
