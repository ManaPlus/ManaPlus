/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gui/gui.h"

#include "gui/focushandler.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/widgets/label.h"
#include "gui/widgets/window.h"

#include "gui/widgets/tabs/tab.h"

#ifndef DYECMD
#include "dragdrop.h"
#else  // DYECMD
#include "resources/image/image.h"
#endif  // DYECMD
#include "settings.h"

#include "listeners/focuslistener.h"
#include "listeners/guiconfiglistener.h"
#include "listeners/keylistener.h"

#include "input/inputmanager.h"

#include "input/touch/touchmanager.h"

#include "render/renderers.h"

#include "resources/imageset.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/langs.h"
#include "utils/sdlsharedhelper.h"
#include "utils/timer.h"

#include "net/ipc.h"

#include "debug.h"

Gui *gui = nullptr;
Font *boldFont = nullptr;

Gui::Gui() :
    mTop(nullptr),
    mGraphics(nullptr),
    mInput(nullptr),
    mFocusHandler(new FocusHandler),
    mKeyListeners(),
    mLastMousePressButton(MouseButton::EMPTY),
    mLastMousePressTimeStamp(0U),
    mLastMouseX(0),
    mLastMouseY(0),
    mClickCount(1),
    mLastMouseDragButton(MouseButton::EMPTY),
    mWidgetWithMouseQueue(),
    mConfigListener(new GuiConfigListener(this)),
    mGuiFont(nullptr),
    mInfoParticleFont(nullptr),
    mHelpFont(nullptr),
    mSecureFont(nullptr),
    mNpcFont(nullptr),
    mMouseCursors(nullptr),
    mMouseCursorAlpha(1.0F),
    mMouseInactivityTimer(0),
    mCursorType(Cursor::CURSOR_POINTER),
#ifdef ANDROID
    mLastMouseRealX(0),
    mLastMouseRealY(0),
#endif  // ANDROID
    mFocusListeners(),
    mForegroundColor(theme->getColor(ThemeColorId::TEXT, 255)),
    mForegroundColor2(theme->getColor(ThemeColorId::TEXT_OUTLINE, 255)),
    mTime(0),
    mTime10(0),
    mCustomCursor(false),
    mDoubleClick(true)
{
}

void Gui::postInit(Graphics *const graphics)
{
    logger->log1("Initializing GUI...");
    // Set graphics
    setGraphics(graphics);

    // Set input
    guiInput = new SDLInput;
    setInput(guiInput);

    // Set focus handler
    delete mFocusHandler;
    mFocusHandler = new FocusHandler;

    // Initialize top GUI widget
    WindowContainer *const guiTop = new WindowContainer(nullptr);
    guiTop->setFocusable(true);
    guiTop->setSize(graphics->mWidth, graphics->mHeight);
    guiTop->setOpaque(Opaque_false);
    Window::setWindowContainer(guiTop);
    setTop(guiTop);

    const StringVect langs = getLang();
    const bool isJapan = (!langs.empty() && langs[0].size() > 3
        && langs[0].substr(0, 3) == "ja_");
    const bool isChinese = (!langs.empty() && langs[0].size() > 3
        && langs[0].substr(0, 3) == "zh_");

    // Set global font
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    mGuiFont = new Font(fontFile, fontSize, TTF_STYLE_NORMAL);

    // Set particle font
    fontFile = config.getValue("particleFont", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    mInfoParticleFont = new Font(fontFile, fontSize, TTF_STYLE_BOLD);

    // Set bold font
    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    boldFont = new Font(fontFile, fontSize, TTF_STYLE_NORMAL);

    // Set help font
    fontFile = config.getValue("helpFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("helpFont");

    mHelpFont = new Font(fontFile, fontSize, TTF_STYLE_NORMAL);

    // Set secure font
    fontFile = config.getValue("secureFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("secureFont");

    mSecureFont = new Font(fontFile, fontSize, TTF_STYLE_NORMAL);

    // Set npc font
    const int npcFontSize = config.getIntValue("npcfontSize");
    fontFile = config.getValue("npcFont", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("npcFont");

    mNpcFont = new Font(fontFile, npcFontSize, TTF_STYLE_NORMAL);

    Widget::setGlobalFont(mGuiFont);

    // Initialize mouse cursor and listen for changes to the option
    setUseCustomCursor(config.getBoolValue("customcursor"));
    setDoubleClick(config.getBoolValue("doubleClick"));
    config.addListener("customcursor", mConfigListener);
    config.addListener("doubleClick", mConfigListener);
}

Gui::~Gui()
{
    config.removeListeners(mConfigListener);
    delete2(mConfigListener);

    if (mMouseCursors != nullptr)
    {
        mMouseCursors->decRef();
        mMouseCursors = nullptr;
    }

    if (windowContainer != nullptr)
        windowContainer->slowLogic();
    Widget *top = mTop;
    if (Widget::widgetExists(mTop))
        setTop(nullptr);
    Window::setWindowContainer(nullptr);
    delete top;

    delete2(mGuiFont);
    delete2(boldFont);
    delete2(mHelpFont);
    delete2(mSecureFont);
    delete2(mInfoParticleFont);
    delete2(mNpcFont);
    delete2(guiInput);
    delete2(theme);

    delete2(mFocusHandler);
    Label::finalCleanup();
    Tab::finalCleanup();
    Widget::cleanGlobalFont();
}

void Gui::logic()
{
    BLOCK_START("Gui::logic")
    ResourceManager::clearScheduled();

    if (mTop == nullptr)
    {
        BLOCK_END("Gui::logic")
        return;
    }

    handleModalFocus();
    handleModalMouseInputFocus();

    if (guiInput != nullptr)
        handleMouseInput();

    mTop->logic();
    BLOCK_END("Gui::logic")
}

void Gui::slowLogic()
{
    BLOCK_START("Gui::slowLogic")
    Palette::advanceGradients();

    // Fade out mouse cursor after extended inactivity
    if (mMouseInactivityTimer < 100 * 15)
    {
        ++mMouseInactivityTimer;
        mMouseCursorAlpha = std::min(1.0F, mMouseCursorAlpha + 0.05F);
    }
    else
    {
        mMouseCursorAlpha = std::max(0.0F, mMouseCursorAlpha - 0.005F);
    }
    if (mGuiFont != nullptr)
        mGuiFont->slowLogic(0);
    if (mInfoParticleFont != nullptr)
        mInfoParticleFont->slowLogic(1);
    if (mHelpFont != nullptr)
        mHelpFont->slowLogic(2);
    if (mSecureFont != nullptr)
        mSecureFont->slowLogic(3);
    if (boldFont != nullptr)
        boldFont->slowLogic(4);
    if (mNpcFont != nullptr)
        mNpcFont->slowLogic(5);
    if (windowContainer != nullptr)
        windowContainer->slowLogic();

    const time_t time = cur_time;
    if (mTime != time)
    {
        logger->flush();
        if (ipc != nullptr)
            ipc->flush();
        mTime = time;

        if (time > mTime10 || mTime10 - time > 10)
        {
            mTime10 = time + 10;
            ResourceManager::cleanOrphans(false);
            guiInput->simulateMouseMove();
        }
    }

    BLOCK_END("Gui::slowLogic")
}

void Gui::clearFonts()
{
    if (mGuiFont != nullptr)
        mGuiFont->clear();
    if (mInfoParticleFont != nullptr)
        mInfoParticleFont->clear();
    if (mHelpFont != nullptr)
        mHelpFont->clear();
    if (mSecureFont != nullptr)
        mSecureFont->clear();
    if (boldFont != nullptr)
        boldFont->clear();
    if (mNpcFont != nullptr)
        mNpcFont->clear();
}

bool Gui::handleInput()
{
    if (mInput != nullptr)
        return handleKeyInput();
    return false;
}

bool Gui::handleKeyInput()
{
    if (guiInput == nullptr)
        return false;

    BLOCK_START("Gui::handleKeyInput")
    bool consumed(false);

    while (!mInput->isKeyQueueEmpty())
    {
        const KeyInput keyInput = guiInput->dequeueKeyInput();

        KeyEvent eventToGlobalKeyListeners(nullptr,
            keyInput.getType(),
            keyInput.getActionId(), keyInput.getKey());

#ifdef USE_SDL2
        if (!keyInput.getText().empty())
            eventToGlobalKeyListeners.setText(keyInput.getText());
#endif  // USE_SDL2

        distributeKeyEventToGlobalKeyListeners(
            eventToGlobalKeyListeners);

        // If a global key listener consumes the event it will not be
        // sent further to the source of the event.
        if (eventToGlobalKeyListeners.isConsumed())
        {
            consumed = true;
            continue;
        }

        if (mFocusHandler != nullptr)
        {
            bool eventConsumed = false;

            // Send key inputs to the focused widgets
            if (mFocusHandler->getFocused() != nullptr)
            {
                KeyEvent event(getKeyEventSource(),
                    keyInput.getType(),
                    keyInput.getActionId(), keyInput.getKey());
#ifdef USE_SDL2
                if (!keyInput.getText().empty())
                    event.setText(keyInput.getText());
#endif  // USE_SDL2

                if (!mFocusHandler->getFocused()->isFocusable())
                    mFocusHandler->focusNone();
                else
                    distributeKeyEvent(event);

                eventConsumed = event.isConsumed();
                if (eventConsumed)
                    consumed = true;
            }

            // If the key event hasn't been consumed and
            // tabbing is enable check for tab press and
            // change focus.
            if (!eventConsumed && keyInput.getActionId()
                == InputAction::GUI_TAB &&
                keyInput.getType() == KeyEventType::PRESSED)
            {
                if (inputManager.isActionActive(InputAction::GUI_MOD))
                    mFocusHandler->tabPrevious();
                else
                    mFocusHandler->tabNext();
            }
        }
    }  // end while
    BLOCK_END("Gui::handleKeyInput")
    return consumed;
}

void Gui::draw()
{
    BLOCK_START("Gui::draw 1")
    Widget *const top = getTop();
    if (top == nullptr)
        return;
    mGraphics->pushClipArea(top->getDimension());

    if (isBatchDrawRenders(openGLMode))
    {
        top->draw(mGraphics);
        touchManager.draw();
    }
    else
    {
        top->safeDraw(mGraphics);
        touchManager.safeDraw();
    }

    int mouseX;
    int mouseY;
    const MouseStateType button = getMouseState(mouseX, mouseY);

    if ((settings.mouseFocused ||
        ((button & SDL_BUTTON(1)) != 0)) &&
        mMouseCursors != nullptr &&
        mCustomCursor &&
        mMouseCursorAlpha > 0.0F)
    {
#ifndef DYECMD
        const Image *const image = dragDrop.getItemImage();
        if (mGuiFont != nullptr)
        {
            const std::string &str = dragDrop.getText();
            if (!str.empty())
            {
                const int posX = mouseX - mGuiFont->getWidth(str) / 2;
                const int posY = mouseY +
                    (image != nullptr ? image->mBounds.h / 2 : 0);
                mGuiFont->drawString(mGraphics,
                    mForegroundColor, mForegroundColor2,
                    str,
                    posX, posY);
            }
        }
        if (image != nullptr)
        {
            const int posX = mouseX - (image->mBounds.w / 2);
            const int posY = mouseY - (image->mBounds.h / 2);
            mGraphics->drawImage(image, posX, posY);
        }
#endif  // DYECMD

        Image *const mouseCursor = mMouseCursors->get(
            CAST_SIZE(mCursorType));
        if (mouseCursor != nullptr)
        {
            mouseCursor->setAlpha(mMouseCursorAlpha);
            mGraphics->drawImage(mouseCursor, mouseX - 15, mouseY - 17);
        }
    }

    mGraphics->popClipArea();
    BLOCK_END("Gui::draw 1")
}

void Gui::videoResized() const
{
    WindowContainer *const top = static_cast<WindowContainer *>(getTop());

    if (top != nullptr)
    {
        const int oldWidth = top->getWidth();
        const int oldHeight = top->getHeight();

        top->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
        top->adjustAfterResize(oldWidth, oldHeight);
    }

    if (viewport != nullptr)
        viewport->videoResized();
    Widget::distributeWindowResizeEvent();
}

void Gui::setUseCustomCursor(const bool customCursor)
{
    if (settings.options.hideCursor)
    {
        SDL::showCursor(false);
        return;
    }
    if (customCursor != mCustomCursor)
    {
        mCustomCursor = customCursor;

        if (mCustomCursor)
        {
            // Hide the SDL mouse cursor
            SDL::showCursor(false);

            // Load the mouse cursor
            if (mMouseCursors != nullptr)
                mMouseCursors->decRef();
            mMouseCursors = Theme::getImageSetFromTheme("mouse.png", 40, 40);

            if (mMouseCursors == nullptr)
                logger->log("Error: Unable to load mouse cursors.");
        }
        else
        {
            // Show the SDL mouse cursor
            SDL::showCursor(true);

            // Unload the mouse cursor
            if (mMouseCursors != nullptr)
            {
                mMouseCursors->decRef();
                mMouseCursors = nullptr;
            }
        }
    }
}

void Gui::handleMouseMoved(const MouseInput &mouseInput)
{
    // Check if the mouse leaves the application window.
    if (mTop != nullptr &&
        !mWidgetWithMouseQueue.empty() &&
        (mouseInput.getX() < 0 ||
        mouseInput.getY() < 0 ||
        !mTop->getDimension().isPointInRect(mouseInput.getX(),
        mouseInput.getY())))
    {
        // Distribute an event to all widgets in the
        // "widget with mouse" queue.
        while (!mWidgetWithMouseQueue.empty())
        {
            Widget *const widget = mWidgetWithMouseQueue.front();

            if (Widget::widgetExists(widget))
            {
                distributeMouseEvent(widget,
                    MouseEventType::EXITED,
                    mouseInput.getButton(),
                    mouseInput.getX(),
                    mouseInput.getY(),
                    true,
                    true);
            }

            mWidgetWithMouseQueue.pop_front();
        }

        mMouseInactivityTimer = 0;
        return;
    }

    const int mouseX = mouseInput.getX();
    const int mouseY = mouseInput.getY();
    const MouseButtonT button = mouseInput.getButton();

    // Check if there is a need to send mouse exited events by
    // traversing the "widget with mouse" queue.
    bool widgetWithMouseQueueCheckDone = mWidgetWithMouseQueue.empty();
    while (!widgetWithMouseQueueCheckDone)
    {
        unsigned int iterations = 0;
        for (std::deque<Widget*>::iterator
             iter = mWidgetWithMouseQueue.begin();
             iter != mWidgetWithMouseQueue.end();
             ++ iter)
        {
            Widget *const widget = *iter;

            // If a widget in the "widget with mouse queue" doesn't
            // exists anymore it should be removed from the queue.
            if (!Widget::widgetExists(widget))
            {
                mWidgetWithMouseQueue.erase(iter);
                break;
            }
            else
            {
                int x;
                int y;
                widget->getAbsolutePosition(x, y);

                if (x > mouseX
                    || y > mouseY
                    || x + widget->getWidth() <= mouseX
                    || y + widget->getHeight() <= mouseY
                    || !widget->isVisible())
                {
                    distributeMouseEvent(widget,
                        MouseEventType::EXITED,
                        button,
                        mouseX,
                        mouseY,
                        true,
                        true);
                    mClickCount = 1;
                    mLastMousePressTimeStamp = 0U;
                    mWidgetWithMouseQueue.erase(iter);
                    break;
                }
            }

            iterations++;
        }

        widgetWithMouseQueueCheckDone =
            (CAST_SIZE(iterations) == mWidgetWithMouseQueue.size());
    }

    // Check all widgets below the mouse to see if they are
    // present in the "widget with mouse" queue. If a widget
    // is not then it should be added and an entered event should
    // be sent to it.
    Widget* parent = getMouseEventSource(mouseX, mouseY);
    Widget* widget = parent;

    // If a widget has modal mouse input focus then it will
    // always be returned from getMouseEventSource, but we only wan't to
    // send mouse entered events if the mouse has actually entered the
    // widget with modal mouse input focus, hence we need to check if
    // that's the case. If it's not we should simply ignore to send any
    // mouse entered events.
    if ((mFocusHandler->getModalMouseInputFocused() != nullptr)
        && widget == mFocusHandler->getModalMouseInputFocused()
        && Widget::widgetExists(widget) &&
        (widget != nullptr))
    {
        int x, y;
        widget->getAbsolutePosition(x, y);

        if (x > mouseX || y > mouseY
            || x + widget->getWidth() <= mouseX
            || y + widget->getHeight() <= mouseY)
        {
            parent = nullptr;
        }
    }

    while (parent != nullptr)
    {
        parent = widget->getParent();

        // Check if the widget is present in the "widget with mouse" queue.
        bool widgetIsPresentInQueue = false;
        FOR_EACH (std::deque<Widget*>::const_iterator,
                  iter, mWidgetWithMouseQueue)
        {
            if (*iter == widget)
            {
                widgetIsPresentInQueue = true;
                break;
            }
        }

        // Widget is not present, send an entered event and add
        // it to the "widget with mouse" queue.
        if (!widgetIsPresentInQueue
            && Widget::widgetExists(widget))
        {
            distributeMouseEvent(widget,
                MouseEventType::ENTERED,
                button,
                mouseX,
                mouseY,
                true,
                true);
            mWidgetWithMouseQueue.push_front(widget);
        }

        const Widget *const swap = widget;
        widget = parent;
        parent = swap->getParent();
    }

    if (mFocusHandler->getDraggedWidget() != nullptr)
    {
        distributeMouseEvent(mFocusHandler->getDraggedWidget(),
            MouseEventType::DRAGGED,
            mLastMouseDragButton,
            mouseX,
            mouseY,
            false,
            false);
    }
    else
    {
        Widget *const sourceWidget = getMouseEventSource(mouseX, mouseY);
        distributeMouseEvent(sourceWidget,
            MouseEventType::MOVED,
            button,
            mouseX,
            mouseY,
            false,
            false);
    }
    mMouseInactivityTimer = 0;
}

void Gui::handleMousePressed(const MouseInput &mouseInput)
{
    const int x = mouseInput.getX();
    const int y = mouseInput.getY();
    const MouseButtonT button = mouseInput.getButton();
    const unsigned int timeStamp = mouseInput.getTimeStamp();

    Widget *sourceWidget = getMouseEventSource(x, y);

    if (mFocusHandler->getDraggedWidget() != nullptr)
        sourceWidget = mFocusHandler->getDraggedWidget();

    if (sourceWidget == nullptr)
        return;
    int sourceWidgetX;
    int sourceWidgetY;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

    if (((mFocusHandler->getModalFocused() != nullptr)
        && sourceWidget->isModalFocused())
        || (mFocusHandler->getModalFocused() == nullptr))
    {
        sourceWidget->requestFocus();
    }

    if (mDoubleClick &&
        timeStamp - mLastMousePressTimeStamp < 250U &&
        mLastMousePressButton == button)
    {
        mClickCount ++;
    }
    else
    {
        mClickCount = 1;
    }

    distributeMouseEvent(sourceWidget,
        MouseEventType::PRESSED,
        button,
        x,
        y,
        false,
        false);
    mFocusHandler->setLastWidgetPressed(sourceWidget);
    mFocusHandler->setDraggedWidget(sourceWidget);
    mLastMouseDragButton = button;
    mLastMousePressButton = button;
    mLastMousePressTimeStamp = timeStamp;
}

void Gui::updateFonts()
{
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    mGuiFont->loadFont(fontFile, fontSize, TTF_STYLE_NORMAL);

    fontFile = config.getValue("particleFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    mInfoParticleFont->loadFont(fontFile, fontSize, TTF_STYLE_BOLD);

    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    boldFont->loadFont(fontFile, fontSize, TTF_STYLE_NORMAL);

    const int npcFontSize = config.getIntValue("npcfontSize");

    fontFile = config.getValue("npcFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("npcFont");

    mNpcFont->loadFont(fontFile, npcFontSize, TTF_STYLE_NORMAL);
}

void Gui::distributeMouseEvent(Widget *const source,
                               const MouseEventTypeT type,
                               const MouseButtonT button,
                               const int x, const int y,
                               const bool force,
                               const bool toSourceOnly)
{
    if ((source == nullptr) || (mFocusHandler == nullptr))
        return;

    Widget *widget = source;

    if (!force)
    {
        if (mFocusHandler->getModalFocused() != nullptr
            && !widget->isModalFocused())
        {
            return;
        }
        if (mFocusHandler->getModalMouseInputFocused() != nullptr
            && !widget->isModalMouseInputFocused())
        {
            return;
        }
    }

    MouseEvent event(source,
        type, button,
        x, y, mClickCount);

    Widget* parent = source;
    while (parent != nullptr)
    {
        // If the widget has been removed due to input
        // cancel the distribution.
        if (!Widget::widgetExists(widget))
            break;

        parent = widget->getParent();

        if (widget->isEnabled() || force)
        {
            int widgetX;
            int widgetY;
            widget->getAbsolutePosition(widgetX, widgetY);

            event.setX(x - widgetX);
            event.setY(y - widgetY);

            std::list<MouseListener*> mouseListeners
                = widget->getMouseListeners();

            const MouseEventTypeT mouseType = event.getType();
            // Send the event to all mouse listeners of the widget.
            FOR_EACH (std::list<MouseListener*>::const_iterator,
                 it, mouseListeners)
            {
                switch (mouseType)
                {
                    case MouseEventType::ENTERED:
                        (*it)->mouseEntered(event);
                        break;
                    case MouseEventType::EXITED:
                        (*it)->mouseExited(event);
                        break;
                    case MouseEventType::MOVED:
                        (*it)->mouseMoved(event);
                        break;
                    case MouseEventType::PRESSED:
                        (*it)->mousePressed(event);
                        break;
                    case MouseEventType::RELEASED:
                    case MouseEventType::RELEASED2:
                        (*it)->mouseReleased(event);
                        break;
                    case MouseEventType::WHEEL_MOVED_UP:
                        (*it)->mouseWheelMovedUp(event);
                        break;
                    case MouseEventType::WHEEL_MOVED_DOWN:
                        (*it)->mouseWheelMovedDown(event);
                        break;
                    case MouseEventType::DRAGGED:
                        (*it)->mouseDragged(event);
                        break;
                    case MouseEventType::CLICKED:
                        (*it)->mouseClicked(event);
                        break;
                    default:
                        break;
                }
            }

            if (toSourceOnly)
                break;
        }

        const Widget *const swap = widget;
        widget = parent;
        parent = swap->getParent();

#ifndef DYECMD
        if (type == MouseEventType::RELEASED)
            dragDrop.clear();
#endif  // DYECMD

        if ((widget == nullptr) || event.isConsumed())
            break;

        // If a non modal focused widget has been reach
        // and we have modal focus cancel the distribution.
        if ((mFocusHandler->getModalFocused() != nullptr)
            && !widget->isModalFocused())
        {
            break;
        }

        // If a non modal mouse input focused widget has been reach
        // and we have modal mouse input focus cancel the distribution.
        if ((mFocusHandler->getModalMouseInputFocused() != nullptr)
            && !widget->isModalMouseInputFocused())
        {
            break;
        }
    }
}

void Gui::resetClickCount()
{
    mClickCount = 1;
    mLastMousePressTimeStamp = 0;
}

MouseEvent *Gui::createMouseEvent(Window *const widget)
{
    if ((viewport == nullptr) || (widget == nullptr))
        return nullptr;

    int x = 0;
    int y = 0;
    int mouseX = 0;
    int mouseY = 0;

    getAbsolutePosition(widget, x, y);
    getMouseState(mouseX, mouseY);

    return new MouseEvent(widget,
        MouseEventType::MOVED,
        MouseButton::EMPTY,
        mouseX - x,
        mouseY - y,
        mClickCount);
}

void Gui::getAbsolutePosition(Widget *restrict widget,
                              int &restrict x,
                              int &restrict y)
{
    if (widget == nullptr)
        return;
    x = 0;
    y = 0;
    while (widget->getParent() != nullptr)
    {
        x += widget->getX();
        y += widget->getY();
        widget = widget->getParent();
    }
}

void Gui::handleMouseInput()
{
    BLOCK_START("Gui::handleMouseInput")
    while (!mInput->isMouseQueueEmpty())
    {
        const MouseInput mouseInput = guiInput->dequeueMouseInput();

        if (touchManager.processEvent(mouseInput))
        {
#ifdef ANDROID
#ifndef USE_SDL2
            SDL_WarpMouse(mLastMouseX, mLastMouseY,
                mLastMouseRealX, mLastMouseRealY);
#endif  // USE_SDL2
#endif  // ANDROID

            mMouseInactivityTimer = 0;
            continue;
        }

        // Save the current mouse state. It will be needed if modal focus
        // changes or modal mouse input focus changes.
        mLastMouseX = mouseInput.getX();
        mLastMouseY = mouseInput.getY();
#ifdef ANDROID
        mLastMouseRealX = mouseInput.getRealX();
        mLastMouseRealY = mouseInput.getRealY();
#endif  // ANDROID

        switch (mouseInput.getType())
        {
            case MouseEventType::PRESSED:
                handleMousePressed(mouseInput);
                break;
            case MouseEventType::RELEASED:
                handleMouseReleased(mouseInput);
                break;
            case MouseEventType::MOVED:
                handleMouseMoved(mouseInput);
                break;
            case MouseEventType::WHEEL_MOVED_DOWN:
                handleMouseWheelMovedDown(mouseInput);
                break;
            case MouseEventType::WHEEL_MOVED_UP:
                handleMouseWheelMovedUp(mouseInput);
                break;
            case MouseEventType::CLICKED:
            case MouseEventType::ENTERED:
            case MouseEventType::EXITED:
            case MouseEventType::DRAGGED:
            case MouseEventType::RELEASED2:
            default:
                break;
        }
    }
    BLOCK_END("Gui::handleMouseInput")
}

void Gui::handleMouseReleased(const MouseInput &mouseInput)
{
    Widget *sourceWidget = getMouseEventSource(
        mouseInput.getX(), mouseInput.getY());

    int sourceWidgetX;
    int sourceWidgetY;
    if (mFocusHandler->getDraggedWidget() != nullptr)
    {
        if (sourceWidget != mFocusHandler->getLastWidgetPressed())
            mFocusHandler->setLastWidgetPressed(nullptr);

        Widget *const oldWidget = sourceWidget;
        sourceWidget = mFocusHandler->getDraggedWidget();
        if ((oldWidget != nullptr) && oldWidget != sourceWidget)
        {
            oldWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
            distributeMouseEvent(oldWidget,
                MouseEventType::RELEASED2,
                mouseInput.getButton(),
                mouseInput.getX(),
                mouseInput.getY(),
                false,
                false);
        }
    }

    if (sourceWidget == nullptr)
        return;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
    distributeMouseEvent(sourceWidget,
        MouseEventType::RELEASED,
        mouseInput.getButton(),
        mouseInput.getX(),
        mouseInput.getY(),
        false,
        false);

    if (mouseInput.getButton() == mLastMousePressButton
        && mFocusHandler->getLastWidgetPressed() == sourceWidget)
    {
        distributeMouseEvent(sourceWidget,
            MouseEventType::CLICKED,
            mouseInput.getButton(),
            mouseInput.getX(),
            mouseInput.getY(),
            false,
            false);

        mFocusHandler->setLastWidgetPressed(nullptr);
    }
    else
    {
        mLastMousePressButton = MouseButton::EMPTY;
        mClickCount = 0;
    }

    if (mFocusHandler->getDraggedWidget() != nullptr)
        mFocusHandler->setDraggedWidget(nullptr);
}

void Gui::addGlobalFocusListener(FocusListener* focusListener)
{
    mFocusListeners.push_back(focusListener);
}

void Gui::removeGlobalFocusListener(FocusListener* focusListener)
{
    mFocusListeners.remove(focusListener);
}

void Gui::distributeGlobalFocusGainedEvent(const Event &focusEvent)
{
    for (FocusListenerIterator iter = mFocusListeners.begin();
         iter != mFocusListeners.end();
         ++ iter)
    {
        (*iter)->focusGained(focusEvent);
    }
}

void Gui::removeDragged(const Widget *const widget)
{
    if (mFocusHandler == nullptr)
        return;

    if (mFocusHandler->getDraggedWidget() == widget)
        mFocusHandler->setDraggedWidget(nullptr);
}

MouseStateType Gui::getMouseState(int &x, int &y)
{
    const MouseStateType res = SDL_GetMouseState(&x, &y);
    const int scale = mainGraphics->getScale();
    x /= scale;
    y /= scale;
    return res;
}

void Gui::setTop(Widget *const top)
{
    if (mTop != nullptr)
        mTop->setFocusHandler(nullptr);
    if (top != nullptr)
        top->setFocusHandler(mFocusHandler);

    mTop = top;
}

void Gui::setGraphics(Graphics *const graphics)
{
    mGraphics = graphics;
}

Graphics* Gui::getGraphics() const
{
    return mGraphics;
}

void Gui::setInput(SDLInput *const input)
{
    mInput = input;
}

SDLInput* Gui::getInput() const
{
    return mInput;
}

void Gui::addGlobalKeyListener(KeyListener *const keyListener)
{
    mKeyListeners.push_back(keyListener);
}

void Gui::removeGlobalKeyListener(KeyListener *const keyListener)
{
    mKeyListeners.remove(keyListener);
}

void Gui::handleMouseWheelMovedDown(const MouseInput& mouseInput)
{
    Widget* sourceWidget = getMouseEventSource(
        mouseInput.getX(), mouseInput.getY());

    if (mFocusHandler->getDraggedWidget() != nullptr)
        sourceWidget = mFocusHandler->getDraggedWidget();

    if (sourceWidget != nullptr)
    {
        int sourceWidgetX = 0;
        int sourceWidgetY = 0;

        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
        distributeMouseEvent(sourceWidget,
            MouseEventType::WHEEL_MOVED_DOWN,
            mouseInput.getButton(),
            mouseInput.getX(),
            mouseInput.getY(),
            false,
            false);
    }
}

void Gui::handleMouseWheelMovedUp(const MouseInput& mouseInput)
{
    Widget* sourceWidget = getMouseEventSource(
        mouseInput.getX(), mouseInput.getY());

    if (mFocusHandler->getDraggedWidget() != nullptr)
        sourceWidget = mFocusHandler->getDraggedWidget();

    if (sourceWidget != nullptr)
    {
        int sourceWidgetX;
        int sourceWidgetY;

        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
        distributeMouseEvent(sourceWidget,
            MouseEventType::WHEEL_MOVED_UP,
            mouseInput.getButton(),
            mouseInput.getX(),
            mouseInput.getY(),
            false,
            false);
    }
}

Widget* Gui::getWidgetAt(const int x, const int y) const
{
    // If the widget's parent has no child then we have found the widget..
    Widget* parent = mTop;
    Widget* child = mTop;
    Widget* selectable = mTop;

    while (child != nullptr)
    {
        Widget *const swap = child;
        int parentX, parentY;
        parent->getAbsolutePosition(parentX, parentY);
        child = parent->getWidgetAt(x - parentX, y - parentY);
        parent = swap;
        if (parent->isSelectable())
            selectable = parent;
    }

    return selectable;
}

Widget* Gui::getMouseEventSource(const int x, const int y) const
{
    Widget *const widget = getWidgetAt(x, y);
    if (widget == nullptr)
        return nullptr;

    if (mFocusHandler != nullptr &&
        mFocusHandler->getModalMouseInputFocused() != nullptr &&
        !widget->isModalMouseInputFocused())
    {
        return mFocusHandler->getModalMouseInputFocused();
    }

    return widget;
}

Widget* Gui::getKeyEventSource() const
{
    Widget* widget = mFocusHandler->getFocused();

    while (widget != nullptr &&
           widget->getInternalFocusHandler() != nullptr &&
           widget->getInternalFocusHandler()->getFocused() != nullptr)
    {
        widget = widget->getInternalFocusHandler()->getFocused();
    }

    return widget;
}

void Gui::distributeKeyEvent(KeyEvent &event) const
{
    Widget* parent = event.getSource();
    Widget* widget = parent;

    if (parent == nullptr)
        return;
    if (mFocusHandler->getModalFocused() != nullptr &&
        !widget->isModalFocused())
    {
        return;
    }
    if (mFocusHandler->getModalMouseInputFocused() != nullptr &&
        !widget->isModalMouseInputFocused())
    {
        return;
    }

    while (parent != nullptr)
    {
        // If the widget has been removed due to input
        // cancel the distribution.
        if (!Widget::widgetExists(widget))
            break;

        parent = widget->getParent();

        if (widget->isEnabled())
        {
            std::list<KeyListener*> keyListeners
                = widget->getKeyListeners();

            const KeyEventTypeT eventType = event.getType();
            // Send the event to all key listeners of the source widget.
            FOR_EACH (std::list<KeyListener*>::const_iterator,
                 it, keyListeners)
            {
                switch (eventType)
                {
                    case KeyEventType::PRESSED:
                        (*it)->keyPressed(event);
                        break;
                    case KeyEventType::RELEASED:
                        (*it)->keyReleased(event);
                        break;
                    default:
                        break;
                }
            }
        }

        const Widget *const swap = widget;
        widget = parent;
        parent = swap->getParent();

        // If a non modal focused widget has been reach
        // and we have modal focus cancel the distribution.
        if ((widget != nullptr) &&
            (mFocusHandler->getModalFocused() != nullptr) &&
            !widget->isModalFocused())
        {
            break;
        }
    }
}

void Gui::distributeKeyEventToGlobalKeyListeners(KeyEvent& event)
{
    BLOCK_START("Gui::distributeKeyEventToGlobalKeyListeners")
    const KeyEventTypeT eventType = event.getType();
    FOR_EACH (KeyListenerListIterator, it, mKeyListeners)
    {
        switch (eventType)
        {
            case KeyEventType::PRESSED:
                (*it)->keyPressed(event);
                break;
            case KeyEventType::RELEASED:
                (*it)->keyReleased(event);
                break;
            default:
                break;
        }

        if (event.isConsumed())
            break;
    }
    BLOCK_END("Gui::distributeKeyEventToGlobalKeyListeners")
}

void Gui::handleModalMouseInputFocus()
{
    BLOCK_START("Gui::handleModalMouseInputFocus")
    Widget *const lastModalWidget
        = mFocusHandler->getLastWidgetWithModalMouseInputFocus();
    Widget *const modalWidget = mFocusHandler->getModalMouseInputFocused();
    if (lastModalWidget != modalWidget)
    {
        // Check if modal mouse input focus has been gained by a widget.
        if (lastModalWidget == nullptr)
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(modalWidget);
        }
        // Check if modal mouse input focus has been released.
        else
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(nullptr);
        }
    }
    BLOCK_END("Gui::handleModalMouseInputFocus")
}

void Gui::handleModalFocus()
{
    BLOCK_START("Gui::handleModalFocus")
    Widget *const lastModalWidget
        = mFocusHandler->getLastWidgetWithModalFocus();
    Widget *const modalWidget = mFocusHandler->getModalFocused();

    if (lastModalWidget != modalWidget)
    {
        // Check if modal focus has been gained by a widget.
        if (lastModalWidget == nullptr)
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalFocus(modalWidget);
        }
        // Check if modal focus has been released.
        else
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalFocus(nullptr);
        }
    }
    BLOCK_END("Gui::handleModalFocus")
}

void Gui::handleModalFocusGained()
{
    // Distribute an event to all widgets in the "widget with mouse" queue.
    while (!mWidgetWithMouseQueue.empty())
    {
        Widget *const widget = mWidgetWithMouseQueue.front();

        if (Widget::widgetExists(widget))
        {
            distributeMouseEvent(widget,
                MouseEventType::EXITED,
                mLastMousePressButton,
                mLastMouseX,
                mLastMouseY,
                true,
                true);
        }

        mWidgetWithMouseQueue.pop_front();
    }

    mFocusHandler->setLastWidgetWithModalMouseInputFocus(
        mFocusHandler->getModalMouseInputFocused());
}

void Gui::handleModalFocusReleased()
{
      // Check all widgets below the mouse to see if they are
    // present in the "widget with mouse" queue. If a widget
    // is not then it should be added and an entered event should
    // be sent to it.
    Widget* widget = getMouseEventSource(mLastMouseX, mLastMouseY);
    Widget* parent = widget;

    while (parent != nullptr &&
           widget != nullptr)
    {
        parent = widget->getParent();

        // Check if the widget is present in the "widget with mouse" queue.
        bool widgetIsPresentInQueue = false;
        FOR_EACH (std::deque<Widget*>::const_iterator,
                  iter, mWidgetWithMouseQueue)
        {
            if (*iter == widget)
            {
                widgetIsPresentInQueue = true;
                break;
            }
        }

        // Widget is not present, send an entered event and add
        // it to the "widget with mouse" queue.
        if (!widgetIsPresentInQueue && Widget::widgetExists(widget))
        {
            distributeMouseEvent(widget,
                MouseEventType::ENTERED,
                mLastMousePressButton,
                mLastMouseX,
                mLastMouseY,
                false,
                true);
            mWidgetWithMouseQueue.push_front(widget);
        }

        const Widget *const swap = widget;
        widget = parent;
        parent = swap->getParent();
    }
}

int Gui::getMousePressLength() const
{
    if (mLastMousePressTimeStamp == 0u)
        return 0;
    unsigned int ticks = SDL_GetTicks();
    if (ticks > mLastMousePressTimeStamp)
        return ticks - mLastMousePressTimeStamp;
    return mLastMousePressTimeStamp - ticks;
}
