/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "repository.h"
#include "repository_p.h"
#include "definition.h"
#include "definition_p.h"
#include "theme.h"
#include "themedata_p.h"
#include "wildcardmatcher_p.h"

#include "file/globaldirs.h"

#include <cassert>
#include <limits>
#include <iostream>

using namespace KSyntaxHighlighting;

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
    foreach (const auto &def, d->m_defs)
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
    const auto it = std::lower_bound(m_themes.begin(), m_themes.end(), theme, [](const Theme &lhs, const Theme &rhs) {
        return lhs.name() < rhs.name();
    });
    if (it == m_themes.end() || (*it).name() != theme.name()) {
        m_themes.insert(it, theme);
        return;
    }
    if (themeRevision(*it) < themeRevision(theme))
        *it = theme;
}

quint16 RepositoryPrivate::nextFormatId()
{
    assert(m_formatId < std::numeric_limits<quint16>::max());
    return ++m_formatId;
}

void Repository::reload()
{
    std::cerr << "Reloading syntax definitions!" << std::endl;
    foreach (const auto &def, d->m_defs)
        DefinitionData::get(def.second)->clear();
    d->m_defs.clear();

    d->m_themes.clear();

    d->m_formatId = 0;

    d->load(this);
}
