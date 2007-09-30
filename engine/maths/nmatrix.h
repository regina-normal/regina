
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

/*! \file nmatrix.h
 *  \brief Deals with matrices of elements of various types.
 */

#ifndef __NMATRIX_H
#ifndef __DOXYGEN
#define __NMATRIX_H
#endif

#include <memory>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a matrix of elements of the given type T.
 *
 * \pre Type T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 *
 * \ifacespython Not present, although the subclass NMatrixInt is.
 */
template <class T>
class NMatrix {
    protected:
        unsigned long nRows;
            /**< The number of rows in the matrix. */
        unsigned long nCols;
            /**< The number of columns in the matrix. */
        T** data;
            /**< The actual entries in the matrix.
             *   <tt>data[r][c]</tt> is the element in row \a r,
             *   column \a c. */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default
         * constructors.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        NMatrix(unsigned long rows, unsigned long cols) :
                nRows(rows), nCols(cols), data(new T*[rows]){
            for (unsigned long i = 0; i < rows; i++)
                data[i] = new T[cols];
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        NMatrix(const NMatrix& cloneMe) : nRows(cloneMe.nRows),
                nCols(cloneMe.nCols), data(new T*[cloneMe.nRows]) {
            unsigned long r, c;
            for (r = 0; r < nRows; r++) {
                data[r] = new T[nCols];
                for (c = 0; c < nCols; c++)
                    data[r][c] = cloneMe.data[r][c];
            }
        }
        /**
         * Destroys this matrix.
         */
        virtual ~NMatrix() {
            for (unsigned long i = 0; i < nRows; i++)
                delete[] data[i];
            delete[] data;
        }

        /**
         * Sets every entry in the matrix to the given value.
         *
         * @param value the value to assign to each entry.
         */
        void initialise(const T& value) {
            unsigned long r, c;
            for (r = 0; r < nRows; r++)
                for (c = 0; c < nCols; c++)
                    data[r][c] = value;
        }

        /**
         * Returns the number of rows in this matrix.
         *
         * @return the number of rows.
         */
        unsigned long rows() const {
            return nRows;
        }
        /**
         * Returns the number of columns in this matrix.
         *
         * @return the number of columns.
         */
        unsigned long columns() const {
            return nCols;
        }

        /**
         * Returns the entry at the given row and column.
         * Rows and columns are numbered beginning at zero.
         *
         * \pre \a row is between 0 and rows()-1 inclusive.
         * \pre \a column is between 0 and columns()-1 inclusive.
         *
         * \ifacespython Although the entry() routine gives direct
         * read-write access to matrix elements, the syntax
         * <tt>matrix.entry(row, column) = value</tt> still cannot be
         * used in python to set a matrix element directly.  For this,
         * you can use the syntax <tt>matrix.set(row, column, value)</tt>.
         * This set() routine returns nothing, and is provided for python
         * only (i.e., it is not part of the C++ calculation engine).
         *
         * @param row the row of the desired entry.
         * @param column the column of the desired entry.
         * @return a reference to the entry in the given row and column.
         */
        T& entry(unsigned long row, unsigned long column) {
            return data[row][column];
        }
        /**
         * Returns the entry at the given row and column.
         * Rows and columns are numbered beginning at zero.
         *
         * \pre \a row is between 0 and rows()-1 inclusive.
         * \pre \a column is between 0 and columns()-1 inclusive.
         *
         * \ifacespython Not present, although the non-const form of
         * this routine is.
         *
         * @param row the row of the desired entry.
         * @param column the column of the desired entry.
         * @return a reference to the entry in the given row and column.
         */
        const T& entry(unsigned long row, unsigned long column) const {
            return data[row][column];
        }

        /**
         * Determines whether this and the given matrix are identical.
         *
         * Two matrices are identical if and only if (i) their dimensions
         * are the same, and (ii) the corresponding elements of each
         * matrix are equal.
         *
         * Note that this routine can happily deal with two matrices of
         * different dimensions (in which case it will always return
         * \c false).
         *
         * This routine returns \c true if and only if the inequality operator
         * (!=) returns \c false.
         *
         * \pre The type \a T provides an equality operator (==).
         *
         * @param other the matrix to compare with this.
         * @return \c true if the matrices are equal as described above,
         * or \c false otherwise.
         */
        bool operator == (const NMatrix<T>& other) const {
            if (nRows != other.nRows || nCols != other.nCols)
                return false;

            unsigned long r, c;
            for (r = 0; r < nRows; ++r)
                for (c = 0; c < nCols; ++c)
                    if (! (data[r][c] == other.data[r][c]))
                        return false;

            return true;
        }

        /**
         * Determines whether this and the given matrix are different.
         *
         * Two matrices are different if either (i) their dimensions
         * differ, or (ii) the corresponding elements of each matrix differ
         * in at least one location.
         *
         * Note that this routine can happily deal with two matrices of
         * different dimensions (in which case it will always return
         * \c true).
         *
         * This routine returns \c true if and only if the equality operator
         * (==) returns \c false.
         *
         * \pre The type \a T provides an equality operator (==).
         *
         * @param other the matrix to compare with this.
         * @return \c true if the matrices are different as described above,
         * or \c false otherwise.
         */
        bool operator != (const NMatrix<T>& other) const {
            return ! ((*this) == other);
        }

        /**
         * Writes a complete representation of the matrix to the given
         * output stream.
         * Each row will be written on a separate line with elements in
         * each row separated by single spaces.
         *
         * \ifacespython Not present, even if a subclass of NMatrix
         * is mirrored and its inherited routines are mirrored also.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeMatrix(std::ostream& out) const {
            unsigned long r, c;
            for (r = 0; r < nRows; r++) {
                for (c = 0; c < nCols; c++) {
                    if (c > 0) out << ' ';
                    out << data[r][c];
                }
                out << '\n';
            }
        }

        /**
         * Swaps the elements of the two given rows in the matrix.
         *
         * \pre The two given rows are between 0 and rows()-1 inclusive.
         *
         * @param first the first row to swap.
         * @param second the second row to swap.
         */
        void swapRows(unsigned long first, unsigned long second) {
            T tmp;
            for (unsigned long i = 0; i < nCols; i++) {
                tmp = data[first][i];
                data[first][i] = data[second][i];
                data[second][i] = tmp;
            }
        }
        /**
         * Swaps the elements of the two given columns in the matrix.
         *
         * \pre The two given columns are between 0 and columns()-1 inclusive.
         *
         * @param first the first column to swap.
         * @param second the second column to swap.
         */
        void swapColumns(unsigned long first, unsigned long second) {
            T tmp;
            for (unsigned long i = 0; i < nRows; i++) {
                tmp = data[i][first];
                data[i][first] = data[i][second];
                data[i][second] = tmp;
            }
        }
};

/**
 * Represents a matrix of elements from a given ring T.
 *
 * \pre Type T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 * \pre Type T provides binary operators <tt>+</tt>, <tt>-</tt> and
 * <tt>*</tt> and unary operators <tt>+=</tt>, <tt>-=</tt> and <tt>*=</tt>.
 * \pre Type T has a long integer constructor.  That is, if \c a is of type T,
 * then \c a can be initialised to a long integer \c l using <tt>a(l)</tt>.
 * Here the value 1 refers to the multiplicative identity in the ring T.
 *
 * \ifacespython Not present, although the subclass NMatrixInt is.
 */
template <class T>
class NMatrixRing : public NMatrix<T> {
    public:
        static T zero;
            /**< Zero in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */
        static T one;
            /**< One (the multiplicative identity) in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default
         * constructors.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        NMatrixRing(unsigned long rows, unsigned long cols) :
                NMatrix<T>(rows, cols) {
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        NMatrixRing(const NMatrix<T>& cloneMe) :
                NMatrix<T>(cloneMe) {
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will
         * have <tt>entry(r,c)</tt> equal to <tt>one</tt> if
         * <tt>r == c</tt> and <tt>zero</tt> otherwise.
         */
        void makeIdentity() {
            initialise(zero);
            for (unsigned long i = 0; i < this->nRows && i < this->nCols; i++)
                this->data[i][i] = one;
        }

        /**
         * Determines whether this matrix is a square identity matrix.
         *
         * If this matrix is square, isIdentity() will return \c true if
         * and only if the matrix has ones in the main diagonal and zeroes
         * everywhere else.
         *
         * If this matrix is not square, isIdentity() will always return
         * \c false (even if makeIdentity() was called earlier).
         *
         * @return \c true if and only if this is a square identity matrix.
         */
        bool isIdentity() const {
            if (this->nRows != this->nCols)
                return false;

            unsigned long r, c;
            for (r = 0; r < this->nRows; ++r)
                for (c = 0; c < this->nCols; ++c) {
                    if (r == c && this->data[r][c] != one)
                        return false;
                    if (r != c && this->data[r][c] != zero)
                        return false;
                }

            return true;
        }

        /**
         * Adds the given source row to the given destination row.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         */
        void addRow(unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < this->nCols; i++)
                this->data[dest][i] += this->data[source][i];
        }
        /**
         * Adds the given number of copies of the given source row to
         * the given destination row.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        void addRow(unsigned long source, unsigned long dest,
                T copies) {
            for (unsigned long i = 0; i < this->nCols; i++)
                this->data[dest][i] += copies * this->data[source][i];
        }
        /**
         * Adds the given source column to the given destination column.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         */
        void addCol(unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < this->nRows; i++)
                this->data[i][dest] += this->data[i][source];
        }
        /**
         * Adds the given number of copies of the given source column to
         * the given destination column.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        void addCol(unsigned long source, unsigned long dest,
                T copies) {
            for (unsigned long i = 0; i < this->nRows; i++)
                this->data[i][dest] += copies * this->data[i][source];
        }
        /**
         * Multiplies the given row by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The given row is between 0 and rows()-1 inclusive.
         *
         * @param row the row to work with.
         * @param factor the factor by which to multiply the given row.
         */
        void multRow(unsigned long row, T factor) {
            for (unsigned long i = 0; i < this->nCols; i++)
                this->data[row][i] *= factor;
        }
        /**
         * Multiplies the given column by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The given column is between 0 and columns()-1 inclusive.
         *
         * @param column the column to work with.
         * @param factor the factor by which to multiply the given column.
         */
        void multCol(unsigned long column, T factor) {
            for (unsigned long i = 0; i < this->nRows; i++)
                this->data[i][column] *= factor;
        }

        /**
         * Multiplies this by the given matrix, and returns the result.
         * This matrix is not changed.
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * \warning The returned matrix will be of the exact class
         * NMatrixRing<T>, even if both this and \a other are of some common
         * subclass of NMatrixRing<T>.  If you need a subclass to be returned,
         * consider calling multiplyAs() instead.
         *
         * \ifacespython The multiplication operator for a subclass (such as
         * NMatrixInt) will return a new matrix of that same subclass.
         * That is, the python multiplication operator really calls
         * multiplyAs(), not this routine.
         *
         * @param other the matrix by which to multiply this matrix.
         * @return a newly allocated matrix representing
         * <tt>this * other</tt>.
         */
        std::auto_ptr<NMatrixRing<T> > operator * (const NMatrixRing<T>& other)
                const {
            std::auto_ptr<NMatrixRing<T> > ans(new NMatrixRing<T>(
                this->nRows, other.nCols));

            unsigned long row, col, k;
            for (row = 0; row < this->nRows; row++)
                for (col = 0; col < other.nCols; col++) {
                    ans->data[row][col] = zero;
                    for (k = 0; k < this->nCols; k++)
                        ans->data[row][col] +=
                            (this->data[row][k] * other.data[k][col]);
                }

            return ans;
        }

        /**
         * Multiplies this by the given matrix, and returns a new matrix of
         * subclass \a MatrixClass.  This matrix is not changed.
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         * \pre The class \a MatrixClass is a subclass of NMatrixRing<T>,
         * and can be fully initialised by calling the two-argument constructor
         * (passing the row and column counts) and then settng individual
         * elements via \a data[r][c].  In particular, there should not be any
         * new data members that need explicit initialisation.
         *
         * \ifacespython Not present, but the python multiplication operator
         * performs the same task (see the python notes for operator *).
         *
         * @param other the matrix by which to multiply this matrix.
         * @return a newly allocated matrix representing
         * <tt>this * other</tt>.
         */
        template <class MatrixClass>
        std::auto_ptr<MatrixClass> multiplyAs(const NMatrixRing<T>& other)
                const {
            std::auto_ptr<MatrixClass> ans(new MatrixClass(
                this->nRows, other.nCols));

            unsigned long row, col, k;
            for (row = 0; row < this->nRows; row++)
                for (col = 0; col < other.nCols; col++) {
                    ans->data[row][col] = zero;
                    for (k = 0; k < this->nCols; k++)
                        ans->data[row][col] +=
                            (this->data[row][k] * other.data[k][col]);
                }

            return ans;
        }

        /**
         * Evaluates the determinant of the matrix.
         *
         * This algorithm has quartic complexity, and uses the dynamic
         * programming approach of Mahajan and Vinay.  For further
         * details, see Meena Mahajan and V. Vinay, "Determinant:
         * Combinatorics, algorithms, and complexity", Chicago J. Theor.
         * Comput. Sci., Vol. 1997, Article 5.
         *
         * \pre This is a square matrix.
         *
         * @return the determinant of this matrix.
         */
        T det() const {
            unsigned long n = this->nRows;

            // Just in case...
            if (n != this->nCols || n == 0)
                return zero;

            T* partial[2];
            partial[0] = new T[n * n];
            partial[1] = new T[n * n];

            unsigned long len, head, curr, prevHead, prevCurr;

            // Treat the smallest cases of len = 1 separately.
            int layer = 0;
            for (head = 0; head < n; head++) {
                partial[0][head + head * n] = one;
                for (curr = head + 1; curr < n; curr++)
                    partial[0][head + curr * n] = zero;
            }

            // Work up through incrementing values of len.
            for (len = 2; len <= n; len++) {
                layer ^= 1;
                for (head = 0; head < n; head++) {
                    // If curr == head, we need to open a new clow.
                    partial[layer][head + head * n] = zero;
                    for (prevHead = 0; prevHead < head; prevHead++)
                        for (prevCurr = prevHead; prevCurr < n; prevCurr++)
                            partial[layer][head + head * n] -=
                                (partial[layer ^ 1][prevHead + prevCurr * n] *
                                this->data[prevCurr][prevHead]);

                    // If curr > head, we need to continue an existing clow.
                    for (curr = head + 1; curr < n; curr++) {
                        partial[layer][head + curr * n] = zero;
                        for (prevCurr = head; prevCurr < n; prevCurr++)
                            partial[layer][head + curr * n] +=
                                (partial[layer ^ 1][head + prevCurr * n] *
                                this->data[prevCurr][curr]);
                    }
                }
            }

            // All done.  Sum up the determinant.
            T ans = zero;
            for (head = 0; head < n; head++)
                for (curr = head; curr < n; curr++)
                    ans += (partial[layer][head + curr * n] *
                        this->data[curr][head]);

            delete[] partial[0];
            delete[] partial[1];
            return (n % 2 == 0 ? -ans : ans);
        }
};

template <class T>
T NMatrixRing<T>::zero(0L);
    /**< Zero in the underlying ring.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NMatrixRing<T>::one(1L);
    /**< One (the multiplicative identity) in the underlying ring.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */

/*@}*/

} // namespace regina

#endif

