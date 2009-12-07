
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

/*! \file manifold/nsnappeacensusmfd.h
 *  \brief Deals with 3-manifolds from the SnapPea census.
 */

#ifndef __NSNAPPEACENSUSMFD_H
#ifndef __DOXYGEN
#define __NSNAPPEACENSUSMFD_H
#endif

#include "nmanifold.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a 3-manifold from the SnapPea cusped census.
 *
 * The SnapPea cusped census is the census of cusped hyperbolic 3-manifolds
 * formed from up to seven tetrahedra.  This census was tabulated by
 * Callahan, Hildebrand and Weeks, and is shipped with SnapPea 3.0d3
 * (and also with Regina).
 *
 * The census is split into five different sections according to number
 * of tetrahedra and orientability.  Each of these sections corresponds
 * to one of the section constants defined in this class.
 *
 * For further details regarding the SnapPea census, see "A census of cusped
 * hyperbolic 3-manifolds", Patrick J. Callahan, Martin V. Hildebrand and
 * Jeffrey R. Weeks, Math. Comp. 68 (1999), no. 225, pp. 321--332.
 *
 * Note that this class is closely tied to NSnapPeaCensusTri.
 * In particular, the section constants defined in NSnapPeaCensusTri and
 * NSnapPeaCensusManifold are identical, and so may be freely mixed.
 * Furthermore, the section and index parameters of an NSnapPeaCensusTri
 * are identical to those of its corresponding NSnapPeaCensusManifold.
 *
 * All of the optional NManifold routines are implemented for this class.
 *
 * \testpart
 */
class NSnapPeaCensusManifold : public NManifold {
    public:
        static const char SEC_5;
            /**< Represents the collection of manifolds formed from five or
                 fewer tetrahedra (both orientable and non-orientable).
                 There are 415 manifolds in this section. */
        static const char SEC_6_OR;
            /**< Represents the collection of orientable manifolds formed
                 from six tetrahedra.
                 There are 962 manifolds in this section. */
        static const char SEC_6_NOR;
            /**< Represents the collection of non-orientable manifolds formed
                 from six tetrahedra.
                 There are 259 manifolds in this section. */
        static const char SEC_7_OR;
            /**< Represents the collection of orientable manifolds formed
                 from seven tetrahedra.
                 There are 3552 manifolds in this section. */
        static const char SEC_7_NOR;
            /**< Represents the collection of non-orientable manifolds formed
                 from seven tetrahedra.
                 There are 887 manifolds in this section. */

    private:
        char section;
            /**< The section of the SnapPea census to which this
                 manifold belongs.  This must be one of the section
                 constants defined in this class. */
        unsigned long index;
            /**< The index within the given section of this specific
                 manifold.  Note that the first index in each section
                 is zero. */

    public:
        /**
         * Creates a new SnapPea census manifold with the given parameters.
         *
         * @param newSection the section of the SnapPea census to which
         * this manifold belongs.  This must be one of the section
         * constants defined in this class.
         * @param newIndex specifies which particular manifold within the
         * given section is represented.  The indices for each section
         * begin counting at zero, and so this index
         * must be between 0 and <i>k</i>-1, where <i>k</i> is the total
         * number of manifolds in the given section.
         */
        NSnapPeaCensusManifold(char newSection, unsigned long newIndex);
        /**
         * Creates a clone of the given SnapPea census manifold.
         *
         * @param cloneMe the census manifold to clone.
         */
        NSnapPeaCensusManifold(const NSnapPeaCensusManifold& cloneMe);
        /**
         * Destroys this structure.
         */
        virtual ~NSnapPeaCensusManifold();
        /**
         * Returns the section of the SnapPea census to which this
         * manifold belongs.  This will be one of the section constants
         * defined in this class.
         *
         * @return the section of the SnapPea census.
         */
        char getSection() const;
        /**
         * Returns the index of this manifold within its particular
         * section of the SnapPea census.  Note that indices for each
         * section begin counting at zero.
         *
         * @return the index of this manifold within its section.
         */
        unsigned long getIndex() const;
        /**
         * Determines whether this and the given structure represent
         * the same 3-manifold from the SnapPea census.
         *
         * @param compare the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same SnapPea census manifold.
         */
        bool operator == (const NSnapPeaCensusManifold& compare) const;

        NTriangulation* construct() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        std::ostream& writeStructure(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSnapPeaCensusManifold

inline NSnapPeaCensusManifold::NSnapPeaCensusManifold(char newSection,
        unsigned long newIndex) :
        section(newSection), index(newIndex) {
}
inline NSnapPeaCensusManifold::NSnapPeaCensusManifold(
        const NSnapPeaCensusManifold& cloneMe) : NManifold(),
        section(cloneMe.section), index(cloneMe.index) {
}
inline NSnapPeaCensusManifold::~NSnapPeaCensusManifold() {
}
inline char NSnapPeaCensusManifold::getSection() const {
    return section;
}
inline unsigned long NSnapPeaCensusManifold::getIndex() const {
    return index;
}
inline bool NSnapPeaCensusManifold::operator == (
        const NSnapPeaCensusManifold& compare) const {
    return (section == compare.section && index == compare.index);
}

} // namespace regina

#endif

