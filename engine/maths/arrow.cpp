
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

#include "maths/arrow.h"

namespace regina {

const Laurent<Integer>& Arrow::operator [] (const DiagramSequence& d) const {
    if ((! d.empty()) && d[d.size() - 1] == 0)
        throw InvalidArgument("The given diagram sequence should not "
            "end with a zero");

    auto it = terms_.find(d);
    if (it == terms_.end())
        return RingTraits<Laurent<Integer>>::zero;
    else
        return it->second;
}

void Arrow::set(const DiagramSequence& d, const Laurent<Integer>& value) {
    if ((! d.empty()) && d[d.size() - 1] == 0)
        throw InvalidArgument("The given diagram sequence should not "
            "end with a zero");

    if (value.isZero()) {
        terms_.erase(d);
    } else {
        auto result = terms_.emplace(d, value);
        if (! result.second) {
            // A coefficient was already present.  Change it.
            result.first->second = value;
        }
    }
}

void Arrow::set(const DiagramSequence& d, Laurent<Integer>&& value) {
    if ((! d.empty()) && d[d.size() - 1] == 0)
        throw InvalidArgument("The given diagram sequence should not "
            "end with a zero");

    if (value.isZero()) {
        terms_.erase(d);
    } else {
        // Verified: the code below does indeed move value (not copy it).
        auto result = terms_.try_emplace(d, std::move(value));
        if (! result.second) {
            // A coefficient was already present.  Change it.
            // In this scenario, try_emplace() will not have moved the value out
            // yet, and so value is still usable as an rvalue reference.
            result.first->second = std::move(value);
        }
    }
}

void Arrow::multDiagram(size_t index) {
    if (index == 0)
        throw InvalidArgument("The index of a diagram variable must be "
            "strictly positive");

    // Awkwardly, this operation changes the _keys_ in our map.
    std::map<DiagramSequence, Laurent<Integer>> staging;
    auto it = terms_.begin();
    while (it != terms_.end()) {
        auto h = terms_.extract(it++);
        if (h.key().size() >= index) {
            ++h.key()[index - 1];
        } else {
            DiagramSequence seq(index);
            std::copy(h.key().begin(), h.key().end(), seq.begin());
            std::fill(seq.begin() + h.key().size(), seq.end() - 1, 0);
            seq[index - 1] = 1;
            h.key() = std::move(seq);
        }
        staging.insert(std::move(h));
    }

    staging.swap(terms_);
}

Arrow& Arrow::operator += (const Arrow& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (const auto& entry : other.terms_) {
        auto result = terms_.emplace(entry);
        if (! result.second)
            result.first->second += entry.second;
    }

    // We might have zeroed out some terms.
    removeZeroes();
    return *this;
}

Arrow& Arrow::operator -= (const Arrow& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (const auto entry : other.terms_) {
        auto result = terms_.emplace(entry);
        if (result.second)
            result.first->second.negate();
        else
            result.first->second -= entry.second;
    }

    // We might have zeroed out some terms.
    removeZeroes();
    return *this;
}

void Arrow::writeTextShort(std::ostream& out, bool utf8) const {
    if (isZero()) {
        out << '0';
        return;
    }

    if (terms_.size() == 1) {
        auto it = terms_.begin();
        if (it->first.empty()) {
            // This polynomial does not use any diagram variables at all.
            // Just write the Laurent polynomial, without the usual brackets.
            it->second.writeTextShort(out, utf8, "A");
            return;
        }
    }

    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
        const auto& laurent = it->second;

        if (laurent.minExp() == laurent.maxExp()) {
            // We are just adding some multiple of a single power of A.
            long exp = laurent.minExp();
            auto coeff = laurent[exp];

            if (coeff < 0) {
                if (it == terms_.begin()) {
                    if (utf8)
                        out << "\u2212";
                    else
                        out << '-';
                } else {
                    if (utf8)
                        out << " \u2212 ";
                    else
                        out << " - ";
                }

                coeff.negate();
            } else {
                if (it != terms_.begin())
                    out << " + ";
            }

            if (it->first.empty() && exp == 0) {
                // There are no variables to write at all.
                out << coeff;
                continue;
            }

            // There are some variables (A and/or K_i) to write.
            if (coeff != 1)
                out << coeff << ' ';
            if (exp != 0) {
                out << 'A';
                if (exp != 1) {
                    if (utf8)
                        out << regina::superscript(exp);
                    else
                        out << '^' << exp;
                }

                if (it->first.empty())
                    continue;
                out << ' ';
            }
            // All that's left is to write the sequence of K_i (with no
            // leading space).  We do this below.
        } else {
            if (it != terms_.begin())
                out << " + ";

            out << '(';
            it->second.writeTextShort(out, utf8, "A");
            out << ')';

            if (it->first.empty())
                continue;
            out << ' ';
        }

        bool firstK = true;
        for (size_t i = 0; i < it->first.size(); ++i) {
            size_t exp = it->first[i];
            if (exp == 0)
                continue;

            if (firstK)
                firstK = false;
            else
                out << ' ';
            if (utf8) {
                out << 'K' << regina::subscript(i + 1);
                if (exp != 1)
                    out << regina::superscript(exp);
            } else {
                out << "K_" << (i + 1);
                if (exp != 1)
                    out << '^' << exp;
            }
        }
    }
}

void Arrow::tightEncode(std::ostream& out) const {
    // Write the Laurent polynomials (which must be non-zero) before the
    // diagram sequences.  This way we can use the zero Laurent polynomial
    // as an unambiguous terminator.
    for (const auto& t : terms_) {
        t.second.tightEncode(out);

        regina::tightEncode(out, t.first.size());
        for (auto i : t.first)
            regina::tightEncode(out, i);
    }

    RingTraits<Laurent<Integer>>::zero.tightEncode(out);
}

Arrow Arrow::tightDecode(std::istream& input) {
    Arrow ans;

    while (true) {
        auto coeff = Laurent<Integer>::tightDecode(input);
        if (coeff.isZero())
            return ans;

        size_t len = regina::tightDecode<size_t>(input);
        DiagramSequence seq(len);
        for (size_t& i : seq)
            i = regina::tightDecode<size_t>(input);
        if (len > 0 && seq[len - 1] == 0)
            throw InvalidInput("The tight encoding includes a diagram "
                "sequence ending in zero");

        ans.terms_.emplace(std::move(seq), std::move(coeff));
    }
}

void Arrow::removeZeroes() {
    auto it = terms_.begin();
    while (it != terms_.end())
        if (it->second.isZero())
            it = terms_.erase(it); // C++11: returns next element.
        else
            ++it;
}

}
