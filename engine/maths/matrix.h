
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

/*! \file maths/matrix.h
 *  \brief Deals with matrices of elements of various types.
 */

#ifndef __MATRIX_H
#ifndef __DOXYGEN
#define __MATRIX_H
#endif

#include <iostream>
#include <memory>
#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "output.h"
#include "maths/integer.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a matrix of elements of the given type \a T.
 *
 * \pre Type \a T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type \a T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 *
 * \ifacespython Not present, although the typedef MatrixInt is.
 */
template <class T>
class Matrix : public Output<Matrix<T> >, public boost::noncopyable {
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
        Matrix(unsigned long rows, unsigned long cols) :
                nRows(rows), nCols(cols), data(new T*[rows]){
            for (unsigned long i = 0; i < rows; i++)
                data[i] = new T[cols];
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        Matrix(const Matrix& cloneMe) : nRows(cloneMe.nRows),
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
        ~Matrix() {
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

#ifdef __DOXYGEN
        /**
         * A Python-only routine that fills the matrix with the given
         * set of elements.
         *
         * The argument \a allValues must be a Python list of length
         * rows() * columns().  Its values will be inserted into the
         * matrix row by row (i.e., the first row will be filled, then
         * the second row, and so on).
         *
         * \ifacescpp Not available; this routine is for Python only.
         *
         * @param allValues the individual elements to place into the matrix.
         */
        void initialise(List allValues);
#endif

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
        bool operator == (const Matrix<T>& other) const {
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
        bool operator != (const Matrix<T>& other) const {
            return ! ((*this) == other);
        }

        /**
         * Writes a complete representation of the matrix to the given
         * output stream.
         * Each row will be written on a separate line with elements in
         * each row separated by single spaces.
         *
         * \ifacespython Not present, even if a subclass of Matrix
         * is mirrored and its inherited routines are mirrored also.
         *
         * @param out the output stream to which to write.
         */
        void writeMatrix(std::ostream& out) const {
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

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            out << nRows << " x " << nCols << " matrix";
        }
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const {
            writeMatrix(out);
        }
};

/**
 * Represents a matrix of elements from a given ring \a T.
 *
 * Note that many important functions (such as entry()) are inherited
 * from the parent class Matrix, and are not documented again here.
 *
 * \pre Type \a T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type \a T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 * \pre Type \a T provides binary operators <tt>+</tt>, <tt>-</tt> and
 * <tt>*</tt> and unary operators <tt>+=</tt>, <tt>-=</tt> and <tt>*=</tt>.
 * \pre Type \a T has a long integer constructor.  That is, if \c a is of
 * type \a T, then \c a can be initialised to a long integer \c l using
 * <tt>a(l)</tt>.
 * Here the value 1 refers to the multiplicative identity in the ring \a T.
 *
 * \ifacespython Not present, although the typedef MatrixInt is.
 */
template <class T>
class MatrixRing : public Matrix<T> {
    public:
        static const T zero;
            /**< Zero in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */
        static const T one;
            /**< One (the multiplicative identity) in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default constructors.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        MatrixRing(unsigned long rows, unsigned long cols) :
                Matrix<T>(rows, cols) {
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        MatrixRing(const Matrix<T>& cloneMe) :
                Matrix<T>(cloneMe) {
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will
         * have <tt>entry(r,c)</tt> equal to <tt>one</tt> if
         * <tt>r == c</tt> and <tt>zero</tt> otherwise.
         */
        void makeIdentity() {
            this->initialise(zero);
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
         * Determines whether this is the zero matrix.
         *
         * @return \c true if and only if all entries in the matrix are zero.
         */
        bool isZero() const {
            for (size_t r=0; r<this->nRows; ++r)
                for (size_t c=0; c<this->nCols; ++c)
                    if (this->data[r][c] != zero)
                        return false;
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
         * MatrixRing<T>, even if both this and \a other are of some common
         * subclass of MatrixRing<T>.  If you need a subclass to be returned,
         * consider calling multiplyAs() instead.
         *
         * \ifacespython The multiplication operator for a subclass (such as
         * MatrixIntDomain) will return a new matrix of that same subclass.
         * That is, the python multiplication operator really calls
         * multiplyAs(), not this routine.
         *
         * @param other the matrix by which to multiply this matrix.
         * @return a newly allocated matrix representing
         * <tt>this * other</tt>.
         */
        std::unique_ptr<MatrixRing<T>> operator * (const MatrixRing<T>& other)
                const {
            std::unique_ptr<MatrixRing<T> > ans(new MatrixRing<T>(
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
         * \pre The class \a MatrixClass is a subclass of MatrixRing<T>,
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
        std::unique_ptr<MatrixClass> multiplyAs(const MatrixRing<T>& other)
                const {
            std::unique_ptr<MatrixClass> ans(new MatrixClass(
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

/**
 * Represents a matrix of elements from a given integral domain \a T.
 *
 * Note that many important functions (such as entry()) are inherited
 * from the superclasses Matrix and MatrixRing, and are not documented
 * again here.  Many other algorithms that work on MatrixIntDomain<Integer>
 * are available in the maths/matrixops.h file. 
 *
 * \pre Type \a T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type \a T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 * \pre Type \a T provides binary operators <tt>+</tt>, <tt>-</tt> and
 * <tt>*</tt> and unary operators <tt>+=</tt>, <tt>-=</tt> and <tt>*=</tt>.
 * \pre Type \a T has a long integer constructor.  That is, if \c a is of
 * type \a T, then \c a can be initialised to a long integer \c l using
 * <tt>a(l)</tt>.
 * \pre Type \a T can be tested for equality or inequality against an integer.
 * \pre Type \a T has member functions divByExact() and gcdWith() that behave
 * in a similar way to the Integer member functions of these names.
 *
 * All of Regina's integer types (Integer, LargeInteger and the various
 * NativeInteger classes) satisfy these requirements.
 *
 * \ifacespython Not present, although the typedef MatrixInt is.
 */
template <typename T>
class MatrixIntDomain : public MatrixRing<T> {
    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default constructors.
         *
         * Note that, for MatrixIntDomain<Integer>, this means that all
         * entries will be initialised to zero.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        MatrixIntDomain(unsigned long rows, unsigned long cols);
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        MatrixIntDomain(const MatrixIntDomain<T>& cloneMe);

        /**
         * Divides all elements of the given row by the given integer.
         * This can only be used when the given integer divides into all
         * row elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given row are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given row (i.e., it leaves no remainder).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row whose elements should be
         * divided by \a divBy.
         * @param divBy the integer to divide each row element by.
         */
        void divRowExact(unsigned long row, const T& divBy);

        /**
         * Divides all elements of the given column by the given integer.
         * This can only be used when the given integer divides into all
         * column elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given column are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given column (i.e., it leaves no remainder).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column whose elements should be
         * divided by \a divBy.
         * @param divBy the integer to divide each column element by.
         */
        void divColExact(unsigned long col, const T& divBy);

        /**
         * Computes the greatest common divisor of all elements of the
         * given row.  The value returned is guaranteed to be non-negative.
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row whose gcd should be computed.
         * @return the greatest common divisor of all elements of this row.
         */
        T gcdRow(unsigned long row);

        /**
         * Computes the greatest common divisor of all elements of the
         * given column.  The value returned is guaranteed to be non-negative.
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column whose gcd should be computed.
         * @return the greatest common divisor of all elements of this column.
         */
        T gcdCol(unsigned long col);

        /**
         * Reduces the given row by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the row is
         * changed at all, it will be divided by a \e positive integer.
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row to reduce.
         */
        void reduceRow(unsigned long row);

        /**
         * Reduces the given column by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the column is
         * changed at all, it will be divided by a \e positive integer.
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column to reduce.
         */
        void reduceCol(unsigned long col);
};

/**
 * A matrix of arbitrary-precision integers.
 *
 * This is the most common class used by Regina when running algebraic
 * algorithms over integer matrices.  Since the underlying type is
 * Regina's Integer class, calculations will be exact regardless of
 * how large the integers become.
 *
 * \ifacespython This represents the class MatrixIntDomain<Integer>,
 * which inherits from Matrix<Integer> and MatrixRing<Integer>.
 * Most but not all inherited functions are implemented; see the individual
 * members' documentation for exceptions where they arise.
 */
typedef MatrixIntDomain<Integer> MatrixInt;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrix has now been renamed to Matrix.
 */
template <class T>
using NMatrix REGINA_DEPRECATED = Matrix<T>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrixRing has now been renamed to MatrixRing.
 */
template <class T>
using NMatrixRing REGINA_DEPRECATED = MatrixRing<T>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrixInt has now been renamed to MatrixInt.
 */
REGINA_DEPRECATED typedef MatrixInt NMatrixInt;

/*@}*/

// Constants for MatrixRing

template <class T>
const T MatrixRing<T>::zero(0L);
    /**< Zero in the underlying ring. */
template <class T>
const T MatrixRing<T>::one(1L);
    /**< One (the multiplicative identity) in the underlying ring. */

// Implementation details for MatrixIntDomain

template <typename T>
inline MatrixIntDomain<T>::MatrixIntDomain(unsigned long rows, unsigned long cols) :
        MatrixRing<T>(rows, cols) {
}
template <typename T>
inline MatrixIntDomain<T>::MatrixIntDomain(const MatrixIntDomain<T>& cloneMe) :
        MatrixRing<T>(cloneMe) {
}

template <typename T>
void MatrixIntDomain<T>::divRowExact(unsigned long row, const T& divBy) {
    for (T* x = this->data[row]; x != this->data[row] + Matrix<T>::nCols; ++x)
        x->divByExact(divBy);
}

template <typename T>
void MatrixIntDomain<T>::divColExact(unsigned long col, const T& divBy) {
    for (T** row = this->data; row != this->data + Matrix<T>::nRows; ++row)
        (*row)[col].divByExact(divBy);
}

template <typename T>
T MatrixIntDomain<T>::gcdRow(unsigned long row) {
    T* x = this->data[row];

    T gcd = *x++;
    while (x != this->data[row] + Matrix<T>::nCols && gcd != 1L && gcd != -1L)
        gcd = gcd.gcd(*x++);

    if (gcd < 0L)
        gcd.negate();
    return gcd;
}

template <typename T>
T MatrixIntDomain<T>::gcdCol(unsigned long col) {
    T** row = this->data;

    T gcd = (*row++)[col];
    while (row != this->data + Matrix<T>::nRows && gcd != 1L && gcd != -1L)
        gcd = gcd.gcd((*row++)[col]);

    if (gcd < 0L)
        gcd.negate();
    return gcd;
}

template <typename T>
void MatrixIntDomain<T>::reduceRow(unsigned long row) {
    T gcd = gcdRow(row);
    if (gcd != 0L && gcd != 1L)
        divRowExact(row, gcd);
}

template <typename T>
inline void MatrixIntDomain<T>::reduceCol(unsigned long col) {
    T gcd = gcdCol(col);
    if (gcd != 0L && gcd != 1L)
        divColExact(col, gcd);
}

} // namespace regina

#endif
