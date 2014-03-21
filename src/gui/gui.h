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

#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "gui/color.h"

#include <deque>
#include <list>

#include "localconsts.h"

class Event;
class FocusHandler;
class FocusListener;
class Graphics;
class GuiConfigListener;
class ImageSet;
class KeyEvent;
class KeyListener;
class MouseEvent;
class MouseInput;
class Font;
class SDLInput;
class Widget;
class Window;

/**
 * \defgroup GUI Core GUI related classes (widgets)
 */

/**
 * \defgroup Interface User interface related classes (windows, dialogs)
 */

/**
 * Main GUI class.
 *
 * \ingroup GUI
 */
class Gui final
{
    public:
        /**
         * Constructor.
         */
        Gui();

        A_DELETE_COPY(Gui)

        /**
         * Destructor.
         */
        ~Gui();

        void postInit(Graphics *const graphics);

        /**
         * Performs logic of the GUI. Overridden to track mouse pointer
         * activity.
         */
        void logic();

        void slowLogic();

        void clearFonts();

        /**
         * Draws the whole Gui by calling draw functions down in the
         * Gui hierarchy. It also draws the mouse pointer.
         */
        void draw();

        /**
         * Called when the application window has been resized.
         */
        void videoResized() const;

        FocusHandler *getFocusHandler() const A_WARN_UNUSED
        { return mFocusHandler; }

        /**
         * Return game font.
         */
        Font *getFont() const A_WARN_UNUSED
        { return mGuiFont; }

        /**
         * Return help font.
         */
        Font *getHelpFont() const A_WARN_UNUSED
        { return mHelpFont; }

        /**
         * Return secure font.
         */
        Font *getSecureFont() const A_WARN_UNUSED
        { return mSecureFont; }

        /**
         * Return npc font.
         */
        Font *getNpcFont() const A_WARN_UNUSED
        { return mNpcFont; }

        /**
         * Return the Font used for "Info Particles", i.e. ones showing, what
         * you picked up, etc.
         */
        Font *getInfoParticleFont() const A_WARN_UNUSED
        { return mInfoParticleFont; }

        /**
         * Sets whether a custom cursor should be rendered.
         */
        void setUseCustomCursor(const bool customCursor);

        /**
         * Sets which cursor should be used.
         */
        void setCursorType(const int index)
        { mCursorType = index; }

        void setDoubleClick(const bool b)
        { mDoubleClick = b; }

        void updateFonts();

        bool handleInput();

        bool handleKeyInput();

        void resetClickCount();

        MouseEvent *createMouseEvent(Window *const widget) A_WARN_UNUSED;

        static void getAbsolutePosition(Widget *restrict widget,
                                        int &restrict x,
                                        int &restrict y);

        void addGlobalFocusListener(FocusListener* focusListener);

        void removeGlobalFocusListener(FocusListener* focusListener);

        void distributeGlobalFocusGainedEvent(const Event &focusEvent);

        void removeDragged(Widget *widget);

        int getLastMouseX() const
        { return mLastMouseX; }

        int getLastMouseY() const
        { return mLastMouseY; }

        static uint32_t getMouseState(int *const x, int *const y);

        /**
         * Sets the top widget. The top widget is the root widget
         * of the GUI. If you want a GUI to be able to contain more
         * than one widget the top widget should be a container.
         *
         * @param top The top widget.
         * @see Container
         * @since 0.1.0
         */
        void setTop(Widget *const top);

        /**
         * Gets the top widget. The top widget is the root widget
         * of the GUI.
         *
         * @return The top widget. NULL if no top widget has been set.
         * @since 0.1.0
         */
        Widget* getTop() const A_WARN_UNUSED
        { return mTop; }

        /**
         * Sets the graphics object to use for drawing.
         *
         * @param graphics The graphics object to use for drawing.
         * @see getGraphics, AllegroGraphics, HGEGraphics, 
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         * @since 0.1.0
         */
        void setGraphics(Graphics *const graphics);

        /**
         * Gets the graphics object used for drawing.
         *
         *  @return The graphics object used for drawing. NULL if no
         *          graphics object has been set.
         * @see setGraphics, AllegroGraphics, HGEGraphics, 
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         * @since 0.1.0
         */
        Graphics* getGraphics() const A_WARN_UNUSED;

        /**
         * Sets the input object to use for input handling.
         *
         * @param input The input object to use for input handling.
         * @see getInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         * @since 0.1.0
         */
        void setInput(SDLInput *const input);

        /**
         * Gets the input object being used for input handling.
         *
         *  @return The input object used for handling input. NULL if no
         *          input object has been set.
         * @see setInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         * @since 0.1.0
         */
        SDLInput* getInput() const A_WARN_UNUSED;

        /**
         * Adds a global key listener to the Gui. A global key listener
         * will receive all key events generated from the GUI and global
         * key listeners will receive the events before key listeners
         * of widgets.
         *
         * @param keyListener The key listener to add.
         * @see removeGlobalKeyListener
         * @since 0.5.0
         */
        void addGlobalKeyListener(KeyListener *const keyListener);

        /**
         * Removes global key listener from the Gui.
         *
         * @param keyListener The key listener to remove.
         * @throws Exception if the key listener hasn't been added.
         * @see addGlobalKeyListener
         * @since 0.5.0
         */
        void removeGlobalKeyListener(KeyListener *const keyListener);

        bool isLongPress() const
        { return getMousePressLength() > 250; }

        int getMousePressLength() const;

    protected:
        void handleMouseMoved(const MouseInput &mouseInput);

        void handleMouseReleased(const MouseInput &mouseInput);

        void handleMousePressed(const MouseInput &mouseInput);

        void handleMouseInput();

        void distributeMouseEvent(Widget *const source,
                                  const int type,
                                  const int button,
                                  const int x, const int y,
                                  const bool force = false,
                                  const bool toSourceOnly = false);

        /**
         *
         * Handles mouse wheel moved down input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        void handleMouseWheelMovedDown(const MouseInput& mouseInput);

        /**
         * Handles mouse wheel moved up input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        void handleMouseWheelMovedUp(const MouseInput& mouseInput);

        /**
         * Gets the widget at a certain position.
         *
         * @return The widget at a certain position.
         * @since 0.6.0
         */
        Widget* getWidgetAt(const int x, const int y) const A_WARN_UNUSED;

        /**
         * Gets the source of the mouse event.
         *
         * @return The source widget of the mouse event.
         * @since 0.6.0
         */
        Widget* getMouseEventSource(const int x,
                                    const int y) const A_WARN_UNUSED;

        /**
         * Gets the source of the key event.
         *
         * @return The source widget of the key event.
         * @since 0.6.0
         */
        Widget* getKeyEventSource() const A_WARN_UNUSED;

        /**
         * Distributes a key event.
         *
         * @param event The key event to distribute.

         * @since 0.6.0
         */
        void distributeKeyEvent(KeyEvent &event) const;

        /**
         * Distributes a key event to the global key listeners.
         *
         * @param event The key event to distribute.
         *
         * @since 0.6.0
         */
        void distributeKeyEventToGlobalKeyListeners(KeyEvent& event);

        /**
         * Handles modal mouse input focus. Modal mouse input focus needs 
         * to be checked at each logic iteration as it might be necessary to 
         * distribute mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        void handleModalMouseInputFocus();

        /**
         * Handles modal focus. Modal focus needs to be checked at 
         * each logic iteration as it might be necessary to distribute
         * mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        void handleModalFocus();

        /**
         * Handles modal focus gained. If modal focus has been gained it might
         * be necessary to distribute mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        void handleModalFocusGained();

        /**
         * Handles modal mouse input focus gained. If modal focus has been
         * gained it might be necessary to distribute mouse entered or mouse
         * exited events.
         *
         * @since 0.8.0
         */
        void handleModalFocusReleased();

    private:
        /**
         * Holds the top widget.
         */
        Widget* mTop;

        /**
         * Holds the graphics implementation used.
         */
        Graphics* mGraphics;

        /**
         * Holds the input implementation used.
         */
        SDLInput* mInput;

        /**
         * Holds the focus handler for the Gui.
         */
        FocusHandler* mFocusHandler;

        /**
         * Typedef.
         */
        typedef std::list<KeyListener*> KeyListenerList;

        /**
         * Typedef.
         */
        typedef KeyListenerList::iterator KeyListenerListIterator;

        /**
         * Holds the global key listeners of the Gui.
         */
        KeyListenerList mKeyListeners;

        /**
         * Holds the last mouse button pressed.
         */
        unsigned int mLastMousePressButton;

        /**
         * Holds the last mouse press time stamp.
         */
        int mLastMousePressTimeStamp;

        /**
         * Holds the last mouse x coordinate.
         */
        int mLastMouseX;

        /**
         * Holds the last mouse y coordinate.
         */
        int mLastMouseY;

        /**
         * Holds the current click count. Used to keep track
         * of clicks for a the last pressed button.
         */
        int mClickCount;

        /**
         * Holds the last button used when a drag of a widget
         * was initiated. Used to be able to release a drag
         * when the same button is released.
         */
        int mLastMouseDragButton;

        /**
         * Holds a stack with all the widgets with the mouse.
         * Used to properly distribute mouse events.
         */
        std::deque<Widget*> mWidgetWithMouseQueue;

        GuiConfigListener *mConfigListener;
        Font *mGuiFont;                  /**< The global GUI font */
        Font *mInfoParticleFont;         /**< Font for Info Particles */
        Font *mHelpFont;                 /**< Font for Help Window */
        Font *mSecureFont;               /**< Font for secure labels */
        Font *mNpcFont;                  /**< Font for npc text */
        ImageSet *mMouseCursors;            /**< Mouse cursor images */
        float mMouseCursorAlpha;
        int mMouseInactivityTimer;
        int mCursorType;
#ifdef ANDROID
        uint16_t mLastMouseRealX;
        uint16_t mLastMouseRealY;
#endif
        typedef std::list<FocusListener*> FocusListenerList;
        typedef FocusListenerList::iterator FocusListenerIterator;
        FocusListenerList mFocusListeners;
        Color mForegroundColor;
        Color mForegroundColor2;
        int mTime;
        bool mCustomCursor;                 /**< Show custom cursor */
        bool mDoubleClick;
};

extern Gui *gui;                            /**< The GUI system */
extern SDLInput *guiInput;                  /**< GUI input */

/**
 * Bolded text font
 */
extern Font *boldFont;

#endif  // GUI_GUI_H
