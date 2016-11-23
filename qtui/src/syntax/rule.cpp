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

#include "rule_p.h"
#include "definition_p.h"

#include <QString>
#include <cassert>
#include <iostream>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

using namespace KSyntaxHighlighting;

static bool isOctalChar(QChar c)
{
    return c.isNumber() && c != QLatin1Char('9') && c != QLatin1Char('8');
}

static bool isHexChar(QChar c)
{
    return c.isNumber()
        || c == QLatin1Char('a') || c == QLatin1Char('A')
        || c == QLatin1Char('b') || c == QLatin1Char('B')
        || c == QLatin1Char('c') || c == QLatin1Char('C')
        || c == QLatin1Char('d') || c == QLatin1Char('D')
        || c == QLatin1Char('e') || c == QLatin1Char('E')
        || c == QLatin1Char('f') || c == QLatin1Char('F');
}

static int matchEscapedChar(const QString &text, int offset)
{
    if (text.at(offset) != QLatin1Char('\\') || text.size() < offset + 2)
        return offset;

    const auto c = text.at(offset + 1);
    static const auto controlChars = QStringLiteral("abefnrtv\"'?\\");
    if (controlChars.contains(c))
        return offset + 2;

    if (c == QLatin1Char('x')) { // hex encoded character
        auto newOffset = offset + 2;
        for (int i = 0; i < 2 && newOffset + i < text.size(); ++i, ++newOffset) {
            if (!isHexChar(text.at(newOffset)))
                break;
        }
        if (newOffset == offset + 2)
            return offset;
        return newOffset;
    }

    if (isOctalChar(c)) { // octal encoding
        auto newOffset = offset + 2;
        for (int i = 0; i < 2 && newOffset + i < text.size(); ++i, ++newOffset) {
            if (!isOctalChar(text.at(newOffset)))
                break;
        }
        if (newOffset == offset + 2)
            return offset;
        return newOffset;
    }

    return offset;
}

static QString quoteCapture(const QString &capture)
{
    auto quoted = capture;
    return quoted.replace(QRegularExpression(QStringLiteral("(\\W)")), QStringLiteral("\\\\1"));
}

static QString replaceCaptures(const QString &pattern, const QStringList &captures, bool quote)
{
    auto result = pattern;
    for (int i = 1; i < captures.size(); ++i) {
        result.replace(QLatin1Char('%') + QString::number(i), quote ? quoteCapture(captures.at(i)) : captures.at(i));
    }
    return result;
}


Rule::Rule() :
    m_column(-1),
    m_firstNonSpace(false),
    m_lookAhead(false),
    m_dynamic(false)
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

ContextSwitch Rule::context() const
{
    return m_context;
}

bool Rule::isLookAhead() const
{
    return m_lookAhead;
}

bool Rule::isDynamic() const
{
    return m_dynamic;
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
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"dynamic")), m_dynamic);

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

MatchResult Rule::match(const QString &text, int offset, const QStringList &captures)
{
    assert(!text.isEmpty());

    const auto result = doMatch(text, offset, captures);
    if (result.offset() == offset || result.offset() == text.size())
        return result;

    for (const auto &subRule : m_subRules) {
        const auto subResult = subRule->match(text, result.offset(), QStringList());
        if (subResult.offset() > result.offset())
            return MatchResult(subResult.offset(), result.captures());
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

bool Rule::isDelimiter(QChar c) const
{
    auto defData = DefinitionData::get(m_def.definition());
    return defData->isDelimiter(c.toLatin1());
}


bool AnyChar::doLoad(xmlTextReaderPtr reader)
{
    m_chars = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));
    if (m_chars.length() == 1)
        std::cerr << "AnyChar rule with just one char: use DetectChar instead." << std::endl;
    return !m_chars.empty();
}

MatchResult AnyChar::doMatch(const QString& text, int offset, const QStringList&)
{
    if (m_chars.find(text.at(offset).toLatin1()) != std::string::npos)
        return offset + 1;
    return offset;
}


bool DetectChar::doLoad(xmlTextReaderPtr reader)
{
    const auto s = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    if (s.empty())
        return false;
    m_char = s.front();
    if (isDynamic()) {
        m_captureIndex = m_char.digitValue();
    }
    return true;
}

MatchResult DetectChar::doMatch(const QString& text, int offset, const QStringList &captures)
{
    if (isDynamic()) {
        if (captures.size() <= m_captureIndex || captures.at(m_captureIndex).isEmpty())
            return offset;
        if (text.at(offset) == captures.at(m_captureIndex).at(0))
            return offset + 1;
        return offset;
    }

    if (text.at(offset) == m_char)
        return offset + 1;
    return offset;
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

MatchResult Detect2Char::doMatch(const QString& text, int offset, const QStringList&)
{
    if (text.size() - offset < 2)
        return offset;
    if (text.at(offset) == m_char1 && text.at(offset + 1) == m_char2)
        return offset + 2;
    return offset;
}


MatchResult DetectIdentifier::doMatch(const QString& text, int offset, const QStringList&)
{
    if (!text.at(offset).isLetter() && text.at(offset) != QLatin1Char('_'))
        return offset;

    for (int i = offset + 1; i < text.size(); ++i) {
        const auto c = text.at(i);
        if (!c.isLetterOrNumber() && c != QLatin1Char('_'))
            return i;
    }

    return text.size();
}


MatchResult DetectSpaces::doMatch(const QString& text, int offset, const QStringList&)
{
    while(offset < text.size() && text.at(offset).isSpace())
        ++offset;
    return offset;
}


MatchResult Float::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    auto newOffset = offset;
    while (newOffset < text.size() && text.at(newOffset).isDigit())
        ++newOffset;

    if (newOffset >= text.size() || text.at(newOffset) != QLatin1Char('.'))
        return offset;
    ++newOffset;

    while (newOffset < text.size() && text.at(newOffset).isDigit())
        ++newOffset;

    if (newOffset == offset + 1) // we only found a decimal point
        return offset;

    auto expOffset = newOffset;
    if (expOffset >= text.size() || (text.at(expOffset) != QLatin1Char('e') && text.at(expOffset) != QLatin1Char('E')))
        return newOffset;
    ++expOffset;

    if (expOffset < text.size() && (text.at(expOffset) == QLatin1Char('+') || text.at(expOffset) == QLatin1Char('-')))
        ++expOffset;
    bool foundExpDigit = false;
    while (expOffset < text.size() && text.at(expOffset).isDigit()) {
        ++expOffset;
        foundExpDigit = true;
    }

    if (!foundExpDigit)
        return newOffset;
    return expOffset;
}


MatchResult HlCChar::doMatch(const QString& text, int offset, const QStringList&)
{
    if (text.size() < offset + 3)
        return offset;

    if (text.at(offset) != QLatin1Char('\'') || text.at(offset + 1) == QLatin1Char('\''))
        return offset;

    auto newOffset = matchEscapedChar(text, offset + 1);
    if (newOffset == offset + 1) {
        if (text.at(newOffset) == QLatin1Char('\\'))
            return offset;
        else
            ++newOffset;
    }
    if (newOffset >= text.size())
        return offset;

    if (text.at(newOffset) == QLatin1Char('\''))
        return newOffset + 1;

    return offset;
}


MatchResult HlCHex::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    if (text.size() < offset + 3)
        return offset;

    if (text.at(offset) != QLatin1Char('0') || (text.at(offset + 1) != QLatin1Char('x') && text.at(offset + 1) != QLatin1Char('X')))
        return offset;

    if (!isHexChar(text.at(offset + 2)))
        return offset;

    offset += 3;
    while (offset < text.size() && isHexChar(text.at(offset)))
        ++offset;

    // TODO Kate matches U/L suffix, QtC does not?

    return offset;
}


MatchResult HlCOct::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    if (text.size() < offset + 2)
        return offset;

    if (text.at(offset) != QLatin1Char('0'))
        return offset;

    if (!isOctalChar(text.at(offset + 1)))
        return offset;

    offset += 2;
    while (offset < text.size() && isOctalChar(text.at(offset)))
        ++offset;

    return offset;
}


MatchResult HlCStringChar::doMatch(const QString& text, int offset, const QStringList&)
{
    return matchEscapedChar(text, offset);
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

MatchResult IncludeRules::doMatch(const QString&, int offset, const QStringList&)
{
    std::cerr << "Unresolved include rule for" << m_contextName << "##" << m_defName << std::endl;
    return offset;
}


MatchResult Int::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    while(offset < text.size() && text.at(offset).isDigit())
        ++offset;
    return offset;
}


bool KeywordListRule::doLoad(xmlTextReaderPtr reader)
{
    m_listName = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));

    auto attr = xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive");
    if (attr) {
        bool cs;
        regina::valueOf(regina::xml::xmlString(attr), cs);
        m_caseSensitivityOverride = cs ? Qt::CaseInsensitive : Qt::CaseSensitive;
        m_hasCaseSensitivityOverride = true;
    } else {
        m_hasCaseSensitivityOverride = false;
    }
    return !m_listName.empty();
}

MatchResult KeywordListRule::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    if (! m_keywordList) {
        const auto def = definition();
        assert(def.isValid());
        auto defData = DefinitionData::get(def);
        m_keywordList = &defData->keywordList(m_listName);
    }

    auto newOffset = offset;
    while (text.size() > newOffset && !isDelimiter(text.at(newOffset)))
        ++newOffset;
    if (newOffset == offset)
        return offset;

    if (m_hasCaseSensitivityOverride) {
        if (m_keywordList->contains(text.midRef(offset, newOffset - offset), m_caseSensitivityOverride))
            return newOffset;
    } else {
        if (m_keywordList->contains(text.midRef(offset, newOffset - offset)))
            return newOffset;
    }
    return offset;
}


bool LineContinue::doLoad(xmlTextReaderPtr reader)
{
    const auto s = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"char"));
    if (s.empty())
        m_char = QLatin1Char('\\');
    else
        m_char = s.front();
    return true;
}

MatchResult LineContinue::doMatch(const QString& text, int offset, const QStringList&)
{
    if (offset == text.size() - 1 && text.at(offset) == m_char)
        return offset + 1;
    return offset;
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

MatchResult RangeDetect::doMatch(const QString& text, int offset, const QStringList&)
{
    if (text.size() - offset < 2)
        return offset;
    if (text.at(offset) != m_begin)
        return offset;

    auto newOffset = offset + 1;
    while (newOffset < text.size()) {
        if (text.at(newOffset) == m_end)
            return newOffset + 1;
        ++newOffset;
    }
    return offset;
}

bool RegExpr::doLoad(xmlTextReaderPtr reader)
{
    m_pattern = QString::fromUtf8(
        regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String")).c_str());
    m_regexp.setPattern(m_pattern);
    bool isMinimal, isCaseInsensitive;
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"minimal")), isMinimal);
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive")), isCaseInsensitive);
    m_regexp.setPatternOptions(
        (isMinimal ? QRegularExpression::InvertedGreedinessOption : QRegularExpression::NoPatternOption) |
        (isCaseInsensitive ? QRegularExpression::CaseInsensitiveOption : QRegularExpression::NoPatternOption));
    return !m_pattern.isEmpty(); // m_regexp.isValid() would be better, but parses the regexp and thus is way too expensive
}

MatchResult RegExpr::doMatch(const QString& text, int offset, const QStringList &captures)
{
    assert(m_regexp.isValid());

    if (isDynamic())
        m_regexp.setPattern(replaceCaptures(m_pattern, captures, true));

    auto result = m_regexp.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::DontCheckSubjectStringMatchOption);
    if (result.capturedStart() == offset)
        return MatchResult(offset + result.capturedLength(), result.capturedTexts());
    return MatchResult(offset, result.capturedStart());
}


bool StringDetect::doLoad(xmlTextReaderPtr reader)
{
    m_string = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));

    bool cs;
    regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"insensitive")), cs);
    m_caseSensitivity = cs ? Qt::CaseInsensitive : Qt::CaseSensitive;

    return !m_string.empty();
}

MatchResult StringDetect::doMatch(const QString& text, int offset, const QStringList &captures)
{
    QString pattern = QString::fromUtf8(m_string.c_str());
    if (isDynamic())
        pattern = replaceCaptures(pattern, captures, false);
    if (text.midRef(offset, pattern.length()).compare(pattern, m_caseSensitivity) == 0)
        return offset + pattern.size();
    return offset;
}


bool WordDetect::doLoad(xmlTextReaderPtr reader)
{
    m_word = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"String"));
    return !m_word.empty();
}

MatchResult WordDetect::doMatch(const QString& text, int offset, const QStringList&)
{
    if (text.size() - offset < m_word.length())
        return offset;

    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    if (text.midRef(offset, m_word.length()).toString().toUtf8().constData() != m_word)
        return offset;

    if (text.size() == offset + m_word.length() || isDelimiter(text.at(offset + m_word.length())))
        return offset + m_word.length();

    return offset;
}
