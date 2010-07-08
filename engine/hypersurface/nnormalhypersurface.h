
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file hypersurface/nnormalhypersurface.h
 *  \brief Deals with an individual normal hypersurface in a 4-manifold
 *  triangulation.
 */

#ifndef __NNORMALHYPERSURFACE_H
#ifndef __DOXYGEN
#define __NNORMALHYPERSURFACE_H
#endif

#include <utility>
#include "shareableobject.h"
#include "maths/nperm5.h"
#include "maths/nray.h"
#include "utilities/nbooleans.h"
#include "utilities/nproperty.h"

namespace regina {

/**
 * \addtogroup hypersurface Normal Hypersurfaces
 * Normal hypersurfaces in 4-manifold triangulations.
 * @{
 */

class Dim4Edge;
class Dim4Triangulation;
class Dim4Vertex;
class NEnumConstraintList;
class NTriangulation;
class NXMLNormalHypersurfaceReader;

/**
 * Stores the vector of a single normal hypersurface in a 4-manifold
 * triangulation.  The different subclasses of NNormalHypersurfaceVector use
 * different underlying coordinate systems for the normal solution space.
 * However, the various coordinate retrieval routines will return values
 * that are independent of the underlying coordinate system.  Thus the
 * coordinates of the normal hypersurface in any coordinate system can be
 * determined without knowledge of the specific underlying coordinate
 * system being used.
 *
 * Note that if a mirrored vector class is being used (see
 * NNormalHypersurfaceVectorMirrored), the vector <b>may not change</b> once
 * the first coordinate lookup routine (such as getTetrahedronCoord() and
 * the like) has been called.  See
 * NNormalHypersurfaceVectorMirrored for further explanation.
 *
 * Note that non-compact hypersurfaces (surfaces with infinitely many pieces)
 * are allowed; in these cases, the corresponding coordinate lookup routines
 * should return NLargeInteger::infinity where appropriate.
 *
 * All subclasses of NNormalHypersurfaceVector <b>must</b> have the following
 * properties:
 *
 * - Normal hypersurfaces can be enumerated by intersecting the non-negative
 *   orthant of the underlying vector space with some linear subspace;
 *
 * - Multiplying a normal hypersurface by \a k corresponds to multiplying
 *   the underlying vector by \a k for any non-negative integer \a k.
 *
 * <b>When deriving classes from NNormalHypersurfaceVector:</b>
 * <ul>
 *   <li>A unique constant (static const int) must be added to the class
 *   NNormalHypersurfaceList to represent the new flavour of coordinate
 *   system.</li>
 *   <li>The file hsflavourregistry.h must be updated to reflect the new
 *   flavour of coordinate system.</li>
 *   <li>Constructors <tt>class(unsigned length)</tt> and
 *   <tt>class(const NVector<NLargeInteger>& cloneMe)</tt> must be
 *   declared and implemented; these will usually just call the
 *   corresponding superclass constructors.</li>
 *   <li>The virtual function <tt>NVector<NLargeInteger>* clone() const</tt>
 *   must be declared but not implemented.  The registry utilities will take
 *   care of its implementation.</li>
 *   <li>All abstract functions must be implemented.</li>
 *   <li>Static public functions <tt>void
 *   makeZeroVector(const Dim4Triangulation*)</tt>,
 *   <tt>NMatrixInt* makeMatchingEquations(Dim4Triangulation*)</tt> and
 *   makeEmbeddedConstraints(Dim4Triangulation*) must be
 *   declared and implemented.</li>
 * </ul>
 *
 * \ifacespython Not present.
 */
class NNormalHypersurfaceVector : public NRay {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalHypersurfaceVector(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalHypersurfaceVector(const NVector<NLargeInteger>& cloneMe);

        /**
         * Determines if the normal hypersurface represented is compact (has
         * finitely many pieces).
         *
         * The default implementation for this routine simply runs
         * through every piece type until a piece type with infinite piece
         * count is found or all piece types have been examined.
         * Subclasses of NNormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return \c true if and only if the normal hypersurface represented
         * is compact.
         */
        virtual bool isCompact(Dim4Triangulation* triang) const;
        /**
         * Determines if the normal hypersurface represented is vertex
         * linking.  A <i>vertex linking</i> hypersurface contains only
         * tetrahedra.
         *
         * The default implementation for this routine simply runs
         * through every non-tetrahedron piece type ensuring that each
         * has no corresponding pieces.
         * Subclasses of NNormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return \c true if and only if the normal hypersurface represented
         * is vertex linking.
         */
        virtual bool isVertexLinking(Dim4Triangulation* triang) const;
        /**
         * Determines if a rational multiple of the normal hypersurface
         * represented is the link of a single vertex.
         *
         * The default implementation for this routine involves counting the
         * number of pieces of every type.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the vertex linked by this hypersurface, or 0 if this
         * hypersurface is not the link of a single vertex.
         */
        virtual const Dim4Vertex* isVertexLink(Dim4Triangulation* triang) const;
        /**
         * Determines if a rational multiple of the normal hypersurface
         * represented is the thin link of a single edge.
         *
         * The default implementation for this routine involves counting the
         * number of pieces of every type.
         * Subclasses of NNormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the edge linked by this hypersurface, or 0 if this
         * hypersurface is not a thin edge link.
         */
        virtual const Dim4Edge* isThinEdgeLink(Dim4Triangulation* triang) const;

        /**
         * Returns the number of tetrahedron pieces of the given type in
         * this normal hypersurface.
         * See NNormalHypersurface::getTetrahedronCoord() for further details.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested tetrahedron pieces reside;
         * this should be between 0 and
         * Dim4Triangulation::getNumberOfPentachora()-1 inclusive.
         * @param vertex the vertex of the given pentachoron around
         * which the requested tetrahedron pieces lie; this should be between
         * 0 and 4 inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of tetrahedron pieces of the given type.
         */
        virtual NLargeInteger getTetrahedronCoord(unsigned long pentIndex,
            int vertex, Dim4Triangulation* triang) const = 0;
        /**
         * Returns the number of prism pieces of the given type
         * in this normal hypersurface.
         * See NNormalHypersurface::getPrismCoord() for further details.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested prism pieces reside;
         * this should be between 0 and
         * Dim4Triangulation::getNumberOfPentachora()-1 inclusive.
         * @param prismType the number of the vertex splitting that this
         * prism type represents; this should be between 0 and 9
         * inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of prism pieces of the given type.
         */
        virtual NLargeInteger getPrismCoord(unsigned long pentIndex,
            int prismType, Dim4Triangulation* triang) const = 0;
        /**
         * Returns the number of times this normal hypersurface crosses the
         * given edge.
         * See NNormalHypersurface::getEdgeWeight() for further details.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Dim4Triangulation::getNumberOfEdges()-1 inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of times this normal hypersurface crosses the
         * given edge.
         */
        virtual NLargeInteger getEdgeWeight(unsigned long edgeIndex,
            Dim4Triangulation* triang) const = 0;

        /**
         * Returns a new normal hypersurface vector of the appropriate length
         * for the given triangulation and for the flavour of coordinate
         * system corresponding to this subclass of NNormalHypersurfaceVector.
         * All elements of the new vector will be initialised to zero.
         *
         * See ::makeZeroVector() for further details.
         *
         * @param triangulation the triangulation upon which the
         * underlying coordinate system is based.
         * @return a new zero vector of the correct class and length.
         */
        #ifdef __DOXYGEN
            static NNormalHypersurfaceVector* makeZeroVector(
                const Dim4Triangulation* triangulation);
        #endif
        /**
         * Creates a new set of normal hypersurface matching equations for
         * the given triangulation using the flavour of coordinate
         * system corresponding to this particular subclass of
         * NNormalHypersurfaceVector.
         *
         * See ::makeMatchingEquations() for further details.
         *
         * @param triangulation the triangulation upon which these
         * matching equations will be based.
         * @return a newly allocated set of matching equations.
         */
        #ifdef __DOXYGEN
            static NMatrixInt* makeMatchingEquations(
                Dim4Triangulation* triangulation);
        #endif
        /**
         * Creates a new set of validity constraints representing
         * the condition that normal hypersurfaces be embedded.  The
         * validity constraints will be expressed relative to the
         * flavour of coordinate system corresponding to this particular
         * subclass of NNormalHypersurfaceVector.
         *
         * @param triangulation the triangulation upon which these
         * validity constraints will be based.
         * @return a newly allocated set of constraints.
         */
        #ifdef __DOXYGEN
            static NEnumConstraintList* makeEmbeddedConstraints(
                Dim4Triangulation* triangulation);
        #endif
};

/**
 * Represents a single normal hypersurface in a 4-manifold triangulation.
 * Once the underlying triangulation changes, this normal hypersurface object
 * is no longer valid.
 *
 * The information provided by the various query methods is independent
 * of the underlying coordinate system being used.
 * See the NNormalHypersurfaceVector class notes for details of what to do
 * when introducing a new flavour of coordinate system.
 *
 * Note that non-compact surfaces (surfaces with infinitely many pieces,
 * are allowed; in these cases, the corresponding coordinate lookup routines
 * will return NLargeInteger::infinity where appropriate.
 */
class NNormalHypersurface : public ShareableObject {
    protected:
        NNormalHypersurfaceVector* vector_;
            /**< Contains the coordinates of the normal hypersurface in
             *   whichever space is appropriate. */
        Dim4Triangulation* triangulation_;
            /**< The triangulation in which this normal hypersurface resides. */

        std::string name_;
            /**< An optional name associated with this hypersurface. */

        mutable NProperty<bool> realBoundary_;
            /**< Does this hypersurface have real boundary (i.e. does it meet
             *   any boundary facets)? */
        mutable NProperty<bool> compact_;
            /**< Is this hypersurface compact (i.e., does it only
             *   contain finitely many pieces)? */

    public:
        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector.
         *
         * \pre The given coordinate vector represents a
         * normal hypersurface inside the given triangulation.
         * \pre The given coordinate vector cannot be the null pointer.
         *
         * \ifacespython Not present.
         *
         * @param triang the triangulation in which this normal hypersurface
         * resides.
         * @param newVector a vector containing the coordinates of the
         * normal hypersurface in whichever space is appropriate.
         */
        NNormalHypersurface(Dim4Triangulation* triangulation,
            NNormalHypersurfaceVector* vector);
        /**
         * Destroys this normal hypersurface.
         * The underlying vector of coordinates will also be deallocated.
         */
        virtual ~NNormalHypersurface();

        /**
         * Creates a newly allocated clone of this normal hypersurface.
         *
         * The name of the normal hypersurface will \e not be copied to the
         * clone; instead the clone will have an empty name.
         *
         * @return a clone of this normal hypersurface.
         */
        NNormalHypersurface* clone() const;

        /**
         * Returns the number of tetrahedron pieces of the given type in
         * this normal hypersurface.
         * A tetrahedron piece type is identified by specifying a
         * pentachoron and a vertex of that pentachoron that the
         * tetrahedron surrounds.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested pieces reside;
         * this should be between 0 and
         * Dim4Triangulation::getNumberOfPentachora()-1 inclusive.
         * @param vertex the vertex of the given pentachoron around
         * which the requested pieces lie; this should be between 0
         * and 4 inclusive.
         * @return the number of tetrahedron pieces of the given type.
         */
        NLargeInteger getTetrahedronCoord(unsigned long pentIndex,
            int vertex) const;
        /**
         * Returns the number of prism pieces of the given type
         * in this normal hypersurface.
         * A prism piece type is identified by specifying a
         * pentachoron and an edge of that pentachoron; prisms of this
         * type will then separate the given edge from the remaining three
         * pentachoron vertices.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested prisms reside;
         * this should be between 0 and
         * Dim4Triangulation::getNumberOfPentachora()-1 inclusive.
         * @param prismType specifies the edge of the given pentachoron that
         * this prism separates from the remaining three vertices;
         * this should be between 0 and 9 inclusive.
         * @return the number of prism pieces of the given type.
         */
        NLargeInteger getPrismCoord(unsigned long pentIndex,
            int prismType) const;
        /**
         * Returns the number of times this normal hypersurface crosses the
         * given edge.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Dim4Triangulation::getNumberOfEdges()-1 inclusive.
         * @return the number of times this normal hypersurface crosses the
         * given edge.
         */
        NLargeInteger getEdgeWeight(unsigned long edgeIndex) const;

        /**
         * Returns the number of coordinates in the specific underlying
         * coordinate system being used.
         *
         * @return the number of coordinates.
         */
        unsigned getNumberOfCoords() const;
        /**
         * Returns the triangulation in which this normal hypersurface
         * resides.
         *
         * @return the underlying triangulation.
         */
        Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * @return the name of associated with this hypersurface.
         */
        const std::string& getName() const;
        /**
         * Sets the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * @param name the new name to associate with this hypersurface.
         */
        void setName(const std::string& name);

        /**
         * The text representation will be in standard tetrahedron-prism
         * coordinates.
         *
         * \ifacespython The paramater \a out does not exist, and is
         * taken to be standard output.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes the underlying coordinate vector to the given output
         * stream in text format.
         * No indication will be given as to which coordinate
         * system is being used or what each coordinate means.
         * No newline will be written.
         *
         * \ifacespython The paramater \a out does not exist, and is
         * taken to be standard output.
         *
         * @param out the output stream to which to write.
         */
        void writeRawVector(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this normal hypersurface and all
         * of its properties.  This routine will be called from within
         * NNormalHypersurfaceList::writeXMLPacketData().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        virtual void writeXMLData(std::ostream& out) const;

        /**
         * Determines if this normal hypersurface is empty (has no
         * pieces whatsoever).
         */
        bool isEmpty() const;
        /**
         * Determines if this normal hypersurface is compact (has
         * finitely many pieces).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * @return \c true if and only if this normal hypersurface is compact.
         */
        bool isCompact() const;
        /**
         * Determines if this hypersurface has any real boundary, that is,
         * whether it meets any boundary facets of the triangulation.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * @return \c true if and only if this hypersurface has real boundary.
         */
        bool hasRealBoundary() const;
        /**
         * Determines whether or not this hypersurface is vertex linking.
         * A <i>vertex linking</i> hypersurface contains only tetrahedra.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this hypersurface is vertex linking.
         */
        bool isVertexLinking() const;
        /**
         * Determines whether or not a rational multiple of this hypersurface
         * is the link of a single vertex.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return the vertex linked by this hypersurface, or 0 if this
         * hypersurface is not the link of a single vertex.
         */
        virtual const Dim4Vertex* isVertexLink() const;
        /**
         * Determines whether or not a rational multiple of this hypersurface
         * is the thin link of a single edge.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return the edge linked by this hypersurface, or 0 if this
         * hypersurface is not a thin edge link.
         */
        virtual const Dim4Edge* isThinEdgeLink() const;

        /**
         * Returns a 3-manifold triangulation describing this normal
         * hypersurface.
         *
         * The triangulation will be simplified via
         * NTriangulation::intelligentSimplify(), which means that the
         * tetrahedra of the final triangulation are not likely to
         * correspond to any particular tetrahedron/prism pieces of
         * this normal hypersurface.
         *
         * The 3-manifold triangulation will be newly allocated, and
         * destroying it is the responsibility of the caller of this routine.
         *
         * \todo \prob Check for absurdly large numbers of pieces and
         * return 0 accordingly.
         *
         * @return a triangulation of this normal hypersurface.
         */
        NTriangulation* triangulate() const;

        /**
         * Determines whether this and the given hypersurface in fact
         * represent the same normal hypersurface.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal pieces of each type, and returns \c true
         * if and only if these counts are the same for both hypersurfaces.
         *
         * It does not matter what coordinate systems the two hypersurfaces
         * use.  In particular, it does not matter if this and the
         * given hypersurface use different coordinate systems.
         *
         * \pre Both this and the given normal hypersurface live within the
         * same 4-manifold triangulation.
         *
         * @param other the hypersurface to be compared with this hypersurface.
         * @return \c true if both hypersurfaces represent the same normal
         * hypersurface, or \c false if not.
         */
        bool sameSurface(const NNormalHypersurface& other) const;

        /**
         * Determines whether this and the given hypersurface are locally
         * compatible.  Local compatibility means that, within each individual
         * pentachoron of the triangulation, it is possible to arrange the
         * normal discs of both hypersurfaces so that none intersect.
         *
         * This is a local constraint, not a global constraint.  That is,
         * we do not insist that we can avoid intersections within all
         * pentachora \e simultaneously.
         *
         * Local compatibility can be formulated in terms of normal piece
         * types.  Two normal hypersurfaces are locally compatible if
         * and only if they together use at most two prism piece types per
         * pentachoron; moreover, if there \e are two prism piece types within
         * a single pentachoron then these prism types are non-intersecting.
         *
         * If one of the two hypersurfaces breaks the local compatibility
         * constraints on its own (for instance, it contains two conflicting
         * prism types within the same pentachoron), then this
         * routine will return \c false regardless of what the other
         * hypersurface contains.
         *
         * \pre Both this and the given normal hypersurface live within the
         * same 4-manifold triangulation.
         *
         * @param other the other hypersurface to test for local compatibility
         * with this hypersurface.
         * @return \c true if the two hypersurfaces are locally compatible, or
         * \c false if they are not.
         */
        bool locallyCompatible(const NNormalHypersurface& other) const;

        /**
         * Gives read-only access to the raw vector that sits beneath this
         * normal hypersurface.
         *
         * Generally users should not need this function.  However, it is
         * provided here in case the need should arise (e.g., for reasons
         * of efficiency).
         *
         * \warning An NNormalHypersurface does not know what underlying
         * coordinate system its raw vector uses.  Unless you already know
         * the coordinate system in advance (i.e., you created the hypersurface
         * yourself), it is best to keep to the coordinate-system-agnostic
         * access functions such as
         * NNormalHypersurfaceVector::getTetrahedronCoord() and
         * NNormalHypersurfaceVector::getPrismCoord().
         *
         * \ifacespython Not present.
         *
         * @return the underlying raw vector.
         */
        const NNormalHypersurfaceVector* rawVector() const;

    protected:
        /**
         * Calculates whether this hypersurface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary() const;

    friend class regina::NXMLNormalHypersurfaceReader;
};

/*@}*/

// Inline functions for NNormalHypersurfaceVector

inline NNormalHypersurfaceVector::NNormalHypersurfaceVector(unsigned length) :
        NRay(length) {
}
inline NNormalHypersurfaceVector::NNormalHypersurfaceVector(
        const NVector<NLargeInteger>& cloneMe) : NRay(cloneMe) {
}

// Inline functions for NNormalHypersurface

inline NNormalHypersurface::~NNormalHypersurface() {
    delete vector_;
}

inline NLargeInteger NNormalHypersurface::getTetrahedronCoord(
        unsigned long pentIndex, int vertex) const {
    return vector_->getTetrahedronCoord(pentIndex, vertex, triangulation_);
}
inline NLargeInteger NNormalHypersurface::getPrismCoord(
        unsigned long pentIndex, int prismType) const {
    return vector_->getPrismCoord(pentIndex, prismType, triangulation_);
}
inline NLargeInteger NNormalHypersurface::getEdgeWeight(unsigned long edgeIndex)
        const {
    return vector_->getEdgeWeight(edgeIndex, triangulation_);
}

inline unsigned NNormalHypersurface::getNumberOfCoords() const {
    return vector_->size();
}
inline Dim4Triangulation* NNormalHypersurface::getTriangulation() const {
    return triangulation_;
}

inline const std::string& NNormalHypersurface::getName() const {
    return name_;
}
inline void NNormalHypersurface::setName(const std::string& name) {
    name_ = name;
}

inline void NNormalHypersurface::writeRawVector(std::ostream& out) const {
    out << *vector_;
}

inline bool NNormalHypersurface::isCompact() const {
    if (! compact_.known())
        compact_ = vector_->isCompact(triangulation_);
    return compact_.value();
}

inline bool NNormalHypersurface::hasRealBoundary() const {
    if (! realBoundary_.known())
        calculateRealBoundary();
    return realBoundary_.value();
}

inline bool NNormalHypersurface::isVertexLinking() const {
    return vector_->isVertexLinking(triangulation_);
}

inline const Dim4Vertex* NNormalHypersurface::isVertexLink() const {
    return vector_->isVertexLink(triangulation_);
}

inline const Dim4Edge* NNormalHypersurface::isThinEdgeLink() const {
    return vector_->isThinEdgeLink(triangulation_);
}

inline const NNormalHypersurfaceVector* NNormalHypersurface::rawVector() const {
    return vector_;
}

} // namespace regina

#endif

