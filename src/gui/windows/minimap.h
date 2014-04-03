/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_MINIMAP_H
#define GUI_WINDOWS_MINIMAP_H

#include "gui/widgets/window.h"

class Image;
class Map;
class TextPopup;

/**
 * Minimap window. Shows a minimap image and the name of the current map.
 *
 * The name of the map is defined by the map property "name". The minimap image
 * is defined by the map property "minimap". The path to the image should be
 * given relative to the root of the client data.
 *
 * \ingroup Interface
 */
class Minimap final : public Window, public ConfigListener
{
    public:
        Minimap();

        A_DELETE_COPY(Minimap)

        ~Minimap();

        /**
         * Sets the map image that should be displayed.
         */
        void setMap(const Map *const map);

        /**
         * Toggles the displaying of the minimap.
         */
        void toggle();

        /**
         * Draws the minimap.
         */
        void draw(Graphics *graphics) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void screenToMap(int &x, int &y);

        void optionChanged(const std::string &name);

    private:
        void deleteMapImage();

        float mWidthProportion;
        float mHeightProportion;
        Image *mMapImage;
        TextPopup *mTextPopup;
        int mMapOriginX;
        int mMapOriginY;
        bool mCustomMapImage;
        bool mAutoResize;
        static bool mShow;
};

extern Minimap *minimap;

#endif  // GUI_WINDOWS_MINIMAP_H
