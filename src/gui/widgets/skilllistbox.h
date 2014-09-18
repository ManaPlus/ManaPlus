/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/widgets/listbox.h"

#include "dragdrop.h"

#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/widgets/skilldata.h"

#include "gui/models/skillmodel.h"

#include "gui/popups/skillpopup.h"

#include "render/graphics.h"

#include "utils/delete2.h"

#include "resources/skillconsts.h"

#include "localconsts.h"

class SkillModel;

class SkillListBox final : public ListBox
{
    public:
        SkillListBox(const Widget2 *const widget,
                     SkillModel *const model) :
            ListBox(widget, model, "skilllistbox.xml"),
            mModel(model),
            mPopup(new SkillPopup),
            mTextColor(getThemeColor(Theme::TEXT)),
            mTextColor2(getThemeColor(Theme::TEXT_OUTLINE)),
            mCooldownColor(getThemeColor(Theme::SKILL_COOLDOWN)),
            mTextPadding(mSkin ? mSkin->getOption("textPadding", 34) : 34),
            mSpacing(mSkin ? mSkin->getOption("spacing", 0) : 0),
            mSkillClicked(false)
        {
            mRowHeight = getFont()->getHeight() * 2 + mSpacing + 2 * mPadding;
            mHighlightColor = getThemeColor(Theme::HIGHLIGHT);
            mPopup->postInit();

            if (mRowHeight < 34)
                mRowHeight = 34;
        }

        A_DELETE_COPY(SkillListBox)

        ~SkillListBox()
        {
            delete2(mModel)
            delete2(mPopup)
        }

        SkillInfo *getSelectedInfo() const
        {
            const int selected = getSelected();
            if (!mListModel || selected < 0
                || selected > mListModel->getNumberOfElements())
            {
                return nullptr;
            }

            return static_cast<SkillModel*>(mListModel)->getSkillAt(selected);
        }

        void draw(Graphics *graphics) override
        {
            if (!mListModel)
                return;

            SkillModel *const model = static_cast<SkillModel*>(mListModel);
            updateAlpha();

            mHighlightColor.a = static_cast<int>(mAlpha * 255.0F);
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
                if (e)
                {
                    if (e->cooldown)
                    {
                        graphics->fillRectangle(Rect(mPadding, y,
                            usableWidth * 100 / e->cooldown, 10));
                    }
                }
            }

            graphics->setColorAll(mTextColor, mTextColor2);
            for (int i = 0, y = 1 + mPadding;
                 i < model->getNumberOfElements();
                 ++i, y += getRowHeight())
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e)
                {
                    const SkillData *const data = e->data;
                    const std::string &description = data->description;
                    graphics->drawImage(data->icon, mPadding, y);
                    font->drawString(graphics, data->name, mTextPadding, y);
                    if (!description.empty())
                    {
                        font->drawString(graphics, description,
                            mTextPadding, y + space);
                    }

                    if (e->skillLevelWidth < 0)
                    {
                        // Add one for padding
                        e->skillLevelWidth = font->getWidth(e->skillLevel) + 1;
                    }

                    font->drawString(graphics, e->skillLevel, width2
                        - e->skillLevelWidth, y);
                }
            }
        }

        unsigned int getRowHeight() const override
        { return mRowHeight; }

        const SkillInfo *getSkillByEvent(const MouseEvent &event) const
        {
            const int y = (event.getY() + mPadding) / getRowHeight();
            if (!mModel || y >= mModel->getNumberOfElements())
                return nullptr;
            const SkillInfo *const skill = mModel->getSkillAt(y);
            if (!skill)
                return nullptr;
            return skill;
        }

        void mouseMoved(MouseEvent &event) override
        {
            ListBox::mouseMoved(event);
            if (!viewport || !dragDrop.isEmpty())
                return;

            const SkillInfo *const skill = getSkillByEvent(event);
            mPopup->show(skill);
            mPopup->position(viewport->mMouseX, viewport->mMouseY);
        }

        void mouseDragged(MouseEvent &event)
        {
            if (event.getButton() == MouseButton::LEFT)
            {
                if (dragDrop.isEmpty())
                {
                    if (mSkillClicked)
                    {
                        mSkillClicked = false;
                        const SkillInfo *const skill = getSkillByEvent(event);
                        if (!skill)
                            return;
                        dragDrop.dragSkill(skill, DRAGDROP_SOURCE_SKILLS);
                        dragDrop.setItem(skill->id + SKILL_MIN_ID);
                    }
                    ListBox::mouseDragged(event);
                }
            }
            else
            {
                ListBox::mouseDragged(event);
            }
        }

        void mousePressed(MouseEvent &event)
        {
            ListBox::mousePressed(event);
            if (event.getButton() == MouseButton::LEFT)
            {
                const SkillInfo *const skill = getSkillByEvent(event);
                if (!skill)
                    return;
                event.consume();
                mSkillClicked = true;
            }
        }

        void mouseReleased(MouseEvent &event)
        {
            ListBox::mouseReleased(event);
        }

        void mouseExited(MouseEvent &event A_UNUSED) override
        {
            mPopup->hide();
        }

    private:
        SkillModel *mModel;
        SkillPopup *mPopup;
        Color mTextColor;
        Color mTextColor2;
        Color mCooldownColor;
        int mTextPadding;
        int mSpacing;
        bool mSkillClicked;
};

#endif  // GUI_WIDGETS_SKILLLISTBOX_H
