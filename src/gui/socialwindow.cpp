/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/socialwindow.h"

#include "actorspritemanager.h"
#include "guild.h"
#include "guildmanager.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "logger.h"
#include "map.h"
#include "party.h"

#include "gui/confirmdialog.h"
#include "gui/okdialog.h"
#include "gui/outfitwindow.h"
#include "gui/setup.h"
#include "gui/textdialog.h"
#include "gui/theme.h"

#include "gui/widgets/avatarlistbox.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/container.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/linkhandler.h"
#include "gui/widgets/popup.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tab.h"
#include "gui/widgets/tabbedarea.h"

#include "net/net.h"
#include "net/guildhandler.h"
#include "net/partyhandler.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

class SocialTab : public Tab
{
protected:
    friend class SocialWindow;

    SocialTab():
            mInviteDialog(0),
            mConfirmDialog(0),
            mScroll(0),
            mList(0)
    {}

    virtual ~SocialTab()
    {
        // Cleanup dialogs
        if (mInviteDialog)
        {
            mInviteDialog->close();
            mInviteDialog->scheduleDelete();
            mInviteDialog = nullptr;
        }

        if (mConfirmDialog)
        {
            mConfirmDialog->close();
            mConfirmDialog->scheduleDelete();
            mConfirmDialog = nullptr;
        }
    }

    virtual void invite() = 0;

    virtual void leave() = 0;

    virtual void updateList() = 0;

    virtual void updateAvatar(std::string name) = 0;

    virtual void resetDamage(std::string name) = 0;

    virtual void selectIndex(unsigned num A_UNUSED)
    { }

    TextDialog *mInviteDialog;
    ConfirmDialog *mConfirmDialog;
    ScrollArea *mScroll;
    AvatarListBox *mList;
};

class SocialGuildTab : public SocialTab, public gcn::ActionListener
{
public:
    SocialGuildTab(Guild *guild):
            mGuild(guild)
    {
        setCaption(_("Guild"));

        setTabColor(&Theme::getThemeColor(Theme::GUILD_SOCIAL_TAB));

        mList = new AvatarListBox(guild);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    }

    ~SocialGuildTab()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
    }

    void action(const gcn::ActionEvent &event)
    {
        if (event.getId() == "do invite")
        {
            std::string name = mInviteDialog->getText();
            Net::getGuildHandler()->invite(mGuild->getId(), name);

            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(
                    _("Invited user %s to guild %s."),
                    name.c_str(), mGuild->getName().c_str()), BY_SERVER);
            }
            mInviteDialog = 0;
        }
        else if (event.getId() == "~do invite")
        {
            mInviteDialog = 0;
        }
        else if (event.getId() == "yes")
        {
            Net::getGuildHandler()->leave(mGuild->getId());
            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(_("Guild %s quit requested."),
                                      mGuild->getName().c_str()), BY_SERVER);
            }
            mConfirmDialog = 0;
        }
        else if (event.getId() == "~yes")
        {
            mConfirmDialog = 0;
        }
    }

    void updateList()
    {
    }

    void updateAvatar(std::string name A_UNUSED)
    {
    }

    void resetDamage(std::string name A_UNUSED)
    {
    }

protected:
    void invite()
    {
        // TODO - Give feedback on whether the invite succeeded
        mInviteDialog = new TextDialog(_("Member Invite to Guild"),
                     strprintf(_("Who would you like to invite to guild %s?"),
                               mGuild->getName().c_str()),
                     socialWindow);
        mInviteDialog->setActionEventId("do invite");
        mInviteDialog->addActionListener(this);
    }

    void leave()
    {
        mConfirmDialog = new ConfirmDialog(_("Leave Guild?"),
                       strprintf(_("Are you sure you want to leave guild %s?"),
                                 mGuild->getName().c_str()),
                       socialWindow);

        mConfirmDialog->addActionListener(this);
    }

private:
    Guild *mGuild;
};

class SocialGuildTab2 : public SocialTab, public gcn::ActionListener
{
public:
    SocialGuildTab2(Guild *guild):
              mGuild(guild)
    {
        setCaption(_("Guild"));

        setTabColor(&Theme::getThemeColor(Theme::GUILD_SOCIAL_TAB));

        mList = new AvatarListBox(guild);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    }

    ~SocialGuildTab2()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
    }

    void action(const gcn::ActionEvent &event A_UNUSED)
    {
/*
        if (event.getId() == "do invite")
        {
            std::string name = mInviteDialog->getText();
            Net::getGuildHandler()->invite(mGuild->getId(), name);

            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(
                    _("Invited user %s to guild %s."),
                    name.c_str(), mGuild->getName().c_str()), BY_SERVER);
            }
            mInviteDialog = 0;
        }
        else if (event.getId() == "~do invite")
        {
            mInviteDialog = 0;
        }
        else if (event.getId() == "yes")
        {
            Net::getGuildHandler()->leave(mGuild->getId());
            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(_("Guild %s quit requested."),
                                      mGuild->getName().c_str()), BY_SERVER);
            }
            mConfirmDialog = 0;
        }
        else if (event.getId() == "~yes")
        {
            mConfirmDialog = 0;
        }
*/
    }

    void updateList()
    {
    }

    void updateAvatar(std::string name A_UNUSED)
    {
    }

    void resetDamage(std::string name A_UNUSED)
    {
    }

protected:
    void invite()
    {
/*
        mInviteDialog = new TextDialog(_("Member Invite to Guild"),
                     strprintf(_("Who would you like to invite to guild %s?"),
                               mGuild->getName().c_str()),
                     socialWindow);
        mInviteDialog->setActionEventId("do invite");
        mInviteDialog->addActionListener(this);
*/
    }

    void leave()
    {
/*
        mConfirmDialog = new ConfirmDialog(_("Leave Guild?"),
                       strprintf(_("Are you sure you want to leave guild %s?"),
                                 mGuild->getName().c_str()),
                       socialWindow);

        mConfirmDialog->addActionListener(this);
*/
    }

private:
    Guild *mGuild;
};

class SocialPartyTab : public SocialTab, public gcn::ActionListener
{
public:
    SocialPartyTab(Party *party):
            mParty(party)
    {
        setCaption(_("Party"));

        setTabColor(&Theme::getThemeColor(Theme::PARTY_SOCIAL_TAB));

        mList = new AvatarListBox(party);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    }

    ~SocialPartyTab()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
    }

    void action(const gcn::ActionEvent &event)
    {
        if (event.getId() == "do invite")
        {
            std::string name = mInviteDialog->getText();
            Net::getPartyHandler()->invite(name);

            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(_("Invited user %s to party."),
                                      name.c_str()), BY_SERVER);
            }
            mInviteDialog = nullptr;
        }
        else if (event.getId() == "~do invite")
        {
            mInviteDialog = nullptr;
        }
        else if (event.getId() == "yes")
        {
            Net::getPartyHandler()->leave();
            if (localChatTab)
            {
                localChatTab->chatLog(strprintf(_("Party %s quit requested."),
                                      mParty->getName().c_str()), BY_SERVER);
            }
            mConfirmDialog = nullptr;
        }
        else if (event.getId() == "~yes")
        {
            mConfirmDialog = nullptr;
        }
    }

    void updateList()
    {
    }

    void updateAvatar(std::string name A_UNUSED)
    {
    }

    void resetDamage(std::string name A_UNUSED)
    {
    }

protected:
    void invite()
    {
        // TODO - Give feedback on whether the invite succeeded
        mInviteDialog = new TextDialog(_("Member Invite to Party"),
                      strprintf(_("Who would you like to invite to party %s?"),
                                mParty->getName().c_str()),
                      socialWindow);
        mInviteDialog->setActionEventId("do invite");
        mInviteDialog->addActionListener(this);
    }

    void leave()
    {
        mConfirmDialog = new ConfirmDialog(_("Leave Party?"),
                       strprintf(_("Are you sure you want to leave party %s?"),
                                 mParty->getName().c_str()),
                       socialWindow);

        mConfirmDialog->addActionListener(this);
    }

private:
    Party *mParty;
};

/*class BuddyTab : public SocialTab
{
    // TODO?
};*/


class BeingsListModal : public AvatarListModel
{
public:
    BeingsListModal()
    {
    }

    ~BeingsListModal()
    {
        delete_all(mMembers);
        mMembers.clear();
    }

    std::vector<Avatar*> *getMembers()
    {
        return &mMembers;
    }

    virtual Avatar *getAvatarAt(int index)
    {
        return mMembers[index];
    }

    int getNumberOfElements()
    {
        return static_cast<int>(mMembers.size());
    }

    std::vector<Avatar*> mMembers;
};

class SocialPlayersTab : public SocialTab
{
public:
    SocialPlayersTab(std::string name)
    {
        mBeings = new BeingsListModal();

        mList = new AvatarListBox(mBeings);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

//        mBeings->getMembers().push_back(new Avatar("test"));
        updateList();
        setCaption(name);
    }

    ~SocialPlayersTab()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
        delete mBeings;
        mBeings = 0;
    }

    void updateList()
    {
        getPlayersAvatars();
    }

    void updateAvatar(std::string name)
    {
        if (!actorSpriteManager)
            return;

        Avatar *avatar = findAvatarbyName(name);
        if (!avatar)
            return;
        if (Party::getParty(1))
        {
            PartyMember *pm = Party::getParty(1)->getMember(name);
            if (pm && pm->getMaxHp() > 0)
            {
                avatar->setMaxHp(pm->getMaxHp());
                avatar->setHp(pm->getHp());
            }
        }
        Being* being = actorSpriteManager->findBeingByName(
            name, Being::PLAYER);
        if (being)
        {
            avatar->setDamageHp(being->getDamageTaken());
            avatar->setLevel(being->getLevel());
            avatar->setGender(being->getGender());
            avatar->setIp(being->getIp());
        }
    }

    void resetDamage(std::string name)
    {
        if (!actorSpriteManager)
            return;

        Avatar *avatar = findAvatarbyName(name);
        if (!avatar)
            return;
        avatar->setDamageHp(0);
        Being* being = actorSpriteManager->findBeingByName(
            name, Being::PLAYER);

        if (being)
            being->setDamageTaken(0);
    }

    Avatar* findAvatarbyName(std::string name)
    {
        std::vector<Avatar*> *avatars = mBeings->getMembers();
        if (!avatars)
            return 0;

        Avatar *ava = 0;
        std::vector<Avatar*>::const_iterator i = avatars->begin();
        while (i != avatars->end())
        {
            ava = (*i);
            if (ava && ava->getName() == name)
                return ava;
            ++i;
        }
        ava = new Avatar(name);
        ava->setOnline(true);
        avatars->push_back(ava);
        return ava;
    }

    void getPlayersAvatars()
    {
        std::vector<Avatar*> *avatars = mBeings->getMembers();
        if (!avatars)
            return;

        if (actorSpriteManager)
        {
//            std::list<Being*> beings = actorSpriteManager->getAll();
            std::vector<std::string> names;
            actorSpriteManager->getPlayerNames(names, false);

            std::vector<Avatar*>::iterator ai = avatars->begin();
            while (ai != avatars->end())
            {
                bool finded = false;
                Avatar *ava = (*ai);
                if (!ava)
                    break;

                std::vector<std::string>::const_iterator i = names.begin();
                while (i != names.end())
                {
                    if (ava->getName() == (*i) && (*i) != "")
                    {
                        finded = true;
                        break;
                    }
                    ++i;
                }

                if (!finded)
                {
                    delete *ai;
                    ai = avatars->erase(ai);
                }
                else
                {
                    ++ai;
                }
            }

            std::vector<std::string>::const_iterator i = names.begin();

            while (i != names.end())
            {
                if ((*i) != "")
                    updateAvatar(*i);
                ++i;
            }
        }
    }

protected:
    void invite()
    {
    }

    void leave()
    {
    }

private:
    BeingsListModal *mBeings;
};


class SocialNavigationTab : public SocialTab
{
public:
    SocialNavigationTab()
    {
        mBeings = new BeingsListModal();

        mList = new AvatarListBox(mBeings);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

        setCaption(_("Nav"));

    }

    ~SocialNavigationTab()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
        delete mBeings;
        mBeings = 0;
    }

    void invite()
    {
    }

    void leave()
    {
    }

    void updateList()
    {
        if (!socialWindow || !player_node)
            return;

        Map* map = socialWindow->getMap();
        if (!map)
            return;

        if (socialWindow->getProcessedPortals())
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();
        std::vector<MapItem*> portals = map->getPortals();

        std::vector<MapItem*>::const_iterator i = portals.begin();
        SpecialLayer *specialLayer = map->getSpecialLayer();

        std::vector<Avatar*>::iterator ia = avatars->begin();

        while (ia != avatars->end())
        {
            delete *ia;
            ++ ia;
        }

        avatars->clear();

        int idx = 0;
        while (i != portals.end())
        {
            MapItem *portal = *i;
            if (!portal)
                continue;

            int x = portal->getX();
            int y = portal->getY();

            std::string name = strprintf("%s [%d %d]",
                portal->getComment().c_str(), x, y);

            Avatar *ava = new Avatar(name);
            if (player_node)
                ava->setOnline(player_node->isReachable(x, y, 0));
            else
                ava->setOnline(false);
            ava->setLevel(-1);
            ava->setType(portal->getType());
            ava->setX(x);
            ava->setY(y);
            avatars->push_back(ava);

            if (config.getBoolValue("drawHotKeys") && idx < 80 && outfitWindow)
            {
                Being *being = actorSpriteManager->findPortalByTile(x, y);
                if (being)
                {
                    being->setName(keyboard.getKeyShortString(
                        outfitWindow->keyName(idx)));
                }

                if (specialLayer)
                {
                    portal = specialLayer->getTile(ava->getX(), ava->getY());
                    if (portal)
                    {
                        portal->setName(keyboard.getKeyShortString(
                            outfitWindow->keyName(idx)));
                    }
                }
            }

            ++i;
            idx ++;
        }
        if (socialWindow)
            socialWindow->setProcessedPortals(true);
    }


    virtual void selectIndex(unsigned num)
    {
        if (!player_node)
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();
        if (!avatars || avatars->size() <= num)
            return;

        Avatar *ava = avatars->at(num);
        if (ava && player_node)
            player_node->navigateTo(ava->getX(), ava->getY());
    }

    void updateNames()
    {
        if (!socialWindow)
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();
        if (!avatars)
            return;

        Map *map = socialWindow->getMap();
        if (!map)
            return;

        Avatar *ava = 0;
        std::vector<Avatar*>::const_iterator i = avatars->begin();
        while (i != avatars->end())
        {
            ava = (*i);
            if (!ava)
                break;

            MapItem *item = map->findPortalXY(ava->getX(), ava->getY());
            if (item)
            {
                std::string name = strprintf("%s [%d %d]",
                    item->getComment().c_str(), item->getX(), item->getY());
                ava->setName(name);
                ava->setOriginalName(name);
            }

            ++i;
        }
    }

    int getPortalIndex(int x, int y)
    {
        if (!socialWindow)
            return -1;

        std::vector<Avatar*> *avatars = mBeings->getMembers();
        if (!avatars)
            return -1;

        Map *map = socialWindow->getMap();
        if (!map)
            return 01;

        Avatar *ava = 0;
        std::vector<Avatar*>::const_iterator i = avatars->begin();
        unsigned num = 0;
        while (i != avatars->end())
        {
            ava = (*i);

            if (!ava)
                break;

            if (ava->getX() == x && ava->getY() == y)
                return num;

            ++i;
            num ++;
        }
        return -1;
    }

    void addPortal(int x, int y)
    {
        if (!socialWindow || !player_node)
            return;

        Map* map = socialWindow->getMap();
        if (!map)
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();

        if (!avatars)
            return;

        MapItem *portal = map->findPortalXY(x, y);
        if (!portal)
            return;

        std::string name = strprintf("%s [%d %d]",
            portal->getComment().c_str(), x, y);

        Avatar *ava = new Avatar(name);
        if (player_node)
            ava->setOnline(player_node->isReachable(x, y, 0));
        else
            ava->setOnline(false);
        ava->setLevel(-1);
        ava->setType(portal->getType());
        ava->setX(x);
        ava->setY(y);
        avatars->push_back(ava);
    }

    void removePortal(int x, int y)
    {
        if (!socialWindow || !player_node)
            return;

        Map* map = socialWindow->getMap();
        if (!map)
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();

        std::vector<Avatar*>::iterator i = avatars->begin();

        if (!avatars)
            return;

        while (i != avatars->end())
        {
            Avatar *ava = (*i);

            if (!ava)
                break;

            if (ava && ava->getX() == x && ava->getY() == y)
            {
                delete ava;
                avatars->erase(i);
                return;
            }

            ++ i;
        }
    }

    void updateAvatar(std::string)
    {
    }

    void resetDamage(std::string)
    {
    }

private:
    BeingsListModal *mBeings;

protected:
//    friend class SocialWindow;
};


class SocialAttackTab : public SocialTab
{
public:
    SocialAttackTab()
    {
        mBeings = new BeingsListModal();

        mList = new AvatarListBox(mBeings);
        mScroll = new ScrollArea(mList);

        mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
        mScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

        setCaption(_("Atk"));
    }

    ~SocialAttackTab()
    {
        delete mList;
        mList = 0;
        delete mScroll;
        mScroll = 0;
        delete mBeings;
        mBeings = 0;
    }

    void invite()
    {
    }

    void leave()
    {
    }

    void updateList()
    {
        if (!socialWindow || !player_node || !actorSpriteManager)
            return;

        std::vector<Avatar*> *avatars = mBeings->getMembers();

        std::list<std::string> mobs
            = actorSpriteManager->getPriorityAttackMobs();
        std::list<std::string>::iterator i = mobs.begin();

        std::vector<Avatar*>::iterator ia = avatars->begin();

        while (ia != avatars->end())
        {
            delete *ia;
            ++ ia;
        }

        avatars->clear();
        Avatar *ava = new Avatar(_("Priority mobs"));
        ava->setOnline(false);
        ava->setLevel(-1);
        ava->setType(MapItem::SEPARATOR);
        ava->setX(0);
        ava->setY(0);
        avatars->push_back(ava);

        while (i != mobs.end())
        {
            std::string name;
            int level = -1;
            if (*i == "")
            {
                name = _("(default)");
                level = 0;
            }
            else
            {
                name = *i;
            }
            ava = new Avatar(name);
            ava->setOnline(true);
            ava->setLevel(level);
            ava->setType(MapItem::PRIORITY);
            ava->setX(0);
            ava->setY(0);
            avatars->push_back(ava);

            ++ i;
        }

        ava = new Avatar(_("Attack mobs"));
        ava->setOnline(false);
        ava->setLevel(-1);
        ava->setType(MapItem::SEPARATOR);
        ava->setX(0);
        ava->setY(0);
        avatars->push_back(ava);

        mobs = actorSpriteManager->getAttackMobs();
        i = mobs.begin();

        while (i != mobs.end())
        {
            std::string name;
            int level = -1;
            if (*i == "")
            {
                name = _("(default)");
                level = 0;
            }
            else
            {
                name = *i;
            }
            ava = new Avatar(name);
            ava->setOnline(true);
            ava->setLevel(level);
            ava->setType(MapItem::ATTACK);
            ava->setX(0);
            ava->setY(0);
            avatars->push_back(ava);

            ++ i;
        }

        ava = new Avatar(_("Ignore mobs"));
        ava->setOnline(false);
        ava->setLevel(-1);
        ava->setType(MapItem::SEPARATOR);
        ava->setX(0);
        ava->setY(0);
        avatars->push_back(ava);

        mobs = actorSpriteManager->getIgnoreAttackMobs();
        i = mobs.begin();

        while (i != mobs.end())
        {
            std::string name;
            int level = -1;
            if (*i == "")
            {
                name = _("(default)");
                level = 0;
            }
            else
            {
                name = *i;
            }
            ava = new Avatar(name);
            ava->setOnline(false);
            ava->setLevel(level);
            ava->setType(MapItem::IGNORE_);
            ava->setX(0);
            ava->setY(0);
            avatars->push_back(ava);

            ++ i;
        }

    }

    void updateAvatar(std::string name A_UNUSED)
    {
    }

    void resetDamage(std::string name A_UNUSED)
    {
    }

private:
    BeingsListModal *mBeings;

};

class CreatePopup : public Popup, public LinkHandler
{
public:
    CreatePopup():
            Popup("SocialCreatePopup")
    {
        mBrowserBox = new BrowserBox;
        mBrowserBox->setPosition(4, 4);
        mBrowserBox->setHighlightMode(BrowserBox::BACKGROUND);
        mBrowserBox->setOpaque(false);
        mBrowserBox->setLinkHandler(this);

        if (Net::getGuildHandler()->isSupported())
            mBrowserBox->addRow(strprintf("@@guild|%s@@", _("Create Guild")));
        mBrowserBox->addRow(strprintf("@@party|%s@@", _("Create Party")));
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

        add(mBrowserBox);

        setContentSize(mBrowserBox->getWidth() + 8,
                       mBrowserBox->getHeight() + 8);
    }

    void handleLink(const std::string &link, gcn::MouseEvent *event A_UNUSED)
    {
        if (link == "guild" && socialWindow)
        {
            socialWindow->showGuildCreate();
        }
        else if (link == "party" && socialWindow)
        {
            socialWindow->showPartyCreate();
        }

        setVisible(false);
    }

    void show(gcn::Widget *parent)
    {
        if (!parent)
            return;

        int x, y;
        parent->getAbsolutePosition(x, y);
        y += parent->getHeight();
        setPosition(x, y);
        setVisible(true);
        requestMoveToTop();
    }

private:
    BrowserBox* mBrowserBox;
};

SocialWindow::SocialWindow() :
    Window(_("Social"), false, 0, "social.xml"),
    mGuildInvited(0),
    mGuildAcceptDialog(0),
    mPartyAcceptDialog(0),
    mMap(0),
    mLastUpdateTime(0),
    mNeedUpdate(false),
    mProcessedPortals(false)
{
    setWindowName("Social");
    setVisible(false);
    setSaveVisible(true);
    setResizable(true);
    setSaveVisible(true);
    setCloseButton(true);
    setStickyButtonLock(true);

    setMinWidth(120);
    setMinHeight(55);
    setDefaultSize(590, 200, 180, 300);
    setupWindow->registerWindowForReset(this);

    mCreateButton = new Button(_("Create"), "create", this);
    mInviteButton = new Button(_("Invite"), "invite", this);
    mLeaveButton = new Button(_("Leave"), "leave", this);
    mTabs = new TabbedArea;

    place(0, 0, mCreateButton);
    place(1, 0, mInviteButton);
    place(2, 0, mLeaveButton);
    place(0, 1, mTabs, 4, 4);

    widgetResized(nullptr);

    mCreatePopup = new CreatePopup();

    loadWindowState();

    mPlayers = new SocialPlayersTab("P");
    mTabs->addTab(mPlayers, mPlayers->mScroll);

    mNavigation = new SocialNavigationTab();
    mTabs->addTab(mNavigation, mNavigation->mScroll);

    if (config.getBoolValue("enableAttackFilter"))
    {
        mAttackFilter = new SocialAttackTab();
        mTabs->addTab(mAttackFilter, mAttackFilter->mScroll);
    }
    else
    {
        mAttackFilter = 0;
    }

    if (player_node && player_node->getParty())
        addTab(player_node->getParty());

    if (player_node && player_node->getGuild())
        addTab(player_node->getGuild());

    updateButtons();
}

SocialWindow::~SocialWindow()
{
    // Cleanup invites
    if (mGuildAcceptDialog)
    {
        mGuildAcceptDialog->close();
        mGuildAcceptDialog->scheduleDelete();
        mGuildAcceptDialog = nullptr;

        mGuildInvited = 0;
    }

    if (mPartyAcceptDialog)
    {
        mPartyAcceptDialog->close();
        mPartyAcceptDialog->scheduleDelete();
        mPartyAcceptDialog = nullptr;

        mPartyInviter = "";
    }
    delete mCreatePopup;
    mCreatePopup = 0;
    delete mPlayers;
    mPlayers = 0;
    delete mNavigation;
    mNavigation = 0;
    delete mAttackFilter;
    mAttackFilter = 0;
}

bool SocialWindow::addTab(Guild *guild)
{
    if (mGuilds.find(guild) != mGuilds.end())
        return false;

    SocialTab *tab = 0;
    if (guild->getServerGuild())
        tab = new SocialGuildTab(guild);
    else
        tab = new SocialGuildTab2(guild);

    mGuilds[guild] = tab;
    mTabs->addTab(tab, tab->mScroll);

    updateButtons();

    return true;
}

bool SocialWindow::removeTab(Guild *guild)
{
    GuildMap::iterator it = mGuilds.find(guild);
    if (it == mGuilds.end())
        return false;

    mTabs->removeTab(it->second);
    delete it->second;
    mGuilds.erase(it);

    updateButtons();

    return true;
}

bool SocialWindow::addTab(Party *party)
{
    if (mParties.find(party) != mParties.end())
        return false;

    SocialPartyTab *tab = new SocialPartyTab(party);
    mParties[party] = tab;

    mTabs->addTab(tab, tab->mScroll);

    updateButtons();

    return true;
}

bool SocialWindow::removeTab(Party *party)
{
    PartyMap::iterator it = mParties.find(party);
    if (it == mParties.end())
        return false;

    mTabs->removeTab(it->second);
    delete it->second;
    mParties.erase(it);

    updateButtons();

    return true;
}

void SocialWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();

    if (event.getSource() == mPartyAcceptDialog)
    {
        // check if they accepted the invite
        if (eventId == "yes")
        {
            if (localChatTab)
            {
                localChatTab->chatLog(
                    strprintf(_("Accepted party invite from %s."),
                    mPartyInviter.c_str()));
            }
            Net::getPartyHandler()->inviteResponse(mPartyInviter, true);
        }
        else if (eventId == "no")
        {
            if (localChatTab)
            {
                localChatTab->chatLog(
                    strprintf(_("Rejected party invite from %s."),
                    mPartyInviter.c_str()));
            }
            Net::getPartyHandler()->inviteResponse(mPartyInviter, false);
        }

        mPartyInviter = "";
        mPartyAcceptDialog = nullptr;
    }
    else if (event.getSource() == mGuildAcceptDialog)
    {
        // check if they accepted the invite
        if (eventId == "yes")
        {
            if (localChatTab)
            {
                localChatTab->chatLog(
                    strprintf(_("Accepted guild invite from %s."),
                    mPartyInviter.c_str()));
            }
            if (!guildManager || !GuildManager::getEnableGuildBot())
                Net::getGuildHandler()->inviteResponse(mGuildInvited, true);
            else
                guildManager->inviteResponse(true);
        }
        else if (eventId == "no")
        {
            if (localChatTab)
            {
                localChatTab->chatLog(
                    strprintf(_("Rejected guild invite from %s."),
                    mPartyInviter.c_str()));
            }
            if (!guildManager || !GuildManager::getEnableGuildBot())
                Net::getGuildHandler()->inviteResponse(mGuildInvited, false);
            else
                guildManager->inviteResponse(false);
        }

        mGuildInvited = 0;
        mGuildAcceptDialog = nullptr;
    }
    else if (event.getId() == "create")
    {
        if (Net::getGuildHandler()->isSupported())
        {
            if (mCreatePopup)
                mCreatePopup->show(mCreateButton);
        }
        else
        {
            showPartyCreate();
        }
    }
    else if (event.getId() == "invite" && mTabs->getSelectedTabIndex() > -1)
    {
        if (mTabs->getSelectedTab())
            static_cast<SocialTab*>(mTabs->getSelectedTab())->invite();
    }
    else if (event.getId() == "leave" && mTabs->getSelectedTabIndex() > -1)
    {
        if (mTabs->getSelectedTab())
            static_cast<SocialTab*>(mTabs->getSelectedTab())->leave();
    }
    else if (event.getId() == "create guild")
    {
        std::string name = mGuildCreateDialog->getText();

        if (name.size() > 16)
        {
            // TODO : State too many characters in input.
            return;
        }

        Net::getGuildHandler()->create(name);
        if (localChatTab)
        {
            localChatTab->chatLog(strprintf(_("Creating guild called %s."),
                                  name.c_str()), BY_SERVER);
        }

        mGuildCreateDialog = nullptr;
    }
    else if (event.getId() == "~create guild")
    {
        mGuildCreateDialog = nullptr;
    }
    else if (event.getId() == "create party")
    {
        std::string name = mPartyCreateDialog->getText();

        if (name.size() > 16)
        {
            // TODO : State too many characters in input.
            return;
        }

        Net::getPartyHandler()->create(name);
        if (localChatTab)
        {
            localChatTab->chatLog(strprintf(_("Creating party called %s."),
                                  name.c_str()), BY_SERVER);
        }

        mPartyCreateDialog = nullptr;
    }
    else if (event.getId() == "~create party")
    {
        mPartyCreateDialog = nullptr;
    }
}

void SocialWindow::showGuildCreate()
{
    mGuildCreateDialog = new TextDialog(_("Guild Name"),
                                        _("Choose your guild's name."), this);
    mGuildCreateDialog->setActionEventId("create guild");
    mGuildCreateDialog->addActionListener(this);
}

void SocialWindow::showGuildInvite(const std::string &guildName,
                                   const int guildId,
                                   const std::string &inviterName)
{
    // check there isnt already an invite showing
    if (mGuildInvited != 0)
    {
        if (localChatTab)
        {
            localChatTab->chatLog(_("Received guild request, but one already "
                "exists."), BY_SERVER);
        }
        return;
    }

    std::string msg = strprintf(_("%s has invited you to join the guild %s."),
                                inviterName.c_str(), guildName.c_str());
    if (localChatTab)
        localChatTab->chatLog(msg, BY_SERVER);

    // show invite
    mGuildAcceptDialog = new ConfirmDialog(_("Accept Guild Invite"),
                                           msg, false, false, this);
    mGuildAcceptDialog->addActionListener(this);

    mGuildInvited = guildId;
}

void SocialWindow::showPartyInvite(const std::string &partyName,
                                   const std::string &inviter)
{
    // check there isnt already an invite showing
    if (mPartyInviter != "")
    {
        if (localChatTab)
        {
            localChatTab->chatLog(_("Received party request, but one already "
                "exists."), BY_SERVER);
        }
        return;
    }

    std::string msg;
    if (inviter.empty())
    {
        if (partyName.empty())
        {
            msg = _("You have been invited you to join a party.");
        }
        else
        {
            msg = strprintf(_("You have been invited to join the %s party."),
                            partyName.c_str());
        }
    }
    else
    {
        if (partyName.empty())
        {
            msg = strprintf(_("%s has invited you to join their party."),
                            inviter.c_str());
        }
        else
        {
            msg = strprintf(_("%s has invited you to join the %s party."),
                            inviter.c_str(), partyName.c_str());
        }
    }

    if (localChatTab)
        localChatTab->chatLog(msg, BY_SERVER);

    // show invite
    mPartyAcceptDialog = new ConfirmDialog(_("Accept Party Invite"),
                                           msg, false, false, this);
    mPartyAcceptDialog->addActionListener(this);

    mPartyInviter = inviter;
}

void SocialWindow::showPartyCreate()
{
    if (!player_node)
        return;

    if (player_node->getParty())
    {
        new OkDialog(_("Create Party"),
                     _("Cannot create party. You are already in a party"),
                     this);
        return;
    }

    mPartyCreateDialog = new TextDialog(_("Party Name"),
                                        _("Choose your party's name."), this);
    mPartyCreateDialog->setActionEventId("create party");
    mPartyCreateDialog->addActionListener(this);
}

void SocialWindow::updateActiveList()
{
    mNeedUpdate = true;
}

void SocialWindow::logic()
{
    unsigned int nowTime = cur_time;
    if (mNeedUpdate && nowTime - mLastUpdateTime > 1)
    {
        mPlayers->updateList();
        mNeedUpdate = false;
        mLastUpdateTime = nowTime;
    }
    else if (nowTime - mLastUpdateTime > 5)
    {
        mPlayers->updateList();
        mNeedUpdate = false;
        mLastUpdateTime = nowTime;
    }

    Window::logic();
}

void SocialWindow::updateAvatar(std::string name)
{
    mPlayers->updateAvatar(name);
}

void SocialWindow::resetDamage(std::string name)
{
    mPlayers->resetDamage(name);
}

void SocialWindow::updateButtons()
{
    if (!mTabs)
        return;

    bool hasTabs = mTabs->getNumberOfTabs() > 0;
    mInviteButton->setEnabled(hasTabs);
    mLeaveButton->setEnabled(hasTabs);
}

void SocialWindow::updatePortals()
{
    if (mNavigation)
        mNavigation->updateList();
}

void SocialWindow::updatePortalNames()
{
    if (mNavigation)
        static_cast<SocialNavigationTab*>(mNavigation)->updateNames();
}

void SocialWindow::selectPortal(unsigned num)
{
    if (mNavigation)
        mNavigation->selectIndex(num);
}

int SocialWindow::getPortalIndex(int x, int y)
{
    if (mNavigation)
    {
        return static_cast<SocialNavigationTab*>(
            mNavigation)->getPortalIndex(x, y);
    }
    else
    {
        return -1;
    }
}

void SocialWindow::addPortal(int x, int y)
{
    if (mNavigation)
        static_cast<SocialNavigationTab*>(mNavigation)->addPortal(x, y);
}

void SocialWindow::removePortal(int x, int y)
{
    if (mNavigation)
        static_cast<SocialNavigationTab*>(mNavigation)->removePortal(x, y);
}

void SocialWindow::nextTab()
{
    if (!mTabs)
        return;

    int tab = mTabs->getSelectedTabIndex();

    tab++;
    if (tab == mTabs->getNumberOfTabs())
        tab = 0;

    mTabs->setSelectedTab(tab);
}

void SocialWindow::prevTab()
{
    if (!mTabs)
        return;

    int tab = mTabs->getSelectedTabIndex();

    if (tab == 0)
        tab = mTabs->getNumberOfTabs();
    tab--;

    mTabs->setSelectedTab(tab);
}

void SocialWindow::updateAttackFilter()
{
    if (mAttackFilter)
        mAttackFilter->updateList();
}

void SocialWindow::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    if (mTabs)
        mTabs->fixSize();
}
