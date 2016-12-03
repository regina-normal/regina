
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

#include "algebra/markedabeliangroup.h"
#include "maths/matrixops.h"
#include "utilities/stringutils.h"
#include <iostream>

namespace regina {

MarkedAbelianGroup::MarkedAbelianGroup(unsigned long rk, 
                                         const Integer &p) : 
    OM(rk, rk), ON(rk,rk), OMR(rk,rk), OMC(rk,rk), OMRi(rk, rk), OMCi(rk, rk), 
    rankOM(0), InvFacList(0), snfrank(0), snffreeindex(0), ifNum(0), ifLoc(0), 
    coeff(Integer::zero), TORLoc(0), TORVec(0), tensorIfLoc(0), 
    tensorIfNum(0), tensorInvFacList(0)
{
    // special case p==1 trivial group
    ornR.reset(new MatrixInt(rk, rk)); ornRi.reset(new MatrixInt(rk, rk)); 
    ornC.reset(new MatrixInt(rk, rk)); ornCi.reset(new MatrixInt(rk, rk));
    for (unsigned long i=0; i<rk; i++) ON.entry(i,i) = p;
    // everything is already in SNF, so these are identity matrices
    OMR.makeIdentity();OMC.makeIdentity(); 
    OMRi.makeIdentity();OMCi.makeIdentity(); 
    ornR->makeIdentity(); ornRi->makeIdentity(); 
    ornC->makeIdentity(); ornCi->makeIdentity(); 
    if ( (p != 0 ) && ( p != 1 ) ) ifNum=rk;
    if (ifNum != 0) InvFacList.resize(ifNum);
    for (unsigned long i=0; i<InvFacList.size(); i++) InvFacList[i] = p;
    if ( p != 1 ) snfrank = rk - ifNum;
}

MarkedAbelianGroup::MarkedAbelianGroup(const MatrixInt& M,
        const MatrixInt& N) :
    OM(M), ON(N), OMR(M.columns(),M.columns()),
    OMC(M.rows(),M.rows()), OMRi(M.columns(),M.columns()),
    OMCi(M.rows(),M.rows()),
    rankOM(0),
    InvFacList(0), snfrank(0), snffreeindex(0), ifNum(0), ifLoc(0), 
    coeff(Integer::zero), TORLoc(0), TORVec(0), tensorIfLoc(0), 
    tensorIfNum(0), tensorInvFacList(0)
{
    MatrixInt tM(M);

    metricalSmithNormalForm(tM, &OMR, &OMRi, &OMC, &OMCi);

    for (unsigned long i=0; (i<tM.rows()) && (i<tM.columns()); i++)
        if (tM.entry(i,i) != 0) rankOM++;
    TORLoc = rankOM; // to keep mod-p calculations happy. 

    // construct the internal presentation matrix.
    std::unique_ptr<MatrixRing<Integer> > prod=OMRi*ON;
    MatrixInt ORN(N.rows()-rankOM, N.columns());
    ornR.reset( new MatrixInt( ORN.columns(), ORN.columns() ) );
    ornRi.reset(new MatrixInt( ORN.columns(), ORN.columns() ) );
    ornC.reset( new MatrixInt( ORN.rows(), ORN.rows() ) );
    ornCi.reset(new MatrixInt( ORN.rows(), ORN.rows() ) );

    for (unsigned long i=0;i<ORN.rows();i++) 
        for (unsigned long j=0;j<ORN.columns();j++)
        ORN.entry(i,j) = prod->entry(i+rankOM,j);

    // put the presentation matrix in Smith normal form, and
    // build the list of invariant factors and their row indexes
    // now compute the rank and column indexes ...

    metricalSmithNormalForm(ORN, &(*ornR), &(*ornRi), &(*ornC), &(*ornCi));

    for (unsigned long i=0; ( (i<ORN.rows()) && (i<ORN.columns()) ); i++)
    {
        if (ORN.entry(i,i)==1) ifLoc++; else 
            if (ORN.entry(i,i)>1) InvFacList.push_back(ORN.entry(i,i)); 
    }

    ifNum = InvFacList.size();
    snfrank = ORN.rows() - ifLoc - ifNum;
    snffreeindex = ifLoc + InvFacList.size();

}

// We'll store H_k(M;Z_p) internally in two different ways.  The reason boils 
// down to the pleasant version of the universal coefficient theorem that you 
// see using Smith normal forms. Summary:  if Z^a --N--> Z^b --M--> Z^c is a 
// chain complex for H_k(M;Z) and p>0 an integer, put M into SNF(M), this gives 
// SNF(M) == OMC*M*OMR. Let SNF(M) == diag[s_0, ..., s_{k-1}, 0, ... 0] and 
// suppose only entries s_i, ..., s_{k-1} share common factors with p.   Then 
// you immediately get the presentation matrix for H_k(M;Z_p) which is a 
// product of the two matrices: [ trunc_top_k_rows[OMRi*N], diag(p,p,...,p) ] 
//  \times [ diag(gcd(s_i,p), ..., gcd(s_{k-1},p) ] here trunc_top_k_rows means 
// remove the first k rows from [OMRi*N].  The left matrix is basically by 
// design the presentation matrix for H_k(M;Z)\times Z_p, and the right matrix 
// the presentation matrix for TOR(H_{k-1}(M;Z), Z_p).   The 2nd matrix is 
// already in SNF.  We apply SNF to the first, and store the change-of-basis 
// matrices in otR, otC, otRi, otCi.  We then concatenate these two diagonal 
// matrices and apply SNF to them to get a situation MarkedAbelianGroup will 
// be happy with. This has the added advantage of us being able to later easily 
// implement the HomMarkedAbelianGroup maps for UCT later when we're interested
//  in that kind of stuff. 
MarkedAbelianGroup::MarkedAbelianGroup(const MatrixInt& M, 
            const MatrixInt& N, const Integer &pcoeff):
    OM(M), ON(N), OMR(M.columns(),M.columns()),
    OMC(M.rows(),M.rows()), OMRi(M.columns(),M.columns()),
    OMCi(M.rows(),M.rows()),
    rankOM(0), InvFacList(0), snfrank(0), snffreeindex(0), 
    ifNum(0), ifLoc(0), coeff(pcoeff), 
    TORLoc(0), TORVec(0), tensorIfLoc(0), tensorInvFacList(0)
{
    // find SNF(M).
    MatrixInt tM(M);

    metricalSmithNormalForm(tM, &OMR, &OMRi, &OMC, &OMCi);

    for (unsigned i=0; ( (i<tM.rows()) && (i<tM.columns()) ); i++)
        if (tM.entry(i,i) != 0) rankOM++;

    // in the case coeff > 0 we need to consider the TOR part of homology. 

    if (coeff > 0) for (unsigned i=0; i<rankOM; i++) 
        if (tM.entry(i,i).gcd(coeff) > 1) { TORVec.push_back(tM.entry(i,i)); }
    TORLoc = rankOM - TORVec.size();

    // okay, lets get a presentation matrix for H_*(M;Z) \otimes Z_p
    // starting by computing the trunc[OMRi*N] matrix and padding with 
    // a diagonal p matrix

    std::unique_ptr<MatrixRing<Integer> > OMRiN = OMRi*ON;

    // hmm, if we're using p == 0 coefficients, lets keep it simple
    if (coeff > 0)
    {
     MatrixInt tensorPres( OMRiN->rows() - rankOM, 
                OMRiN->columns() + OMRiN->rows() - rankOM );
     for (unsigned long i=0; i<tensorPres.rows(); i++) 
     for (unsigned long j=0; j<OMRiN->columns(); j++)
          tensorPres.entry(i,j) = OMRiN->entry(i+rankOM, j);
     for (unsigned long i=0; i< OMRiN->rows() - rankOM; i++)
          tensorPres.entry(i, OMRiN->columns() + i) = coeff;

     // initialize coordinate-change matrices for the SNF computation. 
     otR.reset(new  MatrixInt(tensorPres.columns(), tensorPres.columns() ));
     otRi.reset(new MatrixInt(tensorPres.columns(), tensorPres.columns() ));
     otC.reset(new  MatrixInt(tensorPres.rows(), tensorPres.rows() ));
     otCi.reset(new MatrixInt(tensorPres.rows(), tensorPres.rows() ));

     metricalSmithNormalForm(tensorPres, &(*otR), &(*otRi), &(*otC), &(*otCi));

     // this group is a direct sum of groups of the form Z_q where q = 
     // gcd(p, TORVec[i]), and groups Z_q where q is on the diagonal of 
     // tensorPres, and either q=0 or q>1. unfortunately in rare occurances 
     // these are not the invariant factors of the group, so we assemble these 
     // numbers into a diagonal presentation matrix and apply SNF!  Determine
     //  the size of the matrix we'll need. 
     for (unsigned long i=0; ( (i<tensorPres.rows()) && 
         (i<tensorPres.columns()) ); i++)
      {
       if (tensorPres.entry(i,i) == 1) tensorIfLoc++; else
         if (tensorPres.entry(i,i) > 1) 
           tensorInvFacList.push_back(tensorPres.entry(i,i)); else
           if (tensorPres.entry(i,i) == 0) snfrank++; // should always be zero. 
      }
     tensorIfNum = tensorInvFacList.size();

     MatrixInt diagPres( TORVec.size() + tensorIfNum + snfrank, 
             TORVec.size() + tensorIfNum + snfrank);
     for (unsigned long i=0; i<diagPres.rows(); i++)
      {
       if (i<TORVec.size()) diagPres.entry(i,i) = TORVec[i].gcd(coeff); else
        diagPres.entry(i,i) = tensorPres.entry( i - TORVec.size() + tensorIfLoc, 
         i - TORVec.size() + tensorIfLoc);
      }

     ornR.reset(new  MatrixInt(diagPres.columns(), diagPres.columns() ));
     ornRi.reset(new MatrixInt(diagPres.columns(), diagPres.columns() ));
     ornC.reset(new  MatrixInt(diagPres.rows(), diagPres.rows() ));
     ornCi.reset(new MatrixInt(diagPres.rows(), diagPres.rows() ));

     metricalSmithNormalForm(diagPres, &(*ornR), &(*ornRi), &(*ornC), &(*ornCi));
     for (unsigned long i=0; i<diagPres.rows(); i++)
     { // should only have terms > 1 or == 0. 
        if (diagPres.entry(i,i) > 1) InvFacList.push_back(diagPres.entry(i,i)); 
     }
     snffreeindex = InvFacList.size(); 
     ifNum = InvFacList.size(); 
     ifLoc = diagPres.rows() - ifNum; 
    }
    else
    { // coeff == p == 0 case
     MatrixInt tensorPres( OMRiN->rows() - rankOM, OMRiN->columns() );
     for (unsigned long i=0; i<tensorPres.rows(); i++) 
      for (unsigned long j=0; j<OMRiN->columns(); j++)
          tensorPres.entry(i,j) = OMRiN->entry(i+rankOM, j);

     // initialize coordinate-change matrices for the SNF computation. 
     ornR.reset(new  MatrixInt(tensorPres.columns(), tensorPres.columns() ));
     ornRi.reset(new MatrixInt(tensorPres.columns(), tensorPres.columns() ));
     ornC.reset(new  MatrixInt(tensorPres.rows(), tensorPres.rows() ));
     ornCi.reset(new MatrixInt(tensorPres.rows(), tensorPres.rows() ));

     metricalSmithNormalForm(tensorPres, &(*ornR), &(*ornRi), 
        &(*ornC), &(*ornCi));

     for (unsigned long i=0; ( (i<tensorPres.rows()) && 
         (i<tensorPres.columns()) ); i++)
      {
       if (tensorPres.entry(i,i) == 1) ifLoc++; else
         if (tensorPres.entry(i,i) > 1) 
           InvFacList.push_back(tensorPres.entry(i,i)); 
      }
     snffreeindex = ifLoc + InvFacList.size();
     ifNum = InvFacList.size(); 
     snfrank = tensorPres.rows() - ifLoc - ifNum;
    }
}

bool MarkedAbelianGroup::isChainComplex() const
{
    if (OM.columns() != ON.rows()) return false;
    std::unique_ptr<MatrixRing<Integer> > prod = OM*ON;
    for (unsigned long i=0; i<prod->rows(); i++) 
      for (unsigned long j=0; j<prod->columns(); j++)
        if (prod->entry(i,j) != 0) return false;
    return true;
}

unsigned long MarkedAbelianGroup::torsionRank(const Integer& degree)
        const {
    unsigned long ans = 0;
    for (unsigned long i=0;i<InvFacList.size();i++) {
        if (InvFacList[i] % degree == 0)
            ans++;
    }
    return ans;
}


void MarkedAbelianGroup::writeTextShort(std::ostream& out, bool utf8) const {
    bool writtenSomething = false;

    if (snfrank > 0) {
        if (snfrank > 1)
            out << snfrank << ' ';
        if (utf8)
            out << "\u2124";
        else
            out << 'Z';
        writtenSomething = true;
    }

    std::vector<Integer>::const_iterator it = InvFacList.begin();
    Integer currDegree;
    unsigned currMult = 0;
    while(true) {
        if (it != InvFacList.end()) {
            if ((*it) == currDegree) {
                currMult++;
                it++;
                continue;
            }
        }
        if (currMult > 0) {
            if (writtenSomething)
                out << " + ";
            if (currMult > 1)
                out << currMult << ' ';
            if (utf8)
                out << "\u2124" << regina::subscript(currDegree);
            else
                out << "Z_" << currDegree.stringValue();
            writtenSomething = true;
        }
        if (it == InvFacList.end())
            break;
        currDegree = *it;
        currMult = 1;
        it++;
    }

    if (! writtenSomething)
        out << '0';
}

/*
 * The marked abelian group was defined by matrices M and N
 * with M*N==0.  Think of M as m by l and N as l by n.  Then
 * this routine returns the index-th free generator of the
 * ker(M)/img(N) in Z^l.
 */
std::vector<Integer> MarkedAbelianGroup::freeRep(unsigned long index)
        const {
    static const std::vector<Integer> nullvec;
    if (index >= snfrank) return nullvec;

    std::vector<Integer> retval(OM.columns(),Integer::zero);
    // index corresponds to the (index+snffreeindex)-th column of ornCi
    // we then pad this vector (at the front) with rankOM 0's
    // and apply OMR to it.

     std::vector<Integer> temp(ornCi->rows()+rankOM,Integer::zero);
     for (unsigned long i=0;i<ornCi->rows();i++)
         temp[i+rankOM]=ornCi->entry(i,index+snffreeindex);
     // the above line takes the index+snffreeindex-th column of ornCi and
     // pads it.
     for (unsigned long i=0;i<retval.size();i++) 
         for (unsigned long j=0;j<OMR.columns();j++)
            retval[i] += OMR.entry(i,j)*temp[j];
     // the above takes temp and multiplies it by the matrix OMR.
    return retval;
}

/*
 * The marked abelian group was defined by matrices M and N
 * with M*N==0.  Think of M as m by l and N as l by n.  Then
 * this routine returns the index-th torsion generator of the
 * ker(M)/img(N) in Z^l.
 */
std::vector<Integer> MarkedAbelianGroup::torsionRep(
        unsigned long index) const {
    static const std::vector<Integer> nullvec;
    if (index >= ifNum) return nullvec;
    std::vector<Integer> retval(OM.columns(),Integer::zero);

    if (coeff == 0)
    {
     std::vector<Integer> temp(ornCi->rows()+rankOM,  Integer::zero);
     for (unsigned long i=0;i<ornCi->rows();i++)
       temp[i+TORLoc] = ornCi->entry(i,ifLoc+index);
     // the above line takes the index+snffreeindex-th column of ornCi and
     // pads it suitably
     for (unsigned long i=0;i<retval.size();i++) 
       for (unsigned long j=0;j<OMR.columns();j++) 
         retval[i] += OMR.entry(i,j)*temp[j];
    } 
   else
    { // coeff > 0 with coefficients there's the extra step of dealing with the 
      // UCT splitting start with column column index + ifLoc of matrix ornCi, 
      /// split into two vectors
      // 1) first TORVec.size() entries and 2) remaining entries. 
      std::vector<Integer> firstV(TORVec.size(), Integer::zero);
      std::vector<Integer> secondV(ornC->rows()-TORVec.size(), 
        Integer::zero);
      for (unsigned long i=0; i<firstV.size(); i++)
          firstV[i] = ornCi->entry( i, index + ifLoc );
      for (unsigned long i=0; i<secondV.size(); i++)
          secondV[i] = ornCi->entry( i + firstV.size(), index + ifLoc );
      // 1st vec needs coords scaled appropriately by p/gcd(p,q) and
      //  multiplied by appropriate OMR columns
      for (unsigned long i=0; i<firstV.size(); i++)
          firstV[i] *= coeff.divExact( TORVec[i].gcd(coeff) );
      std::vector<Integer> otCiSecondV(otCi->rows(), Integer::zero);
      for (unsigned long i=0; i<otCi->rows(); i++) 
        for (unsigned long j=tensorIfLoc; j<otCi->columns(); j++)
          otCiSecondV[i] += otCi->entry(i,j) * secondV[j-tensorIfLoc];
      // 2nd vec needs be multiplied by otCi, padded, then have OMR applied. 
      for (unsigned long i=0; i<retval.size(); i++) 
        for (unsigned long j=0; j<firstV.size(); j++)
         retval[i] += OMR.entry(i, TORLoc + j)*firstV[j];
      for (unsigned long i=0; i<retval.size(); i++) 
        for (unsigned long j=0; j<otCiSecondV.size(); j++)
          retval[i] += OMR.entry(i, rankOM+j) * otCiSecondV[j];
      // add answers together. 
   }
 return retval;
}


std::vector<Integer> MarkedAbelianGroup::ccRep(const std::vector<Integer>& SNFRep) const
{
    static const std::vector<Integer> nullV;
    if (SNFRep.size() != snfrank + ifNum) return nullV;

    std::vector<Integer> retval(OM.columns(),Integer::zero);
    std::vector<Integer> temp(ornCi->rows()+TORLoc,Integer::zero);

    if (coeff == 0)
    {
        for (unsigned long j=0; j<ifNum+snfrank; j++) 
          for (unsigned long i=0; i<ornCi->rows(); i++)
            temp[i+TORLoc] += ornCi->entry(i,ifLoc+j) * SNFRep[j];
        for (unsigned long i=0;i<retval.size();i++) 
          for (unsigned long j=0;j<OMR.columns();j++)
            retval[i] += OMR.entry(i,j)*temp[j];
    }
    else
    { // coeff > 0
     std::vector<Integer> firstV(TORVec.size(), Integer::zero);
     std::vector<Integer> secondV(ornC->rows()-TORVec.size(), 
       Integer::zero);
     for (unsigned long i=0; i<firstV.size(); i++) 
       for (unsigned long j=0; j<SNFRep.size(); j++)
         firstV[i] += ornCi->entry( i, j + ifLoc ) * SNFRep[j];
     for (unsigned long i=0; i<secondV.size(); i++) 
       for (unsigned long j=0; j<SNFRep.size(); j++)
         secondV[i] += ornCi->entry( i + firstV.size(), j + ifLoc ) * SNFRep[j];
     // 1st vec needs coords scaled appropriately by p/gcd(p,q) and 
     //  multiplied by appropriate OMR columns
     for (unsigned long i=0; i<firstV.size(); i++)
        firstV[i] *= coeff.divExact( TORVec[i].gcd(coeff) );
     std::vector<Integer> otCiSecondV(otCi->rows(), Integer::zero);
     for (unsigned long i=0; i<otCi->rows(); i++) 
       for (unsigned long j=tensorIfLoc; j<otCi->columns(); j++)
        otCiSecondV[i] += otCi->entry(i,j) * secondV[j-tensorIfLoc];
     // 2nd vec needs be multiplied by otCi, padded, then have OMR applied. 
     for (unsigned long i=0; i<retval.size(); i++) 
       for (unsigned long j=0; j<firstV.size(); j++)
        retval[i] += OMR.entry(i, TORLoc + j)*firstV[j];
     for (unsigned long i=0; i<retval.size(); i++) 
       for (unsigned long j=0; j<otCiSecondV.size(); j++)
         retval[i] += OMR.entry(i, rankOM+j) * otCiSecondV[j];
    }
 return retval;
}

std::vector<Integer> MarkedAbelianGroup::ccRep(
     unsigned long SNFRep) const
{
 static const std::vector<Integer> nullV;
 if (SNFRep >= snfrank + ifNum) return nullV;

 std::vector<Integer> retval(OM.columns(),Integer::zero);
 std::vector<Integer> temp(ornCi->rows()+TORLoc,Integer::zero);

 if (coeff == 0)
  {
    for (unsigned long i=0; i<ornCi->rows(); i++)
        temp[i+TORLoc] = ornCi->entry(i,ifLoc+SNFRep);
    for (unsigned long i=0;i<retval.size();i++) 
      for (unsigned long j=0;j<OMR.columns();j++)
        retval[i] += OMR.entry(i,j)*temp[j];
  }
 else
  { // coeff > 0
    std::vector<Integer> firstV(TORVec.size(), Integer::zero);
    std::vector<Integer> secondV(ornC->rows()-TORVec.size(), 
     Integer::zero);
    for (unsigned long i=0; i<firstV.size(); i++)
        firstV[i] = ornCi->entry( i, SNFRep + ifLoc );
    for (unsigned long i=0; i<secondV.size(); i++)
        secondV[i] = ornCi->entry( i + firstV.size(), SNFRep + ifLoc );
    // 1st vec needs coords scaled appropriately by p/gcd(p,q) 
    //  and multiplied by appropriate OMR columns
    for (unsigned long i=0; i<firstV.size(); i++)
        firstV[i] *= coeff.divExact( TORVec[i].gcd(coeff) );
    std::vector<Integer> otCiSecondV(otCi->rows(), Integer::zero);
    for (unsigned long i=0; i<otCi->rows(); i++) 
      for (unsigned long j=tensorIfLoc; j<otCi->columns(); j++)
        otCiSecondV[i] += otCi->entry(i,j) * secondV[j-tensorIfLoc];
    // 2nd vec needs be multiplied by otCi, padded, then have OMR applied. 
    for (unsigned long i=0; i<retval.size(); i++) 
      for (unsigned long j=0; j<firstV.size(); j++)
       retval[i] += OMR.entry(i, TORLoc + j)*firstV[j];
    for (unsigned long i=0; i<retval.size(); i++) 
     for (unsigned long j=0; j<otCiSecondV.size(); j++)
       retval[i] += OMR.entry(i, rankOM+j) * otCiSecondV[j];
 }
 return retval;
}


/*
 * The marked abelian group was defined by matrices M and N
 * with M*N==0.  Think of M as m by l and N as l by n.
 * When the group was initialized, it was computed to be isomorphic
 * to some Z_{d1} + ... + Z_{dk} + Z^d where d1 | d2 | ... | dk
 * this routine assumes element is in Z^l, and it returns a vector
 * of length d+k where the last d elements represent which class the
 * vector projects to in Z^d, and the first k elements represent the
 * projections to Z_{d1} + ... + Z_{dk}. Of these last elements, they
 * will be returned mod di respectively. Returns an empty vector if
 * element is not in the kernel of M. element is assumed to have
 * OM.columns()==ON.rows() entries.
 */
std::vector<Integer> MarkedAbelianGroup::snfRep(
        const std::vector<Integer>& element)  const {
    std::vector<Integer> retval(snfrank+ifNum,
            Integer::zero);
    // apply OMRi, crop, then apply ornC, tidy up and return.
    static const std::vector<Integer> nullvec; // this is returned if
    // element not in ker(M)
    // first, does element have the right dimensions? if not, abort.
    if (element.size() != OM.columns()) return nullvec;
    // this holds OMRi * element which we will use to check first if
    // element is in the kernel, and then to construct its SNF rep. 
    std::vector<Integer> temp(ON.rows(), Integer::zero); 
    for (unsigned long i=0;i<ON.rows();i++) 
      for (unsigned long j=0;j<ON.rows();j++)
        temp[i] += OMRi.entry(i,j)*element[j];

    // make judgement on if in kernel.
    // needs to be tweaked for mod p coefficients.
    if (coeff == 0)
    { for (unsigned long i=0;i<rankOM;i++) if (temp[i] != 0) return nullvec; }
    else
    { // the first TORLoc-1 terms of tM were units mod p so we need only 
      // check divisibility by p for temp[i] the remaining terms of tM were 
      // given by TORVec[i-TORLoc] and share a common factor with p==coeff. 
      // for element to be in ker(M), we need temp[i]*TORVec[i-TORLoc] % p == 0 
      for (unsigned long i=0; i<rankOM; i++) 
      { 
        if (i<TORLoc) { if (temp[i] % coeff != 0) return nullvec; }
        else { if ((temp[i]*TORVec[i-TORLoc]) % coeff != 0) return nullvec; 
            temp[i] = temp[i].divExact(coeff.divExact(
               coeff.gcd(TORVec[i-TORLoc])));  }
      }
   } //ok

  if (coeff == 0)
   {
     for (unsigned long i=0;i<snfrank;i++) 
      for (unsigned long j=rankOM;j<ON.rows();j++)
        retval[i+ifNum] += ornC->entry(i+snffreeindex,j-rankOM)*temp[j];
     for (unsigned long i=0;i<ifNum;i++)   
       for (unsigned long j=rankOM;j<ON.rows();j++)
         retval[i] += ornC->entry(i+ifLoc,j-rankOM)*temp[j]; 
     // redundant for loops
   }
  else 
   {
    std::vector<Integer> diagPresV( ornC->rows(), Integer::zero);
    for (unsigned long i=0; i<diagPresV.size(); i++)
     {
      // TOR part
      if (i < TORVec.size()) diagPresV[i] = temp[ i + TORLoc ]; else
      // tensor part
      for (unsigned long j=0; j<otC->columns(); j++) 
       diagPresV[i] += otC->entry( i - TORVec.size() + 
        tensorIfLoc, j) * temp[ j + rankOM ];
     }
    // assemble to a diagPres vector, apply ornC
    for (unsigned long i=0; i<retval.size(); i++) 
     for (unsigned long j=0; j<diagPresV.size(); j++) 
      retval[i] += ornC->entry(i,j) * diagPresV[j];
   }
  // do modular reduction to make things look nice...
  for (unsigned long i=0; i<ifNum; i++)
   {
    retval[i] %= InvFacList[i];
    if (retval[i] < 0) retval[i] += InvFacList[i];
   }
 return retval;
}


bool MarkedAbelianGroup::isCycle(const std::vector<Integer> &input) const
{ 
 if (input.size() != OM.columns()) return false;
 for (unsigned long i=0; i<OM.rows(); i++)
  {
   Integer T(Integer::zero);
   for (unsigned long j=0; j<OM.columns(); j++) T += input[j]*OM.entry(i,j);
   if (coeff == 0) { if (T != 0) return false; } else 
       if ( (T % coeff) != 0 ) return false;
  } 
 return true;
}

bool MarkedAbelianGroup::isBoundary(
     const std::vector<Integer> &input) const
{ 
    if (input.size() != OM.columns()) return false;
    std::vector<Integer> snF(snfRep(input));
    if (snF.size() != countInvariantFactors() + rank()) return false;
    for (unsigned long i=0; i<snF.size(); i++) if (snF[i]!=0) return false;
    return true;
}

std::vector<Integer> MarkedAbelianGroup::boundaryMap(
     const std::vector<Integer> &CCrep) const
{
    std::vector<Integer> retval(OM.rows(), Integer::zero);

    if (CCrep.size() == OM.columns()) 
      for (unsigned long i=0; i<OM.rows(); i++)
    {
        for (unsigned long j=0; j<OM.columns(); j++) 
         retval[i] += CCrep[j]*OM.entry(i,j);
        if (coeff > 0) 
        { 
            retval[i] %= coeff;
            if (retval[i] < 0) retval[i] += coeff;
        }
    }
    return retval;
}

// routine checks to see if an object in the CC coords for our group is a 
// boundary, and if so it returns CC coords of what an object that it is a 
// boundary of.  Null vector is returned if not boundary.  
std::vector<Integer> MarkedAbelianGroup::writeAsBoundary(
    const std::vector<Integer> &input) const
{  
    static const std::vector<Integer> nullvec;
    if ( !isCycle(input) ) return nullvec;
    // okay, it's a cycle so lets determine whether or not it is a boundary. 
    std::vector<Integer> temp(ON.rows(), Integer::zero); 
    for (unsigned long i=0; i<OMRi.rows(); i++) 
      for (unsigned long j=0;j<OMRi.columns();j++)
        temp[i] += OMRi.entry(i,j)*input[j];
    for (unsigned long i=0; i<TORVec.size(); i++)
        if (temp[TORLoc + i] % coeff != 0) return nullvec;
    // now we know we're dealing with a cycle with zero TOR part (if coeff != 0) 
    // convert into the diagPres coordinates / standard snfcoords if p==0. 
    std::vector<Integer> retval(ON.columns(), Integer::zero);
    if (coeff == 0)
    {
        std::vector<Integer> snfV(ornC->rows(), Integer::zero);
        for (unsigned long i=0;i<ornC->rows();i++) 
          for (unsigned long j=0;j<ornC->columns();j++)
            snfV[i] += ornC->entry(i,j)*temp[j+rankOM];
        // check divisibility in the invFac coords
        for (unsigned long i=0; i<ifNum; i++)
        { if (snfV[i+ifLoc] % InvFacList[i] != 0) return nullvec;
            snfV[i+ifLoc] /= InvFacList[i]; }
            // check that it's zero on coords missed by N...
            for (unsigned long i=0; i<snfrank; i++)
                if (snfV[i+snffreeindex] != 0) return nullvec;
            // we know it's in the image now. 
            for (unsigned long i=0; i<ornR->rows(); i++) 
              for (unsigned long j=0; j<snffreeindex; j++)
                retval[i] += ornR->entry(i, j) * snfV[j];  
    }
    else 
    {// find tensorV -- apply otC.
        std::vector<Integer> tensorV( otC->rows(), Integer::zero);
        for (unsigned long i=0; i<otC->rows(); i++) 
          for (unsigned long j=0; j<otC->columns(); j++) 
            tensorV[i] += otC->entry(i, j) * temp[ j + rankOM ];
        for (unsigned long i=0; i<tensorIfNum; i++)
        {
            if (tensorV[i+tensorIfLoc] % tensorInvFacList[i] != 0) 
              return nullvec;
            tensorV[i+tensorIfLoc] /= tensorInvFacList[i];
        }
        // so we know it's where it comes from now...
        for (unsigned long i=0; i<retval.size(); i++) 
          for (unsigned long j=0; j<tensorV.size(); j++)
            retval[i] += otR->entry(i,j) * tensorV[j];
        // ah! the other coefficients of otR gives the relevant congruence. 
    }
    return retval;
}

// returns the j+TORLoc -th column of the matrix OMR, rescaled appropriately
//  if it corresponds to a TOR vector.  
std::vector<Integer> MarkedAbelianGroup::cycleGen(unsigned long j) const
{
    static const std::vector<Integer> nullv;
    if (j >= minNumberCycleGens()) return nullv;
    std::vector<Integer> retval( OM.columns(), Integer::zero);
    for (unsigned long i=0; i<retval.size(); i++) 
     retval[i] = OMR.entry(i, j+TORLoc);
    // if j < TORVec.size() rescale by coeff / gcd(coeff, TORVec[j]
    if (j < TORVec.size()) for (unsigned long i=0; i<retval.size(); i++)
        retval[i] *= coeff.divExact(coeff.gcd(TORVec[j]));
    return retval;
}

std::vector<Integer> MarkedAbelianGroup::cycleProjection( 
     const std::vector<Integer> &ccelt ) const
{
    // multiply by OMRi, truncate, multiply by OMR
    static const std::vector<Integer> nullv;
    if (ccelt.size() != OMRi.columns()) return nullv;
    std::vector<Integer> retval( OMRi.columns(), Integer::zero );
    for (unsigned long i=0; i<retval.size(); i++)
    {
        for (unsigned long j=rankOM; j<OMRi.rows(); j++) 
           for (unsigned long k=0; k<ccelt.size(); k++)
            retval[i] += OMR.entry(i,j) * OMRi.entry(j,k) * ccelt[k];
    }
    return retval;
}

std::vector<Integer> MarkedAbelianGroup::cycleProjection(
      unsigned long ccindx ) const
{
    // truncate column cyclenum of OMRi, multiply by OMR
    static const std::vector<Integer> nullv;
    if (ccindx >= OMRi.columns()) return nullv;
    std::vector<Integer> retval( OMRi.columns(), Integer::zero );
    for (unsigned long i=0; i<retval.size(); i++)
    {
        for (unsigned long j=rankOM; j<OMRi.rows(); j++)
            retval[i] += OMR.entry(i,j) * OMRi.entry(j,ccindx);
    }
    return retval;
}


// the trivially presented torsion subgroup
std::unique_ptr<MarkedAbelianGroup> MarkedAbelianGroup::torsionSubgroup()
        const {
    MatrixInt dM(1, countInvariantFactors() );
    MatrixInt dN(countInvariantFactors(), countInvariantFactors() );
    for (unsigned long i=0; i<countInvariantFactors(); i++)
        dN.entry(i,i) = invariantFactor(i);
    return std::unique_ptr<MarkedAbelianGroup>(new MarkedAbelianGroup(dM, dN));
}

// and its canonical inclusion map
std::unique_ptr<HomMarkedAbelianGroup> MarkedAbelianGroup::torsionInclusion()
        const {
    MatrixInt iM( rankCC(), countInvariantFactors() );
    for (unsigned long j=0; j<iM.columns(); j++) {
        std::vector<Integer> jtor( torsionRep(j) );
        for (unsigned long i=0; i<iM.rows(); i++)
            iM.entry(i,j) = jtor[i];
    }
    return std::unique_ptr<HomMarkedAbelianGroup>(new HomMarkedAbelianGroup(
        *torsionSubgroup(), (*this), iM));
}


HomMarkedAbelianGroup::HomMarkedAbelianGroup(const MatrixInt &tobeRedMat,
        const MarkedAbelianGroup &dom,
        const MarkedAbelianGroup &ran) :
    domain_(dom), range_(ran), matrix(ran.M().columns(), dom.M().columns()),
    reducedMatrix_(0), kernel_(0), coKernel_(0), image_(0), reducedKernelLattice(0)
{
    reducedMatrix_ = new MatrixInt(tobeRedMat);

    // If using mod p coeff, p != 0: 
    //
    // we build up the CC map in reverse from the way we computed
    //  the structure of the domain/range groups.
    //  which was: 3) SNF(M,M'), truncate off first TORLoc coords. 
    //             2) SNF the tensorPres matrix, TOR coords fixed. 
    //                Truncate off first tensorIfLoc terms.
    //             1) SNF the combined matrix, truncate off ifLoc terms.
    //
    // Step 1: ran.ornCi*[incl tobeRedMat]*[trunc dom.ornC] puts us in 
    //         diagPres coords ran.ornCi.rows()-by-dom.ornC.rows()
    // Step 2: ran.otCi*(step 1)*[trunc dom.otC] puts us in trunc(SNF(M,M'))
    //         coords
    // Step 3: OMR*(step 2)*[trunc OMRi]

    // If using integer coefficients:
    // 
    // we build up the CC map in reverse of the process for which we found 
    //  the structure of the domain/range
    // groups, which was:  2) SNF(M,M'), truncate off the first 
    //                        rankOM==TORLoc coords
    //                     1) SNF(N,N'), truncate off the first ifLoc terms.
    //
    // Step 1: ran.ornCi*[incl tobeRedMat]*[trunc dom.ornC] puts us 
    //         in trunc(SNF(M,M')) coords
    // Step 2: --void--
    // Step 3: OMR*(step 1)*[trunc OMRi]
    // so we have a common Step 1. 
    MatrixInt step1Mat(ran.ornCi->rows(), dom.ornC->rows());
    for (unsigned long i=0; i<step1Mat.rows(); i++) 
        for (unsigned long j=0; j<step1Mat.columns(); j++)
    { // ran->ornCi.entry(i, k)*tobeRedMat.entry(k, l)*dom->ornC.entry(l, j)
      for (unsigned long k=0; k<tobeRedMat.rows(); k++) 
      for (unsigned long l=0;l<tobeRedMat.columns(); l++)
        step1Mat.entry(i,j) += 
        ran.ornCi->entry(i,k+ran.ifLoc)*tobeRedMat.entry(k,l)*
         dom.ornC->entry(l+dom.ifLoc,j);
    }
    // with mod p coefficients we have this fiddly middle step 2.

    MatrixInt step2Mat( step1Mat.rows()+ran.tensorIfLoc, 
                         step1Mat.columns()+dom.tensorIfLoc );
    // if coeff==0, we'll just copy the step1Mat, if coeff>0 we multiply 
    // the tensor part by ran.otCi, dom.otC resp.
    if (dom.coeff == 0)
        for (unsigned long i=0; i<step2Mat.rows(); i++) 
         for (unsigned long j=0; j<step2Mat.columns(); j++)
            step2Mat.entry(i,j) = step1Mat.entry(i,j);
    else
     for (unsigned long i=0; i<step2Mat.rows(); i++) 
      for (unsigned long j=0; j<step2Mat.columns(); j++)
       { // (ID_TOR xran->otCi.entry(i,k)*incl_tensorIfLoc)*step1Mat.entry(k,l)*
         // ID_TOR x trunc_tensorIfLoc*dom->otC.entry(l, j)) app shifted...
        if (i < ran.TORVec.size()) 
         {
          if (j < dom.TORVec.size()) 
           { step2Mat.entry(i,j) = step1Mat.entry(i,j); } else
           { // [step1 UR corner] * [dom->otC first tensorIfLoc rows cropped]
             for (unsigned long k=dom.tensorIfLoc; k<dom.otC->rows(); k++)
             step2Mat.entry(i,j) += 
             step1Mat.entry(i,k-dom.tensorIfLoc+dom.TORVec.size())*
                dom.otC->entry(k,j-dom.TORVec.size());
           }                                  
         } else
        if (j < dom.TORVec.size()) 
         {
          for (unsigned long k=ran.tensorIfLoc; k<ran.otCi->columns(); k++) 
           step2Mat.entry(i,j) += ran.otCi->entry(i-ran.TORVec.size(),k)*
           step1Mat.entry(k-ran.tensorIfLoc+ran.TORVec.size(),j);
         } else 
        {
         for (unsigned long k=ran.tensorIfLoc; k<ran.otCi->rows(); k++) 
         for (unsigned long l=dom.tensorIfLoc; l<dom.otC->rows(); l++)
         step2Mat.entry(i,j) += ran.otCi->entry(i-ran.TORVec.size(),k)*
         step1Mat.entry(k-ran.tensorIfLoc+ran.TORVec.size(),
         l-dom.tensorIfLoc+dom.TORVec.size())*
         dom.otC->entry(l,j-dom.TORVec.size());
        }
      }
    // now we rescale the TOR components appropriately, various row/column 
    // mult and divisions. multiply first ran.TORLoc rows by p/gcd(p,q)
    // divide first dom.TORLoc rows by p/gcd(p,q)

    for (unsigned long i=0; i<ran.TORVec.size(); i++) 
      for (unsigned long j=0; j<step2Mat.columns(); j++)
        step2Mat.entry(i,j) *= ran.coeff.divExact(ran.coeff.gcd(ran.TORVec[i]));
    for (unsigned long i=0; i<step2Mat.rows(); i++) 
      for (unsigned long j=0; j<dom.TORVec.size(); j++)
        step2Mat.entry(i,j) /= dom.coeff.divExact(dom.coeff.gcd(dom.TORVec[j]));
    // previous line of code divisibility is good thing to check when debugging. 
    // step 3, move it all up to the CC coordinates.
    // ran.OMR * incl_ifLoc * step2Mat * proj_ifLoc * dom.OMRi
    for (unsigned long i=0; i<matrix.rows(); i++) 
     for (unsigned long j=0; j<matrix.columns(); j++)
    {
     for (unsigned long k=ran.TORLoc; k<ran.OMR.columns(); k++) 
       for (unsigned long l=dom.TORLoc;l<dom.OMRi.rows(); l++)
          matrix.entry(i,j) += ran.OMR.entry(i,k) *
          step2Mat.entry(k-ran.TORLoc,l-dom.TORLoc) *
          dom.OMRi.entry(l,j);
    }
 // done
}


HomMarkedAbelianGroup::HomMarkedAbelianGroup(const HomMarkedAbelianGroup& g):
        domain_(g.domain_), range_(g.range_), matrix(g.matrix) {
    if (g.reducedMatrix_) { reducedMatrix_ = new MatrixInt(*g.reducedMatrix_); }
     else reducedMatrix_ = 0;
    if (g.kernel_) { kernel_ = new MarkedAbelianGroup(*g.kernel_); }
     else kernel_ = 0;
    if (g.coKernel_) { coKernel_ = new MarkedAbelianGroup(*g.coKernel_); }
     else coKernel_ = 0;
    if (g.image_) { image_ = new MarkedAbelianGroup(*g.image_); }
     else image_ = 0;
    if (g.reducedKernelLattice) { reducedKernelLattice = 
       new MatrixInt(*g.reducedKernelLattice); } 
     else reducedKernelLattice = 0;
}

void HomMarkedAbelianGroup::computeReducedMatrix()
{
 if (!reducedMatrix_)
  {
   reducedMatrix_ = new MatrixInt( range_.minNumberOfGenerators(),
    domain_.minNumberOfGenerators() );

   for (unsigned long j=0; j<reducedMatrix_->columns(); j++)
    {
     std::vector<Integer> colV(
      (j<domain_.countInvariantFactors()) ?
       domain_.torsionRep(j) :
       domain_.freeRep(j-domain_.countInvariantFactors()) );
     std::vector<Integer> icv( matrix.rows(), Integer::zero);
     for (unsigned long i=0; i<icv.size(); i++) 
      for (unsigned long k=0; k<matrix.columns(); k++)
       icv[i] += matrix.entry(i,k) * colV[k];
     std::vector<Integer> midge( range_.snfRep(icv) );
     for (unsigned long i=0; i<midge.size(); i++)
     reducedMatrix_->entry(i,j) = midge[i];
    }
  }
}

void HomMarkedAbelianGroup::computeReducedKernelLattice() {
    if (!reducedKernelLattice) {
        computeReducedMatrix();
        const MatrixInt& redMatrix(*reducedMatrix_);

        std::vector<Integer> dcL(range_.rank() +
            range_.countInvariantFactors() );
        for (unsigned long i=0; i<dcL.size(); i++)
            if (i<range_.countInvariantFactors())
                dcL[i]=range_.invariantFactor(i);
            else
                dcL[i]=Integer::zero;

        reducedKernelLattice = preImageOfLattice( redMatrix, dcL ).release();
    }
}

void HomMarkedAbelianGroup::computeKernel() {
    if (!kernel_) {
        computeReducedKernelLattice();
        MatrixInt dcLpreimage( *reducedKernelLattice );

        MatrixInt R( dcLpreimage.columns(), dcLpreimage.columns() );
        MatrixInt Ri( dcLpreimage.columns(), dcLpreimage.columns() );
        MatrixInt C( dcLpreimage.rows(), dcLpreimage.rows() );
        MatrixInt Ci( dcLpreimage.rows(), dcLpreimage.rows() );

        metricalSmithNormalForm( dcLpreimage, &R, &Ri, &C, &Ci );

        // the matrix representing the domain lattice in dcLpreimage
        // coordinates is given by domainLattice * R * (dcLpreimage inverse) * C

        MatrixInt workMat( dcLpreimage.columns(),
            domain_.countInvariantFactors() );

        for (unsigned long i=0;i<workMat.rows();i++)
            for (unsigned long j=0;j<workMat.columns();j++)
                for (unsigned long k=0;k<R.columns();k++) {
                    workMat.entry(i,j) += (domain_.invariantFactor(j) *
                        R.entry(i,k) * C.entry(k,j) ) / dcLpreimage.entry(k,k);
                }

        MatrixInt dummy( 1, dcLpreimage.columns() );

        kernel_ = new MarkedAbelianGroup(dummy, workMat);
    }
}



void HomMarkedAbelianGroup::computeCokernel() {
    if (!coKernel_) {
        computeReducedMatrix();

        MatrixInt ccrelators( reducedMatrix_->rows(),
            reducedMatrix_->columns() + range_.countInvariantFactors() );
        unsigned i,j;
        for (i=0;i<reducedMatrix_->rows();i++)
            for (j=0;j<reducedMatrix_->columns();j++)
                ccrelators.entry(i,j)=reducedMatrix_->entry(i,j);
        for (i=0;i<range_.countInvariantFactors();i++)
            ccrelators.entry(i,i+reducedMatrix_->columns())=
                range_.invariantFactor(i);

        MatrixInt ccgenerators( 1, reducedMatrix_->rows() );

        coKernel_ = new MarkedAbelianGroup(ccgenerators, ccrelators);
    }
}


void HomMarkedAbelianGroup::computeImage() {
    if (!image_) {
        computeReducedKernelLattice();
        const MatrixInt& dcLpreimage( *reducedKernelLattice );

        MatrixInt imgCCm(1, dcLpreimage.rows() );
        MatrixInt imgCCn(dcLpreimage.rows(),
            dcLpreimage.columns() + domain_.countInvariantFactors() );

        for (unsigned long i=0;i<domain_.countInvariantFactors();i++)
            imgCCn.entry(i,i) = domain_.invariantFactor(i);

        for (unsigned long i=0;i<imgCCn.rows();i++)
            for (unsigned long j=0;j< dcLpreimage.columns(); j++)
                imgCCn.entry(i,j+domain_.countInvariantFactors()) =
                    dcLpreimage.entry(i,j);

        image_ = new MarkedAbelianGroup(imgCCm, imgCCn);
    }
}

std::unique_ptr<HomMarkedAbelianGroup> HomMarkedAbelianGroup::operator * (const 
      HomMarkedAbelianGroup &X) const
{
    std::unique_ptr<MatrixRing<Integer> > prod=matrix*X.matrix;
    MatrixInt compMat(matrix.rows(), X.matrix.columns() );
    for (unsigned long i=0;i<prod->rows();i++) 
      for (unsigned long j=0;j<prod->columns();j++)
        compMat.entry(i,j) = prod->entry(i, j);
    return std::unique_ptr<HomMarkedAbelianGroup>(new 
        HomMarkedAbelianGroup(X.domain_, range_, compMat));
}

std::vector<Integer> HomMarkedAbelianGroup::evalCC(
   const std::vector<Integer> &input) const
{
    std::vector<Integer> retval(matrix.rows(), Integer::zero);
    for (unsigned long i=0; i<retval.size(); i++) 
      for (unsigned long j=0; j<matrix.columns(); j++)
        retval[i] += input[j]*matrix.entry(i,j);
    return retval;
}

std::vector<Integer> HomMarkedAbelianGroup::evalSNF(
    const std::vector<Integer> &input) const
{
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    static const std::vector<Integer> nullV; 
    if (input.size() != domain_.minNumberOfGenerators() ) return nullV;
    std::vector<Integer> retval( 
          range_.minNumberOfGenerators(), Integer::zero );

    for (unsigned long i=0; i<retval.size(); i++) 
    {
        for (unsigned long j=0; j<reducedMatrix().columns(); j++)
            retval[i] += input[j] * reducedMatrix().entry(i,j);
        if ( i < range_.countInvariantFactors() ) {
            retval[i] %= range_.invariantFactor(i);
            if (retval[i]<0) retval[i] += range_.invariantFactor(i); }
    }
    return retval;
}


void HomMarkedAbelianGroup::writeReducedMatrix(std::ostream& out) const {
    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();

    out<<"Reduced Matrix is "<<reducedMatrix_->rows()<<" by "
        <<reducedMatrix_->columns()<<" corresponding to domain ";
    domain_.writeTextShort(out);
    out<<" and range ";
    range_.writeTextShort(out);
    out<<"\n";
    for (unsigned long i=0;i<reducedMatrix_->rows();i++) {
        out<<"[";
        for (unsigned long j=0;j<reducedMatrix_->columns();j++) {
            out<<reducedMatrix_->entry(i,j);
            if (j+1 < reducedMatrix_->columns()) out<<" ";
        }
        out<<"]\n";
    }
}

void HomMarkedAbelianGroup::writeTextShort(std::ostream& out) const {
    if (isIsomorphism()) out<<"isomorphism"; else 
    if (isZero()) out<<"zero map"; else 
    if (isMonic()) { // monic not epic
        out<<"monic, with cokernel ";
        cokernel().writeTextShort(out);
    } else if (isEpic()) { // epic not monic
        out<<"epic, with kernel ";
        kernel().writeTextShort(out);
    } else { // nontrivial not epic, not monic
        out<<"kernel ";
        kernel().writeTextShort(out);
        out<<" | cokernel ";
        cokernel().writeTextShort(out);
        out<<" | image ";
        image().writeTextShort(out);
    }
}

void HomMarkedAbelianGroup::writeTextLong(std::ostream& out) const
{
    out<<"hom[ "; domain_.writeTextShort(out); out<<" --> ";
    range_.writeTextShort(out); out<<" ] ";
    writeTextShort(out);
}


bool HomMarkedAbelianGroup::isIdentity() const
{
    if (!(domain_.equalTo(range_))) return false;
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    if (!reducedMatrix_->isIdentity()) return false;
    return true;
}

bool HomMarkedAbelianGroup::isCycleMap() const
{
    for (unsigned long j=0; j<domain_.minNumberCycleGens(); j++)
    {
        std::vector<Integer> cycJ( domain_.cycleGen(j) );
        std::vector<Integer> FcycJ( range_.rankCC(), Integer::zero );
        for (unsigned long i=0; i<matrix.rows(); i++) 
          for (unsigned long k=0; k<matrix.columns(); k++)
            FcycJ[i] += matrix.entry(i,k) * cycJ[k];
        if (!range_.isCycle(FcycJ)) { return false; }
    }
    return true;
}

//  Returns a HomMarkedAbelianGroup representing the induced map
//  on the torsion subgroups. 
std::unique_ptr<HomMarkedAbelianGroup> HomMarkedAbelianGroup::torsionSubgroup()
        const {
    std::unique_ptr<MarkedAbelianGroup> dom = domain_.torsionSubgroup();
    std::unique_ptr<MarkedAbelianGroup> ran = range_.torsionSubgroup();

    MatrixInt mat(range_.countInvariantFactors(),
        domain_.countInvariantFactors() );
    for (unsigned long j=0; j<domain_.countInvariantFactors(); j++) {
        // std::vector<Integer> in range's snfRep coords
        std::vector<Integer> temp(range_.snfRep(evalCC(
            domain_.torsionRep(j))));
        for (unsigned long i=0; i<range_.countInvariantFactors(); i++)
            mat.entry(i,j) = temp[i];
    }

    return std::unique_ptr<HomMarkedAbelianGroup>(new HomMarkedAbelianGroup(
        *dom, *ran, mat));
}


/**
 * Given two HomMarkedAbelianGroups, you have two diagrams:
 * Z^a --N1--> Z^b --M1--> Z^c   Z^g --N3--> Z^h --M3--> Z^i
 *                   ^                             ^
 *                   |  this                       | other
 * Z^d --N2--> Z^e --M2--> Z^f   Z^j --N4--> Z^k --M4--> Z^l
 * @return true if and only if M1 == N3, M2 == N4 and diagram commutes
 *         commutes.
 */
bool HomMarkedAbelianGroup::isChainMap(
  const HomMarkedAbelianGroup &other) const
{
 if ( (range().M().rows() != other.range().N().rows()) ||
      (range().M().columns() != other.range().N().columns()) ||
      (domain().M().rows() != other.domain().N().rows()) ||
      (domain().M().columns() != other.domain().N().columns())
    ) return false;
 if ( (range().M() != other.range().N()) ||
      (domain().M() != other.domain().N()) ) return false;
 std::unique_ptr< MatrixRing<Integer> >
    prodLU = range_.M() * definingMatrix();
 std::unique_ptr< MatrixRing<Integer> >
    prodBR = other.definingMatrix() * domain_.M();
 if ( (*prodLU) != (*prodBR) ) return false;
 return true;
}


// Start with the reduced matrix which is a 2x2 block matrix:
//
//  [A|B]
//  [---]  where D is an inveritble square matrix, 0 is a zero matrix,  
//  [0|D]  A a square matrix and B maybe not square. 
//         the columns of D represent the free factors of the domain, 
//      the rows of D the free factors of the range, 
//     the columns/rows of A represent the torsion factors of the domain/range
//     respectively.  So the inverse matrix must have the form
//  [A'|B']
//  [-----]
//  [0 |D']  where D' is the inverse of D, A' represents the inverse 
//           automorphism of
//  Z_p1 + Z_p2 + ... Z_pk, etc.  And so B' must equal -A'BD', which is readily
//computable.  So it all boils down to computing A'.  So we need a routine which
//makes a matrix A representing an automorphism of Z_p1 + ... Z_pk and then 
// computes the matrix representing the inverse automorphism.  
// So to do this we'll need a new matrixops.cpp command -- call it 
// torsionAutInverse.  
std::unique_ptr<HomMarkedAbelianGroup> HomMarkedAbelianGroup::inverseHom() const
{
 const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
 MatrixInt invMat( reducedMatrix_->columns(), reducedMatrix_->rows() );
 if (!isIsomorphism()) return std::unique_ptr<HomMarkedAbelianGroup>(
     new HomMarkedAbelianGroup( invMat, range_, domain_ ));
 // get A, B, D from reducedMatrix
 // A must be square with domain/range_.countInvariantFactors() columns
 // D must be square with domain/range_.rank() columns
 // B may not be square with domain_.rank() columns and
 //  range_.countInvariantFactors() rows.
 MatrixInt A(range_.countInvariantFactors(), domain_.countInvariantFactors());
 MatrixInt B(range_.countInvariantFactors(), domain_.rank());
 MatrixInt D(range_.rank(), domain_.rank());
 for (unsigned long i=0; i<A.rows(); i++)
   for (unsigned long j=0; j<A.columns(); j++)
     A.entry(i,j) = reducedMatrix_->entry(i,j);
 for (unsigned long i=0; i<B.rows(); i++)
   for (unsigned long j=0; j<B.columns(); j++)
     B.entry(i,j) = reducedMatrix_->entry(i, j + A.columns());
 for (unsigned long i=0; i<D.rows(); i++)
   for (unsigned long j=0; j<D.columns(); j++)
     D.entry(i,j) = reducedMatrix_->entry( i + A.rows(), j + A.columns() );
 // compute A', B', D'
 // use void columnEchelonForm(MatrixInt &M, MatrixInt &R, MatrixInt &Ri,
 // const std::vector<unsigned> &rowList); 
 //  from matrixOps to compute the inverse of D.
 MatrixInt Di(D.rows(), D.columns()); Di.makeIdentity();
 MatrixInt Dold(D.rows(), D.columns()); Dold.makeIdentity();
 std::vector<unsigned> rowList(D.rows());
 for (unsigned i=0; i<rowList.size(); i++) rowList[i]=i;
 columnEchelonForm(D, Di, Dold, rowList); 
 // now Di is the inverse of the old D, and D is the identity, 
 // and Dold is the old D.
 std::vector<Integer> invF(domain_.countInvariantFactors());
 for (unsigned long i=0; i<invF.size(); i++) 
    invF[i] = domain_.invariantFactor(i);
 std::unique_ptr<MatrixInt> Ai = torsionAutInverse( A, invF);
 // then Bi is given by Bi = -AiBDi
    MatrixInt Bi(range_.countInvariantFactors(), domain_.rank());
    MatrixInt Btemp(range_.countInvariantFactors(), domain_.rank());
    // Btemp will give -BDi
    // Bi will be AiBtemp
    for (unsigned long i=0; i<Btemp.rows(); i++) 
      for (unsigned long j=0; j<Btemp.columns(); j++)
        for (unsigned long k=0; k<Btemp.columns(); k++)
            Btemp.entry(i,j) -= B.entry(i,k)*Di.entry(k,j);
    for (unsigned long i=0; i<Bi.rows(); i++) 
      for (unsigned long j=0; j<Bi.columns(); j++)
        for (unsigned long k=0; k<Ai->columns(); k++)
            Bi.entry(i,j) += Ai->entry(i,k)*Btemp.entry(k,j);
    // reduce Ai and Bi respectively. 
    for (unsigned long i=0; i<Ai->rows(); i++)
    {
     for (unsigned long j=0; j<Ai->columns(); j++)
      {
       Ai->entry(i,j) %= domain_.invariantFactor(i);
       if (Ai->entry(i,j) < 0) Ai->entry(i,j) += domain_.invariantFactor(i);
      }
     for (unsigned long j=0; j<Bi.columns(); j++)
      {
       Bi.entry(i,j) %= domain_.invariantFactor(i);
       if (Bi.entry(i,j) < 0) Bi.entry(i,j) += domain_.invariantFactor(i);
      }
    }
 // compute the coefficients of invMat.  We're in the funny situation where we 
 // know what invMat's reduced matrix, not its chain-complex level defining 
 // matrix.  So we use the alternative HomMarkedAbelianGroup constructor 
 // designed specifically for this situation.  
 // assemble into invMat  [A'|B']
 //                       [-----]
 //                       [0 |D'] 

 for (unsigned long i=0; i<Ai->rows(); i++) 
   for (unsigned long j=0; j<Ai->columns(); j++)
     invMat.entry(i,j) = Ai->entry(i,j);
 for (unsigned long i=0; i<Di.rows(); i++) 
   for (unsigned long j=0; j<Di.columns(); j++)
     invMat.entry(i+Ai->rows(),j+Ai->columns()) = Di.entry(i,j);
 for (unsigned long i=0; i<Bi.rows(); i++) 
   for (unsigned long j=0; j<Bi.columns(); j++)
     invMat.entry(i,j+Ai->columns()) = Bi.entry(i,j);
 return std::unique_ptr<HomMarkedAbelianGroup>(
   new HomMarkedAbelianGroup( invMat, range_, domain_ ));
}



} // namespace regina

