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

#include "gui/setup.h"

#include "chatwindow.h"
#include "configuration.h"
#include "game.h"
#include "main.h"
#include "touchmanager.h"

#include "gui/setup_audio.h"
#include "gui/setup_chat.h"
#include "gui/setup_colors.h"
#include "gui/setup_joystick.h"
#include "gui/setup_other.h"
#include "gui/setup_theme.h"
#include "gui/setup_input.h"
#include "gui/setup_perfomance.h"
#include "gui/setup_players.h"
#include "gui/setup_relations.h"
#include "gui/setup_touch.h"
#include "gui/setup_video.h"
#include "gui/setup_visual.h"

#include "gui/widgets/label.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Window *statusWindow;

Setup::Setup():
    Window(_("Setup"), false, nullptr, "setup.xml"),
    gcn::ActionListener(),
    mResetWindows(nullptr),
    mPanel(new TabbedArea(this))
{
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);

    int width = 620;
    const int height = 450;

    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setContentSize(width, height);
    //setMaxHeight(height);

    static const char *buttonNames[] =
    {
        N_("Apply"),
        N_("Cancel"),
        N_("Store"),
        N_("Reset Windows"),
        nullptr
    };
    int x = width;
    const int buttonPadding = getOption("buttonPadding", 5);
    for (const char ** curBtn = buttonNames; *curBtn; ++ curBtn)
    {
        Button *const btn = new Button(this, gettext(*curBtn), *curBtn, this);
        x -= btn->getWidth() + buttonPadding;
        btn->setPosition(x, height - btn->getHeight() - buttonPadding);
        add(btn);

        // Store this button, as it needs to be enabled/disabled
        if (!strcmp(*curBtn, "Reset Windows"))
            mResetWindows = btn;
    }

    mPanel->setDimension(gcn::Rectangle(5, 5, width - 10, height - 40));
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
    mTabs.push_back(new Setup_Other(this));

    for (std::list<SetupTab*>::const_iterator i = mTabs.begin(),
         i_end = mTabs.end();
         i != i_end; ++i)
    {
        SetupTab *const tab = *i;
        mPanel->addTab(tab->getName(), tab);
    }

    add(mPanel);

    Label *const version = new Label(this, FULL_VERSION);
//    version->setPosition(9, height - version->getHeight() - 9);
    if (mResetWindows)
    {
        version->setPosition(9,
            height - version->getHeight() - mResetWindows->getHeight() - 9);
    }
    else
    {
        version->setPosition(9, height - version->getHeight() - 30);
    }
    add(version);

    center();

    setInGame(false);
    enableVisibleSound(true);
}

Setup::~Setup()
{
    delete_all(mTabs);
}

void Setup::action(const gcn::ActionEvent &event)
{
    if (Game::instance())
        Game::instance()->resetAdjustLevel();
    if (event.getId() == "Apply")
    {
        setVisible(false);
        for_each(mTabs.begin(), mTabs.end(), std::mem_fun(&SetupTab::apply));
    }
    else if (event.getId() == "Cancel")
    {
        doCancel();
    }
    else if (event.getId() == "Store")
    {
        if (chatWindow)
            chatWindow->saveState();
        config.write();
        serverConfig.write();
    }
    else if (event.getId() == "Reset Windows")
    {
        // Bail out if this action happens to be activated before the windows
        // are created (though it should be disabled then)
        if (!statusWindow)
            return;

        for (std::list<Window*>::const_iterator it = mWindowsToReset.begin(),
             it_end = mWindowsToReset.end();
             it != it_end; ++it)
        {
            if (*it)
                (*it)->resetToDefaultSize();
        }
    }
}

void Setup::setInGame(const bool inGame)
{
    mResetWindows->setEnabled(inGame);
}

void Setup::externalUpdate()
{
    for (std::list<SetupTab*>::const_iterator it = mTabs.begin(),
         it_end = mTabs.end(); it != it_end; ++ it)
    {
        if (*it)
            (*it)->externalUpdated();
    }
}

void Setup::registerWindowForReset(Window *const window)
{
    mWindowsToReset.push_back(window);
}

void Setup::doCancel()
{
    setVisible(false);
    for_each(mTabs.begin(), mTabs.end(), std::mem_fun(&SetupTab::cancel));
}

void Setup::activateTab(const std::string &name)
{
    std::string tmp = gettext(name.c_str());
    mPanel->setSelectedTabByName(tmp);
}

void Setup::setVisible(bool visible)
{
    touchManager.setTempHide(visible);
    Window::setVisible(visible);
}

Setup *setupWindow;
