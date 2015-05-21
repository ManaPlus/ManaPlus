/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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
#include "settings.h"
#include "spellshortcut.h"
#include "touchmanager.h"

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/spellpopup.h"
#include "gui/popups/textboxpopup.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/desktop.h"

#include "utils/delete2.h"
#include "utils/files.h"
#include "utils/sdlcheckutils.h"
#include "utils/sdlhelper.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#ifdef USE_SDL2
#include <SDL2_framerate.h>
#else
#include <SDL_framerate.h>
#endif

#include <SDL_image.h>

#ifdef WIN32
#include <SDL_syswm.h>
#endif

#include "debug.h"

FPSmanager fpsManager;

namespace
{
    SDL_Surface *mIcon(nullptr);
#ifndef USE_SDL2
    int mKeyboardHeight(0);
#endif
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
    setupWindow = new SetupWindow;
    setupWindow->postInit();
    helpWindow = new HelpWindow;
    helpWindow->postInit();
    didYouKnowWindow = new DidYouKnowWindow;
    didYouKnowWindow->postInit();
    popupMenu = new PopupMenu;
    popupMenu->postInit();
    beingPopup = new BeingPopup;
    beingPopup->postInit();
    textPopup = new TextPopup;
    textPopup->postInit();
    textBoxPopup = new TextBoxPopup;
    textBoxPopup->postInit();
    itemPopup = new ItemPopup;
    itemPopup->postInit();
    spellPopup = new SpellPopup;
    spellPopup->postInit();
}

void WindowManager::deleteWindows()
{
    delete2(textBoxPopup);
    delete2(textPopup);
    delete2(beingPopup);
    delete2(itemPopup);
    delete2(spellPopup);
    delete2(popupMenu);
    delete2(didYouKnowWindow);
    delete2(helpWindow);
    delete2(setupWindow);
    delete2(userPalette);
    delete2(spellManager)
    delete2(spellShortcut)
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
#endif
}

void WindowManager::setFramerate(const int fpsLimit)
{
    if (!fpsLimit)
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

void WindowManager::resizeVideo(int actualWidth,
                                int actualHeight,
                                const bool always)
{
    // Keep a minimum size. This isn't adhered to by the actual window, but
    // it keeps some window positions from getting messed up.
    actualWidth = std::max(470, actualWidth);
    actualHeight = std::max(320, actualHeight);

    if (!mainGraphics)
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

        if (gui)
            gui->videoResized();

        if (desktop)
            desktop->setSize(width, height);

        client->moveButtons(width);

        Game *const game = Game::instance();
        if (game)
            game->videoResized(width, height);

        if (gui)
            gui->draw();

        config.setValue("screenwidth", actualWidth);
        config.setValue("screenheight", actualHeight);
    }
}

void WindowManager::applyGrabMode()
{
    SDL::grabInput(mainGraphics->getWindow(),
        config.getBoolValue("grabinput"));
}

void WindowManager::applyGamma()
{
    if (config.getFloatValue("enableGamma"))
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
#endif
}

void WindowManager::applyScale()
{
    const int scale = config.getIntValue("scale");
    if (mainGraphics->getScale() == scale)
        return;
    mainGraphics->setScale(scale);
    resizeVideo(mainGraphics->mActualWidth,
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
#else
    iconFile.append(".png");
#endif
    iconFile = Files::getPath(iconFile);
    logger->log("Loading icon from file: %s", iconFile.c_str());

#ifdef WIN32
    static SDL_SysWMinfo pInfo;
    if (mainGraphics)
        SDL::getWindowWMInfo(mainGraphics->getWindow(), &pInfo);
    else
        SDL::getWindowWMInfo(nullptr, &pInfo);
    // Attempt to load icon from .ico file
    HICON icon = (HICON) LoadImage(nullptr, iconFile.c_str(),
        IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
    // If it's failing, we load the default resource file.
    if (!icon)
    {
        logger->log("icon load error");
        icon = LoadIcon(GetModuleHandle(nullptr), "A");
    }
    if (icon)
        SetClassLong(pInfo.window, GCL_HICON, reinterpret_cast<LONG>(icon));
#else
    mIcon = MIMG_Load(iconFile.c_str());
    if (mIcon)
    {
#ifdef USE_SDL2
        SDL_SetSurfaceAlphaMod(mIcon, SDL_ALPHA_OPAQUE);
#else
        SDL_SetAlpha(mIcon, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
#endif
        SDL::SetWindowIcon(mainGraphics->getWindow(), mIcon);
    }
#endif
}

bool WindowManager::isKeyboardVisible()
{
#ifdef USE_SDL2
    return SDL_IsTextInputActive();
#else
    return mKeyboardHeight > 1;
#endif
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
#endif

void WindowManager::deleteIcon()
{
    MSDL_FreeSurface(mIcon);
}
