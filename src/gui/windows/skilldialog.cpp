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

#include "gui/windows/skilldialog.h"

#include "configuration.h"
#include "effectmanager.h"
#include "itemshortcut.h"

#include "being/attributes.h"
#include "being/localplayer.h"

#include "gui/models/skillmodel.h"

#include "gui/windows/setupwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/skilllistbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/tabs/skilltab.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/skillhandler.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/timer.h"

#include "resources/beingcommon.h"
#include "resources/skillconsts.h"

#include "debug.h"

SkillDialog *skillDialog = nullptr;

static SkillOwner::Type parseOwner(const std::string &str)
{
    if (str == "player")
        return SkillOwner::Player;
    else if (str == "mercenary")
        return SkillOwner::Mercenary;
    else if (str == "homunculus")
        return SkillOwner::Homunculus;
    return SkillOwner::Player;
}

SkillDialog::SkillDialog() :
    // TRANSLATORS: skills dialog name
    Window(_("Skills"), false, nullptr, "skills.xml"),
    ActionListener(),
    mSkills(),
    mDurations(),
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

void SkillDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "inc")
    {
        const SkillTab *const tab = static_cast<const SkillTab *const>(
            mTabs->getSelectedTab());
        if (tab)
        {
            if (const SkillInfo *const info = tab->getSelectedInfo())
                playerHandler->increaseSkill(static_cast<uint16_t>(info->id));
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
                mUseButton->setEnabled(info->isUsable());
                mUseButton->setCaption(info->useButton);
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
                mUseButton->setCaption(_("Use"));
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
            useSkill(info);
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
        PlayerInfo::getAttribute(Attributes::SKILL_POINTS)));
    mPointsLabel->adjustSize();

    FOR_EACH (SkillMap::const_iterator, it, mSkills)
    {
        SkillInfo *const info = (*it).second;
        if (info && info->modifiable)
            info->update();
    }
}

void SkillDialog::updateModels()
{
    std::set<SkillModel*> models;

    FOR_EACH (SkillMap::const_iterator, it, mSkills)
    {
        SkillInfo *const info = (*it).second;
        if (info)
        {
            SkillModel *const model = info->model;
            if (model)
                models.insert(model);
        }
    }
    FOR_EACH (std::set<SkillModel*>::iterator, it, models)
    {
        SkillModel *const model = *it;
        if (model)
            model->updateVisibilities();
    }
}

void SkillDialog::clearSkills()
{
    mTabs->removeAll();
    mDeleteTabs.clear();
    mDefaultModel = nullptr;

    delete_all(mSkills);
    mSkills.clear();
    mDurations.clear();
}

void SkillDialog::hideSkills(const SkillOwner::Type owner)
{
    FOR_EACH (SkillMap::iterator, it, mSkills)
    {
        SkillInfo *const info = (*it).second;
        if (info && info->owner == owner)
        {
            PlayerInfo::setSkillLevel(info->id, 0);
            info->visible = false;
        }
    }
}

void SkillDialog::loadSkills()
{
    clearSkills();
    loadXmlFile(paths.getStringValue("skillsFile"));
    if (mSkills.empty())
        loadXmlFile(paths.getStringValue("skillsFile2"));
    loadXmlFile(paths.getStringValue("skillsPatchFile"));
    loadXmlDir("skillsPatchDir", loadXmlFile);

    update();
}

void SkillDialog::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName);
    XmlNodePtrConst root = doc.rootNode();

    int setCount = 0;

    if (!root || !xmlNameEqual(root, "skills"))
    {
        logger->log("Error loading skills: " + fileName);
        return;
    }

    for_each_xml_child_node(set, root)
    {
        if (xmlNameEqual(set, "include"))
        {
            const std::string name = XML::getProperty(set, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        else if (xmlNameEqual(set, "set"))
        {
            setCount++;
            const std::string setName = XML::getProperty(set, "name",
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

                    std::string name = XML::langProperty(node, "name",
                        // TRANSLATORS: skills dialog. skill id
                        strprintf(_("Skill %d"), id));

                    SkillInfo *skill = getSkill(id);
                    if (!skill)
                    {
                        skill = new SkillInfo;
                        skill->id = static_cast<unsigned int>(id);
                        skill->modifiable = false;
                        skill->visible = false;
                        skill->model = model;
                        skill->update();
                        skill->useButton = XML::getProperty(
                            node, "useButton", _("Use"));
                        skill->owner = parseOwner(XML::getProperty(
                            node, "owner", "player"));
                        skill->errorText = XML::getProperty(
                            node, "errorText", name);
                        model->addSkill(skill);
                        mSkills[id] = skill;
                    }

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
            SkillListBox *const listbox = new SkillListBox(this, model);
            listbox->setActionEventId("sel");
            listbox->addActionListener(this);
            ScrollArea *const scroll = new ScrollArea(this, listbox, false);
            scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
            scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            SkillTab *const tab = new SkillTab(this, setName, listbox);
            mDeleteTabs.push_back(tab);

            mTabs->addTab(tab, scroll);
        }
    }
}

bool SkillDialog::updateSkill(const int id,
                              const int range,
                              const bool modifiable,
                              const SkillType::SkillType type,
                              const int sp)
{
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info)
        {
            info->modifiable = modifiable;
            info->range = range;
            info->type = type;
            info->sp = sp;
            info->update();
        }
        return true;
    }
    return false;
}

void SkillDialog::addSkill(const SkillOwner::Type owner,
                           const int id,
                           const std::string &name,
                           const int level,
                           const int range,
                           const bool modifiable,
                           const SkillType::SkillType type,
                           const int sp)
{
    if (mDefaultModel)
    {
        SkillInfo *const skill = new SkillInfo;
        skill->id = static_cast<unsigned int>(id);
        skill->type = type;
        skill->owner = owner;
        SkillData *const data = skill->data;
        if (name.empty())
        {
            data->name = "Unknown skill Id: " + toString(id);
            data->dispName = data->name;
        }
        else
        {
            data->name = name;
            data->dispName = strprintf("%s, %u", name.c_str(), skill->id);
        }
        data->description.clear();
        data->setIcon("");
        data->shortName = toString(skill->id);
        skill->modifiable = modifiable;
        skill->visible = false;
        skill->model = mDefaultModel;
        skill->level = level;
        // TRANSLATORS: skills dialog. skill level
        skill->skillLevel = strprintf(_("Lvl: %d"), level);
        skill->range = range;
        skill->sp = sp;
        skill->update();
        skill->useButton = _("Use");
        skill->errorText = name;
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

void SkillDialog::setSkillDuration(const SkillOwner::Type owner,
                                   const int id,
                                   const int duration)
{
    SkillMap::const_iterator it = mSkills.find(id);
    SkillInfo *info = nullptr;
    if (it == mSkills.end())
    {
        addSkill(owner, id, "", 0, 0, false, SkillType::Unknown, 0);
        it = mSkills.find(id);
    }
    if (it != mSkills.end())
    {
        info = (*it).second;
    }
    if (info)
    {
        info->duration = duration;
        info->durationTime = tick_time;
        addSkillDuration(info);
    }
}

void SkillDialog::widgetResized(const Event &event)
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
    useSkill(info);
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
            mUseButton->setCaption(info->useButton);
        }
        else
        {
            mUseButton->setEnabled(false);
            mUseButton->setCaption(_("Use"));
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
            effectManager->trigger(effectId, localPlayer);
    }
}

void SkillDialog::useSkill(const SkillInfo *const info)
{
    if (info && localPlayer)
    {
        switch (info->type)
        {
            case SkillType::Attack:
            case SkillType::Support:
            {
                const Being *const being = localPlayer->getTarget();
                if (being)
                {
                    skillHandler->useBeing(info->level,
                        info->id, being->getId());
                }
                break;
            }
            case SkillType::Self:
                skillHandler->useBeing(info->level,
                    info->id, localPlayer->getId());
                break;

            case SkillType::Ground:
            case SkillType::TargetTrap:
                // for now unused
                break;

            case SkillType::Unknown:
            case SkillType::Unused:
            default:
                break;
        }
    }
}

void SkillDialog::addSkillDuration(SkillInfo *const skill)
{
    FOR_EACH (std::vector<SkillInfo*>::const_iterator, it, mDurations)
    {
        if ((*it)->id == skill->id)
            return;
    }
    mDurations.push_back(skill);
}

void SkillDialog::slowLogic()
{
    FOR_EACH (std::vector<SkillInfo*>::iterator, it, mDurations)
    {
        SkillInfo *const skill = *it;
        if (skill)
        {
            const int time = get_elapsed_time(skill->durationTime);
            if (time >= skill->duration)
            {
                it = mDurations.erase(it);
                skill->cooldown = 0;
                skill->duration = 0;
                skill->durationTime = 0;
                if (it != mDurations.begin())
                    -- it;
            }
            else if (time)
            {
                skill->cooldown = skill->duration * 100 / time;
            }
        }
    }
}
