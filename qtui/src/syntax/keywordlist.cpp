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

#include "keywordlist_p.h"

#include <cassert>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

using namespace KSyntaxHighlighting;

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

bool KeywordList::contains(const QStringRef &str) const
{
    return contains(str, m_caseSensitive);
}

bool KeywordList::contains(const QStringRef &str, bool caseSensitivityOverride) const
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
        return m_keywords.count(str.toString().toUtf8().constData());
    return m_lowerCaseKeywords.count(str.toString().toLower().toUtf8().constData());
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
