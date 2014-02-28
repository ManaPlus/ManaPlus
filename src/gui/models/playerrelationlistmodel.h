/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_MODELS_PLAYERRELATIONLISTMODEL_H
#define GUI_MODELS_PLAYERRELATIONLISTMODEL_H

#include "being/playerrelations.h"

#include "utils/gettext.h"

static const char *const RELATION_NAMES[PlayerRelation::RELATIONS_NR] =
{
    // TRANSLATORS: relation type
    N_("Neutral"),
    // TRANSLATORS: relation type
    N_("Friend"),
    // TRANSLATORS: relation type
    N_("Disregarded"),
    // TRANSLATORS: relation type
    N_("Ignored"),
    // TRANSLATORS: relation type
    N_("Erased"),
    // TRANSLATORS: relation type
    N_("Blacklisted"),
    // TRANSLATORS: relation type
    N_("Enemy")
};

class PlayerRelationListModel final : public ListModel
{
    public:
        ~PlayerRelationListModel()
        { }

        int getNumberOfElements() override final
        {
            return PlayerRelation::RELATIONS_NR;
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "";
            return gettext(RELATION_NAMES[i]);
        }
};

#endif  // GUI_MODELS_PLAYERRELATIONLISTMODEL_H
