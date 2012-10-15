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

#ifndef BROWSERBOX_H
#define BROWSERBOX_H

#include <guichan/mouselistener.hpp>
#include <guichan/widget.hpp>

#include <list>
#include <vector>

#include "localconsts.h"

class Image;
class Resource;
class LinkHandler;

struct BROWSER_LINK final
{
    int x1, x2, y1, y2;     /**< Where link is placed */
    std::string link;
    std::string caption;
};

class LinePart final
{
    public:
        LinePart(const int x, const int y, const gcn::Color color,
                 const std::string &text, const bool bold) :
            mX(x), mY(y), mColor(color), mText(text), mType(0),
            mImage(nullptr), mBold(bold)
        {
        }

        LinePart(const int x, const int y, const gcn::Color color,
                 Image *const image) :
            mX(x), mY(y), mColor(color), mText(""), mType(1),
            mImage(image), mBold(false)
        {
        }

        ~LinePart();

        int mX, mY;
        gcn::Color mColor;
        std::string mText;
        unsigned char mType;
        Image *mImage;
        bool mBold;
};

/**
 * A simple browser box able to handle links and forward events to the
 * parent conteiner.
 */
class BrowserBox final : public gcn::Widget,
                         public gcn::MouseListener
{
    public:
        /**
         * Constructor.
         */
        BrowserBox(const unsigned int mode = AUTO_SIZE,
                   const bool opaque = true);

        A_DELETE_COPY(BrowserBox)

        /**
         * Destructor.
         */
        ~BrowserBox();

        /**
         * Sets the handler for links.
         */
        void setLinkHandler(LinkHandler *linkHandler);

        /**
         * Sets the BrowserBox opacity.
         */
        void setOpaque(bool opaque);

        /**
         * Sets the Highlight mode for links.
         */
        void setHighlightMode(const unsigned int highMode);

        /**
         * Sets the maximum numbers of rows in the browser box. 0 = no limit.
         */
        void setMaxRow(unsigned max)
        { mMaxRows = max; };

        /**
         * Disable links & user defined colors to be used in chat input.
         */
/*
        void disableLinksAndUserColors();
*/
        /**
         * Adds a text row to the browser.
         */
        void addRow(const std::string &row, const bool atTop = false);

        /**
         * Adds a menu line to the browser.
         */
        void addRow(const std::string &cmd, const char *const text);

        void addImage(const std::string &path);

        /**
         * Remove all rows.
         */
        void clearRows();

//        void setSize(int width, int height);

//        void widgetResized(const gcn::Event &event);

        /**
         * Handles mouse actions.
         */
        void mousePressed(gcn::MouseEvent &event) override;

        void mouseMoved(gcn::MouseEvent &event) override;

        /**
         * Draws the browser box.
         */
        void draw(gcn::Graphics *graphics) override;

        void updateHeight();

        /**
         * BrowserBox modes.
         */
        enum
        {
            AUTO_SIZE = 0,
            AUTO_WRAP       /**< Maybe it needs a fix or to be redone. */
        };

        enum
        {
            RED = 0,
            GREEN = 1,
            BLUE = 2,
            ORANGE = 3,
            YELLOW = 4,
            PINK = 5,
            PURPLE = 6,
            GRAY = 7,
            BROWN = 8,
            COLORS_MAX = 9
        };

        /**
         * Highlight modes for links.
         * This can be used for a bitmask.
         */
        enum
        {
            UNDERLINE  = 1,
            BACKGROUND = 2
        };

        typedef std::list<std::string> TextRows;

        TextRows &getRows()
        { return mTextRows; }

        bool hasRows() const
        { return !mTextRows.empty(); }

        void setAlwaysUpdate(bool n)
        { mAlwaysUpdate = n; }

        void setProcessVersion(bool n)
        { mProcessVersion = n; }

        void setEnableImages(bool n)
        { mEnableImages = n; }

        std::string getTextAtPos(const int x, const int y) const;

    private:
        int calcHeight();

        typedef TextRows::iterator TextRowIterator;
        typedef TextRows::const_iterator TextRowCIter;
        TextRows mTextRows;
        std::list<int> mTextRowLinksCount;

        typedef std::vector<LinePart> LinePartList;
        typedef LinePartList::iterator LinePartIterator;
        typedef LinePartList::const_iterator LinePartCIter;
        LinePartList mLineParts;

        typedef std::vector<BROWSER_LINK> Links;
        typedef Links::iterator LinkIterator;
        Links mLinks;

        LinkHandler *mLinkHandler;
        unsigned int mMode;
        unsigned int mHighMode;
        bool mOpaque;
        bool mUseLinksAndUserColors;
        int mSelectedLink;
        unsigned int mMaxRows;
        int mHeight;
        int mWidth;
        int mYStart;
        int mUpdateTime;
        bool mAlwaysUpdate;
        bool mProcessVersion;
        bool mEnableImages;

        gcn::Color mBackgroundColor;
        gcn::Color mHighlightColor;
        gcn::Color mHyperLinkColor;
        gcn::Color mColors[COLORS_MAX];
};

#endif
