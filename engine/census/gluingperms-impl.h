
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file census/gluingperms-impl.h
 *  \brief Contains implementation details for the GluingPerms class template.
 *
 *  This file is \e not included automatically by gluingperms.h.
 *  However, typical end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of GluingPerms for \ref stddim "standard dimensions".
 */

#ifndef __REGINA_GLUINGPERMS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMS_IMPL_H
#endif

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "census/gluingperms.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"
#include "utilities/stringutils.h"

namespace regina {

template <int dim>
std::string GluingPerms<dim>::data() const {
    std::ostringstream out;
    dumpData(out);
    return out.str();
}

template <int dim>
Triangulation<dim> GluingPerms<dim>::triangulate() const {
    unsigned nSimp = size();

    Triangulation<dim> ans;
    auto* simp = new Simplex<dim>*[nSimp];

    unsigned t, facet;
    for (t = 0; t < nSimp; ++t)
        simp[t] = ans.newSimplex();

    for (t = 0; t < nSimp; ++t)
        for (facet = 0; facet <= dim; ++facet)
            if ((! pairing_.isUnmatched(t, facet)) &&
                    (! simp[t]->adjacentSimplex(facet)))
                simp[t]->join(facet, simp[pairing_.dest(t, facet).simp],
                    perm(t, facet));

    delete[] simp;
    return ans;
}

template <int dim>
typename GluingPerms<dim>::Index GluingPerms<dim>::gluingToIndex(
        const FacetSpec<dim>& source, const Perm<dim+1>& gluing) const {
    Perm<dim+1> permSn_1 = Perm<dim+1>(pairing_.dest(source).facet, dim)
        * gluing * Perm<dim+1>(source.facet, dim);
    return Perm<dim>::contract(permSn_1).SnIndex();
}

template <int dim>
typename GluingPerms<dim>::Index GluingPerms<dim>::gluingToIndex(
        unsigned simp, unsigned facet, const Perm<dim+1>& gluing) const {
    Perm<dim+1> permSn_1 =
        Perm<dim+1>(pairing_.dest(simp, facet).facet, dim) * gluing *
        Perm<dim+1>(facet, dim);
    return Perm<dim>::contract(permSn_1).SnIndex();
}

template <int dim>
void GluingPerms<dim>::dumpData(std::ostream& out) const {
    out << pairing_.toTextRep() << std::endl;

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
GluingPerms<dim>::GluingPerms(std::istream& in) :
        pairing_(in), permIndices_(new Index[pairing_.size() * (dim + 1)]) {
    // The FacetPairing constructor has already skipped whitespace and
    // read a single line describing the facet pairing.

    unsigned simp, facet;
    for (simp = 0; simp < pairing_.size(); ++simp)
        for (facet = 0; facet <= dim; ++facet) {
            in >> permIndex(simp, facet);
            // Don't test the range of permIndex(simp, facet) since the
            // gluing permutation set could still be under construction.
        }

    // Did we hit an unexpected EOF?
    if (in.eof())
        throw InvalidInput("Unexpected end of input stream "
            "while attempting to read GluingPerms");
}

template <int dim>
GluingPerms<dim> GluingPerms<dim>::fromData(const std::string& data) {
    try {
        std::istringstream in(data);
        return GluingPerms<dim>(in);
    } catch (const InvalidInput& exc) {
        throw InvalidArgument(exc.what());
    }
}

template <int dim>
void GluingPerms<dim>::writeTextShort(std::ostream& out) const {
    for (size_t simp = 0; simp < size(); ++simp) {
        if (simp > 0)
            out << " | ";
        for (int i = 0; i <= dim; ++i) {
            if (i > 0)
                out << ", ";
            if (pairing_.isUnmatched(simp, i))
                out << "bdry";
            else if (permIndex(simp, i) < 0)
                out << "??";
            else
                out << pairing_.dest(simp, i).simp << " (" << perm(simp, i)
                    << ')';
        }
    }
}

template <int dim>
void GluingPerms<dim>::writeTextLong(std::ostream& out) const {
    if constexpr (dim == 2)
        out << "  Triangle  |  glued to:";
    else if constexpr (dim == 3)
        out << "  Tet  |  glued to:";
    else if constexpr (dim == 4)
        out << "  Pent  |  glued to:";
    else
        out << "  Simplex  |  glued to:";
    for (int i = dim; i >= 0; --i) {
        out << "     (";
        for (int j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    if constexpr (dim == 2)
        out << "  ----------+-----------";
    else if constexpr (dim == 3)
        out << "  -----+-----------";
    else if constexpr (dim == 4)
        out << "  ------+-----------";
    else
        out << "  ---------+-----------";
    for (int i = dim; i >= 0; --i)
        for (int j = 0; j < 7 + dim; ++j)
            out << '-';
    out << '\n';
    for (size_t simp = 0; simp < size(); ++simp) {
        if constexpr (dim == 2)
            out << "      ";
        else if constexpr (dim == 3)
            out << ' ';
        else if constexpr (dim == 4)
            out << "  ";
        else
            out << "     ";
        out << std::setw(4) << simp << "  |           ";
        for (int i = dim; i >= 0; --i) {
            if (pairing_.isUnmatched(simp, i)) {
                for (int j = 0; j < dim - 1; ++j)
                    out << ' ';
                out << "boundary";
            } else if (permIndex(simp, i) < 0) {
                for (int j = 0; j < dim; ++j)
                    out << ' ';
                out << "unknown";
            } else {
                auto gluing = perm(simp, i);
                out << std::setw(4)
                    << pairing_.dest(simp, i).simp << " (";
                for (int j = 0; j <= dim; ++j) {
                    if (j != i)
                        out << regina::digit(gluing[j]);
                }
                out << ")";
            }
        }
        out << '\n';
    }
}

} // namespace regina

#endif

