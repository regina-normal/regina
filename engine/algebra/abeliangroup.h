
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

/*! \file algebra/abeliangroup.h
 *  \brief Deals with finitely generated abelian groups.
 */

#ifndef __REGINA_ABELIANGROUP_H
#ifndef __DOXYGEN
#define __REGINA_ABELIANGROUP_H
#endif

#include <set>
#include <vector>
#include "regina-core.h"
#include "maths/integer.h"
#include "core/output.h"

namespace regina {

template <typename, bool> class Matrix;
using MatrixInt = Matrix<Integer, true>;

/**
 * \defgroup algebra Algebraic Structures
 * Various algebraic structures.
 */

/**
 * Represents a finitely generated abelian group.
 *
 * The torsion elements of the group are stored in terms of their
 * invariant factors.  For instance, Z_2+Z_3 will appear as
 * Z_6, and Z_2+Z_2+Z_3 will appear as Z_2+Z_6.
 *
 * In general the factors will appear as Z_<i>d0</i>+...+Z_<i>dn</i>,
 * where the invariant factors \a di are all greater than 1 and satisfy
 * <i>d0</i>|<i>d1</i>|...|<i>dn</i>.  Note that this representation is
 * unique.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \optlong Look at using sparse matrices for storage of SNF and
 * the like.
 *
 * \ingroup algebra
 */
class AbelianGroup :
        public ShortOutput<AbelianGroup, true>,
        public TightEncodable<AbelianGroup> {
    protected:
        size_t rank_ { 0 };
            /**< The rank of the group (the number of Z components). */
        std::vector<Integer> revInvFactors_;
            /**< The invariant factors \a d0,...,\a dn as
                 described in the AbelianGroup notes.
                 These are stored in reverse order, since addTorsion()
                 always extends the vector on the \a d0 end.
                 Note that we cannot use std::deque (which does support
                 pushing to the front), since older gcc versions do not
                 have a noexcept std::deque move constructor. */

    public:
        /**
         * Creates a new trivial group.
         */
        AbelianGroup() = default;
        /**
         * Creates a clone of the given group.
         */
        AbelianGroup(const AbelianGroup&) = default;
        /**
         * Moves the contents of the given group to this new group.
         * This is a fast (constant time) operation.
         *
         * The group that was passed will no longer be usable.
         */
        AbelianGroup(AbelianGroup&&) noexcept = default;
        /**
         * Creates a free abelian group of the given rank.
         *
         * \param rank the rank of the new group.
         */
        AbelianGroup(size_t rank);
        /**
         * Creates a new group with the given rank and invariant factors.
         *
         * \exception InvalidArgument The invariant factors were not all
         * greater than 1, and/or they did not satisfy the divisibily
         * requirement (where each invariant factor must divide the one
         * after it).
         *
         * \nopython Instead, use the constructor that takes the invariant
         * factors as a Python list.
         *
         * \tparam T an integer type, which may be a native C++ integer
         * type or one of Regina's own integer types.
         *
         * \param rank the rank of the new group (i.e., the number of
         * copies of \a Z).
         * \param invFac the list of invariant factors \a d0, \a d1, ...,
         * as described in the class notes, where each invariant factor
         * is greater than 1 and divides the invariant factor after it.
         */
        template <typename T>
        AbelianGroup(size_t rank, std::initializer_list<T> invFac);
        /**
         * Creates a new group with the given rank and invariant factors.
         *
         * \exception InvalidArgument The invariant factors were not all
         * greater than 1, and/or they did not satisfy the divisibily
         * requirement (where each invariant factor must divide the one
         * after it).
         *
         * \tparam Container a container or view that supports reverse
         * iteration via rbegin(), rend(), that has an empty() function,
         * and whose elements may be of a native C++ integer type or one of
         * Regina's own integer types.  A suitable example might be
         * std::vector<int>.
         *
         * \param rank the rank of the new group (i.e., the number of
         * copies of \a Z).
         * \param invFac the list of invariant factors \a d0, \a d1, ...,
         * as described in the class notes, where each invariant factor
         * is greater than 1 and divides the invariant factor after it.
         */
        template <typename Container>
        AbelianGroup(size_t rank, const Container& invFac);
        /**
         * Creates the abelian group defined by the given presentation matrix.
         *
         * Each column of the matrix represents a generator, and each
         * row of the matrix represents a relation.
         *
         * \param presentation a presentation matrix for the new group.
         */
        AbelianGroup(MatrixInt presentation);
        /**
         * Creates an abelian group as the homology of a chain complex.
         * The abelian group is the kernel of \a M modulo the image of \a N.
         *
         * The matrices should be thought of as acting on column vectors:
         * this means that the product `B×A` applies the linear
         * transformation \a A, then the linear transformation \a B.
         * This is consistent (for example) with the convention that
         * Regina uses for for multiplying permutations.
         *
         * \pre M.columns() = N.rows().  This condition will be tested,
         * and an exception will be thrown if it does not hold.
         *
         * \pre The product M×N = 0.  This condition will _not_ be tested
         * (for efficiency reasons); this is left to the user/programmer
         * to ensure.
         *
         * \exception InvalidArgument The number of columns in \a M does
         * not match the number of rows in \a N.
         *
         * \param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * \param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         */
        AbelianGroup(MatrixInt M, MatrixInt N);
        /**
         * Creates an abelian group as the homology of a chain complex,
         * using mod-\a p coefficients.
         * The abelian group is the kernel of \a M modulo the image of \a N.
         *
         * The matrices should be thought of as acting on column vectors:
         * this means that the product `B×A` applies the linear
         * transformation \a A, then the linear transformation \a B.
         * This is consistent (for example) with the convention that
         * Regina uses for for multiplying permutations.
         *
         * \pre M.columns() = N.rows().  This condition will be tested,
         * and an exception will be thrown if it does not hold.
         *
         * \pre The product M×N = 0.  This condition will _not_ be tested
         * (for efficiency reasons); this is left to the user/programmer
         * to ensure.
         *
         * \exception InvalidArgument The number of columns in \a M does
         * not match the number of rows in \a N.
         *
         * \param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * \param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         * \param p the modulus, which may be any Integer.
         * Zero is interpreted as a request for integer coefficents,
         * which will give the same result as the
         * AbelianGroup(MatrixInt, MatrixInt) constructor.
         *
         * \author Ryan Budney
         */
        AbelianGroup(MatrixInt M, MatrixInt N, const Integer &p);
        /**
         * Increments the rank of the group by the given integer.
         * This integer may be positive, negative or zero.
         *
         * \pre The current rank plus the given integer is non-negative.
         * In other words, if we are subtracting rank then we are not
         * trying to subtract more rank than the group actually has.
         *
         * \param extraRank the extra rank to add; this defaults to 1.
         */
        void addRank(long extraRank = 1);
        /**
         * Adds the given torsion element to the group.
         *
         * As of Regina 7.0, this routine is much faster than it used to be.
         * In particular, if you have many torsion elements to add, it is now
         * efficient just to call addTorsion() for each new torsion element,
         * one at a time.
         *
         * In this routine we add a single copy of Z_<i>d</i>, where \a d is
         * the given degree.
         *
         * \param degree the degree of the new torsion element; this
         * must be strictly positive.
         */
        void addTorsion(Integer degree);
        /**
         * Adds the abelian group defined by the given presentation to this
         * group.  Note that this routine might be slow since calculating the
         * new invariant factors is not trivial.
         *
         * \param presentation a presentation matrix for the group to be
         * added to this group, where each column represents a generator
         * and each row a relation.
         */
        void addGroup(MatrixInt presentation);
        /**
         * Adds the given abelian group to this group.
         *
         * \param group the group to add to this one.
         */
        void addGroup(const AbelianGroup& group);

        /**
         * Returns the rank of the group.
         * This is the number of included copies of \a Z.
         *
         * Equivalently, the rank is the maximum number of linearly independent
         * elements, and it indicates the size of the largest free abelian
         * subgroup.  The rank effectively ignores all torsion elements.
         *
         * \warning SnapPy users should be aware that SnapPy defines
         * rank differently. Specifically, SnapPy's AbelianGroup.rank()
         * computation includes torsion factors also.
         *
         * \return the number of included copies of \a Z.
         */
        size_t rank() const;
        /**
         * Returns the rank in the group of the torsion term of given degree.
         * If the given degree is \a d, this routine will return the
         * largest \a m for which <i>m</i>Z_<i>d</i> is a subgroup
         * of this group.
         *
         * For instance, if this group is Z_6+Z_12, the torsion term of
         * degree 2 has rank 2 (one occurrence in Z_6 and one in Z_12),
         * and the torsion term of degree 4 has rank 1 (one occurrence
         * in Z_12).
         *
         * \pre The given degree is at least 2.
         *
         * \param degree the degree of the torsion term to query.
         * \return the rank in the group of the given torsion term.
         */
        size_t torsionRank(const Integer& degree) const;
        /**
         * Returns the rank in the group of the torsion term of given degree.
         * If the given degree is \a d, this routine will return the
         * largest \a m for which <i>m</i>Z_<i>d</i> is a subgroup
         * of this group.
         *
         * For instance, if this group is Z_6+Z_12, the torsion term of
         * degree 2 has rank 2 (one occurrence in Z_6 and one in Z_12),
         * and the torsion term of degree 4 has rank 1 (one occurrence
         * in Z_12).
         *
         * \pre The given degree is at least 2.
         *
         * \param degree the degree of the torsion term to query.
         * \return the rank in the group of the given torsion term.
         */
        size_t torsionRank(unsigned long degree) const;
        /**
         * Returns the number of invariant factors that describe the
         * torsion elements of this group.
         * See the AbelianGroup class notes for further details.
         *
         * \return the number of invariant factors.
         */
        size_t countInvariantFactors() const;
        /**
         * Returns the given invariant factor describing the torsion
         * elements of this group.
         * See the AbelianGroup class notes for further details.
         *
         * If the invariant factors are <i>d0</i>|<i>d1</i>|...|<i>dn</i>,
         * this routine will return \a di where \a i is the
         * value of parameter \a index.
         *
         * \param index the index of the invariant factor to return;
         * this must be between 0 and countInvariantFactors()-1 inclusive.
         * \return the requested invariant factor.
         */
        const Integer& invariantFactor(size_t index) const;

        /**
         * Determines whether this is the trivial (zero) group.
         *
         * \return \c true if and only if this is the trivial group.
         */
        bool isTrivial() const;
        /**
         * Determines whether this is the infinite cyclic group (Z).
         *
         * \return \c true if and only if this is the infinite cyclic group.
         */
        bool isZ() const;
        /**
         * Determines whether this is the free abelian group of the given rank.
         *
         * \param r the rank of the free abelian group that we are testing for.
         * \return \c true if and only if this is the free abelian group
         * of rank \a r.
         */
        bool isFree(size_t r) const;
        /**
         * Determines whether this is the non-trivial cyclic group on
         * the given number of elements.
         *
         * As a special case, if \a n = 0 then this routine will test for the
         * infinite cyclic group (i.e., it will behave the same as isZ()).
         * If \a n = 1, then this routine will test for the trivial group
         * (i.e., it will behave the same as isTrivial()).
         *
         * \param n the number of elements of the cyclic group in question.
         * \return \c true if and only if this is the cyclic group Z_n.
         */
        bool isZn(size_t n) const;
        /**
         * Determines whether this and the given abelian group have
         * identical presentations (which means they are isomorphic).
         *
         * Since the AbelianGroup class stores _only_ the invariants required
         * to identify the isomorphism type, two groups will compare as equal
         * if and only if they are isomorphic.  This is in contrast to the
         * comparisons for GroupPresentation (which tests for identical
         * generators and relations), or for MarkedAbelianGroup (which tests
         * for identical chain complex presentations).
         *
         * \param other the group with which this should be compared.
         * \return \c true if and only if the two groups have identical
         * presentations (i.e., they are isomorphic).
         */
        bool operator == (const AbelianGroup& other) const;

        /**
         * Sets this to be a clone of the given group.
         *
         * \return a reference to this group.
         */
        AbelianGroup& operator = (const AbelianGroup&) = default;

        /**
         * Moves the contents of the given group to this group.
         * This is a fast (constant time) operation.
         *
         * The group that was passed will no longer be usable.
         *
         * \return a reference to this group.
         */
        AbelianGroup& operator = (AbelianGroup&&) noexcept = default;

        /**
         * Swaps the contents of this and the given abelian group.
         *
         * \param other the group whose contents should be swapped with this.
         */
        void swap(AbelianGroup& other) noexcept;

        /**
         * Writes the tight encoding of this abelian group to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Reconstructs an abelian group from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of an abelian group.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for an abelian group.
         * \return the abelian group represented by the given tight encoding.
         */
        static AbelianGroup tightDecode(std::istream& input);

        /**
         * Writes a chunk of XML containing this abelian group.
         *
         * \python The argument \a out should be an open Python file
         * object.
         *
         * \param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * The text representation will be of the form `3 Z + 4 Z_2 + Z_120`.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * AbelianGroup notes.
         *
         * \nopython Use str() or utf8() instead.
         *
         * \param out the output stream to which to write.
         * \param utf8 if \c true, then richer unicode characters will
         * be used to make the output more pleasant to read.  In particular,
         * the output will use subscript digits and the blackboard bold Z.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false) const;
};

/**
 * Swaps the contents of the two given abelian groups.
 *
 * This global routine simply calls AbelianGroup::swap(); it is provided
 * so that AbelianGroup meets the C++ Swappable requirements.
 *
 * \param lhs the group whose contents should be swapped with \a rhs.
 * \param rhs the group whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(AbelianGroup& lhs, AbelianGroup& rhs) noexcept;

// Inline functions for AbelianGroup

inline AbelianGroup::AbelianGroup(size_t rank) : rank_(rank) {
}

template <typename T>
inline AbelianGroup::AbelianGroup(size_t rank,
        std::initializer_list<T> invFac) : rank_(rank) {
    if (invFac.size() > 0) {
        auto it = std::rbegin(invFac);
        while (true) {
            if (*it <= 1)
                throw InvalidArgument(
                    "Each invariant factor must be strictly greater than 1");
            revInvFactors_.push_back(*it);
            auto prev = it++;
            if (it == std::rend(invFac))
                return;
            if ((*prev) % (*it) != 0)
                throw InvalidArgument(
                    "Each invariant factor must divide the next");
        }
    }
}

template <typename Container>
inline AbelianGroup::AbelianGroup(size_t rank, const Container& invFac) :
        rank_(rank) {
    if (! invFac.empty()) {
        auto it = invFac.rbegin();
        while (true) {
            if (*it <= 1)
                throw InvalidArgument(
                    "Each invariant factor must be strictly greater than 1");
            revInvFactors_.push_back(*it);
            auto prev = it++;
            if (it == invFac.rend())
                return;
            if ((*prev) % (*it) != 0)
                throw InvalidArgument(
                    "Each invariant factor must divide the next");
        }
    }
}

inline void AbelianGroup::swap(AbelianGroup& other) noexcept {
    std::swap(rank_, other.rank_);
    revInvFactors_.swap(other.revInvFactors_);
}

inline void AbelianGroup::addRank(long extraRank) {
    rank_ += extraRank;
}

inline size_t AbelianGroup::rank() const {
    return rank_;
}

inline size_t AbelianGroup::torsionRank(unsigned long degree) const {
    return torsionRank(Integer(degree));
}

inline size_t AbelianGroup::countInvariantFactors() const {
    return revInvFactors_.size();
}

inline const Integer& AbelianGroup::invariantFactor(size_t index) const {
    return revInvFactors_[revInvFactors_.size() - index - 1];
}

inline bool AbelianGroup::isTrivial() const {
    return (rank_ == 0 && revInvFactors_.empty());
}

inline bool AbelianGroup::isZ() const {
    return (rank_ == 1 && revInvFactors_.empty());
}

inline bool AbelianGroup::isFree(size_t r) const {
    return (rank_ == r && revInvFactors_.empty());
}

inline bool AbelianGroup::isZn(size_t n) const {
    return (n == 0 ? isZ() : n == 1 ? isTrivial() :
        (rank_ == 0 && revInvFactors_.size() == 1 &&
            revInvFactors_.front() == n));
}

inline bool AbelianGroup::operator == (const AbelianGroup& other) const {
    return (rank_ == other.rank_ && revInvFactors_ == other.revInvFactors_);
}

inline void AbelianGroup::tightEncode(std::ostream& out) const {
    regina::tightEncode(out, rank_);
    for (auto it = revInvFactors_.rbegin(); it != revInvFactors_.rend(); ++it)
        regina::tightEncode(out, *it);
    regina::tightEncode(out, 0);
}

inline void swap(AbelianGroup& a, AbelianGroup& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

