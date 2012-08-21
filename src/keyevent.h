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

class KeyEvent : public gcn::KeyEvent
{
    public:
        KeyEvent(gcn::Widget* source,
                 bool shiftPressed,
                 bool controlPressed,
                 bool altPressed,
                 bool metaPressed,
                 unsigned int type,
                 bool numericPad,
                 int actionId,
                 const gcn::Key& key);

        virtual ~KeyEvent();

        int getActionId()
        { return mActionId; }

    protected:
        int mActionId;
};

#endif
