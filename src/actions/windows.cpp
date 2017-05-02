/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "actions/actiondef.h"

#include "being/localplayer.h"

#include "gui/dialogsmanager.h"

#include "gui/windows/bankwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/mailwindow.h"
#include "gui/windows/minimap.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "utils/gettext.h"

#include "net/net.h"

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;

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
            setupWindow->setVisible(Visible_true);
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
            helpWindow->setVisible(Visible_false);
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

impHandler(helpWindowShow)
{
    if (!chatWindow || !chatWindow->isInputFocused())
        return showHelpPage("index", true);
    if (!event.tab)
        return showHelpPage("chatcommands", true);
    switch (event.tab->getType())
    {
        case ChatTabType::PARTY:
            return showHelpPage("chatparty", true);
        case ChatTabType::GUILD:
            return showHelpPage("chatguild", true);
        case ChatTabType::WHISPER:
            return showHelpPage("chatwhisper", true);
        case ChatTabType::DEBUG:
            return showHelpPage("chatdebug", true);
        case ChatTabType::TRADE:
            return showHelpPage("chattrade", true);
        case ChatTabType::BATTLE:
            return showHelpPage("chatbattle", true);
        case ChatTabType::LANG:
            return showHelpPage("chatlang", true);
        case ChatTabType::GM:
            return showHelpPage("chatgm", true);
        case ChatTabType::CHANNEL:
            return showHelpPage("chatchannel", true);
        default:
        case ChatTabType::UNKNOWN:
        case ChatTabType::INPUT:
            return showHelpPage("chatcommands", true);
    }
}

impHandler0(aboutWindowShow)
{
    return showHelpPage("about", false);
}

static void showHideWindow(Window *const window)
{
    if (window)
    {
        window->setVisible(fromBool(
            !window->isWindowVisible(), Visible));
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

impHandler0(bankWindowShow)
{
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() == ServerType::TMWATHENA)
        return false;
#endif  // TMWA_SUPPORT

    showHideWindow(bankWindow);
    return true;
}

impHandler0(cartWindowShow)
{
    if (Net::getNetworkType() == ServerType::TMWATHENA ||
        !localPlayer ||
        !localPlayer->getHaveCart())
    {
        return false;
    }

    showHideWindow(cartWindow);
    if (inventoryWindow)
        inventoryWindow->updateDropButton();
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

impHandler0(quickWindowShow)
{
    if (setupWindow)
    {
        if (setupWindow->isWindowVisible())
            setupWindow->doCancel();
        setupWindow->setVisible(Visible_true);
        // TRANSLATORS: settings tab name
        setupWindow->activateTab(_("Quick"));
        setupWindow->requestMoveToTop();
        return true;
    }
    return false;
}

impHandler0(mailWindowShow)
{
    showHideWindow(mailWindow);
    return true;
}

}  // namespace Actions
