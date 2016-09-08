/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_MODELS_LANGLISTMODEL_H
#define GUI_MODELS_LANGLISTMODEL_H

#include "gui/models/extendedlistmodel.h"

#include "resources/loaders/imageloader.h"

#include "utils/gettext.h"

#include "localconsts.h"

struct Language final
{
    std::string name;
    std::string value;
    std::string icon;
};

const int langs_count = 22;

const Language LANG_NAME[langs_count] =
{
    // TRANSLATORS: language
    {N_("(default)"), "", ""},
    // TRANSLATORS: language
    {N_("Catalan"), "ca_ES", "ca.png"},
    // TRANSLATORS: language
    {N_("Chinese (China)"), "zh_CN", "cn.png"},
    // TRANSLATORS: language
    {N_("Chinese (Hong Kong)"), "zh_HK", "hk.png"},
    // TRANSLATORS: language
    {N_("Czech"), "cs_CZ", "cz.png"},
    // TRANSLATORS: language
    {N_("Dutch (Belgium/Flemish)"), "nl_BE", "nl_BE.png"},
    // TRANSLATORS: language
    {N_("English"), "C", "en.png"},
    // TRANSLATORS: language
    {N_("Finnish"), "fi_FI", "fi.png"},
    // TRANSLATORS: language
    {N_("French"), "fr_FR", "fr.png"},
    // TRANSLATORS: language
    {N_("German"), "de_DE", "de.png"},
    // TRANSLATORS: language
    {N_("Indonesian"), "id_ID", "id.png"},
    // TRANSLATORS: language
    {N_("Italian"), "it_IT", "it.png"},
    // TRANSLATORS: language
    {N_("Japanese"), "ja_JP", "jp.png"},
    // TRANSLATORS: language
    {N_("Polish"), "pl_PL", "pl.png"},
    // TRANSLATORS: language
    {N_("Portuguese"), "pt_PT", "pt.png"},
    // TRANSLATORS: language
    {N_("Portuguese (Brazilian)"), "pt_BR", "pt_BR.png"},
    // TRANSLATORS: language
    {N_("Russian"), "ru_RU", "ru.png"},
    // TRANSLATORS: language
    {N_("Spanish (Castilian)"), "es_ES", "es.png"},
    // TRANSLATORS: language
    {N_("Swedish (Sweden)"), "sv_SE", "se.png"},
    // TRANSLATORS: language
    {N_("Turkish"), "tr_TR", "tr.png"},
    // TRANSLATORS: language
    {N_("Ukrainian"), "uk_UA", "ua.png"},
    // TRANSLATORS: language
    {N_("Esperanto"), "eo", "eo.png"}
};

class LangListModel final : public ExtendedListModel
{
    public:
        LangListModel()
        {
            for (int f = 0; f < langs_count; f ++)
            {
                const std::string icon = LANG_NAME[f].icon;
                if (!icon.empty())
                {
                    mIcons[f] = Loader::getImage("graphics/flags/"
                        + icon);
                }
                else
                {
                    mIcons[f] = nullptr;
                }
            }
        }

        A_DELETE_COPY(LangListModel)

        ~LangListModel()
        {
            for (int f = 0; f < langs_count; f ++)
            {
                Image *const img = mIcons[f];
                if (img)
                    img->decRef();
            }
        }

        int getNumberOfElements() override final A_WARN_UNUSED
        { return langs_count; }

        std::string getElementAt(int i) override final A_WARN_UNUSED
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";

            return gettext(LANG_NAME[i].name.c_str());
        }

        const Image *getImageAt(int i) override final A_WARN_UNUSED
        {
            if (i >= getNumberOfElements() || i < 0)
                return nullptr;
            return mIcons[i];
        }

        Image *mIcons[langs_count] A_NONNULLPOINTER;
};

#endif  // GUI_MODELS_LANGLISTMODEL_H
