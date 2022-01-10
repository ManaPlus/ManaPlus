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

#ifndef LISTENERS_TOOLTIPLISTENER_H
#define LISTENERS_TOOLTIPLISTENER_H

#include "listeners/mouselistener.h"

#include <string>

#include "localconsts.h"

class ToolTipListener notfinal : public MouseListener
{
    public:
        ToolTipListener();

        A_DELETE_COPY(ToolTipListener)

        void setToolTip(const std::string &str)
        { mToolTip = str; }

        std::string getToolTip() const
        { return mToolTip; }

        void mouseMoved(MouseEvent &event) override;

        void mouseExited(MouseEvent &event) override;

    protected:
        std::string mToolTip;
};

#endif  // LISTENERS_TOOLTIPLISTENER_H
