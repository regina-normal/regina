
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

/*! \file subcomplex/snappedtwosphere.h
 *  \brief Deals with 2-spheres made from two snapped 3-balls in a
 *  triangulation.
 */

#ifndef __SNAPPEDTWOSPHERE_H
#ifndef __DOXYGEN
#define __SNAPPEDTWOSPHERE_H
#endif

#include "regina-core.h"
#include "output.h"
#include "subcomplex/snappedball.h"
#include <boost/noncopyable.hpp>

namespace regina {

class SnappedBall;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a 2-sphere made from two snapped 3-balls in a triangulation.
 * This occurs when two snapped 3-balls are glued together at their
 * equators (note that this gluing does not have to extend to triangular faces).
 * Each 3-ball has a central disc (bounded by the 3-ball's equator and bisecting
 * its internal edge), and these two discs together form an embedded
 * 2-sphere in the triangulation.
 *
 * This 2-sphere can be cut along and the two resulting 2-sphere
 * boundaries filled in with 3-balls, and the resulting triangulation has
 * the same number of tetrahedra as the original.  If the snapped
 * 2-sphere was separating, the resulting triangulation will contain the
 * two terms of the corresponding connected sum.
 */
class REGINA_API SnappedTwoSphere :
        public ShortOutput<SnappedTwoSphere>,
        public boost::noncopyable {
    private:
        SnappedBall* ball[2];
            /**< The two snapped 3-balls whose equators are joined. */

    public:
        /**
         * Destroys this snapped 2-sphere; note that the corresponding
         * snapped 3-balls will also be destroyed.
         */
        ~SnappedTwoSphere();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        SnappedTwoSphere* clone() const;

        /**
         * Returns one of the two snapped 3-balls whose equators are
         * joined.
         *
         * @param index specifies which of the two 3-balls to return;
         * this must be either 0 or 1.
         * @return the corresponding snapped 3-ball.
         */
        const SnappedBall* snappedBall(int index) const;

        /**
         * Determines if the two given tetrahedra together form a snapped
         * 2-sphere.
         *
         * \pre The two given tetrahedra are distinct.
         *
         * @param tet1 the first tetrahedron to examine.
         * @param tet2 the second tetrahedron to examine.
         * @return a newly created structure containing details of the
         * snapped 2-sphere, or \c null if the given tetrahedra do not
         * form a snapped 2-sphere.
         */
        static SnappedTwoSphere* formsSnappedTwoSphere(Tetrahedron<3>* tet1,
            Tetrahedron<3>* tet2);
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
         * @param ball1 the first snapped 3-ball to examine.
         * @param ball2 the second snapped 3-ball to examine.
         * @return a newly created structure containing details of the
         * snapped 2-sphere, or \c null if the given snapped 3-balls do not
         * form a snapped 2-sphere.
         */
        static SnappedTwoSphere* formsSnappedTwoSphere(SnappedBall* ball1,
            SnappedBall* ball2);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        SnappedTwoSphere();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSnappedTwoSphere has now been renamed to
 * SnappedTwoSphere.
 */
REGINA_DEPRECATED typedef SnappedTwoSphere NSnappedTwoSphere;

/*@}*/

// Inline functions for SnappedTwoSphere

inline SnappedTwoSphere::SnappedTwoSphere() {
}
inline SnappedTwoSphere::~SnappedTwoSphere() {
    delete ball[0]; delete ball[1];
}
inline const SnappedBall* SnappedTwoSphere::snappedBall(int index) const {
    return ball[index];
}
inline void SnappedTwoSphere::writeTextShort(std::ostream& out) const {
    out << "Snapped 2-sphere";
}

} // namespace regina

#endif

