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
    mRedraw(true),
    mShowJoystick(false),
    mShowButtons(false),
    mButtonsSize(1),
    mJoystickSize(1),
    mShow(false),
    mInGame(false),
    mTempHideButtons(false)
{
    for (int f = 0; f < actionsSize; f ++)
        mActions[f] = false;
}

TouchManager::~TouchManager()
{
    config.removeListeners(this);
    clear();
    delete mVertexes;
    mVertexes = nullptr;
}

void TouchManager::init()
{
    config.addListener("showScreenJoystick", this);
    config.addListener("showScreenButtons", this);
    config.addListener("screenButtonsSize", this);
    config.addListener("screenJoystickSize", this);

    mShowJoystick = config.getBoolValue("showScreenJoystick");
    mShowButtons = config.getBoolValue("showScreenButtons");
    mButtonsSize = config.getIntValue("screenButtonsSize");
    mJoystickSize = config.getIntValue("screenJoystickSize");

    setHalfJoyPad(getPadSize() / 2);

#ifdef ANDROID
    loadTouchItem(&mKeyboard, "keyboard_icon.xml", "", -1, -1, 28, 28, NORMAL,
        nullptr, nullptr, &showKeyboard, nullptr);
#endif

    if (mShowJoystick)
        loadPad();
    if (mShowButtons)
        loadButtons();
}

void TouchManager::loadTouchItem(TouchItem **item, std::string name,
                                 std::string imageName,
                                 int x, int y, int width, int height, int type,
                                 TouchFuncPtr fAll, TouchFuncPtr fPressed,
                                 TouchFuncPtr fReleased, TouchFuncPtr fOut)
{
    *item = nullptr;
    Theme *theme = Theme::instance();
    if (!theme)
        return;
    ImageRect *images = new ImageRect;
    for (int f = 0; f < 9; f ++)
        images->grid[f] = nullptr;

    Image *icon;
    if (imageName.empty())
        icon = nullptr;
    else
        icon = Theme::getImageFromThemeXml(imageName, "");

    Skin *const skin = theme->loadSkinRect(*images, name, "");
    if (skin)
    {
        Image *image = images->grid[0];
        if (image)
        {
            if (x == -1)
                x = skin->getOption("x", 10);
            if (y == -1)
                y = skin->getOption("y", 10);
            const int pad = skin->getPadding();
            const int pad2 = 2 * pad;
            switch (type)
            {
                case LEFT:
                    y += (mainGraphics->mHeight - height) / 2;
                    break;
                case RIGHT:
                    x = mainGraphics->mWidth - width - pad2 - x;
                    y = mainGraphics->mHeight - height - pad2 - y;
                    break;
                case NORMAL:
                default:
                    break;
            }
            *item = new TouchItem(gcn::Rectangle(x, y,
                width + pad2, height + pad2), type,
                images, icon,  x + pad, y + pad, width, height,
                fAll, fPressed, fReleased, fOut);
            mObjects.push_back(*item);
        }
        theme->unload(skin);
    }
    mRedraw = true;
}

void TouchManager::clear()
{
    Theme *theme = Theme::instance();
    if (!theme)
        return;
    for (TouchItemVectorCIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        unload(*it);
    }
    mObjects.clear();
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
                if (item && item->images && (mShow || item == mKeyboard))
                {
                    mainGraphics->calcWindow(mVertexes, item->x, item->y,
                        item->width, item->height, *item->images);
                    const Image *const icon = item->icon;
                    if (icon)
                    {
                        mainGraphics->calcTile(mVertexes, icon,
                            item->x + (item->width - icon->mBounds.w) / 2,
                            item->y + (item->height - icon->mBounds.h) / 2);
                    }
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
            if (item && item->images && (mShow || item == mKeyboard))
            {
                mainGraphics->drawImageRect(item->x, item->y,
                    item->width, item->height, *item->images);
                const Image *const icon = item->icon;
                if (icon)
                {
                    mainGraphics->drawImage(icon,
                        item->x + (item->width - icon->mBounds.w) / 2,
                        item->y + (item->height - icon->mBounds.h) / 2);
                }
            }
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
        if (!item || (!mShow && item != mKeyboard))
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

void TouchManager::resize(int width, int height)
{
    mRedraw = true;
    for (TouchItemVectorCIter it = mObjects.begin(),
         it_end = mObjects.end(); it != it_end; ++ it)
    {
        TouchItem *const item = *it;
        if (!item)
            continue;

        switch (item->type)
        {
            case LEFT:
                if (height != mainGraphics->mHeight)
                {
                    item->y += (height - item->height) / 2;
                    item->rect.y += (height - item->rect.y) / 2;
                }
                break;
            case RIGHT:
            {
                const int diffW = width - mainGraphics->mWidth;
                const int diffH = height - mainGraphics->mHeight;
                item->x += diffW;
                item->rect.x += diffW;
                item->y += diffH;
                item->rect.y += diffH;
                break;
            }
            case NORMAL:
            default:
                break;
        }
    }
}

void TouchManager::unload(TouchItem *item)
{
    if (item)
    {
        Theme *const theme = Theme::instance();
        if (!theme)
            return;
        if (item->images)
        {
            theme->unloadRect(*item->images);
            delete item->images;
            item->images = nullptr;
            if (item->icon)
            {
                item->icon->decRef();
                item->icon = nullptr;
            }
        }
        delete item;
    }
}

void TouchManager::unloadTouchItem(TouchItem **unloadItem)
{
    Theme *theme = Theme::instance();
    if (!theme)
        return;
    for (TouchItemVectorIter it = mObjects.begin(), it_end = mObjects.end();
         it != it_end; ++ it)
    {
        TouchItem *item = *it;
        if (item && *unloadItem == item)
        {
            mObjects.erase(it);
            unload(item);
            return;
        }
    }
}

void TouchManager::loadPad()
{
    const int sz = (mJoystickSize + 2) * 50;
    loadTouchItem(&mPad, "dpad.xml", "dpad_image.xml", -1, -1, sz, sz, LEFT,
        &padEvents, &padClick, &padUp, &padOut);
}

void TouchManager::loadButtons()
{
    const int sz = (mButtonsSize + 1) * 50;
    Theme *theme = Theme::instance();
    if (!theme)
        return;
    Skin *const skin = theme->load("dbutton.xml", "");

    if (skin)
    {
        const int x = skin->getOption("x", 10);
        const int y = skin->getOption("y", 10);
        const int pad = skin->getPadding();
        const int pad2 = 2 * pad;
        const int skipWidth = pad2 + sz + x;

        loadTouchItem(&mAttack, "dbutton.xml", "dbutton_image.xml", x, y,
            sz, sz, RIGHT, nullptr, &attackClick, nullptr, nullptr);

        loadTouchItem(&mCancel, "dbutton.xml", "dbutton_image.xml",
            skipWidth, y, sz, sz, RIGHT, nullptr, &cancelClick,
            nullptr, nullptr);

        theme->unload(skin);
    }

}

void TouchManager::optionChanged(const std::string &value)
{
    logger->log("changed");
    if (value == "showScreenJoystick")
    {
        if (mShowJoystick == config.getBoolValue("showScreenJoystick"))
            return;
        mShowJoystick = config.getBoolValue("showScreenJoystick");
        if (mShowJoystick)
            loadPad();
        else
            unloadTouchItem(&mPad);
        mRedraw = true;
    }
    else if (value == "showScreenButtons")
    {
        if (mShowButtons == config.getBoolValue("showScreenButtons"))
            return;
        mShowButtons = config.getBoolValue("showScreenButtons");
        if (mShowButtons)
        {
            loadButtons();
        }
        else
        {
            unloadTouchItem(&mAttack);
            unloadTouchItem(&mCancel);
        }
        mRedraw = true;
    }
    else if (value == "screenButtonsSize")
    {
        if (mButtonsSize == config.getIntValue("screenButtonsSize"))
            return;
        mButtonsSize = config.getIntValue("screenButtonsSize");
        if (mShowButtons)
        {
            unloadTouchItem(&mAttack);
            unloadTouchItem(&mCancel);
            loadButtons();
        }
    }
    else if (value == "screenJoystickSize")
    {
        if (mJoystickSize == config.getIntValue("screenJoystickSize"))
            return;
        mJoystickSize = config.getIntValue("screenJoystickSize");
        setHalfJoyPad(getPadSize() / 2);
        if (mShowJoystick)
        {
            unloadTouchItem(&mPad);
            loadPad();
        }
    }
}

void TouchManager::setInGame(bool b)
{
    mInGame = b;
    mShow = mInGame && !mTempHideButtons;
    mRedraw = true;
}

void TouchManager::setTempHide(bool b)
{
    mTempHideButtons = b;
    mShow = mInGame && !mTempHideButtons;
    mRedraw = true;
}
