
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

#include "syntax/contextswitch.h"
#include "syntax/definition.h"
#include "syntax/definitionref.h"
#include "syntax/definition_p.h"
#include "syntax/qtmatcher.h"
#include "syntax/rule.h"

#include <cassert>
#include <iostream>

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

bool QtMatcher::isDelimiter(regina::syntax::Rule& r, QChar c) const {
    return regina::syntax::DefinitionData::get(r.definition())->isDelimiter(c.toLatin1());
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::AnyChar& rule, int offset)
{
    if (rule.chars().find(m_text.at(offset).toLatin1()) != std::string::npos)
        return offset + 1;
    return offset;
}


regina::syntax::MatchResult QtMatcher::match(regina::syntax::DetectChar& rule, int offset)
{
    if (m_text.at(offset) == rule.matchChar())
        return offset + 1;
    return offset;
}


regina::syntax::MatchResult QtMatcher::match(regina::syntax::Detect2Char& rule, int offset)
{
    if (m_text.size() - offset < 2)
        return offset;
    if (m_text.at(offset) == rule.matchChar1() && m_text.at(offset + 1) == rule.matchChar2())
        return offset + 2;
    return offset;
}


regina::syntax::MatchResult QtMatcher::match(regina::syntax::DetectIdentifier& rule, int offset)
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


regina::syntax::MatchResult QtMatcher::match(regina::syntax::DetectSpaces& rule, int offset)
{
    while(offset < m_text.size() && m_text.at(offset).isSpace())
        ++offset;
    return offset;
}


regina::syntax::MatchResult QtMatcher::match(regina::syntax::Float& rule, int offset)
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


regina::syntax::MatchResult QtMatcher::match(regina::syntax::HlCChar& rule, int offset)
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


regina::syntax::MatchResult QtMatcher::match(regina::syntax::HlCHex& rule, int offset)
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


regina::syntax::MatchResult QtMatcher::match(regina::syntax::HlCOct& rule, int offset)
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


regina::syntax::MatchResult QtMatcher::match(regina::syntax::HlCStringChar& rule, int offset)
{
    return matchEscapedChar(m_text, offset);
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::IncludeRules& rule, int offset)
{
    std::cerr << "Unresolved include rule for" << rule.contextName() << "##" << rule.definitionName() << std::endl;
    return offset;
}


regina::syntax::MatchResult QtMatcher::match(regina::syntax::Int& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, m_text.at(offset - 1)))
        return offset;

    while(offset < m_text.size() && m_text.at(offset).isDigit())
        ++offset;
    return offset;
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::KeywordListRule& rule, int offset)
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

regina::syntax::MatchResult QtMatcher::match(regina::syntax::LineContinue& rule, int offset)
{
    if (offset == m_text.size() - 1 && m_text.at(offset) == rule.continueChar())
        return offset + 1;
    return offset;
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::RangeDetect& rule, int offset)
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

regina::syntax::MatchResult QtMatcher::match(regina::syntax::RegExpr& rule, int offset)
{
    regina::syntax::RegEx* re = rule.regex();
    if (! (re && dynamic_cast<QtRegEx*>(re))) {
        QtRegEx* qre = new QtRegEx;

        qre->setPattern(QString::fromUtf8(rule.pattern().c_str()));
        qre->setPatternOptions(
            (rule.minimal() ? QRegularExpression::InvertedGreedinessOption :
                QRegularExpression::NoPatternOption) |
            (rule.caseInsensitive() ? QRegularExpression::CaseInsensitiveOption :
                QRegularExpression::NoPatternOption));

        assert(qre->isValid());
        rule.replaceRegEx(re = qre);
    }

    auto result = static_cast<QtRegEx*>(re)->match(m_text, offset, QRegularExpression::NormalMatch, QRegularExpression::DontCheckSubjectStringMatchOption);

    if (result.capturedStart() == offset)
        return regina::syntax::MatchResult(offset + result.capturedLength());
    // Be kind: if the rule matched later in the string, remember this so we don't check again
    // until we reach that position.
    return regina::syntax::MatchResult(offset, result.capturedStart());
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::StringDetect& rule, int offset)
{
    QString pattern = QString::fromUtf8(rule.string().c_str());
    if (m_text.midRef(offset, pattern.length()).compare(pattern, rule.caseSensitivity() ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0)
        return offset + pattern.size();
    return offset;
}

regina::syntax::MatchResult QtMatcher::match(regina::syntax::WordDetect& rule, int offset)
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
