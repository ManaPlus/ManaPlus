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

#ifndef GUI_FOCUSHANDLER_H
#define GUI_FOCUSHANDLER_H

#include <guichan/focushandler.hpp>

#include <list>

#include "localconsts.h"

/**
 * The focus handler. This focus handler does exactly the same as the Guichan
 * focus handler, but keeps a stack of modal widgets to be able to handle
 * multiple modal focus requests.
 */
class FocusHandler final : public gcn::FocusHandler
{
    public:
        FocusHandler() :
            mModalStack()
        { }

        A_DELETE_COPY(FocusHandler)

        /**
         * Sets modal focus to a widget. When there is already a modal widget
         * then that widget loses modal focus and will regain it after this
         * widget releases his modal focus.
         */
        void requestModalFocus(gcn::Widget *widget) override final;

        /**
         * Releases modal focus of a widget. When this widget had modal focus
         * and there are other widgets that had also requested modal focus,
         * then modal focus will be transfered to the last of those.
         */
        void releaseModalFocus(gcn::Widget *widget) override final;

        /**
         * Removes a widget from the focus handler. Also makes sure no dangling
         * pointers remain in modal focus stack.
         */
        void remove(gcn::Widget *widget) override final;

        /**
         * Overloaded to allow windows to move to the top when one of their
         * widgets is tabbed to when tabbing through focusable elements.
         */
        void tabNext() override final;

        void tabPrevious() override final;

        void distributeFocusGainedEvent(const gcn::Event &focusEvent)
                                        override final;

    private:
        /**
         * Checks to see if the widget tabbed to is in a window, and if it is,
         * it requests the window be moved to the top.
         */
        void checkForWindow() const;

        /**
         * Stack of widgets that have requested modal forcus.
         */
        std::list<gcn::Widget*> mModalStack;
};

#endif  // GUI_FOCUSHANDLER_H
