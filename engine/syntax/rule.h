
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

/**
 * \file syntax/rule.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_RULE_H
#ifndef __DOXYGEN
#define __SYNTAX_RULE_H
#endif

#include "regina-core.h"
#include "syntax/contextswitch.h"
#include "syntax/definition.h"
#include "syntax/definitionref.h"
#include "syntax/keywordlist.h"
#include "syntax/matcher.h"

#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace regina {
namespace syntax {

class REGINA_API Rule : public boost::noncopyable
{
public:
    Rule();
    virtual ~Rule();

    typedef std::shared_ptr<Rule> Ptr;

    Definition definition() const;
    void setDefinition(const Definition &def);

    const std::string& attribute() const;
    const ContextSwitch& context() const;
    bool isLookAhead() const;
    bool firstNonSpace() const;
    int requiredColumn() const;

    bool load(xmlTextReaderPtr reader);
    void resolveContext();

    MatchResult match(Matcher& m, int offset);

    static Rule::Ptr create(const std::string& name);

protected:
    virtual bool doLoad(xmlTextReaderPtr reader);
    virtual MatchResult doMatch(Matcher& m, int offset) = 0;

private:
    DefinitionRef m_def;
    std::string m_attribute;
    ContextSwitch m_context;
    std::vector<Rule::Ptr> m_subRules;
    int m_column;
    bool m_firstNonSpace;
    bool m_lookAhead;
};


class REGINA_API AnyChar : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_chars;

public:
    const std::string& chars() const;
};

class REGINA_API DetectChar : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    char m_char;

public:
    char matchChar() const;
};

class REGINA_API Detect2Char : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    char m_char1;
    char m_char2;

public:
    char matchChar1() const;
    char matchChar2() const;
};

class REGINA_API DetectIdentifier : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API DetectSpaces : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API Float : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API IncludeRules : public Rule
{
public:
    const std::string& contextName() const;
    const std::string& definitionName() const;
    bool includeAttribute() const;

protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_contextName;
    std::string m_defName;
    bool m_includeAttribute;
};

class REGINA_API Int : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API HlCChar : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API HlCHex : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API HlCOct : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API HlCStringChar : public Rule
{
protected:
    MatchResult doMatch(Matcher& m, int offset) override;
};

class REGINA_API KeywordListRule : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_listName;
    const KeywordList* m_keywordList; // may be null
    bool m_hasCaseSensitivityOverride;
    bool m_caseSensitivityOverride;

public:
    KeywordListRule() : m_keywordList(0) {}
    const KeywordList* keywordList(); // may load the list; return value is non-null
    bool caseSensitivity() const;
};

class REGINA_API LineContinue : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    char m_char;

public:
    char continueChar() const;
};

class REGINA_API RangeDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    char m_begin;
    char m_end;

public:
    char begin() const;
    char end() const;
};

class REGINA_API RegExpr : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_pattern;
    bool m_minimal;
    bool m_caseInsensitive;
    RegEx* m_regex; // may be null

public:
    RegExpr() : m_regex(0) {}
    ~RegExpr() { delete m_regex; }
    const std::string& pattern() const;
    bool minimal() const;
    bool caseInsensitive() const;
    RegEx* regex(); // may return null

    void replaceRegEx(RegEx* re);
};

class REGINA_API StringDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_string;
    bool m_caseSensitivity;

public:
    const std::string& string() const;
    bool caseSensitivity() const;
};

class REGINA_API WordDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(Matcher& m, int offset) override;

private:
    std::string m_word;

public:
    const std::string& word() const;
};

inline const std::string& AnyChar::chars() const {
    return m_chars;
}

inline MatchResult AnyChar::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline char DetectChar::matchChar() const {
    return m_char;
}

inline MatchResult DetectChar::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline char Detect2Char::matchChar1() const {
    return m_char1;
}

inline char Detect2Char::matchChar2() const {
    return m_char2;
}

inline MatchResult Detect2Char::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult DetectIdentifier::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult DetectSpaces::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult Float::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult IncludeRules::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult Int::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult HlCChar::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult HlCHex::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult HlCOct::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline MatchResult HlCStringChar::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline bool KeywordListRule::caseSensitivity() const {
    if (m_hasCaseSensitivityOverride)
        return m_caseSensitivityOverride;
    else
        return m_keywordList->caseSensitive();
}

inline MatchResult KeywordListRule::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline char LineContinue::continueChar() const {
    return m_char;
}

inline MatchResult LineContinue::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline char RangeDetect::begin() const {
    return m_begin;
}

inline char RangeDetect::end() const {
    return m_end;
}

inline MatchResult RangeDetect::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline const std::string& RegExpr::pattern() const {
    return m_pattern;
}

inline bool RegExpr::minimal() const {
    return m_minimal;
}

inline bool RegExpr::caseInsensitive() const {
    return m_caseInsensitive;
}

inline RegEx* RegExpr::regex() {
    return m_regex;
}

inline void RegExpr::replaceRegEx(RegEx* re) {
    delete m_regex;
    m_regex = re;
}

inline MatchResult RegExpr::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline const std::string& StringDetect::string() const {
    return m_string;
}

inline bool StringDetect::caseSensitivity() const {
    return m_caseSensitivity;
}

inline MatchResult StringDetect::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

inline const std::string& WordDetect::word() const {
    return m_word;
}

inline MatchResult WordDetect::doMatch(Matcher& m, int offset) {
    return m.match(*this, offset);
}

} } // namespace regina::syntax

#endif
