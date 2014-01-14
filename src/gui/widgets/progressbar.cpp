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

#include "gui/widgets/progressbar.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include <guichan/font.hpp>

#include "debug.h"

int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(const Widget2 *const widget, float progress,
                         const int width, const int height,
                         const int backColor,
                         const std::string &skin, const std::string &skinFill):
    gcn::Widget(),
    Widget2(widget),
    gcn::WidgetListener(),
    mFillRect(),
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
    mRedraw(true)
{
    mBackgroundColor = Theme::getProgressColor(backColor >= 0
        ? backColor : 0, mProgress);
    mBackgroundColorToGo = mBackgroundColor;
    mForegroundColor2 = getThemeColor(Theme::PROGRESS_BAR_OUTLINE);

    // The progress value is directly set at load time:
    if (mProgress > 1.0F || mProgress < 0.0F)
        mProgress = 1.0F;

    mForegroundColor = getThemeColor(Theme::PROGRESS_BAR);
    addWidgetListener(this);
    setSize(width, height);

    Theme *const theme = Theme::instance();
    if (theme)
    {
        mSkin = theme->load(skin, "progressbar.xml");
        if (mSkin)
        {
            setPadding(mSkin->getPadding());
            mFillPadding = mSkin->getOption("fillPadding");
            mFillImage = mSkin->getOption("fillImage") != 0;
            if (mFillImage)
                theme->loadRect(mFillRect, skinFill, "progressbar_fill.xml");
        }
        setHeight(2 * mPadding + getFont()->getHeight() + 2);
    }

    mInstances++;
}

ProgressBar::~ProgressBar()
{
    if (gui)
        gui->removeDragged(this);

    mInstances--;
    Theme *const theme = Theme::instance();
    if (mSkin)
    {
        if (theme)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
    if (theme)
        theme->unloadRect(mFillRect);
    delete mVertexes;
    mVertexes = nullptr;
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
    const float alpha = std::max(client->getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());
    mAlpha = alpha;
}

void ProgressBar::draw(gcn::Graphics *graphics)
{
    BLOCK_START("ProgressBar::draw")
    updateAlpha();
    mBackgroundColor.a = static_cast<int>(mAlpha * 255);
    render(static_cast<Graphics*>(graphics));
    BLOCK_END("ProgressBar::draw")
}

void ProgressBar::setProgress(const float progress)
{
    const float p = std::min(1.0F, std::max(0.0F, progress));
    mProgressToGo = p;
    mRedraw = true;

    if (!mSmoothProgress)
        mProgress = p;

    if (mProgressPalette >= 0)
    {
        mBackgroundColorToGo = Theme::getProgressColor(
            mProgressPalette, progress);
    }
}

void ProgressBar::setProgressPalette(const int progressPalette)
{
    const int oldPalette = mProgressPalette;
    mProgressPalette = progressPalette;
    mRedraw = true;

    if (mProgressPalette != oldPalette && mProgressPalette >= 0)
    {
        mBackgroundColorToGo = Theme::getProgressColor(
            mProgressPalette, mProgressToGo);
    }
}

void ProgressBar::setBackgroundColor(const gcn::Color &color)
{
    mRedraw = true;
    mBackgroundColorToGo = color;

    if (!mSmoothColorChange)
        mBackgroundColor = color;
}

void ProgressBar::setColor(const gcn::Color &color1, const gcn::Color &color2)
{
    mForegroundColor = color1;
    mForegroundColor2 = color2;
}

void ProgressBar::render(Graphics *graphics)
{
    if (!mSkin)
        return;

    if (isBatchDrawRenders(openGLMode))
    {
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
                int width = static_cast<int>(mProgress
                    * static_cast<float>(maxWidth));
                if (width > 0)
                {
                    if (width > maxWidth)
                        width = maxWidth;
                    graphics->calcWindow(mVertexes, mFillPadding, mFillPadding,
                        width, mDimension.height - pad, mFillRect);
                }
            }
        }

        graphics->drawTileCollection(mVertexes);
    }
    else
    {
        graphics->drawImageRect(0, 0, mDimension.width, mDimension.height,
            mSkin->getBorder());
        if (mFillImage)
        {
            const unsigned int pad = 2 * mFillPadding;
            const int maxWidth = mDimension.width - pad;
            int width = static_cast<int>(mProgress
                * static_cast<float>(maxWidth));
            if (width > 0)
            {
                if (width > maxWidth)
                    width = maxWidth;
                graphics->drawImageRect(mFillPadding, mFillPadding,
                    width, mDimension.height - pad, mFillRect);
            }
        }
    }

    // The bar
    if (!mFillImage && mProgress > 0)
    {
        graphics->setColor(mBackgroundColor);
        const unsigned int pad = 2 * mFillPadding;
        const int maxWidth = mDimension.width - pad;
        int width = static_cast<int>(mProgress * static_cast<float>(maxWidth));
        if (width > 0)
        {
            if (width > maxWidth)
                width = maxWidth;
            graphics->fillRectangle(gcn::Rectangle(mFillPadding, mFillPadding,
                width, mDimension.height - pad));
        }
    }

    // The label
    if (!mText.empty())
    {
        const gcn::Color oldColor = graphics->getColor();

        gcn::Font *const font = gui->getFont();
        const int textX = mDimension.width / 2;
        const int textY = (mDimension.height - font->getHeight()) / 2;

        graphics->setColorAll(mForegroundColor, mForegroundColor2);
        font->drawString(graphics, mText, textX
            - font->getWidth(mText) / 2, textY);

        graphics->setColor(oldColor);
    }
}

void ProgressBar::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void ProgressBar::widgetMoved(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}
