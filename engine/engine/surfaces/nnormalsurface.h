
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file nnormalsurface.h
 *  \brief Deals with an individual normal surface.
 */

#ifndef __NNORMALSURFACE_H
#ifndef __DOXYGEN
#define __NNORMALSURFACE_H
#endif

#include <utility>
#include "shareableobject.h"
#include "maths/nray.h"
#include "property/npropertyholder.h"
#include "triangulation/nperm.h"

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
extern const NPerm triDiscArcs[4][3];
#else
extern const NPerm __triDiscArcs[12];
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
extern const NPerm quadDiscArcs[3][4];
#else
extern const NPerm __quadDiscArcs[12];
#define quadDiscArcs(i, j) __quadDiscArcs[(4 * (i)) + (j)]
#endif

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of octahedral normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>octDiscArcs[i]</tt> lists the boundary arcs of the
 * octahedral disc of type <i>i</i>.  See NNormalSurface::getOctCoord()
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
extern const NPerm octDiscArcs[3][8];
#else
extern const NPerm __octDiscArcs[24];
#define octDiscArcs(i, j) __octDiscArcs[(8 * (i)) + (j)]
#endif

class NTriangulation;
class NEdge;
class NVertex;
class NXMLNormalSurfaceReader;
class NCompConstraintSet;

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
 *   createNonNegativeCone(NTriangulation*, int, RayOutputIterator,
 *   FaceOutputIterator)</tt> and
 *   <tt>NMatrixInt* makeMatchingEquations(NTriangulation*)</tt> must be
 *   declared and implemented.  See ::createNonNegativeCone() and
 *   ::makeMatchingEquations() for further details.</li>
 * </ul>
 *
 * \todo \feature Implement quad-oct space.
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
         * octahedral discs.
         *
         * Note that this has nothing to do with whether or not this
         * specific surface contains octahedral discs.
         *
         * @return \c true if and only if almost normal surfaces are allowed.
         */
        virtual bool allowsAlmostNormal() const = 0;

        /**
         * Determines if this normal surface has more than one
         * octahedral disc.  It may be assumed that at most one
         * octahedral type exists in this surface.  This routine will
         * return \c true if an octahedral type does exist and its
         * coordinate is greater than one.
         *
         * The default implementation for this routine simply calculates
         * all the octahedral coordinates and returns as soon as a
         * positive or negative result can be established.  Subclasses
         * of NNormalSurfaceVector should override this if they can provide a
         * faster implementation.
         *
         * If a subclass does not allow for almost normal surfaces, this
         * routine will never be called and thus does not need to be
         * overwritten.
         *
         * \pre At most one octahedral type exists in this surface.
         * \pre This normal surface vector is using a
         * flavour of coordinate system that allows for almost normal
         * surfaces.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if there is an octahedral type
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
         * \pre The given triangulation is connected.  If the
         * triangulation is not connected, this routine will still
         * return a result but that result will be unreliable.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return \c true if and only if the normal surface represented
         * is compact.
         */
        virtual bool isSplitting(NTriangulation* triang) const;

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
         * Returns the number of octahedral discs of the given type
         * in this normal surface.
         * See NNormalSurface::getOctCoord() for further details.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octahedrons reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param octType the number of the vertex splitting that this
         * octahedron type represents; this should be between 0 and 2
         * inclusive.
         * @param triang the triangulation in which this normal surface lives.
         * @return the number of octahedral discs of the given type.
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
         * Writes to the given output iterators newly allocated rays and
         * faces representing the cone obtained by setting all coordinates
         * non-negative in the flavour of coordinate system
         * corresponding to this particular subclass of
         * NNormalSurfaceVector.
         *
         * The elements written to \a rays \b must be of this
         * particular subclass of NNormalSurfaceVector.
         *
         * See ::createNonNegativeCone() for further details.
         *
         * @param triangulation the triangulation upon which the
         * underlying coordinate system is based.
         * @param rays the output iterator to which the newly allocated
         * extremal rays will be written; these rays must all be of this
         * particular subclass of NNormalSurfaceVector.  This iterator
         * must accept objects of type <tt>NRay*</tt>.
         * @param faces the output iterator to which the newly allocated face
         * perpendiculars will be written; these vectors may be of any
         * subclass of NVector<NLargeInteger>.  This iterator must
         * accept objects of type <tt>NVector\<NLargeInteger\>*</tt>.
         */
        #ifdef __DOXYGEN
            template <class RayOutputIterator, class FaceOutputIterator>
            static void createNonNegativeCone(NTriangulation* triangulation,
                RayOutputIterator rays, FaceOutputIterator faces);
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
         * Creates a new set of compatibility constraints representing
         * the condition that normal surfaces be embedded.  The
         * compatibility constraints will be expressed relative to the
         * flavour of coordinate system corresponding to this particular
         * subclass of NNormalSurfaceVector.
         *
         * @param triangulation the triangulation upon which these
         * compatibility constraints will be based.
         * @return a newly allocated set of constraints.
         */
        #ifdef __DOXYGEN
            static NCompConstraintSet* makeEmbeddedConstraints(
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
 * \todo \feature Calculation of Euler characteristic and orientability
 * for non-compact surfaces.
 * \todo \featurelong Determine which faces in the solution space a
 * normal surface belongs to.
 */
class NNormalSurface : public ShareableObject, public NPropertyHolder {
    protected:
        NNormalSurfaceVector* vector;
            /**< Contains the coordinates of the normal surface in whichever
             *   space is appropriate. */
        NTriangulation* triangulation;
            /**< The triangulation in which this normal surface resides. */

        std::string name;
            /**< An optional name associated with this surface. */

        mutable NLargeInteger eulerChar;
            /**< The Euler characteristic of this surface. */
        mutable bool calculatedEulerChar;
            /**< Have we calculated the Euler characteristic? */
        mutable int orientable;
            /**< Is this surface orientable?
                 1 is true, -1 is false and 0 is undetermined. */
        mutable bool calculatedOrientable;
            /**< Have we calculated the orientability of this surface
                 (or the indeterminibility thereof)? */
        mutable int twoSided;
            /**< Is this surface two-sided?
                 1 is true, -1 is false and 0 is undetermined. */
        mutable bool calculatedTwoSided;
            /**< Have we calculated the two-sidedness of this surface
                 (or the indeterminibility thereof)? */
        mutable int connected;
            /**< Is this surface connected? */
        mutable bool calculatedConnected;
            /**< Have we calculated the connectedness of this surface
                 (or the indeterminibility thereof)? */
        mutable bool realBoundary;
            /**< Does this surface have real boundary (i.e. does it meet
             *   any boundary faces)? */
        mutable bool calculatedRealBoundary;
            /**< Have we calculated whether this surface has real
             *   boundary? */
        mutable bool compact;
            /**< Is this surface compact (i.e. does it only contain
             *   finitely many discs)? */
        mutable bool calculatedCompact;
            /**< Have we calculated whether this surface is compact? */
        mutable bool canCrush;
            /**< Can this surface be crushed without unintended
                 topological side-effects? */
        mutable bool calculatedCanCrush;
            /**< Have we calculated whether this surface can be safely
                 crushed? */

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
         * Returns the number of octahedral discs of the given type
         * in this normal surface.
         * An octahedral disc type is identified by specifying a
         * tetrahedron and a vertex splitting of that tetrahedron that
         * describes how the octahedron partitions the tetrahedron
         * vertices.  See \a vertexSplit for more details on vertex
         * splittings.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octahedrons reside;
         * this should be between 0 and
         * NTriangulation::getNumberOfTetrahedra()-1 inclusive.
         * @param octType the number of the vertex splitting that this
         * octahedron type represents; this should be between 0 and 2
         * inclusive.
         * @return the number of octahedral discs of the given type.
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
         * coordinates.  Octahedral coordinates will only be written if
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
         * Determines if this normal surface is compact (has
         * finitely many discs).
         *
         * @return \c true if and only if this normal surface is compact.
         */
        bool isCompact() const;
        /**
         * Returns the Euler characteristic of this surface.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return the Euler characteristic.
         */
        NLargeInteger getEulerCharacteristic() const;
        /**
         * Returns whether or not this surface is orientable.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is orientable, -1 if this surface
         * is non-orientable and 0 if orientability cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isOrientable() const;
        /**
         * Returns whether or not this surface is two-sided.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is two-sided, -1 if this surface
         * is one-sided and 0 if two-sidedness cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isTwoSided() const;
        /**
         * Returns whether or not this surface is connected.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is connected, -1 if this surface
         * is not connected and 0 if connectedness cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isConnected() const;
        /**
         * Determines if this surface has any real boundary, that is,
         * whether it meets any boundary faces of the triangulation.
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
         * \pre The underlying triangulation is connected.  If the
         * triangulation is not connected, this routine will still
         * return a result but that result will be unreliable.
         *
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this is a splitting surface.
         */
        bool isSplitting() const;

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
         * \warning <b>This routine has not been implemented and so
         * currently returns an empty triangulation.</b>
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octahedral discs.
         *
         * \todo \feature Implement this routine.
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
         * Routine knownCanCrush() can be used to help identify whether
         * these unintended side-effects might occur.
         *
         * \warning This routine can have unintended topological
         * side-effects, as described above.
         * \warning In exceptional cases with non-orientable
         * 3-manifolds, these side-effects might lead to invalid edges
         * (edges whose midpoints are projective plane cusps).
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octahedral discs.
         *
         * @return a pointer to the newly allocated resulting
         * triangulation.
         */
        NTriangulation* crush() const;

        /**
         * Determines whether this surface can be crushed to a point in
         * the associated triangulation with no unintended topological
         * side-effects.
         *
         * Note that this routine cannot determine that there \e will be
         * unintended side-effects; it will either determine that there
         * \e won't be unintended side-effects or it will remain
         * inconclusive.
         *
         * Unintended side-effects include any topological change other
         * than the pure topological effects of cutting along this surface
         * and then identifying the new boundary surface(s) to points.
         *
         * These unintended side-effects can occur when the algorithm
         * used by the crush() routine collapses pillows and footballs
         * to obtain a proper triangulation.  Some examples of the
         * side-effects that can occur are given in the documentation
         * for the crush() routine.
         *
         * \warning Currently this routine always returns \c false,
         * i.e., an inconclusive result.  Its abilities are expected
         * to improve with future releases.
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octahedral discs.
         *
         * \todo \feature Implement this routine!  At least
         * for embedded 2-spheres.
         *
         * @return \c true if this routine determines that this surface
         * can be crushed without unintended side-effects, or \c false if
         * this routine cannot produce a definite answer.
         */
        bool knownCanCrush() const;

    protected:
        virtual void readIndividualProperty(NFile& infile,
            unsigned propType);
        virtual void initialiseAllProperties();

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
        /**
         * Calculates whether it can be quickly determined that this
         * surface can be crushed to a point without unintended
         * topological side-effects.  If conclusive, the result is
         * stored as a property.
         */
        void calculateKnownCanCrush() const;

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

inline NNormalSurface::NNormalSurface(NTriangulation* triang,
        NNormalSurfaceVector* newVector) : vector(newVector),
        triangulation(triang) {
    NNormalSurface::initialiseAllProperties();
}
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
    if (! calculatedCompact) {
        compact = vector->isCompact(triangulation);
        calculatedCompact = true;
    }
    return compact;
}

inline NLargeInteger NNormalSurface::getEulerCharacteristic() const {
    if (! calculatedEulerChar)
        calculateEulerCharacteristic();
    return eulerChar;
}

inline int NNormalSurface::isOrientable() const {
    if (! calculatedOrientable)
        calculateOrientable();
    return orientable;
}

inline int NNormalSurface::isTwoSided() const {
    if (! calculatedTwoSided)
        calculateOrientable();
    return twoSided;
}

inline int NNormalSurface::isConnected() const {
    if (! calculatedConnected)
        calculateOrientable();
    return connected;
}

inline bool NNormalSurface::hasRealBoundary() const {
    if (! calculatedRealBoundary)
        calculateRealBoundary();
    return realBoundary;
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

inline bool NNormalSurface::knownCanCrush() const {
    if (! calculatedCanCrush)
        calculateKnownCanCrush();
    return (calculatedCanCrush && canCrush);
}

} // namespace regina

#endif

