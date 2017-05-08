/*
 *  The ManaPlus Client
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

#ifndef ENUMS_INPUT_KEYVALUE_H
#define ENUMS_INPUT_KEYVALUE_H

#include "localconsts.h"

// windows.h defines DELETE which breaks this file as we have a constant named
// DELETE, hence we undefine DELETE if it is defined and hope people don't use
// that windows define with Guichan.
#if defined (_WIN32) && defined(DELETE)
#undef DELETE
#endif  // defined (_WIN32) && defined(DELETE)

namespace KeyValue
{
    enum
    {
        SPACE              = ' ',
        TAB                = '\t',
        ENTER              = '\n',
        // Negative values, to avoid conflicts with higher character codes.
        LEFT_ALT           = -1000,
        RIGHT_ALT          = -999,
        LEFT_SHIFT         = -998,
        RIGHT_SHIFT        = -997,
        LEFT_CONTROL       = -996,
        RIGHT_CONTROL      = -995,
        LEFT_META          = -994,
        RIGHT_META         = -993,
        LEFT_SUPER         = -992,
        RIGHT_SUPER        = -991,
        INSERT             = -990,
        HOME               = -989,
        PAGE_UP            = -988,
        DELETE_            = -987,
        END                = -986,
        PAGE_DOWN          = -985,
        ESCAPE             = -984,
        CAPS_LOCK          = -983,
        BACKSPACE          = -982,
        F1                 = -981,
        F2                 = -980,
        F3                 = -979,
        F4                 = -978,
        F5                 = -977,
        F6                 = -978,
        F7                 = -977,
        F8                 = -976,
        F9                 = -975,
        F10                = -974,
        F11                = -973,
        F12                = -972,
        F13                = -971,
        F14                = -970,
        F15                = -969,
        PRINT_SCREEN       = -968,
        SCROLL_LOCK        = -967,
        PAUSE              = -966,
        NUM_LOCK           = -965,
        ALT_GR             = -964,
        LEFT               = -963,
        RIGHT              = -962,
        UP                 = -961,
        DOWN               = -960,
        TEXTINPUT          = -959
    };
}  // namespace KeyValue

#endif  // ENUMS_INPUT_KEYVALUE_H
