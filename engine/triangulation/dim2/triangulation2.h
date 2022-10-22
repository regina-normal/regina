
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#ifndef __REGINA_TRIANGULATION2_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION2_H
#endif

#include <memory>
#include <vector>
#include "regina-core.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/markedvector.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

/**
 * \defgroup dim2 2-Manifold Triangulations
 * Details for implementing triangulations of 2-manifolds.
 */

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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \headerfile triangulation/dim2.h
 *
 * \ingroup dim2
 */
template <>
class Triangulation<2> : public detail::TriangulationBase<2> {
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
        Triangulation() = default;
        /**
         * Creates a new copy of the given triangulation.
         *
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * \param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy) = default;
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * \param copy the triangulation to copy.
         * \param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
        /**
         * Moves the given triangulation into this new triangulation.
         *
         * This is much faster than the copy constructor, but is still linear
         * time.  This is because every triangle must be adjusted to point
         * back to this new triangulation instead of \a src.
         *
         * All triangles and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Triangle<2>, Face<2, subdim>, Component<2> or
         * BoundaryComponent<2> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the triangulation to move.
         */
        Triangulation(Triangulation&& src) noexcept = default;
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
         * \exception InvalidArgument Regina could not interpret the given
         * string as representing a triangulation using any of the supported
         * string types.
         *
         * \param description a string that describes a 2-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
        /**
         * Destroys this triangulation.
         *
         * The constituent triangles, the cellular structure and all other
         * properties will also be destroyed.
         */
        ~Triangulation();

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
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        template <int k>
        std::array<Triangle<2>*, k> newTriangles();
        /**
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        void newTriangles(size_t k);
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

        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (const Triangulation&) = default;

        /**
         * Moves the contents of the given triangulation into this
         * triangulation.
         *
         * This is much faster than copy assignment, but is still linear
         * time.  This is because every triangle must be adjusted to point
         * back to this triangulation instead of \a src.
         *
         * All triangles and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Triangle<2>, Face<2, subdim>, Component<2> or
         * BoundaryComponent<2> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this triangulation which may in turn call arbitrary
         * code via any registered packet listeners.  It deliberately does
         * _not_ fire change events on \a src, since it assumes that \a src is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * \param src the triangulation to move.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (Triangulation&& src) = default;

        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All triangles that belong to this triangulation
         * will be moved to \a other, and all triangles
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * will be swapped.
         *
         * In particular, any pointers or references to Triangle<2> and/or
         * Face<2, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both triangulations which may in turn call
         * arbitrary code via any registered packet listeners.
         *
         * \param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<2>& other);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryEdges() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryEdges() const;

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
         * \return \c true.
         */
        bool isValid() const;
        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated as \a V-E+F.
         *
         * This returns the same result as eulerCharTri().
         *
         * \return the Euler characteristic of this triangulation.
         */
        long eulerChar() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary components.
         *
         * \return \c true if and only if this triangulation is closed.
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
         * \return \c false.
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
         * whereby the number of triangles `T` is
         * `T = 2Vi + Vb - 2C`, where `Vi` and `Vb`
         * are the number of internal and boundary vertices respectively,
         * and where `C` is the Euler characteristic of the
         * underlying manifold.
         *
         * \return \c true if and only if this is a minimal triangulation.
         */
        bool isMinimal() const;

        /*@}*/

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a change event.
         */
        void clearAllProperties();

        void calculateSkeleton();

    friend class regina::Face<2, 2>;
    friend class regina::detail::SimplexBase<2>;
    friend class regina::detail::TriangulationBase<2>;
};

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline void Triangulation<2>::clearAllProperties() {
    clearBaseProperties();
}

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim2/triangle2.h"
#include "triangulation/dim2/component2.h"
namespace regina {

// Inline functions for Triangulation<2>

inline Triangulation<2>::Triangulation(const Triangulation& cloneMe,
        bool cloneProps) :
        TriangulationBase<2>(cloneMe, cloneProps) {
    // No properties yet to clone.
}

inline Triangulation<2>::~Triangulation() {
    Snapshottable<Triangulation<2>>::takeSnapshot();
    clearAllProperties();
}

inline Triangle<2>* Triangulation<2>::newTriangle() {
    return newSimplex();
}

inline Triangle<2>* Triangulation<2>::newTriangle(const std::string& desc) {
    return newSimplex(desc);
}

template <int k>
inline std::array<Triangle<2>*, k> Triangulation<2>::newTriangles() {
    return newSimplices<k>();
}

inline void Triangulation<2>::newTriangles(size_t k) {
    newSimplices(k);
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

inline bool Triangulation<2>::hasBoundaryEdges() const {
    return hasBoundaryFacets();
}

inline size_t Triangulation<2>::countBoundaryEdges() const {
    return countBoundaryFacets();
}

inline bool Triangulation<2>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents().empty();
}

inline bool Triangulation<2>::isIdeal() const {
    return false;
}

} // namespace regina

#endif

