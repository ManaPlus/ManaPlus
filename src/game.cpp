/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "game.h"

#include "gui/npcdialog.h"

#include "actorspritemanager.h"
#include "actorsprite.h"
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

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include <physfs.h>

#include <fstream>
#include <sstream>
#include <string>

#include "mumblemanager.h"

#include "debug.h"

Joystick *joystick = NULL;

OkDialog *weightNotice = NULL;
int weightNoticeTime = 0;
OkDialog *deathNotice = NULL;
QuitDialog *quitDialog = NULL;
OkDialog *disconnectedDialog = NULL;

ChatWindow *chatWindow = NULL;
StatusWindow *statusWindow = NULL;
MiniStatusWindow *miniStatusWindow = NULL;
InventoryWindow *inventoryWindow = NULL;
ShopWindow *shopWindow = NULL;
SkillDialog *skillDialog = NULL;
Minimap *minimap = NULL;
EquipmentWindow *equipmentWindow = NULL;
EquipmentWindow *beingEquipmentWindow = NULL;
TradeWindow *tradeWindow = NULL;
HelpWindow *helpWindow = NULL;
DebugWindow *debugWindow = NULL;
ShortcutWindow *itemShortcutWindow = NULL;
ShortcutWindow *emoteShortcutWindow = NULL;
OutfitWindow *outfitWindow = NULL;
SpecialsWindow *specialsWindow = NULL;
ShortcutWindow *dropShortcutWindow = NULL;
ShortcutWindow *spellShortcutWindow = NULL;
WhoIsOnline *whoIsOnline = NULL;
DidYouKnowWindow *didYouKnowWindow = NULL;
KillStats *killStats = NULL;
BotCheckerWindow *botCheckerWindow = NULL;
SocialWindow *socialWindow = NULL;
WindowMenu *windowMenu = NULL;

ActorSpriteManager *actorSpriteManager = NULL;
ChannelManager *channelManager = NULL;
CommandHandler *commandHandler = NULL;
MumbleManager *mumbleManager = NULL;
Particle *particleEngine = NULL;
EffectManager *effectManager = NULL;
SpellManager *spellManager = NULL;
Viewport *viewport = NULL;                    /**< Viewport on the map. */
GuildManager *guildManager = NULL;

ChatTab *localChatTab = NULL;
ChatTab *debugChatTab = NULL;
TradeTab *tradeChatTab = NULL;
BattleTab *battleChatTab = NULL;

const unsigned adjustDelay = 10;

/**
 * Initialize every game sub-engines in the right order
 */
static void initEngines()
{
    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_ENGINESINITALIZING));

    actorSpriteManager = new ActorSpriteManager;
    commandHandler = new CommandHandler;
    channelManager = new ChannelManager;
    effectManager = new EffectManager;
    if (!guildManager)
        guildManager = new GuildManager;

    particleEngine = new Particle(NULL);
    particleEngine->setupEngine();

    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_ENGINESINITALIZED));
}

/**
 * Create all the various globally accessible gui windows
 */
static void createGuiWindows()
{
    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_GUIWINDOWSLOADING));

    if (setupWindow)
        setupWindow->clearWindowsForReset();

    if (emoteShortcut)
        emoteShortcut->load();

    // Create dialogs
    chatWindow = new ChatWindow;
    tradeWindow = new TradeWindow;
    equipmentWindow = new EquipmentWindow(PlayerInfo::getEquipment(),
        player_node);
    beingEquipmentWindow = new EquipmentWindow(0, 0, true);
    beingEquipmentWindow->setVisible(false);
    statusWindow = new StatusWindow;
    miniStatusWindow = new MiniStatusWindow;
    inventoryWindow = new InventoryWindow(PlayerInfo::getInventory());
    shopWindow = new ShopWindow;
    skillDialog = new SkillDialog;
    minimap = new Minimap;
    helpWindow = new HelpWindow;
    debugWindow = new DebugWindow;
    itemShortcutWindow = new ShortcutWindow("ItemShortcut");
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
                                             new EmoteShortcutContainer);
    outfitWindow = new OutfitWindow();
    specialsWindow = new SpecialsWindow();
    dropShortcutWindow = new ShortcutWindow("DropShortcut",
                                            new DropShortcutContainer);

    spellShortcutWindow = new ShortcutWindow("SpellShortcut",
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
        tradeChatTab = 0;
    }

    if (config.getBoolValue("enableBattleTab"))
    {
        battleChatTab = new BattleTab;
        battleChatTab->setAllowHighlight(false);
    }
    else
    {
        battleChatTab = 0;
    }

    if (config.getBoolValue("logToChat"))
        logger->setChatWindow(chatWindow);

    if (!isSafeMode && chatWindow)
        chatWindow->loadState();

    if (setupWindow)
        setupWindow->externalUpdate();

    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_GUIWINDOWSLOADED));
}

#define del_0(X) { delete X; X = 0; }

/**
 * Destroy all the globally accessible gui windows
 */
static void destroyGuiWindows()
{
    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_GUIWINDOWSUNLOADING));

    logger->setChatWindow(NULL);
    if (whoIsOnline)
        whoIsOnline->setAllowUpdate(false);

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

    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_GUIWINDOWSUNLOADED));
}

Game *Game::mInstance = 0;

Game::Game():
    mLastTarget(ActorSprite::UNKNOWN),
    mCurrentMap(0),
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

    disconnectedDialog = NULL;

    mAdjustPerfomance = config.getBoolValue("adjustPerfomance");

    // Create the viewport
    viewport = new Viewport;
    viewport->setDimension(gcn::Rectangle(0, 0, graphics->mWidth,
                                          graphics->mHeight));

    gcn::Container *top = static_cast<gcn::Container*>(gui->getTop());
    top->add(viewport);
    viewport->requestMoveToBottom();

    createGuiWindows();

    windowMenu = new WindowMenu;
//    mWindowMenu = windowMenu;

    windowContainer->add(windowMenu);

    initEngines();

    // Initialize beings
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

    Joystick::init();
    // TODO: The user should be able to choose which one to use
    // Open the first device
    if (Joystick::getNumberOfJoysticks() > 0)
        joystick = new Joystick(0);

    if (setupWindow)
        setupWindow->setInGame(true);
    clearKeysArray();

    if (guildManager && guildManager->getEnableGuildBot())
        guildManager->requestGuildInfo();

    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_CONSTRUCTED));
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
    del_0(guildManager)
    del_0(channelManager)
    del_0(commandHandler)
    del_0(effectManager)
    del_0(joystick)
    del_0(particleEngine)
    del_0(viewport)
    del_0(mCurrentMap)
    del_0(spellManager)
    del_0(spellShortcut)
    del_0(mumbleManager)

    Being::clearCache();

    mInstance = 0;

    Mana::Event::trigger(CHANNEL_GAME, Mana::Event(EVENT_DESTRUCTED));
}

static bool saveScreenshot()
{
    static unsigned int screenshotCount = 0;
    SDL_Surface *screenshot = 0;

    if (!config.getBoolValue("showip"))
    {
        graphics->setSecure(true);
        gui->draw();
        screenshot = graphics->getScreenshot();
        graphics->setSecure(false);
    }
    else
    {
        screenshot = graphics->getScreenshot();
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
        filenameSuffix << branding.getValue("appShort", "ManaPlus")
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

    cur_time = static_cast<int>(time(0));
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
            errorMessage = _("The connection to the server was lost.");


        if (!disconnectedDialog)
        {
            if (viewport)
            {
                Map *map = viewport->getCurrentMap();
                if (map)
                    map->saveExtraLayer();
            }
            Client::closeDialogs();
            disconnectedDialog = new OkDialog(_("Network Error"),
                                              errorMessage, false);
            disconnectedDialog->addActionListener(&errorListener);
            disconnectedDialog->requestMoveToTop();
        }
    }
    else
    {
        if (mAdjustPerfomance)
            adjustPerfomance();
        if (disconnectedDialog)
        {
            disconnectedDialog->scheduleDelete();
            disconnectedDialog = 0;
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

        int maxFps = config.getIntValue("fpslimit");
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

/**
 * The huge input handling method.
 */
void Game::handleInput()
{
    if (joystick)
        joystick->update();

    // Events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        bool used = false;

        updateHistory(event);
        checkKeys();

        // Keyboard events (for discontinuous keys)
        if (event.type == SDL_KEYDOWN)
        {
            gcn::Window *requestedWindow = NULL;

            if (setupWindow && setupWindow->isVisible() &&
                keyboard.getNewKeyIndex() > keyboard.KEY_NO_VALUE)
            {
                keyboard.setNewKey(static_cast<int>(event.key.keysym.sym));
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
                && keyboard.isKeyActive(keyboard.KEY_RIGHT_CLICK))
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
            if (keyboard.isKeyActive(keyboard.KEY_EMOTE))
            {
                // Emotions
                int emotion = keyboard.getKeyEmoteOffset(event.key.keysym.sym);
                if (emotion)
                {
                    if (emoteShortcut)
                        emoteShortcut->useEmote(emotion);
                    used = true;
                    setValidSpeed();
                    return;
                }
            }

            if (keyboard.isEnabled()
                && !chatWindow->isInputFocused()
                && !setupWindow->isVisible()
                && !player_node->getAwayMode()
                && !NpcDialog::isAnyInputFocused())
            {
                bool wearOutfit = false;
                bool copyOutfit = false;
                if (keyboard.isKeyActive(keyboard.KEY_WEAR_OUTFIT))
                    wearOutfit = true;

                if (keyboard.isKeyActive(keyboard.KEY_COPY_OUTFIT))
                    copyOutfit = true;

                if (wearOutfit || copyOutfit)
                {
                    int outfitNum = outfitWindow->keyToNumber(
                        event.key.keysym.sym);
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
                        if (keyboard.isKeyActive(keyboard.KEY_MOVE_RIGHT))
                            outfitWindow->wearNextOutfit();
                        else if (keyboard.isKeyActive(keyboard.KEY_MOVE_LEFT))
                            outfitWindow->wearPreviousOutfit();
                    }
                    setValidSpeed();
                    continue;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_MOVE_TO_POINT))
                {
                    int num = outfitWindow->keyToNumber(
                        event.key.keysym.sym);
                    if (socialWindow && num >= 0)
                    {
                        socialWindow->selectPortal(num);
                        continue;
                    }
                }
            }

            if (!chatWindow->isInputFocused()
                && !gui->getFocusHandler()->getModalFocused()
                && !player_node->getAwayMode())
            {
                NpcDialog *dialog = NpcDialog::getActive();
                if (keyboard.isKeyActive(keyboard.KEY_OK)
                    && (!dialog || !dialog->isTextInputFocused()))
                {
                    // Close the Browser if opened
                    if (helpWindow->isVisible())
                        helpWindow->setVisible(false);
                    // Close the config window, cancelling changes if opened
                    else if (setupWindow->isVisible())
                        setupWindow->action(gcn::ActionEvent(NULL, "cancel"));
                    else if (dialog)
                        dialog->action(gcn::ActionEvent(NULL, "ok"));
                }
                if (keyboard.isKeyActive(keyboard.KEY_TOGGLE_CHAT))
                {
                    if (chatWindow->requestChatFocus())
                        used = true;
                }
                if (dialog)
                {
                    if (keyboard.isKeyActive(keyboard.KEY_MOVE_UP))
                        dialog->move(1);
                    else if (keyboard.isKeyActive(keyboard.KEY_MOVE_DOWN))
                        dialog->move(-1);
                }
            }

            if ((!chatWindow->isInputFocused() &&
                !NpcDialog::isAnyInputFocused())
                || (event.key.keysym.mod & KMOD_ALT))
            {
                if (keyboard.isKeyActive(keyboard.KEY_PREV_CHAT_TAB))
                {
                    chatWindow->prevTab();
                    return;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_NEXT_CHAT_TAB))
                {
                    chatWindow->nextTab();
                    return;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_PREV_SOCIAL_TAB))
                {
                    socialWindow->prevTab();
                    return;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_NEXT_SOCIAL_TAB))
                {
                    socialWindow->nextTab();
                    return;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_CLOSE_CHAT_TAB))
                {
                    chatWindow->closeTab();
                    return;
                }
            }

            const int tKey = keyboard.getKeyIndex(event.key.keysym.sym);
            switch (tKey)
            {
                case KeyboardConfig::KEY_SCROLL_CHAT_UP:
                    if (chatWindow->isVisible())
                    {
                        chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
                        used = true;
                    }
                    break;
                case KeyboardConfig::KEY_SCROLL_CHAT_DOWN:
                    if (chatWindow->isVisible())
                    {
                        chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
                        used = true;
                        return;
                    }
                    break;
                case KeyboardConfig::KEY_WINDOW_HELP:
                    // In-game Help
                    if (helpWindow->isVisible())
                        helpWindow->setVisible(false);
                    else
                    {
                        helpWindow->loadHelp("index");
                        helpWindow->requestMoveToTop();
                    }
                    used = true;
                    break;



               // Quitting confirmation dialog
               case KeyboardConfig::KEY_QUIT:
                    if (!chatWindow->isInputFocused())
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
                        return;
                    }
                    break;
                default:
                    break;
            }

            if (keyboard.isEnabled() && !chatWindow->isInputFocused()
                && !gui->getFocusHandler()->getModalFocused()
                && mValidSpeed
                && !setupWindow->isVisible()
                && !player_node->getAwayMode()
                && !NpcDialog::isAnyInputFocused())
            {
                switch (tKey)
                {
                    case KeyboardConfig::KEY_QUICK_DROP:
                        dropShortcut->dropFirst();
                        break;

                    case KeyboardConfig::KEY_QUICK_DROPN:
                        dropShortcut->dropItems();
                        break;

                    case KeyboardConfig::KEY_SWITCH_QUICK_DROP:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeQuickDropCounter();
                        break;

                    case KeyboardConfig::KEY_MAGIC_INMA1:
                        actorSpriteManager->healTarget(player_node);
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
                        player_node->crazyMove();
                        break;

                    case KeyboardConfig::KEY_CHANGE_CRAZY_MOVES_TYPE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeCrazyMoveType();
                        break;

                    case KeyboardConfig::KEY_CHANGE_PICKUP_TYPE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changePickUpType();
                        break;

                    case KeyboardConfig::KEY_MOVE_TO_TARGET:
                        if (!keyboard.isKeyActive(keyboard.KEY_TARGET_ATTACK)
                            && !keyboard.isKeyActive(keyboard.KEY_ATTACK))
                        {
                            player_node->moveToTarget();
                        }
                        break;

                    case KeyboardConfig::KEY_MOVE_TO_HOME:
                        if (!keyboard.isKeyActive(keyboard.KEY_TARGET_ATTACK)
                            && !keyboard.isKeyActive(keyboard.KEY_ATTACK))
                        {
                            player_node->moveToHome();
                        }
                        setValidSpeed();
                        break;

                    case KeyboardConfig::KEY_SET_HOME:
                        player_node->setHome();
                        break;

                    case KeyboardConfig::KEY_INVERT_DIRECTION:
                        if (!player_node->getDisableGameModifiers())
                            player_node->invertDirection();
                        break;

                    case KeyboardConfig::KEY_CHANGE_ATTACK_WEAPON_TYPE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeAttackWeaponType();
                        break;

                    case KeyboardConfig::KEY_CHANGE_ATTACK_TYPE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeAttackType();
                        break;

                    case KeyboardConfig::KEY_CHANGE_FOLLOW_MODE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeFollowMode();
                        break;

                    case KeyboardConfig::KEY_CHANGE_IMITATION_MODE:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeImitationMode();
                        break;

                    case KeyboardConfig::KEY_MAGIC_ATTACK:
                        player_node->magicAttack();
                        break;

                    case KeyboardConfig::KEY_SWITCH_MAGIC_ATTACK:
                        if (!player_node->getDisableGameModifiers())
                            player_node->switchMagicAttack();
                        break;

                    case KeyboardConfig::KEY_CHANGE_MOVE_TO_TARGET:
                        if (!player_node->getDisableGameModifiers())
                            player_node->changeMoveToTargetType();
                        break;

                    case KeyboardConfig::KEY_COPY_EQUIPED_OUTFIT:
                        outfitWindow->copyFromEquiped();
                        break;

                    case KeyboardConfig::KEY_DISABLE_GAME_MODIFIERS:
                        player_node->switchGameModifiers();
                        break;

                    case KeyboardConfig::KEY_CHANGE_AUDIO:
                        sound.changeAudio();
                        break;

                    case KeyboardConfig::KEY_AWAY:
                        player_node->changeAwayMode();
                        setValidSpeed();
                        break;

                    case KeyboardConfig::KEY_CAMERA:
                        if (!player_node->getDisableGameModifiers())
                            viewport->toggleCameraMode();
                        setValidSpeed();
                        break;

                    default:
                        break;
                }
            }

            if (keyboard.isEnabled()
                && !chatWindow->isInputFocused()
                && !NpcDialog::isAnyInputFocused()
                && !player_node->getAwayMode()
                && !keyboard.isKeyActive(keyboard.KEY_TARGET))
            {
                const int tKey = keyboard.getKeyIndex(event.key.keysym.sym);

                if (setupWindow->isVisible())
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
                    if (itemShortcutWindow && !tradeWindow->isVisible()
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
                            player_node->pickUpItems();
                            used = true;
                            break;
                        case KeyboardConfig::KEY_SIT:
                            // Player sit action
                            if (keyboard.isKeyActive(keyboard.KEY_EMOTE))
                                player_node->updateSit();
                            else
                                player_node->toggleSit();
                            used = true;
                            break;
                        case KeyboardConfig::KEY_HIDE_WINDOWS:
                            // Hide certain windows
                            if (!chatWindow->isInputFocused())
                            {
                                statusWindow->setVisible(false);
                                inventoryWindow->setVisible(false);
                                shopWindow->setVisible(false);
                                skillDialog->setVisible(false);
                                setupWindow->setVisible(false);
                                equipmentWindow->setVisible(false);
                                helpWindow->setVisible(false);
                                debugWindow->setVisible(false);
                                outfitWindow->setVisible(false);
                                dropShortcutWindow->setVisible(false);
                                spellShortcutWindow->setVisible(false);
                                botCheckerWindow->setVisible(false);
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
                            if (!player_node->getDisableGameModifiers())
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
                                localChatTab->chatLog(
                                    _("Ignoring incoming trade requests"),
                                    BY_SERVER);
                                deflt &= ~PlayerRelation::TRADE;
                            }
                            else
                            {
                                localChatTab->chatLog(
                                    _("Accepting incoming trade requests"),
                                    BY_SERVER);
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
        // Active event
        else if (event.type == SDL_ACTIVEEVENT)
        {
//            logger->log("SDL_ACTIVEEVENT");
//            logger->log("state: %d", (int)event.active.state);
//            logger->log("gain: %d", (int)event.active.gain);

            int fpsLimit = 0;
            if (event.active.state & SDL_APPACTIVE)
            {
                if (event.active.gain)
                {   // window restore
                    Client::setIsMinimized(false);
                    if (player_node && !player_node->getAwayMode())
                    {
                        fpsLimit = config.getIntValue("fpslimit");
                        player_node->setHalfAway(false);
                    }
                }
                else
                {   // window minimisation
                    Client::setIsMinimized(true);
                    if (player_node && !player_node->getAwayMode())
                    {
                        fpsLimit = config.getIntValue("altfpslimit");
                        player_node->setHalfAway(true);
                    }
                }
                Client::setFramerate(fpsLimit);
            }
            if (player_node)
                player_node->updateName();

            if (event.active.state & SDL_APPINPUTFOCUS)
                Client::setInputFocused(event.active.gain);
            if (event.active.state & SDL_APPMOUSEFOCUS)
                Client::setMouseFocused(event.active.gain);

            if (player_node->getAwayMode())
            {
                if (Client::getInputFocused() || Client::getMouseFocused())
                    fpsLimit = config.getIntValue("fpslimit");
                else
                    fpsLimit = config.getIntValue("altfpslimit");
                Client::setFramerate(fpsLimit);
            }
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
    if (!keyboard.isEnabled() || player_node->getAwayMode())
        return;

    if (keyboard.isKeyActive(keyboard.KEY_WEAR_OUTFIT)
        || keyboard.isKeyActive(keyboard.KEY_COPY_OUTFIT)
        || setupWindow->isVisible())
    {
        return;
    }

    // Moving player around
    if (player_node->isAlive() && (!Being::isTalking()
        || keyboard.getKeyIndex(event.key.keysym.sym)
        == KeyboardConfig::KEY_TALK)
        && !chatWindow->isInputFocused() && !quitDialog)
    {
        // Get the state of the keyboard keys
        keyboard.refreshActiveKeys();

        // Ignore input if either "ignore" key is pressed
        // Stops the character moving about if the user's window manager
        // uses "ignore+arrow key" to switch virtual desktops.
        if (keyboard.isKeyActive(keyboard.KEY_IGNORE_INPUT_1) ||
            keyboard.isKeyActive(keyboard.KEY_IGNORE_INPUT_2))
        {
            return;
        }

        unsigned char direction = 0;

        // Translate pressed keys to movement and direction
        if (keyboard.isKeyActive(keyboard.KEY_MOVE_UP) ||
            (joystick && joystick->isUp()))
        {
            direction |= Being::UP;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (keyboard.isKeyActive(keyboard.KEY_MOVE_DOWN) ||
                 (joystick && joystick->isDown()))
        {
            direction |= Being::DOWN;
            setValidSpeed();
            player_node->cancelFollow();
        }

        if (keyboard.isKeyActive(keyboard.KEY_MOVE_LEFT) ||
            (joystick && joystick->isLeft()))
        {
            direction |= Being::LEFT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (keyboard.isKeyActive(keyboard.KEY_MOVE_RIGHT) ||
                 (joystick && joystick->isRight()))
        {
            direction |= Being::RIGHT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (!keyboard.isKeyActive(keyboard.KEY_EMOTE))
        {
            if (keyboard.isKeyActive(keyboard.KEY_DIRECT_UP))
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
            else if (keyboard.isKeyActive(keyboard.KEY_DIRECT_DOWN))
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
            else if (keyboard.isKeyActive(keyboard.KEY_DIRECT_LEFT))
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
            else if (keyboard.isKeyActive(keyboard.KEY_DIRECT_RIGHT))
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

        if (keyboard.isKeyActive(keyboard.KEY_EMOTE) && direction != 0)
        {
            if (player_node->getDirection() != direction)
            {
                if (Client::limitPackets(PACKET_DIRECTION))
                {
                    player_node->setDirection(direction);
                    Net::getPlayerHandler()->setDirection(direction);
                }
            }
            direction = 0;
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

        if (((player_node->getAttackType() == 0
            && player_node->getFollow().empty()) || event.type == SDL_KEYDOWN)
            && mValidSpeed)
        {
            // Attacking monsters
            if (keyboard.isKeyActive(keyboard.KEY_ATTACK) ||
                (joystick && joystick->buttonPressed(0)))
            {
                if (player_node->getTarget())
                    player_node->attack(player_node->getTarget(), true);
            }

            if (keyboard.isKeyActive(keyboard.KEY_TARGET_ATTACK)
                && !keyboard.isKeyActive(keyboard.KEY_MOVE_TO_TARGET))
            {
                Being *target = 0;

                bool newTarget = !keyboard.isKeyActive(keyboard.KEY_TARGET);
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

        if (!keyboard.isKeyActive(keyboard.KEY_EMOTE))
        {
            // Target the nearest player/monster/npc
            if ((keyboard.isKeyActive(keyboard.KEY_TARGET_PLAYER) ||
                keyboard.isKeyActive(keyboard.KEY_TARGET_CLOSEST) ||
                keyboard.isKeyActive(keyboard.KEY_TARGET_NPC) ||
                (joystick && joystick->buttonPressed(3))) &&
                !keyboard.isKeyActive(keyboard.KEY_TARGET))
            {
                ActorSprite::Type currentTarget = ActorSprite::UNKNOWN;
                if (keyboard.isKeyActive(keyboard.KEY_TARGET_CLOSEST) ||
                    (joystick && joystick->buttonPressed(3)))
                {
                    currentTarget = ActorSprite::MONSTER;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_TARGET_PLAYER))
                {
                    currentTarget = ActorSprite::PLAYER;
                }
                else if (keyboard.isKeyActive(keyboard.KEY_TARGET_NPC))
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
        if (event.type == SDL_KEYDOWN &&
            keyboard.getKeyIndex(event.key.keysym.sym)
            == KeyboardConfig::KEY_TALK &&
            !keyboard.isKeyActive(keyboard.KEY_EMOTE))
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
        if (!keyboard.isKeyActive(keyboard.KEY_ATTACK)
            && keyboard.isKeyActive(keyboard.KEY_TARGET)
            && !keyboard.isKeyActive(keyboard.KEY_EMOTE))
        {
            player_node->stopAttack();
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

/**
 * Changes the currently active map. Should only be called while the game is
 * running.
 */
void Game::changeMap(const std::string &mapPath)
{
    resetAdjustLevel();

    // Clean up floor items, beings and particles
    actorSpriteManager->clear();

    // Close the popup menu on map change so that invalid options can't be
    // executed.
    viewport->closePopupMenu();
    viewport->cleanHoverItems();

    // Unset the map of the player so that its particles are cleared before
    // being deleted in the next step
    if (player_node)
        player_node->setMap(0);

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
        new OkDialog(_("Could Not Load Map"),
                     strprintf(_("Error while loading %s"), fullMap.c_str()));
    }

    if (mCurrentMap)
        mCurrentMap->saveExtraLayer();

    if (newMap)
        newMap->addExtraLayer();

    if (socialWindow)
        socialWindow->setMap(newMap);

    // Notify the minimap and actorSpriteManager about the map change
    minimap->setMap(newMap);
    actorSpriteManager->setMap(newMap);
    particleEngine->setMap(newMap);
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
    Mana::Event event(EVENT_MAPLOADED);
    event.setString("mapPath", mapPath);
    Mana::Event::trigger(CHANNEL_GAME, event);
}

void Game::updateHistory(SDL_Event &event)
{
    if (!player_node->getAttackType())
        return;

    bool old = false;

    if (event.key.keysym.sym != -1)
    {
        int key = keyboard.getKeyIndex(event.key.keysym.sym);
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

    if (!player_node->getAttackType())
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
