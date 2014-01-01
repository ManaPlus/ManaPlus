/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef INPUT_KEYINPUT_H
#define INPUT_KEYINPUT_H

#include <guichan/keyinput.hpp>

#include <string>

#include "localconsts.h"

class KeyInput final : public gcn::KeyInput
{
    public:
        KeyInput();

        ~KeyInput();

        void setActionId(const int n)
        { mActionId = n; }

        int getActionId() const A_WARN_UNUSED
        { return mActionId; }

#ifdef USE_SDL2
        void setText(const std::string &text)
        { mText = text; }

        std::string getText() const
        { return mText; }
#endif

    protected:
        int mActionId;
#ifdef USE_SDL2
        std::string mText;
#endif
};

#endif  // INPUT_KEYINPUT_H
