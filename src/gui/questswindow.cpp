/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus developers
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

#include "gui/questswindow.h"

#include "logger.h"

#include "gui/gui.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/extendednamesmodel.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include <guichan/font.hpp>

#include "debug.h"

struct QuestItemText
{
    QuestItemText(std::string text0, int type0) :
        text(text0), type(type0)
    {
    }

    std::string text;
    int type;
};

struct QuestItem
{
    int var;
    std::string name;
    std::string group;
    std::set<int> incomplete;
    std::set<int> complete;
    std::vector<QuestItemText> texts;
};

class QuestsModel : public ExtendedNamesModel
{
    public:
        QuestsModel()
        {
        }

        virtual ~QuestsModel()
        { }
};

QuestsWindow::QuestsWindow() :
    Window(_("Quests Window"), false, nullptr, "quest.xml")
{
    setWindowName("Quests");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);

    setDefaultSize(400, 350, ImageRect::RIGHT);
    setMinWidth(400);
    setMinHeight(350);

    mQuestsModel = new QuestsModel;
    mQuestsListBox = new ExtendedListBox(mQuestsModel);
    mQuestsListBox->setActionEventId("select");
    mQuestsListBox->addActionListener(this);

    mQuestScrollArea = new ScrollArea(mQuestsListBox);
    mQuestScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mText = new BrowserBox;
    mText->setOpaque(false);
    mTextScrollArea = new ScrollArea(mText);
    mTextScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mQuestsListBox->setWidth(500);

    mCloseButton = new Button(_("Close"), "close", this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mQuestScrollArea, 4, 3).setPadding(3);
    placer(4, 0, mTextScrollArea, 4, 3).setPadding(3);
    placer(7, 3, mCloseButton);

    loadWindowState();
    loadXml();

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
}

void QuestsWindow::loadXml()
{
    XML::Document doc("quests.xml");
    XmlNodePtr root = doc.rootNode();
    if (!root)
        return;

    for_each_xml_child_node(varNode, root)
    {
        if (xmlNameEqual(varNode, "var"))
        {
            int id = XML::getProperty(varNode, "id", 0);
            if (id < 0)
                continue;
            for_each_xml_child_node(questNode, varNode)
            {
                if (xmlNameEqual(questNode, "quest"))
                    loadQuest(id, questNode);
            }
        }
    }
}

void QuestsWindow::loadQuest(int var, XmlNodePtr node)
{
    QuestItem *quest = new QuestItem();
    quest->name = XML::langProperty(node, "name", _("unknown"));
    quest->group = XML::getProperty(node, "group", "");
    std::string incompleteStr = XML::getProperty(node, "incomplete", "");
    std::string completeStr = XML::getProperty(node, "complete", "");
    if (incompleteStr.empty() || completeStr.empty())
    {
        logger->log("complete flags incorrect");
        delete quest;
        return;
    }
    quest->incomplete = splitToIntSet(incompleteStr, ',');
    quest->complete = splitToIntSet(completeStr, ',');
    if (quest->incomplete.size() + quest->complete.size() == 0)
    {
        logger->log("complete flags incorrect");
        delete quest;
        return;
    }
    for_each_xml_child_node(dataNode, node)
    {
        if (!xmlTypeEqual(dataNode, XML_ELEMENT_NODE))
            continue;
        const char *data = reinterpret_cast<const char*>(
            xmlNodeGetContent(dataNode));
        if (!data)
            continue;
        std::string str = translator->getStr(data);

        if (xmlNameEqual(dataNode, "text"))
            quest->texts.push_back(QuestItemText(str, 0));
        else if (xmlNameEqual(dataNode, "name"))
            quest->texts.push_back(QuestItemText(str, 1));
    }
    mQuests[var].push_back(quest);
}

void QuestsWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "select")
    {
        int id = mQuestsListBox->getSelected();
        if (id < 0)
            return;
        showQuest(mQuestLinks[id]);
    }
    else if (eventId == "close")
    {
        setVisible(false);
    }
}

void QuestsWindow::updateQuest(int var, int val)
{
    mVars[var] = val;
}

void QuestsWindow::rebuild()
{
    mQuestsModel->clear();
    StringVect &names = mQuestsModel->getNames();

    for (std::map<int, int>::const_iterator it = mVars.begin(),
         it_end = mVars.end(); it != it_end; ++ it)
    {
        int var = (*it).first;
        int val = (*it).second;
        const std::vector<QuestItem*> &quests = mQuests[var];
        for (std::vector<QuestItem*>::const_iterator it2 = quests.begin(),
             it2_end = quests.end(); it2 != it2_end; ++ it2)
        {
            if (!*it2)
                continue;
            QuestItem *quest = *it2;
            if (quest->complete.find(val) != quest->complete.end())
            {
                names.push_back(quest->name + _("[complete]"));
                mQuestLinks.push_back(quest);
            }
            else if (quest->incomplete.find(val) != quest->incomplete.end())
            {
                names.push_back(quest->name);
                mQuestLinks.push_back(quest);
            }
        }
    }
//    mQuestsListBox->adjustSize();
}

void QuestsWindow::showQuest(QuestItem *quest)
{
    if (!quest)
        return;

    const std::vector<QuestItemText> &texts = quest->texts;
    mText->clearRows();
    for (std::vector<QuestItemText>::const_iterator it = texts.begin(),
         it_end = texts.end(); it != it_end; ++ it)
    {
        const QuestItemText &data = *it;
        switch (data.type)
        {
            case 0:
            default:
                mText->addRow(translator->getStr(data.text));
                break;
            case 1:
                mText->addRow("[" + translator->getStr(data.text) + "]");
                break;
        }
    }
}
