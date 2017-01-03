/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef NET_TMWA_CHATRECV_H
#define NET_TMWA_CHATRECV_H

#include <string>

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace TmwAthena
{
    namespace ChatRecv
    {
        void processChat(Net::MessageIn &msg);
        void processChatContinue(std::string chatMsg,
                                 const std::string &channel);
        void processGmChat(Net::MessageIn &msg);
        void processWhisper(Net::MessageIn &msg);
        void processWhisperResponse(Net::MessageIn &msg);
        void processWhisperContinue(const std::string &nick,
                                    std::string chatMsg);
        void processBeingChat(Net::MessageIn &msg);
        void processScriptMessage(Net::MessageIn &msg);
    }  // namespace ChatRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_CHATRECV_H
