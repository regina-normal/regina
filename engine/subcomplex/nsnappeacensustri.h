
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

/*! \file subcomplex/nsnappeacensustri.h
 *  \brief Deals with 3-manifold triangulations from the SnapPea census.
 */

#ifndef __NSNAPPEACENSUSTRI_H
#ifndef __DOXYGEN
#define __NSNAPPEACENSUSTRI_H
#endif

#include "nstandardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a 3-manifold triangulation from the SnapPea cusped census.
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
 * Note that this class is closely tied to NSnapPeaCensusManifold.
 * In particular, the section constants defined in NSnapPeaCensusManifold
 * and NSnapPeaCensusTri are identical, and so may be freely mixed.
 * Furthermore, the section and index parameters of an NSnapPeaCensusTri
 * are identical to those of its corresponding NSnapPeaCensusManifold.
 *
 * All of the optional NStandardTriangulation routines are implemented
 * for this class, though getHomologyH1() only returns a solution for
 * the very smallest SnapPea triangulations (for which the homology
 * groups have been hard-coded).
 *
 * \testpart
 */
class NSnapPeaCensusTri: public NStandardTriangulation {
    public:
        static const char SEC_5;
            /**< Represents the collection of triangulations formed from five
                 or fewer tetrahedra (both orientable and non-orientable).
                 There are 415 triangulations in this section. */
        static const char SEC_6_OR;
            /**< Represents the collection of orientable triangulations formed
                 from six tetrahedra.
                 There are 962 triangulations in this section. */
        static const char SEC_6_NOR;
            /**< Represents the collection of non-orientable triangulations
                 formed from six tetrahedra.
                 There are 259 triangulations in this section. */
        static const char SEC_7_OR;
            /**< Represents the collection of orientable triangulations formed
                 from seven tetrahedra.
                 There are 3552 triangulations in this section. */
        static const char SEC_7_NOR;
            /**< Represents the collection of non-orientable triangulations
                 formed from seven tetrahedra.
                 There are 887 triangulations in this section. */

    private:
        char section;
            /**< The section of the SnapPea census to which this
                 triangulation belongs.  This must be one of the section
                 constants defined in this class. */
        unsigned long index;
            /**< The index within the given section of this specific
                 triangulation.  Note that the first index in each section
                 is zero. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NSnapPeaCensusTri* clone() const;

        /**
         * Returns the section of the SnapPea census to which this
         * triangulation belongs.  This will be one of the section constants
         * defined in this class.
         *
         * @return the section of the SnapPea census.
         */
        char getSection() const;

        /**
         * Returns the index of this triangulation within its particular
         * section of the SnapPea census.  Note that indices for each
         * section begin counting at zero.
         *
         * @return the index of this triangulation within its section.
         */
        unsigned long getIndex() const;

        /**
         * Determines whether this and the given structure represent
         * the same triangulation from the SnapPea census.
         *
         * @param compare the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same SnapPea census triangulation.
         */
        bool operator == (const NSnapPeaCensusTri& compare) const;

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
         * @param comp the triangulation component to examine.
         * @return a newly created structure representing the small
         * SnapPea census triangulation, or \c null if the given
         * component is not one of the few SnapPea census
         * triangulations recognised by this routine.
         */
        static NSnapPeaCensusTri* isSmallSnapPeaCensusTri(
            const NComponent* comp);

        NManifold* getManifold() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Creates a new SnapPea census triangulation with the given
         * parameters.
         */
        NSnapPeaCensusTri(char newSection, unsigned long newIndex);

    friend class NSnapPeaCensusManifold;
};

/*@}*/

// Inline functions for NSnapPeaCensusTri

inline NSnapPeaCensusTri* NSnapPeaCensusTri::clone() const {
    return new NSnapPeaCensusTri(section, index);
}

inline char NSnapPeaCensusTri::getSection() const {
    return section;
}

inline unsigned long NSnapPeaCensusTri::getIndex() const {
    return index;
}

inline bool NSnapPeaCensusTri::operator == (const NSnapPeaCensusTri& compare)
        const {
    return (section == compare.section && index == compare.index);
}

inline NSnapPeaCensusTri::NSnapPeaCensusTri(char newSection,
        unsigned long newIndex) :
        section(newSection), index(newIndex) {
}

} // namespace regina

#endif

