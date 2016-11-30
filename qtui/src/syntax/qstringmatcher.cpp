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
#include "qstringmatcher.h"
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

MatchResult AnyChar::doMatch(const QString& text, int offset)
{
    if (m_chars.find(text.at(offset).toLatin1()) != std::string::npos)
        return offset + 1;
    return offset;
}


MatchResult DetectChar::doMatch(const QString& text, int offset)
{
    if (text.at(offset) == m_char)
        return offset + 1;
    return offset;
}


MatchResult Detect2Char::doMatch(const QString& text, int offset)
{
    if (text.size() - offset < 2)
        return offset;
    if (text.at(offset) == m_char1 && text.at(offset + 1) == m_char2)
        return offset + 2;
    return offset;
}


MatchResult DetectIdentifier::doMatch(const QString& text, int offset)
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


MatchResult DetectSpaces::doMatch(const QString& text, int offset)
{
    while(offset < text.size() && text.at(offset).isSpace())
        ++offset;
    return offset;
}


MatchResult Float::doMatch(const QString& text, int offset)
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


MatchResult HlCChar::doMatch(const QString& text, int offset)
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


MatchResult HlCHex::doMatch(const QString& text, int offset)
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


MatchResult HlCOct::doMatch(const QString& text, int offset)
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


MatchResult HlCStringChar::doMatch(const QString& text, int offset)
{
    return matchEscapedChar(text, offset);
}

MatchResult IncludeRules::doMatch(const QString&, int offset)
{
    std::cerr << "Unresolved include rule for" << m_contextName << "##" << m_defName << std::endl;
    return offset;
}


MatchResult Int::doMatch(const QString& text, int offset)
{
    if (offset > 0 && !isDelimiter(text.at(offset - 1)))
        return offset;

    while(offset < text.size() && text.at(offset).isDigit())
        ++offset;
    return offset;
}

MatchResult KeywordListRule::doMatch(const QString& text, int offset)
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

    bool caseSensitivity = (m_hasCaseSensitivityOverride ? m_caseSensitivityOverride : m_keywordList->caseSensitive());
    if (caseSensitivity) {
        if (m_keywordList->contains(text.midRef(offset, newOffset - offset).toString().toUtf8().constData(), true))
            return newOffset;
    } else {
        // We must convert the string to lower-case before calling contains().
        if (m_keywordList->contains(text.midRef(offset, newOffset - offset).toString().toLower().toUtf8().constData(), false))
            return newOffset;
    }
    return offset;
}

MatchResult LineContinue::doMatch(const QString& text, int offset)
{
    if (offset == text.size() - 1 && text.at(offset) == m_char)
        return offset + 1;
    return offset;
}

MatchResult RangeDetect::doMatch(const QString& text, int offset)
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

MatchResult RegExpr::doMatch(const QString& text, int offset)
{
    assert(m_regexp.isValid());

    auto result = m_regexp.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::DontCheckSubjectStringMatchOption);
    if (result.capturedStart() == offset)
        return MatchResult(offset + result.capturedLength());
    // Be kind: if the rule matched later in the string, remember this so we don't check again
    // until we reach that position.
    return MatchResult(offset, result.capturedStart());
}

MatchResult StringDetect::doMatch(const QString& text, int offset)
{
    QString pattern = QString::fromUtf8(m_string.c_str());
    if (text.midRef(offset, pattern.length()).compare(pattern, m_caseSensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0)
        return offset + pattern.size();
    return offset;
}

MatchResult WordDetect::doMatch(const QString& text, int offset)
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
