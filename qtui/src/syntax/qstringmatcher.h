
#ifndef QSTRINGMATCHER_H
#define QSTRINGMATCHER_H

#include "matcher.h"
#include <QString>

namespace KSyntaxHighlighting {

class QStringMatcher : public Matcher {
    virtual MatchResult match(AnyChar& rule, int offset) override;
    virtual MatchResult match(DetectChar& rule, int offset) override;
    virtual MatchResult match(Detect2Char& rule, int offset) override;
    virtual MatchResult match(DetectIdentifier& rule, int offset) override;
    virtual MatchResult match(DetectSpaces& rule, int offset) override;
    virtual MatchResult match(Float& rule, int offset) override;
    virtual MatchResult match(IncludeRules& rule, int offset) override;
    virtual MatchResult match(Int& rule, int offset) override;
    virtual MatchResult match(HlCChar& rule, int offset) override;
    virtual MatchResult match(HlCHex& rule, int offset) override;
    virtual MatchResult match(HlCOct& rule, int offset) override;
    virtual MatchResult match(HlCStringChar& rule, int offset) override;
    virtual MatchResult match(KeywordListRule& rule, int offset) override;
    virtual MatchResult match(LineContinue& rule, int offset) override;
    virtual MatchResult match(RangeDetect& rule, int offset) override;
    virtual MatchResult match(RegExpr& rule, int offset) override;
    virtual MatchResult match(StringDetect& rule, int offset) override;
    virtual MatchResult match(WordDetect& rule, int offset) override;
};

}

#endif

