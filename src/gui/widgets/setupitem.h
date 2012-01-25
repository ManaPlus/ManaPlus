/*
 *  The ManaPlus Client
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

#ifndef SETUPITEM_H
#define SETUPITEM_H

#include "gui/widgets/tabbedarea.h"

#include "guichanfwd.h"

#include "gui/widgets/setuptabscroll.h"
#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/widget.hpp>

#include <list>

class CheckBox;
class Configuration;
class ContainerPlacer;
class DropDown;
class EditDialog;
class HorizontContainer;
class IntTextField;
class Label;
class TextField;

class SetupItem : public gcn::ActionListener
{
    public:
        enum
        {
            VBOOL = 0,
            VSTR,
            VINT,
            VNONE
        };

        SetupItem(std::string text, std::string description,
                  std::string keyName, SetupTabScroll *parent,
                  std::string eventName, bool mainConfig);

        SetupItem(std::string text, std::string description,
                  std::string keyName, SetupTabScroll *parent,
                  std::string eventName, std::string def, bool mainConfig);

        ~SetupItem();

        void load();

        void save();

        virtual void fromWidget() = 0;

        virtual void toWidget() = 0;

        void setWidget(gcn::Widget *widget)
        { mWidget = widget; }

        gcn::Widget *getWidget()
        { return mWidget; }

        Configuration *getConfig();

        virtual std::string getActionEventId();

        virtual void action(const gcn::ActionEvent &event);

        virtual void action();

        virtual void apply(std::string eventName);

        virtual void cancel(std::string eventName);

        virtual void externalUpdated(std::string eventName);
//        virtual int add(ContainerPlacer &place, int x, int y, int width);

        bool isMainConfig()
        { return mMainConfig; }

    protected:
        std::string mText;

        std::string mDescription;

        std::string mKeyName;

        SetupTabScroll *mParent;

        std::string mEventName;

        bool mMainConfig;

        bool mUseDefault;

        std::string mValue;

        std::string mDefault;

        gcn::Widget *mWidget;

        std::list<gcn::Widget*> mTempWidgets;

        int mValueType;
};

class SetupItemCheckBox : public SetupItem
{
    public:
        SetupItemCheckBox(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *parent,
                          std::string eventName, bool mainConfig = true);

        SetupItemCheckBox(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *parent,
                          std::string eventName, std::string def,
                          bool mainConfig = true);

        ~SetupItemCheckBox();

        void createControls();

        void fromWidget();

        void toWidget();

    protected:
        CheckBox *mCheckBox;
};

class SetupItemTextField : public SetupItem
{
    public:
        SetupItemTextField(std::string text, std::string description,
                           std::string keyName, SetupTabScroll *parent,
                           std::string eventName, bool mainConfig = true);

        SetupItemTextField(std::string text, std::string description,
                           std::string keyName, SetupTabScroll *parent,
                           std::string eventName, std::string def,
                           bool mainConfig = true);

        ~SetupItemTextField();

        void createControls();

        void fromWidget();

        void toWidget();

        void action(const gcn::ActionEvent &event);

        void apply(std::string eventName);

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        TextField *mTextField;
        Button *mButton;
        EditDialog *mEditDialog;
};

class SetupItemIntTextField : public SetupItem
{
    public:
        SetupItemIntTextField(std::string text, std::string description,
                              std::string keyName, SetupTabScroll *parent,
                              std::string eventName, int min, int max,
                              bool mainConfig = true);

        SetupItemIntTextField(std::string text, std::string description,
                              std::string keyName, SetupTabScroll *parent,
                              std::string eventName, int min, int max,
                              std::string def, bool mainConfig = true);

        ~SetupItemIntTextField();

        void createControls();

        void fromWidget();

        void toWidget();

        void action(const gcn::ActionEvent &event);

        void apply(std::string eventName);

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        IntTextField *mTextField;
        Button *mButton;
        int mMin;
        int mMax;
        EditDialog *mEditDialog;
};

class SetupItemLabel : public SetupItem
{
    public:
        SetupItemLabel(std::string text, std::string description,
                       SetupTabScroll *parent, bool separator = true);

        ~SetupItemLabel();

        void createControls();

        void fromWidget();

        void toWidget();

        void action(const gcn::ActionEvent &event);

        void apply(std::string eventName);

    protected:
        Label *mLabel;
        bool mIsSeparator;
};

class SetupItemDropDown : public SetupItem
{
    public:
        SetupItemDropDown(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *parent,
                          std::string eventName, gcn::ListModel *model,
                          bool mainConfig = true);

        SetupItemDropDown(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *parent,
                          std::string eventName, gcn::ListModel *model,
                          std::string def, bool mainConfig = true);

        ~SetupItemDropDown();

        void createControls();

        void fromWidget();

        void toWidget();

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        gcn::ListModel *mModel;
        DropDown *mDropDown;
};

#endif
