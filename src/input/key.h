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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INPUT_KEY_H
#define INPUT_KEY_H

#include "localconsts.h"

// windows.h defines DELETE which breaks this file as we have a constant named
// DELETE, hence we undefine DELETE if it is defined and hope people don't use
// that windows define with Guichan.
#if defined (_WIN32) && defined(DELETE)
#undef DELETE
#endif  // defined (_WIN32) && defined(DELETE)

/**
  * Represents a key or a character.
  */
class Key final
{
    public:
        /**
          * Constructor.
          *
          * @param value The ascii or enum value for the key.
          */
        explicit Key(const int value);

        A_DEFAULT_COPY(Key)

        /**
          * Checks if a key is a character.
          *
          * @return True if the key is a letter, number or whitespace,
          *         false otherwise.
          */
        bool isCharacter() const A_WARN_UNUSED;

        /**
          * Checks if a key is a number.
          *
          * @return True if the key is a number (0-9),
          *         false otherwise.
          */
        bool isNumber() const A_WARN_UNUSED;

        /**
          * Checks if a key is a letter.
          *
          * @return True if the key is a letter (a-z,A-Z),
          *         false otherwise.
          */
        bool isLetter() const A_WARN_UNUSED;

        /**
          * Gets the value of the key. If an ascii value exists it
          * will be returned. Otherwise an enum value will be returned.
          *
          * @return the value of the key.
          */
        int getValue() const A_WARN_UNUSED;

        /**
          * Compares two keys.
          *
          * @param key The key to compare this key with.
          * @return True if the keys are equal, false otherwise.
          */
        bool operator==(const Key& key) const;

        /**
          * Compares two keys.
          *
          * @param key The key to compare this key with.
          * @return True if the keys are not equal, false otherwise.
          */
        bool operator!=(const Key& key) const;

    protected:
        /**
          * Holds the value of the key. It may be an ascii value
          * or an enum value.
          */
        int mValue;
};

#endif  // INPUT_KEY_H
