/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "actions/windows.h"

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "render/graphics.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/tradehandler.h"

#include "listeners/updatestatuslistener.h"

#include "resources/map/map.h"

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;
extern std::string tradePartnerName;
extern QuitDialog *quitDialog;

namespace Actions
{

impHandler0(setupWindowShow)
{
    if (setupWindow)
    {
        if (setupWindow->isWindowVisible())
        {
            setupWindow->doCancel();
        }
        else
        {
            setupWindow->setVisible(true);
            setupWindow->requestMoveToTop();
        }
        return true;
    }
    return false;
}

impHandler0(hideWindows)
{
    if (setupWindow)
        setupWindow->hideWindows();
    return true;
}

static bool showHelpPage(const std::string &page, const bool showHide)
{
    if (helpWindow)
    {
        if (showHide && helpWindow->isWindowVisible())
        {
            helpWindow->setVisible(false);
        }
        else
        {
            helpWindow->loadHelp(page);
            helpWindow->requestMoveToTop();
        }
        return true;
    }
    return false;
}

impHandler0(helpWindowShow)
{
    return showHelpPage("index", true);
}

impHandler0(aboutWindowShow)
{
    return showHelpPage("about", false);
}

static void showHideWindow(Window *const window)
{
    if (window)
    {
        window->setVisible(!window->isWindowVisible());
        if (window->isWindowVisible())
            window->requestMoveToTop();
    }
}

impHandler0(statusWindowShow)
{
    showHideWindow(statusWindow);
    return true;
}

impHandler0(inventoryWindowShow)
{
    showHideWindow(inventoryWindow);
    return true;
}

impHandler0(equipmentWindowShow)
{
    showHideWindow(equipmentWindow);
    return true;
}

impHandler0(skillDialogShow)
{
    showHideWindow(skillDialog);
    return true;
}

impHandler0(minimapWindowShow)
{
    if (minimap)
    {
        minimap->toggle();
        return true;
    }
    return false;
}

impHandler0(chatWindowShow)
{
    showHideWindow(chatWindow);
    return true;
}

impHandler0(shortcutWindowShow)
{
    showHideWindow(itemShortcutWindow);
    return true;
}

impHandler0(debugWindowShow)
{
    showHideWindow(debugWindow);
    return true;
}

impHandler0(socialWindowShow)
{
    showHideWindow(socialWindow);
    return true;
}

impHandler0(emoteShortcutWindowShow)
{
    showHideWindow(emoteShortcutWindow);
    return true;
}

impHandler0(outfitWindowShow)
{
    showHideWindow(outfitWindow);
    return true;
}

impHandler0(shopWindowShow)
{
    showHideWindow(shopWindow);
    return true;
}

impHandler0(dropShortcutWindowShow)
{
    showHideWindow(dropShortcutWindow);
    return true;
}

impHandler0(killStatsWindowShow)
{
    showHideWindow(killStats);
    return true;
}

impHandler0(spellShortcutWindowShow)
{
    showHideWindow(spellShortcutWindow);
    return true;
}

impHandler0(botcheckerWindowShow)
{
    showHideWindow(botCheckerWindow);
    return true;
}

impHandler0(whoIsOnlineWindowShow)
{
    showHideWindow(whoIsOnline);
    return true;
}

impHandler0(didYouKnowWindowShow)
{
    showHideWindow(didYouKnowWindow);
    return true;
}

impHandler0(questsWindowShow)
{
    showHideWindow(questsWindow);
    return true;
}

impHandler0(updaterWindowShow)
{
    if (updaterWindow)
        updaterWindow->deleteSelf();
    else
        DialogsManager::createUpdaterWindow();
    return true;
}

}  // namespace Actions
