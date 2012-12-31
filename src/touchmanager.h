/*
 *  The ManaPlus Client
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

#ifndef TOUCHMANAGER_H
#define TOUCHMANAGER_H

#include "resources/image.h"

#include "configlistener.h"
#include "keydata.h"

#include <guichan/mouseinput.hpp>
#include <guichan/rectangle.hpp>

#include <map>
#include <string>
#include <vector>

#include "localconsts.h"

class ImageCollection;
class ImageRect;
class MouseInput;
class Skin;

typedef void (*TouchFuncPtr) (const MouseInput &mouseInput);

const int actionsSize = Input::KEY_TOTAL;
const int buttonsCount = 4;

struct TouchItem final
{
    TouchItem(const gcn::Rectangle rect0, int type0,
              const std::string &eventPressed0,
              const std::string &eventReleased0,
              ImageRect *const images0, Image *const icon0,
              int x0, int y0, int width0, int height0,
              TouchFuncPtr ptrAll, TouchFuncPtr ptrPressed,
              TouchFuncPtr ptrReleased, TouchFuncPtr ptrOut) :
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

        void loadTouchItem(TouchItem **item, std::string name,
                           std::string imageName,
                           int x, int y, int width, int height,
                           int type, const std::string &eventPressed,
                           const std::string &eventReleased,
                           TouchFuncPtr fAll = nullptr,
                           TouchFuncPtr fPressed = nullptr,
                           TouchFuncPtr fReleased = nullptr,
                           TouchFuncPtr fOut = nullptr);

        void clear();

        void draw();

        bool processEvent(const MouseInput &mouseInput);

        bool isActionActive(const int index) const;

        void setActionActive(const int index, const bool value)
        {
            if (index >= 0 && index < actionsSize)
                mActions[index] = value;
        }

        void resize(int width, int height);

        void unload(TouchItem *item);

        void unloadTouchItem(TouchItem **unloadItem);

        void optionChanged(const std::string &value);

        void loadPad();

        void loadButtons();

        void loadKeyboard();

        int getPadSize()
        { return (mJoystickSize + 2) * 50; }

        void setInGame(bool b);

        void setTempHide(bool b);

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
#endif
