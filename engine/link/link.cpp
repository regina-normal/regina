
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "link.h"
#include <sstream>

namespace regina {

Link::Link(const Link& cloneMe) {
    // TODO: Clone.
}

void Link::writeTextShort(std::ostream& out) const {
    if (components_.empty())
        out << "empty link";
    else if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
}

void Link::writeTextLong(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty link" << std::endl;
        return;
    }

    if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
    out << "\n\n";

    int comp = 0;
    CrossingStrand s;
    for (CrossingStrand start : components_) {
        out << "Component " << comp++ << ": ";
        if (! start) {
            out << "no crossings (separated unknot)" << std::endl;
            continue;
        }
        s = start;
        out << s;
        for (++s; s != start; ++s)
            out << ' ' << s;
        out << '\n';
    }

    out << "\nCrossings:";
    for (Crossing* c : crossings_)
        out << ' ' << c->index() << (c->sign() > 0 ? '+' : '-');
    out << std::endl;
}

Link* Link::fromJenkins(std::istream& in) {
    int nComp;
    in >> nComp;
    if (! in)
        return 0;

    if (nComp == 0)
        return new Link;

    Link* ans = new Link;
    ans->components_.resize(nComp);

    // We need to allocate the crossings first.
    // Alas, these appear last in the input.
    // Skip through (and remember) the link components, which appear first.
    int* compLen = new int[nComp];
    int** compInput = new int*[nComp];
    unsigned c, i;
    unsigned foundCrossings = 0;

    for (c = 0; c < nComp; ++c) {
        in >> compLen[c];
        if ((! in) || (compLen[c] < 0)) {
            // Invalid input.
            for (i = 0; i < c; ++i)
                delete[] compInput[i];
            delete[] compInput;
            delete[] compLen;
            delete ans;
            return 0;
        }

        compInput[c] = new int[2 * compLen[c]];
        for (i = 0; i < compLen[c]; ++i) {
            in >> compInput[c][2 * i] >> compInput[c][2 * i + 1];
            if ((! in) || compInput[c][2 * i] < 0 ||
                    (compInput[c][2 * i + 1] != 1 &&
                     compInput[c][2 * i + 1] != -1)) {
                // Invalid input.
                for (i = 0; i <= c; ++i)
                    delete[] compInput[i];
                delete[] compInput;
                delete[] compLen;
                delete ans;
                return 0;
            }
        }

        foundCrossings += compLen[c];
    }

    // *Now* we have the number of crossings.
    if (foundCrossings % 2 != 0) {
        // Invalid input.
        for (i = 0; i < nComp; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        delete ans;
        return 0;
    }
    unsigned nCross = foundCrossings / 2;

    Crossing** tmpCross = new Crossing*[nCross];
    std::fill(tmpCross, tmpCross + nCross, nullptr);

    int label, sign;
    for (i = 0; i < nCross; ++i) {
        in >> label >> sign;
        if ((! in) || (sign != 1 && sign != -1) ||
                label < 0 || label >= nCross || tmpCross[label]) {
            // Invalid input.
            for (i = 0; i < nCross; ++i)
                delete tmpCross[i];
            delete[] tmpCross;
            for (i = 0; i < nComp; ++i)
                delete[] compInput[i];
            delete[] compInput;
            delete[] compLen;
            delete ans;
            return 0;
        }

        tmpCross[label] = new Crossing(sign);
    }

    for (i = 0; i < nCross; ++i)
        ans->crossings_.push_back(tmpCross[i]);
    delete[] tmpCross;

    // Finally, we can connect the crossings together by following the
    // individual link components.
    unsigned startCross, endCross;
    unsigned startStrand, endStrand;
    bool broken = false;
    for (c = 0; c < nComp && ! broken; ++c) {
        for (i = 0; i < compLen[c]; ++i) {
            startCross = compInput[c][2 * i];
            startStrand = (compInput[c][2 * i + 1] > 0 ? 1 : 0);
            endCross = compInput[c][i == compLen[c] - 1 ? 0 : 2 * i + 2];
            endStrand = (compInput[c][i == compLen[c] - 1 ? 1 : 2 * i + 3] > 0 ?
                1 : 0);

            if (ans->crossings_[startCross]->next_[startStrand]) {
                broken = true;
                break;
            }
            ans->crossings_[startCross]->next_[startStrand] =
                CrossingStrand(ans->crossings_[endCross], endStrand);
        }

        ans->components_[c] = CrossingStrand(ans->crossings_[compInput[c][0]],
            (compInput[c][1] > 0 ? 1 : 0));
    }

    // Start cleaning up.
    for (i = 0; i < nComp; ++i)
        delete[] compInput[i];
    delete[] compInput;
    delete[] compLen;

    if (broken) {
        // Invalid input.
        delete ans;
        return 0;
    }

    // Set up prev links to match next links.
    CrossingStrand next;
    for (Crossing* cross : ans->crossings_) {
        next = cross->next_[0];
        next.crossing()->prev_[next.strand()] = CrossingStrand(cross, 0);

        next = cross->next_[1];
        next.crossing()->prev_[next.strand()] = CrossingStrand(cross, 1);
    }

    // All done!
    return ans;
}

Link* Link::fromJenkins(const std::string& str) {
    std::istringstream in(str);
    return fromJenkins(in);
}

void Link::reflect() {
    for (Crossing* cross : crossings_)
        cross->sign_ = -cross->sign_;
}

void Link::rotate() {
    for (CrossingStrand& s : components_)
        s.strand_ ^= 1;

    for (Crossing* cross : crossings_) {
        std::swap(cross->next_[0], cross->next_[1]);
        std::swap(cross->prev_[0], cross->prev_[1]);
        cross->next_[0].strand_ ^= 1;
        cross->next_[1].strand_ ^= 1;
        cross->prev_[0].strand_ ^= 1;
        cross->prev_[1].strand_ ^= 1;
    }
}

void Link::writeXMLPacketData(std::ostream& out) const {
    // TODO
}

} // namespace regina

