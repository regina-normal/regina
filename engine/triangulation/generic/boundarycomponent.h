
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

#ifndef __BOUNDARYCOMPONENT_H
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT_H
#endif

/*! \file triangulation/generic/boundarycomponent.h
 *  \brief Internal header for boundary components of triangulations of
 *  arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#include "triangulation/detail/boundarycomponent.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * A component of the boundary of a <i>dim</i>-manifold triangulation.
 *
 * Regina recognises three types of boundary components:
 *
 * - A <i>real boundary component</i> is formed from unglued facets
 *   of top-dimensional simplices of the triangulation.  For example,
 *   a real boundary component of a 3-manifold triangulation is formed
 *   from boundary triangles in the 2-skeleton.
 *
 * - An <i>ideal boundary component</i> consists of a single ideal vertex;
 *   that is, a vertex whose link is a closed (<i>dim</i>-1)-manifold but not
 *   a sphere.  This is only possible for dimensions \a dim &ge; 3.
 *
 * - An <i>invalid vertex boundary component</i> consists of a single invalid
 *   invalid vertex that does not belong to a real boundary component.  This
 *   means the vertex link must be an ideal (<i>dim</i>-1)-manifold, or an
 *   invalid (<i>dim</i>-1)-manifold with no real boundary.  Invalid vertex
 *   boundary components are only possible for dimensions \a dim &ge; 4.
 *
 * Ideal and invalid vertex boundary components are \e only recognised
 * when \a dim is one of Regina's \ref stddim "standard dimensions".
 * This is because, in higher dimensions, the relevant conditions rely
 * on undecidable problems.
 *
 * Regina stores different skeletal information for different types of
 * boundary components:
 *
 * - For real boundary components in \ref stddim "standard dimensions",
 *   Regina stores all boundary faces of all dimensions 0,1,...,<i>dim</i>-1.
 *
 * - For real boundary components in higher (non-standard) dimensions,
 *   Regina only stores the boundary (<i>dim</i>-1)-faces (i.e., the unglued
 *   facets of top-dimensional simplices that make up the boundary component).
 *   This is simply to avoid excessive memory usage.
 *
 * - For ideal and invalid vertex boundary components, Regina stores the
 *   relevant vertex.  Such boundary components do not contain any faces
 *   of any dimension greater than zero.
 *
 * We can encounter some interesting cases with invalid triangulations.
 * Consider some face whose link has more than one boundary component
 * (which makes the face invalid).  This means that different parts of the
 * (<i>dim</i>)-manifold boundary are effectively "pinched" together.
 * If this happens, the different parts of the boundary that are pinched
 * might or might not be listed as part of the same boundary component;
 * if not then the offending face will be included in \e all of these
 * boundary components.  Nevertheless, only one of these can be considered
 * the "official" boundary component of the face as returned by
 * Face::boundaryComponent().  This is all a bit of a mess, but then again the
 * entire triangulation is invalid and so you almost certainly have
 * bigger problems to deal with.
 *
 * Boundary components are highly temporary: whenever a triangulation changes,
 * all of its boundary component objects will be deleted and new ones will be
 * created in their place.
 *
 * Boundary component objects are all created, managed and destroyed by the
 * class Triangulation<dim>.  See the Triangulation notes for further
 * information on working with <i>dim</i>-dimensional triangulations.
 *
 * For dimension \a dim = 3, this template is specialised and offers
 * more functionality.  In order to use this specialised class, you will
 * need to include the corresponding header triangulation/dim3.h.
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., BoundaryComponent2 and BoundaryComponent3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim>
class BoundaryComponent : public detail::BoundaryComponentBase<dim> {
    static_assert(dim != 3,
        "The generic implementation of BoundaryComponent<dim> "
        "should not be used for dimension 3.");

    private:
        /**
         * Default constructor.
         */
        BoundaryComponent() = default;

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

/*@}*/

} // namespace regina

#endif

