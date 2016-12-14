/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  Parts of this file are modified from the KDE syntax-highlighting      *
 *  framework, which is copyright (C) 2016 Volker Krause <vkrause@kde.org>*
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

#import <Foundation/Foundation.h>

#import "NSMatcher.h"

#import "syntax/contextswitch.h"
#import "syntax/definition.h"
#import "syntax/definitionref.h"
#import "syntax/definition_p.h"
#import "syntax/rule.h"

using namespace regina::syntax;

static bool isOctalChar(unichar c)
{
    return [[NSCharacterSet decimalDigitCharacterSet] characterIsMember:c] &&
        c != '9' && c != '8';
}

static bool isHexChar(unichar c)
{
    return[[NSCharacterSet decimalDigitCharacterSet] characterIsMember:c]
        || c == 'a' || c == 'A'
        || c == 'b' || c == 'B'
        || c == 'c' || c == 'C'
        || c == 'd' || c == 'D'
        || c == 'e' || c == 'E'
        || c == 'f' || c == 'F';
}

static int matchEscapedChar(NSString* text, int offset)
{
    if ([text characterAtIndex:offset] != '\\' || text.length < offset + 2)
        return offset;

    const auto c = [text characterAtIndex:(offset + 1)];
    static const NSCharacterSet* controlChars = [NSCharacterSet characterSetWithCharactersInString:@"abefnrtv\"'?\\"];
    if ([controlChars characterIsMember:c])
        return offset + 2;

    if (c == 'x') { // hex encoded character
        auto newOffset = offset + 2;
        for (int i = 0; i < 2 && newOffset + i < text.length; ++i, ++newOffset) {
            if (!isHexChar([text characterAtIndex:newOffset]))
                break;
        }
        if (newOffset == offset + 2)
            return offset;
        return newOffset;
    }

    if (isOctalChar(c)) { // octal encoding
        auto newOffset = offset + 2;
        for (int i = 0; i < 2 && newOffset + i < text.length; ++i, ++newOffset) {
            if (!isOctalChar([text characterAtIndex:newOffset]))
                break;
        }
        if (newOffset == offset + 2)
            return offset;
        return newOffset;
    }

    return offset;
}

bool NSMatcher::isDelimiter(Rule& r, unichar c) const {
    return DefinitionData::get(r.definition())->isDelimiter(c) ||
        [[NSCharacterSet newlineCharacterSet] characterIsMember:c];
}

MatchResult NSMatcher::match(AnyChar& rule, int offset)
{
    if (rule.chars().find([m_text characterAtIndex:offset]) != std::string::npos)
        return offset + 1;
    return offset;
}


MatchResult NSMatcher::match(DetectChar& rule, int offset)
{
    if ([m_text characterAtIndex:offset] == rule.matchChar())
        return offset + 1;
    return offset;
}


MatchResult NSMatcher::match(Detect2Char& rule, int offset)
{
    if (m_text.length - offset < 2)
        return offset;
    if ([m_text characterAtIndex:offset] == rule.matchChar1() && [m_text characterAtIndex:(offset + 1)] == rule.matchChar2())
        return offset + 2;
    return offset;
}


MatchResult NSMatcher::match(DetectIdentifier& rule, int offset)
{
    if (! [[NSCharacterSet letterCharacterSet] characterIsMember:[m_text characterAtIndex:offset]] &&
            [m_text characterAtIndex:offset] != '_')
        return offset;

    for (int i = offset + 1; i < m_text.length; ++i) {
        const auto c = [m_text characterAtIndex:i];
        if (! [[NSCharacterSet alphanumericCharacterSet] characterIsMember:c] && c != '_')
            return i;
    }

    return m_text.length;
}


MatchResult NSMatcher::match(DetectSpaces& rule, int offset)
{
    // TODO: Check that whitespaceAndNewline is what we need.
    while(offset < m_text.length && [[NSCharacterSet whitespaceAndNewlineCharacterSet]
                                     characterIsMember:[m_text characterAtIndex:offset]])
        ++offset;
    return offset;
}


MatchResult NSMatcher::match(Float& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    auto newOffset = offset;
    while (newOffset < m_text.length && [[NSCharacterSet decimalDigitCharacterSet]
                                         characterIsMember:[m_text characterAtIndex:newOffset]])
        ++newOffset;

    if (newOffset >= m_text.length || [m_text characterAtIndex:newOffset] != '.')
        return offset;
    ++newOffset;

    while (newOffset < m_text.length && [[NSCharacterSet decimalDigitCharacterSet]
                                         characterIsMember:[m_text characterAtIndex:newOffset]])
        ++newOffset;

    if (newOffset == offset + 1) // we only found a decimal point
        return offset;

    auto expOffset = newOffset;
    if (expOffset >= m_text.length || ([m_text characterAtIndex:expOffset] != 'e' && [m_text characterAtIndex:expOffset] != 'E'))
        return newOffset;
    ++expOffset;

    if (expOffset < m_text.length && ([m_text characterAtIndex:expOffset] == '+' || [m_text characterAtIndex:expOffset] == '-'))
        ++expOffset;
    bool foundExpDigit = false;
    while (expOffset < m_text.length && [[NSCharacterSet decimalDigitCharacterSet]
                                         characterIsMember:[m_text characterAtIndex:expOffset]]) {
        ++expOffset;
        foundExpDigit = true;
    }

    if (!foundExpDigit)
        return newOffset;
    return expOffset;
}


MatchResult NSMatcher::match(HlCChar& rule, int offset)
{
    if (m_text.length < offset + 3)
        return offset;

    if ([m_text characterAtIndex:offset] != '\'' || [m_text characterAtIndex:(offset + 1)] == '\'')
        return offset;

    auto newOffset = matchEscapedChar(m_text, offset + 1);
    if (newOffset == offset + 1) {
        if ([m_text characterAtIndex:newOffset] == '\\')
            return offset;
        else
            ++newOffset;
    }
    if (newOffset >= m_text.length)
        return offset;

    if ([m_text characterAtIndex:newOffset] == '\'')
        return newOffset + 1;

    return offset;
}


MatchResult NSMatcher::match(HlCHex& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    if (m_text.length < offset + 3)
        return offset;

    if ([m_text characterAtIndex:offset] != '0' || ([m_text characterAtIndex:(offset + 1)] != 'x' && [m_text characterAtIndex:(offset + 1)] != 'X'))
        return offset;

    if (!isHexChar([m_text characterAtIndex:(offset + 2)]))
        return offset;

    offset += 3;
    while (offset < m_text.length && isHexChar([m_text characterAtIndex:offset]))
        ++offset;

    // TODO Kate matches U/L suffix, QtC does not?

    return offset;
}


MatchResult NSMatcher::match(HlCOct& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    if (m_text.length < offset + 2)
        return offset;

    if ([m_text characterAtIndex:offset] != '0')
        return offset;

    if (!isOctalChar([m_text characterAtIndex:(offset + 1)]))
        return offset;

    offset += 2;
    while (offset < m_text.length && isOctalChar([m_text characterAtIndex:offset]))
        ++offset;

    return offset;
}


MatchResult NSMatcher::match(HlCStringChar& rule, int offset)
{
    return matchEscapedChar(m_text, offset);
}

MatchResult NSMatcher::match(IncludeRules& rule, int offset)
{
    NSLog(@"Unresolved include rule for %s##%s", rule.contextName().c_str(), rule.definitionName().c_str());
    return offset;
}


MatchResult NSMatcher::match(Int& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    while(offset < m_text.length && [[NSCharacterSet decimalDigitCharacterSet]
                                     characterIsMember:[m_text characterAtIndex:offset]])
        ++offset;
    return offset;
}

MatchResult NSMatcher::match(KeywordListRule& rule, int offset)
{
    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    auto keywordList = rule.keywordList();

    auto newOffset = offset;
    while (m_text.length > newOffset && !isDelimiter(rule, [m_text characterAtIndex:newOffset]))
        ++newOffset;
    if (newOffset == offset)
        return offset;

    NSString* substr = [m_text substringWithRange:NSMakeRange(offset, newOffset - offset)];
    if (rule.caseSensitivity()) {
        if (keywordList->contains(substr.UTF8String, true))
            return newOffset;
    } else {
        // We must convert the string to lower-case before calling contains().
        if (keywordList->contains(substr.lowercaseString.UTF8String, false))
            return newOffset;
    }
    return offset;
}

MatchResult NSMatcher::match(LineContinue& rule, int offset)
{
    if (offset == m_text.length - 1 && [m_text characterAtIndex:offset] == rule.continueChar())
        return offset + 1;
    return offset;
}

MatchResult NSMatcher::match(RangeDetect& rule, int offset)
{
    if (m_text.length - offset < 2)
        return offset;
    if ([m_text characterAtIndex:offset] != rule.begin())
        return offset;

    auto newOffset = offset + 1;
    while (newOffset < m_text.length) {
        if ([m_text characterAtIndex:newOffset] == rule.end())
            return newOffset + 1;
        ++newOffset;
    }
    return offset;
}

MatchResult NSMatcher::match(RegExpr& rule, int offset)
{
    RegEx* re = rule.regex();
    if (! (re && dynamic_cast<NSRegEx*>(re))) {
        NSRegularExpression* r = [NSRegularExpression
                                  regularExpressionWithPattern:[NSString stringWithUTF8String:rule.pattern().c_str()]
                                  options:(rule.caseInsensitive() ? NSRegularExpressionCaseInsensitive : 0)
                                  error:nil /* no error checking for now */];

        rule.replaceRegEx(re = new NSRegEx(r));
    }

    NSRegularExpression* r = static_cast<NSRegEx*>(re)->regex();

    NSRange result = [r rangeOfFirstMatchInString:m_text
                                          options:0
                                            range:NSMakeRange(offset, m_text.length - offset)];

    // TODO: We aren't using minimal(), which in Qt sets InvertedGreedinessOption.
    // This option is not used by the python highlighting rules, so we ignore it for now.

    if (result.location == offset)
        return MatchResult(offset + result.length);
    if (result.location == NSNotFound)
        return MatchResult(offset, -1);
    // Be kind: if the rule matched later in the string, remember this so we don't check again
    // until we reach that position.
    return MatchResult(offset, result.location);
}

MatchResult NSMatcher::match(StringDetect& rule, int offset)
{
    if (offset + rule.string().length() > m_text.length)
        return offset;

    if ([m_text compare:[NSString stringWithUTF8String:rule.string().c_str()]
                options:(rule.caseSensitivity() ? 0 /* NSLiteralSearch could be faster? */ : NSCaseInsensitiveSearch)
                  range:NSMakeRange(offset, rule.string().length())] == NSOrderedSame)
        return offset + rule.string().length();
    return offset;
}

MatchResult NSMatcher::match(WordDetect& rule, int offset)
{
    if (m_text.length - offset < rule.word().length())
        return offset;

    if (offset > 0 && !isDelimiter(rule, [m_text characterAtIndex:(offset - 1)]))
        return offset;

    if ([m_text compare:[NSString stringWithUTF8String:rule.word().c_str()]
                options:NSLiteralSearch
                  range:NSMakeRange(offset, rule.word().length())] != NSOrderedSame)
        return offset;

    if (m_text.length == offset + rule.word().length() || isDelimiter(rule, [m_text characterAtIndex:(offset + rule.word().length())]))
        return offset + rule.word().length();

    return offset;
}
