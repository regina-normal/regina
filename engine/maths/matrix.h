
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include <type_traits> // for std::enable_if_t
#include "regina-core.h"
#include "core/output.h"
#include "maths/integer.h"
#include "utilities/intutils.h"

#ifndef __DOXYGEN
// The following macros are used to conditionally enable member functions
// of Matrix only for certain underlying types T.
//
// A side-effect is that every member function that uses these macros is
// now a \e template member function (this is so we can use SFINAE to
// remove unwanted member functions without compile errors).  The user
// should never specify their own template arguments, and indeed the template
// parameter pack Args is there precisely to stop users from doing this.
/**
 * Use this as the return type for a non-static Matrix member function that is
 * only available when working with matrices over rings.
 *
 * Equivalent to \a returnType if the Matrix template argument \a ring is
 * \c true, and removes the member function completey otherwise.
 *
 * This macro cannot be used with templated member functions.
 */
#define REGINA_ENABLE_FOR_RING(returnType) \
    template <typename... Args, typename Return = returnType> \
    std::enable_if_t<ring, Return>
/**
 * Use this as the return type for a static Matrix member function that is
 * only available when working with matrices over rings.
 *
 * Equivalent to <tt>static returnType</tt> if the Matrix template
 * argument \a ring is \c true, and removes the member function completey
 * otherwise.
 *
 * This macro cannot be used with templated member functions.
 */
#define REGINA_ENABLE_FOR_RING_STATIC(returnType) \
    template <typename... Args, typename Return = returnType> \
    static std::enable_if_t<ring, Return>
/**
 * Use this as the return type for a deprecated Matrix member function that is
 * only available when working with matrices over rings.
 *
 * Equivalent to <tt>[[deprecated]] returnType</tt> if the Matrix template
 * argument \a ring is \c true, and removes the member function completey
 * otherwise.
 *
 * This macro cannot be used with templated member functions.
 */
#define REGINA_ENABLE_FOR_RING_DEPRECATED(returnType) \
    template <typename... Args, typename Return = returnType> \
    [[deprecated]] std::enable_if_t<ring, Return>
/**
 * Use this as the return type for a Matrix member function that is only
 * available when working with matrices over Regina's own integer classes.
 *
 * Equivalent to \a returnType if the Matrix template argument \a T is one of
 * Regina's own integer classes (Integer, LargeInteger or NativeInteger),
 * and removes the member function completey otherwise.
 *
 * This macro cannot be used with templated member functions.
 */
#define REGINA_ENABLE_FOR_REGINA_INTEGER(returnType) \
    template <typename... Args, typename Return = returnType> \
    std::enable_if_t<IsReginaInteger<T>::value, Return>
#endif

namespace regina {

class Rational;

/**
 * \weakgroup maths
 * @{
 */

/**
 * Provides additive and multiplicative identity constants for the Matrix class.
 *
 * \deprecated These constants are deprecated, and will be removed in a future
 * release of Regina.  You should simply use the integers 0 and 1 instead.
 *
 * \tparam T the underlying type, which represents an element of a ring.
 * \tparam ring \c true if the constants \c zero and \c one should be defined.
 * If \a ring is \c false, then this class will be empty.
 */
template <typename T, bool ring>
struct MatrixRingIdentities {
    [[deprecated]] static const T zero;
        /**< The additive identity in the underlying ring. */
    [[deprecated]] static const T one;
        /**< The multiplicative identity in the underlying ring. */
};

#ifndef __DOXYGEN
template <typename T>
struct MatrixRingIdentities<T, false> {
};
#endif

/**
 * Represents a matrix of elements of the given type \a T.
 *
 * As of Regina 5.3, the old subclasses of Matrix have now been merged into a
 * single Matrix class.  The additional member functions that the old
 * subclasses MatrixRing and MatrixIntDomain used to provide are now part of
 * Matrix, and are enabled or disabled according to the Matrix template
 * parameters.
 *
 * It is generally safe to just use the type Matrix<T>, since the \c ring
 * argument has a sensible default.  At present, \c ring defaults to \c true
 * (thereby enabling member functions designed for matrices over rings)
 * when \a T is one of the following types:
 *
 * - native C++ integer types (i.e., where std::is_integral<T>::value
 *   is \c true and \a T is not bool); or
 *
 * - Regina's own types Integer, LargeInteger, NativeInteger<...>, and Rational.
 *
 * Other types may be added to this list in future versions of Regina.
 *
 * There are several requirements for the underlying type \a T.
 * For all matrix types:
 *
 * - \a T must have a default constructor and an assignment operator.
 *
 * - An element <i>t</i> of type \a T must be writable to an output stream
 *   using the standard stream operator <tt>&lt;&lt;</tt>.
 *
 * If \a ring is \c true, then in addition to this:
 *
 * - \a T must support binary operators <tt>+</tt>, <tt>-</tt> and <tt>*</tt>,
 *   and unary operators <tt>+=</tt>, <tt>-=</tt> and <tt>*=</tt>.
 *
 * - \a T must be able to be constructed or assigned to from the integers
 *   0 and 1 (representing the additive and multiplicative identities in the
 *   ring respectively).  Likewise, \a T must be able to be tested for
 *   equality or inequality against 0 or 1 also.
 *
 * In particular, all of Regina's integer and rational types (Integer,
 * LargeInteger, NativeInteger<...> and Rational) satisfy all of these
 * requirements, and will set \a ring to \c true by default.
 *
 * The header maths/matrixops.h contains several other algorithms that
 * work with the specific class Matrix<Integer>.
 *
 * This class is designed to avoid deep copies wherever possible.
 * In particular, it supports C++11 move constructors and move assignment.
 * Functions that take or return objects by value are designed to be just as
 * efficient as working with references or pointers, and long chains of
 * operators such as <tt>a = b * c * d</tt> do not make unwanted deep copies.
 *
 * \ifacespython Not present in general, although the specific type
 * Matrix<Integer> is available under the name MatrixInt.
 *
 * \tparam T the type of each individual matrix element.
 * \tparam ring \c true if we should enable member functions that only
 * work when T represents an element of a ring.  This has a sensible default;
 * see above in the class documentation for details.
 */
template <class T, bool ring =
        ((std::is_integral<T>::value && ! std::is_same<T, bool>::value) ||
        IsReginaInteger<T>::value || std::is_same<T, Rational>::value)>
class Matrix : public Output<Matrix<T>>, public MatrixRingIdentities<T, ring> {
    static_assert(ring || ! IsReginaInteger<T>::value,
        "Using Matrix with Regina's own integer types requires ring=true.");

    public:
        typedef T Coefficient;
            /**< The type of each entry in the matrix. */

    private:
        unsigned long rows_;
            /**< The number of rows in the matrix. */
        unsigned long cols_;
            /**< The number of columns in the matrix. */
        T** data_;
            /**< The actual entries in the matrix.
             *   <tt>data_[r][c]</tt> is the element in row \a r,
             *   column \a c. */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default constructors.
         *
         * In particular, this means that for Regina's own integer classes
         * (Integer, LargeInteger and NativeInteger), all entries will be
         * initialised to zero.
         *
         * \warning If \a T is a native C++ integer type (such as \c int
         * or \c long), then the matrix elements will not be initialised
         * to any particular value.
         *
         * \pre The given number of rows and columns are both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        Matrix(unsigned long rows, unsigned long cols) :
                rows_(rows), cols_(cols), data_(new T*[rows]){
            for (unsigned long i = 0; i < rows; i++)
                data_[i] = new T[cols];
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * This constructor induces a deep copy of \a src.
         *
         * @param src the matrix to clone.
         */
        Matrix(const Matrix& src) : rows_(src.rows_),
                cols_(src.cols_), data_(new T*[src.rows_]) {
            unsigned long r, c;
            for (r = 0; r < rows_; r++) {
                data_[r] = new T[cols_];
                for (c = 0; c < cols_; c++)
                    data_[r][c] = src.data_[r][c];
            }
        }
        /**
         * Moves the given matrix into this new matrix.
         * This is a fast (constant time) operation.
         *
         * The matrix that is passed (\a src) will no longer be usable.
         *
         * @param src the matrix to move.
         */
        Matrix(Matrix&& src) noexcept :
                rows_(src.rows_), cols_(src.cols_), data_(src.data_) {
            src.data_ = nullptr;
        }
        /**
         * Destroys this matrix.
         */
        ~Matrix() {
            if (data_) {
                for (unsigned long i = 0; i < rows_; ++i)
                    delete[] data_[i];
                delete[] data_;
            }
        }

        /**
         * Copies the given matrix into this matrix.
         *
         * It does not matter if this and the given matrix have different
         * sizes; if they do then this matrix will be resized as a result.
         *
         * This operator induces a deep copy of \a src.
         *
         * @param src the matrix to copy.
         * @return a reference to this matrix.
         */
        Matrix& operator = (const Matrix& src) {
            if (rows_ != src.rows_ || cols_ != src.cols_) {
                for (unsigned long i = 0; i < rows_; ++i)
                    delete[] data_[i];
                delete[] data_;

                rows_ = src.rows_;
                cols_ = src.cols_;

                data_ = new T*[rows_];
                for (unsigned long i = 0; i < rows_; ++i)
                    data_[i] = new T[cols_];
            }

            for (unsigned long i = 0; i < rows_; ++i)
                std::copy(src.data_[i], src.data_[i] + cols_, data_[i]);
            return *this;
        }
        /**
         * Moves the given matrix into this matrix.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given matrix have different
         * sizes; if they do then this matrix will be resized as a result.
         *
         * The matrix that is passed (\a src) will no longer be usable.
         *
         * @param src the matrix to move.
         * @return a reference to this matrix.
         */
        Matrix& operator = (Matrix&& src) noexcept {
            // Strictly speaking, we could just assign cols_ instead of
            // swapping.
            std::swap(rows_, src.rows_);
            std::swap(cols_, src.cols_);
            std::swap(data_, src.data_);
            // Let src dispose of the original contents in its own destructor.
            return *this;
        }

        /**
         * Sets every entry in the matrix to the given value.
         *
         * @param value the value to assign to each entry.
         */
        void initialise(const T& value) {
            unsigned long r, c;
            for (r = 0; r < rows_; r++)
                for (c = 0; c < cols_; c++)
                    data_[r][c] = value;
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
            return rows_;
        }
        /**
         * Returns the number of columns in this matrix.
         *
         * @return the number of columns.
         */
        unsigned long columns() const {
            return cols_;
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
            return data_[row][column];
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
            return data_[row][column];
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
        bool operator == (const Matrix& other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_)
                return false;

            unsigned long r, c;
            for (r = 0; r < rows_; ++r)
                for (c = 0; c < cols_; ++c)
                    if (! (data_[r][c] == other.data_[r][c]))
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
        bool operator != (const Matrix& other) const {
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
            for (r = 0; r < rows_; r++) {
                for (c = 0; c < cols_; c++) {
                    if (c > 0) out << ' ';
                    out << data_[r][c];
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
            for (unsigned long i = 0; i < cols_; i++) {
                tmp = data_[first][i];
                data_[first][i] = data_[second][i];
                data_[second][i] = tmp;
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
            for (unsigned long i = 0; i < rows_; i++) {
                tmp = data_[i][first];
                data_[i][first] = data_[i][second];
                data_[i][second] = tmp;
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
            out << rows_ << " x " << cols_ << " matrix";
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

        /**
         * Returns an identity matrix of the given size.
         * The matrix returned will have \a size rows and \a size columns.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * @param size the number of rows and columns of the matrix to build.
         * @return an identity matrix of the given size.
         */
        REGINA_ENABLE_FOR_RING_STATIC(Matrix) identity(unsigned long size) {
            Matrix ans(size, size);
            ans.initialise(0);
            for (unsigned long i = 0; i < size; ++i)
                ans.data_[i][i] = 1;
            return ans;
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will have
         * <tt>entry(r,c)</tt> equal to 1 if <tt>r == c</tt> and 0 otherwise.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         */
        REGINA_ENABLE_FOR_RING(void) makeIdentity() {
            this->initialise(0);
            for (unsigned long i = 0; i < this->rows_ && i < this->cols_; i++)
                this->data_[i][i] = 1;
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
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * @return \c true if and only if this is a square identity matrix.
         */
        REGINA_ENABLE_FOR_RING(bool) isIdentity() const {
            if (this->rows_ != this->cols_)
                return false;

            unsigned long r, c;
            for (r = 0; r < this->rows_; ++r)
                for (c = 0; c < this->cols_; ++c) {
                    if (r == c && this->data_[r][c] != 1)
                        return false;
                    if (r != c && this->data_[r][c] != 0)
                        return false;
                }

            return true;
        }

        /**
         * Determines whether this is the zero matrix.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * @return \c true if and only if all entries in the matrix are zero.
         */
        REGINA_ENABLE_FOR_RING(bool) isZero() const {
            for (size_t r=0; r<this->rows_; ++r)
                for (size_t c=0; c<this->cols_; ++c)
                    if (this->data_[r][c] != 0)
                        return false;
            return true;
        }

        /**
         * Adds the given source row to the given destination row.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         */
        REGINA_ENABLE_FOR_RING(void) addRow(
                unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < this->cols_; i++)
                this->data_[dest][i] += this->data_[source][i];
        }
        /**
         * Adds the given number of copies of the given source row to
         * the given destination row.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        REGINA_ENABLE_FOR_RING(void) addRow(
                unsigned long source, unsigned long dest, T copies) {
            for (unsigned long i = 0; i < this->cols_; i++)
                this->data_[dest][i] += copies * this->data_[source][i];
        }
        /**
         * Adds the given source column to the given destination column.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         */
        REGINA_ENABLE_FOR_RING(void) addCol(
                unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < this->rows_; i++)
                this->data_[i][dest] += this->data_[i][source];
        }
        /**
         * Adds the given number of copies of the given source column to
         * the given destination column.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        REGINA_ENABLE_FOR_RING(void) addCol(
                unsigned long source, unsigned long dest, T copies) {
            for (unsigned long i = 0; i < this->rows_; i++)
                this->data_[i][dest] += copies * this->data_[i][source];
        }
        /**
         * Multiplies the given row by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The given row is between 0 and rows()-1 inclusive.
         *
         * @param row the row to work with.
         * @param factor the factor by which to multiply the given row.
         */
        REGINA_ENABLE_FOR_RING(void) multRow(unsigned long row, T factor) {
            for (unsigned long i = 0; i < this->cols_; i++)
                this->data_[row][i] *= factor;
        }
        /**
         * Multiplies the given column by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The given column is between 0 and columns()-1 inclusive.
         *
         * @param column the column to work with.
         * @param factor the factor by which to multiply the given column.
         */
        REGINA_ENABLE_FOR_RING(void) multCol(unsigned long column, T factor) {
            for (unsigned long i = 0; i < this->rows_; i++)
                this->data_[i][column] *= factor;
        }

        /**
         * Multiplies this by the given matrix, and returns the result.
         * This matrix is not changed.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * @param other the other matrix to multiply this matrix by.
         * @return the product matrix <tt>this * other</tt>.
         */
        REGINA_ENABLE_FOR_RING(Matrix) operator * (const Matrix& other) const {
            Matrix ans(this->rows_, other.cols_);

            unsigned long row, col, k;
            for (row = 0; row < rows_; ++row)
                for (col = 0; col < other.cols_; ++col) {
                    ans.data_[row][col] = 0;
                    for (k = 0; k < cols_; ++k)
                        ans.data_[row][col] +=
                            (data_[row][k] * other.data_[k][col]);
                }

            return ans;
        }

        /**
         * Deprecated alias for the multiplication operator.
         * Multiplies this by the given matrix, and returns a new matrix
         * (of this same class) as a result.  This matrix is not changed.
         *
         * As of Regina 5.3, the template parameter \a MatrixClass has been
         * removed, since Regina's old matrix class hierarchy has been replaced
         * with a single Matrix class.  It is harmless if you still pass a
         * template parameter, but your parameter will be ignored.
         *
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * \deprecated Simply use the multiplication operator (which is now
         * identical to this routine).  This routine multiplyAs() dated back
         * to when Matrix had a hierarchy of subclasses that offered different
         * capabilities according to the underlying type \a T.  Now that there
         * is just a single class Matrix, this routine is no longer required.
         *
         * \ifacespython Not present.
         *
         * @param other the other matrix to multiply this matrix by.
         * @return the product matrix <tt>this * other</tt>.
         */
        REGINA_ENABLE_FOR_RING_DEPRECATED(Matrix) multiplyAs(
                const Matrix& other) const {
            return (*this * other);
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
         * This routine is only available when the template argument \a ring
         * is \c true.
         *
         * \pre This is a square matrix.
         *
         * @return the determinant of this matrix.
         */
        REGINA_ENABLE_FOR_RING(T) det() const {
            unsigned long n = this->rows_;

            // Just in case...
            if (n != this->cols_ || n == 0)
                return 0;

            T* partial[2];
            partial[0] = new T[n * n];
            partial[1] = new T[n * n];

            unsigned long len, head, curr, prevHead, prevCurr;

            // Treat the smallest cases of len = 1 separately.
            int layer = 0;
            for (head = 0; head < n; head++) {
                partial[0][head + head * n] = 1;
                for (curr = head + 1; curr < n; curr++)
                    partial[0][head + curr * n] = 0;
            }

            // Work up through incrementing values of len.
            for (len = 2; len <= n; len++) {
                layer ^= 1;
                for (head = 0; head < n; head++) {
                    // If curr == head, we need to open a new clow.
                    partial[layer][head + head * n] = 0;
                    for (prevHead = 0; prevHead < head; prevHead++)
                        for (prevCurr = prevHead; prevCurr < n; prevCurr++)
                            partial[layer][head + head * n] -=
                                (partial[layer ^ 1][prevHead + prevCurr * n] *
                                this->data_[prevCurr][prevHead]);

                    // If curr > head, we need to continue an existing clow.
                    for (curr = head + 1; curr < n; curr++) {
                        partial[layer][head + curr * n] = 0;
                        for (prevCurr = head; prevCurr < n; prevCurr++)
                            partial[layer][head + curr * n] +=
                                (partial[layer ^ 1][head + prevCurr * n] *
                                this->data_[prevCurr][curr]);
                    }
                }
            }

            // All done.  Sum up the determinant.
            T ans = 0;
            for (head = 0; head < n; head++)
                for (curr = head; curr < n; curr++)
                    ans += (partial[layer][head + curr * n] *
                        this->data_[curr][head]);

            delete[] partial[0];
            delete[] partial[1];
            return (n % 2 == 0 ? -ans : ans);
        }

        /**
         * Divides all elements of the given row by the given integer.
         * This can only be used when the given integer divides into all
         * row elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
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
        REGINA_ENABLE_FOR_REGINA_INTEGER(void) divRowExact(
                unsigned long row, const T& divBy) {
            for (T* x = this->data_[row]; x != this->data_[row] + cols_; ++x)
                x->divByExact(divBy);
        }

        /**
         * Divides all elements of the given column by the given integer.
         * This can only be used when the given integer divides into all
         * column elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
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
        REGINA_ENABLE_FOR_REGINA_INTEGER(void) divColExact(
                unsigned long col, const T& divBy) {
            for (T** row = this->data_; row != this->data_ + rows_; ++row)
                (*row)[col].divByExact(divBy);
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given row.  The value returned is guaranteed to be non-negative.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row whose gcd should be computed.
         * @return the greatest common divisor of all elements of this row.
         */
        REGINA_ENABLE_FOR_REGINA_INTEGER(T) gcdRow(unsigned long row) {
            T* x = this->data_[row];

            T gcd = *x++;
            while (x != this->data_[row] + cols_ && gcd != 1 && gcd != -1)
                gcd = gcd.gcd(*x++);

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given column.  The value returned is guaranteed to be non-negative.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column whose gcd should be computed.
         * @return the greatest common divisor of all elements of this column.
         */
        REGINA_ENABLE_FOR_REGINA_INTEGER(T) gcdCol(unsigned long col) {
            T** row = this->data_;

            T gcd = (*row++)[col];
            while (row != this->data_ + rows_ && gcd != 1 && gcd != -1)
                gcd = gcd.gcd((*row++)[col]);

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Reduces the given row by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the row is
         * changed at all, it will be divided by a \e positive integer.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row to reduce.
         */
        REGINA_ENABLE_FOR_REGINA_INTEGER(void) reduceRow(unsigned long row) {
            T gcd = gcdRow(row);
            if (gcd != 0 && gcd != 1)
                divRowExact(row, gcd);
        }

        /**
         * Reduces the given column by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the column is
         * changed at all, it will be divided by a \e positive integer.
         *
         * This routine is only available when \a T is one of Regina's
         * own integer classes (Integer, LargeInteger, or NativeIntgeger).
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column to reduce.
         */
        REGINA_ENABLE_FOR_REGINA_INTEGER(void) reduceCol(unsigned long col) {
            T gcd = gcdCol(col);
            if (gcd != 0 && gcd != 1)
                divColExact(col, gcd);
        }
};

/**
 * A matrix of arbitrary-precision integers.
 *
 * This is the most common class used by Regina when running algebraic
 * algorithms over integer matrices.  Since the underlying type is
 * Regina's Integer class, calculations will be exact regardless of
 * how large the integers become.
 */
typedef Matrix<Integer> MatrixInt;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class MatrixRing has now been absorbed into the main
 * Matrix class.  The old MatrixRing functionality is made available to
 * Matrix when the template parameter \a ring is \c true.
 */
template <class T>
using MatrixRing [[deprecated]] = Matrix<T, true>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class MatrixIntDomain has now been absorbed into the main
 * Matrix class.  The old MatrixIntDomain functionality is made available to
 * Matrix only when \a T is one of Regina's own integer classes (Integer,
 * LargeInteger, or NativeInteger).
 *
 * \pre T is one of Regina's own integer classes (Integer, LargeInteger,
 * or NativeInteger).
 */
template <class T>
using MatrixIntDomain [[deprecated]] = Matrix<T, true>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrix has now been renamed to Matrix.
 */
template <class T>
using NMatrix [[deprecated]] = Matrix<T, false>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrixRing was long ago renamed to MatrixRing,
 * and has now been absorbed into the main Matrix class.  You should set the
 * extra template parameter \a ring for the Matrix class to \c true.
 */
template <class T>
using NMatrixRing [[deprecated]] = Matrix<T, true>;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NMatrixInt has now been renamed to MatrixInt.
 */
[[deprecated]] typedef MatrixInt NMatrixInt;

/*@}*/

// Constants for MatrixRingIdentities:

template <typename T, bool ring>
const T MatrixRingIdentities<T, ring>::zero(0);

template <typename T, bool ring>
const T MatrixRingIdentities<T, ring>::one(1);

} // namespace regina

#endif
