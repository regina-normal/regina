
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/**
 * \file syntax/keywordlist.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_KEYWORDLIST_H
#ifndef __DOXYGEN
#define __SYNTAX_KEYWORDLIST_H
#endif

#include "regina-core.h"
#include <set>
#include <string>
#include <vector>
#include <libxml/xmlreader.h>

namespace regina::syntax {

class KeywordList
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

    // Make this class non-copyable.
    KeywordList(const KeywordList&) = delete;
    KeywordList& operator = (const KeywordList&) = delete;

private:
    std::string m_name;
    std::set<std::string> m_keywords;
    mutable std::set<std::string> m_lowerCaseKeywords;
    bool m_caseSensitive;
};

inline bool KeywordList::caseSensitive() const {
    return m_caseSensitive;
}

} // namespace regina::syntax

#endif
