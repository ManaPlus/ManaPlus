/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_relations.h"

#include "actormanager.h"

#include "being/localplayer.h"

#include "gui/models/ignorechoiceslistmodel.h"
#include "gui/models/playertablemodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/guitable.h"

#include "utils/delete2.h"

#include "debug.h"

static const int COLUMNS_NR = 2;  // name plus listbox
static const int NAME_COLUMN = 0;
static const unsigned int NAME_COLUMN_WIDTH = 230;
static const unsigned int RELATION_CHOICE_COLUMN = 1;
static const unsigned int RELATION_CHOICE_COLUMN_WIDTH = 80;

static const std::string ACTION_DELETE("delete");
static const std::string ACTION_TABLE("table");
static const std::string ACTION_STRATEGY("strategy");

static const char *const table_titles[COLUMNS_NR] =
{
    // TRANSLATORS: relations table header
    N_("Name"),
    // TRANSLATORS: relations table header
    N_("Relation")
};

Setup_Relations::Setup_Relations(const Widget2 *const widget) :
    SetupTab(widget),
    PlayerRelationsListener(),
    mPlayerTableTitleModel(new StaticTableModel(1, COLUMNS_NR)),
    mPlayerTableModel(new PlayerTableModel(this)),
    mPlayerTable(new GuiTable(this, mPlayerTableModel, Opaque_true)),
    mPlayerTitleTable(new GuiTable(this, mPlayerTableTitleModel, Opaque_true)),
    mPlayerScrollArea(new ScrollArea(this,
        mPlayerTable, Opaque_true, std::string())),
    // TRANSLATORS: relation dialog button
    mDefaultTrading(new CheckBox(this, _("Allow trading"),
        (playerRelations.getDefault() & PlayerRelation::TRADE) != 0U,
        nullptr, std::string())),
    // TRANSLATORS: relation dialog button
    mDefaultWhisper(new CheckBox(this, _("Allow whispers"),
        (playerRelations.getDefault() & PlayerRelation::WHISPER) != 0U,
        nullptr, std::string())),
    // TRANSLATORS: relation dialog button
    mDeleteButton(new Button(this, _("Delete"), ACTION_DELETE,
        BUTTON_SKIN, this)),
    mIgnoreActionChoicesModel(new IgnoreChoicesListModel),
    mIgnoreActionChoicesBox(new DropDown(widget, mIgnoreActionChoicesModel,
        false, Modal_false, nullptr, std::string()))
{
    // TRANSLATORS: relation dialog name
    setName(_("Relations"));

    mPlayerTable->setOpaque(Opaque_false);

    mPlayerTableTitleModel->fixColumnWidth(NAME_COLUMN, NAME_COLUMN_WIDTH);
    mPlayerTableTitleModel->fixColumnWidth(RELATION_CHOICE_COLUMN,
                                           RELATION_CHOICE_COLUMN_WIDTH);
    mPlayerTitleTable->setBackgroundColor(getThemeColor(
        ThemeColorId::TABLE_BACKGROUND, 255U));
    mPlayerTitleTable->setSelectableGui(false);

    for (int i = 0; i < COLUMNS_NR; i++)
    {
        mPlayerTableTitleModel->set(0, i, new Label(
            this, gettext(table_titles[i])));
    }

    mPlayerTitleTable->setLinewiseSelection(true);

    mPlayerScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mPlayerTable->setActionEventId(ACTION_TABLE);
    mPlayerTable->setLinewiseSelection(true);
    mPlayerTable->addActionListener(this);

    // TRANSLATORS: relation dialog label
    Label *const ignore_action_label = new Label(this, _("When ignoring:"));

    mIgnoreActionChoicesBox->setActionEventId(ACTION_STRATEGY);
    mIgnoreActionChoicesBox->addActionListener(this);

    int ignore_strategy_index = 0;  // safe default

    if (playerRelations.getPlayerIgnoreStrategy() != nullptr)
    {
        ignore_strategy_index = playerRelations.getPlayerIgnoreStrategyIndex(
            playerRelations.getPlayerIgnoreStrategy()->mShortName);
        if (ignore_strategy_index < 0)
            ignore_strategy_index = 0;
    }
    mIgnoreActionChoicesBox->setSelected(ignore_strategy_index);
    mIgnoreActionChoicesBox->adjustHeight();

    reset();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPlayerTitleTable, 6, 1);
    place(0, 1, mPlayerScrollArea, 6, 4).setPadding(2);
    place(0, 5, mDeleteButton, 1, 1);
    place(3, 5, ignore_action_label, 1, 1);
    place(4, 5, mIgnoreActionChoicesBox, 2, 1).setPadding(2);
    place(3, 6, mDefaultTrading, 3, 1);
    place(3, 7, mDefaultWhisper, 3, 1);

    playerRelations.addListener(this);

    setDimension(Rect(0, 0, 500, 350));
}

Setup_Relations::~Setup_Relations()
{
    playerRelations.removeListener(this);
    delete2(mIgnoreActionChoicesModel)
}


void Setup_Relations::reset()
{
    // We now have to search through the list of ignore choices to find the
    // current selection. We could use an index into the table of config
    // options in playerRelations instead of strategies to sidestep this.
    int selection = 0;
    for (size_t i = 0, sz = playerRelations.getPlayerIgnoreStrategies()
         ->size(); i < sz; ++ i)
    {
        if ((*playerRelations.getPlayerIgnoreStrategies())[i] ==
            playerRelations.getPlayerIgnoreStrategy())
        {
            selection = CAST_S32(i);
            break;
        }
    }
    mIgnoreActionChoicesBox->setSelected(selection);
}

void Setup_Relations::apply()
{
    playerRelations.store();

    const unsigned int old_default_relations = playerRelations.getDefault() &
        ~(PlayerRelation::TRADE | PlayerRelation::WHISPER);
    playerRelations.setDefault(old_default_relations
        | (mDefaultTrading->isSelected() ? PlayerRelation::TRADE : 0)
        | (mDefaultWhisper->isSelected() ? PlayerRelation::WHISPER : 0));

    if (actorManager != nullptr)
        actorManager->updatePlayerNames();

    if (localPlayer != nullptr)
        localPlayer->setCheckNameSetting(true);
}

void Setup_Relations::cancel()
{
}

void Setup_Relations::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == ACTION_TABLE)
    {
        // temporarily eliminate ourselves: we are fully aware of this change,
        // so there is no need for asynchronous updates.  (In fact, thouse
        // might destroy the widet that triggered them, which would be rather
        // embarrassing.)
        playerRelations.removeListener(this);

        const int row = mPlayerTable->getSelectedRow();
        if (row >= 0)
            mPlayerTableModel->updateModelInRow(row);

        playerRelations.addListener(this);
    }
    else if (eventId == ACTION_DELETE)
    {
        const int player_index = mPlayerTable->getSelectedRow();

        if (player_index < 0)
            return;

        playerRelations.removePlayer(mPlayerTableModel->getPlayerAt(
            player_index));
    }
    else if (eventId == ACTION_STRATEGY)
    {
        const int sel = mIgnoreActionChoicesBox->getSelected();
        if (sel < 0)
            return;
        PlayerIgnoreStrategy *const s =
            (*playerRelations.getPlayerIgnoreStrategies())[sel];

        playerRelations.setPlayerIgnoreStrategy(s);
    }
}

void Setup_Relations::updatedPlayer(const std::string &name A_UNUSED)
{
    mPlayerTableModel->playerRelationsUpdated();
    mDefaultTrading->setSelected(
            (playerRelations.getDefault() & PlayerRelation::TRADE) != 0U);
    mDefaultWhisper->setSelected(
            (playerRelations.getDefault() & PlayerRelation::WHISPER) != 0U);
    if (localPlayer != nullptr)
        localPlayer->updateName();
}

void Setup_Relations::updateAll()
{
    PlayerTableModel *const model = new PlayerTableModel(this);
    mPlayerTable->setModel(model);
    delete mPlayerTableModel;
    mPlayerTableModel = model;
    int ignore_strategy_index = 0;  // safe default

    if (playerRelations.getPlayerIgnoreStrategy() != nullptr)
    {
        ignore_strategy_index = playerRelations.getPlayerIgnoreStrategyIndex(
                playerRelations.getPlayerIgnoreStrategy()->mShortName);
        if (ignore_strategy_index < 0)
            ignore_strategy_index = 0;
    }
    mIgnoreActionChoicesBox->setSelected(ignore_strategy_index);
    mIgnoreActionChoicesBox->adjustHeight();
    reset();
}
void Setup_Relations::externalUpdated()
{
    mDefaultTrading->setSelected(
        (playerRelations.getDefault() & PlayerRelation::TRADE) != 0U);
    mDefaultWhisper->setSelected(
        (playerRelations.getDefault() & PlayerRelation::WHISPER) != 0U);
}
