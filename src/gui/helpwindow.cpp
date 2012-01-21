/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/helpwindow.h"

#include "logger.h"

#include "gui/gui.h"
#include "gui/setup.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "resources/resourcemanager.h"
#include "configuration.h"

#include "utils/gettext.h"

#include "debug.h"

HelpWindow::HelpWindow():
    Window(_("Help"), false, nullptr, "help.xml")
{
    setMinWidth(300);
    setMinHeight(250);
    setContentSize(455, 350);
    setWindowName("Help");
    setResizable(true);
    setStickyButtonLock(true);

    setupWindow->registerWindowForReset(this);

    setDefaultSize(500, 400, ImageRect::CENTER);

    mBrowserBox = new BrowserBox;
    mBrowserBox->setOpaque(false);
    mScrollArea = new ScrollArea(mBrowserBox);
    Button *okButton = new Button(_("Close"), "close", this);

    mScrollArea->setDimension(gcn::Rectangle(5, 5, 445,
                                             335 - okButton->getHeight()));
    okButton->setPosition(450 - okButton->getWidth(),
                          345 - okButton->getHeight());

    mBrowserBox->setLinkHandler(this);
    mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVersion(true);
    mBrowserBox->setEnableImages(true);

    place(0, 0, mScrollArea, 5, 3).setPadding(3);
    place(4, 3, okButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
}

void HelpWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
        setVisible(false);
}

void HelpWindow::handleLink(const std::string &link,
                            gcn::MouseEvent *event A_UNUSED)
{
    std::string helpFile = link;
    loadHelp(helpFile);
}

void HelpWindow::loadHelp(const std::string &helpFile)
{
    mBrowserBox->clearRows();

    loadFile("header");
    loadFile(helpFile);

    mScrollArea->setVerticalScrollAmount(0);
    setVisible(true);
}

void HelpWindow::loadFile(const std::string &file)
{
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
            resman->loadTextFile(name, lines);
        if (lines.empty() && langs.size() > 1)
        {
            name = helpPath + langs[1] + "/" + file + ".txt";
            resman->loadTextFile(name, lines);
        }
    }

    if (lines.empty())
        resman->loadTextFile(helpPath + file + ".txt", lines);

    for (unsigned int i = 0; i < lines.size(); ++i)
        mBrowserBox->addRow(lines[i]);
}
