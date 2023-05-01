
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file subcomplex/layeredlensspace.h
 *  \brief Deals with layered lens space components of a triangulation.
 */

#ifndef __REGINA_LAYEREDLENSSPACE_H
#ifndef __DOXYGEN
#define __REGINA_LAYEREDLENSSPACE_H
#endif

#include "regina-core.h"
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * Represents a layered lens space component of a triangulation.
 * A layered lens space is considered to be any layered solid torus glued
 * to a degenerate (2,1,1) layered solid torus (i.e., a one-triangle mobius
 * strip).  Note that the three possible gluing options represent the
 * three possible ways of closing the initial torus - either twisting it
 * shut (in one of two possible ways) or snapping it shut without any twist.
 * 
 * A layered lens space must contain at least one tetrahedron.
 *
 * All optional StandardTriangulation routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class LayeredLensSpace : public StandardTriangulation {
    private:
        LayeredSolidTorus torus_;
            /**< The layered solid torus that forms the basis of this
                 layered lens space. */
        int mobiusBoundaryGroup_;
            /**< The edge group of the top level tetrahedron in the
                 layered solid torus to which the boundary of the mobius
                 strip is glued. */
        unsigned long p_,q_;
            /**< The lens space parameters for L(p,q). */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        LayeredLensSpace(const LayeredLensSpace&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        LayeredLensSpace& operator = (const LayeredLensSpace&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(LayeredLensSpace& other) noexcept;

        /**
         * Returns the first parameter \a p of this lens space L(p,q).
         *
         * \return the first parameter \a p.
         */
        unsigned long p() const;
        /**
         * Returns the second parameter \a q of this lens space L(p,q).
         *
         * \return the second parameter \a q.
         */
        unsigned long q() const;

        /**
         * Returns the layered solid torus to which the mobius strip is
         * glued.
         *
         * \return the layered solid torus.
         */
        const LayeredSolidTorus& torus() const;
        /**
         * Determines which edge of the layered solid torus is glued to
         * the boundary of the mobius strip (i.e., the weight 2 edge
         * of the degenerate (2,1,1) layered solid torus).  The return
         * value will be one of the three top level tetrahedron edge
         * groups in the layered solid torus; see
         * LayeredSolidTorus::topEdge() for further details about
         * edge groups.
         *
         * \return the top level edge group of the layered solid torus to
         * which the mobius strip boundary is glued.
         */
        int mobiusBoundaryGroup() const;
        /**
         * Determines if the layered solid torus that forms the basis for
         * this lens space is snapped shut (folded closed without a twist).
         *
         * \return \c true if and only if the torus is snapped shut.
         */
        bool isSnapped() const;
        /**
         * Determines if the layered solid torus that forms the basis for
         * this lens space is twisted shut (folded closed with a twist).
         *
         * \return \c true if and only if the torus is twisted shut.
         */
        bool isTwisted() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of layered lens space.
         *
         * Specifically, two layered lens spaces will compare as equal if and
         * only if they are formed from layered solid tori with the same
         * parameters, closed off in the same way.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * the same layered lens space can be built from a layered solid
         * torus starting at either of its two "ends").
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of layered lens space.
         */
        bool operator == (const LayeredLensSpace& other) const;

        /**
         * Determines whether this and the given structure represent
         * different types of layered lens space.
         *
         * Specifically, two layered lens spaces will compare as equal if and
         * only if they are formed from layered solid tori with the same
         * parameters, closed off in the same way.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * the same layered lens space can be built from a layered solid
         * torus starting at either of its two "ends").
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent different types of layered lens space.
         */
        bool operator != (const LayeredLensSpace& other) const;

        /**
         * Determines if the given triangulation component is a layered
         * lens space.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param comp the triangulation component to examine.
         * \return a structure containing details of the layered lens space,
         * or \c null if the given component is not a layered lens space.
         */
        static std::unique_ptr<LayeredLensSpace> recognise(
            const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure initialised with the given layered
         * solid torus.  All other data members will be left uninitialised.
         */
        LayeredLensSpace(const LayeredSolidTorus& torus);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls LayeredLensSpace::swap(); it is provided
 * so that LayeredLensSpace meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(LayeredLensSpace& a, LayeredLensSpace& b) noexcept;

// Inline functions for LayeredLensSpace

inline LayeredLensSpace::LayeredLensSpace(const LayeredSolidTorus& torus) :
        torus_(torus) {
}

inline void LayeredLensSpace::swap(LayeredLensSpace& other) noexcept {
    torus_.swap(other.torus_);
    std::swap(mobiusBoundaryGroup_, other.mobiusBoundaryGroup_);
    std::swap(p_, other.p_);
    std::swap(q_, other.q_);
}

inline unsigned long LayeredLensSpace::p() const {
    return p_;
}
inline unsigned long LayeredLensSpace::q() const {
    return q_;
}
inline const LayeredSolidTorus& LayeredLensSpace::torus() const {
    return torus_;
}
inline int LayeredLensSpace::mobiusBoundaryGroup() const {
    return mobiusBoundaryGroup_;
}
inline bool LayeredLensSpace::isSnapped() const {
    return (torus_.topEdge(mobiusBoundaryGroup_, 1) == -1);
}
inline bool LayeredLensSpace::isTwisted() const {
    return (torus_.topEdge(mobiusBoundaryGroup_, 1) != -1);
}

inline bool LayeredLensSpace::operator == (const LayeredLensSpace& other)
        const {
    return torus_ == other.torus_ &&
        mobiusBoundaryGroup_ == other.mobiusBoundaryGroup_;
}
inline bool LayeredLensSpace::operator != (const LayeredLensSpace& other)
        const {
    return torus_ != other.torus_ ||
        mobiusBoundaryGroup_ != other.mobiusBoundaryGroup_;
}

inline void LayeredLensSpace::writeTextLong(std::ostream& out) const {
    out << "Layered lens space ";
    writeName(out);
}

inline void swap(LayeredLensSpace& a, LayeredLensSpace& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

