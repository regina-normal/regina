
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

} // namespace regina

#endif

