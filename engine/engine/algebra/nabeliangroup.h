
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nabeliangroup.h
 *  \brief Deals with finitely generated abelian groups.
 */

#ifndef __NABELIANGROUP_H
#ifndef __DOXYGEN
#define __NABELIANGROUP_H
#endif

#include <set>
#include "utilities/nmpi.h"
#include "shareableobject.h"

namespace regina {

class NFile;
class NMatrixInt;

/**
 * \addtogroup algebra Algebraic Structures
 * Various algebraic structures.
 * @{
 */

/**
 * Represents a finitely generated abelian group.
 *
 * The torsion elements of the group are stored in terms of their
 * invariant factors.  For instance, Z_2+Z_3 will appear as
 * Z_6, and Z_2+Z_2+Z_3 will appear as Z_2+Z_6.
 *
 * In general the factors will appear as Z_<i>d0</i>+...+Z_<i>dn</i>,
 * where the invariant factors <i>di</i> are all greater than 1 and satisfy
 * <i>d0</i>|<i>d1</i>|...|<i>dn</i>.  Note that this representation is
 * unique.
 *
 * \todo \optlong Look at using sparse matrices for storage of SNF and
 * the like.
 */
class NAbelianGroup : public ShareableObject {
    protected:
        unsigned rank;
            /**< The rank of the group (the number of Z components). */
        std::multiset<NLargeInteger> invariantFactors;
            /**< The invariant factors <i>d0</i>,...,<i>dn</i> as
             *   described in the NAbelianGroup notes. */
    
    public:
        /**
         * Creates a new trivial group.
         */
        NAbelianGroup();
        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        NAbelianGroup(const NAbelianGroup& cloneMe);
        /**
         * Destroys the group.
         */
        virtual ~NAbelianGroup();
        
        /**
         * Increments the rank of the group by the given non-negative
         * integer.
         *
         * @param extraRank the extra rank to add; this defaults to 1.
         */
        void addRank(unsigned extraRank = 1);
        /**
         * Adds the given torsion element to the group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.  If many different
         * torsion elements are to be added, consider using
         * addTorsionElements() instead so the invariant factors need
         * only be calculated once.
         *
         * In this routine we add a specified number of copies of
         * Z_<i>d</i>, where <i>d</i> is some given degree.
         *
         * \pre The given degree is at least 2 and the
         * given multiplicity is at least 1.
         *
         * @param degree <i>d</i>, where we are adding copies of
         * <i>Z_d</i> to the torsion.
         * @param mult the multiplicity <i>m</i>, where we are adding
         * precisely <i>m</i> copies of <i>Z_d</i>; this defaults to 1.
         */
        void addTorsionElement(const NLargeInteger& degree, unsigned mult = 1);
        /**
         * Adds the given torsion element to the group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.  If many different
         * torsion elements are to be added, consider using
         * addTorsionElements() instead so the invariant factors need
         * only be calculated once.
         *
         * In this routine we add a specified number of copies of
         * Z_<i>d</i>, where <i>d</i> is some given degree.
         *
         * \pre The given degree is at least 2 and the
         * given multiplicity is at least 1.
         *
         * @param degree <i>d</i>, where we are adding copies of
         * <i>Z_d</i> to the torsion.
         * @param mult the multiplicity <i>m</i>, where we are adding
         * precisely <i>m</i> copies of <i>Z_d</i>; this defaults to 1.
         */
        void addTorsionElement(unsigned long degree, unsigned mult = 1);
        /**
         * Adds the given set of torsion elements to this group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.
         *
         * The torsion elements to add are described by a list of
         * integers <i>k1</i>,...,<i>km</i>, where we are adding
         * Z_<i>k1</i>,...,Z_<i>km</i>.  Unlike invariant factors, the
         * <i>ki</i> are not required to divide each other.
         *
         * \pre Each integer in the given list is strictly greater than 1.
         *
         * @param torsion a list containing the torsion elements to add,
         * as described above.
         */
        void addTorsionElements(const std::multiset<NLargeInteger>& torsion);
        /**
         * Adds the abelian group defined by the given presentation to this
         * group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.
         *
         * @param presentation a presentation matrix for the group to be
         * added to this group, where each column represents a generator
         * and each row a relation.
         */
        void addGroup(const NMatrixInt& presentation);
        /**
         * Adds the given abelian group to this group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.
         *
         * @param group the group to add to this one.
         */
        void addGroup(const NAbelianGroup& group);

        /**
         * Returns the rank of the group.
         * This is the number of included copies of <i>Z</i>.
         *
         * @return the rank of the group.
         */
        unsigned getRank() const;
        /**
         * Returns the rank in the group of the torsion term of given degree.
         * If the given degree is <i>d</i>, this routine will return the
         * largest <i>m</i> for which <i>m</i>Z_<i>d</i> is a subgroup
         * of this group.
         *
         * For instance, if this group is Z_6+Z_12, the torsion term of
         * degree 2 has rank 2 (one occurrence in Z_6 and one in Z_12),
         * and the torsion term of degree 4 has rank 1 (one occurrence
         * in Z_12).
         *
         * \pre The given degree is at least 2.
         *
         * @param degree the degree of the torsion term to query.
         * @return the rank in the group of the given torsion term.
         */
        unsigned getTorsionRank(const NLargeInteger& degree) const;
        /**
         * Returns the rank in the group of the torsion term of given degree.
         * If the given degree is <i>d</i>, this routine will return the
         * largest <i>m</i> for which <i>m</i>Z_<i>d</i> is a subgroup
         * of this group.
         *
         * For instance, if this group is Z_6+Z_12, the torsion term of
         * degree 2 has rank 2 (one occurrence in Z_6 and one in Z_12),
         * and the torsion term of degree 4 has rank 1 (one occurrence
         * in Z_12).
         *
         * \pre The given degree is at least 2.
         *
         * @param degree the degree of the torsion term to query.
         * @return the rank in the group of the given torsion term.
         */
        unsigned getTorsionRank(unsigned long degree) const;
        /**
         * Returns the number of invariant factors that describe the
         * torsion elements of this group.
         * See the NAbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        unsigned long getNumberOfInvariantFactors() const;
        /**
         * Returns the given invariant factor describing the torsion
         * elements of this group.
         * See the NAbelianGroup class notes for further details.
         *
         * If the invariant factors are <i>d0</i>|<i>d1</i>|...|<i>dn</i>,
         * this routine will return <i>di</i> where <i>i</i> is the
         * value of parameter \a index.
         *
         * @param index the index of the invariant factor to return;
         * this must be between 0 and getNumberOfInvariantFactors()-1
         * inclusive.
         * @return the requested invariant factor.
         */
        const NLargeInteger& getInvariantFactor(unsigned long index) const;

        /**
         * Writes a chunk of XML containing this abelian group.
         *
         * \ifaces Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;
        /**
         * Writes this abelian group to the given old-style binary file.
         *
         * \deprecated For the preferred way to write data to file, see
         * writeXMLData() instead.
         *
         * \pre The given file is currently opened for writing.
         *
         * \ifaces Not present.
         *
         * @param out the file to which to write.
         */
        void writeToFile(NFile& out) const;
        /**
         * Reads an abelian group from the given old-style binary file.
         * The group will be newly allocated, and responsibility for its
         * destruction lies with the caller of this routine.
         *
         * \deprecated For the preferred way of reading abelian groups
         * from file, see class NXMLAbelianGroupReader instead.
         *
         * \pre The given file is currently opened for reading.
         *
         * \ifaces Not present.
         *
         * @param in the file from which to read.
         * @return a newly allocated group read from the given file, or
         * 0 if problems arose.
         */
        static NAbelianGroup* readFromFile(NFile& in);

        /**
         * The text representation will be of the form
         * <tt>3 Z + 4 Z_2 + Z_120</tt>.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * NAbelianGroup notes.
         */
        virtual void writeTextShort(std::ostream& out) const;

    protected:
        /**
         * Replaces the torsion elements of this group with those
         * in the abelian group represented by the given Smith normal
         * form presentation matrix.  Any zero columns in the matrix
         * will also be added to the rank as additional copies of Z.
         * Note that preexisting torsion elements will be deleted, but
         * preexisting rank will not.
         *
         * \pre The given matrix is in Smith normal
         * form, with the diagonal consisting of a series of positive,
         * non-decreasing integers followed by zeroes.
         *
         * @param matrix a matrix containing the Smith normal form
         * presentation matrix for the new torsion elements,
         * where each column represents a generator
         * and each row a relation.
         */
        void replaceTorsion(const NMatrixInt& matrix);
};

/*@}*/

// Inline functions for NAbelianGroup

inline NAbelianGroup::NAbelianGroup() : rank(0) {
}

inline NAbelianGroup::NAbelianGroup(const NAbelianGroup& g) :
        ShareableObject(), rank(g.rank),
        invariantFactors(g.invariantFactors) {
}

inline NAbelianGroup::~NAbelianGroup() {
}

inline void NAbelianGroup::addRank(unsigned extraRank) {
    rank += extraRank;
}

inline void NAbelianGroup::addTorsionElement(unsigned long degree,
        unsigned mult) {
    addTorsionElement(NLargeInteger(degree), mult);
}

inline unsigned NAbelianGroup::getRank() const {
    return rank;
}

inline unsigned NAbelianGroup::getTorsionRank(unsigned long degree) const {
    return getTorsionRank(NLargeInteger(degree));
}

inline unsigned long NAbelianGroup::getNumberOfInvariantFactors() const {
    return invariantFactors.size();
}

} // namespace regina

#endif

