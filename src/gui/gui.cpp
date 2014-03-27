/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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
#include "gui/font.h"
#include "gui/palette.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/window.h"

#include "client.h"
#include "configuration.h"
#include "dragdrop.h"
#include "touchmanager.h"

#include "events/keyevent.h"

#include "listeners/focuslistener.h"
#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "input/inputmanager.h"
#include "input/keydata.h"
#include "input/keyinput.h"
#include "input/mouseinput.h"

#include "resources/cursor.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/langs.h"
#include "utils/timer.h"

#include "debug.h"

// Guichan stuff
Gui *gui = nullptr;
SDLInput *guiInput = nullptr;

// Bolded font
Font *boldFont = nullptr;

class GuiConfigListener final : public ConfigListener
{
    public:
        explicit GuiConfigListener(Gui *const g):
            mGui(g)
        {}

        A_DELETE_COPY(GuiConfigListener)

        virtual ~GuiConfigListener()
        {
            CHECKLISTENERS
        }

        void optionChanged(const std::string &name)
        {
            if (!mGui)
                return;
            if (name == "customcursor")
                mGui->setUseCustomCursor(config.getBoolValue("customcursor"));
            else if (name == "doubleClick")
                mGui->setDoubleClick(config.getBoolValue("doubleClick"));
        }
    private:
        Gui *mGui;
};

Gui::Gui() :
    mTop(nullptr),
    mGraphics(nullptr),
    mInput(nullptr),
    mFocusHandler(new FocusHandler),
    mKeyListeners(),
    mLastMousePressButton(0),
    mLastMousePressTimeStamp(0),
    mLastMouseX(0),
    mLastMouseY(0),
    mClickCount(1),
    mLastMouseDragButton(0),
    mWidgetWithMouseQueue(),
    mConfigListener(new GuiConfigListener(this)),
    mGuiFont(),
    mInfoParticleFont(),
    mHelpFont(),
    mSecureFont(),
    mNpcFont(),
    mMouseCursors(nullptr),
    mMouseCursorAlpha(1.0F),
    mMouseInactivityTimer(0),
    mCursorType(Cursor::CURSOR_POINTER),
#ifdef ANDROID
    mLastMouseRealX(0),
    mLastMouseRealY(0),
#endif
    mFocusListeners(),
    mForegroundColor(theme->getColor(Theme::TEXT, 255)),
    mForegroundColor2(theme->getColor(Theme::TEXT_OUTLINE, 255)),
    mTime(0),
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
    guiTop->setOpaque(false);
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

    mGuiFont = new Font(fontFile, fontSize);

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

    boldFont = new Font(fontFile, fontSize);

    // Set help font
    fontFile = config.getValue("helpFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("helpFont");

    mHelpFont = new Font(fontFile, fontSize);

    // Set secure font
    fontFile = config.getValue("secureFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("secureFont");

    mSecureFont = new Font(fontFile, fontSize);

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

    mNpcFont = new Font(fontFile, npcFontSize);

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

    if (mMouseCursors)
    {
        mMouseCursors->decRef();
        mMouseCursors = nullptr;
    }

    delete2(mGuiFont);
    delete2(boldFont);
    delete2(mHelpFont);
    delete2(mSecureFont);
    delete2(mInfoParticleFont);
    delete2(mNpcFont);
    delete getTop();

    delete2(guiInput);

    delete2(theme);

    if (Widget::widgetExists(mTop))
        setTop(nullptr);

    delete2(mFocusHandler);
}

void Gui::logic()
{
    BLOCK_START("Gui::logic")
    ResourceManager *const resman = ResourceManager::getInstance();
    resman->clearScheduled();

    if (!mTop)
    {
        BLOCK_END("Gui::logic")
        return;
    }

    handleModalFocus();
    handleModalMouseInputFocus();

    if (guiInput)
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
    if (mGuiFont)
        mGuiFont->slowLogic(0);
    if (mInfoParticleFont)
        mInfoParticleFont->slowLogic(1);
    if (mHelpFont)
        mHelpFont->slowLogic(2);
    if (mSecureFont)
        mSecureFont->slowLogic(3);
    if (boldFont)
        boldFont->slowLogic(4);
    if (mNpcFont)
        mNpcFont->slowLogic(5);
    if (windowContainer)
        windowContainer->slowLogic();

    const int time = cur_time;
    if (mTime != time)
    {
        logger->flush();
        mTime = time;
    }

    BLOCK_END("Gui::slowLogic")
}

void Gui::clearFonts()
{
    if (mGuiFont)
        mGuiFont->clear();
    if (mInfoParticleFont)
        mInfoParticleFont->clear();
    if (mHelpFont)
        mHelpFont->clear();
    if (mSecureFont)
        mSecureFont->clear();
    if (boldFont)
        boldFont->clear();
    if (mNpcFont)
        mNpcFont->clear();
}

bool Gui::handleInput()
{
    if (mInput)
        return handleKeyInput();
    else
        return false;
}

bool Gui::handleKeyInput()
{
    if (!guiInput)
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
#endif

        distributeKeyEventToGlobalKeyListeners(
            eventToGlobalKeyListeners);

        // If a global key listener consumes the event it will not be
        // sent further to the source of the event.
        if (eventToGlobalKeyListeners.isConsumed())
        {
            consumed = true;
            continue;
        }

        if (mFocusHandler)
        {
            bool eventConsumed = false;

            // Send key inputs to the focused widgets
            if (mFocusHandler->getFocused())
            {
                KeyEvent event(getKeyEventSource(),
                    keyInput.getType(),
                    keyInput.getActionId(), keyInput.getKey());
#ifdef USE_SDL2
                if (!keyInput.getText().empty())
                    event.setText(keyInput.getText());
#endif

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
                == static_cast<int>(Input::KEY_GUI_TAB)
                && keyInput.getType() == KeyInput::PRESSED)
            {
                if (inputManager.isActionActive(Input::KEY_GUI_MOD))
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
    mGraphics->pushClipArea(getTop()->getDimension());
    getTop()->draw(mGraphics);
    touchManager.draw();

    int mouseX;
    int mouseY;
    const uint8_t button = getMouseState(&mouseX, &mouseY);

    if ((client->getMouseFocused() || button & SDL_BUTTON(1))
        && mMouseCursors && mCustomCursor && mMouseCursorAlpha > 0.0F)
    {
        const Image *const image = dragDrop.getItemImage();
        if (image)
        {
            const int posX = mouseX - (image->mBounds.w / 2);
            const int posY = mouseY - (image->mBounds.h / 2);
            mGraphics->drawImage(image, posX, posY);
        }
        if (mGuiFont)
        {
            const std::string &str = dragDrop.getText();
            if (!str.empty())
            {
                const int posX = mouseX - mGuiFont->getWidth(str) / 2;
                const int posY = mouseY + (image ? image->mBounds.h / 2 : 0);
                mGraphics->setColorAll(mForegroundColor, mForegroundColor2);
                mGuiFont->drawString(mGraphics, str, posX, posY);
            }
        }

        Image *const mouseCursor = mMouseCursors->get(mCursorType);
        if (mouseCursor)
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
    WindowContainer *const top = static_cast<WindowContainer* const>(getTop());

    if (top)
    {
        const int oldWidth = top->getWidth();
        const int oldHeight = top->getHeight();

        top->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
        top->adjustAfterResize(oldWidth, oldHeight);
    }
}

void Gui::setUseCustomCursor(const bool customCursor)
{
    if (customCursor != mCustomCursor)
    {
        mCustomCursor = customCursor;

        if (mCustomCursor)
        {
            // Hide the SDL mouse cursor
            SDL_ShowCursor(SDL_DISABLE);

            // Load the mouse cursor
            if (mMouseCursors)
                mMouseCursors->decRef();
            mMouseCursors = Theme::getImageSetFromTheme("mouse.png", 40, 40);

            if (!mMouseCursors)
                logger->log("Error: Unable to load mouse cursors.");
        }
        else
        {
            // Show the SDL mouse cursor
            SDL_ShowCursor(SDL_ENABLE);

            // Unload the mouse cursor
            if (mMouseCursors)
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
    if (!mWidgetWithMouseQueue.empty() && (mouseInput.getX() < 0
        || mouseInput.getY() < 0 || !mTop->getDimension().isPointInRect(
        mouseInput.getX(), mouseInput.getY())))
    {
        // Distribute an event to all widgets in the
        // "widget with mouse" queue.
        while (!mWidgetWithMouseQueue.empty())
        {
            Widget *const widget = mWidgetWithMouseQueue.front();

            if (Widget::widgetExists(widget))
            {
                distributeMouseEvent(widget,
                                     MouseEvent::EXITED,
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
    const int button = mouseInput.getButton();

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
                                         MouseEvent::EXITED,
                                         button,
                                         mouseX,
                                         mouseY,
                                         true,
                                         true);
                    mClickCount = 1;
                    mLastMousePressTimeStamp = 0;
                    mWidgetWithMouseQueue.erase(iter);
                    break;
                }
            }

            iterations++;
        }

        widgetWithMouseQueueCheckDone =
            (iterations == mWidgetWithMouseQueue.size());
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
    if (mFocusHandler->getModalMouseInputFocused()
        && widget == mFocusHandler->getModalMouseInputFocused()
        && Widget::widgetExists(widget))
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

    while (parent)
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
                                 MouseEvent::ENTERED,
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

    if (mFocusHandler->getDraggedWidget())
    {
        distributeMouseEvent(mFocusHandler->getDraggedWidget(),
                             MouseEvent::DRAGGED,
                             mLastMouseDragButton,
                             mouseX,
                             mouseY);
    }
    else
    {
        Widget *const sourceWidget = getMouseEventSource(mouseX, mouseY);
        distributeMouseEvent(sourceWidget,
                             MouseEvent::MOVED,
                             button,
                             mouseX,
                             mouseY);
    }
    mMouseInactivityTimer = 0;
}

void Gui::handleMousePressed(const MouseInput &mouseInput)
{
    const int x = mouseInput.getX();
    const int y = mouseInput.getY();
    const unsigned int button = mouseInput.getButton();
    const int timeStamp = mouseInput.getTimeStamp();

    Widget *sourceWidget = getMouseEventSource(x, y);

    if (mFocusHandler->getDraggedWidget())
        sourceWidget = mFocusHandler->getDraggedWidget();

    int sourceWidgetX;
    int sourceWidgetY;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

    if ((mFocusHandler->getModalFocused()
        && sourceWidget->isModalFocused())
        || !mFocusHandler->getModalFocused())
    {
        sourceWidget->requestFocus();
    }

    if (mDoubleClick && timeStamp - mLastMousePressTimeStamp < 250
        && mLastMousePressButton == button)
    {
        mClickCount ++;
    }
    else
    {
        mClickCount = 1;
    }

    distributeMouseEvent(sourceWidget, MouseEvent::PRESSED, button, x, y);
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

    mGuiFont->loadFont(fontFile, fontSize);

    fontFile = config.getValue("particleFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    mInfoParticleFont->loadFont(fontFile, fontSize, TTF_STYLE_BOLD);

    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    boldFont->loadFont(fontFile, fontSize);

    const int npcFontSize = config.getIntValue("npcfontSize");

    fontFile = config.getValue("npcFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("npcFont");

    mNpcFont->loadFont(fontFile, npcFontSize);
}

void Gui::distributeMouseEvent(Widget *const source,
                               const int type,
                               const int button,
                               const int x, const int y,
                               const bool force,
                               const bool toSourceOnly)
{
    if (!source || !mFocusHandler)
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
    while (parent)
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

            unsigned int mouseType = event.getType();
            // Send the event to all mouse listeners of the widget.
            FOR_EACH (std::list<MouseListener*>::const_iterator,
                 it, mouseListeners)
            {
                switch (mouseType)
                {
                    case MouseEvent::ENTERED:
                        (*it)->mouseEntered(event);
                        break;
                    case MouseEvent::EXITED:
                        (*it)->mouseExited(event);
                        break;
                    case MouseEvent::MOVED:
                        (*it)->mouseMoved(event);
                        break;
                    case MouseEvent::PRESSED:
                        (*it)->mousePressed(event);
                        break;
                    case MouseEvent::RELEASED:
                    case 100:  // manual hack for release on target after drag
                        (*it)->mouseReleased(event);
                        break;
                    case MouseEvent::WHEEL_MOVED_UP:
                        (*it)->mouseWheelMovedUp(event);
                        break;
                    case MouseEvent::WHEEL_MOVED_DOWN:
                        (*it)->mouseWheelMovedDown(event);
                        break;
                    case MouseEvent::DRAGGED:
                        (*it)->mouseDragged(event);
                        break;
                    case MouseEvent::CLICKED:
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

        if (type == MouseEvent::RELEASED)
            dragDrop.clear();

        if (event.isConsumed())
            break;

        // If a non modal focused widget has been reach
        // and we have modal focus cancel the distribution.
        if (mFocusHandler->getModalFocused()
            && !widget->isModalFocused())
        {
            break;
        }

        // If a non modal mouse input focused widget has been reach
        // and we have modal mouse input focus cancel the distribution.
        if (mFocusHandler->getModalMouseInputFocused()
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
    if (!viewport || !widget)
        return nullptr;

    int x = 0;
    int y = 0;
    int mouseX = 0;
    int mouseY = 0;

    getAbsolutePosition(widget, x, y);
    getMouseState(&mouseX, &mouseY);

    return new MouseEvent(widget,
        0, 0,
        mouseX - x, mouseY - y,
        mClickCount);
}

void Gui::getAbsolutePosition(Widget *restrict widget,
                              int &restrict x, int &restrict y)
{
    x = 0;
    y = 0;
    while (widget->getParent())
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
#endif
#endif
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
#endif
        switch (mouseInput.getType())
        {
            case MouseInput::PRESSED:
                handleMousePressed(mouseInput);
                break;
            case MouseInput::RELEASED:
                handleMouseReleased(mouseInput);
                break;
            case MouseInput::MOVED:
                handleMouseMoved(mouseInput);
                break;
            case MouseInput::WHEEL_MOVED_DOWN:
                handleMouseWheelMovedDown(mouseInput);
                break;
            case MouseInput::WHEEL_MOVED_UP:
                handleMouseWheelMovedUp(mouseInput);
                break;
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
    if (mFocusHandler->getDraggedWidget())
    {
        if (sourceWidget != mFocusHandler->getLastWidgetPressed())
            mFocusHandler->setLastWidgetPressed(nullptr);

        Widget *const oldWidget = sourceWidget;
        sourceWidget = mFocusHandler->getDraggedWidget();
        if (oldWidget != sourceWidget)
        {
            oldWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
            distributeMouseEvent(oldWidget,
                100,
                mouseInput.getButton(),
                mouseInput.getX(),
                mouseInput.getY());
        }
    }

    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
    distributeMouseEvent(sourceWidget,
                         MouseEvent::RELEASED,
                         mouseInput.getButton(),
                         mouseInput.getX(),
                         mouseInput.getY());

    if (mouseInput.getButton() == mLastMousePressButton
        && mFocusHandler->getLastWidgetPressed() == sourceWidget)
    {
        distributeMouseEvent(sourceWidget,
                             MouseEvent::CLICKED,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());

        mFocusHandler->setLastWidgetPressed(nullptr);
    }
    else
    {
        mLastMousePressButton = 0;
        mClickCount = 0;
    }

    if (mFocusHandler->getDraggedWidget())
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

void Gui::removeDragged(Widget *widget)
{
    if (!mFocusHandler)
        return;

    if (mFocusHandler->getDraggedWidget() == widget)
        mFocusHandler->setDraggedWidget(nullptr);
}

uint32_t Gui::getMouseState(int *const x, int *const y)
{
    const uint32_t res = SDL_GetMouseState(x, y);
    const int scale = mainGraphics->getScale();
    (*x) /= scale;
    (*y) /= scale;
    return res;
}

void Gui::setTop(Widget *const top)
{
    if (mTop)
        mTop->setFocusHandler(nullptr);
    if (top)
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

    if (mFocusHandler->getDraggedWidget())
        sourceWidget = mFocusHandler->getDraggedWidget();

    int sourceWidgetX = 0;
    int sourceWidgetY = 0;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

    distributeMouseEvent(sourceWidget,
                         MouseEvent::WHEEL_MOVED_DOWN,
                         mouseInput.getButton(),
                         mouseInput.getX(),
                         mouseInput.getY());
}

void Gui::handleMouseWheelMovedUp(const MouseInput& mouseInput)
{
    Widget* sourceWidget = getMouseEventSource(
        mouseInput.getX(), mouseInput.getY());

    if (mFocusHandler->getDraggedWidget())
        sourceWidget = mFocusHandler->getDraggedWidget();

    int sourceWidgetX, sourceWidgetY;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

    distributeMouseEvent(sourceWidget,
                         MouseEvent::WHEEL_MOVED_UP,
                         mouseInput.getButton(),
                         mouseInput.getX(),
                         mouseInput.getY());
}

Widget* Gui::getWidgetAt(const int x, const int y) const
{
    // If the widget's parent has no child then we have found the widget..
    Widget* parent = mTop;
    Widget* child = mTop;

    while (child)
    {
        Widget *const swap = child;
        int parentX, parentY;
        parent->getAbsolutePosition(parentX, parentY);
        child = parent->getWidgetAt(x - parentX, y - parentY);
        parent = swap;
    }

    return parent;
}

Widget* Gui::getMouseEventSource(const int x, const int y) const
{
    Widget *const widget = getWidgetAt(x, y);
    if (!widget)
        return nullptr;

    if (mFocusHandler && mFocusHandler->getModalMouseInputFocused()
        && !widget->isModalMouseInputFocused())
    {
        return mFocusHandler->getModalMouseInputFocused();
    }

    return widget;
}

Widget* Gui::getKeyEventSource() const
{
    Widget* widget = mFocusHandler->getFocused();

    while (widget && widget->getInternalFocusHandler()
           && widget->getInternalFocusHandler()->getFocused())
    {
        widget = widget->getInternalFocusHandler()->getFocused();
    }

    return widget;
}

void Gui::distributeKeyEvent(KeyEvent &event) const
{
    Widget* parent = event.getSource();
    Widget* widget = event.getSource();

    if (mFocusHandler->getModalFocused() && !widget->isModalFocused())
        return;

    if (mFocusHandler->getModalMouseInputFocused()
        && !widget->isModalMouseInputFocused())
    {
        return;
    }

    while (parent)
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

            const unsigned int eventType = event.getType();
            // Send the event to all key listeners of the source widget.
            FOR_EACH (std::list<KeyListener*>::const_iterator,
                 it, keyListeners)
            {
                switch (eventType)
                {
                    case KeyEvent::PRESSED:
                        (*it)->keyPressed(event);
                        break;
                    case KeyEvent::RELEASED:
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
        if (mFocusHandler->getModalFocused() && !widget->isModalFocused())
            break;
    }
}

void Gui::distributeKeyEventToGlobalKeyListeners(KeyEvent& event)
{
    const unsigned int eventType = event.getType();
    FOR_EACH (KeyListenerListIterator, it, mKeyListeners)
    {
        switch (eventType)
        {
            case KeyEvent::PRESSED:
                (*it)->keyPressed(event);
                break;
            case KeyEvent::RELEASED:
                (*it)->keyReleased(event);
                break;
            default:
                break;
        }

        if (event.isConsumed())
            break;
    }
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
        if (!lastModalWidget)
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
        if (!lastModalWidget)
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
                                 MouseEvent::EXITED,
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

    while (parent)
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
                                 MouseEvent::ENTERED,
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
    if (!mLastMousePressTimeStamp)
        return 0;
    int ticks = SDL_GetTicks();
    if (ticks > mLastMousePressTimeStamp)
        return ticks - mLastMousePressTimeStamp;
    return mLastMousePressTimeStamp - ticks;
}
