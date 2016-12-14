
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

/*! \file hypersurface/normalhypersurface.h
 *  \brief Deals with an individual normal hypersurface in a 4-manifold
 *  triangulation.
 */

#ifndef __NORMALHYPERSURFACE_H
#ifndef __DOXYGEN
#define __NORMALHYPERSURFACE_H
#endif

#include <utility>
#include "regina-core.h"
#include "output.h"
#include "algebra/abeliangroup.h"
#include "hypersurface/hypercoords.h"
#include "maths/ray.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/property.h"
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \addtogroup hypersurface Normal Hypersurfaces
 * Normal hypersurfaces in 4-manifold triangulations.
 * @{
 */

class EnumConstraints;

template <typename> class MatrixIntDomain;
typedef MatrixIntDomain<Integer> MatrixInt;

/**
 * A template that stores information about a particular
 * normal hypersurface coordinate system.  Much of this information is
 * given in the form of compile-time constants and types.
 *
 * To iterate through cases for a given value of HyperCoords that is not
 * known until runtime, see the various forCoords() routines defined in
 * hscoordregistry.h.
 *
 * This HyperInfo template should only be defined for \a coordType
 * arguments that represent coordinate systems in which you can create
 * and store normal hypersurfaces within 4-manifold triangulations.
 *
 * At a bare minimum, each specialisation of this template must provide:
 *
 * - a typedef \a Class that represents the corresponding
 *   NormalHypersurfaceVector subclass;
 * - a static function name() that returns a C-style string giving the
 *   human-readable name of the coordinate system.
 *
 * \ifacespython Not present.
 *
 * \tparam coordType one of the #HyperCoords constants, indicating which
 * coordinate system we are querying.
 */
template <HyperCoords coordType>
struct HyperInfo;

/**
 * Defines various constants, types and virtual functions for a subclass
 * of NormalHypersurfaceVector.
 *
 * Every subclass of NormalHypersurfaceVector \a must include
 * REGINA_NORMAL_HYPERSURFACE_FLAVOUR at the beginning of the class definition.
 *
 * This macro provides the class with:
 *
 * - a compile-time enum constant \a coordsID, which is equal to the
 *   corresponding HyperCoords constant;
 * - a typedef \a Info, which refers to the corresponding specialisation 
 *   of the HyperInfo<> tempate;
 * - a copy constructor that takes a vector of the same subclass;
 * - declarations and implementations of the virtual function
 *   NormalHypersurfaceVector::clone().
 *
 * @param class_ the name of this subclass of NormalHypersurfaceVector.
 * @param id the corresponding NNormalCoords constant.
 * @param superclass the vector class from which \a class_ is derived.
 * This is typically NormalHypersurfaceVector, though in some cases
 * (e.g., prism coordinates) it may be different.
 */
#define REGINA_NORMAL_HYPERSURFACE_FLAVOUR(class_, id, superclass) \
    public: \
        typedef HyperInfo<id> Info; \
        static constexpr const HyperCoords coordsID = id; \
        inline class_(const class_& cloneMe) : \
                superclass(cloneMe.coords()) {} \
        inline virtual NormalHypersurfaceVector* clone() const { \
            return new class_(*this); \
        }

/**
 * Stores the vector of a single normal hypersurface in a 4-manifold
 * triangulation.  The different subclasses of NormalHypersurfaceVector use
 * different underlying coordinate systems for the normal solution space.
 * However, the various coordinate retrieval routines will return values
 * that are independent of the underlying coordinate system.  Thus the
 * coordinates of the normal hypersurface in any coordinate system can be
 * determined without knowledge of the specific underlying coordinate
 * system being used.
 *
 * Note that non-compact hypersurfaces (surfaces with infinitely many pieces)
 * are allowed; in these cases, the corresponding coordinate lookup routines
 * should return LargeInteger::infinity where appropriate.
 *
 * All subclasses of NormalHypersurfaceVector <b>must</b> have the following
 * properties:
 *
 * - Normal hypersurfaces can be enumerated by intersecting the non-negative
 *   orthant of the underlying vector space with some linear subspace;
 *
 * - Adding two normal hypersurfaces corresponds to adding the two underlying
 *   vectors.
 *
 * <b>When deriving classes from NormalHypersurfaceVector:</b>
 * <ul>
 *   <li>A new value must be added to the HyperCoords enum in hypercoords.h
 *   to represent the new coordinate system.</li>
 *   <li>The file hscoordregistry-impl.h must be updated to reflect the new
 *   coordinate system (the file itself contains instructions on
 *   how to do this).</li>
 *   <li>A corresponding specialisation of HyperInfo<> must be defined,
 *   typically in the same header as the new vector subclass.</li>
 *   <li>The macro REGINA_NORMAL_HYPERSURFACE_FLAVOUR must be added to
 *   the beginning of the new vector subclass.  This will declare and
 *   define various constants, typedefs and virtual functions (see the
 *   REGINA_NORMAL_HYPERSURFACE_FLAVOUR macro documentation for details).</li>
 *   <li>Constructors <tt>class(size_t length)</tt> and
 *   <tt>class(const Vector<LargeInteger>& cloneMe)</tt> must be
 *   declared and implemented; these will usually just call the
 *   corresponding superclass constructors.</li>
 *   <li>All abstract functions must be implemented, except for those
 *   already provided by REGINA_NORMAL_HYPERSURFACE_FLAVOUR.
 *   Note that coordinate functions such as tetrahedra() must return the
 *   \e total number of pieces of the requested type; if your new coordinate
 *   system adorns pieces with extra information (such as orientation) then
 *   your implementation must compute the appropriate sum.</li>
 *   <li>Static public functions <tt>void
 *   makeZeroVector(const Triangulation<4>*)</tt>,
 *   <tt>MatrixInt* makeMatchingEquations(const Triangulation<4>*)</tt> and
 *   makeEmbeddedConstraints(const Triangulation<4>*) must be
 *   declared and implemented.</li>
 * </ul>
 *
 * \ifacespython Not present.
 */
class REGINA_API NormalHypersurfaceVector : public boost::noncopyable {
    protected:
        Ray coords_;
            /**< The raw vector of normal coordinates. */

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NormalHypersurfaceVector(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NormalHypersurfaceVector(const Vector<LargeInteger>& cloneMe);

        /**
         * A virtual destructor.  This is required because here we
         * introduce virtual functions into the Ray hierarchy.
         */
        virtual ~NormalHypersurfaceVector();

        /**
         * Gives read-only access to the underlying vector of coordinates.
         *
         * @return the vector of coordinates.
         */
        const Ray& coords() const;

        /**
         * Creates a newly allocated clone of this vector.
         * The clone will be of the same subclass of NormalHypersurfaceVector
         * as this vector.
         */
        virtual NormalHypersurfaceVector* clone() const = 0;

        /**
         * Returns the number of coordinates in the underlying vector.
         *
         * @return the number of coordinates.
         */
        size_t size() const;

        /**
         * Returns the given coordinate from the underlying vector.
         *
         * @param index the index of the coordinate to retrieve; this
         * must be between 0 and size()-1 inclusive.
         * @return the coordinate at the given index.
         */
        const NLargeInteger& operator [] (size_t index) const;

        /**
         * Sets the given normal coordinate to the given value.
         *
         * The default implementation simply sets the coordinate in the
         * underlying vector.  Subclasses should reimplement this if they
         * carry any additional information that also need adjusting.
         *
         * @param index the index of the coordinate to set; this must e
         * between 0 and size()-1 inclusive.
         * @param value the new value to assign to the given coordinate.
         */
        virtual void setElement(size_t index, const LargeInteger& value);

        /**
         * Adds the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * The default implementation simply adds the coordinates of the
         * underlying vectors.  Subclasses should reimplement this if they
         * carry any additional information that also need adjusting.
         *
         * \pre This and the given vector represent normal hypersurfaces in
         * the same triangulation, and use the same normal coordinate system.
         *
         * @param other the vector to add to this vector.
         */
        virtual void operator += (const NormalHypersurfaceVector& other);

        /**
         * Scales this vector down by the greatest common divisor of all
         * its elements.  The resulting vector will be the smallest
         * multiple of the original that maintains integral entries, and
         * these entries will have the same signs as the originals.
         *
         * This routine poses no problem for vectors containing infinite
         * elements; such elements are simply ignored and left at
         * infinity.
         *
         * The default implementation simply scales down the underlying vector.
         * Subclasses should reimplement this if they carry any additional
         * information that also needs adjusting.
         */
        virtual void scaleDown();

        /**
         * Determines if the normal hypersurface represented is compact (has
         * finitely many pieces).
         *
         * The default implementation for this routine simply runs
         * through every piece type until a piece type with infinite piece
         * count is found or all piece types have been examined.
         * Subclasses of NormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return \c true if and only if the normal hypersurface represented
         * is compact.
         */
        virtual bool isCompact(const Triangulation<4>* triang) const;
        /**
         * Determines if the normal hypersurface represented is vertex
         * linking.  A <i>vertex linking</i> hypersurface contains only
         * tetrahedra.
         *
         * The default implementation for this routine simply runs
         * through every non-tetrahedron piece type ensuring that each
         * has no corresponding pieces.
         * Subclasses of NormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return \c true if and only if the normal hypersurface represented
         * is vertex linking.
         */
        virtual bool isVertexLinking(const Triangulation<4>* triang) const;
        /**
         * Determines if a rational multiple of the normal hypersurface
         * represented is the link of a single vertex.
         *
         * The default implementation for this routine involves counting the
         * number of pieces of every type.
         * Subclasses of NormalSurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the vertex linked by this hypersurface, or 0 if this
         * hypersurface is not the link of a single vertex.
         */
        virtual const Vertex<4>* isVertexLink(const Triangulation<4>* triang)
            const;
        /**
         * Determines if a rational multiple of the normal hypersurface
         * represented is the thin link of a single edge.
         *
         * The default implementation for this routine involves counting the
         * number of pieces of every type.
         * Subclasses of NormalHypersurfaceVector should override this if
         * they can provide a faster implementation.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the edge linked by this hypersurface, or 0 if this
         * hypersurface is not a thin edge link.
         */
        virtual const Edge<4>* isThinEdgeLink(const Triangulation<4>* triang)
            const;

        /**
         * Returns the number of tetrahedron pieces of the given type in
         * this normal hypersurface.
         * See NormalHypersurface::tetrahedra() for further details.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested tetrahedron pieces reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * @param vertex the vertex of the given pentachoron around
         * which the requested tetrahedron pieces lie; this should be between
         * 0 and 4 inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of tetrahedron pieces of the given type.
         */
        virtual LargeInteger tetrahedra(size_t pentIndex,
            int vertex, const Triangulation<4>* triang) const = 0;
        /**
         * Returns the number of prism pieces of the given type
         * in this normal hypersurface.
         * See NormalHypersurface::prisms() for further details.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested prism pieces reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * @param prismType specifies the edge of the given pentachoron that
         * this prism separates from the opposite triangle;
         * this should be between 0 and 9 inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of prism pieces of the given type.
         */
        virtual LargeInteger prisms(size_t pentIndex,
            int prismType, const Triangulation<4>* triang) const = 0;
        /**
         * Returns the number of times this normal hypersurface crosses the
         * given edge.
         * See NormalHypersurface::edgeWeight() for further details.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Triangulation<4>::countEdges()-1 inclusive.
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return the number of times this normal hypersurface crosses the
         * given edge.
         */
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const Triangulation<4>* triang) const = 0;

        /**
         * Returns a new normal hypersurface vector of the appropriate length
         * for the given triangulation and for the coordinate
         * system corresponding to this subclass of NormalHypersurfaceVector.
         * All elements of the new vector will be initialised to zero.
         *
         * See ::makeZeroVector() for further details.
         *
         * @param triangulation the triangulation upon which the
         * underlying coordinate system is based.
         * @return a new zero vector of the correct class and length.
         */
        #ifdef __DOXYGEN
            static NormalHypersurfaceVector* makeZeroVector(
                const Triangulation<4>* triangulation);
        #endif
        /**
         * Creates a new set of normal hypersurface matching equations for
         * the given triangulation using the coordinate
         * system corresponding to this particular subclass of
         * NormalHypersurfaceVector.
         *
         * See ::makeMatchingEquations() for further details.
         *
         * @param triangulation the triangulation upon which these
         * matching equations will be based.
         * @return a newly allocated set of matching equations.
         */
        #ifdef __DOXYGEN
            static MatrixInt* makeMatchingEquations(
                const Triangulation<4>* triangulation);
        #endif
        /**
         * Creates a new set of validity constraints representing
         * the condition that normal hypersurfaces be embedded.  The
         * validity constraints will be expressed relative to the
         * coordinate system corresponding to this particular
         * subclass of NormalHypersurfaceVector.
         *
         * @param triangulation the triangulation upon which these
         * validity constraints will be based.
         * @return a newly allocated set of constraints.
         */
        #ifdef __DOXYGEN
            static EnumConstraints* makeEmbeddedConstraints(
                const Triangulation<4>* triangulation);
        #endif
};

/**
 * Represents a single normal hypersurface in a 4-manifold triangulation.
 * Once the underlying triangulation changes, this normal hypersurface object
 * is no longer valid.
 *
 * The information provided by the various query methods is independent
 * of the underlying coordinate system being used.
 * See the NormalHypersurfaceVector class notes for details of what to do
 * when introducing a new coordinate system.
 *
 * Note that non-compact surfaces (surfaces with infinitely many pieces,
 * are allowed; in these cases, the corresponding coordinate lookup routines
 * will return LargeInteger::infinity where appropriate.
 */
class REGINA_API NormalHypersurface :
        public ShortOutput<NormalHypersurface>,
        public boost::noncopyable {
    protected:
        NormalHypersurfaceVector* vector_;
            /**< Contains the coordinates of the normal hypersurface in
             *   whichever space is appropriate. */
        const Triangulation<4>* triangulation_;
            /**< The triangulation in which this normal hypersurface resides. */

        std::string name_;
            /**< An optional name associated with this hypersurface. */

        mutable Property<bool> orientable_;
            /**< Is this hypersurface orientable? */
        mutable Property<bool> twoSided_;
            /**< Is this hypersurface two-sided? */
        mutable Property<bool> connected_;
            /**< Is this hypersurface connected? */
        mutable Property<bool> realBoundary_;
            /**< Does this hypersurface have real boundary (i.e. does it meet
                 any boundary facets)? */
        mutable Property<bool> compact_;
            /**< Is this hypersurface compact (i.e., does it only
                 contain finitely many pieces)? */
        mutable Property<AbelianGroup, StoreManagedPtr> H1_;
            /**< First homology group of the hypersurface. */

    public:
        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector.
         *
         * This normal hypersurface will claim ownership of the given vector
         * (i.e., you should not change or delete the vector yourself
         * afterwards).
         *
         * \pre The given coordinate vector represents a
         * normal hypersurface inside the given triangulation.
         * \pre The given coordinate vector cannot be the null pointer.
         *
         * \ifacespython Not present.
         *
         * @param triangulation the triangulation in which this normal
         * hypersurface resides.
         * @param vector a vector containing the coordinates of the
         * normal hypersurface in whichever space is appropriate.
         */
        NormalHypersurface(const Triangulation<4>* triangulation,
            NormalHypersurfaceVector* vector);
        /**
         * A Python-only routine that creates a new normal hypersurface
         * inside the given triangulation with the given coordinate vector.
         *
         * \pre The given coordinate system is one in which Regina is
         * able to enumerate and store normal hypersurfaces (not a system
         * like regina::HS_EDGE_WEIGHT, which is for viewing purposes only).
         * \pre The given coordinate vector represents a normal hypersurface
         * inside the given triangulation (in particular, it satisfies the
         * relevant system of matching equations).  This will not be checked,
         * and things \e will go wrong if you break it.
         *
         * \ifacescpp Not available; this routine is for Python only.
         *
         * @param triang the triangulation in which this normal hypersurface
         * resides.
         * @param coordSystem the coordinate system used by this normal
         * hypersurface.
         * @param allCoords the corresponding vector of normal coordinates,
         * expressed as a Python list.  The list elements will be
         * converted internally to LargeInteger objects.
         */
        #ifdef __DOXYGEN
        NormalHypersurface(const Triangulation<4>* triang,
            HyperCoords coordSystem, List allCoords);
        #endif
        /**
         * Destroys this normal hypersurface.
         * The underlying vector of coordinates will also be deallocated.
         */
        ~NormalHypersurface();

        /**
         * Creates a newly allocated clone of this normal hypersurface.
         *
         * The name of the normal hypersurface will \e not be copied to the
         * clone; instead the clone will have an empty name.
         *
         * @return a clone of this normal hypersurface.
         */
        NormalHypersurface* clone() const;

        /**
         * Creates a newly allocated hypersurface that is the double of this
         * hypersurface.
         *
         * @return the double of this normal hypersurface.
         */
        NormalHypersurface* doubleHypersurface() const;

        /**
         * Returns the number of tetrahedron pieces of the given type in
         * this normal hypersurface.
         * A tetrahedron piece type is identified by specifying a
         * pentachoron and a vertex of that pentachoron that the
         * tetrahedron surrounds.
         *
         * If you are using a coordinate system that adorns pieces with
         * additional information (such as orientation), this routine
         * returns the \e total number of tetrahedra in the given
         * pentachoron of the given type.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested pieces reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * @param vertex the vertex of the given pentachoron around
         * which the requested pieces lie; this should be between 0
         * and 4 inclusive.
         * @return the number of tetrahedron pieces of the given type.
         */
        LargeInteger tetrahedra(size_t pentIndex, int vertex) const;
        /**
         * Returns the number of prism pieces of the given type
         * in this normal hypersurface.
         * A prism piece type is identified by specifying a
         * pentachoron and an edge of that pentachoron; prisms of this
         * type will then separate edge \a i of the pentachoron from
         * triangle \a i of the pentachoron.
         *
         * If you are using a coordinate system that adorns pieces with
         * additional information (such as orientation), this routine
         * returns the \e total number of prisms in the given
         * pentachoron of the given type.
         *
         * @param pentIndex the index in the triangulation of the
         * pentachoron in which the requested prisms reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * @param prismType specifies the edge of the given pentachoron that
         * this prism separates from the opposite triangle;
         * this should be between 0 and 9 inclusive.
         * @return the number of prism pieces of the given type.
         */
        LargeInteger prisms(size_t pentIndex, int prismType) const;
        /**
         * Returns the number of times this normal hypersurface crosses the
         * given edge.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Triangulation<4>::countEdges()-1 inclusive.
         * @return the number of times this normal hypersurface crosses the
         * given edge.
         */
        LargeInteger edgeWeight(size_t edgeIndex) const;

        /**
         * Returns the number of coordinates in the specific underlying
         * coordinate system being used.
         *
         * @return the number of coordinates.
         */
        size_t countCoords() const;
        /**
         * Returns the triangulation in which this normal hypersurface resides.
         *
         * @return the underlying triangulation.
         */
        const Triangulation<4>* triangulation() const;

        /**
         * Returns the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * @return the name of associated with this hypersurface.
         */
        const std::string& name() const;
        /**
         * Sets the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * @param name the new name to associate with this hypersurface.
         */
        void setName(const std::string& name);

        /**
         * Writes this hypersurface to the given output stream, using
         * standard tetrahedron-prism coordinates.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
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
         * NormalHypersurfaces::writeXMLPacketData().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

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
         * Returns whether or not this hypersurface is orientable.
         *·
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * @return \c true if this hypersurface is orientable, or \c false if
         * this hypersurface is non-orientable.
         */
        bool isOrientable() const;
        /**
         * Returns whether or not this hypersurface is two-sided.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * @return \c true if this hypersurface is two-sided, or \c false if
         * this hypersurface is one-sided.
         */
        bool isTwoSided() const;
        /**
         * Returns whether or not this hypersurface is connected.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * @return \c true if this hypersurface is connected, or \c false if
         * this hypersurface is disconnected.
         */
        bool isConnected() const;
        /**
         * Determines if this hypersurface has any real boundary, that is,
         * whether it meets any boundary tetrahedra of the triangulation.
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
        const Vertex<4>* isVertexLink() const;
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
        const Edge<4>* isThinEdgeLink() const;

        /**
         * Returns the first homology group of this hypersurface.
         *
         * There is an important caveat regarding invalid 4-manifold
         * triangulations.  If the underlying triangulation has edge
         * links that are not spheres, then it is possible that this
         * normal hypersurface is not a compact 3-manifold.  In such a
         * case, this routine will compute homology in the same way as
         * Triangulation<3>::homology() - that is, by effectively truncating
         * ideal points of the hypersurface (which may arise where the
         * hypersurface meets an invalid edge).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * @return the first homology group.
         */
        const AbelianGroup& homology() const;

        /**
         * Returns a 3-manifold triangulation describing this normal
         * hypersurface.
         *
         * The triangulation will be simplified via
         * Triangulation<3>::intelligentSimplify(), which means that the
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
         * \pre This normal hypersurface is compact and embedded.
         *
         * @return a triangulation of this normal hypersurface.
         */
        Triangulation<3>* triangulate() const;

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
        bool sameSurface(const NormalHypersurface& other) const;

        /**
         * Determines whether this hypersurface is embedded.  This is true if
         * and only if the surface contains no conflicting prism types.
         *
         * @return \c true if and only if this hypersurface is embedded.
         */
        bool embedded() const;

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
        bool locallyCompatible(const NormalHypersurface& other) const;

        /**
         * Gives read-only access to the raw vector that sits beneath this
         * normal hypersurface.
         *
         * Generally users should not need this function.  However, it is
         * provided here in case the need should arise (e.g., for reasons
         * of efficiency).
         *
         * \warning An NormalHypersurface does not know what underlying
         * coordinate system its raw vector uses.  Unless you already know
         * the coordinate system in advance (i.e., you created the hypersurface
         * yourself), it is best to keep to the coordinate-system-agnostic
         * access functions such as NormalHypersurfaceVector::tetrahedra() and
         * NormalHypersurfaceVector::prisms().
         *
         * \ifacespython Not present.
         *
         * @return the underlying raw vector.
         */
        const Ray& rawVector() const;

    protected:
        /**
         * Calculates whether this hypersurface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary() const;
        /**
         * Calculate and store all properties that we derive from the
         * 3-manifold triangulation of this hypersurface.
         */
        void calculateFromTriangulation() const;

    friend class XMLNormalHypersurfaceReader;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalHypersurfaceVector has now been renamed to
 * NormalHypersurfaceVector.
 */
REGINA_DEPRECATED typedef NormalHypersurfaceVector NNormalHypersurfaceVector;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalHypersurface has now been renamed to
 * NormalHypersurface.
 */
REGINA_DEPRECATED typedef NormalHypersurface NNormalHypersurface;

/*@}*/

// Inline functions for NormalHypersurfaceVector

inline NormalHypersurfaceVector::NormalHypersurfaceVector(size_t length) :
        coords_(length) {
}
inline NormalHypersurfaceVector::NormalHypersurfaceVector(
        const Vector<LargeInteger>& cloneMe) : coords_(cloneMe) {
}
inline NormalHypersurfaceVector::~NormalHypersurfaceVector() {
}

inline const Ray& NormalHypersurfaceVector::coords() const {
    return coords_;
}

inline size_t NormalHypersurfaceVector::size() const {
    return coords_.size();
}

inline const NLargeInteger& NormalHypersurfaceVector::operator []
        (size_t index) const {
    return coords_[index];
}

inline void NormalHypersurfaceVector::setElement(size_t index,
        const LargeInteger& value) {
    coords_.setElement(index, value);
}

inline void NormalHypersurfaceVector::operator += (
        const NormalHypersurfaceVector& other) {
    coords_ += other.coords_;
}

inline void NormalHypersurfaceVector::scaleDown() {
    coords_.scaleDown();
}

// Inline functions for NormalHypersurface

inline NormalHypersurface::~NormalHypersurface() {
    delete vector_;
}

inline LargeInteger NormalHypersurface::tetrahedra(
        size_t pentIndex, int vertex) const {
    return vector_->tetrahedra(pentIndex, vertex, triangulation_);
}
inline LargeInteger NormalHypersurface::prisms(
        size_t pentIndex, int prismType) const {
    return vector_->prisms(pentIndex, prismType, triangulation_);
}
inline LargeInteger NormalHypersurface::edgeWeight(size_t edgeIndex) const {
    return vector_->edgeWeight(edgeIndex, triangulation_);
}

inline size_t NormalHypersurface::countCoords() const {
    return vector_->size();
}
inline const Triangulation<4>* NormalHypersurface::triangulation() const {
    return triangulation_;
}

inline const std::string& NormalHypersurface::name() const {
    return name_;
}
inline void NormalHypersurface::setName(const std::string& name) {
    name_ = name;
}

inline void NormalHypersurface::writeRawVector(std::ostream& out) const {
    out << vector_->coords();
}

inline bool NormalHypersurface::isCompact() const {
    if (! compact_.known())
        compact_ = vector_->isCompact(triangulation_);
    return compact_.value();
}

inline bool NormalHypersurface::isOrientable() const {
    if (! orientable_.known())
        calculateFromTriangulation();
    return orientable_.value();
}

inline bool NormalHypersurface::isTwoSided() const {
    if (! twoSided_.known())
        calculateFromTriangulation();
    return twoSided_.value();
}

inline bool NormalHypersurface::isConnected() const {
    if (! connected_.known())
        calculateFromTriangulation();
    return connected_.value();
}

inline bool NormalHypersurface::hasRealBoundary() const {
    if (! realBoundary_.known())
        calculateRealBoundary();
    return realBoundary_.value();
}

inline const AbelianGroup& NormalHypersurface::homology() const {
    if (! H1_.known())
        calculateFromTriangulation();
    return *H1_.value();
}

inline bool NormalHypersurface::isVertexLinking() const {
    return vector_->isVertexLinking(triangulation_);
}

inline const Vertex<4>* NormalHypersurface::isVertexLink() const {
    return vector_->isVertexLink(triangulation_);
}

inline const Edge<4>* NormalHypersurface::isThinEdgeLink() const {
    return vector_->isThinEdgeLink(triangulation_);
}

inline const Ray& NormalHypersurface::rawVector() const {
    return vector_->coords();
}

} // namespace regina

#endif

