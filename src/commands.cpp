/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "commands.h"

#include "auctionmanager.h"
#include "actormanager.h"
#include "client.h"
#include "configuration.h"
#include "game.h"
#include "gamemodifiers.h"
#include "graphicsmanager.h"
#include "guildmanager.h"
#include "main.h"
#include "party.h"
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "gui/chatconsts.h"
#include "gui/gui.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/tabs/whispertab.h"

#if defined USE_OPENGL
#include "render/normalopenglgraphics.h"
#endif

#if defined USE_OPENGL && defined DEBUG_SDLFONT
#include "render/nullopenglgraphics.h"
#endif

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/download.h"
#include "net/gamehandler.h"
#include "net/guildhandler.h"
#include "net/ipc.h"
#include "net/net.h"
#include "net/uploadcharinfo.h"
#include "net/partyhandler.h"
#include "net/pethandler.h"
#include "net/tradehandler.h"

#ifdef DEBUG_DUMP_LEAKS1
#include "resources/image.h"
#include "resources/resource.h"
#include "resources/subimage.h"
#endif

#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "resources/db/itemdb.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/process.h"
#include "utils/timer.h"

#include "debug.h"

#define impHandler(name) bool name(InputEvent &event)
#define impHandler0(name) bool name(InputEvent &event A_UNUSED)

extern std::string tradePartnerName;
extern char **environ;

namespace Commands
{

impHandler(hack)
{
    Net::getChatHandler()->sendRaw(event.args);
    return true;
}

impHandler(removeName)
{
    if (event.tab)
    {
        event.tab->setRemoveNames(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(disableAway)
{
    if (event.tab)
    {
        event.tab->setNoAway(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(enableAway)
{
    if (event.tab)
    {
        event.tab->setNoAway(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(testParticle)
{
    if (localPlayer)
    {
        localPlayer->setTestParticle(event.args);
        return true;
    }
    return false;
}

impHandler0(createItems)
{
    BuyDialog *const dialog = new BuyDialog();
    const ItemDB::ItemInfos &items = ItemDB::getItemInfos();
    FOR_EACH (ItemDB::ItemInfos::const_iterator, it, items)
    {
        const ItemInfo *const info = (*it).second;
        const int id = info->getId();
        if (id <= 500)
            continue;

        int colors = info->getColorsSize();
        if (colors >= 255)
            colors = 254;

        if (!colors)
        {
            dialog->addItem(id, 1, 100, 0);
        }
        else
        {
            for (unsigned char f = 0; f < colors; f ++)
            {
                if (!info->getColor(f).empty())
                    dialog->addItem(id, f, 100, 0);
            }
        }
    }
    dialog->sort();
    return true;
}

impHandler(talkRaw)
{
    Net::getChatHandler()->talkRaw(event.args);
    return true;
}

impHandler(talkPet)
{
    // in future probably need add channel detection
    if (!localPlayer->getPets().empty())
        Net::getChatHandler()->talkPet(event.args, GENERAL_CHANNEL);
    else
        Net::getChatHandler()->talk(event.args, GENERAL_CHANNEL);
    return true;
}

impHandler(gm)
{
    Net::getChatHandler()->talk("@wgm " + event.args, GENERAL_CHANNEL);
    return true;
}

static int uploadUpdate(void *ptr,
                        DownloadStatus::Type status,
                        size_t total A_UNUSED,
                        size_t remaining A_UNUSED)
{
    if (status == DownloadStatus::Idle || status == DownloadStatus::Starting)
        return 0;

    UploadChatInfo *const info = reinterpret_cast<UploadChatInfo*>(ptr);
    if (status == DownloadStatus::Complete)
    {
        std::string str = Net::Download::getUploadResponse();
        const size_t sz = str.size();
        if (sz > 0)
        {
            if (str[sz - 1] == '\n')
                str = str.substr(0, sz - 1);
            str.append(info->addStr);
            ChatTab *const tab = info->tab;
            if (chatWindow && (!tab || chatWindow->isTabPresent(tab)))
            {
                str = strprintf("%s [@@%s |%s@@]",
                    info->text.c_str(), str.c_str(), str.c_str());
                outStringNormal(tab, str, str);
            }
            else
            {
                // TRANSLATORS: file uploaded message
                new OkDialog(_("File uploaded"), str,
                    // TRANSLATORS: ok dialog button
                    _("OK"),
                    DialogType::OK,
                    true, false, nullptr, 260);
            }
        }
    }
    delete2(info->upload);
    delete info;
    return 0;
}

static void uploadFile(const std::string &str,
                       const std::string &fileName,
                       const std::string &addStr,
                       ChatTab *const tab)
{
    UploadChatInfo *const info = new UploadChatInfo();
    Net::Download *const upload = new Net::Download(info,
        "http://sprunge.us",
        &uploadUpdate,
        false, true, false);
    info->upload = upload;
    info->text = str;
    info->addStr = addStr;
    info->tab = tab;
    upload->setFile(fileName);
    upload->start();
}

impHandler(uploadConfig)
{
    uploadFile(_("Uploaded config into:"),
        config.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadServerConfig)
{
    uploadFile(_("Uploaded server config into:"),
        serverConfig.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadLog)
{
    uploadFile(_("Uploaded log into:"),
        settings.logFileName,
        "?txt",
        event.tab);
    return true;
}

impHandler0(debugSpawn)
{
    int cnt = atoi(event.args.c_str());
    if (cnt < 1)
        cnt = 1;
    const int half = cnt / 2;
    for (int x = -half; x < cnt - half; x ++)
    {
        for (int y =  -half; y < cnt - half; y ++)
            actorManager->cloneBeing(localPlayer, x, y, cnt);
    }
    return true;
}

void replaceVars(std::string &str)
{
    if (!localPlayer || !actorManager)
        return;

    if (str.find("<PLAYER>") != std::string::npos)
    {
        const Being *target = localPlayer->getTarget();
        if (!target || target->getType() != ActorType::PLAYER)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 20, ActorType::PLAYER, true);
        }
        if (target)
            replaceAll(str, "<PLAYER>", target->getName());
        else
            replaceAll(str, "<PLAYER>", "");
    }
    if (str.find("<MONSTER>") != std::string::npos)
    {
        const Being *target = localPlayer->getTarget();
        if (!target || target->getType() != ActorType::MONSTER)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 20, ActorType::MONSTER, true);
        }
        if (target)
            replaceAll(str, "<MONSTER>", target->getName());
        else
            replaceAll(str, "<MONSTER>", "");
    }
    if (str.find("<PEOPLE>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        actorManager->getPlayerNames(names, false);
        FOR_EACH (StringVectCIter, it, names)
        {
            if (*it != localPlayer->getName())
                newStr.append(*it).append(",");
        }
        if (newStr[newStr.size() - 1] == ',')
            newStr = newStr.substr(0, newStr.size() - 1);
        if (!newStr.empty())
            replaceAll(str, "<PEOPLE>", newStr);
        else
            replaceAll(str, "<PEOPLE>", "");
    }
    if (str.find("<PARTY>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        const Party *party = nullptr;
        if (localPlayer->isInParty() && (party = localPlayer->getParty()))
        {
            party->getNames(names);
            FOR_EACH (StringVectCIter, it, names)
            {
                if (*it != localPlayer->getName())
                    newStr.append(*it).append(",");
            }
            if (newStr[newStr.size() - 1] == ',')
                newStr = newStr.substr(0, newStr.size() - 1);
            if (!newStr.empty())
                replaceAll(str, "<PARTY>", newStr);
            else
                replaceAll(str, "<PARTY>", "");
        }
        else
        {
            replaceAll(str, "<PARTY>", "");
        }
    }
}

}  // namespace Commands
