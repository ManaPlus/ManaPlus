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

#include "gui/widgets/resizegrip.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"

#include "gui/theme.h"

#include "resources/image.h"

#include <guichan/graphics.hpp>

#include "debug.h"

Image *ResizeGrip::gripImage = nullptr;
int ResizeGrip::mInstances = 0;
float ResizeGrip::mAlpha = 1.0;

ResizeGrip::ResizeGrip(const std::string &image)
{
    if (mInstances == 0)
    {
        // Load the grip image
        gripImage = Theme::getImageFromThemeXml(image, "");

        if (gripImage)
            gripImage->setAlpha(mAlpha);
    }

    mInstances++;

    if (gripImage)
    {
        setWidth(gripImage->mBounds.w + 2);
        setHeight(gripImage->mBounds.h + 2);
    }
    else
    {
        setWidth(2);
        setHeight(2);
    }
}

ResizeGrip::~ResizeGrip()
{
    mInstances--;
    if (mInstances == 0 && gripImage)
        gripImage->decRef();
}

void ResizeGrip::draw(gcn::Graphics *graphics)
{
    if (!gripImage)
        return;

    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        gripImage->setAlpha(mAlpha);
    }

    static_cast<Graphics*>(graphics)->drawImage(gripImage, 0, 0);
}
