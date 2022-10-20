
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

/*! \file manifold/snappeacensusmfd.h
 *  \brief Deals with 3-manifolds from the SnapPea census.
 */

#ifndef __REGINA_SNAPPEACENSUSMFD_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEACENSUSMFD_H
#endif

#include "regina-core.h"
#include "manifold.h"

namespace regina {

/**
 * Represents a 3-manifold from the SnapPea cusped census.
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
 * Note that this class is closely tied to SnapPeaCensusTri.
 * In particular, the section constants defined in SnapPeaCensusTri and
 * SnapPeaCensusManifold are identical, and so may be freely mixed.
 * Furthermore, the section and index parameters of a SnapPeaCensusTri
 * are identical to those of its corresponding SnapPeaCensusManifold.
 *
 * All of the optional Manifold routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other manifold classes.
 *
 * \ingroup manifold
 */
class SnapPeaCensusManifold : public Manifold {
    public:
        static constexpr char SEC_5 = 'm';
            /**< Represents the collection of manifolds formed from five or
                 fewer tetrahedra (both orientable and non-orientable).
                 There are 415 manifolds in this section. */
        static constexpr char SEC_6_OR = 's';
            /**< Represents the collection of orientable manifolds formed
                 from six tetrahedra.
                 There are 962 manifolds in this section. */
        static constexpr char SEC_6_NOR = 'x';
            /**< Represents the collection of non-orientable manifolds formed
                 from six tetrahedra.
                 There are 259 manifolds in this section. */
        static constexpr char SEC_7_OR = 'v';
            /**< Represents the collection of orientable manifolds formed
                 from seven tetrahedra.
                 There are 3552 manifolds in this section. */
        static constexpr char SEC_7_NOR = 'y';
            /**< Represents the collection of non-orientable manifolds formed
                 from seven tetrahedra.
                 There are 887 manifolds in this section. */

    private:
        char section_;
            /**< The section of the SnapPea census to which this
                 manifold belongs.  This must be one of the section
                 constants defined in this class. */
        unsigned long index_;
            /**< The index within the given section of this specific
                 manifold.  Note that the first index in each section
                 is zero. */

    public:
        /**
         * Creates a new SnapPea census manifold with the given parameters.
         *
         * \param section the section of the SnapPea census to which
         * this manifold belongs.  This must be one of the section
         * constants defined in this class.
         * \param index specifies which particular manifold within the
         * given section is represented.  The indices for each section
         * begin counting at zero, and so this index
         * must be between 0 and <i>k</i>-1, where <i>k</i> is the total
         * number of manifolds in the given section.
         */
        SnapPeaCensusManifold(char section, unsigned long index);
        /**
         * Creates a new copy of the given SnapPea census manifold.
         */
        SnapPeaCensusManifold(const SnapPeaCensusManifold&) = default;
        /**
         * Returns the section of the SnapPea census to which this
         * manifold belongs.  This will be one of the section constants
         * defined in this class.
         *
         * @return the section of the SnapPea census.
         */
        char section() const;
        /**
         * Returns the index of this manifold within its particular
         * section of the SnapPea census.  Note that indices for each
         * section begin counting at zero.
         *
         * @return the index of this manifold within its section.
         */
        unsigned long index() const;
        /**
         * Determines whether this and the given structure represent
         * the same 3-manifold from the SnapPea census.
         *
         * As of Regina 5.0, this test respects the recent discovery that
         * the manifolds \c x101 and \c x103 are homeomorphic.
         * For details, see B.B., <i>A duplicate pair in the SnapPea census</i>,
         * Experimental Mathematics, 23:170-173, 2014.
         *
         * \param compare the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same SnapPea census manifold.
         */
        bool operator == (const SnapPeaCensusManifold& compare) const;
        /**
         * Determines whether this and the given structure represent
         * different 3-manifolds from the SnapPea census.
         *
         * As of Regina 5.0, this test respects the recent discovery that
         * the manifolds \c x101 and \c x103 are homeomorphic.
         * For details, see B.B., <i>A duplicate pair in the SnapPea census</i>,
         * Experimental Mathematics, 23:170-173, 2014.
         *
         * \param compare the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent different SnapPea census manifolds.
         */
        bool operator != (const SnapPeaCensusManifold& compare) const;

        /**
         * Sets this to be a copy of the given SnapPea census manifold.
         *
         * @return a reference to this SnapPea census manifold.
         */
        SnapPeaCensusManifold& operator = (const SnapPeaCensusManifold&) =
            default;

        /**
         * Swaps the contents of this and the given SnapPea census manifold.
         *
         * \param other the census manifold whose contents should be swapped
         * with this.
         */
        void swap(SnapPeaCensusManifold& other) noexcept;

        Triangulation<3> construct() const override;
        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        std::ostream& writeStructure(std::ostream& out) const override;
};

/**
 * Swaps the contents of the two given SnapPea census manifolds.
 *
 * This global routine simply calls SnapPeaCensusManifold::swap(); it is
 * provided so that SnapPeaCensusManifold meets the C++ Swappable requirements.
 *
 * \param a the first census manifold whose contents should be swapped.
 * \param b the second census manifold whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(SnapPeaCensusManifold& a, SnapPeaCensusManifold& b) noexcept;

// Inline functions for SnapPeaCensusManifold

inline SnapPeaCensusManifold::SnapPeaCensusManifold(char section,
        unsigned long index) : section_(section), index_(index) {
}
inline char SnapPeaCensusManifold::section() const {
    return section_;
}
inline unsigned long SnapPeaCensusManifold::index() const {
    return index_;
}
inline bool SnapPeaCensusManifold::operator == (
        const SnapPeaCensusManifold& compare) const {
    if (section_ == SEC_6_NOR && compare.section_ == SEC_6_NOR &&
            (index_ == 101 || index_ == 103) &&
            (compare.index_ == 101 || compare.index_ == 103))
        return true;
    return (section_ == compare.section_ && index_ == compare.index_);
}
inline bool SnapPeaCensusManifold::operator != (
        const SnapPeaCensusManifold& compare) const {
    if (section_ == SEC_6_NOR && compare.section_ == SEC_6_NOR &&
            (index_ == 101 || index_ == 103) &&
            (compare.index_ == 101 || compare.index_ == 103))
        return false;
    return (section_ != compare.section_ || index_ != compare.index_);
}
inline bool SnapPeaCensusManifold::isHyperbolic() const {
    return true;
}

inline void SnapPeaCensusManifold::swap(SnapPeaCensusManifold& other) noexcept {
    std::swap(section_, other.section_);
    std::swap(index_, other.index_);
}

inline void swap(SnapPeaCensusManifold& a, SnapPeaCensusManifold& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

