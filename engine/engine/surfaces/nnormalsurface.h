
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

/*! \file nnormalsurface.h
 *  \brief Deals with an individual normal surface.
 */

#ifndef __NNORMALSURFACE_H
#ifndef __DOXYGEN
#define __NNORMALSURFACE_H
#endif

#include "shareableobject.h"
#include "utilities/nmpi.h"
#include "maths/nvectordense.h"
#include "property/npropertyholder.h"
#include "triangulation/nperm.h"

/**
 * Lists which vertex splits split which pairs of vertices.
 * There are three vertex splits, numbered 0,1,2.  Each vertex
 * split split the four tetrahedron vertices 0,1,2,3 into two pairs.
 * <tt>vertexSplit[i][j]</tt> is the number of the vertex split that
 * keeps vertices <tt>i</tt> and <tt>j</tt> together.
 *
 * \ifacesjava This array is a static member of NNormalSurface.
 */
extern const int vertexSplit[4][4];
/**
 * Lists which vertex splits meet which edges.
 * See \a vertexSplit for details on what a vertex split is.
 * <tt>vertexSplitMeeting[i][j][0,1]</tt> are the numbers of the two
 * vertex splits that meet the edge joining tetrahedron vertices
 * <tt>i</tt> and <tt>j</tt>.
 *
 * \ifacesjava This array is a static member of NNormalSurface.
 */
extern const int vertexSplitMeeting[4][4][2];

/**
 * Lists the vertices which each vertex split splits.
 * See \a vertexSplit for details on what a vertex split is.
 * Vertex split number \c i splits the vertex pairs
 * <tt>vertexSplitDefn[i][0,1]</tt> and
 * <tt>vertexSplitDefn[i][2,3]</tt>.
 *
 * \ifacesjava This array is a static member of NNormalSurface.
 */
extern const int vertexSplitDefn[3][4];

/**
 * Lists the second vertex with which each vertex is paired under each
 * vertex split.
 * See \a vertexSplit for details on what a vertex split is.
 * Vertex split number \c i pairs vertex \c v with
 * vertex <tt>vertexSplitPartner[i][v]</tt>.
 *
 * \ifacesjava This array is a static member of NNormalSurface.
 */
extern const int vertexSplitPartner[3][4];

/**
 * Contains strings describing which vertices each vertex split splits.
 * See \a vertexSplit for details on what a vertex split is.
 * The string describing vertex split number \c i is
 * <tt>vertexSplitString[i]</tt> and is of the form <tt>02/13</tt>,
 * which in this case is the vertex split that splits vertices 0,2 from
 * vertices 1,3.
 *
 * \ifacesjava This array is a static member of NNormalSurface and
 * is a one dimensional array of strings.
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
 * \ifacesjava This array is a static member of NNormalSurface.
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
 * \ifacesjava This array is a static member of NNormalSurface.
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
 * \ifacesjava This array is a static member of NNormalSurface.
 */
#ifdef __DOXYGEN
extern const NPerm octDiscArcs[3][8];
#else
extern const NPerm __octDiscArcs[24];
#define octDiscArcs(i, j) __octDiscArcs[(8 * (i)) + (j)]
#endif

class NTriangulation;

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
 *   <li>Follow the instructions on the \ref HowTo page for
 *   adding a new normal surface coordinate system.</li>
 *   <li>Constructors <tt>class(unsigned length)</tt> and
 *   <tt>class(const NVector<NLargeInteger>& cloneMe)</tt> should be
 *   declared and implemented; these will usually just call the
 *   corresponding superclass constructors.</li>
 *   <li>Virtual functions <tt>NVector<NLargeInteger>* clone() const</tt> and
 *   <tt>bool allowsAlmostNormal() const</tt> should be declared but not
 *   implemented.  The registry utilities will take care of their
 *   implementations.</li>
 *   <li>All abstract functions must be implemented.</li>
 *   <li>Static public functions <tt>NDoubleList<NNormalSurfaceVector*>*
 *   createNonNegativeCone(NTriangulation*)</tt> and
 *   <tt>NMatrixInt* makeMatchingEquations(NTriangulation*)</tt> must be
 *   declared and implemented.  See ::createNonNegativeCone() and
 *   ::makeMatchingEquations() for further details.</li>
 * </ul>
 *
 * \todo \feature Implement quad-oct space.
 * \todo \feature Implement viewing by boundary face arcs.
 * \todo \optlong Investigate using sparse vectors for storage.
 *
 * \ifaces Not present.
 */
class NNormalSurfaceVector : public NVectorDense<NLargeInteger> {
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
         * Determines if this and the given embedded normal surface can
         * be summed to give another embedded normal surface.
         *
         * \pre Both this and the given vector
         * represent \e embedded normal surfaces.
         * \pre Both this and the given vector use the
         * same underlying coordinate system, that is, are of the same
         * subclass of NNormalSurfaceVector.
         * 
         * @param other the vector whose compatibility with this is being
         * examined.
         * @return \c true if and only if this and the given vector can
         * be added to give an embedded normal surface.
         */
        virtual bool isCompatibleWith(const NNormalSurfaceVector& other)
            const = 0;
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
         * Scales this vector down by the greatest common divisor of all
         * its elements.  The resulting vector will be the smallest
         * multiple of the original that maintains integeral entries,
         * and these entries will have the same signs as the originals.
         *
         * This routine poses no problem for vectors containing infinite
         * elements; such elements are simply ignored and left at
         * infinity.
         */
        void scaleDown();

        /**
         * Returns a newly allocated list of newly allocated rays
         * representing the cone obtained by setting all coordinates
         * non-negative in the flavour of coordinate system
         * corresponding to this particular subclass of
         * NNormalSurfaceVector.
         *
         * See ::createNonNegativeCone() for further details.
         *
         * \ifaces Not present.
         *
         * @param triangulation the triangulation upon which the
         * underlying coordinate system is based.
         * @return a newly allocated list of newly allocated arrays
         * representing the extremal rays of the non-negative cone.
         */
        #ifdef __DOXYGEN
            static NDoubleList<NNormalSurfaceVector*>* createNonNegativeCone(
                NTriangulation* triangulation);
        #endif
        /**
         * Creates a new set of normal surface matching equations for
         * the given triangulation using the flavour of coordinate
         * system corresponding to this particular subclass of
         * NNormalSurfaceVector.
         *
         * See ::makeMatchingEquations() for further details.
         *
         * \ifaces Not present.
         *
         * @param triangulation the triangulation upon which these
         * matching equations will be based.
         * @return a newly allocated set of matching equations.
         */
         #ifdef __DOXYGEN
            static NMatrixInt* makeMatchingEquations(
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
 * \todo \feature Collapse a normal surface.
 * \todo \feature Calculation of Euler characteristic and orientability
 * for non-compact surfaces.
 * \todo \featurelong Determine which faces in the solution space a
 * normal surface belongs to.
 * \todo \featurelong Allow individual surfaces to be named.
 */
class NNormalSurface : public ShareableObject, public NPropertyHolder {
    protected:
        NNormalSurfaceVector* vector;
            /**< Contains the coordinates of the normal surface in whichever
             *   space is appropriate. */
        NTriangulation* triangulation;
            /**< The triangulation in which this normal surface resides. */

        NLargeInteger eulerChar;
            /**< The Euler characteristic of this surface. */
        bool calculatedEulerChar;
            /**< Have we calculated the Euler characteristic? */
        int orientable;
            /**< Is this surface orientable?
                 1 is true, -1 is false and 0 is undetermined. */
        bool calculatedOrientable;
            /**< Have we calculated the orientability of this surface
                 (or the indeterminibility thereof)? */
        int twoSided;
            /**< Is this surface two-sided?
                 1 is true, -1 is false and 0 is undetermined. */
        bool calculatedTwoSided;
            /**< Have we calculated the two-sidedness of this surface
                 (or the indeterminibility thereof)? */
        int connected;
            /**< Is this surface connected? */
        bool calculatedConnected;
            /**< Have we calculated the connectedness of this surface
                 (or the indeterminibility thereof)? */
        bool realBoundary;
            /**< Does this surface have real boundary (i.e. does it meet
             *   any boundary faces)? */
        bool calculatedRealBoundary;
            /**< Have we calculated whether this surface has real
             *   boundary? */
        bool compact;
            /**< Is this surface compact (i.e. does it only contain
             *   finitely many discs)? */
        bool calculatedCompact;
            /**< Have we calculated whether this surface is compact? */

    public:
        /**
         * Creates a new normal surface inside the given triangulation
         * with the given coordinate vector.
         *
         * \pre The given coordinate vector represents a
         * normal surface inside the given triangulation.
         * \pre The given coordinate vector cannot be the null pointer.
         *
         * \ifaces Not present.
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
         * \ifaces Not present.
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
         */
        NTriangulation* getTriangulation() const;

        /**
         * The text representation will be in standard triangle-quad-oct
         * coordinates.  Octahedral coordinates will only be written if
         * the surface is of a potentially almost normal flavour.
         */
        void writeTextShort(ostream& out) const;
        /**
         * Writes the underlying coordinate vector to the given output
         * stream in text format.
         * No indication will be given as to which coordinate
         * system is being used or what each coordinate means.
         * No newline will be written.
         * 
         * \ifaces The paramater \a out does not exist, and is
         * taken to be standard output.  If the engine is being accessed
         * through CORBA, this routine does nothing.
         * \ifacescorba Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeRawVector(ostream& out) const;

        /**
         * Writes this normal surface and all of its properties to
         * the given file.
         *
         * This routine writes precisely what readFromFile() reads.
         *
         * \pre The given file is currently opened for writing.
         *
         * \ifaces Not present.
         *
         * @param out the file to which to write.
         */
        void writeToFile(NFile& out) const;

        /**
         * Reads a normal surface and all of its properties from the
         * given file.  The flavour of coordinate system being used must
         * be known in advance and passed to this routine.
         *
         * This routine reads precisely what writeToFile() writes.
         *
         * \pre The given file is currently opened for reading.
         *
         * \ifaces Not present.
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
        bool isCompact();
        /**
         * Returns the Euler characteristic of this surface.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return the Euler characteristic.
         */
        NLargeInteger getEulerCharacteristic();
        /**
         * Returns whether or not this surface is orientable.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is orientable, -1 if this surface
         * is non-orientable and 0 if orientability cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isOrientable();
        /**
         * Returns whether or not this surface is two-sided.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is two-sided, -1 if this surface
         * is one-sided and 0 if two-sidedness cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isTwoSided();
        /**
         * Returns whether or not this surface is connected.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return 1 if this surface is connected, -1 if this surface
         * is not connected and 0 if connectedness cannot be determined
         * (for instance, if there are too many normal discs).
         */
        int isConnected();
        /**
         * Determines if this surface has any real boundary, that is,
         * whether it meets any boundary faces of the triangulation.
         *
         * @return \c true if and only if this surface has real boundary.
         */
        bool hasRealBoundary();

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
         * \warning The number of tetrahedra in the new triangulation
         * can be very large ((6<i>D</i>+5<i>T</i> as an upper bound,
         * where <i>D</i> is the number of normal discs in the surface).
         * \warning This routine currently <b>does nothing</b>; the code
         * is still being written.
         *
         * \pre This normal surface is compact.
         * \pre This normal surface contains no octahedral discs.
         *
         * \todo \feature Implement this routine.
         *
         * \ifaces Not present and will not be present until this
         * routine is implemented.
         *
         * @return a pointer to the newly allocated resulting
         * triangulation.
         * @author David Letscher
         */
        NTriangulation* cutAlong();

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
        void calculateEulerCharacteristic();
        /**
         * Calculates whether this surface is orientable and/or
         * two-sided and stores the results as properties.
         *
         * \pre This normal surface is compact (has finitely many discs).
         */
        void calculateOrientable();
        /**
         * Calculates whether this surface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary();
};

// Inline functions for NNormalSurfaceVector

inline NNormalSurfaceVector::NNormalSurfaceVector(unsigned length) :
        NVectorDense<NLargeInteger>(length, zero) {
}
inline NNormalSurfaceVector::NNormalSurfaceVector(
        const NVector<NLargeInteger>& cloneMe) :
        NVectorDense<NLargeInteger>(cloneMe) {
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

inline void NNormalSurface::writeRawVector(ostream& out) const {
    out << *vector;
}

inline bool NNormalSurface::isCompact() {
    if (! calculatedCompact) {
        compact = vector->isCompact(triangulation);
        calculatedCompact = true;
    }
    return compact;
}

inline NLargeInteger NNormalSurface::getEulerCharacteristic() {
    if (! calculatedEulerChar)
        calculateEulerCharacteristic();
    return eulerChar;
}

inline int NNormalSurface::isOrientable() {
    if (! calculatedOrientable)
        calculateOrientable();
    return orientable;
}

inline int NNormalSurface::isTwoSided() {
    if (! calculatedTwoSided)
        calculateOrientable();
    return twoSided;
}

inline int NNormalSurface::isConnected() {
    if (! calculatedConnected)
        calculateOrientable();
    return connected;
}

inline bool NNormalSurface::hasRealBoundary() {
    if (! calculatedRealBoundary)
        calculateRealBoundary();
    return realBoundary;
}

inline bool NNormalSurface::isVertexLinking() const {
    return vector->isVertexLinking(triangulation);
}

inline bool NNormalSurface::isSplitting() const {
    return vector->isSplitting(triangulation);
}
        
#endif

