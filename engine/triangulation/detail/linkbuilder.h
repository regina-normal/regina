
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/detail/linkbuilder.h
 *  \brief Contains helper class that implements building the triangulation
 *  of the link of a face. The methods are implemented in
 *  linkbuilder-impl.h.
 */

#ifndef __REGINA_LINK_BUILDER_H
#ifndef __DOXYGEN
#define __REGINA_LINK_BUILDER_H
#endif

#include "triangulation/forward.h"

#include <memory>

namespace regina::detail {

template<int dim, int subdim> class FaceBase;

/** Helper class that implements building the triangulation of a link of a
 * face Face<dim, subdim>.
 */ 
template<int dim, int subdim>
struct LinkBuilder
{
    static constexpr int linkDimension = dim - subdim - 1;
        /**< Gives the dimension of the triangulation of the link of
             the face. */

    /**
     * The type of triangulation of the link of the face.
     */
    using LinkTriangulation = Triangulation<linkDimension>;

    /**
     * A custom deleter simply calling the destructor of
     * `LinkTriangulation` through delete.
     *
     * Note that the implementation is elsewhere so that files
     * including LinkBuilder do not need to include the header
     * definining the `Triangulation` class.
     */
    struct Deleter
    {
        void operator() (const LinkTriangulation * trig);
    };

    /**
     * Clients can use this type to hold on to the link triangulation
     * while only forward-declaring the `Triangulation` class.
     */
    using UniquePtr = std::unique_ptr<const LinkTriangulation, Deleter>;

    /**
     * Construct the triangulation of the link of the given face.
     */
    static
    UniquePtr
    build(const FaceBase<dim, subdim> &face);
};

}

#endif
