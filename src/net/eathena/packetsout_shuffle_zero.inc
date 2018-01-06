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

#define PACKETSOUT_SHUFFLE_ZERO_VOID

if (packets_zero)
{

// 20171019
if (packetVersion == 20171019)
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

// 20171023
if (packetVersion == 20171023)
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

// 20171024
if (packetVersion == 20171024)
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

// 20171025
if (packetVersion == 20171025)
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

// 20171027
if (packetVersion == 20171027)
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

// 20171030
if (packetVersion == 20171030)
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

// 20171031
if (packetVersion == 20171031)
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

// 20171109
if (packetVersion == 20171109)
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

// 20171113
if (packetVersion == 20171113)
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

// 20171115
if (packetVersion == 20171115)
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
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0881,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x091c,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0959,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0966,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171116
if (packetVersion == 20171116)
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
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0881,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x091c,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0959,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0966,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171117
if (packetVersion == 20171117)
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
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0881,  19, clif->pWantToConnection);
    packet(CMSG_HOMUNCULUS_MENU,          0x091c,   5, clif->pHomMenu);
    packet(CMSG_STORAGE_PASSWORD,         0x0922,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0959,   5, clif->pChangeDir);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0966,  26, clif->pFriendsListAdd);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171121
if (packetVersion == 20171121)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0202,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x022d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0363,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x0811,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0866,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0889,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0892,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089e,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x08ad,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0918,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091f,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0943,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0950,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171122
if (packetVersion == 20171122)
{
    packet(CMSG_STORAGE_PASSWORD,         0x0202,  36, clif->pStoragePassword);
    packet(CMSG_BUYINGSTORE_SELL,         0x022d,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0362,   6, clif->pDropItem);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0363,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0368,   6, clif->pSolveCharName);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0369,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0437,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x0438,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x0811,   5, clif->pHomMenu);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0815,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0817,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0819,  -1, clif->pSearchStoreInfo);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0835,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_CLICK,        0x0838,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SKILL_USE_BEING,          0x083c,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x0866,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0889,   8, clif->pMoveFromKafra);
//  packet(UNKNOWN,                       0x0892,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x089e,   5, clif->pChangeDir);
    packet(CMSG_PARTY_INVITE2,            0x08ad,  26, clif->pPartyInvite2);
    packet(CMSG_ITEM_PICKUP,              0x0918,   6, clif->pTakeItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x091f,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0928,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0943,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0950,   4, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171123
if (packetVersion == 20171123)
{
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0366,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x0367,   5, clif->pHomMenu);
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
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x085f,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0860,  19, clif->pWantToConnection);
//  packet(UNKNOWN,                       0x0876,   4, clif->pDull);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0882,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PARTY_INVITE2,            0x088c,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0896,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x089e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a8,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_TO_STORAGE,          0x092b,   8, clif->pMoveToKafra);
    packet(CMSG_ITEM_PICKUP,              0x0930,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0935,   8, clif->pMoveFromKafra);
    packet(CMSG_STORAGE_PASSWORD,         0x0947,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x0960,   8, clif->pDull);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171127
if (packetVersion == 20171127)
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
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171128
if (packetVersion == 20171128)
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
    packet(CMSG_STORAGE_PASSWORD,         0x0893,  36, clif->pStoragePassword);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171130
if (packetVersion == 20171130)
{
//  packet(UNKNOWN,                       0x0361,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_NAME_REQUEST,             0x0864,   6, clif->pGetCharNameRequest);
    packet(CMSG_SEARCHSTORE_CLICK,        0x086f,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0871,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0872,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0875,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0878,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x0881,  10, clif->pUseSkillToPos);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0884,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0886,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_STORAGE_PASSWORD,         0x0887,  36, clif->pStoragePassword);
    packet(CMSG_MOVE_TO_STORAGE,          0x088b,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0894,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x0899,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x08a0,   5, clif->pChangeDir);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a7,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0925,  -1, clif->pReqOpenBuyingStore);
//  packet(UNKNOWN,                       0x0928,   4, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0930,  19, clif->pWantToConnection);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0931,  26, clif->pFriendsListAdd);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0935,   8, clif->pMoveFromKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x093a,   7, clif->pActionRequest);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0947,  -1, clif->pSearchStoreInfo);
    packet(CMSG_HOMUNCULUS_MENU,          0x094c,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x094f,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_PING,                 0x095b,   6, clif->pTickSend);
    packet(CMSG_PARTY_INVITE2,            0x095f,  26, clif->pPartyInvite2);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0960,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_BEING,          0x0965,  10, clif->pUseSkillToId);
}

// 20171204
if (packetVersion == 20171204)
{
    packet(CMSG_ITEM_PICKUP,              0x0281,   6, clif->pTakeItem);
    packet(CMSG_MAP_PING,                 0x035f,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0360,   6, clif->pReqClickBuyingStore);
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
//  packet(UNKNOWN,                       0x085b,   8, clif->pDull);
    packet(CMSG_MAP_SERVER_CONNECT,       0x086f,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x087d,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0885,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0887,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0892,   4, clif->pDull);
    packet(CMSG_STORAGE_PASSWORD,         0x0896,  36, clif->pStoragePassword);
    packet(CMSG_PARTY_INVITE2,            0x08aa,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0917,   8, clif->pMoveFromKafra);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x091b,  26, clif->pFriendsListAdd);
//  packet(UNKNOWN,                       0x0921,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_HOMUNCULUS_MENU,          0x0924,   5, clif->pHomMenu);
    packet(CMSG_MOVE_TO_STORAGE,          0x0937,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0960,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x096a,   6, clif->pGetCharNameRequest);
}

// 20171206
if (packetVersion == 20171206)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171208
if (packetVersion == 20171208)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171209
if (packetVersion == 20171209)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171210
if (packetVersion == 20171210)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171211
if (packetVersion == 20171211)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171212
if (packetVersion == 20171212)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171213
if (packetVersion == 20171213)
{
    packet(CMSG_SKILL_USE_BEING,          0x0281,  10, clif->pUseSkillToId);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0366,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0436,   6, clif->pReqClickBuyingStore);
    packet(CMSG_ITEM_PICKUP,              0x0835,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x083c,   8, clif->pDull);
//  packet(UNKNOWN,                       0x0860,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0862,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0864,   5, clif->pWalkToXY);
    packet(CMSG_MOVE_TO_STORAGE,          0x0870,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_POSITION,       0x0878,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x087a,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x087b,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0885,   6, clif->pSolveCharName);
    packet(CMSG_PARTY_INVITE2,            0x088d,  26, clif->pPartyInvite2);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0893,  26, clif->pFriendsListAdd);
    packet(CMSG_HOMUNCULUS_MENU,          0x0897,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_CLICK,        0x089e,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_BUYINGSTORE_CREATE,       0x08a4,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x08ac,   7, clif->pActionRequest);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0920,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0934,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0936,   8, clif->pMoveFromKafra);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x093a,  -1, clif->pItemListWindowSelected);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0940,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_NAME_REQUEST,             0x0941,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x0959,  36, clif->pStoragePassword);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x095b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x095c,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x095f,   6, clif->pTickSend);
}

// 20171214
if (packetVersion == 20171214)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x023b,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x02c4,   6, clif->pDropItem);
    packet(CMSG_NAME_REQUEST,             0x0438,   6, clif->pGetCharNameRequest);
    packet(CMSG_STORAGE_PASSWORD,         0x085b,  36, clif->pStoragePassword);
//  packet(UNKNOWN,                       0x085e,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0862,   6, clif->pSolveCharName);
//  packet(UNKNOWN,                       0x0864,   8, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x086a,   5, clif->pWalkToXY);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0871,  19, clif->pWantToConnection);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0873,  -1, clif->pItemListWindowSelected);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0886,   6, clif->pReqClickBuyingStore);
//  packet(UNKNOWN,                       0x088b,   4, clif->pDull);
    packet(CMSG_MOVE_TO_STORAGE,          0x088c,   8, clif->pMoveToKafra);
    packet(CMSG_SKILL_USE_BEING,          0x0892,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_PICKUP,              0x0894,   6, clif->pTakeItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0897,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x08a3,  26, clif->pFriendsListAdd);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x08a5,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x08ac,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0933,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x094a,   5, clif->pChangeDir);
    packet(CMSG_HOMUNCULUS_MENU,          0x094d,   5, clif->pHomMenu);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x094f,  -1, clif->pSearchStoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x0951,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_FROM_STORAGE,        0x095a,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x095b,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_MAP_PING,                 0x0960,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0969,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SKILL_USE_POSITION,       0x096a,  10, clif->pUseSkillToPos);
}

// 20171220
if (packetVersion == 20171220)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x022d,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x0281,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0438,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0819,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0838,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_SELL,         0x085f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0867,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x086c,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0872,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087a,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x087e,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0899,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x089b,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a3,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a5,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ad,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x091f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0921,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x092d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092e,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0932,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0934,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0937,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x094d,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x095b,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x096a,   5, clif->pChangeDir);
}

// 20171221
if (packetVersion == 20171221)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x022d,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_SKILL_USE_POSITION,       0x0281,  10, clif->pUseSkillToPos);
    packet(CMSG_NAME_REQUEST,             0x0438,   6, clif->pGetCharNameRequest);
    packet(CMSG_BUYINGSTORE_OPEN,         0x07ec,   6, clif->pReqClickBuyingStore);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0819,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0835,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_PING,                 0x0838,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_SELL,         0x085f,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0867,   6, clif->pSolveCharName);
    packet(CMSG_ITEM_PICKUP,              0x086c,   6, clif->pTakeItem);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0872,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x087a,  12, clif->pSearchStoreInfoListItemClick);
//  packet(UNKNOWN,                       0x087e,   4, clif->pDull);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0899,   5, clif->pWalkToXY);
//  packet(UNKNOWN,                       0x089b,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x089c,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x08a3,  -1, clif->pItemListWindowSelected);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x08a5,   6, clif->pDropItem);
    packet(CMSG_MAP_SERVER_CONNECT,       0x08ad,  19, clif->pWantToConnection);
    packet(CMSG_BUYINGSTORE_CREATE,       0x091f,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x0921,   5, clif->pHomMenu);
//  packet(UNKNOWN,                       0x092d,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x092e,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0932,  26, clif->pPartyInvite2);
    packet(CMSG_MOVE_TO_STORAGE,          0x0934,   8, clif->pMoveToKafra);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0937,   7, clif->pActionRequest);
    packet(CMSG_SKILL_USE_BEING,          0x094d,  10, clif->pUseSkillToId);
    packet(CMSG_STORAGE_PASSWORD,         0x095b,  36, clif->pStoragePassword);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x096a,   5, clif->pChangeDir);
}

// 20171227
if (packetVersion == 20171227)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x02c4,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0860,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x086c,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0871,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0872,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0874,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0876,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0877,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0878,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x087b,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0885,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088b,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0894,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x089f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091c,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0920,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0925,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x0930,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0939,   6, clif->pTickSend);
    packet(CMSG_NAME_REQUEST,             0x093e,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0945,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0946,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x095e,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0967,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0968,  -1, clif->pItemListWindowSelected);
}

// 20171229
if (packetVersion == 20171229)
{
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x02c4,   2, clif->pSearchStoreInfoNextPage);
    packet(CMSG_PLAYER_CHANGE_DIR,        0x0860,   5, clif->pChangeDir);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x086c,   7, clif->pActionRequest);
    packet(CMSG_SOLVE_CHAR_NAME,          0x0871,   6, clif->pSolveCharName);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0872,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0874,  19, clif->pWantToConnection);
    packet(CMSG_SKILL_USE_POSITION,       0x0876,  10, clif->pUseSkillToPos);
//  packet(UNKNOWN,                       0x0877,   8, clif->pDull);
    packet(CMSG_ITEM_PICKUP,              0x0878,   6, clif->pTakeItem);
    packet(CMSG_MOVE_TO_STORAGE,          0x087b,   8, clif->pMoveToKafra);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0885,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0889,  -1, clif->pSearchStoreInfo);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x088b,   2, clif->pReqCloseBuyingStore);
//  packet(UNKNOWN,                       0x0894,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_OPEN,         0x089f,   6, clif->pReqClickBuyingStore);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x091c,   6, clif->pDropItem);
    packet(CMSG_BUYINGSTORE_SELL,         0x0920,  -1, clif->pReqTradeBuyingStore);
//  packet(UNKNOWN,                       0x0925,   4, clif->pDull);
    packet(CMSG_SEARCHSTORE_CLICK,        0x092d,  12, clif->pSearchStoreInfoListItemClick);
    packet(CMSG_STORAGE_PASSWORD,         0x0930,  36, clif->pStoragePassword);
    packet(CMSG_MAP_PING,                 0x0939,   6, clif->pTickSend);
    packet(CMSG_NAME_REQUEST,             0x093e,   6, clif->pGetCharNameRequest);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x0941,  26, clif->pFriendsListAdd);
    packet(CMSG_PARTY_INVITE2,            0x0945,  26, clif->pPartyInvite2);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x0946,   5, clif->pWalkToXY);
    packet(CMSG_BUYINGSTORE_CREATE,       0x094b,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_HOMUNCULUS_MENU,          0x095e,   5, clif->pHomMenu);
    packet(CMSG_SKILL_USE_BEING,          0x0967,  10, clif->pUseSkillToId);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0968,  -1, clif->pItemListWindowSelected);
}

// 20180103
if (packetVersion == 20180103)
{
    packet(CMSG_PLAYER_CHANGE_DIR,        0x035f,   5, clif->pChangeDir);
//  packet(UNKNOWN,                       0x0360,  18, clif->pPartyBookingRegisterReq);
    packet(CMSG_BUYINGSTORE_SELL,         0x0366,  -1, clif->pReqTradeBuyingStore);
    packet(CMSG_PLAYER_CHANGE_DEST,       0x07e4,   5, clif->pWalkToXY);
    packet(CMSG_SKILL_USE_POSITION,       0x083c,  10, clif->pUseSkillToPos);
    packet(CMSG_HOMUNCULUS_MENU,          0x0860,   5, clif->pHomMenu);
    packet(CMSG_ITEM_LIST_WINDOW_SELECT,  0x0866,  -1, clif->pItemListWindowSelected);
    packet(CMSG_FRIENDS_ADD_PLAYER,       0x086d,  26, clif->pFriendsListAdd);
    packet(CMSG_MAP_PING,                 0x0871,   6, clif->pTickSend);
    packet(CMSG_BUYINGSTORE_CREATE,       0x0872,  -1, clif->pReqOpenBuyingStore);
    packet(CMSG_SEARCHSTORE_NEXT_PAGE,    0x0874,   2, clif->pSearchStoreInfoNextPage);
//  packet(UNKNOWN,                       0x087b,   8, clif->pDull);
    packet(CMSG_BUYINGSTORE_CLOSE,        0x0885,   2, clif->pReqCloseBuyingStore);
    packet(CMSG_PLAYER_CHANGE_ACT,        0x0888,   7, clif->pActionRequest);
    packet(CMSG_PLAYER_INVENTORY_DROP,    0x0890,   6, clif->pDropItem);
    packet(CMSG_SKILL_USE_BEING,          0x0896,  10, clif->pUseSkillToId);
    packet(CMSG_MOVE_TO_STORAGE,          0x089d,   8, clif->pMoveToKafra);
    packet(CMSG_NAME_REQUEST,             0x08a6,   6, clif->pGetCharNameRequest);
    packet(CMSG_ITEM_PICKUP,              0x091e,   6, clif->pTakeItem);
//  packet(UNKNOWN,                       0x0924,   4, clif->pDull);
    packet(CMSG_SKILL_USE_POSITION_MORE,  0x0938,  90, clif->pUseSkillToPosMoreInfo);
    packet(CMSG_PARTY_INVITE2,            0x093b,  26, clif->pPartyInvite2);
    packet(CMSG_SEARCHSTORE_SEARCH,       0x0956,  -1, clif->pSearchStoreInfo);
    packet(CMSG_MAP_SERVER_CONNECT,       0x0958,  19, clif->pWantToConnection);
    packet(CMSG_STORAGE_PASSWORD,         0x095b,  36, clif->pStoragePassword);
    packet(CMSG_SOLVE_CHAR_NAME,          0x095d,   6, clif->pSolveCharName);
    packet(CMSG_BUYINGSTORE_OPEN,         0x0964,   6, clif->pReqClickBuyingStore);
    packet(CMSG_MOVE_FROM_STORAGE,        0x0966,   8, clif->pMoveFromKafra);
    packet(CMSG_SEARCHSTORE_CLICK,        0x096a,  12, clif->pSearchStoreInfoListItemClick);
}

}
