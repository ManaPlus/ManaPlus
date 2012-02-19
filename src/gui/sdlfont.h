/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef SDLFONT_H
#define SDLFONT_H

#include <guichan/font.hpp>

#ifdef __WIN32__
#include <SDL/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#include <list>
#include <string>

#define CACHES_NUMBER 256

class SDLTextChunk;

/**
 * A wrapper around SDL_ttf for allowing the use of TrueType fonts.
 *
 * <b>NOTE:</b> This class initializes SDL_ttf as necessary.
 */
class SDLFont : public gcn::Font
{
    public:
        /**
         * Constructor.
         *
         * @param filename  Font filename.
         * @param size      Font size.
         */
        SDLFont(std::string filename, int size, int style = 0);

        /**
         * Destructor.
         */
        ~SDLFont();

        void loadFont(std::string filename, int size, int style = 0);

        void createSDLTextChunk(SDLTextChunk *chunk);

        virtual int getWidth(const std::string &text) const;

        virtual int getHeight() const;

        std::list<SDLTextChunk> *getCache()
        { return mCache; }

        /**
         * @see Font::drawString
         */
        void drawString(gcn::Graphics *graphics,
                        const std::string &text,
                        int x, int y);

        void clear();

        void doClean();

        int getCreateCounter() const
        { return mCreateCounter; }

        int getDeleteCounter() const
        { return mDeleteCounter; }

    private:
        TTF_Font *mFont;
        unsigned mCreateCounter;
        unsigned mDeleteCounter;

        // Word surfaces cache
        mutable std::list<SDLTextChunk> mCache[CACHES_NUMBER];
        int mCleanTime;
};

#endif
