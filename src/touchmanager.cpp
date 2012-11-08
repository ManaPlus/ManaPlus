/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "touchmanager.h"

#include "graphics.h"
#include "touchactions.h"

#include "gui/theme.h"

#include "debug.h"

TouchManager touchManager;

TouchManager::TouchManager()
{
}

TouchManager::~TouchManager()
{
    clear();
}

void TouchManager::init()
{
#ifdef ANDROID
    Image *image = Theme::getImageFromThemeXml("keyboard_icon.xml", "");
    TouchItem *keyboard = new TouchItem(gcn::Rectangle(10, 10,
        image->getWidth(), image->getHeight()), image, 10, 10,
        nullptr, nullptr, &showKeyboard);
    mObjects.push_back(keyboard);
#endif
}

void TouchManager::clear()
{
    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        const TouchItem *const item = *it;
        if (item)
        {
            if (item->image)
                item->image->decRef();
            delete *it;
        }
    }
    mObjects.clear();
}

void TouchManager::draw()
{
    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        const TouchItem *const item = *it;
        if (item && item->image)
            mainGraphics->drawImage(item->image, item->x, item->y);
    }
}

bool TouchManager::processEvent(const gcn::MouseInput &mouseInput)
{
    const int x = mouseInput.getX();
    const int y = mouseInput.getY();

    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        const TouchItem *const item = *it;
        if (item && item->rect.isPointInRect(x, y))
        {
            if (item->funcAll)
            {
                item->funcAll(mouseInput);
            }
            else
            {
                switch (mouseInput.getType())
                {
                    case gcn::MouseInput::PRESSED:
                        if (item->funcPressed)
                            item->funcPressed(mouseInput);
                        break;
                    case gcn::MouseInput::RELEASED:
                        if (item->funcReleased)
                            item->funcReleased(mouseInput);
                        break;
                    default:
                        break;
                }
            }
            return true;
        }
    }
    return false;
}
