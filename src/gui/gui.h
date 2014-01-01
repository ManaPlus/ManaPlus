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

#include <guichan/color.hpp>
#include <guichan/focuslistener.hpp>
#include <guichan/gui.hpp>

#include "localconsts.h"

class Graphics;
class GuiConfigListener;
class ImageSet;
class MouseEvent;
class MouseInput;
class SDLFont;
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

        gcn::FocusHandler *getFocusHandler() const A_WARN_UNUSED
        { return mFocusHandler; }

        /**
         * Return game font.
         */
        SDLFont *getFont() const A_WARN_UNUSED
        { return mGuiFont; }

        /**
         * Return help font.
         */
        SDLFont *getHelpFont() const A_WARN_UNUSED
        { return mHelpFont; }

        /**
         * Return secure font.
         */
        SDLFont *getSecureFont() const A_WARN_UNUSED
        { return mSecureFont; }

        /**
         * Return npc font.
         */
        SDLFont *getNpcFont() const A_WARN_UNUSED
        { return mNpcFont; }

        /**
         * Return the Font used for "Info Particles", i.e. ones showing, what
         * you picked up, etc.
         */
        SDLFont *getInfoParticleFont() const A_WARN_UNUSED
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

        void getAbsolutePosition(gcn::Widget *restrict widget,
                                 int &restrict x, int &restrict y);

        void addGlobalFocusListener(gcn::FocusListener* focusListener);

        void removeGlobalFocusListener(gcn::FocusListener* focusListener);

        void distributeGlobalFocusGainedEvent(const gcn::Event &focusEvent);

        void removeDragged(gcn::Widget *widget);

        int getLastMouseX() const
        { return mLastMouseX; }

        int getLastMouseY() const
        { return mLastMouseY; }

    protected:
        void handleMouseMoved(const gcn::MouseInput &mouseInput);

        void handleMouseReleased(const gcn::MouseInput &mouseInput);

        void handleMousePressed(const gcn::MouseInput &mouseInput);

        void handleMouseInput();

        void distributeMouseEvent(gcn::Widget* source, int type, int button,
                                  int x, int y, bool force = false,
                                  bool toSourceOnly = false);

    private:
        GuiConfigListener *mConfigListener;
        SDLFont *mGuiFont;                  /**< The global GUI font */
        SDLFont *mInfoParticleFont;         /**< Font for Info Particles */
        SDLFont *mHelpFont;                 /**< Font for Help Window */
        SDLFont *mSecureFont;               /**< Font for secure labels */
        SDLFont *mNpcFont;                  /**< Font for npc text */
        ImageSet *mMouseCursors;            /**< Mouse cursor images */
        float mMouseCursorAlpha;
        int mMouseInactivityTimer;
        int mCursorType;
#ifdef ANDROID
        uint16_t mLastMouseRealX;
        uint16_t mLastMouseRealY;
#endif
        typedef std::list<gcn::FocusListener*> FocusListenerList;
        typedef FocusListenerList::iterator FocusListenerIterator;
        FocusListenerList mFocusListeners;
        gcn::Color mForegroundColor;
        gcn::Color mForegroundColor2;
        bool mCustomCursor;                 /**< Show custom cursor */
        bool mDoubleClick;
};

extern Gui *gui;                            /**< The GUI system */
extern SDLInput *guiInput;                  /**< GUI input */

/**
 * Bolded text font
 */
extern SDLFont *boldFont;

#endif  // GUI_GUI_H
