/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/tabs/setup_relations.h"

#include "actormanager.h"

#include "being/localplayer.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/guitable.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

static const int COLUMNS_NR = 2;  // name plus listbox
static const int NAME_COLUMN = 0;
static const unsigned int RELATION_CHOICE_COLUMN = 1;

static const unsigned int ROW_HEIGHT = 12;
// The following column widths really shouldn't be hardcoded
// but should scale with the size of the widget... except
// that, right now, the widget doesn't exactly scale either.
static const unsigned int NAME_COLUMN_WIDTH = 230;
static const unsigned int RELATION_CHOICE_COLUMN_WIDTH = 80;

#define WIDGET_AT(row, column) (((row) * COLUMNS_NR) + column)

#include "debug.h"

static const char *const table_titles[COLUMNS_NR] =
{
    // TRANSLATORS: relations table header
    N_("Name"),
    // TRANSLATORS: relations table header
    N_("Relation")
};

static const char *const RELATION_NAMES[PlayerRelation::RELATIONS_NR] =
{
    // TRANSLATORS: relation type
    N_("Neutral"),
    // TRANSLATORS: relation type
    N_("Friend"),
    // TRANSLATORS: relation type
    N_("Disregarded"),
    // TRANSLATORS: relation type
    N_("Ignored"),
    // TRANSLATORS: relation type
    N_("Erased"),
    // TRANSLATORS: relation type
    N_("Blacklisted"),
    // TRANSLATORS: relation type
    N_("Enemy")
};

class PlayerRelationListModel final : public gcn::ListModel
{
public:
    ~PlayerRelationListModel()
    { }

    int getNumberOfElements() override final
    {
        return PlayerRelation::RELATIONS_NR;
    }

    std::string getElementAt(int i) override final
    {
        if (i >= getNumberOfElements() || i < 0)
            return "";
        return gettext(RELATION_NAMES[i]);
    }
};

class PlayerTableModel final : public Widget2, public TableModel
{
public:
    explicit PlayerTableModel(const Widget2 *const widget) :
        Widget2(widget),
        TableModel(),
        mPlayers(nullptr),
        mWidgets(),
        mListModel(new PlayerRelationListModel)
    {
        playerRelationsUpdated();
    }

    A_DELETE_COPY(PlayerTableModel)

    ~PlayerTableModel()
    {
        freeWidgets();
        delete mListModel;
        mListModel = nullptr;
        delete mPlayers;
        mPlayers = nullptr;
    }

    int getRows() const override final
    {
        if (mPlayers)
            return static_cast<int>(mPlayers->size());
        else
            return 0;
    }

    int getColumns() const override final
    {
        return COLUMNS_NR;
    }

    int getRowHeight() const override final
    {
        return ROW_HEIGHT;
    }

    int getColumnWidth(const int index) const override final
    {
        if (index == NAME_COLUMN)
            return NAME_COLUMN_WIDTH;
        else
            return RELATION_CHOICE_COLUMN_WIDTH;
    }

    void playerRelationsUpdated()
    {
        signalBeforeUpdate();

        freeWidgets();
        StringVect *const player_names = player_relations.getPlayers();

        if (!player_names)
            return;

        delete mPlayers;
        mPlayers = player_names;

        // set up widgets
        for (unsigned int r = 0, sz = static_cast<unsigned int>(
             player_names->size()); r < sz; ++r)
        {
            const std::string name = (*player_names)[r];
            gcn::Widget *const widget = new Label(this, name);
            mWidgets.push_back(widget);

            DropDown *const choicebox = new DropDown(this, mListModel);
            choicebox->setSelected(player_relations.getRelation(name));
            mWidgets.push_back(choicebox);
        }

        signalAfterUpdate();
    }

    void updateModelInRow(const int row) const
    {
        const DropDown *const choicebox = static_cast<DropDown *>(
            getElementAt(row, RELATION_CHOICE_COLUMN));
        player_relations.setRelation(getPlayerAt(row),
            static_cast<PlayerRelation::Relation>(
            choicebox->getSelected()));
    }


    gcn::Widget *getElementAt(int row, int column) const override final
    {
        return mWidgets[WIDGET_AT(row, column)];
    }

    void freeWidgets()
    {
        delete mPlayers;
        mPlayers = nullptr;

        delete_all(mWidgets);
        mWidgets.clear();
    }

    std::string getPlayerAt(const int index) const
    {
        if (index < 0 || index >= static_cast<signed>(mPlayers->size()))
            return "";
        return (*mPlayers)[index];
    }

protected:
    StringVect *mPlayers;
    std::vector<gcn::Widget *> mWidgets;
    PlayerRelationListModel *mListModel;
};

/**
 * Class for choosing one of the various `what to do when ignoring a player' options
 */
class IgnoreChoicesListModel final : public gcn::ListModel
{
public:
    ~IgnoreChoicesListModel()
    { }

    int getNumberOfElements() override final
    {
        return static_cast<int>(player_relations.getPlayerIgnoreStrategies()
                                ->size());
    }

    std::string getElementAt(int i) override final
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";

        return (*player_relations.getPlayerIgnoreStrategies())
            [i]->mDescription;
    }
};

static const std::string ACTION_DELETE("delete");
static const std::string ACTION_TABLE("table");
static const std::string ACTION_STRATEGY("strategy");

Setup_Relations::Setup_Relations(const Widget2 *const widget) :
    SetupTab(widget),
    PlayerRelationsListener(),
    mPlayerTableTitleModel(new StaticTableModel(1, COLUMNS_NR)),
    mPlayerTableModel(new PlayerTableModel(this)),
    mPlayerTable(new GuiTable(this, mPlayerTableModel)),
    mPlayerTitleTable(new GuiTable(this, mPlayerTableTitleModel)),
    mPlayerScrollArea(new ScrollArea(mPlayerTable)),
    // TRANSLATORS: relation dialog button
    mDefaultTrading(new CheckBox(this, _("Allow trading"),
        player_relations.getDefault() & PlayerRelation::TRADE)),
    // TRANSLATORS: relation dialog button
    mDefaultWhisper(new CheckBox(this, _("Allow whispers"),
       player_relations.getDefault() & PlayerRelation::WHISPER)),
    mDeleteButton(new Button(this, _("Delete"), ACTION_DELETE, this)),
    mIgnoreActionChoicesModel(new IgnoreChoicesListModel),
    mIgnoreActionChoicesBox(new DropDown(widget, mIgnoreActionChoicesModel))
{
    // TRANSLATORS: relation dialog name
    setName(_("Relations"));

    mPlayerTable->setOpaque(false);

    mPlayerTableTitleModel->fixColumnWidth(NAME_COLUMN, NAME_COLUMN_WIDTH);
    mPlayerTableTitleModel->fixColumnWidth(RELATION_CHOICE_COLUMN,
                                           RELATION_CHOICE_COLUMN_WIDTH);
    mPlayerTitleTable->setBackgroundColor(getThemeColor(
        Theme::TABLE_BACKGROUND));
    mPlayerTitleTable->setSelectable(false);

    for (int i = 0; i < COLUMNS_NR; i++)
    {
        mPlayerTableTitleModel->set(0, i, new Label(
            this, gettext(table_titles[i])));
    }

    mPlayerTitleTable->setLinewiseSelection(true);

    mPlayerScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mPlayerTable->setActionEventId(ACTION_TABLE);
    mPlayerTable->setLinewiseSelection(true);
    mPlayerTable->addActionListener(this);

    // TRANSLATORS: relation dialog label
    Label *const ignore_action_label = new Label(this, _("When ignoring:"));

    mIgnoreActionChoicesBox->setActionEventId(ACTION_STRATEGY);
    mIgnoreActionChoicesBox->addActionListener(this);

    int ignore_strategy_index = 0;  // safe default

    if (player_relations.getPlayerIgnoreStrategy())
    {
        ignore_strategy_index = player_relations.getPlayerIgnoreStrategyIndex(
            player_relations.getPlayerIgnoreStrategy()->mShortName);
        if (ignore_strategy_index < 0)
            ignore_strategy_index = 0;
    }
    mIgnoreActionChoicesBox->setSelected(ignore_strategy_index);
    mIgnoreActionChoicesBox->adjustHeight();

    reset();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPlayerTitleTable, 6);
    place(0, 1, mPlayerScrollArea, 6, 4).setPadding(2);
    place(0, 5, mDeleteButton);
    place(3, 5, ignore_action_label, 1);
    place(4, 5, mIgnoreActionChoicesBox, 2).setPadding(2);
    place(3, 6, mDefaultTrading, 3);
    place(3, 7, mDefaultWhisper, 3);

    player_relations.addListener(this);

    setDimension(gcn::Rectangle(0, 0, 500, 350));
}

Setup_Relations::~Setup_Relations()
{
    player_relations.removeListener(this);
    delete mIgnoreActionChoicesModel;
    mIgnoreActionChoicesModel = nullptr;
}


void Setup_Relations::reset()
{
    // We now have to search through the list of ignore choices to find the
    // current selection. We could use an index into the table of config
    // options in player_relations instead of strategies to sidestep this.
    int selection = 0;
    for (size_t i = 0, sz = player_relations.getPlayerIgnoreStrategies()
         ->size(); i < sz; ++ i)
    {
        if ((*player_relations.getPlayerIgnoreStrategies())[i] ==
            player_relations.getPlayerIgnoreStrategy())
        {
            selection = static_cast<int>(i);
            break;
        }
    }
    mIgnoreActionChoicesBox->setSelected(selection);
}

void Setup_Relations::apply()
{
    player_relations.store();

    const unsigned int old_default_relations = player_relations.getDefault() &
        ~(PlayerRelation::TRADE | PlayerRelation::WHISPER);
    player_relations.setDefault(old_default_relations
        | (mDefaultTrading->isSelected() ? PlayerRelation::TRADE : 0)
        | (mDefaultWhisper->isSelected() ? PlayerRelation::WHISPER : 0));

    if (actorManager)
        actorManager->updatePlayerNames();

    if (player_node)
        player_node->setCheckNameSetting(true);
}

void Setup_Relations::cancel()
{
}

void Setup_Relations::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == ACTION_TABLE)
    {
        // temporarily eliminate ourselves: we are fully aware of this change,
        // so there is no need for asynchronous updates.  (In fact, thouse
        // might destroy the widet that triggered them, which would be rather
        // embarrassing.)
        player_relations.removeListener(this);

        const int row = mPlayerTable->getSelectedRow();
        if (row >= 0)
            mPlayerTableModel->updateModelInRow(row);

        player_relations.addListener(this);
    }
    else if (eventId == ACTION_DELETE)
    {
        const int player_index = mPlayerTable->getSelectedRow();

        if (player_index < 0)
            return;

        player_relations.removePlayer(mPlayerTableModel->getPlayerAt(
            player_index));
    }
    else if (eventId == ACTION_STRATEGY)
    {
        PlayerIgnoreStrategy *const s =
            (*player_relations.getPlayerIgnoreStrategies())[
            mIgnoreActionChoicesBox->getSelected()];

        player_relations.setPlayerIgnoreStrategy(s);
    }
}

void Setup_Relations::updatedPlayer(const std::string &name A_UNUSED)
{
    mPlayerTableModel->playerRelationsUpdated();
    mDefaultTrading->setSelected(
            player_relations.getDefault() & PlayerRelation::TRADE);
    mDefaultWhisper->setSelected(
            player_relations.getDefault() & PlayerRelation::WHISPER);
    if (player_node)
        player_node->updateName();
}

void Setup_Relations::updateAll()
{
    PlayerTableModel *const model = new PlayerTableModel(this);
    mPlayerTable->setModel(model);
    delete mPlayerTableModel;
    mPlayerTableModel = model;
    int ignore_strategy_index = 0;  // safe default

    if (player_relations.getPlayerIgnoreStrategy())
    {
        ignore_strategy_index = player_relations.getPlayerIgnoreStrategyIndex(
                player_relations.getPlayerIgnoreStrategy()->mShortName);
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
        player_relations.getDefault() & PlayerRelation::TRADE);
    mDefaultWhisper->setSelected(
        player_relations.getDefault() & PlayerRelation::WHISPER);
}
