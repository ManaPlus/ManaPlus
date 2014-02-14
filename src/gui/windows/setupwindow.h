/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_SETUPWINDOW_H
#define GUI_WINDOWS_SETUPWINDOW_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

#include <list>

class Button;
class Label;
class SetupTab;
class TabbedArea;

/**
 * The setup dialog. Displays several tabs for configuring different aspects
 * of the game.
 *
 * \ingroup GUI
 */
class SetupWindow final : public Window,
                          public gcn::ActionListener
{
    public:
        SetupWindow();

        A_DELETE_COPY(SetupWindow)

        ~SetupWindow();

        void postInit() override final;

        void action(const gcn::ActionEvent &event) override final;

        void setInGame(const bool inGame);

        void externalUpdate();

        void externalUnload();

        void registerWindowForReset(Window *const window);

        void clearWindowsForReset()
        { mWindowsToReset.clear(); }

        void doCancel();

        void activateTab(const std::string &name);

        void setVisible(bool visible) override final;

        void widgetResized(const gcn::Event &event) override final;

    private:
        void unloadModTab();

        std::list<SetupTab*> mTabs;
        SetupTab *mModsTab;
        std::list<Window*> mWindowsToReset;
        std::vector<Button*> mButtons;
        Button *mResetWindows;
        TabbedArea *mPanel;
        Label *mVersion;
};

extern SetupWindow* setupWindow;

#endif  // GUI_WINDOWS_SETUPWINDOW_H
