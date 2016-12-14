
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/**
 * \file syntax/matcher.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_MATCHER_H
#ifndef __DOXYGEN
#define __SYNTAX_MATCHER_H
#endif

#include "regina-core.h"
#include <cstddef>
#include <boost/noncopyable.hpp>

namespace regina {
namespace syntax {

class Rule;

class AnyChar;
class DetectChar;
class Detect2Char;
class DetectIdentifier;
class DetectSpaces;
class Float;
class IncludeRules;
class Int;
class HlCChar;
class HlCHex;
class HlCOct;
class HlCStringChar;
class KeywordListRule;
class LineContinue;
class RangeDetect;
class RegExpr;
class StringDetect;
class WordDetect;

class REGINA_API RegEx : public boost::noncopyable {
    public:
        virtual ~RegEx() {}
};

class REGINA_API MatchResult
{
    public:
        MatchResult(int offset); // implicit
        explicit MatchResult(int offset, int skipOffset);

        int offset() const;
        int skipOffset() const;

    private:
        int m_offset;
        int m_skipOffset;
};

class REGINA_API Matcher : public boost::noncopyable {
    public:
        virtual bool textEmpty() const = 0;
        virtual size_t textSize() const = 0;

        virtual MatchResult match(AnyChar& rule, int offset) = 0;
        virtual MatchResult match(DetectChar& rule, int offset) = 0;
        virtual MatchResult match(Detect2Char& rule, int offset) = 0;
        virtual MatchResult match(DetectIdentifier& rule, int offset) = 0;
        virtual MatchResult match(DetectSpaces& rule, int offset) = 0;
        virtual MatchResult match(Float& rule, int offset) = 0;
        virtual MatchResult match(IncludeRules& rule, int offset) = 0;
        virtual MatchResult match(Int& rule, int offset) = 0;
        virtual MatchResult match(HlCChar& rule, int offset) = 0;
        virtual MatchResult match(HlCHex& rule, int offset) = 0;
        virtual MatchResult match(HlCOct& rule, int offset) = 0;
        virtual MatchResult match(HlCStringChar& rule, int offset) = 0;
        virtual MatchResult match(KeywordListRule& rule, int offset) = 0;
        virtual MatchResult match(LineContinue& rule, int offset) = 0;
        virtual MatchResult match(RangeDetect& rule, int offset) = 0;
        virtual MatchResult match(RegExpr& rule, int offset) = 0;
        virtual MatchResult match(StringDetect& rule, int offset) = 0;
        virtual MatchResult match(WordDetect& rule, int offset) = 0;
};

inline MatchResult::MatchResult(int offset) :
        m_offset(offset), m_skipOffset(0) {
}

inline MatchResult::MatchResult(int offset, int skipOffset) :
        m_offset(offset), m_skipOffset(skipOffset) {
}

inline int MatchResult::offset() const {
    return m_offset;
}

inline int MatchResult::skipOffset() const {
    return m_skipOffset;
}

} } // namespace regina::syntax

#endif

