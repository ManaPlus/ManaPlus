/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/buttoninfo.h"
#include "gui/buttontext.h"
#include "gui/popupmanager.h"
#include "gui/skin.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/skilldialog.h"

#include "gui/widgets/button.h"

#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

WindowMenu *windowMenu = nullptr;

WindowMenu::WindowMenu(const Widget2 *const widget) :
    Container(widget),
    ConfigListener(),
    ActionListener(),
    SelectionListener(),
    MouseListener(),
    mSkin(theme != nullptr ? theme->load("windowmenu.xml", "",
        true, Theme::getThemePath()) : nullptr),
    mPadding(mSkin != nullptr ? mSkin->getPadding() : 1),
    mSpacing(mSkin != nullptr ? mSkin->getOption("spacing", 3) : 3),
    mButtons(),
    mButtonTexts(),
    mButtonNames(),
    mHaveMouse(false),
    mAutoHide(1),
    mSmallWindow(mainGraphics->getWidth() < 600)
{
    int x = mPadding;
    int h = 0;

    setFocusable(false);
    // TRANSLATORS: short button name for who is online window.
    addButton(N_("ONL"),
        // TRANSLATORS: long button name for who is online window.
        _("Who is online"), x, h, InputAction::WINDOW_ONLINE,
        Visible_true);
    // TRANSLATORS: short button name for help window.
    addButton(N_("HLP"),
        // TRANSLATORS: long button name for help window.
        _("Help"), x, h, InputAction::WINDOW_HELP,
        Visible_true);
    // TRANSLATORS: short button name for quests window.
    addButton(N_("QE"),
        // TRANSLATORS: long button name for quests window.
        _("Quests"), x, h, InputAction::WINDOW_QUESTS,
        Visible_true);
    // TRANSLATORS: short button name for kill stats window.
    addButton(N_("KS"),
        // TRANSLATORS: long button name for kill stats window.
        _("Kill stats"), x, h, InputAction::WINDOW_KILLS,
        Visible_true);
    addButton(":-)",
        // TRANSLATORS: long button name for emotes window.
        _("Smilies"), x, h, InputAction::WINDOW_EMOTE_SHORTCUT,
        Visible_true);
    // TRANSLATORS: short button name for chat window.
    addButton(N_("CH"),
        // TRANSLATORS: longt button name for chat window.
        _("Chat"), x, h, InputAction::WINDOW_CHAT,
#ifdef ANDROID
        Visible_true);
#else  // ANDROID
        Visible_false);
#endif  // ANDROID

    // TRANSLATORS: short button name for status window.
    addButton(N_("STA"),
        // TRANSLATORS: long button name for status window.
        _("Status"), x, h, InputAction::WINDOW_STATUS,
        Visible_true);
    // TRANSLATORS: short button name for equipment window.
    addButton(N_("EQU"),
        // TRANSLATORS: long button name for equipment window.
        _("Equipment"), x, h, InputAction::WINDOW_EQUIPMENT,
        Visible_true);
    // TRANSLATORS: short button name for inventory window.
    addButton(N_("INV"),
        // TRANSLATORS: long button name for inventory window.
        _("Inventory"), x, h, InputAction::WINDOW_INVENTORY,
        Visible_true);
    // TRANSLATORS: short button name for cart window.
    addButton(N_("CA"),
        // TRANSLATORS: long button name for cart window.
        _("Cart"), x, h, InputAction::WINDOW_CART,
        Visible_true);
    // TRANSLATORS: short button name for map window.
    addButton(N_("MAP"),
        // TRANSLATORS: long button name for map window.
        _("Map"), x, h, InputAction::WINDOW_MINIMAP,
        Visible_false);

    if (skillDialog->hasSkills())
    {
        // TRANSLATORS: short button name for skills window.
        addButton(N_("SKI"),
            // TRANSLATORS: long button name for skills window.
            _("Skills"), x, h, InputAction::WINDOW_SKILL,
        Visible_true);
    }

    // TRANSLATORS: short button name for social window.
    addButton(N_("SOC"),
        // TRANSLATORS: long button name for social window.
        _("Social"), x, h, InputAction::WINDOW_SOCIAL,
        Visible_true);
    // TRANSLATORS: short button name for shortcuts window.
    addButton(N_("SH"),
        // TRANSLATORS: long button name for shortcuts window.
        _("Shortcuts"), x, h, InputAction::WINDOW_SHORTCUT,
        Visible_true);
    // TRANSLATORS: short button name for spells window.
    addButton(N_("SP"),
        // TRANSLATORS: long button name for spells window.
        _("Spells"), x, h, InputAction::WINDOW_SPELLS,
        Visible_true);
    // TRANSLATORS: short button name for drops window.
    addButton(N_("DR"),
        // TRANSLATORS: long button name for drops window.
        _("Drop"), x, h, InputAction::WINDOW_DROP,
        Visible_false);
    // TRANSLATORS: short button name for did you know window.
    addButton(N_("YK"),
        // TRANSLATORS: long button name for did you know window.
        _("Did you know"), x, h, InputAction::WINDOW_DIDYOUKNOW,
        Visible_false);
    // TRANSLATORS: short button name for shop window.
    addButton(N_("SHP"),
        // TRANSLATORS: long button name for shop window.
        _("Shop"), x, h, InputAction::WINDOW_SHOP,
        Visible_false);
    // TRANSLATORS: short button name for outfits window.
    addButton(N_("OU"),
        // TRANSLATORS: long button name for outfits window.
        _("Outfits"), x, h, InputAction::WINDOW_OUTFIT,
        Visible_false);
    // TRANSLATORS: short button name for updates window.
    addButton(N_("UP"),
        // TRANSLATORS: long button name for updates window.
        _("Updates"), x, h, InputAction::WINDOW_UPDATER,
        Visible_false);
    // TRANSLATORS: short button name for bank window.
    addButton(N_("BA"),
        // TRANSLATORS: long button name for bank window.
        _("Bank"), x, h, InputAction::WINDOW_BANK,
        Visible_true);
    // TRANSLATORS: short button name for mail window.
    addButton(N_("MA"),
        // TRANSLATORS: long button name for mail window.
        _("Mail"), x, h, InputAction::WINDOW_MAIL,
        Visible_true);
    // TRANSLATORS: short button name for server info window.
    addButton(N_("SI"),
        // TRANSLATORS: long button name for server info window.
        _("Server info"), x, h, InputAction::WINDOW_SERVER_INFO,
        Visible_true);
    // TRANSLATORS: short button name for debug window.
    addButton(N_("DBG"),
        // TRANSLATORS: long button name for debug window.
        _("Debug"), x, h, InputAction::WINDOW_DEBUG,
#ifdef ANDROID
        Visible_true);
#else  // ANDROID
        Visible_false);
#endif  // ANDROID

    // TRANSLATORS: short button name for windows list menu.
    addButton(N_("WIN"),
        // TRANSLATORS: long button name for windows list menu.
        _("Windows"), x, h, InputAction::SHOW_WINDOWS,
        Visible_false);
    // TRANSLATORS: short button name for setup window.
    addButton(N_("SET"),
        // TRANSLATORS: long button name for setup window.
        _("Setup"), x, h, InputAction::WINDOW_SETUP,
        Visible_true);

    x += mPadding - mSpacing;
    if (mainGraphics != nullptr)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));

    loadButtons();

    addMouseListener(this);
    setVisible(Visible_true);

    config.addListener("autohideButtons", this);
    mAutoHide = config.getIntValue("autohideButtons");
}

WindowMenu::~WindowMenu()
{
    config.removeListener("autohideButtons", this);
    CHECKLISTENERS

    for (std::map <std::string, ButtonInfo*>::iterator
         it = mButtonNames.begin(),
         it_fend = mButtonNames.end(); it != it_fend; ++it)
    {
        delete (*it).second;
    }
    mButtonNames.clear();
    FOR_EACH (STD_VECTOR <Button*>::iterator, it, mButtons)
    {
        Button *const btn = *it;
        if (btn == nullptr)
            continue;
        if (btn->mVisible == Visible_false)
            delete btn;
    }
    delete_all(mButtonTexts);
    mButtonTexts.clear();
    if (mSkin != nullptr)
    {
        if (theme != nullptr)
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
    if (info == nullptr)
        return;

    inputManager.executeAction(info->key);
}

void WindowMenu::addButton(const char *const text,
                           const std::string &description,
                           int &restrict x,
                           int &restrict h,
                           const InputActionT key,
                           const Visible visible)
{
    Button *const btn = new Button(this, gettext(text), text, this);
    btn->setPosition(x, mPadding);
    btn->setDescription(description);
    btn->setTag(CAST_S32(key));
    add(btn);
    btn->setFocusable(false);
    x += btn->getWidth() + mSpacing;
    h = btn->getHeight() + 2 * mPadding;
    mButtons.push_back(btn);
    mButtonNames[text] = new ButtonInfo(btn, key, visible);
    if (key != InputAction::SHOW_WINDOWS)
        mButtonTexts.push_back(new ButtonText(description, key));
}

void WindowMenu::mousePressed(MouseEvent &event)
{
    if (popupManager == nullptr)
        return;

    if (event.getButton() == MouseButton::RIGHT)
    {
        if (mSmallWindow)
            return;

        event.consume();
        Button *const btn = dynamic_cast<Button*>(event.getSource());
        if (btn == nullptr)
            return;
        if (popupMenu != nullptr)
        {
            popupMenu->showPopup(getX() + event.getX(),
                getY() + event.getY(), btn);
        }
    }
}

void WindowMenu::mouseMoved(MouseEvent &event)
{
    mHaveMouse = true;

    if (textPopup == nullptr)
        return;

    if (event.getSource() == this)
    {
        textPopup->hide();
        return;
    }

    const Button *const btn = dynamic_cast<const Button *>(
        event.getSource());

    if (btn == nullptr)
    {
        textPopup->hide();
        return;
    }

    const int x = event.getX();
    const int y = event.getY();
    const InputActionT key = static_cast<InputActionT>(btn->getTag());
    const Rect &rect = mDimension;
    if (key != InputAction::NO_VALUE)
    {
        textPopup->show(x + rect.x, y + rect.y, btn->getDescription(),
            // TRANSLATORS: short key name
            strprintf(_("Key: %s"), inputManager.getKeyValueString(
            key).c_str()));
    }
    else
    {
        textPopup->show(x + rect.x, y + rect.y, btn->getDescription());
    }
}

void WindowMenu::mouseExited(MouseEvent& event A_UNUSED)
{
    mHaveMouse = false;
    if (textPopup == nullptr)
        return;

    textPopup->hide();
}

void WindowMenu::showButton(const std::string &name,
                            const Visible visible)
{
    const ButtonInfo *const info = mButtonNames[name];
    if ((info == nullptr) || (info->button == nullptr))
        return;

    info->button->setVisible(visible);
    updateButtons();
    saveButtons();
}

void WindowMenu::updateButtons()
{
    int x = mPadding;
    int h = 0;
    FOR_EACH (STD_VECTOR <Button*>::const_iterator, it, mButtons)
        safeRemove(*it);
    const int pad2 = 2 * mPadding;
    FOR_EACH (STD_VECTOR <Button*>::iterator, it, mButtons)
    {
        Button *const btn = *it;
        if (btn == nullptr)
            continue;
        if (btn->mVisible == Visible_true)
        {
            btn->setPosition(x, mPadding);
            add(btn);
            x += btn->getWidth() + mSpacing;
            h = btn->getHeight() + pad2;
        }
    }
    x += mPadding - mSpacing;
    if (mainGraphics != nullptr)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));
}

void WindowMenu::loadButtons()
{
    if (!mSmallWindow)
    {
        if (config.getValue("windowmenu0", "").empty())
        {
            for (std::map <std::string, ButtonInfo*>::iterator
                 it = mButtonNames.begin(),
                 it_fend = mButtonNames.end(); it != it_fend; ++it)
            {
                const ButtonInfo *const info = (*it).second;
                if (info == nullptr ||
                    info->button == nullptr ||
                    info->visible == Visible_true)
                {
                    continue;
                }
                info->button->setVisible(Visible_false);
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
            const ButtonInfo *const info = mButtonNames[str];
            if ((info == nullptr) || (info->button == nullptr))
                continue;
            info->button->setVisible(Visible_false);
        }
    }
    else
    {
        for (std::map <std::string, ButtonInfo*>::iterator
             it = mButtonNames.begin(),
             it_fend = mButtonNames.end(); it != it_fend; ++it)
        {
            const ButtonInfo *const info = (*it).second;
            if ((info == nullptr) || (info->button == nullptr))
                continue;
            Button *const button = info->button;
            const std::string &str = button->getActionEventId();
            button->setVisible(fromBool(
                str == "SET" || str == "WIN", Visible));
        }
    }
    updateButtons();
}

void WindowMenu::saveButtons() const
{
    int i = 0;
    FOR_EACH (STD_VECTOR <Button*>::const_iterator, it, mButtons)
    {
        const Button *const btn = *it;
        if ((btn != nullptr) && btn->mVisible == Visible_false)
        {
            config.setValue("windowmenu" + toString(i),
                btn->getActionEventId());
            i ++;
        }
    }
    for (int f = i; f < 30; f ++)
        config.deleteKey("windowmenu" + toString(f));
}

void WindowMenu::drawChildren(Graphics *const graphics)
{
    if (mHaveMouse ||
        mAutoHide == 0 ||
        (mAutoHide == 1 &&
        mainGraphics != nullptr &&
        (mSmallWindow || mainGraphics->mWidth > 800)))
    {
        Container::drawChildren(graphics);
    }
}

void WindowMenu::safeDrawChildren(Graphics *const graphics)
{
    if (mHaveMouse ||
        mAutoHide == 0 ||
        (mAutoHide == 1 &&
        mainGraphics != nullptr &&
        (mSmallWindow || mainGraphics->mWidth > 800)))
    {
        Container::safeDrawChildren(graphics);
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
#endif  // USE_PROFILER
