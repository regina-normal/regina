
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

/*! \file subcomplex/snappeacensustri.h
 *  \brief Deals with 3-manifold triangulations from the SnapPea census.
 */

#ifndef __REGINA_SNAPPEACENSUSTRI_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEACENSUSTRI_H
#endif

#include "regina-core.h"
#include "standardtri.h"

namespace regina {

/**
 * Represents a 3-manifold triangulation from the SnapPea cusped census.
 *
 * The SnapPea cusped census is the census of cusped hyperbolic 3-manifolds
 * formed from up to seven tetrahedra.  This census was tabulated by
 * Callahan, Hildebrand and Weeks, and is shipped with SnapPea 3.0d3
 * (and also with Regina).
 *
 * \note The modern cusped hyperbolic census now extends to nine tetrahedra,
 * and indeed the 9-tetrahedron database is accessible through the
 * Census lookup routines.  However, for the time being, the scope of these
 * SnapPeaCensusManifold and SnapPeaCensusTri classes is restricted to the
 * original Callahan-Hildebrand-Weeks 7-tetrahedron census only.
 *
 * The census is split into five different sections according to number
 * of tetrahedra and orientability.  Each of these sections corresponds
 * to one of the section constants defined in this class.
 *
 * For further details regarding the SnapPea census, see "A census of cusped
 * hyperbolic 3-manifolds", Patrick J. Callahan, Martin V. Hildebrand and
 * Jeffrey R. Weeks, Math. Comp. 68 (1999), no. 225, pp. 321--332.
 *
 * Note that this class is closely tied to SnapPeaCensusManifold.
 * In particular, the section constants defined in SnapPeaCensusManifold
 * and SnapPeaCensusTri are identical, and so may be freely mixed.
 * Furthermore, the section and index parameters of a SnapPeaCensusTri
 * are identical to those of its corresponding SnapPeaCensusManifold.
 *
 * All of the optional StandardTriangulation routines are implemented
 * for this class.
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
class SnapPeaCensusTri: public StandardTriangulation {
    public:
        static constexpr char SEC_5 = 'm';
            /**< Represents the collection of triangulations formed from five
                 or fewer tetrahedra (both orientable and non-orientable).
                 There are 415 triangulations in this section. */
        static constexpr char SEC_6_OR = 's';
            /**< Represents the collection of orientable triangulations formed
                 from six tetrahedra.
                 There are 962 triangulations in this section. */
        static constexpr char SEC_6_NOR = 'x';
            /**< Represents the collection of non-orientable triangulations
                 formed from six tetrahedra.
                 There are 259 triangulations in this section. */
        static constexpr char SEC_7_OR = 'v';
            /**< Represents the collection of orientable triangulations formed
                 from seven tetrahedra.
                 There are 3552 triangulations in this section. */
        static constexpr char SEC_7_NOR = 'y';
            /**< Represents the collection of non-orientable triangulations
                 formed from seven tetrahedra.
                 There are 887 triangulations in this section. */

    private:
        char section_;
            /**< The section of the SnapPea census to which this
                 triangulation belongs.  This must be one of the section
                 constants defined in this class. */
        size_t index_;
            /**< The index within the given section of this specific
                 triangulation.  Note that the first index in each section
                 is zero. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        SnapPeaCensusTri(const SnapPeaCensusTri&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        SnapPeaCensusTri& operator = (const SnapPeaCensusTri&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(SnapPeaCensusTri& other) noexcept;

        /**
         * Returns the section of the SnapPea census to which this
         * triangulation belongs.  This will be one of the section constants
         * defined in this class.
         *
         * \return the section of the SnapPea census.
         */
        char section() const;

        /**
         * Returns the index of this triangulation within its particular
         * section of the SnapPea census.  Note that indices for each
         * section begin counting at zero.
         *
         * \return the index of this triangulation within its section.
         */
        size_t index() const;

        /**
         * Determines whether this and the given structure represent
         * the same triangulation from the SnapPea census.
         *
         * The SnapPea census manifolds \c x101 and \c x103 are homeomorphic;
         * however, the corresponding triangulations represented by
         * SnapPeaCensusTri will compare as different (since this class
         * describes the specific triangulation, not the underlying manifold).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass means they describe isomorphic structures).
         *
         * \param compare the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same SnapPea census triangulation.
         */
        bool operator == (const SnapPeaCensusTri& compare) const;

        /**
         * Determines whether this and the given structure represent
         * different triangulations from the SnapPea census.
         *
         * The SnapPea census manifolds \c x101 and \c x103 are homeomorphic;
         * however, the corresponding triangulations represented by
         * SnapPeaCensusTri will compare as different (since this class
         * describes the specific triangulation, not the underlying manifold).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass means they describe isomorphic structures).
         *
         * \param compare the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent different SnapPea census triangulations.
         */
        bool operator != (const SnapPeaCensusTri& compare) const;

        /**
         * Determines if the given triangulation component is one of the
         * smallest SnapPea census triangulations.
         *
         * This routine is able to recognise a small selection of
         * triangulations from the beginning of the SnapPea census, by
         * way of hard-coding their structures and properties.
         * Most triangulations from the census however will not be
         * recognised by this routine.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param comp the triangulation component to examine.
         * \return a structure representing the small SnapPea census
         * triangulation, or \c null if the given component is not one of
         * the few SnapPea census triangulations recognised by this routine.
         */
        static std::unique_ptr<SnapPeaCensusTri> recognise(
            const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;

    private:
        /**
         * Creates a new SnapPea census triangulation with the given
         * parameters.
         */
        SnapPeaCensusTri(char section, size_t index);

    friend class SnapPeaCensusManifold;
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls SnapPeaCensusTri::swap(); it is provided
 * so that SnapPeaCensusTri meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SnapPeaCensusTri& a, SnapPeaCensusTri& b) noexcept;

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline SnapPeaCensusTri::SnapPeaCensusTri(char section, size_t index) :
        section_(section), index_(index) {
}

// Inline functions for SnapPeaCensusTri

inline void SnapPeaCensusTri::swap(SnapPeaCensusTri& other) noexcept {
    std::swap(section_, other.section_);
    std::swap(index_, other.index_);
}

inline char SnapPeaCensusTri::section() const {
    return section_;
}

inline size_t SnapPeaCensusTri::index() const {
    return index_;
}

inline bool SnapPeaCensusTri::operator == (const SnapPeaCensusTri& compare)
        const {
    return (section_ == compare.section_ && index_ == compare.index_);
}

inline bool SnapPeaCensusTri::operator != (const SnapPeaCensusTri& compare)
        const {
    return (section_ != compare.section_ || index_ != compare.index_);
}

inline void swap(SnapPeaCensusTri& a, SnapPeaCensusTri& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

