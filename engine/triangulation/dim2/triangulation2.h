
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

/*! \file triangulation/dim2/triangulation2.h
 *  \brief Internal header for 2-dimensional triangulations.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

#ifndef __TRIANGULATION2_H
#ifndef __DOXYGEN
#define __TRIANGULATION2_H
#endif

#include <memory>
#include <vector>
#include "regina-core.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/markedvector.h"
#include "utilities/property.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

class XMLPacketReader;

/**
 * \addtogroup dim2 2-Manifold Triangulations
 * Details for implementing triangulations of 2-manifolds.
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_TRIANGULATION2> {
    typedef Triangulation<2> Class;
    inline static const char* name() {
        return "2-Manifold Triangulation";
    }
};
#endif

/**
 * Represents a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Triangulation class template;
 * see the Triangulation documentation for a general overview of how
 * the triangulation classes work.
 *
 * This 2-dimensional specialisation offers significant extra functionality,
 * including many functions specific to 2-manifolds.
 *
 * \headerfile triangulation/dim2.h
 */
template <>
class REGINA_API Triangulation<2> :
        public Packet,
        public detail::TriangulationBase<2> {
    REGINA_PACKET(Triangulation<2>, PACKET_TRIANGULATION2)

    public:
        typedef std::vector<Triangle<2>*>::const_iterator TriangleIterator;
            /**< A dimension-specific alias for SimplexIterator,
                 used to iterate through triangles. */
        typedef FaceList<2, 1>::Iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef FaceList<2, 0>::Iterator VertexIterator;
            /**< Used to iterate through vertices. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Triangulation();
        /**
         * Creates a new copy of the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * @param copy the triangulation to copy.
         * @param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a triangulation.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - isomorphism signatures (see fromIsoSig()).
         *
         * This list may grow in future versions of Regina.
         *
         * Regina will also set the packet label accordingly.
         *
         * If Regina cannot interpret the given string, this will be
         * left as the empty triangulation.
         *
         * @param description a string that describes a 2-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
        /**
         * Destroys this triangulation.
         *
         * The constituent triangles, the cellular structure and all other
         * properties will also be destroyed.
         */
        virtual ~Triangulation();

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual bool dependsOnParent() const;

        /*@}*/
        /**
         * \name Triangles
         */
        /*@{*/

        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Triangle<2>* newTriangle();
        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Triangle<2>* newTriangle(const std::string& desc);
        /**
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         */
        void removeTriangle(Triangle<2>* tri);
        /**
         * A dimension-specific alias for removeSimplexAt().
         *
         * See removeSimplexAt() for further information.
         */
        void removeTriangleAt(size_t index);
        /**
         * A dimension-specific alias for removeAllSimplices().
         *
         * See removeAllSimplices() for further information.
         */
        void removeAllTriangles();

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Always returns \c true.
         *
         * This routine determines if this triangulation is valid; however,
         * there is nothing that can go wrong with vertex links in 2-manifold
         * triangulations, and so this routine always returns \c true.
         *
         * This no-op routine is provided for consistency with higher
         * dimensional triangulations, and to assist with writing
         * dimension-agnostic code.
         *
         * @return \c true.
         */
        bool isValid() const;
        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated as \a V-E+F.
         *
         * This returns the same result as eulerCharTri().
         *
         * @return the Euler characteristic of this triangulation.
         */
        long eulerChar() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary components.
         *
         * @return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;
        /**
         * Always returns \c false.
         *
         * This routine determines if this triangulation is ideal (has a
         * non-trivial vertex link); however, every vertex link in a
         * 2-manifold triangulation is either the interval or the
         * circle, and so ideal triangulations cannot exist.
         * Therefore this routine always returns \c false.
         *
         * This no-op routine is provided for consistency with higher
         * dimensional triangulations, and to assist with writing
         * dimension-agnostic code.
         *
         * @return \c false.
         */
        bool isIdeal() const;

        /**
         * Determines whether this is a minimal triangulation of the
         * underlying 2-manifold; that is, it uses the fewest possible
         * triangles.
         *
         * Testing for minimality is simple in two dimensions (unlike
         * higher dimensions, where it becomes extremely difficult).
         * With the exception of the sphere, disc and projective plane
         * (which require a minimum of 2, 1 and 2 triangles respectively),
         * a closed triangulation is minimal if and only if it has one
         * vertex, and a bounded triangulation is minimal if and only if
         * it has one vertex per boundary component and no internal vertices.
         *
         * The proof is based on a simple Euler characteristic calculation,
         * whereby the number of triangles <tt>T</tt> is
         * <tt>T = 2Vi + Vb - 2C</tt>, where <tt>Vi</tt> and <tt>Vb</tt>
         * are the number of internal and boundary vertices respectively,
         * and where <tt>C</tt> is the Euler characteristic of the
         * underlying manifold.
         *
         * @return \c true if and only if this is a minimal triangulation.
         */
        bool isMinimal() const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Checks the eligibility of and/or performs a 1-3 move
         * upon the given triangle.
         * This involves replacing one triangle with three triangles:
         * each new triangle runs from one edge of
         * the original triangle to a new common internal degree three vertex.
         *
         * This move can always be performed.  The \a check argument is
         * present (as for other moves), but is simply ignored (since
         * the move is always legal).  The \a perform argument is also
         * present for consistency with other moves, but if it is set to
         * \c false then this routine does nothing and returns no useful
         * information.
         *
         * Note that after performing this move, all skeletal objects
         * (edges, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a t)
         * can no longer be used.
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * @param t the triangle about which to perform the move.
         * @param check this argument is ignored, since this move is
         * always legal (see the notes above).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return \c true always.
         */
        bool oneThreeMove(Triangle<2>* t, bool check = true,
            bool perform = true);

        /*@}*/

        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        REGINA_INLINE_REQUIRED void clearAllProperties();
        /**
         * Swaps all calculated properties, including skeletal data,
         * with the given triangulation.  This is called by
         * TriangulationBase::swapContents(), and by nothing else.
         */
        void swapAllProperties(Triangulation<2>& other);

        void calculateSkeleton();

    friend class regina::Face<2, 2>;
    friend class regina::detail::SimplexBase<2>;
    friend class regina::detail::TriangulationBase<2>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Triangulation, you should use
 * the templated class name Triangulation<2>.
 */
REGINA_DEPRECATED typedef Triangulation<2> Dim2Triangulation;

// Additional face typedefs that do not have their own headers:

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2VertexEmbedding, you should
 * use either the new alias VertexEmbedding<2>, or the full class name
 * FaceEmbedding<2, 0>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<2, 0> Dim2VertexEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2EdgeEmbedding, you should
 * use either the new alias EdgeEmbedding<2>, or the full class name
 * FaceEmbedding<2, 1>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<2, 1> Dim2EdgeEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Vertex, you should use
 * either the new alias Vertex<2>, or the full class name Face<2, 0>.
 */
REGINA_DEPRECATED typedef Face<2, 0> Dim2Vertex;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Edge, you should use
 * either the new alias Edge<2>, or the full class name Face<2, 1>.
 */
REGINA_DEPRECATED typedef Face<2, 1> Dim2Edge;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim2/triangle2.h"
#include "triangulation/dim2/component2.h"
#include "triangulation/dim2/boundarycomponent2.h"
namespace regina {

// Inline functions for Triangulation<2>

inline Triangulation<2>::Triangulation() {
}

inline Triangulation<2>::Triangulation(const Triangulation& cloneMe) :
        TriangulationBase<2>(cloneMe) {
    // No properties yet to clone.
}

inline Triangulation<2>::Triangulation(const Triangulation& cloneMe,
        bool cloneProps) :
        TriangulationBase<2>(cloneMe, cloneProps) {
    // No properties yet to clone.
}

inline Triangulation<2>::~Triangulation() {
    clearAllProperties();
}

inline void Triangulation<2>::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << simplices_.size()
        << (simplices_.size() == 1 ? " triangle" : " triangles");
}

inline bool Triangulation<2>::dependsOnParent() const {
    return false;
}

inline Triangle<2>* Triangulation<2>::newTriangle() {
    return newSimplex();
}

inline Triangle<2>* Triangulation<2>::newTriangle(const std::string& desc) {
    return newSimplex(desc);
}

inline void Triangulation<2>::removeTriangle(Triangle<2>* tri) {
    removeSimplex(tri);
}

inline void Triangulation<2>::removeTriangleAt(size_t index) {
    removeSimplexAt(index);
}

inline void Triangulation<2>::removeAllTriangles() {
    removeAllSimplices();
}

inline bool Triangulation<2>::isValid() const {
    return true;
}

inline long Triangulation<2>::eulerChar() const {
    ensureSkeleton();

    // Cast away the unsignedness of std::vector::size().
    return static_cast<long>(countVertices())
        - static_cast<long>(countEdges())
        + static_cast<long>(simplices_.size());
}

inline bool Triangulation<2>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents().empty();
}

inline bool Triangulation<2>::isIdeal() const {
    return false;
}

inline Packet* Triangulation<2>::internalClonePacket(Packet*) const {
    return new Triangulation(*this);
}

inline void Triangulation<2>::clearAllProperties() {
    clearBaseProperties();
}

inline void Triangulation<2>::swapAllProperties(Triangulation<2>& other) {
    swapBaseProperties(other);
}

} // namespace regina

#endif

