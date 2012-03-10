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

#include "gui/skilldialog.h"

#include "itemshortcut.h"
#include "localplayer.h"
#include "logger.h"
#include "playerinfo.h"
#include "configuration.h"

#include "gui/setup.h"
#include "gui/shortcutwindow.h"
#include "gui/textpopup.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tab.h"
#include "gui/widgets/tabbedarea.h"
#include "gui/widgets/windowcontainer.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/specialhandler.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include <guichan/font.hpp>

#include <set>
#include <string>

#include "debug.h"

class SkillModel;
class SkillEntry;

class SkillModel : public gcn::ListModel
{
public:
    int getNumberOfElements()
    { return static_cast<int>(mVisibleSkills.size()); }

    SkillInfo *getSkillAt(int i) const
    { return mVisibleSkills.at(i); }

    std::string getElementAt(int i)
    {
        if (getSkillAt(i))
            return getSkillAt(i)->name;
        else
            return "";
    }

    void updateVisibilities();

    void addSkill(SkillInfo *info)
    { mSkills.push_back(info); }

private:
    SkillList mSkills;
    SkillList mVisibleSkills;
};

class SkillListBox : public ListBox
{
public:
    SkillListBox(SkillModel *model):
        ListBox(model),
        mModel(model),
        mPopup(new TextPopup()),
        mHighlightColor(Theme::getThemeColor(Theme::HIGHLIGHT)),
        mTextColor(Theme::getThemeColor(Theme::TEXT))
    {
    }

    ~SkillListBox()
    {
        delete mModel;
        mModel = nullptr;
        delete mPopup;
        mPopup = nullptr;
    }

    SkillInfo *getSelectedInfo()
    {
        const int selected = getSelected();
        if (!mListModel || selected < 0
            || selected > mListModel->getNumberOfElements())
        {
            return nullptr;
        }

        return static_cast<SkillModel*>(mListModel)->getSkillAt(selected);
    }

    void draw(gcn::Graphics *gcnGraphics)
    {
        if (!mListModel)
            return;

        SkillModel* model = static_cast<SkillModel*>(mListModel);

        updateAlpha();

        Graphics *graphics = static_cast<Graphics*>(gcnGraphics);

        mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
        graphics->setColor(mHighlightColor);
        graphics->setFont(getFont());

        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(0, getRowHeight()
                * mSelected, getWidth(), getRowHeight()));
        }

        // Draw the list elements
        graphics->setColor(mTextColor);
        for (int i = 0, y = 1;
             i < model->getNumberOfElements();
             ++i, y += getRowHeight())
        {
            SkillInfo *e = model->getSkillAt(i);

            if (e)
                e->draw(graphics, y, getWidth());
        }
    }

    unsigned int getRowHeight() const
    { return 34; }

    void mouseMoved(gcn::MouseEvent &event)
    {
        ListBox::mouseMoved(event);
        if (!viewport)
            return;

        int y = event.getY() / getRowHeight();
        if (!mModel || y >= mModel->getNumberOfElements())
            return;
        SkillInfo *skill = mModel->getSkillAt(y);
        if (!skill)
            return;

        mPopup->show(viewport->getMouseX(), viewport->getMouseY(),
            skill->dispName);
    }

    void mouseExited(gcn::MouseEvent &event A_UNUSED)
    {
        mPopup->hide();
    }

private:
    SkillModel *mModel;
    TextPopup *mPopup;
    gcn::Color mHighlightColor;
    gcn::Color mTextColor;
};

class SkillTab : public Tab
{
public:
    SkillTab(const std::string &name, SkillListBox *listBox):
            mListBox(listBox)
    {
        setCaption(name);
    }

    ~SkillTab()
    {
        delete mListBox;
        mListBox = nullptr;
    }

    SkillInfo *getSelectedInfo()
    {
        if (mListBox)
            return mListBox->getSelectedInfo();
        else
            return nullptr;
    }

private:
    SkillListBox *mListBox;
};

SkillDialog::SkillDialog():
    Window(_("Skills"), false, nullptr, "skills.xml")
{
    setWindowName("Skills");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(windowContainer->getWidth() - 280, 30, 275, 425);
    setupWindow->registerWindowForReset(this);

    mTabs = new TabbedArea();
    mPointsLabel = new Label("0");
    mUseButton = new Button(_("Use"), "use", this);
    mUseButton->setEnabled(false);
    mIncreaseButton = new Button(_("Up"), "inc", this);
    mDefaultModel = nullptr;

    place(0, 0, mTabs, 5, 5);
    place(0, 5, mPointsLabel, 4);
    place(3, 5, mUseButton);
    place(4, 5, mIncreaseButton);

    setLocationRelativeTo(getParent());
    loadWindowState();
}

SkillDialog::~SkillDialog()
{
    // Clear gui
    loadSkills("");
}

void SkillDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "inc")
    {
        SkillTab *tab = static_cast<SkillTab*>(mTabs->getSelectedTab());
        if (tab)
        {
            if (SkillInfo *info = tab->getSelectedInfo())
                Net::getPlayerHandler()->increaseSkill(info->id);
        }
    }
    else if (event.getId() == "sel")
    {
        SkillTab *tab = static_cast<SkillTab*>(mTabs->getSelectedTab());
        if (tab)
        {
            if (SkillInfo *info = tab->getSelectedInfo())
            {
                mUseButton->setEnabled(info->range > 0);

                int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < SHORTCUT_TABS && itemShortcut[num])
                {
                    itemShortcut[num]->setItemSelected(
                        info->id + SKILL_MIN_ID);
                }
            }
            else
            {
                mUseButton->setEnabled(false);
            }
        }
    }
    else if (event.getId() == "use")
    {
        SkillTab *tab = static_cast<SkillTab*>(mTabs->getSelectedTab());
        if (tab)
        {
            const SkillInfo *info = tab->getSelectedInfo();
            if (info && player_node && player_node->getTarget())
            {
                const Being *being = player_node->getTarget();
                if (being)
                {
                    Net::getSpecialHandler()->useBeing(info->level,
                        info->id, being->getId());
                }
            }
        }
    }
    else if (event.getId() == "close")
    {
        setVisible(false);
    }
}

std::string SkillDialog::update(int id)
{
    SkillMap::const_iterator i = mSkills.find(id);

    if (i != mSkills.end())
    {
        SkillInfo *info = i->second;
        if (info)
        {
            info->update();
            return info->name;
        }
    }

    return std::string();
}

void SkillDialog::update()
{
    mPointsLabel->setCaption(strprintf(_("Skill points available: %d"),
                             PlayerInfo::getAttribute(SKILL_POINTS)));
    mPointsLabel->adjustSize();

    for (SkillMap::const_iterator it = mSkills.begin();
         it != mSkills.end();
          ++ it)
    {
        if ((*it).second && (*it).second->modifiable)
            (*it).second->update();
    }
}

void SkillDialog::loadSkills(const std::string &file)
{
    mTabs->removeAll();

    delete_all(mSkills);
    mSkills.clear();

    if (file.length() == 0)
        return;

    XML::Document doc(file);
    XmlNodePtr root = doc.rootNode();

    int setCount = 0;
    std::string setName;
    ScrollArea *scroll;
    SkillListBox *listbox;
    SkillTab *tab;

    if (!root || !xmlNameEqual(root, "skills"))
    {
        logger->log("Error loading skills file: %s", file.c_str());

        if (Net::getNetworkType() != ServerInfo::MANASERV)
        {
            SkillModel *model = new SkillModel();
            if (!mDefaultModel)
                mDefaultModel = model;

            SkillInfo *skill = new SkillInfo;
            skill->id = 1;
            skill->name = _("basic");
            skill->dispName = _("Skill: basic, Id: 1");
            skill->shortName = "bas";
            skill->setIcon("");
            skill->modifiable = true;
            skill->visible = true;
            skill->model = model;
            skill->update();

            model->addSkill(skill);
            mSkills[1] = skill;

            model->updateVisibilities();

            listbox = new SkillListBox(model);
            listbox->setActionEventId("sel");
            listbox->addActionListener(this);
            scroll = new ScrollArea(listbox);
            scroll->setOpaque(false);
            scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
            scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            tab = new SkillTab("Skills", listbox);
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
                strprintf(_("Skill Set %d"), setCount));

            SkillModel *model = new SkillModel();
            if (!mDefaultModel)
                mDefaultModel = model;

            for_each_xml_child_node(node, set)
            {
                if (xmlNameEqual(node, "skill"))
                {
                    int id = atoi(XML::getProperty(node, "id", "-1").c_str());
                    std::string name = XML::langProperty(node, "name",
                        strprintf(_("Skill %d"), id));
                    std::string icon = XML::getProperty(node, "icon", "");

                    SkillInfo *skill = new SkillInfo;
                    skill->id = static_cast<short unsigned>(id);
                    skill->name = name;
                    skill->dispName = strprintf(_("Skill: %s, Id: %d"),
                        name.c_str(), skill->id);
                    skill->shortName = XML::langProperty(node,
                        "shortName", name.substr(0, 3));
                    skill->setIcon(icon);
                    skill->modifiable = false;
                    skill->visible = false;
                    skill->particle = XML::getProperty(node, "particle", "");
                    skill->soundHit = XML::getProperty(node, "soundHit", "");
                    skill->soundMiss = XML::getProperty(node, "soundMiss", "");
                    skill->model = model;
                    skill->update();

                    model->addSkill(skill);

                    mSkills[id] = skill;
                }
            }

            model->updateVisibilities();

            // possible leak listbox, scroll
            listbox = new SkillListBox(model);
            listbox->setActionEventId("sel");
            listbox->addActionListener(this);
            scroll = new ScrollArea(listbox);
            scroll->setOpaque(false);
            scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
            scroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            tab = new SkillTab(setName, listbox);
            mDeleteTabs.push_back(tab);

            mTabs->addTab(tab, scroll);
        }
    }
    update();
}

bool SkillDialog::updateSkill(int id, int range, bool modifiable)
{
    SkillMap::const_iterator it = mSkills.find(id);

    if (it != mSkills.end())
    {
        SkillInfo *info = it->second;
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

void SkillDialog::addSkill(int id, int level, int range, bool modifiable)
{
    if (mDefaultModel)
    {
        SkillInfo *skill = new SkillInfo;
        skill->id = static_cast<short unsigned>(id);
        skill->name = "Unknown skill Id: " + toString(id);
        skill->dispName = "Unknown skill Id: " + toString(id);
        skill->setIcon("");
        skill->modifiable = modifiable;
        skill->visible = false;
        skill->model = mDefaultModel;
        skill->level = level;
        skill->skillLevel = strprintf(_("Lvl: %d"), level);
        skill->range = range;
        skill->update();

        mDefaultModel->addSkill(skill);

        mSkills[id] = skill;
        mDefaultModel->updateVisibilities();
    }
}

void SkillModel::updateVisibilities()
{
    mVisibleSkills.clear();

    for (SkillList::const_iterator it = mSkills.begin();
         it != mSkills.end();
         ++ it)
    {
        if ((*it)->visible)
            mVisibleSkills.push_back((*it));
    }
}

SkillInfo::SkillInfo() :
    id(0), name(""), shortName(""), dispName(""), icon(nullptr),
    modifiable(false), visible(false), model(nullptr), level(0),
    skillLevel(""), skillLevelWidth(0), skillExp(""), progress(0.0f), range(0),
    particle(""), soundHit(""), soundMiss("")
{
}

SkillInfo::~SkillInfo()
{
    if (icon)
    {
        icon->decRef();
        icon = nullptr;
    }
}

void SkillInfo::setIcon(const std::string &iconPath)
{
    ResourceManager *res = ResourceManager::getInstance();
    if (!iconPath.empty())
    {
        icon = res->getImage(iconPath);
    }

    if (!icon)
    {
        icon = Theme::getImageFromTheme(
            paths.getStringValue("unknownItemFile"));
    }
}

void SkillInfo::update()
{
    int baseLevel = PlayerInfo::getStatBase(id);
    int effLevel = PlayerInfo::getStatEffective(id);

    std::pair<int, int> exp = PlayerInfo::getStatExperience(id);

    if (!modifiable && baseLevel == 0 && effLevel == 0 && exp.second == 0)
    {
        if (visible)
        {
            visible = false;
            if (model)
                model->updateVisibilities();
        }

        return;
    }

    bool updateVisibility = !visible;
    visible = true;

    if (effLevel != baseLevel)
    {
        skillLevel = strprintf(_("Lvl: %d (%+d)"), baseLevel,
                               effLevel - baseLevel);
    }
    else
    {
        if (baseLevel == 0)
            skillLevel.clear();
        else
            skillLevel = strprintf(_("Lvl: %d"), baseLevel);
    }
    level = baseLevel;
    skillLevelWidth = -1;

    if (exp.second)
    {
        skillExp = strprintf("%d / %d", exp.first, exp.second);
        progress = static_cast<float>(exp.first)
                   / static_cast<float>(exp.second);
    }
    else
    {
        skillExp.clear();
        progress = 0.0f;
    }

    color = Theme::getProgressColor(Theme::PROG_EXP, progress);

    if (updateVisibility && model)
        model->updateVisibilities();
}

void SkillInfo::draw(Graphics *graphics, int y, int width)
{
    graphics->drawImage(icon, 1, y);
    graphics->drawText(name, 34, y);

    if (skillLevelWidth < 0)
    {
        // Add one for padding
        skillLevelWidth = graphics->getFont()->getWidth(skillLevel) + 1;
    }

    graphics->drawText(skillLevel, width - skillLevelWidth, y);

    if (!skillExp.empty())
    {
        gcn::Rectangle rect(33, y + 15, width - 33, 17);

        ProgressBar::render(graphics, rect, color, progress, skillExp);
    }
}

SkillInfo* SkillDialog::getSkill(int id)
{
    return mSkills[id];
}

void SkillDialog::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    if (mTabs)
        mTabs->fixSize();
}

void SkillDialog::useItem(int itemId)
{
    const SkillInfo *info = mSkills[itemId - SKILL_MIN_ID];
    if (info && player_node && player_node->getTarget())
    {
        const Being *being = player_node->getTarget();
        if (being)
        {
            Net::getSpecialHandler()->useBeing(info->level,
                info->id, being->getId());
        }
    }
}
