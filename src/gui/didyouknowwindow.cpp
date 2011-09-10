/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/didyouknowwindow.h"

#include "logger.h"

#include "gui/gui.h"
#include "gui/setup.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "resources/resourcemanager.h"
#include "configuration.h"

#include "utils/gettext.h"

#include "debug.h"

static const int minTip = 1;
static const int maxTip = 14;

DidYouKnowWindow::DidYouKnowWindow():
    Window(_("Did You Know?"), false, 0, "didyouknow.xml")
{
    setMinWidth(300);
    setMinHeight(250);
    setContentSize(455, 350);
    setWindowName("DidYouKnow");
    setResizable(true);
    setStickyButtonLock(true);

    setupWindow->registerWindowForReset(this);

    setDefaultSize(500, 400, ImageRect::CENTER);

    mBrowserBox = new BrowserBox;
    mBrowserBox->setOpaque(false);
    mScrollArea = new ScrollArea(mBrowserBox);
    Button *okButton = new Button(_("Close"), "close", this);
    mButtonPrev = new Button(_("< Previous"), "prev", this);
    mButtonNext = new Button(_("Next >"), "next", this);
    mOpenAgainCheckBox = new CheckBox(_("Auto open this window"),
        config.getBoolValue("showDidYouKnow"), this, "openagain");

    mScrollArea->setDimension(gcn::Rectangle(5, 5, 445,
                              335 - okButton->getHeight()));
    okButton->setPosition(450 - okButton->getWidth(),
                          345 - okButton->getHeight());

    mBrowserBox->setLinkHandler(this);
    mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVersion(true);
    mBrowserBox->setEnableImages(true);

    place(0, 0, mScrollArea, 5, 3).setPadding(3);
    place(0, 3, mOpenAgainCheckBox, 5);
    place(1, 4, mButtonPrev, 1);
    place(2, 4, mButtonNext, 1);
    place(4, 4, okButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
}

void DidYouKnowWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
        setVisible(false);

    unsigned num = config.getIntValue("currentTip");

    if (event.getId() == "prev")
        loadData(num - 1);
    else if (event.getId() == "next")
        loadData(num + 1);
    else if (event.getId() == "openagain")
        config.setValue("showDidYouKnow", mOpenAgainCheckBox->isSelected());
}

void DidYouKnowWindow::handleLink(const std::string &link A_UNUSED,
                                  gcn::MouseEvent *event A_UNUSED)
{
}

void DidYouKnowWindow::loadData(int num)
{
    mBrowserBox->clearRows();
    if (!num)
        num = config.getIntValue("currentTip") + 1;

    if (num < minTip || num > maxTip)
        num = minTip;

    config.setValue("currentTip", num);

    loadFile(num);

    mScrollArea->setVerticalScrollAmount(0);
}

void DidYouKnowWindow::loadFile(int num)
{
    const std::string file = strprintf("tips/%d", num);
    const std::vector<std::string> langs = getLang();
    ResourceManager *resman = ResourceManager::getInstance();
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");

    std::vector<std::string> lines;
    if (!langs.empty())
    {
        std::string name = helpPath + langs[0] + "/" + file + ".txt";
        if (resman->exists(name))
            lines = resman->loadTextFile(name);
        if (lines.empty() && langs.size() > 1)
        {
            name = helpPath + langs[1] + "/" + file + ".txt";
            lines = resman->loadTextFile(name);
        }
    }

    if (lines.empty())
        lines = resman->loadTextFile(helpPath + file + ".txt");

    for (unsigned int i = 0; i < lines.size(); ++i)
        mBrowserBox->addRow(lines[i]);
}

void DidYouKnowWindow::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible || isVisible())
        loadData();
}
