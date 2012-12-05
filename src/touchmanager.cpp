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

#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "mouseinput.h"
#include "touchactions.h"

#include "gui/theme.h"

#include "debug.h"

TouchManager touchManager;

extern int openGLMode;

TouchManager::TouchManager() :
    mKeyboard(nullptr),
    mPad(nullptr),
    mAttack(nullptr),
    mVertexes(new ImageCollection),
    mRedraw(true)
{
    for (int f = 0; f < actionsSize; f ++)
        mActions[f] = false;
}

TouchManager::~TouchManager()
{
    clear();
}

void TouchManager::init()
{
#ifdef ANDROID
    loadTouchItem(&mKeyboard, "keyboard_icon.xml", NORMAL,
        nullptr, nullptr, &showKeyboard, nullptr);
#endif

    if (config.getBoolValue("showScreenJoystick"))
    {
        loadTouchItem(&mPad, "dpad.xml", LEFT,
            &padEvents, &padClick, &padUp, &padOut);
        loadTouchItem(&mAttack, "dpad_attack.xml", RIGHT,
            nullptr, &attackClick, &attackUp, &attackOut);
        loadTouchItem(&mCancel, "dpad_cancel.xml", RIGHT,
            nullptr, &cancelClick, &cancelUp, &cancelOut);
    }
}

void TouchManager::loadTouchItem(TouchItem **item, std::string name, int type,
                                 TouchFuncPtr fAll, TouchFuncPtr fPressed,
                                 TouchFuncPtr fReleased, TouchFuncPtr fOut)
{
    *item = nullptr;
    Theme *theme = Theme::instance();
    if (!theme)
        return;
    Skin *const skin = theme->load(name, "");
    if (skin)
    {
        const ImageRect &images = skin->getBorder();
        Image *image = images.grid[0];
        if (image)
        {
            image->incRef();
            int x = skin->getOption("x", 10);
            int y = skin->getOption("y", 10);
            const int pad = skin->getPadding();
            const int pad2 = 2 * pad;
            switch (type)
            {
                case LEFT:
                    y += (mainGraphics->mHeight - image->mBounds.h) / 2;
                    break;
                case RIGHT:
                    x = mainGraphics->mWidth - image->mBounds.w - pad2 - x;
                    y = mainGraphics->mHeight - image->mBounds.h - pad2 - y;
                    break;
                case NORMAL:
                default:
                    break;
            }
            *item = new TouchItem(gcn::Rectangle(x, y,
                image->getWidth() + pad2, image->getHeight() + pad2),
                image, x + pad, y + pad, fAll, fPressed, fReleased, fOut);
            mObjects.push_back(*item);
        }
        theme->unload(skin);
    }
    mRedraw = true;
}

void TouchManager::clear()
{
//    unloadTouchItem(&mPad);
//    unloadTouchItem(&mKeyboard);

    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        TouchItem *item = *it;
        if (item)
        {
            if (item->image)
                item->image->decRef();
            delete item;
        }
    }
    mObjects.clear();
    mRedraw = true;
}

void TouchManager::unloadTouchItem(TouchItem **item0)
{
    TouchItem *item = *item0;
    if (item)
    {
        if (item->image)
            item->image->decRef();
        delete item;
        *item0 = nullptr;
    }
    mRedraw = true;
}

void TouchManager::draw()
{
    if (openGLMode != 2)
    {
        if (mRedraw)
        {
            mRedraw = false;
            mVertexes->clear();
            for (TouchItemVectorCIter it = mObjects.begin(),
                 it_end = mObjects.end();
                 it != it_end; ++ it)
            {
                const TouchItem *const item = *it;
                if (item && item->image)
                {
                    mainGraphics->calcTile(mVertexes, item->image,
                        item->x, item->y);
                }
            }
        }
        mainGraphics->drawTile(mVertexes);
    }
    else
    {
        for (TouchItemVectorCIter it = mObjects.begin(),
             it_end = mObjects.end();
             it != it_end; ++ it)
        {
            const TouchItem *const item = *it;
            if (item && item->image)
                mainGraphics->drawImage(item->image, item->x, item->y);
        }
    }
}

bool TouchManager::processEvent(const MouseInput &mouseInput)
{
    const int x = mouseInput.getTouchX();
    const int y = mouseInput.getTouchY();

    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        const TouchItem *const item = *it;
        if (!item)
            continue;
        const gcn::Rectangle &rect = item->rect;
        if (rect.isPointInRect(x, y))
        {
            MouseInput event = mouseInput;
            event.setX(event.getTouchX() - item->x);
            event.setY(event.getTouchY() - item->y);
            if (item->funcAll)
                item->funcAll(event);

            switch (mouseInput.getType())
            {
                case gcn::MouseInput::PRESSED:
                    if (item->funcPressed)
                        item->funcPressed(event);
                    break;
                case gcn::MouseInput::RELEASED:
                    if (item->funcReleased)
                        item->funcReleased(event);
                    break;
                default:
                    break;
            }
            return true;
        }
        else if (item->funcOut)
        {
            item->funcOut(mouseInput);
        }
    }
    return false;
}

bool TouchManager::isActionActive(const int index) const
{
    if (index < 0 || index > actionsSize)
        return false;
    return mActions[index];
}
