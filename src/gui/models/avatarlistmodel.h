/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_MODELS_AVATARLISTMODEL_H
#define GUI_MODELS_AVATARLISTMODEL_H

#include "avatar.h"

#include "gui/models/listmodel.h"

#include <string>

class AvatarListModel : public ListModel
{
    public:
        virtual Avatar *getAvatarAt(const int i) A_WARN_UNUSED = 0;

        std::string getElementAt(int i) override final A_WARN_UNUSED
        { return getAvatarAt(i)->getName(); }
};

#endif  // GUI_MODELS_AVATARLISTMODEL_H
