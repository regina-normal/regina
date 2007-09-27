
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

#include "maths/matrixops.h"
#include "maths/numbertheory.h"

namespace regina {

void smithNormalForm(NMatrixInt& matrix) {
    unsigned long currStage = 0;
    unsigned long nonEmptyRows = matrix.rows();
    unsigned long nonEmptyCols = matrix.columns();
    bool flag;
    unsigned long i, j, k;
    NLargeInteger d, u, v, a, b;
    NLargeInteger tmp;
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
                matrix.entry(currStage, i).swap(
                    matrix.entry(nonEmptyRows-1, i));
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
                matrix.entry(i, currStage).swap(
                    matrix.entry(i, nonEmptyCols-1));
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
            for (j=currStage; j<nonEmptyRows; j++) {
                tmp = u * matrix.entry(j, currStage) +
                    v * matrix.entry(j, i);
                matrix.entry(j, i) = a * matrix.entry(j, i) -
                    b * matrix.entry(j, currStage);
                matrix.entry(j, currStage) = tmp;
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
                        matrix.entry(currStage, k) +=
                            matrix.entry(i, k);
                    goto loopStart;
                }

        // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (matrix.entry(currStage, currStage) < 0)
            matrix.entry(currStage, currStage).negate();
        currStage++;
    }
}

void smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& RowSpaceBasis, NMatrixInt& RowSpaceBasisInv,
        NMatrixInt& ColSpaceBasis, NMatrixInt& ColSpaceBasisInv) {
    unsigned long currStage = 0;
    unsigned long nonEmptyRows = matrix.rows();
    unsigned long nonEmptyCols = matrix.columns();
    bool flag;
    unsigned long i, j, k;
    NLargeInteger d, u, v, a, b;
    NLargeInteger tmp;

    for (i=0;i<matrix.columns();i++) {
        RowSpaceBasis.entry(i,i)=1;
        RowSpaceBasisInv.entry(i,i)=1;
    }
    for (i=0;i<matrix.rows();i++) {
        ColSpaceBasis.entry(i,i)=1;
        ColSpaceBasisInv.entry(i,i)=1;
    }

    while ((currStage < nonEmptyRows) && (currStage < nonEmptyCols)) {
        loopStart:

        // Have we got an empty row?
        flag = true;
        for (i=currStage; i<nonEmptyCols; i++)
            if (matrix.entry(currStage, i) != 0) {
                flag = false;
                break;
            }
        if (flag) {
            // Empty row!
            if (currStage == nonEmptyRows-1) {
                nonEmptyRows--;
                continue;
            }
            // Switch it with a row at the bottom.
            for (i=currStage; i<nonEmptyCols; i++) {
                matrix.entry(currStage, i).swap(
                    matrix.entry(nonEmptyRows-1, i));
            }
            for (i=0;i<matrix.rows();i++) {
                // Corresponding ops for ColpaceBasis(Inv)
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
                flag = false;
                break;
            }
        if (flag) {
            // Empty column!
            if (currStage == nonEmptyCols-1) {
                nonEmptyCols--;
                continue;
            }
            // Switch it with a column on the end.
            for (i=currStage; i<nonEmptyRows; i++) {
                matrix.entry(i, currStage).swap(
                    matrix.entry(i, nonEmptyCols-1));
            }
            for (i=0; i<matrix.columns();i++) {
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
            for (j=currStage; j<nonEmptyRows; j++) {
                tmp = u * matrix.entry(j, currStage) +
                    v * matrix.entry(j, i);
                matrix.entry(j, i) = a * matrix.entry(j, i) -
                    b * matrix.entry(j, currStage);
                matrix.entry(j, currStage) = tmp;
            }
            for (j=0; j<matrix.columns(); j++) {
                tmp = u * RowSpaceBasis.entry(j, currStage) +
                    v * RowSpaceBasis.entry(j, i);
                RowSpaceBasis.entry(j, i) = a * RowSpaceBasis.entry(j, i) -
                    b * RowSpaceBasis.entry(j, currStage);
                RowSpaceBasis.entry(j, currStage) = tmp;

                tmp = a * RowSpaceBasisInv.entry(currStage, j) +
                    b * RowSpaceBasisInv.entry(i, j);
                RowSpaceBasisInv.entry(i, j) =
                    u * RowSpaceBasisInv.entry(i, j) -
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
            for (j=0; j<matrix.rows(); j++) {
                tmp = u * ColSpaceBasis.entry(currStage, j) +
                    v * ColSpaceBasis.entry(i, j);
                ColSpaceBasis.entry(i, j) = a * ColSpaceBasis.entry(i, j) -
                    b * ColSpaceBasis.entry(currStage, j);
                ColSpaceBasis.entry(currStage, j) = tmp;

                tmp = a * ColSpaceBasisInv.entry(j, currStage) +
                    b * ColSpaceBasisInv.entry(j, i);
                ColSpaceBasisInv.entry(j, i) =
                    u * ColSpaceBasisInv.entry(j, i) -
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
                    for (k=0; k<matrix.rows();k++) {
                        ColSpaceBasis.entry(currStage,k) +=
                            ColSpaceBasis.entry(i,k);
                        ColSpaceBasisInv.entry(k, i) -=
                            ColSpaceBasisInv.entry(k,currStage);
                    }
                    goto loopStart;
                }
        // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (matrix.entry(currStage, currStage) < 0) {
            matrix.entry(currStage, currStage).negate();
            for (j=0; j<matrix.rows(); j++) {
                // we're thinking of this as a row op
                ColSpaceBasis.entry( currStage, j ).negate();
                ColSpaceBasisInv.entry( j,currStage ).negate();
            }
        }
        currStage++;
    }

}

void columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
        const std::vector<unsigned> &rowList) {
    unsigned i,j;

    unsigned CR=0;
    unsigned CC=0;
    // these are the indices of the current WORKING rows and columns
    // respectively.
    // thus the entries of M above CR will not change, and to the left of
    // CC all that can happen is some reduction.

    std::vector<unsigned> rowNZlist; // in the current row, this is the
                                     // list of column coordinates
                                     // for the non-zero entries.

    NLargeInteger d,r; // given two NLargeIntegers a and b, we will represent
                       // a/b by d and a % b by r in the algorithm.
    NLargeInteger u,v,gcd, a,b; // for column operations u,v,a,b represent
                                // a 2x2 matrix.
    NLargeInteger tmp;

    // the algorithm will think of itself as working top to bottom.
    while ( (CR<rowList.size()) && (CC<M.columns())) {
        // build rowNZlist
        rowNZlist.clear();

        for (i=CC;i<M.columns();i++)
            if (M.entry(rowList[CR],i) != 0)
                rowNZlist.push_back(i);

        // now the column operations
        if (rowNZlist.size() == 0) {
            // nothing to do.
            CR++;
            continue;
        } else if (rowNZlist.size() == 1) {
            // let's move this entry to be the leading entry.
            if (rowNZlist[0]==CC) {
                // step 1: ensure entry(CR,CC) is positive.
                if (M.entry(rowList[CR],CC)<0) {
                    // step 1: negate column CC.
                    for (i=0;i<M.rows();i++)
                        M.entry(i,CC)=(-M.entry(i,CC));
                    // step 2: modify R, this is a right multiplication so a
                    // column op.
                    for (i=0;i<R.rows();i++)
                        R.entry(i,CC)=(-R.entry(i,CC));
                    // step 3: modify Ri, so this is a row op.
                    for (i=0;i<Ri.columns();i++)
                        Ri.entry(CC,i)=(-Ri.entry(CC,i));
                    // done.
                }
                // step 2: reduce entries(CR,i) for i<CC
                for (i=0;i<CC;i++) { // write entry(CR,i) as d*entry(CR,CC) + r.
                    d = M.entry(rowList[CR],i).divisionAlg(
                        M.entry(rowList[CR],CC), r );
                    // perform reduction on column i. this is subtracting
                    // d times column CC from column i.
                    for (j=0;j<M.rows();j++)
                        M.entry(j,i) = M.entry(j,i) - d*M.entry(j,CC);
                    // modify R
                    for (j=0;j<R.rows();j++)
                        R.entry(j,i) = R.entry(j,i) - d*R.entry(j,CC);
                    // modify Ri -- the corresponding row op is addition of
                    // d times row i to row CC
                    for (j=0;j<Ri.columns();j++)
                        Ri.entry(CC,j) = Ri.entry(CC,j) + d*Ri.entry(i,j);
                    // done.
                }
                // done, move on.
                CC++;
                CR++;
                continue;
            } else {
                // permute column rowNZlist[0] with CC.
                for (i=0;i<M.rows();i++)
                    M.entry(i,CC).swap(M.entry(i,rowNZlist[0]));
                // modify R
                for (i=0;i<R.rows();i++)
                    R.entry(i,CC).swap(R.entry(i,rowNZlist[0]));
                // modify Ri
                for (i=0;i<Ri.columns();i++)
                    Ri.entry(CC,i).swap(Ri.entry(rowNZlist[0],i));
                // done.
                continue;
            }
        } else {
            // there is at least 2 non-zero entries to deal with. we go
            // through them, one by one, a pair at a time.
            while (rowNZlist.size()>1) {
                // do column reduction on columns rowNZlist[0] and rowNZlist[1]
                // first we need to find the approp modification matrix.
                // This will be the matrix ( u -b ) where ua+vb = 1. We get
                //                         ( v a  )
                // a and b from entry(CR, r[0]) and entry(CR, r[1])
                // by dividing by their GCD, found with
                // rowNZlist[0].gcdWithCoeffs(rowNZlist[1],u,v)
                gcd = M.entry(rowList[CR], rowNZlist[0]).gcdWithCoeffs(
                    M.entry(rowList[CR], rowNZlist[1]), u,v);
                a = M.entry(rowList[CR], rowNZlist[0]).divExact(gcd);
                b = M.entry(rowList[CR], rowNZlist[1]).divExact(gcd);
                // so multiplication on the right by the above matrix
                // corresponds to replacing column r[0] by u r[0] + v r[1]
                // and column r[1] by -b r[0] + a r[1].
                for (i=0;i<M.rows();i++) {
                    tmp = u * M.entry( i, rowNZlist[0] ) +
                        v * M.entry(i, rowNZlist[1] );
                    M.entry(i,rowNZlist[1]) = a * M.entry( i, rowNZlist[1]) -
                        b * M.entry( i, rowNZlist[0]);
                    M.entry(i,rowNZlist[0]) = tmp;
                }
                // modify R
                for (i=0;i<R.rows();i++) {
                    tmp = u * R.entry( i, rowNZlist[0] ) +
                        v * R.entry(i, rowNZlist[1] );
                    R.entry(i,rowNZlist[1]) = a * R.entry( i, rowNZlist[1]) -
                        b * R.entry( i, rowNZlist[0]);
                    R.entry(i,rowNZlist[0]) = tmp;
                }
                // modify Ri
                for (i=0;i<Ri.columns();i++) {
                    tmp = a * Ri.entry( rowNZlist[0], i ) +
                        b * Ri.entry( rowNZlist[1], i );
                    Ri.entry( rowNZlist[1], i ) =
                        u * Ri.entry( rowNZlist[1], i ) -
                        v * Ri.entry( rowNZlist[0], i );
                    Ri.entry( rowNZlist[0], i ) = tmp;
                }
                // modify rowNZlist by deleting the entry corresponding to
                // rowNZlist[1]
                rowNZlist.erase( rowNZlist.begin()+1 );
                // done.
            }
            continue;
        }
    }
}


NMatrixInt preImageOfLattice(const NMatrixInt& hom,
        const std::vector<NLargeInteger>& L) {
    // there are two main steps to this algorithm.
    // 1) find a basis for the domain which splits into a) vectors sent to the
    //    complement of the primitive subspace generated by the range lattice
    //    and b) a basis of vectors sent to the primitive subspace generated
    //    by the range lattice.
    // 2) modify the basis (b) by column ops to get the preimage of the lattice.
    // step (1) is an application of the columnEchelonForm
    // step (2) starts with another application of columnEchelonForm, but then
    // it finishes with a variation on it...

    unsigned i,j;

    NMatrixInt basis(hom.columns(), hom.columns() );
    basis.makeIdentity();
    NMatrixInt basisi(hom.columns(), hom.columns() );
    basisi.makeIdentity();
    // and we proceed to modify it solely via column operations.
    // one for every column operation performed on homModL
    NMatrixInt homModL(hom);

    // set up two lists: the coordinates that correspond to free generators
    // of the range and coordinates corresponding to torsion generators.

    // these lists need to be built from L
    std::vector<unsigned> freeList;
    std::vector<unsigned> torList;
    for (i=0;i<L.size();i++)
        if (L[i]==0)
            freeList.push_back(i);
        else
            torList.push_back(i);

    // so first put the free image part of it in column echelon form

    columnEchelonForm( homModL, basis, basisi, freeList );

    std::vector<unsigned> torCol(0);
    bool zeroCol;

    for (i=0; i<homModL.columns(); i++) {
        zeroCol=true;
        for (j=0; j<freeList.size(); j++)
            if (homModL.entry( freeList[j], i) != 0)
                zeroCol=false;
        if (zeroCol)
            torCol.push_back(i);
    }

    // set up a new matrix consisting of columns being sent to the primitive
    // subspace generated by the torsion lattice.

    NMatrixInt tHom( homModL.rows(), torCol.size() );
    NMatrixInt tBasis( basis.rows(), torCol.size() ); // this will be the
                                                      // eventual retval.
    NMatrixInt dummy( torCol.size(), 0 ); // needed when we call
    // columnEchelonForm. choosing it to have 0 columns speeds up
    // the algorithm.

    for (i=0;i<tHom.rows();i++)
        for (j=0;j<tHom.columns();j++)
            tHom.entry(i,j) = homModL.entry(i, torCol[j]);

    for (i=0;i<basis.rows();i++)
        for (j=0;j<torCol.size();j++)
            tBasis.entry(i,j) = basis.entry(i, torCol[j]);

    columnEchelonForm( tHom, tBasis, dummy, torList );

    // so now we have a primitive collection of vectors being sent to the
    // primitive subspace generated by the torsion lattice in the target.
    // The idea is to run through the rows, for each non-zero row, through
    // a basis change we can ensure there is at most one non-zero entry.
    // multiply this column by the smallest factor so that it is in
    // the torsion lattice, repeat. etc.

    unsigned CR=0; // current row under consideration. The actual row index
    // will of course be torList[CR] since all other rows are already zero.

    std::vector<unsigned> rowNZlist; // in the current row, this is the list
    // of column coordinates for the non-zero entries.

    NLargeInteger d,r; // given two NLargeIntegers a and b, we will represent
    // a/b by d and a % b by r in the algorithm.
    NLargeInteger u,v,gcd, a,b; // for column operations u,v,a,b represent
    // a 2x2 matrix.
    NLargeInteger tmp;

    while (CR<torList.size()) {
        // build rowNZlist
        rowNZlist.clear();

        for (i=0;i<tHom.columns();i++)
            if (tHom.entry(torList[CR],i) != 0)
                rowNZlist.push_back(i);
        // okay, so now we have a list of non-zero entries.
        // case 1: rowNZlist.size()==0, increment CR, continue;

        if (rowNZlist.size()==0) {
            CR++;
            continue;
        }
        // case 2: rowNZlist.size()==1 multiply column only if neccessary,
        // increment CR, continue;

        if (rowNZlist.size()==1) {
            // check to see if tHom.entry(torList[CR], rowNZlist[0]) %
            // L[torList[CR]] == 0 if not, find smallest positive integer
            // s.t. when multiplied by it, above becomes true.
            gcd = tHom.entry(torList[CR], rowNZlist[0]).gcd( L[torList[CR]] );
            d = L[torList[CR]].divExact(gcd); // devByExact changed to divExact Sept 26th, 2007
            // multiply column  rowNZlist[0] of tHom by d.
            for (i=0;i<torList.size();i++)
                tHom.entry( torList[i], rowNZlist[0] ) *= d;
            // corresponding operation on tBasis.
            for (i=0;i<tBasis.rows();i++)
                tBasis.entry( i, rowNZlist[0] ) *= d;
            // done.
            CR++;
            continue;
        }
        // case 3: rowNZlist.size()>1.row ops to reduce rowNZlist.size().
        // then continue
        while (rowNZlist.size()>1) {
            // do column op on columns rowNZlist[0] and rowNZlist[1]
            // first we need to find the approp modification matrix. This will
            // be the matrix ( u -b ) where ua+vb = 1. We get a and b from
            //               ( v a  ) from entry(torList[CR], r[0]) and
            // entry(torlist[CR], r[1]) by dividing by their GCD, found with
            // rowNZlist[0].gcdWithCoeffs(rowNZlist[1],u,v)
            gcd = tHom.entry(torList[CR], rowNZlist[0]).gcdWithCoeffs(
                        tHom.entry(torList[CR], rowNZlist[1]), u,v);
            a = tHom.entry(torList[CR], rowNZlist[0]).divExact(gcd);
            b = tHom.entry(torList[CR], rowNZlist[1]).divExact(gcd);
            // so multiplication on the right by the above matrix corresponds
            // to replacing column r[0] by u r[0] + v r[1] and column r[1] by
            // -b r[0] + a r[1].
            for (i=0;i<torList.size();i++) {
                tmp = u * tHom.entry( torList[i], rowNZlist[0] ) +
                    v * tHom.entry( torList[i], rowNZlist[1] );
                tHom.entry( torList[i],rowNZlist[1]) =
                    a * tHom.entry( torList[i], rowNZlist[1]) -
                    b * tHom.entry( torList[i], rowNZlist[0]);
                tHom.entry( torList[i],rowNZlist[0]) = tmp;
            }
            // modify tBasis
            for (i=0;i<tBasis.rows();i++) {
                tmp = u * tBasis.entry( i, rowNZlist[0] ) +
                    v * tBasis.entry(i, rowNZlist[1] );
                tBasis.entry(i,rowNZlist[1]) =
                    a * tBasis.entry( i, rowNZlist[1]) -
                    b * tBasis.entry( i, rowNZlist[0]);
                tBasis.entry(i,rowNZlist[0]) = tmp;
            }

            // now rowNZlist[1] entry is zero, remove it from the list.
            rowNZlist.erase( rowNZlist.begin()+1 );
        }
    }


    return tBasis;
}


} // namespace regina

