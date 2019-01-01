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

#ifndef GUI_MODELS_NAMESMODEL_H
#define GUI_MODELS_NAMESMODEL_H

#include "utils/stringvector.h"

#include "gui/models/listmodel.h"

#include "localconsts.h"

class NamesModel notfinal : public ListModel
{
    public:
        NamesModel();

        A_DELETE_COPY(NamesModel)

        ~NamesModel() override;

        int getNumberOfElements() override final A_WARN_UNUSED;

        std::string getElementAt(int i) override final A_WARN_UNUSED;

        StringVect &getNames() noexcept2 A_WARN_UNUSED
        { return mNames; }

        size_t size() const noexcept2 A_WARN_UNUSED
        { return mNames.size(); }

        void clear()
        { mNames.clear(); }

        void add(const std::string &str)
        { mNames.push_back(str); }

        void fillFromArray(const char *const *const arr,
                           const std::size_t size);

        void fillFromVector(const StringVect &vect);

    protected:
        StringVect mNames;
};

#endif  // GUI_MODELS_NAMESMODEL_H
