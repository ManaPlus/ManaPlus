/*
 *  The Mana World
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef StatusPopup_H
#define StatusPopup_H

#include "gui/widgets/popup.h"

#include "resources/iteminfo.h"

#include <guichan/mouselistener.hpp>

class TextBox;

/**
 * A popup that displays information about an item.
 */
class StatusPopup : public Popup
{
    public:
        /**
         * Constructor. Initializes the item popup.
         */
        StatusPopup();

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~StatusPopup();

        /**
         * Sets the location to display the item popup.
         */
        void view(int x, int y);

//        void mouseMoved(gcn::MouseEvent &mouseEvent);

        void update();

    private:
        void updateLabels();
        void setLabelText(gcn::Label *label, char *text, int key);

        gcn::Label *mMoveType;
        gcn::Label *mCrazyMoveType;
        gcn::Label *mMoveToTargetType;
        gcn::Label *mFollowMode;
        gcn::Label *mAttackType;
        gcn::Label *mAttackWeaponType;
        gcn::Label *mDropCounter;
        gcn::Label *mPickUpType;
        gcn::Label *mMapType;
        gcn::Label *mMagicAttackType;
        gcn::Label *mDisableGameModifiers;
        gcn::Label *mImitationMode;
        gcn::Label *mAwayMode;
        gcn::Label *mCameraMode;
};

#endif // StatusPopup_H
