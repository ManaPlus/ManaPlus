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

#include "gui/windowmenu.h"

#include "emoteshortcut.h"
#include "graphics.h"
#include "keyboardconfig.h"

#include "gui/didyouknowwindow.h"
#include "gui/emotepopup.h"
#include "gui/skilldialog.h"
#include "gui/specialswindow.h"
#include "gui/textpopup.h"
#include "gui/viewport.h"

#include "gui/widgets/window.h"
#include "gui/widgets/windowcontainer.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/gettext.h"

#include <string>

#include "debug.h"

extern Window *equipmentWindow;
extern Window *inventoryWindow;
extern Window *itemShortcutWindow;
extern Window *dropShortcutWindow;
extern Window *setupWindow;
extern Window *statusWindow;
extern Window *whoIsOnline;
extern Window *killStats;
extern Window *spellShortcutWindow;
extern Window *botCheckerWindow;
extern Window *socialWindow;

WindowMenu::WindowMenu():
    mEmotePopup(nullptr),
    mHaveMouse(false),
    mAutoHide(1)
{
    int x = 0, h = 0;

    addButton(N_("BC"), _("Bot checker"), x, h,
              Input::KEY_WINDOW_BOT_CHECKER);
    addButton(N_("ONL"), _("Who is online"), x, h,
              Input::KEY_NO_VALUE);
    addButton(N_("KS"), _("Kill stats"), x, h,
              Input::KEY_WINDOW_KILLS);
    addButton(":-)", _("Smilies"), x, h,
              Input::KEY_WINDOW_EMOTE_SHORTCUT);
    addButton(N_("STA"), _("Status"), x, h, Input::KEY_WINDOW_STATUS);
    addButton(N_("EQU"), _("Equipment"), x, h,
              Input::KEY_WINDOW_EQUIPMENT);
    addButton(N_("INV"), _("Inventory"), x, h,
              Input::KEY_WINDOW_INVENTORY);

    if (skillDialog->hasSkills())
    {
        addButton(N_("SKI"), _("Skills"), x, h,
                  Input::KEY_WINDOW_SKILL);
    }

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        addButton(N_("SPE"), _("Specials"), x, h,
                  Input::KEY_NO_VALUE);
    }
#endif

    addButton(N_("SOC"), _("Social"), x, h, Input::KEY_WINDOW_SOCIAL);
    addButton(N_("SH"), _("Shortcuts"), x, h,
              Input::KEY_WINDOW_SHORTCUT);
    addButton(N_("SP"), _("Spells"), x, h, Input::KEY_WINDOW_SPELLS);
    addButton(N_("DR"), _("Drop"), x, h, Input::KEY_WINDOW_DROP);
    addButton(N_("YK"), _("Did you know"), x, h,
        Input::KEY_WINDOW_DIDYOUKNOW);
    addButton(N_("SET"), _("Setup"), x, h, Input::KEY_WINDOW_SETUP);

    if (mainGraphics)
    {
        setDimension(gcn::Rectangle(mainGraphics->mWidth - x - 3,
            3, x - 3, h));
    }

    loadButtons();
    mTextPopup = new TextPopup;

    addMouseListener(this);
    setVisible(true);

    config.addListener("autohideButtons", this);
    mAutoHide = config.getIntValue("autohideButtons");
}

WindowMenu::~WindowMenu()
{
    config.removeListener("autohideButtons", this);

    delete mTextPopup;
    mTextPopup = nullptr;
    mButtonNames.clear();
    std::vector <gcn::Button*>::iterator it, it_end;
    for (it = mButtons.begin(), it_end = mButtons.end(); it != it_end; ++it)
    {
        Button *btn = dynamic_cast<Button*>(*it);
        if (!btn)
            continue;
        if (!btn->isVisible())
        {
            delete btn;
        }
    }
}

void WindowMenu::action(const gcn::ActionEvent &event)
{
    Window *window = nullptr;

    if (event.getId() == ":-)")
    {
        if (!mEmotePopup)
        {
            const gcn::Widget *s = event.getSource();
            if (s)
            {
                const gcn::Rectangle &r = s->getDimension();
                const int parentX = s->getParent()->getX();

                mEmotePopup = new EmotePopup;
                const int offset = (r.width - mEmotePopup->getWidth()) / 2;
                mEmotePopup->setPosition(parentX + r.x + offset,
                                     r.y + r.height + 5);

                mEmotePopup->addSelectionListener(this);
            }
            else
            {
                mEmotePopup = nullptr;
            }
        }
        else
        {
            if (windowContainer)
                windowContainer->scheduleDelete(mEmotePopup);
            mEmotePopup = nullptr;
        }
    }
    else if (event.getId() == "STA")
    {
        window = statusWindow;
    }
    else if (event.getId() == "EQU")
    {
        window = equipmentWindow;
    }
    else if (event.getId() == "INV")
    {
        window = inventoryWindow;
    }
    else if (event.getId() == "SKI")
    {
        window = skillDialog;
    }
    else if (event.getId() == "SPE")
    {
        window = specialsWindow;
    }
    else if (event.getId() == "SH")
    {
        window = itemShortcutWindow;
    }
    else if (event.getId() == "SOC")
    {
        window = socialWindow;
    }
    else if (event.getId() == "DR")
    {
        window = dropShortcutWindow;
    }
    else if (event.getId() == "SET")
    {
        window = setupWindow;
    }
    else if (event.getId() == "ONL")
    {
        window = whoIsOnline;
    }
    else if (event.getId() == "KS")
    {
        window = killStats;
    }
    else if (event.getId() == "BC")
    {
        window = botCheckerWindow;
    }
    else if (event.getId() == "SP")
    {
        window = spellShortcutWindow;
    }
    else if (event.getId() == "YK")
    {
        window = didYouKnowWindow;
    }

    if (window)
    {
        window->setVisible(!window->isVisible());
        if (window->isVisible())
            window->requestMoveToTop();
    }
}

void WindowMenu::valueChanged(const gcn::SelectionEvent &event)
{
    if (event.getSource() == mEmotePopup)
    {
        int emote = mEmotePopup->getSelectedEmote();
        if (emote && emoteShortcut)
            emoteShortcut->useEmote(emote);

        if (windowContainer)
            windowContainer->scheduleDelete(mEmotePopup);
        mEmotePopup = nullptr;
    }
}

void WindowMenu::addButton(const char* text, std::string description,
                           int &x, int &h, int key)
{
    Button *btn = new Button(gettext(text), text, this);
    btn->setPosition(x, 0);
    btn->setDescription(description);
    btn->setTag(key);
    add(btn);
    mButtons.push_back(btn);
    x += btn->getWidth() + 3;
    h = btn->getHeight();
    mButtonNames[text] = btn;
}

void WindowMenu::mousePressed(gcn::MouseEvent &event)
{
    if (!viewport)
        return;

    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Button *btn = dynamic_cast<Button*>(event.getSource());
        if (!btn)
            return;
        if (viewport)
        {
            viewport->showPopup(getX() + event.getX(),
                getY() + event.getY(), btn);
        }
    }
}

void WindowMenu::mouseMoved(gcn::MouseEvent &event)
{
    mHaveMouse = true;

    if (!mTextPopup)
        return;

    if (event.getSource() == this)
    {
        mTextPopup->hide();
        return;
    }

    Button *btn = dynamic_cast<Button*>(event.getSource());

    if (!btn)
    {
        mTextPopup->hide();
        return;
    }

    const int x = event.getX();
    const int y = event.getY();
    int key = btn->getTag();
    if (key != Input::KEY_NO_VALUE)
    {
        mTextPopup->show(x + getX(), y + getY(), btn->getDescription(),
            strprintf(_("Key: %s"), keyboard.getKeyValueString(key).c_str()));
    }
    else
    {
        mTextPopup->show(x + getX(), y + getY(), btn->getDescription());
    }
}

void WindowMenu::mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED)
{
    mHaveMouse = false;
    if (!mTextPopup)
        return;

    mTextPopup->hide();
}

void WindowMenu::showButton(std::string name, bool visible)
{
    Button *btn = dynamic_cast<Button*>(mButtonNames[name]);
    if (!btn)
        return;

    btn->setVisible(visible);
    updateButtons();
    saveButtons();
}

void WindowMenu::updateButtons()
{
    int x = 0, h = 0;
    std::vector <gcn::Button*>::const_iterator it, it_end;
    for (it = mButtons.begin(), it_end = mButtons.end(); it != it_end; ++it)
        safeRemove(*it);
    for (it = mButtons.begin(), it_end = mButtons.end(); it != it_end; ++it)
    {
        Button *btn = dynamic_cast<Button*>(*it);
        if (!btn)
            continue;
        if (btn->isVisible())
        {
            btn->setPosition(x, 0);
            add(btn);
            x += btn->getWidth() + 3;
            h = btn->getHeight();
        }
    }
    if (mainGraphics)
    {
        setDimension(gcn::Rectangle(mainGraphics->mWidth - x - 3,
            3, x - 3, h));
    }
}

void WindowMenu::loadButtons()
{
    if (config.getValue("windowmenu0", "") == "")
        return;

    for (int f = 0; f < 15; f ++)
    {
        std::string str = config.getValue("windowmenu" + toString(f), "");
        if (str == "" || str == "SET")
            continue;
        Button *btn = dynamic_cast<Button*>(mButtonNames[str]);
        if (!btn)
            continue;
        btn->setVisible(false);
    }
    updateButtons();
}

void WindowMenu::saveButtons()
{
    std::vector <gcn::Button*>::iterator it, it_end;
    int i = 0;
    for (it = mButtons.begin(), it_end = mButtons.end();
         it != it_end; ++it)
    {
        Button *btn = dynamic_cast<Button*>(*it);
        if (!btn->isVisible())
        {
            config.setValue("windowmenu" + toString(i),
                btn->getActionEventId());
            i ++;
        }
    }
    for (int f = i; f < 15; f ++)
        config.deleteKey("windowmenu" + toString(f));
}

void WindowMenu::drawChildren(gcn::Graphics* graphics)
{
    if (mHaveMouse || !mAutoHide || (mAutoHide == 1
        && mainGraphics && mainGraphics->mWidth > 800))
    {
        Container::drawChildren(graphics);
    }
}

void WindowMenu::optionChanged(const std::string &name)
{
    if (name == "autohideButtons")
        mAutoHide = config.getIntValue("autohideButtons");
}
