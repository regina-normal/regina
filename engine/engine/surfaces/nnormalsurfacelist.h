
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file nnormalsurfacelist.h
 *  \brief Contains a packet representing a collection of normal
 *  surfaces in a 3-manifold.
 */

#ifndef __NNORMALSURFACELIST_H
#ifndef __DOXYGEN
#define __NNORMALSURFACELIST_H
#endif

#include "packet/npacket.h"
#include "property/npropertyholder.h"
#include "utilities/ndynamicarray.h"
#include "utilities/ndoublelist.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nsurfaceset.h"

class NTriangulation;
class NMatrixInt;

/**
 * A packet representing a collection of normal surfaces in a 3-manifold.
 * Such a packet must always be a child packet of the triangulation from
 * which the surfaces were obtained.  If this triangulation changes, the
 * information contained in this packet will become invalid.
 *
 * See the NNormalSurfaceVector class notes for details of what to do
 * when introducing a new flavour of coordinate system.
 *
 * \todo \feature Allow custom matching equations.
 * \todo \feature Allow generating only closed surfaces.
 */
class NNormalSurfaceList : public NPacket, public NPropertyHolder,
        public NSurfaceSet {
    public:
        static const int packetType;
        
        static const int STANDARD = 0;
            /**< Represents standard triangle-quad coordinates for
             *   normal surfaces. */
        static const int AN_STANDARD = 100;
            /**< Represents standard triangle-quad-oct coordinates
             *   for almost normal surfaces. */

        static const int QUAD = 1;
            /**< Represents quad coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */

        static const int EDGE_WEIGHT = 200;
            /**< Represents edge weight coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */
        static const int FACE_ARCS = 201;
            /**< Represents face arc coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */

    protected:
        NDynamicArray<NNormalSurface*> surfaces;
            /**< Contains the normal surfaces stored in this packet. */
        int flavour;
            /**< Stores which flavour of coordinate system is being
             *   used by the normal surfaces in this packet. */
        bool embedded;
            /**< Stores whether we are only interested in embedded
             *   normal surfaces. */

    public:
        /**
         * Creates a new list of normal surfaces in the given
         * triangulation using the given flavour of coordinate system.
         * The normal surfaces will actually be calculated in
         * this routine, so it may be slow; all vertex surfaces in the
         * given coordinate
         * space will be found and stored.  Their representations will
         * use the smallest possible integer coordinates.
         * The option is offered to find only embedded normal surfaces
         * or to also include immersed and singular normal surfaces.
         *
         * This constructor will insert this normal surface list as a
         * child of the given triangulation.  This triangulation \b must
         * remain the parent of this normal surface list, and must not
         * change while this normal surface list is alive.
         *
         * \todo \feature Allow picking up the first ``interesting'' surface
         * and bailing en route.
         * \todo \featurelong Determine the faces of the normal solution
         * space.
         * \todo \featurelong Allow either subsets of normal surface
         * lists or allow deletion of surfaces from lists.
         * \todo \opt Investigate obvious compressions.
         * \todo \opt Investigate monte carlo methods.
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param newFlavour the flavour of coordinate system to be used;
         * this must be one of the predefined coordinate system
         * constants in NNormalSurfaceList.
         * @param embeddedOnly \c true if only embedded normal surfaces
         * are to be produced, or \c false if immersed and singular
         * normal surfaces are also to be produced; this defaults to
         * \c true.
         */
        NNormalSurfaceList(NTriangulation* owner, int newFlavour,
                bool embeddedOnly = true);
        /**
         * Destroys this list and all the surfaces within.
         */
        ~NNormalSurfaceList();

        virtual int getFlavour() const;
        virtual bool allowsAlmostNormal() const;
        virtual bool isEmbeddedOnly() const;
        virtual NTriangulation* getTriangulation() const;
        virtual unsigned long getNumberOfSurfaces() const;
        virtual const NNormalSurface* getSurface(unsigned long index) const;
        virtual ShareableObject* getShareableObject();
        
        virtual int getPacketType() const;
        virtual NString getPacketName() const;
        virtual void writeTextShort(ostream& o) const;
        virtual void writeTextLong(ostream& o) const;
        virtual void writePacket(NFile& out) const;
        static NNormalSurfaceList* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;

        /**
         * Returns a newly created matrix containing the matching
         * equations that were used to create this normal surface list.
         * The destruction of this matrix is the responsibility of the
         * caller of this routine.  Multiple calls to this routine will
         * result in the construction of multiple matrices.  This
         * routine in fact merely calls makeMatchingEquations() with the
         * appropriate parameters.
         *
         * The format of the matrix is identical to that returned by
         * makeMatchingEquations().
         * 
         * @return the matching equations used to create this normal
         * surface list.
         */
        NMatrixInt* recreateMatchingEquations() const;

    protected:
        /**
         * Creates a new normal surface list performing no intialisation
         * whatsoever other than property initialisation.
         */
        NNormalSurfaceList();

        virtual NPacket* internalClonePacket(NPacket* parent) const;

        virtual void readIndividualProperty(NFile& infile,
            unsigned propType);
        virtual void initialiseAllProperties();
};

/**
 * Returns a newly allocated list of newly allocated rays representing
 * the cone in the given flavour of coordinate system obtained by
 * setting all coordinates non-negative.
 *
 * The list will contain the extremal rays of this cone, each being a
 * unit vector along a coordinate axis.  They will be placed in the list
 * in order from the unit vector along the 0th coordinate axis to the
 * last.
 *
 * The resulting list of extremal rays is guaranteed not to contain any
 * duplicates or redundancies.
 *
 * \ifaces Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param flavour the flavour of coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalSurfaceList.
 * @return a newly allocated list of newly allocated arrays representing
 * the extremal rays of the non-negative cone.
 */
NDoubleList<NNormalSurfaceVector*>* createNonNegativeCone(
        NTriangulation* triangulation, int flavour);
/**
 * Creates a new set of normal surface matching equations for the
 * given triangulation using the given flavour of coordinate system.
 * The returned matrix will be newly allocated and its destruction will
 * be the responsibility of the caller of this routine.
 *
 * Each equation will be represented as a row of the matrix.
 * Each column of the matrix represents a coordinate in the given
 * flavour of coordinate system.
 *
 * @param triangulation the triangulation upon which these matching equations
 * will be based.
 * @param flavour the flavour of coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalSurfaceList.
 * @return a newly allocated set of matching equations.
 */
NMatrixInt* makeMatchingEquations(NTriangulation* triangulation,
    int flavour);

// Inline functions for NNormalSurfaceList

inline NNormalSurfaceList::NNormalSurfaceList() {
    NNormalSurfaceList::initialiseAllProperties();
}

inline NNormalSurfaceList::~NNormalSurfaceList() {
    surfaces.flushAndDelete();
}

inline int NNormalSurfaceList::getFlavour() const {
    return flavour;
}

inline bool NNormalSurfaceList::isEmbeddedOnly() const {
    return embedded;
}

inline NTriangulation* NNormalSurfaceList::getTriangulation() const {
    return (NTriangulation*)getTreeParent();
}

inline unsigned long NNormalSurfaceList::getNumberOfSurfaces() const {
    return surfaces.size();
}

inline const NNormalSurface* NNormalSurfaceList::getSurface(
        unsigned long index) const {
    return surfaces[index];
}

inline ShareableObject* NNormalSurfaceList::getShareableObject() {
    return this;
}

inline bool NNormalSurfaceList::dependsOnParent() const {
    return true;
}

inline NMatrixInt* NNormalSurfaceList::recreateMatchingEquations() const {
    return makeMatchingEquations(getTriangulation(), flavour);
}

#endif

