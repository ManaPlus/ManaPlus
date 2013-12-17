/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_SETUPITEM_H
#define GUI_WIDGETS_SETUPITEM_H

#include "gui/widgets/button.h"
#include "gui/widgets/tabs/setuptabscroll.h"

#include <guichan/actionlistener.hpp>

#include <list>
#include <vector>

class CheckBox;
class Configuration;
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

class SetupItem : public gcn::ActionListener,
                  public Widget2
{
    public:
        enum
        {
            VBOOL = 0,
            VSTR,
            VINT,
            VNONE
        };

        A_DELETE_COPY(SetupItem)

        virtual ~SetupItem();

        void load();

        void save() const;

        virtual void fromWidget() = 0;

        virtual void toWidget() = 0;

        void setWidget(gcn::Widget *widget)
        { mWidget = widget; }

        gcn::Widget *getWidget() const A_WARN_UNUSED
        { return mWidget; }

        Configuration *getConfig() const A_WARN_UNUSED;

        virtual std::string getActionEventId() const A_WARN_UNUSED;

        virtual void action(const gcn::ActionEvent &event) override;

        virtual void action();

        virtual void apply(const std::string &eventName);

        virtual void cancel(const std::string &eventName);

        virtual void externalUpdated(const std::string &eventName);

        bool isMainConfig() const A_WARN_UNUSED
        { return mMainConfig; }

        void fixFirstItemSize(gcn::Widget *const widget);

        void rereadValue();

    protected:
        SetupItem(const std::string &restrict text,
                  const std::string &restrict description,
                  const std::string &restrict keyName,
                  SetupTabScroll *restrict const parent,
                  const std::string &restrict eventName,
                  const bool mainConfig);

        SetupItem(const std::string &restrict text,
                  const std::string &restrict description,
                  const std::string &restrict keyName,
                  SetupTabScroll *restrict const parent,
                  const std::string &restrict eventName,
                  const std::string &def,
                  const bool mainConfig);

        std::string mText;

        std::string mDescription;

        std::string mKeyName;

        SetupTabScroll *mParent;

        std::string mEventName;

        std::string mValue;

        std::string mDefault;

        gcn::Widget *mWidget;

        std::list<gcn::Widget*> mTempWidgets;

        int mValueType;

        bool mMainConfig;

        bool mUseDefault;
};

class SetupItemCheckBox final : public SetupItem
{
    public:
        SetupItemCheckBox(const std::string &restrict text,
                          const std::string &restrict description,
                          const std::string &restrict keyName,
                          SetupTabScroll *restrict const parent,
                          const std::string &restrict eventName,
                          const bool mainConfig = true);

        SetupItemCheckBox(const std::string &restrict text,
                          const std::string &restrict description,
                          const std::string &restrict keyName,
                          SetupTabScroll *restrict const parent,
                          const std::string &restrict eventName,
                          const std::string &restrict def,
                          const bool mainConfig = true);

        A_DELETE_COPY(SetupItemCheckBox)

        ~SetupItemCheckBox();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

    protected:
        CheckBox *mCheckBox;
};

class SetupItemTextField final : public SetupItem
{
    public:
        SetupItemTextField(const std::string &restrict text,
                           const std::string &restrict description,
                           const std::string &restrict keyName,
                           SetupTabScroll *restrict const parent,
                           const std::string &restrict eventName,
                           const bool mainConfig = true);

        SetupItemTextField(const std::string &restrict text,
                           const std::string &restrict description,
                           const std::string &restrict keyName,
                           SetupTabScroll *restrict const parent,
                           const std::string &restrict eventName,
                           const std::string &restrict def,
                           const bool mainConfig = true);

        A_DELETE_COPY(SetupItemTextField)

        ~SetupItemTextField();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const gcn::ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

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
        SetupItemIntTextField(const std::string &restrict text,
                              const std::string &restrict description,
                              const std::string &restrict keyName,
                              SetupTabScroll *restrict const parent,
                              const std::string &restrict eventName,
                              const int min, const int max,
                              const bool mainConfig = true);

        SetupItemIntTextField(const std::string &restrict text,
                              const std::string &restrict description,
                              const std::string &restrict keyName,
                              SetupTabScroll *restrict const parent,
                              const std::string &restrict eventName,
                              const int min, const int max,
                              const std::string &restrict def,
                              const bool mainConfig = true);

        A_DELETE_COPY(SetupItemIntTextField)

        ~SetupItemIntTextField();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const gcn::ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        IntTextField *mTextField;
        Button *mButton;
        EditDialog *mEditDialog;
        int mMin;
        int mMax;
};

class SetupItemLabel final : public SetupItem
{
    public:
        SetupItemLabel(const std::string &restrict text,
                       const std::string &restrict description,
                       SetupTabScroll *restrict const parent,
                       const bool separator = true);

        A_DELETE_COPY(SetupItemLabel)

        ~SetupItemLabel();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const gcn::ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

    protected:
        Label *mLabel;
        bool mIsSeparator;
};

class SetupItemDropDown final : public SetupItem
{
    public:
        SetupItemDropDown(const std::string &restrict text,
                          const std::string &restrict description,
                          const std::string &restrict keyName,
                          SetupTabScroll *restrict const parent,
                          const std::string &restrict eventName,
                          gcn::ListModel *restrict const model,
                          const int width, const bool mainConfig = true);

        SetupItemDropDown(const std::string &restrict text,
                          const std::string &restrict description,
                          const std::string &restrict keyName,
                          SetupTabScroll *restrict const parent,
                          const std::string &restrict eventName,
                          gcn::ListModel *restrict const model,
                          const int width,
                          const std::string &restrict def,
                          const bool mainConfig = true);

        A_DELETE_COPY(SetupItemDropDown)

        ~SetupItemDropDown();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        gcn::ListModel *mModel;
        DropDown *mDropDown;
        int mWidth;
};

class SetupItemSlider final : public SetupItem
{
    public:
        SetupItemSlider(const std::string &restrict text,
                        const std::string &restrict description,
                        const std::string &restrict keyName,
                        SetupTabScroll *restrict const parent,
                        const std::string &restrict eventName,
                        const double min, const double max,
                        const int width = 150, const bool onTheFly = false,
                        const bool mainConfig = true);

        SetupItemSlider(const std::string &restrict text,
                        const std::string &restrict description,
                        const std::string &restrict keyName,
                        SetupTabScroll *restrict const parent,
                        const std::string &restrict eventName,
                        const double min, const double max,
                        const std::string &restrict def, const int width = 150,
                        const bool onTheFly = false,
                        const bool mainConfig = true);

        A_DELETE_COPY(SetupItemSlider)

        ~SetupItemSlider();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const gcn::ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

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
        SetupItemSlider2(const std::string &restrict text,
                         const std::string &restrict description,
                         const std::string &restrict keyName,
                         SetupTabScroll *restrict const parent,
                         const std::string &restrict eventName,
                         const int min, const int max,
                         SetupItemNames *restrict const values,
                         const bool onTheFly = false,
                         const bool mainConfig = true,
                         const bool doNotAlign = false);

        SetupItemSlider2(const std::string &restrict text,
                         const std::string &restrict description,
                         const std::string &restrict keyName,
                         SetupTabScroll *restrict const parent,
                         const std::string &restrict eventName,
                         const int min, const int max,
                         SetupItemNames *restrict const values,
                         const std::string &restrict def,
                         const bool onTheFly = false,
                         const bool mainConfig = true,
                         const bool doNotAlign = false);

        A_DELETE_COPY(SetupItemSlider2)

        ~SetupItemSlider2();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const gcn::ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

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
        int mInvertValue;
        bool mInvert;
        bool mOnTheFly;
        bool mDoNotAlign;
};

class SetupItemSliderList : public SetupItem
{
    public:
        A_DELETE_COPY(SetupItemSliderList)

        virtual ~SetupItemSliderList();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        virtual void action(const gcn::ActionEvent &event) override;

        void apply(const std::string &eventName) override final;

        virtual void addMoreControls() = 0;

    protected:
        SetupItemSliderList(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            gcn::ListModel *restrict const model,
                            const int width = 150, const bool onTheFly = false,
                            const bool mainConfig = true);

        SetupItemSliderList(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            gcn::ListModel *restrict const model,
                            const std::string &restrict def,
                            const int width = 150,
                            const bool onTheFly = false,
                            const bool mainConfig = true);

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
        SetupItemSound(const std::string &restrict text,
                       const std::string &restrict description,
                       const std::string &restrict keyName,
                       SetupTabScroll *restrict const parent,
                       const std::string &restrict eventName,
                       gcn::ListModel *restrict const model,
                       const int width = 150,
                       const bool onTheFly = false,
                       const bool mainConfig = true);

        A_DELETE_COPY(SetupItemSound)

        void action(const gcn::ActionEvent &event) override final;

        void addMoreControls() override final;

    protected:
        Button *mButton;
};

#endif  // GUI_WIDGETS_SETUPITEM_H
