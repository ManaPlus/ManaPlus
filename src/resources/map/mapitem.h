/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_MAPITEM_H
#define RESOURCES_MAP_MAPITEM_H

#include <string>

#include "localconsts.h"

class Graphics;
class Image;

class MapItem final
{
    public:
        friend class Map;
        friend class MapLayer;
        friend class SpecialLayer;

        MapItem();

        explicit MapItem(const int type);

        MapItem(const int type, std::string comment);

        MapItem(const int type, std::string comment, const int x, const int y);

        A_DELETE_COPY(MapItem)

        ~MapItem();

        int getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void setType(const int type);

        void setPos(const int x, const int y);

        int getX() const noexcept2 A_WARN_UNUSED
        { return mX; }

        int getY() const noexcept2 A_WARN_UNUSED
        { return mY; }

        const std::string &getComment() const noexcept2 A_WARN_UNUSED
        { return mComment; }

        void setComment(const std::string &comment) noexcept2
        { mComment = comment; }

        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        void setName(const std::string &name) noexcept2
        { mName = name; }

        void draw(Graphics *const graphics,
                  const int x, const int y,
                  const int dx, const int dy) const A_NONNULL(2);

    private:
        Image *mImage;
        std::string mComment;
        std::string mName;
        int mType;
        int mX;
        int mY;
};

#endif  // RESOURCES_MAP_MAPITEM_H
