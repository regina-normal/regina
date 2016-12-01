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

#ifndef KSYNTAXHIGHLIGHTING_REPOSITORY_P_H
#define KSYNTAXHIGHLIGHTING_REPOSITORY_P_H

#include <map>
#include <boost/noncopyable.hpp>

namespace regina {
namespace syntax {

class Definition;
class Repository;
class Theme;

class RepositoryPrivate : public boost::noncopyable
{
public:
    RepositoryPrivate();

    static RepositoryPrivate* get(Repository *repo);

    void load(Repository *repo);
    void addDefinition(const Definition &def);

    void addTheme(const Theme &theme);

    uint16_t nextFormatId();

    std::map<std::string, Definition> m_defs;

    std::vector<Theme> m_themes;

    uint16_t m_formatId;
};

} } // namespace regina::syntax

#endif
