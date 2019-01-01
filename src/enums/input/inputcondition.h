/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef ENUMS_INPUT_INPUTCONDITION_H
#define ENUMS_INPUT_INPUTCONDITION_H

namespace InputCondition
{
    enum Type
    {
        DEFAULT    = 1U,                      // default condition
        ENABLED    = 2U,                      // keyboard must be enabled
        NOINPUT    = 4U,                      // input items must be unfocused
        NOAWAY     = 8U,                      // player not in away mode
        NOSETUP    = 16U,                     // setup window is hidde
        VALIDSPEED = 32U,                     // valid speed
        NOMODAL    = 64U,                     // modal windows inactive
        NONPCINPUT = 128U,                    // npc input field inactive
        EMODS      = 256U,                    // game modifiers enabled
        NOTARGET   = 512U,                    // no target/untarget keys
                                             // pressed
        NOFOLLOW   = 1024U,                   // follow mode disabled
        INGAME     = 2048U,                   // game must be started
        NOBUYSELL  = 4096U,                   // no active buy or sell dialogs
        NONPCDIALOG = 8192U,                  // no active npc dialog or
                                             // dialog almost closed
        NOTALKING  = 16384U,                  // player have no opened
                                             // dialogs what prevent moving
        ALIVE      = 32768U,                  // player alive
        NOVENDING  = 65536U,                  // vending disabled
        NOROOM     = 131072U,                 // not joined room
        NOBLOCK    = 262144U,                 // no blocked move (trick dead)
        KEY_DOWN   = 524288U,                 // key press allowed
        KEY_UP     = 1048576U,                // key release allowed
        SHORTCUT   = 2U + 4 + 16 + 512 + 2048,  // flags for shortcut keys
        SHORTCUT0  = 2U + 4 + 16 + 512,       // flags for shortcut keys
        GAME       = 2U + 4 + 8 + 16 + 64 + 2048,  // main game key
        GAME2      = 2U + 8 + 16 + 64 + 2048,
        ARROWKEYS  = 2U + 4 + 8 + 16 + 64 + 2048 + 4096 +
                     16384 + 65536 + 131072 + 262144
    };
}  // namespace InputCondition

#endif  // ENUMS_INPUT_INPUTCONDITION_H
