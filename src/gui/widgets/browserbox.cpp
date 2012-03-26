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

#include "gui/widgets/browserbox.h"

#include "client.h"
#include "graphics.h"
#include "logger.h"

#include "utils/stringutils.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"

#include "gui/widgets/linkhandler.h"

#include "resources/image.h"
#include "resources/resource.h"
#include "resources/resourcemanager.h"

#include <guichan/graphics.hpp>
#include <guichan/font.hpp>
#include <guichan/cliprectangle.hpp>

#include <algorithm>

#include "debug.h"

BrowserBox::BrowserBox(unsigned int mode, bool opaque):
    gcn::Widget(),
    mLinkHandler(nullptr),
    mMode(mode),
    mHighMode(UNDERLINE | BACKGROUND),
    mOpaque(opaque),
    mUseLinksAndUserColors(true),
    mSelectedLink(-1),
    mMaxRows(0),
    mHeight(0),
    mWidth(0),
    mYStart(0),
    mUpdateTime(-1),
    mAlwaysUpdate(true),
    mProcessVersion(false),
    mEnableImages(false)
{
    setFocusable(true);
    addMouseListener(this);

    mBackgroundColor = Theme::getThemeColor(Theme::BACKGROUND);
    mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
    mHyperLinkColor = Theme::getThemeColor(Theme::HYPERLINK);
    mColors[RED] = Theme::getThemeColor(Theme::RED);
    mColors[GREEN] = Theme::getThemeColor(Theme::GREEN);
    mColors[BLUE] = Theme::getThemeColor(Theme::BLUE);
    mColors[ORANGE] = Theme::getThemeColor(Theme::ORANGE);
    mColors[YELLOW] = Theme::getThemeColor(Theme::YELLOW);
    mColors[PINK] = Theme::getThemeColor(Theme::PINK);
    mColors[PURPLE] = Theme::getThemeColor(Theme::PURPLE);
    mColors[GRAY] = Theme::getThemeColor(Theme::GRAY);
    mColors[BROWN] = Theme::getThemeColor(Theme::BROWN);
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
}

BrowserBox::~BrowserBox()
{
}

void BrowserBox::setLinkHandler(LinkHandler* linkHandler)
{
    mLinkHandler = linkHandler;
}

void BrowserBox::setOpaque(bool opaque)
{
    mOpaque = opaque;
}

void BrowserBox::setHighlightMode(unsigned int highMode)
{
    mHighMode = highMode;
}

void BrowserBox::addRow(const std::string &row, bool atTop)
{
    std::string tmp = row;
    std::string newRow;
    std::string::size_type idx1, idx2, idx3;
    gcn::Font *font = getFont();

    if (getWidth() < 0)
        return;

    // Use links and user defined colors
    if (mUseLinksAndUserColors)
    {
        BROWSER_LINK bLink;

        // Check for links in format "@@link|Caption@@"
        idx1 = tmp.find("@@");
        while (idx1 != std::string::npos)
        {
            idx2 = tmp.find("|", idx1);
            idx3 = tmp.find("@@", idx2);

            if (idx2 == std::string::npos || idx3 == std::string::npos)
                break;
            bLink.link = tmp.substr(idx1 + 2, idx2 - (idx1 + 2));
            bLink.caption = tmp.substr(idx2 + 1, idx3 - (idx2 + 1));
            bLink.y1 = static_cast<int>(mTextRows.size()) * font->getHeight();
            bLink.y2 = bLink.y1 + font->getHeight();

            newRow += tmp.substr(0, idx1);

            std::string tmp2 = newRow;
            idx1 = tmp2.find("##");
            while (idx1 != std::string::npos)
            {
                tmp2.erase(idx1, 3);
                idx1 = tmp2.find("##");
            }
            bLink.x1 = font->getWidth(tmp2) - 1;
            bLink.x2 = bLink.x1 + font->getWidth(bLink.caption) + 1;

            mLinks.push_back(bLink);

            newRow += "##<" + bLink.caption;

            tmp.erase(0, idx3 + 2);
            if (!tmp.empty())
                newRow += "##>";

            idx1 = tmp.find("@@");
        }

        newRow += tmp;
    }
    // Don't use links and user defined colors
    else
    {
        newRow = row;
    }

    if (mProcessVersion)
        newRow = replaceAll(newRow, "%VER%", SMALL_VERSION);

    if (atTop)
        mTextRows.push_front(newRow);
    else
        mTextRows.push_back(newRow);

    //discard older rows when a row limit has been set
    if (mMaxRows > 0)
    {
        while (mTextRows.size() > mMaxRows)
        {
            mTextRows.pop_front();

            int yStart = 0;

            LinePartIterator i = mLineParts.begin();
            if (i != mLineParts.end())
            {
                ++ i;
                for (; i != mLineParts.end(); ++ i)
                {
                    const LinePart &part = *i;
                    if (!part.mX)
                    {
                        yStart = part.mY;
                        break;
                    }
                }
            }

            LinkIterator it = mLinks.begin();
            LinkIterator it_end = mLinks.end();
            while (it != it_end)
            {
                (*it).y1 -= yStart;
                (*it).y2 -= yStart;

                if ((*it).y1 < 0)
                    it = mLinks.erase(it);
                else
                    ++ it;
            }
        }
    }

    // Auto size mode
    if (mMode == AUTO_SIZE)
    {
        std::string plain = newRow;
        for (idx1 = plain.find("##");
             idx1 != std::string::npos;
             idx1 = plain.find("##"))
        {
            plain.erase(idx1, 3);
        }

        // Adjust the BrowserBox size
        int w = font->getWidth(plain);
        if (w > getWidth())
            setWidth(w);
    }

    if (mMode == AUTO_WRAP)
    {
        unsigned int y = 0;
        unsigned int nextChar;
        const char *hyphen = "~";
        int hyphenWidth = font->getWidth(hyphen);
        unsigned x = 0;

        for (TextRowIterator i = mTextRows.begin(); i != mTextRows.end(); ++i)
        {
            std::string tempRow = *i;
            for (unsigned int j = 0; j < tempRow.size(); j++)
            {
                std::string character = tempRow.substr(j, 1);
                x += font->getWidth(character);
                nextChar = j + 1;

                // Wraping between words (at blank spaces)
                if (nextChar < tempRow.size() && tempRow.at(nextChar) == ' ')
                {
                    int nextSpacePos = static_cast<int>(
                        tempRow.find(" ", (nextChar + 1)));
                    if (nextSpacePos <= 0)
                        nextSpacePos = static_cast<int>(tempRow.size()) - 1;

                    unsigned nextWordWidth = font->getWidth(
                        tempRow.substr(nextChar,
                        (nextSpacePos - nextChar)));

                    if ((x + nextWordWidth + 10)
                        > static_cast<unsigned>(getWidth()))
                    {
                        x = 15; // Ident in new line
                        y += 1;
                        j ++;
                    }
                }
                // Wrapping looong lines (brutal force)
                else if ((x + 2 * hyphenWidth)
                         > static_cast<unsigned>(getWidth()))
                {
                    x = 15; // Ident in new line
                    y += 1;
                }
            }
        }

        setHeight(font->getHeight() * (static_cast<int>(
            mTextRows.size()) + y));
    }
    else
    {
        setHeight(font->getHeight() * static_cast<int>(mTextRows.size()));
    }
    mUpdateTime = 0;
    updateHeight();
}

void BrowserBox::addRow(const std::string &cmd, char *text)
{
    addRow(strprintf("@@%s|%s@@", cmd.c_str(), text));
}

void BrowserBox::addImage(const std::string &path)
{
    if (!mEnableImages)
        return;

    mTextRows.push_back("~~~" + path);
}

void BrowserBox::clearRows()
{
    mTextRows.clear();
    mLinks.clear();
    setWidth(0);
    setHeight(0);
    mSelectedLink = -1;
    mUpdateTime = 0;
    updateHeight();
}

struct MouseOverLink
{
    MouseOverLink(int x, int y) : mX(x), mY(y)
    { }

    bool operator() (BROWSER_LINK &link) const
    {
        return (mX >= link.x1 && mX < link.x2 &&
                mY >= link.y1 && mY < link.y2);
    }
    int mX, mY;
};

void BrowserBox::mousePressed(gcn::MouseEvent &event)
{
    if (!mLinkHandler)
        return;

    LinkIterator i = find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    if (i != mLinks.end())
        mLinkHandler->handleLink(i->link, &event);
}

void BrowserBox::mouseMoved(gcn::MouseEvent &event)
{
    LinkIterator i = find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    mSelectedLink = (i != mLinks.end())
        ? static_cast<int>(i - mLinks.begin()) : -1;
}

void BrowserBox::draw(gcn::Graphics *graphics)
{
    gcn::ClipRectangle cr = graphics->getCurrentClipArea();
    Graphics *graphics2 = static_cast<Graphics*>(graphics);
    mYStart = cr.y - cr.yOffset;
    int yEnd = mYStart + cr.height;
    if (mYStart < 0)
        mYStart = 0;

    if (getWidth() != mWidth)
        updateHeight();

    if (mOpaque)
    {
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
    }

    if (mSelectedLink >= 0 && mSelectedLink
        < static_cast<signed>(mLinks.size()))
    {
        if ((mHighMode & BACKGROUND))
        {
            graphics->setColor(mHighlightColor);
            graphics->fillRectangle(gcn::Rectangle(
                mLinks[mSelectedLink].x1,
                mLinks[mSelectedLink].y1,
                mLinks[mSelectedLink].x2 - mLinks[mSelectedLink].x1,
                mLinks[mSelectedLink].y2 - mLinks[mSelectedLink].y1
                ));
        }

        if ((mHighMode & UNDERLINE))
        {
            graphics->setColor(mHyperLinkColor);
            graphics->drawLine(
                mLinks[mSelectedLink].x1,
                mLinks[mSelectedLink].y2,
                mLinks[mSelectedLink].x2,
                mLinks[mSelectedLink].y2);
        }
    }

    gcn::Font *font = getFont();

    for (LinePartIterator i = mLineParts.begin();
        i != mLineParts.end();
        ++i)
    {
        const LinePart &part = *i;
        if (part.mY + 50 < mYStart)
            continue;
        if (part.mY > yEnd)
            break;
        if (!part.mType)
        {
            graphics->setColor(part.mColor);
            if (part.mBold)
                boldFont->drawString(graphics, part.mText, part.mX, part.mY);
            else
                font->drawString(graphics, part.mText, part.mX, part.mY);
        }
        else if (part.mImage)
        {
            graphics2->drawImage(part.mImage, part.mX, part.mY);
        }
    }

    return;
}

int BrowserBox::calcHeight()
{
    unsigned x = 0, y = 0;
    int wrappedLines = 0;
    int moreHeight = 0;
    int maxWidth = getWidth();
    int link = 0;
    bool bold = false;

    if (getWidth() < 0)
        return 1;

    gcn::Font *font = getFont();

    int fontHeight = font->getHeight();
    int fontWidthMinus = font->getWidth("-");
    char const *hyphen = "~";
    int hyphenWidth = font->getWidth(hyphen);

    gcn::Color selColor = getForegroundColor();
    const gcn::Color textColor = getForegroundColor();
    ResourceManager *resman = ResourceManager::getInstance();

    mLineParts.clear();

    for (TextRowIterator i = mTextRows.begin(); i != mTextRows.end(); ++i)
    {
        const std::string row = *(i);
        bool wrapped = false;
        x = 0;

        // Check for separator lines
        if (row.find("---", 0) == 0)
        {
            const int dashWidth = fontWidthMinus;
            for (x = 0; x < static_cast<unsigned>(getWidth()); x ++)
            {
                mLineParts.push_back(LinePart(x, y, selColor, "-", false));
                x += dashWidth - 2;
            }

            y += fontHeight;
            continue;
        }
        else if (mEnableImages && row.find("~~~", 0) == 0)
        {
            std::string str = row.substr(3);
            if (str.size() > 2 && str.substr(str.size() - 1) == "~")
                str = str.substr(0, str.size() - 1);
            Image *img = resman->getImage(str);
            if (img)
            {
                img->incRef();
                mLineParts.push_back(LinePart(x, y, selColor, img));
                y += img->getHeight() + 2;
                moreHeight += img->getHeight();
                if (img->getWidth() > maxWidth)
                    maxWidth = img->getWidth() + 2;
            }
            continue;
        }

        gcn::Color prevColor = selColor;
        bold = false;

        // TODO: Check if we must take texture size limits into account here
        // TODO: Check if some of the O(n) calls can be removed
        for (std::string::size_type start = 0, end = std::string::npos;
             start != std::string::npos;
             start = end, end = std::string::npos)
        {
            // Wrapped line continuation shall be indented
            if (wrapped)
            {
                y += fontHeight;
                x = 15;
                wrapped = false;
            }

            // "Tokenize" the string at control sequences
            if (mUseLinksAndUserColors)
                end = row.find("##", start + 1);

            if (mUseLinksAndUserColors ||
                (!mUseLinksAndUserColors && (start == 0)))
            {
                // Check for color change in format "##x", x = [L,P,0..9]
                if (row.find("##", start) == start && row.size() > start + 2)
                {
                    const char c = row.at(start + 2);

                    bool valid;
                    const gcn::Color col = Theme::getThemeColor(c, valid);

                    if (c == '>')
                    {
                        selColor = prevColor;
                    }
                    else if (c == '<')
                    {
//                        link++;
                        prevColor = selColor;
                        selColor = col;
                    }
                    else if (c == 'B')
                    {
                        bold = true;
                    }
                    else if (c == 'b')
                    {
                        bold = false;
                    }
                    else if (valid)
                    {
                        selColor = col;
                    }
                    else
                    {

                        switch (c)
                        {
                            case '1': selColor = mColors[RED]; break;
                            case '2': selColor = mColors[GREEN]; break;
                            case '3': selColor = mColors[BLUE]; break;
                            case '4': selColor = mColors[ORANGE]; break;
                            case '5': selColor = mColors[YELLOW]; break;
                            case '6': selColor = mColors[PINK]; break;
                            case '7': selColor = mColors[PURPLE]; break;
                            case '8': selColor = mColors[GRAY]; break;
                            case '9': selColor = mColors[BROWN]; break;
                            case '0':
                            default:
                                selColor = textColor;
                        }
                    }

                    if (c == '<' && link < static_cast<signed>(mLinks.size()))
                    {
                        const int size =
                            font->getWidth(mLinks[link].caption) + 1;

                        mLinks[link].x1 = x;
                        mLinks[link].y1 = y;
                        mLinks[link].x2 = mLinks[link].x1 + size;
                        mLinks[link].y2 = y + fontHeight - 1;
                        link++;
                    }
                    start += 3;

                    if (start == row.size())
                        break;
                }
            }

            std::string::size_type len =
                end == std::string::npos ? end : end - start;

            if (start >= row.length())
                break;

            std::string part = row.substr(start, len);

            int width = 0;
            if (bold)
                width = boldFont->getWidth(part);
            else
                width = font->getWidth(part);

            // Auto wrap mode
            if (mMode == AUTO_WRAP && getWidth() > 0 && width > 0
                && (x + width + 10) > static_cast<unsigned>(getWidth()))
            {
                bool forced = false;

                /* FIXME: This code layout makes it easy to crash remote
                   clients by talking garbage. Forged long utf-8 characters
                   will cause either a buffer underflow in substr or an
                   infinite loop in the main loop. */
                do
                {
                    if (!forced)
                        end = row.rfind(' ', end);

                    // Check if we have to (stupidly) force-wrap
                    if (end == std::string::npos || end <= start)
                    {
                        forced = true;
                        end = row.size();
                        x += hyphenWidth; // Account for the wrap-notifier
                        continue;
                    }

                    // Skip to the start of the current character
                    while ((row[end] & 192) == 128)
                        end--;
                    end--; // And then to the last byte of the previous one

                    part = row.substr(start, end - start + 1);
                    if (bold)
                        width = boldFont->getWidth(part);
                    else
                        width = font->getWidth(part);
                }
                while (end > start && width > 0 && (x + width + 10)
                       > static_cast<unsigned>(getWidth()));

                if (forced)
                {
                    x -= hyphenWidth; // Remove the wrap-notifier accounting
                    mLineParts.push_back(LinePart(getWidth() - hyphenWidth,
                        y, selColor, hyphen, bold));
                    end++; // Skip to the next character
                }
                else
                {
                    end += 2; // Skip to after the space
                }

                wrapped = true;
                wrappedLines++;
            }

            mLineParts.push_back(LinePart(x, y, selColor, part.c_str(), bold));

            if (bold)
                width = boldFont->getWidth(part);
            else
                width = font->getWidth(part);

            if (mMode == AUTO_WRAP && width == 0)
                break;

            x += width;
        }
        y += fontHeight;
    }
    if (getWidth() != maxWidth)
        setWidth(maxWidth);

    return (static_cast<int>(mTextRows.size()) + wrappedLines)
        * fontHeight + moreHeight;
}

void BrowserBox::updateHeight()
{
    if (mAlwaysUpdate || mUpdateTime != cur_time
        || mTextRows.size() < 3 || !mUpdateTime)
    {
        mWidth = getWidth();
        mHeight = calcHeight();
        setHeight(mHeight);
        mUpdateTime = cur_time;
    }
}

std::string BrowserBox::getTextAtPos(const int x, const int y)
{
    int textX = 0;
    int textY = 0;

    getAbsolutePosition(textX, textY);
    if (x < textX || y < textY)
        return ""; // mouse position ourside of correct widget (outside of tab)

    textX = x - textX;
    textY = y - textY;

    std::string str = "";

    int lastY = 0;

    for (LinePartIterator i = mLineParts.begin();
        i != mLineParts.end();
        ++i)
    {
        const LinePart &part = *i;
        if (part.mY + 50 < mYStart)
            continue;
        if (part.mY > textY)
            break;

        if (part.mY > lastY)
        {
            str = part.mText;
            lastY = part.mY;
        }
        else
        {
            str += part.mText;
        }
    }

    return str;
}

LinePart::~LinePart()
{
    if (mImage)
        mImage->decRef();
    mImage = nullptr;
}
