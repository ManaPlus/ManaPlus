/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef BUYSELLDIALOG_H
#define BUYSELLDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

class Button;

/**
 * A dialog to choose between buying or selling at a shop.
 *
 * \ingroup Interface
 */
class BuySellDialog final : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor. The action listener passed will receive "sell", "buy"
         * or "cancel" events when the respective buttons are pressed.
         *
         * @see Window::Window
         */
        explicit BuySellDialog(const int npcId);

        explicit BuySellDialog(const std::string &nick);

        A_DELETE_COPY(BuySellDialog)

        virtual ~BuySellDialog();

        void init();

        void setVisible(bool visible);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override;

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !dialogInstances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

    private:
        typedef std::list<BuySellDialog*> DialogList;
        static DialogList dialogInstances;

        int mNpcId;
        std::string mNick;
        Button *mBuyButton;
};

#endif
