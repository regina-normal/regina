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

#include "contextswitch_p.h"
#include "definition.h"
#include "definitionref_p.h"
#include "definition_p.h"
#include "qtmatcher.h"
#include "rule_p.h"

#include <cassert>
#include <iostream>

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

bool QtMatcher::isDelimiter(Rule& r, QChar c) const {
    return DefinitionData::get(r.definition())->isDelimiter(c.toLatin1());
}

MatchResult QtMatcher::match(AnyChar& rule, int offset)
{
    if (rule.chars().find(m_text.at(offset).toLatin1()) != std::string::npos)
        return offset + 1;
    return offset;
}


MatchResult QtMatcher::match(DetectChar& rule, int offset)
{
    if (m_text.at(offset) == rule.matchChar())
        return offset + 1;
    return offset;
}


MatchResult QtMatcher::match(Detect2Char& rule, int offset)
{
    if (m_text.size() - offset < 2)
        return offset;
    if (m_text.at(offset) == rule.matchChar1() && m_text.at(offset + 1) == rule.matchChar2())
        return offset + 2;
    return offset;
}


MatchResult QtMatcher::match(DetectIdentifier& rule, int offset)
{
    if (!m_text.at(offset).isLetter() && m_text.at(offset) != QLatin1Char('_'))
        return offset;

    for (int i = offset + 1; i < m_text.size(); ++i) {
        const auto c = m_text.at(i);
        if (!c.isLetterOrNumber() && c != QLatin1Char('_'))
            return i;
    }

    return m_text.size();
}


MatchResult QtMatcher::match(DetectSpaces& rule, int offset)
{
    while(offset < m_text.size() && m_text.at(offset).isSpace())
        ++offset;
    return offset;
}


MatchResult QtMatcher::match(Float& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    auto newOffset = offset;
    while (newOffset < m_text.size() && m_text.at(newOffset).isDigit())
        ++newOffset;

    if (newOffset >= m_text.size() || m_text.at(newOffset) != QLatin1Char('.'))
        return offset;
    ++newOffset;

    while (newOffset < m_text.size() && m_text.at(newOffset).isDigit())
        ++newOffset;

    if (newOffset == offset + 1) // we only found a decimal point
        return offset;

    auto expOffset = newOffset;
    if (expOffset >= m_text.size() || (m_text.at(expOffset) != QLatin1Char('e') && m_text.at(expOffset) != QLatin1Char('E')))
        return newOffset;
    ++expOffset;

    if (expOffset < m_text.size() && (m_text.at(expOffset) == QLatin1Char('+') || m_text.at(expOffset) == QLatin1Char('-')))
        ++expOffset;
    bool foundExpDigit = false;
    while (expOffset < m_text.size() && m_text.at(expOffset).isDigit()) {
        ++expOffset;
        foundExpDigit = true;
    }

    if (!foundExpDigit)
        return newOffset;
    return expOffset;
}


MatchResult QtMatcher::match(HlCChar& rule, int offset)
{
    if (m_text.size() < offset + 3)
        return offset;

    if (m_text.at(offset) != QLatin1Char('\'') || m_text.at(offset + 1) == QLatin1Char('\''))
        return offset;

    auto newOffset = matchEscapedChar(m_text, offset + 1);
    if (newOffset == offset + 1) {
        if (m_text.at(newOffset) == QLatin1Char('\\'))
            return offset;
        else
            ++newOffset;
    }
    if (newOffset >= m_text.size())
        return offset;

    if (m_text.at(newOffset) == QLatin1Char('\''))
        return newOffset + 1;

    return offset;
}


MatchResult QtMatcher::match(HlCHex& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    if (m_text.size() < offset + 3)
        return offset;

    if (m_text.at(offset) != QLatin1Char('0') || (m_text.at(offset + 1) != QLatin1Char('x') && m_text.at(offset + 1) != QLatin1Char('X')))
        return offset;

    if (!isHexChar(m_text.at(offset + 2)))
        return offset;

    offset += 3;
    while (offset < m_text.size() && isHexChar(m_text.at(offset)))
        ++offset;

    // TODO Kate matches U/L suffix, QtC does not?

    return offset;
}


MatchResult QtMatcher::match(HlCOct& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    if (m_text.size() < offset + 2)
        return offset;

    if (m_text.at(offset) != QLatin1Char('0'))
        return offset;

    if (!isOctalChar(m_text.at(offset + 1)))
        return offset;

    offset += 2;
    while (offset < m_text.size() && isOctalChar(m_text.at(offset)))
        ++offset;

    return offset;
}


MatchResult QtMatcher::match(HlCStringChar& rule, int offset)
{
    return matchEscapedChar(m_text, offset);
}

MatchResult QtMatcher::match(IncludeRules& rule, int offset)
{
    std::cerr << "Unresolved include rule for" << rule.contextName() << "##" << rule.definitionName() << std::endl;
    return offset;
}


MatchResult QtMatcher::match(Int& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    while(offset < m_text.size() && m_text.at(offset).isDigit())
        ++offset;
    return offset;
}

MatchResult QtMatcher::match(KeywordListRule& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    auto keywordList = rule.keywordList();

    auto newOffset = offset;
    while (m_text.size() > newOffset && !isDelimiter(rule, m_text.at(newOffset)))
        ++newOffset;
    if (newOffset == offset)
        return offset;

    if (rule.caseSensitivity()) {
        if (keywordList->contains(m_text.midRef(offset, newOffset - offset).toString().toUtf8().constData(), true))
            return newOffset;
    } else {
        // We must convert the string to lower-case before calling contains().
        if (keywordList->contains(m_text.midRef(offset, newOffset - offset).toString().toLower().toUtf8().constData(), false))
            return newOffset;
    }
    return offset;
}

MatchResult QtMatcher::match(LineContinue& rule, int offset)
{
    if (offset == m_text.size() - 1 && m_text.at(offset) == rule.continueChar())
        return offset + 1;
    return offset;
}

MatchResult QtMatcher::match(RangeDetect& rule, int offset)
{
    if (m_text.size() - offset < 2)
        return offset;
    if (m_text.at(offset) != rule.begin())
        return offset;

    auto newOffset = offset + 1;
    while (newOffset < m_text.size()) {
        if (m_text.at(newOffset) == rule.end())
            return newOffset + 1;
        ++newOffset;
    }
    return offset;
}

MatchResult QtMatcher::match(RegExpr& rule, int offset)
{
    assert(rule.regexp().isValid());

    auto result = rule.regexp().match(m_text, offset, QRegularExpression::NormalMatch, QRegularExpression::DontCheckSubjectStringMatchOption);
    if (result.capturedStart() == offset)
        return MatchResult(offset + result.capturedLength());
    // Be kind: if the rule matched later in the string, remember this so we don't check again
    // until we reach that position.
    return MatchResult(offset, result.capturedStart());
}

MatchResult QtMatcher::match(StringDetect& rule, int offset)
{
    QString pattern = QString::fromUtf8(rule.string().c_str());
    if (m_text.midRef(offset, pattern.length()).compare(pattern, rule.caseSensitivity() ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0)
        return offset + pattern.size();
    return offset;
}

MatchResult QtMatcher::match(WordDetect& rule, int offset)
{
    if (m_text.size() - offset < rule.word().length())
        return offset;

    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    if (m_text.midRef(offset, rule.word().length()).toString().toUtf8().constData() != rule.word())
        return offset;

    if (m_text.size() == offset + rule.word().length() || isDelimiter(rule, m_text.at(offset + rule.word().length())))
        return offset + rule.word().length();

    return offset;
}
