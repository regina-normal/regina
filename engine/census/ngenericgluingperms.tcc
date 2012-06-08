
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/* Template definitions for ngenericgluingperms.h. */

#include <algorithm>
#include "census/ngenericgluingperms.h"
#include "utilities/stringutils.h"

namespace regina {

template <int dim>
NGenericGluingPerms<dim>::NGenericGluingPerms(
        const NGenericGluingPerms<dim>& cloneMe) :
        pairing_(cloneMe.pairing_), inputError_(false) {
    unsigned nSimp = cloneMe.size();

    permIndices_ = new int[nSimp * (dim + 1)];
    std::copy(cloneMe.permIndices_,
        cloneMe.permIndices_ + nSimp * (dim + 1), permIndices_);
}

template <int dim>
typename NGenericGluingPerms<dim>::Triangulation*
        NGenericGluingPerms<dim>::triangulate() const {
    unsigned nSimp = size();

    Triangulation* ans = new Triangulation;
    Simplex** simp = new Simplex*[nSimp];

    unsigned t, facet;
    for (t = 0; t < nSimp; ++t)
        simp[t] = ans->newSimplex();

    for (t = 0; t < nSimp; ++t)
        for (facet = 0; facet <= dim; ++facet)
            if ((! pairing_->isUnmatched(t, facet)) &&
                    (! simp[t]->adjacentSimplex(facet)))
                simp[t]->joinTo(facet, simp[pairing_->dest(t, facet).simp],
                    gluingPerm(t, facet));

    delete[] simp;
    return ans;
}

template <int dim>
int NGenericGluingPerms<dim>::gluingToIndex(const NFacetSpec<dim>& source,
        const Perm& gluing) const {
    Perm permSn_1 = Perm(pairing_->dest(source).facet, dim) * gluing *
        Perm(source.facet, dim);
    return (std::find(Perm::Sn_1, Perm::Sn_1 + Perm::nPerms_1, permSn_1)
        - Perm::Sn_1);
}

template <int dim>
int NGenericGluingPerms<dim>::gluingToIndex(unsigned simp, unsigned facet,
        const Perm& gluing) const {
    Perm permSn_1 = Perm(pairing_->dest(simp, facet).facet, dim) * gluing *
        Perm(facet, dim);
    return (std::find(Perm::Sn_1, Perm::Sn_1 + Perm::nPerms_1, permSn_1)
        - Perm::Sn_1);
}

template <int dim>
void NGenericGluingPerms<dim>::dumpData(std::ostream& out) const {
    out << pairing_->toTextRep() << std::endl;

    unsigned simp, facet;
    for (simp = 0; simp < size(); ++simp)
        for (facet = 0; facet <= dim; ++facet) {
            if (simp || facet)
                out << ' ';
            out << permIndex(simp, facet);
        }
    out << std::endl;
}

template <int dim>
NGenericGluingPerms<dim>::NGenericGluingPerms(std::istream& in) :
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

    pairing_ = FacetPairing::fromTextRep(line);
    if (! pairing_) {
        inputError_ = true; return;
    }

    unsigned nSimps = pairing_->size();
    if (nSimps == 0) {
        inputError_ = true; return;
    }

    permIndices_ = new int[nSimps * (dim + 1)];

    unsigned simp, facet;
    for (simp = 0; simp < nSimps; ++simp)
        for (facet = 0; facet <= dim; ++facet) {
            in >> permIndex(simp, facet);
            // Don't test the range of permIndex(simp, facet) since the
            // gluing permutation set could still be under construction.
        }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

} // namespace regina

