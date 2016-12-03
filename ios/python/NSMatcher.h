/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

#import "syntax/matcher.h"

/**
 * A subclass of regina::syntax::RegEx that holds an Objective-C regular
 * expression, designed for working with unicode strings of type NSString.
 */
class NSRegEx : public regina::syntax::RegEx {
    private:
        NSRegularExpression* m_regex;

    public:
        NSRegEx(NSRegularExpression* regex);
        ~NSRegEx();
        NSRegularExpression* regex();
};

/**
 * A subclass of regina::syntax::Matcher designed for matching unicode
 * strings of type NSString.
 */
class NSMatcher : public regina::syntax::Matcher {
    private:
        NSString* m_text;

    public:
        NSMatcher(NSString* text) : m_text(text) {}

        bool textEmpty() const override;
        size_t textSize() const override;

        regina::syntax::MatchResult match(regina::syntax::AnyChar& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::DetectChar& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::Detect2Char& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::DetectIdentifier& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::DetectSpaces& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::Float& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::IncludeRules& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::Int& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::HlCChar& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::HlCHex& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::HlCOct& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::HlCStringChar& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::KeywordListRule& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::LineContinue& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::RangeDetect& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::RegExpr& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::StringDetect& rule, int offset) override;
        regina::syntax::MatchResult match(regina::syntax::WordDetect& rule, int offset) override;

    private:
        bool isDelimiter(regina::syntax::Rule& r, unichar c) const;
};

inline NSRegEx::NSRegEx(NSRegularExpression* regex) : m_regex(regex) {
}

inline NSRegEx::~NSRegEx() {
    // I hope this destroys m_regex - not sure how Obj-C and C++ interact
    // with regard to ARC.
    m_regex = nil;
}

inline NSRegularExpression* NSRegEx::regex() {
    return m_regex;
}

inline bool NSMatcher::textEmpty() const {
    return m_text.length == 0;
}

inline size_t NSMatcher::textSize() const {
    return m_text.length;
}

