
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file nlayeredlensspace.h
 *  \brief Deals with layered lens space components of a triangulation.
 */

#ifndef __NLAYEREDLENSSPACE_H
#ifndef __DOXYGEN
#define __NLAYEREDLENSSPACE_H
#endif

#include "subcomplex/nlayeredsolidtorus.h"

class NComponent;

/**
 * Represents a layered lens space component of a triangulation.
 * A layered lens space is considered to be any layered solid torus glued
 * to a degenerate (2,1,1) layered solid torus (i.e., a one-triangle mobius
 * strip).  Note that the three possible gluing options represent the
 * three possible ways of closing the initial torus - either twisting it
 * shut (in one of two possible ways) or snapping it shut without any twist.
 * 
 * A layered lens space must contain at least one tetrahedron.
 */
class NLayeredLensSpace : public ShareableObject {
    private:
        NLayeredSolidTorus* torus;
            /**< The layered solid torus that forms the basis of this
                 layered lens space. */
        int mobiusBoundaryGroup;
            /**< The edge group of the top level tetrahedron in the
                 layered solid torus to which the boundary of the mobius
                 strip is glued. */
        unsigned long p,q;
            /**< The lens space parameters for L(p,q). */
    
    public:
        /**
         * Destroys this lens space; note that the corresponding layered
         * solid torus will also be destroyed.
         */
        virtual ~NLayeredLensSpace();
        /**
         * Returns a newly created clone of this structure.
         *
         * \ifaces This routine is named \a cloneMe.
         *
         * @return a newly created clone.
         */
        NLayeredLensSpace* clone() const;

        /**
         * Returns the first parameter \a p of this lens space L(p,q).
         *
         * @return the first parameter \a p.
         */
        unsigned long getP() const;
        /**
         * Returns the second parameter \a q of this lens space L(p,q).
         *
         * @return the second parameter \a q.
         */
        unsigned long getQ() const;

        /**
         * Returns the layered solid torus to which the mobius strip is
         * glued.
         *
         * @return the layered solid torus.
         */
        const NLayeredSolidTorus& getTorus() const;
        /**
         * Determines which edge of the layered solid torus is glued to
         * the boundary of the mobius strip (i.e., the weight 2 edge
         * of the degenerate (2,1,1) layered solid torus).  The return
         * value will be one of the three top level tetrahedron edge
         * groups in the layered solid torus; see
         * NLayeredSolidTorus::getTopEdge() for further details about
         * edge groups.
         *
         * @return the top level edge group of the layered solid torus to
         * which the mobius strip boundary is glued.
         */
        int getMobiusBoundaryGroup() const;
        /**
         * Determines if the layered solid torus that forms the basis for
         * this lens space is snapped shut (folded closed without a twist).
         *
         * @return \c true if and only if the torus is snapped shut.
         */
        bool isSnapped() const;
        /**
         * Determines if the layered solid torus that forms the basis for
         * this lens space is twisted shut (folded closed with a twist).
         *
         * @return \c true if and only if the torus is twisted shut.
         */
        bool isTwisted() const;

        /**
         * Determines if the given triangulation component is a layered
         * lens space.
         *
         * \ifaces This routine is a member of class Engine.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * layered lens space, or \c null if the given component is
         * not a layered lens space.
         */
        static NLayeredLensSpace* isLayeredLensSpace(const NComponent* comp);

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        NLayeredLensSpace();
};

// Inline functions for NLayeredLensSpace

inline NLayeredLensSpace::NLayeredLensSpace() {
}
inline NLayeredLensSpace::~NLayeredLensSpace() {
    delete torus;
}

inline unsigned long NLayeredLensSpace::getP() const {
    return p;
}
inline unsigned long NLayeredLensSpace::getQ() const {
    return q;
}
inline const NLayeredSolidTorus& NLayeredLensSpace::getTorus() const {
    return *torus;
}
inline int NLayeredLensSpace::getMobiusBoundaryGroup() const {
    return mobiusBoundaryGroup;
}
inline bool NLayeredLensSpace::isSnapped() const {
    return (torus->getTopEdge(mobiusBoundaryGroup, 1) == -1);
}
inline bool NLayeredLensSpace::isTwisted() const {
    return (torus->getTopEdge(mobiusBoundaryGroup, 1) != -1);
}
inline void NLayeredLensSpace::writeTextShort(std::ostream& out) const {
    out << "L( " << p << ", " << q << " ) layered lens space";
}

#endif

