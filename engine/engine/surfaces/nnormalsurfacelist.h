
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

/*! \file nnormalsurfacelist.h
 *  \brief Contains a packet representing a collection of normal
 *  surfaces in a 3-manifold.
 */

#ifndef __NNORMALSURFACELIST_H
#ifndef __DOXYGEN
#define __NNORMALSURFACELIST_H
#endif

#include <algorithm>
#include <vector>
#include "packet/npacket.h"
#include "property/npropertyholder.h"
#include "utilities/memutils.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nsurfaceset.h"

namespace regina {

class NTriangulation;
class NMatrixInt;
class NXMLPacketReader;
class NXMLNormalSurfaceListReader;

/**
 * \weakgroup surfaces
 * @{
 */

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
 * \todo \feature Generate facets of the solution space representing
 * embedded surfaces.
 */
class NNormalSurfaceList : public NPacket, public NPropertyHolder,
        public NSurfaceSet {
    public:
        static const int packetType;
        
        static const int STANDARD;
            /**< Represents standard triangle-quad coordinates for
             *   normal surfaces. */
        static const int AN_STANDARD;
            /**< Represents standard triangle-quad-oct coordinates
             *   for almost normal surfaces. */

        static const int QUAD;
            /**< Represents quad coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */

        static const int EDGE_WEIGHT;
            /**< Represents edge weight coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */
        static const int FACE_ARCS;
            /**< Represents face arc coordinates for normal surfaces.
             *   This flavour is for representation only; surface
             *   vectors and lists of this flavour cannot be created. */

    protected:
        std::vector<NNormalSurface*> surfaces;
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
        virtual ~NNormalSurfaceList();

        virtual int getFlavour() const;
        virtual bool allowsAlmostNormal() const;
        virtual bool isEmbeddedOnly() const;
        virtual NTriangulation* getTriangulation() const;
        virtual unsigned long getNumberOfSurfaces() const;
        virtual const NNormalSurface* getSurface(unsigned long index) const;
        virtual ShareableObject* getShareableObject();
        
        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
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
        virtual void writeXMLPacketData(std::ostream& out) const;

        virtual void readIndividualProperty(NFile& infile,
            unsigned propType);
        virtual void initialiseAllProperties();

        /**
         * An output iterator used to insert surfaces into an
         * NNormalSurfaceList.
         *
         * Objects of type <tt>NNormalSurface*</tt> and
         * <tt>NNormalSurfaceVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NNormalSurface
         * will be automatically created.
         *
         * Some basic checks may be performed before insertion; see the
         * documentation for operator=(NNormalSurface*) and
         * operator=(NNormalSurfaceVector*) for details.
         */
        friend struct SurfaceInserter {
            NNormalSurfaceList* list;
                /**< The list into which surfaces will be inserted. */
            NTriangulation* owner;
                /**< The triangulation in which the surfaces to be
                 *   inserted are contained. */

            /**
             * Creates a new uninitialised output iterator.
             *
             * \warning This iterator must not be used until its
             * surface list and triangulation have been initialised.
             */
            SurfaceInserter();
            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param newList the list into which surfaces will be
             * inserted.
             * @param newOwner the triangulation in which the surfaces
             * to be inserted are contained.
             */
            SurfaceInserter(NNormalSurfaceList& newList,
                NTriangulation* newOwner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            SurfaceInserter(const SurfaceInserter& cloneMe);

            /**
             * Sets this iterator to be a clone of the given output iterator.
             *
             * @param cloneMe the output iterator to clone.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(const SurfaceInserter& cloneMe);

            /**
             * Appends a normal surface to the end of the appropriate
             * surface list.
             *
             * The given surface will be deallocated with the other
             * surfaces in this list.
             *
             * @param surface the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NNormalSurface* surface);
            /**
             * Appends the normal surface corresponding to the given
             * vector to the end of the appropriate surface list.
             *
             * The given vector will be owned by the newly created
             * normal surface and will be deallocated with the other
             * surfaces in this list.
             *
             * If the surface list allows almost normal surfaces, the
             * vector will be checked for multiple octagonal discs.  If
             * multiple octagonal discs are found, the vector will be
             * deleted immediately and no surface will be inserted.
             *
             * @param vector the vector of the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NNormalSurfaceVector* vector);

            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator *();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++(int);
        };

    friend class regina::NXMLNormalSurfaceListReader;
};

/**
 * Writes to the given output iterators newly allocated rays and faces
 * representing the cone in the given flavour of coordinate system obtained by
 * setting all coordinates non-negative.
 *
 * To \a rays will be written the extremal rays of this cone,
 * each being a unit vector along a coordinate axis.  They will be written
 * in order from the unit vector along the 0th coordinate axis
 * to the last, and will all be of the subclass of NNormalSurfaceVector
 * corresponding to the given flavour of coordinate system.
 *
 * To \a faces will be written the vectors perpendicular to the
 * hyperplanes that make up the faces of this cone.  Each of these
 * vectors will also be a unit vector along a coordinate axis.  These
 * vectors will be written in the same order as the rays, and
 * will all be of class NVectorUnit.
 *
 * The resulting lists of extremal rays and faces are guaranteed not to
 * contain any duplicates or redundancies.
 *
 * \ifaces Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param flavour the flavour of coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalSurfaceList.
 * @param rays the output iterator to which the newly allocated extremal rays
 * will be written; this must accept objects of type <tt>NConeRay*</tt>.
 * @param faces the output iterator to which the newly allocated face
 * perpendiculars will be written; this must accept objects of type
 * <tt>NVector\<NLargeInteger\>*</tt>.
 */
template <class RayOutputIterator, class FaceOutputIterator>
void createNonNegativeCone(NTriangulation* triangulation, int flavour,
    RayOutputIterator rays, FaceOutputIterator faces);
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

/*@}*/

// Inline functions for NNormalSurfaceList

inline NNormalSurfaceList::NNormalSurfaceList() {
    NNormalSurfaceList::initialiseAllProperties();
}

inline NNormalSurfaceList::~NNormalSurfaceList() {
    for_each(surfaces.begin(), surfaces.end(), FuncDelete<NNormalSurface>());
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

inline NNormalSurfaceList::SurfaceInserter::SurfaceInserter() : list(0),
        owner(0) {
}

inline NNormalSurfaceList::SurfaceInserter::SurfaceInserter(
        NNormalSurfaceList& newList, NTriangulation* newOwner) :
        list(&newList), owner(newOwner) {
}

inline NNormalSurfaceList::SurfaceInserter::SurfaceInserter(
        const SurfaceInserter& cloneMe) : list(cloneMe.list),
        owner(cloneMe.owner) {
}


inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        const SurfaceInserter& cloneMe) {
    list = cloneMe.list;
    owner = cloneMe.owner;
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        NNormalSurface* surface) {
    list->surfaces.push_back(surface);
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        NNormalSurfaceVector* vector) {
    if ((! list->allowsAlmostNormal()) ||
            (! vector->hasMultipleOctDiscs(owner)))
        list->surfaces.push_back(new NNormalSurface(owner, vector));
    else
        delete vector;
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator *() {
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator ++() {
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator ++(int) {
    return *this;
}

} // namespace regina

// Template definitions

#include "surfaces/nnormalsurfacelist.tcc"

#endif

