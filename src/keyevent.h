/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "keydata.h"

#include <guichan/key.hpp>
#include <guichan/keyevent.hpp>
#include <guichan/widget.hpp>

#include <string>

class KeyEvent final : public gcn::KeyEvent
{
    public:
        KeyEvent(gcn::Widget *const source,
                 const bool shiftPressed,
                 const bool controlPressed,
                 const bool altPressed,
                 const bool metaPressed,
                 const unsigned int type,
                 const bool numericPad,
                 const int actionId,
                 const gcn::Key& key);

        A_DELETE_COPY(KeyEvent);

        virtual ~KeyEvent();

        int getActionId() const
        { return mActionId; }

    protected:
        int mActionId;
};

#endif
