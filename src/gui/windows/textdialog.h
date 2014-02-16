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

#ifndef GUI_WINDOWS_TEXTDIALOG_H
#define GUI_WINDOWS_TEXTDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class PasswordField;
class TextField;

/**
* An option dialog.
 *
 * \ingroup GUI
 */
class TextDialog final : public Window,
                         public ActionListener
{
public:
    /**
     * Constructor.
     *
     * @see Window::Window
     */
    TextDialog(const std::string &restrict title,
               const std::string &restrict msg,
               Window *const parent = nullptr,
               const bool isPassword = false);

    A_DELETE_COPY(TextDialog)

    ~TextDialog();

    void postInit() override final;

    /**
     * Called when receiving actions from the widgets.
     */
    void action(const ActionEvent &event) override final;

    /**
     * Get the text in the textfield
     */
    const std::string &getText() const A_WARN_UNUSED;

    void setText(const std::string &text);

    static bool isActive() A_WARN_UNUSED
    { return instances; }

    void close() override final;

private:
    static int instances;

    TextField *mTextField;
    PasswordField *mPasswordField;
    Button *mOkButton;
    bool mEnabledKeyboard;
};

#endif  // GUI_WINDOWS_TEXTDIALOG_H
