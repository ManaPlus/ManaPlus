/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_POPUPS_SKILLPOPUP_H
#define GUI_POPUPS_SKILLPOPUP_H

#include "gui/widgets/popup.h"

#include "enums/resources/skill/casttype.h"

class Label;
class TextBox;

struct SkillInfo;

/**
 * A popup that displays information about an item.
 */
class SkillPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the skill popup.
         */
        SkillPopup();

        A_DELETE_COPY(SkillPopup)

        /**
         * Destructor. Cleans up the skill popup on deletion.
         */
        ~SkillPopup();

        void postInit() override final;

        /**
         * Sets the info to be displayed given a particular item.
         */
        void show(const SkillInfo *const skill,
                  const int level,
                  const CastTypeT type,
                  const int offsetX,
                  const int offsetY);

        void mouseMoved(MouseEvent &event) override final;

        void reset();

    private:
        Label *mSkillName A_NONNULLPOINTER;
        TextBox *mSkillDesc A_NONNULLPOINTER;
        TextBox *mSkillEffect A_NONNULLPOINTER;
        TextBox *mSkillLevel A_NONNULLPOINTER;
        TextBox *mSkillCastType A_NONNULLPOINTER;
        CastTypeT mCastType;
        unsigned int mLastId;
        int mLastLevel;
        int mOffsetX;
        int mOffsetY;
};

extern SkillPopup *skillPopup;

#endif  // GUI_POPUPS_SKILLPOPUP_H
