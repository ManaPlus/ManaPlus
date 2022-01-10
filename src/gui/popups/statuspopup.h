/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#ifndef GUI_POPUPS_STATUSPOPUP_H
#define GUI_POPUPS_STATUSPOPUP_H

#include "enums/input/inputaction.h"

#include "gui/widgets/popup.h"

class Label;

const int STATUSPOPUP_NUM_LABELS = 16;

/**
 * A popup that displays information about an item.
 */
class StatusPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the item popup.
         */
        StatusPopup();

        A_DELETE_COPY(StatusPopup)

        void postInit() override final;

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~StatusPopup() override final;

        /**
         * Sets the location to display the item popup.
         */
        void view(const int x, const int y);

        void update();

    private:
        void updateLabels() const;

        void setLabelText(const int num,
                          const std::string &text,
                          const InputActionT key) const;

        Label *mLabels[STATUSPOPUP_NUM_LABELS] A_NONNULLPOINTER;
};

#endif  // GUI_POPUPS_STATUSPOPUP_H
