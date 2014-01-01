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

#include "gui/windows/didyouknowwindow.h"

#include "configuration.h"

#include "gui/sdlfont.h"

#include "gui/windows/setup.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "utils/gettext.h"
#include "utils/process.h"

#include "utils/translation/podict.h"
#include "utils/translation/translationmanager.h"

#include "debug.h"

static const int minTip = 1;
static const int maxTip = 18;

DidYouKnowWindow::DidYouKnowWindow() :
    // TRANSLATORS: did you know window name
    Window(_("Did You Know?"), false, nullptr, "didyouknow.xml"),
    gcn::ActionListener(),
    mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(mBrowserBox,
        true, "didyouknow_background.xml")),
    // TRANSLATORS: did you know window button
    mButtonPrev(new Button(this, _("< Previous"), "prev", this)),
    // TRANSLATORS: did you know window button
    mButtonNext(new Button(this, _("Next >"), "next", this)),
    // TRANSLATORS: did you know window checkbox
    mOpenAgainCheckBox(new CheckBox(this, _("Auto open this window"),
        config.getBoolValue("showDidYouKnow"), this, "openagain"))
{
    setMinWidth(300);
    setMinHeight(220);
    setContentSize(455, 350);
    setWindowName("DidYouKnow");
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);
    setDefaultSize(500, 400, ImageRect::CENTER);

    mBrowserBox->setOpaque(false);
    // TRANSLATORS: did you know window button
    Button *const okButton = new Button(this, _("Close"), "close", this);

    mBrowserBox->setLinkHandler(this);
    mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVersion(true);
    mBrowserBox->setEnableImages(true);
    mBrowserBox->setEnableKeys(true);
    mBrowserBox->setEnableTabs(true);

    place(0, 0, mScrollArea, 5, 3).setPadding(3);
    place(0, 3, mOpenAgainCheckBox, 5);
    place(1, 4, mButtonPrev, 1);
    place(2, 4, mButtonNext, 1);
    place(4, 4, okButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
    enableVisibleSound(true);
}

void DidYouKnowWindow::postInit()
{
    widgetResized(gcn::Event(nullptr));
}

void DidYouKnowWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        setVisible(false);
    }
    else
    {
        const unsigned num = config.getIntValue("currentTip");
        if (eventId == "prev")
        {
            loadData(num - 1);
        }
        else if (eventId == "next")
        {
            loadData(num + 1);
        }
        else if (eventId == "openagain")
        {
            config.setValue("showDidYouKnow",
                mOpenAgainCheckBox->isSelected());
        }
    }
}

void DidYouKnowWindow::handleLink(const std::string &link,
                                  gcn::MouseEvent *event A_UNUSED)
{
    if (strStartWith(link, "http://") || strStartWith(link, "https://"))
        openBrowser(link);
}

void DidYouKnowWindow::loadData(int num)
{
    mBrowserBox->clearRows();
    if (!num)
    {
        const int curTip = config.getIntValue("currentTip");
        if (curTip == 1)
            num = maxTip;
        else
            num = curTip + 1;
    }

    if (num < minTip || num > maxTip)
        num = minTip;

    config.setValue("currentTip", num);

    loadFile(num);

    mScrollArea->setVerticalScrollAmount(0);
}

void DidYouKnowWindow::loadFile(const int num)
{
    const std::string file = strprintf("tips/%d", num);
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");

    StringVect lines;
    TranslationManager::translateFile(helpPath.append(file).append(".txt"),
        translator, lines);

    for (size_t i = 0, sz = lines.size(); i < sz; ++i)
        mBrowserBox->addRow(lines[i]);
}

void DidYouKnowWindow::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible || isWindowVisible())
        loadData();
}
