
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nsnappedtwosphere.h
 *  \brief Deals with 2-spheres made from two snapped 3-balls in a
 *  triangulation.
 */

#ifndef __NSNAPPEDTWOSPHERE_H
#ifndef __DOXYGEN
#define __NSNAPPEDTWOSPHERE_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsnappedball.h"
#else
    #include "engine/subcomplex/nsnappedball.h"
#endif

class NTetrahedron;
class NTriangulation;
class NSnappedBall;

/**
 * Represents a 2-sphere made from two snapped 3-balls in a triangulation.
 * This occurs when two snapped 3-balls are glued together at their
 * equators (note that this gluing does not have to extend to faces).  Each
 * 3-ball has a central disc (bounded by the 3-ball's equator and bisecting
 * its internal edge), and these two discs together form an embedded
 * 2-sphere in the triangulation.
 *
 * This 2-sphere can be cut along and the two resulting 2-sphere
 * boundaries filled in with 3-balls, and the resulting triangulation has
 * the same number of tetrahedra as the original.  If the snapped
 * 2-sphere was separating, the resulting triangulation will contain the
 * two terms of the corresponding connected sum.
 */
class NSnappedTwoSphere : public ShareableObject {
    private:
        NSnappedBall* ball[2];
            /**< The two snapped 3-balls whose equators are joined. */
    
    public:
        /**
         * Destroys this snapped 2-sphere; note that the corresponding
         * snapped 3-balls will also be destroyed.
         */
        virtual ~NSnappedTwoSphere();
        /**
         * Returns a newly created clone of this structure.
         *
         * \ifaces This routine is named \a cloneMe.
         *
         * @return a newly created clone.
         */
        NSnappedTwoSphere* clone() const;

        /**
         * Returns one of the two snapped 3-balls whose equators are
         * joined.
         *
         * @param index specifies which of the two 3-balls to return;
         * this must be either 0 or 1.
         * @return the corresponding snapped 3-ball.
         */
        NSnappedBall* getSnappedBall(int index) const;
        /**
         * Cuts along the 2-sphere and fills the two new boundary components
         * with 3-balls.  The number of tetrahedra in the triangulation
         * will not change.
         *
         * Note that if this 2-sphere is separating, this routine will
         * effectively split the corresponding connected sum into its
         * two original terms.
         *
         * \todo \bugurgent This routine currently does nothing!
         */
        void reduceTriangulation() const;
        /**
         * Makes a clone of this triangulation and reduces the clone as
         * described by reduceTriangulation().  The original
         * triangulation containing this 2-sphere will not be modified.
         * The clone will have no parent packet.
         *
         * \todo \bugurgent This routine currently does nothing!
         *
         * @param original the triangulation in which this 2-sphere occurs.
         * @return the newly created reduced triangulation.
         */
        NTriangulation* getReducedTriangulation(const NTriangulation* original)
            const;

        /**
         * Determines if the two given tetrahedra together form a snapped
         * 2-sphere.
         *
         * \pre The two given tetrahedra are distinct.
         *
         * \ifaces This routine is a member of class Engine.
         *
         * @param tet1 the first tetrahedron to examine.
         * @param tet2 the second tetrahedron to examine.
         * @return a newly created structure containing details of the
         * snapped 2-sphere, or \c null if the given tetrahedra do not
         * form a snapped 2-sphere.
         */
        static NSnappedTwoSphere* formsSnappedTwoSphere(NTetrahedron* tet1,
            NTetrahedron* tet2);
        /**
         * Determines if the two given snapped 3-balls together form a snapped
         * 2-sphere.
         *
         * If this is the case, the snapped 3-balls stored in
         * the structure returned will be clones of the
         * original 3-balls, not the original 3-balls themselves.
         *
         * \pre The two given snapped 3-balls use distinct tetrahedra.
         *
         * \ifaces This routine is a member of class Engine.
         *
         * @param tet1 the first snapped 3-ball to examine.
         * @param tet2 the second snapped 3-ball to examine.
         * @return a newly created structure containing details of the
         * snapped 2-sphere, or \c null if the given snapped 3-balls do not
         * form a snapped 2-sphere.
         */
        static NSnappedTwoSphere* formsSnappedTwoSphere(NSnappedBall* ball1,
            NSnappedBall* ball2);

        void writeTextShort(ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        NSnappedTwoSphere();
};

// Inline functions for NSnappedTwoSphere

inline NSnappedTwoSphere::NSnappedTwoSphere() {
}
inline NSnappedTwoSphere::~NSnappedTwoSphere() {
    delete ball[0]; delete ball[1];
}
inline NSnappedBall* NSnappedTwoSphere::getSnappedBall(int index) const {
    return ball[index];
}
inline void NSnappedTwoSphere::writeTextShort(ostream& out) const {
    out << "Snapped 2-sphere";
}

#endif

