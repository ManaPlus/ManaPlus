/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#ifndef StatusPopup_H
#define StatusPopup_H

#include "keydata.h"

#include "gui/widgets/popup.h"

#include "resources/iteminfo.h"

#include <guichan/mouselistener.hpp>

class TextBox;

namespace gcn
{
    class Label;
}

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

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~StatusPopup();

        /**
         * Sets the location to display the item popup.
         */
        void view(const int x, const int y);

//        void mouseMoved(gcn::MouseEvent &mouseEvent);

        void update();

    private:
        void updateLabels();

        void setLabelText(gcn::Label *const label, const char *const text,
                          const int key) const;

        void setLabelText2(gcn::Label *const label, const std::string &text,
                           const Input::KeyAction key) const;

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
        gcn::Label *mPvpAttackType;
        gcn::Label *mDisableGameModifiers;
        gcn::Label *mImitationMode;
        gcn::Label *mAwayMode;
        gcn::Label *mCameraMode;
};

#endif // StatusPopup_H
