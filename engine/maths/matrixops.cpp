
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

#include "maths/matrixops.h"
#include "maths/numbertheory.h"

namespace regina {

void smithNormalForm(MatrixInt& matrix) {
    unsigned long currStage = 0;
    unsigned long nonEmptyRows = matrix.rows();
    unsigned long nonEmptyCols = matrix.columns();
    bool flag;
    unsigned long i, j, k;
    Integer d, u, v, a, b;
    Integer tmp;
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


void smithNormalForm(MatrixInt& matrix,
        MatrixInt& rowSpaceBasis, MatrixInt& rowSpaceBasisInv,
        MatrixInt& colSpaceBasis, MatrixInt& colSpaceBasisInv) {
    unsigned long currStage = 0;
    unsigned long nonEmptyRows = matrix.rows();
    unsigned long nonEmptyCols = matrix.columns();
    bool flag;
    unsigned long i, j, k;
    Integer d, u, v, a, b;
    Integer tmp;

    rowSpaceBasis.makeIdentity();
    rowSpaceBasisInv.makeIdentity();
    colSpaceBasis.makeIdentity();
    colSpaceBasisInv.makeIdentity();

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
            colSpaceBasis.swapRows(currStage, nonEmptyRows-1);
            colSpaceBasisInv.swapColumns(currStage, nonEmptyRows-1);

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
            rowSpaceBasis.swapColumns(currStage, nonEmptyCols-1);
            rowSpaceBasisInv.swapRows(currStage, nonEmptyCols-1);

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
                tmp = u * rowSpaceBasis.entry(j, currStage) +
                    v * rowSpaceBasis.entry(j, i);
                rowSpaceBasis.entry(j, i) = a * rowSpaceBasis.entry(j, i) -
                    b * rowSpaceBasis.entry(j, currStage);
                rowSpaceBasis.entry(j, currStage) = tmp;

                tmp = a * rowSpaceBasisInv.entry(currStage, j) +
                    b * rowSpaceBasisInv.entry(i, j);
                rowSpaceBasisInv.entry(i, j) =
                    u * rowSpaceBasisInv.entry(i, j) -
                    v * rowSpaceBasisInv.entry(currStage, j);
                rowSpaceBasisInv.entry(currStage, j) = tmp;
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
                tmp = u * colSpaceBasis.entry(currStage, j) +
                    v * colSpaceBasis.entry(i, j);
                colSpaceBasis.entry(i, j) = a * colSpaceBasis.entry(i, j) -
                    b * colSpaceBasis.entry(currStage, j);
                colSpaceBasis.entry(currStage, j) = tmp;

                tmp = a * colSpaceBasisInv.entry(j, currStage) +
                    b * colSpaceBasisInv.entry(j, i);
                colSpaceBasisInv.entry(j, i) =
                    u * colSpaceBasisInv.entry(j, i) -
                    v * colSpaceBasisInv.entry(j, currStage);
                colSpaceBasisInv.entry(j, currStage) = tmp;
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
                    colSpaceBasis.addRow(i, currStage);
                    colSpaceBasisInv.addCol(currStage, i, -1);

                    goto loopStart;
                }

        // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (matrix.entry(currStage, currStage) < 0) {
            matrix.entry(currStage, currStage).negate();
            for (j=0; j<matrix.rows(); j++) {
                // we're thinking of this as a row op
                colSpaceBasis.entry( currStage, j ).negate();
                colSpaceBasisInv.entry( j,currStage ).negate();
            }
        }
        currStage++;
    }

}

unsigned rowBasis(MatrixInt& matrix) {
    unsigned n = matrix.columns();

    // Make a copy of the input matrix, and reduce it to row echelon form.
    MatrixInt echelon(matrix);

    unsigned doneRows = 0;
    unsigned rank = echelon.rows();

    unsigned* lead = new unsigned[n];
    unsigned r, c, tmp;
    for (c = 0; c < n; ++c)
        lead[c] = c;

    Integer coeff1, coeff2;
    while (doneRows < rank) {
        // INV: For i < doneRows, echelon[i, lead[i]] is non-zero, and
        // every other entry echelon[j, lead[i]] is zero for j > i.

        // Find the first non-zero entry in row doneRows.
        for (c = doneRows; c < n; ++c)
            if (echelon.entry(doneRows, lead[c]) != MatrixInt::zero)
                break;

        if (c == n) {
            // We have a zero row.  Push it to the bottom.
            --rank;
            if (doneRows < rank) {
                echelon.swapRows(doneRows, rank);
                matrix.swapRows(doneRows, rank);
            }
        } else {
            // We have a non-zero row.
            // Save the column in which we found our non-zero entry.
            tmp = lead[doneRows]; lead[doneRows] = lead[c]; lead[c] = tmp;

            // Make all lower entries in column lead[doneRows] equal to zero.
            // Do this with only integer arithmetic.  This could lead to
            // some very large matrix entries, though we're using Integer
            // so the worst that can happen is that things get slow.
            coeff1 = echelon.entry(doneRows, lead[doneRows]);

            for (r = doneRows + 1; r < rank; ++r) {
                coeff2 = echelon.entry(r, lead[doneRows]);
                if (coeff2 != MatrixInt::zero) {
                    echelon.multRow(r, coeff1);
                    echelon.addRow(doneRows, r, -coeff2);

                    // Factor out the gcd of this row.
                    echelon.reduceRow(r);
                }
            }
            ++doneRows;
        }
    }

    // All done!
    delete[] lead;
    return rank;
}

unsigned rowBasisAndOrthComp(MatrixInt& input, MatrixInt& complement) {
    unsigned n = input.columns();

    // Make a copy of the input matrix, and reduce it to row echelon form.
    MatrixInt echelon(input);

    unsigned doneRows = 0;
    unsigned rank = echelon.rows();

    unsigned* lead = new unsigned[n];
    unsigned r, c, tmp;
    for (c = 0; c < n; ++c)
        lead[c] = c;

    Integer coeff1, coeff2;
    while (doneRows < rank) {
        // INV: For i < doneRows, echelon[i, lead[i]] is non-zero, and
        // every other entry echelon[j, lead[i]] is zero for j > i.

        // Find the first non-zero entry in row doneRows.
        for (c = doneRows; c < n; ++c)
            if (echelon.entry(doneRows, lead[c]) != MatrixInt::zero)
                break;

        if (c == n) {
            // We have a zero row.  Push it to the bottom.
            --rank;
            if (doneRows < rank) {
                echelon.swapRows(doneRows, rank);
                input.swapRows(doneRows, rank);
            }
        } else {
            // We have a non-zero row.
            // Save the column in which we found our non-zero entry.
            tmp = lead[doneRows]; lead[doneRows] = lead[c]; lead[c] = tmp;

            // Make all lower entries in column lead[doneRows] equal to zero.
            // Do this with only integer arithmetic.  This could lead to
            // some very large matrix entries, though we're using Integer
            // so the worst that can happen is that things get slow.
            coeff1 = echelon.entry(doneRows, lead[doneRows]);

            for (r = doneRows + 1; r < rank; ++r) {
                coeff2 = echelon.entry(r, lead[doneRows]);
                if (coeff2 != MatrixInt::zero) {
                    echelon.multRow(r, coeff1);
                    echelon.addRow(doneRows, r, -coeff2);

                    // Factor out the gcd of this row.
                    echelon.reduceRow(r);
                }
            }
            ++doneRows;
        }
    }

    // Now form the basis for the orthogonal complement.
    complement.initialise(MatrixInt::zero);

    Integer lcmLead = 1;
    for (r = 0; r < n; ++r) {
        complement.entry(r, lead[r]) = lcmLead;
        complement.entry(r, lead[r]).negate();

        for (c = 0; c < r && c < rank; ++c) {
            complement.entry(r, lead[c]) = echelon.entry(c, lead[r]) * lcmLead;
            complement.entry(r, lead[c]).divByExact(echelon.entry(c, lead[c]));
        }

        complement.reduceRow(r);

        if (r < rank) {
            coeff1 = echelon.entry(r, lead[r]);
            lcmLead = lcmLead.lcm(coeff1);

            for (tmp = 0; tmp < r; ++tmp) {
                coeff2 = echelon.entry(tmp, lead[r]);
                if (coeff2 != MatrixInt::zero) {
                    echelon.multRow(tmp, coeff1);
                    echelon.addRow(r, tmp, -coeff2);

                    // Factor out the gcd of this row.
                    echelon.reduceRow(tmp);
                }
                // TODO: Is this actually necessary?
                lcmLead = lcmLead.lcm(echelon.entry(tmp, lead[tmp]));
            }
        }
    }

    // All done!
    delete[] lead;
    return rank;
}

void columnEchelonForm(MatrixInt &M, MatrixInt &R, MatrixInt &Ri,
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

    Integer d,r; // given two Integers a and b, we will represent
                 // a/b by d and a % b by r in the algorithm.
    Integer u,v,gcd, a,b; // for column operations u,v,a,b represent
                          // a 2x2 matrix.
    Integer tmp;

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


std::unique_ptr<MatrixInt> preImageOfLattice(const MatrixInt& hom,
        const std::vector<Integer>& L) {
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

    MatrixInt basis(hom.columns(), hom.columns() );
    basis.makeIdentity();
    MatrixInt basisi(hom.columns(), hom.columns() );
    basisi.makeIdentity();
    // and we proceed to modify it solely via column operations.
    // one for every column operation performed on homModL
    MatrixInt homModL(hom);

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

    MatrixInt tHom( homModL.rows(), torCol.size() );
    std::unique_ptr<MatrixInt> tBasis(
        new MatrixInt( basis.rows(), torCol.size() )); // this will be the
                                                        // eventual retval.
    MatrixInt dummy( torCol.size(), 0 ); // needed when we call
    // columnEchelonForm. choosing it to have 0 columns speeds up
    // the algorithm.

    for (i=0;i<tHom.rows();i++)
        for (j=0;j<tHom.columns();j++)
            tHom.entry(i,j) = homModL.entry(i, torCol[j]);

    for (i=0;i<basis.rows();i++)
        for (j=0;j<torCol.size();j++)
            tBasis->entry(i,j) = basis.entry(i, torCol[j]);

    columnEchelonForm( tHom, *tBasis, dummy, torList );

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

    Integer d,r; // given two Integers a and b, we will represent
    // a/b by d and a % b by r in the algorithm.
    Integer u,v,gcd, a,b; // for column operations u,v,a,b represent
    // a 2x2 matrix.
    Integer tmp;

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
            d = L[torList[CR]].divExact(gcd); 
            // multiply column  rowNZlist[0] of tHom by d.
            for (i=0;i<torList.size();i++)
                tHom.entry( torList[i], rowNZlist[0] ) *= d;
            // corresponding operation on tBasis.
            for (i=0;i<tBasis->rows();i++)
                tBasis->entry( i, rowNZlist[0] ) *= d;
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
            for (i=0;i<tBasis->rows();i++) {
                tmp = u * tBasis->entry( i, rowNZlist[0] ) +
                    v * tBasis->entry(i, rowNZlist[1] );
                tBasis->entry(i,rowNZlist[1]) =
                    a * tBasis->entry( i, rowNZlist[1]) -
                    b * tBasis->entry( i, rowNZlist[0]);
                tBasis->entry(i,rowNZlist[0]) = tmp;
            }
            // now rowNZlist[1] entry is zero, remove it from the list.
            rowNZlist.erase( rowNZlist.begin()+1 );
        }
    }
    return tBasis;
}

// Lemma 1: [a b | c d] representing an element of End(Z_n x Z_{mn}) is in 
//  Aut(Z_n x Z_{mn}) if and only if dA-bc is a unit of Z_{mn} for some lift 
//  A \in Z_{mn} of a \in Z_n
//   
//  You can get an explicit formula for the inverse, basically it boils down 
//  to a comparison with the Z_n^2 case, and the observation that 
//  Z_{mn} --> Z_n is surjective on units. 

// The algorithm:
//
//  Step 1: reduce all entries mod p_i where i is the row index.
//  Step 2: Consider the bottom row of A.  Consider a group of columns for which 
//          they all share the same p_i. standard Gaussian elimination works to 
//          put zeros in all but one entry of this row.  Potential problem here 
//          the 1x1 case where the entry is a unit mod p_1
//  Step 3: Now we are in the situation where in this row, any two non-zero 
//          entries have distinct p_i's, where now i is the column index. 
//          Let them be in columns i and n respectively.  
//          Let l_1a_{ni} + l_2a_{nn} = gcd(a_{ni},a_{nn})=g, 
//          consider matrix [ v_n l_1 | -v_i l_2 ] where 
//          v_n = a_{nn}/g and v_i=a_{ni}/g.  This is a valid
//          column operation by Lemma 1 and some congruence munching. 
//          Apply, this reduces this bottom row to the point where it has only 
//          one non-zero entry and it is a unit mod the relevant p_i, 
//          so we can multiply by its inverse
//  Step 4: repeat inductively to square submatrix above and to the left of the 
//          nn entry.  This results in an upper diagonal matrix.  
//          Reapply step 1 gives all 1's down diagonal.
//  Step 5: row ops to convert to identity.  
// *Step 6: keep track of all the corresponding matrices, put together to 
//          assemble inverse. Notice it's all standard Gaussian elimination, 
//          just done in a funny order and with some modular arithmatic
//          stuffed in there. 
std::unique_ptr<MatrixInt> torsionAutInverse(const MatrixInt& input,
        const std::vector<Integer> &invF) {
    // inductive step begins right away. Start at bottom row.
    MatrixInt workMat( input );
    MatrixInt colOps( input.rows(), input.columns() );
    colOps.makeIdentity();

    unsigned long wRow = input.rows();
    while (wRow > 0)
    {
        wRow--;
        // step 1 modular reduction on the current row. And find last non-zero 
        // entry in this row up to wRow column
        Integer R; // divisionAlg needs a remainder so we give it one, 
                   //  although we discard it.
        unsigned long pivCol=0; 
        for (unsigned long i=0; i<=wRow; i++)
        {
            workMat.entry(wRow, i).divisionAlg(invF[wRow], R);
            workMat.entry(wRow, i) = R;
            if (R!=0) pivCol=i;
        } // now pivCol is the last non-zero entry in the 0..wRow square smatrix 

        // Step 2: transpose pivCol and column wRow
        if (wRow != pivCol) for (unsigned long i=0; i<workMat.rows(); i++)
        {
            workMat.entry(i, wRow).swap(workMat.entry(i, pivCol));
            colOps.entry(i, wRow).swap(colOps.entry(i, pivCol));
        }
        pivCol = wRow;

        // Step 3 Gauss eliminate whatever can be done. Start at rightmost 
        //  column (pivCol) and work to the left 
        unsigned long wCol = pivCol;
        while (wCol > 0)
        {
            wCol--;
            Integer g, l1, l2;
            g = workMat.entry( wRow, wCol ).gcdWithCoeffs( 
                               workMat.entry(wRow, pivCol), l1, l2 );
            Integer u1, u2;
            u1 = workMat.entry(wRow, wCol).divExact(g); 
            u2 = workMat.entry(wRow, pivCol).divExact(g); 
            // u1 l1 + u2 l2 = 1
            // [ u2 l1 | -u1 l2 ] is column op matrix for wCol and pivCol
            for (unsigned long i=0; i<workMat.rows(); i++)
            {
             // wCol -> u2 wCol - u1 pivCol, pivCol -> l1 wCol + l2 pivCol
             Integer W(workMat.entry(i, wCol)), 
                           P(workMat.entry(i, pivCol));
             workMat.entry(i, wCol) = u2*W - u1*P; 
             workMat.entry(i, pivCol) = l1*W + l2*P;
             W = colOps.entry(i, wCol); P = colOps.entry(i, pivCol);
             colOps.entry(i, wCol) = u2*W - u1*P; 
             colOps.entry(i, pivCol) = l1*W + l2*P; 
            }
        }
        // now workMat.entry(wRow, pivCol) is a unit mod invF[pivCol], 
        //  so find its inverse
        Integer g, a1, a2;
        g= workMat.entry( wRow, pivCol ).gcdWithCoeffs( invF[pivCol], a1, a2 );
        // a1 represents this multiplicative inverse so multiply column by it. 
        for (unsigned long i=0; i<workMat.rows(); i++)
        {
            colOps.entry( i, pivCol ) *= a1;
            workMat.entry( i, pivCol ) *= a1;
        }

        // step 4 mod reduce the only entry left, 
        //  recurse back to step 1 on the next row up. 
        workMat.entry(wRow, pivCol).divisionAlg(invF[wRow], R); 
        workMat.entry(wRow, pivCol) = R;
        // so we should have 1's down the diagonal now as long 
        //  as I haven't screwed up.
    }

    MatrixInt rowOps( input.rows(), input.columns() );
    rowOps.makeIdentity();

    // step 5 upper triang -> identity.  Use row i to kill i-th entry of row j.
    for (unsigned long i=1; i<workMat.columns(); i++) 
     for (unsigned long j=0; j<i; j++)
    {
        Integer X(workMat.entry(j, i)); 
        // now subtract X times row i from row j in both
        // workMat and retval. I guess we could eventually
        // avoid the ops on workMat since it won't affect
        // the return value but for debugging purposes we'll 
        // keep it for now.
        for (unsigned long k=0; k<workMat.columns(); k++)
        { 
            rowOps.entry(j, k) -= X*rowOps.entry(i, k);
            workMat.entry(j, k) -= X*workMat.entry(i, k);
        }
    }

    MatrixInt* retval = new MatrixInt( input.rows(), input.columns() );
    for (unsigned long i=0; i<colOps.rows(); i++) 
     for (unsigned long j=0; j<rowOps.columns(); j++)
    {
        for (unsigned long k=0; k<colOps.columns(); k++) 
          retval->entry(i,j) += colOps.entry(i,k)*rowOps.entry(k,j);
        retval->entry(i,j) %= invF[i];
        if (retval->entry(i,j) < 0) retval->entry(i,j) += invF[i];
    }

    // done
    return std::unique_ptr<MatrixInt>(retval);
}


bool metricFindPivot(const unsigned long &currStage, const MatrixInt &matrix, 
        unsigned long &pr, unsigned long &pc,
        const std::vector<Integer> &rowNorm,
        const std::vector<Integer> &colNorm, 
        const std::vector<Integer> &rowGCD) {
    bool pivotFound = false;
    // find the smallest positive rowGCD
    Integer SProwGCD(Integer::zero);

    for (unsigned long i=currStage; i<matrix.rows(); i++)
        if (rowGCD[i] != 0) {
            if (SProwGCD == 0) SProwGCD = rowGCD[i].abs();
            else if (SProwGCD > rowGCD[i].abs()) SProwGCD = rowGCD[i].abs();
        }

    for (unsigned long i=currStage; i<matrix.rows(); i++)
     if (rowGCD[i].abs() == SProwGCD)
      for (unsigned long j=currStage; j<matrix.columns(); j++)
       {
        if (matrix.entry(i,j) == 0) continue;
        if (pivotFound == false) { pivotFound = true; pr = i; pc = j; }
        else
         {
         // okay, so now we have a previous potential pivot and this one.
         //  Have to choose which one we 
         // prefer. 1st step, is the magnitude smaller? 
         if ( matrix.entry(i,j).abs() < matrix.entry(pr,pc).abs() ) 
          { pr = i; pc = j; }
         else // if not, maybe they're the same magnitude...
         if ( matrix.entry(i,j).abs() == matrix.entry(pr,pc).abs() )
          { // if magnitude == 1 we use the relative weight comparison. 
           if ( matrix.entry(i,j).abs() == 1 )
            {
            if ( (rowNorm[i] - matrix.entry(i,j).abs())*
                 (colNorm[j] - matrix.entry(i,j).abs()) < 
                 (rowNorm[pr] - matrix.entry(pr,pc).abs())*
                 (colNorm[pc] - matrix.entry(pr,pc).abs()) ) { pr = i; pc = j; }
            }   // if magnitude > 1 we use the rowNorm comparison.
           else // if rows the same? use colNorm...
            {
             if (i == pr) { if ( colNorm[j] < colNorm[pc] ) { pr = i; pc =j; } }
             else
              { if ( rowNorm[i] < rowNorm[pr] ) { pr = i; pc = j; } }
            }
          }
        }
      } 
    return pivotFound;
}

// switch rows i and j in matrix.  Keep track of change-of-basis
void metricSwitchRows(const unsigned long &currStage, const unsigned long &i, 
                      const unsigned long &j, 
        MatrixInt &matrix, MatrixInt *colBasis, MatrixInt *colBasisInv, 
        std::vector<Integer> &rowNorm, std::vector<Integer> &rowGCD)
{
    rowNorm[i].swap(rowNorm[j]); rowGCD[i].swap(rowGCD[j]);
    if (colBasis) colBasis->swapRows(i, j); 
    if (colBasisInv) colBasisInv->swapColumns(i, j);
    for (unsigned long k=currStage; k<matrix.columns(); k++)
        matrix.entry(i, k).swap(matrix.entry(j,k));
}

// switch columns i and j in matrix.  Keep track of change-of-basis matrix
void metricSwitchCols(const unsigned long &currStage, const unsigned long &i, 
        const unsigned long &j, 
        MatrixInt &matrix, MatrixInt *rowBasis, MatrixInt *rowBasisInv, 
        std::vector<Integer> &colNorm)
{
    colNorm[i].swap(colNorm[j]); 
    if (rowBasis) rowBasis->swapColumns(i, j); 
    if (rowBasisInv) rowBasisInv->swapRows(i, j);
    for (unsigned long k=currStage; k<matrix.rows(); k++)
        matrix.entry(k, i).swap(matrix.entry(k, j));
}

// columns operation using 2x2-matrix [a b|c d] on columns i, j resp.
void metricColOp(const unsigned long &currStage, const unsigned long &i,
        const unsigned long &j, MatrixInt &matrix, 
        const Integer a, const Integer b, 
        const Integer c, const Integer d, 
        MatrixInt *rowBasis, MatrixInt *rowBasisInv, 
        std::vector<Integer> &rowNorm, 
        std::vector<Integer> &colNorm)
{
    Integer t1, t2;
    // smart rowMetric recomputation and transformation
    colNorm[i] = Integer::zero; colNorm[j] = Integer::zero;
    for (unsigned long k=currStage; k<matrix.rows(); k++)
    {
        t1 = a*matrix.entry(k, i) + c*matrix.entry(k, j);
        t2 = b*matrix.entry(k, i) + d*matrix.entry(k, j);
        rowNorm[k] += t1.abs() + t2.abs() - matrix.entry(k,i).abs() 
                                          - matrix.entry(k,j).abs();
        matrix.entry(k, i) = t1;    matrix.entry(k, j) = t2; 
        colNorm[i] += t1.abs();    colNorm[j] += t2.abs();
    } // now modify rowBasis and rowBasisInv
    if (rowBasis) for (unsigned long k=0; k<matrix.columns(); k++)
    {
        // apply same column op to rowBasis
        t1 = a*rowBasis->entry(k, i) + c*rowBasis->entry(k, j);
        t2 = b*rowBasis->entry(k, i) + d*rowBasis->entry(k, j);
        rowBasis->entry(k, i) = t1;  rowBasis->entry(k, j) = t2;
    }
    if (rowBasisInv) for (unsigned long k=0; k<matrix.columns(); k++)
    {  // apply inverse row op to rowBasisInv
        t1 = d*rowBasisInv->entry(i, k) - b*rowBasisInv->entry(j, k);
        t2 = -c*rowBasisInv->entry(i, k) + a*rowBasisInv->entry(j, k);
        rowBasisInv->entry(i, k) = t1; rowBasisInv->entry(j, k) = t2;
    }
}

// row operation using 2x2-matrix [a b|c d] on rows i, j resp.
void metricRowOp(const unsigned long &currStage, const unsigned long &i, 
    const unsigned long &j, MatrixInt &matrix, 
    const Integer a, const Integer b, 
    const Integer c, const Integer d, 
    MatrixInt *colBasis, MatrixInt *colBasisInv, 
    std::vector<Integer> &rowNorm, std::vector<Integer> &colNorm,
    std::vector<Integer> &rowGCD)
{
    Integer t1, t2;
    // smart norm recomputation and transformation
    rowNorm[i] = Integer::zero; rowNorm[j] = Integer::zero;
    rowGCD[i] = Integer::zero;  rowGCD[j] = Integer::zero;
    for (unsigned long k=currStage; k<matrix.columns(); k++)
    {
        t1 = a*matrix.entry(i, k) + b*matrix.entry(j, k);
        t2 = c*matrix.entry(i, k) + d*matrix.entry(j, k);
        colNorm[k] += t1.abs() + t2.abs() - matrix.entry(i, k).abs() 
                                          - matrix.entry(j, k).abs();
        matrix.entry(i, k) = t1;    matrix.entry(j, k) = t2; 
        rowNorm[i] += t1.abs();    rowNorm[j] += t2.abs();
        rowGCD[i] = rowGCD[i].gcd(t1);  rowGCD[j] = rowGCD[j].gcd(t2);
    } // now modify colBasis and colBasisInv
    if (colBasis) for (unsigned long k=0; k<matrix.rows(); k++)
    {
        // apply same row op to colBasis
        t1 = a*colBasis->entry(i, k) + b*colBasis->entry(j, k);
        t2 = c*colBasis->entry(i, k) + d*colBasis->entry(j, k);
        colBasis->entry(i, k) = t1;  colBasis->entry(j, k) = t2;
    }
    if (colBasisInv) for (unsigned long k=0; k<matrix.rows(); k++)
    {
        // apply inverse column op to colBasisInv
        t1 =  d*colBasisInv->entry(k, i) - c*colBasisInv->entry(k, j);
        t2 = -b*colBasisInv->entry(k, i) + a*colBasisInv->entry(k, j);
        colBasisInv->entry(k, i) = t1; colBasisInv->entry(k, j) = t2;
    }
}

/**
 *  This routine converts mxn matrix "matrix" into its Smith Normal Form.
 * It assumes rowSpaceBasis and rowSpaceBasisInv are pointers to MatrixInts, 
 * if alloceted, having dimension mxm, and colSpaceBasis and colSpaceBasisInv
 * has dimensions nxn.  These matrices record the row and columns operations
 * used to convert between "matrix" and its Smith Normal Form.  Specifically, 
 * if orig_matrix is "matrix" before metricalSmithNormalForm is called, and
 * after_matrix is "matrix" after metricalSmithNormalForm is called, then
 * we have the relations:
 *
 *    (*colSpaceBasis) * orig_matrix * (*rowSpaceBasis) == after_matrix
 *
 *    (*colSpaceBasisInv) * after_matrix * (*rowSpaceBasisInv) == orig_matrix
 *
 *  If any of rowSpaceBasis, colSpaceBasis or rowSpaceBasisInv or 
 * colSpaceBasisInv are not allocated, this algotithm does not bother to compute
 * them (and is correspondingly faster. 
 *
 *  This routine uses a first-order technique to intelligently choose the
 * pivot when computing the Smith Normal Form, attempting to keep the matrix
 * sparse and its norm small throughout the reduction process.  The technique
 * is loosely based on the papers: 
 *
 *  Havas, Holt, Rees. Recognizing badly Presented Z-modules. Linear Algebra
 * and its Applications. 192:137--163 (1993). 
 *
 *  Markowitz. The elimination form of the inverse and its application to linear
 * programming. Management Sci. 3:255--269 (1957).
 */
void metricalSmithNormalForm(MatrixInt& matrix,
        MatrixInt *rowSpaceBasis, MatrixInt *rowSpaceBasisInv,
        MatrixInt *colSpaceBasis, MatrixInt *colSpaceBasisInv) {
    if (rowSpaceBasis) rowSpaceBasis->makeIdentity();    
    if (rowSpaceBasisInv) rowSpaceBasisInv->makeIdentity();
    if (colSpaceBasis) colSpaceBasis->makeIdentity();    
    if (colSpaceBasisInv) colSpaceBasisInv->makeIdentity();
    // set up metrics. 
    std::vector<Integer> rowNorm(matrix.rows(), Integer::zero);
    std::vector<Integer> colNorm(matrix.columns(), Integer::zero);
    std::vector<Integer> rowGCD(matrix.rows(), Integer::zero);
    for (unsigned long i=0; i<matrix.rows(); i++) 
     for (unsigned long j=0; j<matrix.columns(); j++)
      { rowNorm[i] += matrix.entry(i,j).abs();
        colNorm[j] += matrix.entry(i,j).abs();
        rowGCD[i]  = rowGCD[i].gcd(matrix.entry(i,j)); }

        unsigned long currStage = 0;
        unsigned long i, j;
        while (metricFindPivot(currStage, matrix, i, j, 
                               rowNorm, colNorm, rowGCD))
        { // entry i,j is now the pivot, so we move it to currStage, currStage.
            if (i != currStage) metricSwitchRows(currStage, currStage, i, 
                    matrix, colSpaceBasis, colSpaceBasisInv, rowNorm, rowGCD);
            if (j != currStage) metricSwitchCols(currStage, currStage, j, 
                    matrix, rowSpaceBasis, rowSpaceBasisInv, colNorm);
            Integer g, u, v;
rowMuckerLoop: 
        // we come back here if the column operations later on mess 
        // up row currStage first we do the col ops, eliminating 
        // entries to the right of currStage, currStage
            for (j=currStage+1; j<matrix.columns(); j++) 
             if (matrix.entry(currStage, j) != 0)
            {
                g = matrix.entry(currStage, currStage).gcdWithCoeffs(
                     matrix.entry(currStage, j), u, v);
                metricColOp(currStage, currStage, j, matrix, u, 
                            -matrix.entry(currStage,j).divExact(g), v, 
                            matrix.entry(currStage, currStage).divExact(g), 
                            rowSpaceBasis, rowSpaceBasisInv, rowNorm, colNorm);
            }
            // then the row ops, eliminating entries below currStage, currStage
            for (i=currStage+1; i<matrix.rows(); i++) 
             if (matrix.entry(i, currStage) != 0)
            {
                g = matrix.entry(currStage, currStage).gcdWithCoeffs(
                     matrix.entry(i, currStage), u, v);
                metricRowOp(currStage, currStage, i, matrix, u, v, 
                            -matrix.entry(i,currStage).divExact(g), 
                            matrix.entry(currStage,currStage).divExact(g), 
                    colSpaceBasis, colSpaceBasisInv, rowNorm, colNorm, rowGCD);
            }
            // scan row currStage, if it isn't zero, goto rowMuckerLoop
            for (j=currStage+1; j<matrix.columns(); j++) 
             if (matrix.entry(currStage, j) != 0) goto rowMuckerLoop;
            // ensure matrix.entry(currStage, currStage) is positive
            if (matrix.entry(currStage, currStage)<0)
            { // we'll make it a column operation
                for (i=currStage; i<matrix.rows(); i++) 
                    matrix.entry(i, currStage).negate();
                if (rowSpaceBasis) for (i=0; i<matrix.columns(); i++)
                    rowSpaceBasis->entry( i, currStage ).negate();
                if (rowSpaceBasisInv) for (i=0; i<matrix.columns(); i++)
                    rowSpaceBasisInv->entry( currStage, i ).negate();
            }
     // run through rows currStage+1 to bottom, check if divisible by 
     //  matrix.entry(cs,cs). if not, record row and gcd( matrix.entry(cs,cs), 
     //  rowGCD[this row],  pick the row with the lowest of these gcds...
            unsigned long rowT=currStage;
            Integer bestGCD(matrix.entry(currStage, currStage).abs());       
            for (i=currStage+1; i<matrix.rows(); i++)       
            {
                g = matrix.entry(currStage, currStage).gcd(rowGCD[i]).abs();
                if ( g < bestGCD )
                { rowT = i; bestGCD = g; }
            }
            if ( rowT > currStage )
            {
                metricRowOp(currStage, currStage, rowT, matrix, 
                        Integer::one, Integer::one, 
                        Integer::zero, Integer::one, colSpaceBasis, 
                        colSpaceBasisInv, rowNorm, colNorm, rowGCD);
                goto rowMuckerLoop;
            }
            // done
            currStage++;
        }
        // no pivot found -- matrix down and to the right of currStage is zero.
        // so we're done.
}



} // namespace regina



