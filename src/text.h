/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
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

#ifndef TEXT_H
#define TEXT_H

#include "render/graphics.h"

#include "gui/color.h"

#include "localconsts.h"

class Font;

class Text
{
    friend class TextManager;

    public:
        /**
         * Constructor creates a text object to display on the screen.
         */
        Text(const std::string &text, const int x, const int y,
             const Graphics::Alignment alignment,
             const Color *const color, const bool isSpeech = false,
             Font *const font = nullptr);

        A_DELETE_COPY(Text)

        /**
         * Destructor. The text is removed from the screen.
         */
        virtual ~Text();

        void setColor(const Color *const color);

        int getWidth() const A_WARN_UNUSED
        { return mWidth; }

        int getHeight() const A_WARN_UNUSED
        { return mHeight; }

        /**
         * Allows the originator of the text to specify the ideal coordinates.
         */
        void adviseXY(const int x, const int y, const bool move);

        /**
         * Draws the text.
         */
        virtual void draw(Graphics *const graphics,
                          const int xOff, const int yOff);

    private:
        Font *mFont;      /**< The font of the text */
        int mX;                /**< Actual x-value of left of text written. */
        int mY;                /**< Actual y-value of top of text written. */
        int mWidth;            /**< The width of the text. */
        int mHeight;           /**< The height of the text. */
        int mXOffset;          /**< The offset of mX from the desired x. */
        static int mInstances; /**< Instances of text. */
        std::string mText;     /**< The text to display. */
        const Color *mColor;     /**< The color of the text. */
        const Color mOutlineColor;
        bool mIsSpeech;        /**< Is this text a speech bubble? */

    protected:
        static ImageRect mBubble;   /**< Speech bubble graphic */
};

class FlashText final : public Text
{
    public:
        FlashText(const std::string &text, const int x, const int y,
                  const Graphics::Alignment alignment,
                  const Color *const color,
                  Font *const font = nullptr);

        A_DELETE_COPY(FlashText)

        /**
         * Remove the text from the screen
         */
        ~FlashText()
        { }

        /**
         * Flash the text for so many refreshes.
         */
        void flash(const int time)
        { mTime = time; }

        /**
         * Draws the text.
         */
        void draw(Graphics *const graphics,
                  const int xOff, const int yOff) override final;

    private:
        int mTime;             /**< Time left for flashing */
};

#endif  // TEXT_H
