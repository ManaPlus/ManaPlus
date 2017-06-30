/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SKILLLISTBOX_H
#define GUI_WIDGETS_SKILLLISTBOX_H

#include "const/resources/skill.h"

#include "dragdrop.h"

#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/widgets/listbox.h"

#include "gui/fonts/font.h"

#include "gui/models/skillmodel.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/skillpopup.h"

#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "render/graphics.h"

#include "localconsts.h"

class SkillModel;

class SkillListBox final : public ListBox
{
    public:
        SkillListBox(const Widget2 *const widget,
                     SkillModel *const model) :
            ListBox(widget, model, "skilllistbox.xml"),
            mModel(model),
            mTextColor(getThemeColor(ThemeColorId::TEXT)),
            mTextColor2(getThemeColor(ThemeColorId::TEXT_OUTLINE)),
            mCooldownColor(getThemeColor(ThemeColorId::SKILL_COOLDOWN)),
            mTextPadding(mSkin != nullptr ?
                mSkin->getOption("textPadding", 34) : 34),
            mSpacing(mSkin != nullptr ? mSkin->getOption("spacing", 0) : 0),
            mSkillClicked(false)
        {
            mRowHeight = getFont()->getHeight() * 2 + mSpacing + 2 * mPadding;
            mHighlightColor = getThemeColor(ThemeColorId::HIGHLIGHT);

            if (mRowHeight < 34)
                mRowHeight = 34;
        }

        A_DELETE_COPY(SkillListBox)

        ~SkillListBox()
        {
            delete2(mModel)
        }

        SkillInfo *getSelectedInfo() const
        {
            const int selected = getSelected();
            if ((mListModel == nullptr) || selected < 0
                || selected > mListModel->getNumberOfElements())
            {
                return nullptr;
            }

            return static_cast<SkillModel*>(mListModel)->getSkillAt(selected);
        }

        void draw(Graphics *const graphics) override final A_NONNULL(2)
        {
            if (mListModel == nullptr)
                return;

            SkillModel *const model = static_cast<SkillModel*>(mListModel);
            updateAlpha();

            mHighlightColor.a = CAST_S32(mAlpha * 255.0F);
            graphics->setColor(mHighlightColor);

            const int width1 = getWidth();
            const int usableWidth = width1 - 2 * mPadding;

            // Draw filled rectangle around the selected list element
            if (mSelected >= 0)
            {
                graphics->fillRectangle(Rect(mPadding, getRowHeight()
                    * mSelected + mPadding, usableWidth,
                    getRowHeight()));
            }

            // Draw the list elements
            Font *const font = getFont();
            const int space = font->getHeight() + mSpacing;
            const int width2 = width1 - mPadding;

            graphics->setColor(mCooldownColor);
            for (int i = 0, y = 1 + mPadding;
                 i < model->getNumberOfElements();
                 ++i, y += getRowHeight())
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e != nullptr)
                {
                    if (e->cooldown != 0)
                    {
                        graphics->fillRectangle(Rect(mPadding, y,
                            usableWidth * 100 / e->cooldown, 10));
                    }
                }
            }

            for (int i = 0, y = 1 + mPadding;
                 i < model->getNumberOfElements();
                 ++i, y += getRowHeight())
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e != nullptr)
                {
                    const SkillData *const data = e->data;
                    const std::string &description = data->description;
                    graphics->drawImage(data->icon, mPadding, y);
                    font->drawString(graphics,
                        mTextColor,
                        mTextColor2,
                        data->name,
                        mTextPadding, y);
                    if (!description.empty())
                    {
                        font->drawString(graphics,
                            mTextColor,
                            mTextColor2,
                            description,
                            mTextPadding,
                            y + space);
                    }

                    if (e->skillLevelWidth < 0)
                    {
                        // Add one for padding
                        e->skillLevelWidth = font->getWidth(e->skillLevel) + 1;
                    }

                    font->drawString(graphics,
                        mTextColor,
                        mTextColor2,
                        e->skillLevel,
                        width2 - e->skillLevelWidth,
                        y);
                }
            }
        }

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2)
        {
            SkillListBox::draw(graphics);
        }

        unsigned int getRowHeight() const override final
        { return mRowHeight; }

        const SkillInfo *getSkillByEvent(const MouseEvent &event) const
        {
            const int y = (event.getY() + mPadding) / getRowHeight();
            if (mModel == nullptr || y >= mModel->getNumberOfElements())
                return nullptr;
            const SkillInfo *const skill = mModel->getSkillAt(y);
            if (skill == nullptr)
                return nullptr;
            return skill;
        }

        void mouseMoved(MouseEvent &event) override final
        {
            ListBox::mouseMoved(event);
            if ((viewport == nullptr) || !dragDrop.isEmpty())
                return;

            const SkillInfo *const skill = getSkillByEvent(event);
            if (skill == nullptr)
                return;
            skillPopup->show(skill,
                skill->customSelectedLevel,
                skill->customCastType,
                skill->customOffsetX,
                skill->customOffsetY);
            skillPopup->position(viewport->mMouseX,
                viewport->mMouseY);
        }

        void mouseDragged(MouseEvent &event) override final
        {
            if (event.getButton() == MouseButton::LEFT)
            {
                if (dragDrop.isEmpty())
                {
                    if (mSkillClicked)
                    {
                        mSkillClicked = false;
                        const SkillInfo *const skill = getSkillByEvent(event);
                        if (skill == nullptr)
                            return;
                        dragDrop.dragSkill(skill, DragDropSource::Skills);
                        dragDrop.setItem(skill->id + SKILL_MIN_ID);
                        dragDrop.setItemData(strprintf("%d %d %d",
                            CAST_S32(skill->customCastType),
                            skill->customOffsetX,
                            skill->customOffsetY));
                    }
                    ListBox::mouseDragged(event);
                }
            }
            else
            {
                ListBox::mouseDragged(event);
            }
        }

        void mousePressed(MouseEvent &event) override final
        {
            ListBox::mousePressed(event);
            const MouseButtonT button = event.getButton();
            if (button == MouseButton::LEFT ||
                button == MouseButton::RIGHT)
            {
                const SkillInfo *const skill = getSkillByEvent(event);
                if (skill == nullptr)
                    return;
                event.consume();
                mSkillClicked = true;
                SkillModel *const model = static_cast<SkillModel*>(
                    mListModel);
                if ((model != nullptr) &&
                    mSelected >= 0 &&
                    model->getSkillAt(mSelected) == skill)
                {
                    skillPopup->hide();
                    if (button == MouseButton::LEFT &&
                        event.getX() >
                        getWidth() - mPadding - skill->skillLevelWidth)
                    {
                        popupMenu->showSkillLevelPopup(skill);
                    }
                    else if (button == MouseButton::RIGHT)
                    {
                        popupMenu->showSkillPopup(skill);
                    }
                }
            }
        }

        void mouseReleased(MouseEvent &event) override final
        {
            ListBox::mouseReleased(event);
        }

        void mouseExited(MouseEvent &event A_UNUSED) override final
        {
            skillPopup->hide();
        }

    private:
        SkillModel *mModel;
        Color mTextColor;
        Color mTextColor2;
        Color mCooldownColor;
        int mTextPadding;
        int mSpacing;
        bool mSkillClicked;
};

#endif  // GUI_WIDGETS_SKILLLISTBOX_H
