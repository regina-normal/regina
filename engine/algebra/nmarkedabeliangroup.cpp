
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

#include "algebra/nmarkedabeliangroup.h"
#include "maths/matrixops.h"
#include <iostream>

namespace regina {

unsigned long rbGetRank(const NMatrixInt& M) // I don't know how to avoid
                                             // using this, unfortunately.
{ // this is needed in NMarkedAbelianGroup::NMarkedAbelianGroup
    NMatrixInt temp(M);
    smithNormalForm(temp);
    unsigned long i=0;
    while ( (i<temp.rows()) && (i<temp.columns()) && (temp.entry(i,i)!=0) )
        i++;
    return i; // i is the rank of M
}

NMarkedAbelianGroup::NMarkedAbelianGroup(const unsigned long &rk, const NLargeInteger &p) : 
OM(rk, rk), ON(rk,rk), OMR(rk,rk), OMC(rk,rk), OMRi(rk, rk), OMCi(rk, rk), rankOM(0), ornR(rk, rk),
ornRi(rk, rk), ornC(rk, rk), ornCi(rk, rk), InvFacList(0), snfrank(0), snffreeindex(0), ifNum(0), ifLoc(0)
{
for (unsigned long i=0; i<rk; i++) ON.entry(i,i) = p;
// everything is already in SNF, so these are identity matrices
OMR.makeIdentity();OMC.makeIdentity(); 
OMRi.makeIdentity();OMCi.makeIdentity(); 
ornR.makeIdentity(); ornRi.makeIdentity(); 
ornC.makeIdentity(); ornCi.makeIdentity(); 
if (p != NLargeInteger::zero) ifNum=rk;
if (ifNum != 0) InvFacList.resize(ifNum);
for (unsigned long i=0; i<InvFacList.size(); i++) InvFacList[i] = p;
snfrank = rk - ifNum;
}

NMarkedAbelianGroup::NMarkedAbelianGroup(const NMatrixInt& M,
        const NMatrixInt& N) :
        OM(M), ON(N), OMR(M.columns(),M.columns()),
        OMC(M.rows(),M.rows()), OMRi(M.columns(),M.columns()),
        OMCi(M.rows(),M.rows()),
        rankOM(rbGetRank(M)),
        ornR(N.columns(),N.columns()),        ornRi(N.columns(),N.columns()),
        ornC(N.rows()-rankOM,N.rows()-rankOM),
        ornCi(N.rows()-rankOM,N.rows()-rankOM),
        InvFacList(0), snfrank(0), snffreeindex(0), ifNum(0), ifLoc(0) {
    // find SNF(M).
    NMatrixInt tM(M);

    smithNormalForm(tM, OMR, OMRi, OMC, OMCi);
    // now construct OMRi * N, and delete first SNF_OM_firstzero rows,
    // constructing ORN.

    // construct the internal presentation matrix.
    std::auto_ptr<NMatrixRing<NLargeInteger> > prod=OMRi*ON;

    NMatrixInt ORN(N.rows()-rankOM, N.columns());

    unsigned long i;
    unsigned long j;

    for (i=0;i<ORN.rows();i++)
        for (j=0;j<ORN.columns();j++)
            ORN.entry(i,j) = prod->entry(i+rankOM,j);

    // put the presentation matrix in Smith normal form, and
    // build the list of invariant factors and their row indexes
    // now compute the rank and column indexes ...
    smithNormalForm(ORN, ornR, ornRi, ornC, ornCi);
    i=0;
    unsigned long totO=0; // number diag entries == 1
    unsigned long totIF=0;// number diag entries > 1
    unsigned long totFR=0;// number diag entries == 0

    while ((i<ORN.rows()) && (i<ORN.columns())) {
        if (ORN.entry(i,i)==1) totO++;
        else if (ORN.entry(i,i)>1) totIF++; 
        else totFR++;
        i++;
    }

    ifNum=totIF;
    ifLoc=totO;

    InvFacList.resize(ifNum);
    for (i=0;i<ifNum;i++)
        InvFacList[i]=ORN.entry(ifLoc+i,ifLoc+i);

    snfrank=ORN.rows()-totO-totIF;
    snffreeindex=totO+totIF;
}


unsigned NMarkedAbelianGroup::getTorsionRank(const NLargeInteger& degree)
        const {
    unsigned ans = 0;
    for (unsigned long i=0;i<InvFacList.size();i++) {
        if (InvFacList[i] % degree == 0)
            ans++;
    }
    return ans;
}


void NMarkedAbelianGroup::writeTextShort(std::ostream& out) const {
    bool writtenSomething = false;

    if (snfrank > 0) {
        if (snfrank > 1)
            out << snfrank << ' ';
        out << 'Z';
        writtenSomething = true;
    }

    std::vector<NLargeInteger>::const_iterator it = InvFacList.begin();
    NLargeInteger currDegree;
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
std::vector<NLargeInteger> NMarkedAbelianGroup::getFreeRep(unsigned long index)
        const {
    std::vector<NLargeInteger> retval(OM.columns(),NLargeInteger::zero);
    // index corresponds to the (index+snffreeindex)-th column of ornCi
    // we then pad this vector (at the front) with rankOM 0's
    // and apply OMR to it.

    std::vector<NLargeInteger> temp(ornCi.rows()+rankOM,NLargeInteger::zero);

    for (unsigned long i=0;i<ornCi.rows();i++)
        temp[i+rankOM]=ornCi.entry(i,index+snffreeindex);
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
std::vector<NLargeInteger> NMarkedAbelianGroup::getTorsionRep(
        unsigned long index) const {
    std::vector<NLargeInteger> retval(OM.columns(),NLargeInteger::zero);
    // index corresponds to the (InvFacIndex[index])-th column of ornCi
    // we then pad this vector (at the front) with rankOM 0's
    // and apply OMR to it.

    std::vector<NLargeInteger> temp(ornCi.rows()+rankOM,NLargeInteger::zero);

    for (unsigned long i=0;i<ornCi.rows();i++)
        temp[i+rankOM]=ornCi.entry(i,ifLoc+index);
    // the above line takes the index+snffreeindex-th column of ornCi and
    // pads it.

    for (unsigned long i=0;i<retval.size();i++)
        for (unsigned long j=0;j<OMR.columns();j++)
            retval[i] += OMR.entry(i,j)*temp[j];
    // the above takes temp and multiplies it by the matrix OMR.

    return retval;
}


/*
 * TODO: with this do we need getTorsionRep  and getFreeRep ? 
 *       consider erasing them.
 */

std::vector<NLargeInteger> NMarkedAbelianGroup::getCCRep(const std::vector<NLargeInteger> SNFRep) const
{
    std::vector<NLargeInteger> retval(OM.columns(),NLargeInteger::zero);
    // the first
    // index corresponds to the (InvFacIndex[index])-th column of ornCi
    // we then pad this vector (at the front) with rankOM 0's
    // and apply OMR to it.

    std::vector<NLargeInteger> temp(ornCi.rows()+rankOM,NLargeInteger::zero);

    for (unsigned long j=0; j<ifNum+snfrank; j++) for (unsigned long i=0; i<ornCi.rows(); i++)
         temp[i+rankOM] += ornCi.entry(i,ifLoc+j) * SNFRep[j];

    for (unsigned long i=0;i<retval.size();i++)
        for (unsigned long j=0;j<OMR.columns();j++)
            retval[i] += OMR.entry(i,j)*temp[j];
    // the above takes temp and multiplies it by the matrix OMR.

    return retval;
}

bool NMarkedAbelianGroup::isCycle(const std::vector<NLargeInteger> &input) const
{
bool retval = true;
if (input.size() == OM.columns())
 {
   for (unsigned long i=0; i<OM.rows(); i++)
	{
	NLargeInteger T(NLargeInteger::zero);
	for (unsigned long j=0; j<OM.columns(); j++) T += input[j]*OM.entry(i,j);
	if (T != NLargeInteger::zero) retval = false;
	}
 } else retval = false;
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
std::vector<NLargeInteger> NMarkedAbelianGroup::getSNFIsoRep(
        const std::vector<NLargeInteger>& element)  const {
    std::vector<NLargeInteger> retval(snfrank+InvFacList.size(),
        NLargeInteger::zero);
    // apply OMRi, crop, then apply ornC, tidy up and return.
    static const std::vector<NLargeInteger> nullvec; // this is returned if
                                                     // element not in ker(M)

    std::vector<NLargeInteger> temp(ON.rows(),
        NLargeInteger::zero); // this holds OMRi * element
    // if first rankOM entries are 0, then element is in the kernel

    bool eltinker=true;

    // set up temp.
    for (unsigned long i=0;i<ON.rows();i++)
        for (unsigned long j=0;j<ON.rows();j++)
            temp[i] += OMRi.entry(i,j)*element[j];
    for (unsigned long i=0;i<rankOM;i++)
        if (temp[i] != 0)
            eltinker=false;
    // ON.rows - rankOM == ORN.rows()
    if (eltinker==true) {
        // The last snfrank elts are the free generators
        for (unsigned long i=0;i<snfrank;i++)
            for (unsigned long j=rankOM;j<ON.rows();j++)
                retval[i + ifNum] += ornC.entry(snffreeindex+i,j-rankOM)*temp[j];
        // the ifNum elts are torsion generators.
        for (unsigned long i=0;i<ifNum;i++) {
            for (unsigned long j=rankOM;j<ON.rows();j++)
                retval[i] += ornC.entry(ifLoc+i,j-rankOM)*temp[j];
            retval[i] = (retval[i] % InvFacList[i]);

            // the element might still be negative, though in this case
            // it should be no less than (- InvFacList[i] + 1).
            // we can put it in the required range just by adding InvFacList[i].
            if (retval[i] < 0)
                retval[i] += InvFacList[i];
        }
        return retval;
    } else
        return nullvec;
}




// this is when you want to define an NHomMarkedAbelianGroup from a its reduced matrix, not via
// a chain map of chain complexes. 
// So matrix needs to be computed, and reducedMatrix properly assigned.
// at first, we make reducedMatrix unassigned and matrix zero with the proper dimensions.

NHomMarkedAbelianGroup::NHomMarkedAbelianGroup(const NMatrixInt &tobeRedMat, 
		       const NMarkedAbelianGroup &dom, 
		       const NMarkedAbelianGroup &ran) : 
domain(dom), range(ran), matrix(ran.getM().columns(), dom.getM().columns()), reducedMatrix(0), 
kernel(0), coKernel(0), image(0), reducedKernelLattice(0)
{
// need to compute matrix and assign reducedMatrix
reducedMatrix = new NMatrixInt(tobeRedMat);
// rarely would matrix be correct at this stage, so we proceed to fix that...
// roughly we're looking for matrix = getCCRep ( tobeRedMat * SNFIsoRep ( projkerMdomain 

NMatrixInt domMRB( domain.getMRB() );
NMatrixInt domMRBi( domain.getMRBi() );

NMatrixInt domKerProj( domMRB.rows(), domMRB.columns() );
// define this matrix to be domMRB [ 0 0 | 0 I ] domMRBi where the first 0 is getrankM() square
for (unsigned long i=0; i<domMRB.rows(); i++) for (unsigned long j=0; j<domMRB.columns(); j++)
 for (unsigned long k=domain.getRankM(); k<domMRB.rows(); k++)
	domKerProj.entry(i,j) += domMRB.entry(i,k) * domMRBi.entry(k,j);

for (unsigned long j=0; j<domKerProj.columns(); j++)
 {
 // for each column of domKerProj write as a std::vector, push through SNFIsoRep
 std::vector<NLargeInteger> colj(domKerProj.rows());
 for (unsigned long i=0; i<domKerProj.rows(); i++) colj[i] = domKerProj.entry(i,j);
 std::vector<NLargeInteger> ircj(domain.getSNFIsoRep(colj));

 // push through tobeRedMat
 std::vector<NLargeInteger> rmj(ircj.size(), NLargeInteger::zero);
 for (unsigned long i=0; i<tobeRedMat.rows(); i++) for (unsigned long k=0; k<tobeRedMat.columns(); k++) 
  rmj[i] += tobeRedMat.entry(i,k) * ircj[k];

 // apply getCCRep
 std::vector<NLargeInteger> Mcolj(range.getCCRep(rmj));

 // assemble into matrix.
 for (unsigned long i=0; i<matrix.rows(); i++) matrix.entry(i,j) = Mcolj[i];
 }

// done
}


NHomMarkedAbelianGroup::NHomMarkedAbelianGroup(const NHomMarkedAbelianGroup& g):
        ShareableObject(), domain(g.domain), range(g.range), matrix(g.matrix) {
    if (g.reducedMatrix) {
        reducedMatrix = new NMatrixInt(*g.reducedMatrix);
    } else reducedMatrix = 0;

    if (g.kernel) {
        kernel = new NMarkedAbelianGroup(*g.kernel);
    } else kernel = 0;

    if (g.coKernel) {
        coKernel = new NMarkedAbelianGroup(*g.coKernel);
    } else coKernel = 0;

    if (g.image) {
        image = new NMarkedAbelianGroup(*g.image);
    } else image = 0;

    if (g.reducedKernelLattice) {
        reducedKernelLattice = new NMatrixInt(*g.reducedKernelLattice);
    } else reducedKernelLattice = 0;
}

void NHomMarkedAbelianGroup::computeReducedMatrix() {
    if (!reducedMatrix) {
        unsigned long i,j,k;

        NMatrixInt kerMatrix( matrix.rows()-range.getRankM(),
                matrix.columns()-domain.getRankM() );
        // kerMatrix = truncate (range.getMRBi() * matrix * domain.getMRBi)
        // to construct this we do it in two steps:
        // step 1) temp1 = truncate columns (matrix * domain.getMRBi )
        // step 2) kerMatrix = truncate rows (range.getMRBi * temp1 )

        const NMatrixInt& dcckb(domain.getMRB());
        const NMatrixInt& rcckb(range.getMRBi());

        NMatrixInt temp1( matrix.rows(), matrix.columns()-domain.getRankM() );
        for (i=0;i<temp1.rows();i++)
            for (j=0;j<temp1.columns();j++)
                for (k=0;k<matrix.columns();k++)
                    temp1.entry(i,j) += matrix.entry(i,k) *
                        dcckb.entry(k,j + domain.getRankM() );

        for (i=0;i<kerMatrix.rows();i++)
            for (j=0;j<kerMatrix.columns();j++)
                for (k=0;k<rcckb.rows();k++)
                    kerMatrix.entry(i,j) +=
                        rcckb.entry(i+range.getRankM(), k) * temp1.entry(k,j);

        reducedMatrix = new NMatrixInt( kerMatrix.rows()-range.getTorsionLoc(),
                kerMatrix.columns()-domain.getTorsionLoc() );

        const NMatrixInt& dccqb(domain.getNCBi());
        const NMatrixInt& rccqb(range.getNCB());

        NMatrixInt temp2( kerMatrix.rows(),
            kerMatrix.columns() - domain.getTorsionLoc() );
        for (i=0;i<temp2.rows();i++)
            for (j=0;j<temp2.columns();j++)
                for (k=0;k<kerMatrix.columns();k++) {
                    temp2.entry(i,j) += kerMatrix.entry(i,k) *
                        dccqb.entry(k,j + domain.getTorsionLoc() );
                }

        for (i=0;i<reducedMatrix->rows();i++)
            for (j=0;j<reducedMatrix->columns();j++)
		{
                for (k=0;k<rccqb.rows();k++)
                    reducedMatrix->entry(i,j) +=
                        rccqb.entry(i+range.getTorsionLoc(), k) *
                        temp2.entry(k,j);
                // if representing torsion, mod out to reduce the size of the integers to something reasonable.
                if (i < range.getNumberOfInvariantFactors()) 
			{
			reducedMatrix->entry(i,j) %= range.getInvariantFactor(i);
			if (reducedMatrix->entry(i,j) < 0) reducedMatrix->entry(i,j) +=
				range.getInvariantFactor(i);
			}
		}
    }
}

void NHomMarkedAbelianGroup::computeReducedKernelLattice() {
    if (!reducedKernelLattice) {
        computeReducedMatrix();

        unsigned long i;

        const NMatrixInt& redMatrix(*reducedMatrix);

        // the kernel is the dcLpreimage lattice mod the domain lattice.
        // so after computing the dcLpreimage lattice, we need to represent
        // the domain lattice in its coordinates.

        std::vector<NLargeInteger> dcL(range.getRank() +
            range.getNumberOfInvariantFactors() );
        for (i=0; i<dcL.size(); i++)
            if (i<range.getNumberOfInvariantFactors())
                dcL[i]=range.getInvariantFactor(i);
            else
                dcL[i]="0";

        reducedKernelLattice = preImageOfLattice( redMatrix, dcL ).release();
    }
}

void NHomMarkedAbelianGroup::computeKernel() {
    if (!kernel) {
        computeReducedKernelLattice();
        NMatrixInt dcLpreimage( *reducedKernelLattice );

        unsigned long i,j,k;

        NMatrixInt R( dcLpreimage.columns(), dcLpreimage.columns() );
        NMatrixInt Ri( dcLpreimage.columns(), dcLpreimage.columns() );
        NMatrixInt C( dcLpreimage.rows(), dcLpreimage.rows() );
        NMatrixInt Ci( dcLpreimage.rows(), dcLpreimage.rows() );

        smithNormalForm( dcLpreimage, R, Ri, C, Ci );

        // the matrix representing the domain lattice in dcLpreimage
        // coordinates is given by domainLattice * R * (dcLpreimage inverse) * C

        NMatrixInt workMat( dcLpreimage.columns(),
            domain.getNumberOfInvariantFactors() );

        for (i=0;i<workMat.rows();i++)
            for (j=0;j<workMat.columns();j++)
                for (k=0;k<R.columns();k++) {
                    workMat.entry(i,j) += (domain.getInvariantFactor(j) *
                        R.entry(i,k) * C.entry(k,j) ) / dcLpreimage.entry(k,k);
                }

        NMatrixInt dummy( 1, dcLpreimage.columns() );

        kernel = new NMarkedAbelianGroup(dummy, workMat);
    }
}



void NHomMarkedAbelianGroup::computeCokernel() {
    if (!coKernel) {
        computeReducedMatrix();

        NMatrixInt ccrelators( reducedMatrix->rows(),
            reducedMatrix->columns() + range.getNumberOfInvariantFactors() );
        unsigned i,j;
        for (i=0;i<reducedMatrix->rows();i++)
            for (j=0;j<reducedMatrix->columns();j++)
                ccrelators.entry(i,j)=reducedMatrix->entry(i,j);
        for (i=0;i<range.getNumberOfInvariantFactors();i++)
            ccrelators.entry(i,i+reducedMatrix->columns())=
                range.getInvariantFactor(i);

        NMatrixInt ccgenerators( 1, reducedMatrix->rows() );

        coKernel = new NMarkedAbelianGroup(ccgenerators, ccrelators);
    }
}


void NHomMarkedAbelianGroup::computeImage() {
    if (!image) {
        computeReducedKernelLattice();
        const NMatrixInt& dcLpreimage( *reducedKernelLattice );

        unsigned long i,j;

        NMatrixInt imgCCm(1, dcLpreimage.rows() );
        NMatrixInt imgCCn(dcLpreimage.rows(),
            dcLpreimage.columns() + domain.getNumberOfInvariantFactors() );

        for (i=0;i<domain.getNumberOfInvariantFactors();i++)
            imgCCn.entry(i,i) = domain.getInvariantFactor(i);

        for (i=0;i<imgCCn.rows();i++)
            for (j=0;j< dcLpreimage.columns(); j++)
                imgCCn.entry(i,j+domain.getNumberOfInvariantFactors()) =
                    dcLpreimage.entry(i,j);

        image = new NMarkedAbelianGroup(imgCCm, imgCCn);
    }
}

NHomMarkedAbelianGroup NHomMarkedAbelianGroup::operator * (const NHomMarkedAbelianGroup &X) const
{
std::auto_ptr<NMatrixRing<NLargeInteger> > prod=matrix*X.matrix;

NMatrixInt compMat(matrix.rows(), X.matrix.columns() );

for (unsigned long i=0;i<prod->rows();i++) for (unsigned long j=0;j<prod->columns();j++)
            compMat.entry(i,j) = prod->entry(i, j);

return NHomMarkedAbelianGroup(X.domain, range, compMat);
}

std::vector<NLargeInteger> NHomMarkedAbelianGroup::evalCC(const std::vector<NLargeInteger> &input) const
{
static const std::vector<NLargeInteger> nullV;
std::vector<NLargeInteger> retval;

if (domain.isCycle(input))
 {
  retval.resize(matrix.rows(), NLargeInteger::zero);
  for (unsigned long i=0; i<retval.size(); i++) for (unsigned long j=0; j<matrix.columns(); j++)
	retval[i] += input[j]*matrix.entry(i,j);
 } else retval = nullV;
return retval;
}

std::vector<NLargeInteger> NHomMarkedAbelianGroup::evalSNF(const std::vector<NLargeInteger> &input) const
{
const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();
static const std::vector<NLargeInteger> nullV; 
std::vector<NLargeInteger> retval;

if (input.size() == (domain.getRank() + domain.getNumberOfInvariantFactors()) )
 {
   retval.resize(range.getRank()+range.getNumberOfInvariantFactors(), NLargeInteger::zero);
   for (unsigned long i=0; i<retval.size(); i++) for (unsigned long j=0; j<getReducedMatrix().columns(); j++)
	retval[i] += input[j] * getReducedMatrix().entry(i,j);
 } else retval = nullV; 

return retval;
}


void NHomMarkedAbelianGroup::writeReducedMatrix(std::ostream& out) const {
    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();

    unsigned long i,j;
    out<<"Reduced Matrix is "<<reducedMatrix->rows()<<" by "
        <<reducedMatrix->columns()<<" corresponding to domain ";
    domain.writeTextShort(out);
    out<<" and range ";
    range.writeTextShort(out);
    out<<"\n";
    for (i=0;i<reducedMatrix->rows();i++) {
        out<<"[";
        for (j=0;j<reducedMatrix->columns();j++) {
            out<<reducedMatrix->entry(i,j);
            if (j+1 < reducedMatrix->columns()) out<<" ";
        }
        out<<"]\n";
    }
}

void NHomMarkedAbelianGroup::writeTextShort(std::ostream& out) const {
// todo: have completely different set of descriptors if an endomorphism domain == range
//       not so important at the moment though.  New descriptors would include things like
//       automorphism, projection, differential, ... 
    if (isIso())
        out<<"isomorphism";
    else if (isZero()) // zero map
        out<<"zero map";
    else if (isMonic()) { // monic not epic
        out<<"monic, with cokernel ";
        getCokernel().writeTextShort(out);
    } else if (isEpic()) { // epic not monic
        out<<"epic, with kernel ";
        getKernel().writeTextShort(out);
    } else { // nontrivial not epic, not monic
        out<<"kernel ";
        getKernel().writeTextShort(out);
        out<<" | cokernel ";
        getCokernel().writeTextShort(out);
        out<<" | image ";
        getImage().writeTextShort(out);
    }
}


bool NHomMarkedAbelianGroup::isIdentity() const
{
bool retval(true);
if (domain == range)
 {
     const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();
     if (!reducedMatrix->isIdentity()) retval = false;
 } else retval = false;
return retval;
}

bool NHomMarkedAbelianGroup::isChainMap() const
{
// run through a basis for ker OM, plug in and check isCycle()
bool retval(true);
// the last getRankCC() - getRankM() columns of getMRB() are a basis for the kernel of the defining matrix M
for (unsigned long i=domain.getRankM(); i<domain.getRankCC(); i++)
 {
 std::vector<NLargeInteger> domChain(domain.getRankCC(), NLargeInteger::zero);
 for (unsigned long j=0; j<domain.getRankCC(); j++) domChain[j] = domain.getMRB().entry(j,i);
 if (!range.isCycle(evalCC(domChain))) retval = false;
 }

return retval;
}


// I think there's a smart way to compute this, as the reduced matrix is a 2x2 block matrix:
//
//  [A|B]
//  [---]  where D is an inveritble square matrix, 0 is a zero matrix,  
//  [0|D]  A a square matrix and B maybe not square. 
//         the columns of D represent the free factors of the domain, 
// 	   the rows of D the free factors of the range, 
//         the columns/rows of A represent the torsion factors of the domain/range
// 	   respectively.  So the inverse matrix must have the form
//  [A'|B']
//  [-----]
//  [0 |D']  where D' is the inverse of D, A' represents the inverse automorphism of
//           Z_p1 + Z_p2 + ... Z_pk, etc.  And so B' must equal -A'BD', which is readily
//           computable.  So it all boils down to computing A'.  So we need a routine which
//           takes a matrix A representing an automorphism of Z_p1 + ... Z_pk and then computes
//           the matrix representing the inverse automorphism.  
//           So to do this we'll need a new matrixops.cpp command -- call it torsionAutInverse.  

// okay, so torsionAutInverse seems to be okay now. I hope. Lets get this working so we can test 
// it.
NHomMarkedAbelianGroup NHomMarkedAbelianGroup::inverseHom() const
{
const_cast<NHomMarkedAbelianGroup*>(this)->computeReducedMatrix();
NMatrixInt invMat( reducedMatrix->columns(), reducedMatrix->rows() );

if (isIso()) 
 {
  // get A, B, D from reducedMatrix
  // A must be square with domain/range.getNumberOfInvariantFactors() columns
  // D must be square with domain/range.getRank() columns
  // B may not be square with domain.getRank() columns and range.getNumberOfInvariantFactors() rows.
  NMatrixInt A(range.getNumberOfInvariantFactors(), domain.getNumberOfInvariantFactors());
  NMatrixInt B(range.getNumberOfInvariantFactors(), domain.getRank());
  NMatrixInt D(range.getRank(), domain.getRank());
  for (unsigned long i=0; i<A.rows(); i++) for (unsigned long j=0; j<A.columns(); j++)
   A.entry(i,j) = reducedMatrix->entry(i,j);
  for (unsigned long i=0; i<B.rows(); i++) for (unsigned long j=0; j<B.columns(); j++)
   B.entry(i,j) = reducedMatrix->entry(i, j + A.columns());
  for (unsigned long i=0; i<D.rows(); i++) for (unsigned long j=0; j<D.columns(); j++)
   D.entry(i,j) = reducedMatrix->entry( i + A.rows(), j + A.columns() );
  // compute A', B', D'
  // let's use void columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
  //      const std::vector<unsigned> &rowList); from matrixOps to compute the inverse of D.
  NMatrixInt Di(D.rows(), D.columns()); Di.makeIdentity();
  NMatrixInt Dold(D.rows(), D.columns()); Dold.makeIdentity();
  std::vector<unsigned> rowList(D.rows());
  for (unsigned i=0; i<rowList.size(); i++) rowList[i]=i;
  columnEchelonForm(D, Di, Dold, rowList); // now Di is the inverse of the old D, and D is the identity, 
      // and Dold is the old D.
  //NMatrixInt torsionAutInverse(const NMatrixInt& input, const std::vector<NLargeInteger> &invF);
  // can be used to compute A'.  So we need to make a vector containing the invariant factors. 
  std::vector<NLargeInteger> invF(domain.getNumberOfInvariantFactors());
  for (unsigned long i=0; i<invF.size(); i++) invF[i] = domain.getInvariantFactor(i);
  NMatrixInt Ai(torsionAutInverse( A, invF));
  // then Bi is given by Bi = -AiBDi
  NMatrixInt Bi(range.getNumberOfInvariantFactors(), domain.getRank());
  NMatrixInt Btemp(range.getNumberOfInvariantFactors(), domain.getRank());
   // Btemp will give -BDi
   // Bi will be AiBtemp
   // Strangely, we don't seem to have a convienient matrix multiplication available so we just do it
   // `by hand'.  Perhaps this is what Ben's multiplyAs routine is about?  But it isn't used anywhere
   // and the docs are pretty vague.  Ask Ben to clarify.
  for (unsigned long i=0; i<Btemp.rows(); i++) for (unsigned long j=0; j<Btemp.columns(); j++)
   for (unsigned long k=0; k<Btemp.columns(); k++)
    Btemp.entry(i,j) -= B.entry(i,k)*Di.entry(k,j);
  for (unsigned long i=0; i<Bi.rows(); i++) for (unsigned long j=0; j<Bi.columns(); j++)
   for (unsigned long k=0; k<Ai.columns(); k++)
    Bi.entry(i,j) += Ai.entry(i,k)*Btemp.entry(k,j);

  // compute the coefficients of invMat.  We're in the funny situation where we know what invMat's
  // reduced matrix, not its chain-complex level defining matrix.  So we use the alternative
  // NHomMarkedAbelianGroup constructor designed specifically for this situation.  
  // assemble into invMat  [A'|B']
  //                       [-----]
  //                       [0 |D'] 

  for (unsigned long i=0; i<Ai.rows(); i++) for (unsigned long j=0; j<Ai.columns(); j++)
	invMat.entry(i,j) = Ai.entry(i,j);
  for (unsigned long i=0; i<Di.rows(); i++) for (unsigned long j=0; j<Di.columns(); j++)
	invMat.entry(i+Ai.rows(),j+Ai.columns()) = Di.entry(i,j);
  for (unsigned long i=0; i<Bi.rows(); i++) for (unsigned long j=0; j<Bi.columns(); j++)
	invMat.entry(i,j+Ai.columns()) = Bi.entry(i,j);
  // for debugging purposes check that reducedMat of inverseHom is the A' B' 0 D' matrix
 }

return NHomMarkedAbelianGroup( invMat, range, domain );
}


} // namespace regina

