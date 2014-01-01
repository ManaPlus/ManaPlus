/*
 *  The ManaPlus Client
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

#ifndef TOUCHMANAGER_H
#define TOUCHMANAGER_H

#include "resources/image.h"

#include "configlistener.h"

#include "input/keydata.h"

#include <guichan/rectangle.hpp>

#include <string>
#include <vector>

#include "localconsts.h"

class ImageCollection;
class ImageRect;
class MouseInput;

typedef void (*TouchFuncPtr) (const MouseInput &mouseInput);

const int actionsSize = Input::KEY_TOTAL;
const int buttonsCount = 12;

struct TouchItem final
{
    TouchItem(const std::string &text0,
              const gcn::Rectangle &rect0, const int type0,
              const std::string &eventPressed0,
              const std::string &eventReleased0,
              ImageRect *const images0, Image *const icon0,
              const int x0, const int y0, const int width0, const int height0,
              const TouchFuncPtr ptrAll, const TouchFuncPtr ptrPressed,
              const TouchFuncPtr ptrReleased, const TouchFuncPtr ptrOut) :
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
    gcn::Rectangle rect;
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

typedef std::vector<TouchItem*> TouchItemVector;
typedef TouchItemVector::const_iterator TouchItemVectorCIter;
typedef TouchItemVector::iterator TouchItemVectorIter;

class TouchManager final : public ConfigListener
{
    public:
        TouchManager();

        ~TouchManager();

        A_DELETE_COPY(TouchManager)

        enum Types
        {
            NORMAL = 0,
            LEFT = 1,
            RIGHT = 2
        };

        void init();

        void loadTouchItem(TouchItem **item, const std::string &name,
                           const std::string &imageName,
                           const std::string &text,
                           int x, int y, const int width, const int height,
                           const int type, const std::string &eventPressed,
                           const std::string &eventReleased,
                           const TouchFuncPtr fAll = nullptr,
                           const TouchFuncPtr fPressed = nullptr,
                           const TouchFuncPtr fReleased = nullptr,
                           const TouchFuncPtr fOut = nullptr);

        void clear();

        void draw();

        bool processEvent(const MouseInput &mouseInput);

        bool isActionActive(const int index) const;

        void setActionActive(const int index, const bool value)
        {
            if (index >= 0 && index < actionsSize)
                mActions[index] = value;
        }

        void resize(const int width, const int height);

        static void unload(TouchItem *const item);

        void unloadTouchItem(TouchItem **unloadItem);

        void optionChanged(const std::string &value);

        void loadPad();

        void loadButtons();

        void loadKeyboard();

        int getPadSize()
        { return (mJoystickSize + 2) * 50; }

        void setInGame(const bool b);

        void setTempHide(const bool b);

        void shutdown();

        void executeAction(const std::string &event);

    private:
        TouchItem *mKeyboard;
        TouchItem *mPad;
        TouchItem *mButtons[buttonsCount];
        TouchItemVector mObjects;
        ImageCollection *mVertexes;
        bool mActions[actionsSize];
        bool mRedraw;
        bool mShowJoystick;
        bool mShowButtons;
        bool mShowKeyboard;
        int mButtonsSize;
        int mJoystickSize;
        int mButtonsFormat;
        bool mShow;
        bool mInGame;
        bool mTempHideButtons;
};

extern TouchManager touchManager;

#endif  // TOUCHMANAGER_H

