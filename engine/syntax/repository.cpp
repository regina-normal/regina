
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This file is modified from the KDE syntax-highlighting framework,     *
 *  which is copyright (C) 2016 Volker Krause <vkrause@kde.org>           *
 *  and is distributed under the GNU Library General Public License as    *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "syntax/repository.h"
#include "syntax/repository_p.h"
#include "syntax/definition.h"
#include "syntax/definition_p.h"
#include "syntax/theme.h"
#include "syntax/themedata_p.h"

#include "file/globaldirs.h"

#include <cassert>
#include <limits>
#include <iostream>

namespace regina {
namespace syntax {

RepositoryPrivate::RepositoryPrivate() :
    m_formatId(0)
{
}

RepositoryPrivate* RepositoryPrivate::get(Repository *repo)
{
    return repo->d.get();
}

Repository::Repository() :
    d(new RepositoryPrivate)
{
    d->load(this);
}

Repository::~Repository()
{
    // reset repo so we can detect in still alive definition instances
    // that the repo was deleted
    for (const auto &def : d->m_defs)
        DefinitionData::get(def.second)->repo = nullptr;
}

Definition Repository::definitionForName(const std::string& defName) const
{
    const auto it = d->m_defs.find(defName);
    if (it != d->m_defs.end())
        return it->second;

    return Definition();
}

const std::vector<Theme>& Repository::themes() const
{
    return d->m_themes;
}

Theme Repository::theme(const std::string& themeName) const
{
    for (auto theme : d->m_themes) {
        if (theme.name() == themeName) {
            return theme;
        }
    }

    return Theme();
}

Theme Repository::defaultTheme(Repository::DefaultTheme t)
{
    if (t == DarkTheme)
        return theme("Breeze Dark");
    return theme("Default");
}

void RepositoryPrivate::load(Repository *repo)
{
    std::string syntaxBase = regina::GlobalDirs::home() + "/syntax/";

    for (const char* file : { "alert.xml", "alert_indent.xml", "modelines.xml", "python.xml" }) {
        Definition def;
        auto defData = DefinitionData::get(def);
        defData->repo = repo;
        if (defData->loadMetaData(syntaxBase + file))
            addDefinition(def);
    }

    // load themes
    auto themeData = std::unique_ptr<ThemeData>(new ThemeData);
    if (themeData->load(syntaxBase + "default.theme"))
        addTheme(Theme(themeData.release()));
}

void RepositoryPrivate::addDefinition(const Definition &def)
{
    const auto it = m_defs.find(def.name());
    if (it == m_defs.end()) {
        m_defs.insert(std::make_pair(def.name(), def));
        return;
    }

    if (it->second.version() >= def.version())
        return;
    m_defs.insert(std::make_pair(def.name(), def));
}

static int themeRevision(const Theme &theme)
{
    auto data = ThemeData::get(theme);
    return data->revision();
}

void RepositoryPrivate::addTheme(const Theme &theme)
{
    const auto it = std::lower_bound(m_themes.begin(), m_themes.end(), theme);
    if (it == m_themes.end() || (*it).name() != theme.name()) {
        m_themes.insert(it, theme);
        return;
    }
    if (themeRevision(*it) < themeRevision(theme))
        *it = theme;
}

uint16_t RepositoryPrivate::nextFormatId()
{
    assert(m_formatId < std::numeric_limits<uint16_t>::max());
    return ++m_formatId;
}

void Repository::reload()
{
    std::cerr << "Reloading syntax definitions!" << std::endl;
    for (const auto &def : d->m_defs)
        DefinitionData::get(def.second)->clear();
    d->m_defs.clear();

    d->m_themes.clear();

    d->m_formatId = 0;

    d->load(this);
}

} } // namespace regina::syntax
