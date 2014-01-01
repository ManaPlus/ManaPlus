/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
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

#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <list>

#include "localconsts.h"

class Graphics;
class Text;

class TextManager final
{
    public:
        /**
         * Constructor
         */
        TextManager();

        A_DELETE_COPY(TextManager)

        /**
         * Destroy the manager
         */
        ~TextManager();

        /**
         * Add text to the manager
         */
        void addText(Text *const text);

        /**
         * Move the text around the screen
         */
        void moveText(Text *const text, const int x, const int y) const;

        /**
         * Remove the text from the manager
         */
        void removeText(const Text *const text);

        /**
         * Draw the text
         */
        void draw(Graphics *const graphics,
                  const int xOff, const int yOff);

    private:
        /**
         * Position the text so as to avoid conflict
         */
        void place(const Text *const textObj, const Text *const omit,
                   const int &x, int &y, const int h) const;

        typedef std::list<Text *> TextList; /**< The container type */
        TextList mTextList; /**< The container */
};

extern TextManager *textManager;

#endif  // TEXTMANAGER_H
