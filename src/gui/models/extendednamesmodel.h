/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef GUI_MODELS_EXTENDEDNAMESMODEL_H
#define GUI_MODELS_EXTENDEDNAMESMODEL_H

#include "utils/stringvector.h"

#include "gui/models/extendedlistmodel.h"

class ExtendedNamesModel : public ExtendedListModel
{
    public:
        ExtendedNamesModel();

        A_DELETE_COPY(ExtendedNamesModel)

        virtual ~ExtendedNamesModel();

        virtual int getNumberOfElements() override final A_WARN_UNUSED;

        virtual std::string getElementAt(int i) override final A_WARN_UNUSED;

        virtual const Image *getImageAt(int i) override final A_WARN_UNUSED;

        StringVect &getNames() A_WARN_UNUSED
        { return mNames; }

        std::vector<Image*> &getImages() A_WARN_UNUSED
        { return mImages; }

        size_t size() A_WARN_UNUSED
        { return mNames.size(); }

        void clear();

    protected:
        StringVect mNames;
        std::vector<Image*> mImages;
};

#endif  // GUI_MODELS_EXTENDEDNAMESMODEL_H
