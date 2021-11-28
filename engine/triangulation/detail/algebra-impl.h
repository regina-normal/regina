
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

/*! \file triangulation/detail/algebra-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is \e not included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __REGINA_ALGEBRA_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_ALGEBRA_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina::detail {

template <int dim>
template <int subdim>
MatrixInt TriangulationBase<dim>::boundaryMap() const {
    static_assert(subdim > 0 && subdim <= dim);
    MatrixInt ans(countFaces<subdim>(), countFaces<subdim - 1>());

    if constexpr (subdim == dim) {
        size_t row = 0;
        for (auto s : simplices_) {
            Perm<dim + 1> subface;
            int i = subdim;
            while (true) {
                auto* facet = s->template face<dim - 1>(i);
                if (Perm<dim>::contract(
                        s->template faceMapping<dim - 1>(i).inverse()
                            * subface).sign() > 0) {
                    // The boundary face was not reflected.
                    if (i % 2)
                        ++ans.entry(row, facet->index());
                    else
                        --ans.entry(row, facet->index());
                } else {
                    // The boundary face was reflected.
                    if (i % 2)
                        --ans.entry(row, facet->index());
                    else
                        ++ans.entry(row, facet->index());
                }

                if (i > 0) {
                    --i;
                    subface = subface * Perm<dim + 1>(i, subdim);
                } else
                    break;
            }
            ++row;
        }
    } else {
        size_t row = 0;
        for (Face<dim, subdim>* f : faces<subdim>()) {
            Simplex<dim>* s = f->front().simplex();
            Perm<dim + 1> subface = f->front().vertices();
            int i = subdim;
            while (true) {
                unsigned facetNum = Face<dim, subdim - 1>::faceNumber(subface);
                auto* facet = s->template face<subdim - 1>(facetNum);
                if constexpr (subdim == 1) {
                    // We do not need to worry about orientation of
                    // (subdim-1)-faces, since these are vertices
                    // (and there is no Perm<1> class).
                    if (i % 2)
                        ++ans.entry(row, facet->index());
                    else
                        --ans.entry(row, facet->index());
                } else if (Perm<subdim>::contract(
                        s->template faceMapping<subdim - 1>(facetNum).inverse()
                            * subface).sign() > 0) {
                    // The boundary face was not reflected.
                    if (i % 2)
                        ++ans.entry(row, facet->index());
                    else
                        --ans.entry(row, facet->index());
                } else {
                    // The boundary face was reflected.
                    if (i % 2)
                        --ans.entry(row, facet->index());
                    else
                        ++ans.entry(row, facet->index());
                }

                if (i > 0) {
                    --i;
                    subface = subface * Perm<dim + 1>(i, subdim);
                } else
                    break;
            }
            ++row;
        }
    }

    return ans;
}

} // namespace regina::detail

#endif
