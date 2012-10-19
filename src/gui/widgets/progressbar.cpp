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
#include "graphics.h"
#include "graphicsvertexes.h"
#include "textrenderer.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "utils/dtor.h"

#include <guichan/font.hpp>

#include "debug.h"

int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(float progress,
                         const int width, const int height,
                         const int color):
    gcn::Widget(),
    gcn::WidgetListener(),
    mProgress(progress),
    mProgressToGo(progress),
    mSmoothProgress(true),
    mProgressPalette(color),
    mSmoothColorChange(true),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mPadding(2),
    mFillPadding(3)
{
    // The progress value is directly set at load time:
    if (mProgress > 1.0f || mProgress < 0.0f)
        mProgress = 1.0f;

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
    updateAlpha();

    mColor.a = static_cast<int>(mAlpha * 255);

    render(static_cast<Graphics*>(graphics));
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

    gcn::Font *const oldFont = graphics->getFont();
    const gcn::Color oldColor = graphics->getColor();

    if (mRedraw || graphics->getRedraw())
    {
        mRedraw = false;
        graphics->calcWindow(mVertexes, 0, 0,
            mDimension.width, mDimension.height, mSkin->getBorder());
    }

    graphics->drawImageRect2(mVertexes, mSkin->getBorder());

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
        const int textX = mDimension.width / 2;
        const int textY = (mDimension.height - boldFont->getHeight()) / 2;

        TextRenderer::renderText(graphics, mText, textX, textY,
                                 gcn::Graphics::CENTER,
                                 Theme::getThemeColor(Theme::PROGRESS_BAR),
                                 gui->getFont(), true, false);
    }

    graphics->setFont(oldFont);
    graphics->setColor(oldColor);
}

void ProgressBar::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}

void ProgressBar::widgetMoved(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
}
