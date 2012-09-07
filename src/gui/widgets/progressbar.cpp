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

ImageRect ProgressBar::mBorder;
int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(float progress,
                         const int width, const int height,
                         const int color):
    gcn::Widget(),
    WidgetListener(),
    mProgress(progress),
    mProgressToGo(progress),
    mSmoothProgress(true),
    mProgressPalette(color),
    mSmoothColorChange(true),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true)
{
    // The progress value is directly set at load time:
    if (progress > 1.0f || progress < 0.0f)
        progress = 1.0f;

    mColor = Theme::getProgressColor(color >= 0 ? color : 0, mProgress);
    mColorToGo = mColor;
    addWidgetListener(this);
    setSize(width, height);

    if (mInstances == 0)
    {
        for (int f = 0; f < 9; f ++)
            mBorder.grid[f] = nullptr;

        if (Theme::instance())
            Theme::instance()->loadRect(mBorder, "progressbar.xml", "");
    }

    mInstances++;
}

ProgressBar::~ProgressBar()
{
    mInstances--;
    if (mInstances == 0 && Theme::instance())
    {
        Theme::instance()->unloadRect(mBorder);
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
        for (int i = 0; i < 9; i++)
        {
            if (mBorder.grid[i])
                mBorder.grid[i]->setAlpha(mAlpha);
        }
    }

}

void ProgressBar::draw(gcn::Graphics *graphics)
{
    updateAlpha();

    mColor.a = static_cast<int>(mAlpha * 255);

    gcn::Rectangle rect = getDimension();
    rect.x = 0;
    rect.y = 0;

    render(static_cast<Graphics*>(graphics), rect, mColor,
           mProgress, mText, mVertexes, &mRedraw);
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

void ProgressBar::render(Graphics *graphics, const gcn::Rectangle &area,
                         const gcn::Color &color, const float progress,
                         const std::string &text, GraphicsVertexes *const vert,
                         bool *const redraw)
{
    gcn::Font *const oldFont = graphics->getFont();
    const gcn::Color oldColor = graphics->getColor();

    if (*redraw || graphics->getRedraw())
    {
        *redraw = false;
        graphics->calcWindow(vert, area.x, area.y,
            area.width, area.height, mBorder);
    }

    graphics->drawImageRect2(vert, mBorder);

    // The bar
    if (progress > 0)
    {
        graphics->setColor(color);
        graphics->fillRectangle(gcn::Rectangle(area.x + 4, area.y + 4,
            static_cast<int>(progress * static_cast<float>(area.width - 8)),
            area.height - 8));
    }

    // The label
    if (!text.empty())
    {
        const int textX = area.x + area.width / 2;
        const int textY = area.y + (area.height - boldFont->getHeight()) / 2;

        TextRenderer::renderText(graphics, text, textX, textY,
                                 gcn::Graphics::CENTER,
                                 Theme::getThemeColor(Theme::PROGRESS_BAR),
                                 gui->getFont(), true, false);
    }

    graphics->setFont(oldFont);
    graphics->setColor(oldColor);
}

void ProgressBar::render(Graphics *graphics, const gcn::Rectangle &area,
                         const gcn::Color &color, const float progress,
                         const std::string &text)
{
    gcn::Font *const oldFont = graphics->getFont();
    const gcn::Color oldColor = graphics->getColor();

    graphics->drawImageRect(area, mBorder);

    // The bar
    if (progress > 0)
    {
        graphics->setColor(color);
        graphics->fillRectangle(gcn::Rectangle(area.x + 4, area.y + 4,
            static_cast<int>(progress * static_cast<float>(area.width - 8)),
            area.height - 8));
    }

    // The label
    if (!text.empty())
    {
        const int textX = area.x + area.width / 2;
        const int textY = area.y + (area.height - boldFont->getHeight()) / 2;

        TextRenderer::renderText(graphics, text, textX, textY,
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
