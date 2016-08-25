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

#include "gui/popups/popupmenu.h"

#include "actormanager.h"
#include "configuration.h"
#include "party.h"
#include "settings.h"

#include "being/flooritem.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "const/spells.h"

#include "enums/resources/map/mapitemtype.h"

#include "input/inputmanager.h"

#include "gui/buttontext.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "gui/windowmenu.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/minimap.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/textcommandeditor.h"
#include "gui/windows/textdialog.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "gui/widgets/tabs/chat/whispertab.h"

#include "net/adminhandler.h"
#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/homunculushandler.h"
#include "net/mercenaryhandler.h"
#include "net/npchandler.h"
#include "net/net.h"
#include "net/pethandler.h"
#include "net/serverfeatures.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/guildmanager.h"
#endif

#include "resources/chatobject.h"
#include "resources/iteminfo.h"

#include "resources/db/npcdb.h"

#include "resources/item/item.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"

#include "resources/skill/skillinfo.h"

#include "utils/copynpaste.h"
#include "utils/checkutils.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include "debug.h"

std::string tradePartnerName;

PopupMenu *popupMenu = nullptr;

PopupMenu::PopupMenu() :
    Popup("PopupMenu", "popupmenu.xml"),
    mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "popupbrowserbox.xml")),
    mScrollArea(nullptr),
    mBeingId(BeingId_zero),
    mFloorItemId(BeingId_zero),
    mItemId(0),
    mItemIndex(-1),
    mItemColor(ItemColor_one),
    mMapItem(nullptr),
    mTab(nullptr),
    mSpell(nullptr),
    mWindow(nullptr),
    mRenameListener(),
    mPlayerListener(),
    mDialog(nullptr),
    mButton(nullptr),
    mName(),
    mExtName(),
    mTextField(nullptr),
    mType(ActorType::Unknown),
    mSubType(BeingTypeId_zero),
    mX(0),
    mY(0),
    mAllowCleanMenu(true)
{
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mRenameListener.setMapItem(nullptr);
    mRenameListener.setDialog(nullptr);
    mPlayerListener.setNick("");
    mPlayerListener.setDialog(nullptr);
    mPlayerListener.setType(ActorType::Unknown);
    mScrollArea = new ScrollArea(this, mBrowserBox, false);
    mScrollArea->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    addMouseListener(this);
}

void PopupMenu::postInit()
{
    Popup::postInit();
    add(mScrollArea);
}

void PopupMenu::showPopup(const int x, const int y, const Being *const being)
{
    if (!being || !localPlayer || !actorManager)
        return;

    mBeingId = being->getId();
    mName = being->getName();
    mExtName = being->getExtName();
    mType = being->getType();
    mSubType = being->getSubType();
    mBrowserBox->clearRows();
    mX = x;
    mY = y;

    const std::string &name = mName;
    if (being->getType() != ActorType::SkillUnit)
    {
        mBrowserBox->addRow(name + being->getGenderSignWithSpace());
    }

    switch (being->getType())
    {
        case ActorType::Player:
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade with player
            mBrowserBox->addRow("/trade 'NAME'", _("Trade"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade attack player
            mBrowserBox->addRow("/attack 'NAME'", _("Attack"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: send whisper to player
            mBrowserBox->addRow("/whispertext 'NAME'", _("Whisper"));
            addMailCommands();
            addGmCommands();
            mBrowserBox->addRow("##3---");

            // TRANSLATORS: popup menu item
            // TRANSLATORS: heal player
            mBrowserBox->addRow("/heal :'BEINGID'", _("Heal"));
            mBrowserBox->addRow("##3---");

            addPlayerRelation(name);
            mBrowserBox->addRow("##3---");

            addFollow();
            addPartyName(being->getPartyName());

            const Guild *const guild1 = being->getGuild();
            const Guild *const guild2 = localPlayer->getGuild();
            if (guild2)
            {
                if (guild1)
                {
                    if (guild1->getId() == guild2->getId())
                    {
                        mBrowserBox->addRow("/kickguild 'NAME'",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: kick player from guild
                            _("Kick from guild"));
                        if (guild2->getServerGuild())
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@guild-pos|%s >@@",
                                // TRANSLATORS: popup menu item
                                // TRANSLATORS: change player position in guild
                                _("Change pos in guild")));
                        }
                    }
                }
                else if (guild2->getMember(mName))
                {
                    mBrowserBox->addRow("/kickguild 'NAME'",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: kick player from guild
                        _("Kick from guild"));
                    if (guild2->getServerGuild())
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@guild-pos|%s >@@",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: change player position in guild
                            _("Change pos in guild")));
                    }
                }
                else
                {
#ifdef TMWA_SUPPORT
                    if (guild2->getServerGuild()
                        || (guildManager && guildManager->havePower()))
#endif
                    {
                        mBrowserBox->addRow("/guild 'NAME'",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: invite player to guild
                            _("Invite to guild"));
                    }
                }
            }

            // TRANSLATORS: popup menu item
            // TRANSLATORS: set player invisible for self by id
            mBrowserBox->addRow("/nuke 'NAME'", _("Nuke"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to player location
            mBrowserBox->addRow("/navigateto 'NAME'", _("Move"));
            addPlayerMisc();
            addBuySell(being);
            addChat(being);
            break;
        }

        case ActorType::Npc:
            if (!addBeingMenu())
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: talk with npc
                mBrowserBox->addRow("/talk 'NAME'", _("Talk"));
                if (serverFeatures->haveNpcWhispers())
                {
                    mBrowserBox->addRow("/whispertext NPC:'NAME'",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: whisper to npc
                        _("Whisper"));
                }
                // TRANSLATORS: popup menu item
                // TRANSLATORS: buy from npc
                mBrowserBox->addRow("/buy 'NAME'", _("Buy"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: sell to npc
                mBrowserBox->addRow("/sell 'NAME'", _("Sell"));
            }

            addGmCommands();
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to npc location
            mBrowserBox->addRow("/navigateto 'NAME'", _("Move"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add comment to npc
            mBrowserBox->addRow("addcomment", _("Add comment"));
            addChat(being);
            break;

        case ActorType::Monster:
        {
            // Monsters can be attacked
            // TRANSLATORS: popup menu item
            // TRANSLATORS: attack monster
            mBrowserBox->addRow("/attack :'BEINGID'", _("Attack"));
            addCatchPetCommands();
            addGmCommands();
            mBrowserBox->addRow("##3---");

            if (config.getBoolValue("enableAttackFilter"))
            {
                mBrowserBox->addRow("##3---");
                if (actorManager->isInAttackList(name)
                    || actorManager->isInIgnoreAttackList(name)
                    || actorManager->isInPriorityAttackList(name))
                {
                    mBrowserBox->addRow("/removeattack 'NAME'",
                        // TRANSLATORS: remove monster from attack list
                        // TRANSLATORS: popup menu item
                        _("Remove from attack list"));
                }
                else
                {
                    mBrowserBox->addRow("/addpriorityattack 'NAME'",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to priotiry attack list
                        _("Add to priority attack list"));
                    mBrowserBox->addRow("/addattack 'NAME'",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to attack list
                        _("Add to attack list"));
                    mBrowserBox->addRow("/addignoreattack 'NAME'",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to ignore list
                        _("Add to ignore list"));
                }
            }
            break;
        }

        case ActorType::Mercenary:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: Mercenary move to master
            mBrowserBox->addRow("mercenary to master", _("Move to master"));
            addGmCommands();
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: fire mercenary
            mBrowserBox->addRow("fire mercenary", _("Fire"));
            mBrowserBox->addRow("##3---");
            break;

        case ActorType::Homunculus:
        {
            const HomunculusInfo *const info = PlayerInfo::getHomunculus();
            if (info)
            {
                mBrowserBox->addRow("homunculus to master",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: Mercenary move to master
                    _("Move to master"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: feed homunculus
                mBrowserBox->addRow("homunculus feed", _("Feed"));
                mBrowserBox->addRow("##3---");
                // TRANSLATORS: popup menu item
                // TRANSLATORS: pet rename item
                mBrowserBox->addRow("/sethomunname", _("Rename"));
                mBrowserBox->addRow("##3---");
                // TRANSLATORS: popup menu item
                // TRANSLATORS: delete homunculus
                mBrowserBox->addRow("homunculus delete", _("Kill"));
                mBrowserBox->addRow("##3---");
            }
            addGmCommands();
            break;
        }
        case ActorType::Pet:
            if (being->getOwner() == localPlayer)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: feed pet
                mBrowserBox->addRow("pet feed", _("Feed"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: pet drop loot
                mBrowserBox->addRow("pet drop loot", _("Drop loot"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: pet unequip item
                mBrowserBox->addRow("pet unequip", _("Unequip"));
                addGmCommands();
                mBrowserBox->addRow("##3---");
                // TRANSLATORS: popup menu item
                // TRANSLATORS: pet rename item
                mBrowserBox->addRow("/setpetname", _("Rename"));
                mBrowserBox->addRow("##3---");
                const BeingInfo *const info = being->getInfo();
                std::string msg;
                if (info)
                    msg = info->getString(0);
                if (!msg.empty())
                {
                    mBrowserBox->addRow("pet to egg", msg.c_str());
                }
                else
                {
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: pet return to egg
                    mBrowserBox->addRow("pet to egg", _("Return to egg"));
                }
                mBrowserBox->addRow("##3---");
            }
            else
            {
                addCatchPetCommands();
            }
            break;
        case ActorType::SkillUnit:
        {
            const BeingId id = being->getCreatorId();
            std::string creatorName;
            Being *const creator = actorManager->findBeing(id);
            if (creator)
                creatorName = creator->getName();
            else
                creatorName = actorManager->getSeenPlayerById(id);

            if (creatorName.empty())
                creatorName = strprintf("?%d", CAST_S32(id));

            mBrowserBox->addRow(strprintf("%s, %d (%s)",
                name.c_str(),
                CAST_S32(being->getSubType()),
                creatorName.c_str()));
            break;
        }
        case ActorType::Avatar:
        case ActorType::Unknown:
        case ActorType::FloorItem:
        case ActorType::Portal:
        case ActorType::Elemental:
        default:
            break;
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add being name to chat
    mBrowserBox->addRow("/addtext 'NAME'", _("Add name to chat"));
    mBrowserBox->addRow("##3---");

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

bool PopupMenu::addBeingMenu()
{
    Being *being = actorManager->findBeing(mBeingId);
    if (!being)
        return false;

    BeingInfo *const info = NPCDB::get(fromInt(
        being->getSubType(), BeingTypeId));
    if (!info)
        return false;

    const std::vector<BeingMenuItem> &menu = info->getMenu();
    FOR_EACH (std::vector<BeingMenuItem>::const_iterator, it, menu)
    {
        const BeingMenuItem &item = *it;
        mBrowserBox->addRow("/" + item.command, item.name.c_str());
    }
    return true;
}

void PopupMenu::setMousePos()
{
    if (viewport)
    {
        mX = viewport->mMouseX;
        mY = viewport->mMouseY;
    }
    else
    {
        Gui::getMouseState(mX, mY);
    }
}

void PopupMenu::setMousePos2()
{
    if (mX == 0 && mY == 0)
    {
        if (viewport)
        {
            mX = viewport->mMouseX;
            mY = viewport->mMouseY;
        }
        else
        {
            Gui::getMouseState(mX, mY);
        }
    }
}

void PopupMenu::showPopup(const int x, const int y,
                          const std::vector<ActorSprite*> &beings)
{
    mX = x;
    mY = y;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Players"));
    FOR_EACH (std::vector<ActorSprite*>::const_iterator, it, beings)
    {
        const Being *const being = dynamic_cast<const Being*>(*it);
        const ActorSprite *const actor = *it;
        if (being && !being->getName().empty())
        {
            mBrowserBox->addRow(strprintf("@@player_%u|%s >@@",
                CAST_U32(being->getId()), (being->getName()
                + being->getGenderSignWithSpace()).c_str()));
        }
        else if (actor->getType() == ActorType::FloorItem)
        {
            const FloorItem *const floorItem
                = static_cast<const FloorItem*>(actor);
            mBrowserBox->addRow(strprintf("@@flooritem_%u|%s >@@",
                CAST_U32(actor->getId()),
                floorItem->getName().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(x, y);
}

void PopupMenu::showPlayerPopup(const std::string &nick)
{
    if (nick.empty() || !localPlayer)
        return;

    setMousePos();
    mName = nick;
    mExtName = nick;
    mBeingId = BeingId_zero;
    mType = ActorType::Player;
    mSubType = BeingTypeId_zero;
    mBrowserBox->clearRows();

    const std::string &name = mName;

    mBrowserBox->addRow(name);

    // TRANSLATORS: popup menu item
    // TRANSLATORS: send whisper to player
    mBrowserBox->addRow("/whispertext 'NAME'", _("Whisper"));
    addMailCommands();
    addGmCommands();
    mBrowserBox->addRow("##3---");

    addPlayerRelation(name);
    mBrowserBox->addRow("##3---");

    addFollow();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add comment to player
    mBrowserBox->addRow("addcomment", _("Add comment"));

    if (localPlayer->isInParty())
    {
        const Party *const party = localPlayer->getParty();
        if (party)
        {
            const PartyMember *const member = party->getMember(mName);
            if (member)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from party
                mBrowserBox->addRow("/kickparty 'NAME'", _("Kick from party"));
                mBrowserBox->addRow("##3---");
                const PartyMember *const o = party->getMember(
                    localPlayer->getName());
                showAdoptCommands();
                if (o && member->getMap() == o->getMap())
                {
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: move to player position
                    mBrowserBox->addRow("/navigate 'X' 'Y'", _("Move"));
                }
            }
        }
    }

    const Guild *const guild2 = localPlayer->getGuild();
    if (guild2)
    {
        if (guild2->getMember(mName))
        {
#ifdef TMWA_SUPPORT
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
#endif
            {
                mBrowserBox->addRow("/kickguild 'NAME'",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: kick player from guild
                    _("Kick from guild"));
            }
            if (guild2->getServerGuild())
            {
                mBrowserBox->addRow(strprintf(
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: change player position in guild
                    "@@guild-pos|%s >@@", _("Change pos in guild")));
            }
        }
        else
        {
#ifdef TMWA_SUPPORT
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
#endif
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: invite player to guild
                mBrowserBox->addRow("/guild 'NAME'", _("Invite to guild"));
            }
        }
    }

    addBuySellDefault();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player name to chat
    mBrowserBox->addRow("/addtext 'NAME'", _("Add name to chat"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showPopup(const int x, const int y,
                          const FloorItem *const floorItem)
{
    if (!floorItem)
        return;

    mX = x;
    mY = y;
    mFloorItemId = floorItem->getId();
    mItemId = floorItem->getItemId();
    mType = ActorType::FloorItem;
    mSubType = BeingTypeId_zero;
    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = floorItem->getCard(f);
    mBrowserBox->clearRows();
    const std::string name = floorItem->getName();
    mName = name;
    mExtName = name;

    mBrowserBox->addRow(name);

    if (config.getBoolValue("enablePickupFilter"))
    {
        if (actorManager->isInPickupList(name)
            || (actorManager->isInPickupList("")
            && !actorManager->isInIgnorePickupList(name)))
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: pickup item from ground
            mBrowserBox->addRow("/pickup 'FLOORID'", _("Pick up"));
            mBrowserBox->addRow("##3---");
        }
        addPickupFilter(name);
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: pickup item from ground
        mBrowserBox->addRow("/pickup 'FLOORID'", _("Pick up"));
    }
    addProtection();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add item name to chat
    mBrowserBox->addRow("/addchat 'FLOORID''CARDS'", _("Add to chat"));
    mBrowserBox->addRow("##3---");
    addGmCommands();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showPopup(const int x, const int y, MapItem *const mapItem)
{
    if (!mapItem)
        return;

    mMapItem = mapItem;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Map Item"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: rename map item
    mBrowserBox->addRow("rename map", _("Rename"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove map item
    mBrowserBox->addRow("remove map", _("Remove"));

    if (localPlayer && localPlayer->isGM())
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp to map item
        mBrowserBox->addRow("/slide 'MAPX' 'MAPY'", _("Warp"));
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showMapPopup(const int x, const int y,
                             const int x2, const int y2,
                             const bool isMinimap)
{
    mX = x2;
    mY = y2;

    if (isMinimap)
        mWindow = minimap;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Map Item"));

    if (localPlayer && localPlayer->isGM())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp to map item
        mBrowserBox->addRow("/slide 'MAPX' 'MAPY'", _("Warp"));
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: move to map item
    mBrowserBox->addRow("/navigate 'X' 'Y'", _("Move"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: move camera to map item
    mBrowserBox->addRow("/movecamera 'X' 'Y'", _("Move camera"));
    if (settings.cameraMode != 0)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: restore camera to default view
        mBrowserBox->addRow("/restorecamera", _("Restore camera"));
    }
    mBrowserBox->addRow("##3---");
    if (isMinimap)
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("window close", _("Close"));
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showOutfitsWindowPopup(const int x, const int y)
{
    mX = x;
    mY = y;
    mWindow = outfitWindow;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Outfits"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: clear selected outfit
    mBrowserBox->addRow("clear outfit", _("Clear outfit"));
    mBrowserBox->addRow("##3---");

    addWindowMenu(outfitWindow);
    mBrowserBox->addRow("##3---");

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showSpellPopup(const int x, const int y,
                               TextCommand *const cmd)
{
    if (!cmd)
        return;

    mBrowserBox->clearRows();

    mSpell = cmd;
    mX = x;
    mY = y;

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Spells"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: edit selected spell
    mBrowserBox->addRow("edit spell", _("Edit spell"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChatPopup(const int x, const int y, ChatTab *const tab)
{
    if (!tab || !actorManager || !localPlayer)
        return;

    mTab = tab;
    mX = x;
    mY = y;
    mWindow = chatWindow;

    mBrowserBox->clearRows();

    const ChatTabTypeT &type = tab->getType();
    if (type == ChatTabType::WHISPER || type == ChatTabType::CHANNEL)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close chat tab
        mBrowserBox->addRow("chat close", _("Close"));
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove all text from chat tab
    mBrowserBox->addRow("/chatclear", _("Clear"));
    mBrowserBox->addRow("##3---");

    if (tab->getAllowHighlight())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: disable chat tab highlight
        mBrowserBox->addRow("disable highlight", _("Disable highlight"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable chat tab highlight
        mBrowserBox->addRow("enable highlight", _("Enable highlight"));
    }
    if (tab->getRemoveNames())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: do not remove player names from chat tab
        mBrowserBox->addRow("dont remove name", _("Don't remove name"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: remove player names from chat tab
        mBrowserBox->addRow("remove name", _("Remove name"));
    }
    if (tab->getNoAway())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable away messages in chat tab
        mBrowserBox->addRow("enable away", _("Enable away"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: disable away messages in chat tab
        mBrowserBox->addRow("disable away", _("Disable away"));
    }
    mBrowserBox->addRow("##3---");
    if (type == ChatTabType::PARTY)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable away messages in chat tab
        mBrowserBox->addRow("/leaveparty", _("Leave"));
        mBrowserBox->addRow("##3---");
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy selected text to clipboard
    mBrowserBox->addRow("/chatclipboard 'X' 'Y'", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");

    if (type == ChatTabType::WHISPER)
    {
        const WhisperTab *const wTab = static_cast<WhisperTab*>(tab);
        std::string name = wTab->getNick();

        const Being* const being  = actorManager->findBeingByName(
            name, ActorType::Player);

        addGmCommands();
        if (being)
        {
            mBeingId = being->getId();
            mName = being->getName();
            mExtName = being->getExtName();
            mType = being->getType();
            mSubType = being->getSubType();

            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade with player
            mBrowserBox->addRow("/trade 'NAME'", _("Trade"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: attack player
            mBrowserBox->addRow("/attack 'NAME'", _("Attack"));
            addMailCommands();
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: heal player
            mBrowserBox->addRow("/heal :'BEINGID'", _("Heal"));
            mBrowserBox->addRow("##3---");
            addPlayerRelation(name);
            mBrowserBox->addRow("##3---");
            addFollow();
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to player position
            mBrowserBox->addRow("/navigateto 'NAME'", _("Move"));
            addPlayerMisc();
            addBuySell(being);
            mBrowserBox->addRow("##3---");
            addParty(wTab->getNick());
            const Guild *const guild1 = being->getGuild();
            const Guild *const guild2 = localPlayer->getGuild();
            if (guild2)
            {
                if (guild1)
                {
                    if (guild1->getId() == guild2->getId())
                    {
#ifdef TMWA_SUPPORT
                        if (guild2->getServerGuild() || (guildManager
                            && guildManager->havePower()))
#endif
                        {
                            mBrowserBox->addRow("/kickguild 'NAME'",
                                // TRANSLATORS: popup menu item
                                // TRANSLATORS: kick player from guild
                                _("Kick from guild"));
                        }
                        if (guild2->getServerGuild())
                        {
                            mBrowserBox->addRow(strprintf("@@guild-pos|%s >@@",
                                // TRANSLATORS: popup menu item
                                // TRANSLATORS: change player position in guild
                                 _("Change pos in guild")));
                        }
                    }
                }
                else
                {
#ifdef TMWA_SUPPORT
                    if (guild2->getServerGuild() || (guildManager
                        && guildManager->havePower()))
#endif
                    {
                        mBrowserBox->addRow("/guild 'NAME'",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: invite player to guild
                            _("Invite to guild"));
                    }
                }
            }
        }
        else
        {
            mBeingId = BeingId_zero;
            mName = name;
            mExtName = name;
            mType = ActorType::Player;
            mSubType = BeingTypeId_zero;
            addPlayerRelation(name);
            mBrowserBox->addRow("##3---");
            addMailCommands();
            addFollow();

            if (localPlayer->isInParty())
            {
                const Party *const party = localPlayer->getParty();
                if (party)
                {
                    const PartyMember *const m = party->getMember(mName);
                    if (m)
                    {
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: move to player location
                        mBrowserBox->addRow("/navigateto 'NAME'", _("Move"));
                    }
                }
            }
            addPlayerMisc();
            addBuySellDefault();
            if (serverFeatures->havePartyNickInvite())
                addParty(wTab->getNick());
            mBrowserBox->addRow("##3---");
        }
    }

    addWindowMenu(chatWindow);
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChangePos(const int x, const int y)
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Change guild position"));

    if (!localPlayer)
        return;

    mX = x;
    mY = y;
    const Guild *const guild = localPlayer->getGuild();
    if (guild)
    {
        const PositionsMap &map = guild->getPositions();
        FOR_EACH (PositionsMap::const_iterator, itr, map)
        {
            mBrowserBox->addRow(strprintf("@@guild-pos-%u|%s@@",
                itr->first, itr->second.c_str()));
        }
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close menu
        mBrowserBox->addRow("cancel", _("Cancel"));

        showPopup(x, y);
    }
    else
    {
        mBeingId = BeingId_zero;
        mFloorItemId = BeingId_zero;
        mItemIndex = -1;
        mItemId = 0;
        for (int f = 0; f < maxCards; f ++)
            mItemCards[f] = 0;
        mMapItem = nullptr;
        mName.clear();
        mExtName.clear();
        mType = ActorType::Unknown;
        mSubType = BeingTypeId_zero;
        mX = 0;
        mY = 0;
        setVisible(Visible_false);
    }
}

void PopupMenu::showWindowPopup(Window *const window)
{
    if (!window)
        return;

    setMousePos();
    mWindow = window;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("window"));

    addWindowMenu(window);

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::addWindowMenu(const Window *const window)
{
    if (!window)
        return;

    if (window->getAlowClose())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close window
        mBrowserBox->addRow("window close", _("Close"));
    }

    if (window->isStickyButtonLock())
    {
        if (window->isSticky())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: unlock window
            mBrowserBox->addRow("window unlock", _("Unlock"));
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: lock window
            mBrowserBox->addRow("window lock", _("Lock"));
        }
    }
}

void PopupMenu::showEmoteType()
{
    setMousePos();

    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Show emotes for:"));

    // TRANSLATORS: popup menu item
    // TRANSLATORS: show emotes for player
    mBrowserBox->addRow("/setemotetype player", _("Player"));

    // TRANSLATORS: popup menu item
    // TRANSLATORS: show emotes for pet
    mBrowserBox->addRow("/setemotetype pet", _("Pet"));

    if (serverFeatures->haveServerPets())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: show emotes for homuncules
        mBrowserBox->addRow("/setemotetype homun", _("Homunculus"));

        // TRANSLATORS: popup menu item
        // TRANSLATORS: show emotes for mercenary
        mBrowserBox->addRow("/setemotetype merc", _("Mercenary"));
    }
    mBrowserBox->addRow("##3---");

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::handleLink(const std::string &link,
                           MouseEvent *event A_UNUSED)
{
    Being *being = nullptr;
    if (actorManager)
        being = actorManager->findBeing(mBeingId);

    mAllowCleanMenu = true;

    if (link == "chat close" && mTab)
    {
        inputManager.executeChatCommand(InputAction::CLOSE_CHAT_TAB,
            std::string(), mTab);
    }
    else if (link == "remove map" && mMapItem)
    {
        if (viewport)
        {
            const Map *const map = viewport->getMap();
            if (map)
            {
                SpecialLayer *const specialLayer = map->getSpecialLayer();
                if (specialLayer)
                {
                    const bool isHome = (mMapItem->getType()
                        == CAST_S32(MapItemType::HOME));
                    const int x = static_cast<const int>(mMapItem->getX());
                    const int y = static_cast<const int>(mMapItem->getY());
                    specialLayer->setTile(x, y,
                        CAST_S32(MapItemType::EMPTY));
                    if (socialWindow)
                        socialWindow->removePortal(x, y);
                    if (isHome && localPlayer)
                    {
                        localPlayer->removeHome();
                        localPlayer->saveHomes();
                    }
                }
            }
        }
    }
    else if (link == "rename map" && mMapItem)
    {
        mRenameListener.setMapItem(mMapItem);
        CREATEWIDGETV(mDialog, TextDialog,
            // TRANSLATORS: dialog caption
            // TRANSLATORS: number of chars in string should be near original
            _("Rename map sign          "),
            // TRANSLATORS: label
            // TRANSLATORS: number of chars in string should be near original
            _("Name:                    "));
        mRenameListener.setDialog(mDialog);
        mDialog->setText(mMapItem->getComment());
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&mRenameListener);
    }
    else if (link == "edit spell" && mSpell)
    {
        CREATEWIDGET(TextCommandEditor, mSpell);
    }
    else if (link == "addcomment" && !mName.empty())
    {
        TextDialog *const dialog = CREATEWIDGETR(TextDialog,
            // TRANSLATORS: dialog caption
            // TRANSLATORS: number of chars in string should be near original
            _("Player comment            "),
            // TRANSLATORS: label
            // TRANSLATORS: number of chars in string should be near original
            _("Comment:                      "));
        mPlayerListener.setDialog(dialog);
        mPlayerListener.setNick(mName);
        mPlayerListener.setType(mType);

        if (being)
        {
            being->updateComment();
            dialog->setText(being->getComment());
        }
        else
        {
            dialog->setText(Being::loadComment(mName,
                static_cast<ActorTypeT>(mType)));
        }
        dialog->setActionEventId("ok");
        dialog->addActionListener(&mPlayerListener);
    }
    else if (link == "enable highlight" && mTab)
    {
        inputManager.executeChatCommand(InputAction::ENABLE_HIGHLIGHT,
            std::string(), mTab);
    }
    else if (link == "disable highlight" && mTab)
    {
        inputManager.executeChatCommand(InputAction::DISABLE_HIGHLIGHT,
            std::string(), mTab);
    }
    else if (link == "dont remove name" && mTab)
    {
        inputManager.executeChatCommand(InputAction::DONT_REMOVE_NAME,
            std::string(), mTab);
    }
    else if (link == "remove name" && mTab)
    {
        inputManager.executeChatCommand(InputAction::REMOVE_NAME,
            std::string(), mTab);
    }
    else if (link == "disable away" && mTab)
    {
        inputManager.executeChatCommand(InputAction::DISABLE_AWAY,
            std::string(), mTab);
    }
    else if (link == "enable away" && mTab)
    {
        inputManager.executeChatCommand(InputAction::ENABLE_AWAY,
            std::string(), mTab);
    }
    else if (link == "attack moveup")
    {
        if (actorManager)
        {
            const int idx = actorManager->getAttackMobIndex(mName);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorManager->getAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = it;
                while (it != mobs.end())
                {
                    if (*it == mName)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorManager->setAttackMobs(mobs);
                        actorManager->rebuildAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "priority moveup")
    {
        if (actorManager)
        {
            const int idx = actorManager->
                getPriorityAttackMobIndex(mName);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorManager->getPriorityAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = it;
                while (it != mobs.end())
                {
                    if (*it == mName)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorManager->setPriorityAttackMobs(mobs);
                        actorManager->rebuildPriorityAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "attack movedown")
    {
        if (actorManager)
        {
            const int idx = actorManager->getAttackMobIndex(mName);
            const int size = actorManager->getAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorManager->getAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = it;
                while (it != mobs.end())
                {
                    if (*it == mName)
                    {
                        ++ it2;
                        if (it2 == mobs.end())
                            break;

                        mobs.splice(it, mobs, it2);
                        actorManager->setAttackMobs(mobs);
                        actorManager->rebuildAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "priority movedown")
    {
        if (localPlayer && actorManager)
        {
            const int idx = actorManager
                ->getPriorityAttackMobIndex(mName);
            const int size = actorManager->getPriorityAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorManager->getPriorityAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = it;
                while (it != mobs.end())
                {
                    if (*it == mName)
                    {
                        ++ it2;
                        if (it2 == mobs.end())
                            break;

                        mobs.splice(it, mobs, it2);
                        actorManager->setPriorityAttackMobs(mobs);
                        actorManager->rebuildPriorityAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "items" && being)
    {
        if (being == localPlayer)
        {
            if (equipmentWindow && !equipmentWindow->isWindowVisible())
                equipmentWindow->setVisible(Visible_true);
        }
        else
        {
            if (beingEquipmentWindow)
            {
                beingEquipmentWindow->setBeing(being);
                beingEquipmentWindow->setVisible(Visible_true);
            }
        }
    }
    else if (link == "undress item" && being && mItemId)
    {
        being->undressItemById(mItemId);
    }
    else if (link == "guild-pos" && !mName.empty())
    {
        showChangePos(getX(), getY());
        return;
    }
    else if (link == "clear outfit")
    {
        if (outfitWindow)
            outfitWindow->clearCurrentOutfit();
    }
    else if (link == "clipboard copy")
    {
        if (mTextField)
            mTextField->handleCopy();
    }
    else if (link == "clipboard paste")
    {
        if (mTextField)
            mTextField->handlePaste();
    }
    else if (link == "open link" && !mName.empty())
    {
        openBrowser(mName);
    }
    else if (link == "clipboard link" && !mName.empty())
    {
        sendBuffer(mName);
    }
    else if (link == "goto" && !mName.empty())
    {
        adminHandler->gotoName(mName);
    }
    else if (link == "recall" && !mName.empty())
    {
        adminHandler->recallName(mName);
    }
    else if (link == "ipcheck" && !mName.empty())
    {
        adminHandler->ipcheckName(mName);
    }
    else if (link == "gm" && !mName.empty())
    {
        showGMPopup(mName);
        return;
    }
    else if (link == "mute" && !mName.empty())
    {
        showMuteCommands();
        return;
    }
    else if (link == "show" && !mName.empty())
    {
        showPlayerMenu();
        return;
    }
    else if (link == "window close" && mWindow)
    {
        if (Widget::widgetExists(mWindow))
            mWindow->close();
    }
    else if (link == "window unlock" && mWindow)
    {
        if (Widget::widgetExists(mWindow))
            mWindow->setSticky(false);
    }
    else if (link == "window lock" && mWindow)
    {
        if (Widget::widgetExists(mWindow))
            mWindow->setSticky(true);
    }
    else if (link == "join chat" && being)
    {
        const ChatObject *const chat = being->getChat();
        if (chat)
            chatHandler->joinChat(chat, "");
    }
    else if (link == "fire mercenary")
    {
        mercenaryHandler->fire();
    }
    else if (link == "mercenary to master")
    {
        mercenaryHandler->moveToMaster();
    }
    else if (link == "homunculus to master")
    {
        homunculusHandler->moveToMaster();
    }
    else if (link == "homunculus feed")
    {
        homunculusHandler->feed();
    }
    else if (link == "homunculus delete")
    {
        homunculusHandler->fire();
    }
    else if (link == "craftmenu")
    {
        showCraftPopup();
        return;
    }
    else if (link == "pet feed")
    {
        petHandler->feed();
    }
    else if (link == "pet drop loot")
    {
        petHandler->dropLoot();
    }
    else if (link == "pet to egg")
    {
        petHandler->returnToEgg();
    }
    else if (link == "pet unequip")
    {
        petHandler->unequip();
    }
    else if (!link.compare(0, 10, "guild-pos-"))
    {
        if (localPlayer)
        {
            const int num = atoi(link.substr(10).c_str());
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
                guildHandler->changeMemberPostion(
                    guild->getMember(mName), num);
            }
        }
    }
    else if (!link.compare(0, 7, "player_"))
    {
        if (actorManager)
        {
            mBeingId = fromInt(atoi(link.substr(7).c_str()), BeingId);
            being = actorManager->findBeing(mBeingId);
            if (being)
            {
                showPopup(getX(), getY(), being);
                return;
            }
        }
    }
    else if (!link.compare(0, 10, "flooritem_"))
    {
        if (actorManager)
        {
            const BeingId id = fromInt(atoi(
                link.substr(10).c_str()), BeingId);
            if (id != BeingId_zero)
            {
                const FloorItem *const item = actorManager->findItem(id);
                if (item)
                {
                    mFloorItemId = item->getId();
                    mItemId = item->getItemId();
                    for (int f = 0; f < maxCards; f ++)
                        mItemCards[f] = item->getCard(f);
                    showPopup(getX(), getY(), item);
                    return;
                }
            }
        }
    }
    else if (!link.compare(0, 12, "hide button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), Visible_false);
    }
    else if (!link.compare(0, 12, "show button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), Visible_true);
    }
    else if (!link.compare(0, 9, "hide bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), Visible_false);
    }
    else if (!link.compare(0, 9, "show bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), Visible_true);
    }
    else if (!link.compare(0, 12, "show window_"))
    {
        const int id = atoi(link.substr(12).c_str());
        if (id >= 0)
            inputManager.executeAction(static_cast<InputActionT>(id));
    }
    else if (!link.compare(0, 6, "mute_+"))
    {
        if (being)
        {
            const int time = atoi(link.substr(6).c_str());
            adminHandler->mute(being, 1, time);
        }
    }
    else if (!link.compare(0, 6, "mute_-"))
    {
        if (being)
        {
            const int time = atoi(link.substr(6).c_str());
            adminHandler->mute(being, 0, time);
        }
    }
    else if (!link.empty() && link[0] == '/')
    {
        std::string cmd = link.substr(1);
        replaceAll(cmd, "'NAME'", mName);
        replaceAll(cmd, "'ENAME'", escapeString(mName));
        replaceAll(cmd, "'EXTNAME'", mExtName);
        replaceAll(cmd, "'EEXTNAME'", escapeString(mExtName));
        replaceAll(cmd, "'X'", toString(mX));
        replaceAll(cmd, "'Y'", toString(mY));
        replaceAll(cmd, "'BEINGID'", toString(toInt(mBeingId, int)));
        replaceAll(cmd, "'FLOORID'", toString(toInt(mFloorItemId, int)));
        replaceAll(cmd, "'ITEMID'", toString(mItemId));
        replaceAll(cmd, "'ITEMCOLOR'", toString(toInt(mItemColor, int)));
        replaceAll(cmd, "'BEINGTYPEID'", toString(CAST_S32(mType)));
        replaceAll(cmd, "'BEINGSUBTYPEID'", toString(CAST_S32(mSubType)));
        replaceAll(cmd, "'PLAYER'", localPlayer->getName());
        replaceAll(cmd, "'EPLAYER'", escapeString(localPlayer->getName()));
        replaceAll(cmd, "'PLAYERX'", toString(localPlayer->getTileX()));
        replaceAll(cmd, "'PLAYERY'", toString(localPlayer->getTileY()));
        if (mItemIndex >= 0)
            replaceAll(cmd, "'INVINDEX'", toString(mItemIndex));
        else
            replaceAll(cmd, "'INVINDEX'", "0");
        if (mMapItem)
        {
            replaceAll(cmd, "'MAPX'", toString(mMapItem->getX()));
            replaceAll(cmd, "'MAPY'", toString(mMapItem->getY()));
        }
        else
        {
            replaceAll(cmd, "'MAPX'", toString(mX));
            replaceAll(cmd, "'MAPY'", toString(mY));
        }

        std::string cards;
        for (int f = 0; f < maxCards; f ++)
        {
            const int id = mItemCards[f];
            if (id)
            {
                cards.append(",");
                cards.append(toString(id));
            }
        }
        replaceAll(cmd, "'CARDS'", cards);
        replaceAll(cmd, "'ECARDS'", escapeString(cards));
        if (actorManager)
        {
            if (!being)
            {
                being = actorManager->findBeingByName(mName,
                    ActorType::Player);
            }
        }
        if (being)
            replaceAll(cmd, "'PARTY'", being->getPartyName());
        else
            replaceAll(cmd, "'PARTY'", "");

        const size_t pos = cmd.find(' ');
        const std::string type(cmd, 0, pos);
        std::string args(cmd, pos == std::string::npos ? cmd.size() : pos + 1);
        args = trim(args);
        inputManager.executeChatCommand(type, args, mTab);
    }
    // Unknown actions
    else if (link != "cancel")
    {
        reportAlways("PopupMenu: Warning, unknown action '%s'", link.c_str());
    }

    if (!mAllowCleanMenu)
        return;

    setVisible(Visible_false);

    mBeingId = BeingId_zero;
    mFloorItemId = BeingId_zero;
    mItemId = 0;
    mItemIndex = -1;
    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = 0;
    mItemColor = ItemColor_one;
    mMapItem = nullptr;
    mTab = nullptr;
    mSpell = nullptr;
    mWindow = nullptr;
    mDialog = nullptr;
    mButton = nullptr;
    mName.clear();
    mExtName.clear();
    mTextField = nullptr;
    mType = ActorType::Unknown;
    mSubType = BeingTypeId_zero;
    mX = 0;
    mY = 0;
}

void PopupMenu::showPopup(Window *const parent,
                          const int x, const int y,
                          const Item *const item,
                          const InventoryTypeT type)
{
    if (!item)
        return;

    mItemId = item->getId();
    mItemIndex = item->getInvIndex();
    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = item->getCard(f);
    mItemColor = item->getColor();
    mWindow = parent;
    mX = x;
    mY = y;
    mName.clear();
    mExtName.clear();
    mBrowserBox->clearRows();

    const int cnt = item->getQuantity();
    const bool isProtected = PlayerInfo::isItemProtected(mItemId);

    switch (type)
    {
        case InventoryType::Inventory:
            if (tradeWindow && tradeWindow->isWindowVisible() && !isProtected)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add item to trade
                mBrowserBox->addRow("/addtrade 'INVINDEX'", _("Add to trade"));
                if (cnt > 1)
                {
                    if (cnt > 10)
                    {
                        mBrowserBox->addRow("/addtrade 'INVINDEX' 10",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: add 10 item amount to trade
                            _("Add to trade 10"));
                    }
                    mBrowserBox->addRow("/addtrade 'INVINDEX' /",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add half item amount to trade
                        _("Add to trade half"));
                    mBrowserBox->addRow("/addtrade 'INVINDEX' -1",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add all amount except one item to trade
                        _("Add to trade all-1"));
                    mBrowserBox->addRow("/addtrade 'INVINDEX' all",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add all amount item to trade
                        _("Add to trade all"));
                }
                mBrowserBox->addRow("##3---");
            }
            if (InventoryWindow::isStorageActive())
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add item to storage
                mBrowserBox->addRow("/invtostorage 'INVINDEX'", _("Store"));
                if (cnt > 1)
                {
                    if (cnt > 10)
                    {
                        mBrowserBox->addRow("/invtostorage 'INVINDEX' 10",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: add 10 item amount to storage
                            _("Store 10"));
                    }
                    mBrowserBox->addRow("/invtostorage 'INVINDEX' /",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add half item amount to storage
                        _("Store half"));
                    mBrowserBox->addRow("/invtostorage 'INVINDEX' -1",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add all except one item to storage
                        _("Store all-1"));
                    mBrowserBox->addRow("/invtostorage 'INVINDEX' all",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add all item amount to storage
                        _("Store all"));
                }
                mBrowserBox->addRow("##3---");
            }
            if (npcHandler)
            {
                NpcDialog *const dialog = npcHandler->getCurrentNpcDialog();
                if (dialog &&
                    dialog->getInputState() == NpcDialog::NPC_INPUT_ITEM_CRAFT)
                {
                    mBrowserBox->addRow("craftmenu",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: sub menu for craft
                        _("Move to craft..."));
                }
            }
            addUseDrop(item, isProtected);
            break;

        case InventoryType::Storage:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: get item from storage
            mBrowserBox->addRow("/storagetoinv 'INVINDEX'", _("Retrieve"));
            if (cnt > 1)
            {
                if (cnt > 10)
                {
                    mBrowserBox->addRow("/storagetoinv 'INVINDEX' 10",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: get 10 item amount from storage
                        _("Retrieve 10"));
                }
                mBrowserBox->addRow("/storagetoinv 'INVINDEX' /",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: get half item amount from storage
                    _("Retrieve half"));
                mBrowserBox->addRow("/storagetoinv 'INVINDEX' -1",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: get all except one item amount from storage
                    _("Retrieve all-1"));
                mBrowserBox->addRow("/storagetoinv 'INVINDEX' all",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: get all item amount from storage
                    _("Retrieve all"));
            }
            break;

        case InventoryType::Trade:
        case InventoryType::Npc:
        case InventoryType::Cart:
        case InventoryType::Vending:
        case InventoryType::Mail:
        case InventoryType::Craft:
        case InventoryType::TypeEnd:
        default:
            break;
    }


    addProtection();
    if (config.getBoolValue("enablePickupFilter"))
    {
        mName = item->getName();
        mExtName = mName;
        mBrowserBox->addRow("##3---");
        addPickupFilter(mName);
    }
    addGmCommands();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add item name to chat
    mBrowserBox->addRow("/addchat 'ITEMID''CARDS'", _("Add to chat"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showItemPopup(const int x, const int y,
                              const int itemId,
                              const ItemColor color)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    Item *const item = inv->findItem(itemId, color);
    if (item)
    {
        showItemPopup(x, y, item);
    }
    else
    {
        mItemId = itemId;
        mItemIndex = -1;
        mItemColor = color;
        for (int f = 0; f < maxCards; f ++)
            mItemCards[f] = 0;
        mX = x;
        mY = y;
        mBrowserBox->clearRows();

        if (!PlayerInfo::isItemProtected(mItemId))
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: use item
            mBrowserBox->addRow("/use 'ITEMID'", _("Use"));
        }
        addProtection();
        addGmCommands();
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close menu
        mBrowserBox->addRow("cancel", _("Cancel"));

        showPopup(x, y);
    }
}

void PopupMenu::showItemPopup(const int x, const int y,
                              const Item *const item)
{
    mX = x;
    mY = y;
    if (item)
    {
        mItemId = item->getId();
        mItemColor = item->getColor();
        mItemIndex = item->getInvIndex();
        for (int f = 0; f < maxCards; f ++)
            mItemCards[f] = item->getCard(f);
    }
    else
    {
        mItemId = 0;
        mItemColor = ItemColor_one;
        mItemIndex = -1;
        for (int f = 0; f < maxCards; f ++)
            mItemCards[f] = 0;
    }
    mName.clear();
    mExtName.clear();
    mBrowserBox->clearRows();

    if (item)
    {
        const bool isProtected = PlayerInfo::isItemProtected(mItemId);
        addUseDrop(item, isProtected);
        if (InventoryWindow::isStorageActive())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to storage
            mBrowserBox->addRow("/invtostorage 'INVINDEX'", _("Store"));
        }
        // TRANSLATORS: popup menu item
        // TRANSLATORS: add item name to chat
        mBrowserBox->addRow("/addchat 'ITEMID''CARDS'", _("Add to chat"));

        if (config.getBoolValue("enablePickupFilter"))
        {
            mName = item->getName();
            mExtName = mName;
            mBrowserBox->addRow("##3---");
            addPickupFilter(mName);
        }
    }
    addProtection();
    addGmCommands();
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showDropPopup(const int x,
                              const int y,
                              const Item *const item)
{
    mX = x;
    mY = y;
    mName.clear();
    mExtName.clear();
    mBrowserBox->clearRows();

    if (item)
    {
        mItemId = item->getId();
        mItemColor = item->getColor();
        mItemIndex = item->getInvIndex();
        for (int f = 0; f < maxCards; f ++)
            mItemCards[f] = item->getCard(f);
        const bool isProtected = PlayerInfo::isItemProtected(mItemId);
        addUseDrop(item, isProtected);
        if (InventoryWindow::isStorageActive())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to storage
            mBrowserBox->addRow("/invtostorage 'INVINDEX'", _("Store"));
        }
        addProtection();
        // TRANSLATORS: popup menu item
        // TRANSLATORS: add item name to chat
        mBrowserBox->addRow("/addchat 'ITEMID''CARDS'", _("Add to chat"));
        if (config.getBoolValue("enablePickupFilter"))
        {
            mName = item->getName();
            mExtName = mName;
            mBrowserBox->addRow("##3---");
            addPickupFilter(mName);
        }
    }
    addGmCommands();
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("/cleardrops", _("Clear drop window"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y, Button *const button)
{
    if (!button || !windowMenu)
        return;

    mButton = button;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    std::vector<Button *> names = windowMenu->getButtons();
    for (std::vector<Button *>::const_iterator it = names.begin(),
         it_end = names.end(); it != it_end; ++ it)
    {
        const Button *const btn = *it;
        if (!btn || btn->getActionEventId() == "SET")
            continue;

        if (btn->mVisible == Visible_true)
        {
            mBrowserBox->addRow(strprintf("@@hide button_%s|%s %s (%s)@@",
                // TRANSLATORS: popup menu item
                btn->getActionEventId().c_str(), _("Hide"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show button_%s|%s %s (%s)@@",
                // TRANSLATORS: popup menu item
                btn->getActionEventId().c_str(), _("Show"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y, const ProgressBar *const b)
{
    if (!b || !miniStatusWindow)
        return;

    mName = b->text();
    mExtName = mName;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    std::vector <ProgressBar*> bars = miniStatusWindow->getBars();
    ProgressBar *onlyBar = nullptr;
    int cnt = 0;

    // search for alone visible bar
    for (std::vector <ProgressBar*>::const_iterator it = bars.begin(),
         it_end = bars.end(); it != it_end; ++it)
    {
        ProgressBar *const bar = *it;
        if (!bar)
            continue;

        if (bar->mVisible == Visible_true)
        {
            cnt ++;
            onlyBar = bar;
        }
    }
    if (cnt > 1)
        onlyBar = nullptr;

    for (std::vector <ProgressBar*>::const_iterator it = bars.begin(),
         it_end = bars.end(); it != it_end; ++it)
    {
        ProgressBar *const bar = *it;
        if (!bar || bar == onlyBar)
            continue;

        if (bar->mVisible == Visible_true)
        {
            mBrowserBox->addRow(strprintf("@@hide bar_%s|%s %s@@",
                // TRANSLATORS: popup menu item
                bar->getActionEventId().c_str(), _("Hide"),
                bar->getId().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show bar_%s|%s %s@@",
                // TRANSLATORS: popup menu item
                bar->getActionEventId().c_str(), _("Show"),
                bar->getId().c_str()));
        }
    }

    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("/yellowbar", _("Open yellow bar settings"));
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("/resetmodifiers", _("Reset yellow bar"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy status to chat
    mBrowserBox->addRow("/bartochat 'NAME'", _("Copy to chat"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showAttackMonsterPopup(const int x, const int y,
                                       const std::string &name,
                                       const int type)
{
    if (!localPlayer || !actorManager)
        return;

    mName = name;
    mExtName = name;
    mType = ActorType::Monster;
    mSubType = BeingTypeId_zero;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("(default)"));
    }
    else
    {
        mBrowserBox->addRow(name);
    }
    switch (type)
    {
        case MapItemType::ATTACK:
        {
            const int idx = actorManager->getAttackMobIndex(name);
            const int size = actorManager->getAttackMobsSize();
            if (idx > 0)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target up
                mBrowserBox->addRow("attack moveup", _("Move up"));
            }
            if (idx + 1 < size)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target down
                mBrowserBox->addRow("attack movedown", _("Move down"));
            }
            break;
        }
        case MapItemType::PRIORITY:
        {
            const int idx = actorManager->
                getPriorityAttackMobIndex(name);
            const int size = actorManager->getPriorityAttackMobsSize();
            if (idx > 0)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target up
                mBrowserBox->addRow("priority moveup", _("Move up"));
            }
            if (idx + 1 < size)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target down
                mBrowserBox->addRow("priority movedown", _("Move down"));
            }
            break;
        }
        case MapItemType::IGNORE_:
            break;
        default:
            break;
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove attack target
    mBrowserBox->addRow("/removeattack 'NAME'", _("Remove"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPickupItemPopup(const int x, const int y,
                                    const std::string &name)
{
    if (!localPlayer || !actorManager)
        return;

    mName = name;
    mExtName = name;
    mType = ActorType::FloorItem;
    mSubType = BeingTypeId_zero;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("(default)"));
    }
    else
    {
        mBrowserBox->addRow(name);
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove item from pickup filter
    mBrowserBox->addRow("/removepickup 'NAME'", _("Remove"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showUndressPopup(const int x, const int y,
                                 const Being *const being,
                                 const Item *const item)
{
    if (!being || !item)
        return;

    mBeingId = being->getId();
    mItemId = item->getId();
    mItemColor = item->getColor();
    mItemIndex = item->getInvIndex();
    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = item->getCard(f);
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: undress item from player
    mBrowserBox->addRow("undress item", _("Undress"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add item name to chat
    mBrowserBox->addRow("/addchat 'ITEMID''CARDS'", _("Add to chat"));
    addGmCommands();
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showTextFieldPopup(TextField *const input)
{
    setMousePos();
    mTextField = input;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy text to clipboard
    mBrowserBox->addRow("clipboard copy", _("Copy"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: paste text from clipboard
    mBrowserBox->addRow("clipboard paste", _("Paste"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showLinkPopup(const std::string &link)
{
    setMousePos();
    mName = link;
    mExtName = link;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: open link in browser
    mBrowserBox->addRow("open link", _("Open link"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy link to clipboard
    mBrowserBox->addRow("clipboard link", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showWindowsPopup()
{
    setMousePos();
    mBrowserBox->clearRows();
    const std::vector<ButtonText*> &names = windowMenu->getButtonTexts();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Show window"));

    FOR_EACH (std::vector<ButtonText*>::const_iterator, it, names)
    {
        const ButtonText *const btn = *it;
        if (!btn)
            continue;

        mBrowserBox->addRow(strprintf("show window_%d",
            CAST_S32(btn->key)),
            btn->text.c_str());
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showNpcDialogPopup(const BeingId npcId,
                                   const int x, const int y)
{
    mBeingId = npcId;
    mX = x;
    mY = y;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy npc text to clipboard
    mBrowserBox->addRow("/npcclipboard 'X' 'Y'", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showSkillPopup(const SkillInfo *const info)
{
    if (!info)
        return;
    setMousePos();

    // using mItemId as skill id
    mItemId = info->id;
    // using mItemIndex as skill level
    mItemIndex = info->level;
    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Skill"));
    mBrowserBox->addRow("/showskilllevels 'ITEMID'",
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set skill level
        _("Skill level..."));
    mBrowserBox->addRow("/showskilltypes 'ITEMID'",
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set skill cast type
        _("Skill cast type..."));
    mBrowserBox->addRow("/showskilloffsetx 'ITEMID'",
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set skill cast offset by x
        _("Skill offset by x..."));
    mBrowserBox->addRow("/showskilloffsety 'ITEMID'",
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set skill cast offset by y
        _("Skill offset by y..."));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showSkillOffsetPopup(const SkillInfo *const info,
                                     const bool isOffsetX)
{
    if (!info)
        return;
    setMousePos2();

    // using mItemId as skill id
    mItemId = info->id;
    // using mItemIndex as skill level
    mItemIndex = info->level;
    mBrowserBox->clearRows();
    char letter = ' ';

    if (isOffsetX)
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("Skill cast offset by x"));
        letter = 'x';
    }
    else
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("Skill cast offset by y"));
        letter = 'y';
    }
    for (int f = -9; f <= 9; f ++)
    {
        const std::string command = strprintf(
            "/setskilloffset%c 'ITEMID' %d", letter, f);
        if (f == 0)
        {
            mBrowserBox->addRow(command,
                strprintf("%d", f).c_str());
        }
        else
        {
            mBrowserBox->addRow(command,
                strprintf("%+d", f).c_str());
        }
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showSkillLevelPopup(const SkillInfo *const info)
{
    if (!info)
        return;
    setMousePos2();

    // using mItemId as skill id
    mItemId = info->id;
    // using mItemIndex as skill level
    mItemIndex = info->level;

    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = 0;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: skill level header
    mBrowserBox->addRow(_("Select skill level"));
    for (int f = 1; f <= mItemIndex; f ++)
    {
        mBrowserBox->addRow(strprintf("/selectskilllevel %d %d", mItemId, f),
            // TRANSLATORS: popup menu item
            // TRANSLATORS: skill level
            strprintf(_("Level: %d"), f).c_str());
    }
    mBrowserBox->addRow(strprintf("/selectskilllevel %d 0", mItemId),
        // TRANSLATORS: popup menu item
        // TRANSLATORS: skill level
        _("Max level"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showSkillTypePopup(const SkillInfo *const info)
{
    if (!info)
        return;
    setMousePos2();

    // using mItemId as skill id
    mItemId = info->id;
    // using mItemIndex as skill level
    mItemIndex = info->level;

    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = 0;
    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: skill cast type header
    mBrowserBox->addRow(_("Select skill cast type"));
    mBrowserBox->addRow(strprintf("/selectskilltype %d 0", mItemId),
        // TRANSLATORS: popup menu item
        // TRANSLATORS: skill cast type
        _("Default"));
    mBrowserBox->addRow(strprintf("/selectskilltype %d 1", mItemId),
        // TRANSLATORS: popup menu item
        // TRANSLATORS: skill cast type
        _("Target"));
    mBrowserBox->addRow(strprintf("/selectskilltype %d 2", mItemId),
        // TRANSLATORS: popup menu item
        // TRANSLATORS: skill cast type
        _("Mouse position"));
    mBrowserBox->addRow(strprintf("/selectskilltype %d 3", mItemId),
        // TRANSLATORS: popup menu item
        // TRANSLATORS: skill cast type
        _("Self"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::showPopup(int x, int y)
{
    const int pad2 = 2 * mPadding;
    const int bPad2 = 2 * mBrowserBox->getPadding();
    mBrowserBox->setPosition(mPadding, mPadding);
    mScrollArea->setPosition(mPadding, mPadding);
    // add padding to initial size before draw browserbox
    int height = mBrowserBox->getHeight();
    if (height + pad2 >= mainGraphics->getHeight())
    {
        height = mainGraphics->getHeight() - bPad2 - pad2;
        mBrowserBox->setWidth(mBrowserBox->getWidth() + bPad2 + 5);
        mScrollArea->setWidth(mBrowserBox->getWidth() + pad2 + 10);
        setContentSize(mBrowserBox->getWidth() + pad2 + 20,
            height + pad2);
    }
    else
    {
        mBrowserBox->setWidth(mBrowserBox->getWidth() + bPad2);
        mScrollArea->setWidth(mBrowserBox->getWidth() + pad2);
        setContentSize(mBrowserBox->getWidth() + pad2,
            height + pad2);
    }
    if (mainGraphics->mWidth < (x + getWidth() + 5))
        x = mainGraphics->mWidth - getWidth();
    if (mainGraphics->mHeight < (y + getHeight() + 5))
        y = mainGraphics->mHeight - getHeight();
    mScrollArea->setHeight(height);
    setPosition(x, y);
    setVisible(Visible_true);
    requestMoveToTop();
    mAllowCleanMenu = false;
}

void PopupMenu::addNormalRelations()
{
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player to disregarded list
    mBrowserBox->addRow("/disregard 'NAME'", _("Disregard"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player to ignore list
    mBrowserBox->addRow("/ignore 'NAME'", _("Ignore"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player to black list
    mBrowserBox->addRow("/blacklist 'NAME'", _("Black list"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player to enemy list
    mBrowserBox->addRow("/enemy 'NAME'", _("Set as enemy"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player to erased list
    mBrowserBox->addRow("/erase 'NAME'", _("Erase"));
}

void PopupMenu::addPlayerRelation(const std::string &name)
{
    switch (player_relations.getRelation(name))
    {
        case Relation::NEUTRAL:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to friends list
            mBrowserBox->addRow("/friend 'NAME'", _("Be friend"));
            addNormalRelations();
            break;

        case Relation::FRIEND:
            addNormalRelations();
            break;

        case Relation::BLACKLISTED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("/unignore 'NAME'", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("/disregard 'NAME'", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("/ignore 'NAME'", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to enemy list
            mBrowserBox->addRow("/enemy 'NAME'", _("Set as enemy"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("/erase 'NAME'", _("Erase"));
            break;

        case Relation::DISREGARDED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("/unignore 'NAME'", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to completle ignore list
            mBrowserBox->addRow("/ignore 'NAME'", _("Completely ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("/erase 'NAME'", _("Erase"));
            break;

        case Relation::IGNORED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("/unignore 'NAME'", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("/erase 'NAME'", _("Erase"));
            break;

        case Relation::ENEMY2:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("/unignore 'NAME'", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("/disregard 'NAME'", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("/ignore 'NAME'", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to black list
            mBrowserBox->addRow("/blacklist 'NAME'", _("Black list"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("/erase 'NAME'", _("Erase"));
            break;

        case Relation::ERASED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("/unignore 'NAME'", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("/disregard 'NAME'", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("/ignore 'NAME'", _("Completely ignore"));
            break;

        default:
            break;
    }
}

void PopupMenu::addFollow()
{
    if (features.getBoolValue("allowFollow"))
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("/follow 'NAME'", _("Follow"));
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: imitate player
    mBrowserBox->addRow("/imitation 'NAME'", _("Imitate"));
}

void PopupMenu::addBuySell(const Being *const being)
{
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow("##3---");
        const bool haveVending = serverFeatures->haveVending();
        if (being->isSellShopEnabled())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: buy item
            mBrowserBox->addRow("/buy 'NAME'", _("Buy"));
        }
        else if (!haveVending)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: buy item
            mBrowserBox->addRow("/buy 'NAME'", _("Buy (?)"));
        }
        if (being->isBuyShopEnabled())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: sell item
            mBrowserBox->addRow("/sell 'NAME'", _("Sell"));
        }
        else if (!haveVending)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: sell item
            mBrowserBox->addRow("/sell 'NAME'", _("Sell (?)"));
        }
    }
}

void PopupMenu::addBuySellDefault()
{
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: buy item
        mBrowserBox->addRow("/buy 'NAME'", _("Buy (?)"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: sell item
        mBrowserBox->addRow("/sell 'NAME'", _("Sell (?)"));
    }
}

void PopupMenu::addPartyName(const std::string &partyName)
{
    if (localPlayer->isInParty())
    {
        if (localPlayer->getParty())
        {
            if (localPlayer->getParty()->getName() != partyName)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: invite player to party
                mBrowserBox->addRow("/party 'NAME'", _("Invite to party"));
            }
            else
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from party
                mBrowserBox->addRow("/kickparty 'NAME'", _("Kick from party"));
                showAdoptCommands();
            }
            mBrowserBox->addRow("##3---");
        }
    }
}

void PopupMenu::addParty(const std::string &nick)
{
    if (localPlayer->isInParty())
    {
        const Party *const party = localPlayer->getParty();
        if (party)
        {
            if (!party->isMember(nick))
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: invite player to party
                mBrowserBox->addRow("/party 'NAME'", _("Invite to party"));
            }
            else
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from party
                mBrowserBox->addRow("/kickparty 'NAME'", _("Kick from party"));
                showAdoptCommands();
            }
            mBrowserBox->addRow("##3---");
        }
    }
}

void PopupMenu::addChat(const Being *const being)
{
    if (!being)
        return;
    const ChatObject *const chat = being->getChat();
    if (chat)
    {
        mBrowserBox->addRow("join chat",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: invite player to party
            strprintf(_("Join chat %s"), chat->title.c_str()).c_str());
        mBrowserBox->addRow("##3---");
    }
}

void PopupMenu::addPlayerMisc()
{
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("items", _("Show Items"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: undress player
    mBrowserBox->addRow("/undress :'BEINGID'", _("Undress"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add comment to player
    mBrowserBox->addRow("addcomment", _("Add comment"));
}

void PopupMenu::addPickupFilter(const std::string &name)
{
    if (actorManager->isInPickupList(name)
        || actorManager->isInIgnorePickupList(name))
    {
        mBrowserBox->addRow("/removepickup 'NAME'",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove item from pickup list
            _("Remove from pickup list"));
    }
    else
    {
        mBrowserBox->addRow("/addpickup 'NAME'",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: popup menu item
            _("Add to pickup list"));
        mBrowserBox->addRow("/ignorepickup 'NAME'",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to pickup list
            _("Add to ignore list"));
    }
}

void PopupMenu::showPopup(const int x, const int y,
                          ListModel *const model)
{
    if (!model)
        return;

    mBrowserBox->clearRows();
    for (int f = 0, sz = model->getNumberOfElements(); f < sz; f ++)
    {
        mBrowserBox->addRow(strprintf("dropdown_%d", f),
            model->getElementAt(f).c_str());
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(x, y);
}

void PopupMenu::clear()
{
    if (mDialog)
    {
        mDialog->close();
        mDialog = nullptr;
    }
    mItemIndex = -1;
    mItemId = 0;
    for (int f = 0; f < maxCards; f ++)
        mItemCards[f] = 0;
    mMapItem = nullptr;
    mTab = nullptr;
    mSpell = nullptr;
    mWindow = nullptr;
    mButton = nullptr;
    mTextField = nullptr;
}

void PopupMenu::addProtection()
{
    if (PlayerInfo::isItemProtected(mItemId))
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: remove protection from item
        mBrowserBox->addRow("/unprotectitem 'ITEMID'", _("Unprotect item"));
    }
    else
    {
        if (mItemId < SPELL_MIN_ID)
        {
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add protection to item
            mBrowserBox->addRow("/protectitem 'ITEMID'", _("Protect item"));
        }
    }
}

void PopupMenu::addUseDrop(const Item *const item, const bool isProtected)
{
    const ItemInfo &info = item->getInfo();
    const std::string &str = (item->isEquipment() == Equipm_true
        && item->isEquipped() == Equipped_true)
        ? info.getUseButton2() : info.getUseButton();

    if (str.empty())
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("/useinv 'INVINDEX'", _("Use"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("/useinv 'INVINDEX'", str.c_str());
    }

    if (!isProtected)
    {
        mBrowserBox->addRow("##3---");
        if (item->getQuantity() > 1)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop item
            mBrowserBox->addRow("/dropinv 'INVINDEX'", _("Drop..."));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop all item amount
            mBrowserBox->addRow("/dropinvall 'INVINDEX'", _("Drop all"));
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop item
            mBrowserBox->addRow("/dropinv 'INVINDEX'", _("Drop"));
        }
    }
}

void PopupMenu::addGmCommands()
{
    if (localPlayer->isGM())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: gm commands
        mBrowserBox->addRow("gm", _("GM..."));
    }
}

void PopupMenu::showPlayerMenu()
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(strprintf(_("Show %s"),
        mName.c_str()));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player account info
    mBrowserBox->addRow("/showaccountinfo 'NAME'", _("Account info"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player level
    mBrowserBox->addRow("/showlevel 'NAME'", _("Level"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player stats
    mBrowserBox->addRow("/showstats 'NAME'", _("Stats"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player inventory list
    mBrowserBox->addRow("/showinventory 'NAME'", _("Inventory"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player storage list
    mBrowserBox->addRow("/showstorage 'NAME'", _("Storage"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show player cart list
    mBrowserBox->addRow("/showcart 'NAME'", _("Cart"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm commands
    mBrowserBox->addRow("/gmcommands 'NAME'", _("Commands"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm char commands
    mBrowserBox->addRow("/gmcharcommands 'NAME'", _("Char commands"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: back to gm menu
    mBrowserBox->addRow("gm", _("Back"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(getX(), getY());
}

void PopupMenu::showPlayerGMCommands(const std::string &name)
{
    const bool legacy = Net::getNetworkType() == ServerType::TMWATHENA;
    if (!legacy)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: find player position
        mBrowserBox->addRow("/locateplayer 'NAME'", _("Locate"));
    }
    if (legacy)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: check player ip
        mBrowserBox->addRow("ipcheck", _("Check ip"));
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: go to player position
    mBrowserBox->addRow("goto", _("Goto"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: recall player to current position
    mBrowserBox->addRow("recall", _("Recall"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: revive player
    mBrowserBox->addRow("/alive 'NAME'", _("Revive"));
    if (!legacy)
    {
        Being *const being = actorManager->findBeingByName(name,
            ActorType::Player);
        if (being)
        {
            if (!being->getPartyName().empty())
            {
                mBrowserBox->addRow("/partyrecall 'PARTY'",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: recall all party members to player location
                    _("Recall party"));
            }
            if (!being->getGuildName().empty())
            {
                mBrowserBox->addRow("/guildrecall 'PARTY'",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: recall all guild members to player location
                    _("Recall guild"));
            }
        }
        if (localPlayer && localPlayer->isInParty())
        {
            const Party *const party = localPlayer->getParty();
            if (party && party->isMember(name))
            {
                mBrowserBox->addRow("/gmpartyleader 'NAME'",
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: give party leader status
                    _("Give party leader"));
            }
        }

        // TRANSLATORS: popup menu item
        // TRANSLATORS: nuke player
        mBrowserBox->addRow("/gmnuke 'NAME'", _("Nuke"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: kill player
        mBrowserBox->addRow("/kill 'NAME'", _("Kill"));
        if (mBeingId != BeingId_zero)
        {
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: show menu
            mBrowserBox->addRow("show", _("Show..."));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute menu
            mBrowserBox->addRow("mute", _("Mute..."));
        }
    }
    if (legacy)
    {
        if (localPlayer->getName() == mName)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: hide player
            mBrowserBox->addRow("/hide", _("Hide"));
            mBrowserBox->addRow("##3---");
        }
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: heal player
        mBrowserBox->addRow("/gmheal 'NAME'", _("Heal"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable immortal mode for player
        mBrowserBox->addRow("/immortal 'NAME'", _("Immortal"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: send player to jail
        mBrowserBox->addRow("/jail 'NAME'", _("Jail"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: restore player from jail
        mBrowserBox->addRow("/unjail 'NAME'", _("Unjail"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: hide player
        mBrowserBox->addRow("/hide 'NAME'", _("Hide"));
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set player as killer
        mBrowserBox->addRow("/killer 'NAME'", _("Killer"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set player as killable
        mBrowserBox->addRow("/killable 'NAME'", _("Killable"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: set player save position
        mBrowserBox->addRow("/savepos 'NAME'", _("Set save"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp player to save position
        mBrowserBox->addRow("/loadpos 'NAME'", _("Warp to save"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp player to random position on same map
        mBrowserBox->addRow("/randomwarp 'NAME'", _("Warp to random"));
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: spawn player clone
        mBrowserBox->addRow("/spawnclone 'NAME'", _("Spawn clone"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: spawn slave player clone
        mBrowserBox->addRow("/spawnslaveclone 'NAME'", _("Spawn slave clone"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: spawn evil player clone
        mBrowserBox->addRow("/spawnevilclone 'NAME'", _("Spawn evil clone"));
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: break guild
        mBrowserBox->addRow("/gmbreakguild 'NAME'", _("Break guild"));
    }
    if (mBeingId != BeingId_zero)
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: kick player
        mBrowserBox->addRow("/kick :'BEINGID'", _("Kick"));
    }
}

void PopupMenu::showMuteCommands()
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(strprintf(_("Mute %s"),
        mName.c_str()));
    if (mBeingId != BeingId_zero &&
        serverFeatures->haveMute())
    {
        mBrowserBox->addRow("mute_+1",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Mute %d"), 1).c_str());
        mBrowserBox->addRow("mute_+5",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Mute %d"), 5).c_str());
        mBrowserBox->addRow("mute_+10",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Mute %d"), 10).c_str());
        mBrowserBox->addRow("mute_+15",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Mute %d"), 15).c_str());
        mBrowserBox->addRow("mute_+30",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Mute %d"), 30).c_str());

        mBrowserBox->addRow("mute_-1",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Unmute %d"), 1).c_str());
        mBrowserBox->addRow("mute_-5",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Unmute %d"), 5).c_str());
        mBrowserBox->addRow("mute_-10",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Unmute %d"), 10).c_str());
        mBrowserBox->addRow("mute_-15",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Unmute %d"), 15).c_str());
        mBrowserBox->addRow("mute_-30",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: mute player
            strprintf(_("Unmute %d"), 30).c_str());
        mBrowserBox->addRow("##3---");
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: back to gm menu
    mBrowserBox->addRow("gm", _("Back"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(getX(), getY());
}

void PopupMenu::showNpcGMCommands()
{
    if (mBeingId != BeingId_zero)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: kick player
        mBrowserBox->addRow("/kick :'BEINGID'", _("Kick"));
        const bool legacy = Net::getNetworkType() == ServerType::TMWATHENA;
        if (!legacy)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: hide npc
            mBrowserBox->addRow("/hidenpc 'EXTNAME'", _("Hide"));
            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow("/npcmove 'EEXTNAME' 'PLAYERX' 'PLAYERY'",
                // TRANSLATORS: popup menu item
                // TRANSLATORS: warp npc to player location
                _("Recall"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: disguise to npc
            mBrowserBox->addRow("/disguise 'BEINGSUBTYPEID'", _("Disguise"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: warp to npc
            mBrowserBox->addRow("/gotonpc 'EXTNAME'", _("Goto"));
        }
    }
}

void PopupMenu::showMonsterGMCommands()
{
    if (mBeingId != BeingId_zero)
    {
        const bool legacy = Net::getNetworkType() == ServerType::TMWATHENA;
        // TRANSLATORS: popup menu item
        // TRANSLATORS: kick monster
        mBrowserBox->addRow("/kick :'BEINGID'", _("Kick"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: spawn monster
        mBrowserBox->addRow("/spawn 'BEINGSUBTYPEID'", _("Spawn same"));
        if (!legacy)
        {
            mBrowserBox->addRow("/spawnslave 'BEINGSUBTYPEID'",
                // TRANSLATORS: popup menu item
                // TRANSLATORS: spawn slave monster
                _("Spawn slave"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: disguise to monster
            mBrowserBox->addRow("/disguise 'BEINGSUBTYPEID'", _("Disguise"));
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: show monster information
            mBrowserBox->addRow("/monsterinfo 'BEINGSUBTYPEID'", _("Info"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: show selected monster in current map
            mBrowserBox->addRow("/mobsearch 'BEINGSUBTYPEID'", _("Search"));
            mBrowserBox->addRow("/mobspawnsearch 'BEINGSUBTYPEID'",
                // TRANSLATORS: popup menu item
                // TRANSLATORS: show selected monster spawns in all maps
                _("Search spawns"));
        }
    }
}

void PopupMenu::showFloorItemGMCommands()
{
    const bool legacy = Net::getNetworkType() == ServerType::TMWATHENA;
    if (legacy)
        return;
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show item information
    mBrowserBox->addRow("/iteminfo 'ITEMID'", _("Info"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show who drops item
    mBrowserBox->addRow("/whodrops 'ITEMID'", _("Who drops"));
}

void PopupMenu::showItemGMCommands()
{
    const bool legacy = Net::getNetworkType() == ServerType::TMWATHENA;
    if (legacy)
        return;
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show item information
    mBrowserBox->addRow("/iteminfo 'ITEMID'", _("Info"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: show who drops item
    mBrowserBox->addRow("/whodrops 'ITEMID'", _("Who drops"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 1", _("Add 1"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 5", _("Add 5"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 10", _("Add 10"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 100", _("Add 100"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 1000", _("Add 1000"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: gm create item
    mBrowserBox->addRow("/createitem 'ITEMID' 10000", _("Add 10000"));
}

void PopupMenu::showGMPopup(const std::string &name)
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("GM commands"));
    if (localPlayer->isGM())
    {
        switch (mType)
        {
            case ActorType::Player:
                showPlayerGMCommands(name);
                break;
            case ActorType::Npc:
                showNpcGMCommands();
                break;
            case ActorType::Monster:
                showMonsterGMCommands();
                break;
            case ActorType::FloorItem:
                showFloorItemGMCommands();
                break;
            case ActorType::Homunculus:
                showHomunGMCommands();
                break;
            case ActorType::Pet:
                showPetGMCommands();
                break;
            case ActorType::Mercenary:
                showMercenaryGMCommands();
                break;
            case ActorType::SkillUnit:
                break;
            default:
            case ActorType::Unknown:
                if (mItemId != 0)
                    showItemGMCommands();
                break;
            case ActorType::Portal:
            case ActorType::Avatar:
            case ActorType::Elemental:
                break;
        }
    }

    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(getX(), getY());
}

void PopupMenu::showHomunGMCommands()
{
}

void PopupMenu::showPetGMCommands()
{
}

void PopupMenu::showMercenaryGMCommands()
{
}

void PopupMenu::showCraftPopup()
{
    mBrowserBox->clearRows();

    for (int f = 0; f < 9; f ++)
    {
        const std::string cmd = strprintf("/craft %d", f);
        mBrowserBox->addRow(cmd,
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move current item to craft slot
            strprintf(_("Move to craft %d"), f + 1).c_str());
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(mX, mY);
}

void PopupMenu::addMailCommands()
{
    if (!serverFeatures->haveMail())
        return;

    // TRANSLATORS: popup menu item
    // TRANSLATORS: open mail dialog
    mBrowserBox->addRow("/mailto 'NAME'", _("Mail to..."));
}

void PopupMenu::addCatchPetCommands()
{
    if (!serverFeatures->haveServerPets())
        return;
    // TRANSLATORS: popup menu item
    // TRANSLATORS: catch pet command
    mBrowserBox->addRow("/catchpet :'BEINGID'", _("Taming pet"));
}

void PopupMenu::showAdoptCommands()
{
    if (!serverFeatures->haveFamily())
        return;
    // TRANSLATORS: popup menu item
    // TRANSLATORS: adopt child command
    mBrowserBox->addRow("/adoptchild 'NAME'", _("Adopt child"));
}

void PopupMenu::addSocialMenu()
{
    mBrowserBox->clearRows();
    setMousePos();
    const Party *const party = localPlayer->getParty();
    if (party)
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: leave party
        mBrowserBox->addRow("/leaveparty", _("Leave party"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: create party
        mBrowserBox->addRow("/createparty", _("Create party"));
    }

    if (serverFeatures->haveNativeGuilds())
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: leave guild
            mBrowserBox->addRow("/leaveguild", _("Leave guild"));
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: create guild
            mBrowserBox->addRow("/createguild", _("Create guild"));
        }
    }
    mBrowserBox->addRow("##3---");
}

void PopupMenu::showPartyPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showGuildPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: change guild notice message
    mBrowserBox->addRow("/guildnotice", _("Change notice"));

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showAttackPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showNavigationPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showPickupPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showPlayersPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::showFriendsPopup()
{
    addSocialMenu();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(mX, mY);
}

void PopupMenu::moveUp()
{
    mBrowserBox->moveSelectionUp();
}

void PopupMenu::moveDown()
{
    mBrowserBox->moveSelectionDown();
}

void PopupMenu::select()
{
    mBrowserBox->selectSelection();
}
