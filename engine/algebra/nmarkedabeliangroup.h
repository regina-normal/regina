
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file nmarkedabeliangroup.h
 *  \brief Deals with abelian groups given by chain complexes.
 */

#ifndef __NMARKEDABELIANGROUP_H
#ifndef __DOXYGEN
#define __NMARKEDABELIANGROUP_H
#endif

#include <vector>
#include "maths/nmatrixint.h"

namespace regina {

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
 * where a=N.columns(), M.columns()=b=N.rows(), and c=M.rows(). 
 *
 * This class allows one to retrieve the invariant factors, the rank, and
 * the corresponding vectors in the kernel of \a M.  Moreover, given a
 * vector in the kernel of \a M, it decribes the homology class of the
 * vector (the free part, and its position in the invariant factors).
 *
 * The purpose of this class is to allow one to not only
 * represent homology groups, but it gives the foundation for studying maps
 * between homology groups.  This is used in the computation of the torsion
 * H_1 form coming from Poincare Duality.
 *
 * @author Ryan Budney
 *
 * \todo \optlong Look at using sparse matrices for storage of SNF and
 * the like.
 */
class NMarkedAbelianGroup : public ShareableObject {
    private:
        /** Internal original M */
        NMatrixInt OM; // copy of initializing M
        /** Internal original N */
        NMatrixInt ON; // copy of initializing N assumes M*N == 0
        /** Internal change of basis */
        NMatrixInt OMR;  // OMC*OM*OMR is the SNF of OM
        /** Internal change of basis */
        NMatrixInt OMC;
        /** Internal change of basis */
        NMatrixInt OMRi; // OM = OMCi*SNF(OM)*OMRi
        /** Internal change of basis */
        NMatrixInt OMCi;
        /** Internal rank of M */
        unsigned long rankOM; // this is the index of the first zero entry
                              // in the SNF of OM.
        /** Internal reduced N matrix */
        NMatrixInt ORN; // this is the reducted ON matrix, ORN = [OMRi * ON]
                        // where the brackets indicate removal of the
                        // first rankOM rows.
        /** Internal change of basis */
        NMatrixInt ornR;
        /** Internal change of basis */
        NMatrixInt ornRi; // ornC * ORN * ornR is the SNF(ORN).
        /** Internal change of basis */
        NMatrixInt ornC;
        /** Internal change of basis */
        NMatrixInt ornCi;
        /** Internal SNF of reduced N matrix */
        NMatrixInt SNF_ORN;

        /** Internal list of invariant factors. */
        std::vector<NLargeInteger> InvFacList;
        /** The number of free generators, from SNF_ORN */
        unsigned long snfrank;
        /** The row index of the first free generator in SNF_ORN. */
        unsigned long snffreeindex;
        /** Number of invariant factors. */
        unsigned long ifNum;
        /** Row index of invariant factors in SNF_ORN */
        unsigned long ifLoc;


    public:

        /**
         * Creates a marked abelian group from a chain complex.
         * See the class notes for details.
         *
         * \pre M.columns() = N.rows().
         * \pre The product M*N = 0.
         *
         * @param M `right' matrix in chain complex
         * @param N `left' matrix in chain complex
         */
        NMarkedAbelianGroup(const NMatrixInt& M, const NMatrixInt& N);

        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        NMarkedAbelianGroup(const NMarkedAbelianGroup& cloneMe);

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
         * torsion elements of this group.  This is the minimal number
         * of torsion generators.
         * See the NMarkedAbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        unsigned long getNumberOfInvariantFactors() const;

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
         * @param other the group with which this should be compared.
         * @return \c true if and only if the two groups are isomorphic.
         */
        bool operator == (const NMarkedAbelianGroup& other) const;

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
         * Returns the requested free generator.
         *
         * As described in the class overview, this marked abelian group
         * is defined by matrices \a M and \a N where M*N = 0.
         * If \a M is an \a m by \a l matrix and \a N is an \a l by \a n
         * matrix, then this routine returns the (\a index)th free
         * generator of ker(M)/img(N) in \a Z^l.
         *
         * \ifacespython The return value will be a python list.
         *
         * @param index specifies which free generator to look up;
         * this must be between 0 and getRank()-1 inclusive.
         * @return the coordinates of the free generator in the nullspace of
         * \a M; this vector will have length M.columns() (or
         * equivalently, N.rows()).
         */
        std::vector<NLargeInteger> getFreeRep(unsigned long index) const;

        /**
         * Returns the requested generator of the torsion subgroup.
         *
         * As described in the class overview, this marked abelian group
         * is defined by matrices \a M and \a N where M*N = 0.
         * If \a M is an \a m by \a l matrix and \a N is an \a l by \a n
         * matrix, then this routine returns the (\a index)th torsion
         * generator of ker(M)/img(N) in \a Z^l.
         *
         * \ifacespython The return value will be a python list.
         *
         * @param index specifies which generator in the torsion subgroup;
         * this must be at least 0 and strictly less than the number of
         * non-trivial invariant factors.
         * @return the coordinates of the generator in the nullspace of
         * \a M; this vector will have length M.columns() (or
         * equivalently, N.rows()).
         */
        std::vector<NLargeInteger> getTorsionRep(unsigned long index) const;

        /**
         * The marked abelian group was defined by matrices M and N
         * with M*N==0.  Think of M as m by l and N as l by n.
         * When the group was initialized, it was computed to be isomorphic
         * to some Z^d + Z_{d1} + ... + Z_{dk} where d1 | d2 | ... | dk
         * this routine assumes element is in Z^l, and it returns a vector
         * of length d+k where the first d elements represent which class the
         * vector projects to in Z^d, and the last k elements represent the
         * projections to Z_{d1} + ... + Z_{dk}. Of these last elements, they
         * will be returned mod di respectively.
         *
         * \pre The vector \a element has precisely M.columns() entries
         * (or equivalently, N.rows() entries).
         *
         * \ifacespython Both \a element and the return value are python lists.
         *
         * @param element a vector of length M.columns().
         * @return a vector that describes element in the standard
         * Z^d + Z_{d1} + ... + Z_{dk} form, or the empty vector if
         * \a element is not in the kernel of \a M.
         */
        std::vector<NLargeInteger> getSNFIsoRep(
            const std::vector<NLargeInteger>& element) const;

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
         * @return the matrix getMCBi() as described above.
         */
        const NMatrixInt& getMCBi() const;

        /**
         * The 2nd step in determining the isomorphism class of the group is to
         * look at the matrix getMRBi()*N. Some of its columns are zero, so we
         * truncate this matrix and denote it by [getMRBi()*N)].  Now we put
         * this truncated matrix into Smith Normal Form, and keep track of
         * the matrices that do it.
         *
         * getNCB()*[getMRBi()*N]*getNRB() is the SNF of [getMRBi()*N].
         * getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         * respectively.
         *
         * @return the matrix getNRB() described above.
         */
        const NMatrixInt& getNRB() const;

        /**
         * The 2nd step in determining the isomorphism class of the group is to
         * look at the matrix getMRBi()*N. Some of its columns are zero, so we
         * truncate this matrix and denote it by [getMRBi()*N)].  Now we put
         * this truncated matrix into Smith Normal Form, and keep track of
         * the matrices that do it.
         *
         * getNCB()*[getMRBi()*N]*getNRB() is the SNF of [getMRBi()*N].
         * getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         * respectively.
         *
         * @return the matrix getNRBi() described above.
         */
        const NMatrixInt& getNRBi() const;

        /**
         * The 2nd step in determining the isomorphism class of the group is to
         * look at the matrix getMRBi()*N. Some of its columns are zero, so we
         * truncate this matrix and denote it by [getMRBi()*N)].  Now we put
         * this truncated matrix into Smith Normal Form, and keep track of
         * the matrices that do it.
         *
         * getNCB()*[getMRBi()*N]*getNRB() is the SNF of [getMRBi()*N].
         * getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         * respectively.
         *
         * @return the matrix getNCB() described above.
         */
        const NMatrixInt& getNCB() const;

        /**
         * The 2nd step in determining the isomorphism class of the group is to
         * look at the matrix getMRBi()*N. Some of its columns are zero, so we
         * truncate this matrix and denote it by [getMRBi()*N)].  Now we put
         * this truncated matrix into Smith Normal Form, and keep track of
         * the matrices that do it.
         *
         * getNCB()*[getMRBi()*N]*getNRB() is the SNF of [getMRBi()*N].
         * getNCBi() and getNRBi() are the inverses of getNCB() and getNRB()
         * respectively.
         *
         * @return the matrix getNCBi() described above.
         */
        const NMatrixInt& getNCBi() const;

        /**
         * Returns the rank of the defining matrix \a M.
         *
         * The matrix \a M is the `right' matrix used in defining the chain
         * complex.  See the class overview for further details.
         *
         * @return the rank of the defining matrix \a M.
         */
        unsigned long getRankM() const;

        /**
         * Returns the index of the first free generator in the reduced
         * \a N matrix.
         *
         * @return the index of the first free generator.
         */
        unsigned long getFreeLoc() const; // internal: snffreeindex

        /**
         * Returns the index of the first torsion generator in the
         * reduced \a N matrix.
         *
         * @return the index of the first torsion generator.
         */
        unsigned long getTorsionLoc() const; // internal: ifLoc

        /**
         * Returns the `right' matrix used in defining the chain complex.
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
         *
         * This is a copy of the matrix \a N that was originally passed to the
         * class constructor.  See the class overview for further details on
         * matrices \a M and \a N and their roles in defining the chain complex.
         *
         * @return a reference to the defining matrix N.
         */
        const NMatrixInt& getN() const;
};

/**
 * Represents a homomorphism of finitely generated abelian groups.
 *
 * One initializes a homomorphism of f.g. abelian groups by passing the
 * constructor two f.g. abelian groups and a matrix which describes the
 * linear map between the free abelian groups in the centre of the respective
 * chain complexes that you used when defining the f.g. abelian groups.
 *
 * So for example, if the domain was initialized by the chain complex
 * <tt>Z^a --A--> Z^b --B--> Z^c</tt> and the range was initialized by
 * <tt>Z^d --D--> Z^e --E--> Z^f</tt>, then mat needs to be an e-by-b matrix,
 * and of course, you only get something that's well-defined if mat extends to
 * a chain map, which this function assumes.
 *
 * @author Ryan Budney
 */
class NHomMarkedAbelianGroup : public ShareableObject {
    private:
        /** domain */
        NMarkedAbelianGroup domain;
        /** range */
        NMarkedAbelianGroup range;
        /** matrix describing map from domain to range, in the coordinates
            of the chain complexes used to construct domain and range */
        NMatrixInt matrix;

        /** short description of matrix in SNF coordinates */
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
        /** compute the CoKernel */
        void computeCoKernel();
        /** compute the Image */
        void computeImage();

    public:

        /**
         * Constructs a homomorphism from two marked abelian groups and
         * a matrix that indicates where the generators are sent.
         * This is the sole NHomMarkedAbelianGroup constructor, other than
         * the copy constructor.
         *
         * The matrix must be given in the chain-complex coordinates.
         * If \a domain was defined via the chain complex
         * <tt>Z^a --N1--> Z^b --M1--> Z^c</tt>, and \a range was
         * defined via <tt>>Z^d --N2--> Z^e --M2--> Z^f</tt>, then \a mat is
         * a matrix that describes a homomorphism from Z^b to Z^e
         * (an e-by-b matrix).
         *
         * In order for this to make sense as a homomorphism of the groups
         * represented by \a domain and \a range respectively, one requires
         * img(mat*N1) to be a subset of img(N2).  This is not checked, but
         * assumed.  Similarly, ker(M1) must be sent into ker(M2).
         *
         * \pre The matrix \a has the required dimensions and sends
         * img(mat*N1) and ker(M1) into img(N2) and ker(M2) respectively,
         * as described above.
         *
         * @param domain the domain group.
         * @param range the range group.
         * @param mat the matrix that describes the homomorphism from \a domain
         * to \a range.
         */
        NHomMarkedAbelianGroup(const NMarkedAbelianGroup& domain,
                const NMarkedAbelianGroup& range,
                const NMatrixInt &mat);
        /**
         * Copy constructor.
         * @param g object to copy
         */
        NHomMarkedAbelianGroup(const NHomMarkedAbelianGroup& g);
        /**
         * Destructor.
         */
        virtual ~NHomMarkedAbelianGroup();

        /**
         * Is this group epic?
         *
         * @return true if homomorphism is epic.
         */
        bool isEpic() const;
        /**
         * Is this group monic?
         *
         * @return true if homomorphism is monic.
         */
        bool isMonic() const;
        /**
         * Is this group an isomorphism?
         *
         * @return true if homomorphism is an isomorphism.
         */
        bool isIso() const;
        /**
         * Is this group the zero map?
         *
         * @return true if homomorphism is the zero map.
         */
        bool isZero() const;

        /**
         * Computes the kernel.
         *
         * @return the kernel of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getKernel() const;
        /**
         * Computes the cokernel.
         *
         * @return cokernel of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getCoKernel() const;
        /**
         * Computes the image.
         *
         * @return image of the homomorphism, as a marked abelian group.
         */
        const NMarkedAbelianGroup& getImage() const;

        /**
         * Short text representation: this will state if the
         * map is an isomorphism or not, if monoid or epic, and
         * if it is not monoic, describes kernel, not epic, describes
         * co-kernel and image.
         */
        virtual void writeTextShort(std::ostream& out) const;

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
         * @return the matrix which was used to define the homomorphism.
         */
        const NMatrixInt& getDefiningMatrix() const;

        /**
         * For internal use only.
         *
         * Gets the internal reduced matrix representing the
         * homomorphism.
         *
         * @return a copy of the internal representation of the homomorphism.
         */
        const NMatrixInt& getReducedMatrix() const;

        /**
         * For internal use only.
         *
         * Writes a text-readable version of the reduced matrix, this is
         * a description of the homomorphism in some specific coordinates
         * at present only meant to be internal to NHomMarkedAbelianGroup.
         *
         * \ifacespython The \a out argument is missing; instead this is
         * assumed to be standard output.
         *
         * @param out the output stream.
         */
        void writeReducedMatrix(std::ostream& out) const;
};

/*@}*/

// Inline functions for NMarkedAbelianGroup

// copy constructor
inline NMarkedAbelianGroup::NMarkedAbelianGroup(const NMarkedAbelianGroup& g) :
        ShareableObject(),
        OM(g.OM), ON(g.ON), OMR(g.OMR), OMC(g.OMC), OMRi(g.OMRi), OMCi(g.OMCi),
        rankOM(g.rankOM),
        ORN(g.ORN), ornR(g.ornR), ornRi(g.ornRi), ornC(g.ornC), ornCi(g.ornCi),
        SNF_ORN(g.SNF_ORN),
        InvFacList(g.InvFacList), snfrank(g.snfrank),
        snffreeindex(g.snffreeindex),
        ifNum(g.ifNum), ifLoc(g.ifLoc) {
}

// destructor
inline NMarkedAbelianGroup::~NMarkedAbelianGroup() {}

inline unsigned NMarkedAbelianGroup::getTorsionRank(unsigned long degree)
        const {
    return getTorsionRank(NLargeInteger(degree));
}

inline unsigned long NMarkedAbelianGroup::getNumberOfInvariantFactors() const {
    return InvFacList.size();
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
    return ornR;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNRBi() const {
    return ornRi;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNCB() const {
    return ornC;
}
inline const NMatrixInt& NMarkedAbelianGroup::getNCBi() const {
    return ornCi;
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

// Inline functions for NHomMarkedAbelianGroup

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

} // namespace regina

#endif

