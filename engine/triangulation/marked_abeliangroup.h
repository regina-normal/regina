
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

/*! \file marked_abeliangroup.h
 *  \brief Deals with finitely generated abelian groups.
 */

#ifndef __MARKEDABELIANGROUP_H
#ifndef __DOXYGEN
#define __MARKEDABELIANGROUP_H
#endif

#include <vector>
#include "utilities/nmpi.h"
#include "utilities/nindexedarray.h"
#include "shareableobject.h"
#include "maths/nmatrixint.h"

namespace regina {

class NFile;
class NMatrixInt;

/**
 * \addtogroup algebra Algebraic Structures
 * Various algebraic structures.
 * @{
 */

/**
 * Represents a finitely generated abelian group given by a chain complex. (RBADD)
 *
 * This class is initialized with a chain complex.  The chain complex is given
 * in terms of two NMatrixInt classes, M and N such that M*N=0. The abelian group
 * is the kernel of M mod the image of N. It then allows one to
 * retrieve the invariant factors, the rank, and the corresponding vectors in
 * the kernel of M.  Moreover, given a vector in the kernel of M, it decribes
 * the homology class of the vector (the free part, and its position in the
 * invariant factors).  The purpose of this class is to allow one to not only
 * represent homology groups, but it gives the foundation for studying maps between
 * homology groups.  This is used in the computation of the torsion H_1 form
 * coming from Poincare Duality.
 * 
 * \testpart
 *
 * @author Ryan Budney
 * 
 * \todo \optlong Look at using sparse matrices for storage of SNF and
 * the like.
 */
class MarkedAbelianGroup : public ShareableObject{
    protected:
        NMatrixInt OM; // copy of initializing M
        NMatrixInt ON; // copy of initializing N assumes M*N == 0
        /** This stores the original differentials M and N respectively. */
        NMatrixInt OMR;  // OMC*OM*OMR is the SNF of OM
        NMatrixInt OMC;
        NMatrixInt OMRi; // OM = OMCi*SNF(OM)*OMRi
        NMatrixInt OMCi;
        unsigned long rankOM; // this is the index of the first zero entry
                                        // in the SNF of OM.
        /** This stores the info for SNF(OM) */
        NMatrixInt ORN; // this is the reducted ON matrix, ORN = [OMRi * ON] with
                        // where the brackets indicate removal of the first
                        // rankOM rows.
        NMatrixInt ornR;  
        NMatrixInt ornRi; // ornC * ORN * ornR is the SNF(ORN).
        NMatrixInt ornC;
        NMatrixInt ornCi;
        
        NMatrixInt SNF_ORN; // SNF(ORN)

        std::vector<NLargeInteger> InvFacList;  // list of invariant factors > 1
        NIndexedArray<unsigned long> InvFacIndex; // row indices of invariant factors
                                                  // this corresponds to InvFacList

        unsigned long snfrank;      // the number of free generators from SNF_ORN
        unsigned long snffreeindex; // the row index of the first free generator 
                                    //  in SNF_ORN
        unsigned long ifNum; // number of invariant factors
        unsigned long ifLoc; // row index of first invariant factor. not relevant
                                // if ifnum==0. 

    public:

        /**
         * MarkedAbelianGroups can only be created via a chain complex.
         * this assumes the product M*N=0, so among other things, it needs
         * to be well-defined, ie: M.rows()==N.columns().
         */
        MarkedAbelianGroup(const NMatrixInt& M, const NMatrixInt& N);
        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        MarkedAbelianGroup(const MarkedAbelianGroup& cloneMe);
        /**
         * Destroys the group.
         */
        virtual ~MarkedAbelianGroup();

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
         * See the MarkedAbelianGroup class notes for further details.
         *
         * @return the number of invariant factors.
         */
        unsigned long getNumberOfInvariantFactors() const;
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
        bool operator == (const MarkedAbelianGroup& other) const;

        /**
         * The text representation will be of the form
         * <tt>3 Z + 4 Z_2 + Z_120</tt>.
         * The torsion elements will be written in terms of the
         * invariant factors of the group, as described in the
         * MarkedAbelianGroup notes.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * The marked abelian group was defined by matrices M and N
         * with M*N==0.  Think of M as m by l and N as l by n.  Then
         * this routine returns the index-th free generator of the
         * ker(M)/img(N) in Z^l. 
         */
        std::vector<NLargeInteger> getFreeRep(unsigned long index) const;
        /**
         * The marked abelian group was defined by matrices M and N
         * with M*N==0.  Think of M as m by l and N as l by n.  Then
         * this routine returns the index-th torsion generator of the
         * ker(M)/img(N) in Z^l. 
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
         */
        std::vector<NLargeInteger> getSNFisoRep(std::vector<NLargeInteger>& element) 
                const;

        /**
         * Copies of the various internal change of basis matrices.
         */
        NMatrixInt getMRB() const; // these matrices are for putting OM in SNF
        NMatrixInt getMRBi() const;// getMCB() * OM * getMRB() = SNF(OM)
        NMatrixInt getMCB() const; // getMCBi() * SNF(OM) * getMRBi() = OM
        NMatrixInt getMCBi() const; 
                             // the truncated getMRBi() * ON is then put into SNF
        NMatrixInt getNRB() const; 
        NMatrixInt getNRBi() const;
        NMatrixInt getNCB() const; // getNCB() [ getMRBi()*ON ] getNRB() is SNF.
        NMatrixInt getNCBi() const;
        /**
         * Copies of the relevant data for interpreting the matrices.
         */
        unsigned long getRankOM() const; // number of non-zero entries in SNF(OM)
        unsigned long getFreeLoc() const;
        unsigned long getTorLoc() const;
        unsigned long getTorNum() const;
        /**
         * Copies of the original chain complex matrices.
         */
        NMatrixInt getOM() const;
        NMatrixInt getON() const;


};

/*@}*/

// Inline functions for MarkedAbelianGroup

// copy constructor
inline MarkedAbelianGroup::MarkedAbelianGroup(const MarkedAbelianGroup& g) :
        ShareableObject(), 
        OM(g.OM), ON(g.ON), OMR(g.OMR), OMC(g.OMC), OMRi(g.OMRi), OMCi(g.OMCi),
        rankOM(g.rankOM),
        ORN(g.ORN), ornR(g.ornR), ornRi(g.ornRi), ornC(g.ornC), ornCi(g.ornCi),
        SNF_ORN(g.SNF_ORN),
        InvFacList(g.InvFacList), InvFacIndex(g.InvFacIndex),
        snfrank(g.snfrank), snffreeindex(g.snffreeindex),
        ifNum(g.ifNum), ifLoc(g.ifLoc)
 {}

// destructor
inline MarkedAbelianGroup::~MarkedAbelianGroup() {}

inline        NMatrixInt MarkedAbelianGroup::getMRB() const { return OMR; }
inline        NMatrixInt MarkedAbelianGroup::getMRBi() const { return OMRi; }
inline        NMatrixInt MarkedAbelianGroup::getMCB() const { return OMC; }
inline        NMatrixInt MarkedAbelianGroup::getMCBi() const { return OMCi; }
inline        NMatrixInt MarkedAbelianGroup::getNRB() const { return ornR; }
inline        NMatrixInt MarkedAbelianGroup::getNRBi() const { return ornRi; }
inline        NMatrixInt MarkedAbelianGroup::getNCB() const { return ornC; }
inline        NMatrixInt MarkedAbelianGroup::getNCBi() const { return ornCi; }

inline        unsigned long MarkedAbelianGroup::getRankOM() const { return rankOM; }
inline        unsigned long MarkedAbelianGroup::getFreeLoc() const { return snffreeindex; }
inline        unsigned long MarkedAbelianGroup::getTorLoc() const { return ifLoc; }
inline        unsigned long MarkedAbelianGroup::getTorNum() const { return InvFacList.size(); }

inline        NMatrixInt MarkedAbelianGroup::getOM() const { return OM; }
inline        NMatrixInt MarkedAbelianGroup::getON() const { return ON; }



/** These are intended to be internal routines, although they could be used for
  *  other purposes. RBMOD_smithNormalForm is a modification of the smithNormalForm
  *  algorithm, it returns the appropriate change-of-basis matrices corresponding to
  *  all the row and column operations performed in the process of constructing the
  *  smithNormalForm.  RBADD_columnEchelonForm puts a matrix into reduced column
  *  echelon form with respect to a certain submatrix specified by rowList. This
  *  is used in the RBADD_preImageOfLattice algorithm, which computes the kernel
  *  of a homomorphism Z^n --> Z_p1 + Z_p2 + ... + Z_pk specified by a matrix.
  */
void RBMOD_smithNormalForm(NMatrixInt& matrix, 
        NMatrixInt& RowSpaceBasis, NMatrixInt& RowSpaceBasisInv,
        NMatrixInt& ColSpaceBasis, NMatrixInt& ColSpaceBasisInv);

void RBADD_columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri, 
                                std::vector<unsigned> &rowList);

NMatrixInt RBADD_preImageOfLattice(NMatrixInt& hom, std::vector<NLargeInteger>& L);


/**
 * Represents a homomorphism of finitely generated abelian groups. (RBADD)
 * One initializes a homomorphism of f.g. abelian groups by passing the constructor
 * two f.g. abelian groups and a matrix which describes the linear map between the
 * free abelian groups in the centre of the respective chain complexes that you
 * used when defining the f.g. abelian groups.  So for example, if dom was initialized
 * by the chain complex  Z^a -- A --> Z^b -- B --> Z^c and ran was initialized by
 * Z^d -- D --> Z^e -- E --> Z^f, then mat needs to be an e-by-b matrix, and of course,
 * you only get something that's well-defined if mat extends to a chain map, which
 * this function assumes. 
 *
 * \testpart
 *
 * @author Ryan Budney
 * 
 */

class HomMarkedAbelianGroup {
    protected:
        
        MarkedAbelianGroup domain;
        MarkedAbelianGroup range;
        NMatrixInt matrix;

        bool reducedMatrixComputed;
        NMatrixInt* reducedMatrix;
        bool kernelComputed;
        MarkedAbelianGroup* kernel; 
        bool coKernelComputed;
        MarkedAbelianGroup* coKernel; 
        bool imageComputed;
        MarkedAbelianGroup* image;
        bool reducedKernelLatticeComputed;
        NMatrixInt* reducedKernelLattice;

        void computeReducedKernelLattice();
        void computeReducedMatrix();
        void computeKernel();
        void computeCoKernel(); 
        void computeImage(); 

    public:

//       * Homomorphisms of marked abelian groups are generally created from
//         * two marked abelian groups and a matrix that indicates where the generators
//         * are sent.
        HomMarkedAbelianGroup(const MarkedAbelianGroup& dom, 
                               const MarkedAbelianGroup& ran,
                              const NMatrixInt &mat);
//         * Creates a clone of the given group.
//         *
//         * @param cloneMe the group to clone.
        HomMarkedAbelianGroup(const HomMarkedAbelianGroup& g);
//         * Destroys the group.
        virtual ~HomMarkedAbelianGroup();

        bool isEpic();
        bool isMonic();
        bool isIso();
        bool isZero();

        MarkedAbelianGroup getKernel();
        MarkedAbelianGroup getCoKernel();
        MarkedAbelianGroup getImage();

        /**
         * Short text representation: this will state if the
         * map is an isomorphism or not, if monoid or epic, and
         * if it is not monoic, describes kernel, not epic, describes
         * co-kernel and image.
         */
        virtual void writeTextShort(std::ostream& out);

        void writeRedMatrix(std::ostream& out);

        NMatrixInt getDefiningMatrix();
        NMatrixInt getRedMatrix();
};


inline HomMarkedAbelianGroup::~HomMarkedAbelianGroup() 
 {
 if (reducedMatrixComputed) { delete reducedMatrix; }
 if (kernelComputed) { delete kernel; }
 if (coKernelComputed) { delete coKernel; }
 if (imageComputed) { delete image; }
 if (reducedKernelLatticeComputed) { delete reducedKernelLattice; }
 }

inline NMatrixInt HomMarkedAbelianGroup::getDefiningMatrix()
 { return matrix; }

inline NMatrixInt HomMarkedAbelianGroup::getRedMatrix()
 { computeReducedMatrix();
   return *reducedMatrix; }



} // namespace regina

#endif

