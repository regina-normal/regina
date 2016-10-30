
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

/*! \file algebra/abeliangroup.h
 *  \brief Deals with finitely generated abelian groups.
 */

#ifndef __ABELIANGROUP_H
#ifndef __DOXYGEN
#define __ABELIANGROUP_H
#endif

#include <set>
#include "regina-core.h"
#include "maths/integer.h"
#include "output.h"
#include <boost/noncopyable.hpp>

namespace regina {

template <typename> class MatrixIntDomain;
typedef MatrixIntDomain<Integer> MatrixInt;

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
class REGINA_API AbelianGroup :
        public ShortOutput<AbelianGroup, true>,
        public boost::noncopyable {
    protected:
        unsigned rank_;
            /**< The rank of the group (the number of Z components). */
        std::multiset<Integer> invariantFactors;
            /**< The invariant factors <i>d0</i>,...,<i>dn</i> as
             *   described in the AbelianGroup notes. */

    public:
        /**
         * Creates a new trivial group.
         */
        AbelianGroup();
        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        AbelianGroup(const AbelianGroup& cloneMe);
        /**
         * Creates an abelian group as the homology of a chain complex.
         *
         * \pre M.columns() = N.rows().
         * \pre The product M*N = 0.
         *
         * @param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * @param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         *
         * @author Ryan Budney
         */
        AbelianGroup(const MatrixInt& M, const MatrixInt& N);
        /**
         * Creates an abelian group as the homology of a chain complex,
         * using mod-\a p coefficients.
         *
         * \pre M.columns() = N.rows().
         * \pre The product M*N = 0.
         *
         * @param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * @param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         * @param p the modulus, which may be any Integer.
         * Zero is interpreted as a request for integer coefficents,
         * which will give the same result as the
         * AbelianGroup(const MatrixInt&, const MatrixInt&) constructor.
         *
         * @author Ryan Budney
         */
        AbelianGroup(const MatrixInt& M, const MatrixInt& N,
            const Integer &p);

        /**
         * Increments the rank of the group by the given integer.
         * This integer may be positive, negative or zero.
         *
         * \pre The current rank plus the given integer is non-negative.
         * In other words, if we are subtracting rank then we are not
         * trying to subtract more rank than the group actually has.
         *
         * @param extraRank the extra rank to add; this defaults to 1.
         */
        void addRank(int extraRank = 1);
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
        void addTorsionElement(const Integer& degree, unsigned mult = 1);
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
         * \ifacespython This routine takes a python list as its argument.
         *
         * @param torsion a list containing the torsion elements to add,
         * as described above.
         */
        void addTorsionElements(const std::multiset<Integer>& torsion);
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
        void addGroup(const MatrixInt& presentation);
        /**
         * Adds the given abelian group to this group.
         * Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.
         *
         * @param group the group to add to this one.
         */
        void addGroup(const AbelianGroup& group);

        /**
         * Returns the rank of the group.
         * This is the number of included copies of <i>Z</i>.
         *
         * @return the rank of the group.
         */
        unsigned rank() const;
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
        unsigned torsionRank(const Integer& degree) const;
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
        unsigned torsionRank(unsigned long degree) const;
        /**
         * Returns the number of invariant factors that describe the
         * torsion elements of this group.
         * See the AbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        size_t countInvariantFactors() const;
        /**
         * Returns the given invariant factor describing the torsion
         * elements of this group.
         * See the AbelianGroup class notes for further details.
         *
         * If the invariant factors are <i>d0</i>|<i>d1</i>|...|<i>dn</i>,
         * this routine will return <i>di</i> where <i>i</i> is the
         * value of parameter \a index.
         *
         * @param index the index of the invariant factor to return;
         * this must be between 0 and countInvariantFactors()-1 inclusive.
         * @return the requested invariant factor.
         */
        const Integer& invariantFactor(size_t index) const;

        /**
         * Determines whether this is the trivial (zero) group.
         *
         * @return \c true if and only if this is the trivial group.
         */
        bool isTrivial() const;
        /**
         * Determines whether this is the infinite cyclic group (Z).
         *
         * @return \c true if and only if this is the infinite cyclic group.
         */
        bool isZ() const;
        /**
         * Determines whether this is the non-trivial cyclic group on
         * the given number of elements.
         *
         * As a special case, if \a n = 0 then this routine will test for the
         * infinite cyclic group (i.e., it will behave the same as isZ()).
         * If \a n = 1, then this routine will test for the trivial group
         * (i.e., it will behave the same as isTrivial()).
         *
         * @param n the number of elements of the cyclic group in question.
         * @return \c true if and only if this is the cyclic group Z_n.
         */
        bool isZn(unsigned long n) const;
        /**
         * Determines whether this and the given abelian group are
         * isomorphic.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are isomorphic.
         */
        bool operator == (const AbelianGroup& other) const;
        /**
         * Determines whether this and the given abelian group are
         * non-isomorphic.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are non-isomorphic.
         */
        bool operator != (const AbelianGroup& other) const;

        /**
         * Writes a chunk of XML containing this abelian group.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * The text representation will be of the form
         * <tt>3 Z + 4 Z_2 + Z_120</tt>.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * AbelianGroup notes.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         * @param utf8 if \c true, then richer unicode characters will
         * be used to make the output more pleasant to read.  In particular,
         * the output will use subscript digits and the blackboard bold Z.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false) const;

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
        void replaceTorsion(const MatrixInt& matrix);
};


/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NAbelianGroup has now been renamed to
 * AbelianGroup.
 */
REGINA_DEPRECATED typedef AbelianGroup NAbelianGroup;

/*@}*/

// Inline functions for AbelianGroup

inline AbelianGroup::AbelianGroup() : rank_(0) {
}

inline AbelianGroup::AbelianGroup(const AbelianGroup& g) :
        rank_(g.rank_), invariantFactors(g.invariantFactors) {
}

inline void AbelianGroup::addRank(int extraRank) {
    rank_ += extraRank;
}

inline void AbelianGroup::addTorsionElement(unsigned long degree,
        unsigned mult) {
    addTorsionElement(Integer(degree), mult);
}

inline unsigned AbelianGroup::rank() const {
    return rank_;
}

inline unsigned AbelianGroup::torsionRank(unsigned long degree) const {
    return torsionRank(Integer(degree));
}

inline size_t AbelianGroup::countInvariantFactors() const {
    return invariantFactors.size();
}

inline bool AbelianGroup::isTrivial() const {
    return (rank_ == 0 && invariantFactors.empty());
}

inline bool AbelianGroup::isZ() const {
    return (rank_ == 1 && invariantFactors.empty());
}

inline bool AbelianGroup::isZn(unsigned long n) const {
    return (n == 0 ? isZ() : n == 1 ? isTrivial() :
        (rank_ == 0 && invariantFactors.size() == 1 &&
            *invariantFactors.begin() == n));
}

inline bool AbelianGroup::operator == (const AbelianGroup& other) const {
    return (rank_ == other.rank_ && invariantFactors == other.invariantFactors);
}

inline bool AbelianGroup::operator != (const AbelianGroup& other) const {
    return (rank_ != other.rank_ || invariantFactors != other.invariantFactors);
}

} // namespace regina

#endif

