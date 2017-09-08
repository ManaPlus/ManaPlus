/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/windowmanager.h"

#include "client.h"
#include "configuration.h"
#include "game.h"
#include "main.h"
#include "settings.h"
#include "spellmanager.h"

#include "fs/virtfs/tools.h"

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/shortcut/spellshortcut.h"

#include "gui/popups/textpopup.h"

#ifdef DYECMD
#include "render/graphics.h"
#else  // DYECMD
#include "gui/popups/beingpopup.h"
#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/skillpopup.h"
#include "gui/popups/spellpopup.h"
#include "gui/popups/textboxpopup.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/questswindow.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "utils/gettext.h"
#endif  // DYECMD

#include "gui/widgets/createwidget.h"
#include "gui/widgets/desktop.h"

#include "input/touch/touchmanager.h"

#include "utils/delete2.h"
#include "utils/sdlcheckutils.h"
#include "utils/sdlhelper.h"
#ifdef __native_client__
#include "utils/naclmessages.h"
#endif  // __native_client__

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif  // USE_SDL2
#endif  // ANDROID

#ifdef USE_SDL2
#include <SDL2_framerate.h>
#else  // USE_SDL2
#include <SDL_framerate.h>
#endif  // USE_SDL2

#include <SDL_image.h>

#ifdef WIN32
#include <SDL_syswm.h>
#endif  // WIN32
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

FPSmanager fpsManager;

namespace
{
    SDL_Surface *mIcon(nullptr);
#ifndef USE_SDL2
    int mKeyboardHeight(0);
#endif  // USE_SDL2

    bool mIsMinimized(false);
    bool mNewMessageFlag(false);
}  // namespace

void WindowManager::init()
{
    // Initialize frame limiting
    fpsManager.framecount = 0;
    fpsManager.rateticks = 0;
    fpsManager.lastticks = 0;
    fpsManager.rate = 0;
}

void WindowManager::createWindows()
{
    userPalette = new UserPalette;
#ifndef DYECMD
    CREATEWIDGETV0(setupWindow, SetupWindow);
    CREATEWIDGETV0(helpWindow, HelpWindow);
    CREATEWIDGETV0(didYouKnowWindow, DidYouKnowWindow);
    CREATEWIDGETV0(popupMenu, PopupMenu);
    CREATEWIDGETV0(beingPopup, BeingPopup);
    CREATEWIDGETV0(textBoxPopup, TextBoxPopup);
    CREATEWIDGETV0(itemPopup, ItemPopup);
    CREATEWIDGETV0(spellPopup, SpellPopup);
    CREATEWIDGETV0(skillPopup, SkillPopup);
    delete2(debugChatTab);
    if (chatWindow != nullptr)
    {
        chatWindow->scheduleDelete();
        chatWindow = nullptr;
    }
    CREATEWIDGETV(chatWindow, ChatWindow,
        "DebugChat");
    // TRANSLATORS: chat tab header
    debugChatTab = new ChatTab(chatWindow, _("Debug"), "",
        "#Debug", ChatTabType::DEBUG);
    debugChatTab->setAllowHighlight(false);
    chatWindow->setVisible(Visible_false);
    if (debugWindow != nullptr)
    {
        debugWindow->scheduleDelete();
        debugWindow = nullptr;
    }
    CREATEWIDGETV(debugWindow, DebugWindow,
        "DebugDebug");
#endif  // DYECMD

    CREATEWIDGETV0(textPopup, TextPopup);
}

void WindowManager::createValidateWindows()
{
#ifndef DYECMD
    CREATEWIDGETV0(skillDialog, SkillDialog);
    skillDialog->loadSkills();
    CREATEWIDGETV(beingEquipmentWindow, EquipmentWindow,
        nullptr, nullptr, true);
    CREATEWIDGETV0(questsWindow, QuestsWindow);
#endif  // DYECMD
}

void WindowManager::deleteValidateWindows()
{
#ifndef DYECMD
    delete2(skillDialog);
    delete2(beingEquipmentWindow);
    delete2(questsWindow);
#endif  // DYECMD
}

void WindowManager::deleteWindows()
{
#ifndef DYECMD
    delete2(textBoxPopup);
    delete2(beingPopup);
    delete2(itemPopup);
    delete2(spellPopup);
    delete2(skillPopup);
    delete2(popupMenu);
    delete2(didYouKnowWindow);
    delete2(helpWindow);
    delete2(setupWindow);
    delete2(userPalette);
    delete2(spellManager)
    delete2(spellShortcut)

    delete2(debugChatTab);
    delete2(chatWindow);
    delete2(debugWindow);
#endif  // DYECMD

    delete2(textPopup);
}

void WindowManager::initTitle()
{
    if (settings.options.test.empty())
    {
        settings.windowCaption = strprintf("%s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION);
    }
    else
    {
        settings.windowCaption = strprintf(
            "Please wait - VIDEO MODE TEST - %s %s - Please wait",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION);
    }

    SDL::SetWindowTitle(mainGraphics->getWindow(),
        settings.windowCaption.c_str());
#ifndef WIN32
    setIcon();
#endif  // WIN32
}

void WindowManager::updateTitle()
{
    std::string str;
    if (settings.login.empty())
        str = settings.serverName;
    else
        str.append(settings.login).append(" ").append(settings.serverName);
    if (str.empty())
    {
        settings.windowCaption = strprintf("%s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION);
    }
    else
    {
        settings.windowCaption = strprintf("%s %s - %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION,
            str.c_str());
    }
    SDL::SetWindowTitle(mainGraphics->getWindow(),
        settings.windowCaption.c_str());
}

void WindowManager::setFramerate(const unsigned int fpsLimit)
{
    if (fpsLimit == 0u)
        return;

    if (!settings.limitFps)
        return;

    SDL_setFramerate(&fpsManager, fpsLimit);
}

int WindowManager::getFramerate()
{
    if (!settings.limitFps)
        return 0;

    return SDL_getFramerate(&fpsManager);
}

void WindowManager::doResizeVideo(const int actualWidth,
                                  const int actualHeight,
                                  const bool always)
{
    if (!always
        && mainGraphics->mActualWidth == actualWidth
        && mainGraphics->mActualHeight == actualHeight)
    {
        return;
    }

#ifdef __native_client__
    naclPostMessage("resize-window",
        strprintf("%d,%d", actualWidth, actualHeight));
#else  // __native_client__

    resizeVideo(actualWidth, actualHeight, always);
#endif  // __native_client__
}

void WindowManager::resizeVideo(int actualWidth,
                                int actualHeight,
                                const bool always)
{
    // Keep a minimum size. This isn't adhered to by the actual window, but
    // it keeps some window positions from getting messed up.
    actualWidth = std::max(470, actualWidth);
    actualHeight = std::max(320, actualHeight);

    if (mainGraphics == nullptr)
        return;
    if (!always
        && mainGraphics->mActualWidth == actualWidth
        && mainGraphics->mActualHeight == actualHeight)
    {
        return;
    }

    if (mainGraphics->resizeScreen(actualWidth, actualHeight))
    {
        const int width = mainGraphics->mWidth;
        const int height = mainGraphics->mHeight;
        touchManager.resize(width, height);

        if (gui != nullptr)
            gui->videoResized();

        if (desktop != nullptr)
            desktop->setSize(width, height);

        client->moveButtons(width);

#ifndef DYECMD
        Game *const game = Game::instance();
        if (game != nullptr)
            Game::videoResized(width, height);
#endif  // DYECMD

        if (gui != nullptr)
            gui->draw();

        config.setValue("screenwidth", actualWidth);
        config.setValue("screenheight", actualHeight);
    }
}

bool WindowManager::setFullScreen(const bool fs)
{
#ifdef __native_client__
    naclPostMessage("set-fullscreen",
        fs ? "on" : "off");
    return true;
#else  // __native_client__

    if (mainGraphics == nullptr)
        return false;
    return mainGraphics->setFullscreen(fs);
#endif  // __native_client__
}

void WindowManager::applyGrabMode()
{
    SDL::grabInput(mainGraphics->getWindow(),
        config.getBoolValue("grabinput"));
}

void WindowManager::applyGamma()
{
    if (config.getBoolValue("enableGamma"))
    {
        SDL::setGamma(mainGraphics->getWindow(),
            config.getFloatValue("gamma"));
    }
}

void WindowManager::applyVSync()
{
    const int val = config.getIntValue("vsync");
    if (val > 0 && val < 2)
        SDL::setVsync(val);
}

void WindowManager::applyKeyRepeat()
{
#ifndef USE_SDL2
    SDL_EnableKeyRepeat(config.getIntValue("repeateDelay"),
        config.getIntValue("repeateInterval"));
#endif  // USE_SDL2
}

void WindowManager::applyScale()
{
    const int scale = config.getIntValue("scale");
    if (mainGraphics->getScale() == scale)
        return;
    mainGraphics->setScale(scale);
    doResizeVideo(mainGraphics->mActualWidth,
        mainGraphics->mActualHeight,
        true);
}

void WindowManager::setIsMinimized(const bool n)
{
    mIsMinimized = n;
    if (!n && mNewMessageFlag)
    {
        mNewMessageFlag = false;
        SDL::SetWindowTitle(mainGraphics->getWindow(),
            settings.windowCaption.c_str());
    }
}

void WindowManager::newChatMessage()
{
    if (!mNewMessageFlag && mIsMinimized)
    {
        // show * on window caption
        SDL::SetWindowTitle(mainGraphics->getWindow(),
            ("*" + settings.windowCaption).c_str());
        mNewMessageFlag = true;
    }
}

void WindowManager::setIcon()
{
    std::string iconFile = branding.getValue("appIcon", "icons/manaplus");
#ifdef WIN32
    iconFile.append(".ico");
#else  // WIN32

    iconFile.append(".png");
#endif  // WIN32

    iconFile = VirtFs::getPath(iconFile);
    logger->log("Loading icon from file: %s", iconFile.c_str());

#ifdef WIN32
    static SDL_SysWMinfo pInfo;
    if (mainGraphics)
        SDL::getWindowWMInfo(mainGraphics->getWindow(), &pInfo);
    else
        SDL::getWindowWMInfo(nullptr, &pInfo);
    // Attempt to load icon from .ico file
    HICON icon = static_cast<HICON>(LoadImage(nullptr,
        iconFile.c_str(),
        IMAGE_ICON,
        64, 64,
        LR_LOADFROMFILE));
    // If it's failing, we load the default resource file.
    if (!icon)
    {
        logger->log("icon load error");
        icon = LoadIcon(GetModuleHandle(nullptr), "A");
    }
    if (icon)
    {
#ifdef WIN64
        SetClassLongPtr(pInfo.window,
            GCLP_HICON,
            reinterpret_cast<LONG_PTR>(icon));
#else  // WIN64
        SetClassLong(pInfo.window,
            GCL_HICON,
            reinterpret_cast<LONG>(icon));
#endif  // WIN64
    }
#else  // WIN32

    mIcon = MIMG_Load(iconFile.c_str());
    if (mIcon != nullptr)
    {
#ifdef USE_SDL2
        SDL_SetSurfaceAlphaMod(mIcon, SDL_ALPHA_OPAQUE);
#else  // USE_SDL2

        SDL_SetAlpha(mIcon, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
#endif  // USE_SDL2

        SDL::SetWindowIcon(mainGraphics->getWindow(), mIcon);
    }
#endif  // WIN32
}

bool WindowManager::isKeyboardVisible()
{
#ifdef USE_SDL2
    return SDL_GetEventState(SDL_TEXTINPUT) == SDL_ENABLE;
#else  // USE_SDL2

    return mKeyboardHeight > 1;
#endif  // USE_SDL2
}

bool WindowManager::getIsMinimized()
{
    return mIsMinimized;
}

#ifndef USE_SDL2
void WindowManager::updateScreenKeyboard(const int height)
{
    mKeyboardHeight = height;
}
#endif  // USE_SDL2

void WindowManager::deleteIcon()
{
    MSDL_FreeSurface(mIcon);
    mIcon = nullptr;
}
