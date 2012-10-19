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

#include "configuration.h"
#include "logger.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"
#include "gui/setup.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/langs.h"
#include "utils/process.h"

#include "utils/translation/podict.h"
#include "utils/translation/translationmanager.h"

#include "debug.h"

HelpWindow::HelpWindow():
    Window(_("Help"), false, nullptr, "help.xml"),
    gcn::ActionListener(),
    mBrowserBox(new BrowserBox(this)),
    mScrollArea(new ScrollArea(mBrowserBox, true, "help_background.xml"))
{
    setMinWidth(300);
    setMinHeight(250);
    setContentSize(455, 350);
    setWindowName("Help");
    setResizable(true);
    setStickyButtonLock(true);

    setupWindow->registerWindowForReset(this);

    setDefaultSize(500, 400, ImageRect::CENTER);

    mBrowserBox->setOpaque(false);
    Button *const okButton = new Button(_("Close"), "close", this);

    mBrowserBox->setLinkHandler(this);
    mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVersion(true);
    mBrowserBox->setEnableImages(true);

    place(0, 0, mScrollArea, 5, 3).setPadding(3);
    place(4, 3, okButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
    loadTags();
    enableVisibleSound(true);
}

void HelpWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
        setVisible(false);
}

void HelpWindow::handleLink(const std::string &link,
                            gcn::MouseEvent *event A_UNUSED)
{
    if (!strStartWith(link, "http://"))
    {
        std::string helpFile = link;
        loadHelp(helpFile);
    }
    else
    {
        openBrowser(link);
    }
}

void HelpWindow::loadHelp(const std::string &helpFile)
{
    mBrowserBox->clearRows();

    loadFile("header");
    loadFile(helpFile);
    loadFile("footer");

    mScrollArea->setVerticalScrollAmount(0);
    setVisible(true);
}

void HelpWindow::loadFile(const std::string &file)
{
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");

    StringVect lines;
    TranslationManager::translateFile(helpPath + file + ".txt",
        translator, lines);

    for (size_t i = 0, sz = lines.size(); i < sz; ++i)
        mBrowserBox->addRow(lines[i]);
}

void HelpWindow::loadTags()
{
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");
    StringVect lines;
    ResourceManager::loadTextFile(helpPath + "tags.idx", lines);
    for (StringVectCIter it = lines.begin(), it_end = lines.end();
         it != it_end; ++ it)
    {
        const std::string &str = *it;
        const size_t idx = str.find('|');
        if (idx != std::string::npos)
            mTagFileMap[str.substr(idx + 1)].insert(str.substr(0, idx));
    }
}

void HelpWindow::search(const std::string &text0)
{
    std::string text = text0;
    toLower(text);
    if (mTagFileMap.find(text) == mTagFileMap.end())
    {
        loadHelp("searchnotfound");
    }
    else
    {
        const HelpNames &names = mTagFileMap[text];
        if (names.size() == 1)
        {
            loadHelp(*names.begin());
        }
        else
        {
            if (!translator)
                return;
            mBrowserBox->clearRows();
            loadFile("header");
            loadFile("searchmany");
            for (HelpNamesCIter it = names.begin(), it_end = names.end();
                 it != it_end; ++ it)
            {
                const char *const str = (*it).c_str();
                mBrowserBox->addRow(strprintf(" -> @@%s|%s@@", str,
                    translator->getChar(str)));
            }
            loadFile("footer");
            mScrollArea->setVerticalScrollAmount(0);
            setVisible(true);
        }
    }
}
