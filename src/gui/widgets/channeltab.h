/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef CHANNELTAB_H
#define CHANNELTAB_H

#include "chattab.h"

class Channel;

/**
 * A tab for a chat channel.
 */
class ChannelTab final : public ChatTab
{
    public:
        A_DELETE_COPY(ChannelTab)

        Channel *getChannel() const A_WARN_UNUSED
        { return mChannel; }

        void showHelp() override;

        bool handleCommand(const std::string &type,
                           const std::string &args) override;

    protected:
        friend class Channel;

        /**
         * Constructor.
         */
        ChannelTab(const Widget2 *const widget, Channel *const channel);

        /**
         * Destructor.
         */
        ~ChannelTab();

        void handleInput(const std::string &msg) override;

    private:
        Channel *mChannel;
};

#endif // CHANNELTAB_H
