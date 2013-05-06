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

#include "gui/buyselldialog.h"

#include "gui/setup.h"

#include "net/buysellhandler.h"
#include "net/net.h"
#include "net/npchandler.h"

#include "utils/gettext.h"

#include "debug.h"

BuySellDialog::DialogList BuySellDialog::dialogInstances;

BuySellDialog::BuySellDialog(const int npcId) :
    // TRANSLATORS: shop window name
    Window(_("Shop"), false, nullptr, "buysell.xml"),
    gcn::ActionListener(),
    mNpcId(npcId),
    mNick(""),
    mBuyButton(nullptr)
{
    init();
}

BuySellDialog::BuySellDialog(const std::string &nick) :
    // TRANSLATORS: shop window name
    Window(_("Shop"), false, nullptr, "buysell.xml"),
    gcn::ActionListener(),
    mNpcId(-1),
    mNick(nick),
    mBuyButton(nullptr)
{
    init();
}

void BuySellDialog::init()
{
    setWindowName("BuySell");
    setCloseButton(true);

    static const char *buttonNames[] =
    {
        // TRANSLATORS: shop window button
        N_("Buy"),
        // TRANSLATORS: shop window button
        N_("Sell"),
        // TRANSLATORS: shop window button
        N_("Cancel"),
        nullptr
    };
    const int buttonPadding = getOption("buttonpadding", 10);
    int x = buttonPadding;
    const int y = buttonPadding;

    for (const char **curBtn = buttonNames; *curBtn; curBtn++)
    {
        Button *const btn = new Button(this, gettext(*curBtn), *curBtn, this);
        if (!mBuyButton)
            mBuyButton = btn;  // For focus request
        btn->setPosition(x, y);
        add(btn);
        x += btn->getWidth() + buttonPadding;
    }
    if (mBuyButton)
    {
        mBuyButton->requestFocus();
        setContentSize(x, 2 * y + mBuyButton->getHeight());
    }

    center();
    setDefaultSize();
    loadWindowState();
    enableVisibleSound(true);

    dialogInstances.push_back(this);
    setVisible(true);
}

BuySellDialog::~BuySellDialog()
{
    dialogInstances.remove(this);
}

void BuySellDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible)
    {
        if (mBuyButton)
            mBuyButton->requestFocus();
    }
    else
    {
        scheduleDelete();
    }
}

void BuySellDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "Buy")
    {
        if (mNpcId != -1)
            Net::getNpcHandler()->buy(mNpcId);
        else
            Net::getBuySellHandler()->requestSellList(mNick);
    }
    else if (eventId == "Sell")
    {
        if (mNpcId != -1)
            Net::getNpcHandler()->sell(mNpcId);
        else
            Net::getBuySellHandler()->requestBuyList(mNick);
    }

    close();
}

void BuySellDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, dialogInstances)
    {
        if (*it)
            (*it)->close();
    }
}
