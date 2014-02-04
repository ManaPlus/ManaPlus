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

#include "gui/windows/helpwindow.h"

#include "configuration.h"

#include "gui/sdlfont.h"

#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/paths.h"
#include "utils/process.h"

#include "utils/translation/podict.h"
#include "utils/translation/translationmanager.h"

#include "debug.h"

HelpWindow::HelpWindow() :
    // TRANSLATORS: help window name
    Window(_("Help"), false, nullptr, "help.xml"),
    gcn::ActionListener(),
    // TRANSLATORS: help window. button.
    mDYKButton(new Button(this, _("Did you know..."), "DYK", this)),
    mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(mBrowserBox, true, "help_background.xml")),
    mTagFileMap()
{
    setMinWidth(300);
    setMinHeight(220);
    setContentSize(455, 350);
    setWindowName("Help");
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setDefaultSize(500, 400, ImageRect::CENTER);

    mBrowserBox->setOpaque(false);

    mBrowserBox->setLinkHandler(this);
    mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVersion(true);
    mBrowserBox->setEnableImages(true);
    mBrowserBox->setEnableKeys(true);
    mBrowserBox->setEnableTabs(true);

    place(4, 3, mDYKButton);
    place(0, 0, mScrollArea, 5, 3).setPadding(3);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
    loadTags();
    enableVisibleSound(true);
    widgetResized(gcn::Event(nullptr));
}

void HelpWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "DYK")
    {
        if (didYouKnowWindow)
        {
            didYouKnowWindow->setVisible(!didYouKnowWindow->isWindowVisible());
            if (didYouKnowWindow->isWindowVisible())
                didYouKnowWindow->requestMoveToTop();
        }
    }
}

void HelpWindow::handleLink(const std::string &link,
                            gcn::MouseEvent *event A_UNUSED)
{
    if (!strStartWith(link, "http://") && !strStartWith(link, "https://"))
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
    if (!checkPath(helpFile))
        return;
    mBrowserBox->clearRows();
    loadFile("header");
    loadFile(helpFile);
    loadFile("footer");
    mScrollArea->setVerticalScrollAmount(0);
    setVisible(true);
}

void HelpWindow::loadFile(std::string file)
{
    trim(file);
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");

    StringVect lines;
    TranslationManager::translateFile(helpPath.append(file).append(".txt"),
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
    ResourceManager::loadTextFile(helpPath.append("tags.idx"), lines);
    FOR_EACH (StringVectCIter, it, lines)
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
    trim(text);
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
            FOR_EACH (HelpNamesCIter, it, names)
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
