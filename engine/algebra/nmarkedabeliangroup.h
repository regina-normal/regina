
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

/*! \file algebra/nmarkedabeliangroup.h
 *  \brief Deals with abelian groups given by chain complexes.
 */

#ifndef __NMARKEDABELIANGROUP_H
#ifndef __DOXYGEN
#define __NMARKEDABELIANGROUP_H
#endif

#include <vector>
#include <memory>
#include "maths/nmatrixint.h"
#include "utilities/ptrutils.h"

namespace regina {

class NHomMarkedAbelianGroup;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents a finitely generated abelian group given by a chain complex.
 *
 * This class is initialized with a chain complex.  The chain complex is given
 * in terms of two integer matrices \a M and \a N such that M*N=0. The abelian
 * group is the kernel of \a M mod the image of \a N.
 *
 * In other words, we are computing the homology of the chain complex
 * <tt>Z^a --N--> Z^b --M--> Z^c</tt>
 * where a=N.columns(), M.columns()=b=N.rows(), and c=M.rows().  An additional
 * constructor allows one to take the homology with coefficients in an arbitrary
 * cyclic group.
 *
 * This class allows one to retrieve the invariant factors, the rank, and
 * the corresponding vectors in the kernel of \a M.  Moreover, given a
 * vector in the kernel of \a M, it decribes the homology class of the
 * vector (the free part, and its position in the invariant factors).
 *
 * The purpose of this class is to allow one to not only
 * represent homology groups, but it gives coordinates on the group allowing
 * for the construction of homomorphisms, and keeping track of subgroups.
 *
 * Some routines in this class refer to the internal <i>presentation
 * matrix</i>.  This is a proper presentation matrix for the abelian group,
 * and is created by constructing the product getMRBi() * \a N, and then
 * removing the first getRankM() rows.
 *
 * @author Ryan Budney
 *
 * \todo \optlong Look at using sparse matrices for storage of SNF and the like.
 * \todo Testsuite additions: isBoundary(), boundaryMap(), writeAsBdry(),
 * cycleGen().
 */
class NMarkedAbelianGroup : public ShareableObject {
    private:
        /** Internal original M */
        NMatrixInt OM; // copy of initializing M
        /** Internal original N */
        NMatrixInt ON; // copy of initializing N assumes M*N == 0
        /** Internal change of basis. SNF(OM) == OMC*OM*OMR */
        NMatrixInt OMR, OMC;
        /** Internal change of basis. OM = OMCi*SNF(OM)*OMRi */
        NMatrixInt OMRi, OMCi;
        /** Internal rank of M */
        unsigned long rankOM; // this is the index of the first zero entry
                              // in SNF(OM)

        /* Internal reduced N matrix: */
        // In the notes below, ORN refers to the internal presentation
        // matrix [OMRi * ON], where the brackets indicate removal of the
        // first rankOM rows.

        /** Internal change of basis. ornC * ORN * ornR is the SNF(ORN). */
        std::auto_ptr<NMatrixInt> ornR, ornC;
        /** Internal change of basis. These are the inverses of ornR and ornC
            respectively. */
        std::auto_ptr<NMatrixInt> ornRi, ornCi; 
        /** Internal change of basis matrix for homology with coefficents.
            otC * tensorPres * otR == SNF(tensorPres) */
        std::auto_ptr<NMatrixInt> otR, otC, otRi, otCi;

        /** Internal list of invariant factors. */
        std::vector<NLargeInteger> InvFacList;
        /** The number of free generators, from SNF(ORN) */
        unsigned long snfrank;
        /** The row index of the first zero along the diagonal in SNF(ORN). */
        unsigned long snffreeindex;
        /** Number of invariant factors. */
        unsigned long ifNum;
        /** Row index of first invariant factor (ie entry > 1) in SNF(ORN) */
        unsigned long ifLoc;

        // These variables store information for mod-p homology computations.
        /** coefficients to use in homology computation **/
        NLargeInteger coeff;
        /** TORLoc stores the location of the first TOR entry from the SNF
            of OM:  TORLoc == rankOM-TORVec.size() */
        unsigned long TORLoc;
        /** TORVec's i-th entry stores the entries q where Z_p --q-->Z_p
            is the i-th TOR entry from the SNF of OM */
        std::vector<NLargeInteger> TORVec;

        /** invariant factor data in the tensor product presentation matrix
            SNF */
        unsigned long tensorIfLoc;
        unsigned long tensorIfNum;
        std::vector<NLargeInteger> tensorInvFacList;
        // and NHomMarkedAbelianGroup at present needs to see some of the
        // internals of NMarkedAbelianGroup
        // at present this is only used for inverseHom().
        friend class NHomMarkedAbelianGroup;

    public:

        /**
         * Creates a marked abelian group from a chain complex. This constructor
         * assumes you're interested in homology with integer coefficents of
         * the chain complex.  See the class notes for details.
         *
         * \pre M.columns() = N.rows().
         * \pre The product M*N = 0.
         *
         * @param M the `right' matrix in the chain complex
         * @param N the `left' matrix in the chain complex
         */
        NMarkedAbelianGroup(const NMatrixInt& M, const NMatrixInt& N);

        /**
         * Creates a marked abelian group from a chain complex with
         * coefficients in Z_p.
         *
         * \pre M.columns() = N.rows().
         * \pre The product M*N = 0.
         *
         * @param M the `right' matrix in the chain complex
         * @param N the `left' matrix in the chain complex
         * @param pcoeff specifies the coefficient ring, Z_pcoeff. We require
         * \a pcoeff >= 0.  If you know beforehand that \a pcoeff=0, it's
         * more efficient to use the previous constructor.
         */
        NMarkedAbelianGroup(const NMatrixInt& M, const NMatrixInt& N,
            const NLargeInteger &pcoeff);

        /**
         * Creates a free Z_p-module of a given rank using the direct sum
         * of the standard chain complex <tt>0 --> Z --p--> Z --> 0</tt>.
         * So this group is isomorphic to <tt>n Z_p</tt>.  Moreover, if
         * constructed using the previous constructor, \a M would be zero
         * and \a N would be diagonal and square with \a p down the diagonal.
         *
         * @param rk the rank of the group as a Z_p-module.  That is, if the
         * group is <tt>n Z_p</tt>, then \a rk should be \a n.
         * @param \a p describes the type of ring that we use to talk about
         * the "free" module.
         */
        NMarkedAbelianGroup(unsigned long rk, const NLargeInteger &p);

        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        NMarkedAbelianGroup(const NMarkedAbelianGroup& cloneMe);

        /**
         * Determines whether this is a chain complex.  Specifically,
         * this routine returns \c true if and only if
         * M*N = 0 where M and N are the definining matrices.
         *
         * @return \c true if and only if M*N = 0.
         */
        bool isChainComplex() const;

        /**
         * Destroys the group.
         */
        virtual ~NMarkedAbelianGroup();

        /**
         * Returns the rank of the group.
         * This is the number of included copies of <i>Z</i>.
         *
         * @return the rank of the group.
         */
        unsigned long getRank() const;

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
        unsigned long getTorsionRank(const NLargeInteger& degree) const;

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
        unsigned long getTorsionRank(unsigned long degree) const;

        /**
         * Returns the number of invariant factors that describe the
         * torsion elements of this group.  This is the minimal number
         * of torsion generators.
         * See the NMarkedAbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        unsigned long getNumberOfInvariantFactors() const;

        /**
         * Returns the minimum number of generators for the group.
         *
         * @return the minimum number of generators.
         */
        unsigned long minNumberOfGenerators() const;

        /**
         * Returns the given invariant factor describing the torsion
         * elements of this group.
         * See the NMarkedAbelianGroup class notes for further details.
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
         * Determines whether this is the trivial (zero) group.
         *
         * @return \c true if and only if this is the trivial group.
         */
        bool isTrivial() const;

        /**
         * Determines whether this and the given abelian group are
         * isomorphic.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Users should switch to the less ambiguously named
         * routine isIsomorphicto() instead.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are isomorphic.
         */
        bool operator == (const NMarkedAbelianGroup &other) const;

        /**
         * Determines whether this and the given abelian group are
         * isomorphic.
         *
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are isomorphic.
         */
        bool isIsomorphicTo(const NMarkedAbelianGroup &other) const;

        /**
         * Determines whether or not the two NMarkedAbelianGroups are
         * identical, which means they have exactly the same presentation
         * matrices.  This is useful for determining if two
         * NHomMarkedAbelianGroups are composable.  See isIsomorphicTo() if
         * all you care about is the isomorphism relation among groups
         * defined by presentation matrices.
         *
         * @param other the NMarkedAbelianGroup with which this should be
         * compared.
         * @return \c true if and only if the two groups have identical
         * chain-complex definitions.
         */
        bool equalTo(const NMarkedAbelianGroup& other) const;

        /**
         * The text representation will be of the form
         * <tt>3 Z + 4 Z_2 + Z_120</tt>.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * NMarkedAbelianGroup notes.
         *
         * @param out the stream to write to.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Returns the requested free generator in the original chain
         * complex defining the group.
         *
         * As described in the class overview, this marked abelian group
         * is defined by matrices \a M and \a N where M*N = 0.
         * If \a M is an \a m by \a l matrix and \a N is an \a l by \a n
         * matrix, then this routine returns the (\a index)th free
         * generator of ker(M)/img(N) in \a Z^l.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * \ifacespython The return value will be a python list.
         *
         * @param index specifies which free generator to look up;
         * this must be between 0 and getRank()-1 inclusive.
         * @return the coordinates of the free generator in the nullspace of
         * \a M; this vector will have length M.columns() (or
         * equivalently, N.rows()). If this generator does not exist, 
         * you will receive an empty vector.
         */
        std::vector<NLargeInteger> getFreeRep(unsigned long index) const;

        /**
         * Returns the requested generator of the torsion subgroup but
         * represented in the original chain complex defining the group.
         *
         * As described in the class overview, this marked abelian group
         * is defined by matrices \a M and \a N where M*N = 0.
         * If \a M is an \a m by \a l matrix and \a N is an \a l by \a n
         * matrix, then this routine returns the (\a index)th torsion
         * generator of ker(M)/img(N) in \a Z^l.
         *
         * \ifacespython The return value will be a python list.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @param index specifies which generator in the torsion subgroup;
         * this must be at least 0 and strictly less than the number of
         * non-trivial invariant factors.  If not, you receive an empty
         * vector.
         * @return the coordinates of the generator in the nullspace of
         * \a M; this vector will have length M.columns() (or
         * equivalently, N.rows()).
         */
        std::vector<NLargeInteger> getTorsionRep(unsigned long index) const;

        /**
         * A combination of getFreeRep and getTorsion rep, this routine takes
         * a vector which represents an element in the group in the SNF
         * coordinates and returns a corresponding vector in the original
         * chain complex.
         *
         * This routine is the inverse to snfRep() described below.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @param SNFRep a vector of size the number of generators of
         * the group, i.e., it must be valid in the SNF coordinates.  If not,
         * an empty vector is returned.
         * @return a corresponding vector whose length is M.columns(),
         * where \a M is one of the matrices that defines the chain
         * complex; see the class notes for details.
         */
        std::vector<NLargeInteger> ccRep(
            const std::vector<NLargeInteger>& SNFRep) const;

        /**
         * Same as ccRep(const std::vector<NLargeInteger>&), but we assume you
         * only want the chain complex representation of a standard basis
         * vector from SNF coordinates.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @param index specifies which standard basis vector from SNF
         * coordinates; this must be between 0 and
         * minNumberOfGenerators()-1 inclusive.
         * @return a corresponding vector whose length is M.columns(),
         * where \a M is one of the matrices that defines the chain
         * complex; see the class notes for details.
         */
        std::vector<NLargeInteger> ccRep(unsigned long SNFRep) const;

        /**
         * Projects an element of the chain complex to the subspace of cycles.
         * Returns an empty vector if the input element does not have
         * dimensions of the chain complex.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @param ccelt a vector whose length is M.columns(),
         * where \a M is one of the matrices that defines the chain
         * complex (see the class notes for details).
         * @return a corresponding vector, also in the chain complex
         * coordinates.
         */
        std::vector<NLargeInteger> cycleProjection(
            const std::vector<NLargeInteger> &ccelt) const;

        /**
         * Projects an element of the chain complex to the subspace of cycles.
         * Returns an empty vector if the input index is out of bounds.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @param ccindx the index of the standard basis vector in chain
         * complex coordinates.
         * @return the resulting projection, in the chain complex
         * coordinates.
         */
        std::vector<NLargeInteger> cycleProjection(unsigned long ccindx) const;

        /**
         * Given a vector, determines if it represents a cycle in the chain
         * complex.
         *
         * @param input an input vector in chain complex coordinates.
         * @return \c true if and only if the given vector represents a cycle.
         */
        bool isCycle(const std::vector<NLargeInteger> &input) const;

        /**
         * Computes the differential of the given vector in the chain
         * complex whose kernel is the cycles.  In other words, this
         * routine returns <tt>M*CCrep</tt>.
         *
         * @param ccelt a vector whose length is M.columns(),
         * where \a M is one of the matrices that defines the chain
         * complex (see the class notes for details).
         * @return the differential, expressed as a vector of length M.rows().
         */
        std::vector<NLargeInteger> boundaryMap(
            const std::vector<NLargeInteger> &CCrep) const;

        /**
         * Given a vector, determines if it represents a boundary in the chain
         * complex.
         *
         * @param input a vector whose length is M.columns(),
         * where \a M is one of the matrices that defines the chain
         * complex (see the class notes for details).
         * @return \c true if and only if the given vector represents a
         * boundary.
         */
        bool isBoundary(const std::vector<NLargeInteger> &input) const;

        /**
         * Expresses the given vector as a boundary in the chain complex
         * (if the vector is indeed a boundary at all).  This routine
         * uses chain complex coordinates for both the input and the
         * return value.
         *
         * \warning If you're using mod-p coefficients and if your element
         * projects to a non-trivial element of TOR, then Nv != input as
         * elements of TOR aren't in the image of N.  In this case,
         * input-Nv represents the projection to TOR.
         *
         * \warning The return value may change from version to version
         * of Regina, since it depends on the choice of Smith normal form.
         *
         * @return a length zero vector if the input is not a boundary;
         * otherwise a vector \a v such that <tt>Nv=input</tt>.
         */
        std::vector<NLargeInteger> writeAsBoundary(
            const std::vector<NLargeInteger> &input) const;

        /**
         * Returns the rank of the chain complex supporting the homology
         * computation.
         *
         * @return the rank of the chain complex.
         */
        unsigned long getRankCC() const;

        /**
         * Expresses the given vector as a combination of free and torsion
         * generators.  This answer is coordinate dependant, meaning the answer
         * may change depending on how the Smith normal form is computed.
         *
         * Recall that this marked abelian was defined by matrices \a M
         * and \a N with M*N=0; suppose that \a M is an \a m by \a l matrix
         * and \a N is an \a l by \a n matrix.  This abelian group is then
         * the quotient ker(M)/img(N) in \a Z^l.
         *
         * When it is constructed, this group is computed to be isomorphic to
         * some Z_{d0} + ... + Z_{dk} + Z^d, where:
         *
         * - \a d is the number of free generators, as returned by getRank();
         * - \a d1, ..., \a dk are the invariant factors that describe the
         *   torsion elements of the group, where
         *   1 < \a d1 | \a d2 | ... | \a dk.
         *
         * This routine takes a single argument \a v, which must be a
         * vector in \a Z^l.
         *
         * If \a v belongs to ker(M), this routine describes how it
         * projects onto the group ker(M)/img(N).  Specifically, it
         * returns a vector of length \a d + \a k, where:
         *
         * - The first \a k elements describe the projection of \a v
         *   to the torsion component Z_{d1} + ... + Z_{dk}.  These
         *   elements are returned as non-negative integers modulo
         *   \a d1, ..., \a dk respectively.
         * - The remaining \a d elements describe the projection of \a v
         *   to the free component \a Z^d.
         *
         * In other words, suppose \a v belongs to ker(M) and snfRep(v)
         * returns the vector (\a b1, ..., \a bk, \a a1, ..., \a ad).
         * Suppose furthermore that the free generators returned
         * by getFreeRep(0..(d-1)) are \a f1, ..., \a fd respectively, and
         * that the torsion generators returned by getTorsionRep(0..(k-1))
         * are \a t1, ..., \a tk respectively.  Then
         * \a v = \a b1.t1 + ... + \a bk.tk + \a a1.f1 + ... + \a ad.fd
         * modulo img(N).
         *
         * If \a v does not belong to ker(M), this routine simply returns
         * the empty vector.
         *
         * \warning The return value may change from version to version
         * of Regina, as it depends on the choice of Smith normal form.
         *
         * \pre Vector \a v has length M.columns(), or equivalently N.rows().
         *
         * \ifacespython Both \a v and the return value are python lists.
         *
         * @param v a vector of length M.columns().
         * @return a vector that describes \a v in the standard
         * Z_{d1} + ... + Z_{dk} + Z^d form, or the empty vector if
         * \a v is not in the kernel of \a M.
         */
        std::vector<NLargeInteger> snfRep(
            const std::vector<NLargeInteger>& v) const;

        /**
         * A deprecated alternative to snfRep().
         *
         * \deprecated This routine has been renamed to snfRep().  See
         * snfRep() for details, preconditions and warnings.
         *
         * \ifacespython Both \a v and the return value are python lists.
         *
         * @param v a vector of length M.columns().
         * @return a vector that describes \a v in the standard
         * Z_{d1} + ... + Z_{dk} + Z^d form, or the empty vector if
         * \a v is not in the kernel of \a M.
         */
        std::vector<NLargeInteger> getSNFIsoRep(
            const std::vector<NLargeInteger>& v) const;

        /**
         * Returns the number of generators of ker(M), where M is one of
         * the defining matrices of the chain complex.
         *
         * @return the number of generators of ker(M).
         */
        unsigned long minNumberCycleGens() const;

        /**
         * Returns the <i>i</i>th generator of the cycles, i.e., the kernel of
         * M in the chain complex.
         *
         * \warning The return value may change from version to version
         * of Regina, as it depends on the choice of Smith normal form.
         *
         * @param i between 0 and minNumCycleGens()-1.
         * @return the corresponding generator in chain complex coordinates.
         */
        std::vector<NLargeInteger> cycleGen(unsigned long i) const;

        /**
         * Returns a change-of-basis matrix for the Smith normal form of \a M.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * Recall from the class overview that this marked abelian group
         * is defined by matrices \a M and \a N, where M*N = 0.
         *
         * - getMCB() * M * getMRB() is the Smith normal form of \a M;
         * - getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getMRB() as described above.
         */
        const NMatrixInt& getMRB() const;
        /**
         * Returns an inverse change-of-basis matrix for the Smith normal
         * form of \a M.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * Recall from the class overview that this marked abelian group
         * is defined by matrices \a M and \a N, where M*N = 0.
         *
         * - getMCB() * M * getMRB() is the Smith normal form of \a M;
         * - getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getMRBi() as described above.
         */
        const NMatrixInt& getMRBi() const;
        /**
         * Returns a change-of-basis matrix for the Smith normal form of \a M.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * Recall from the class overview that this marked abelian group
         * is defined by matrices \a M and \a N, where M*N = 0.
         *
         * - getMCB() * M * getMRB() is the Smith normal form of \a M;
         * - getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getMCB() as described above.
         */
        const NMatrixInt& getMCB() const;
        /**
         * Returns an inverse change-of-basis matrix for the Smith normal
         * form of \a M.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * Recall from the class overview that this marked abelian group
         * is defined by matrices \a M and \a N, where M*N = 0.
         *
         * - getMCB() * M * getMRB() is the Smith normal form of \a M;
         * - getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getMCBi() as described above.
         */
        const NMatrixInt& getMCBi() const;
        /**
         * Returns a change-of-basis matrix for the Smith normal form of
         * the internal presentation matrix.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * For details on the internal presentation matrix, see the class
         * overview.  If \a P is the internal presentation matrix, then:
         *
         * - getNCB() * P * getNRB() is the Smith normal form of \a P;
         * - getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getNRB() as described above.
         */
        const NMatrixInt& getNRB() const;
        /**
         * Returns an inverse change-of-basis matrix for the Smith normal
         * form of the internal presentation matrix.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * For details on the internal presentation matrix, see the class
         * overview.  If \a P is the internal presentation matrix, then:
         *
         * - getNCB() * P * getNRB() is the Smith normal form of \a P;
         * - getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getNRBi() as described above.
         */
        const NMatrixInt& getNRBi() const;
        /**
         * Returns a change-of-basis matrix for the Smith normal form of
         * the internal presentation matrix.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * For details on the internal presentation matrix, see the class
         * overview.  If \a P is the internal presentation matrix, then:
         *
         * - getNCB() * P * getNRB() is the Smith normal form of \a P;
         * - getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getNCB() as described above.
         */
        const NMatrixInt& getNCB() const;
        /**
         * Returns an inverse change-of-basis matrix for the Smith normal
         * form of the internal presentation matrix.
         *
         * This is one of several routines that returns information on
         * how we determine the isomorphism-class of this group.
         *
         * For details on the internal presentation matrix, see the class
         * overview.  If \a P is the internal presentation matrix, then:
         *
         * - getNCB() * P * getNRB() is the Smith normal form of \a P;
         * - getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         *   respectively.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the matrix getNCBi() as described above.
         */
        const NMatrixInt& getNCBi() const;

        /**
         * Returns the rank of the defining matrix \a M.
         *
         * The matrix \a M is the `right' matrix used in defining the chain
         * complex.  See the class overview for further details.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the rank of the defining matrix \a M.
         */
        unsigned long getRankM() const;

        /**
         * Returns the index of the first free generator in the Smith
         * normal form of the internal presentation matrix.  See the class
         * overview for details.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the index of the first free generator.
         */
        unsigned long getFreeLoc() const;

        /**
         * Returns the index of the first torsion generator in the Smith
         * normal form of the internal presentation matrix.  See the class
         * overview for details.
         *
         * \deprecated This routine will be removed in Regina 5.0.
         *
         * @return the index of the first torsion generator.
         */
        unsigned long getTorsionLoc() const;

        /**
         * Returns the `right' matrix used in defining the chain complex.
         * Our group was defined as the kernel of \a M mod the image of \a N.
         * This is the matrix \a M.
         *
         * This is a copy of the matrix \a M that was originally passed to the
         * class constructor.  See the class overview for further details on
         * matrices \a M and \a N and their roles in defining the chain complex.
         *
         * @return a reference to the defining matrix M.
         */
        const NMatrixInt& getM() const;
        /**
         * Returns the `left' matrix used in defining the chain complex.
         * Our group was defined as the kernel of \a M mod the image of \a N.
         * This is the matrix \a N.
         *
         * This is a copy of the matrix \a N that was originally passed to the
         * class constructor.  See the class overview for further details on
         * matrices \a M and \a N and their roles in defining the chain complex.
         *
         * @return a reference to the defining matrix N.
         */
        const NMatrixInt& getN() const;
        /**
         * Returns the coefficients used for the computation of homology.
         * That is, this routine returns the integer \a p where we use
         * coefficients in Z_p.  If we use coefficients in the integers Z,
         * then this routine returns 0.
         *
         * @return the coefficients used in the homology calculation.
         */
        const NLargeInteger& coefficients() const;
};

/**
 * Represents a homomorphism of finitely generated abelian groups.
 *
 * One initializes such a homomorphism by providing:
 *
 * - two finitely generated abelian groups, which act as domain and range;
 * - a matrix describing the linear map between the free abelian
 *   groups in the centres of the respective chain complexes that were
 *   used to define the domain and range.  If the abelian groups are computed
 *   via homology with coefficients, the range coefficients must be a quotient
 *   of the domain coefficients.
 *
 * So for example, if the domain was initialized by the chain complex
 * <tt>Z^a --A--> Z^b --B--> Z^c</tt> with mod p coefficients, and the range
 * was initialized by <tt>Z^d --D--> Z^e --E--> Z^f</tt> with mod q
 * coefficients, then the matrix needs to be an e-by-b matrix.
 * Furthermore, you only obtain a well-defined
 * homomorphism if this matrix extends to a cycle map, which this class
 * assumes but which the user can confirm with isCycleMap(). Moreover,
 * \a q should divide \a p: this allows for \a q > 0 and \a p = 0,
 * which means the domain has Z coefficients and the range has mod \a q
 * coefficients.
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
 * descriptors if an endomorphism domain = range (not so important at the
 * moment though).  New descriptors would include things like automorphism,
 * projection, differential, finite order, etc.
 *
 * \todo \optlong Add map factorization, so that every homomorphism can be
 * split as a composite of a projection followed by an inclusion.  Add
 * kernelInclusion(), coKerMap(), etc.  Add a liftMap() call, i.e., a
 * procedure to find a lift of a map if one exists.
 *
 * @author Ryan Budney
 */
class NHomMarkedAbelianGroup : public ShareableObject {
    private:
        /** internal rep of domain of the homomorphism */
        NMarkedAbelianGroup domain;
        /** internal rep of range of the homomorphism */
        NMarkedAbelianGroup range;
        /** matrix describing map from domain to range, in the coordinates
            of the chain complexes used to construct domain and range, see
            above description */
        NMatrixInt matrix;

        /** short description of matrix in SNF coordinates -- this means we've
            conjugated matrix by the relevant change-of-basis maps in both the
            domain and range so that we are using the coordinates of Smith
            Normal form.  We also truncate off the trivial Z/Z factors so that
            reducedMatrix will not have the same dimensions as matrix. This
            means the torsion factors appear first, followed by the free
            factors. */
        NMatrixInt* reducedMatrix;
        /** pointer to kernel of map */
        NMarkedAbelianGroup* kernel;
        /** pointer to coKernel of map */
        NMarkedAbelianGroup* coKernel;
        /** pointer to image */
        NMarkedAbelianGroup* image;
        /** pointer to a lattice which describes the kernel of the
            homomorphism. */
        NMatrixInt* reducedKernelLattice;

        /** compute the ReducedKernelLattice */
        void computeReducedKernelLattice();
        /** compute the ReducedMatrix */
        void computeReducedMatrix();
        /** compute the Kernel */
        void computeKernel();
        /** compute the Cokernel */
        void computeCokernel();
        /** compute the Image */
        void computeImage();

    public:
        /**
         * Constructs a homomorphism from two marked abelian groups and
         * a matrix that indicates where the generators are sent.
         * The roles of the two groups and the matrix are described in
         * detail in the NHomMarkedAbelianGroup class overview.
         *
         * The matrix must be given in the chain-complex coordinates.
         * Specifically, if the domain was defined via the chain complex
         * <tt>Z^a --N1--> Z^b --M1--> Z^c</tt> and the range was
         * defined via <tt>Z^d --N2--> Z^e --M2--> Z^f</tt>, then \a mat is
         * an e-by-b matrix that describes a homomorphism from Z^b to Z^e.
         *
         * In order for this to make sense as a homomorphism of the groups
         * represented by the domain and range respectively, one requires
         * img(mat*N1) to be a subset of img(N2).  Similarly, ker(M1) must
         * be sent into ker(M2).  These facts are not checked, but are
         * assumed as preconditions of this constructor.
         *
         * \pre The matrix \a mat has the required dimensions e-by-b,
         * gives img(mat*N1) as a subset of img(N2), and sends ker(M1)
         * into ker(M2), as explained in the detailed notes above.
         *
         * @param dom the domain group.
         * @param ran the range group.
         * @param mat the matrix that describes the homomorphism from 
         * \a dom to \a ran.
         */
        NHomMarkedAbelianGroup(const NMarkedAbelianGroup& dom,
                const NMarkedAbelianGroup& ran,
                const NMatrixInt &mat);

        /**
         * Copy constructor.
         *
         * @param h the homomorphism to clone.
         */
        NHomMarkedAbelianGroup(const NHomMarkedAbelianGroup& h);
        /**
         * Destructor.
         */
        virtual ~NHomMarkedAbelianGroup();

        /**
         * Determines whether this and the given homomorphism together
         * form a chain map.
         *
         * Given two NHomMarkedAbelianGroups, you have two diagrams:
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
        bool isChainMap(const NHomMarkedAbelianGroup &other) const;

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
         * A deprecated alternative to isIsomorphism().
         *
         * \deprecated This routine will be removed in a future version
         * of Regina; please use the identical routine isIsomorphism() instead.
         *
         * @return true if this homomorphism is an isomorphism.
         */
        bool isIso() const;
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
         * @return true if and only if the domain and range are defined via
         * the same chain complexes and the induced map on homology is the
         * identity.
         */
        bool isIdentity() const;

        /**
         * Returns the kernel of this homomorphism.
         *
         * @return the kernel of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getKernel() const;
        /**
         * Returns the cokernel of this homomorphism.
         *
         * @return the cokernel of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getCokernel() const;
        /**
         * Returns the image of this homomorphism.
         *
         * @return the image of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getImage() const;

        /**
         * Short text representation.  This will state some basic
         * properties of the homomorphism, such as:
         *
         * - whether the map is the identity;
         * - whether the map is an isomorphism;
         * - whether the map is monic or epic;
         * - if it is not monic, describes the kernel;
         * - if it is not epic, describes the co-kernel;
         * - if it is neither monic nor epic, describes the image.
         *
         * @param out the stream to write to.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * A more detailed text representation of the homomorphism.
         *
         * @param out the stream to write to.
         */
        virtual void writeTextLong(std::ostream& out) const;

        /**
         * Returns the domain of this homomorphism.
         *
         * @return the domain that was used to define the homomorphism.
         */
        const NMarkedAbelianGroup& getDomain() const;
        /**
         * Returns the range of this homomorphism.
         *
         * @return the range that was used to define the homomorphism.
         */
        const NMarkedAbelianGroup& getRange() const;
        /**
         * Returns the defining matrix for the homomorphism.
         *
         * @return the matrix that was used to define the homomorphism.
         */
        const NMatrixInt& getDefiningMatrix() const;

        /**
         * Returns the internal reduced matrix representing the homomorphism.
         * This is where the rows/columns of the matrix represent
         * first the free generators, then the torsion summands in the order
         * of the invariant factors:
         *
         *             Z^d + Z_{d0} + ... + Z_{dk}
         * where:
         *
         * - \a d is the number of free generators, as returned by getRank();
         * - \a d1, ..., \a dk are the invariant factors that describe the
         *   torsion elements of the group, where
         *   1 < \a d1 | \a d2 | ... | \a dk.
         *
         * @return a copy of the internal representation of the homomorphism.
         */
        const NMatrixInt& getReducedMatrix() const;

        /**
         * Evaluate the image of a vector under this homomorphism, using
         * the original chain complexes' coordinates. This involves
         * multiplication by the defining matrix.
         *
         * @param input an input vector in the domain chain complex's
         * coordinates, of length getDomain().getM().columns().
         * @return the image of this vector in the range chain complex's
         * coordinates, of length getRange().getM().columns().
         */
        std::vector<NLargeInteger> evalCC(
            const std::vector<NLargeInteger> &input) const; 

        /**
         * Evaluate the image of a vector under this homomorphism, using
         * the Smith normal form coordinates. This is just multiplication by
         * the reduced matrix, returning the empty vector if the input vector
         * has the wrong dimensions.
         *
         * \warning Smith normal form coordinates are sensitive to the
         * implementation of the Smith Normal Form, i.e., they are not
         * canonical.
         *
         * @param input an input vector in the domain SNF coordinates,
         * of length getDomain().minNumberOfGenerators().
         * @return the image of this vector in the range chain complex's
         * coordinates, of length getRange().minNumberOfGenerators().
         */
        std::vector<NLargeInteger> evalSNF(
            const std::vector<NLargeInteger> &input) const;

        /**
         * Returns the inverse to an NHomMarkedAbelianGroup. If this
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
        std::auto_ptr<NHomMarkedAbelianGroup> inverseHom() const;

        /**
         * Returns the composition of two homomorphisms.
         *
         * \pre the homomorphisms must be composable, meaning that the
         * range of X must have the same presentation matrices as the
         * domain of this homomorphism.
         *
         * @param X the homomorphism to compose this with.
         * @return a newly created composite homomorphism.
         */
        std::auto_ptr<NHomMarkedAbelianGroup> operator * (
            const NHomMarkedAbelianGroup &X) const;

        /**
         * Writes a human-readable version of the reduced matrix to the
         * given output stream.  This is a description of the homomorphism
         * in some specific coordinates at present only meant to be
         * internal to NHomMarkedAbelianGroup.  At present, these coordinates
         * have the torsion factors of the group appearing first, followed by
         * the free factors.
         *
         * \ifacespython The \a out argument is missing; instead this is
         * assumed to be standard output.
         *
         * @param out the output stream.
         */
        void writeReducedMatrix(std::ostream& out) const;

    private:
        /**
         * For those situations where you want to define an NHomMarkedAbelianGroup
         * from its reduced matrix, not from a chain map.  This is in the situation where
         * the SNF coordinates have particular meaning to the user.  At present I only use this
         * for NHomMarkedAbelianGroup::inverseHom().  Moreover, this routine assumes tebeRedMat
         * actually can be the reduced matrix of some chain map -- this is not a restriction in
         * the coeff==0 case, but it is if coeff > 0. 
         *
         * \todo Erase completely once made obsolete by right/left inverse.
         */
        NHomMarkedAbelianGroup(const NMatrixInt &tobeRedMat,
                const NMarkedAbelianGroup &dom, 
                const NMarkedAbelianGroup &ran);
};

/*@}*/

// Inline functions for NMarkedAbelianGroup

// copy constructor
inline NMarkedAbelianGroup::NMarkedAbelianGroup(const NMarkedAbelianGroup& g) :
        ShareableObject(),
        OM(g.OM), ON(g.ON), OMR(g.OMR), OMC(g.OMC), OMRi(g.OMRi), OMCi(g.OMCi),
        rankOM(g.rankOM),
        ornR(clonePtr(g.ornR)), ornC(clonePtr(g.ornC)),
        ornRi(clonePtr(g.ornRi)), ornCi(clonePtr(g.ornCi)),
        otR(clonePtr(g.otR)), otC(clonePtr(g.otC)),
        otRi(clonePtr(g.otRi)), otCi(clonePtr(g.otCi)),
        InvFacList(g.InvFacList), snfrank(g.snfrank),
        snffreeindex(g.snffreeindex),
        ifNum(g.ifNum), ifLoc(g.ifLoc), coeff(g.coeff), TORLoc(g.TORLoc),
        TORVec(g.TORVec), tensorIfLoc(g.tensorIfLoc),
        tensorIfNum(g.tensorIfNum), tensorInvFacList(g.tensorInvFacList) {
}

// destructor
inline NMarkedAbelianGroup::~NMarkedAbelianGroup() {
}

inline unsigned long NMarkedAbelianGroup::getTorsionRank(unsigned long degree)
        const {
    return getTorsionRank(NLargeInteger(degree));
}

inline unsigned long NMarkedAbelianGroup::getNumberOfInvariantFactors() const {
    return ifNum;
}

inline const NLargeInteger& NMarkedAbelianGroup::getInvariantFactor(
        unsigned long index) const {
    return InvFacList[index];
}

inline unsigned long NMarkedAbelianGroup::getRank() const {
    return snfrank;
}

inline unsigned long NMarkedAbelianGroup::minNumberOfGenerators() const {
    return snfrank + ifNum;
}

inline unsigned long NMarkedAbelianGroup::getRankCC() const {
    return OM.columns();
}

inline std::vector<NLargeInteger> NMarkedAbelianGroup::getSNFIsoRep(
        const std::vector<NLargeInteger>& v) const {
    return snfRep(v);
}

inline unsigned long NMarkedAbelianGroup::minNumberCycleGens() const {
    return OM.columns() - TORLoc;
}

inline bool NMarkedAbelianGroup::isTrivial() const {
    return ( (snfrank==0) && (InvFacList.size()==0) );
}

inline bool NMarkedAbelianGroup::equalTo(const NMarkedAbelianGroup& other)
        const {
    return ( (OM == other.OM) && (ON == other.ON) && (coeff == other.coeff) );
}

inline bool NMarkedAbelianGroup::operator == (
        const NMarkedAbelianGroup &other) const {
    return isIsomorphicTo(other);
}

inline bool NMarkedAbelianGroup::isIsomorphicTo(
        const NMarkedAbelianGroup &other) const {
    return ((InvFacList == other.InvFacList) && (snfrank == other.snfrank));
}

inline const NMatrixInt& NMarkedAbelianGroup::getMRB() const {
    return OMR;
}
inline const NMatrixInt& NMarkedAbelianGroup::getMRBi() const {
    return OMRi;
}
inline const NMatrixInt& NMarkedAbelianGroup::getMCB() const {
    return OMC;
}
inline const NMatrixInt& NMarkedAbelianGroup::getMCBi() const {
    return OMCi;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNRB() const {
    return *ornR;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNRBi() const {
    return *ornRi;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNCB() const {
    return *ornC;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNCBi() const {
    return *ornCi;
}

inline unsigned long NMarkedAbelianGroup::getRankM() const {
    return rankOM;
}
inline unsigned long NMarkedAbelianGroup::getFreeLoc() const {
    return snffreeindex;
}
inline unsigned long NMarkedAbelianGroup::getTorsionLoc() const {
    return ifLoc;
}

inline const NMatrixInt& NMarkedAbelianGroup::getM() const {
    return OM;
}
inline const NMatrixInt& NMarkedAbelianGroup::getN() const {
    return ON;
}
inline const NLargeInteger& NMarkedAbelianGroup::coefficients() const {
    return coeff;
}


// Inline functions for NHomMarkedAbelianGroup

inline NHomMarkedAbelianGroup::NHomMarkedAbelianGroup(
        const NMarkedAbelianGroup& dom,
        const NMarkedAbelianGroup& ran,
        const NMatrixInt &mat) :
        domain(dom), range(ran), matrix(mat),
        reducedMatrix(0), kernel(0), coKernel(0), image(0),
        reducedKernelLattice(0) {
}

inline NHomMarkedAbelianGroup::~NHomMarkedAbelianGroup() {
    if (reducedMatrix)
        delete reducedMatrix;
    if (kernel)
        delete kernel;
    if (coKernel)
        delete coKernel;
    if (image)
        delete image;
    if (reducedKernelLattice)
        delete reducedKernelLattice;
}

inline const NMarkedAbelianGroup& NHomMarkedAbelianGroup::getDomain() const {
    return domain;
}
inline const NMarkedAbelianGroup& NHomMarkedAbelianGroup::getRange() const {
    return range;
}
inline const NMatrixInt& NHomMarkedAbelianGroup::getDefiningMatrix() const {
    return matrix;
}

inline const NMatrixInt& NHomMarkedAbelianGroup::getReducedMatrix() const {
    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    return *reducedMatrix;
}

inline bool NHomMarkedAbelianGroup::isEpic() const {
    return getCokernel().isTrivial();
}

inline bool NHomMarkedAbelianGroup::isMonic() const {
    return getKernel().isTrivial();
}

inline bool NHomMarkedAbelianGroup::isIso() const {
    return isIsomorphism();
}

inline bool NHomMarkedAbelianGroup::isIsomorphism() const {
    return (getCokernel().isTrivial() && getKernel().isTrivial());
}

inline bool NHomMarkedAbelianGroup::isZero() const {
    return getImage().isTrivial();
}

inline const NMarkedAbelianGroup& NHomMarkedAbelianGroup::getKernel() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeKernel();
    return *kernel;
}

inline const NMarkedAbelianGroup& NHomMarkedAbelianGroup::getImage() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeImage();
    return *image;
}

inline const NMarkedAbelianGroup& NHomMarkedAbelianGroup::getCokernel() const {
    // Cast away const to compute the kernel -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeCokernel();
    return *coKernel;
}

} // namespace regina

#endif

