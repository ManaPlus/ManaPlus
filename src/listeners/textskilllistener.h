/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef LISTENERS_TEXTSKILLLISTENER_H
#define LISTENERS_TEXTSKILLLISTENER_H

#include "listeners/actionlistener.h"

#include "localconsts.h"

class TextDialog;

class TextSkillListener final : public ActionListener
{
    public:
        TextSkillListener();

        A_DELETE_COPY(TextSkillListener)

        void action(const ActionEvent &event) override final;

        void setSkill(const int skillId,
                      const int x,
                      const int y,
                      const int level);

        void setDialog(TextDialog *dialog)
        { mDialog = dialog; }

    private:
        TextDialog *mDialog;
        int mSkillId;
        int mX;
        int mY;
        int mLevel;
};

#endif  // LISTENERS_TEXTSKILLLISTENER_H
