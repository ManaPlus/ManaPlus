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
class Slider;
class SliderList;
class TextField;

namespace gcn
{
    class ListModel;
}

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
                  std::string keyName, SetupTabScroll *const parent,
                  std::string eventName, const bool mainConfig);

        SetupItem(std::string text, std::string description,
                  std::string keyName, SetupTabScroll *const parent,
                  std::string eventName, std::string def,
                  const bool mainConfig);

        A_DELETE_COPY(SetupItem);

        ~SetupItem();

        void load();

        void save() const;

        virtual void fromWidget() = 0;

        virtual void toWidget() = 0;

        void setWidget(gcn::Widget *widget)
        { mWidget = widget; }

        gcn::Widget *getWidget() const
        { return mWidget; }

        Configuration *getConfig() const;

        virtual std::string getActionEventId();

        virtual void action(const gcn::ActionEvent &event) override;

        virtual void action();

        virtual void apply(std::string eventName);

        virtual void cancel(std::string eventName);

        virtual void externalUpdated(std::string eventName);

        bool isMainConfig() const
        { return mMainConfig; }

        void fixFirstItemSize(gcn::Widget *const widget);

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

class SetupItemCheckBox final : public SetupItem
{
    public:
        SetupItemCheckBox(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *const parent,
                          std::string eventName, const bool mainConfig = true);

        SetupItemCheckBox(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *const parent,
                          std::string eventName, std::string def,
                          const bool mainConfig = true);

        A_DELETE_COPY(SetupItemCheckBox);

        ~SetupItemCheckBox();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

    protected:
        CheckBox *mCheckBox;
};

class SetupItemTextField final : public SetupItem
{
    public:
        SetupItemTextField(std::string text, std::string description,
                           std::string keyName, SetupTabScroll *const parent,
                           std::string eventName,
                           const bool mainConfig = true);

        SetupItemTextField(std::string text, std::string description,
                           std::string keyName, SetupTabScroll *const parent,
                           std::string eventName, std::string def,
                           const bool mainConfig = true);

        A_DELETE_COPY(SetupItemTextField);

        ~SetupItemTextField();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName) override;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        TextField *mTextField;
        Button *mButton;
        EditDialog *mEditDialog;
};

class SetupItemIntTextField final : public SetupItem
{
    public:
        SetupItemIntTextField(std::string text, std::string description,
                              std::string keyName,
                              SetupTabScroll *const parent,
                              std::string eventName,
                              const int min, const int max,
                              const bool mainConfig = true);

        SetupItemIntTextField(std::string text, std::string description,
                              std::string keyName,
                              SetupTabScroll *const parent,
                              std::string eventName,
                              const int min, const int max,
                              std::string def, const bool mainConfig = true);

        A_DELETE_COPY(SetupItemIntTextField);

        ~SetupItemIntTextField();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName) override;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        IntTextField *mTextField;
        Button *mButton;
        int mMin;
        int mMax;
        EditDialog *mEditDialog;
};

class SetupItemLabel final : public SetupItem
{
    public:
        SetupItemLabel(std::string text, std::string description,
                       SetupTabScroll *const parent,
                       const bool separator = true);

        A_DELETE_COPY(SetupItemLabel);

        ~SetupItemLabel();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName) override;

    protected:
        Label *mLabel;
        bool mIsSeparator;
};

class SetupItemDropDown final : public SetupItem
{
    public:
        SetupItemDropDown(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *const parent,
                          std::string eventName, gcn::ListModel *const model,
                          const bool mainConfig = true);

        SetupItemDropDown(std::string text, std::string description,
                          std::string keyName, SetupTabScroll *const parent,
                          std::string eventName, gcn::ListModel *const model,
                          std::string def, const bool mainConfig = true);

        A_DELETE_COPY(SetupItemDropDown);

        ~SetupItemDropDown();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        gcn::ListModel *mModel;
        DropDown *mDropDown;
};

class SetupItemSlider final : public SetupItem
{
    public:
        SetupItemSlider(std::string text, std::string description,
                        std::string keyName, SetupTabScroll *const parent,
                        std::string eventName,
                        const double min, const double max,
                        const int width = 150, const bool onTheFly = false,
                        const bool mainConfig = true);

        SetupItemSlider(std::string text, std::string description,
                        std::string keyName, SetupTabScroll *const parent,
                        std::string eventName,
                        const double min, const double max,
                        std::string def, const int width = 150,
                        const bool onTheFly = false,
                        const bool mainConfig = true);

        A_DELETE_COPY(SetupItemSlider);

        ~SetupItemSlider();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName);

        void updateLabel();

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        Slider *mSlider;
        double mMin;
        double mMax;
        int mWidth;
        bool mOnTheFly;
};

typedef std::vector<std::string> SetupItemNames;
typedef SetupItemNames::iterator SetupItemNamesIter;
typedef SetupItemNames::const_iterator SetupItemNamesConstIter;

class SetupItemSlider2 final : public SetupItem
{
    public:
        SetupItemSlider2(std::string text, std::string description,
                         std::string keyName, SetupTabScroll *const parent,
                         std::string eventName, const int min, const int max,
                         SetupItemNames *const values,
                         const bool onTheFly = false,
                         const bool mainConfig = true,
                         const bool doNotAlign = false);

        SetupItemSlider2(std::string text, std::string description,
                         std::string keyName, SetupTabScroll *const parent,
                         std::string eventName, const int min, const int max,
                         SetupItemNames *const values, std::string def,
                         const bool onTheFly = false,
                         const bool mainConfig = true,
                         const bool doNotAlign = false);

        A_DELETE_COPY(SetupItemSlider2);

        ~SetupItemSlider2();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName) override;

        void setInvertValue(const int v);

    protected:
        void updateLabel();

        int getMaxWidth();

        HorizontContainer *mHorizont;
        Label *mLabel;
        Label *mLabel2;
        Slider *mSlider;
        SetupItemNames *mValues;
        int mMin;
        int mMax;
        bool mInvert;
        int mInvertValue;
        bool mOnTheFly;
        bool mDoNotAlign;
};

class SetupItemSliderList : public SetupItem
{
    public:
        SetupItemSliderList(std::string text, std::string description,
                            std::string keyName, SetupTabScroll *const parent,
                            std::string eventName, gcn::ListModel *const model,
                            const int width = 150, const bool onTheFly = false,
                            const bool mainConfig = true);

        SetupItemSliderList(std::string text, std::string description,
                            std::string keyName, SetupTabScroll *const parent,
                            std::string eventName, gcn::ListModel *const model,
                            std::string def, const int width = 150,
                            const bool onTheFly = false,
                            const bool mainConfig = true);

        A_DELETE_COPY(SetupItemSliderList);

        ~SetupItemSliderList();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

        virtual void action(const gcn::ActionEvent &event) override;

        void apply(std::string eventName) override;

        virtual void addMoreControls() = 0;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        SliderList *mSlider;
        gcn::ListModel *mModel;
        int mWidth;
        bool mOnTheFly;
};

class SetupItemSound final : public SetupItemSliderList
{
    public:
        SetupItemSound(std::string text, std::string description,
                       std::string keyName, SetupTabScroll *const parent,
                       std::string eventName, gcn::ListModel *const model,
                       const int width = 150, const bool onTheFly = false,
                       const bool mainConfig = true);

        A_DELETE_COPY(SetupItemSound);

        void action(const gcn::ActionEvent &event) override;

        void addMoreControls() override;

    protected:
        Button *mButton;
};

#endif
