/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "input/touch/touchmanager.h"

#include "configuration.h"

#include "gui/gui.h"
#include "gui/skin.h"
#include "gui/theme.h"

#include "gui/fonts/font.h"

#include "input/inputmanager.h"
#include "input/mouseinput.h"

#include "input/touch/touchactions.h"

#include "utils/delete2.h"

#include "render/graphics.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "debug.h"

TouchManager touchManager;

extern RenderType openGLMode;

TouchManager::TouchManager() :
    mKeyboard(nullptr),
    mPad(nullptr),
    mObjects(),
    mVertexes(nullptr),
    mRedraw(true),
    mShowJoystick(false),
    mShowButtons(false),
    mShowKeyboard(false),
    mButtonsSize(1),
    mJoystickSize(1),
    mButtonsFormat(0),
    mWidth(0),
    mHeight(0),
    mShow(false),
    mInGame(false),
    mTempHideButtons(false)
{
    for (int f = 0; f < actionsSize; f ++)
        mActions[f] = false;
    for (int f = 0; f < buttonsCount; f ++)
        mButtons[f] = nullptr;
}

TouchManager::~TouchManager()
{
    clear();
    CHECKLISTENERS
}

void TouchManager::shutdown() restrict2
{
    config.removeListeners(this);
}

void TouchManager::init() restrict2
{
    delete mVertexes;
    mVertexes = new ImageCollection;

    config.addListener("showScreenJoystick", this);
    config.addListener("showScreenButtons", this);
    config.addListener("showScreenKeyboard", this);
    config.addListener("screenButtonsSize", this);
    config.addListener("screenJoystickSize", this);
    config.addListener("screenButtonsFormat", this);

    mShowJoystick = config.getBoolValue("showScreenJoystick");
    mShowButtons = config.getBoolValue("showScreenButtons");
    mShowKeyboard = config.getBoolValue("showScreenKeyboard");
    mButtonsSize = config.getIntValue("screenButtonsSize");
    mJoystickSize = config.getIntValue("screenJoystickSize");
    mButtonsFormat = config.getIntValue("screenButtonsFormat");

    setHalfJoyPad(getPadSize() / 2);

    if (mShowKeyboard)
        loadKeyboard();
    if (mShowJoystick)
        loadPad();
    if (mShowButtons)
        loadButtons();
    mWidth = mainGraphics->mWidth;
    mHeight = mainGraphics->mHeight;
}

void TouchManager::loadTouchItem(TouchItem **restrict item,
                                 const std::string &restrict name,
                                 const std::string &restrict imageName,
                                 const std::string &restrict text,
                                 int x, int y,
                                 const int width, const int height,
                                 const int type,
                                 const std::string &restrict eventPressed,
                                 const std::string &restrict eventReleased,
                                 const TouchFuncPtr fAll,
                                 const TouchFuncPtr fPressed,
                                 const TouchFuncPtr fReleased,
                                 const TouchFuncPtr fOut) restrict2
{
    *item = nullptr;
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
        Image *const image = images->grid[0];
        if (image)
        {
            if (x == -1)
                x = skin->getOption("x", 10);
            if (y == -1)
                y = skin->getOption("y", 10);
            const int pad = skin->getPadding();
            const int pad2 = 2 * pad;
            const int border = skin->getOption("clickborder");
            const int border2 = border * 2;
            const int diff = pad - border;
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
            *item = new TouchItem(text, Rect(x + diff, y + diff,
                width + border2, height + border2), type,
                eventPressed, eventReleased, images, icon,
                x + pad, y + pad, width, height,
                fAll, fPressed, fReleased, fOut);
            mObjects.push_back(*item);
        }
        else
        {
            delete images;
        }
        theme->unload(skin);
    }
    else
    {
        delete images;
    }
    mRedraw = true;
}

void TouchManager::clear() restrict2
{
    FOR_EACH (TouchItemVectorCIter, it, mObjects)
        unload(*it);
    mObjects.clear();
    delete2(mVertexes);
    mRedraw = true;
}

void TouchManager::draw() restrict2
{
    if (mRedraw)
    {
        mRedraw = false;
        mVertexes->clear();
        FOR_EACH (TouchItemVectorCIter, it, mObjects)
        {
            const TouchItem *const item = *it;
            if (item && item->images && (mShow ||
                (item == mKeyboard && mShowKeyboard)))
            {
                mainGraphics->calcWindow(mVertexes, item->x, item->y,
                    item->width, item->height, *item->images);
                const Image *const icon = item->icon;
                if (icon)
                {
                    mainGraphics->calcTileCollection(mVertexes, icon,
                        item->x + (item->width - icon->mBounds.w) / 2,
                        item->y + (item->height - icon->mBounds.h) / 2);
                }
            }
        }
        mainGraphics->finalize(mVertexes);
    }
    mainGraphics->drawTileCollection(mVertexes);
    drawText();
}

void TouchManager::safeDraw() restrict2
{
    FOR_EACH (TouchItemVectorCIter, it, mObjects)
    {
        const TouchItem *const item = *it;
        if (item && item->images && (mShow ||
            (item == mKeyboard && mShowKeyboard)))
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
    drawText();
}

void TouchManager::drawText() restrict2
{
    if (!gui)
        return;

    Font *const font = boldFont;
    const Color &color1 = theme->getColor(ThemeColorId::TEXT, 255);
    const Color &color2 = theme->getColor(ThemeColorId::TEXT_OUTLINE, 255);

    FOR_EACH (TouchItemVectorCIter, it, mObjects)
    {
        const TouchItem *const item = *it;
        if (item && mShow && !item->text.empty())
        {
            const std::string str = item->text;
            const int textX = (item->rect.width - font->getWidth(str))
                / 2 + item->x;
            const int textY = (item->rect.height - font->getHeight())
                / 2 + item->y;
            font->drawString(mainGraphics,
                color1,
                color2,
                str, textX, textY);
        }
    }
}

bool TouchManager::processEvent(const MouseInput &mouseInput) restrict2
{
    const int x = mouseInput.getTouchX();
    const int y = mouseInput.getTouchY();

    FOR_EACH (TouchItemVectorCIter, it, mObjects)
    {
        const TouchItem *const item = *it;
        if (!item || (!mShow && (item != mKeyboard || !mShowKeyboard)))
            continue;
        const Rect &rect = item->rect;
        if (rect.isPointInRect(x, y))
        {
            MouseInput event = mouseInput;
            event.setX(event.getTouchX() - item->x);
            event.setY(event.getTouchY() - item->y);
            if (item->funcAll)
                item->funcAll(event);

            switch (mouseInput.getType())
            {
                case MouseEventType::PRESSED:
                    if (!item->eventPressed.empty())
                        executeAction(item->eventPressed);
                    else if (item->funcPressed)
                        item->funcPressed(event);
                    break;
                case MouseEventType::RELEASED:
                    if (!item->eventReleased.empty())
                        executeAction(item->eventReleased);
                    else if (item->funcReleased)
                        item->funcReleased(event);
                    break;
                default:
                case MouseEventType::MOVED:
                case MouseEventType::WHEEL_MOVED_DOWN:
                case MouseEventType::WHEEL_MOVED_UP:
                case MouseEventType::CLICKED:
                case MouseEventType::ENTERED:
                case MouseEventType::EXITED:
                case MouseEventType::DRAGGED:
                case MouseEventType::RELEASED2:
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

bool TouchManager::isActionActive(const InputActionT index) restrict2 const
{
    if (CAST_S32(index) < 0 ||
        CAST_S32(index) >= actionsSize)
    {
        return false;
    }
    return mActions[CAST_SIZE(index)];
}

void TouchManager::resize(const int width, const int height) restrict2
{
    mRedraw = true;
    const int maxHeight = mHeight;
    const int diffW = width - mWidth;
    const int diffH = height - maxHeight;
    FOR_EACH (TouchItemVectorCIter, it, mObjects)
    {
        TouchItem *const item = *it;
        if (!item)
            continue;

        switch (item->type)
        {
            case LEFT:
                if (height != maxHeight)
                {
                    item->y = (height - item->height) / 2;
                    item->rect.y = (height - item->rect.y) / 2;
                }
                break;
            case RIGHT:
            {
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
    mWidth = mainGraphics->mWidth;
    mHeight = mainGraphics->mHeight;
}

void TouchManager::unload(TouchItem *restrict const item)
{
    if (item)
    {
        if (item->images)
        {
            Theme::unloadRect(*item->images);
            delete2(item->images);
            if (item->icon)
            {
                item->icon->decRef();
                item->icon = nullptr;
            }
        }
        delete item;
    }
}

void TouchManager::unloadTouchItem(TouchItem *restrict *unloadItem) restrict2
{
    FOR_EACH (TouchItemVectorIter, it, mObjects)
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

void TouchManager::loadPad() restrict2
{
    const int sz = (mJoystickSize + 2) * 50;
    loadTouchItem(&mPad, "dpad.xml", "dpad_image.xml", "", -1, -1, sz, sz,
        LEFT, "", "", &padEvents, &padClick, &padUp, &padOut);
}

void TouchManager::loadButtons() restrict2
{
    const int sz = (mButtonsSize + 1) * 50;
    if (!theme)
        return;
    Skin *const skin = theme->load("dbutton.xml", "");

    if (skin)
    {
        const int x = skin->getOption("x", 10);
        const int y = skin->getOption("y", 10);
        const int pad = skin->getPadding();
        const int pad2 = 2 * pad + sz;
        const int skipWidth = pad2 + x;
        const int skipHeight = pad2 + y;

        switch (mButtonsFormat)
        {
            // 2x1
            case 0:
            default:
            {
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", x, y, sz, sz, RIGHT, "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
            // 2x2
            case 1:
            {
                loadTouchItem(&mButtons[3], "dbutton.xml", "dbutton_image.xml",
                    "4", x, y, sz, sz, RIGHT, "screenActionButton3", "");
                loadTouchItem(&mButtons[2], "dbutton.xml", "dbutton_image.xml",
                    "3", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton2", "");
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", x, skipHeight, sz, sz, RIGHT,
                    "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth, skipHeight, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
            // 3x3
            case 2:
            {
                const int pad4 = pad2 * 2;
                const int skipWidth2 = pad4 + x;
                const int skipHeight2 = pad4 + y;
                loadTouchItem(&mButtons[8], "dbutton.xml", "dbutton_image.xml",
                    "9", x, y, sz, sz, RIGHT, "screenActionButton8", "");
                loadTouchItem(&mButtons[7], "dbutton.xml", "dbutton_image.xml",
                    "8", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton7", "");
                loadTouchItem(&mButtons[6], "dbutton.xml", "dbutton_image.xml",
                    "7", skipWidth2, y, sz, sz, RIGHT,
                    "screenActionButton6", "");
                loadTouchItem(&mButtons[5], "dbutton.xml", "dbutton_image.xml",
                    "6", x, skipHeight, sz, sz, RIGHT,
                    "screenActionButton5", "");
                loadTouchItem(&mButtons[4], "dbutton.xml", "dbutton_image.xml",
                    "5", skipWidth, skipHeight, sz, sz, RIGHT,
                    "screenActionButton4", "");
                loadTouchItem(&mButtons[3], "dbutton.xml", "dbutton_image.xml",
                    "4", skipWidth2, skipHeight, sz, sz, RIGHT,
                    "screenActionButton3", "");
                loadTouchItem(&mButtons[2], "dbutton.xml", "dbutton_image.xml",
                    "3", x, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton2", "");
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", skipWidth, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth2, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
            // 4x2
            case 3:
            {
                const int skipWidth2 = pad2 * 2 + x;
                const int skipWidth3 = pad2 * 3 + x;
                loadTouchItem(&mButtons[7], "dbutton.xml", "dbutton_image.xml",
                    "8", x, y, sz, sz, RIGHT, "screenActionButton7", "");
                loadTouchItem(&mButtons[6], "dbutton.xml", "dbutton_image.xml",
                    "7", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton6", "");
                loadTouchItem(&mButtons[5], "dbutton.xml", "dbutton_image.xml",
                    "6", skipWidth2, y, sz, sz, RIGHT,
                    "screenActionButton5", "");
                loadTouchItem(&mButtons[4], "dbutton.xml", "dbutton_image.xml",
                    "5", skipWidth3, y, sz, sz, RIGHT,
                    "screenActionButton4", "");
                loadTouchItem(&mButtons[3], "dbutton.xml", "dbutton_image.xml",
                    "4", x, skipHeight, sz, sz, RIGHT,
                    "screenActionButton3", "");
                loadTouchItem(&mButtons[2], "dbutton.xml", "dbutton_image.xml",
                    "3", skipWidth, skipHeight, sz, sz, RIGHT,
                    "screenActionButton2", "");
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", skipWidth2, skipHeight, sz, sz, RIGHT,
                    "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth3, skipHeight, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
            // 4x3
            case 4:
            {
                const int skipWidth2 = pad2 * 2 + x;
                const int skipWidth3 = pad2 * 3 + x;
                const int skipHeight2 = pad2 * 2 + y;
                loadTouchItem(&mButtons[11], "dbutton.xml",
                    "dbutton_image.xml", "12", x, y, sz, sz, RIGHT,
                    "screenActionButton11", "");
                loadTouchItem(&mButtons[10], "dbutton.xml",
                    "dbutton_image.xml", "11", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton10", "");
                loadTouchItem(&mButtons[9], "dbutton.xml", "dbutton_image.xml",
                    "10", skipWidth2, y, sz, sz, RIGHT,
                    "screenActionButton9", "");
                loadTouchItem(&mButtons[8], "dbutton.xml", "dbutton_image.xml",
                    "9", skipWidth3, y, sz, sz, RIGHT,
                    "screenActionButton8", "");
                loadTouchItem(&mButtons[7], "dbutton.xml", "dbutton_image.xml",
                    "8", x, skipHeight, sz, sz, RIGHT,
                    "screenActionButton7", "");
                loadTouchItem(&mButtons[6], "dbutton.xml", "dbutton_image.xml",
                    "7", skipWidth, skipHeight, sz, sz, RIGHT,
                    "screenActionButton6", "");
                loadTouchItem(&mButtons[5], "dbutton.xml", "dbutton_image.xml",
                    "6", skipWidth2, skipHeight, sz, sz, RIGHT,
                    "screenActionButton5", "");
                loadTouchItem(&mButtons[4], "dbutton.xml", "dbutton_image.xml",
                    "5", skipWidth3, skipHeight, sz, sz, RIGHT,
                    "screenActionButton4", "");
                loadTouchItem(&mButtons[3], "dbutton.xml", "dbutton_image.xml",
                    "4", x, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton3", "");
                loadTouchItem(&mButtons[2], "dbutton.xml", "dbutton_image.xml",
                    "3", skipWidth, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton2", "");
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", skipWidth2, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth3, skipHeight2, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
            // 3x2
            case 5:
            {
                const int pad4 = pad2 * 2;
                const int skipWidth2 = pad4 + x;
                loadTouchItem(&mButtons[5], "dbutton.xml", "dbutton_image.xml",
                    "6", x, y, sz, sz, RIGHT, "screenActionButton5", "");
                loadTouchItem(&mButtons[4], "dbutton.xml", "dbutton_image.xml",
                    "5", skipWidth, y, sz, sz, RIGHT,
                    "screenActionButton4", "");
                loadTouchItem(&mButtons[3], "dbutton.xml", "dbutton_image.xml",
                    "4", skipWidth2, y, sz, sz, RIGHT,
                    "screenActionButton3", "");
                loadTouchItem(&mButtons[2], "dbutton.xml", "dbutton_image.xml",
                    "3", x, skipHeight, sz, sz, RIGHT,
                    "screenActionButton2", "");
                loadTouchItem(&mButtons[1], "dbutton.xml", "dbutton_image.xml",
                    "2", skipWidth, skipHeight, sz, sz, RIGHT,
                    "screenActionButton1", "");
                loadTouchItem(&mButtons[0], "dbutton.xml", "dbutton_image.xml",
                    "1", skipWidth2, skipHeight, sz, sz, RIGHT,
                    "screenActionButton0", "");
                break;
            }
        };
        theme->unload(skin);
    }
}

void TouchManager::loadKeyboard() restrict2
{
    loadTouchItem(&mKeyboard, "keyboard_icon.xml", "", "", -1, -1, 28, 28,
        NORMAL, "", "screenActionKeyboard");
}

void TouchManager::optionChanged(const std::string &value) restrict2
{
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
            for (int f = 0; f < buttonsCount; f ++)
                unloadTouchItem(&mButtons[f]);
        }
        mRedraw = true;
    }
    else if (value == "showScreenKeyboard")
    {
        if (mShowKeyboard == config.getBoolValue("showScreenKeyboard"))
            return;
        mShowKeyboard = config.getBoolValue("showScreenKeyboard");
        if (mShowKeyboard)
            loadKeyboard();
        else
            unloadTouchItem(&mKeyboard);
        mRedraw = true;
    }
    else if (value == "screenButtonsSize")
    {
        if (mButtonsSize == config.getIntValue("screenButtonsSize"))
            return;
        mButtonsSize = config.getIntValue("screenButtonsSize");
        if (mShowButtons)
        {
            for (int f = 0; f < buttonsCount; f ++)
                unloadTouchItem(&mButtons[f]);
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
    else if (value == "screenButtonsFormat")
    {
        if (mButtonsFormat == config.getIntValue("screenButtonsFormat"))
            return;
        mButtonsFormat = config.getIntValue("screenButtonsFormat");
        if (mShowButtons)
        {
            for (int f = 0; f < buttonsCount; f ++)
                unloadTouchItem(&mButtons[f]);
            loadButtons();
        }
    }
}

void TouchManager::setInGame(const bool b) restrict2
{
    mInGame = b;
    mShow = mInGame && !mTempHideButtons;
    mRedraw = true;
}

void TouchManager::setTempHide(const bool b) restrict2
{
    mTempHideButtons = b;
    mShow = mInGame && !mTempHideButtons;
    mRedraw = true;
}

void TouchManager::executeAction(const std::string &restrict event)
{
    inputManager.executeAction(static_cast<InputActionT>(
        config.getIntValue(event)));
}
