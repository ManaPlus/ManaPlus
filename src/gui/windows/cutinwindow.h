/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_WINDOWS_CUTINWINDOW_H
#define GUI_WINDOWS_CUTINWINDOW_H

#include "gui/widgets/window.h"

#include "enums/cutin.h"

class AnimatedSprite;

class CutInWindow final : public Window
{
    public:
        CutInWindow();

        A_DELETE_COPY(CutInWindow)

        ~CutInWindow() override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void draw2(Graphics *const graphics) A_NONNULL(2);

        void show(const std::string &name,
                  const CutInT cutin);

        void hide();

        void logic() override final;

    private:
        AnimatedSprite *mImage;
        int mOldTitleBarHeight;
};

extern CutInWindow *cutInWindow;

#endif  // GUI_WINDOWS_CUTINWINDOW_H
