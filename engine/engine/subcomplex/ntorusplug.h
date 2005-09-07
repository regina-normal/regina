
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ntorusplug.h
 *  \brief Deals with plugged I-bundle triangulations.
 */

#ifndef __NTORUSPLUG_H
#ifndef __DOXYGEN
#define __NTORUSPLUG_H
#endif

#include <list>
#include "subcomplex/nstandardtri.h"
#include "triangulation/nperm.h"

namespace regina {

class NLayeredSolidTorus;

/**
 * \weakgroup subcomplex
 * @{
 */

struct NTemporaryFibre {
    long alpha, beta;

    NTemporaryFibre(long newAlpha, long newBeta) :
        alpha(newAlpha), beta(newBeta) {}
};

/**
 * 0----2 1
 * | A / /|
 * |  / / |
 * | / /  |
 * |/ / B |
 * 1 2----0
 *
 * Input: Pass faces A, B with 012 -> vertices mappings.
 * Output: Get fibre parameters where main fibres are parallel
 * to 01 and base orbifold runs parallel to 02.
 */
class NTorusPlug : public NStandardTriangulation {
    protected:
        NTetrahedron* internalBdry[2];
        NPerm internalBdryPerm[2];
            /**< 012 -> internalBdry vertices. */
    public:
        virtual ~NTorusPlug();

        virtual NTemporaryFibre plugFibre() const;
            /**< Return 0,0 if it's not a single fibre. */

        static NTorusPlug* isPlugged(
            NTetrahedron* externalBdry1, NPerm externalVertices1,
            NTetrahedron* externalBdry2, NPerm externalVertices2);

    protected:
        NTorusPlug(NTetrahedron* internalBdry1, NPerm internalVertices1,
            NTetrahedron* internalBdry2, NPerm internalVetices2);

        static bool isBad(NTetrahedron* t,
            const std::list<NTetrahedron*>& avoidTets);

        /**
         * avoidTets is the tetrahedra we've already been through, plus
         * the original external boundary tetrahedra.
         */
        static NTorusPlug* isPlugged(
            NTetrahedron* externalBdry1, NPerm externalVertices1,
            NTetrahedron* externalBdry2, NPerm externalVertices2,
            std::list<NTetrahedron*>& avoidTets);
};

class NTorusPlugMobius : public NTorusPlug {
    private:
        int orientation;
            /** 0, 1 or 2 according to whether the weight two edge on
                the boundary is 12, 02 or 01. */

    public:
        virtual ~NTorusPlugMobius();

        NTemporaryFibre plugFibre() const;

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NTorusPlug* isPlugged(
            NTetrahedron* externalBdry1, NPerm externalVertices1,
            NTetrahedron* externalBdry2, NPerm externalVertices2);

    private:
        NTorusPlugMobius(NTetrahedron* internalBdry1, NPerm internalVertices1,
            NTetrahedron* internalBdry2, NPerm internalVetices2);
};

class NTorusPlugLST : public NTorusPlug {
    private:
        NLayeredSolidTorus* lst;
            /**< Must be non-null. */
        NPerm roles;
            /**< Suppose roles == p.
                 Edge group p[0] of the LST is glued to edge 01 (vertical).
                 Edge group p[1] of the LST is glued to edge 02 (horizontal).
                 Edge group p[2] of the LSG is glued to edge 12 (diagonal).
                 */

    public:
        virtual ~NTorusPlugLST();

        NTemporaryFibre plugFibre() const;

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NTorusPlug* isPlugged(
            NTetrahedron* externalBdry1, NPerm externalVertices1,
            NTetrahedron* externalBdry2, NPerm externalVertices2);

    private:
        NTorusPlugLST(NTetrahedron* internalBdry1, NPerm internalVertices1,
            NTetrahedron* internalBdry2, NPerm internalVetices2);
};

class NTorusPlugDouble : public NTorusPlug {
    private:
        int partition;
            /**< Partition is parallel to vertical (0), horizontal (1) or
                 diagonal (2) edges?  This must be 0; the field is merely
                 provided for future expansion. */
        int layering;
            /**< The third tetrahedron is layered over the vertical (0),
                 horizontal (1) or diagonal (2) edge?  This cannot be the
                 same as partition. */
        NTorusPlug* plug[2];
            /** Must be non-null. */

    public:
        virtual ~NTorusPlugDouble();

        void insertPlugFibres(std::list<NTemporaryFibre>& results) const;

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NTorusPlug* isPlugged(
            NTetrahedron* externalBdry1, NPerm externalVertices1,
            NTetrahedron* externalBdry2, NPerm externalVertices2,
            std::list<NTetrahedron*>& avoidTets);

    private:
        NTorusPlugDouble(NTetrahedron* internalBdry1, NPerm internalVertices1,
            NTetrahedron* internalBdry2, NPerm internalVetices2);
};

/*@}*/

// Inline functions for NTorusPlug

inline NTorusPlug::~NTorusPlug() {
}

inline NTemporaryFibre NTorusPlug::plugFibre() const {
    return NTemporaryFibre(0, 0);
}

inline NTorusPlug::NTorusPlug(
        NTetrahedron* internalBdry1, NPerm internalVertices1,
        NTetrahedron* internalBdry2, NPerm internalVertices2) {
    internalBdry[0] = internalBdry1;
    internalBdry[1] = internalBdry2;
    internalBdryPerm[0] = internalVertices1;
    internalBdryPerm[1] = internalVertices2;
}

// Inline functions for NTorusPlugMobius

inline NTorusPlugMobius::~NTorusPlugMobius() {
}

inline NTorusPlugMobius::NTorusPlugMobius(
        NTetrahedron* internalBdry1, NPerm internalVertices1,
        NTetrahedron* internalBdry2, NPerm internalVertices2) :
        NTorusPlug(internalBdry1, internalVertices1,
            internalBdry2, internalVertices2) {
}

// Inline functions for NTorusPlugLST

inline NTorusPlugLST::NTorusPlugLST(
        NTetrahedron* internalBdry1, NPerm internalVertices1,
        NTetrahedron* internalBdry2, NPerm internalVertices2) :
        NTorusPlug(internalBdry1, internalVertices1,
            internalBdry2, internalVertices2), lst(0) {
}

// Inline functions for NTorusPlugDouble

inline NTorusPlugDouble::NTorusPlugDouble(
        NTetrahedron* internalBdry1, NPerm internalVertices1,
        NTetrahedron* internalBdry2, NPerm internalVertices2) :
        NTorusPlug(internalBdry1, internalVertices1,
            internalBdry2, internalVertices2) {
    plug[0] = plug[1] = 0;
}

} // namespace regina

#endif

