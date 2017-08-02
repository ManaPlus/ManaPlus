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

#include "gui/windows/skilldialog.h"

#include "configuration.h"
#include "effectmanager.h"
#include "spellmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/resources/spriteaction.h"

#include "enums/resources/skill/skillsettype.h"

#include "gui/shortcut/itemshortcut.h"

#include "gui/windows/setupwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/tabs/skilltab.h"

#include "gui/windows/textdialog.h"

#include "listeners/textskilllistener.h"

#include "net/playerhandler.h"
#include "net/skillhandler.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/timer.h"

#include "resources/beingcommon.h"

#include "debug.h"

SkillDialog *skillDialog = nullptr;

namespace
{
    TextSkillListener textSkillListener;
}  // namespace

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
    Window(_("Skills"), Modal_false, nullptr, "skills.xml"),
    ActionListener(),
    mSkills(),
    mDurations(),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    mDeleteTabs(),
    mPointsLabel(new Label(this, "0")),
    // TRANSLATORS: skills dialog button
    mUseButton(new Button(this, _("Use"), "use", this)),
    // TRANSLATORS: skills dialog button
    mIncreaseButton(new Button(this, _("Up"), "inc", this)),
    mDefaultModel(nullptr),
    mDefaultTab(nullptr)
{
    setWindowName("Skills");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(windowContainer->getWidth() - 280, 30, 275, 425);
    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    mUseButton->setEnabled(false);
    mIncreaseButton->setEnabled(false);
    mTabs->setSelectable(false);
    mTabs->getTabContainer()->setSelectable(false);
    mTabs->getWidgetContainer()->setSelectable(false);

    place(0, 0, mTabs, 5, 5);
    place(0, 5, mPointsLabel, 4);
    place(3, 5, mUseButton);
    place(4, 5, mIncreaseButton);
}

void SkillDialog::postInit()
{
    Window::postInit();
    setLocationRelativeTo(getParent());
    loadWindowState();
    enableVisibleSound(true);
}

SkillDialog::~SkillDialog()
{
    clearSkills();
}

void SkillDialog::addDefaultTab()
{
    mDefaultModel = new SkillModel;
    SkillListBox *const listbox = new SkillListBox(this,
        mDefaultModel);
    listbox->setActionEventId("sel");
    listbox->addActionListener(this);
    ScrollArea *const scroll = new ScrollArea(this,
        listbox,
        Opaque_false);
    scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);
    // TRANSLATORS: unknown skills tab name
    mDefaultTab = new SkillTab(this, _("Unknown"), listbox);
    mDeleteTabs.push_back(mDefaultTab);
    mDefaultTab->setVisible(Visible_false);
    mTabs->addTab(mDefaultTab, scroll);
    mTabs->adjustTabPositions();
    mTabs->setSelectedTabDefault();
}

void SkillDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "inc")
    {
        if (playerHandler == nullptr)
            return;
        const SkillTab *const tab = static_cast<const SkillTab *>(
            mTabs->getSelectedTab());
        if (tab != nullptr)
        {
            if (const SkillInfo *const info = tab->getSelectedInfo())
                playerHandler->increaseSkill(CAST_U16(info->id));
        }
    }
    else if (eventId == "sel")
    {
        const SkillTab *const tab = static_cast<const SkillTab *>(
            mTabs->getSelectedTab());
        if (tab != nullptr)
        {
            if (const SkillInfo *const info = tab->getSelectedInfo())
            {
                mUseButton->setEnabled(info->isUsable());
                mUseButton->setCaption(info->useButton);
                mIncreaseButton->setEnabled(info->id < SKILL_VAR_MIN_ID);
                const int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < CAST_S32(SHORTCUT_TABS)
                    && (itemShortcut[num] != nullptr))
                {
                    itemShortcut[num]->setItemSelected(
                        info->id + SKILL_MIN_ID);
                }
            }
            else
            {
                mUseButton->setEnabled(false);
                mIncreaseButton->setEnabled(false);
                // TRANSLATORS: skills dialog button
                mUseButton->setCaption(_("Use"));
            }
        }
    }
    else if (eventId == "use")
    {
        const SkillTab *const tab = static_cast<const SkillTab *>(
            mTabs->getSelectedTab());
        if (tab != nullptr)
        {
            const SkillInfo *const info = tab->getSelectedInfo();
            useSkill(info,
                fromBool(config.getBoolValue("skillAutotarget"), AutoTarget),
                info->customSelectedLevel,
                info->useTextParameter,
                std::string(),
                info->customCastType,
                info->customOffsetX,
                info->customOffsetY);
        }
    }
    else if (eventId == "close")
    {
        setVisible(Visible_false);
    }
}

std::string SkillDialog::update(const int id)
{
    const SkillMap::const_iterator i = mSkills.find(id);

    if (i != mSkills.end())
    {
        SkillInfo *const info = i->second;
        if (info != nullptr)
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
        PlayerInfo::getAttribute(Attributes::PLAYER_SKILL_POINTS)));
    mPointsLabel->adjustSize();

    ItemShortcut *const shortcuts = itemShortcut[SHORTCUT_AUTO_TAB];
    shortcuts->clear();
    size_t idx = 0;

    FOR_EACH (SkillMap::const_iterator, it, mSkills)
    {
        SkillInfo *const info = (*it).second;
        if (info == nullptr)
            continue;
        if (info->modifiable == Modifiable_true)
            info->update();
        if (info->visible == Visible_false ||
            idx >= SHORTCUT_ITEMS ||
            !info->data->autoTab)
        {
            continue;
        }
        const SkillType::SkillType type = info->type;
        if (type == SkillType::Attack ||
            type == SkillType::Ground ||
            type == SkillType::Self ||
            type == SkillType::Support)
        {
            shortcuts->setItemFast(idx,
                info->id + SKILL_MIN_ID,
                fromInt(info->customSelectedLevel, ItemColor));

            shortcuts->setItemData(idx, strprintf("%d %d %d",
                CAST_S32(info->customCastType),
                info->customOffsetX,
                info->customOffsetY));
            idx ++;
        }
    }

    skillPopup->reset();
}

void SkillDialog::updateModels()
{
    std::set<SkillModel*> models;

    FOR_EACH (SkillMap::const_iterator, it, mSkills)
    {
        SkillInfo *const info = (*it).second;
        if (info != nullptr)
        {
            SkillModel *const model = info->model;
            if (model != nullptr)
                models.insert(model);
        }
    }
    FOR_EACH (std::set<SkillModel*>::iterator, it, models)
    {
        SkillModel *const model = *it;
        if (model != nullptr)
            model->updateVisibilities();
    }
}

void SkillDialog::clearSkills()
{
    mTabs->removeAll();
    mDeleteTabs.clear();
    mDefaultTab = nullptr;
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
        if ((info != nullptr) && info->owner == owner)
        {
            PlayerInfo::setSkillLevel(info->id, 0);
            if (info->alwaysVisible == Visible_false)
                info->visible = Visible_false;
        }
    }
}

void SkillDialog::loadSkills()
{
    clearSkills();
    loadXmlFile(paths.getStringValue("skillsFile"), SkipError_false);
    if (mSkills.empty())
        loadXmlFile(paths.getStringValue("skillsFile2"), SkipError_false);
    loadXmlFile(paths.getStringValue("skillsPatchFile"), SkipError_true);
    loadXmlDir("skillsPatchDir", loadXmlFile);
    addDefaultTab();

    update();
}

void SkillDialog::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodePtrConst root = doc.rootNode();

    int setCount = 0;

    if ((root == nullptr) || !xmlNameEqual(root, "skills"))
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
                loadXmlFile(name, skipError);
            continue;
        }
        else if (xmlNameEqual(set, "set"))
        {
            setCount++;
            const std::string setName = XML::getProperty(set, "name",
                // TRANSLATORS: skills dialog default skill tab
                strprintf(_("Skill Set %d"), setCount));

            const std::string setTypeStr = XML::getProperty(set, "type", "");
            SkillSetTypeT setType = SkillSetType::VerticalList;
            if (setTypeStr.empty() ||
                setTypeStr == "list" ||
                setTypeStr == "vertical")
            {
                setType = SkillSetType::VerticalList;
            }
            else if (setTypeStr == "rectangle")
            {
                setType = SkillSetType::Rectangle;
            }

            bool alwaysVisible = false;
            SkillModel *const model = new SkillModel;
            SkillTab *tab = nullptr;
            ScrollArea *scroll = nullptr;

            switch (setType)
            {
                case SkillSetType::VerticalList:
                {
                    // possible leak listbox, scroll
                    SkillListBox *const listbox = new SkillListBox(this,
                        model);
                    listbox->setActionEventId("sel");
                    listbox->addActionListener(this);
                    scroll = new ScrollArea(this,
                        listbox,
                        Opaque_false);
                    scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
                    scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);
                    tab = new SkillTab(this, setName, listbox);
                    break;
                }
                case SkillSetType::Rectangle:
                {
                    SkillRectangleListBox *const listbox =
                        new SkillRectangleListBox(this,
                        model);
                    listbox->setActionEventId("sel");
                    listbox->addActionListener(this);
                    scroll = new ScrollArea(this,
                        listbox,
                        Opaque_false);
                    scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
                    scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);
                    tab = new SkillTab(this, setName, listbox);
                    break;
                }
                default:
                    reportAlways("Unsupported skillset type: %s",
                        setTypeStr.c_str());
                    return;
            }
            if (mDefaultModel == nullptr)
            {
                mDefaultModel = model;
                mDefaultTab = tab;
            }

            mDeleteTabs.push_back(tab);
            if (alwaysVisible == true)
                tab->setVisible(Visible_true);
            else
                tab->setVisible(Visible_false);
            mTabs->addTab(tab, scroll);

            for_each_xml_child_node(node, set)
            {
                if (xmlNameEqual(node, "skill"))
                {
                    SkillInfo *const skill = loadSkill(node, model);
                    if (skill == nullptr)
                        continue;
                    if (skill->alwaysVisible == Visible_true)
                        alwaysVisible = true;
                    skill->tab = tab;
                    for_each_xml_child_node(levelNode, node)
                    {
                        if (!xmlNameEqual(levelNode, "level"))
                            continue;
                        loadSkillData(node, skill);
                    }
                }
            }

            model->updateVisibilities();
        }
    }
}

SkillInfo *SkillDialog::loadSkill(XmlNodeConstPtr node,
                                  SkillModel *const model)
{
    int id = XML::getIntProperty(node, "id", -1, -1, 1000000);
    if (id == -1)
    {
        id = XML::getIntProperty(node, "var", -1, -1, 100000);
        if (id == -1)
            return nullptr;
        id += SKILL_VAR_MIN_ID;
    }

    std::string name = XML::langProperty(node, "name",
        // TRANSLATORS: skills dialog. skill id
        strprintf(_("Skill %d"), id));

    SkillInfo *skill = getSkill(id);
    if (skill == nullptr)
    {
        skill = new SkillInfo;
        skill->id = CAST_U32(id);
        skill->modifiable = Modifiable_false;
        skill->model = model;
        skill->update();
        skill->useButton = XML::getProperty(
            // TRANSLATORS: skills dialog button
            node, "useButton", _("Use"));
        skill->owner = parseOwner(XML::getProperty(
            node, "owner", "player"));
        skill->errorText = XML::getProperty(
            node, "errorText", name);
        skill->alwaysVisible = fromBool(XML::getBoolProperty(
            node, "alwaysVisible", false), Visible);
        skill->castingAction = XML::getProperty(node,
            "castingAction", SpriteAction::CAST);
        skill->castingRideAction = XML::getProperty(node,
            "castingRideAction", SpriteAction::CASTRIDE);
        skill->castingSkyAction = XML::getProperty(node,
            "castingSkyAction", SpriteAction::CASTSKY);
        skill->castingWaterAction = XML::getProperty(node,
            "castingWaterAction", SpriteAction::CASTWATER);
        skill->useTextParameter = XML::getBoolProperty(
            node, "useTextParameter", false);
        skill->x = XML::getProperty(node,
            "x", 0);
        skill->y = XML::getProperty(node,
            "y", 0);
        skill->visible = skill->alwaysVisible;
        model->addSkill(skill);
        mSkills[id] = skill;
    }

    loadSkillData(node, skill);
    return skill;
}

void SkillDialog::loadSkillData(XmlNodeConstPtr node,
                                SkillInfo *const skill)
{
    if (skill == nullptr)
        return;
    const int level = (skill->alwaysVisible == Visible_true) ?
        0 : XML::getProperty(node, "level", 0);
    SkillData *data = skill->getData(level);
    if (data == nullptr)
        data = new SkillData;

    const std::string name = XML::langProperty(node, "name",
        // TRANSLATORS: skills dialog. skill id
        strprintf(_("Skill %u"), skill->id));
    data->name = name;
    const std::string icon = XML::getProperty(node, "icon", "");
    if (icon.empty())
    {
        data->setIcon(paths.getStringValue("missingSkillIcon"));
        data->haveIcon = false;
    }
    else
    {
        data->setIcon(icon);
        data->haveIcon = true;
    }
    if (skill->id < SKILL_VAR_MIN_ID)
    {
        data->dispName = strprintf("%s, %u",
            name.c_str(),
            skill->id);
    }
    else
    {
        data->dispName = strprintf("%s, (%u)",
            name.c_str(),
            skill->id - SKILL_VAR_MIN_ID);
    }
    data->shortName = XML::langProperty(node,
        "shortName", name.substr(0, 3));
    data->description = XML::langProperty(
        node, "description", "");
    data->particle = XML::getProperty(
        node, "particle", "");
    data->castingParticle = XML::getProperty(
        node, "castingParticle", "");
    data->castingAnimation = XML::getProperty(
        node,
        "castingAnimation",
        paths.getStringValue("skillCastingAnimation"));

    data->soundHit.sound = XML::getProperty(
        node, "soundHit", "");
    data->soundHit.delay = XML::getProperty(
        node, "soundHitDelay", 0);
    data->soundMiss.sound = XML::getProperty(
        node, "soundMiss", "");
    data->soundMiss.delay = XML::getProperty(
        node, "soundMissDelay", 0);
    data->invokeCmd = XML::getProperty(
        node, "invokeCmd", "");
    data->updateEffectId = XML::getProperty(
        node, "levelUpEffectId", -1);
    data->removeEffectId = XML::getProperty(
        node, "removeEffectId", -1);
    data->hitEffectId = XML::getProperty(
        node, "hitEffectId", -1);
    data->missEffectId = XML::getProperty(
        node, "missEffectId", -1);
    data->castingSrcEffectId = XML::getProperty(
        node, "castingSrcEffectId", -1);
    data->castingDstEffectId = XML::getProperty(
        node, "castingDstEffectId", -1);
    data->srcEffectId = XML::getProperty(
        node, "srcEffectId", -1);
    data->dstEffectId = XML::getProperty(
        node, "dstEffectId", -1);
    data->castingGroundEffectId = XML::getProperty(
        node, "castingGroundEffectId", -1);
    data->autoTab = XML::getBoolProperty(
        node, "autoTab", true);

    skill->addData(level, data);
}

void SkillDialog::removeSkill(const int id)
{
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info != nullptr)
        {
            info->level = 0;
            info->update();
            PlayerInfo::setSkillLevel(id, 0);
            if (info->alwaysVisible == Visible_false)
                info->visible = Visible_false;
        }
    }
}

bool SkillDialog::updateSkill(const int id,
                              const int range,
                              const Modifiable modifiable,
                              const SkillType::SkillType type,
                              const int sp)
{
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info != nullptr)
        {
            info->modifiable = modifiable;
            info->range = range;
            info->type = type;
            info->sp = sp;
            info->update();
            if (info->tab != nullptr)
            {
                info->tab->setVisible(Visible_true);
                mTabs->adjustTabPositions();
                mTabs->setSelectedTabDefault();
            }
        }
        return true;
    }
    return false;
}

std::string SkillDialog::getDefaultSkillIcon(const SkillType::SkillType type)
{
    std::string icon;
    switch (type)
    {
        case SkillType::Attack:
            icon = paths.getStringValue("attackSkillIcon");
            break;
        case SkillType::Ground:
            icon = paths.getStringValue("groundSkillIcon");
            break;
        case SkillType::Self:
            icon = paths.getStringValue("selfSkillIcon");
            break;
        case SkillType::Unused:
            icon = paths.getStringValue("unusedSkillIcon");
            break;
        case SkillType::Support:
            icon = paths.getStringValue("supportSkillIcon");
            break;
        case SkillType::TargetTrap:
            icon = paths.getStringValue("trapSkillIcon");
            break;
        case SkillType::Unknown:
            icon = paths.getStringValue("unknownSkillIcon");
            break;
        default:
            break;
    }
    return icon;
}

void SkillDialog::addSkill(const SkillOwner::Type owner,
                           const int id,
                           const std::string &name,
                           const int level,
                           const int range,
                           const Modifiable modifiable,
                           const SkillType::SkillType type,
                           const int sp)
{
    if (mDefaultModel != nullptr)
    {
        SkillInfo *const skill = new SkillInfo;
        skill->id = CAST_U32(id);
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
        const std::string icon = getDefaultSkillIcon(type);
        if (icon.empty())
        {
            data->setIcon(paths.getStringValue("missingSkillIcon"));
            data->haveIcon = false;
        }
        else
        {
            data->setIcon(icon);
            data->haveIcon = true;
        }
        data->autoTab = settings.unknownSkillsAutoTab;
        data->shortName = toString(skill->id);
        skill->modifiable = modifiable;
        skill->visible = Visible_false;
        skill->alwaysVisible = Visible_false;
        skill->model = mDefaultModel;
        skill->level = level;
        // TRANSLATORS: skills dialog. skill level
        skill->skillLevel = strprintf(_("Lvl: %d"), level);
        skill->range = range;
        skill->sp = sp;
        skill->update();
        // TRANSLATORS: skills dialog button
        skill->useButton = _("Use");
        // TRANSLATORS: skill error message
        skill->errorText = strprintf(_("Failed skill: %s"), name.c_str());
        skill->tab = mDefaultTab;
        mDefaultModel->addSkill(skill);
        mDefaultTab->setVisible(Visible_true);
        mTabs->adjustTabPositions();
        mTabs->setSelectedTabDefault();

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
        addSkill(owner, id, "", 0, 0, Modifiable_false, SkillType::Unknown, 0);
        it = mSkills.find(id);
    }
    if (it != mSkills.end())
    {
        info = (*it).second;
    }
    if (info != nullptr)
    {
        info->duration = duration;
        info->durationTime = tick_time;
        addSkillDuration(info);
    }
}

void SkillDialog::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    if (mTabs != nullptr)
        mTabs->adjustSize();
}

void SkillDialog::useItem(const int itemId,
                          const AutoTarget autoTarget,
                          const int level,
                          const std::string &data) const
{
    const std::map<int, SkillInfo*>::const_iterator
        it = mSkills.find(itemId - SKILL_MIN_ID);
    if (it == mSkills.end())
        return;

    const SkillInfo *const info = (*it).second;
    CastTypeT castType = CastType::Default;
    int offsetX = 0;
    int offsetY = 0;

    if (!data.empty())
    {
        STD_VECTOR<int> vect;
        splitToIntVector(vect, data, ' ');
        const size_t sz = vect.size();
        if (sz > 0)
            castType = static_cast<CastTypeT>(vect[0]);
        if (sz > 2)
        {
            offsetX = vect[1];
            offsetY = vect[2];
        }
    }
    useSkill(info,
        autoTarget,
        level,
        false,
        std::string(),
        castType,
        offsetX,
        offsetY);
}

void SkillDialog::updateTabSelection()
{
    const SkillTab *const tab = static_cast<SkillTab*>(
        mTabs->getSelectedTab());
    if (tab != nullptr)
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
            // TRANSLATORS: inventory button
            mUseButton->setCaption(_("Use"));
        }
    }
}

void SkillDialog::updateQuest(const int var,
                              const int val1,
                              const int val2 A_UNUSED,
                              const int val3 A_UNUSED,
                              const int time1 A_UNUSED)
{
    const int id = var + SKILL_VAR_MIN_ID;
    const SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info != nullptr)
        {
            PlayerInfo::setSkillLevel(id, val1);
            info->level = val1;
            info->update();
        }
    }
}

SkillData *SkillDialog::getSkillData(const int id) const
{
    const SkillMap::const_iterator it = mSkills.find(id);
    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info != nullptr)
            return info->data;
    }
    return nullptr;
}

SkillData *SkillDialog::getSkillDataByLevel(const int id,
                                            const int level) const
{
    const SkillMap::const_iterator it = mSkills.find(id);
    if (it != mSkills.end())
    {
        SkillInfo *const info = it->second;
        if (info != nullptr)
            return info->getData1(level);
    }
    return nullptr;
}

void SkillDialog::playUpdateEffect(const int id) const
{
    if (effectManager == nullptr)
        return;
    const SkillData *const data = getSkillData(id);
    if (data == nullptr)
        return;
    effectManager->triggerDefault(data->updateEffectId,
        localPlayer,
        paths.getIntValue("skillLevelUpEffectId"));
}

void SkillDialog::playRemoveEffect(const int id) const
{
    if (effectManager == nullptr)
        return;
    const SkillData *const data = getSkillData(id);
    if (data == nullptr)
        return;
    effectManager->triggerDefault(data->removeEffectId,
        localPlayer,
        paths.getIntValue("skillRemoveEffectId"));
}

void SkillDialog::playCastingDstTileEffect(const int id,
                                           const int level,
                                           const int x,
                                           const int y,
                                           const int delay) const
{
    if (effectManager == nullptr)
        return;
    SkillData *const data = getSkillDataByLevel(id, level);
    if (data == nullptr)
        return;
    effectManager->triggerDefault(data->castingGroundEffectId,
        x * 32,
        y * 32,
        cur_time + delay / 1000,  // end time in seconds
        paths.getIntValue("skillCastingGroundEffectId"));
}

void SkillDialog::useSkill(const int skillId,
                           const AutoTarget autoTarget,
                           int level,
                           const bool withText,
                           const std::string &text,
                           CastTypeT castType,
                           const int offsetX,
                           const int offsetY)
{
    SkillInfo *const info = skillDialog->getSkill(skillId);
    if (info == nullptr)
        return;
    if (castType == CastType::Default)
        castType = info->customCastType;
    useSkill(info,
        autoTarget,
        level,
        withText,
        text,
        castType,
        offsetX,
        offsetY);
}

void SkillDialog::useSkill(const SkillInfo *const info,
                           const AutoTarget autoTarget,
                           int level,
                           const bool withText,
                           const std::string &text,
                           const CastTypeT castType,
                           const int offsetX,
                           const int offsetY)
{
    if ((info == nullptr) || (localPlayer == nullptr))
        return;
    if (level == 0)
        level = info->level;

    const SkillData *data = info->getData1(level);
    if (data != nullptr)
    {
        const std::string cmd = data->invokeCmd;
        if (!cmd.empty())
            SpellManager::invokeCommand(cmd, localPlayer->getTarget());
    }
    switch (castType)
    {
        default:
        case CastType::Default:
            useSkillDefault(info,
                autoTarget,
                level,
                withText,
                text,
                offsetX,
                offsetY);
            break;
        case CastType::Target:
        {
            const Being *const being = localPlayer->getTarget();
            useSkillTarget(info,
                autoTarget,
                level,
                withText,
                text,
                being,
                offsetX,
                offsetY);
            break;
        }
        case CastType::Position:
        {
            int x = 0;
            int y = 0;
            viewport->getMouseTile(x, y);
            useSkillPosition(info,
                level,
                withText,
                text,
                x,
                y,
                offsetX,
                offsetY);
            break;
        }
        case CastType::Self:
            // +++ probably need call useSkillSelf
            useSkillTarget(info,
                autoTarget,
                level,
                withText,
                text,
                localPlayer,
                offsetX,
                offsetY);
            break;
    }
}

void SkillDialog::useSkillTarget(const SkillInfo *const info,
                                 const AutoTarget autoTarget,
                                 int level,
                                 const bool withText,
                                 const std::string &text,
                                 const Being *being,
                                 int offsetX,
                                 int offsetY)
{
    SkillType::SkillType type = info->type;
    if ((type & SkillType::Attack) != 0)
    {
        if ((being == nullptr) && autoTarget == AutoTarget_true)
        {
            if (localPlayer != nullptr)
            {
                being = localPlayer->setNewTarget(ActorType::Monster,
                    AllowSort_true);
            }
        }
        if (being != nullptr)
        {
            skillHandler->useBeing(info->id,
                level,
                being->getId());
        }
    }
    else if ((type & SkillType::Support) != 0)
    {
        if (being == nullptr)
            being = localPlayer;
        if (being != nullptr)
        {
            skillHandler->useBeing(info->id,
                level,
                being->getId());
        }
    }
    else if ((type & SkillType::Self) != 0)
    {
        skillHandler->useBeing(info->id,
            level,
            localPlayer->getId());
    }
    else if ((type & SkillType::Ground) != 0)
    {
        if (being == nullptr)
            return;
        being->fixDirectionOffsets(offsetX, offsetY);
        const int x = being->getTileX() + offsetX;
        const int y = being->getTileY() + offsetY;
        if (info->useTextParameter)
        {
            if (withText)
            {
                skillHandler->usePos(info->id,
                    level,
                    x, y,
                    text);
            }
            else
            {
                const SkillData *data = info->getData1(level);
                textSkillListener.setSkill(info->id,
                    x,
                    y,
                    level);
                TextDialog *const dialog = CREATEWIDGETR(TextDialog,
                    // TRANSLATORS: text skill dialog header
                    strprintf(_("Add text to skill %s"),
                    data->name.c_str()),
                    // TRANSLATORS: text skill dialog field
                    _("Text: "));
                dialog->setModal(Modal_true);
                textSkillListener.setDialog(dialog);
                dialog->setActionEventId("ok");
                dialog->addActionListener(&textSkillListener);
            }
        }
        else
        {
            skillHandler->usePos(info->id,
                level,
                x, y);
        }
    }
    else if ((type & SkillType::TargetTrap) != 0)
    {
        // for now unused
    }
    else if (type == SkillType::Unknown ||
             type == SkillType::Unused)
    {
        // unknown / unused
    }
    else
    {
        reportAlways("Unsupported skill type: %d", type);
    }
}

void SkillDialog::useSkillPosition(const SkillInfo *const info,
                                   int level,
                                   const bool withText,
                                   const std::string &text,
                                   const int x,
                                   const int y,
                                   int offsetX,
                                   int offsetY)
{
    SkillType::SkillType type = info->type;
    if ((type & SkillType::Ground) != 0)
    {
        localPlayer->fixDirectionOffsets(offsetX, offsetY);
        if (info->useTextParameter)
        {
            if (withText)
            {
                skillHandler->usePos(info->id,
                    level,
                    x + offsetX,
                    y + offsetY,
                    text);
            }
            else
            {
                const SkillData *data = info->getData1(level);
                textSkillListener.setSkill(info->id,
                    x + offsetX,
                    y + offsetY,
                    level);
                TextDialog *const dialog = CREATEWIDGETR(TextDialog,
                    // TRANSLATORS: text skill dialog header
                    strprintf(_("Add text to skill %s"),
                    data->name.c_str()),
                    // TRANSLATORS: text skill dialog field
                    _("Text: "));
                dialog->setModal(Modal_true);
                textSkillListener.setDialog(dialog);
                dialog->setActionEventId("ok");
                dialog->addActionListener(&textSkillListener);
            }
        }
        else
        {
            skillHandler->usePos(info->id,
                level,
                x + offsetX,
                y + offsetY);
        }
    }
    else if ((type & SkillType::Support) != 0)
    {
        // wrong type
        skillHandler->useBeing(info->id,
            level,
            localPlayer->getId());
    }
    else if ((type & SkillType::Self) != 0)
    {
        skillHandler->useBeing(info->id,
            level,
            localPlayer->getId());
    }
    else if ((type & SkillType::Attack) != 0)
    {
        // do nothing
        // +++ probably need select some target on x,y position?
    }
    else if ((type & SkillType::TargetTrap) != 0)
    {
        // for now unused
    }
    else if (type == SkillType::Unknown ||
             type == SkillType::Unused)
    {
        // unknown / unused
    }
    else
    {
        reportAlways("Unsupported skill type: %d", type);
    }
}

void SkillDialog::useSkillDefault(const SkillInfo *const info,
                                  const AutoTarget autoTarget,
                                  int level,
                                  const bool withText,
                                  const std::string &text,
                                  int offsetX,
                                  int offsetY)
{
    SkillType::SkillType type = info->type;
    if ((type & SkillType::Attack) != 0)
    {
        const Being *being = localPlayer->getTarget();
        if ((being == nullptr) && autoTarget == AutoTarget_true)
        {
            being = localPlayer->setNewTarget(ActorType::Monster,
                AllowSort_true);
        }
        if (being != nullptr)
        {
            skillHandler->useBeing(info->id,
                level,
                being->getId());
        }
    }
    else if ((type & SkillType::Support) != 0)
    {
        const Being *being = localPlayer->getTarget();
        if (being == nullptr)
            being = localPlayer;
        if (being != nullptr)
        {
            skillHandler->useBeing(info->id,
                level,
                being->getId());
        }
    }
    else if ((type & SkillType::Self) != 0)
    {
        skillHandler->useBeing(info->id,
            level,
            localPlayer->getId());
    }
    else if ((type & SkillType::Ground) != 0)
    {
        int x = 0;
        int y = 0;
        viewport->getMouseTile(x, y);
        localPlayer->fixDirectionOffsets(offsetX, offsetY);
        x += offsetX;
        y += offsetY;
        if (info->useTextParameter)
        {
            if (withText)
            {
                skillHandler->usePos(info->id,
                    level,
                    x, y,
                    text);
            }
            else
            {
                const SkillData *data = info->getData1(level);
                textSkillListener.setSkill(info->id,
                    x,
                    y,
                    level);
                TextDialog *const dialog = CREATEWIDGETR(TextDialog,
                    // TRANSLATORS: text skill dialog header
                    strprintf(_("Add text to skill %s"),
                    data->name.c_str()),
                    // TRANSLATORS: text skill dialog field
                    _("Text: "));
                dialog->setModal(Modal_true);
                textSkillListener.setDialog(dialog);
                dialog->setActionEventId("ok");
                dialog->addActionListener(&textSkillListener);
            }
        }
        else
        {
            skillHandler->usePos(info->id,
                level,
                x, y);
        }
    }
    else if ((type & SkillType::TargetTrap) != 0)
    {
        // for now unused
    }
    else if (type == SkillType::Unknown ||
             type == SkillType::Unused)
    {
        // unknown / unused
    }
    else
    {
        reportAlways("Unsupported skill type: %d", type);
    }
}

void SkillDialog::addSkillDuration(SkillInfo *const skill)
{
    if (skill == nullptr)
        return;

    FOR_EACH (STD_VECTOR<SkillInfo*>::const_iterator, it, mDurations)
    {
        if ((*it)->id == skill->id)
            return;
    }
    mDurations.push_back(skill);
}

void SkillDialog::slowLogic()
{
    FOR_EACH_SAFE (STD_VECTOR<SkillInfo*>::iterator, it, mDurations)
    {
        SkillInfo *const skill = *it;
        if (skill != nullptr)
        {
            const int time = get_elapsed_time(skill->durationTime);
            if (time >= skill->duration)
            {
                it = mDurations.erase(it);
                skill->cooldown = 0;
                skill->duration = 0;
                skill->durationTime = 0;
                if (it == mDurations.end())
                    return;
                if (it != mDurations.begin())
                    -- it;
            }
            else if (time != 0)
            {
                skill->cooldown = skill->duration * 100 / time;
            }
        }
    }
}

void SkillDialog::selectSkillLevel(const int skillId,
                                   const int level)
{
    SkillInfo *const info = getSkill(skillId);
    if (info == nullptr)
        return;
    if (level > info->level)
        info->customSelectedLevel = info->level;
    else
        info->customSelectedLevel = level;
    info->update();
}

void SkillDialog::selectSkillCastType(const int skillId,
                                      const CastTypeT type)
{
    SkillInfo *const info = getSkill(skillId);
    if (info == nullptr)
        return;
    info->customCastType = type;
    info->update();
}

void SkillDialog::setSkillOffsetX(const int skillId,
                                  const int offset)
{
    SkillInfo *const info = getSkill(skillId);
    if (info == nullptr)
        return;
    info->customOffsetX = offset;
    info->update();
}

void SkillDialog::setSkillOffsetY(const int skillId,
                                  const int offset)
{
    SkillInfo *const info = getSkill(skillId);
    if (info == nullptr)
        return;
    info->customOffsetY = offset;
    info->update();
}
