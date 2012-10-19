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

#include "configuration.h"
#include "logger.h"
#include "sound.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"
#include "gui/theme.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/extendednamesmodel.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"

#include "utils/translation/podict.h"

#include <guichan/font.hpp>

#include "debug.h"

struct QuestItemText final
{
    QuestItemText(std::string text0, const int type0) :
        text(text0), type(type0)
    {
    }

    std::string text;
    int type;
};

struct QuestItem final
{
    QuestItem() :
        var(0), completeFlag(-1)
    {
    }

    int var;
    std::string name;
    std::string group;
    std::set<int> incomplete;
    std::set<int> complete;
    std::vector<QuestItemText> texts;
    int completeFlag;
};

class QuestsModel final : public ExtendedNamesModel
{
    public:
        QuestsModel() :
            ExtendedNamesModel()
        {
        }

        A_DELETE_COPY(QuestsModel)

        virtual ~QuestsModel()
        { }
};

QuestsWindow::QuestsWindow() :
    Window(_("Quests"), false, nullptr, "quests.xml"),
    gcn::ActionListener(),
    mQuestsModel(new QuestsModel),
    mQuestsListBox(new ExtendedListBox(this, mQuestsModel)),
    mQuestScrollArea(new ScrollArea(mQuestsListBox,
        getOptionBool("showlistbackground"), "quests_list_background.xml")),
    mItemLinkHandler(new ItemLinkHandler),
    mText(new BrowserBox(this, BrowserBox::AUTO_WRAP)),
    mTextScrollArea(new ScrollArea(mText,
        getOptionBool("showtextbackground"), "quests_text_background.xml")),
    mCloseButton(new Button(_("Close"), "close", this)),
    mCompleteIcon(Theme::getImageFromThemeXml("complete_icon.xml", "")),
    mIncompleteIcon(Theme::getImageFromThemeXml("incomplete_icon.xml", ""))
{
    setWindowName("Quests");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setSaveVisible(true);

    setDefaultSize(400, 350, ImageRect::RIGHT);
    setMinWidth(400);
    setMinHeight(350);

    mQuestsListBox->setActionEventId("select");
    mQuestsListBox->addActionListener(this);

    mQuestScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mText->setOpaque(false);
    mText->setLinkHandler(mItemLinkHandler);
    mTextScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mQuestsListBox->setWidth(500);
    if (gui->getNpcFont()->getHeight() < 20)
        mQuestsListBox->setRowHeight(20);
    else
        mQuestsListBox->setRowHeight(gui->getNpcFont()->getHeight());

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mQuestScrollArea, 4, 3).setPadding(3);
    placer(4, 0, mTextScrollArea, 4, 3).setPadding(3);
    placer(7, 3, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();
    enableVisibleSound(true);
    loadXml();
}

QuestsWindow::~QuestsWindow()
{
    delete mQuestsModel;
    mQuestsModel = nullptr;

    for (std::map<int, std::vector<QuestItem*> >::iterator it
         = mQuests.begin(), it_end = mQuests.end(); it != it_end; ++ it)
    {
        std::vector<QuestItem*> &quests = (*it).second;
        for (std::vector<QuestItem*>::iterator it2 = quests.begin(),
             it2_end = quests.end(); it2 != it2_end; ++ it2)
        {
            delete *it2;
        }
    }
    delete mItemLinkHandler;
    mItemLinkHandler = nullptr;
    mQuests.clear();
    mQuestLinks.clear();
    if (mCompleteIcon)
    {
        mCompleteIcon->decRef();
        mCompleteIcon = nullptr;
    }
    if (mIncompleteIcon)
    {
        mIncompleteIcon->decRef();
        mIncompleteIcon = nullptr;
    }
}

void QuestsWindow::loadXml()
{
    XML::Document doc("quests.xml");
    const XmlNodePtr root = doc.rootNode();
    if (!root)
        return;

    for_each_xml_child_node(varNode, root)
    {
        if (xmlNameEqual(varNode, "var"))
        {
            const int id = XML::getProperty(varNode, "id", 0);
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

void QuestsWindow::loadQuest(const int var, const XmlNodePtr node)
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
    if (quest->incomplete.empty() && quest->complete.empty())
    {
        logger->log("complete flags incorrect");
        delete quest;
        return;
    }
    for_each_xml_child_node(dataNode, node)
    {
        if (!xmlTypeEqual(dataNode, XML_ELEMENT_NODE))
            continue;
        const char *const data = reinterpret_cast<const char*>(
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
        const int id = mQuestsListBox->getSelected();
        if (id < 0)
            return;
        showQuest(mQuestLinks[id]);
    }
    else if (eventId == "close")
    {
        setVisible(false);
    }
}

void QuestsWindow::updateQuest(const int var, const int val)
{
    mVars[var] = val;
}

void QuestsWindow::rebuild(const bool playSound)
{
    mQuestsModel->clear();
    mQuestLinks.clear();
    StringVect &names = mQuestsModel->getNames();
    std::vector<Image*> &images = mQuestsModel->getImages();
    std::vector<QuestItem*> complete;
    std::vector<QuestItem*> incomplete;
    int updatedQuest = -1;
    int newCompleteStatus = -1;

    for (std::map<int, int>::const_iterator it = mVars.begin(),
         it_end = mVars.end(); it != it_end; ++ it)
    {
        const int var = (*it).first;
        const int val = (*it).second;
        const std::vector<QuestItem*> &quests = mQuests[var];
        for (std::vector<QuestItem*>::const_iterator it2 = quests.begin(),
             it2_end = quests.end(); it2 != it2_end; ++ it2)
        {
            if (!*it2)
                continue;
            QuestItem *const quest = *it2;
            // complete quest
            if (quest->complete.find(val) != quest->complete.end())
                complete.push_back(quest);
            // incomplete quest
            else if (quest->incomplete.find(val) != quest->incomplete.end())
                incomplete.push_back(quest);
        }
    }

    int k = 0;

    for (std::vector<QuestItem*>::const_iterator it = complete.begin(),
        it_end = complete.end(); it != it_end; ++ it, k ++)
    {
        QuestItem *const quest = *it;
        if (quest->completeFlag == 0)
        {
            updatedQuest = k;
            newCompleteStatus = 1;
        }
        quest->completeFlag = 1;
        mQuestLinks.push_back(quest);
        names.push_back(quest->name);
        if (mCompleteIcon)
        {
            mCompleteIcon->incRef();
            images.push_back(mCompleteIcon);
        }
        else
        {
            images.push_back(nullptr);
        }
    }

    for (std::vector<QuestItem*>::const_iterator it = incomplete.begin(),
        it_end = incomplete.end(); it != it_end; ++ it, k ++)
    {
        QuestItem *const quest = *it;
        if (quest->completeFlag == -1)
        {
            updatedQuest = k;
            newCompleteStatus = 0;
        }
        quest->completeFlag = 0;
        mQuestLinks.push_back(quest);
        names.push_back(quest->name);
        if (mIncompleteIcon)
        {
            mIncompleteIcon->incRef();
            images.push_back(mIncompleteIcon);
        }
        else
        {
            images.push_back(nullptr);
        }
    }

    if (updatedQuest == -1)
        updatedQuest = static_cast<int>(mQuestLinks.size() - 1);
    else if (updatedQuest >= static_cast<int>(mQuestLinks.size()))
        updatedQuest = static_cast<int>(mQuestLinks.size() - 1);
    if (updatedQuest >= 0)
    {
        mQuestsListBox->setSelected(updatedQuest);
        showQuest(mQuestLinks[updatedQuest]);
        if (playSound)
        {
            switch (newCompleteStatus)
            {
                case 0:
                    sound.playSfx(paths.getValue("newQuestSfx", ""));
                    break;
                case 1:
                    sound.playSfx(paths.getValue("completeQuestSfx", ""));
                    break;
                default:
                    break;
            }
        }
    }
}

void QuestsWindow::showQuest(const QuestItem *const quest)
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
