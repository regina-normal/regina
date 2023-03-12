
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

/*! \file subcomplex/layeredloop.h
 *  \brief Deals with layered loop components of a triangulation.
 */

#ifndef __REGINA_LAYEREDLOOP_H
#ifndef __DOXYGEN
#define __REGINA_LAYEREDLOOP_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * Represents a layered loop component of a triangulation.
 *
 * A layered loop is a layered chain of \a n tetrahedra whose bottom
 * tetrahedron is layered onto its top tetrahedron to form a complete
 * loop.  See the LayeredChain class notes for a description of a
 * layered chain.
 *
 * To make a layered chain into a layered
 * loop, the bottom faces of the first tetrahedron will be
 * layered upon the top faces of the last tetrahedron, completing the loop.
 * At this stage there is a choice.  The layering can be done in the usual
 * fashion, or there may be a \a twist in which the upper square (the
 * bottom faces of the first tetrahedron) is rotated 180 degrees before
 * being layered on the lower annulus (the top faces of the last
 * tetrahedron).
 *
 * If there is a twist, the two hinge edges become identified and the
 * entire component has a single vertex.  If there is no twist, the
 * two hinge edges remain distinct (and disjoint) and the entire
 * component has two vertices.
 *
 * The \a length of the layered loop is the number of tetrahedra it
 * contains.  A layered loop must contain at least one tetrahedron.
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
class LayeredLoop : public StandardTriangulation {
    private:
        unsigned long length_;
            /**< The length of this layered loop. */
        Edge<3>* hinge_[2];
            /**< The hinge edge(s) of this layered loop.  If the loop is
                 twisted, the second element in this array will be \c null. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        LayeredLoop(const LayeredLoop&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        LayeredLoop& operator = (const LayeredLoop&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(LayeredLoop& other) noexcept;

        /**
         * Returns the length of this layered loop.
         * See the general class notes for further details.
         *
         * \return the length of this layered loop.
         */
        unsigned long length() const;
        /**
         * Determines if this layered loop contains a twist.
         * See the general class notes for further details.
         *
         * \return \c true if and only if this layered loop contains a
         * twist.
         */
        bool isTwisted() const;
        /**
         * Returns the requested hinge edge of this layered loop.
         * See the general class notes for further details.
         * If there is only one hinge but parameter \a which is 1,
         * \c null will be returned.
         *
         * \param which specifies which hinge to return; this must be 0
         * or 1.
         * \return the requested hinge edge.
         */
        Edge<3>* hinge(int which) const;

        /**
         * Determines whether this and the given structure represent
         * the same type of layered loop.
         *
         * Specifically, two layered loops will compare as equal if they
         * have the same length and either both are twisted or both are
         * untwisted.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass means they describe isomorphic structures).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of layered loop.
         */
        bool operator == (const LayeredLoop& other) const;

        /**
         * Determines whether this and the given structure represent
         * different types of layered loop.
         *
         * Specifically, two layered loops will compare as equal if they
         * have the same length and either both are twisted or both are
         * untwisted.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass means they describe isomorphic structures).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent different types of layered loop.
         */
        bool operator != (const LayeredLoop& other) const;

        /**
         * Determines if the given triangulation component is a layered loop.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param comp the triangulation component to examine.
         * \return a structure containing details of the layered loop, or
         * \c null if the given component is not a layered loop.
         */
        static std::unique_ptr<LayeredLoop> recognise(const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given data.
         */
        LayeredLoop(unsigned long length, Edge<3>* hinge0, Edge<3>* hinge1);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls LayeredLoop::swap(); it is provided
 * so that LayeredLoop meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(LayeredLoop& a, LayeredLoop& b) noexcept;

// Inline functions for LayeredLoop

inline LayeredLoop::LayeredLoop(unsigned long length, Edge<3>* hinge0,
        Edge<3>* hinge1) : length_(length), hinge_ { hinge0, hinge1 } {
}

inline void LayeredLoop::swap(LayeredLoop& other) noexcept {
    std::swap(length_, other.length_);
    std::swap(hinge_[0], other.hinge_[0]);
    std::swap(hinge_[1], other.hinge_[1]);
}

inline unsigned long LayeredLoop::length() const {
    return length_;
}
inline bool LayeredLoop::isTwisted() const {
    return (hinge_[1] == nullptr);
}
inline Edge<3>* LayeredLoop::hinge(int which) const {
    return hinge_[which];
}
inline bool LayeredLoop::operator == (const LayeredLoop& other) const {
    if (length_ != other.length_)
        return false;
    if (hinge_[1] && ! other.hinge_[1])
        return false;
    if (other.hinge_[1] && ! hinge_[1])
        return false;
    return true;
}
inline bool LayeredLoop::operator != (const LayeredLoop& other) const {
    return ! ((*this) == other);
}
inline std::ostream& LayeredLoop::writeName(std::ostream& out) const {
    return out << (hinge_[1] ? "C(" : "C~(") << length_ << ')';
}
inline std::ostream& LayeredLoop::writeTeXName(std::ostream& out) const {
    return out << (hinge_[1] ? "C_{" : "\\tilde{C}_{") << length_ << '}';
}
inline void LayeredLoop::writeTextLong(std::ostream& out) const {
    out << "Layered loop (" << (hinge_[1] ? "not twisted" : "twisted") <<
        ") of length " << length_;
}

inline void swap(LayeredLoop& a, LayeredLoop& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

