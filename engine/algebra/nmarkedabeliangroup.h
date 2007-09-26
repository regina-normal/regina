
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
#include "utilities/nmpi.h"
#include "utilities/nindexedarray.h"
#include "shareableobject.h"
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
 * in terms of two NMatrixInt classes, M and N such that M*N=0. The abelian
 * group is the kernel of M mod the image of N. It then allows one to
 * retrieve the invariant factors, the rank, and the corresponding vectors in
 * the kernel of M.  Moreover, given a vector in the kernel of M, it decribes
 * the homology class of the vector (the free part, and its position in the
 * invariant factors).  The purpose of this class is to allow one to not only
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
    protected:
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
        /** Internal list of indices of invariant factors. */
        NIndexedArray<unsigned long> InvFacIndex;
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
         * NMarkedAbelianGroups can only be created via a chain complex.
         * this assumes the product M*N=0, so among other things, it needs
         * to be well-defined, ie: M.rows()==N.columns().
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
         * torsion elements of this group.
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
         * @param out is the stream to write to.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * The marked abelian group was defined by matrices M and N
         * with M*N==0.  Think of M as m by l and N as l by n.  Then
         * this routine returns the index-th free generator of the
         * ker(M)/img(N) in Z^l.
         *
         * @param index specifies which free generator we're looking up.
         * @return vector<NLargeInteger> gives the coordinates of the
         * free generator in the nullspace of M
         */
        std::vector<NLargeInteger> getFreeRep(unsigned long index) const;

        /**
         * The marked abelian group was defined by matrices M and N
         * with M*N==0.  Think of M as m by l and N as l by n.  Then
         * this routine returns the index-th torsion generator of the
         * ker(M)/img(N) in Z^l.
         *
         * @param index specifies which generator in the torsion subgroup
         * @return vector<NLargeInteger>  gives the coordinates of
         * the generator in the nullspace of M
         */
        std::vector<NLargeInteger> getTorRep(unsigned long index) const;

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
         * @param element is a vector in the nullspace of M
         * @return a vector that describes element in the standard
         *  Z^d + Z_{d1} + ... + Z_{dk} form.
         */
        std::vector<NLargeInteger> getSNFisoRep(
            std::vector<NLargeInteger>& element) const;

        /**
         * These routines return information on how we determined the
         * isomorphism-class of the group.  Originally, the group was defined
         * by two matrices M and N with M*N==0.
         *
         * getMCB() * M * getMRB() is the Smith Normal Form of M.
         * getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         * respectively.
         *
         * @return the matrix getMRB() described above.
         */
        NMatrixInt getMRB() const; // these matrices are for putting OM in SNF
        /**
         * These routines return information on how we determined the
         * isomorphism-class of the group.  Originally, the group was defined
         * by two matrices M and N with M*N==0.
         *
         * getMCB() * M * getMRB() is the Smith Normal Form of M.
         * getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         * respectively.
         *
         * @return the matrix getMRBi() described above.
         */
        NMatrixInt getMRBi() const;// getMCB() * OM * getMRB() = SNF(OM)
        /**
         * These routines return information on how we determined the
         * isomorphism-class of the group.  Originally, the group was defined
         * by two matrices M and N with M*N==0.
         *
         * getMCB() * M * getMRB() is the Smith Normal Form of M.
         * getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         * respectively.
         *
         * @return the matrix getMCB() described above.
         */
        NMatrixInt getMCB() const;
        /**
         * These routines return information on how we determined the
         * isomorphism-class of the group.  Originally, the group was defined
         * by two matrices M and N with M*N==0.
         *
         * getMCB() * M * getMRB() is the Smith Normal Form of M.
         * getMCBi() and getMRBi() are the inverses of getMCB() and getMRB()
         * respectively.
         *
         * @return the matrix getMCBi() described above.
         */
        NMatrixInt getMCBi() const;

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
        NMatrixInt getNRB() const;

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
        NMatrixInt getNRBi() const;

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
        NMatrixInt getNCB() const;

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
        NMatrixInt getNCBi() const;

        /**
         * Gives the rank of the defining matrix M.
         *
         * @return the rank of the defining matrix M.
         */
        unsigned long getRankOM() const;

        /**
         * Index of first free generator in the reduced N matrix.
         *
         * @return index of the first free generator in reduced N matrix.
         */
        unsigned long getFreeLoc() const;// internal: snffreeindex

        /**
         * Index of the first torsion generator in the reduced N matrix.
         *
         * @return Index of the first torsion generator in the reduced N matrix.
         */
        unsigned long getTorLoc() const; // internal: ifLoc

        /**
         * Number of invariant factors ie: minimal number of torsion generators.
         *
         * @return number of invariant factors in torsion subgroup.
         */
        unsigned long getTorNum() const; // internal: InvFacList.size()

        /**
         * Copies of the original chain complex matrices.
         *
         * @return a copy of the defining matrix M.
         */
        NMatrixInt getOM() const;
        /**
         * Copies of the original chain complex matrices.
         *
         * @return a copy of the defining matrix N.
         */
        NMatrixInt getON() const;
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
        InvFacList(g.InvFacList), InvFacIndex(g.InvFacIndex),
        snfrank(g.snfrank), snffreeindex(g.snffreeindex),
        ifNum(g.ifNum), ifLoc(g.ifLoc) {
}

// destructor
inline NMarkedAbelianGroup::~NMarkedAbelianGroup() {}

inline NMatrixInt NMarkedAbelianGroup::getMRB() const {
    return OMR;
}
inline NMatrixInt NMarkedAbelianGroup::getMRBi() const {
    return OMRi;
}
inline NMatrixInt NMarkedAbelianGroup::getMCB() const {
    return OMC;
}
inline NMatrixInt NMarkedAbelianGroup::getMCBi() const {
    return OMCi;
}
inline NMatrixInt NMarkedAbelianGroup::getNRB() const {
    return ornR;
}
inline NMatrixInt NMarkedAbelianGroup::getNRBi() const {
    return ornRi;
}
inline NMatrixInt NMarkedAbelianGroup::getNCB() const {
    return ornC;
}
inline NMatrixInt NMarkedAbelianGroup::getNCBi() const {
    return ornCi;
}

inline unsigned long NMarkedAbelianGroup::getRankOM() const {
    return rankOM;
}
inline unsigned long NMarkedAbelianGroup::getFreeLoc() const {
    return snffreeindex;
}
inline unsigned long NMarkedAbelianGroup::getTorLoc() const {
    return ifLoc;
}
inline unsigned long NMarkedAbelianGroup::getTorNum() const {
    return InvFacList.size();
}

inline NMatrixInt NMarkedAbelianGroup::getOM() const {
    return OM;
}
inline NMatrixInt NMarkedAbelianGroup::getON() const {
    return ON;
}



/**
 * These are intended to be internal routines, although they could be used for
 * other purposes. RBMOD_smithNormalForm is a modification of the
 * smithNormalForm algorithm, it returns the appropriate change-of-basis
 * matrices corresponding to all the row and column operations performed in
 * the process of constructing the smithNormalForm.  RBADD_columnEchelonForm
 * puts a matrix into reduced column echelon form with respect to a certain
 * submatrix specified by rowList. This is used in the RBADD_preImageOfLattice
 * algorithm, which computes the kernel of a homomorphism
 * Z^n --> Z_p1 + Z_p2 + ... + Z_pk specified by a matrix.
 *
 * @param matrix is the original m x n matrix to put into Smith Normal Form.
 * When the algorithm terminates, it *is* in its Smith Normal Form.
 * @param RowSpaceBasis change of basis matrix
 * @param RowSpaceBasisInv inverse of RowSpaceBasis
 * @param ColSpaceBasis change of basis matrix
 * @param ColSpaceBasisInv invase of ColSpaceBasis
 * when the algorithm terminates, the original matrix is equal to
 * ColSpaceBasisInv * matrix * RowSpaceBasisInv.
 * thus, one obtains matrix from the old matrix by multiplying on the
 * left by ColSpaceBasis and on the right by RowSpaceBasis.
 */
void RBMOD_smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& RowSpaceBasis, NMatrixInt& RowSpaceBasisInv,
        NMatrixInt& ColSpaceBasis, NMatrixInt& ColSpaceBasisInv);

/**
 * Modification of RBMOD_smithNormalForm to only put the matrix in Column
 * Echelon form with respect to a collection of rows.
 *
 * @param M matrix to reduce
 * @param R row-reduction matrix
 * @param Ri the inverse of R
 * @param rowList the rows to pay attention to.
 */
void RBADD_columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
        std::vector<unsigned> &rowList);

/**
 * Given a homomorphism (hom) from Z^n to Z^m and a sublattice of Z^m
 * represented by L, this procedure computes the preimage of L under hom.
 *
 * @param hom the matrix representing the homomorphism from Z^n to Z^m
 * @param L the sublattice of Z^m
 * @return a matrix whose columns are a basis for the preimage lattice.
 */
NMatrixInt RBADD_preImageOfLattice(NMatrixInt& hom, std::vector<NLargeInteger>& L);


/**
 * Represents a homomorphism of finitely generated abelian groups.
 * One initializes a homomorphism of f.g. abelian groups by passing the
 * constructor two f.g. abelian groups and a matrix which describes the
 * linear map between the free abelian groups in the centre of the respective
 * chain complexes that you used when defining the f.g. abelian groups.  So
 * for example, if dom was initialized by the chain complex
 * Z^a -- A --> Z^b -- B --> Z^c and ran was initialized by
 * Z^d -- D --> Z^e -- E --> Z^f, then mat needs to be an e-by-b matrix, and
 * of course, you only get something that's well-defined if mat extends to
 * a chain map, which this function assumes.
 *
 * @author Ryan Budney
 */

class NHomMarkedAbelianGroup : public ShareableObject {
    protected:
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

//       * Homomorphisms of marked abelian groups are generally created from
//       * two marked abelian groups and a matrix that indicates where the
//       * generators are sent.
        /**
         * This is the sole NHomMarkedAbelianGroup constructor, other than
         * the copy constructor.
         *
         * @param dom domain group
         * @param ran range group
         * @param mat the matrix that describes the homomorphism from dom
         * to ran, given in the chain-complex coordinates.
         */
        NHomMarkedAbelianGroup(const NMarkedAbelianGroup& dom,
                const NMarkedAbelianGroup& ran,
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
         * Is it epic? query.
         *
         * @return true if homomorphism is epic
         */
        bool isEpic() const;
        /**
         * Is it monic? query
         *
         * @return true if homomorphism is monic
         */
        bool isMonic() const;
        /**
         * Is it an isomorphism? query
         *
         * @return true if homomorphism is an isomorphism
         */
        bool isIso() const;
        /**
         * Is it the zero map? query.
         *
         * @return true if homomorphism is the zero map
         */
        bool isZero() const;

        /**
         * Computes the kernel.
         *
         * @return the kernel of the homomorphism, as a marked abelian group
         */
        NMarkedAbelianGroup getKernel() const;
        /**
         * Computes the cokernel.
         *
         * @return cokernel of the homomorphism, as a marked abelian group
         */
        NMarkedAbelianGroup getCoKernel() const;
        /**
         * Computes the image.
         *
         * @return image of the homomorphism, as a marked abelian group
         */
        NMarkedAbelianGroup getImage() const;

        /**
         * Short text representation: this will state if the
         * map is an isomorphism or not, if monoid or epic, and
         * if it is not monoic, describes kernel, not epic, describes
         * co-kernel and image.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Writes a text-readable version of the reduced matrix, this is
         * a description of the homomorphism in some specific coordinates
         * at present only meant to be internal to NHomMarkedAbelianGroup.
         *
         * @param out is the output stream
         */
        void writeRedMatrix(std::ostream& out) const;

        /**
         * Returns the defining matrix for the homomorphism.
         *
         * @return the matrix which was used to define the homomorphism.
         */
        NMatrixInt getDefiningMatrix() const;
        /**
         * Gets the internal reduced matrix representing the
         * homomorphism.
         *
         * @return a copy of the internal representation of the homomorphism.
         */
        NMatrixInt getRedMatrix() const;
};


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

inline NMatrixInt NHomMarkedAbelianGroup::getDefiningMatrix() const {
    return matrix;
}

inline NMatrixInt NHomMarkedAbelianGroup::getRedMatrix() const {
    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    return *reducedMatrix;
}



} // namespace regina

#endif

