
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <algorithm>
#include "census/dim4gluingperms.h"
#include "dim4/dim4triangulation.h"
#include "utilities/stringutils.h"

namespace regina {

Dim4GluingPerms::Dim4GluingPerms(const Dim4GluingPerms& cloneMe) :
        pairing_(cloneMe.pairing_), inputError_(false) {
    unsigned nPent = cloneMe.getNumberOfPentachora();

    permIndices_ = new int[nPent * 5];
    std::copy(cloneMe.permIndices_, cloneMe.permIndices_ + nPent * 5,
        permIndices_);
}

Dim4Triangulation* Dim4GluingPerms::triangulate() const {
    unsigned nPent = getNumberOfPentachora();

    Dim4Triangulation* ans = new Dim4Triangulation;
    Dim4Pentachoron** pent = new Dim4Pentachoron*[nPent];
    std::generate(pent, pent + nPent, FuncNew<Dim4Pentachoron>());

    unsigned p, facet;
    for (p = 0; p < nPent; ++p)
        for (facet = 0; facet < 5; ++facet)
            if ((! pairing_->isUnmatched(p, facet)) &&
                    (! pent[p]->adjacentPentachoron(facet)))
                pent[p]->joinTo(facet, pent[pairing_->dest(p, facet).pent],
                    gluingPerm(p, facet));

    for (p = 0; p < nPent; ++p)
        ans->addPentachoron(pent[p]);

    delete[] pent;
    return ans;
}

int Dim4GluingPerms::gluingToIndex(const Dim4PentFacet& source,
        const NPerm5& gluing) const {
    NPerm4 permS4 = perm5to4(NPerm5(pairing_->dest(source).facet, 4) *
        gluing * NPerm5(source.facet, 4));
    return (std::find(NPerm4::S4, NPerm4::S4 + 24, permS4) - NPerm4::S4);
}

int Dim4GluingPerms::gluingToIndex(unsigned pent, unsigned facet,
        const NPerm5& gluing) const {
    NPerm4 permS4 = perm5to4(NPerm5(pairing_->dest(pent, facet).facet, 4) *
        gluing * NPerm5(facet, 4));
    return (std::find(NPerm4::S4, NPerm4::S4 + 24, permS4) - NPerm4::S4);
}

void Dim4GluingPerms::dumpData(std::ostream& out) const {
    out << pairing_->toTextRep() << std::endl;

    unsigned pent, facet;
    for (pent = 0; pent < getNumberOfPentachora(); ++pent)
        for (facet = 0; facet < 5; ++facet) {
            if (pent || facet)
                out << ' ';
            out << permIndex(pent, facet);
        }
    out << std::endl;
}

Dim4GluingPerms::Dim4GluingPerms(std::istream& in) :
        pairing_(0), permIndices_(0), inputError_(false) {
    // Remember that we can safely abort before allocating arrays, since C++
    // delete tests for nullness.
    std::string line;

    // Skip initial whitespace to find the facet pairing.
    while (true) {
        std::getline(in, line);
        if (in.eof()) {
            inputError_ = true; return;
        }
        line = regina::stripWhitespace(line);
        if (line.length() > 0)
            break;
    }

    pairing_ = Dim4FacetPairing::fromTextRep(line);
    if (! pairing_) {
        inputError_ = true; return;
    }

    unsigned nPent = pairing_->getNumberOfPentachora();
    if (nPent == 0) {
        inputError_ = true; return;
    }

    permIndices_ = new int[nPent * 5];

    unsigned pent, facet;
    for (pent = 0; pent < nPent; ++pent)
        for (facet = 0; facet < 5; ++facet) {
            in >> permIndex(pent, facet);
            // Don't test the range of permIndex(pent, facet) since the
            // gluing permutation set could still be under construction.
        }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

} // namespace regina

