
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

#include "syntax/keywordlist.h"

#include <algorithm>
#include <cassert>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {
namespace syntax {

KeywordList::KeywordList() :
    m_caseSensitive(true)
{
}

KeywordList::~KeywordList()
{
}

bool KeywordList::isEmpty() const
{
    return m_keywords.empty();
}

const std::string& KeywordList::name() const
{
    return m_name;
}

bool KeywordList::contains(const std::string& str, bool caseSensitivityOverride) const
{
    // TODO: Make lower-case transformations unicode-aware.
    if ((! caseSensitivityOverride) && m_lowerCaseKeywords.empty()) {
        std::string lower;
        for (const auto& kw : m_keywords) {
            lower = kw;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            m_lowerCaseKeywords.insert(lower);
        }
    }

    // TODO avoid the copy in toString!
    if (caseSensitivityOverride)
        return m_keywords.count(str);
    else
        return m_lowerCaseKeywords.count(str);
}

void KeywordList::load(xmlTextReaderPtr reader)
{
    m_name = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));

    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
                if (regina::xml::xmlString(xmlTextReaderName(reader)) == "item") {
                    m_keywords.insert(regina::stripWhitespace(regina::xml::xmlString(xmlTextReaderReadInnerXml(reader))));
                }
                if (xmlTextReaderNext(reader) != 1)
                    return;
                break;
            case 15 /* end element */:
                return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

void KeywordList::setCaseSensitivity(bool caseSensitive)
{
    m_caseSensitive = caseSensitive;
}

} } // namespace regina::syntax
