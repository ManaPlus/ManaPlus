/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/windows/setupwindow.h"

#include "configuration.h"
#include "game.h"
#include "main.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/statuswindow.h"

#include "gui/widgets/createwidget.h"

#include "gui/widgets/tabs/setup_audio.h"
#include "gui/widgets/tabs/setup_chat.h"
#include "gui/widgets/tabs/setup_colors.h"
#include "gui/widgets/tabs/setup_input.h"
#include "gui/widgets/tabs/setup_joystick.h"
#include "gui/widgets/tabs/setup_misc.h"
#include "gui/widgets/tabs/setup_mods.h"
#include "gui/widgets/tabs/setup_perfomance.h"
#include "gui/widgets/tabs/setup_players.h"
#include "gui/widgets/tabs/setup_quick.h"
#include "gui/widgets/tabs/setup_relations.h"
#include "gui/widgets/tabs/setup_theme.h"
#include "gui/widgets/tabs/setup_touch.h"
#include "gui/widgets/tabs/setup_video.h"
#include "gui/widgets/tabs/setup_visual.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/tabbedarea.h"

#include "input/touch/touchmanager.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

SetupWindow *setupWindow = nullptr;

SetupWindow::SetupWindow() :
    // TRANSLATORS: setup window name
    Window(_("Setup"), Modal_false, nullptr, "setup.xml"),
    ActionListener(),
    mTabs(),
    mWindowsToReset(),
    mButtons(),
    mModsTab(nullptr),
    mQuickTab(nullptr),
    mResetWindows(nullptr),
    mPanel(CREATEWIDGETR(TabbedArea, this)),
    mVersion(new Label(this, FULL_VERSION)),
    mButtonPadding(5)
{
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);
}

void SetupWindow::postInit()
{
    Window::postInit();
    int width = 620;
    const int height = 450;

    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setContentSize(width, height);
    setMinWidth(310);
    setMinHeight(210);

    mPanel->setSelectable(false);
    mPanel->getTabContainer()->setSelectable(false);
    mPanel->getWidgetContainer()->setSelectable(false);

    static const char *const buttonNames[] =
    {
        // TRANSLATORS: setup button
        N_("Apply"),
        // TRANSLATORS: setup button
        N_("Cancel"),
        // TRANSLATORS: setup button
        N_("Store"),
        // TRANSLATORS: setup button
        N_("Reset Windows"),
        nullptr
    };
    int x = width;
    mButtonPadding = getOption("buttonPadding", 5);
    for (const char *const * curBtn = buttonNames;
         *curBtn != nullptr;
         ++ curBtn)
    {
        Button *const btn = new Button(this,
            gettext(*curBtn),
            *curBtn,
            BUTTON_SKIN,
            this);
        mButtons.push_back(btn);
        x -= btn->getWidth() + mButtonPadding;
        btn->setPosition(x, height - btn->getHeight() - mButtonPadding);
        add(btn);

        // Store this button, as it needs to be enabled/disabled
        if (strcmp(*curBtn, "Reset Windows") == 0)
            mResetWindows = btn;
    }

    mPanel->setDimension(Rect(5, 5, width - 10, height - 40));
    mPanel->enableScrollButtons(true);

    mTabs.push_back(new Setup_Video(this));
    mTabs.push_back(new Setup_Visual(this));
    mTabs.push_back(new Setup_Audio(this));
    mTabs.push_back(new Setup_Perfomance(this));
    mTabs.push_back(new Setup_Touch(this));
    mTabs.push_back(new Setup_Input(this));
    mTabs.push_back(new Setup_Joystick(this));
    mTabs.push_back(new Setup_Colors(this));
    mTabs.push_back(new Setup_Chat(this));
    mTabs.push_back(new Setup_Players(this));
    mTabs.push_back(new Setup_Relations(this));
    mTabs.push_back(new Setup_Theme(this));
    mTabs.push_back(new Setup_Misc(this));

    FOR_EACH (std::list<SetupTab*>::const_iterator, i, mTabs)
    {
        SetupTab *const tab = *i;
        mPanel->addTab(tab->getName(), tab);
    }
    add(mPanel);

    if (mResetWindows != nullptr)
    {
        mVersion->setPosition(9,
            height - mVersion->getHeight() - mResetWindows->getHeight() - 9);
    }
    else
    {
        mVersion->setPosition(9, height - mVersion->getHeight() - 30);
    }
    add(mVersion);

    center();

    widgetResized(Event(nullptr));
    setInGame(false);
    enableVisibleSound(true);
}

SetupWindow::~SetupWindow()
{
    delete_all(mTabs);
    mButtons.clear();
    setupWindow = nullptr;
}

void SetupWindow::action(const ActionEvent &event)
{
    if (Game::instance() != nullptr)
        Game::instance()->resetAdjustLevel();
    const std::string &eventId = event.getId();

    if (eventId == "Apply")
    {
        setVisible(Visible_false);
        for_each(mTabs.begin(), mTabs.end(), std::mem_fun(&SetupTab::apply));
    }
    else if (eventId == "Cancel")
    {
        doCancel();
    }
    else if (eventId == "Store")
    {
        if (chatWindow != nullptr)
            chatWindow->saveState();
        config.write();
        serverConfig.write();
    }
    else if (eventId == "Reset Windows")
    {
        // Bail out if this action happens to be activated before the windows
        // are created (though it should be disabled then)
        if (statusWindow == nullptr)
            return;

        FOR_EACH (std::list<Window*>::const_iterator, it, mWindowsToReset)
        {
            if (*it != nullptr)
                (*it)->resetToDefaultSize();
        }
    }
}

void SetupWindow::setInGame(const bool inGame)
{
    if (mResetWindows != nullptr)
        mResetWindows->setEnabled(inGame);
}

void SetupWindow::externalUpdate()
{
    unloadAdditionalTabs();
    mModsTab = new Setup_Mods(this);
    mTabs.push_back(mModsTab);
    mPanel->addTab(mModsTab->getName(), mModsTab);
    mQuickTab = new Setup_Quick(this);
    mTabs.push_back(mQuickTab);
    mPanel->addTab(mQuickTab->getName(), mQuickTab);
    FOR_EACH (std::list<SetupTab*>::const_iterator, it, mTabs)
    {
        if (*it != nullptr)
            (*it)->externalUpdated();
    }
}

void SetupWindow::unloadTab(SetupTab *const page)
{
    if (page != nullptr)
    {
        mTabs.remove(page);
        mPanel->removeTab(mPanel->getTab(page->getName()));
    }
}

void SetupWindow::unloadAdditionalTabs()
{
    unloadTab(mModsTab);
    unloadTab(mQuickTab);
    delete2(mModsTab)
    delete2(mQuickTab)
}

void SetupWindow::externalUnload()
{
    FOR_EACH (std::list<SetupTab*>::const_iterator, it, mTabs)
    {
        if (*it != nullptr)
            (*it)->externalUnloaded();
    }
    unloadAdditionalTabs();
}

void SetupWindow::registerWindowForReset(Window *const window)
{
    mWindowsToReset.push_back(window);
}

void SetupWindow::unregisterWindowForReset(const Window *const window)
{
    FOR_EACH (std::list<Window*>::iterator, it, mWindowsToReset)
    {
        if (*it == window)
        {
            mWindowsToReset.erase(it);
            return;
        }
    }
}

void SetupWindow::hideWindows()
{
    FOR_EACH (std::list<Window*>::const_iterator, it, mWindowsToReset)
    {
        Window *const window = *it;
        if ((window != nullptr) && !window->isSticky())
            window->setVisible(Visible_false);
    }
    setVisible(Visible_false);
}

void SetupWindow::doCancel()
{
    setVisible(Visible_false);
    for_each(mTabs.begin(), mTabs.end(), std::mem_fun(&SetupTab::cancel));
}

void SetupWindow::activateTab(const std::string &name)
{
    std::string tmp = gettext(name.c_str());
    mPanel->setSelectedTabByName(tmp);
}

void SetupWindow::setVisible(Visible visible)
{
    touchManager.setTempHide(visible == Visible_true);
    Window::setVisible(visible);
}

void SetupWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    const Rect area = getChildrenArea();
    int x = area.width;
    const int height = area.height;
    const int width = area.width;
    mPanel->setDimension(Rect(5, 5, width - 10, height - 40));
    FOR_EACH (STD_VECTOR<Button*>::iterator, it, mButtons)
    {
        Button *const btn = *it;
        x -= btn->getWidth() + mButtonPadding;
        btn->setPosition(x, height - btn->getHeight() - mButtonPadding);
    }
    if (mResetWindows != nullptr)
    {
        mVersion->setPosition(9,
            height - mVersion->getHeight() - mResetWindows->getHeight() - 9);
    }
    else
    {
        mVersion->setPosition(9, height - mVersion->getHeight() - 30);
    }
}
