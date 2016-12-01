
#ifndef SYNTAX_QTMATCHER_H
#define SYNTAX_QTMATCHER_H

#include "syntax/matcher.h"
#include <QString>
#include <QRegularExpression>

namespace regina {
namespace syntax {

class Rule;

class QtRegEx : public RegEx, public QRegularExpression {
};

class QtMatcher : public Matcher {
    private:
        const QString& m_text;

    public:
        QtMatcher(const QString& text) : m_text(text) {}

        bool textEmpty() const override;
        size_t textSize() const override;

        MatchResult match(AnyChar& rule, int offset) override;
        MatchResult match(DetectChar& rule, int offset) override;
        MatchResult match(Detect2Char& rule, int offset) override;
        MatchResult match(DetectIdentifier& rule, int offset) override;
        MatchResult match(DetectSpaces& rule, int offset) override;
        MatchResult match(Float& rule, int offset) override;
        MatchResult match(IncludeRules& rule, int offset) override;
        MatchResult match(Int& rule, int offset) override;
        MatchResult match(HlCChar& rule, int offset) override;
        MatchResult match(HlCHex& rule, int offset) override;
        MatchResult match(HlCOct& rule, int offset) override;
        MatchResult match(HlCStringChar& rule, int offset) override;
        MatchResult match(KeywordListRule& rule, int offset) override;
        MatchResult match(LineContinue& rule, int offset) override;
        MatchResult match(RangeDetect& rule, int offset) override;
        MatchResult match(RegExpr& rule, int offset) override;
        MatchResult match(StringDetect& rule, int offset) override;
        MatchResult match(WordDetect& rule, int offset) override;

    private:
        bool isDelimiter(Rule& r, QChar c) const;
};

inline bool QtMatcher::textEmpty() const {
    return m_text.isEmpty();
}

inline size_t QtMatcher::textSize() const {
    return m_text.size();
}

} } // namespace regina::syntax

#endif

