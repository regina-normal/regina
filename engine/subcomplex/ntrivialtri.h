
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file ntrivialtri.h
 *  \brief Deals with a few specific trivial triangulations.
 */

#ifndef __NTRIVIALTRI_H
#ifndef __DOXYGEN
#define __NTRIVIALTRI_H
#endif

#include "subcomplex/nstandardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents one of a few particular trivial triangulations that do not
 * belong to any of the other larger families.
 *
 * All optional NStandardTriangulation routines are implemented for this
 * class.
 */
class NTrivialTri : public NStandardTriangulation {
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
         * triangulation has no Mobius band faces.
         */
        static const int N3_1;
        /**
         * Represents the three-tetrahedron triangulation N(3,2) of the
         * projective plane bundle over the circle.  This particular
         * triangulation has two Mobius band faces.
         */
        static const int N3_2;
    private:
        int type;
            /**< The specific triangulation being represented.
                 This must be one of the triangulation constants defined
                 in this class. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NTrivialTri* clone() const;

        /**
         * Returns the specific trivial triangulation being represented.
         *
         * @return the specific triangulation.  This will be one of the
         * triangulation constants defined in this class.
         */
        int getType() const;

        /**
         * Determines if the given triangulation component is one of the
         * trivial triangulations recognised by this class.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure representing the trivial
         * triangulation, or \c null if the given component is not one
         * of the triangulations recognised by this class.
         */
        static NTrivialTri* isTrivialTriangulation(const NComponent* comp);

        NManifold* getManifold() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure.
         */
        NTrivialTri(int newType);
};

/*@}*/

// Inline functions for NTrivialTri

inline NTrivialTri::NTrivialTri(int newType) : type(newType) {
}

inline NTrivialTri* NTrivialTri::clone() const {
    return new NTrivialTri(type);
}

inline int NTrivialTri::getType() const {
    return type;
}

} // namespace regina

#endif

