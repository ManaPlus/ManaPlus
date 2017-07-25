/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "gui/windows/questswindow.h"

#include "actormanager.h"
#include "configuration.h"
#include "effectmanager.h"

#include "being/localplayer.h"

#include "enums/gui/layouttype.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/models/questsmodel.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "resources/questeffect.h"
#include "resources/questitem.h"

#include "resources/db/questdb.h"

#include "resources/map/map.h"

#include "debug.h"

QuestsWindow *questsWindow = nullptr;

QuestsWindow::QuestsWindow() :
    // TRANSLATORS: quests window name
    Window(_("Quests"), Modal_false, nullptr, "quests.xml"),
    ActionListener(),
    mQuestsModel(new QuestsModel),
    mQuestsListBox(CREATEWIDGETR(ExtendedListBox,
        this, mQuestsModel, "extendedlistbox.xml")),
    mQuestScrollArea(new ScrollArea(this, mQuestsListBox,
        fromBool(getOptionBool("showlistbackground"), Opaque),
        "quests_list_background.xml")),
    mItemLinkHandler(new ItemLinkHandler),
    mText(new BrowserBox(this, Opaque_true,
        "browserbox.xml")),
    mTextScrollArea(new ScrollArea(this, mText,
        fromBool(getOptionBool("showtextbackground"), Opaque),
        "quests_text_background.xml")),
    // TRANSLATORS: quests window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    mCompleteIcon(Theme::getImageFromThemeXml("complete_icon.xml", "")),
    mIncompleteIcon(Theme::getImageFromThemeXml("incomplete_icon.xml", "")),
    mMapEffects(),
    mVars(nullptr),
    mQuests(nullptr),
    mAllEffects(nullptr),
    mNpcEffects(),
    mQuestLinks(),
    mQuestReverseLinks(),
    mNewQuestEffectId(paths.getIntValue("newQuestEffectId")),
    mCompleteQuestEffectId(paths.getIntValue("completeQuestEffectId")),
    mMap(nullptr)
{
    setWindowName("Quests");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setSaveVisible(true);

    setDefaultSize(400, 350, ImagePosition::RIGHT);
    setMinWidth(310);
    setMinHeight(220);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    mQuestsListBox->setActionEventId("select");
    mQuestsListBox->addActionListener(this);

    mQuestScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mText->setOpaque(Opaque_false);
    mText->setLinkHandler(mItemLinkHandler);
    mTextScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mQuestsListBox->setWidth(500);
    if ((gui == nullptr) || gui->getNpcFont()->getHeight() < 20)
        mQuestsListBox->setRowHeight(20);
    else
        mQuestsListBox->setRowHeight(gui->getNpcFont()->getHeight());

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mQuestScrollArea, 4, 3).setPadding(3);
    placer(4, 0, mTextScrollArea, 4, 3).setPadding(3);
    placer(7, 3, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    loadWindowState();
    enableVisibleSound(true);
    QuestDb::load();
    mVars = QuestDb::getVars();
    mQuests = QuestDb::getQuests();
    mAllEffects = QuestDb::getAllEffects();
}

QuestsWindow::~QuestsWindow()
{
    delete2(mQuestsModel);

    QuestDb::unload();

    delete2(mItemLinkHandler);
    mQuestLinks.clear();
    mQuestReverseLinks.clear();
    if (mCompleteIcon != nullptr)
    {
        mCompleteIcon->decRef();
        mCompleteIcon = nullptr;
    }
    if (mIncompleteIcon != nullptr)
    {
        mIncompleteIcon->decRef();
        mIncompleteIcon = nullptr;
    }
}

void QuestsWindow::action(const ActionEvent &event)
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
        setVisible(Visible_false);
    }
}

void QuestsWindow::updateQuest(const int var,
                               const int val1,
                               const int val2,
                               const int val3,
                               const int time1)
{
    (*mVars)[var] = QuestVar(val1, val2, val3, time1);
}

void QuestsWindow::rebuild(const bool playSound)
{
    mQuestsModel->clear();
    mQuestLinks.clear();
    mQuestReverseLinks.clear();
    StringVect &names = mQuestsModel->getNames();
    STD_VECTOR<Image*> &images = mQuestsModel->getImages();
    STD_VECTOR<QuestItem*> complete;
    STD_VECTOR<QuestItem*> incomplete;
    STD_VECTOR<QuestItem*> hidden;
    int updatedQuest = -1;
    int newCompleteStatus = -1;

    FOR_EACHP (NpcQuestVarMapCIter, it, mVars)
    {
        const int var = (*it).first;
        const QuestVar &val = (*it).second;
        const STD_VECTOR<QuestItem*> &quests = (*mQuests)[var];
        FOR_EACH (STD_VECTOR<QuestItem*>::const_iterator, it2, quests)
        {
            if (*it2 == nullptr)
                continue;
            QuestItem *const quest = *it2;
            // complete quest
            if (quest->complete.find(val.var1) != quest->complete.end())
            {
                complete.push_back(quest);
            }
            // incomplete quest
            else if (quest->incomplete.find(val.var1) !=
                     quest->incomplete.end())
            {
                incomplete.push_back(quest);
            }
            // hidden quest
            else
            {
                hidden.push_back(quest);
            }
        }
    }

    int k = 0;

    for (STD_VECTOR<QuestItem*>::const_iterator it = complete.begin(),
        it_end = complete.end(); it != it_end; ++ it, k ++)
    {
        QuestItem *const quest = *it;
        if (quest->completeFlag == 0 || (quest->broken
            && quest->completeFlag == -1))
        {
            updatedQuest = k;
            newCompleteStatus = 1;
        }
        quest->completeFlag = 1;
        mQuestLinks.push_back(quest);
        mQuestReverseLinks[quest->var] = k;
        names.push_back(quest->name);
        if (mCompleteIcon != nullptr)
        {
            mCompleteIcon->incRef();
            images.push_back(mCompleteIcon);
        }
        else
        {
            images.push_back(nullptr);
        }
    }

    for (STD_VECTOR<QuestItem*>::const_iterator it = incomplete.begin(),
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
        mQuestReverseLinks[quest->var] = k;
        names.push_back(quest->name);
        if (mIncompleteIcon != nullptr)
        {
            mIncompleteIcon->incRef();
            images.push_back(mIncompleteIcon);
        }
        else
        {
            images.push_back(nullptr);
        }
    }

    FOR_EACH (STD_VECTOR<QuestItem*>::const_iterator, it, hidden)
        (*it)->completeFlag = -1;

    if (updatedQuest == -1 || updatedQuest >= CAST_S32(
        mQuestLinks.size()))
    {
        updatedQuest = CAST_S32(mQuestLinks.size() - 1);
    }
    if (updatedQuest >= 0)
    {
        mQuestsListBox->setSelected(updatedQuest);
        showQuest(mQuestLinks[updatedQuest]);
        if (playSound && (effectManager != nullptr))
        {
            switch (newCompleteStatus)
            {
                case 0:
                    effectManager->trigger(mNewQuestEffectId, localPlayer);
                    break;
                case 1:
                    effectManager->trigger(mCompleteQuestEffectId,
                        localPlayer);
                    break;
                default:
                    break;
            }
        }
    }
    updateEffects();
}

void QuestsWindow::showQuest(const QuestItem *const quest)
{
    if (quest == nullptr)
        return;

    const STD_VECTOR<QuestItemText> &texts = quest->texts;
    const QuestVar &var = (*mVars)[quest->var];
    const std::string var1 = toString(var.var1);
    const std::string var2 = toString(var.var2);
    const std::string var3 = toString(var.var3);
    const std::string timeStr = timeDiffToString(var.time1);
    mText->clearRows();
    FOR_EACH (STD_VECTOR<QuestItemText>::const_iterator, it, texts)
    {
        const QuestItemText &data = *it;
        std::string text = data.text;
        replaceAll(text, "{@@var1}", var1);
        replaceAll(text, "{@@var2}", var2);
        replaceAll(text, "{@@var3}", var3);
        replaceAll(text, "{@@time}", timeStr);
        switch (data.type)
        {
            case QuestType::TEXT:
            default:
                mText->addRow(text);
                break;
            case QuestType::NAME:
                mText->addRow(std::string("[").append(text).append("]"));
                break;
            case QuestType::REWARD:
                mText->addRow(std::string(
                    // TRANSLATORS: quest reward
                    _("Reward:")).append(
                    " ").append(
                    text));
                break;
            case QuestType::GIVER:
                mText->addRow(std::string(
                    // TRANSLATORS: quest giver name
                    _("Quest Giver:")).append(
                    " ").append(
                    text));
                break;
            case QuestType::NPC:
                mText->addRow(std::string(
                    // TRANSLATORS: quest npc name
                    _("Npc:")).append(
                    " ").append(
                    text));
                break;
            case QuestType::COORDINATES:
                mText->addRow(std::string(
                    strprintf("%s [@@=navigate %s %s|%s@@]",
                    // TRANSLATORS: quest coordinates
                    _("Coordinates:"),
                    data.data1.c_str(),
                    data.data2.c_str(),
                    text.c_str())));
                break;
        }
    }
    mText->updateHeight();
}

void QuestsWindow::setMap(const Map *const map)
{
    if (mMap != map)
    {
        mMap = map;
        mMapEffects.clear();
        if (mMap == nullptr)
            return;

        const std::string name = mMap->getProperty("shortName");
        FOR_EACHP (STD_VECTOR<QuestEffect*>::const_iterator, it,  mAllEffects)
        {
            const QuestEffect *const effect = *it;
            if ((effect != nullptr) && name == effect->map)
                mMapEffects.push_back(effect);
        }
        updateEffects();
    }
}

void QuestsWindow::updateEffects()
{
    NpcQuestEffectMap oldNpc = mNpcEffects;
    mNpcEffects.clear();

    FOR_EACH (STD_VECTOR<const QuestEffect*>::const_iterator,
              it,  mMapEffects)
    {
        const QuestEffect *const effect = *it;
        if (effect != nullptr)
        {
            const NpcQuestVarMapCIter varIt = mVars->find(effect->var);
            if (varIt != mVars->end())
            {
                const std::set<int> &vals = effect->values;
                if (vals.find((*mVars)[effect->var].var1) != vals.end())
                    mNpcEffects[effect->id] = effect;
            }
        }
    }
    if (actorManager == nullptr)
        return;

    std::set<BeingTypeId> removeEffects;
    std::map<BeingTypeId, int> addEffects;

    // for old effects
    FOR_EACH (NpcQuestEffectMapCIter, it, oldNpc)
    {
        const BeingTypeId id = (*it).first;
        const QuestEffect *const effect = (*it).second;

        const NpcQuestEffectMapCIter itNew = mNpcEffects.find(id);
        if (itNew == mNpcEffects.end())
        {   // in new list no effect for this npc
            removeEffects.insert(id);
        }
        else
        {   // in new list exists effect for this npc
            const QuestEffect *const newEffect = (*itNew).second;
            if (effect != newEffect)
            {   // new effects is not equal to old effect
                addEffects[id] = newEffect->effectId;
                removeEffects.insert(id);
            }
        }
    }

    // for new effects
    FOR_EACH (NpcQuestEffectMapCIter, it, mNpcEffects)
    {
        const BeingTypeId id = (*it).first;
        const QuestEffect *const effect = (*it).second;

        const NpcQuestEffectMapCIter itNew = oldNpc.find(id);
        // check if old effect was not present
        if (itNew == oldNpc.end())
            addEffects[id] = effect->effectId;
    }
    if (!removeEffects.empty() || !addEffects.empty())
        actorManager->updateEffects(addEffects, removeEffects);
}

void QuestsWindow::addEffect(Being *const being)
{
    if (being == nullptr)
        return;
    const BeingTypeId id = being->getSubType();
    const std::map<BeingTypeId, const QuestEffect*>::const_iterator
        it = mNpcEffects.find(id);
    if (it != mNpcEffects.end())
    {
        const QuestEffect *const effect = (*it).second;
        if (effect != nullptr)
            being->addSpecialEffect(effect->effectId);
    }
}

void QuestsWindow::selectQuest(const int varId)
{
    std::map<int, int>::const_iterator it = mQuestReverseLinks.find(varId);
    if (it == mQuestReverseLinks.end())
        return;
    if (mVisible == Visible_false)
        setVisible(Visible_true);
    const int listPos = (*it).second;
    if (listPos < 0)
        return;
    showQuest(mQuestLinks[listPos]);
    mQuestsListBox->setSelected(listPos);
    requestMoveToTop();
}
