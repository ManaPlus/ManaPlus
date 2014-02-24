/*
 *  Desktop widget
 *  Copyright (c) 2009-2010  The Mana World Development Team
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

#include "gui/widgets/desktop.h"

#include "configuration.h"
#include "main.h"

#include "gui/widgets/label.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"
#include "resources/wallpaper.h"

#include "debug.h"

Desktop::Desktop(const Widget2 *const widget) :
    Container(widget),
    WidgetListener(),
    mWallpaper(nullptr),
    mVersionLabel(nullptr),
    mSkin(nullptr),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND, 128)),
    mBackgroundGrayColor(getThemeColor(Theme::BACKGROUND_GRAY)),
    mShowBackground(true)
{
    addWidgetListener(this);

    Wallpaper::loadWallpapers();

    Theme *const theme = Theme::instance();
    if (theme)
        mSkin = theme->load("desktop.xml", "");

    if (mSkin)
        mShowBackground = mSkin->getOption("showBackground");

    const std::string appName = branding.getValue("appName", std::string());
    if (appName.empty())
    {
        mVersionLabel = new Label(this, FULL_VERSION);
    }
    else
    {
        mVersionLabel = new Label(this, strprintf("%s (%s)", FULL_VERSION,
            appName.c_str()));
    }

    mVersionLabel->setBackgroundColor(getThemeColor(Theme::BACKGROUND, 128));
}

Desktop::~Desktop()
{
    if (mWallpaper)
    {
        mWallpaper->decRef();
        mWallpaper = nullptr;
    }
    if (Theme::instance())
        Theme::instance()->unload(mSkin);
}

void Desktop::postInit()
{
    if (mSkin)
    {
        add(mVersionLabel, mSkin->getOption("versionX", 25),
            mSkin->getOption("versionY", 2));
    }
    else
    {
        add(mVersionLabel, 25, 2);
    }
}

void Desktop::reloadWallpaper()
{
    Wallpaper::loadWallpapers();
    setBestFittingWallpaper();
}

void Desktop::widgetResized(const Event &event A_UNUSED)
{
    setBestFittingWallpaper();
}

void Desktop::draw(Graphics *graphics)
{
    BLOCK_START("Desktop::draw")

    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    if (mWallpaper)
    {
        const int wallpWidth = mWallpaper->getWidth();
        const int wallpHeight = mWallpaper->getHeight();

        if (width > wallpWidth || height > wallpHeight)
        {
            graphics->setColor(mBackgroundGrayColor);
            graphics->fillRectangle(Rect(0, 0, width, height));
        }

        if (imageHelper->useOpenGL() == RENDER_SOFTWARE)
        {
            graphics->drawImage(mWallpaper,
                (width - wallpWidth) / 2,
                (height - wallpHeight) / 2);
        }
        else
        {
            graphics->drawRescaledImage(mWallpaper, 0, 0, width, height);
        }
    }
    else
    {
        graphics->setColor(mBackgroundGrayColor);
        graphics->fillRectangle(Rect(0, 0, width, height));
    }

    // Draw a thin border under the application version...
    graphics->setColor(mBackgroundColor);
    graphics->fillRectangle(Rect(mVersionLabel->getDimension()));

    Container::draw(graphics);
    BLOCK_END("Desktop::draw")
}

void Desktop::setBestFittingWallpaper()
{
    if (!mShowBackground || !config.getBoolValue("showBackground"))
        return;

    const std::string wallpaperName =
        Wallpaper::getWallpaper(getWidth(), getHeight());

    Image *const nWallPaper = Theme::getImageFromTheme(wallpaperName);

    if (nWallPaper)
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        if (mWallpaper)
        {
            resman->decRefDelete(mWallpaper);
            mWallpaper = nullptr;
        }

        const Rect &rect = mDimension;
        const int width = rect.width;
        const int height = rect.height;

        if (imageHelper->useOpenGL() == RENDER_SOFTWARE &&
            (nWallPaper->getWidth() != width
            || nWallPaper->getHeight() != height))
        {
            // We rescale to obtain a fullscreen wallpaper...
            Image *const newRsclWlPpr = resman->getRescaled(
                nWallPaper, width, height);

            if (newRsclWlPpr)
            {
                resman->decRefDelete(nWallPaper);
                // We replace the resource in the resource manager
                mWallpaper = newRsclWlPpr;
            }
            else
            {
                mWallpaper = nWallPaper;
            }
        }
        else
        {
            mWallpaper = nWallPaper;
        }
    }
    else
    {
        logger->log("Couldn't load %s as wallpaper", wallpaperName.c_str());
    }
}
