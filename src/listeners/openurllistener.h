/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef LISTENERS_OPENURLLISTENER_H
#define LISTENERS_OPENURLLISTENER_H

#include "listeners/actionlistener.h"

#include "utils/process.h"

#include "localconsts.h"

struct OpenUrlListener final : public ActionListener
{
    OpenUrlListener() :
        ActionListener(),
        url()
    {
    }

    A_DELETE_COPY(OpenUrlListener)

    public:
        void action(const ActionEvent &event) override final
        {
            if (event.getId() == "yes")
                openBrowser(url);
        }

        std::string url;
};

#endif  // LISTENERS_OPENURLLISTENER_H
