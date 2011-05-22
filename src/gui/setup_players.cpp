/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/setup_players.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "localplayer.h"
#include "log.h"

#include "gui/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/table.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include <string>
#include <vector>

#define COLUMNS_NR 2 // name plus listbox
#define NAME_COLUMN 0
#define RELATION_CHOICE_COLUMN 1

#define ROW_HEIGHT 12
// The following column widths really shouldn't be hardcoded but should scale with the size of the widget... except
// that, right now, the widget doesn't exactly scale either.
#define NAME_COLUMN_WIDTH 230
#define RELATION_CHOICE_COLUMN_WIDTH 80

#define WIDGET_AT(row, column) (((row) * COLUMNS_NR) + column)

static const char *table_titles[COLUMNS_NR] =
{
    N_("Name"),
    N_("Relation")
};

static const char *RELATION_NAMES[PlayerRelation::RELATIONS_NR] =
{
    N_("Neutral"),
    N_("Friend"),
    N_("Disregarded"),
    N_("Ignored"),
    N_("Erased")
};

class PlayerRelationListModel : public gcn::ListModel
{
public:
    virtual ~PlayerRelationListModel() { }

    virtual int getNumberOfElements()
    {
        return PlayerRelation::RELATIONS_NR;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return "";
        return gettext(RELATION_NAMES[i]);
    }
};

class PlayerTableModel : public TableModel
{
public:
    PlayerTableModel() :
        mPlayers(NULL),
        mListModel(new PlayerRelationListModel)
    {
        playerRelationsUpdated();
    }

    virtual ~PlayerTableModel()
    {
        freeWidgets();
        delete mListModel;
        mListModel = 0;
        delete mPlayers;
        mPlayers = 0;
    }

    virtual int getRows() const
    {
        if (mPlayers)
            return static_cast<int>(mPlayers->size());
        else
            return 0;
    }

    virtual int getColumns() const
    {
        return COLUMNS_NR;
    }

    virtual int getRowHeight() const
    {
        return ROW_HEIGHT;
    }

    virtual int getColumnWidth(int index) const
    {
        if (index == NAME_COLUMN)
            return NAME_COLUMN_WIDTH;
        else
            return RELATION_CHOICE_COLUMN_WIDTH;
    }

    virtual void playerRelationsUpdated()
    {
        signalBeforeUpdate();

        freeWidgets();
        std::vector<std::string> *player_names = player_relations.getPlayers();

        if (!player_names)
            return;

        delete mPlayers;
        mPlayers = player_names;

        // set up widgets
        for (unsigned int r = 0; r < player_names->size(); ++r)
        {
            std::string name = (*player_names)[r];
            gcn::Widget *widget = new Label(name);
            mWidgets.push_back(widget);

            gcn::DropDown *choicebox = new DropDown(mListModel);
            choicebox->setSelected(player_relations.getRelation(name));
            mWidgets.push_back(choicebox);
        }

        signalAfterUpdate();
    }

    virtual void updateModelInRow(int row)
    {
        gcn::DropDown *choicebox = static_cast<gcn::DropDown *>(
                                   getElementAt(row, RELATION_CHOICE_COLUMN));
        player_relations.setRelation(getPlayerAt(row),
                                   static_cast<PlayerRelation::Relation>(
                                   choicebox->getSelected()));
    }


    virtual gcn::Widget *getElementAt(int row, int column) const
    {
        return mWidgets[WIDGET_AT(row, column)];
    }

    virtual void freeWidgets()
    {
        delete mPlayers;
        mPlayers = 0;

        delete_all(mWidgets);
        mWidgets.clear();
    }

    std::string getPlayerAt(int index) const
    {
        return (*mPlayers)[index];
    }

protected:
    std::vector<std::string> *mPlayers;
    std::vector<gcn::Widget *> mWidgets;
    PlayerRelationListModel *mListModel;
};

/**
 * Class for choosing one of the various `what to do when ignoring a player' options
 */
class IgnoreChoicesListModel : public gcn::ListModel
{
public:
    virtual ~IgnoreChoicesListModel() { }

    virtual int getNumberOfElements()
    {
        return static_cast<int>(player_relations.getPlayerIgnoreStrategies()
                                ->size());
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return (*player_relations.getPlayerIgnoreStrategies())
            [i]->mDescription;
    }
};

#define ACTION_DELETE "delete"
#define ACTION_OLD "old"
#define ACTION_TABLE "table"
#define ACTION_STRATEGY "strategy"
#define ACTION_WHISPER_TAB "whisper tab"
#define ACTION_SHOW_GENDER "show gender"
#define ACTION_SHOW_LEVEL "show level"
#define ACTION_TARGET_DEAD "target dead"
#define ACTION_SHOW_OWN_NAME "show own name"
#define ACTION_SECURE_TRADES "secure trades"

Setup_Players::Setup_Players():
    mPlayerTableTitleModel(new StaticTableModel(1, COLUMNS_NR)),
    mPlayerTableModel(new PlayerTableModel),
    mPlayerTable(new GuiTable(mPlayerTableModel)),
    mPlayerTitleTable(new GuiTable(mPlayerTableTitleModel)),
    mPlayerScrollArea(new ScrollArea(mPlayerTable)),
    mDefaultTrading(new CheckBox(_("Allow trading"),
                player_relations.getDefault() & PlayerRelation::TRADE)),
    mDefaultWhisper(new CheckBox(_("Allow whispers"),
                player_relations.getDefault() & PlayerRelation::WHISPER)),
    mDeleteButton(new Button(_("Delete"), ACTION_DELETE, this)),
    mOldButton(new Button(_("Old"), ACTION_OLD, this)),
    mWhisperTab(config.getBoolValue("whispertab")),
    mWhisperTabCheckBox(new CheckBox(_("Put all whispers in tabs"),
                        mWhisperTab)),
    mShowGender(config.getBoolValue("showgender")),
    mShowGenderCheckBox(new CheckBox(_("Show gender"), mShowGender)),
    mShowLevel(config.getBoolValue("showlevel")),
    mShowOwnName(config.getBoolValue("showownname")),
    mTargetDead(config.getBoolValue("targetDeadPlayers")),
    mSecureTrades(config.getBoolValue("securetrades"))
{
    setName(_("Players"));

    mPlayerTable->setOpaque(false);

    mPlayerTableTitleModel->fixColumnWidth(NAME_COLUMN, NAME_COLUMN_WIDTH);
    mPlayerTableTitleModel->fixColumnWidth(RELATION_CHOICE_COLUMN,
                                           RELATION_CHOICE_COLUMN_WIDTH);
    mPlayerTitleTable->setBackgroundColor(gcn::Color(0xbf, 0xbf, 0xbf));

    mIgnoreActionChoicesModel = new IgnoreChoicesListModel;
    mIgnoreActionChoicesBox = new DropDown(mIgnoreActionChoicesModel);

    for (int i = 0; i < COLUMNS_NR; i++)
    {
        mPlayerTableTitleModel->set(0, i,
                new Label(gettext(table_titles[i])));
    }

    mPlayerTitleTable->setLinewiseSelection(true);

    mPlayerScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mPlayerTable->setActionEventId(ACTION_TABLE);
    mPlayerTable->setLinewiseSelection(true);
    mPlayerTable->addActionListener(this);

    gcn::Label *ignore_action_label = new Label(_("When ignoring:"));

    mIgnoreActionChoicesBox->setActionEventId(ACTION_STRATEGY);
    mIgnoreActionChoicesBox->addActionListener(this);

    int ignore_strategy_index = 0; // safe default

    if (player_relations.getPlayerIgnoreStrategy())
    {
        ignore_strategy_index = player_relations.getPlayerIgnoreStrategyIndex(
            player_relations.getPlayerIgnoreStrategy()->mShortName);
        if (ignore_strategy_index < 0)
            ignore_strategy_index = 0;
    }
    mIgnoreActionChoicesBox->setSelected(ignore_strategy_index);
    mIgnoreActionChoicesBox->adjustHeight();

    mWhisperTabCheckBox->setActionEventId(ACTION_WHISPER_TAB);
    mWhisperTabCheckBox->addActionListener(this);

    mShowGenderCheckBox->setActionEventId(ACTION_SHOW_GENDER);
    mShowGenderCheckBox->addActionListener(this);

    mShowLevelCheckBox = new CheckBox(_("Show level"), mShowLevel);
    mShowLevelCheckBox->setActionEventId(ACTION_SHOW_LEVEL);
    mShowLevelCheckBox->addActionListener(this);

    mShowOwnNameCheckBox = new CheckBox(_("Show own name"), mShowOwnName);
    mShowOwnNameCheckBox->setActionEventId(ACTION_SHOW_OWN_NAME);
    mShowOwnNameCheckBox->addActionListener(this);

    mTargetDeadCheckBox = new CheckBox(_("Target dead players"), mTargetDead);
    mTargetDeadCheckBox->setActionEventId(ACTION_TARGET_DEAD);
    mTargetDeadCheckBox->addActionListener(this);

    mSecureTradesCheckBox = new CheckBox(_("Secure trades"), mSecureTrades);
    mSecureTradesCheckBox->setActionEventId(ACTION_SECURE_TRADES);
    mSecureTradesCheckBox->addActionListener(this);

    reset();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPlayerTitleTable, 5);
    place(0, 1, mPlayerScrollArea, 5, 4).setPadding(2);
    place(0, 5, mDeleteButton);
    place(0, 6, mShowGenderCheckBox, 3).setPadding(2);
    place(0, 7, mShowLevelCheckBox, 3).setPadding(2);
    place(0, 8, mShowOwnNameCheckBox, 3).setPadding(2);
    place(1, 5, mOldButton, 1);
    place(3, 5, ignore_action_label);
    place(3, 6, mIgnoreActionChoicesBox, 2).setPadding(2);
    place(3, 7, mDefaultTrading, 2);
    place(3, 8, mDefaultWhisper, 2);
    place(3, 9, mSecureTradesCheckBox, 2);
    place(0, 9, mWhisperTabCheckBox, 4).setPadding(4);
    place(0, 10, mTargetDeadCheckBox, 4).setPadding(4);

    player_relations.addListener(this);

    setDimension(gcn::Rectangle(0, 0, 500, 350));
}

Setup_Players::~Setup_Players()
{
    player_relations.removeListener(this);
    delete mIgnoreActionChoicesModel;
    mIgnoreActionChoicesModel = 0;
}


void Setup_Players::reset()
{
    // We now have to search through the list of ignore choices to find the
    // current selection. We could use an index into the table of config
    // options in player_relations instead of strategies to sidestep this.
    int selection = 0;
    for (unsigned int i = 0;
                      i < player_relations.getPlayerIgnoreStrategies()->size();
                      ++i)
        if ((*player_relations.getPlayerIgnoreStrategies())[i] ==
            player_relations.getPlayerIgnoreStrategy())
        {

            selection = i;
            break;
        }

    mIgnoreActionChoicesBox->setSelected(selection);
}

void Setup_Players::apply()
{
    player_relations.store();

    unsigned int old_default_relations = player_relations.getDefault() &
                                         ~(PlayerRelation::TRADE |
                                           PlayerRelation::WHISPER);
    player_relations.setDefault(old_default_relations
                                | (mDefaultTrading->isSelected() ?
                                       PlayerRelation::TRADE : 0)
                                | (mDefaultWhisper->isSelected() ?
                                       PlayerRelation::WHISPER : 0));
    config.setValue("whispertab", mWhisperTab);
    config.setValue("showlevel", mShowLevel);
    config.setValue("showownname", mShowOwnName);
    config.setValue("targetDeadPlayers", mTargetDead);
    config.setValue("showgender", mShowGender);
    config.setValue("securetrades", mSecureTrades);

    if (actorSpriteManager)
        actorSpriteManager->updatePlayerNames();

    if (player_node)
        player_node->setCheckNameSetting(true);
}

void Setup_Players::cancel()
{
    mWhisperTab = config.getBoolValue("whispertab");
    mWhisperTabCheckBox->setSelected(mWhisperTab);
    mShowGender = config.getBoolValue("showgender");
    mShowGenderCheckBox->setSelected(mShowGender);
    mShowLevel = config.getBoolValue("showlevel");
    mShowLevelCheckBox->setSelected(mShowLevel);
    mShowOwnName = config.getBoolValue("showownname");
    mShowOwnNameCheckBox->setSelected(mShowOwnName);
    mTargetDead = config.getBoolValue("targetDeadPlayers");
    mTargetDeadCheckBox->setSelected(mTargetDead);
    mSecureTrades = config.getBoolValue("securetrades");
    mSecureTradesCheckBox->setSelected(mSecureTrades);
}

void Setup_Players::action(const gcn::ActionEvent &event)
{
    if (event.getId() == ACTION_TABLE)
    {
        // temporarily eliminate ourselves: we are fully aware of this change,
        // so there is no need for asynchronous updates.  (In fact, thouse
        // might destroy the widet that triggered them, which would be rather
        // embarrassing.)
        player_relations.removeListener(this);

        int row = mPlayerTable->getSelectedRow();
        if (row >= 0)
            mPlayerTableModel->updateModelInRow(row);

        player_relations.addListener(this);

    }
    else if (event.getId() == ACTION_DELETE)
    {
        int player_index = mPlayerTable->getSelectedRow();

        if (player_index < 0)
            return;

        std::string name = mPlayerTableModel->getPlayerAt(player_index);

        player_relations.removePlayer(name);
    }
    else if (event.getId() == ACTION_OLD)
    {
        player_relations.load(true);
        updateAll();
    }
    else if (event.getId() == ACTION_STRATEGY)
    {
        PlayerIgnoreStrategy *s =
            (*player_relations.getPlayerIgnoreStrategies())[
                mIgnoreActionChoicesBox->getSelected()];

        player_relations.setPlayerIgnoreStrategy(s);
    }
    else if (event.getId() == ACTION_WHISPER_TAB)
    {
        mWhisperTab = mWhisperTabCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_GENDER)
    {
        mShowGender = mShowGenderCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_LEVEL)
    {
        mShowLevel = mShowLevelCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_OWN_NAME)
    {
        mShowOwnName = mShowOwnNameCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_TARGET_DEAD)
    {
        mTargetDead = mTargetDeadCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SECURE_TRADES)
    {
        mSecureTrades = mSecureTradesCheckBox->isSelected();
    }
}


void Setup_Players::updatedPlayer(const std::string &name _UNUSED_)
{
    mPlayerTableModel->playerRelationsUpdated();
    mDefaultTrading->setSelected(
            player_relations.getDefault() & PlayerRelation::TRADE);
    mDefaultWhisper->setSelected(
            player_relations.getDefault() & PlayerRelation::WHISPER);
    if (player_node)
        player_node->updateName();
}

void Setup_Players::updateAll()
{
    PlayerTableModel *model = new PlayerTableModel();
    mPlayerTable->setModel(model);
    delete mPlayerTableModel;
    mPlayerTableModel = model;
    int ignore_strategy_index = 0; // safe default

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
void Setup_Players::externalUpdated()
{
    mDefaultTrading->setSelected(
        player_relations.getDefault() & PlayerRelation::TRADE);
    mDefaultWhisper->setSelected(
        player_relations.getDefault() & PlayerRelation::WHISPER);
}