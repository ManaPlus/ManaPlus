/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "game.h"

#include "gui/npcdialog.h"

#include "actorspritemanager.h"
#include "actorsprite.h"
#include "auctionmanager.h"
#include "being.h"
#include "channelmanager.h"
#include "client.h"
#include "commandhandler.h"
#include "configuration.h"
#include "dropshortcut.h"
#include "effectmanager.h"
#include "emoteshortcut.h"
#include "event.h"
#include "guildmanager.h"
#include "graphics.h"
#include "itemshortcut.h"
#include "joystick.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "logger.h"
#include "map.h"
#include "particle.h"
#include "playerrelations.h"
#include "sound.h"
#include "spellmanager.h"
#include "spellshortcut.h"

#include "gui/botcheckerwindow.h"
#include "gui/buyselldialog.h"
#include "gui/chatwindow.h"
#include "gui/confirmdialog.h"
#include "gui/debugwindow.h"
#include "gui/didyouknowwindow.h"
#include "gui/equipmentwindow.h"
#include "gui/gui.h"
#include "gui/helpwindow.h"
#include "gui/inventorywindow.h"
#include "gui/killstats.h"
#include "gui/minimap.h"
#include "gui/ministatuswindow.h"
#include "gui/npcpostdialog.h"
#include "gui/okdialog.h"
#include "gui/outfitwindow.h"
#include "gui/sdlinput.h"
#include "gui/setup.h"
#include "gui/shopwindow.h"
#include "gui/shortcutwindow.h"
#include "gui/socialwindow.h"
#include "gui/specialswindow.h"
#include "gui/skilldialog.h"
#include "gui/statuswindow.h"
#include "gui/quitdialog.h"
#include "gui/textdialog.h"
#include "gui/tradewindow.h"
#include "gui/viewport.h"
#include "gui/windowmenu.h"
#include "gui/whoisonline.h"

#include "gui/widgets/battletab.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/dropshortcutcontainer.h"
#include "gui/widgets/emoteshortcutcontainer.h"
#include "gui/widgets/itemshortcutcontainer.h"
#include "gui/widgets/spellshortcutcontainer.h"
#include "gui/widgets/tradetab.h"

#include "net/gamehandler.h"
#include "net/generalhandler.h"
#include "net/net.h"
#include "net/packetcounters.h"
#include "net/playerhandler.h"

#include "resources/imagewriter.h"
#include "resources/mapdb.h"
#include "resources/mapreader.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/mkdir.h"

#include "utils/translation/translationmanager.h"

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include <physfs.h>

#include <fstream>
#include <sstream>
#include <string>

#include "mumblemanager.h"

#include "debug.h"

Joystick *joystick = nullptr;

OkDialog *weightNotice = nullptr;
int weightNoticeTime = 0;
OkDialog *deathNotice = nullptr;
QuitDialog *quitDialog = nullptr;
OkDialog *disconnectedDialog = nullptr;

ChatWindow *chatWindow = nullptr;
StatusWindow *statusWindow = nullptr;
MiniStatusWindow *miniStatusWindow = nullptr;
InventoryWindow *inventoryWindow = nullptr;
ShopWindow *shopWindow = nullptr;
SkillDialog *skillDialog = nullptr;
Minimap *minimap = nullptr;
EquipmentWindow *equipmentWindow = nullptr;
EquipmentWindow *beingEquipmentWindow = nullptr;
TradeWindow *tradeWindow = nullptr;
HelpWindow *helpWindow = nullptr;
DebugWindow *debugWindow = nullptr;
ShortcutWindow *itemShortcutWindow = nullptr;
ShortcutWindow *emoteShortcutWindow = nullptr;
OutfitWindow *outfitWindow = nullptr;
SpecialsWindow *specialsWindow = nullptr;
ShortcutWindow *dropShortcutWindow = nullptr;
ShortcutWindow *spellShortcutWindow = nullptr;
WhoIsOnline *whoIsOnline = nullptr;
DidYouKnowWindow *didYouKnowWindow = nullptr;
KillStats *killStats = nullptr;
BotCheckerWindow *botCheckerWindow = nullptr;
SocialWindow *socialWindow = nullptr;
WindowMenu *windowMenu = nullptr;

ActorSpriteManager *actorSpriteManager = nullptr;
ChannelManager *channelManager = nullptr;
CommandHandler *commandHandler = nullptr;
MumbleManager *mumbleManager = nullptr;
Particle *particleEngine = nullptr;
EffectManager *effectManager = nullptr;
SpellManager *spellManager = nullptr;
Viewport *viewport = nullptr;                    /**< Viewport on the map. */
GuildManager *guildManager = nullptr;
AuctionManager *auctionManager = nullptr;

ChatTab *localChatTab = nullptr;
ChatTab *debugChatTab = nullptr;
TradeTab *tradeChatTab = nullptr;
BattleTab *battleChatTab = nullptr;

const unsigned adjustDelay = 10;

/**
 * Initialize every game sub-engines in the right order
 */
static void initEngines()
{
    Event::trigger(CHANNEL_GAME, Event(EVENT_ENGINESINITALIZING));

    actorSpriteManager = new ActorSpriteManager;
    commandHandler = new CommandHandler;
    channelManager = new ChannelManager;
    effectManager = new EffectManager;
    AuctionManager::init();
    GuildManager::init();

    particleEngine = new Particle(nullptr);
    particleEngine->setupEngine();

    Event::trigger(CHANNEL_GAME, Event(EVENT_ENGINESINITALIZED));
}

/**
 * Create all the various globally accessible gui windows
 */
static void createGuiWindows()
{
    Event::trigger(CHANNEL_GAME, Event(EVENT_GUIWINDOWSLOADING));

    if (setupWindow)
        setupWindow->clearWindowsForReset();

    if (emoteShortcut)
        emoteShortcut->load();

    // Create dialogs
    chatWindow = new ChatWindow;
    tradeWindow = new TradeWindow;
    equipmentWindow = new EquipmentWindow(PlayerInfo::getEquipment(),
        player_node);
    beingEquipmentWindow = new EquipmentWindow(nullptr, nullptr, true);
    beingEquipmentWindow->setVisible(false);
    statusWindow = new StatusWindow;
    miniStatusWindow = new MiniStatusWindow;
    inventoryWindow = new InventoryWindow(PlayerInfo::getInventory());
    shopWindow = new ShopWindow;
    skillDialog = new SkillDialog;
    minimap = new Minimap;
    helpWindow = new HelpWindow;
    debugWindow = new DebugWindow;
    itemShortcutWindow = new ShortcutWindow(
        "ItemShortcut", "items.xml", 83, 460);

    for (int f = 0; f < SHORTCUT_TABS; f ++)
    {
        itemShortcutWindow->addTab(toString(f + 1),
            new ItemShortcutContainer(f));
    }
    didYouKnowWindow = new DidYouKnowWindow;
    if (config.getBoolValue("showDidYouKnow"))
    {
        didYouKnowWindow->setVisible(true);
        didYouKnowWindow->loadData();
    }

    emoteShortcutWindow = new ShortcutWindow("EmoteShortcut",
        new EmoteShortcutContainer, "emotes.xml");
    outfitWindow = new OutfitWindow();
    specialsWindow = new SpecialsWindow();
    dropShortcutWindow = new ShortcutWindow("DropShortcut",
        new DropShortcutContainer, "drops.xml");

    spellShortcutWindow = new ShortcutWindow("SpellShortcut", "spells.xml",
                                             265, 328);
    for (int f = 0; f < SPELL_SHORTCUT_TABS; f ++)
    {
        spellShortcutWindow->addTab(toString(f + 1),
            new SpellShortcutContainer(f));
    }

    botCheckerWindow = new BotCheckerWindow();
    whoIsOnline = new WhoIsOnline();
    killStats = new KillStats;
    socialWindow = new SocialWindow();

    localChatTab = new ChatTab(_("General"));
    localChatTab->setAllowHighlight(false);
    localChatTab->loadFromLogFile("#General");

    debugChatTab = new ChatTab(_("Debug"));
    debugChatTab->setAllowHighlight(false);

    if (config.getBoolValue("enableTradeTab"))
    {
        tradeChatTab = new TradeTab;
        tradeChatTab->setAllowHighlight(false);
    }
    else
    {
        tradeChatTab = nullptr;
    }

    if (config.getBoolValue("enableBattleTab"))
    {
        battleChatTab = new BattleTab;
        battleChatTab->setAllowHighlight(false);
    }
    else
    {
        battleChatTab = nullptr;
    }

    if (config.getBoolValue("logToChat"))
        logger->setChatWindow(chatWindow);

    if (!isSafeMode && chatWindow)
        chatWindow->loadState();

    if (setupWindow)
        setupWindow->externalUpdate();

    if (player_node)
        player_node->updateStatus();

    Event::trigger(CHANNEL_GAME, Event(EVENT_GUIWINDOWSLOADED));
}

#define del_0(X) { delete X; X = nullptr; }

/**
 * Destroy all the globally accessible gui windows
 */
static void destroyGuiWindows()
{
    Event::trigger(CHANNEL_GAME, Event(EVENT_GUIWINDOWSUNLOADING));

    logger->setChatWindow(nullptr);
    if (whoIsOnline)
        whoIsOnline->setAllowUpdate(false);

    if (auctionManager)
        auctionManager->clear();

    if (guildManager)
        guildManager->clear();

    del_0(windowMenu);
    del_0(localChatTab) // Need to do this first, so it can remove itself
    del_0(debugChatTab)
    del_0(tradeChatTab)
    del_0(battleChatTab)
    logger->log("start deleting");
    del_0(chatWindow)
    logger->log("end deleting");
    del_0(statusWindow)
    del_0(miniStatusWindow)
    del_0(inventoryWindow)
    del_0(shopWindow)
    del_0(skillDialog)
    del_0(minimap)
    del_0(equipmentWindow)
    del_0(beingEquipmentWindow)
    del_0(tradeWindow)
    del_0(helpWindow)
    del_0(debugWindow)
    del_0(itemShortcutWindow)
    del_0(emoteShortcutWindow)
    del_0(outfitWindow)
    del_0(specialsWindow)
    del_0(socialWindow)
    del_0(dropShortcutWindow);
    del_0(spellShortcutWindow);
    del_0(botCheckerWindow);
    del_0(whoIsOnline);
    del_0(killStats);
    del_0(didYouKnowWindow);

    Event::trigger(CHANNEL_GAME, Event(EVENT_GUIWINDOWSUNLOADED));

    if (auctionManager && AuctionManager::getEnableAuctionBot())
        auctionManager->reload();

    if (guildManager && GuildManager::getEnableGuildBot())
        guildManager->reload();
}

Game *Game::mInstance = nullptr;

Game::Game():
    mLastTarget(ActorSprite::UNKNOWN),
    mCurrentMap(nullptr),
    mMapName(""),
    mValidSpeed(true),
    mLastAction(0),
    mNextAdjustTime(cur_time + adjustDelay),
    mAdjustLevel(0),
    mLowerCounter(0)
{
    spellManager = new SpellManager;
    spellShortcut = new SpellShortcut;

    assert(!mInstance);
    mInstance = this;

    disconnectedDialog = nullptr;

    mAdjustPerfomance = config.getBoolValue("adjustPerfomance");

    // Create the viewport
    viewport = new Viewport;
    viewport->setSize(mainGraphics->mWidth, mainGraphics->mHeight);

    gcn::Container *top = static_cast<gcn::Container*>(gui->getTop());
    top->add(viewport);
    viewport->requestMoveToBottom();

    createGuiWindows();

    windowMenu = new WindowMenu;
//    mWindowMenu = windowMenu;

    if (windowContainer)
        windowContainer->add(windowMenu);

    initEngines();

    // Initialize beings
    if (actorSpriteManager)
        actorSpriteManager->setPlayer(player_node);

    /*
     * To prevent the server from sending data before the client
     * has initialized, I've modified it to wait for a "ping"
     * from the client to complete its initialization
     *
     * Note: This only affects the latest eAthena version.  This
     * packet is handled by the older version, but its response
     * is ignored by the client
     */
    Net::getGameHandler()->ping(tick_time);

    if (setupWindow)
        setupWindow->setInGame(true);
    clearKeysArray();

    if (guildManager && GuildManager::getEnableGuildBot())
        guildManager->requestGuildInfo();

    Event::trigger(CHANNEL_GAME, Event(EVENT_CONSTRUCTED));
}

Game::~Game()
{
    config.write();
    serverConfig.write();

    resetAdjustLevel();

//    delete mWindowMenu;
//    mWindowMenu = 0;

    destroyGuiWindows();

    del_0(actorSpriteManager)
    if (Client::getState() != STATE_CHANGE_MAP)
        del_0(player_node)
    del_0(channelManager)
    del_0(commandHandler)
    del_0(effectManager)
    del_0(particleEngine)
    del_0(viewport)
    del_0(mCurrentMap)
    del_0(spellManager)
    del_0(spellShortcut)
    del_0(auctionManager)
    del_0(guildManager)
    del_0(mumbleManager)

    Being::clearCache();

    mInstance = nullptr;

    Event::trigger(CHANNEL_GAME, Event(EVENT_DESTRUCTED));
}

static bool saveScreenshot()
{
    static unsigned int screenshotCount = 0;
    SDL_Surface *screenshot = nullptr;

    if (!config.getBoolValue("showip"))
    {
        mainGraphics->setSecure(true);
        mainGraphics->prepareScreenshot();
        gui->draw();
        screenshot = mainGraphics->getScreenshot();
        mainGraphics->setSecure(false);
    }
    else
    {
        screenshot = mainGraphics->getScreenshot();
    }

    if (!screenshot)
        return false;

    // Search for an unused screenshot name
    std::stringstream filenameSuffix;
    std::stringstream filename;
    std::fstream testExists;
    std::string screenshotDirectory = Client::getScreenshotDirectory();
    bool found = false;

    if (mkdir_r(screenshotDirectory.c_str()) != 0)
    {
        logger->log("Directory %s doesn't exist and can't be created! "
                    "Setting screenshot directory to home.",
                    screenshotDirectory.c_str());
        screenshotDirectory = std::string(PHYSFS_getUserDir());
    }

    do
    {
        screenshotCount++;
        filenameSuffix.str("");
        filename.str("");
        filename << screenshotDirectory << "/";
        filenameSuffix << branding.getValue("appName", "ManaPlus")
                       << "_Screenshot_" << screenshotCount << ".png";
        filename << filenameSuffix.str();
        testExists.open(filename.str().c_str(), std::ios::in);
        found = !testExists.is_open();
        testExists.close();
    }
    while (!found);

    const bool success = ImageWriter::writePNG(screenshot, filename.str());

    if (success)
    {
        std::stringstream chatlogentry;
        // TODO: Make it one complete gettext string below
        chatlogentry << _("Screenshot saved as ") << filenameSuffix.str();
        if (localChatTab)
            localChatTab->chatLog(chatlogentry.str(), BY_SERVER);
    }
    else
    {
        if (localChatTab)
        {
            localChatTab->chatLog(_("Saving screenshot failed!"),
                                  BY_SERVER);
        }
        logger->log1("Error: could not save screenshot.");
    }

    SDL_FreeSurface(screenshot);

    return success;
}

void Game::logic()
{
    handleInput();

    // Handle all necessary game logic
    ActorSprite::actorLogic();
    if (actorSpriteManager)
        actorSpriteManager->logic();
    if (particleEngine)
        particleEngine->update();
    if (mCurrentMap)
        mCurrentMap->update();

    cur_time = static_cast<int>(time(nullptr));
    Being::reReadConfig();
    if (killStats)
        killStats->recalcStats();
    if (shopWindow)
        shopWindow->updateTimes();
    PacketCounters::update();

    // Handle network stuff
    if (!Net::getGameHandler()->isConnected())
    {
        if (Client::getState() == STATE_CHANGE_MAP)
            return; // Not a problem here

        if (Client::getState() != STATE_ERROR)
        {
            if (!disconnectedDialog)
            {
                errorMessage = _("The connection to the server was lost.");
                disconnectedDialog = new OkDialog(_("Network Error"),
                    errorMessage, DIALOG_ERROR, false);
                disconnectedDialog->addActionListener(&errorListener);
                disconnectedDialog->requestMoveToTop();
            }
        }

        if (viewport && !errorMessage.empty())
        {
            Map *map = viewport->getCurrentMap();
            if (map)
            {
                logger->log("state: %d", Client::getState());
                map->saveExtraLayer();
            }
        }
        closeDialogs();
        Client::setFramerate(config.getIntValue("fpslimit"));
        mNextAdjustTime = cur_time + adjustDelay;
        if (Client::getState() != STATE_ERROR)
            errorMessage = "";
    }
    else
    {
        if (mAdjustPerfomance)
            adjustPerfomance();
        if (disconnectedDialog)
        {
            disconnectedDialog->scheduleDelete();
            disconnectedDialog = nullptr;
        }
    }
}

void Game::adjustPerfomance()
{
    if (mNextAdjustTime <= adjustDelay)
    {
        mNextAdjustTime = cur_time + adjustDelay;
    }
    else if (mNextAdjustTime < (unsigned)cur_time)
    {
        mNextAdjustTime = cur_time + adjustDelay;

        if (mAdjustLevel > 3 || !player_node || player_node->getHalfAway()
            || player_node->getAway())
        {
            return;
        }

        int maxFps = Client::getFramerate();
        if (maxFps != config.getIntValue("fpslimit"))
            return;

        if (!maxFps)
            maxFps = 30;
        else if (maxFps < 10)
            return;

        if (fps < maxFps - 10)
        {
            if (mLowerCounter < 2)
            {
                mLowerCounter ++;
                mNextAdjustTime = cur_time + 1;
                return;
            }
            mLowerCounter = 0;
            mAdjustLevel ++;
            switch (mAdjustLevel)
            {
                case 1:
                {
                    if (config.getBoolValue("beingopacity"))
                    {
                        config.setValue("beingopacity", false);
                        config.setSilent("beingopacity", true);
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto disable Show "
                                "beings transparency", BY_SERVER);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = cur_time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                }
                case 2:
                    if (Particle::emitterSkip < 4)
                    {
                        Particle::emitterSkip = 4;
//                        config.setValue("particleEmitterSkip", 3);
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto lower Particle "
                                "effects", BY_SERVER);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = cur_time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                case 3:
                    if (!config.getBoolValue("alphaCache"))
                    {
                        config.setValue("alphaCache", true);
                        config.setSilent("alphaCache", false);
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto enable opacity cache",
                                BY_SERVER);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::resetAdjustLevel()
{
    if (!mAdjustPerfomance)
        return;

    mNextAdjustTime = cur_time + adjustDelay;
    switch (mAdjustLevel)
    {
        case 1:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            break;
        case 2:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            Particle::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            break;
        default:
        case 3:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            Particle::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            config.setValue("alphaCache",
                config.getBoolValue("alphaCache"));
            break;
    }
    mAdjustLevel = 0;
}

bool Game::handleOutfitsKeys(SDL_Event &event, bool &used)
{
    if (keyboard.isEnabled()
        && !chatWindow->isInputFocused()
        && !setupWindow->isVisible()
        && !player_node->getAway()
        && !NpcDialog::isAnyInputFocused()
        && !InventoryWindow::isAnyInputFocused())
    {
        bool wearOutfit = false;
        bool copyOutfit = false;
        if (keyboard.isActionActive(keyboard.KEY_WEAR_OUTFIT))
            wearOutfit = true;

        if (keyboard.isActionActive(keyboard.KEY_COPY_OUTFIT))
            copyOutfit = true;

        if (wearOutfit || copyOutfit)
        {
            int outfitNum = outfitWindow->keyToNumber(
                keyboard.getKeyFromEvent(event));
            if (outfitNum >= 0)
            {
                used = true;
                if (wearOutfit)
                    outfitWindow->wearOutfit(outfitNum);
                else if (copyOutfit)
                    outfitWindow->copyOutfit(outfitNum);
            }
            else
            {
                if (keyboard.isActionActive(keyboard.KEY_MOVE_RIGHT))
                    outfitWindow->wearNextOutfit();
                else if (keyboard.isActionActive(keyboard.KEY_MOVE_LEFT))
                    outfitWindow->wearPreviousOutfit();
            }
            setValidSpeed();
            return true;
        }
        else if (keyboard.isActionActive(keyboard.KEY_MOVE_TO_POINT))
        {
            int num = outfitWindow->keyToNumber(
                keyboard.getKeyFromEvent(event));
            if (socialWindow && num >= 0)
            {
                socialWindow->selectPortal(num);
                return true;
            }
        }
    }
    return false;
}

bool Game::handleSwitchKeys(SDL_Event &event, bool &used)
{
    if ((!chatWindow || !chatWindow->isInputFocused())
        && !gui->getFocusHandler()->getModalFocused()
        && !player_node->getAway())
    {
        NpcDialog *dialog = NpcDialog::getActive();
        if (keyboard.isActionActive(keyboard.KEY_OK)
            && (!dialog || !dialog->isTextInputFocused()))
        {
            // Close the Browser if opened
            if (helpWindow->isVisible())
                helpWindow->setVisible(false);
            // Close the config window, cancelling changes if opened
            else if (setupWindow->isVisible())
            {
                setupWindow->action(gcn::ActionEvent(
                    nullptr, "cancel"));
            }
            else if (dialog)
            {
                dialog->action(gcn::ActionEvent(nullptr, "ok"));
            }
        }
        if (chatWindow && keyboard.isActionActive(
            keyboard.KEY_TOGGLE_CHAT))
        {
            if (!InventoryWindow::isAnyInputFocused())
            {
                if (chatWindow->requestChatFocus())
                    used = true;
            }
        }
        if (dialog && !dialog->isInputFocused())
        {
            if (keyboard.isActionActive(keyboard.KEY_MOVE_UP)
                || keyboard.isActionActive(keyboard.KEY_MOVE_DOWN))
            {
                dialog->refocus();
            }
        }
    }

    if (chatWindow && ((!chatWindow->isInputFocused() &&
        !NpcDialog::isAnyInputFocused() &&
        !InventoryWindow::isAnyInputFocused())
        || (event.key.keysym.mod & KMOD_ALT)))
    {
        if (keyboard.isActionActive(keyboard.KEY_PREV_CHAT_TAB))
        {
            chatWindow->prevTab();
            return true;
        }
        else if (keyboard.isActionActive(keyboard.KEY_NEXT_CHAT_TAB))
        {
            chatWindow->nextTab();
            return true;
        }
        else if (keyboard.isActionActive(keyboard.KEY_PREV_SOCIAL_TAB))
        {
            socialWindow->prevTab();
            return true;
        }
        else if (keyboard.isActionActive(keyboard.KEY_NEXT_SOCIAL_TAB))
        {
            socialWindow->nextTab();
            return true;
        }
        else if (keyboard.isActionActive(keyboard.KEY_CLOSE_CHAT_TAB))
        {
            chatWindow->closeTab();
            return true;
        }
    }

    const int tKey = keyboard.getKeyIndex(event);
    switch (tKey)
    {
        case KeyboardConfig::KEY_SCROLL_CHAT_UP:
            if (chatWindow && chatWindow->isVisible())
            {
                chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
                used = true;
            }
            break;
        case KeyboardConfig::KEY_SCROLL_CHAT_DOWN:
            if (chatWindow && chatWindow->isVisible())
            {
                chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
                used = true;
                return true;
            }
            break;
        case KeyboardConfig::KEY_WINDOW_HELP:
            // In-game Help
            if (helpWindow)
            {
                if (helpWindow->isVisible())
                {
                    helpWindow->setVisible(false);
                }
                else
                {
                    helpWindow->loadHelp("index");
                    helpWindow->requestMoveToTop();
                }
            }
            used = true;
            break;
        // Quitting confirmation dialog
        case KeyboardConfig::KEY_QUIT:
            if (!chatWindow || !chatWindow->isInputFocused())
            {
                if (viewport && viewport->isPopupMenuVisible())
                {
                    viewport->closePopupMenu();
                }
                else
                {
                    quitDialog = new QuitDialog(&quitDialog);
                    quitDialog->requestMoveToTop();
                }
                return true;
            }
            break;
        default:
            break;
    }

    if (keyboard.isEnabled()
        && (!chatWindow || !chatWindow->isInputFocused())
        && !NpcDialog::isAnyInputFocused()
        && (!player_node || !player_node->getAway()))
    {
        if (!gui->getFocusHandler()->getModalFocused()
            && mValidSpeed
            && (!setupWindow || !setupWindow->isVisible())
            && !InventoryWindow::isAnyInputFocused())
        {
            switch (tKey)
            {
                case KeyboardConfig::KEY_QUICK_DROP:
                    if (dropShortcut)
                        dropShortcut->dropFirst();
                    break;

                case KeyboardConfig::KEY_QUICK_DROPN:
                    if (dropShortcut)
                        dropShortcut->dropItems();
                    break;

                case KeyboardConfig::KEY_SWITCH_QUICK_DROP:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeQuickDropCounter();
                    }
                    break;

                case KeyboardConfig::KEY_MAGIC_INMA1:
                    if (actorSpriteManager)
                        actorSpriteManager->healTarget();
                    setValidSpeed();
                    break;

                case KeyboardConfig::KEY_MAGIC_ITENPLZ:
                    if (Net::getPlayerHandler()->canUseMagic()
                        && PlayerInfo::getAttribute(MP) >= 3)
                    {
                        actorSpriteManager->itenplz();
                    }
                    setValidSpeed();
                    break;

                case KeyboardConfig::KEY_CRAZY_MOVES:
                    if (player_node)
                        player_node->crazyMove();
                    break;

                case KeyboardConfig::KEY_CHANGE_CRAZY_MOVES_TYPE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeCrazyMoveType();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_PICKUP_TYPE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changePickUpType();
                    }
                    break;

                case KeyboardConfig::KEY_MOVE_TO_TARGET:
                    if (player_node)
                    {
                        if (!keyboard.isActionActive(
                            keyboard.KEY_TARGET_ATTACK)
                            && !keyboard.isActionActive(keyboard.KEY_ATTACK))
                        {
                            player_node->moveToTarget();
                        }
                    }
                    break;

                case KeyboardConfig::KEY_MOVE_TO_HOME:
                    if (player_node)
                    {
                        if (!keyboard.isActionActive(
                            keyboard.KEY_TARGET_ATTACK)
                            && !keyboard.isActionActive(keyboard.KEY_ATTACK))
                        {
                            player_node->moveToHome();
                        }
                        setValidSpeed();
                    }
                    break;

                case KeyboardConfig::KEY_SET_HOME:
                    if (player_node)
                        player_node->setHome();
                    break;

                case KeyboardConfig::KEY_INVERT_DIRECTION:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->invertDirection();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_ATTACK_WEAPON_TYPE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeAttackWeaponType();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_ATTACK_TYPE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeAttackType();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_FOLLOW_MODE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeFollowMode();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_IMITATION_MODE:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeImitationMode();
                    }
                    break;

                case KeyboardConfig::KEY_MAGIC_ATTACK:
                    if (player_node)
                        player_node->magicAttack();
                    break;

                case KeyboardConfig::KEY_SWITCH_MAGIC_ATTACK:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->switchMagicAttack();
                    }
                    break;

                case KeyboardConfig::KEY_SWITCH_PVP_ATTACK:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->switchPvpAttack();
                    }
                    break;

                case KeyboardConfig::KEY_CHANGE_MOVE_TO_TARGET:
                    if (player_node)
                    {
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeMoveToTargetType();
                    }
                    break;

                case KeyboardConfig::KEY_COPY_EQUIPED_OUTFIT:
                    if (outfitWindow)
                        outfitWindow->copyFromEquiped();
                    break;

                case KeyboardConfig::KEY_DISABLE_GAME_MODIFIERS:
                    if (player_node)
                        player_node->switchGameModifiers();
                    break;

                case KeyboardConfig::KEY_CHANGE_AUDIO:
                    sound.changeAudio();
                    break;

                case KeyboardConfig::KEY_AWAY:
                    if (player_node)
                    {
                        player_node->changeAwayMode();
                        player_node->updateStatus();
                        setValidSpeed();
                    }
                    break;

                case KeyboardConfig::KEY_CAMERA:
                    if (player_node && viewport)
                    {
                        if (!player_node->getDisableGameModifiers())
                            viewport->toggleCameraMode();
                        setValidSpeed();
                    }
                    break;

                default:
                    break;
            }
        }

        gcn::Window *requestedWindow = nullptr;

        if (!NpcDialog::isAnyInputFocused()
            && !InventoryWindow::isAnyInputFocused()
            && !keyboard.isActionActive(keyboard.KEY_TARGET)
            && !keyboard.isActionActive(keyboard.KEY_UNTARGET))
        {
            if (setupWindow && setupWindow->isVisible())
            {
                if (tKey == KeyboardConfig::KEY_WINDOW_SETUP)
                {
                    setupWindow->doCancel();
                    used = true;
                }
            }
            else
            {
                // Do not activate shortcuts if tradewindow is visible
                if (itemShortcutWindow && tradeWindow
                    && !tradeWindow->isVisible()
                    && !setupWindow->isVisible())
                {
                    int num = itemShortcutWindow->getTabIndex();
                    if (num >= 0 && num < SHORTCUT_TABS)
                    {
                        // Checks if any item shortcut is pressed.
                        for (int i = KeyboardConfig::KEY_SHORTCUT_1;
                              i <= KeyboardConfig::KEY_SHORTCUT_20;
                              i ++)
                        {
                            if (tKey == i && !used)
                            {
                                itemShortcut[num]->useItem(
                                    i - KeyboardConfig::KEY_SHORTCUT_1);
                                break;
                            }
                        }
                    }
                }

                switch (tKey)
                {
                    case KeyboardConfig::KEY_PICKUP:
                        if (player_node)
                            player_node->pickUpItems();
                        used = true;
                        break;
                    case KeyboardConfig::KEY_SIT:
                        // Player sit action
                        if (player_node)
                        {
                            if (keyboard.isActionActive(keyboard.KEY_EMOTE))
                                player_node->updateSit();
                            else
                                player_node->toggleSit();
                        }
                        used = true;
                        break;
                    case KeyboardConfig::KEY_HIDE_WINDOWS:
                        // Hide certain windows
                        if (!chatWindow || !chatWindow->isInputFocused())
                        {
                            if (statusWindow)
                                statusWindow->setVisible(false);
                            if (inventoryWindow)
                                inventoryWindow->setVisible(false);
                            if (shopWindow)
                                shopWindow->setVisible(false);
                            if (skillDialog)
                                skillDialog->setVisible(false);
                            if (setupWindow)
                                setupWindow->setVisible(false);
                            if (equipmentWindow)
                                equipmentWindow->setVisible(false);
                            if (helpWindow)
                                helpWindow->setVisible(false);
                            if (debugWindow)
                                debugWindow->setVisible(false);
                            if (outfitWindow)
                                outfitWindow->setVisible(false);
                            if (dropShortcutWindow)
                                dropShortcutWindow->setVisible(false);
                            if (spellShortcutWindow)
                                spellShortcutWindow->setVisible(false);
                            if (botCheckerWindow)
                                botCheckerWindow->setVisible(false);
                            if (socialWindow)
                                socialWindow->setVisible(false);
                        }
                        break;
                    case KeyboardConfig::KEY_WINDOW_STATUS:
                        requestedWindow = statusWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_INVENTORY:
                        requestedWindow = inventoryWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SHOP:
                        requestedWindow = shopWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_EQUIPMENT:
                        requestedWindow = equipmentWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SKILL:
                        requestedWindow = skillDialog;
                        break;
                    case KeyboardConfig::KEY_WINDOW_KILLS:
                        requestedWindow = killStats;
                        break;
                    case KeyboardConfig::KEY_WINDOW_MINIMAP:
                        minimap->toggle();
                        break;
                    case KeyboardConfig::KEY_WINDOW_CHAT:
                        requestedWindow = chatWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SHORTCUT:
                        requestedWindow = itemShortcutWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SETUP:
                        requestedWindow = setupWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_DEBUG:
                        requestedWindow = debugWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SOCIAL:
                        requestedWindow = socialWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_EMOTE_SHORTCUT:
                        requestedWindow = emoteShortcutWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_OUTFIT:
                        requestedWindow = outfitWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_DROP:
                        requestedWindow = dropShortcutWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_SPELLS:
                        requestedWindow = spellShortcutWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_BOT_CHECKER:
                        requestedWindow = botCheckerWindow;
                        break;
                    case KeyboardConfig::KEY_WINDOW_ONLINE:
                        requestedWindow = whoIsOnline;
                        break;
                    case KeyboardConfig::KEY_SCREENSHOT:
                        // Screenshot (picture, hence the p)
                        saveScreenshot();
                        used = true;
                        break;
                    case KeyboardConfig::KEY_PATHFIND:
                        // Find path to mouse (debug purpose)
                        if (!player_node || !player_node->
                            getDisableGameModifiers())
                        {
                            if (viewport)
                                viewport->toggleDebugPath();
                            if (miniStatusWindow)
                                miniStatusWindow->updateStatus();
                            if (mCurrentMap)
                                mCurrentMap->redrawMap();
                            used = true;
                        }
                        break;
                    case KeyboardConfig::KEY_TRADE:
                    {
                        // Toggle accepting of incoming trade requests
                        unsigned int deflt = player_relations.getDefault();
                        if (deflt & PlayerRelation::TRADE)
                        {
                            if (localChatTab)
                            {
                                localChatTab->chatLog(
                                    _("Ignoring incoming trade requests"),
                                    BY_SERVER);
                            }
                            deflt &= ~PlayerRelation::TRADE;
                        }
                        else
                        {
                            if (localChatTab)
                            {
                                localChatTab->chatLog(
                                    _("Accepting incoming trade requests"),
                                    BY_SERVER);
                            }
                            deflt |= PlayerRelation::TRADE;
                        }

                        player_relations.setDefault(deflt);

                        used = true;
                    }
                        break;
                    default:
                        break;
                }
            }

            if (requestedWindow)
            {
                requestedWindow->setVisible(!requestedWindow->isVisible());
                if (requestedWindow->isVisible())
                    requestedWindow->requestMoveToTop();
                used = true;
            }
        }
    }
    return false;
}

void Game::handleMoveAndAttack(SDL_Event &event, bool wasDown)
{
    // Moving player around
    if (player_node->isAlive() && (!Being::isTalking()
        || keyboard.getKeyIndex(event)
        == KeyboardConfig::KEY_TALK)
        && chatWindow && !chatWindow->isInputFocused()
        && !InventoryWindow::isAnyInputFocused() && !quitDialog)
    {
        // Get the state of the keyboard keys
        keyboard.refreshActiveKeys();

        // Ignore input if either "ignore" key is pressed
        // Stops the character moving about if the user's window manager
        // uses "ignore+arrow key" to switch virtual desktops.
        if (keyboard.isActionActive(keyboard.KEY_IGNORE_INPUT_1) ||
            keyboard.isActionActive(keyboard.KEY_IGNORE_INPUT_2))
        {
            return;
        }

        unsigned char direction = 0;

        // Translate pressed keys to movement and direction
        if (keyboard.isActionActive(keyboard.KEY_MOVE_UP) ||
            (joystick && joystick->isUp()))
        {
            direction |= Being::UP;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (keyboard.isActionActive(keyboard.KEY_MOVE_DOWN) ||
                 (joystick && joystick->isDown()))
        {
            direction |= Being::DOWN;
            setValidSpeed();
            player_node->cancelFollow();
        }

        if (keyboard.isActionActive(keyboard.KEY_MOVE_LEFT) ||
            (joystick && joystick->isLeft()))
        {
            direction |= Being::LEFT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (keyboard.isActionActive(keyboard.KEY_MOVE_RIGHT) ||
                 (joystick && joystick->isRight()))
        {
            direction |= Being::RIGHT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (!keyboard.isActionActive(keyboard.KEY_EMOTE))
        {
            if (keyboard.isActionActive(keyboard.KEY_DIRECT_UP))
            {
                if (player_node->getDirection() != Being::UP)
                {
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        player_node->setDirection(Being::UP);
                        Net::getPlayerHandler()->setDirection(Being::UP);
                    }
                }
            }
            else if (keyboard.isActionActive(keyboard.KEY_DIRECT_DOWN))
            {
                if (player_node->getDirection() != Being::DOWN)
                {
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        player_node->setDirection(Being::DOWN);
                        Net::getPlayerHandler()->setDirection(Being::DOWN);
                    }
                }
            }
            else if (keyboard.isActionActive(keyboard.KEY_DIRECT_LEFT))
            {
                if (player_node->getDirection() != Being::LEFT)
                {
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        player_node->setDirection(Being::LEFT);
                        Net::getPlayerHandler()->setDirection(Being::LEFT);
                    }
                }
            }
            else if (keyboard.isActionActive(keyboard.KEY_DIRECT_RIGHT))
            {
                if (player_node->getDirection() != Being::RIGHT)
                {
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        player_node->setDirection(Being::RIGHT);
                        Net::getPlayerHandler()->setDirection(Being::RIGHT);
                    }
                }
            }
        }

        if (keyboard.isActionActive(keyboard.KEY_EMOTE) && direction != 0)
        {
            if (player_node->getDirection() != direction)
            {
                if (Client::limitPackets(PACKET_DIRECTION))
                {
                    player_node->setDirection(direction);
                    Net::getPlayerHandler()->setDirection(direction);
                }
            }
//            direction = 0;
        }
        else
        {
            if (!viewport->getCameraMode())
            {
                player_node->specialMove(direction);
            }
            else
            {
                int dx = 0;
                int dy = 0;
                if (direction & Being::LEFT)
                    dx = -5;
                else if (direction & Being::RIGHT)
                    dx = 5;

                if (direction & Being::UP)
                    dy = -5;
                else if (direction & Being::DOWN)
                    dy = 5;
                viewport->moveCamera(dx, dy);
            }
        }

        bool joyAttack(false);
        if (joystick && joystick->buttonPressed(0))
            joyAttack = true;

        if ((((player_node->getAttackType() == 0
            && player_node->getFollow().empty()) || wasDown)
            || joyAttack) && mValidSpeed)
        {
            // Attacking monsters
            if (keyboard.isActionActive(keyboard.KEY_ATTACK))
            {
                if (player_node->getTarget())
                    player_node->attack(player_node->getTarget(), true);
            }

            if ((keyboard.isActionActive(keyboard.KEY_TARGET_ATTACK)
                || joyAttack)
                && !keyboard.isActionActive(keyboard.KEY_MOVE_TO_TARGET))
            {
                Being *target = nullptr;

                bool newTarget = !keyboard.isActionActive(keyboard.KEY_TARGET);
                // A set target has highest priority
                if (!player_node->getTarget())
                {
                    // Only auto target Monsters
                    target = actorSpriteManager->findNearestLivingBeing(
                            player_node, 90, ActorSprite::MONSTER);
                }
                else
                {
                    target = player_node->getTarget();
                }

                player_node->attack2(target, newTarget);
            }
        }

        if (!keyboard.isActionActive(keyboard.KEY_EMOTE))
        {
            // Target the nearest player/monster/npc
            if ((keyboard.isActionActive(keyboard.KEY_TARGET_PLAYER) ||
                keyboard.isActionActive(keyboard.KEY_TARGET_CLOSEST) ||
                keyboard.isActionActive(keyboard.KEY_TARGET_NPC) ||
                (joystick && joystick->buttonPressed(3))) &&
                !keyboard.isActionActive(keyboard.KEY_TARGET) &&
                !keyboard.isActionActive(keyboard.KEY_UNTARGET))
            {
                ActorSprite::Type currentTarget = ActorSprite::UNKNOWN;
                if (keyboard.isActionActive(keyboard.KEY_TARGET_CLOSEST) ||
                    (joystick && joystick->buttonPressed(3)))
                {
                    currentTarget = ActorSprite::MONSTER;
                }
                else if (keyboard.isActionActive(keyboard.KEY_TARGET_PLAYER))
                {
                    currentTarget = ActorSprite::PLAYER;
                }
                else if (keyboard.isActionActive(keyboard.KEY_TARGET_NPC))
                {
                    currentTarget = ActorSprite::NPC;
                }

                Being *target = actorSpriteManager->findNearestLivingBeing(
                    player_node, 20, currentTarget);

                if (target && (target != player_node->getTarget() ||
                    currentTarget != mLastTarget))
                {
                    player_node->setTarget(target);
                    mLastTarget = currentTarget;
                }
            }
            else
            {
                mLastTarget = ActorSprite::UNKNOWN; // Reset last target
            }
        }

        // Talk to the nearest NPC if 't' pressed
        if (wasDown && keyboard.getKeyIndex(event) == KeyboardConfig::KEY_TALK
            && !keyboard.isActionActive(keyboard.KEY_EMOTE))
        {
            Being *target = player_node->getTarget();

            if (target)
            {
                if (target->canTalk())
                    target->talkTo();
                else if (target->getType() == Being::PLAYER)
                    new BuySellDialog(target->getName());
            }
        }

        // Stop attacking if the right key is pressed
        if (!keyboard.isActionActive(keyboard.KEY_ATTACK)
            && !keyboard.isActionActive(keyboard.KEY_EMOTE))
        {
            if (keyboard.isActionActive(keyboard.KEY_TARGET)
                || (joystick && joystick->buttonPressed(4)))
            {
                player_node->stopAttack();
            }
            else if (keyboard.isActionActive(keyboard.KEY_UNTARGET))
            {
                player_node->untarget();
            }
        }

        if (joystick)
        {
            if (joystick->buttonPressed(1))
                player_node->pickUpItems();
            else if (joystick->buttonPressed(2))
                player_node->toggleSit();
        }
    }
}

void Game::handleActive(SDL_Event &event)
{
//    logger->log("SDL_ACTIVEEVENT");
//    logger->log("state: %d", (int)event.active.state);
//    logger->log("gain: %d", (int)event.active.gain);

    int fpsLimit = 0;
    if (event.active.state & SDL_APPACTIVE)
    {
        if (event.active.gain)
        {   // window restore
            Client::setIsMinimized(false);
            if (!player_node && !player_node->getAway())
                fpsLimit = config.getIntValue("fpslimit");
            if (player_node)
                player_node->setHalfAway(false);
        }
        else
        {   // window minimisation
            Client::setIsMinimized(true);
            if (player_node && !player_node->getAway())
            {
                fpsLimit = config.getIntValue("altfpslimit");
                player_node->setHalfAway(true);
            }
        }
        if (player_node)
            player_node->updateStatus();
    }
    if (player_node)
        player_node->updateName();

    if (event.active.state & SDL_APPINPUTFOCUS)
        Client::setInputFocused(event.active.gain);
    if (event.active.state & SDL_APPMOUSEFOCUS)
        Client::setMouseFocused(event.active.gain);

    if (!fpsLimit)
    {
        if (player_node && player_node->getAway())
        {
            if (Client::getInputFocused() || Client::getMouseFocused())
                fpsLimit = config.getIntValue("fpslimit");
            else
                fpsLimit = config.getIntValue("altfpslimit");
        }
        else
        {
            fpsLimit = config.getIntValue("fpslimit");
        }
    }
    Client::setFramerate(fpsLimit);
    mNextAdjustTime = cur_time + adjustDelay;
}

/**
 * The huge input handling method.
 */
void Game::handleInput()
{
    if (joystick)
        joystick->update();

    bool wasDown(false);
    // Events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        bool used = false;

        updateHistory(event);
        checkKeys();

        if (event.type == SDL_VIDEORESIZE)
        {
            // Let the client deal with this one (it'll pass down from there)
            Client::resize(event.resize.w, event.resize.h);
        }
        // Keyboard events (for discontinuous keys)
        else if (event.type == SDL_KEYDOWN)
        {
            wasDown = true;

            logger->log("key. sym=%d, scancode=%d", event.key.keysym.sym, event.key.keysym.scancode);

            if (setupWindow && setupWindow->isVisible() &&
                keyboard.getNewKeyIndex() > keyboard.KEY_NO_VALUE)
            {
                keyboard.setNewKey(event);
                keyboard.callbackNewKey();
                keyboard.setNewKeyIndex(keyboard.KEY_NO_VALUE);
                return;
            }

            // send straight to gui for certain windows
            if (quitDialog || TextDialog::isActive() ||
                NpcPostDialog::isActive())
            {
                try
                {
                    guiInput->pushInput(event);
                }
                catch (const gcn::Exception &e)
                {
                    const char* err = e.getMessage().c_str();
                    logger->log("Warning: guichan input exception: %s", err);
                }
                return;
            }

            if (chatWindow && !chatWindow->isInputFocused()
                && keyboard.isActionActive(keyboard.KEY_RIGHT_CLICK))
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                gcn::MouseEvent event2(viewport, false, false, false, false,
                        0, gcn::MouseEvent::RIGHT, mouseX, mouseY, 1);
                if (viewport)
                    viewport->mousePressed(event2);
                continue;
            }

            // Mode switch to emotes
            if (keyboard.isActionActive(keyboard.KEY_EMOTE))
            {
                // Emotions
                int emotion = keyboard.getKeyEmoteOffset(event);
                if (emotion)
                {
                    if (emoteShortcut)
                        emoteShortcut->useEmote(emotion);
//                    used = true;
                    setValidSpeed();
                    return;
                }
            }

            if (handleOutfitsKeys(event, used))
                continue;

            if (handleSwitchKeys(event, used))
                return;

        }
        // Active event
        else if (event.type == SDL_ACTIVEEVENT)
        {
            handleActive(event);
        }
        // Quit event
        else if (event.type == SDL_QUIT)
        {
            Client::setState(STATE_EXIT);
        }

        // Push input to GUI when not used
        if (!used)
        {
            try
            {
                if (guiInput)
                    guiInput->pushInput(event);
            }
            catch (const gcn::Exception &e)
            {
                const char *err = e.getMessage().c_str();
                logger->log("Warning: guichan input exception: %s", err);
            }
        }

    } // End while

    // If the user is configuring the keys then don't respond.
    if (!player_node || !keyboard.isEnabled() || player_node->getAway())
        return;

    // If pressed outfits keys, stop processing keys.
    if (keyboard.isActionActive(keyboard.KEY_WEAR_OUTFIT)
        || keyboard.isActionActive(keyboard.KEY_COPY_OUTFIT)
        || (setupWindow && setupWindow->isVisible()))
    {
        return;
    }

    handleMoveAndAttack(event, wasDown);
}

/**
 * Changes the currently active map. Should only be called while the game is
 * running.
 */
void Game::changeMap(const std::string &mapPath)
{
    resetAdjustLevel();

    // Clean up floor items, beings and particles
    if (actorSpriteManager)
        actorSpriteManager->clear();

    // Close the popup menu on map change so that invalid options can't be
    // executed.
    if (viewport)
    {
        viewport->closePopupMenu();
        viewport->cleanHoverItems();
    }

    // Unset the map of the player so that its particles are cleared before
    // being deleted in the next step
    if (player_node)
        player_node->setMap(nullptr);

    if (particleEngine)
        particleEngine->clear();

    mMapName = mapPath;

    std::string fullMap = paths.getValue("maps", "maps/")
                          + mMapName + ".tmx";
    std::string realFullMap = paths.getValue("maps", "maps/")
                              + MapDB::getMapName(mMapName) + ".tmx";

    ResourceManager *resman = ResourceManager::getInstance();
    if (!resman->exists(realFullMap))
        realFullMap += ".gz";

    // Attempt to load the new map
    Map *newMap = MapReader::readMap(fullMap, realFullMap);

    if (!newMap)
    {
        logger->log("Error while loading %s", fullMap.c_str());
        new OkDialog(_("Could Not Load Map"), strprintf(
            _("Error while loading %s"), fullMap.c_str()),
            DIALOG_ERROR, false);
    }

    if (mCurrentMap)
        mCurrentMap->saveExtraLayer();

    if (newMap)
        newMap->addExtraLayer();

    if (socialWindow)
        socialWindow->setMap(newMap);

    // Notify the minimap and actorSpriteManager about the map change
    if (minimap)
        minimap->setMap(newMap);
    if (actorSpriteManager)
        actorSpriteManager->setMap(newMap);
    if (particleEngine)
        particleEngine->setMap(newMap);
    if (viewport)
        viewport->setMap(newMap);

    // Initialize map-based particle effects
    if (newMap)
        newMap->initializeParticleEffects(particleEngine);

    // Start playing new music file when necessary
    std::string oldMusic = mCurrentMap ? mCurrentMap->getMusicFile() : "";
    std::string newMusic = newMap ? newMap->getMusicFile() : "";
    if (newMusic != oldMusic)
    {
        if (newMusic.empty())
            sound.fadeOutMusic();
        else
            sound.fadeOutAndPlayMusic(newMusic);
    }

    if (mCurrentMap)
        mCurrentMap->saveExtraLayer();

    delete mCurrentMap;
    mCurrentMap = newMap;
//    mCurrentMap = 0;

    if (mumbleManager)
        mumbleManager->setMap(mapPath);
    Event event(EVENT_MAPLOADED);
    event.setString("mapPath", mapPath);
    Event::trigger(CHANNEL_GAME, event);
}

void Game::updateHistory(SDL_Event &event)
{
    if (!player_node || !player_node->getAttackType())
        return;

    bool old = false;

    if (event.key.keysym.sym != -1)
    {
        int key = keyboard.getKeyIndex(event);
        int time = cur_time;
        int idx = -1;
        for (int f = 0; f < MAX_LASTKEYS; f ++)
        {
            if (mLastKeys[f].key == key)
            {
                idx = f;
                old = true;
                break;
            }
            else if (idx >= 0 && mLastKeys[f].time < mLastKeys[idx].time)
                idx = f;
        }
        if (idx < 0)
        {
            idx = 0;
            for (int f = 0; f < MAX_LASTKEYS; f ++)
            {
                if (mLastKeys[f].key == -1
                    ||  mLastKeys[f].time < mLastKeys[idx].time)
                {
                    idx = f;
                }
            }
        }

        if (idx < 0)
            idx = 0;

        if (!old)
        {
            mLastKeys[idx].time = time;
            mLastKeys[idx].key = key;
            mLastKeys[idx].cnt = 0;
        }
        else
        {
            mLastKeys[idx].cnt++;
        }
    }
}

void Game::checkKeys()
{
    const int timeRange = 120;
    const int cntInTime = 130;

    if (!player_node || !player_node->getAttackType())
        return;

    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        if (mLastKeys[f].key != -1)
        {
            if (mLastKeys[f].time + timeRange < cur_time)
            {
                if (mLastKeys[f].cnt > cntInTime)
                    mValidSpeed = false;
                mLastKeys[f].key = -1;
            }
        }
    }
}

void Game::setValidSpeed()
{
    clearKeysArray();
    mValidSpeed = true;
}

void Game::clearKeysArray()
{
    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        mLastKeys[f].time = 0;
        mLastKeys[f].key = -1;
        mLastKeys[f].cnt = 0;
    }
}

void Game::closeDialogs()
{
    Client::closeDialogs();
    if (deathNotice)
    {
        deathNotice->scheduleDelete();
        deathNotice = nullptr;
    }
}

void Game::videoResized(int width, int height)
{
    if (viewport)
        viewport->setSize(width, height);
    if (windowMenu)
        windowMenu->setPosition(width - 3 - windowMenu->getWidth(), 3);
}
