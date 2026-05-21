
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/generic/triangulation.h
 *  \brief Internal header for working with triangulations of arbitrary
 *  dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_TRIANGULATION_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION_H
#endif

#include "packet/packet.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include "utilities/xmlutils.h"
#include <type_traits>
#include "triangulation/detail/triangulation.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * \defgroup generic Generic triangulations
 * Details for implementing triangulations in arbitrary dimensions.
 */

/**
 * Deprecated function object used for sorting faces of triangulations by
 * increasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangulation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * increasing edge degree, you would use DegreeLessThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * An object of this class behaves like a reference: it is lightweight and can
 * be copy-constructed cheaply, but it does not support assignments or swaps.
 *
 * \deprecated This comparison is a one-liner.  Just use a lambda instead.
 *
 * \nopython
 *
 * \ingroup generic
 */
template <int dim, int subdim>
requires (standardDim(dim) && subdim >= 0 && subdim < dim)
class [[deprecated]] DegreeLessThan {
    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * \param tri the triangulation with which we are working.
         */
        DegreeLessThan(const Triangulation<dim>& tri) : tri_(tri) {}
        /**
         * Creates a new clone of the given function object.
         */
        DegreeLessThan(const DegreeLessThan&) = default;
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * \param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * \param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * \return \c true if and only if face \a a has smaller degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const {
            return (tri_.template face<subdim>(a)->degree() <
                    tri_.template face<subdim>(b)->degree());
        }

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeLessThan& operator = (const DegreeLessThan&) = delete;
};

/**
 * Deprecated function object used for sorting faces of triangulations by
 * decreasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangulation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * decreasing edge degree, you would use DegreeGreaterThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * An object of this class behaves like a reference: it is lightweight and can
 * be copy-constructed cheaply, but it does not support assignments or swaps.
 *
 * \deprecated This comparison is a one-liner.  Just use a lambda instead.
 *
 * \nopython
 *
 * \ingroup generic
 */
template <int dim, int subdim>
requires (standardDim(dim) && subdim >= 0 && subdim < dim)
class [[deprecated]] DegreeGreaterThan {
    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * \param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const Triangulation<dim>& tri) : tri_(tri) {}
        /**
         * Creates a new clone of the given function object.
         */
        DegreeGreaterThan(const DegreeGreaterThan&) = default;
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * \param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * \param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * \return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const {
            return (tri_.template face<subdim>(a)->degree() >
                    tri_.template face<subdim>(b)->degree());
        }

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeGreaterThan& operator = (const DegreeGreaterThan&) = delete;
};

} // namespace regina

#endif
