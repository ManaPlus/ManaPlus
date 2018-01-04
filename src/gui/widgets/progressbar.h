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

#ifndef GUI_WIDGETS_PROGRESSBAR_H
#define GUI_WIDGETS_PROGRESSBAR_H

#include "gui/fonts/textchunk.h"

#include "gui/widgets/widget.h"

#include "listeners/widgetlistener.h"

#include "resources/imagerect.h"

#include "localconsts.h"

class ImageCollection;
class Skin;

/**
 * A progress bar.
 *
 * \ingroup GUI
 */
class ProgressBar final : public Widget,
                          public WidgetListener
{
    public:
        /**
         * Constructor, initializes the progress with the given value.
         */
        ProgressBar(const Widget2 *const widget,
                    float progress,
                    const int width,
                    const int height,
                    const ProgressColorIdT backColor,
                    const std::string &skin,
                    const std::string &skinFill);

        A_DELETE_COPY(ProgressBar)

        ~ProgressBar() override final;

        /**
         * Performs progress bar logic (fading colors)
         */
        void logic() override final;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the progress bar.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        /**
         * Sets the current progress.
         */
        void setProgress(const float progress);

        /**
         * Returns the current progress.
         */
        float getProgress() const noexcept2 A_WARN_UNUSED
        { return mProgress; }

        /**
         * Change the ProgressPalette for this ProgressBar to follow or -1 to
         * disable this and manage color manually.
         */
        void setProgressPalette(const ProgressColorIdT progressPalette);

        /**
         * Change the color of the progress bar.
         */
        void setBackgroundColor(const Color &color);

        void setColor(const Color &color1, const Color &color2);

        /**
         * Returns the color of the progress bar.
         */
        const Color &getBackgroundColor() const noexcept2 A_WARN_UNUSED
        { return mBackgroundColor; }

        /**
         * Sets the text shown on the progress bar.
         */
        void setText(const std::string &str);

        /**
         * Returns the text shown on the progress bar.
         */
        const std::string &text() const noexcept2 A_WARN_UNUSED
        { return mText; }

        /**
         * Set whether the progress is moved smoothly.
         */
        void setSmoothProgress(bool smoothProgress) noexcept2
        { mSmoothProgress = smoothProgress; }

        /**
         * Set whether the color changing is made smoothly.
         */
        void setSmoothColorChange(bool smoothColorChange) noexcept2
        { mSmoothColorChange = smoothColorChange; }

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

        void widgetHidden(const Event &event) override final;

        void setPadding(unsigned int padding) noexcept2
        { mPadding = padding; }

    private:
        ImageRect mFillRect;
        TextChunk mTextChunk;
        Skin *mSkin;
        float mProgress;
        float mProgressToGo;

        Color mBackgroundColorToGo;

        std::string mText;
        ImageCollection *mVertexes A_NONNULLPOINTER;
        ProgressColorIdT mProgressPalette;
        unsigned int mPadding;
        unsigned int mFillPadding;

        static int mInstances;
        static float mAlpha;

        bool mFillImage;
        bool mSmoothProgress;
        bool mSmoothColorChange;
        bool mTextChanged;
};

#endif  // GUI_WIDGETS_PROGRESSBAR_H
