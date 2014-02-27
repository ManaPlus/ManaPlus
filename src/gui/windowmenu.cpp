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

#include "gui/windowmenu.h"

#include "configuration.h"

#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/popups/textpopup.h"

#include "gui/windows/skilldialog.h"

#include "gui/widgets/button.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include <string>

#include "debug.h"

WindowMenu::WindowMenu(const Widget2 *const widget) :
    Container(widget),
    ActionListener(),
    SelectionListener(),
    MouseListener(),
    mSkin(Theme::instance() ? Theme::instance()->load("windowmenu.xml", "")
          : nullptr),
    mPadding(mSkin ? mSkin->getPadding() : 1),
    mSpacing(mSkin ? mSkin->getOption("spacing", 3) : 3),
    mTextPopup(new TextPopup),
    mButtons(),
    mButtonTexts(),
    mButtonNames(),
    mHaveMouse(false),
    mAutoHide(1),
    mSmallWindow(mainGraphics->getWidth() < 600)
{
    mTextPopup->postInit();
    int x = mPadding;
    int h = 0;

    setFocusable(false);
    // TRANSLATORS: short button name for who is online window.
    addButton(N_("ONL"),
        _("Who is online"), x, h, Input::KEY_WINDOW_ONLINE);
    // TRANSLATORS: short button name for help window.
    addButton(N_("HLP"),
        _("Help"), x, h, Input::KEY_WINDOW_HELP);
    // TRANSLATORS: short button name for quests window.
    addButton(N_("QE"),
        _("Quests"), x, h, Input::KEY_WINDOW_QUESTS);
    // TRANSLATORS: short button name for bot checker window.
    addButton(N_("BC"),
        _("Bot checker"), x, h, Input::KEY_WINDOW_BOT_CHECKER, false);
    // TRANSLATORS: short button name for kill stats window.
    addButton(N_("KS"),
        _("Kill stats"), x, h, Input::KEY_WINDOW_KILLS);
    addButton(":-)",
        _("Smilies"), x, h, Input::KEY_WINDOW_EMOTE_SHORTCUT);
    // TRANSLATORS: short button name for chat window.
    addButton(N_("CH"),
        _("Chat"), x, h, Input::KEY_WINDOW_CHAT,
#ifdef ANDROID
        true);
#else
        false);
#endif
    // TRANSLATORS: short button name for status window.
    addButton(N_("STA"),
        _("Status"), x, h, Input::KEY_WINDOW_STATUS);
    // TRANSLATORS: short button name for equipment window.
    addButton(N_("EQU"),
        _("Equipment"), x, h, Input::KEY_WINDOW_EQUIPMENT);
    // TRANSLATORS: short button name for inventory window.
    addButton(N_("INV"),
        _("Inventory"), x, h, Input::KEY_WINDOW_INVENTORY);
    // TRANSLATORS: short button name for map window.
    addButton(N_("MAP"),
        _("Map"), x, h, Input::KEY_WINDOW_MINIMAP, false);

    if (skillDialog->hasSkills())
    {
        // TRANSLATORS: short button name for skills window.
        addButton(N_("SKI"),
            _("Skills"), x, h, Input::KEY_WINDOW_SKILL);
    }

    // TRANSLATORS: short button name for social window.
    addButton(N_("SOC"),
    // TRANSLATORS: full button name
        _("Social"), x, h, Input::KEY_WINDOW_SOCIAL);
    // TRANSLATORS: short button name for shortcuts window.
    addButton(N_("SH"),
        // TRANSLATORS: full button name
        _("Shortcuts"), x, h, Input::KEY_WINDOW_SHORTCUT);
    // TRANSLATORS: short button name for spells window.
    addButton(N_("SP"),
        // TRANSLATORS: full button name
        _("Spells"), x, h, Input::KEY_WINDOW_SPELLS);
    // TRANSLATORS: short button name for drops window.
    addButton(N_("DR"),
        // TRANSLATORS: full button name
        _("Drop"), x, h, Input::KEY_WINDOW_DROP, false);
    // TRANSLATORS: short button name for did you know window.
    addButton(N_("YK"),
        // TRANSLATORS: full button name
        _("Did you know"), x, h, Input::KEY_WINDOW_DIDYOUKNOW, false);
    // TRANSLATORS: short button name for shop window.
    addButton(N_("SHP"),
        // TRANSLATORS: full button name
        _("Shop"), x, h, Input::KEY_WINDOW_SHOP, false);
    // TRANSLATORS: short button name for outfits window.
    addButton(N_("OU"),
        // TRANSLATORS: full button name
        _("Outfits"), x, h, Input::KEY_WINDOW_OUTFIT, false);
    // TRANSLATORS: short button name for debug window.
    addButton(N_("DBG"),
        // TRANSLATORS: full button name
        _("Debug"), x, h, Input::KEY_WINDOW_DEBUG,
#ifdef ANDROID
        true);
#else
        false);
#endif
    // TRANSLATORS: short button name for windows list menu.
    addButton(N_("WIN"),
        // TRANSLATORS: full button name
        _("Windows"), x, h, Input::KEY_SHOW_WINDOWS, false);
    // TRANSLATORS: short button name for setup window.
    addButton(N_("SET"),
        // TRANSLATORS: full button name
        _("Setup"), x, h, Input::KEY_WINDOW_SETUP);

    x += mPadding - mSpacing;
    if (mainGraphics)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));

    loadButtons();

    addMouseListener(this);
    setVisible(true);

    config.addListener("autohideButtons", this);
    mAutoHide = config.getIntValue("autohideButtons");
}

WindowMenu::~WindowMenu()
{
    config.removeListener("autohideButtons", this);
    CHECKLISTENERS

    delete mTextPopup;
    mTextPopup = nullptr;
    for (std::map <std::string, ButtonInfo*>::iterator
         it = mButtonNames.begin(),
         it_end = mButtonNames.end(); it != it_end; ++it)
    {
        delete (*it).second;
    }
    mButtonNames.clear();
    for (std::vector <Button*>::iterator it = mButtons.begin(),
         it_end = mButtons.end(); it != it_end; ++it)
    {
        Button *const btn = dynamic_cast<Button*>(*it);
        if (!btn)
            continue;
        if (!btn->isVisible())
        {
            delete btn;
        }
    }
    delete_all(mButtonTexts);
    mButtonTexts.clear();
    if (mSkin)
    {
        Theme *const theme = Theme::instance();
        if (theme)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
}

void WindowMenu::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();

    const std::map <std::string, ButtonInfo*>::iterator
        it = mButtonNames.find(eventId);
    if (it == mButtonNames.end())
        return;

    const ButtonInfo *const info = (*it).second;
    if (!info)
        return;

    inputManager.executeAction(info->key);
}

void WindowMenu::addButton(const char *const text,
                           const std::string &description,
                           int &restrict x, int &restrict h, const int key,
                           const bool visible)
{
    Button *const btn = new Button(this, gettext(text), text, this);
    btn->setPosition(x, mPadding);
    btn->setDescription(description);
    btn->setTag(key);
    add(btn);
    btn->setFocusable(false);
    x += btn->getWidth() + mSpacing;
    h = btn->getHeight() + 2 * mPadding;
    mButtons.push_back(btn);
    mButtonNames[text] = new ButtonInfo(btn, key, visible);
    if (key != Input::KEY_SHOW_WINDOWS)
        mButtonTexts.push_back(new ButtonText(description, key));
}

void WindowMenu::mousePressed(MouseEvent &event)
{
    if (!viewport)
        return;

    if (!mSmallWindow && event.getButton() == MouseEvent::RIGHT)
    {
        Button *const btn = dynamic_cast<Button*>(event.getSource());
        if (!btn)
            return;
        if (viewport)
        {
            viewport->showPopup(getX() + event.getX(),
                getY() + event.getY(), btn);
        }
    }
}

void WindowMenu::mouseMoved(MouseEvent &event)
{
    mHaveMouse = true;

    if (!mTextPopup)
        return;

    if (event.getSource() == this)
    {
        mTextPopup->hide();
        return;
    }

    const Button *const btn = dynamic_cast<const Button *const>(
        event.getSource());

    if (!btn)
    {
        mTextPopup->hide();
        return;
    }

    const int x = event.getX();
    const int y = event.getY();
    const int key = btn->getTag();
    const Rect &rect = mDimension;
    if (key != Input::KEY_NO_VALUE)
    {
        mTextPopup->show(x + rect.x, y + rect.y, btn->getDescription(),
            // TRANSLATORS: short key name
            strprintf(_("Key: %s"), inputManager.getKeyValueString(
            key).c_str()));
    }
    else
    {
        mTextPopup->show(x + rect.x, y + rect.y, btn->getDescription());
    }
}

void WindowMenu::mouseExited(MouseEvent& mouseEvent A_UNUSED)
{
    mHaveMouse = false;
    if (!mTextPopup)
        return;

    mTextPopup->hide();
}

void WindowMenu::showButton(const std::string &name, const bool visible)
{
    const ButtonInfo *const info = dynamic_cast<ButtonInfo *const>(
        mButtonNames[name]);
    if (!info || !info->button)
        return;

    info->button->setVisible(visible);
    updateButtons();
    saveButtons();
}

void WindowMenu::updateButtons()
{
    int x = mPadding;
    int h = 0;
    FOR_EACH (std::vector <Button*>::const_iterator, it, mButtons)
        safeRemove(*it);
    const int pad2 = 2 * mPadding;
    FOR_EACH (std::vector <Button*>::iterator, it, mButtons)
    {
        Button *const btn = dynamic_cast<Button *const>(*it);
        if (!btn)
            continue;
        if (btn->isVisible())
        {
            btn->setPosition(x, mPadding);
            add(btn);
            x += btn->getWidth() + mSpacing;
            h = btn->getHeight() + pad2;
        }
    }
    x += mPadding - mSpacing;
    if (mainGraphics)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));
}

void WindowMenu::loadButtons()
{
    if (!mSmallWindow)
    {
        if (config.getValue("windowmenu0", "") == "")
        {
            for (std::map <std::string, ButtonInfo*>::iterator
                 it = mButtonNames.begin(),
                 it_end = mButtonNames.end(); it != it_end; ++it)
            {
                const ButtonInfo *const info = (*it).second;
                if (!info || !info->button || info->visible)
                    continue;
                info->button->setVisible(false);
            }
            updateButtons();
            return;
        }

        for (int f = 0; f < 30; f ++)
        {
            const std::string str = config.getValue(
                "windowmenu" + toString(f), "");
            if (str.empty() || str == "SET")
                continue;
            const ButtonInfo *const info = dynamic_cast<ButtonInfo *const>(
                mButtonNames[str]);
            if (!info || !info->button)
                continue;
            info->button->setVisible(false);
        }
    }
    else
    {
        for (std::map <std::string, ButtonInfo*>::iterator
             it = mButtonNames.begin(),
             it_end = mButtonNames.end(); it != it_end; ++it)
        {
            const ButtonInfo *const info = (*it).second;
            if (!info || !info->button)
                continue;
            Button *const button = info->button;
            const std::string &str = button->getActionEventId();
            button->setVisible(str == "SET" || str == "WIN");
        }
    }
    updateButtons();
}

void WindowMenu::saveButtons() const
{
    int i = 0;
    for (std::vector <Button*>::const_iterator it = mButtons.begin(),
         it_end = mButtons.end();
         it != it_end; ++it)
    {
        const Button *const btn = dynamic_cast<const Button *const>(*it);
        if (btn && !btn->isVisible())
        {
            config.setValue("windowmenu" + toString(i),
                btn->getActionEventId());
            i ++;
        }
    }
    for (int f = i; f < 30; f ++)
        config.deleteKey("windowmenu" + toString(f));
}

void WindowMenu::drawChildren(Graphics* graphics)
{
    if (mHaveMouse || !mAutoHide || (mAutoHide == 1
        && mainGraphics && (mSmallWindow || mainGraphics->mWidth > 800)))
    {
        Container::drawChildren(graphics);
    }
}

void WindowMenu::optionChanged(const std::string &name)
{
    if (name == "autohideButtons")
        mAutoHide = config.getIntValue("autohideButtons");
}

#ifdef USE_PROFILER
void WindowMenu::logicChildren()
{
    BLOCK_START("WindowMenu::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("WindowMenu::logicChildren")
}
#endif
