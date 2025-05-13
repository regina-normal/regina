
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "link/tangle.h"
#include "utilities/stringutils.h"
#include <cctype>
#include <climits>
#include <iterator>

// We wish to use std::to_chars(), but GCC only implements it in gcc-8.1.
// We will need a fallback option for (in particular) gcc-7.
//
// We only test the presence of the <charconv> include, not the feature test
// macro __cpp_lib_to_chars, since although gcc-8.1 implements std::to_chars()
// for integer types (which is enough for us), a full implementation for all
// types (with the feature test macro now defined) only appeared in gcc-11.1.
//
// Note that macOS provides <charconv> but only supports std::to_chars() when
// targeting macOS 10.15 or above.  However, macOS 10.14 is now past its
// end-of-life (as of October 2021); moreover, other parts of Regina also
// require macOS 10.15 (e.g., parts that use std::filesystem::path).
// So on macOS we will assume that std::to_chars() is available.
//
#if __has_include(<charconv>)
  #include <charconv>
  // We can use std::to_chars().
  #define __regina_has_to_chars 1
#else
  #warning This compiler does not support std::to_chars().
  // Fall back to snprintf().
  #include <cstdio>
  #undef __regina_has_to_chars
#endif

namespace regina {

Link Link::fromGauss(const std::string& s) {
    // Work with the largest integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;
    std::istringstream in(s);
    std::vector<Int> terms;

    Int i;
    while (true) {
        in >> i;
        if (! in) {
            if (in.eof())
                break;
            throw InvalidArgument("fromGauss(): invalid integer in sequence");
        }
        terms.push_back(i);
    }
    return fromGauss(terms.begin(), terms.end());
}

Link Link::fromOrientedGauss(const std::string& s) {
    std::vector<std::string> terms = basicTokenise(s);
    return fromOrientedGauss(terms.begin(), terms.end());
}

Link Link::fromSignedGauss(const std::string& s) {
    // TODO: Make this a vector of substrings, now we have C++20.
    std::vector<std::string> terms;

    std::string::size_type len = s.length();
    std::string::size_type pos = 0;

    // Skip initial whitespace.
    while (pos < len && isspace(s[pos]))
        ++pos;
    if (pos == len)
        return { 1 }; // Zero-crossing unknot

    // Extract each token.
    std::string::size_type tokStart;
    while (pos < len) {
        // Find the characters making up this token.
        tokStart = pos;
        while (pos < len && s[pos] != '+' && s[pos] != '-')
            ++pos;
        if (pos == len) {
            // We never found the terminating sign.
            // The only scenario where this is acceptable is if the leftover
            // characters are entirely whitespace.
            for ( ; tokStart < len; ++tokStart)
                if (! isspace(s[tokStart]))
                    throw InvalidArgument("fromSignedGauss(): unexpected "
                        "characters at the end of the code");
            // We're fine; this was legitimately the end of the list of terms.
            break;
        }
        ++pos;
        terms.push_back(s.substr(tokStart, pos - tokStart));
    }

    // Now build the link!
    return fromSignedGauss(terms.begin(), terms.end());
}

bool Link::parseOrientedGaussTerm(const std::string& s,
        size_t nCross, size_t& crossing, int& strand, int& sign) {
    if (s.length() < 3)
        return false;

    if (s[0] == '+')
        strand = 1;
    else if (s[0] == '-')
        strand = 0;
    else
        return false;

    if (s[1] == '<')
        sign = (strand == 1 ? 1 : -1);
    else if (s[1] == '>')
        sign = (strand == 1 ? -1 : 1);
    else
        return false;

    if (! valueOf(s.substr(2), crossing))
        return false;

    return (crossing > 0 && crossing <= nCross);
}

bool Link::parseSignedGaussTerm(const std::string& s,
        size_t nCross, size_t& crossing, int& strand, int& sign) {
    size_t len = s.length();
    if (len < 3)
        return false;

    if (s[0] == 'U' || s[0] == 'u')
        strand = 0;
    else if (s[0] == 'O' || s[0] == 'o')
        strand = 1;
    else
        return false;

    if (s[len - 1] == '+')
        sign = 1;
    else if (s[len - 1] == '-')
        sign = -1;
    else
        return false;

    if (! valueOf(s.substr(1, len - 2), crossing))
        return false;

    return (crossing > 0 && crossing <= nCross);
}

std::string Link::gauss() const {
    std::ostringstream out;
    gauss(out);
    return out.str();
}

void Link::gauss(std::ostream& out) const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return;

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        if (s != start)
            out << ' ';

        if (s.strand() == 0)
            out << '-';
        out << (s.crossing()->index() + 1);

        ++s;
    } while (s != start);
}

std::vector<int> Link::gaussData() const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return {};
    if (crossings_.size() > INT_MAX)
        throw NotImplemented("This Gauss code has entries that cannot "
            "fit into a C++ int");

    std::vector<int> ans;
    ans.reserve(crossings_.size() * 2);

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        if (s.strand() == 0)
            ans.push_back(- static_cast<int>(s.crossing()->index() + 1));
        else
            ans.push_back(static_cast<int>(s.crossing()->index() + 1));
        ++s;
    } while (s != start);

    return ans;
}

std::string Link::orientedGauss() const {
    std::ostringstream out;
    orientedGauss(out);
    return out.str();
}

void Link::orientedGauss(std::ostream& out) const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return;

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        if (s != start)
            out << ' ';

        if (s.strand() == 0)
            out << '-';
        else
            out << '+';
        if ((s.strand() == 0 && s.crossing()->sign() < 0) ||
                (s.strand() == 1 && s.crossing()->sign() > 0))
            out << '<';
        else
            out << '>';
        out << (s.crossing()->index() + 1);

        ++s;
    } while (s != start);
}

std::vector<std::string> Link::orientedGaussData() const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return {};

    std::vector<std::string> ans;
    ans.reserve(2 * crossings_.size());

    // It seems safe to use 2^64 as an upper bound on the number of crossings.
    // On typical machines, size_t should not exceed this; moreover, even if
    // we did have more crossings than this, none of Regina's algorithms
    // would ever finish for a knot of this size.
    //
    // Since 2^64 is a 20-digit number in base 10, this gives a maximum token
    // length of 22 (allowing for the prefixes +> +< -> -<).
    static constexpr int maxTokenLen = 22;
    char token[maxTokenLen + 1]; // allow for null termination

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        token[0] = (s.strand() == 0 ? '-' : '+');
        token[1] = ((s.strand() == 0 && s.crossing()->sign() < 0) ||
            (s.strand() == 1 && s.crossing()->sign() > 0) ? '<' : '>');
#if __regina_has_to_chars
        auto result = std::to_chars(token + 2, token + maxTokenLen,
            s.crossing()->index() + 1);
        if (result.ec != std::errc())
            throw ImpossibleScenario("Could not convert crossing index "
                " to a string via std::to_chars()");
        *result.ptr = 0;
#else
        int result = snprintf(token + 2,
            maxTokenLen - 1 /* includes null terminator */, "%zu",
            s.crossing()->index() + 1);
        if (result < 0 || result >= maxTokenLen - 1)
            throw ImpossibleScenario("Could not convert crossing index "
                " to a string via snprintf()");
#endif
        ans.emplace_back(token);

        ++s;
    } while (s != start);

    return ans;
}

std::string Tangle::orientedGauss() const {
    std::ostringstream out;
    orientedGauss(out);
    return out.str();
}

void Tangle::orientedGauss(std::ostream& out) const {
    out << type_;

    for (int i = 0; i < 2; ++i) {
        for (StrandRef s = end_[i][0]; s; ++s) {
            out << ' ';

            if (s.strand() == 0)
                out << '-';
            else
                out << '+';

            if ((s.strand() == 0 && s.crossing()->sign() < 0) ||
                    (s.strand() == 1 && s.crossing()->sign() > 0))
                out << '<';
            else
                out << '>';
            out << (s.crossing()->index() + 1);
        }

        if (i == 0)
            out << " _";
    }
}

Tangle Tangle::fromOrientedGauss(const std::string& s) {
    std::vector<std::string> terms = basicTokenise(s);
    return fromOrientedGauss(terms.begin(), terms.end());
}

std::string Link::signedGauss() const {
    std::ostringstream out;
    signedGauss(out);
    return out.str();
}

void Link::signedGauss(std::ostream& out) const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return;

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        if (s.strand() == 0)
            out << 'U';
        else
            out << 'O';

        out << (s.crossing()->index() + 1);

        if (s.crossing()->sign() > 0)
            out << '+';
        else
            out << '-';

        ++s;
    } while (s != start);
}

std::vector<std::string> Link::signedGaussData() const {
    if (components_.size() != 1)
        throw NotImplemented(
            "Gauss codes are only implemented for single-component links");
    if (crossings_.empty())
        return {};

    std::vector<std::string> ans;
    ans.reserve(2 * crossings_.size());

    // It seems safe to use 2^64 as an upper bound on the number of crossings.
    // On typical machines, size_t should not exceed this; moreover, even if
    // we did have more crossings than this, none of Regina's algorithms
    // would ever finish for a knot of this size.
    //
    // Since 2^64 is a 20-digit number in base 10, this gives a maximum token
    // length of 22 (allowing for the prefix U/O and the suffix +/-).
    static constexpr int maxTokenLen = 22;
    char token[maxTokenLen + 1]; // allow for null termination

    StrandRef start = components_.front();
    StrandRef s = start;
    do {
        token[0] = (s.strand() == 0 ? 'U' : 'O');
#if __regina_has_to_chars
        auto result = std::to_chars(token + 1, token + maxTokenLen - 1,
            s.crossing()->index() + 1);
        if (result.ec != std::errc())
            throw ImpossibleScenario("Could not convert crossing index "
                " to a string via std::to_chars()");
        *(result.ptr++) = (s.crossing()->sign() > 0 ? '+' : '-');
        *result.ptr = 0;
#else
        int result = snprintf(token + 1,
            maxTokenLen - 2 /* includes null terminator */, "%zu",
            s.crossing()->index() + 1);
        if (result < 0 || result >= maxTokenLen - 1)
            throw ImpossibleScenario("Could not convert crossing index "
                " to a string via snprintf()");
        token[result + 1] = (s.crossing()->sign() > 0 ? '+' : '-');
        token[result + 2] = 0;
#endif
        ans.emplace_back(token);

        ++s;
    } while (s != start);

    return ans;
}

} // namespace regina

