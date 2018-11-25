/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/progressbar.h"

#include "settings.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "utils/delete2.h"

#include "render/graphics.h"

#include "render/vertexes/imagecollection.h"

#include "debug.h"

int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(const Widget2 *const widget,
                         float progress,
                         const int width,
                         const int height,
                         const ProgressColorIdT backColor,
                         const std::string &skin,
                         const std::string &skinFill) :
    Widget(widget),
    WidgetListener(),
    mFillRect(),
    mTextChunk(),
    mSkin(nullptr),
    mProgress(progress),
    mProgressToGo(progress),
    mBackgroundColorToGo(),
    mText(),
    mVertexes(new ImageCollection),
    mProgressPalette(backColor),
    mPadding(2),
    mFillPadding(3),
    mFillImage(false),
    mSmoothProgress(true),
    mSmoothColorChange(true),
    mTextChanged(true)
{
    mBackgroundColor = Theme::getProgressColor(
        backColor >= ProgressColorId::PROG_HP
        ? backColor : ProgressColorId::PROG_HP,
        mProgress);
    mBackgroundColorToGo = mBackgroundColor;
    mForegroundColor2 = getThemeColor(ThemeColorId::PROGRESS_BAR_OUTLINE,
        255U);

    // The progress value is directly set at load time:
    if (mProgress > 1.0F || mProgress < 0.0F)
        mProgress = 1.0F;

    mForegroundColor = getThemeColor(ThemeColorId::PROGRESS_BAR, 255U);
    addWidgetListener(this);
    setSize(width, height);

    if (theme != nullptr)
    {
        mSkin = theme->load(skin,
            "progressbar.xml",
            true,
            Theme::getThemePath());
        if (mSkin != nullptr)
        {
            setPadding(mSkin->getPadding());
            mFillPadding = mSkin->getOption("fillPadding");
            mFillImage = mSkin->getOption("fillImage") != 0;
            if (mFillImage)
            {
                theme->loadRect(mFillRect,
                    skinFill,
                    "progressbar_fill.xml",
                    0,
                    8);
            }
        }
        setHeight(2 * mPadding + getFont()->getHeight() + 2);
    }

    mInstances++;
}

ProgressBar::~ProgressBar()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    mInstances--;
    if (mSkin != nullptr)
    {
        if (theme != nullptr)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
    Theme::unloadRect(mFillRect, 0, 8);
    delete2(mVertexes)
    mTextChunk.deleteImage();
}

void ProgressBar::logic()
{
    BLOCK_START("ProgressBar::logic")
    if (mSmoothColorChange && mBackgroundColorToGo != mBackgroundColor)
    {
        // Smoothly changing the color for a nicer effect.
        if (mBackgroundColorToGo.r > mBackgroundColor.r)
            mBackgroundColor.r++;
        if (mBackgroundColorToGo.g > mBackgroundColor.g)
            mBackgroundColor.g++;
        if (mBackgroundColorToGo.b > mBackgroundColor.b)
            mBackgroundColor.b++;
        if (mBackgroundColorToGo.r < mBackgroundColor.r)
            mBackgroundColor.r--;
        if (mBackgroundColorToGo.g < mBackgroundColor.g)
            mBackgroundColor.g--;
        if (mBackgroundColorToGo.b < mBackgroundColor.b)
            mBackgroundColor.b--;
        mRedraw = true;
    }

    if (mSmoothProgress && mProgressToGo != mProgress)
    {
        // Smoothly showing the progressbar changes.
        if (mProgressToGo > mProgress)
            mProgress = std::min(1.0F, mProgress + 0.005F);
        if (mProgressToGo < mProgress)
            mProgress = std::max(0.0F, mProgress - 0.005F);
        mRedraw = true;
    }
    BLOCK_END("ProgressBar::logic")
}

void ProgressBar::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());
    mAlpha = alpha;
}

void ProgressBar::draw(Graphics *const graphics)
{
    BLOCK_START("ProgressBar::draw")
    if (mSkin == nullptr)
    {
        BLOCK_END("ProgressBar::draw")
        return;
    }

    updateAlpha();
    mBackgroundColor.a = CAST_U32(mAlpha * 255);

    if (mRedraw || graphics->getRedraw())
    {
        mRedraw = false;
        mVertexes->clear();
        graphics->calcWindow(mVertexes, 0, 0,
            mDimension.width, mDimension.height, mSkin->getBorder());
        if (mFillImage)
        {
            const unsigned int pad = 2 * mFillPadding;
            const int maxWidth = mDimension.width - pad;
            int width = CAST_S32(mProgress
                * static_cast<float>(maxWidth));
            if (width > 0)
            {
                if (width > maxWidth)
                    width = maxWidth;
                graphics->calcWindow(mVertexes, mFillPadding, mFillPadding,
                    width, mDimension.height - pad, mFillRect);
            }
        }
        graphics->finalize(mVertexes);
    }

    graphics->drawTileCollection(mVertexes);

    // The bar
    if (!mFillImage && mProgress > 0)
    {
        graphics->setColor(mBackgroundColor);
        const unsigned int pad = 2 * mFillPadding;
        const int maxWidth = mDimension.width - pad;
        int width = CAST_S32(mProgress * static_cast<float>(maxWidth));
        if (width > 0)
        {
            if (width > maxWidth)
                width = maxWidth;
            graphics->fillRectangle(Rect(mFillPadding, mFillPadding,
                width, mDimension.height - pad));
        }
    }

    // The label
    if (!mText.empty())
    {
        Font *const font = gui->getFont();
        if (mTextChanged)
        {
            mTextChunk.textFont = font;
            mTextChunk.deleteImage();
            mTextChunk.text = mText;
            mTextChunk.color = mForegroundColor;
            mTextChunk.color2 = mForegroundColor2;
            font->generate(mTextChunk);
            mTextChanged = false;
        }

        const Image *const image = mTextChunk.img;
        if (image != nullptr)
        {
            const int textX = (mDimension.width - font->getWidth(mText)) / 2;
            const int textY = (mDimension.height - font->getHeight()) / 2;
            graphics->drawImage(image, textX, textY);
        }
    }
    BLOCK_END("ProgressBar::draw")
}

void ProgressBar::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ProgressBar::safeDraw")
    if (mSkin == nullptr)
    {
        BLOCK_END("ProgressBar::safeDraw")
        return;
    }

    updateAlpha();
    mBackgroundColor.a = CAST_U32(mAlpha * 255);

    graphics->drawImageRect(0, 0, mDimension.width, mDimension.height,
        mSkin->getBorder());
    if (mFillImage)
    {
        const unsigned int pad = 2 * mFillPadding;
        const int maxWidth = mDimension.width - pad;
        int width = CAST_S32(mProgress
            * static_cast<float>(maxWidth));
        if (width > 0)
        {
            if (width > maxWidth)
                width = maxWidth;
            graphics->drawImageRect(mFillPadding, mFillPadding,
                width, mDimension.height - pad, mFillRect);
        }
    }

    // The bar
    if (!mFillImage && mProgress > 0)
    {
        graphics->setColor(mBackgroundColor);
        const unsigned int pad = 2 * mFillPadding;
        const int maxWidth = mDimension.width - pad;
        int width = CAST_S32(mProgress * static_cast<float>(maxWidth));
        if (width > 0)
        {
            if (width > maxWidth)
                width = maxWidth;
            graphics->fillRectangle(Rect(mFillPadding, mFillPadding,
                width, mDimension.height - pad));
        }
    }

    // The label
    if (!mText.empty())
    {
        Font *const font = gui->getFont();
        if (mTextChanged)
        {
            mTextChunk.textFont = font;
            mTextChunk.deleteImage();
            mTextChunk.text = mText;
            mTextChunk.color = mForegroundColor;
            mTextChunk.color2 = mForegroundColor2;
            font->generate(mTextChunk);
            mTextChanged = false;
        }

        const Image *const image = mTextChunk.img;
        if (image != nullptr)
        {
            const int textX = (mDimension.width - font->getWidth(mText)) / 2;
            const int textY = (mDimension.height - font->getHeight()) / 2;
            graphics->drawImage(image, textX, textY);
        }
    }
    BLOCK_END("ProgressBar::safeDraw")
}

void ProgressBar::setProgress(const float progress)
{
    const float p = std::min(1.0F, std::max(0.0F, progress));
    mProgressToGo = p;
    mRedraw = true;

    if (!mSmoothProgress)
        mProgress = p;

    if (mProgressPalette >= ProgressColorId::PROG_HP)
    {
        mBackgroundColorToGo = Theme::getProgressColor(
            mProgressPalette, progress);
    }
}

void ProgressBar::setProgressPalette(const ProgressColorIdT progressPalette)
{
    const ProgressColorIdT oldPalette = mProgressPalette;
    mProgressPalette = progressPalette;
    mRedraw = true;

    if (mProgressPalette != oldPalette &&
        mProgressPalette >= ProgressColorId::PROG_HP)
    {
        mBackgroundColorToGo = Theme::getProgressColor(
            mProgressPalette, mProgressToGo);
    }
}

void ProgressBar::setBackgroundColor(const Color &color)
{
    mRedraw = true;
    mBackgroundColorToGo = color;

    if (!mSmoothColorChange)
        mBackgroundColor = color;
}

void ProgressBar::setColor(const Color &color1, const Color &color2)
{
    mForegroundColor = color1;
    mForegroundColor2 = color2;
    mTextChanged = true;
}

void ProgressBar::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void ProgressBar::widgetMoved(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void ProgressBar::setText(const std::string &str)
{
    if (mText != str)
    {
        mText = str;
        mTextChanged = true;
    }
}

void ProgressBar::widgetHidden(const Event &event A_UNUSED)
{
    mTextChanged = true;
    mTextChunk.deleteImage();
}
