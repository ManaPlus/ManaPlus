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

#ifndef WINDOWMENU_H
#define WINDOWMENU_H

#include "configlistener.h"

#include "gui/widgets/container.h"
#include "gui/widgets/button.h"

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

#include "localconsts.h"

#include <map>
#include <vector>

class TextPopup;

/**
 * The window menu. Allows showing and hiding many of the different windows
 * used in the game.
 *
 * \ingroup Interface
 */
class WindowMenu : public Container,
                   public ConfigListener,
                   public gcn::ActionListener,
                   public gcn::SelectionListener,
                   public gcn::MouseListener
{
    public:
        WindowMenu();

        ~WindowMenu();

        void action(const gcn::ActionEvent &event);

//        void valueChanged(const gcn::SelectionEvent &event);

        void mousePressed(gcn::MouseEvent &event);

        void mouseMoved(gcn::MouseEvent &event);

        void mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED);

        std::map <std::string, gcn::Button*> &getButtonNames()
        { return mButtonNames; }

        std::vector <gcn::Button*> &getButtons()
        { return mButtons; }

        void showButton(std::string name, bool visible);

        void loadButtons();

        void saveButtons();

        void optionChanged(const std::string &name);

    private:
        void drawChildren(gcn::Graphics* graphics);

        inline void addButton(const char* text, std::string description,
                              int &x, int &h, int key, bool visible = true);

        void updateButtons();

        TextPopup *mTextPopup;
        std::vector <gcn::Button*> mButtons;
        std::map <std::string, gcn::Button*> mButtonNames;
        bool mHaveMouse;
        int mAutoHide;
};

extern WindowMenu *windowMenu;

#endif
