
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

#include "algebra/marked_abeliangroup.h"
#include "maths/matrixops.h"
#include "file/nfile.h"
#include <iostream>

namespace regina {

/** this is a version of Burton's smithNormalForm() algorithm.
        Given matrix, it returns four matrices RowSpaceBasis, ColSpaceBasis,
        RowSpaceBasisInv, ColSpaceBasisInv such that
        ColSpaceBasis * matrix * RowSpaceBasis is in smith normal form.
**/
void RBMOD_smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& RowSpaceBasis, NMatrixInt& RowSpaceBasisInv,
        NMatrixInt& ColSpaceBasis, NMatrixInt& ColSpaceBasisInv) {
    unsigned long currStage = 0;
    unsigned long nonEmptyRows = matrix.rows();
    unsigned long nonEmptyCols = matrix.columns();
    bool flag;
    unsigned long i, j, k;
    NLargeInteger d, u, v, a, b;
    NLargeInteger tmp;

    for (i=0;i<matrix.columns();i++)
        { RowSpaceBasis.entry(i,i)=1;
          RowSpaceBasisInv.entry(i,i)=1; }
    for (i=0;i<matrix.rows();i++)
        { ColSpaceBasis.entry(i,i)=1;
          ColSpaceBasisInv.entry(i,i)=1; }

    while ((currStage < nonEmptyRows) && (currStage < nonEmptyCols)) {
        loopStart:

        // Have we got an empty row?
        flag = true;
        for (i=currStage; i<nonEmptyCols; i++)
            if (matrix.entry(currStage, i) != 0) {
                flag = false; break;
            }
        if (flag) {
            // Empty row!
            if (currStage == nonEmptyRows-1) {
                nonEmptyRows--; continue;
            }
            // Switch it with a row at the bottom.
            for (i=currStage; i<nonEmptyCols; i++)
                {
                matrix.entry(currStage, i).swap(
                    matrix.entry(nonEmptyRows-1, i));
                }
            for (i=0;i<matrix.rows();i++)
                { // Corresponding ops for ColpaceBasis(Inv)
                ColSpaceBasis.entry( currStage, i).swap(
                        ColSpaceBasis.entry( nonEmptyRows-1, i));
                ColSpaceBasisInv.entry( i, currStage ).swap(
                        ColSpaceBasisInv.entry( i, nonEmptyRows-1 ));
                }
                nonEmptyRows--;
            continue;
        }

        // Have we got an empty column?
        flag = true;
        for (i=currStage; i<nonEmptyRows; i++)
            if (matrix.entry(i, currStage) != 0) {
                flag = false; break;
            }
        if (flag) {
            // Empty column!
            if (currStage == nonEmptyCols-1) {
                nonEmptyCols--; continue;
            }
            // Switch it with a column on the end.
            for (i=currStage; i<nonEmptyRows; i++)
                {
                matrix.entry(i, currStage).swap(
                    matrix.entry(i, nonEmptyCols-1));
                }
            for (i=0; i<matrix.columns();i++)
                {
                RowSpaceBasis.entry(i, currStage).swap(
                        RowSpaceBasis.entry(i, nonEmptyCols-1));
                RowSpaceBasisInv.entry(currStage, i).swap(
                    RowSpaceBasisInv.entry(nonEmptyCols-1, i));
                }
            nonEmptyCols--;
            continue;
        }
        // Get zeros in the current row.
        for (i=currStage+1; i<nonEmptyCols; i++) {
            if (matrix.entry(currStage, i) == 0)
                continue;
            // Put a zero in (currStage, i).
            a = matrix.entry(currStage, currStage);
            b = matrix.entry(currStage, i);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to columns currStage and i.
            for (j=currStage; j<nonEmptyRows; j++)
            {
                tmp = u * matrix.entry(j, currStage) +
                    v * matrix.entry(j, i);
                matrix.entry(j, i) = a * matrix.entry(j, i) -
                    b * matrix.entry(j, currStage);
                matrix.entry(j, currStage) = tmp;
            }
             for (j=0; j<matrix.columns(); j++)
                {
                tmp = u * RowSpaceBasis.entry(j, currStage) +
                    v * RowSpaceBasis.entry(j, i);
                RowSpaceBasis.entry(j, i) = a * RowSpaceBasis.entry(j, i) -
                    b * RowSpaceBasis.entry(j, currStage);
                RowSpaceBasis.entry(j, currStage) = tmp;

                tmp = a * RowSpaceBasisInv.entry(currStage, j) +
                    b * RowSpaceBasisInv.entry(i, j);
                RowSpaceBasisInv.entry(i, j) = u * RowSpaceBasisInv.entry(i, j) -
                    v * RowSpaceBasisInv.entry(currStage, j);
                RowSpaceBasisInv.entry(currStage, j) = tmp;
                 }
        }
        // Get zeros in the current column.
        // Check to see if we change anything and thus muck up the row.
        flag = false;
        for (i=currStage+1; i<nonEmptyRows; i++) {
            if (matrix.entry(i, currStage) == 0)
                continue;
            // Put a zero in (i, currStage).
            flag = true;
            a = matrix.entry(currStage, currStage);
            b = matrix.entry(i, currStage);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to rows currStage and i.
            for (j=currStage; j<nonEmptyCols; j++) {
                tmp = u * matrix.entry(currStage, j) +
                    v * matrix.entry(i, j);
                matrix.entry(i, j) = a * matrix.entry(i, j) -
                    b * matrix.entry(currStage, j);
                matrix.entry(currStage, j) = tmp;
            }
            for (j=0; j<matrix.rows(); j++)
                {
                tmp = u * ColSpaceBasis.entry(currStage, j) +
                    v * ColSpaceBasis.entry(i, j);
                ColSpaceBasis.entry(i, j) = a * ColSpaceBasis.entry(i, j) -
                    b * ColSpaceBasis.entry(currStage, j);
                ColSpaceBasis.entry(currStage, j) = tmp;

                tmp = a * ColSpaceBasisInv.entry(j, currStage) +
                    b * ColSpaceBasisInv.entry(j, i);
                ColSpaceBasisInv.entry(j, i) = u * ColSpaceBasisInv.entry(j, i) -
                    v * ColSpaceBasisInv.entry(j, currStage);
                ColSpaceBasisInv.entry(j, currStage) = tmp;
                }
        }
        if (flag) {
            // The clean row was mucked up.
            continue;
        }

        // Check that entry (currStage, currStage) divides everything
        // else.
        for (i=currStage+1; i<nonEmptyRows; i++)
            for (j=currStage+1; j<nonEmptyCols; j++)
                if ((matrix.entry(i, j) % matrix.entry(currStage, currStage))
                        != 0) {
                    // Add row i to the current stage row and start this
                    // stage over.
                    for (k=currStage+1; k<nonEmptyCols; k++)
                        matrix.entry(currStage, k) += matrix.entry(i, k);
                    for (k=0; k<matrix.rows();k++)
                        {
                        ColSpaceBasis.entry(currStage,k) += ColSpaceBasis.entry(i,k);
                        ColSpaceBasisInv.entry(k, i) -= ColSpaceBasisInv.entry(k,currStage);
                        }
                    goto loopStart;
                }
       // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (matrix.entry(currStage, currStage) < 0)
                {
                matrix.entry(currStage, currStage).negate();
                for (j=0; j<matrix.rows(); j++) // we're thinking of this as a row op
                        {
                        ColSpaceBasis.entry( currStage, j ).negate();
                        ColSpaceBasisInv.entry( j,currStage ).negate();
                        }
                }
        currStage++;
    }

}


/** Given a matrix M and a list of rows from M, rowList, this altorithm puts
        M in reduced column echelon form with respect to the rowList.
        It returns the corresponding change of coordinate matrices R and Ri.
        M*R=columnechelon(M), columnechelon(M)Ri = M. M is replaced by its column
        echelon form at the end of the alg. This assumed R and Ri are passed to the
        function as the identity matrix.
        */

void RBADD_columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
                                std::vector<unsigned> &rowList)
{ // R and Ri are square with dimensions equal to M.columns(). If R and Ri begin
  // as the identity matrices, then MR=columnechelon(M) and columnechelon(M)Ri = M.
  // we do this so that if R and Ri already are non-trivial coordinate transformations, they
  // are modified appropriately by the algorithm.

unsigned i,j;

// now we start the column echelon reduction.
// our convention is that a matrix is in column-echelon form if:
//  A) each column is either zero or there is a first non-zero entry which is positive.
//  B) from left-to-right, the first non-zero entries have strictly increasing indices.
//  C) given a first non-zero column entry, in that row all the elements to the left are smaller
//     and non-negative. (all elts to the right are zero by (B)
//  D) in a row which has no first non-zero column entry, all elements are zero.
//  E) thus all the zero columns are on the right-side of the matrix.

unsigned CR=0; unsigned CC=0;
        // these are the indices of the current WORKING rows and columns respectively.
        // thus the entries of M above CR will not change, and to the left of CC all that
        // can happen is some reduction.

std::vector<unsigned> rowNZlist; // in the current row, this is the list of column coordinates
                                // for the non-zero entries.

NLargeInteger d,r; // given two NLargeIntegers a and b, we will represent
                   // a/b by d and a % b by r in the algorithm.
NLargeInteger u,v,gcd, a,b; // for column operations u,v,a,b represent a 2x2 matrix.
NLargeInteger tmp;

while ( (CR<rowList.size()) && (CC<M.columns())) // the algorithm will think of itself as
                                                // working top to bottom.
        {
        // build rowNZlist
        rowNZlist.clear();

        for (i=CC;i<M.columns();i++) if (M.entry(rowList[CR],i) != 0) rowNZlist.push_back(i);

        // now the column operations
        if (rowNZlist.size() == 0) { CR++; continue; } // nothing to do.
        else if (rowNZlist.size() == 1) // let's move this entry to be the leading entry.
                {
                if (rowNZlist[0]==CC)
                        {
                        // step 1: ensure entry(CR,CC) is positive.
                        if (M.entry(rowList[CR],CC)<0)
                                {
                                // step 1: negate column CC.
                                for (i=0;i<M.rows();i++) M.entry(i,CC)=(-M.entry(i,CC));
                                // step 2: modify R, this is a right multiplication so a column op.
                                for (i=0;i<R.rows();i++) R.entry(i,CC)=(-R.entry(i,CC));
                                // step 3: modify Ri, so this is a row op.
                                for (i=0;i<Ri.columns();i++) Ri.entry(CC,i)=(-Ri.entry(CC,i));
                                // done.
                                }
                        // step 2: reduce entries(CR,i) for i<CC
                        for (i=0;i<CC;i++)
                                { // write entry(CR,i) as d*entry(CR,CC) + r.
                                  d = M.entry(rowList[CR],i).divisionAlg( M.entry(rowList[CR],CC), r );
                                  // perform reduction on column i. this is subtracting
                                  // d times column CC from column i.
                                  for (j=0;j<M.rows();j++) M.entry(j,i) =
                                        M.entry(j,i) - d*M.entry(j,CC);
                                  // modify R
                                  for (j=0;j<R.rows();j++) R.entry(j,i) =
                                        R.entry(j,i) - d*R.entry(j,CC);
                                  // modify Ri -- the corresponding row op is addition of
                                        // d times row i to row CC
                                  for (j=0;j<Ri.columns();j++) Ri.entry(CC,j) =
                                        Ri.entry(CC,j) + d*Ri.entry(i,j);
                                  // done.
                                }
                        // done, move on.
                        CC++; CR++;
                        continue;
                        }
                else
                        {
                        // permute column rowNZlist[0] with CC.
                        for (i=0;i<M.rows();i++) M.entry(i,CC).swap(M.entry(i,rowNZlist[0]));
                        // modify R
                        for (i=0;i<R.rows();i++) R.entry(i,CC).swap(R.entry(i,rowNZlist[0]));
                        // modify Ri
                        for (i=0;i<Ri.columns();i++) Ri.entry(CC,i).swap(Ri.entry(rowNZlist[0],i));
                        // done.
                        continue;
                        }
                }
        else        { // there is at least 2 non-zero entries to deal with. we go through them,
                  // one by one, a pair at a time.
                 while (rowNZlist.size()>1)
                        {
                        // do column reduction on columns rowNZlist[0] and rowNZlist[1]
                        // first we need to find the approp modification matrix. This will
                        // be the matrix ( u -b ) where ua+vb = 1. We get a and b from
                        //               ( v a  ) from entry(CR, r[0]) and entry(CR, r[1])
                        // by dividing by their GCD, found with
                        // rowNZlist[0].gcdWithCoeffs(rowNZlist[1],u,v)
                        gcd = M.entry(rowList[CR], rowNZlist[0]).gcdWithCoeffs(
                                M.entry(rowList[CR], rowNZlist[1]), u,v);
                        a = M.entry(rowList[CR], rowNZlist[0]).divExact(gcd);
                        b = M.entry(rowList[CR], rowNZlist[1]).divExact(gcd);
                        // so multiplication on the right by the above matrix corresponds to
                        // replacing column r[0] by u r[0] + v r[1] and column r[1] by
                        // -b r[0] + a r[1].
                        for (i=0;i<M.rows();i++)
                                {
                                tmp = u * M.entry( i, rowNZlist[0] ) + v * M.entry(i, rowNZlist[1] );
                                M.entry(i,rowNZlist[1]) = a * M.entry( i, rowNZlist[1]) -
                                                          b * M.entry( i, rowNZlist[0]);
                                M.entry(i,rowNZlist[0]) = tmp;
                                }
                        // modify R
                        for (i=0;i<R.rows();i++)
                                {
                                tmp = u * R.entry( i, rowNZlist[0] ) + v * R.entry(i, rowNZlist[1] );
                                R.entry(i,rowNZlist[1]) = a * R.entry( i, rowNZlist[1]) -
                                                          b * R.entry( i, rowNZlist[0]);
                                R.entry(i,rowNZlist[0]) = tmp;
                                }
                        // modify Ri
                        for (i=0;i<Ri.columns();i++)
                                {
                                tmp = a * Ri.entry( rowNZlist[0], i ) + b * Ri.entry( rowNZlist[1], i );
                                Ri.entry( rowNZlist[1], i ) = u * Ri.entry( rowNZlist[1], i ) -
                                                              v * Ri.entry( rowNZlist[0], i );
                                Ri.entry( rowNZlist[0], i ) = tmp;
                                }
                        // modify rowNZlist by deleting the entry corresp. to rowNZlist[1]
                        rowNZlist.erase( rowNZlist.begin()+1 );
                        // done.
                        }
                 continue;
                }
        }
}


/** Given a homomorphism from Z^n to Z_p1 + ... + Z_pk, the kernel
        of this homomorphism is some rank-n lattice in Z^n.  This algorithm
        finds a basis for the lattice.  hom is a k by n matrix representing
        the homomorphism and L is a k-vector whose entries are p1 through pk.
        A basis is returned.
        */
NMatrixInt RBADD_preImageOfLattice(NMatrixInt& hom, std::vector<NLargeInteger>& L)
{        // there are two main steps to this algorithm.
        // 1) find a basis for the domain which splits into a) vectors sent to the complement
        //    of the primitive subspace generated by the range lattice and b) a basis of
        //    vectors sent to the primitive subspace generated by the range lattice.
        // 2) modify the basis (b) by column ops to get the preimage of the lattice.
        // step (1) is an application of the columnEchelonForm
        // step (2) starts with another application of columnEchelonForm, but then it
        //              finishes with a variation on it...

unsigned i,j;

NMatrixInt basis(hom.columns(), hom.columns() ); basis.makeIdentity();
NMatrixInt basisi(hom.columns(), hom.columns() ); basisi.makeIdentity();
// and we proceed to modify it solely via column operations.
// one for every column operation performed on homModL
NMatrixInt homModL(hom);

// set up two lists: the coordinates that correspond to free generators of the range
// and coordinates corresponding to torsion generators.

// these lists need to be built from L
std::vector<unsigned> freeList;
std::vector<unsigned> torList;
for (i=0;i<L.size();i++) if (L[i]==0) freeList.push_back(i); else torList.push_back(i);

// so first put the free image part of it in column echelon form

RBADD_columnEchelonForm( homModL, basis, basisi, freeList );

std::vector<unsigned> torCol(0);
bool zeroCol;

for (i=0; i<homModL.columns(); i++)
        {
        zeroCol=true;
        for (j=0; j<freeList.size(); j++) if (homModL.entry( freeList[j], i) != 0) zeroCol=false;
        if (zeroCol) torCol.push_back(i);
        }

// set up a new matrix consisting of columns being sent to the primitive subspace
// generated by the torsion lattice.

NMatrixInt tHom( homModL.rows(), torCol.size() );
NMatrixInt tBasis( basis.rows(), torCol.size() ); // this will be the the eventual retval.
NMatrixInt dummy( torCol.size(), 0 ); // needed when we call RBADD_columnEchelonForm. choosing
                                // it to have 0 columns speeds up the algorithm.

for (i=0;i<tHom.rows();i++) for (j=0;j<tHom.columns();j++)
        tHom.entry(i,j) = homModL.entry(i, torCol[j]);

for (i=0;i<basis.rows();i++) for (j=0;j<torCol.size();j++)
        tBasis.entry(i,j) = basis.entry(i, torCol[j]);

RBADD_columnEchelonForm( tHom, tBasis, dummy, torList );

// so now we have a primitive collection of vectors being sent to the primitive
// subspace generated by the torsion lattice in the target. The idea is to run through
// the rows, for each non-zero row, through a basis change we can ensure there is at most
// one non-zero entry.  multiply this column by the smallest factor so that it is in
// the torsion lattice, repeat. etc.

unsigned CR=0; // current row under consideration. The actual row index will of course
        // be torList[CR] since all other rows are already zero.

std::vector<unsigned> rowNZlist; // in the current row, this is the list of column coordinates
                                // for the non-zero entries.

NLargeInteger d,r; // given two NLargeIntegers a and b, we will represent
                   // a/b by d and a % b by r in the algorithm.
NLargeInteger u,v,gcd, a,b; // for column operations u,v,a,b represent a 2x2 matrix.
NLargeInteger tmp;

while (CR<torList.size())
        {
        // build rowNZlist
        rowNZlist.clear();

        for (i=0;i<tHom.columns();i++) if (tHom.entry(torList[CR],i) != 0) rowNZlist.push_back(i);
        // okay, so now we have a list of non-zero entries.
        // case 1: rowNZlist.size()==0, increment CR, continue;

        if (rowNZlist.size()==0) { CR++; continue; }
        // case 2: rowNZlist.size()==1 multiply column only if neccessary, increment CR, continue;

        if (rowNZlist.size()==1)
                { // check to see if tHom.entry(torList[CR], rowNZlist[0]) % L[torList[CR]] == 0
                  // if not, find smallest positive integer s.t. when multiplied by it, above becomes
                  // true.
                gcd = tHom.entry(torList[CR], rowNZlist[0]).gcd( L[torList[CR]] );
                d = L[torList[CR]].divByExact(gcd);
                // multiply column  rowNZlist[0] of tHom by d.
                for (i=0;i<torList.size();i++) tHom.entry( torList[i], rowNZlist[0] ) *= d;
                // corresponding operation on tBasis.
                for (i=0;i<tBasis.rows();i++) tBasis.entry( i, rowNZlist[0] ) *= d;
                // done.
                CR++; continue;
                }
        // case 3: rowNZlist.size()>1.row ops to reduce rowNZlist.size(). then continue
        while (rowNZlist.size()>1)
                {
                // do column op on columns rowNZlist[0] and rowNZlist[1]
                // first we need to find the approp modification matrix. This will
                // be the matrix ( u -b ) where ua+vb = 1. We get a and b from
                //               ( v a  ) from entry(torList[CR], r[0]) and entry(torlist[CR], r[1])
                // by dividing by their GCD, found with
                // rowNZlist[0].gcdWithCoeffs(rowNZlist[1],u,v)
                gcd = tHom.entry(torList[CR], rowNZlist[0]).gcdWithCoeffs(
                        tHom.entry(torList[CR], rowNZlist[1]), u,v);
                a = tHom.entry(torList[CR], rowNZlist[0]).divExact(gcd);
                b = tHom.entry(torList[CR], rowNZlist[1]).divExact(gcd);
                // so multiplication on the right by the above matrix corresponds to
                // replacing column r[0] by u r[0] + v r[1] and column r[1] by
                // -b r[0] + a r[1].
                for (i=0;i<torList.size();i++)
                        {
                        tmp = u * tHom.entry( torList[i], rowNZlist[0] ) +
                              v * tHom.entry( torList[i], rowNZlist[1] );
                        tHom.entry( torList[i],rowNZlist[1]) =
                              a * tHom.entry( torList[i], rowNZlist[1]) -
                              b * tHom.entry( torList[i], rowNZlist[0]);
                        tHom.entry( torList[i],rowNZlist[0]) = tmp;
                        }
                // modify tBasis
                for (i=0;i<tBasis.rows();i++)
                        {
                        tmp = u * tBasis.entry( i, rowNZlist[0] ) + v * tBasis.entry(i, rowNZlist[1] );
                        tBasis.entry(i,rowNZlist[1]) = a * tBasis.entry( i, rowNZlist[1]) -
                                                       b * tBasis.entry( i, rowNZlist[0]);
                        tBasis.entry(i,rowNZlist[0]) = tmp;
                        }

                // now rowNZlist[1] entry is zero, remove it from the list.
                rowNZlist.erase( rowNZlist.begin()+1 );
                }
        }


return tBasis;
}


unsigned long rbGetRank(const NMatrixInt& M) // I don't know how to avoid using this, unfortunately.
        { // this is needed in MarkedAbelianGroup::MarkedAbelianGroup
        NMatrixInt temp(M);
        smithNormalForm(temp);
        unsigned long i=0;
        while ( (i<temp.rows()) && (i<temp.columns()) && (temp.entry(i,i)!=0) ) i++;
        return i; // i is the rank of M
        }

MarkedAbelianGroup::MarkedAbelianGroup(const NMatrixInt& M, const NMatrixInt& N) :
        OM(M), ON(N), OMR(M.columns(),M.columns()),
        OMC(M.rows(),M.rows()), OMRi(M.columns(),M.columns()),OMCi(M.rows(),M.rows()),
        rankOM(rbGetRank(M)),
        ORN(N.rows()-rbGetRank(M),N.columns()),
        ornR(N.columns(),N.columns()),        ornRi(N.columns(),N.columns()),
        ornC(N.rows()-rbGetRank(M),N.rows()-rbGetRank(M)),
        ornCi(N.rows()-rbGetRank(M),N.rows()-rbGetRank(M)),
        SNF_ORN(N.rows()-rbGetRank(M),N.columns()),
        InvFacList(0), InvFacIndex(0),
        snfrank(0), snffreeindex(0), ifNum(0), ifLoc(0)
 {
 // find SNF(M).
 NMatrixInt tM(M);

 RBMOD_smithNormalForm(tM, OMR, OMRi, OMC, OMCi);
 // now construct OMRi * N, and delete first SNF_OM_firstzero rows, constructing ORN.

 NMatrixRing<NLargeInteger>* prod=OMRi*ON;

 unsigned long i;
 unsigned long j;

 for (i=0;i<ORN.rows();i++) for (j=0;j<ORN.columns();j++) ORN.entry(i,j) =
         prod->entry(i+rankOM,j);

 delete(prod);

 NMatrixInt tX(ORN);
 RBMOD_smithNormalForm(tX, ornR, ornRi, ornC, ornCi);
 for (i=0;i<tX.rows();i++) for (j=0;j<tX.columns();j++)
         SNF_ORN.entry(i,j)=tX.entry(i,j);
 // now build the list of invariant factors and their row indexes
 // now compute the rank and column indexes ...
 i=0;
 unsigned long totO=0; // number diag entries == 1
 unsigned long totIF=0;// number diag entries > 1
 unsigned long totFR=0;// number diag entries == 0

 while ((i<SNF_ORN.rows()) && (i<SNF_ORN.columns()))
        {
        if (SNF_ORN.entry(i,i)==1) totO++;
        else if (SNF_ORN.entry(i,i)>1)
                {
                totIF++;
                InvFacIndex.push_back(i);
                }
        else totFR++;
        i++;
        }

 ifNum=totIF;
 ifLoc=totO;

 InvFacList.resize(InvFacIndex.size());
 for (i=0;i<InvFacList.size();i++)
         InvFacList[i]=SNF_ORN.entry(InvFacIndex[i],InvFacIndex[i]);

 snfrank=SNF_ORN.rows()-totO-totIF;
 snffreeindex=totO+totIF;
}


// this goes through the list of invariant factors in order, stops
// at the `index'-th one in increasing order and returns it...
// if the index is out of bounds, it returns zero.
// invariant factors are stored in the matrix SNF_ORN so one has to
// get past the entries labelled with 1, and avoid any possible tail
// entries labelled with 0.
/** Gives the index-th invariant factor, in increasing order.
        returns 0 for an invalid index. */
const NLargeInteger& MarkedAbelianGroup::getInvariantFactor( unsigned long index) const
{
        return InvFacList[index];
}

unsigned long MarkedAbelianGroup::getNumberOfInvariantFactors() const
{
        return InvFacList.size();
}

unsigned MarkedAbelianGroup::getTorsionRank(const NLargeInteger& degree) const
{
    unsigned ans = 0;
    for (unsigned long i=0;i<InvFacList.size();i++)
        {
        if (InvFacList[i] % degree == 0) ans++;
        }
    return ans;
}


void MarkedAbelianGroup::writeTextShort(std::ostream& out) const {
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

unsigned MarkedAbelianGroup::getRank() const
{
    return snfrank;
}

bool MarkedAbelianGroup::isTrivial() const
{
    bool retval=true;
    if ( (snfrank>0) || (InvFacList.size()>0) ) retval=false;
    return (retval);
}

bool MarkedAbelianGroup::operator == (const MarkedAbelianGroup& other) const
{
    bool retval=false;
    if ((InvFacList == other.InvFacList) && (snfrank==other.snfrank)) retval=true;
    return retval;
}


/*
 * The marked abelian group was defined by matrices M and N
 * with M*N==0.  Think of M as m by l and N as l by n.  Then
 * this routine returns the index-th free generator of the
 * ker(M)/img(N) in Z^l.
 */
std::vector<NLargeInteger> MarkedAbelianGroup::getFreeRep(unsigned long index) const
{
std::vector<NLargeInteger> retval(OM.columns(),"0");
// index corresponds to the (index+snffreeindex)-th column of ornCi
// we then pad this vector (at the front) with rankOM 0's
// and apply OMR to it.

std::vector<NLargeInteger> temp(ornCi.rows()+rankOM,"0");

for (unsigned long i=0;i<ornCi.rows();i++) temp[i+rankOM]=ornCi.entry(i,index+snffreeindex);
// the above line takes the index+snffreeindex-th column of ornCi and pads it.

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
std::vector<NLargeInteger> MarkedAbelianGroup::getTorRep(unsigned long index) const
{
std::vector<NLargeInteger> retval(OM.columns(),NLargeInteger::zero);
// index corresponds to the (InvFacIndex[index])-th column of ornCi
// we then pad this vector (at the front) with rankOM 0's
// and apply OMR to it.

std::vector<NLargeInteger> temp(ornCi.rows()+rankOM,NLargeInteger::zero);

for (unsigned long i=0;i<ornCi.rows();i++) temp[i+rankOM]=ornCi.entry(i,InvFacIndex[index]);
// the above line takes the index+snffreeindex-th column of ornCi and pads it.

for (unsigned long i=0;i<retval.size();i++)
 for (unsigned long j=0;j<OMR.columns();j++)
  retval[i] += OMR.entry(i,j)*temp[j];
// the above takes temp and multiplies it by the matrix OMR.

return retval;
}


/*
 * The marked abelian group was defined by matrices M and N
 * with M*N==0.  Think of M as m by l and N as l by n.
 * When the group was initialized, it was computed to be isomorphic
 * to some Z^d + Z_{d1} + ... + Z_{dk} where d1 | d2 | ... | dk
 * this routine assumes element is in Z^l, and it returns a vector
 * of length d+k where the first d elements represent which class the
 * vector projects to in Z^d, and the last k elements represent the
 * projections to Z_{d1} + ... + Z_{dk}. Of these last elements, they
 * will be returned mod di respectively. Returns an empty vector if
 * element is not in the kernel of M. element is assumed to have
 * OM.columns()==ON.rows() entries.
 */
std::vector<NLargeInteger> MarkedAbelianGroup::getSNFisoRep(std::vector<NLargeInteger>& element)  const
{
std::vector<NLargeInteger> retval(snfrank+InvFacList.size(),NLargeInteger::zero);
// apply OMRi, crop, then apply ornC, tidy up and return.
std::vector<NLargeInteger> nullvec(0); // this is returned if element not in ker(M)

std::vector<NLargeInteger> temp(ON.rows(),NLargeInteger::zero); // this holds OMRi * element
        // if first rankOM entries are 0, then element is in the kernel

bool eltinker=true;

// set up temp.
for (unsigned long i=0;i<ON.rows();i++) for (unsigned long j=0;j<ON.rows();i++)
        temp[i] += OMRi.entry(i,j)*element[j];
for (unsigned long i=0;i<rankOM;i++) if (temp[i] != 0) eltinker=false;
// ON.rows - rankOM == ORN.rows()
if (eltinker==true)
        {
        // set up retval. The first snfrank elts are the free generators
        for (unsigned long i=0;i<snfrank;i++)
                for (unsigned long j=rankOM;j<ON.rows();i++)
                retval[i] += ornC.entry(snffreeindex+i,j)*temp[j];
        // the remaining InvFacList.size() elts are torsion generators.
        for (unsigned long i=0;i<ifNum;i++)
                {
                for (unsigned long j=rankOM;j<ON.rows();i++)
                retval[i+snfrank] += ornC.entry(ifLoc+i,j)*temp[j];
                retval[i+snfrank] = (retval[i+snfrank] % InvFacList[i]);
                }
        }
else retval=nullvec;


return retval;
}


HomMarkedAbelianGroup::HomMarkedAbelianGroup(const MarkedAbelianGroup& dom,
                               const MarkedAbelianGroup& ran,
                              const NMatrixInt &mat):
domain(dom), range(ran), matrix(mat),
reducedMatrixComputed(false), reducedMatrix(0),
kernelComputed(false), kernel(0),
coKernelComputed(false), coKernel(0),
imageComputed(false), image(0),
reducedKernelLatticeComputed(false), reducedKernelLattice(0)
{}


HomMarkedAbelianGroup::HomMarkedAbelianGroup(const HomMarkedAbelianGroup& g):
domain(g.domain), range(g.range), matrix(g.matrix)
{
reducedMatrixComputed=g.reducedMatrixComputed;
kernelComputed=g.kernelComputed;
coKernelComputed=g.coKernelComputed;
imageComputed=g.imageComputed;
reducedKernelLatticeComputed=g.reducedKernelLatticeComputed;

if (reducedMatrixComputed)
        {
        reducedMatrix = new NMatrixInt(*g.reducedMatrix);
        } else reducedMatrix = 0;
if (kernelComputed)
        {
        kernel = new MarkedAbelianGroup(*g.kernel);
        } else kernel = 0;
if (coKernelComputed)
        {
        coKernel = new MarkedAbelianGroup(*g.coKernel);
        } else coKernel = 0;
if (imageComputed)
        {
        image = new MarkedAbelianGroup(*g.image);
        } else image = 0;
if (reducedKernelLatticeComputed)
        {
        reducedKernelLattice = new NMatrixInt(*g.reducedKernelLattice);
        } else reducedKernelLattice = 0;
}

void HomMarkedAbelianGroup::computeReducedMatrix()
{
if (!reducedMatrixComputed)
 {
 unsigned long i,j,k;

 NMatrixInt kerMatrix( matrix.rows()-range.getRankOM(),
                      matrix.columns()-domain.getRankOM() );
        // kerMatrix = truncate (range.getMRBi() * matrix * domain.getMRBi)
        // to construct this we do it in two steps:
        // step 1) temp1 = truncate columns (matrix * domain.getMRBi )
        // step 2) kerMatrix = truncate rows (range.getMRBi * temp1 )

 NMatrixInt dcckb(domain.getMRB());
 NMatrixInt rcckb(range.getMRBi());

 NMatrixInt temp1( matrix.rows(), matrix.columns()-domain.getRankOM() );
 for (i=0;i<temp1.rows();i++) for (j=0;j<temp1.columns();j++)
  for (k=0;k<matrix.columns();k++) temp1.entry(i,j) += matrix.entry(i,k) *
                          dcckb.entry(k,j + domain.getRankOM() );

 for (i=0;i<kerMatrix.rows();i++) for (j=0;j<kerMatrix.columns();j++)
  for (k=0;k<rcckb.rows();k++)
   kerMatrix.entry(i,j) += rcckb.entry(i+range.getRankOM(), k) * temp1.entry(k,j);

 NMatrixInt redMatrix( kerMatrix.rows()-range.getTorLoc(),
                      kerMatrix.columns()-domain.getTorLoc() );

 NMatrixInt dccqb(domain.getNCBi());
 NMatrixInt rccqb(range.getNCB());

 NMatrixInt temp2( kerMatrix.rows(),
        kerMatrix.columns() - domain.getTorLoc() );
 for (i=0;i<temp2.rows();i++) for (j=0;j<temp2.columns();j++)
  for (k=0;k<kerMatrix.columns();k++)
         {temp2.entry(i,j) += kerMatrix.entry(i,k) *
                         dccqb.entry(k,j + domain.getTorLoc() ); }

 for (i=0;i<redMatrix.rows();i++) for (j=0;j<redMatrix.columns();j++)
  for (k=0;k<rccqb.rows();k++) redMatrix.entry(i,j) +=
          rccqb.entry(i+range.getTorLoc(), k) * temp2.entry(k,j);

 reducedMatrix = new NMatrixInt(redMatrix);
 reducedMatrixComputed=true;
}}

void HomMarkedAbelianGroup::computeReducedKernelLattice()
{
 if (!reducedKernelLattice)
 {
 computeReducedMatrix();

 unsigned long i;

 NMatrixInt redMatrix(*reducedMatrix);

 // the kernel is the dcLpreimage lattice mod the domain lattice.
 // so after computing the dcLpreimage lattice, we need to represent
 // the domain lattice in its coordinates.

 std::vector<NLargeInteger> dcL(range.getRank() + range.getNumberOfInvariantFactors() );
 for (i=0; i<dcL.size(); i++) if (i<range.getNumberOfInvariantFactors())
        dcL[i]=range.getInvariantFactor(i); else dcL[i]="0";

 reducedKernelLatticeComputed = true;
 reducedKernelLattice = new NMatrixInt( RBADD_preImageOfLattice( redMatrix, dcL ) );
 }
}

void HomMarkedAbelianGroup::computeKernel()
{
 if (!kernelComputed)
 {
 computeReducedKernelLattice();
 NMatrixInt dcLpreimage( *reducedKernelLattice );

 unsigned long i,j,k;

 NMatrixInt R( dcLpreimage.columns(), dcLpreimage.columns() );
 NMatrixInt Ri( dcLpreimage.columns(), dcLpreimage.columns() );
 NMatrixInt C( dcLpreimage.rows(), dcLpreimage.rows() );
 NMatrixInt Ci( dcLpreimage.rows(), dcLpreimage.rows() );

 RBMOD_smithNormalForm( dcLpreimage, R, Ri, C, Ci );

 // the matrix representing the domain lattice in dcLpreimage coordinates is
 // given by domainLattice * R * (dcLpreimage inverse) * C

 NMatrixInt workMat( dcLpreimage.columns(), domain.getNumberOfInvariantFactors() );

 for (i=0;i<workMat.rows();i++) for (j=0;j<workMat.columns();j++)
        for (k=0;k<R.columns();k++)
        {
        workMat.entry(i,j) += (domain.getInvariantFactor(j) *
                R.entry(i,k) * C.entry(k,j) ) / dcLpreimage.entry(k,k);
        }

 NMatrixInt dummy( 1, dcLpreimage.columns() );

 MarkedAbelianGroup retval( dummy, workMat );

 kernelComputed=true;
 kernel = new MarkedAbelianGroup(retval);
}}



void HomMarkedAbelianGroup::computeCoKernel()
{
if (!coKernelComputed)
 {
 computeReducedMatrix();

 NMatrixInt ccrelators( reducedMatrix->rows(), reducedMatrix->columns() +
                        range.getNumberOfInvariantFactors() );
 unsigned i,j;
 for (i=0;i<reducedMatrix->rows();i++) for (j=0;j<reducedMatrix->columns();j++)
        ccrelators.entry(i,j)=reducedMatrix->entry(i,j);
 for (i=0;i<range.getNumberOfInvariantFactors();i++)
        ccrelators.entry(i,i+reducedMatrix->columns())=range.getInvariantFactor(i);

 NMatrixInt ccgenerators( 1, reducedMatrix->rows() );
 MarkedAbelianGroup retval(ccgenerators, ccrelators);

 coKernelComputed=true;
 coKernel = new MarkedAbelianGroup(retval);
}}




void HomMarkedAbelianGroup::computeImage()
{
if (!imageComputed)
 {
 computeReducedKernelLattice();
 NMatrixInt dcLpreimage( *reducedKernelLattice );

 unsigned long i,j;

 NMatrixInt imgCCm(1, dcLpreimage.rows() );
 NMatrixInt imgCCn(dcLpreimage.rows(),  dcLpreimage.columns() + domain.getNumberOfInvariantFactors() );

 for (i=0;i<domain.getNumberOfInvariantFactors();i++)
        imgCCn.entry(i,i) = domain.getInvariantFactor(i);

 for (i=0;i<imgCCn.rows();i++) for (j=0;j< dcLpreimage.columns(); j++)
        imgCCn.entry(i,j+domain.getNumberOfInvariantFactors()) = dcLpreimage.entry(i,j);

 MarkedAbelianGroup retval(imgCCm, imgCCn);

 imageComputed=true;
 image = new MarkedAbelianGroup(retval);
 }
}



bool HomMarkedAbelianGroup::isEpic()
{
        bool retval=false;
        if (getCoKernel().isTrivial()) retval=true;
        return retval;
}

bool HomMarkedAbelianGroup::isMonic()
{
        bool retval=false;
        if (getKernel().isTrivial()) retval=true;
        return retval;
}

bool HomMarkedAbelianGroup::isIso()
{
        bool retval=false;
        if (getCoKernel().isTrivial() && getKernel().isTrivial())
                retval=true;
        return retval;
}

bool HomMarkedAbelianGroup::isZero()
{
        bool retval=false;
        if (getImage().isTrivial())
                retval=true;
        return retval;
}



MarkedAbelianGroup HomMarkedAbelianGroup::getKernel()
{
computeKernel();
return *kernel;
}

MarkedAbelianGroup HomMarkedAbelianGroup::getImage()
{
computeImage();
return *image;
}

MarkedAbelianGroup HomMarkedAbelianGroup::getCoKernel()
{
computeCoKernel();
return *coKernel;
}



void HomMarkedAbelianGroup::writeRedMatrix(std::ostream& out)
{
computeReducedMatrix();
unsigned long i,j;
out<<"Reduced Matrix is "<<reducedMatrix->rows()<<" by "<<reducedMatrix->columns()<<
        " corresponding to domain ";
domain.writeTextShort(out);
out<<" and range ";
range.writeTextShort(out);
out<<"\n";
for (i=0;i<reducedMatrix->rows();i++)
        {
        out<<"[";
        for (j=0;j<reducedMatrix->columns();j++)
                {
                out<<reducedMatrix->entry(i,j);
                if (j+1 < reducedMatrix->columns()) out<<" ";
                }
        out<<"]\n";
        }

}


void HomMarkedAbelianGroup::writeTextShort(std::ostream& out)
{

if (isIso()) out<<"isomorphism "; else
if (isZero()) out<<"zero map "; else
if (isMonic())
         { // monic not epic
          out<<"monic, with cokernel ";
          getCoKernel().writeTextShort(out);
         } else
if (isEpic())
         { // epic not monic
          out<<"epic, with kernel ";
          getKernel().writeTextShort(out);
         }
else
         { // nontrivial not epic, not monic
          out<<"kernel ";
          getKernel().writeTextShort(out);
          out<<" | cokernel ";
           getCoKernel().writeTextShort(out);
          out<<" | image ";
          getImage().writeTextShort(out);
          }

}







} // namespace regina



