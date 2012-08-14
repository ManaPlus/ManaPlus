/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef TILESET_H
#define TILESET_H

#include "resources/imageset.h"

#include <map>

/**
 * A tileset, which is basically just an image set but it stores a firstgid.
 */
class Tileset : public ImageSet
{
    public:
        /**
         * Constructor.
         */
        Tileset(Image *img, int w, int h, int firstGid,
                int margin, int spacing):
            ImageSet(img, w, h, margin, spacing),
            mFirstGid(firstGid)
        {
        }

        /**
         * Returns the first gid.
         */
        int getFirstGid() const
        { return mFirstGid; }

        /**
         * Set tileset property.
         */
        void setProperties(std::map<std::string, std::string> props)
        { mProperties = props; }

        /**
         * Returns property value.
         */
        std::string getProperty(std::string name)
        {
            std::map<std::string, std::string>::const_iterator
                it = mProperties.find(name);
            if (it == mProperties.end())
                return "";
            return mProperties[name];
        }

    private:
        int mFirstGid;

        std::map<std::string, std::string> mProperties;
};

#endif
