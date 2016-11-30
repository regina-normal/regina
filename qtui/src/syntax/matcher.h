
#ifndef MATCHER_H
#define MATCHER_H

namespace KSyntaxHighlighting {

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

class MatchResult
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

class Matcher {
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

}

#endif

