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

static void outStringNormal(ChatTab *const tab,
                            const std::string &str,
                            const std::string &def)
{
    if (!localPlayer)
        return;

    if (!tab)
    {
        Net::getChatHandler()->talk(str, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTabType::PARTY:
        {
            Net::getPartyHandler()->chat(str);
            break;
        }
        case ChatTabType::GUILD:
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                {
                    if (tmwServerVersion > 0)
                        return;
                    Net::getGuildHandler()->chat(guild->getId(), str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
            }
            break;
        }
        case ChatTabType::WHISPER:
        {
            const WhisperTab *const whisper
                = static_cast<const WhisperTab *const>(tab);
            tab->chatLog(localPlayer->getName(), str);
            Net::getChatHandler()->privateMessage(whisper->getNick(), str);
            break;
        }
        default:
            Net::getChatHandler()->talk(def, GENERAL_CHANNEL);
            break;
    }
}

impHandler(hack)
{
    Net::getChatHandler()->sendRaw(event.args);
    return true;
}

impHandler0(priceSave)
{
    if (shopWindow)
    {
        shopWindow->saveList();
        return true;
    }
    return false;
}

impHandler0(disconnect)
{
    Net::getGameHandler()->disconnect2();
    return true;
}

impHandler(undress)
{
    if (!actorManager)
        return false;

    Being *const target = actorManager->findNearestByName(event.args);
    if (target)
        Net::getBeingHandler()->undress(target);
    return true;
}

impHandler(attack)
{
    if (!localPlayer || !actorManager)
        return false;

    Being *const target = actorManager->findNearestByName(event.args);
    if (target)
        localPlayer->setTarget(target);
    localPlayer->attack2(localPlayer->getTarget(), true);
    return true;
}

impHandler0(dirs)
{
    if (!localPlayer || !debugChatTab)
        return false;

    debugChatTab->chatLog("config directory: "
        + settings.configDir);
    debugChatTab->chatLog("logs directory: "
        + settings.localDataDir);
    debugChatTab->chatLog("screenshots directory: "
        + settings.screenshotDir);
    debugChatTab->chatLog("temp directory: "
        + settings.tempDir);
    return true;
}

impHandler(info)
{
    if (!event.tab || !localPlayer || tmwServerVersion > 0)
        return false;

    switch (event.tab->getType())
    {
        case ChatTabType::GUILD:
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
                Net::getGuildHandler()->info(guild->getId());
            break;
        }
        default:
            break;
    }
    return true;
}

impHandler(wait)
{
    if (localPlayer)
    {
        localPlayer->waitFor(event.args);
        return true;
    }
    return false;
}

impHandler0(uptime)
{
    if (!debugChatTab)
        return false;

    if (cur_time < start_time)
    {
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), "unknown"));
    }
    else
    {
        std::string str;
        int timeDiff = cur_time - start_time;

        const int weeks = timeDiff / 60 / 60 / 24 / 7;
        if (weeks > 0)
        {
            // TRANSLATORS: uptime command
            str = strprintf(ngettext(N_("%d week"), N_("%d weeks"),
                weeks), weeks);
            timeDiff -= weeks * 60 * 60 * 24 * 7;
        }

        const int days = timeDiff / 60 / 60 / 24;
        if (days > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d day"), N_("%d days"),
                days), days));
            timeDiff -= days * 60 * 60 * 24;
        }
        const int hours = timeDiff / 60 / 60;
        if (hours > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d hour"), N_("%d hours"),
                hours), hours));
            timeDiff -= hours * 60 * 60;
        }
        const int min = timeDiff / 60;
        if (min > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d minute"), N_("%d minutes"),
                min), min));
            timeDiff -= min * 60;
        }

        if (timeDiff > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d second"), N_("%d seconds"),
                timeDiff), timeDiff));
        }
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), str.c_str()));
    }
    return true;
}

impHandler(addPriorityAttack)
{
    if (!actorManager
        || actorManager->isInPriorityAttackList(event.args))
    {
        return false;
    }

    actorManager->removeAttackMob(event.args);
    actorManager->addPriorityAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addAttack)
{
    if (!actorManager || actorManager->isInAttackList(event.args))
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(removeAttack)
{
    if (!actorManager || event.args.empty()
        || !actorManager->isInAttackList(event.args))
    {
        return false;
    }

    actorManager->removeAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addIgnoreAttack)
{
    if (!actorManager || actorManager->isInIgnoreAttackList(event.args))
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addIgnoreAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler0(cacheInfo)
{
    if (!chatWindow || !debugChatTab)
        return false;

/*
    Font *const font = chatWindow->getFont();
    if (!font)
        return;

    const TextChunkList *const cache = font->getCache();
    if (!cache)
        return;

    unsigned int all = 0;
    debugChatTab->chatLog(_("font cache size"));
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].size)
        {
            const unsigned int sz = static_cast<int>(cache[f].size);
            all += sz;
            str.append(strprintf("%d: %u, ", f, sz));
        }
    }
    debugChatTab->chatLog(str);
    debugChatTab->chatLog(strprintf("%s %d", _("Cache size:"), all));
#ifdef DEBUG_FONT_COUNTERS
    debugChatTab->chatLog("");
    debugChatTab->chatLog(strprintf("%s %d",
        _("Created:"), font->getCreateCounter()));
    debugChatTab->chatLog(strprintf("%s %d",
        _("Deleted:"), font->getDeleteCounter()));
#endif
*/
    return true;
}

impHandler0(serverIgnoreAll)
{
    Net::getChatHandler()->ignoreAll();
    return true;
}

impHandler0(serverUnIgnoreAll)
{
    Net::getChatHandler()->unIgnoreAll();
    return true;
}

impHandler(dumpGraphics)
{
    std::string str = strprintf("%s,%s,%dX%dX%d,", PACKAGE_OS, SMALL_VERSION,
        mainGraphics->getWidth(), mainGraphics->getHeight(),
        mainGraphics->getBpp());

    if (mainGraphics->getFullScreen())
        str.append("F");
    else
        str.append("W");
    if (mainGraphics->getHWAccel())
        str.append("H");
    else
        str.append("S");

    if (mainGraphics->getDoubleBuffer())
        str.append("D");
    else
        str.append("_");

#if defined USE_OPENGL
    str.append(strprintf(",%d", mainGraphics->getOpenGL()));
#else
    str.append(",0");
#endif

    str.append(strprintf(",%f,", static_cast<double>(settings.guiAlpha)))
        .append(config.getBoolValue("adjustPerfomance") ? "1" : "0")
        .append(config.getBoolValue("alphaCache") ? "1" : "0")
        .append(config.getBoolValue("enableMapReduce") ? "1" : "0")
        .append(config.getBoolValue("beingopacity") ? "1" : "0")
        .append(",")
        .append(config.getBoolValue("enableAlphaFix") ? "1" : "0")
        .append(config.getBoolValue("disableAdvBeingCaching") ? "1" : "0")
        .append(config.getBoolValue("disableBeingCaching") ? "1" : "0")
        .append(config.getBoolValue("particleeffects") ? "1" : "0")
        .append(strprintf(",%d-%d", fps, config.getIntValue("fpslimit")));
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler0(dumpEnvironment)
{
    logger->log1("Start environment variables");
    for (char **env = environ; *env; ++ env)
        logger->log1(*env);
    logger->log1("End environment variables");
    if (debugChatTab)
    {
        // TRANSLATORS: dump environment command
        debugChatTab->chatLog(_("Environment variables dumped"));
    }
    return true;
}

impHandler(dumpTests)
{
    const std::string str = config.getStringValue("testInfo");
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler(setDrop)
{
    GameModifiers::setQuickDropCounter(atoi(event.args.c_str()));
    return true;
}

impHandler0(error)
{
    const int *const ptr = nullptr;
    logger->log("test %d", *ptr);
    exit(1);
}

impHandler(url)
{
    if (event.tab)
    {
        std::string url = event.args;
        if (!strStartWith(url, "http") && !strStartWith(url, "?"))
            url = "http://" + url;
        std::string str(strprintf("[@@%s |%s@@]", url.c_str(), event.args.c_str()));
        outStringNormal(event.tab, str, str);
        return true;
    }
    return false;
}

impHandler(open)
{
    std::string url = event.args;
    if (!strStartWith(url, "http"))
        url = "http://" + url;
    openBrowser(url);
    return true;
}

impHandler(execute)
{
    const size_t idx = event.args.find(" ");
    std::string name;
    std::string params;
    if (idx == std::string::npos)
    {
        name = event.args;
    }
    else
    {
        name = event.args.substr(0, idx);
        params = event.args.substr(idx + 1);
    }
    execFile(name, name, params, "");
    return true;
}

impHandler(enableHighlight)
{
    if (event.tab)
    {
        event.tab->setAllowHighlight(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(disableHighlight)
{
    if (event.tab)
    {
        event.tab->setAllowHighlight(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(dontRemoveName)
{
    if (event.tab)
    {
        event.tab->setRemoveNames(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
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

impHandler0(testsdlfont)
{
#if defined USE_OPENGL && defined DEBUG_SDLFONT
    Font *font = new Font("fonts/dejavusans.ttf", 18);
    timespec time1;
    timespec time2;
    NullOpenGLGraphics *nullGraphics = new NullOpenGLGraphics;
    std::vector<std::string> data;
    volatile int width = 0;

    for (int f = 0; f < 300; f ++)
        data.push_back("test " + toString(f) + "string");
    nullGraphics->beginDraw();

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for (int f = 0; f < 500; f ++)
    {
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
        {
            width += font->getWidth(*it);
            font->drawString(nullGraphics, *it, 10, 10);
        }
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
            font->drawString(nullGraphics, *it, 10, 10);

        font->doClean();
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);

    delete nullGraphics;
    delete font;

    int64_t diff = (static_cast<long long int>(
        time2.tv_sec) * 1000000000LL + static_cast<long long int>(
        time2.tv_nsec)) / 100000 - (static_cast<long long int>(
        time1.tv_sec) * 1000000000LL + static_cast<long long int>(
        time1.tv_nsec)) / 100000;
    if (debugChatTab)
        debugChatTab->chatLog("sdlfont time: " + toString(diff));
#endif
    return true;
}

impHandler(dumpMods)
{
    std::string str = "enabled mods: " + serverConfig.getValue("mods", "");
    outStringNormal(event.tab, str, str);
    return true;
}

#ifdef USE_OPENGL
impHandler(dumpGL)
{
    std::string str = graphicsManager.getGLVersion();
    outStringNormal(event.tab, str, str);
    return true;
}
#else
impHandler0(dumpGL)
{
    return true;
}
#endif

#ifdef DEBUG_DUMP_LEAKS1
void showRes(std::string str, ResourceManager::Resources *res);

void showRes(std::string str, ResourceManager::Resources *res)
{
    if (!res)
        return;

    str.append(toString(res->size()));
    if (debugChatTab)
        debugChatTab->chatLog(str);
    logger->log(str);
    ResourceManager::ResourceIterator iter = res->begin();
    const ResourceManager::ResourceIterator iter_end = res->end();
    while (iter != iter_end)
    {
        if (iter->second && iter->second->getRefCount())
        {
            char type = ' ';
            char isNew = 'N';
            if (iter->second->getDumped())
                isNew = 'O';
            else
                iter->second->setDumped(true);

            SubImage *const subImage = dynamic_cast<SubImage *const>(
                iter->second);
            Image *const image = dynamic_cast<Image *const>(iter->second);
            int id = 0;
            if (subImage)
                type = 'S';
            else if (image)
                type = 'I';
            if (image)
                id = image->getGLImage();
            logger->log("Resource %c%c: %s (%d) id=%d", type,
                isNew, iter->second->getIdPath().c_str(),
                iter->second->getRefCount(), id);
        }
        ++ iter;
    }
}

impHandler(dump)
{
    if (!debugChatTab)
        return false;

    ResourceManager *const resman = ResourceManager::getInstance();

    if (!event.args.empty())
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        showRes(_("Resource images:"), res);
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        showRes(_("Resource orphaned images:"), res);
    }
    else
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource images:") + toString(res->size()));
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource orphaned images:")
            + toString(res->size()));
    }
    return true;
}

#elif defined ENABLE_MEM_DEBUG
impHandler0(dump)
{
    check_leaks();
    return true;
}
#else
impHandler0(dump)
{
    return true;
}
#endif


impHandler0(dumpOGL)
{
#if defined USE_OPENGL && !defined ANDROID
    NormalOpenGLGraphics::dumpSettings();
#endif
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
