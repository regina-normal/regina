
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

/*! \file algebra/markedabeliangroup.h
 *  \brief Deals with abelian groups given by chain complexes.
 */

#ifndef __REGINA_MARKEDABELIANGROUP_H
#ifndef __DOXYGEN
#define __REGINA_MARKEDABELIANGROUP_H
#endif

#include <optional>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "algebra/abeliangroup.h"
#include "maths/matrix.h"
#include "maths/vector.h"

namespace regina {

class HomMarkedAbelianGroup;

/**
 * Represents a finitely generated abelian group given by a chain complex.
 *
 * This class is initialized with a chain complex.  The chain complex is given
 * in terms of two integer matrices \a M and \a N such that M×N=0.  These
 * matrices should be thought of as acting on column vectors: this means for
 * example that the product `B×A` applies the linear transformation
 * \a A, then the linear transformation \a B.  This is consistent with the
 * convention that Regina uses for for multiplying permutations.
 *
 * The abelian group that this class computes is the kernel of \a M modulo the
 * image of \a N.  In other words, we compute the homology of the chain complex
 * `Z^a --N--> Z^b --M--> Z^c`,
 * where \a a = N.columns(), \a b = M.columns() = N.rows(), and \a c = M.rows().
 * An additional constructor allows you to take the homology with coefficients
 * in an arbitrary cyclic group.
 *
 * Like the simpler class AbelianGroup, this group will be isomorphic to some
 * `Z_{d0} + ... + Z_{dk} + Z^r`, where:
 *
 * - \a r is the number of free generators, as returned by rank();
 *
 * - \a d1, ..., \a dk are the _invariant factors_ that describe the
 *   torsion elements of the group, where 1 < \a d1 | \a d2 | ... | \a dk.
 *
 * This class allows you to retrieve the invariant factors, the rank, and
 * the corresponding vectors in the kernel of \a M.  Moreover, given a
 * vector in the kernel of \a M, it decribes the homology class of the
 * vector (the free part, and its position in the invariant factors).
 *
 * The purpose of this class is to not just represent homology groups, but
 * also to support coordinates on the group allowing for the construction of
 * homomorphisms, and keeping track of subgroups.
 *
 * This routine makes frequent use of two coordinate systems:
 *
 * - _Chain complex coordinates_ describe vectors of length \a b
 *   (using the notation above); that is, elements of the domain of \a M,
 *   or equivalently the codomain of \a N.
 *
 * - _SNF (or Smith normal form) coordinates_ describe elements of this
 *   abelian group in terms of the torsion and free generators.  A vector in
 *   SNF coordinates has length (\a k + \a r), again using the notation above,
 *   where the first \a k elements store coefficients for the generators of the
 *   torsion components Z_{d1}, ..., Z_{dk}, and the remaining \a r elements
 *   store coefficients for the \a r free generators.  You can see how the
 *   torsion and free generators appear in chain complex coordinates by
 *   calling torsionRep() or freeRep() respectively.
 *
 * Be aware that the choice of torsion and free generators is typically
 * not unique, and this will affect the results of many member functions of
 * this class.  These choices are subject to change between different versions
 * of Regina; in particular, they depend upon the particular algorithm
 * used for computing Smith normal forms.
 *
 * Some routines in this class refer to the _internal presentation
 * matrix_.  This is a proper presentation matrix for the abelian group;
 * if you are looking at the implementation details, this refers to the
 * matrix \a pres, created by taking the product `MRi_ * N` and then
 * removing the first \a rankM_ rows.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \author Ryan Budney and B.B.
 *
 * \todo \optlong Look at using sparse matrices for storage of SNF and the like.
 *
 * \ingroup algebra
 */
class MarkedAbelianGroup : public ShortOutput<MarkedAbelianGroup, true> {
    private:
        /** Internal original M */
        MatrixInt M_; // copy of initializing M
        /** Internal original N */
        MatrixInt N_; // copy of initializing N assumes M×N == 0
        /** Internal change of basis. SNF(M_) == MC×M_×MR_ */
        MatrixInt MR_;
        /** Internal change of basis. M_ = MCi×SNF(M_)×MRi_ */
        MatrixInt MRi_;
        /** Internal rank of M */
        size_t rankM_; // this is the index of the first zero entry in SNF(M_)

        /* Internal reduced N matrix: */
        // In the notes below, pres refers to the internal presentation
        // matrix [MRi_ * N_], where the brackets indicate removal of the
        // first rankM_ rows.

        /** Internal change of basis. presC_ * pres * presR_ is the SNF(pres). */
        MatrixInt presR_, presC_;
        /** Internal change of basis. This is the inverse of presC_. */
        MatrixInt presCi_;
        /** Internal change of basis matrix for homology with coefficents.
            tensorC_ * tensorPres * tensorR_ == SNF(tensorPres) */
        MatrixInt tensorR_, tensorC_, tensorCi_;

        /** Internal list of invariant factors. */
        std::vector<Integer> invFac_;
        /** The number of free generators, from SNF(pres) */
        size_t snfFreeRank_;
        /** The row index of the first zero along the diagonal in SNF(pres). */
        size_t snfFreeIndex_;
        /** Row index of first invariant factor (ie entry > 1) in SNF(pres) */
        size_t ifLoc_;

        // These variables store information for mod-p homology computations.
        /** coefficients to use in homology computation **/
        Integer coeff_;
        /** The i-th entry of TORVec_ stores the entries q where Z_p --q-->Z_p
            is the i-th TOR entry from the SNF of M_. */
        std::vector<Integer> TORVec_;

        /** invariant factor data in the tensor product presentation matrix
            SNF */
        size_t tensorIfLoc_;
        std::vector<Integer> tensorInvFac_;

    public:
        /**
         * Creates a marked abelian group from a chain complex. This constructor
         * assumes you are interested in homology with integer coefficents.
         *
         * The abelian group is the kernel of \a M modulo the image of \a N.
         * See the class notes for further details.
         *
         * \pre M.columns() = N.rows().  This condition will be tested,
         * and an exception will be thrown if it does not hold.
         *
         * \pre The product M×N = 0.  This condition will \e not be tested
         * (for efficiency reasons); this is left to the user/programmer
         * to ensure.
         *
         * \exception InvalidArgument The number of columns in \a M does
         * not match the number of rows in \a N.
         *
         * @param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * @param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         */
        MarkedAbelianGroup(MatrixInt M, MatrixInt N);

        /**
         * Creates a marked abelian group from a chain complex with
         * coefficients in Z_p.
         *
         * The abelian group is the kernel of \a M modulo the image of \a N.
         * See the class notes for further details.
         *
         * \pre M.columns() = N.rows().  This condition will be tested,
         * and an exception will be thrown if it does not hold.
         *
         * \pre The product M×N = 0.  This condition will \e not be tested
         * (for efficiency reasons); this is left to the user/programmer
         * to ensure.
         *
         * \exception InvalidArgument The number of columns in \a M does
         * not match the number of rows in \a N.
         *
         * @param M the `right' matrix in the chain complex; that is,
         * the matrix that one takes the kernel of when computing homology.
         * @param N the `left' matrix in the chain complex; that is, the
         * matrix that one takes the image of when computing homology.
         * @param pcoeff specifies the coefficient ring, Z_pcoeff.
         * This must be non-negative; a value of 0 indicates that you
         * are using integer coefficients (in which case it is more
         * efficient to use the constructor that just takes two matrices).
         */
        MarkedAbelianGroup(MatrixInt M, MatrixInt N, Integer pcoeff);

        /**
         * Creates a free `Z_p`-module of a given rank using the direct sum
         * of the standard chain complex `0 --> Z --p--> Z --> 0`.
         * This group is isomorphic to `n Z_p`.  Moreover, if
         * constructed using the matrices-with-coefficients constructor,
         * \a M would be zero and \a N would be diagonal and square with
         * \a p down the diagonal.
         *
         * @param rank the rank of the group as a Z_p-module.  That is, if the
         * group is `n Z_p`, then \a rank should be \a n.
         * @param p describes the type of ring that we use to talk about
         * the "free" module.
         */
        MarkedAbelianGroup(size_t rank, const Integer &p);

        /**
         * Creates a clone of the given group.
         */
        MarkedAbelianGroup(const MarkedAbelianGroup&) = default;

        /**
         * Moves the contents of the given group to this new group.
         * This is a fast (constant time) operation.
         *
         * The group that was passed will no longer be usable.
         */
        MarkedAbelianGroup(MarkedAbelianGroup&&) noexcept = default;

        /**
         * Sets this to be a clone of the given group.
         *
         * @return a reference to this group.
         */
        MarkedAbelianGroup& operator = (const MarkedAbelianGroup&) = default;

        /**
         * Moves the contents of the given group to this group.
         * This is a fast (constant time) operation.
         *
         * The group that was passed will no longer be usable.
         *
         * @return a reference to this group.
         */
        MarkedAbelianGroup& operator = (MarkedAbelianGroup&&) noexcept =
            default;

        /**
         * Swaps the contents of this and the given group.
         *
         * @param other the group whose contents should be swapped with this.
         */
        void swap(MarkedAbelianGroup& other) noexcept;

        /**
         * Returns the "right" matrix that was used to define the chain complex.
         * Our group was defined as the kernel of \a M modulo the image of \a N.
         * This is the matrix \a M.
         *
         * This is the matrix \a M that was originally passed to the
         * class constructor.  See the class overview for further details on
         * matrices \a M and \a N and their roles in defining the chain complex.
         *
         * @return a reference to the defining matrix \a M.
         */
        const MatrixInt& m() const;

        /**
         * Returns the "left" matrix that was used to define the chain complex.
         * Our group was defined as the kernel of \a M modulo the image of \a N.
         * This is the matrix \a N.
         *
         * This is the matrix \a N that was originally passed to the
         * class constructor.  See the class overview for further details on
         * matrices \a M and \a N and their roles in defining the chain complex.
         *
         * @return a reference to the defining matrix \a N.
         */
        const MatrixInt& n() const;

        /**
         * Returns the coefficients used for the computation of homology.
         * That is, this routine returns the integer \a p where we use
         * coefficients in Z_p.  If we use coefficients in the integers Z,
         * then this routine returns 0.
         *
         * @return the coefficients used in the homology calculation.
         */
        const Integer& coefficients() const;

        /**
         * Returns the rank of the group.
         * This is the number of included copies of <i>Z</i>.
         *
         * Equivalently, the rank is the maximum number of linearly independent
         * elements, and it indicates the size of the largest free abelian
         * subgroup.  The rank effectively ignores all torsion elements.
         *
         * @return the rank of the group.
         */
        size_t rank() const;

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
        size_t torsionRank(const Integer& degree) const;

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
        size_t torsionRank(unsigned long degree) const;

        /**
         * Returns the number of invariant factors that describe the
         * torsion elements of this group.  This is the minimal number
         * of torsion generators.
         * See the MarkedAbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        size_t countInvariantFactors() const;

        /**
         * Returns just the underlying abelian group, without any of the
         * chain complex information.
         *
         * @return the abelian group.
         */
        AbelianGroup unmarked() const;

        /**
         * Returns the rank of the chain complex supporting the homology
         * computation.
         *
         * This is the dimension of a vector in chain complex coordinates.
         * In the class notes, this is given by M.columns() and N.rows(),
         * where \a M and \a N are the matrices used to define the chain
         * complex.
         *
         * @return the rank of the chain complex.
         */
        size_t ccRank() const;

        /**
         * Returns the minimum number of generators for this group.
         *
         * This is the dimension of a vector in SNF coordinates.
         * It will always be equal to rank() + countInvariantFactors().
         *
         * @return the minimum number of generators.
         */
        size_t snfRank() const;

        /**
         * Returns the given invariant factor describing the torsion
         * elements of this group.
         * See the MarkedAbelianGroup class notes for further details.
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
         * Determines whether this and the given abelian group are
         * isomorphic.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are isomorphic.
         */
        bool isIsomorphicTo(const MarkedAbelianGroup &other) const;

        /**
         * Determines whether this and the given group were formed from
         * identical chain complex constructions.
         *
         * This is \e not an isomorphism test.  For this comparison to return
         * \c true, both groups must have been constructed from identical
         * matrices \a M and \a N, using homology with the same coefficients.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the this and the given group have
         * identical chain complex definitions.
         */
        bool operator == (const MarkedAbelianGroup& other) const;

        /**
         * Determines whether this and the given group were formed from
         * different chain complex constructions.
         *
         * This is \e not an isomorphism test.  For this comparison to return
         * \c false (i.e., for the chain complex constructions to be considered
         * identical), both groups must have been constructed from identical
         * matrices \a M and \a N, using homology with the same coefficients.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the this and the given group
         * do not have identical chain complex definitions.
         */
        bool operator != (const MarkedAbelianGroup& other) const;

        /**
         * Returns the requested free generator of this group,
         * represented in the original chain complex defining the group.
         *
         * The generator will be presented in chain complex coordinates.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The argument \a index is out of
         * range (i.e., greater than or equal to rank()).
         *
         * @param index specifies which free generator to look up;
         * this must be between 0 and rank()-1 inclusive.
         * @return the (\a index)th free generator, presented in
         * chain complex coordinates.
         */
        Vector<Integer> freeRep(size_t index) const;

        /**
         * Returns the requested generator of the torsion subgroup,
         * represented in the original chain complex defining the group.
         *
         * The generator will be presented in chain complex coordinates.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The argument \a index is out of
         * range (i.e., greater than or equal to countInvariantFactors()).
         *
         * @param index specifies which generator in the torsion subgroup;
         * this must be between 0 and countInvariantFactors()-1 inclusive.
         * @return the (\a index)th torsion generator, presented in
         * chain complex coordinates.
         */
        Vector<Integer> torsionRep(size_t index) const;

        /**
         * A combination of freeRep and torsionRep, this routine takes
         * a group element expressed in SNF coordinates and returns a
         * corresponding vector in the original chain complex.
         *
         * This routine is, in some sense, an inverse to snfRep().
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The size of the given vector was
         * not precisely snfRank().
         *
         * @param snf any vector in SNF coordinates.
         * @return a corresponding vector in chain complex coordinates.
         */
        Vector<Integer> ccRep(const Vector<Integer>& snf) const;

        /**
         * A combination of freeRep() and torsionRep() that expresses
         * a group element as a vector in the original chain complex.
         *
         * This is similar to the variant of ccRep() that takes a vector
         * as input, but it assumes that your input is a standard basis vector
         * from SNF coordinates.  Calling ccRep(snfGen) is equivalent to
         * passing the (\a snfGen)th unit vector to ccRep().
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The given index was greater than
         * or equal to the number of generators in SNF coordinates
         * (i.e., greater than or equal to snfRank()).
         *
         * @param snfGen specifies which standard basis vector to use from
         * SNF coordinates; this must be between 0 and snfRank()-1 inclusive.
         * @return a corresponding vector in chain complex coordinates.
         */
        Vector<Integer> ccRep(size_t snfGen) const;

        /**
         * Expresses the given cycle as a combination of free and torsion
         * generators.
         *
         * This routine takes a single argument \a cycle, which should be a
         * cycle in chain complex coordinates.  This routine then returns
         * this cycle as a group element, expressed in SNF coordinates.
         * See the class notes for a full explanation of what these
         * concepts mean.
         *
         * In the vector that is returned, the coordinates that hold
         * coefficients for the torsion generators will be non-negative
         * integers modulo the corresponding invariant factors
         * \a d1, ..., \a dk.
         *
         * Specifically, using the notation from the class notes,
         * suppose \a cycle belongs to ker(\a M) and snfRep(\a cycle)
         * returns the vector (\a b1, ..., \a bk, \a a1, ..., \a ar).
         * Suppose furthermore that the free generators returned
         * by freeRep(0..(r-1)) are \a f1, ..., \a fr respectively, and
         * that the torsion generators returned by torsionRep(0..(k-1))
         * are \a t1, ..., \a tk respectively.  Then
         * \a cycle = \a b1.t1 + ... + \a bk.tk + \a a1.f1 + ... + \a ar.fr
         * modulo img(\a N).
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The given vector was the wrong
         * size, or is not a cycle (i.e., not in the kernel of \a M).
         *
         * @param cycle a cycle, presented in chain complex coordinates.
         * @return the group element corresponding to \a cycle, expressed in
         * SNF coordinates.
         */
        Vector<Integer> snfRep(const Vector<Integer>& cycle) const;

        /**
         * Projects an element of the chain complex to the subspace of cycles.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The length of the given vector was
         * not the dimension of the chain complex (i.e., the number of
         * chain complex coordinates).
         *
         * @param ccelt any vector in chain complex coordinates.
         * @return a corresponding vector, also in the chain complex
         * coordinates.
         */
        Vector<Integer> cycleProjection(const Vector<Integer>& ccelt) const;

        /**
         * Projects a standard basis vector of the chain complex to the
         * subspace of cycles.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The index \a ccindx was greater than or
         * equal to the dimension of the chain complex (i.e., ccRank()).
         *
         * @param ccindx the index of the standard basis vector in chain
         * complex coordinates.
         * @return the resulting projection, in the chain complex
         * coordinates.
         */
        Vector<Integer> cycleProjection(size_t ccindx) const;

        /**
         * Determines whether the given vector represents a cycle in the
         * chain complex.
         *
         * @param chain any vector in chain complex coordinates.
         * @return \c true if and only if the given vector represents a cycle.
         */
        bool isCycle(const Vector<Integer>& chain) const;

        /**
         * Computes the differential of the given vector in the chain
         * complex whose kernel is the cycles.  In other words, this
         * routine returns `M×chain`, where \a M is the "right"
         * matrix passed to the class constructor.
         *
         * \exception InvalidArgument The given vector is not in chain
         * complex coordinates; that is, its length is not M.columns().
         *
         * @param chain any vector in chain complex coordinates.
         * @return the differential, expressed as a vector of length M.rows().
         */
        Vector<Integer> boundaryOf(const Vector<Integer>& chain) const;

        /**
         * Determines whether the given vector represents a boundary in the
         * chain complex.
         *
         * @param chain any vector in chain complex coordinates.
         * @return \c true if and only if the given vector represents a
         * boundary.
         */
        bool isBoundary(const Vector<Integer>& chain) const;

        /**
         * Expresses the given vector as a boundary in the chain complex.
         *
         * \warning If you are using mod \a p coefficients and if your element
         * projects to a non-trivial element of TOR, then `N×v != bdry` as
         * elements of TOR are not in the image of \a N.  In this case,
         * (`bdry - N×v`) represents the projection to TOR.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The given vector is not a boundary.
         *
         * @param bdry a boundary vector, given in chain complex coordinates.
         * @return a vector \a v such that `N×v=bdry`.
         */
        Vector<Integer> asBoundary(const Vector<Integer>& bdry) const;

        /**
         * Returns the number of generators of the kernel of \a M, where
         * \a M is the "right" matrix used to define the chain complex.
         *
         * @return the number of generators of ker(\a M).
         */
        size_t cycleRank() const;

        /**
         * Returns the requested generator of the cycles, i.e., the kernel of
         * the "right" matrix \a M in the chain complex.
         *
         * \warning The return value is not unique.  The specific choice
         * of vector may change between different versions of Regina.
         *
         * \exception InvalidArgument The argument \a index was out of range
         * (i.e., greater than or equal to cycleRank()).
         *
         * @param index indicates which generator to return; this must be
         * between 0 and cycleRank()-1 inclusive.
         * @return the (\a index)th generator of the cycles, expressed in
         * chain complex coordinates.
         */
        Vector<Integer> cycleGen(size_t index) const;

        /**
         * Returns the torsion subgroup of this group.
         *
         * @return the torsion subgroup.
         */
        MarkedAbelianGroup torsionSubgroup() const;

        /**
         * Returns a map representing the inclusion of the torsion subgroup
         * into this group.
         *
         * @return the inclusion map for the torsion subgroup.
         */
        HomMarkedAbelianGroup torsionInclusion() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * The text representation will be of the form `3 Z + 4 Z_2 + Z_120`.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * MarkedAbelianGroup notes.
         *
         * \nopython Use str() or utf8() instead.
         *
         * @param out the stream to write to.
         * @param utf8 if \c true, then richer unicode characters will
         * be used to make the output more pleasant to read.  In particular,
         * the output will use subscript digits and the blackboard bold Z.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false) const;

    friend class HomMarkedAbelianGroup;
};

/**
 * Swaps the contents of the two given abelian groups.
 *
 * This global routine simply calls MarkedAbelianGroup::swap(); it is provided
 * so that MarkedAbelianGroup meets the C++ Swappable requirements.
 *
 * @param lhs the group whose contents should be swapped with \a rhs.
 * @param rhs the group whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(MarkedAbelianGroup& lhs, MarkedAbelianGroup& rhs) noexcept;

/**
 * Represents a homomorphism of finitely generated abelian groups.
 *
 * One initializes such a homomorphism by providing:
 *
 * - two finitely generated abelian groups, which act as domain and codomain;
 * - a matrix describing the linear map between the free abelian
 *   groups in the centres of the respective chain complexes that were
 *   used to define the domain and codomain.  If the abelian groups are computed
 *   via homology with coefficients, the codomain coefficients must be a
 *   quotient of the domain coefficients.
 *
 * So for example, if the domain was initialized by the chain complex
 * `Z^a --A--> Z^b --B--> Z^c` with mod p coefficients, and the codomain
 * was initialized by `Z^d --D--> Z^e --E--> Z^f` with mod q
 * coefficients, then the matrix needs to be an e-by-b matrix.
 * Furthermore, you only obtain a well-defined
 * homomorphism if this matrix extends to a cycle map, which this class
 * assumes but which the user can confirm with isCycleMap(). Moreover,
 * \a q should divide \a p: this allows for \a q > 0 and \a p = 0,
 * which means the domain has Z coefficients and the codomain has mod \a q
 * coefficients.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \optlong preImageOf in CC and SNF coordinates.  This routine would
 * return a generating list of elements in the preimage, thought of as an
 * affine subspace. Or maybe just one element together with the kernel
 * inclusion.  IMO smarter to be a list because that way there's a more
 * pleasant way to make it empty.  Or we could have a variety of routines
 * among these themes.  Store some minimal data for efficient computations of
 * preImage, eventually replacing the internals of inverseHom() with a more
 * flexible set of tools. Also add an isInImage() in various coordinates.
 *
 * \todo \optlong writeTextShort() have completely different set of
 * descriptors if an endomorphism domain = codomain (not so important at the
 * moment though).  New descriptors would include things like automorphism,
 * projection, differential, finite order, etc.
 *
 * \todo \optlong Add map factorization, so that every homomorphism can be
 * split as a composite of a projection followed by an inclusion.  Add
 * kernelInclusion(), coKerMap(), etc.  Add a liftMap() call, i.e., a
 * procedure to find a lift of a map if one exists.
 *
 * \author Ryan Budney
 *
 * \ingroup algebra
 */
class HomMarkedAbelianGroup : public Output<HomMarkedAbelianGroup> {
    private:
        /** internal rep of domain of the homomorphism */
        MarkedAbelianGroup domain_;
        /** internal rep of codomain of the homomorphism */
        MarkedAbelianGroup codomain_;
        /** matrix describing map from domain to codomain, in the coordinates
            of the chain complexes used to construct domain and codomain, see
            above description */
        MatrixInt matrix;

        /** short description of matrix in SNF coordinates -- this means we've
            conjugated matrix by the relevant change-of-basis maps in both the
            domain and codomain so that we are using the coordinates of Smith
            Normal form.  We also truncate off the trivial Z/Z factors so that
            reducedMatrix will not have the same dimensions as matrix. This
            means the torsion factors appear first, followed by the free
            factors.  This is no value if it has not yet been computed. */
        std::optional<MatrixInt> reducedMatrix_;
        /** pointer to kernel of map, or no value if not yet computed. */
        std::optional<AbelianGroup> kernel_;
        /** pointer to coKernel of map, or no value if not yet computed. */
        std::optional<AbelianGroup> cokernel_;
        /** pointer to image, or no value if not yet computed. */
        std::optional<AbelianGroup> image_;
        /** pointer to a lattice which describes the kernel of the
            homomorphism, or no value if not yet computed. */
        std::optional<MatrixInt> reducedKernelLattice_;

        /** compute the ReducedKernelLattice if not yet done */
        void computeReducedKernelLattice();
        /** compute the ReducedMatrix if not yet done */
        void computeReducedMatrix();
        /** compute the Kernel if not yet done */
        void computeKernel();
        /** compute the Cokernel if not yet done */
        void computeCokernel();
        /** compute the Image if not yet done */
        void computeImage();

    public:
        /**
         * Constructs a homomorphism from two marked abelian groups and
         * a matrix that indicates where the generators are sent.
         * The roles of the two groups and the matrix are described in
         * detail in the HomMarkedAbelianGroup class overview.
         *
         * The matrix must be given in the chain-complex coordinates.
         * Specifically, if the domain was defined via the chain complex
         * `Z^a --N1--> Z^b --M1--> Z^c` and the codomain was
         * defined via `Z^d --N2--> Z^e --M2--> Z^f`, then \a mat is
         * an e-by-b matrix that describes a homomorphism from Z^b to Z^e.
         *
         * In order for this to make sense as a homomorphism of the groups
         * represented by the domain and codomain respectively, one requires
         * `img(mat×N1)` to be a subset of img(N2).  Similarly, ker(M1) must
         * be sent into ker(M2).  These facts are not checked, but are
         * assumed as preconditions of this constructor.
         *
         * \pre The matrix \a mat has the required dimensions e-by-b,
         * gives `img(mat×N1)` as a subset of img(N2), and sends ker(M1)
         * into ker(M2), as explained in the detailed notes above.
         *
         * @param dom the domain group.
         * @param codom the codomain group.
         * @param mat the matrix that describes the homomorphism from
         * \a dom to \a ran.
         */
        HomMarkedAbelianGroup(MarkedAbelianGroup dom, MarkedAbelianGroup codom,
            MatrixInt mat);

        /**
         * Creates a clone of the given homomorphism.
         */
        HomMarkedAbelianGroup(const HomMarkedAbelianGroup&) = default;

        /**
         * Moves the contents of the given homomorphism into this new
         * homomorphism.  This is a fast (constant time) operation.
         *
         * The homomorphism that was passed will no longer be usable.
         */
        HomMarkedAbelianGroup(HomMarkedAbelianGroup&& src) noexcept
            = default;

        /**
         * Sets this to be a clone of the given homomorphism.
         *
         * @return a reference to this homomorphism.
         */
        HomMarkedAbelianGroup& operator = (const HomMarkedAbelianGroup&)
            = default;

        /**
         * Moves the contents of the given homomorphism to this homomorphism.
         * This is a fast (constant time) operation.
         *
         * The homomorphism that was passed will no longer be usable.
         *
         * @return a reference to this homomorphism.
         */
        HomMarkedAbelianGroup& operator = (HomMarkedAbelianGroup&&) noexcept
            = default;

        /**
         * Swaps the contents of this and the given homomorphism.
         *
         * @param other the homomorphism whose contents should be swapped with
         * this.
         */
        void swap(HomMarkedAbelianGroup& other) noexcept;

        /**
         * Determines whether this and the given homomorphism together
         * form a chain map.
         *
         * Given two HomMarkedAbelianGroups, you have two diagrams:
         * <pre>
         * Z^a --N1--> Z^b --M1--> Z^c   Z^g --N3--> Z^h --M3--> Z^i
         *                   ^                             ^
         *                   |this.matrix                  |other.matrix
         * Z^d --N2--> Z^e --M2--> Z^f   Z^j --N4--> Z^k --M4--> Z^l
         * </pre>
         * If c=g and f=j and M1=N3 and M2=N4, you can ask if these maps
         * commute, i.e., whether you have a map of chain complexes.
         *
         * @param other the other homomorphism to analyse in conjunction
         * with this.
         * @return true if and only if c=g, M1=N3, f=j, M2=N4,
         * and the diagram commutes.
         */
        bool isChainMap(const HomMarkedAbelianGroup &other) const;

        /**
         * Is this at least a cycle map?  If not, pretty much any further
         * computations you try with this class will be give you nothing
         * more than carefully-crafted garbage.  Technically, this routine
         * only checks that cycles are sent to cycles, since it only has access
         * to three of the four maps you need to verify you have a cycle map.
         *
         * @return \c true if and only if this is a chain map.
         */
        bool isCycleMap() const;

        /**
         * Is this an epic homomorphism?
         *
         * @return true if this homomorphism is epic.
         */
        bool isEpic() const;
        /**
         * Is this a monic homomorphism?
         *
         * @return true if this homomorphism is monic.
         */
        bool isMonic() const;
        /**
         * Is this an isomorphism?
         *
         * @return true if this homomorphism is an isomorphism.
         */
        bool isIsomorphism() const;
        /**
         * Is this the zero map?
         *
         * @return true if this homomorphism is the zero map.
         */
        bool isZero() const;
        /**
         * Is this the identity automorphism?
         *
         * @return true if and only if the domain and codomain are defined via
         * the same chain complexes and the induced map on homology is the
         * identity.
         */
        bool isIdentity() const;

        /**
         * Returns the kernel of this homomorphism.
         *
         * @return the kernel of the homomorphism.
         */
        const AbelianGroup& kernel() const;
        /**
         * Returns the cokernel of this homomorphism.
         *
         * @return the cokernel of the homomorphism.
         */
        const AbelianGroup& cokernel() const;
        /**
         * Returns the image of this homomorphism.
         *
         * @return the image of the homomorphism.
         */
        const AbelianGroup& image() const;

        /**
         * Returns a very brief summary of the type of map.
         * This will state some basic properties of the homomorphism, such as:
         *
         * - whether the map is the identity;
         * - whether the map is an isomorphism;
         * - whether the map is monic or epic;
         * - if it is not monic, describes the kernel;
         * - if it is not epic, describes the co-kernel;
         * - if it is neither monic nor epic, describes the image.
         *
         * @return a brief summary.
         */
        std::string summary() const;

        /**
         * Writes a very brief summary of the type of map to the given
         * output stream.
         *
         * This writes exactly the same information as the no-argument variant
         * of summary() returns; see that routine for further details.
         *
         * \nopython Instead you can call the no-argument variant of summary(),
         * which returns this same information in string form.
         *
         * @param out the output stream to which to write.
         */
        void summary(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the stream to write to.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * @param out the stream to write to.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns the domain of this homomorphism.
         *
         * @return the domain that was used to define the homomorphism.
         */
        const MarkedAbelianGroup& domain() const;
        /**
         * Returns the codomain of this homomorphism.
         *
         * @return the codomain that was used to define the homomorphism.
         */
        const MarkedAbelianGroup& codomain() const;
        /**
         * Returns the defining matrix for the homomorphism.
         *
         * @return the matrix that was used to define the homomorphism.
         */
        const MatrixInt& definingMatrix() const;

        /**
         * Returns the internal reduced matrix representing the homomorphism.
         * This is where the rows/columns of the matrix represent
         * first the torsion summands in the order of the invariant factors,
         * and then the free generators:
         *
         *             Z_{d0} + ... + Z_{dk} + Z^r
         * where:
         *
         * - \a r is the number of free generators, as returned by rank();
         * - \a d1, ..., \a dk are the invariant factors that describe the
         *   torsion elements of the group, where
         *   1 < \a d1 | \a d2 | ... | \a dk.
         *
         * @return a copy of the internal representation of the homomorphism.
         */
        const MatrixInt& reducedMatrix() const;

        /**
         * Evaluate the image of a vector under this homomorphism, using
         * the original chain complexes' coordinates. This involves
         * multiplication by the defining matrix.
         *
         * \exception InvalidArgument The given vector was not in the
         * original chain complex coordinates; that is, its length was not
         * `domain().M().columns()`.
         *
         * @param input an input vector in the domain chain complex's
         * coordinates, of length domain().M().columns().
         * @return the image of this vector in the codomain chain complex's
         * coordinates, of length codomain().M().columns().
         */
        Vector<Integer> evalCC(const Vector<Integer>& input) const;

        /**
         * Evaluate the image of a vector under this homomorphism, using
         * the Smith normal form coordinates. This is just multiplication by
         * the reduced matrix.
         *
         * \warning Smith normal form coordinates are sensitive to the
         * implementation of the Smith Normal Form, i.e., they are not
         * canonical.
         *
         * \exception InvalidArgument The given vector was not in domain SNF
         * coordinates; that is, its length was not `domain().snfRank()`.
         *
         * @param input an input vector in the domain SNF coordinates,
         * of length domain().snfRank().
         * @return the image of this vector in the codomain chain complex's
         * coordinates, of length codomain().snfRank().
         */
        Vector<Integer> evalSNF(const Vector<Integer>& input) const;

        /**
         * Returns the inverse to a HomMarkedAbelianGroup. If this
         * homomorphism is not invertible, this routine returns the zero
         * homomorphism.
         *
         * If you are computing with mod-p coefficients, this routine will
         * further require that this invertible map preserves the UCT
         * splitting of the group, i.e., it gives an isomorphism of the
         * tensor product parts and the TOR parts.  At present this suffices
         * since we're only using this to construct maps between
         * homology groups in different coordinate systems.
         *
         * @return the inverse homomorphism, or the zero homomorphism if
         * this is not invertible.
         */
        HomMarkedAbelianGroup inverseHom() const;

        /**
         * Returns the composition of two homomorphisms.
         *
         * \pre the homomorphisms must be composable, meaning that the
         * codomain of X must have the same presentation matrices as the
         * domain of this homomorphism.
         *
         * @param X the homomorphism to compose this with.
         * @return the composite homomorphism.
         */
        HomMarkedAbelianGroup operator * (const HomMarkedAbelianGroup& X) const;

        /**
         * Returns the composition of two homomorphisms.
         *
         * \pre the homomorphisms must be composable, meaning that the
         * codomain of X must have the same presentation matrices as the
         * domain of this homomorphism.
         *
         * @param X the homomorphism to compose this with.
         * @return the composite homomorphism.
         */
        HomMarkedAbelianGroup operator * (HomMarkedAbelianGroup&& X) const;

        /**
         * Returns a HomMarkedAbelianGroup representing the induced map
         * on the torsion subgroups.
         */
        HomMarkedAbelianGroup torsionSubgroup() const;

    private:
        /**
         * For those situations where you want to define an
         * HomMarkedAbelianGroup from its reduced matrix, not from a chain
         * map.  This is in the situation where the SNF coordinates have
         * particular meaning to the user.  At present I only use this
         * for HomMarkedAbelianGroup::inverseHom().  Moreover, this routine
         * assumes redMat actually can be the reduced matrix of some
         * chain map -- this is not a restriction in
         * the coeff_==0 case, but it is if coeff_ > 0.
         *
         * \todo Erase completely once made obsolete by right/left inverse.
         */
        HomMarkedAbelianGroup(MatrixInt redMat,
                MarkedAbelianGroup dom, MarkedAbelianGroup ran);
};

/**
 * Swaps the contents of the two given homomorphisms.
 *
 * This global routine simply calls HomMarkedAbelianGroup::swap(); it is
 * provided so that HomMarkedAbelianGroup meets the C++ Swappable requirements.
 *
 * @param lhs the homomorphism whose contents should be swapped with \a rhs.
 * @param rhs the homomorphism whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(HomMarkedAbelianGroup& lhs, HomMarkedAbelianGroup& rhs) noexcept;

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline const AbelianGroup& HomMarkedAbelianGroup::kernel() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeKernel();
    return *kernel_;
}

inline const AbelianGroup& HomMarkedAbelianGroup::image() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeImage();
    return *image_;
}

inline const AbelianGroup& HomMarkedAbelianGroup::cokernel() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeCokernel();
    return *cokernel_;
}

// Inline functions for MarkedAbelianGroup

inline size_t MarkedAbelianGroup::torsionRank(unsigned long degree) const {
    return torsionRank(Integer(degree));
}

inline size_t MarkedAbelianGroup::countInvariantFactors() const {
    return invFac_.size();
}

inline const Integer& MarkedAbelianGroup::invariantFactor(
        size_t index) const {
    return invFac_[index];
}

inline size_t MarkedAbelianGroup::rank() const {
    return snfFreeRank_;
}

inline AbelianGroup MarkedAbelianGroup::unmarked() const {
    return AbelianGroup(snfFreeRank_, invFac_);
}

inline size_t MarkedAbelianGroup::snfRank() const {
    return snfFreeRank_ + invFac_.size();
}

inline size_t MarkedAbelianGroup::ccRank() const {
    return M_.columns();
}

inline size_t MarkedAbelianGroup::cycleRank() const {
    return M_.columns() - (rankM_ - TORVec_.size());
}

inline bool MarkedAbelianGroup::isTrivial() const {
    return ( (snfFreeRank_==0) && invFac_.empty() );
}

inline bool MarkedAbelianGroup::isZ() const {
    return ( (snfFreeRank_==1) && invFac_.empty() );
}

inline bool MarkedAbelianGroup::operator == (const MarkedAbelianGroup& other)
        const {
    return (M_ == other.M_) && (N_ == other.N_) && (coeff_ == other.coeff_);
}

inline bool MarkedAbelianGroup::operator != (const MarkedAbelianGroup& other)
        const {
    return (M_ != other.M_) || (N_ != other.N_) || (coeff_ != other.coeff_);
}

inline bool MarkedAbelianGroup::isIsomorphicTo(
        const MarkedAbelianGroup &other) const {
    return ((invFac_ == other.invFac_) && (snfFreeRank_ == other.snfFreeRank_));
}

inline const MatrixInt& MarkedAbelianGroup::m() const {
    return M_;
}
inline const MatrixInt& MarkedAbelianGroup::n() const {
    return N_;
}
inline const Integer& MarkedAbelianGroup::coefficients() const {
    return coeff_;
}

inline void swap(MarkedAbelianGroup& a, MarkedAbelianGroup& b) noexcept {
    a.swap(b);
}

// Inline functions for HomMarkedAbelianGroup

inline HomMarkedAbelianGroup::HomMarkedAbelianGroup(
        MarkedAbelianGroup dom, MarkedAbelianGroup ran, MatrixInt mat) :
        domain_(std::move(dom)), codomain_(std::move(ran)),
        matrix(std::move(mat)) {
}

inline const MarkedAbelianGroup& HomMarkedAbelianGroup::domain() const {
    return domain_;
}
inline const MarkedAbelianGroup& HomMarkedAbelianGroup::codomain() const {
    return codomain_;
}
inline const MatrixInt& HomMarkedAbelianGroup::definingMatrix() const {
    return matrix;
}

inline const MatrixInt& HomMarkedAbelianGroup::reducedMatrix() const {
    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    return *reducedMatrix_;
}

inline bool HomMarkedAbelianGroup::isEpic() const {
    return cokernel().isTrivial();
}

inline bool HomMarkedAbelianGroup::isMonic() const {
    return kernel().isTrivial();
}

inline bool HomMarkedAbelianGroup::isIsomorphism() const {
    return (cokernel().isTrivial() && kernel().isTrivial());
}

inline bool HomMarkedAbelianGroup::isZero() const {
    return image().isTrivial();
}

inline Vector<Integer> HomMarkedAbelianGroup::evalCC(
       const Vector<Integer>& input) const {
    if (input.size() != domain_.M_.columns())
        throw InvalidArgument(
            "The argument to evalCC() is a vector of the wrong size");

    return matrix * input;
}

inline void swap(HomMarkedAbelianGroup& a, HomMarkedAbelianGroup& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

