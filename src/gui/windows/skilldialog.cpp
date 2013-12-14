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

#include "gui/windows/skilldialog.h"

#include "configuration.h"
#include "dragdrop.h"
#include "effectmanager.h"
#include "itemshortcut.h"

#include "being/localplayer.h"

#include "gui/viewport.h"

#include "gui/popups/textpopup.h"

#include "gui/windows/setup.h"
#include "gui/windows/shortcutwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/skillmodel.h"
#include "gui/widgets/tabs/tab.h"
#include "gui/widgets/tabbedarea.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/skillhandler.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

class SkillListBox final : public ListBox
{
    public:
        SkillListBox(const Widget2 *const widget, SkillModel *const model) :
            ListBox(widget, model, "skilllistbox.xml"),
            mModel(model),
            mPopup(new TextPopup),
            mHighlightColor(getThemeColor(Theme::HIGHLIGHT)),
            mTextColor(getThemeColor(Theme::TEXT)),
            mTextColor2(getThemeColor(Theme::TEXT_OUTLINE)),
            mTextPadding(mSkin ? mSkin->getOption("textPadding", 34) : 34),
            mSpacing(mSkin ? mSkin->getOption("spacing", 0) : 0),
            mRowHeight(getFont()->getHeight() * 2 + mSpacing + 2 * mPadding),
            mSkillClicked(false)
        {
            mPopup->postInit();

            if (mRowHeight < 34)
                mRowHeight = 34;
        }

        A_DELETE_COPY(SkillListBox)

        ~SkillListBox()
        {
            delete mModel;
            mModel = nullptr;
            delete mPopup;
            mPopup = nullptr;
        }

        SkillInfo *getSelectedInfo() const
        {
            const int selected = getSelected();
            if (!mListModel || selected < 0
                || selected > mListModel->getNumberOfElements())
            {
                return nullptr;
            }

            return static_cast<SkillModel*>(mListModel)->getSkillAt(selected);
        }

        void draw(gcn::Graphics *gcnGraphics) override
        {
            if (!mListModel)
                return;

            SkillModel *const model = static_cast<SkillModel*>(mListModel);
            updateAlpha();
            Graphics *const graphics = static_cast<Graphics *const>(
                gcnGraphics);

            mHighlightColor.a = static_cast<int>(mAlpha * 255.0F);
            graphics->setColor(mHighlightColor);

            // Draw filled rectangle around the selected list element
            if (mSelected >= 0)
            {
                graphics->fillRectangle(gcn::Rectangle(mPadding, getRowHeight()
                    * mSelected + mPadding, getWidth() - 2 * mPadding,
                    getRowHeight()));
            }

            // Draw the list elements
            graphics->setColorAll(mTextColor, mTextColor2);
            gcn::Font *const font = getFont();
            const int space = font->getHeight() + mSpacing;
            const int width2 = getWidth() - mPadding;
            for (int i = 0, y = 1;
                 i < model->getNumberOfElements();
                 ++i, y += getRowHeight())
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e)
                {
                    const SkillData *const data = e->data;
                    const int yPad = y + mPadding;
                    const std::string &description = data->description;
                    DRAW_IMAGE(graphics, data->icon, mPadding, yPad);
                    font->drawString(graphics, data->name, mTextPadding, yPad);
                    if (!description.empty())
                    {
                        font->drawString(graphics, description,
                            mTextPadding, yPad + space);
                    }

                    if (e->skillLevelWidth < 0)
                    {
                        // Add one for padding
                        e->skillLevelWidth = font->getWidth(e->skillLevel) + 1;
                    }

                    font->drawString(graphics, e->skillLevel, width2
                        - e->skillLevelWidth, yPad);
                }
            }
        }

        unsigned int getRowHeight() const override
        { return mRowHeight; }

        const SkillInfo *getSkillByEvent(const gcn::MouseEvent &event) const
        {
            const int y = (event.getY() + mPadding) / getRowHeight();
            if (!mModel || y >= mModel->getNumberOfElements())
                return nullptr;
            const SkillInfo *const skill = mModel->getSkillAt(y);
            if (!skill)
                return nullptr;
            return skill;
        }

        void mouseMoved(gcn::MouseEvent &event) override
        {
            ListBox::mouseMoved(event);
            if (!viewport || !dragDrop.isEmpty())
                return;

            const SkillInfo *const skill = getSkillByEvent(event);
            if (!skill)
                return;

            mPopup->show(viewport->getMouseX(), viewport->getMouseY(),
                skill->data->dispName, skill->data->description);
        }

        void mouseDragged(gcn::MouseEvent &event)
        {
            if (event.getButton() == gcn::MouseEvent::LEFT)
            {
                if (dragDrop.isEmpty())
                {
                    if (mSkillClicked)
                    {
                        mSkillClicked = false;
                        const SkillInfo *const skill = getSkillByEvent(event);
                        if (!skill)
                            return;
                        dragDrop.dragSkill(skill, DRAGDROP_SOURCE_SKILLS);
                        dragDrop.setItem(skill->id + SKILL_MIN_ID);
                    }
                    ListBox::mouseDragged(event);
                }
            }
            else
            {
                ListBox::mouseDragged(event);
            }
        }

        void mousePressed(gcn::MouseEvent &event)
        {
            ListBox::mousePressed(event);
            if (event.getButton() == gcn::MouseEvent::LEFT)
            {
                const SkillInfo *const skill = getSkillByEvent(event);
                if (!skill)
                    return;
                mSkillClicked = true;
            }
        }

        void mouseReleased(gcn::MouseEvent &event)
        {
            ListBox::mouseReleased(event);
        }

        void mouseExited(gcn::MouseEvent &event A_UNUSED) override
        {
            mPopup->hide();
        }

    private:
        SkillModel *mModel;
        TextPopup *mPopup;
        gcn::Color mHighlightColor;
        gcn::Color mTextColor;
        gcn::Color mTextColor2;
        int mTextPadding;
        int mSpacing;
        int mRowHeight;
        bool mSkillClicked;
};

class SkillTab final : public Tab
{
    public:
        SkillTab(const Widget2 *const widget,
                 const std::string &name, SkillListBox *const listBox) :
            Tab(widget),
            mListBox(listBox)
        {
            setCaption(name);
        }

        A_DELETE_COPY(SkillTab)

        ~SkillTab()
        {
            delete mListBox;
            mListBox = nullptr;
        }

        SkillInfo *getSelectedInfo() const
        {
            if (mListBox)
                return mListBox->getSelectedInfo();
            else
                return nullptr;
        }

    protected:
        void setCurrent() override
        {
            if (skillDialog)
                skillDialog->updateTabSelection();
        }

    private:
        SkillListBox *mListBox;
};

SkillDialog::SkillDialog() :
    // TRANSLATORS: skills dialog name
    Window(_("Skills"), false, nullptr, "skills.xml"),
    gcn::ActionListener(),
    mSkills(),
    mTabs(new TabbedArea(this)),
    mDeleteTabs(),
    mPointsLabel(new Label(this, "0")),
    // TRANSLATORS: skills dialog button
    mUseButton(new Button(this, _("Use"), "use", this)),
    // TRANSLATORS: skills dialog button
    mIncreaseButton(new Button(this, _("Up"), "inc", this)),
    mDefaultModel(nullptr)
{
    mTabs->postInit();
    setWindowName("Skills");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(windowContainer->getWidth() - 280, 30, 275, 425);
    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    mUseButton->setEnabled(false);
    mIncreaseButton->setEnabled(false);

    place(0, 0, mTabs, 5, 5);
    place(0, 5, mPointsLabel, 4);
    place(3, 5, mUseButton);
    place(4, 5, mIncreaseButton);
}

void SkillDialog::postInit()
{
    setLocationRelativeTo(getParent());
    loadWindowState();
    enableVisibleSound(true);
}

SkillDialog::~SkillDialog()
{
    clearSkills();
}

void SkillDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "inc")
    {
        const SkillTab *const tab = static_cast<const SkillTab *const>(
            mTabs->getSelectedTab());
        if (tab)
        {
            if (const SkillInfo *const info = tab->getSelectedInfo())
                Net::getPlayerHandler()->increaseSkill(info->id);
        }
    }
    else if (eventId == "sel")
    {
        const SkillTab *const tab = static_cast<const SkillTab *const>(
            mTabs->getSelectedTab());
        if (tab)
        {
            if (const SkillInfo *const info = tab->getSelectedInfo())
            {
                mUseButton->setEnabled(info->range > 0);
                mIncreaseButton->setEnabled(info->id < SKILL_VAR_MIN_ID);
                const int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS)
                    && itemShortcut[num])
                {
                    itemShortcut[num]->setItemSelected(
                        info->id + SKILL_MIN_ID);
                }
            }
            else
            {
                mUseButton->setEnabled(false);
                mIncreaseButton->setEnabled(false);
            }
        }
    }
    else if (eventId == "use")
    {
        const SkillTab *const tab = static_cast<const SkillTab *const>(
            mTabs->getSelectedTab());
        if (tab)
        {
            const SkillInfo *const info = tab->getSelectedInfo();
            if (info && player_node && player_node->getTarget())
            {
                const Being *const being = player_node->getTarget();
                if (being)
                {
                    Net::getSkillHandler()->useBeing(info->level,
                        info->id, being->getId());
                }
            }
        }
    }
    else if (eventId == "close")
    {
        setVisible(false);
    }
}

std::string SkillDialog::update(const int id)
{
    const SkillMap::const_iterator i = mSkills.find(id);

    if (i != mSkills.end())
    {
        SkillInfo *const info = i->second;
        if (info)
        {
            info->update();
            return info->data->name;
        }
    }

    return std::string();
}

void SkillDialog::update()
{
    // TRANSLATORS: skills dialog label
    mPointsLabel->setCaption(strprintf(_("Skill points available: %d"),
        PlayerInfo::getAttribute(PlayerInfo::SKILL_POINTS)));
    mPointsLabel->adjustSize();

    FOR_EACH (SkillMap::const_iterator, it, mSkills)
    {
        if ((*it).second && (*it).second->modifiable)
            (*it).second->update();
    }
}

void SkillDialog::clearSkills()
{
    mTabs->removeAll();
    mDeleteTabs.clear();
    mDefaultModel = nullptr;

    delete_all(mSkills);
    mSkills.clear();
}

void SkillDialog::loadSkills()
{
    clearSkills();

    XML::Document doc(paths.getStringValue("skillsFile"));
    XML::Document doc2(paths.getStringValue("skillsFile2"));
    XmlNodePtr root = doc.rootNode();

    int setCount = 0;
    std::string setName;
    ScrollArea *scroll;
    SkillListBox *listbox;
    SkillTab *tab;

    if (!root || !xmlNameEqual(root, "skills"))
        root = doc2.rootNode();

    if (!root || !xmlNameEqual(root, "skills"))
    {
        logger->log("Error loading skills");

#ifdef MANASERV_SUPPORT
        if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
        {
            SkillModel *const model = new SkillModel();
            if (!mDefaultModel)
                mDefaultModel = model;

            SkillInfo *const skill = new SkillInfo;
            skill->id = 1;
            // TRANSLATORS: skills dialog default skills tab
            skill->data->name = _("basic");
            skill->data->description.clear();
            // TRANSLATORS: skills dialog default skill name
            skill->data->dispName = _("basic, 1");
            skill->data->shortName = "bas";
            skill->data->setIcon("");
            skill->modifiable = true;
            skill->visible = true;
            skill->model = model;
            skill->update();

            model->addSkill(skill);
            mSkills[1] = skill;

            model->updateVisibilities();

            listbox = new SkillListBox(this, model);
            listbox->postInit();
            listbox->setActionEventId("sel");
            listbox->addActionListener(this);
            scroll = new ScrollArea(listbox, false);
            scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
            scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            tab = new SkillTab(this, "Skills", listbox);
            mDeleteTabs.push_back(tab);

            mTabs->addTab(tab, scroll);

            update();
        }
        return;
    }

    for_each_xml_child_node(set, root)
    {
        if (xmlNameEqual(set, "set"))
        {
            setCount++;
            setName = XML::getProperty(set, "name",
                // TRANSLATORS: skills dialog default skill tab
                strprintf(_("Skill Set %d"), setCount));

            SkillModel *const model = new SkillModel();
            if (!mDefaultModel)
                mDefaultModel = model;

            for_each_xml_child_node(node, set)
            {
                if (xmlNameEqual(node, "skill"))
                {
                    int id = XML::getIntProperty(node, "id", -1, -1, 1000000);
                    if (id == -1)
                    {
                        id = XML::getIntProperty(node, "var", -1, -1, 100000);
                        if (id == -1)
                            continue;
                        id += SKILL_VAR_MIN_ID;
                    }

                    SkillInfo *skill = getSkill(id);
                    if (!skill)
                    {
                        skill = new SkillInfo;
                        skill->id = static_cast<unsigned int>(id);
                        skill->modifiable = false;
                        skill->visible = false;
                        skill->model = model;
                        skill->update();
                        model->addSkill(skill);
                        mSkills[id] = skill;
                    }

                    std::string name = XML::langProperty(node, "name",
                        // TRANSLATORS: skills dialog. skill id
                        strprintf(_("Skill %d"), id));
                    std::string icon = XML::getProperty(node, "icon", "");
                    const int level = XML::getProperty(node, "level", 0);
                    SkillData *data = skill->getData(level);
                    if (!data)
                        data = new SkillData();

                    data->name = name;
                    data->setIcon(icon);
                    if (skill->id < SKILL_VAR_MIN_ID)
                    {
                        data->dispName = strprintf("%s, %u",
                            name.c_str(), skill->id);
                    }
                    else
                    {
                        data->dispName = strprintf("%s, (%u)",
                            name.c_str(), skill->id - SKILL_VAR_MIN_ID);
                    }
                    data->shortName = XML::langProperty(node,
                        "shortName", name.substr(0, 3));
                    data->description = XML::langProperty(
                        node, "description", "");
                    data->particle = XML::getProperty(
                        node, "particle", "");

                    data->soundHit.sound = XML::getProperty(
                        node, "soundHit", "");
                    data->soundHit.delay = XML::getProperty(
                        node, "soundHitDelay", 0);
                    data->soundMiss.sound = XML::getProperty(
                        node, "soundMiss", "");
                    data->soundMiss.delay = XML::getProperty(
                        node, "soundMissDelay", 0);

                    skill->addData(level, data);
                }
            }

            model->updateVisibilities();

            // possible leak listbox, scroll
            listbox = new SkillListBox(this, model);
            listbox->setActionEventId("sel");
            listbox->addActionListener(this);
            scroll = new ScrollArea(listbox, false);
            scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
            scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            tab = new SkillTab(this, setName, listbox);
            mDeleteTabs.push_back(tab);

            mTabs->addTab(tab, scroll);
        }
    }
    update();
}

bool SkillDialog::updateSkill(const int id, const int range,
                              const bool modifiable)
{
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info)
        {
            info->modifiable = modifiable;
            info->range = range;
            info->update();
        }
        return true;
    }
    return false;
}

void SkillDialog::addSkill(const int id, const int level, const int range,
                           const bool modifiable)
{
    if (mDefaultModel)
    {
        SkillInfo *const skill = new SkillInfo;
        skill->id = static_cast<unsigned int>(id);
        SkillData *const data = skill->data;
        data->name = "Unknown skill Id: " + toString(id);
        data->dispName = data->name;
        data->description.clear();
        data->setIcon("");
        skill->modifiable = modifiable;
        skill->visible = false;
        skill->model = mDefaultModel;
        skill->level = level;
        // TRANSLATORS: skills dialog. skill level
        skill->skillLevel = strprintf(_("Lvl: %d"), level);
        skill->range = range;
        skill->update();

        mDefaultModel->addSkill(skill);

        mSkills[id] = skill;
        mDefaultModel->updateVisibilities();
    }
}

SkillInfo* SkillDialog::getSkill(const int id) const
{
    SkillMap::const_iterator it = mSkills.find(id);
    if (it != mSkills.end())
        return (*it).second;
    return nullptr;
}

SkillInfo* SkillDialog::getSkillByItem(const int itemId) const
{
    SkillMap::const_iterator it = mSkills.find(itemId - SKILL_MIN_ID);
    if (it != mSkills.end())
        return (*it).second;
    return nullptr;
}

void SkillDialog::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    if (mTabs)
        mTabs->adjustSize();
}

void SkillDialog::useItem(const int itemId) const
{
    const std::map<int, SkillInfo*>::const_iterator
        it = mSkills.find(itemId - SKILL_MIN_ID);
    if (it == mSkills.end())
        return;

    const SkillInfo *const info = (*it).second;
    if (info && player_node && player_node->getTarget())
    {
        const Being *const being = player_node->getTarget();
        if (being)
        {
            Net::getSkillHandler()->useBeing(info->level,
                info->id, being->getId());
        }
    }
}

void SkillDialog::updateTabSelection()
{
    const SkillTab *const tab = static_cast<SkillTab*>(
        mTabs->getSelectedTab());
    if (tab)
    {
        if (const SkillInfo *const info = tab->getSelectedInfo())
        {
            mUseButton->setEnabled(info->range > 0);
            mIncreaseButton->setEnabled(info->id < SKILL_VAR_MIN_ID);
        }
        else
        {
            mUseButton->setEnabled(false);
        }
    }
}

void SkillDialog::updateQuest(const int var, const int val)
{
    const int id = var + SKILL_VAR_MIN_ID;
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info)
        {
            PlayerInfo::setSkillLevel(id, val);
            info->level = val;
            info->update();
        }
    }
}

void SkillDialog::playUpdateEffect(const int id) const
{
    const int effectId = paths.getIntValue("skillLevelUpEffectId");
    if (!effectManager || effectId == -1)
        return;
    const SkillMap::const_iterator it = mSkills.find(id);
    if (it != mSkills.end())
    {
        if (it->second)
            effectManager->trigger(effectId, player_node);
    }
}
