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

#ifndef KSYNTAXHIGHLIGHTING_RULE_P_H
#define KSYNTAXHIGHLIGHTING_RULE_P_H

#include "contextswitch_p.h"
#include "definition.h"
#include "definitionref_p.h"
#include "keywordlist_p.h"
#include "matchresult_p.h"

#include <QRegularExpression>

#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace KSyntaxHighlighting {

class Rule : public boost::noncopyable
{
public:
    Rule();
    virtual ~Rule();

    typedef std::shared_ptr<Rule> Ptr;

    Definition definition() const;
    void setDefinition(const Definition &def);

    const std::string& attribute() const;
    ContextSwitch context() const;
    bool isLookAhead() const;
    bool isDynamic() const;
    bool firstNonSpace() const;
    int requiredColumn() const;

    bool load(xmlTextReaderPtr reader);
    void resolveContext();

    MatchResult match(const QString &text, int offset, const QStringList &captures);

    static Rule::Ptr create(const std::string& name);

protected:
    virtual bool doLoad(xmlTextReaderPtr reader);
    virtual MatchResult doMatch(const QString &text, int offset, const QStringList &captures) = 0;

    bool isDelimiter(QChar c) const;

private:
    DefinitionRef m_def;
    std::string m_attribute;
    ContextSwitch m_context;
    std::vector<Rule::Ptr> m_subRules;
    int m_column;
    bool m_firstNonSpace;
    bool m_lookAhead;
    bool m_dynamic;
};


class AnyChar : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;

private:
    std::string m_chars;
};

class DetectChar : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;

private:
    QChar m_char;
    int m_captureIndex;
};

class Detect2Char : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;

private:
    QChar m_char1;
    QChar m_char2;
};

class DetectIdentifier : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class DetectSpaces : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class Float : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class IncludeRules : public Rule
{
public:
    const std::string& contextName() const;
    const std::string& definitionName() const;
    bool includeAttribute() const;

protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;

private:
    std::string m_contextName;
    std::string m_defName;
    bool m_includeAttribute;
};

class Int : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;
};

class HlCChar : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class HlCHex : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class HlCOct : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class HlCStringChar : public Rule
{
protected:
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;
};

class KeywordListRule : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;

private:
    std::string m_listName;
    const KeywordList* m_keywordList; // may be null
    bool m_hasCaseSensitivityOverride;
    Qt::CaseSensitivity m_caseSensitivityOverride;

public:
    KeywordListRule() : m_keywordList(0) {}
};

class LineContinue : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;

private:
    QChar m_char;
};

class RangeDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList&) override;

private:
    QChar m_begin;
    QChar m_end;
};

class RegExpr : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;

private:
    QString m_pattern;
    QRegularExpression m_regexp;
};

class StringDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;

private:
    std::string m_string;
    Qt::CaseSensitivity m_caseSensitivity;
};

class WordDetect : public Rule
{
protected:
    bool doLoad(xmlTextReaderPtr reader) override;
    MatchResult doMatch(const QString & text, int offset, const QStringList &captures) override;

private:
    std::string m_word;
};

}

#endif // KSYNTAXHIGHLIGHTING_RULE_P_H
