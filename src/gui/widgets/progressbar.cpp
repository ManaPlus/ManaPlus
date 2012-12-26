/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "configuration.h"
#include "graphicsvertexes.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "utils/dtor.h"

#include <guichan/font.hpp>

#include "debug.h"

int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(const Widget2 *const widget, float progress,
                         const int width, const int height,
                         const int color):
    gcn::Widget(),
    Widget2(widget),
    gcn::WidgetListener(),
    mProgress(progress),
    mProgressToGo(progress),
    mSmoothProgress(true),
    mProgressPalette(color),
    mSmoothColorChange(true),
    mVertexes(new ImageCollection),
    mRedraw(true),
    mPadding(2),
    mFillPadding(3),
    mOutlineColor(getThemeColor(Theme::OUTLINE))
{
    // The progress value is directly set at load time:
    if (mProgress > 1.0f || mProgress < 0.0f)
        mProgress = 1.0f;

    mForegroundColor = getThemeColor(Theme::PROGRESS_BAR);
    mColor = Theme::getProgressColor(color >= 0 ? color : 0, mProgress);
    mColorToGo = mColor;
    addWidgetListener(this);
    setSize(width, height);

    if (Theme::instance())
    {
        mSkin = Theme::instance()->load("progressbar.xml", "");
        setPadding(mSkin->getPadding());
        mFillPadding = mSkin->getOption("fillPadding");
        setHeight(2 * mPadding + getFont()->getHeight() + 2);
    }

    mInstances++;
}

ProgressBar::~ProgressBar()
{
    mInstances--;
    if (mSkin)
    {
        if (Theme::instance())
            Theme::instance()->unload(mSkin);
        mSkin = nullptr;
    }
    delete mVertexes;
    mVertexes = nullptr;
}

void ProgressBar::logic()
{
    BLOCK_START("ProgressBar::logic")
    if (mSmoothColorChange && mColorToGo != mColor)
    {
        // Smoothly changing the color for a nicer effect.
        if (mColorToGo.r > mColor.r)
            mColor.r++;
        if (mColorToGo.g > mColor.g)
            mColor.g++;
        if (mColorToGo.b > mColor.b)
            mColor.b++;
        if (mColorToGo.r < mColor.r)
            mColor.r--;
        if (mColorToGo.g < mColor.g)
            mColor.g--;
        if (mColorToGo.b < mColor.b)
            mColor.b--;
    }

    if (mSmoothProgress && mProgressToGo != mProgress)
    {
        // Smoothly showing the progressbar changes.
        if (mProgressToGo > mProgress)
            mProgress = std::min(1.0f, mProgress + 0.005f);
        if (mProgressToGo < mProgress)
            mProgress = std::max(0.0f, mProgress - 0.005f);
    }
    BLOCK_END("ProgressBar::logic")
}

void ProgressBar::updateAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
/*
        ImageRect &rect = mSkin->getBorder();
        for (int i = 0; i < 9; i++)
        {
            if (rect.grid[i])
                rect.grid[i]->setAlpha(mAlpha);
        }
*/
    }

}

void ProgressBar::draw(gcn::Graphics *graphics)
{
    BLOCK_START("ProgressBar::draw")
    updateAlpha();
    mColor.a = static_cast<int>(mAlpha * 255);
    render(static_cast<Graphics*>(graphics));
    BLOCK_END("ProgressBar::draw")
}

void ProgressBar::setProgress(const float progress)
{
    const float p = std::min(1.0f, std::max(0.0f, progress));
    mProgressToGo = p;

    if (!mSmoothProgress)
        mProgress = p;

    if (mProgressPalette >= 0)
        mColorToGo = Theme::getProgressColor(mProgressPalette, progress);
}

void ProgressBar::setProgressPalette(const int progressPalette)
{
    const int oldPalette = mProgressPalette;
    mProgressPalette = progressPalette;

    if (mProgressPalette != oldPalette && mProgressPalette >= 0)
        mColorToGo = Theme::getProgressColor(mProgressPalette, mProgressToGo);
}

void ProgressBar::setColor(const gcn::Color &color)
{
    mColorToGo = color;

    if (!mSmoothColorChange)
        mColor = color;
}

void ProgressBar::render(Graphics *graphics)
{
    if (!mSkin)
        return;

    if (openGLMode != 2)
    {
        if (mRedraw || graphics->getRedraw())
        {
            mRedraw = false;
            mVertexes->clear();
            graphics->calcWindow(mVertexes, 0, 0,
                mDimension.width, mDimension.height, mSkin->getBorder());
        }

        graphics->drawTile(mVertexes);
    }
    else
    {
        graphics->drawImageRect(0, 0, mDimension.width, mDimension.height,
            mSkin->getBorder());
    }

    // The bar
    if (mProgress > 0)
    {
        graphics->setColor(mColor);
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

        graphics->setColor(mForegroundColor);
        graphics->setColor2(mOutlineColor);
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
