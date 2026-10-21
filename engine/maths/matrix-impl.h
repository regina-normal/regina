
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file maths/matrix-impl.h
 *  \brief Contains implementations of some member functions in the templated
 *  Matrix class.
 *
 *  This file is _not_ included automatically by matrix.h.  However, typical
 *  end users should not need to include it, since Regina's calculation engine
 *  provides full explicit instantiations for `Matrix<Integer>`, which is where
 *  these routines are most commonly used.
 */

#ifndef __REGINA_MATRIX_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_MATRIX_IMPL_H
#endif

#include "maths/matrix.h"
#include "maths/numbertheory.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
size_t Matrix<T>::makeRowEchelon() requires ReginaInteger<T> {
    size_t i;
    T* pos;

    // The current working row and column:
    // The entries to the left of currCol will not change, and
    // above currRow all that can happen is some reduction.
    size_t currRow = 0;
    size_t currCol = 0;

    // The algorithm works from left to right.
    while (currRow < rows_ && currCol < cols_) {
        // Identify the first non-zero entry in currCol.
        for (i = currRow, pos = data_ + currRow * cols_ + currCol;
                i < rows_; ++i, pos += cols_)
            if (*pos != 0)
                break;

        if (i == rows_) {
            // We found no non-zero entry.  Nothing to do.
            ++currCol;
            continue;
        }

        if (i > currRow) {
            // Swap rows so this first non-zero entry is currRow.
            swapRows(currRow, i);
        }

        // Now our first non-zero entry is in currRow.
        T* currPos = data_ + currRow * cols_ + currCol;

        // Zero out all entries in currCol that appear *below* currRow.
        for (i = currRow + 1, pos = currPos + cols_; i < rows_;
                ++i, pos += cols_) {
            // Now pos points to entry (i, currCol).
            if (*pos != 0) {
                auto [gcd, u, v] = currPos->gcdWithCoeffs(*pos);
                T a = currPos->divExact(gcd);
                T b = -(pos->divExact(gcd));
                combRows(currRow, i, u, v, b, a);
            }
        }

        // Ensure that our leading coefficient (currRow, currCol) is
        // positive, and reduce the entries in currCol *above* currRow.
        if (*currPos < 0)
            negateRow(currRow);
        for (i = 0, pos = data_ + currCol; pos != currPos; ++i, pos += cols_) {
            auto [d, r] = pos->divisionAlg(*currPos);
            if (d != 0)
                addRow(currRow /* source */, i /* dest */, -d);
        }

        ++currRow;
        ++currCol;
    }

    return currRow;
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
size_t Matrix<T>::makeColumnEchelon() requires ReginaInteger<T> {
    size_t i;
    T* pos;

    // The current working row and column:
    // The entries above currRow will not change, and to the left of
    // currCol all that can happen is some reduction.
    size_t currRow = 0;
    size_t currCol = 0;

    // The algorithm works from top to bottom.
    while (currRow < rows_ && currCol < cols_) {
        // Identify the first non-zero entry in currRow.
        for (i = currCol, pos = data_ + currRow * cols_ + currCol;
                i < cols_; ++i, ++pos)
            if (*pos != 0)
                break;

        if (i == cols_) {
            // We found no non-zero entry.  Nothing to do.
            ++currRow;
            continue;
        }

        if (i > currCol) {
            // Swap columns so this first non-zero entry is currCol.
            swapCols(currCol, i);
        }

        // Now our first non-zero entry is in currCol.
        T* currPos = data_ + currRow * cols_ + currCol;

        // Zero out all entries in currRow that appear right of currCol.
        for (i = currCol + 1, pos = currPos + 1; i < cols_; ++i, ++pos) {
            // Now pos points to entry (currRow, i).
            if (*pos != 0) {
                auto [gcd, u, v] = currPos->gcdWithCoeffs(*pos);
                T a = currPos->divExact(gcd);
                T b = -(pos->divExact(gcd));
                combCols(currCol, i, u, v, b, a);
            }
        }

        // Ensure that our leading coefficient (currRow, currCol) is
        // positive, and reduce the entries in currRow left of currCol.
        if (*currPos < 0)
            negateCol(currCol);
        for (i = 0, pos = data_ + currRow * cols_; pos != currPos; ++i, ++pos) {
            auto [d, r] = pos->divisionAlg(*currPos);
            if (d != 0)
                addCol(currCol /* source */, i /* dest */, -d);
        }

        ++currRow;
        ++currCol;
    }

    return currCol;
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
void Matrix<T>::makeRowBasis() requires ReginaInteger<T> {
    // Make a copy of this matrix to reduce to row echelon form.
    Matrix echelon(*this);

    // Here our row echelon form is not with respect to columns 0,1,...;
    // rather it is with respect to columns lead[0],lead[1],... .
    FixedArray<size_t> lead(cols_);
    size_t c;
    for (c = 0; c < cols_; ++c)
        lead[c] = c;

    // We will truncate redundant rows from the bottom of this matrix
    // as we go.  We do this by reducing rows_ (which leaves spare
    // capacity in the data_ array, but this is harmless).
    // We will treat echelon as though it were likewise truncated,
    // though we do not bother to adjust echelon.rows_ as well.

    size_t currRow = 0;
    while (currRow < rows_) {
        // INV: For all i < currRow:
        // - echelon[i, lead[i]] != 0;
        // - all entries in the column below echelon[i, lead[i]] are 0.

        // Determine a good value for lead[currRow].
        // The candidates are lead[currRow,...,(cols_-1)].
        for (c = currRow; c < cols_; ++c)
            if (echelon.entry(currRow, lead[c]) != 0)
                break;

        if (c == cols_) {
            // We have a zero row.  We need to truncate.
            --rows_;
            if (currRow == rows_) {
                // That zero row was the last remaining row.
                return;
            } else {
                // The current row is redundant.  Move the lowest
                // surviving row (which is now beyond the end of our
                // recently-truncated matrix) into its place.
                std::move(
                    data_ + rows_ * cols_,
                    data_ + (rows_ + 1) * cols_,
                    data_ + currRow * cols_);
                std::move(
                    echelon.data_ + rows_ * cols_,
                    echelon.data_ + (rows_ + 1) * cols_,
                    echelon.data_ + currRow * cols_);
            }
        } else {
            // This row is independent from the others.
            // Save the column in which we found our non-zero entry.
            std::swap(lead[currRow], lead[c]);

            // Zero out all lower entries in column lead[currRow], using
            // only integer arithmetic.
            const T& coeff1 = echelon.entry(currRow, lead[currRow]);
            for (size_t r = currRow + 1; r < rows_; ++r) {
                // Make coeff2 a copy, because row r is about to change.
                T coeff2 = echelon.entry(r, lead[currRow]);
                if (coeff2 != 0) {
                    echelon.multRow(r, coeff1);
                    echelon.addRow(currRow, r, -coeff2);
                    echelon.reduceRow(r);
                }
            }
            ++currRow;
        }
    }
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::rowBasisAndOrthComp() const
        requires ReginaInteger<T> {
    std::pair<Matrix<T>, Matrix<T>> ans(*this, cols_);

    // We begin by following the logic for makeRowBasis() (see above for
    // full explanations).  We then hang on to the row echelon form and the
    // array of leading non-zero entries, and use these to construct the
    // incremental basis for the orthogonal complement.
    Matrix echelon(*this);

    FixedArray<size_t> lead(cols_);
    size_t c;
    for (c = 0; c < cols_; ++c)
        lead[c] = c;

    size_t currRow = 0;
    while (currRow < ans.first.rows_) {
        for (c = currRow; c < cols_; ++c)
            if (echelon.entry(currRow, lead[c]) != 0)
                break;

        if (c == cols_) {
            --ans.first.rows_;
            if (currRow == ans.first.rows_) {
                break;
            } else {
                std::move(
                    ans.first.data_ + ans.first.rows_ * cols_,
                    ans.first.data_ + (ans.first.rows_ + 1) * cols_,
                    ans.first.data_ + currRow * cols_);
                std::move(
                    echelon.data_ + ans.first.rows_ * cols_,
                    echelon.data_ + (ans.first.rows_ + 1) * cols_,
                    echelon.data_ + currRow * cols_);
            }
        } else {
            std::swap(lead[currRow], lead[c]);

            const T& coeff1 = echelon.entry(currRow, lead[currRow]);
            for (size_t r = currRow + 1; r < ans.first.rows_; ++r) {
                T coeff2 = echelon.entry(r, lead[currRow]);
                if (coeff2 != 0) {
                    echelon.multRow(r, coeff1);
                    echelon.addRow(currRow, r, -coeff2);
                    echelon.reduceRow(r);
                }
            }
            ++currRow;
        }
    }

    // Now we have the row basis in ans.first.
    // Fill ans.second with the incremental basis for the orthogonal complement.
    // Currently ans.second has size cols_ x cols_, and is initialised to zero.

    T lcmLead = 1;
    for (size_t r = 0; r < cols_; ++r) {
        ans.second.entry(r, lead[r]) = -lcmLead;
        for (c = 0; c < r && c < ans.first.rows_; ++c) {
            ans.second.entry(r, lead[c]) =
                (echelon.entry(c, lead[r]) * lcmLead).
                divExact(echelon.entry(c, lead[c]));
        }
        ans.second.reduceRow(r);

        if (r < ans.first.rows_) {
            const T& coeff1 = echelon.entry(r, lead[r]);
            lcmLead.lcmWith(coeff1);

            for (size_t tmp = 0; tmp < r; ++tmp) {
                T coeff2 = echelon.entry(tmp, lead[r]);
                if (coeff2 != 0) {
                    echelon.multRow(tmp, coeff1);
                    echelon.addRow(r, tmp, -coeff2);
                    echelon.reduceRow(tmp);
                }
                // TODO: Is this actually necessary?
                lcmLead.lcmWith(echelon.entry(tmp, lead[tmp]));
            }
        }
    }

    // All done!
    return ans;
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
Matrix<T> Matrix<T>::inverse() && requires ReginaInteger<T> {
    if (rows_ != cols_)
        throw FailedPrecondition("Inverses can only be "
            "computed for square matrices");
    if (rows_ == 0)
        return {};

    // We reduce this matrix to row echelon form, and keep track of
    // the row operations that were performed.  What follows is a
    // variant of the code in makeRowEchelon().
    Matrix inv = identity(rows_);

    size_t i;
    T* pos;

    // The algorithm works its way down the main diagonal.
    for (size_t curr = 0; curr < rows_; ++curr) {
        // Attempt to fix the diagonal element (curr, curr).
        // The entries to the left of column #curr will not change, and
        // above row #curr all that can happen is some reduction.

        // Identify the first non-zero entry in column #curr, starting
        // the search on the diagonal.
        T* diagPos = data_ + curr * (rows_ + 1);
        for (i = curr, pos = diagPos; i < rows_; ++i, pos += rows_)
            if (*pos != 0)
                break;

        if (i == rows_)
            throw NoSolution(); // column is entirely zero

        // Ensure that this first non-zero entry appears on the diagonal
        // (i.e., at position diagPos).
        if (i > curr) {
            swapRows(curr, i);
            inv.swapRows(curr, i);
        }

        // Zero out all entries in column #curr below the diagonal.
        for (i = curr + 1, pos = diagPos + rows_; i < rows_;
                ++i, pos += rows_) {
            // Now pos points to entry (i, curr).
            if (*pos != 0) {
                auto [gcd, u, v] = diagPos->gcdWithCoeffs(*pos);
                T a = diagPos->divExact(gcd);
                T b = -(pos->divExact(gcd));
                combRows(curr, i, u, v, b, a);
                inv.combRows(curr, i, u, v, b, a);
            }
        }

        // Ensure that our entry on the diagonal is exactly 1.
        if (*diagPos != 1) {
            if (*diagPos == -1) {
                negateRow(curr);
                inv.negateRow(curr);
            } else
                throw NoSolution(); // the inverse is non-integer.
        }

        // Zero out all entries in column #curr above the diagonal.
        for (i = 0, pos = data_ + curr; pos != diagPos; ++i, pos += rows_)
            if (*pos != 0) {
                // Handle inv first, because when we adjust this matrix
                // we will modify *pos.
                inv.addRow(curr, i, -*pos);
                addRow(curr, i, -*pos);
            }
    }

    return inv;
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
void Matrix<T>::makeColumnEchelonForRows(const std::vector<size_t>& rowList,
        Matrix& R, Matrix& Ri) requires ReginaInteger<T> {
    if (R.columns() != cols_ || Ri.rows() != cols_)
        throw InvalidArgument("M.makeColumnEchelonFoRows(rowList, R, Ri) "
            "requires M.columns() == R.columns() == Ri.rows()");

    size_t CR=0;
    size_t CC=0;
    // these are the indices of the current WORKING rows and columns
    // respectively.
    // thus the entries of M above CR will not change, and to the left of
    // CC all that can happen is some reduction.

    std::vector<size_t> rowNZlist; // in the current row, this is the
                                   // list of column coordinates
                                   // for the non-zero entries.

    T u,v,a,b; // for column operations u,v,a,b represent a 2x2 matrix.
    // the algorithm will think of itself as working top to bottom.
    while ( (CR<rowList.size()) && (CC<cols_)) {
        // build rowNZlist
        rowNZlist.clear();

        for (size_t i=CC;i<cols_;i++)
            if (entry(rowList[CR],i) != 0)
                rowNZlist.push_back(i);

        // now the column operations
        if (rowNZlist.size() == 0) {
            // nothing to do.
            CR++;
        } else if (rowNZlist.size() == 1) {
            // let's move this entry to be the leading entry.
            if (rowNZlist[0]==CC) {
                // step 1: ensure entry(CR,CC) is positive.
                if (entry(rowList[CR],CC)<0) {
                    negateCol(CC);
                    R.negateCol(CC);
                    Ri.negateRow(CC);
                }
                // step 2: reduce entries(CR,i) for i<CC
                for (size_t i=0;i<CC;i++) {
                    // write entry(CR,i) as d*entry(CR,CC) + r.
                    auto [d, r] = entry(rowList[CR],i).divisionAlg(
                        entry(rowList[CR],CC) );
                    // reduce column i: subtract d * column CC from column i.
                    addCol(CC, i, -d);
                    R.addCol(CC, i, -d);
                    // corresponding update for Ri: add d * row i to row CC
                    Ri.addRow(i, CC, d);
                }
                // done, move on.
                CC++;
                CR++;
            } else {
                swapCols(CC, rowNZlist[0]);
                R.swapCols(CC, rowNZlist[0]);
                Ri.swapRows(CC, rowNZlist[0]);
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
                T gcd = entry(rowList[CR], rowNZlist[0]).gcdWithCoeffs(
                    entry(rowList[CR], rowNZlist[1]), u,v);
                a = entry(rowList[CR], rowNZlist[0]).divExact(gcd);
                b = entry(rowList[CR], rowNZlist[1]).divExact(gcd);
                // so multiplication on the right by the above matrix
                // corresponds to replacing column r[0] by u r[0] + v r[1]
                // and column r[1] by -b r[0] + a r[1].
                combCols(rowNZlist[0], rowNZlist[1], u, v, -b, a);
                R.combCols(rowNZlist[0], rowNZlist[1], u, v, -b, a);
                Ri.combRows(rowNZlist[0], rowNZlist[1], a, b, -v, u);
                // modify rowNZlist by deleting the entry corresponding to
                // rowNZlist[1]
                rowNZlist.erase( rowNZlist.begin()+1 );
            }
        }
    }
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
void Matrix<T>::makeSmith() requires ReginaInteger<T> {
    size_t currStage = 0;
    size_t nonEmptyRows = rows();
    size_t nonEmptyCols = columns();
    bool flag;
    size_t i, j;
    size_t pivotRow, pivotCol;
    T d, u, v, a, b;
    T tmp;
    while ((currStage < nonEmptyRows) && (currStage < nonEmptyCols)) {
        loopStart:

        // Find a good pivot.
        // For now we just take the non-zero entry with smallest absolute value.
        tmp = 0;
        // TODO: Adjust nonEmptyRows and nonEmptyCols as we iterate here.
        for (i = currStage; i < nonEmptyRows; ++i)
            for (j = currStage; j < nonEmptyCols; ++j) {
                T pivotVal = entry(i, j).abs();
                if (pivotVal > 0)
                    if (tmp == 0 || pivotVal < tmp) {
                        tmp = pivotVal;
                        pivotRow = i;
                        pivotCol = j;
                    }
            }

        if (tmp == 0) {
            // The matrix is zero from here on - which means we are done!
            break;
        }

        if (pivotRow != currStage)
            swapRows(currStage, pivotRow);
        if (pivotCol != currStage)
            swapCols(currStage, pivotCol, currStage);

        // Make zeros for the remainder of the current row.
        for (i=currStage+1; i<nonEmptyCols; i++) {
            if (entry(currStage, i) == 0)
                continue;
            // Put a zero in (currStage, i).
            a = entry(currStage, currStage);
            b = entry(currStage, i);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to columns currStage and i.
            combCols(currStage, i, u, v, -b, a, currStage);
        }

        // Make zeros for the remainder of the current column.
        // Check to see if we change anything and thus muck up the row.
        flag = false;
        for (i=currStage+1; i<nonEmptyRows; i++) {
            if (entry(i, currStage) == 0)
                continue;
            // Put a zero in (i, currStage).
            flag = true;
            a = entry(currStage, currStage);
            b = entry(i, currStage);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to rows currStage and i.
            combRows(currStage, i, u, v, -b, a, currStage);
        }
        if (flag) {
            flag = false;
            for (i=currStage+1; i<nonEmptyCols; i++)
                if (entry(currStage, i) != 0) {
                    flag = true;
                    break;
                }
            if (flag) {
                // The clean row was mucked up.
                continue;
            }
        }

        // Check that entry (currStage, currStage) divides everything else.
        T& diag = entry(currStage, currStage);
        for (i=currStage+1; i<nonEmptyRows; i++)
            for (j=currStage+1; j<nonEmptyCols; j++)
                if ((entry(i, j) % diag) != 0) {
                    // Add row i to the current stage row and start this
                    // stage over.
                    addRowFrom(i, currStage, currStage + 1);
                    goto loopStart;
                }

        // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (diag < 0)
            diag.negate();
        ++currStage;
    }
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
std::array<Matrix<T>, 4> Matrix<T>::makeSmithHafnerMcCurley()
        requires ReginaInteger<T> {
    size_t currStage = 0;
    size_t nonEmptyRows = rows();
    size_t nonEmptyCols = columns();
    bool flag;
    size_t i, j;
    size_t pivotRow, pivotCol;
    T d, u, v, a, b;
    T tmp;

    std::array<Matrix, 4> cob {
        identity(cols_), identity(cols_),
        identity(rows_), identity(rows_) };

    while ((currStage < nonEmptyRows) && (currStage < nonEmptyCols)) {
        loopStart:

        // Find a good pivot.
        // For now we just take the non-zero entry with smallest absolute value.
        tmp = 0;
        // TODO: Adjust nonEmptyRows and nonEmptyCols as we iterate here.
        for (i = currStage; i < nonEmptyRows; ++i)
            for (j = currStage; j < nonEmptyCols; ++j) {
                T pivotVal = entry(i, j).abs();
                if (pivotVal > 0)
                    if (tmp == 0 || pivotVal < tmp) {
                        tmp = pivotVal;
                        pivotRow = i;
                        pivotCol = j;
                    }
            }

        if (tmp == 0) {
            // The matrix is zero from here on - which means we are done!
            break;
        }

        if (pivotRow != currStage) {
            swapRows(currStage, pivotRow);
            cob[2].swapRows(currStage, pivotRow);
            cob[3].swapCols(currStage, pivotRow);
        }
        if (pivotCol != currStage) {
            swapCols(currStage, pivotCol, currStage);
            cob[0].swapCols(currStage, pivotCol);
            cob[1].swapRows(currStage, pivotCol);
        }

        // Make zeros for the remainder of the current row.
        for (i=currStage+1; i<nonEmptyCols; i++) {
            if (entry(currStage, i) == 0)
                continue;
            // Put a zero in (currStage, i).
            a = entry(currStage, currStage);
            b = entry(currStage, i);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to columns currStage and i.
            combCols(currStage, i, u, v, -b, a, currStage);
            cob[0].combCols(currStage, i, u, v, -b, a);
            cob[1].combRows(currStage, i, a, b, -v, u);
        }

        // Make zeros for the remainder of the current column.
        // Check to see if we change anything and thus muck up the row.
        flag = false;
        for (i=currStage+1; i<nonEmptyRows; i++) {
            if (entry(i, currStage) == 0)
                continue;
            // Put a zero in (i, currStage).
            flag = true;
            a = entry(currStage, currStage);
            b = entry(i, currStage);
            d = a.gcdWithCoeffs(b, u, v);
            a.divByExact(d);
            b.divByExact(d);
            // Do a modification to rows currStage and i.
            combRows(currStage, i, u, v, -b, a, currStage);
            cob[2].combRows(currStage, i, u, v, -b, a);
            cob[3].combCols(currStage, i, a, b, -v, u);
        }
        if (flag) {
            flag = false;
            for (i=currStage+1; i<nonEmptyCols; i++)
                if (entry(currStage, i) != 0) {
                    flag = true;
                    break;
                }
            if (flag) {
                // The clean row was mucked up.
                continue;
            }
        }

        // Check that entry (currStage, currStage) divides everything else.
        T& diag = entry(currStage, currStage);
        for (i=currStage+1; i<nonEmptyRows; i++)
            for (j=currStage+1; j<nonEmptyCols; j++)
                if ((entry(i, j) % diag) != 0) {
                    // Add row i to the current stage row and start this
                    // stage over.
                    addRowFrom(i, currStage, currStage + 1);
                    cob[2].addRow(i, currStage);
                    cob[3].addCol(currStage, i, -1);
                    goto loopStart;
                }

        // This stage is complete!
        // Make sure the diagonal entry is positive before leaving it.
        if (diag < 0) {
            diag.negate();
            // we're thinking of this as a row op
            cob[2].negateRow(currStage);
            cob[3].negateCol(currStage);
        }
        ++currStage;
    }

    return cob;
}

namespace detail {

/**
 * Internal to `Matrix<T>::makeSmithMetrical()`.
 */
template <ReginaInteger T>
bool metricFindPivot(size_t currStage, const Matrix<T>& matrix,
        size_t &pr, size_t &pc,
        const std::vector<T> &rowNorm,
        const std::vector<T> &colNorm,
        const std::vector<T> &rowGCD) {
    // find the smallest positive rowGCD
    T minGCD; // zero
    for (size_t i=currStage; i<matrix.rows(); i++)
        if (rowGCD[i] != 0)
            if (minGCD == 0 || minGCD > rowGCD[i].abs())
                minGCD = rowGCD[i].abs();

    bool pivotFound = false;
    for (size_t i=currStage; i<matrix.rows(); i++)
        if (rowGCD[i].abs() == minGCD)
            for (size_t j=currStage; j<matrix.columns(); j++) {
                if (matrix.entry(i,j) == 0)
                    continue;

                if (pivotFound == false) {
                    pivotFound = true;
                    pr = i;
                    pc = j;
                } else {
                    // okay, so now we have a previous potential pivot and this
                    // one.  Have to choose which one we prefer.
                    // 1st step, compare the magnitudes...
                    auto oldAbs = matrix.entry(pr,pc).abs();
                    auto newAbs = matrix.entry(i,j).abs();
                    if (newAbs < oldAbs) {
                        pr = i;
                        pc = j;
                    } else if (newAbs == oldAbs) {
                        if (newAbs == 1 ) {
                            // Use relative weight comparison.
                            if ((rowNorm[i] - newAbs) * (colNorm[j] - newAbs) <
                                    (rowNorm[pr] - oldAbs) *
                                    (colNorm[pc] - oldAbs)) {
                                pr = i;
                                pc = j;
                            }
                        } else {
                            // Use the rowNorm comparison (or if the rows are
                            // the same, the colNorm comparison).
                            if (i == pr) {
                                if (colNorm[j] < colNorm[pc]) {
                                    pr = i;
                                    pc = j;
                                }
                            } else {
                                if (rowNorm[i] < rowNorm[pr]) {
                                    pr = i;
                                    pc = j;
                                }
                            }
                        }
                    }
                }
            }

    return pivotFound;
}

/**
 * Internal to `Matrix<T>::makeSmithMetrical()`.
 *
 * This routine performs a pair of column operations using the 2x2 matrix
 * `[a b|c d]` on columns \a i and \a j respectively.
 */
template <ReginaInteger T>
void metricColOp(size_t currStage, size_t i, size_t j, Matrix<T>& matrix,
        const T& a, const T& b, const T& c, const T& d,
        Matrix<T>& rowBasis, Matrix<T>& rowBasisInv,
        std::vector<T> &rowNorm, std::vector<T> &colNorm) {
    T t1, t2;
    // smart rowMetric recomputation and transformation
    colNorm[i] = 0; colNorm[j] = 0;
    for (size_t k=currStage; k<matrix.rows(); k++)
    {
        t1 = a*matrix.entry(k, i) + c*matrix.entry(k, j);
        t2 = b*matrix.entry(k, i) + d*matrix.entry(k, j);
        rowNorm[k] += t1.abs() + t2.abs() - matrix.entry(k,i).abs()
                                          - matrix.entry(k,j).abs();
        matrix.entry(k, i) = t1;
        matrix.entry(k, j) = t2;
        colNorm[i] += t1.abs();
        colNorm[j] += t2.abs();
    }
    rowBasis.combCols(i, j, a, c, b, d); // apply same column op to rowBasis
    rowBasisInv.combRows(i, j, d, -b, -c, a); // apply inverse op to rowBasisInv
}

/**
 * Internal to `Matrix<T>::makeSmithMetrical()`.
 *
 * This routine performs a pair of row operations using the 2x2 matrix
 * `[a b|c d]` on rows \a i and \a j respectively.
 */
template <ReginaInteger T>
void metricRowOp(size_t currStage, size_t i, size_t j, Matrix<T>& matrix,
        const T& a, const T& b, const T& c, const T& d,
        Matrix<T>& colBasis, Matrix<T>& colBasisInv,
        std::vector<T> &rowNorm, std::vector<T> &colNorm,
        std::vector<T> &rowGCD) {
    T t1, t2;
    // smart norm recomputation and transformation
    rowNorm[i] = rowNorm[j] = 0;
    rowGCD[i] = rowGCD[j] = 0;
    for (size_t k=currStage; k<matrix.columns(); k++) {
        t1 = a*matrix.entry(i, k) + b*matrix.entry(j, k);
        t2 = c*matrix.entry(i, k) + d*matrix.entry(j, k);
        colNorm[k] += t1.abs() + t2.abs() - matrix.entry(i, k).abs()
                                          - matrix.entry(j, k).abs();
        matrix.entry(i, k) = t1;
        matrix.entry(j, k) = t2;
        rowNorm[i] += t1.abs();
        rowNorm[j] += t2.abs();
        rowGCD[i].gcdWith(t1);
        rowGCD[j].gcdWith(t2);
    }
    colBasis.combRows(i, j, a, b, c, d); // apply same op to colBasis
    colBasisInv.combCols(i, j, d, -c, -b, a); // apply inverse op to colBasisInv
}

} // namespace detail

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
std::array<Matrix<T>, 4> Matrix<T>::makeSmithMetrical()
        requires ReginaInteger<T> {
    // This routine uses a first-order technique to intelligently choose the
    // pivot when computing the Smith Normal Form, attempting to keep the matrix
    // sparse and its norm small throughout the reduction process.
    // The technique is loosely based on the papers:
    //
    // - Havas, Holt, Rees. Recognizing badly Presented Z-modules.
    //   Linear Algebra and its Applications. 192:137--163 (1993).
    //
    // - Markowitz. The elimination form of the inverse and its application
    //   to linear programming. Management Sci. 3:255--269 (1957).

    std::array<Matrix, 4> cob {
        identity(cols_), identity(cols_),
        identity(rows_), identity(rows_) };

    // set up metrics (all vectors are initialised to zero by default).
    std::vector<T> rowNorm(rows_);
    std::vector<T> colNorm(cols_);
    std::vector<T> rowGCD(rows_);

    T* pos = data_;
    for (size_t i=0; i<rows(); ++i)
        for (size_t j=0; j<columns(); ++j) {
            rowNorm[i] += pos->abs();
            colNorm[j] += pos->abs();
            rowGCD[i].gcdWith(*pos);
            ++pos;
        }

    size_t currStage = 0;
    size_t i, j;
    while (detail::metricFindPivot(currStage, *this, i, j,
            rowNorm, colNorm, rowGCD)) {
        // entry i,j is now the pivot, so we move it to currStage, currStage.
        if (i != currStage) {
            rowNorm[currStage].swap(rowNorm[i]);
            rowGCD[currStage].swap(rowGCD[i]);
            cob[2].swapRows(currStage, i);
            cob[3].swapCols(currStage, i);
            swapRows(currStage, i, currStage);
        }
        if (j != currStage) {
            colNorm[currStage].swap(colNorm[j]);
            cob[0].swapCols(currStage, j);
            cob[1].swapRows(currStage, j);
            swapCols(currStage, j, currStage);
        }
        T g, u, v;
rowMuckerLoop:
        // we come back here if the column operations later on mess
        // up row currStage first we do the col ops, eliminating
        // entries to the right of currStage, currStage
        for (j=currStage+1; j<columns(); j++)
            if (entry(currStage, j) != 0) {
                g = entry(currStage, currStage).gcdWithCoeffs(
                     entry(currStage, j), u, v);
                detail::metricColOp(currStage, currStage, j, *this, u,
                            -entry(currStage,j).divExact(g), v,
                            entry(currStage, currStage).divExact(g),
                            cob[0], cob[1], rowNorm, colNorm);
            }
        // then the row ops, eliminating entries below currStage, currStage
        for (i=currStage+1; i<rows(); i++)
            if (entry(i, currStage) != 0) {
                g = entry(currStage, currStage).gcdWithCoeffs(
                     entry(i, currStage), u, v);
                detail::metricRowOp(currStage, currStage, i, *this, u, v,
                            -entry(i,currStage).divExact(g),
                            entry(currStage,currStage).divExact(g),
                            cob[2], cob[3], rowNorm, colNorm,
                            rowGCD);
            }
        // scan row currStage, if it isn't zero, goto rowMuckerLoop
        for (j=currStage+1; j<columns(); j++)
            if (entry(currStage, j) != 0) goto rowMuckerLoop;
        // ensure entry(currStage, currStage) is positive
        if (entry(currStage, currStage)<0) {
            // we'll make it a column operation
            for (i=currStage; i<rows(); i++)
                entry(i, currStage).negate();
            cob[0].negateCol(currStage);
            cob[1].negateRow(currStage);
        }
        // run through rows currStage+1 to bottom, check if divisible by
        // entry(cs,cs). if not, record row and gcd(entry(cs,cs),
        // rowGCD[this row], pick the row with the lowest of these gcds...
        size_t rowT=currStage;
        T bestGCD(entry(currStage, currStage).abs());
        for (i=currStage+1; i<rows(); i++) {
            g = entry(currStage, currStage).gcd(rowGCD[i]).abs();
            if ( g < bestGCD ) {
                rowT = i;
                bestGCD = g;
            }
        }
        if ( rowT > currStage ) {
            detail::metricRowOp(currStage, currStage, rowT, *this,
                    RingTraits<T>::one, RingTraits<T>::one,
                    RingTraits<T>::zero, RingTraits<T>::one,
                    cob[2], cob[3], rowNorm, colNorm, rowGCD);
            goto rowMuckerLoop;
        }
        // done
        currStage++;
    }
    // no pivot found -- matrix down and to the right of currStage is zero.
    // so we're done.
    return cob;
}

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
Matrix<T> Matrix<T>::preimageOfLattice(const std::vector<T>& sublattice) const
        requires ReginaInteger<T> {
    if (sublattice.size() != rows_)
        throw InvalidArgument("preimageOfLattice() requires the length of the "
            "sublattice vector to match the number of matrix rows");

    // there are two main steps to this algorithm.
    // 1) find a basis for the domain which splits into a) vectors sent to the
    //    complement of the primitive subspace generated by the range lattice
    //    and b) a basis of vectors sent to the primitive subspace generated
    //    by the range lattice.
    // 2) modify the basis (b) by column ops to get the preimage of the lattice.
    // step (1) is an application of column echelon form
    // step (2) starts with another application of column echelon form, but then
    // it finishes with a variation on it...

    MatrixInt basis = MatrixInt::identity(cols_);
    MatrixInt basisi = MatrixInt::identity(cols_);
    // and we proceed to modify it solely via column operations.
    // one for every column operation performed on homModL
    MatrixInt homModL(*this);

    // set up two lists: the coordinates that correspond to free generators
    // of the range and coordinates corresponding to torsion generators.

    // these lists need to be built from L
    std::vector<size_t> freeList;
    std::vector<size_t> torList;
    for (size_t i=0;i<sublattice.size();i++)
        if (sublattice[i]==0)
            freeList.push_back(i);
        else
            torList.push_back(i);

    // so first put the free image part of it in column echelon form

    homModL.makeColumnEchelonForRows(freeList, basis, basisi);

    std::vector<size_t> torCol;
    bool zeroCol;

    for (size_t i=0; i<homModL.columns(); i++) {
        zeroCol=true;
        for (auto pos : freeList)
            if (homModL.entry( pos, i) != 0)
                zeroCol=false;
        if (zeroCol)
            torCol.push_back(i);
    }

    // set up a new matrix consisting of columns being sent to the primitive
    // subspace generated by the torsion lattice.

    MatrixInt tHom( homModL.rows(), torCol.size() );
    MatrixInt tBasis( basis.rows(), torCol.size() ); // this will be the
                                                     // eventual retval.
    MatrixInt dummy( torCol.size(), 0 ); // needed when we compute
    // column echelon form. choosing it to have 0 columns speeds up
    // the algorithm.

    for (size_t i=0;i<tHom.rows();i++)
        for (size_t j=0;j<tHom.columns();j++)
            tHom.entry(i,j) = homModL.entry(i, torCol[j]);

    for (size_t i=0;i<basis.rows();i++)
        for (size_t j=0;j<torCol.size();j++)
            tBasis.entry(i,j) = basis.entry(i, torCol[j]);

    tHom.makeColumnEchelonForRows(torList, tBasis, dummy);

    // so now we have a primitive collection of vectors being sent to the
    // primitive subspace generated by the torsion lattice in the target.
    // The idea is to run through the rows, for each non-zero row, through
    // a basis change we can ensure there is at most one non-zero entry.
    // multiply this column by the smallest factor so that it is in
    // the torsion lattice, repeat. etc.

    size_t CR=0; // current row under consideration. The actual row index
    // will of course be torList[CR] since all other rows are already zero.

    std::vector<size_t> rowNZlist; // in the current row, this is the list
    // of column coordinates for the non-zero entries.

    T d,r; // given two integers a and b, we will represent
    // a/b by d and a % b by r in the algorithm.
    T u,v,gcd, a,b; // for column operations u,v,a,b represent
    // a 2x2 matrix.
    T tmp;

    while (CR<torList.size()) {
        // build rowNZlist
        rowNZlist.clear();

        for (size_t i=0;i<tHom.columns();i++)
            if (tHom.entry(torList[CR],i) != 0)
                rowNZlist.push_back(i);
        // okay, so now we have a list of non-zero entries.
        // case 1: rowNZlist.size()==0, increment CR, continue;

        if (rowNZlist.empty()) {
            CR++;
            continue;
        }
        // case 2: rowNZlist.size()==1 multiply column only if neccessary,
        // increment CR, continue;

        if (rowNZlist.size()==1) {
            // check to see if tHom.entry(torList[CR], rowNZlist[0]) %
            // L[torList[CR]] == 0 if not, find smallest positive integer
            // s.t. when multiplied by it, above becomes true.
            gcd = tHom.entry(torList[CR], rowNZlist[0]).gcd(
                sublattice[torList[CR]]);
            d = sublattice[torList[CR]].divExact(gcd);
            // multiply column  rowNZlist[0] of tHom by d.
            for (auto pos : torList)
                tHom.entry( pos, rowNZlist[0] ) *= d;
            // corresponding operation on tBasis.
            for (size_t i=0;i<tBasis.rows();i++)
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
            for (auto pos : torList) {
                tmp = u * tHom.entry( pos, rowNZlist[0] ) +
                    v * tHom.entry( pos, rowNZlist[1] );
                tHom.entry( pos,rowNZlist[1]) =
                    a * tHom.entry( pos, rowNZlist[1]) -
                    b * tHom.entry( pos, rowNZlist[0]);
                tHom.entry( pos,rowNZlist[0]) = tmp;
            }
            // modify tBasis
            for (size_t i=0;i<tBasis.rows();i++) {
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

#endif

