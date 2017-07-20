/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/widgets/staticbrowserbox.h"

#include "enums/gui/linkhighlightmode.h"

#include "gui/gui.h"
#include "gui/mouseoverlink.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "gui/widgets/browserbox.inc"
#include "gui/widgets/linkhandler.h"

#include "render/graphics.h"

#include "resources/imageset.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"
#include "resources/loaders/imagesetloader.h"

#include "utils/browserboxtools.h"
#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stdmove.h"
#include "utils/stringutils.h"
#include "utils/translation/podict.h"

#include <algorithm>

#include "debug.h"

ImageSet *StaticBrowserBox::mEmotes = nullptr;
int StaticBrowserBox::mInstances = 0;

StaticBrowserBox::StaticBrowserBox(const Widget2 *const widget,
                                   const Opaque opaque,
                                   const std::string &skin) :
    Widget(widget),
    MouseListener(),
    mTextRows(),
    mTextRowLinksCount(),
    mLineParts(),
    mLinks(),
    mLinkHandler(nullptr),
    mSkin(nullptr),
    mHighlightMode(0),
    mSelectedLink(-1),
    mHeight(0),
    mWidth(0),
    mYStart(0),
    mPadding(0),
    mNewLinePadding(15U),
    mItemPadding(0),
    mHighlightColor(getThemeColor(ThemeColorId::HIGHLIGHT)),
    mHyperLinkColor(getThemeColor(ThemeColorId::HYPERLINK)),
    mOpaque(opaque),
    mUseLinksAndUserColors(true),
    mUseEmotes(true),
    mProcessVars(false),
    mEnableImages(false),
    mEnableKeys(false),
    mEnableTabs(false),
    mSeparator(false)
{
    mAllowLogic = false;

    setFocusable(true);
    addMouseListener(this);

    mBackgroundColor = getThemeColor(ThemeColorId::BACKGROUND);

    if (theme != nullptr)
        mSkin = theme->load(skin, "browserbox.xml");
    if (mInstances == 0)
    {
        mEmotes = Loader::getImageSet(
            "graphics/sprites/chatemotes.png", 17, 18);
    }
    mInstances ++;

    if (mSkin != nullptr)
    {
        mPadding = mSkin->getPadding();
        mNewLinePadding = CAST_U32(
            mSkin->getOption("newLinePadding", 15));
        mItemPadding = mSkin->getOption("itemPadding");
        if (mSkin->getOption("highlightBackground") != 0)
            mHighlightMode |= LinkHighlightMode::BACKGROUND;
        if (mSkin->getOption("highlightUnderline") != 0)
            mHighlightMode |= LinkHighlightMode::UNDERLINE;
    }

    readColor(BLACK);
    readColor(RED);
    readColor(GREEN);
    readColor(BLUE);
    readColor(ORANGE);
    readColor(YELLOW);
    readColor(PINK);
    readColor(PURPLE);
    readColor(GRAY);
    readColor(BROWN);

    mForegroundColor = getThemeColor(ThemeColorId::BROWSERBOX);
    mForegroundColor2 = getThemeColor(ThemeColorId::BROWSERBOX_OUTLINE);
}

StaticBrowserBox::~StaticBrowserBox()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    if (theme != nullptr)
    {
        theme->unload(mSkin);
        mSkin = nullptr;
    }

    mInstances --;
    if (mInstances == 0)
    {
        if (mEmotes != nullptr)
        {
            mEmotes->decRef();
            mEmotes = nullptr;
        }
    }
}

void StaticBrowserBox::setLinkHandler(LinkHandler* linkHandler)
{
    mLinkHandler = linkHandler;
}

void StaticBrowserBox::addSeparator(const std::string &row)
{
    if (mSeparator)
        return;
    addRow(row, false);
    mSeparator = true;
}

void StaticBrowserBox::addRow(const std::string &row,
                              const bool atTop)
{
    std::string tmp = row;
    std::string newRow;
    size_t idx1;
    const Font *const font = getFont();
    int linksCount = 0;

    if (getWidth() < 0)
        return;

    mSeparator = false;

    if (mProcessVars)
    {
        BrowserBoxTools::replaceVars(tmp);
    }

    // Use links and user defined colors
    if (mUseLinksAndUserColors)
    {
        BrowserLink bLink;

        // Check for links in format "@@link|Caption@@"
        const uint32_t sz = CAST_U32(mTextRows.size());

        if (mEnableKeys)
        {
            BrowserBoxTools::replaceKeys(tmp);
        }

        idx1 = tmp.find("@@");
        while (idx1 != std::string::npos)
        {
            const size_t idx2 = tmp.find('|', idx1);
            const size_t idx3 = tmp.find("@@", idx2);

            if (idx2 == std::string::npos || idx3 == std::string::npos)
                break;
            bLink.link = tmp.substr(idx1 + 2, idx2 - (idx1 + 2));
            bLink.caption = tmp.substr(idx2 + 1, idx3 - (idx2 + 1));
            bLink.y1 = CAST_S32(sz) * font->getHeight();
            bLink.y2 = bLink.y1 + font->getHeight();
            if (bLink.caption.empty())
            {
                bLink.caption = BrowserBoxTools::replaceLinkCommands(
                    bLink.link);
                if (translator != nullptr)
                    bLink.caption = translator->getStr(bLink.caption);
            }

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

    if (mEnableTabs)
    {
        BrowserBoxTools::replaceTabs(newRow);
    }

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

    std::string plain = STD_MOVE(newRow);
    // workaround if used only one string started from bold
    // width for this string can be calculated wrong
    // this workaround fix width if string start from bold sign
    const bool startBold = (plain.find("##B") == 0);
    for (idx1 = plain.find("##");
         idx1 != std::string::npos;
         idx1 = plain.find("##"))
    {
        plain.erase(idx1, 3);
    }

    // Adjust the StaticBrowserBox size. This need only for implementing "---"
    const int w = startBold ?
        boldFont->getWidth(plain) : font->getWidth(plain) + 2 * mPadding;
    if (w > getWidth())
        setWidth(w);
}

void StaticBrowserBox::addRow(const std::string &cmd,
                              const char *const text)
{
    addRow(strprintf("@@%s|%s@@", encodeLinkText(cmd).c_str(),
        encodeLinkText(text).c_str()));
}

void StaticBrowserBox::addImage(const std::string &path)
{
    if (!mEnableImages)
        return;

    mTextRows.push_back("~~~" + path);
    mTextRowLinksCount.push_back(0);
}

void StaticBrowserBox::clearRows()
{
    mTextRows.clear();
    mTextRowLinksCount.clear();
    mLinks.clear();
    setWidth(0);
    setHeight(0);
    mSelectedLink = -1;
}

void StaticBrowserBox::mousePressed(MouseEvent &event)
{
    if (mLinkHandler == nullptr)
        return;

    const LinkIterator i = std::find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    if (i != mLinks.end())
    {
        mLinkHandler->handleLink(i->link, &event);
        event.consume();
    }
}

void StaticBrowserBox::mouseMoved(MouseEvent &event)
{
    const LinkIterator i = std::find_if(mLinks.begin(), mLinks.end(),
        MouseOverLink(event.getX(), event.getY()));

    mSelectedLink = (i != mLinks.end())
        ? CAST_S32(i - mLinks.begin()) : -1;
}

void StaticBrowserBox::mouseExited(MouseEvent &event A_UNUSED)
{
    mSelectedLink = -1;
}

void StaticBrowserBox::draw(Graphics *const graphics)
{
    BLOCK_START("StaticBrowserBox::draw")
    const ClipRect &cr = graphics->getTopClip();
    mYStart = cr.y - cr.yOffset;
    const int yEnd = mYStart + cr.height;
    if (mYStart < 0)
        mYStart = 0;

    if (mDimension.width != mWidth)
    {
        updateHeight();
        reportAlways("browserbox resize in draw: %d, %d",
            mDimension.width,
            mWidth);
    }

    if (mOpaque == Opaque_true)
    {
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(0, 0,
            mDimension.width, mDimension.height));
    }

    if (mSelectedLink >= 0 &&
        mSelectedLink < CAST_S32(mLinks.size()))
    {
        if ((mHighlightMode & LinkHighlightMode::BACKGROUND) != 0u)
        {
            BrowserLink &link = mLinks[CAST_SIZE(mSelectedLink)];
            graphics->setColor(mHighlightColor);
            graphics->fillRectangle(Rect(
                link.x1,
                link.y1,
                link.x2 - link.x1,
                link.y2 - link.y1));
        }

        if ((mHighlightMode & LinkHighlightMode::UNDERLINE) != 0u)
        {
            BrowserLink &link = mLinks[CAST_SIZE(mSelectedLink)];
            graphics->setColor(mHyperLinkColor);
            graphics->drawLine(
                link.x1,
                link.y2,
                link.x2,
                link.y2);
        }
    }

    Font *const font = getFont();

    FOR_EACH (LinePartCIter, i, mLineParts)
    {
        const LinePart &part = *i;
        if (part.mY + 50 < mYStart)
            continue;
        if (part.mY > yEnd)
            break;
        if (part.mType == 0u)
        {
            if (part.mBold)
            {
                boldFont->drawString(graphics,
                    part.mColor,
                    part.mColor2,
                    part.mText,
                    part.mX, part.mY);
            }
            else
            {
                font->drawString(graphics,
                    part.mColor,
                    part.mColor2,
                    part.mText,
                    part.mX, part.mY);
            }
        }
        else if (part.mImage != nullptr)
        {
            graphics->drawImage(part.mImage, part.mX, part.mY);
        }
    }

    BLOCK_END("StaticBrowserBox::draw")
}

void StaticBrowserBox::safeDraw(Graphics *const graphics)
{
    StaticBrowserBox::draw(graphics);
}

void StaticBrowserBox::updateHeight()
{
    unsigned int y = CAST_U32(mPadding);
    int moreHeight = 0;
    int link = 0;
    bool bold = false;
    const unsigned int wWidth = CAST_U32(mDimension.width - mPadding);
    const Font *const font = getFont();
    const int fontHeight = font->getHeight() + 2 * mItemPadding;
    const int fontWidthMinus = font->getWidth("-");

    Color selColor[2] = {mForegroundColor, mForegroundColor2};
    const Color textColor[2] = {mForegroundColor, mForegroundColor2};
    mLineParts.clear();
    uint32_t dataWidth = 0;

    FOR_EACH (TextRowCIter, i, mTextRows)
    {
        unsigned int x = CAST_U32(mPadding);
        const std::string row = *(i);
        int objects = 0;

        // Check for separator lines
        if (row.find("---", 0) == 0)
        {
            const int dashWidth = fontWidthMinus;
            for (x = CAST_U32(mPadding); x < wWidth; x ++)
            {
                mLineParts.push_back(LinePart(CAST_S32(x),
                    CAST_S32(y) + mItemPadding,
                    selColor[0], selColor[1], "-", false));
                x += CAST_U32(CAST_S32(
                    dashWidth) - 2);
            }

            y += CAST_U32(fontHeight);
            continue;
        }
        else if (mEnableImages && row.find("~~~", 0) == 0)
        {
            std::string str = row.substr(3);
            const size_t sz = str.size();
            if (sz > 2 && str.substr(sz - 1) == "~")
                str = str.substr(0, sz - 1);
            Image *const img = Loader::getImage(str);
            if (img != nullptr)
            {
                img->incRef();
                mLineParts.push_back(LinePart(CAST_S32(x),
                    CAST_S32(y) + mItemPadding,
                    selColor[0], selColor[1], img));
                y += CAST_U32(img->getHeight() + 2);
                moreHeight += img->getHeight();
                if (img->getWidth() + mPadding + 2 > CAST_S32(dataWidth))
                    dataWidth = img->getWidth() + 2 + mPadding;
            }
            continue;
        }

        Color prevColor[2];
        prevColor[0] = selColor[0];
        prevColor[1] = selColor[1];
        bold = false;

        for (size_t start = 0, end = std::string::npos;
             start != std::string::npos;
             start = end, end = std::string::npos)
        {
            size_t idx1 = end;
            size_t idx2 = end;

            // "Tokenize" the string at control sequences
            if (mUseLinksAndUserColors)
                idx1 = row.find("##", start + 1);
            if (start == 0 || mUseLinksAndUserColors)
            {
                // Check for color change in format "##x", x = [L,P,0..9]
                if (row.find("##", start) == start && row.size() > start + 2)
                {
                    const signed char c = row.at(start + 2);

                    bool valid(false);
                    const Color col[2] =
                    {
                        getThemeCharColor(c, valid),
                        getThemeCharColor(CAST_S8(
                            c | 0x80), valid)
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
                            case '0':
                                selColor[0] = mColors[0][ColorName::BLACK];
                                selColor[1] = mColors[1][ColorName::BLACK];
                                break;
                            case '1':
                                selColor[0] = mColors[0][ColorName::RED];
                                selColor[1] = mColors[1][ColorName::RED];
                                break;
                            case '2':
                                selColor[0] = mColors[0][ColorName::GREEN];
                                selColor[1] = mColors[1][ColorName::GREEN];
                                break;
                            case '3':
                                selColor[0] = mColors[0][ColorName::BLUE];
                                selColor[1] = mColors[1][ColorName::BLUE];
                                break;
                            case '4':
                                selColor[0] = mColors[0][ColorName::ORANGE];
                                selColor[1] = mColors[1][ColorName::ORANGE];
                                break;
                            case '5':
                                selColor[0] = mColors[0][ColorName::YELLOW];
                                selColor[1] = mColors[1][ColorName::YELLOW];
                                break;
                            case '6':
                                selColor[0] = mColors[0][ColorName::PINK];
                                selColor[1] = mColors[1][ColorName::PINK];
                                break;
                            case '7':
                                selColor[0] = mColors[0][ColorName::PURPLE];
                                selColor[1] = mColors[1][ColorName::PURPLE];
                                break;
                            case '8':
                                selColor[0] = mColors[0][ColorName::GRAY];
                                selColor[1] = mColors[1][ColorName::GRAY];
                                break;
                            case '9':
                                selColor[0] = mColors[0][ColorName::BROWN];
                                selColor[1] = mColors[1][ColorName::BROWN];
                                break;
                            default:
                                selColor[0] = textColor[0];
                                selColor[1] = textColor[1];
                                break;
                        }
                    }

                    if (c == '<' && link < CAST_S32(mLinks.size()))
                    {
                        int size;
                        if (bold)
                        {
                            size = boldFont->getWidth(
                                mLinks[CAST_SIZE(link)].caption) + 1;
                        }
                        else
                        {
                            size = font->getWidth(
                                mLinks[CAST_SIZE(link)].caption) + 1;
                        }

                        BrowserLink &linkRef = mLinks[CAST_SIZE(
                            link)];
                        linkRef.x1 = CAST_S32(x);
                        linkRef.y1 = CAST_S32(y);
                        linkRef.x2 = linkRef.x1 + size;
                        linkRef.y2 = CAST_S32(y) + fontHeight - 1;
                        link++;
                    }

                    start += 3;
                    if (start == row.size())
                        break;
                }
            }
            if (mUseEmotes)
                idx2 = row.find("%%", start + 1);
            if (idx1 < idx2)
                end = idx1;
            else
                end = idx2;
            if (mUseEmotes)
            {
                // check for emote icons
                if (row.size() > start + 2 && row.substr(start, 2) == "%%")
                {
                    if (objects < 5)
                    {
                        const int cid = row.at(start + 2) - '0';
                        if (cid >= 0)
                        {
                            if (mEmotes != nullptr)
                            {
                                const size_t sz = mEmotes->size();
                                if (CAST_SIZE(cid) < sz)
                                {
                                    Image *const img = mEmotes->get(
                                        CAST_SIZE(cid));
                                    if (img != nullptr)
                                    {
                                        mLineParts.push_back(LinePart(
                                            CAST_S32(x),
                                            CAST_S32(y) + mItemPadding,
                                            selColor[0], selColor[1], img));
                                        x += 18;
                                    }
                                }
                            }
                        }
                        objects ++;
                    }

                    start += 3;
                    if (start == row.size())
                    {
                        if (x > dataWidth)
                            dataWidth = x;
                        break;
                    }
                }
            }
            const size_t len = (end == std::string::npos) ? end : end - start;

            if (start >= row.length())
                break;

            std::string part = row.substr(start, len);

            mLineParts.push_back(LinePart(CAST_S32(x),
                CAST_S32(y) + mItemPadding,
                selColor[0], selColor[1], part.c_str(), bold));

            int width = 0;
            if (bold)
                width = boldFont->getWidth(part);
            else
                width = font->getWidth(part);

            x += CAST_U32(width);
            if (x > dataWidth)
                dataWidth = x;
        }
        y += CAST_U32(fontHeight);
    }
    mWidth = dataWidth + mPadding;
    mHeight = CAST_S32(mTextRows.size())
        * fontHeight + moreHeight + 2 * mPadding;
    setSize(mWidth,
        mHeight);
}

std::string StaticBrowserBox::getTextAtPos(const int x,
                                           const int y) const
{
    int textX = 0;
    int textY = 0;

    getAbsolutePosition(textX, textY);
    if (x < textX || y < textY)
        return std::string();

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

void StaticBrowserBox::setForegroundColorAll(const Color &color1,
                                             const Color &color2)
{
    mForegroundColor = color1;
    mForegroundColor2 = color2;
}

void StaticBrowserBox::moveSelectionUp()
{
    if (mSelectedLink <= 0)
        mSelectedLink = CAST_S32(mLinks.size()) - 1;
    else
        mSelectedLink --;
}

void StaticBrowserBox::moveSelectionDown()
{
    mSelectedLink ++;
    if (mSelectedLink >= static_cast<signed int>(mLinks.size()))
        mSelectedLink = 0;
}

void StaticBrowserBox::selectSelection()
{
    if ((mLinkHandler == nullptr) ||
        mSelectedLink < 0 ||
        mSelectedLink >= static_cast<signed int>(mLinks.size()))
    {
        return;
    }

    mLinkHandler->handleLink(mLinks[CAST_SIZE(mSelectedLink)].link,
        nullptr);
}
