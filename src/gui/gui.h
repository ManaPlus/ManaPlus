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

#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "gui/color.h"

#include "gui/base/gui.hpp"

#include "localconsts.h"

class FocusListener;
class Graphics;
class GuiConfigListener;
class ImageSet;
class MouseEvent;
class MouseInput;
class Font;
class SDLInput;
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
class Gui final : public gcn::Gui
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

        bool handleKeyInput2();

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

    protected:
        void handleMouseMoved(const MouseInput &mouseInput);

        void handleMouseReleased(const MouseInput &mouseInput);

        void handleMousePressed(const MouseInput &mouseInput);

        void handleMouseInput();

        void distributeMouseEvent(Widget* source, int type, int button,
                                  int x, int y, bool force = false,
                                  bool toSourceOnly = false);

    private:
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
