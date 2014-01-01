/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_NPCPOSTDIALOG_H
#define GUI_WINDOWS_NPCPOSTDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

class TextBox;
class TextField;

class NpcPostDialog final : public Window,
                            public gcn::ActionListener
{
    public:
        /**
         * Constructor
         */
        explicit NpcPostDialog(const int npcId);

        A_DELETE_COPY(NpcPostDialog)

        ~NpcPostDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override final;

        void setVisible(bool visible) override final;

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !instances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

    private:
        typedef std::list<NpcPostDialog*> DialogList;
        static DialogList instances;

        int mNpcId;

        TextBox *mText;
        TextField *mSender;
};

#endif  // GUI_WINDOWS_NPCPOSTDIALOG_H
