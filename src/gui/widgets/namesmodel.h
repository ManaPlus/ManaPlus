/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_NAMESMODEL_H
#define GUI_WIDGETS_NAMESMODEL_H

#include "utils/stringvector.h"

#include <guichan/listmodel.hpp>

#include "localconsts.h"

class NamesModel : public gcn::ListModel
{
    public:
        NamesModel();

        virtual ~NamesModel();

        virtual int getNumberOfElements() override;

        virtual std::string getElementAt(int i) override;

        StringVect &getNames()
        { return mNames; }

        size_t size()
        { return mNames.size(); }

        void clear()
        { mNames.clear(); }

    protected:
        StringVect mNames;
};

#endif
