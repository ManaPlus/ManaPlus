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

#ifndef GUI_WIDGETS_WIDGET_H
#define GUI_WIDGETS_WIDGET_H

#include <list>
#include <set>
#include <string>

#include "gui/color.h"
#include "gui/rect.h"

#include "gui/widgets/widget2.h"

#include "localconsts.h"

class ActionListener;
class DeathListener;
class FocusHandler;
class FocusListener;
class Font;
class Graphics;
class KeyListener;
class MouseListener;
class WidgetListener;

/**
  * Abstract class for widgets of Guichan. It contains basic functions 
  * every widget should have.
  *
  * NOTE: Functions begining with underscore "_" should not
  *       be overloaded unless you know what you are doing
  *
  * @author Olof Naessén
  * @author Per Larsson.
  * @since 0.1.0
  */
class Widget : public Widget2
{
    public:
        friend class BasicContainer;

        /**
          * Constructor. Resets member variables. Noteable, a widget is not
          * focusable as default, therefore, widgets that are supposed to be
          * focusable should overide this default in their own constructor.
          */
        explicit Widget(const Widget2 *const widget);

        A_DELETE_COPY(Widget)

        /**
          * Default destructor.
          */
        virtual ~Widget();

        /**
          * Draws the widget. It is called by the parent widget when it is time
          * for the widget to draw itself. The graphics object is set up so
          * that all drawing is relative to the widget, i.e coordinate (0,0) is
          * the top left corner of the widget. It is not possible to draw
          * outside of a widget's dimension.
          *
          * @param graphics aA graphics object to draw with.
          * @since 0.1.0
          */
        virtual void draw(Graphics* graphics) = 0;

        /**
          * Called when a widget is given a chance to draw a frame around itself.
          * The frame is not considered a part of the widget, it only allows a frame
          * to be drawn around the widget, thus a frame will never be included when
          * calculating if a widget should receive events from user input. Also
          * a widget's frame will never be included when calculating a widget's 
          * position.
          * 
          * The size of the frame is calculated using the widget's frame size. 
          * If a widget has a frame size of 10 pixels than the area the drawFrame 
          * function can draw to will be the size of the widget with an additional 
          * extension of 10 pixels in each direction.
          *
          * An example when drawFrame is a useful function is if a widget needs
          * a glow around itself.
          *
          * @param graphics A graphics object to draw with.
          * @see setFrameSize, getFrameSize
          * @since 0.8.0
          */
        virtual void drawFrame(Graphics* graphics A_UNUSED)
        { }

        /**
          * Sets the size of the widget's frame. The frame is not considered a part of 
          * the widget, it only allows a frame to be drawn around the widget, thus a frame 
          * will never be included when calculating if a widget should receive events 
          * from user input. Also a widget's frame will never be included when calculating
          * a widget's position.
          *
          * A frame size of 0 means that the widget has no frame. The default frame size
          * is 0.
          *
          * @param frameSize The size of the widget's frame.
          * @see getFrameSize, drawFrame
          * @since 0.8.0
          */
        void setFrameSize(const unsigned int frameSize)
        { mFrameSize = frameSize; }

        /**
          * Gets the size of the widget's frame. The frame is not considered a part of 
          * the widget, it only allows a frame to be drawn around the widget, thus a frame 
          * will never be included when calculating if a widget should receive events 
          * from user input. Also a widget's frame will never be included when calculating
          * a widget's position.
          *
          * A frame size of 0 means that the widget has no frame. The default frame size
          * is 0.
          *
          * @return The size of the widget's frame.
          * @see setFrameSize, drawFrame
          * @since 0.8.0
          */
        unsigned int getFrameSize() const A_WARN_UNUSED
        { return mFrameSize; }

        /**
          * Called for all widgets in the gui each time Gui::logic is called.
          * You can do logic stuff here like playing an animation.
          *
          * @see Gui::logic
          * @since 0.1.0
          */
        virtual void logic()
        { }

        /**
          * Gets the widget's parent container.
          *
          * @return The widget's parent container. NULL if the widget
          *         has no parent.
          * @since 0.1.0
          */
        Widget* getParent() const A_WARN_UNUSED
        { return mParent; }

        /**
          * Sets the width of the widget.
          *
          * @param width The width of the widget.
          * @see getWidth, setHeight, getHeight, setSize,
          *      setDimension, getDimensi
          * @since 0.1.0
          */
        void setWidth(const int width);

        /**
          * Gets the width of the widget.
          *
          * @return The width of the widget.
          * @see setWidth, setHeight, getHeight, setSize,
          *      setDimension, getDimension
          * @since 0.1.0
          */
        int getWidth() const A_WARN_UNUSED
        { return mDimension.width; }

        /**
          * Sets the height of the widget.
          *
          * @param height The height of the widget.
          * @see getHeight, setWidth, getWidth, setSize,
          *      setDimension, getDimension
          * @since 0.1.0
          */
        void setHeight(const int height);

        /**
          * Gets the height of the widget.
          *
          * @return The height of the widget.
          * @see setHeight, setWidth, getWidth, setSize,
          *      setDimension, getDimension
          * @since 0.1.0
          */
        int getHeight() const A_WARN_UNUSED
        { return mDimension.height; }

        /**
          * Sets the size of the widget.
          *
          * @param width The width of the widget.
          * @param height The height of the widget.
          * @see setWidth, setHeight, getWidth, getHeight,
          *      setDimension, getDimension
          * @since 0.1.0
          */
        void setSize(const int width, const int height);

        /**
          * Sets the x coordinate of the widget. The coordinate is
          * relateive to the widget's parent.
          *
          * @param x The x coordinate of the widget.
          * @see getX, setY, getY, setPosition, setDimension, getDimension
          * @since 0.1.0
          */
        void setX(const int x);

        /**
          * Gets the x coordinate of the widget. The coordinate is
          * relative to the widget's parent.
          *
          * @return The x coordinate of the widget.
          * @see setX, setY, getY, setPosition, setDimension, getDimension
          * @since 0.1.0
          */
        int getX() const A_WARN_UNUSED
        { return mDimension.x; }

        /**
          * Sets the y coordinate of the widget. The coordinate is
          * relative to the widget's parent.
          *
          * @param y The y coordinate of the widget.
          * @see setY, setX, getX, setPosition, setDimension, getDimension
          * @since 0.1.0
          */
        void setY(const int y);

        /**
          * Gets the y coordinate of the widget. The coordinate is
          * relative to the widget's parent.
          *
          * @return The y coordinate of the widget.
          * @see setY, setX, getX, setPosition, setDimension, getDimension
          * @since 0.1.0
          */
        int getY() const A_WARN_UNUSED
        { return mDimension.y; }

        /**
          * Sets position of the widget. The position is relative
          * to the widget's parent.
          *
          * @param x The x coordinate of the widget.
          * @param y The y coordinate of the widget.
          * @see setX, getX, setY, getY, setDimension, getDimension
          * @since 0.1.0
          */
        void setPosition(const int x, const int y);

        /**
          * Sets the dimension of the widget. The dimension is
          * relative to the widget's parent.
          *
          * @param dimension The dimension of the widget.
          * @see getDimension, setX, getX, setY, getY, setPosition
          * @since 0.1.0
          */
        void setDimension(const Rect& dimension);

        /**
          * Gets the dimension of the widget. The dimension is
          * relative to the widget's parent.
          *
          * @return The dimension of the widget.
          * @see getDimension, setX, getX, setY, getY, setPosition
          * @since 0.1.0
          */
        const Rect& getDimension() const A_WARN_UNUSED
        { return mDimension; }

        /**
          * Sets the widget to be fosusable, or not.
          *
          * @param focusable True if the widget should be focusable,
          *                  false otherwise.
          * @see isFocusable
          * @since 0.1.0
          */
        void setFocusable(const bool focusable);

          /**
          * Checks if a widget is focsable.
          *
          * @return True if the widget should be focusable, false otherwise.
          * @see setFocusable
          * @since 0.1.0
          */
        bool isFocusable() const A_WARN_UNUSED;

        /**
          * Checks if the widget is focused.
          *
          * @return True if the widget is focused, false otherwise.
          * @since 0.1.0
          */
        virtual bool isFocused() const A_WARN_UNUSED;

        /**
          * Sets the widget to enabled, or not. A disabled
          * widget will never recieve mouse or key events.
          *
          * @param enabled True if widget should be enabled,
          *                false otherwise.
          * @see isEnabled
          * @since 0.1.0
          */
        void setEnabled(const bool enabled)
        { mEnabled = enabled; }

        /**
          * Checks if the widget is enabled. A disabled
          * widget will never recieve mouse or key events.
          *
          * @return True if widget is enabled, false otherwise.
          * @see setEnabled
          * @since 0.1.0
          */
        bool isEnabled() const A_WARN_UNUSED;

        /**
          * Sets the widget to be visible, or not.
          *
          * @param visible True if widget should be visible, false otherwise.
          * @see isVisible
          * @since 0.1.0
          */
        void setVisible(bool visible);

        /**
          * Checks if the widget is visible.
          *
          * @return True if widget is be visible, false otherwise.
          * @see setVisible
          * @since 0.1.0
          */
        bool isVisible() const A_WARN_UNUSED
        { return mVisible && (!mParent || mParent->isVisible()); }

        /**
          * Sets the base color of the widget.
          *
          * @param color The baseground color.
          * @see getBaseColor
          * @since 0.1.0
          */
        void setBaseColor(const Color& color)
        { mBaseColor = color; }

        /**
          * Gets the base color.
          *
          * @return The base color.
          * @see setBaseColor
          * @since 0.1.0
          */
        const Color& getBaseColor() const A_WARN_UNUSED
        { return mBaseColor; }

        /**
          * Sets the foreground color.
          *
          * @param color The foreground color.
          * @see getForegroundColor
          * @since 0.1.0
          */
        void setForegroundColor(const Color& color)
        { mForegroundColor = color; }

        /**
          * Gets the foreground color.
          *
          * @see setForegroundColor
          * @since 0.1.0
          */
        const Color& getForegroundColor() const A_WARN_UNUSED
        { return mForegroundColor; }

        /**
          * Sets the background color.
          *
          * @param color The background Color.
          * @see setBackgroundColor
          * @since 0.1.0
          */
        void setBackgroundColor(const Color& color)
        { mBackgroundColor = color; }

        /**
          * Gets the background color.
          *
          * @see setBackgroundColor
          * @since 0.1.0
          */
        const Color& getBackgroundColor() const A_WARN_UNUSED
        { return mBackgroundColor; }

        /**
          * Requests focus for the widget. A widget will only recieve focus
          * if it is focusable.
          */
        virtual void requestFocus();

        /**
          * Requests a move to the top in the parent widget.
          */
        virtual void requestMoveToTop();

        /**
          * Requests a move to the bottom in the parent widget.
          */
        virtual void requestMoveToBottom();

        /**
          * Sets the focus handler to be used.
          *
          * WARNING: This function is used internally and should not
          *          be called or overloaded unless you know what you
          *          are doing.
          *
          * @param focusHandler The focus handler to use.
          * @see getFocusHandler
          * @since 0.1.0
          */
        virtual void setFocusHandler(FocusHandler *const focusHandler);

        /**
          * Gets the focus handler used.
          *
          * WARNING: This function is used internally and should not
          *          be called or overloaded unless you know what you
          *          are doing.
          *
          * @return The focus handler used.
          * @see setFocusHandler
          * @since 0.1.0
          */
        virtual FocusHandler* getFocusHandler() A_WARN_UNUSED
        { return mFocusHandler; }

        /**
          * Adds an action listener to the widget. When an action event 
          * is fired by the widget the action listeners of the widget 
          * will get notified.
          *
          * @param actionListener The action listener to add.
          * @see removeActionListener
          * @since 0.1.0
          */
        void addActionListener(ActionListener *const actionListener);

        /**
          * Removes an added action listener from the widget.
          *
          * @param actionListener The action listener to remove.
          * @see addActionListener
          * @since 0.1.0
          */
        void removeActionListener(ActionListener *const actionListener);

        /**
          * Adds a death listener to the widget. When a death event is 
          * fired by the widget the death listeners of the widget will 
          * get notified.
          *
          * @param deathListener The death listener to add.
          * @see removeDeathListener
          * @since 0.1.0
          */
        void addDeathListener(DeathListener *const deathListener);

        /**
          * Removes an added death listener from the widget.
          *
          * @param deathListener The death listener to remove.
          * @see addDeathListener
          * @since 0.1.0
          */
        void removeDeathListener(DeathListener *const deathListener);

        /**
          * Adds a mouse listener to the widget. When a mouse event is 
          * fired by the widget the mouse listeners of the widget will 
          * get notified.
          *
          * @param mouseListener The mouse listener to add.
          * @see removeMouseListener
          * @since 0.1.0
          */
        void addMouseListener(MouseListener *const mouseListener);

        /**
          * Removes an added mouse listener from the widget.
          *
          * @param mouseListener The mouse listener to remove.
          * @see addMouseListener
          * @since 0.1.0
          */
        void removeMouseListener(MouseListener *const mouseListener);

        /**
          * Adds a key listener to the widget. When a key event is 
          * fired by the widget the key listeners of the widget will 
          * get notified.
          *
          * @param keyListener The key listener to add.
          * @see removeKeyListener
          * @since 0.1.0
          */
        void addKeyListener(KeyListener *const keyListener);

        /**
          * Removes an added key listener from the widget.
          *
          * @param keyListener The key listener to remove.
          * @see addKeyListener
          * @since 0.1.0
          */
        void removeKeyListener(KeyListener *const keyListener);

        /**
          * Adds a focus listener to the widget. When a focus event is 
          * fired by the widget the key listeners of the widget will 
          * get notified.
          *
          * @param focusListener The focus listener to add.
          * @see removeFocusListener
          * @since 0.7.0
          */
        void addFocusListener(FocusListener *const focusListener);

        /**
          * Removes an added focus listener from the widget.
          *
          * @param focusListener The focus listener to remove.
          * @see addFocusListener
          * @since 0.7.0
          */
        void removeFocusListener(FocusListener *const focusListener);

        /**
          * Adds a widget listener to the widget. When a widget event is 
          * fired by the widget the key listeners of the widget will 
          * get notified.
          *
          * @param widgetListener The widget listener to add.
          * @see removeWidgetListener
          * @since 0.8.0
          */
        void addWidgetListener(WidgetListener *const widgetListener);

        /**
          * Removes an added widget listener from the widget.
          *
          * @param widgetListener The widget listener to remove.
          * @see addWidgetListener
          * @since 0.8.0
          */
        void removeWidgetListener(WidgetListener *const widgetListener);

        /**
          * Sets the action event identifier of the widget. The identifier is
          * used to be able to identify which action has occured.
          *
          * NOTE: An action event identifier should not be used to identify a
          *       certain widget but rather a certain event in your application.
          *       Several widgets can have the same action event identifer.
          *
          * @param actionEventId The action event identifier.
          * @see getActionEventId
          * @since 0.6.0
          */
        void setActionEventId(const std::string& actionEventId)
        { mActionEventId = actionEventId; }

        /**
          * Gets the action event identifier of the widget.
          *
          * @return The action event identifier of the widget.
          * @see setActionEventId
          * @since 0.6.0
          */
        const std::string& getActionEventId() const
        { return mActionEventId; }

        /**
          * Gets the absolute position on the screen for the widget.
          *
          * @param x The absolute x coordinate will be stored in this parameter.
          * @param y The absolute y coordinate will be stored in this parameter.
          * @since 0.1.0
          */
        virtual void getAbsolutePosition(int& x, int& y) const;

        /**
          * Sets the parent of the widget. A parent must be a BasicContainer.
          *
          * WARNING: This function is used internally and should not
          *          be called or overloaded unless you know what you
          *          are doing.
          *
          * @param parent The parent of the widget.
          * @see getParent
          * @since 0.1.0
          */
        virtual void setParent(Widget* parent)
        { mParent = parent; }

        /**
          * Gets the font set for the widget. If no font has been set, 
          * the global font will be returned. If no global font has been set, 
          * the default font will be returend.
          *
          * @return The font set for the widget.
          * @see setFont, setGlobalFont
          * @since 0.1.0
          */
        Font *getFont() const A_WARN_UNUSED;

        /**
          * Sets the global font to be used by default for all widgets.
          *
          * @param font The global font.
          * @see getGlobalFont
          * @since 0.1.0
          */
        static void setGlobalFont(Font *const font);

        /**
          * Sets the font for the widget. If NULL is passed, the global font 
          * will be used.
          *
          * @param font The font to set for the widget.
          * @see getFont
          * @since 0.1.0
          */
        void setFont(Font *const font);

        /**
          * Called when the font has changed. If the change is global,
          * this function will only be called if the widget doesn't have a
          * font already set.
          *
          * @since 0.1.0
          */
        virtual void fontChanged()
        { }

        /**
          * Checks if a widget exists or not, that is if it still exists
          * an instance of the object.
          *
          * @param widget The widget to check.
          * @return True if an instance of the widget exists, false otherwise.
          * @since 0.1.0
          */
        static bool widgetExists(const Widget* widget) A_WARN_UNUSED;

        /**
          * Checks if tab in is enabled. Tab in means that you can set focus
          * to this widget by pressing the tab button. If tab in is disabled
          * then the focus handler will skip this widget and focus the next
          * in its focus order.
          *
          * @return True if tab in is enabled, false otherwise.
          * @see setTabInEnabled
          * @since 0.1.0
          */
        bool isTabInEnabled() const A_WARN_UNUSED
        { return mTabIn; }

        /**
          * Sets tab in enabled, or not. Tab in means that you can set focus
          * to this widget by pressing the tab button. If tab in is disabled
          * then the FocusHandler will skip this widget and focus the next
          * in its focus order.
          *
          * @param enabled True if tab in should be enabled, false otherwise.
          * @see isTabInEnabled
          * @since 0.1.0
          */
        void setTabInEnabled(const bool enabled)
        { mTabIn = enabled; }

        /**
          * Checks if tab out is enabled. Tab out means that you can lose
          * focus to this widget by pressing the tab button. If tab out is
          * disabled then the FocusHandler ignores tabbing and focus will
          * stay with this widget.
          *
          * @return True if tab out is enabled, false otherwise.
          * @see setTabOutEnabled
          * @since 0.1.0
          */
        bool isTabOutEnabled() const A_WARN_UNUSED
        { return mTabOut; }

        /**
          * Sets tab out enabled. Tab out means that you can lose
          * focus to this widget by pressing the tab button. If tab out is
          * disabled then the FocusHandler ignores tabbing and focus will
          * stay with this widget.
          *
          * @param enabled True if tab out should be enabled, false otherwise.
          * @see isTabOutEnabled
          * @since 0.1.0
          */
        void setTabOutEnabled(const bool enabled)
        { mTabOut = enabled; }

        /**
          * Requests modal focus. When a widget has modal focus, only that
          * widget and it's children may recieve input.
          *
          * @throws Exception if another widget already has modal focus.
          * @see releaseModalFocus, isModalFocused
          * @since 0.4.0
          */
        virtual void requestModalFocus();

        /**
          * Requests modal mouse input focus. When a widget has modal input focus
          * that widget will be the only widget receiving input even if the input
          * occurs outside of the widget and no matter what the input is.
          *
          * @throws Exception if another widget already has modal focus.
          * @see releaseModalMouseInputFocus, isModalMouseInputFocused
          * @since 0.6.0
          */
        virtual void requestModalMouseInputFocus();

        /**
          * Releases modal focus. Modal focus will only be released if the
          * widget has modal focus.
          *
          * @see requestModalFocus, isModalFocused
          * @since 0.4.0
          */
        virtual void releaseModalFocus();

        /**
          * Releases modal mouse input focus. Modal mouse input focus will only
          * be released if the widget has modal mouse input focus.
          *
          * @see requestModalMouseInputFocus, isModalMouseInputFocused
          * @since 0.6.0
          */
        virtual void releaseModalMouseInputFocus();

        /**
          * Checks if the widget or it's parent has modal focus.
          *
          * @return True if the widget has modal focus, false otherwise.
          * @see requestModalFocus, releaseModalFocus
          * @since 0.8.0
          */
        virtual bool isModalFocused() const A_WARN_UNUSED;

        /**
          * Checks if the widget or it's parent has modal mouse input focus.
          *
          * @return True if the widget has modal mouse input focus, false
          *         otherwise.
          * @see requestModalMouseInputFocus, releaseModalMouseInputFocus
          * @since 0.8.0
          */
        virtual bool isModalMouseInputFocused() const A_WARN_UNUSED;

        /**
          * Gets a widget from a certain position in the widget.
          * This function is used to decide which gets mouse input,
          * thus it can be overloaded to change that behaviour.
          *
          * NOTE: This always returns NULL if the widget is not
          *       a container.
          *
          * @param x The x coordinate of the widget to get.
          * @param y The y coordinate of the widget to get.
          * @return The widget at the specified coodinate, NULL
          *         if no widget is found.
          * @since 0.6.0
          */
        virtual Widget *getWidgetAt(int x A_UNUSED,
                                    int y A_UNUSED) A_WARN_UNUSED
        { return nullptr; }

        /**
          * Gets the mouse listeners of the widget.
          *
          * @return The mouse listeners of the widget.
          * @since 0.6.0
          */
        virtual const std::list<MouseListener*>& getMouseListeners()
                                                 A_WARN_UNUSED;

        /**
          * Gets the key listeners of the widget.
          *
          * @return The key listeners of the widget.
          * @since 0.6.0
          */
        virtual const std::list<KeyListener*>& getKeyListeners()
                                               A_WARN_UNUSED;

        /**
          * Gets the focus listeners of the widget.
          *
          * @return The focus listeners of the widget.
          * @since 0.7.0
          */
        virtual const std::list<FocusListener*>& getFocusListeners()
                                                 A_WARN_UNUSED;

        /**
          * Gets the area of the widget occupied by the widget's children.
          * By default this method returns an empty rectangle as not all
          * widgets are containers. If you want to make a container this
          * method should return the area where the children resides. This
          * method is used when drawing children of a widget when computing
          * clip rectangles for the children.
          *
          * An example of a widget that overloads this method is ScrollArea.
          * A ScrollArea has a view of its contant and that view is the
          * children area. The size of a ScrollArea's children area might
          * vary depending on if the scroll bars of the ScrollArea is shown
          * or not.
          *
          * @return The area of the widget occupied by the widget's children.
          * @see BasicContainer
          * @see BasicContainer::getChildrenArea
          * @see BasicContainer::drawChildren
          * @since 0.1.0
          */
        virtual Rect getChildrenArea() A_WARN_UNUSED;

        /**
          * Gets the internal focus handler used.
          *
          * @return the internalFocusHandler used. If no internal focus handler
          *         is used, NULL will be returned.
          * @see setInternalFocusHandler
          * @since 0.1.0
          */
        virtual FocusHandler* getInternalFocusHandler() A_WARN_UNUSED;

        /**
          * Sets the internal focus handler. An internal focus handler is
          * needed if both a widget in the widget and the widget itself
          * should be foucsed at the same time.
          *
          * @param focusHandler The internal focus handler to be used.
          * @see getInternalFocusHandler
          * @since 0.1.0
          */
        void setInternalFocusHandler(FocusHandler *const internalFocusHandler);

        /**
          * Moves a widget to the top of this widget. The moved widget will be
          * drawn above all other widgets in this widget.
          *
          * @param widget The widget to move to the top.
          * @see moveToBottom
          * @since 0.1.0
          */
        virtual void moveToTop(Widget* widget A_UNUSED)
        { }

        /**
          * Moves a widget in this widget to the bottom of this widget.
          * The moved widget will be drawn below all other widgets in this widget.
          *
          * @param widget The widget to move to the bottom.
          * @see moveToTop
          * @since 0.1.0
          */
        virtual void moveToBottom(Widget* widget A_UNUSED)
        { }

        /**
          * Focuses the next widget in the widget.
          * 
          * @see moveToBottom
          * @since 0.1.0
          */
        virtual void focusNext()
        { }

        /**
          * Focuses the previous widget in the widget.
          *
          * @see moveToBottom
          * @since 0.1.0
          */
        virtual void focusPrevious()
        { }

        /**
          * Tries to show a specific part of a widget by moving it. Used if the
          * widget should act as a container.
          *
          * @param widget The target widget.
          * @param area The area to show.
          * @since 0.1.0
          */
        virtual void showWidgetPart(Widget *const widget A_UNUSED,
                                    Rect area A_UNUSED)
        { }

        /**
          * Sets an id of a widget. An id can be useful if a widget needs to be
          * identified in a container. For example, if widgets are created by an
          * XML document, a certain widget can be retrieved given that the widget
          * has an id.
          *
          * @param id The id to set to the widget.
          * @see getId
          * @since 0.8.0
          */
        void setId(const std::string& id)
        { mId = id; }

        /**
          * Gets the id of a widget. An id can be useful if a widget needs to be
          * identified in a container. For example, if widgets are created by an
          * XML document, a certain widget can be retrieved given that the widget
          * has an id.
          *
          * @param id The id to set to the widget.
          * @see setId
          * @since 0.8.0
          */
        const std::string& getId() const A_WARN_UNUSED
        { return mId; }

        /**
          * Shows a certain part of a widget in the widget's parent.
          * Used when widgets want a specific part to be visible in
          * its parent. An example is a TextArea that wants a specific
          * part of its text to be visible when a TextArea is a child
          * of a ScrollArea.
          *
          * @param rectangle The rectangle to be shown.
          * @since 0.8.0
          */
        virtual void showPart(const Rect &rectangle);

        bool isAllowLogic() const
        { return mAllowLogic; }

    protected:
        /**
          * Distributes an action event to all action listeners
          * of the widget.
          *
          * @since 0.8.0
          */
        void distributeActionEvent();

        /**
          * Distributes resized events to all of the widget's listeners.
          *
          * @since 0.8.0
          */
        void distributeResizedEvent();

        /**
          * Distributes moved events to all of the widget's listeners.
          *
          * @since 0.8.0
          */
        void distributeMovedEvent();

        /**
          * Distributes hidden events to all of the widget's listeners.
          *
          * @since 0.8.0
          * @author Olof Naessén
          */
        void distributeHiddenEvent();

        /**
          * Distributes shown events to all of the widget's listeners.
          *
          * @since 0.8.0
          * @author Olof Naessén
          */
        void distributeShownEvent();

        /**
          * Typdef.
          */
        typedef std::list<MouseListener*> MouseListenerList;

        /**
          * Typdef.
          */
        typedef MouseListenerList::iterator MouseListenerIterator;

        /**
          * Holds the mouse listeners of the widget.
          */
        MouseListenerList mMouseListeners;

        /**
          * Typdef.
          */
        typedef std::list<KeyListener*> KeyListenerList;

        /**
          * Holds the key listeners of the widget.
          */
        KeyListenerList mKeyListeners;

        /**
          * Typdef.
          */
        typedef KeyListenerList::iterator KeyListenerIterator;

        /**
          * Typdef.
          */
        typedef std::list<ActionListener*> ActionListenerList;

        /** 
          * Holds the action listeners of the widget.
          */
        ActionListenerList mActionListeners;

        /**
          * Typdef.
          */
        typedef ActionListenerList::iterator ActionListenerIterator;

        /**
          * Typdef.
          */
        typedef std::list<DeathListener*> DeathListenerList;

        /**
          * Holds the death listeners of the widget.
          */ 
        DeathListenerList mDeathListeners;

        /**
          * Typdef.
          */
        typedef DeathListenerList::iterator DeathListenerIterator;

        /**
          * Typdef.
          */
        typedef std::list<FocusListener*> FocusListenerList;

        /**
          * Holds the focus listeners of the widget.
          */
        FocusListenerList mFocusListeners;

        /**
          * Typdef.
          */
        typedef FocusListenerList::iterator FocusListenerIterator;

        typedef std::list<WidgetListener*> WidgetListenerList;

        /**
          * Holds the widget listeners of the widget.
          */
        WidgetListenerList mWidgetListeners;

        /**
          * Typdef.
          */
        typedef WidgetListenerList::iterator WidgetListenerIterator;

        /**
          * Holds the foreground color of the widget.
          */
        Color mForegroundColor;

        /**
          * Holds the background color of the widget.
          */
        Color mBackgroundColor;

        /**
          * Holds the base color of the widget.
          */
        Color mBaseColor;

        /**
          * Holds the dimension of the widget.
          */
        Rect mDimension;

        /**
          * Holds the action event of the widget.
          */
        std::string mActionEventId;

        /**
          * Holds the id of the widget.
          */
        std::string mId;

        /**
          * Holds the focus handler used by the widget.
          */
        FocusHandler* mFocusHandler;

        /**
          * Holds the focus handler used by the widget. NULL
          * if no internal focus handler is used.
          */
        FocusHandler* mInternalFocusHandler;

        /**
          * Holds the parent of the widget. NULL if the widget
          * has no parent.
          */
        Widget* mParent;

        /**
          * Holds the font used by the widget.
          */
        Font* mCurrentFont;

        /** 
          * Holds the frame size of the widget.
          */
        unsigned int mFrameSize;

        /**
          * True if the widget focusable, false otherwise.
          */
        bool mFocusable;

        /**
          * True if the widget visible, false otherwise.
          */
        bool mVisible;

        /**
          * True if the widget has tab in enabled, false otherwise.
          */
        bool mTabIn;

        /**
          * True if the widget has tab in enabled, false otherwise.
          */
        bool mTabOut;

        /**
          * True if the widget is enabled, false otherwise.
          */
        bool mEnabled;

        bool mAllowLogic;

        /**
          * Holds the global font used by the widget.
          */
        static Font* mGlobalFont;

        /**
          * Holds a list of all instances of widgets.
          */
        static std::list<Widget*> mWidgets;

        static std::set<Widget*> mWidgetsSet;
};

#endif  // GUI_WIDGETS_WIDGET_H
