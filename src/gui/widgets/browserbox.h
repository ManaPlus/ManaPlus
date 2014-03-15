/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_BROWSERBOX_H
#define GUI_WIDGETS_BROWSERBOX_H

#include "listeners/mouselistener.h"

#include "gui/widgets/widget.h"

#include <list>
#include <vector>

#include "localconsts.h"

class Image;
class LinkHandler;

struct BrowserLink final
{
    BrowserLink() :
        x1(0),
        x2(0),
        y1(0),
        y2(0),
        link(),
        caption()
    {
    }

    int x1;
    int x2;
    int y1;
    int y2;
    std::string link;
    std::string caption;
};

class LinePart final
{
    public:
        LinePart(const int x, const int y, const Color &color,
                 const Color &color2, const std::string &text,
                 const bool bold) :
            mX(x),
            mY(y),
            mColor(color),
            mColor2(color2),
            mText(text),
            mType(0),
            mImage(nullptr),
            mBold(bold)
        {
        }

        LinePart(const int x, const int y, const Color &color,
                 const Color &color2, Image *const image) :
            mX(x),
            mY(y),
            mColor(color),
            mColor2(color2),
            mText(),
            mType(1),
            mImage(image),
            mBold(false)
        {
        }

        ~LinePart();

        int mX, mY;
        Color mColor;
        Color mColor2;
        std::string mText;
        unsigned char mType;
        Image *mImage;
        bool mBold;
};

/**
 * A simple browser box able to handle links and forward events to the
 * parent conteiner.
 */
class BrowserBox final : public Widget,
                         public MouseListener
{
    public:
        /**
         * Constructor.
         */
        BrowserBox(const Widget2 *const widget,
                   const unsigned int mode,
                   const bool opaque,
                   const std::string &skin);

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
         * Sets the maximum numbers of rows in the browser box. 0 = no limit.
         */
        void setMaxRow(unsigned max)
        { mMaxRows = max; }

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

        /**
         * Handles mouse actions.
         */
        void mousePressed(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent& event) override final;

        /**
         * Draws the browser box.
         */
        void draw(Graphics *graphics) override final;

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
            BLACK,
            GREEN,
            BLUE,
            ORANGE,
            YELLOW,
            PINK,
            PURPLE,
            GRAY,
            BROWN,
            COLORS_MAX
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

        TextRows &getRows() A_WARN_UNUSED
        { return mTextRows; }

        bool hasRows() const A_WARN_UNUSED
        { return !mTextRows.empty(); }

        void setAlwaysUpdate(bool n)
        { mAlwaysUpdate = n; }

        void setProcessVersion(bool n)
        { mProcessVersion = n; }

        void setEnableImages(bool n)
        { mEnableImages = n; }

        void setEnableKeys(bool n)
        { mEnableKeys = n; }

        void setEnableTabs(bool n)
        { mEnableTabs = n; }

        std::string getTextAtPos(const int x, const int y) const A_WARN_UNUSED;

        int getPadding() const A_WARN_UNUSED
        { return mPadding; }

        void setForegroundColorAll(const Color &color1,
                                   const Color &color2);

        int getDataWidth() const
        { return mDataWidth; }

    private:
        int calcHeight() A_WARN_UNUSED;

        typedef TextRows::iterator TextRowIterator;
        typedef TextRows::const_iterator TextRowCIter;
        TextRows mTextRows;
        std::list<int> mTextRowLinksCount;

        typedef std::vector<LinePart> LinePartList;
        typedef LinePartList::iterator LinePartIterator;
        typedef LinePartList::const_iterator LinePartCIter;
        LinePartList mLineParts;

        typedef std::vector<BrowserLink> Links;
        typedef Links::iterator LinkIterator;
        Links mLinks;

        LinkHandler *mLinkHandler;
        Skin *mSkin;
        unsigned int mMode;
        unsigned int mHighMode;
        int mSelectedLink;
        unsigned int mMaxRows;
        int mHeight;
        int mWidth;
        int mYStart;
        int mUpdateTime;
        int mPadding;
        int mNewLinePadding;
        int mItemPadding;
        unsigned int mDataWidth;

        Color mHighlightColor;
        Color mHyperLinkColor;
        Color mColors[2][COLORS_MAX];

        bool mOpaque;
        bool mUseLinksAndUserColors;
        bool mUseEmotes;
        bool mAlwaysUpdate;
        bool mProcessVersion;
        bool mEnableImages;
        bool mEnableKeys;
        bool mEnableTabs;

        static ImageSet *mEmotes;
        static int mInstances;
};

#endif  // GUI_WIDGETS_BROWSERBOX_H
