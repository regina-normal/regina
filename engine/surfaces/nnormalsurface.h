
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

/*! \file surfaces/nnormalsurface.h
 *  \brief Deals with an individual normal surface.
 */

#ifndef __NNORMALSURFACE_H
#ifndef __DOXYGEN
#define __NNORMALSURFACE_H
#endif

#include <utility>
#include "shareableobject.h"
#include "file/nfilepropertyreader.h"
#include "maths/nperm4.h"
#include "maths/nray.h"
#include "surfaces/ndisctype.h"
#include "utilities/nbooleans.h"
#include "utilities/nproperty.h"

namespace regina {

/**
 * \addtogroup surfaces Normal Surfaces
 * Normal surfaces in triangulations.
 * @{
 */

/**
 * Lists which vertex splits split which pairs of vertices.
 * There are three vertex splits, numbered 0,1,2.  Each vertex
 * split split the four tetrahedron vertices 0,1,2,3 into two pairs.
 * <tt>vertexSplit[i][j]</tt> is the number of the vertex split that
 * keeps vertices <tt>i</tt> and <tt>j</tt> together.
 *
 * It is guaranteed that vertex split \a i will keep the vertices of
 * edge \a i together (and will therefore also keep the vertices of edge
 * \a 5-i together).
 */
extern const int vertexSplit[4][4];
/**
 * Lists which vertex splits meet which edges.
 * See \a vertexSplit for details on what a vertex split is.
 * <tt>vertexSplitMeeting[i][j][0,1]</tt> are the numbers of the two
 * vertex splits that meet the edge joining tetrahedron vertices
 * <tt>i</tt> and <tt>j</tt>.
 */
extern const int vertexSplitMeeting[4][4][2];

/**
 * Lists the vertices which each vertex split splits.
 * See \a vertexSplit for details on what a vertex split is.
 * Vertex split number \c i splits the vertex pairs
 * <tt>vertexSplitDefn[i][0,1]</tt> and
 * <tt>vertexSplitDefn[i][2,3]</tt>.
 */
extern const int vertexSplitDefn[3][4];

/**
 * Lists the second vertex with which each vertex is paired under each
 * vertex split.
 * See \a vertexSplit for details on what a vertex split is.
 * Vertex split number \c i pairs vertex \c v with
 * vertex <tt>vertexSplitPartner[i][v]</tt>.
 */
extern const int vertexSplitPartner[3][4];

/**
 * Contains strings describing which vertices each vertex split splits.
 * See \a vertexSplit for details on what a vertex split is.
 * The string describing vertex split number \c i is
 * <tt>vertexSplitString[i]</tt> and is of the form <tt>02/13</tt>,
 * which in this case is the vertex split that splits vertices 0,2 from
 * vertices 1,3.
 */
extern const char vertexSplitString[3][6];

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of triangular normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>triDiscArcs[i]</tt> lists the boundary arcs of the
 * triangular disc of type <i>i</i>.  See NNormalSurface::getTriangleCoord()
 * for further details.
 *
 * Note that every permutation in this array is even.
 *
 * \ifacescpp This array is replaced by a macro
 * <tt>triDiscArcs(discType, arcIndex)</tt> that essentially looks up
 * the corresponding array.  This is necessary because of a bug in gcc 2.95.
 */
#ifdef __DOXYGEN
extern const NPerm4 triDiscArcs[4][3];
#else
extern const NPerm4 __triDiscArcs[12];
#define triDiscArcs(i, j) __triDiscArcs[(3 * (i)) + (j)]
#endif

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of quadrilateral normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>quadDiscArcs[i]</tt> lists the boundary arcs of the
 * quadrilateral disc of type <i>i</i>.  See NNormalSurface::getQuadCoord()
 * for further details.
 *
 * Note that permutation <tt>quadDiscArcs[i][j]</tt> will be even
 * precisely when <tt>j</tt> is even.
 *
 * \ifacescpp This array is replaced by a macro
 * <tt>quadDiscArcs(discType, arcIndex)</tt> that essentially looks up
 * the corresponding array.  This is necessary because of a bug in gcc 2.95.
 */
#ifdef __DOXYGEN
extern const NPerm4 quadDiscArcs[3][4];
#else
extern const NPerm4 __quadDiscArcs[12];
#define quadDiscArcs(i, j) __quadDiscArcs[(4 * (i)) + (j)]
#endif

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of octagonal normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>octDiscArcs[i]</tt> lists the boundary arcs of the
 * octagonal disc of type <i>i</i>.  See NNormalSurface::getOctCoord()
 * for further details.
 *
 * Note that permutation <tt>octDiscArcs[i][j]</tt> will be even
 * precisely when <tt>j</tt> is 0, 1, 4 or 5.
 *
 * \ifacescpp This array is replaced by a macro
 * <tt>octDiscArcs(discType, arcIndex)</tt> that essentially looks up
 * the corresponding array.  This is necessary because of a bug in gcc 2.95.
 */
#ifdef __DOXYGEN
extern const NPerm4 octDiscArcs[3][8];
#else
extern const NPerm4 __octDiscArcs[24];
#define octDiscArcs(i, j) __octDiscArcs[(8 * (i)) + (j)]
#endif

class NEnumConstraintList;
class NTriangulation;
class NEdge;
class NVertex;
class NXMLNormalSurfaceReader;

/**
 * Stores the vector of a single normal surface in a 3-manifold.
 * The different subclasses of NNormalSurfaceVector use different
 * underlying coordinate systems for the normal solution space.
 * However, the various coordinate retrieval routines will return values
 * that are independent of the underlying coordinate system.  Thus the
 * coordinates of the normal surface in any coordinate system can be
 * determined without knowledge of the specific underlying coordinate
 * system being used.
 *
 * Note that if a mirrored vector class is being used (see
 * NNormalSurfaceVectorMirrored), the vector <b>may not change</b> once
 * the first coordinate lookup routine (such as getTriangleCoord() and
 * the like) has been called.  See
 * NNormalSurfaceVectorMirrored for further explanation.
 *
 * Note that non-compact surfaces (surfaces with infinitely many discs,
 * such as spun normal surfaces) are allowed; in these cases, the
 * corresponding coordinate lookup routines should return
 * NLargeInteger::infinity where appropriate.
 *
 * All subclasses of NNormalSurfaceVector <b>must</b> have the following
 * properties:
 *
 * - Normal surfaces can be enumerated by intersecting the non-negative
 *   orthant of the underlying vector space with some linear subspace;
 *
 * - Multiplying a normal surface by \a k corresponds to multiplying
 *   the underlying vector by \a k for any non-negative integer \a k.
 *
 * <b>When deriving classes from NNormalSurfaceVector:</b>
 * <ul>
 *   <li>A unique constant (static const int) must be added to the class
 *   NNormalSurfaceList to represent the new flavour of coordinate
 *   system.</li>
 *   <li>The file flavourregistry.h must be updated to reflect the new
 *   flavour of coordinate system.</li>
 *   <li>Constructors <tt>class(unsigned length)</tt> and
 *   <tt>class(const NVector<NLargeInteger>& cloneMe)</tt> must be
 *   declared and implemented; these will usually just call the
 *   corresponding superclass constructors.</li>
 *   <li>Virtual functions <tt>NVector<NLargeInteger>* clone() const</tt> and
 *   <tt>bool allowsAlmostNormal() const</tt> must be declared but not
 *   implemented.  The registry utilities will take care of their
 *   implementations.</li>
 *   <li>All abstract functions must be implemented.</li>
 *   <li>Static public functions <tt>void
 *   makeZeroVector(const NTriangulation*)</tt>,
 *   <tt>NMatrixInt* makeMatchingEquations(NTriangulation*)</tt> and
 *   makeEmbeddedConstraints(NTriangulation*) must be
 *   declared and implemented.</li>
 * </ul>
 *
 * \testpart
 *
 * \todo \optlong Investigate using sparse vectors for storage.
 *
 * \ifacespython Not present.
 */
class NNormalSurfaceVector : public NRay {
    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVector(unsigned length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVector(const NVector<NLargeInteger>& cloneMe);

        /**
         * Determines if the specific underlying coordinate system
         * allows for almost normal surfaces, that is, allows for
         * octagonal discs.
         *
         * Note that this has nothing to do with whether or not this
         * specific surface \e contains octagonal discs.
         *
         * @return \c true if and only if almost normal surfaces are allowed.
         */
        virtual bool allowsAlmostNormal() const = 0;

        /**
         * Determines if this normal surface has more than one
         * octagonal disc.  It may be assumed that at most one
         * octagonal disc \e type exists in this surface.  This routine will
         * return \c true if an octagonal type does exist and its
         * coordinate is greater than one.
         *
         * The default implementation for this routine simply calculates
         * all the octagonal coordinates and returns as soon as a
         * positive or negative result can be established.  Subclasses
         * of NNormalSurfaceVector should override this if they can provide a
         * faster implementation.
         *
         * If a subclass does not allow for almost normal surfaces, this
         * routine will never be called and thus does not need to be
         * overwritten.
         *
         * \pre At most one octagonal disc \e type exists in this surface.
         * \pre This normal surface vector is using a
         * flavour of coordinate system that allows for almost normal
         * surfaces.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if there is an octagonal disc type
         * present and its coordinate is greater than one.
         */
        virtual bool hasMultipleOctDiscs(NTriangulation* triang) const;
        /**
         * Determines if the normal surface represented is compact (has
         * finitely many discs).
         *
         * The default implementation for this routine simply runs
         * through every disc type until a disc type with infinite disc
         * count is found or all disc types have been examined.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if the normal surface represented
         * is compact.
         */
        virtual bool isCompact(NTriangulation* triang) const;
        /**
         * Determines if the normal surface represented is vertex
         * linking.  A <i>vertex linking</i> surface contains only
         * triangles.
         *
         * The default implementation for this routine simply runs
         * through every non-triangular disc type ensuring that each
         * has no corresponding discs.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if the normal surface represented
         * is vertex linking.
         */
        virtual bool isVertexLinking(NTriangulation* triang) const;
        /**
         * Determines if a rational multiple of the normal surface represented
         * is the link of a single vertex.
         *
         * The default implementation for this routine involves counting the
         * number of discs of every type.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return the vertex linked by this surface, or 0 if this
         * surface is not the link of a single vertex.
         */
        virtual const NVertex* isVertexLink(NTriangulation* triang) const;
        /**
         * Determines if a rational multiple of the normal surface represented
         * is the link of a single thin edge.
         *
         * If there are two different thin edges <i>e1</i> and <i>e2</i> for
         * which the surface could be expressed as either the link of
         * <i>e1</i> or the link of <i>e2</i>, the pair
         * (<i>e1</i>,<i>e2</i>) will be returned.
         * If the surface is the link of only one thin edge <i>e</i>,
         * the pair (<i>e</i>,0) will be returned.
         * If the surface is not the link of any thin edges, the pair
         * (0,0) will be returned.
         *
         * The default implementation for this routine involves counting the
         * number of discs of every type.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return a pair containing the thin edge(s) linked by this surface,
         * as described above.
         */
        virtual std::pair<const NEdge*, const NEdge*> isThinEdgeLink(
            NTriangulation* triang) const;
        /**
         * Determines if the normal surface represented is a splitting
         * surface in the given triangulation.  A \a splitting surface
         * is a compact surface containing precisely
         * one quad per tetrahedron and no other normal (or almost
         * normal) discs.
         *
         * The default implementation for this routine simply runs
         * through and checks the count for each disc type.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if the normal surface represented
         * is a splitting surface.
         */
        virtual bool isSplitting(NTriangulation* triang) const;
        /**
         * Determines if the normal surface represented is a central
         * surface in the given triangulation.  A \a central surface
         * is a compact surface containing at most one normal or almost
         * normal disc per tetrahedron.  If the surface is central, the
         * number of tetrahedra it meets (i.e., the number of discs in
         * the surface) will be returned.
         *
         * The default implementation for this routine simply runs
         * through and checks the count for each disc type.
         * Subclasses of NNormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of tetrahedra that the surface meets if it
         * is a central surface, or 0 if it is not a central surface.
         */
        virtual NLargeInteger isCentral(NTriangulation* triang) const;

        /**
         * Returns the number of triangular discs of the given type in
         * this normal surface.
         * See NNormalSurface::getTriangleCoord() for further details.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested triangles reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param vertex the vertex of the given tetrahedron around
         * which the requested triangles lie; this should be between 0
         * and 3 inclusive.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of triangular discs of the given type.
         */
        virtual NLargeInteger getTriangleCoord(unsigned long tetIndex,
            int vertex, NTriangulation* triang) const = 0;
        /**
         * Returns the number of quadrilateral discs of the given type
         * in this normal surface.
         * See NNormalSurface::getQuadCoord() for further details.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested quadrilaterals reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param quadType the number of the vertex splitting that this
         * quad type represents; this should be between 0 and 2
         * inclusive.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of quadrilateral discs of the given type.
         */
        virtual NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType, NTriangulation* triang) const = 0;
        /**
         * Returns the number of octagonal discs of the given type
         * in this normal surface.
         * See NNormalSurface::getOctCoord() for further details.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octagons reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param octType the number of the vertex splitting that this
         * octagon type represents; this should be between 0 and 2
         * inclusive.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of octagonal discs of the given type.
         */
        virtual NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType, NTriangulation* triang) const = 0;
        /**
         * Returns the number of times this normal surface crosses the
         * given edge.
         * See NNormalSurface::getEdgeWeight() for further details.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * NTriangulation::getNumberOfEdges()-1 inclusive.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of times this normal surface crosses the
         * given edge.
         */
        virtual NLargeInteger getEdgeWeight(unsigned long edgeIndex,
            NTriangulation* triang) const = 0;
        /**
         * Returns the number of arcs in which this normal surface
         * intersects the given face in the given direction.
         * See NNormalSurface::getFaceArcs() for further details.
         *
         * @param faceIndex the index in the triangulation of the face
         * in which we are interested; this should be between 0 and
         * NTriangulation::getNumberOfFaces()-1 inclusive.
         * @param faceVertex the vertex of the face (0, 1 or 2) around
         * which the arcs of intersection that we are interested in lie;
         * only these arcs will be counted.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of times this normal surface intersect the
         * given face with the given arc type.
         */
        virtual NLargeInteger getFaceArcs(unsigned long faceIndex,
            int faceVertex, NTriangulation* triang) const = 0;

        /**
         * Returns a new normal surface vector of the appropriate length
         * for the given triangulation and for the flavour of coordinate
         * system corresponding to this subclass of NNormalSurfaceVector.
         * All elements of the new vector will be initialised to zero.
         *
         * See ::makeZeroVector() for further details.
         *
         * @param triangulation the triangulation upon which the
         * underlying coordinate system is based.
         * @return a new zero vector of the correct class and length.
         */
        #ifdef __DOXYGEN
            static NNormalSurfaceVector* makeZeroVector(
                const NTriangulation* triangulation);
        #endif
        /**
         * Creates a new set of normal surface matching equations for
         * the given triangulation using the flavour of coordinate
         * system corresponding to this particular subclass of
         * NNormalSurfaceVector.
         *
         * See ::makeMatchingEquations() for further details.
         *
         * @param triangulation the triangulation upon which these
         * matching equations will be based.
         * @return a newly allocated set of matching equations.
         */
        #ifdef __DOXYGEN
            static NMatrixInt* makeMatchingEquations(
                NTriangulation* triangulation);
        #endif
        /**
         * Creates a new set of validity constraints representing
         * the condition that normal surfaces be embedded.  The
         * validity constraints will be expressed relative to the
         * flavour of coordinate system corresponding to this particular
         * subclass of NNormalSurfaceVector.
         *
         * @param triangulation the triangulation upon which these
         * validity constraints will be based.
         * @return a newly allocated set of constraints.
         */
        #ifdef __DOXYGEN
            static NEnumConstraintList* makeEmbeddedConstraints(
                NTriangulation* triangulation);
        #endif
};

/**
 * Represents a single normal surface in a 3-manifold.
 * Once the underlying triangulation changes, this normal surface object
 * is no longer valid.
 *
 * The information provided by the various query methods is independent
 * of the underlying coordinate system being used.
 * See the NNormalSurfaceVector class notes for details of what to do
 * when introducing a new flavour of coordinate system.
 *
 * Note that non-compact surfaces (surfaces with infinitely many discs,
 * such as spun normal surfaces) are allowed; in these cases, the
 * corresponding coordinate lookup routines will return
 * NLargeInteger::infinity where appropriate.
 *
 * \testpart
 *
 * \todo \feature Calculation of Euler characteristic and orientability
 * for non-compact surfaces.
 * \todo \featurelong Determine which faces in the solution space a
 * normal surface belongs to.
 */
class NNormalSurface : public ShareableObject, public NFilePropertyReader {
    protected:
        NNormalSurfaceVector* vector;
            /**< Contains the coordinates of the normal surface in whichever
             *   space is appropriate. */
        NTriangulation* triangulation;
            /**< The triangulation in which this normal surface resides. */

        std::string name;
            /**< An optional name associated with this surface. */

        mutable NProperty<NDiscType> octPosition;
            /**< The position of the first non-zero octagonal coordinate,
                 or NDiscType::NONE if there is no non-zero octagonal
                 coordinate.  Here NDiscType::type is an octagon type
                 between 0 and 2 inclusive. */
        mutable NProperty<NLargeInteger> eulerChar;
            /**< The Euler characteristic of this surface. */
        mutable NProperty<NTriBool> orientable;
            /**< Is this surface orientable? */
        mutable NProperty<NTriBool> twoSided;
            /**< Is this surface two-sided? */
        mutable NProperty<NTriBool> connected;
            /**< Is this surface connected? */
        mutable NProperty<bool> realBoundary;
            /**< Does this surface have real boundary (i.e. does it meet
             *   any boundary faces)? */
        mutable NProperty<bool> compact;
            /**< Is this surface compact (i.e. does it only contain
             *   finitely many discs)? */

    public:
        /**
         * Creates a new normal surface inside the given triangulation
         * with the given coordinate vector.
         *
         * \pre The given coordinate vector represents a
         * normal surface inside the given triangulation.
         * \pre The given coordinate vector cannot be the null pointer.
         *
         * \ifacespython Not present.
         *
         * @param triang the triangulation in which this normal surface
         * resides.
         * @param newVector a vector containing the coordinates of the
         * normal surface in whichever space is appropriate.
         */
        NNormalSurface(NTriangulation* triang,
            NNormalSurfaceVector* newVector);
        /**
         * Destroys this normal surface.
         * The underlying vector of coordinates will also be
         * deallocated.
         */
        virtual ~NNormalSurface();

        /**
         * Creates a newly allocated clone of this normal surface.
         *
         * @return a clone of this normal surface.
         */
        NNormalSurface* clone() const;

        /**
         * Creates a newly allocated surface that is the double of this
         * surface.
         *
         * @return the double of this normal surface.
         */
        NNormalSurface* doubleSurface() const;

        /**
         * Returns the number of triangular discs of the given type in
         * this normal surface.
         * A triangular disc type is identified by specifying a
         * tetrahedron and a vertex of that tetrahedron that the
         * triangle surrounds.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested triangles reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param vertex the vertex of the given tetrahedron around
         * which the requested triangles lie; this should be between 0
         * and 3 inclusive.
         * @return the number of triangular discs of the given type.
         */
        NLargeInteger getTriangleCoord(unsigned long tetIndex,
            int vertex) const;
        /**
         * Returns the number of quadrilateral discs of the given type
         * in this normal surface.
         * A quadrilateral disc type is identified by specifying a
         * tetrahedron and a vertex splitting of that tetrahedron that
         * describes how the quadrilateral partitions the tetrahedron
         * vertices.  See \a vertexSplit for more details on vertex
         * splittings.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested quadrilaterals reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param quadType the number of the vertex splitting that this
         * quad type represents; this should be between 0 and 2
         * inclusive.
         * @return the number of quadrilateral discs of the given type.
         */
        NLargeInteger getQuadCoord(unsigned long tetIndex,
            int quadType) const;
        /**
         * Returns the number of octagonal discs of the given type
         * in this normal surface.
         * An octagonal disc type is identified by specifying a
         * tetrahedron and a vertex splitting of that tetrahedron that
         * describes how the octagon partitions the tetrahedron
         * vertices.  See \a vertexSplit for more details on vertex
         * splittings.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octagons reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param octType the number of the vertex splitting that this
         * octagon type represents; this should be between 0 and 2
         * inclusive.
         * @return the number of octagonal discs of the given type.
         */
        NLargeInteger getOctCoord(unsigned long tetIndex,
            int octType) const;
        /**
         * Returns the number of times this normal surface crosses the
         * given edge.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * NTriangulation::getNumberOfEdges()-1 inclusive.
         * @return the number of times this normal surface crosses the
         * given edge.
         */
        NLargeInteger getEdgeWeight(unsigned long edgeIndex) const;
        /**
         * Returns the number of arcs in which this normal surface
         * intersects the given face in the given direction.
         *
         * @param faceIndex the index in the triangulation of the face
         * in which we are interested; this should be between 0 and
         * NTriangulation::getNumberOfFaces()-1 inclusive.
         * @param faceVertex the vertex of the face (0, 1 or 2) around
         * which the arcs of intersection that we are interested in lie;
         * only these arcs will be counted.
         * @return the number of times this normal surface intersect the
         * given face with the given arc type.
         */
        NLargeInteger getFaceArcs(unsigned long faceIndex,
            int faceVertex) const;

        /**
         * Determines the first coordinate position at which this surface
         * has a non-zero octagonal coordinate.  In other words, if this
         * routine returns the disc type \a t, then the octagonal
         * coordinate returned by getOctCoord(t.tetIndex, t.type) is non-zero.
         * Here NDiscType::type represents an octagon type within a
         * tetrahedron, and takes values between 0 and 2 inclusive.
         *
         * If this surface does not contain any octagons, this routine
         * returns NDiscType::NONE instead.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.  Moreover, if the underlying coordinate
         * system does not support almost normal surfaces, then even the
         * first call is fast (it returns NDiscType::NONE immediately).
         *
         * @return the position of the first non-zero octagonal coordinate,
         * or NDiscType::NONE if there is no such coordinate.
         */
        NDiscType getOctPosition() const;

        /**
         * Returns the number of coordinates in the specific underlying
         * coordinate system being used.
         *
         * @return the number of coordinates.
         */
        unsigned getNumberOfCoords() const;
        /**
         * Returns the triangulation in which this normal surface
         * resides.
         *
         * @return the underlying triangulation.
         */
        NTriangulation* getTriangulation() const;

        /**
         * Returns the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * @return the name of associated with this surface.
         */
        const std::string& getName() const;
        /**
         * Sets the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * @param newName the new name to associate with this surface.
         */
        void setName(const std::string& newName);

        /**
         * The text representation will be in standard triangle-quad-oct
         * coordinates.  Octagonal coordinates will only be written if
         * the surface is of a potentially almost normal flavour.
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
         * Writes a chunk of XML containing this normal surface and all
         * of its properties.  This routine will be called from within
         * NNormalSurfaceList::writeXMLPacketData().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        virtual void writeXMLData(std::ostream& out) const;

        /**
         * Writes this normal surface and all of its properties to
         * the given old-style binary file.
         *
         * This routine writes precisely what readFromFile() reads.
         *
         * \deprecated For the preferred way to write data to file, see
         * writeXMLData() instead.
         *
         * \pre The given file is currently opened for writing.
         *
         * \ifacespython Not present.
         *
         * @param out the file to which to write.
         */
        void writeToFile(NFile& out) const;

        /**
         * Reads a normal surface and all of its properties from the
         * given old-style binary file.  The flavour of coordinate system
         * being used must be known in advance and passed to this routine.
         *
         * This routine reads precisely what writeToFile() writes.
         *
         * \deprecated For the preferred way to read surfaces from file,
         * see class NXMLNormalSurfaceReader instead.
         *
         * \pre The given file is currently opened for reading.
         *
         * \ifacespython Not present.
         *
         * @param in the file from which to read.
         * @param flavour the flavour of coordinate system that the
         * normal surface will use.
         * @param triangulation the triangulation within which
         * this normal surface will lie.
         * @return a newly allocated normal surface containing the
         * information read from file.
         */
        static NNormalSurface* readFromFile(NFile& in, int flavour,
            NTriangulation* triangulation);

        /**
         * Determines if this normal surface is empty (has no discs
         * whatsoever).
         */
        bool isEmpty() const;
        /**
         * Determines if this normal surface is compact (has
         * finitely many discs).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * @return \c true if and only if this normal surface is compact.
         */
        bool isCompact() const;
        /**
         * Returns the Euler characteristic of this surface.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return the Euler characteristic.
         */
        NLargeInteger getEulerCharacteristic() const;
        /**
         * Returns whether or not this surface is orientable.
         * 
         * This routine returns an NTriBool since it is possible that
         * the result cannot be determined (for instance, if there
         * are too many normal discs).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \todo \prob Check for absurdly large numbers of discs and bail
         * accordingly.
         *
         * @return true if this surface is orientable, false if this surface
         * is non-orientable and unknown if orientability cannot be
         * determined.
         */
        NTriBool isOrientable() const;
        /**
         * Returns whether or not this surface is two-sided.
         *
         * This routine returns an NTriBool since it is possible that
         * the result cannot be determined (for instance, if there
         * are too many normal discs).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \todo \prob Check for absurdly large numbers of discs and bail
         * accordingly.
         *
         * @return true if this surface is two-sided, false if this surface
         * is one-sided and unknown if two-sidedness cannot be determined.
         */
        NTriBool isTwoSided() const;
        /**
         * Returns whether or not this surface is connected.
         *
         * This routine returns an NTriBool since it is possible that
         * the result cannot be determined (for instance, if there
         * are too many normal discs).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \todo \prob Check for absurdly large numbers of discs and bail
         * accordingly.
         *
         * @return true if this surface is connected, false if this surface
         * is not connected and unknown if connectedness cannot be determined.
         */
        NTriBool isConnected() const;
        /**
         * Determines if this surface has any real boundary, that is,
         * whether it meets any boundary faces of the triangulation.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * @return \c true if and only if this surface has real boundary.
         */
        bool hasRealBoundary() const;

        /**
         * Determines whether or not this surface is vertex linking.
         * A <i>vertex linking</i> surface contains only triangles.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this surface is vertex linking.
         */
        bool isVertexLinking() const;
        /**
         * Determines whether or not a rational multiple of this surface
         * is the link of a single vertex.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return the vertex linked by this surface, or 0 if this
         * surface is not the link of a single vertex.
         */
        virtual const NVertex* isVertexLink() const;
        /**
         * Determines whether or not a rational multiple of this surface
         * is the link of a single thin edge.
         *
         * If there are two different thin edges <i>e1</i> and <i>e2</i> for
         * which this surface could be expressed as either the link of
         * <i>e1</i> or the link of <i>e2</i>, the pair
         * (<i>e1</i>,<i>e2</i>) will be returned.
         * If this surface is the link of only one thin edge <i>e</i>,
         * the pair (<i>e</i>,0) will be returned.
         * If this surface is not the link of any thin edges, the pair
         * (0,0) will be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * \ifacespython This routine returns a tuple of size 2.
         *
         * @return a pair containing the thin edge(s) linked by this surface,
         * as described above.
         */
        virtual std::pair<const NEdge*, const NEdge*> isThinEdgeLink() const;
        /**
         * Determines whether or not this surface is a splitting surface.
         * A \a splitting surface is a compact surface containing
         * precisely one quad per tetrahedron and no other normal (or
         * almost normal) discs.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this is a splitting surface.
         */
        bool isSplitting() const;
        /**
         * Determines whether or not this surface is a central surface.
         * A \a central surface is a compact surface containing
         * at most one normal or almost normal disc per tetrahedron.
         * If this surface is central, the number of tetrahedra that it meets
         * (i.e., the number of discs in the surface) will be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return the number of tetrahedra that this surface meets if it
         * is a central surface, or 0 if it is not a central surface.
         */
        NLargeInteger isCentral() const;

        /**
         * Cuts the associated triangulation along this surface and
         * returns a newly created resulting triangulation.
         * The original triangulation is not changed.
         *
         * Note that, unlike crushing a surface to a point, this
         * operation will not change the topology of the underlying
         * 3-manifold beyond simply slicing along this surface.
         *
         * \warning The number of tetrahedra in the new triangulation
         * can be <i>very</i> large.
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * \todo Have some error flag so we can barf politely if the
         * resulting number of tetrahedra is going to be too large to
         * handle.
         *
         * @return a pointer to the newly allocated resulting
         * triangulation.
         */
        NTriangulation* cutAlong() const;

        /**
         * Crushes this surface to a point in the associated
         * triangulation and returns a newly created resulting
         * triangulation.  The original triangulation is not changed.
         *
         * Crushing the surface will produce a number of tetrahedra,
         * triangular pillows and/or footballs.  The pillows and
         * footballs will then be flattened to faces and edges
         * respectively (resulting in the possible
         * changes mentioned below) to produce a proper triangulation.
         *
         * Note that the new triangulation will have at most the same
         * number of tetrahedra as the old triangulation, and will have
         * strictly fewer tetrahedra if this surface is not vertex
         * linking.
         *
         * The act of flattening pillows and footballs as described
         * above can lead to unintended topological side-effects, beyond
         * the effects of merely cutting along this surface and
         * identifying the new boundary surface(s) to points.
         * Examples of these unintended side-effects can include
         * connected sum decompositions, removal of 3-spheres and
         * small Lens spaces and so on; a full list of possible changes
         * is beyond the scope of this API documentation.
         *
         * \warning This routine can have unintended topological
         * side-effects, as described above.
         * \warning In exceptional cases with non-orientable
         * 3-manifolds, these side-effects might lead to invalid edges
         * (edges whose midpoints are projective plane cusps).
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * @return a pointer to the newly allocated resulting
         * triangulation.
         */
        NTriangulation* crush() const;

        /**
         * Determines whether this and the given surface in fact
         * represent the same normal (or almost normal) surface.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal or almost normal discs of each type, and returns \c true
         * if and only if these counts are the same for both surfaces.
         *
         * It does not matter what coordinate systems the two surfaces
         * use.  In particular, it does not matter if this and the
         * given surface use different coordinate systems, and it
         * does not matter if one surface uses an almost normal
         * coordinate system and the other does not.
         *
         * \pre Both this and the given normal surface live within the
         * same 3-manifold triangulation.
         *
         * @param other the surface to be compared with this surface.
         * @return \c true if both surfaces represent the same normal or
         * almost normal surface, or \c false if not.
         */
        bool sameSurface(const NNormalSurface& other) const;

        /**
         * Determines whether this and the given surface are locally compatible.
         * Local compatibility means that, within each individual tetrahedron
         * of the triangulation, it is possible to arrange the normal
         * discs of both surfaces so that none intersect.
         *
         * This is a local constraint, not a global constraint.  That is,
         * we do not insist that we can avoid intersections within all
         * tetrahedra \e simultaneously.  To test the global constraint,
         * see the (much slower) routine disjoint() instead.
         *
         * Local compatibility can be formulated in terms of normal disc types.
         * Two normal (or almost normal) surfaces are locally compatible if
         * and only if they together have at most one quadrilateral or
         * octagonal disc type per tetrahedron.
         *
         * Note again that this is a local constraint only.  In particular,
         * for almost normal surfaces, it does \e not insist that there is
         * at most one octagonal disc type anywhere within the triangulation.
         *
         * If one of the two surfaces breaks the local compatibility
         * constraints on its own (for instance, it contains two different
         * quadrilateral disc types within the same tetrahedron), then this
         * routine will return \c false regardless of what the other surface
         * contains.
         *
         * \pre Both this and the given normal surface live within the
         * same 3-manifold triangulation.
         *
         * @param other the other surface to test for local compatibility with
         * this surface.
         * @return \c true if the two surfaces are locally compatible, or
         * \c false if they are not.
         */
        bool locallyCompatible(const NNormalSurface& other) const;

        /**
         * Determines whether this and the given surface can be placed
         * within the surrounding triangulation so that they do not intersect
         * anywhere at all.
         *
         * This is a global constraint, and therefore gives a stronger test
         * than locallyCompatible().  However, this global constraint is
         * also much slower to test; the running time is proportional to
         * the total number of normal discs in both surfaces.
         *
         * Note that this routine has a number of preconditions.  Most
         * importantly, it will only work if both this and the given
         * surface use the \e same flavour of coordinate system.
         * Running this test over two surfaces with different coordinate
         * systems could give unpredictable results, and might
         * crash the program entirely.
         *
         * \pre Both this and the given normal surface live within the
         * same 3-manifold triangulation.
         * \pre Both this and the given normal surface are stored using
         * the same flavour of coordinate system (i.e., the same
         * subclass of NNormalSurfaceVector).
         * \pre Both this and the given surface are compact (have
         * finitely many discs), embedded, non-empty and connected.
         *
         * @param other the other surface to test alongside this surface
         * for potential intersections.
         * @return \c true if both surfaces can be embedded without
         * intersecting anywhere, or \c false if this and the given
         * surface are forced to intersect at some point.
         */
        bool disjoint(const NNormalSurface& other) const;

        /**
         * Gives read-only access to the raw vector that sits beneath this
         * normal surface.
         *
         * Generally users should not need this function.  However, it is
         * provided here in case the need should arise (e.g., for reasons
         * of efficiency).
         *
         * \warning An NNormalSurface does not know what underlying
         * coordinate system its raw vector uses.  Unless you already know
         * the coordinate system in advance (i.e., you created the surface
         * yourself), it is best to keep to the coordinate-system-agnostic
         * access functions such as NNormalSurfaceVector::getTriCoord() and
         * NNormalSurfaceVector::getQuadCoord().
         *
         * \ifacespython Not present.
         *
         * @return the underlying raw vector.
         */
        const NNormalSurfaceVector* rawVector() const;

        /**
         * Searches for a non-vertex-linking normal 2-sphere within the
         * given triangulation.  If a non-vertex linking normal 2-sphere
         * exists anywhere at all within the triangulation, then this routine
         * is guaranteed to find one.
         *
         * Note that the surface returned (if any) depends upon the
         * triangulation, and so must be destroyed before the triangulation
         * itself.
         *
         * \warning Currently this routine is quite slow since it
         * involves a full enumeration of vertex normal surfaces.
         *
         * \todo \opturgent Use maximisation of Euler characteristic to
         * make this routine much faster than a plain vertex enumeration.
         *
         * @param tri the triangulation in which to search.
         * @return a newly allocated non-vertex-linking normal sphere
         * within the given triangulation, or 0 if no such sphere exists.
         */
        static NNormalSurface* findNonTrivialSphere(NTriangulation* tri);

        /**
         * Searches the list of vertex octagonal almost normal surfaces for
         * an almost normal 2-sphere within the given triangulation.  This
         * means that tubed almost normal 2-spheres or non-vertex octagonal
         * almost normal 2-spheres will not be found.
         *
         * This search can be done either in standard almost normal
         * coordinates (with triangles, quadrilaterals and octagons), or
         * in quadrilateral-octagon coordinates.  This choice of coordinate
         * system affects how we define "vertex".  The default is to use
         * standard coordinates (where the set of vertex surfaces is larger).
         *
         * For "sufficiently nice" triangulations, if this routine fails
         * to find an almost normal 2-sphere then we can be certain that
         * the triangulation contains no almost normal 2-spheres at all.
         * In particular, this is true for closed orientable one-vertex
         * 0-efficient triangulations.  For a proof in standard coordinates,
         * see "0-efficient triangulations of 3-manifolds", William Jaco
         * and J. Hyam Rubinstein, J. Differential Geom. 65 (2003),
         * no. 1, 61--168.  For a proof in quadrilateral-octagon coordinates,
         * see "Quadrilateral-octagon coordinates for almost normal surfaces",
         * Benjamin A. Burton, preprint, arXiv:0904.3041.
         *
         * Note that the surface that this routine returns (if any) depends
         * upon the triangulation, and so this surface must be destroyed
         * before the triangulation is destroyed.
         *
         * \warning Currently this routine can be quite slow since it
         * performs a full enumeration of vertex almost normal surfaces.
         *
         * \todo \opt Use maximisation of Euler characteristic to
         * make this routine much faster than a plain vertex enumeration.
         *
         * @param tri the triangulation in which to search.
         * @param quadOct \c true if we should search for vertex
         * surfaces in quadrilateral-octagon coordiantes, or \c false
         * (the default) if we should search for surfaces in standard
         * almost normal coordinates.
         * @return a newly allocated vertex octagonal almost normal sphere
         * within the given triangulation, or 0 if no such sphere exists.
         */
        static NNormalSurface* findVtxOctAlmostNormalSphere(
            NTriangulation* tri, bool quadOct = false);

    protected:
        virtual void readIndividualProperty(NFile& infile,
            unsigned propType);

        /**
         * Calculates the position of the first non-zero octagon
         * coordinate and stores it as a property.
         */
        void calculateOctPosition() const;

        /**
         * Calculates the Euler characteristic of this surface and
         * stores it as a property.
         *
         * \pre This normal surface is compact (has finitely many discs).
         */
        void calculateEulerCharacteristic() const;
        /**
         * Calculates whether this surface is orientable and/or
         * two-sided and stores the results as properties.
         *
         * \pre This normal surface is compact (has finitely many discs).
         */
        void calculateOrientable() const;
        /**
         * Calculates whether this surface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary() const;

    friend class regina::NXMLNormalSurfaceReader;
};

/*@}*/

// Inline functions for NNormalSurfaceVector

inline NNormalSurfaceVector::NNormalSurfaceVector(unsigned length) :
        NRay(length) {
}
inline NNormalSurfaceVector::NNormalSurfaceVector(
        const NVector<NLargeInteger>& cloneMe) : NRay(cloneMe) {
}

// Inline functions for NNormalSurface

inline NNormalSurface::~NNormalSurface() {
    delete vector;
}

inline NLargeInteger NNormalSurface::getTriangleCoord(unsigned long tetIndex,
        int vertex) const {
    return vector->getTriangleCoord(tetIndex, vertex, triangulation);
}
inline NLargeInteger NNormalSurface::getQuadCoord(unsigned long tetIndex,
        int quadType) const {
    return vector->getQuadCoord(tetIndex, quadType, triangulation);
}
inline NLargeInteger NNormalSurface::getOctCoord(unsigned long tetIndex,
        int octType) const {
    return vector->getOctCoord(tetIndex, octType, triangulation);
}
inline NLargeInteger NNormalSurface::getEdgeWeight(unsigned long edgeIndex)
        const {
    return vector->getEdgeWeight(edgeIndex, triangulation);
}
inline NLargeInteger NNormalSurface::getFaceArcs(unsigned long faceIndex,
        int faceVertex) const {
    return vector->getFaceArcs(faceIndex, faceVertex, triangulation);
}

inline NDiscType NNormalSurface::getOctPosition() const {
    if (! octPosition.known())
        calculateOctPosition();
    return octPosition.value();
}

inline unsigned NNormalSurface::getNumberOfCoords() const {
    return vector->size();
}
inline NTriangulation* NNormalSurface::getTriangulation() const {
    return triangulation;
}

inline const std::string& NNormalSurface::getName() const {
    return name;
}
inline void NNormalSurface::setName(const std::string& newName) {
    name = newName;
}

inline void NNormalSurface::writeRawVector(std::ostream& out) const {
    out << *vector;
}

inline bool NNormalSurface::isCompact() const {
    if (! compact.known())
        compact = vector->isCompact(triangulation);
    return compact.value();
}

inline NLargeInteger NNormalSurface::getEulerCharacteristic() const {
    if (! eulerChar.known())
        calculateEulerCharacteristic();
    return eulerChar.value();
}

inline NTriBool NNormalSurface::isOrientable() const {
    if (! orientable.known())
        calculateOrientable();
    return orientable.value();
}

inline NTriBool NNormalSurface::isTwoSided() const {
    if (! twoSided.known())
        calculateOrientable();
    return twoSided.value();
}

inline NTriBool NNormalSurface::isConnected() const {
    if (! connected.known())
        calculateOrientable();
    return connected.value();
}

inline bool NNormalSurface::hasRealBoundary() const {
    if (! realBoundary.known())
        calculateRealBoundary();
    return realBoundary.value();
}

inline bool NNormalSurface::isVertexLinking() const {
    return vector->isVertexLinking(triangulation);
}

inline const NVertex* NNormalSurface::isVertexLink() const {
    return vector->isVertexLink(triangulation);
}

inline std::pair<const NEdge*, const NEdge*> NNormalSurface::isThinEdgeLink()
        const {
    return vector->isThinEdgeLink(triangulation);
}

inline bool NNormalSurface::isSplitting() const {
    return vector->isSplitting(triangulation);
}

inline NLargeInteger NNormalSurface::isCentral() const {
    return vector->isCentral(triangulation);
}

inline const NNormalSurfaceVector* NNormalSurface::rawVector() const {
    return vector;
}

} // namespace regina

#endif

