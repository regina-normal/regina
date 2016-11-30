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

#ifndef KSYNTAXHIGHLIGHTING_KEYWORDLIST_P_H
#define KSYNTAXHIGHLIGHTING_KEYWORDLIST_P_H

#include <set>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace KSyntaxHighlighting {

class KeywordList : public boost::noncopyable
{
public:
    KeywordList();
    ~KeywordList();

    bool isEmpty() const;

    const std::string& name() const;
    bool caseSensitive() const;

    /**
     * Checks if @p str is a keyword in this list, overriding the global case-sensitivity setting.
     *
     * \pre If \a caseSensitivityOverride is \c false, then \a str is lower-case.
     */
    bool contains(const std::string& str, bool caseSensitivityOverride) const;

    void load(xmlTextReaderPtr reader);
    void setCaseSensitivity(bool caseSensitive);

private:
    std::string m_name;
    std::set<std::string> m_keywords;
    mutable std::set<std::string> m_lowerCaseKeywords;
    bool m_caseSensitive;
};

inline bool KeywordList::caseSensitive() const {
    return m_caseSensitive;
}

}

#endif // KSYNTAXHIGHLIGHTING_KEYWORDLIST_P_H
