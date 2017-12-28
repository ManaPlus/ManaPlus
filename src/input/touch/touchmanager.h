/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef INPUT_TOUCH_TOUCHMANAGER_H
#define INPUT_TOUCH_TOUCHMANAGER_H

#include "listeners/configlistener.h"

#include "enums/input/inputaction.h"

#include "gui/rect.h"

#include "utils/cast.h"
#include "utils/vector.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageRect;
class MouseInput;

typedef void (*TouchFuncPtr) (const MouseInput &restrict mouseInput);

const int actionsSize = CAST_S32(InputAction::TOTAL);
const int buttonsCount = 12;

struct TouchItem final
{
    TouchItem(const std::string &text0,
              const Rect &rect0,
              const int type0,
              const std::string &restrict eventPressed0,
              const std::string &restrict eventReleased0,
              ImageRect *restrict const images0,
              Image *restrict const icon0,
              const int x0, const int y0,
              const int width0, const int height0,
              const TouchFuncPtr ptrAll,
              const TouchFuncPtr ptrPressed,
              const TouchFuncPtr ptrReleased,
              const TouchFuncPtr ptrOut) :
        text(text0),
        rect(rect0),
        type(type0),
        eventPressed(eventPressed0),
        eventReleased(eventReleased0),
        images(images0),
        icon(icon0),
        x(x0),
        y(y0),
        width(width0),
        height(height0),
        funcAll(ptrAll),
        funcPressed(ptrPressed),
        funcReleased(ptrReleased),
        funcOut(ptrOut)
    {
    }

    A_DELETE_COPY(TouchItem)

    std::string text;
    Rect rect;
    int type;
    std::string eventPressed;
    std::string eventReleased;
    ImageRect *images;
    Image *icon;
    int x;
    int y;
    int width;
    int height;
    TouchFuncPtr funcAll;
    TouchFuncPtr funcPressed;
    TouchFuncPtr funcReleased;
    TouchFuncPtr funcOut;
};

typedef STD_VECTOR<TouchItem*> TouchItemVector;
typedef TouchItemVector::const_iterator TouchItemVectorCIter;
typedef TouchItemVector::iterator TouchItemVectorIter;

class TouchManager final : public ConfigListener
{
    public:
        TouchManager();

        ~TouchManager() override final;

        A_DELETE_COPY(TouchManager)

        enum Types
        {
            NORMAL = 0,
            LEFT = 1,
            RIGHT = 2
        };

        void init() restrict2;

        void loadTouchItem(TouchItem **restrict item,
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
                           const TouchFuncPtr fOut)
                           restrict2 A_NONNULL(2);

        void clear() restrict2;

        void draw() restrict2;

        void safeDraw() restrict2;

        void drawText() restrict2;

        bool processEvent(const MouseInput &mouseInput) restrict2;

        bool isActionActive(const InputActionT index) restrict2 const;

        void setActionActive(const InputActionT index,
                             const bool value) restrict2
        {
            if (CAST_S32(index) >= 0 &&
                CAST_S32(index) < actionsSize)
            {
                mActions[CAST_SIZE(index)] = value;
            }
        }

        void resize(const int width, const int height) restrict2;

        static void unload(TouchItem *restrict const item);

        void unloadTouchItem(TouchItem *restrict *unloadItem) restrict2;

        void optionChanged(const std::string &value) restrict2 override final;

        void loadPad() restrict2;

        void loadButtons() restrict2;

        void loadKeyboard() restrict2;

        int getPadSize() restrict2 const
        { return (mJoystickSize + 2) * 50; }

        void setInGame(const bool b) restrict2;

        void setTempHide(const bool b) restrict2;

        void shutdown() restrict2;

        static void executeAction(const std::string &restrict event);

    private:
        TouchItem *mKeyboard;
        TouchItem *mPad;
        TouchItem *mButtons[buttonsCount] A_NONNULLPOINTER;
        TouchItemVector mObjects;
        ImageCollection *mVertexes A_NONNULLPOINTER;
        bool mActions[actionsSize];
        bool mRedraw;
        bool mShowJoystick;
        bool mShowButtons;
        bool mShowKeyboard;
        int mButtonsSize;
        int mJoystickSize;
        int mButtonsFormat;
        int mWidth;
        int mHeight;
        bool mShow;
        bool mInGame;
        bool mTempHideButtons;
};

extern TouchManager touchManager;

#endif  // INPUT_TOUCH_TOUCHMANAGER_H
