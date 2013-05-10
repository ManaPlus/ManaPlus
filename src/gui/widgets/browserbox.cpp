/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/linkhandler.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include <guichan/graphics.hpp>
#include <guichan/font.hpp>
#include <guichan/cliprectangle.hpp>

#include <algorithm>

#include "debug.h"

Skin *BrowserBox::mSkin = nullptr;
int BrowserBox::mInstances = 0;

BrowserBox::BrowserBox(const Widget2 *const widget, const unsigned int mode,
                       const bool opaque) :
    gcn::Widget(),
    Widget2(widget),
    gcn::MouseListener(),
    mTextRows(),
    mTextRowLinksCount(),
    mLineParts(),
    mLinks(),
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
    mEnableImages(false),
    mPadding(0),
    mNewLinePadding(15),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND)),
    mHighlightColor(getThemeColor(Theme::HIGHLIGHT)),
    mHyperLinkColor(getThemeColor(Theme::HYPERLINK))
{
    setFocusable(true);
    addMouseListener(this);

    if (mInstances == 0)
    {
        if (Theme::instance())
            mSkin = Theme::instance()->load("browserbox.xml", "");
    }
    mInstances ++;

    if (mSkin)
    {
        mPadding = mSkin->getPadding();
        mNewLinePadding = mSkin->getOption("newLinePadding", 15);
    }

    mColors[0][RED] = getThemeColor(Theme::RED);
    mColors[0][GREEN] = getThemeColor(Theme::GREEN);
    mColors[0][BLUE] = getThemeColor(Theme::BLUE);
    mColors[0][ORANGE] = getThemeColor(Theme::ORANGE);
    mColors[0][YELLOW] = getThemeColor(Theme::YELLOW);
    mColors[0][PINK] = getThemeColor(Theme::PINK);
    mColors[0][PURPLE] = getThemeColor(Theme::PURPLE);
    mColors[0][GRAY] = getThemeColor(Theme::GRAY);
    mColors[0][BROWN] = getThemeColor(Theme::BROWN);

    mColors[1][RED] = getThemeColor(Theme::RED_OUTLINE);
    mColors[1][GREEN] = getThemeColor(Theme::GREEN_OUTLINE);
    mColors[1][BLUE] = getThemeColor(Theme::BLUE_OUTLINE);
    mColors[1][ORANGE] = getThemeColor(Theme::ORANGE_OUTLINE);
    mColors[1][YELLOW] = getThemeColor(Theme::YELLOW_OUTLINE);
    mColors[1][PINK] = getThemeColor(Theme::PINK_OUTLINE);
    mColors[1][PURPLE] = getThemeColor(Theme::PURPLE_OUTLINE);
    mColors[1][GRAY] = getThemeColor(Theme::GRAY_OUTLINE);
    mColors[1][BROWN] = getThemeColor(Theme::BROWN_OUTLINE);

    mForegroundColor = getThemeColor(Theme::BROWSERBOX);
    mForegroundColor2 = getThemeColor(Theme::BROWSERBOX_OUTLINE);
}

BrowserBox::~BrowserBox()
{
    if (gui)
        gui->removeDragged(this);

    mInstances --;
    if (mInstances == 0 && Theme::instance())
        Theme::instance()->unload(mSkin);
}

void BrowserBox::setLinkHandler(LinkHandler* linkHandler)
{
    mLinkHandler = linkHandler;
}

void BrowserBox::setOpaque(bool opaque)
{
    mOpaque = opaque;
}

void BrowserBox::setHighlightMode(const unsigned int highMode)
{
    mHighMode = highMode;
}

void BrowserBox::addRow(const std::string &row, const bool atTop)
{
    std::string tmp = row;
    std::string newRow;
    size_t idx1;
    const gcn::Font *const font = getFont();
    int linksCount = 0;

    if (getWidth() < 0)
        return;

    // Use links and user defined colors
    if (mUseLinksAndUserColors)
    {
        BrowserLink bLink;

        // Check for links in format "@@link|Caption@@"
        idx1 = tmp.find("@@");
        const int sz = static_cast<int>(mTextRows.size());
        while (idx1 != std::string::npos)
        {
            const size_t idx2 = tmp.find("|", idx1);
            const size_t idx3 = tmp.find("@@", idx2);

            if (idx2 == std::string::npos || idx3 == std::string::npos)
                break;
            bLink.link = tmp.substr(idx1 + 2, idx2 - (idx1 + 2));
            bLink.caption = tmp.substr(idx2 + 1, idx3 - (idx2 + 1));
            bLink.y1 = sz * font->getHeight();
            bLink.y2 = bLink.y1 + font->getHeight();

            newRow.append(tmp.substr(0, idx1));

            std::string tmp2 = newRow;
            idx1 = tmp2.find("##");
            while (idx1 != std::string::npos)
            {
                tmp2.erase(idx1, 3);
                idx1 = tmp2.find("##");
            }
            bLink.x1 = font->getWidth(tmp2) - 1;
            bLink.x2 = bLink.x1 + font->getWidth(bLink.caption) + 1;

            if (atTop)
                mLinks.insert(mLinks.begin(), bLink);
            else
                mLinks.push_back(bLink);
            linksCount ++;

            newRow.append("##<").append(bLink.caption);

            tmp.erase(0, idx3 + 2);
            if (!tmp.empty())
                newRow.append("##>");

            idx1 = tmp.find("@@");
        }

        newRow.append(tmp);
    }
    // Don't use links and user defined colors
    else
    {
        newRow = row;
    }

    if (mProcessVersion)
        newRow = replaceAll(newRow, "%VER%", SMALL_VERSION);

    if (atTop)
    {
        mTextRows.push_front(newRow);
        mTextRowLinksCount.push_front(linksCount);
    }
    else
    {
        mTextRows.push_back(newRow);
        mTextRowLinksCount.push_back(linksCount);
    }

    // discard older rows when a row limit has been set
    if (mMaxRows > 0 && !mTextRows.empty())
    {
        while (mTextRows.size() > mMaxRows)
        {
            mTextRows.pop_front();
            int cnt = mTextRowLinksCount.front();
            mTextRowLinksCount.pop_front();

            while (cnt && !mLinks.empty())
            {
                mLinks.erase(mLinks.begin());
                cnt --;
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
        const int w = font->getWidth(plain);
        if (w > getWidth())
            setWidth(w);
    }

    if (mMode == AUTO_WRAP)
    {
        unsigned int y = 0;
        unsigned int nextChar;
        const char *const hyphen = "~";
        const int hyphenWidth = font->getWidth(hyphen);
        unsigned x = 0;

        FOR_EACH (TextRowCIter, i, mTextRows)
        {
            std::string tempRow = *i;
            for (unsigned int j = 0, sz = static_cast<unsigned int>(
                 tempRow.size()); j < sz; j++)
            {
                std::string character = tempRow.substr(j, 1);
                x += font->getWidth(character);
                nextChar = j + 1;

                // Wraping between words (at blank spaces)
                if (nextChar < sz && tempRow.at(nextChar) == ' ')
                {
                    int nextSpacePos = static_cast<int>(
                        tempRow.find(" ", (nextChar + 1)));
                    if (nextSpacePos <= 0)
                        nextSpacePos = static_cast<int>(sz) - 1;

                    const unsigned nextWordWidth = font->getWidth(
                        tempRow.substr(nextChar,
                        (nextSpacePos - nextChar)));

                    if ((x + nextWordWidth + 10)
                        > static_cast<unsigned>(getWidth()))
                    {
                        x = mNewLinePadding;  // Ident in new line
                        y += 1;
                        j ++;
                    }
                }
                // Wrapping looong lines (brutal force)
                else if ((x + 2 * hyphenWidth)
                         > static_cast<unsigned>(getWidth()))
                {
                    x = mNewLinePadding;  // Ident in new line
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

void BrowserBox::addRow(const std::string &cmd, const char *const text)
{
    addRow(strprintf("@@%s|%s@@", encodeLinkText(cmd).c_str(),
        encodeLinkText(text).c_str()));
}

void BrowserBox::addImage(const std::string &path)
{
    if (!mEnableImages)
        return;

    mTextRows.push_back("~~~" + path);
    mTextRowLinksCount.push_back(0);
}

void BrowserBox::clearRows()
{
    mTextRows.clear();
    mTextRowLinksCount.clear();
    mLinks.clear();
    setWidth(0);
    setHeight(0);
    mSelectedLink = -1;
    mUpdateTime = 0;
    updateHeight();
}

struct MouseOverLink
{
    MouseOverLink(const int x, const int y) :
        mX(x), mY(y)
    { }

    bool operator() (const BrowserLink &link) const
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

    const LinkIterator i = std::find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    if (i != mLinks.end())
    {
        mLinkHandler->handleLink(i->link, &event);
        event.consume();
    }
}

void BrowserBox::mouseMoved(gcn::MouseEvent &event)
{
    const LinkIterator i = std::find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    mSelectedLink = (i != mLinks.end())
        ? static_cast<int>(i - mLinks.begin()) : -1;
}

void BrowserBox::draw(gcn::Graphics *graphics)
{
    BLOCK_START("BrowserBox::draw")
    const gcn::ClipRectangle cr = graphics->getCurrentClipArea();
    Graphics *const graphics2 = static_cast<Graphics *const>(graphics);
    mYStart = cr.y - cr.yOffset;
    const int yEnd = mYStart + cr.height;
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
                mLinks[mSelectedLink].y2 - mLinks[mSelectedLink].y1));
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

    gcn::Font *const font = getFont();

    FOR_EACH (LinePartCIter, i, mLineParts)
    {
        const LinePart &part = *i;
        if (part.mY + 50 < mYStart)
            continue;
        if (part.mY > yEnd)
            break;
        if (!part.mType)
        {
            graphics2->setColorAll(part.mColor, part.mColor2);
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

    BLOCK_END("BrowserBox::draw")
}

int BrowserBox::calcHeight()
{
    unsigned int y = mPadding;
    int wrappedLines = 0;
    int moreHeight = 0;
    int maxWidth = getWidth() - mPadding;
    int link = 0;
    bool bold = false;
    unsigned int wWidth = maxWidth;

    if (maxWidth < 0)
        return 1;

    const gcn::Font *const font = getFont();

    const int fontHeight = font->getHeight();
    const int fontWidthMinus = font->getWidth("-");
    const char *const hyphen = "~";
    const int hyphenWidth = font->getWidth(hyphen);

    gcn::Color selColor[2] = {mForegroundColor, mForegroundColor2};
    const gcn::Color textColor[2] = {mForegroundColor, mForegroundColor2};
    ResourceManager *const resman = ResourceManager::getInstance();

    mLineParts.clear();

    FOR_EACH (TextRowCIter, i, mTextRows)
    {
        unsigned int x = mPadding;
        const std::string row = *(i);
        bool wrapped = false;

        // Check for separator lines
        if (row.find("---", 0) == 0)
        {
            const int dashWidth = fontWidthMinus;
            for (x = mPadding; x < wWidth; x ++)
            {
                mLineParts.push_back(LinePart(x, y,
                    selColor[0], selColor[1], "-", false));
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
            Image *const img = resman->getImage(str);
            if (img)
            {
                img->incRef();
                mLineParts.push_back(LinePart(x, y,
                    selColor[0], selColor[1], img));
                y += img->getHeight() + 2;
                moreHeight += img->getHeight();
                if (img->getWidth() > maxWidth)
                    maxWidth = img->getWidth() + 2;
            }
            continue;
        }

        gcn::Color prevColor[2];
        prevColor[0] = selColor[0];
        prevColor[1] = selColor[1];
        bold = false;

        for (size_t start = 0, end = std::string::npos;
             start != std::string::npos;
             start = end, end = std::string::npos)
        {
            // Wrapped line continuation shall be indented
            if (wrapped)
            {
                y += fontHeight;
                x = mNewLinePadding + mPadding;
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
                    const signed char c = row.at(start + 2);

                    bool valid(false);
                    const gcn::Color col[2] =
                    {
                        getThemeCharColor(c, valid),
                        getThemeCharColor(c | 0x80, valid)
                    };

                    if (c == '>')
                    {
                        selColor[0] = prevColor[0];
                        selColor[1] = prevColor[1];
                    }
                    else if (c == '<')
                    {
                        prevColor[0] = selColor[0];
                        prevColor[1] = selColor[1];
                        selColor[0] = col[0];
                        selColor[1] = col[1];
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
                        selColor[0] = col[0];
                        selColor[1] = col[1];
                    }
                    else
                    {
                        switch (c)
                        {
                            case '1':
                                selColor[0] = mColors[0][RED];
                                selColor[1] = mColors[1][RED];
                                break;
                            case '2':
                                selColor[0] = mColors[0][GREEN];
                                selColor[1] = mColors[1][GREEN];
                                break;
                            case '3':
                                selColor[0] = mColors[0][BLUE];
                                selColor[1] = mColors[1][BLUE];
                                break;
                            case '4':
                                selColor[0] = mColors[0][ORANGE];
                                selColor[1] = mColors[1][ORANGE];
                                break;
                            case '5':
                                selColor[0] = mColors[0][YELLOW];
                                selColor[1] = mColors[1][YELLOW];
                                break;
                            case '6':
                                selColor[0] = mColors[0][PINK];
                                selColor[1] = mColors[1][PINK];
                                break;
                            case '7':
                                selColor[0] = mColors[0][PURPLE];
                                selColor[1] = mColors[1][PURPLE];
                                break;
                            case '8':
                                selColor[0] = mColors[0][GRAY];
                                selColor[1] = mColors[1][GRAY];
                                break;
                            case '9':
                                selColor[0] = mColors[0][BROWN];
                                selColor[1] = mColors[1][BROWN];
                                break;
                            case '0':
                            default:
                                selColor[0] = textColor[0];
                                selColor[1] = textColor[1];
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

            const size_t len = (end == std::string::npos) ? end : end - start;

            if (start >= row.length())
                break;

            std::string part = row.substr(start, len);

            int width = 0;
            if (bold)
                width = boldFont->getWidth(part);
            else
                width = font->getWidth(part);

            // Auto wrap mode
            if (mMode == AUTO_WRAP && wWidth > 0 && width > 0
                && (x + width + 10) > wWidth)
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
                        x += hyphenWidth;  // Account for the wrap-notifier
                        continue;
                    }

                    // Skip to the start of the current character
                    while ((row[end] & 192) == 128)
                        end--;
                    end--;  // And then to the last byte of the previous one

                    part = row.substr(start, end - start + 1);
                    if (bold)
                        width = boldFont->getWidth(part);
                    else
                        width = font->getWidth(part);
                }
                while (end > start && width > 0 && (x + width + 10) > wWidth);

                if (forced)
                {
                    x -= hyphenWidth;  // Remove the wrap-notifier accounting
                    mLineParts.push_back(LinePart(wWidth - hyphenWidth,
                        y, selColor[0], selColor[1], hyphen, bold));
                    end++;  // Skip to the next character
                }
                else
                {
                    end += 2;  // Skip to after the space
                }

                wrapped = true;
                wrappedLines++;
            }

            mLineParts.push_back(LinePart(x, y, selColor[0], selColor[1],
                part.c_str(), bold));

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
    if (static_cast<signed>(wWidth) != maxWidth)
    {
//        wWidth = maxWidth;
        setWidth(maxWidth);
    }

    return (static_cast<int>(mTextRows.size()) + wrappedLines)
        * fontHeight + moreHeight + 2 * mPadding;
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

std::string BrowserBox::getTextAtPos(const int x, const int y) const
{
    int textX = 0;
    int textY = 0;

    getAbsolutePosition(textX, textY);
    if (x < textX || y < textY)
        return "";

    textY = y - textY;
    std::string str;
    int lastY = 0;

    FOR_EACH (LinePartCIter, i, mLineParts)
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
            str.append(part.mText);
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
