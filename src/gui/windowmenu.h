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

#ifndef GUI_WINDOWMENU_H
#define GUI_WINDOWMENU_H

#include "enums/input/inputaction.h"

#include "gui/widgets/container.h"

#include "listeners/actionlistener.h"
#include "listeners/mouselistener.h"
#include "listeners/selectionlistener.h"

#include "localconsts.h"

class Button;

struct ButtonInfo;
struct ButtonText;

/**
 * The window menu. Allows showing and hiding many of the different windows
 * used in the game.
 *
 * \ingroup Interface
 */
class WindowMenu final : public Container,
                         public ConfigListener,
                         public ActionListener,
                         public SelectionListener,
                         public MouseListener
{
    public:
        explicit WindowMenu(const Widget2 *const widget);

        A_DELETE_COPY(WindowMenu)

        ~WindowMenu() override final;

        void action(const ActionEvent &event) override final;

        void mousePressed(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent& event A_UNUSED) override final;

        std::map <std::string, ButtonInfo*> &getButtonNames() A_WARN_UNUSED
        { return mButtonNames; }

        STD_VECTOR <Button*> &getButtons() A_WARN_UNUSED
        { return mButtons; }

        STD_VECTOR <ButtonText*> &getButtonTexts() A_WARN_UNUSED
        { return mButtonTexts; }

        void showButton(const std::string &name, const Visible visible);

        void loadButtons();

        void saveButtons() const;

        void optionChanged(const std::string &name) override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif  // USE_PROFILER

    protected:
        void drawChildren(Graphics *const graphics) override final
                          A_NONNULL(2);

        void safeDrawChildren(Graphics *const graphics) override final
                              A_NONNULL(2);

    private:
        inline void addButton(const char *const text,
                              const std::string &description,
                              int &restrict x, int &restrict h,
                              const InputActionT key,
                              const Visible visible);

        void updateButtons();

        Skin *mSkin;
        int mPadding;
        int mSpacing;
        STD_VECTOR <Button*> mButtons;
        STD_VECTOR <ButtonText*> mButtonTexts;
        std::map <std::string, ButtonInfo*> mButtonNames;
        bool mHaveMouse;
        int mAutoHide;
        bool mSmallWindow;
};

extern WindowMenu *windowMenu;

#endif  // GUI_WINDOWMENU_H
