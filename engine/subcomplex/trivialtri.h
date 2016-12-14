
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

/*! \file subcomplex/trivialtri.h
 *  \brief Deals with a few specific hard-coded trivial triangulations.
 */

#ifndef __TRIVIALTRI_H
#ifndef __DOXYGEN
#define __TRIVIALTRI_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents one of a few particular hard-coded trivial triangulations
 * that do not belong to any of the other larger families.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API TrivialTri : public StandardTriangulation {
    public:
        /**
         * Represents the two-tetrahedron four-vertex triangulation of
         * the 3-sphere.
         */
        static const int SPHERE_4_VERTEX;
        /**
         * Represents the one-tetrahedron three-vertex triangulation of
         * the ball.  This is a single tetrahedron with two faces as
         * boundary and the other two faces folded together.
         */
        static const int BALL_3_VERTEX;
        /**
         * Represents the one-tetrahedron four-vertex triangulation of
         * the ball.  This is a single tetrahedron with all four faces
         * as boundary.
         */
        static const int BALL_4_VERTEX;
        /**
         * Represents the two-tetrahedron triangulation N(2) of the
         * twisted 2-sphere bundle over the circle.
         */
        static const int N2;
        /**
         * Represents the three-tetrahedron triangulation N(3,1) of the
         * projective plane bundle over the circle.  This particular
         * triangulation has no Mobius band triangles.
         */
        static const int N3_1;
        /**
         * Represents the three-tetrahedron triangulation N(3,2) of the
         * projective plane bundle over the circle.  This particular
         * triangulation has two Mobius band triangles.
         */
        static const int N3_2;
    private:
        int type_;
            /**< The specific triangulation being represented.
                 This must be one of the triangulation constants defined
                 in this class. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        TrivialTri* clone() const;

        /**
         * Returns the specific trivial triangulation being represented.
         *
         * @return the specific triangulation.  This will be one of the
         * triangulation constants defined in this class.
         */
        int type() const;

        /**
         * Determines if the given triangulation component is one of the
         * trivial triangulations recognised by this class.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure representing the trivial
         * triangulation, or \c null if the given component is not one
         * of the triangulations recognised by this class.
         */
        static TrivialTri* isTrivialTriangulation(const Component<3>* comp);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure.
         */
        TrivialTri(int newType);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTrivialTri has now been renamed to
 * TrivialTri.
 */
REGINA_DEPRECATED typedef TrivialTri NTrivialTri;

/*@}*/

// Inline functions for TrivialTri

inline TrivialTri::TrivialTri(int newType) : type_(newType) {
}

inline TrivialTri* TrivialTri::clone() const {
    return new TrivialTri(type_);
}

inline int TrivialTri::type() const {
    return type_;
}

} // namespace regina

#endif

