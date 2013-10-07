
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file generic/isosig.h
 *  \brief Dimension-agnostic code for working with isomorphism signatures.
 */

#ifndef __ISOSIG_H
#ifndef __DOXYGEN
#define __ISOSIG_H
#endif

#include "regina-core.h"
#include "generic/dimtraits.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Constructs the isomorphism signature for the given triangulation.
 *
 * An <i>isomorphism signature</i> is a compact text representation of
 * a triangulation.  Unlike dehydrations for 3-manifold triangulations,
 * an isomorphism signature uniquely determines a triangulation up
 * to combinatorial isomorphism (assuming the dimension is known in advance).
 * That is, two triangulations of dimension \a dim are combinatorially
 * isomorphic if and only if their isomorphism signatures are the same.
 *
 * The isomorphism signature is constructed entirely of
 * printable characters, and has length proportional to
 * <tt>n log n</tt>, where \a n is the number of simplices.
 *
 * Isomorphism signatures are more general than dehydrations:
 * they can be used with any triangulation (including closed,
 * bounded and/or disconnected triangulations, as well
 * as triangulations with large numbers of triangles).
 *
 * The time required to construct the isomorphism signature of a
 * triangulation is <tt>O(n^2 log^2 n)</tt>.
 *
 * The routine fromIsoSig() can be used to recover a
 * triangulation from an isomorphism signature.  The triangulation
 * recovered might not be identical to the original, but it will be
 * combinatorially isomorphic.
 *
 * \warning Do not mix isomorphism signatures between dimensions!
 * It is possible that the same string could corresponding to both a
 * \a p-dimensional triangulation and a \a q-dimensional triangulation
 * for different \a p and \a q.
 *
 * \pre The template argument \a dim must be one of the dimensions that
 * Regina supports.
 *
 * \ifacespython Not present, though you can access this through the
 * isoSig() member function in the corresponding triangulation class.
 *
 * @param tri the triangulation whose isomorphism signature will be computed.
 * @return the isomorphism signature of the given triangulation.
 */
template <int dim>
REGINA_API std::string isoSig(
    const typename DimTraits<dim>::Triangulation& tri);

/**
 * Recovers a full triangulation from an isomorphism signature.
 *
 * See isoSig() for more information on isomorphism signatures.
 * It will be assumed that the signature describes a triangulation of
 * dimension \a dim.
 *
 * The triangulation that is returned will be newly created.
 *
 * Calling isoSig() followed by fromIsoSig() is not guaranteed to
 * produce an identical triangulation to the original, but it
 * \e is guaranteed to produce a combinatorially isomorphic
 * triangulation.
 *
 * \warning Do not mix isomorphism signatures between dimensions!
 * It is possible that the same string could corresponding to both a
 * \a p-dimensional triangulation and a \a q-dimensional triangulation
 * for different \a p and \a q.
 *
 * \pre The template argument \a dim must be one of the dimensions that
 * Regina supports.
 *
 * \ifacespython Not present, though you can access this through the
 * fromIsoSig() member function in the corresponding triangulation class.
 *
 * @param sig the isomorphism signature of the
 * triangulation to construct.  Note that, unlike dehydration
 * strings for 3-manifold triangulations, case is important for
 * isomorphism signatures.
 * @return a newly allocated triangulation if the reconstruction was
 * successful, or null if the given string was not a valid
 * isomorphism signature.
 */
template <int dim>
REGINA_API typename DimTraits<dim>::Triangulation* fromIsoSig(
    const std::string& sig);

/*@}*/

} // namespace regina

#endif

