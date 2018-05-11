
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "tangle.h"
#include <algorithm>
#include <sstream>
#include <stack>

static const char TANGLE_HORIZONTAL = '-';
static const char TANGLE_VERTICAL = '|';
static const char TANGLE_DIAGONAL = 'x';

static const char OP_POS_TWIST = '+';
static const char OP_NEG_TWIST = '-';
static const char OP_TURN = 'o';

namespace regina {

Tangle::Tangle(int num, int den) {
    if (den == 0) {
        // Build the infinity tangle.
        type_ = TANGLE_VERTICAL;
        end_[0][0] = end_[0][1] = end_[1][0] = end_[1][1] = StrandRef();
        return;
    }

    // Normalise so the denominator is positive.
    if (den < 0) {
        den = -den;
        num = -num;
    }

    // Find a sequence of twists and turns that brings us back to the
    // zero tangle.
    std::stack<char> ops;

    while (num != 0) {
        if (num <= -den) {
            ops.push(OP_NEG_TWIST);
            num += den;
        } else if (num >= den) {
            ops.push(OP_POS_TWIST);
            num -= den;
        } else if (num < 0 && (num + den) < -num) {
            ops.push(OP_NEG_TWIST);
            num += den;
        } else if (num > 0 && -(num - den) < num) {
            ops.push(OP_POS_TWIST);
            num -= den;
        } else {
            ops.push(OP_TURN);
            std::swap(num, den);
            if (den < 0)
                den = -den;
            else
                num = -num;
        }
    }

    // Now construct the zero tangle and perform the corresponding
    // twists and turns.
    type_ = TANGLE_HORIZONTAL;
    end_[0][0] = end_[0][1] = end_[1][0] = end_[1][1] = StrandRef();

    char op;
    while (! ops.empty()) {
        op = ops.top();
        ops.pop();

        if (op == OP_TURN)
            turn(1);
        else
            twist(op == OP_POS_TWIST ? 1 : -1);
    }
}

Tangle::Tangle(const Tangle& cloneMe) : type_(cloneMe.type_) {
    for (Crossing* c : cloneMe.crossings_)
        crossings_.push_back(new Crossing(c->sign()));

    int i, j;
    auto it = cloneMe.crossings_.begin();
    for (Crossing* c : crossings_) {
        for (i = 0; i < 2; ++i) {
            c->next_[i] = translate((*it)->next_[i]);
            c->prev_[i] = translate((*it)->prev_[i]);
        }
        ++it;
    }

    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j)
            end_[i][j] = translate(cloneMe.end_[i][j]);
}

void Tangle::swapContents(Tangle& other) {
    if (&other == this)
        return;

    std::swap(type_, other.type_);

    crossings_.swap(other.crossings_);

    int i, j;
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j)
            std::swap(end_[i][j], other.end_[i][j]);
}

void Tangle::twist(int sign) {
    Crossing* c;

    if (type_ == TANGLE_HORIZONTAL || type_ == TANGLE_DIAGONAL) {
        c = new Crossing(-sign);

        // Which string attaches to the lower strand of the new crossing?
        int attach = (
            ((sign > 0 && type_ == TANGLE_HORIZONTAL) ||
             (sign < 0 && type_ == TANGLE_DIAGONAL)) ?
            0 : 1);

        for (int s = 0; s < 2; ++s) {
            // Attach c->strand(s) to the appropriate string.
            if (end_[attach][1]) {
                c->prev_[s] = end_[attach][1];
                end_[attach][1].crossing()->
                    next_[end_[attach][1].strand()] = c->strand(s);
                end_[attach][1] = c->strand(s);
            } else
                end_[attach][0] = end_[attach][1] = c->strand(s);

            attach ^= 1;
        }

        if (type_ == TANGLE_HORIZONTAL)
            type_ = TANGLE_DIAGONAL;
        else
            type_ = TANGLE_HORIZONTAL;
    } else {
        // Vertical tangle.
        reverse(1);

        c = new Crossing(sign);

        // In which order do we see the two strands at c when traversing
        // the right-hand string?
        int first, last;
        if (sign > 0) {
            first = 1;
            last = 0;
        } else {
            first = 0;
            last = 1;
        }

        if (end_[1][0]) {
            c->next_[first] = end_[1][1];
            c->prev_[last] = end_[1][0];
            end_[1][1].crossing()->prev_[end_[1][1].strand()] =
                c->strand(first);
            end_[1][0].crossing()->next_[end_[1][0].strand()] =
                c->strand(last);
        } else {
            c->next_[first] = c->strand(last);
            c->prev_[last] = c->strand(first);
        }

        end_[1][0] = c->strand(first);
        end_[1][1] = c->strand(last);
    }

    crossings_.push_back(c);
}

void Tangle::turn(int direction) {
    int i;
    if (type_ == TANGLE_HORIZONTAL) {
        if (direction > 0) {
            // Switch the indexing of the two strings.
            for (i = 0; i < 2; ++i)
                std::swap(end_[0][i], end_[1][i]);
        } else {
            // Reverse each string, but keep the indexing.
            for (i = 0; i < 2; ++i) {
                reverse(i);
                std::swap(end_[i][0], end_[i][1]);
            }
        }
        type_ = TANGLE_VERTICAL;
    } else if (type_ == TANGLE_VERTICAL) {
        if (direction > 0) {
            // Reverse each string, but keep the indexing.
            for (i = 0; i < 2; ++i) {
                reverse(i);
                std::swap(end_[i][0], end_[i][1]);
            }
        } else {
            // Switch the indexing of the two strings.
            for (i = 0; i < 2; ++i)
                std::swap(end_[0][i], end_[1][i]);
        }
        type_ = TANGLE_HORIZONTAL;
    } else {
        // The diagonal case.

        // Reverse one of the strings...
        int toReverse = (direction > 0 ? 0 : 1);
        reverse(toReverse);
        std::swap(end_[toReverse][0], end_[toReverse][1]);

        // ... and also switch the indexing of the two strings.
        for (i = 0; i < 2; ++i)
            std::swap(end_[0][i], end_[1][i]);
    }
}

void Tangle::writeTextShort(std::ostream& out) const {
    out << crossings_.size() << "-crossing ";
    switch (type_) {
        case TANGLE_HORIZONTAL: out << "horizontal"; break;
        case TANGLE_VERTICAL: out << "vertical"; break;
        case TANGLE_DIAGONAL: out << "diagonal"; break;
    }
    out << " tangle";
}

void Tangle::writeTextLong(std::ostream& out) const {
    out << crossings_.size() << "-crossing ";
    switch (type_) {
        case TANGLE_HORIZONTAL: out << "horizontal ( = )"; break;
        case TANGLE_VERTICAL: out << "vertical ( || )"; break;
        case TANGLE_DIAGONAL: out << "diagonal ( X )"; break;
    }
    out << " tangle\n\n";

    StrandRef s;
    for (int i = 0; i < 2; ++i) {
        out << "String " << i << ':';

        s = end_[i][0];
        if (! s)
            out << " no crossings";
        else
            for ( ; s; ++s)
                out << ' ' << s;
        out << std::endl;
    }
}

void Tangle::reverse(int string) {
    Crossing* c;
    for (StrandRef s = end_[string][0]; s; --s) {
        c = s.crossing();

        c->sign_ = - c->sign_;
        std::swap(c->prev_[s.strand()], c->next_[s.strand()]);
    }
}

char Tangle::extractChar(const char* s) {
    if (*s && ! *(s + 1))
        return *s;
    else
        return 0;
}

char Tangle::extractChar(const std::string& s) {
    if (s.length() == 1)
        return *s.begin();
    else
        return 0;
}

} // namespace regina

