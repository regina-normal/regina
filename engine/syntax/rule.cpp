
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

#include "syntax/rule.h"
#include "syntax/definition_p.h"

#include <cassert>
#include <iostream>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {
namespace syntax {

Rule::Rule() :
    m_column(-1),
    m_firstNonSpace(false),
    m_lookAhead(false)
{
}

Rule::~Rule()
{
}

Definition Rule::definition() const
{
    return m_def.definition();
}

void Rule::setDefinition(const Definition &def)
{
    m_def = def;
}

const std::string& Rule::attribute() const
{
    return m_attribute;
}

const ContextSwitch& Rule::context() const
{
    return m_context;
}

bool Rule::isLookAhead() const
{
    return m_lookAhead;
}

bool Rule::firstNonSpace() const
{
    return m_firstNonSpace;
}

int Rule::requiredColumn() const
{
    return m_column;
}

bool Rule::load(xmlTextReaderPtr reader)
{
    m_attribute = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"attribute"));
    if (regina::xml::xmlString(xmlTextReaderName(reader)) != "IncludeRules") // IncludeRules uses this with a different semantic
        m_context.parse(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"context")));
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"firstNonSpace")), m_firstNonSpace);
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"lookAhead")), m_lookAhead);
    if (! regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"column")), m_column))
        m_column = -1;

    auto result = doLoad(reader);

    if (m_lookAhead && m_context.isStay())
        result = false;

    if (xmlTextReaderIsEmptyElement(reader))
        return result;
    if (xmlTextReaderRead(reader) != 1)
        return result;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
            {
                auto rule = Rule::create(regina::xml::xmlString(xmlTextReaderName(reader)));
                if (rule) {
                    rule->setDefinition(m_def.definition());
                    if (rule->load(reader)) {
                        m_subRules.push_back(rule);
                        if (xmlTextReaderRead(reader) != 1)
                            return result;
                    }
                } else {
                    // Skip current element.
                    if (xmlTextReaderNext(reader) != 1)
                        return result;
                }
                break;
            }
            case 15 /* end element */:
                return result;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return result;
                break;
        }
    }

    return result;
}

void Rule::resolveContext()
{
    m_context.resolve(m_def.definition());
    for (const auto &rule : m_subRules)
        rule->resolveContext();
}

bool Rule::doLoad(xmlTextReaderPtr)
{
    return true;
}

MatchResult Rule::match(Matcher& m, int offset)
{
    assert(! m.textEmpty());

    const auto result = doMatch(m, offset);
    if (result.offset() == offset || result.offset() == m.textSize())
        return result;

    for (const auto &subRule : m_subRules) {
        const auto subResult = subRule->match(m, result.offset());
        if (subResult.offset() > result.offset())
            return MatchResult(subResult.offset());
    }

    return result;
}

Rule::Ptr Rule::create(const std::string& name)
{
    Rule *rule = nullptr;
    if (name == "AnyChar")
        rule = new AnyChar;
    else if (name == "DetectChar")
        rule = new DetectChar;
    else if (name == "Detect2Chars")
        rule = new Detect2Char;
    else if (name == "DetectIdentifier")
        rule = new DetectIdentifier;
    else if (name == "DetectSpaces")
        rule = new DetectSpaces;
    else if (name == "Float")
        rule = new Float;
    else if (name == "Int")
        rule = new Int;
    else if (name == "HlCChar")
        rule = new HlCChar;
    else if (name == "HlCHex")
        rule = new HlCHex;
    else if (name == "HlCOct")
        rule = new HlCOct;
    else if (name == "HlCStringChar")
        rule = new HlCStringChar;
    else if (name == "IncludeRules")
        rule = new IncludeRules;
    else if (name == "keyword")
        rule = new KeywordListRule;
    else if (name == "LineContinue")
        rule = new LineContinue;
    else if (name == "RangeDetect")
        rule = new RangeDetect;
    else if (name == "RegExpr")
        rule = new RegExpr;
    else if (name == "StringDetect")
        rule = new StringDetect;
    else if (name == "WordDetect")
        rule = new WordDetect;
    else
        std::cerr << "Unknown rule type: " << name << std::endl;

    return Ptr(rule);
}

bool AnyChar::doLoad(xmlTextReaderPtr reader)
{
    m_chars = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));
    if (m_chars.length() == 1)
        std::cerr << "AnyChar rule with just one char: use DetectChar instead." << std::endl;
    return !m_chars.empty();
}

bool DetectChar::doLoad(xmlTextReaderPtr reader)
{
    const auto s = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    if (s.empty())
        return false;
    m_char = s.front();
    return true;
}

bool Detect2Char::doLoad(xmlTextReaderPtr reader)
{
    const auto s1 = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    const auto s2 = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char1"));
    if (s1.empty() || s2.empty())
        return false;
    m_char1 = s1.front();
    m_char2 = s2.front();
    return true;
}

const std::string& IncludeRules::contextName() const
{
    return m_contextName;
}

const std::string& IncludeRules::definitionName() const
{
    return m_defName;
}

bool IncludeRules::includeAttribute() const
{
    return m_includeAttribute;
}

bool IncludeRules::doLoad(xmlTextReaderPtr reader)
{
    const auto s = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"context"));
    if (s.empty())
        return false;

    auto pos = s.find("##");
    if (pos != std::string::npos) {
        m_contextName = s.substr(0, pos);
        m_defName = s.substr(pos + 2);
    } else {
        m_contextName = s;
    }

    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"includeAttrib")), m_includeAttribute);

    return !m_contextName.empty() || !m_defName.empty();
}


bool KeywordListRule::doLoad(xmlTextReaderPtr reader)
{
    m_listName = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));

    auto attr = xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive");
    if (attr) {
        bool cs;
        regina::valueOf(regina::xml::xmlString(attr), cs);
        m_caseSensitivityOverride = ! cs;
        m_hasCaseSensitivityOverride = true;
    } else {
        m_hasCaseSensitivityOverride = false;
    }
    return !m_listName.empty();
}

const KeywordList* KeywordListRule::keywordList() {
    if (! m_keywordList) {
        const auto def = definition();
        assert(def.isValid());
        auto defData = DefinitionData::get(def);
        m_keywordList = &defData->keywordList(m_listName);
    }

    return m_keywordList;
}

bool LineContinue::doLoad(xmlTextReaderPtr reader)
{
    const auto s = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    if (s.empty())
        m_char = '\\';
    else
        m_char = s.front();
    return true;
}

bool RangeDetect::doLoad(xmlTextReaderPtr reader)
{
    const auto s1 = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    const auto s2 = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char1"));
    if (s1.empty() || s2.empty())
        return false;
    m_begin = s1.front();
    m_end = s2.front();
    return true;
}

bool RegExpr::doLoad(xmlTextReaderPtr reader)
{
    m_pattern = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));

    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"minimal")), m_minimal);
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive")), m_caseInsensitive);

    return !m_pattern.empty(); // m_regex.isValid() would be better, but parses the regex and thus is way too expensive
}

bool StringDetect::doLoad(xmlTextReaderPtr reader)
{
    m_string = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));

    bool cs;
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive")), cs);
    m_caseSensitivity = ! cs;

    return !m_string.empty();
}

bool WordDetect::doLoad(xmlTextReaderPtr reader)
{
    m_word = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));
    return !m_word.empty();
}

} } // namespace regina::syntax
