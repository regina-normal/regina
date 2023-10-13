
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_MATRIX_H
#ifndef __DOXYGEN
#define __REGINA_MATRIX_H
#endif

#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits> // for std::enable_if_t
#include "regina-core.h"
#include "core/output.h"
#include "maths/integer.h"
#include "utilities/intutils.h"

namespace regina {

class Rational;
template <class> class Vector;

/**
 * Represents a matrix of elements of the given type \a T.
 *
 * As of Regina 5.96, the old subclasses of Matrix have now been merged into a
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
 * - native C++ integer types (i.e., where std::is_integral_v<T>
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
 * - An element \a t of type \a T must be writable to an output stream
 *   using the standard stream operator `<<`.
 *
 * If \a ring is \c true, then in addition to this:
 *
 * - \a T must support binary operators `+`, `-` and `*`,
 *   and unary operators `+=`, `-=` and `*=`.
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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \python Only the specific types Matrix<Integer> and Matrix<bool>
 * are available, under the names MatrixInt and MatrixBool respectively.
 *
 * \tparam T the type of each individual matrix element.
 * \tparam ring \c true if we should enable member functions that only
 * work when T represents an element of a ring.  This has a sensible default;
 * see above in the class documentation for details.
 *
 * \ingroup maths
 */
template <class T, bool ring =
        ((std::is_integral_v<T> && ! std::is_same_v<T, bool>) ||
        IsReginaInteger<T>::value || std::is_same_v<T, Rational>)>
class Matrix : public Output<Matrix<T>> {
    static_assert(ring || ! IsReginaInteger<T>::value,
        "Using Matrix with Regina's own integer types requires ring=true.");

    public:
        /**
         * The type of element that is stored in this matrix.
         */
        using value_type = T;

    private:
        size_t rows_;
            /**< The number of rows in the matrix. */
        size_t cols_;
            /**< The number of columns in the matrix. */
        T** data_;
            /**< The actual entries in the matrix.
             *   `data_[r][c]` is the element in row \a r, column \a c. */

    public:
        /**
         * Creates a new uninitialised matrix.
         *
         * You _must_ initialise this matrix using the assignment operator
         * before you can use it for any purpose.  The only exceptions are:
         *
         * - you can safely destroy an uninitialised matrix;
         *
         * - you can safely assign an uninitialised matrix to another matrix
         *   (either via an assignment operator or copy constructor), in which
         *   case the other matrix will become uninitialised also and subject
         *   to similar constraints;
         *
         * - you can safely call initialised() to test whether a matrix is
         *   initialised or not.
         *
         * \nopython This is because the C++ assignment operators are
         * not accessible to Python.
         */
        Matrix() : rows_(0), cols_(0), data_(nullptr) {
        }
        /**
         * Creates a new square matrix of the given size.  Both the number of
         * rows and the number of columns will be set to \a size.
         *
         * All entries will be initialised using their default constructors.
         * In particular, this means that for Regina's own integer classes
         * (Integer, LargeInteger and NativeInteger), all entries will be
         * initialised to zero.
         *
         * \warning If \a T is a native C++ integer type (such as \c int
         * or \c long), then the matrix elements will not be initialised
         * to any particular value.
         *
         * \pre The given size is strictly positive.
         *
         * \param size the number of rows and columns in the new matrix.
         */
        Matrix(size_t size) :
                rows_(size), cols_(size), data_(new T*[size]) {
            for (size_t i = 0; i < size; ++i)
                data_[i] = new T[size];
        }
        /**
         * Creates a new matrix of the given size.
         *
         * All entries will be initialised using their default constructors.
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
         * \param rows the number of rows in the new matrix.
         * \param cols the number of columns in the new matrix.
         */
        Matrix(size_t rows, size_t cols) :
                rows_(rows), cols_(cols), data_(new T*[rows]) {
            for (size_t i = 0; i < rows; ++i)
                data_[i] = new T[cols];
        }
        /**
         * Creates a new matrix containing the given hard-coded entries.
         * This constructor can be used (for example) to create
         * hard-coded examples directly in C++ code.
         *
         * Each element of the initialiser list \a data describes a single row
         * of the matrix.
         *
         * \pre The list \a data is non-empty (i.e., the number of rows
         * is positive), and each of its elements is non-empty (i.e., the
         * number of columns is positive).
         *
         * \pre All elements of \a data (representing the rows of the matrix)
         * are lists of the same size.
         *
         * \python The argument \a data should be a Python list of
         * Python lists.
         *
         * \param data the rows of the matrix, each given as a list of elements.
         */
        Matrix(std::initializer_list<std::initializer_list<T>> data) :
                rows_(data.size()), cols_(data.begin()->size()),
                data_(new T*[data.size()]) {
            size_t r = 0;
            for (auto row : data) {
                data_[r] = new T[cols_];
                size_t c = 0;
                for (auto elt : row)
                    data_[r][c++] = elt;
                ++r;
            }
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * This constructor induces a deep copy of \a src.
         *
         * This routine is safe to call even if \a src is uninitialised
         * (in which case this matrix will become uninitialised also).
         *
         * \param src the matrix to clone.
         */
        Matrix(const Matrix& src) : rows_(src.rows_), cols_(src.cols_) {
            if (src.data_) {
                data_ = new T*[src.rows_];
                size_t r, c;
                for (r = 0; r < rows_; r++) {
                    data_[r] = new T[cols_];
                    for (c = 0; c < cols_; c++)
                        data_[r][c] = src.data_[r][c];
                }
            } else {
                data_ = nullptr;
            }
        }
        /**
         * Creates a new clone of the given matrix, which may hold objects of
         * a different type.
         *
         * This constructor induces a deep copy of \a src.
         *
         * This routine is safe to call even if \a src is uninitialised
         * (in which case this matrix will become uninitialised also).
         *
         * This constructor is marked as explicit in the hope of avoiding
         * accidental (and unintentional) mixing of matrix classes.
         *
         * \nopython
         *
         * \tparam U the type of object held by the given matrix \a src.
         * It must be possible to _assign_ an object of type \a U to an
         * object of type \a T.
         *
         * \param src the matrix to clone.
         */
        template <typename U, bool U_ring>
        explicit Matrix(const Matrix<U, U_ring>& src) :
                rows_(src.rows()), cols_(src.columns()) {
            if (src.initialised()) {
                data_ = new T*[src.rows()];
                size_t r, c;
                for (r = 0; r < rows_; r++) {
                    data_[r] = new T[cols_];
                    for (c = 0; c < cols_; c++)
                        data_[r][c] = src.entry(r, c);
                }
            } else {
                data_ = nullptr;
            }
        }
        /**
         * Moves the given matrix into this new matrix.
         * This is a fast (constant time) operation.
         *
         * The matrix that is passed (\a src) will no longer be usable.
         *
         * This routine is safe to call even if \a src is uninitialised
         * (in which case this matrix will become uninitialised also).
         *
         * \param src the matrix to move.
         */
        Matrix(Matrix&& src) noexcept :
                rows_(src.rows_), cols_(src.cols_), data_(src.data_) {
            src.data_ = nullptr;
        }
        /**
         * Destroys this matrix.
         *
         * This destructor is safe to call even if \a src is uninitialised.
         */
        ~Matrix() {
            if (data_) {
                for (size_t i = 0; i < rows_; ++i)
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
         * This routine is safe to call even if \a src is uninitialised
         * (in which case this matrix will become uninitialised also).
         *
         * \param src the matrix to copy.
         * \return a reference to this matrix.
         */
        Matrix& operator = (const Matrix& src) {
            // std::copy() exhibits undefined behaviour with self-assignment.
            if (std::addressof(src) == this)
                return *this;

            if (src.data_) {
                if (rows_ != src.rows_ || cols_ != src.cols_ || ! data_) {
                    if (data_) {
                        for (size_t i = 0; i < rows_; ++i)
                            delete[] data_[i];
                        delete[] data_;
                    }

                    rows_ = src.rows_;
                    cols_ = src.cols_;

                    data_ = new T*[rows_];
                    for (size_t i = 0; i < rows_; ++i)
                        data_[i] = new T[cols_];
                }

                for (size_t i = 0; i < rows_; ++i)
                    std::copy(src.data_[i], src.data_[i] + cols_, data_[i]);
            } else {
                if (data_) {
                    for (size_t i = 0; i < rows_; ++i)
                        delete[] data_[i];
                    delete[] data_;
                }
                rows_ = cols_ = 0;
                data_ = nullptr;
            }
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
         * This routine is safe to call even if \a src is uninitialised
         * (in which case this matrix will become uninitialised also).
         *
         * \param src the matrix to move.
         * \return a reference to this matrix.
         */
        Matrix& operator = (Matrix&& src) noexcept {
            // We need to swap rows_, because src needs this information in
            // order to dispose of our original data properly.
            std::swap(rows_, src.rows_);
            cols_ = src.cols_;
            std::swap(data_, src.data_);
            // Let src dispose of the original contents in its own destructor.
            return *this;
        }

        /**
         * Sets every entry in the matrix to the given value.
         *
         * \param value the value to assign to each entry.
         */
        void fill(const T& value) {
            size_t r, c;
            for (r = 0; r < rows_; r++)
                for (c = 0; c < cols_; c++)
                    data_[r][c] = value;
        }
        /**
         * Deprecated function that sets every entry in the matrix to the
         * given value.
         *
         * \deprecated This routine has been renamed to fill(), to make it
         * clear that it has nothing to do with initialised versus
         * uninitialised matrices.
         *
         * \param value the value to assign to each entry.
         */
        [[deprecated]] void initialise(const T& value) {
            fill(value);
        }

        /**
         * Swaps the contents of this and the given matrix.
         *
         * \param other the matrix whose contents are to be swapped with this.
         */
        inline void swap(Matrix& other) noexcept {
            std::swap(rows_, other.rows_);
            std::swap(cols_, other.cols_);
            std::swap(data_, other.data_);
        }

        /**
         * Returns the number of rows in this matrix.
         *
         * \return the number of rows.
         */
        size_t rows() const {
            return rows_;
        }
        /**
         * Returns the number of columns in this matrix.
         *
         * \return the number of columns.
         */
        size_t columns() const {
            return cols_;
        }
        /**
         * Determines whether this matrix is initialised or uninitialised.
         *
         * The only ways for a matrix to be _uninitialised_ are:
         *
         * - it was created using the default constructor, and has not yet been
         *   initialised using the assignment operator;
         *
         * - it was the result of assignment or copy construction from some
         *   other uninitialised matrix.
         *
         * \return \c true if this matrix is initialised, or \c false if it is
         * uninitialised.
         */
        bool initialised() const {
            return data_;
        }

        /**
         * Returns a read-write reference to the entry at the given
         * row and column.  Rows and columns are numbered beginning at zero.
         *
         * \python In general, to assign values to matrix elements you
         * should use the Python-only set() routine.  This entry() routine does
         * give read-write access to matrix elements in Python, but it does
         * not allow them to be set using the assignment operator.
         * In other words, code such as `matrix.entry(r, c).negate()`
         * will work, but `matrix.entry(r, c) = value` will not; instead
         * you will need to call `matrix.set(r, c, value)`.
         *
         * \param row the row of the desired entry; this must be between
         * 0 and rows()-1 inclusive.
         * \param column the column of the desired entry; this must be
         * between 0 and columns()-1 inclusive.
         * \return a reference to the entry in the given row and column.
         */
        T& entry(size_t row, size_t column) {
            return data_[row][column];
        }
        /**
         * Returns a read-only reference to the entry at the given
         * row and column.  Rows and columns are numbered beginning at zero.
         *
         * \param row the row of the desired entry; this must be between
         * 0 and rows()-1 inclusive.
         * \param column the column of the desired entry; this must be
         * between 0 and columns()-1 inclusive.
         * \return a reference to the entry in the given row and column.
         */
        const T& entry(size_t row, size_t column) const {
            return data_[row][column];
        }
#ifdef __APIDOCS
        /**
         * Python-only routine that sets the entry at the given row and column.
         * Rows and columns are numbered beginning at zero.
         *
         * \nocpp For C++ users, entry() is used for both reading and
         * writing: just write `entry(row, column) = value`.
         *
         * \python In general, to assign values to matrix elements you
         * should use the syntax `matrix.set(row, column, value)`.  The entry()
         * routine does give read-write access to matrix elements in Python,
         * but it does not allow them to be set using the assignment operator.
         * In other words, code such as `matrix.entry(r, c).negate()`
         * will work, but `matrix.entry(r, c) = value` will not.
         *
         * \param row the row of the entry to set; this must be between
         * 0 and rows()-1 inclusive.
         * \param column the column of the entry to set; this must be
         * between 0 and columns()-1 inclusive.
         * \param value the new entry to place in the given row and column.
         */
        void set(size_t row, size_t column, const T& value);
#endif

        /**
         * Returns the transpose of this matrix.  This matrix is not changed.
         *
         * \return the transpose.
         */
        Matrix transpose() const {
            Matrix ans(cols_, rows_);

            size_t r, c;
            for (r = 0; r < rows_; r++)
                for (c = 0; c < cols_; c++)
                    ans.data_[c][r] = data_[r][c];

            return ans;
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
         * \param other the matrix to compare with this.
         * \return \c true if the matrices are equal as described above,
         * or \c false otherwise.
         */
        bool operator == (const Matrix& other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_)
                return false;

            size_t r, c;
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
         * \param other the matrix to compare with this.
         * \return \c true if the matrices are different as described above,
         * or \c false otherwise.
         */
        bool operator != (const Matrix& other) const {
            return ! ((*this) == other);
        }

        /**
         * Swaps the elements of the two given rows in the matrix.
         *
         * This operation is constant time (unlike swapping columns,
         * which is linear time).
         *
         * Unlike swapCols(), this operation does not take a \a fromCol
         * argument.  This is because swapping rows is already as fast possible
         * (internally, just a single pointer swap), and so iterating along
         * only part of the row would slow the routine down considerably.
         *
         * \pre The two given rows are between 0 and rows()-1 inclusive.
         *
         * \param first the first row to swap.
         * \param second the second row to swap.
         */
        void swapRows(size_t first, size_t second) {
            if (first != second)
                std::swap(data_[first], data_[second]);
        }
        /**
         * Swaps the elements of the two given columns in the matrix.
         *
         * This operation is linear time (unlike swapping rows,
         * which is constant time).
         *
         * If the optional argument \a fromRow is passed, then the
         * operation will only be performed for the elements from that
         * row down to the bottom of each column (inclusive).
         *
         * \pre The two given columns are between 0 and columns()-1 inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() -1 inclusive.
         *
         * \param first the first column to swap.
         * \param second the second column to swap.
         * \param fromRow the starting point in each column from which the
         * operation will be performed.
         */
        void swapCols(size_t first, size_t second, size_t fromRow = 0) {
            if (first != second) {
                // Give ourselves a chance to use a customised swap(),
                // if one exists for type T.
                using std::swap;
                for (size_t i = fromRow; i < rows_; i++)
                    swap(data_[i][first], data_[i][second]);
            }
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            size_t r, c;
            out << '[';
            for (r = 0; r < rows_; ++r) {
                if (r > 0)
                    out << ' ';
                out << '[';
                for (c = 0; c < cols_; ++c)
                    out << ' ' << data_[r][c];
                out << " ]";
            }
            out << ']';
        }
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const {
            size_t r, c;
            for (r = 0; r < rows_; r++) {
                for (c = 0; c < cols_; c++) {
                    if (c > 0) out << ' ';
                    out << data_[r][c];
                }
                out << '\n';
            }
        }

        /**
         * Returns an identity matrix of the given size.
         * The matrix returned will have \a size rows and \a size columns.
         *
         * \pre The template argument \a ring is \c true.
         *
         * \param size the number of rows and columns of the matrix to build.
         * \return an identity matrix of the given size.
         */
        static Matrix identity(size_t size) {
            static_assert(ring, "Matrix<T>::identity() requires "
                "type T to represent a ring.");

            Matrix ans(size, size);
            ans.fill(0);
            for (size_t i = 0; i < size; ++i)
                ans.data_[i][i] = 1;
            return ans;
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will have
         * `entry(r,c)` equal to 1 if `r == c` and 0 otherwise.
         *
         * \pre The template argument \a ring is \c true.
         */
        void makeIdentity() {
            static_assert(ring, "Matrix<T>::makeIdentity() requires "
                "type T to represent a ring.");

            this->fill(0);
            for (size_t i = 0; i < this->rows_ && i < this->cols_; i++)
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
         * \pre The template argument \a ring is \c true.
         *
         * \return \c true if and only if this is a square identity matrix.
         */
        bool isIdentity() const {
            static_assert(ring, "Matrix<T>::isIdentity() requires "
                "type T to represent a ring.");

            if (this->rows_ != this->cols_)
                return false;

            size_t r, c;
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
         * \pre The template argument \a ring is \c true.
         *
         * \return \c true if and only if all entries in the matrix are zero.
         */
        bool isZero() const {
            static_assert(ring, "Matrix<T>::isZero() requires "
                "type T to represent a ring.");

            for (size_t r=0; r<this->rows_; ++r)
                for (size_t c=0; c<this->cols_; ++c)
                    if (this->data_[r][c] != 0)
                        return false;
            return true;
        }

        /**
         * Adds the given source row to the given destination row.
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * \warning If you only wish to add a portion of a row, be careful:
         * you cannot just pass the usual \a fromCol argument, since this will
         * be interpreted as a coefficient to be used with the other version
         * of addRow() that adds _several_ copies of the source row.
         * Instead you will need to call addRowFrom().
         *
         * \param source the row to add.
         * \param dest the row that will be added to.
         */
        void addRow(size_t source, size_t dest) {
            static_assert(ring, "Matrix<T>::addRow() requires "
                "type T to represent a ring.");

            for (size_t i = 0; i < this->cols_; i++)
                this->data_[dest][i] += this->data_[source][i];
        }
        /**
         * Adds a portion of the given source row to the given destination row.
         *
         * This is similar to addRow(), except that the operation will
         * only be performed for the elements from the column \a fromCol
         * to the rightmost end of the row (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() -1 inclusive.
         *
         * \param source the row to add.
         * \param dest the row that will be added to.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void addRowFrom(size_t source, size_t dest, size_t fromCol) {
            static_assert(ring, "Matrix<T>::addRowFrom() requires "
                "type T to represent a ring.");

            for (size_t i = fromCol; i < this->cols_; i++)
                this->data_[dest][i] += this->data_[source][i];
        }
        /**
         * Adds the given number of copies of the given source row to
         * the given destination row.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * If the optional argument \a fromCol is passed, then the
         * operation will only be performed for the elements from that
         * column to the rightmost end of the row (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() -1 inclusive.
         *
         * \param source the row to add.
         * \param dest the row that will be added to.
         * \param copies the number of copies of \a source to add to \a dest.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void addRow(size_t source, size_t dest, T copies, size_t fromCol = 0) {
            static_assert(ring, "Matrix<T>::addRow() requires "
                "type T to represent a ring.");

            for (size_t i = fromCol; i < this->cols_; i++)
                this->data_[dest][i] += copies * this->data_[source][i];
        }
        /**
         * Adds the given source column to the given destination column.
         *
         * \warning If you only wish to add a portion of a column, be careful:
         * you cannot just pass the usual \a fromRow argument, since this will
         * be interpreted as a coefficient to be used with the other version
         * of addCol() that adds _several_ copies of the source column.
         * Instead you will need to call addColFrom().
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         */
        void addCol(size_t source, size_t dest) {
            static_assert(ring, "Matrix<T>::addCol() requires "
                "type T to represent a ring.");

            for (size_t i = 0; i < this->rows_; i++)
                this->data_[i][dest] += this->data_[i][source];
        }
        /**
         * Adds a portion of the given source column to the given destination
         * column.
         *
         * This is similar to addCol(), except that the operation will
         * only be performed for the elements from the row \a fromRow
         * down to the bottom of the column (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() -1 inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void addColFrom(size_t source, size_t dest, size_t fromRow = 0) {
            static_assert(ring, "Matrix<T>::addColFrom() requires "
                "type T to represent a ring.");

            for (size_t i = fromRow; i < this->rows_; i++)
                this->data_[i][dest] += this->data_[i][source];
        }
        /**
         * Adds the given number of copies of the given source column to
         * the given destination column.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * If the optional argument \a fromRow is passed, then the
         * operation will only be performed for the elements from that
         * row down to the bottom of the column (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() -1 inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         * \param copies the number of copies of \a source to add to \a dest.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void addCol(size_t source, size_t dest, T copies, size_t fromRow = 0) {
            static_assert(ring, "Matrix<T>::addCol() requires "
                "type T to represent a ring.");

            for (size_t i = fromRow; i < this->rows_; i++)
                this->data_[i][dest] += copies * this->data_[i][source];
        }
        /**
         * Multiplies the given row by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * If the optional argument \a fromCol is passed, then the
         * operation will only be performed for the elements from that
         * column to the rightmost end of the row (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The given row is between 0 and rows()-1 inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() -1 inclusive.
         *
         * \param row the row to work with.
         * \param factor the factor by which to multiply the given row.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void multRow(size_t row, T factor, size_t fromCol = 0) {
            static_assert(ring, "Matrix<T>::multRow() requires "
                "type T to represent a ring.");

            for (size_t i = fromCol; i < this->cols_; i++)
                this->data_[row][i] *= factor;
        }
        /**
         * Multiplies the given column by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * If the optional argument \a fromRow is passed, then the
         * operation will only be performed for the elements from that
         * row down to the bottom of the column (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The given column is between 0 and columns()-1 inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() -1 inclusive.
         *
         * \param column the column to work with.
         * \param factor the factor by which to multiply the given column.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void multCol(size_t column, T factor, size_t fromRow = 0) {
            static_assert(ring, "Matrix<T>::multCol() requires "
                "type T to represent a ring.");

            for (size_t i = fromRow; i < this->rows_; i++)
                this->data_[i][column] *= factor;
        }
        /**
         * Rewrites two rows as linear combinations of those two rows.
         *
         * Specifically, if \a R1 and \a R2 are the original values of
         * rows \a row1 and \a row2 respectively, then:
         *
         * - Row \a row1 will become `coeff11 * R1 + coeff12 * R2`;
         * - Row \a row2 will become `coeff21 * R1 + coeff22 * R2`.
         *
         * The four coefficients are passed by value, in case they are
         * elements of the rows to be changed.
         *
         * If the optional argument \a fromCol is passed, then the
         * operation will only be performed for the elements from that
         * column to the rightmost end of each row (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() -1 inclusive.
         *
         * \param row1 the first row to operate on.
         * \param row2 the second row to operate on.
         * \param coeff11 the coefficient of row \a row1 to use when
         * rewriting row \a row1.
         * \param coeff12 the coefficient of row \a row2 to use when
         * rewriting row \a row1.
         * \param coeff21 the coefficient of row \a row1 to use when
         * rewriting row \a row2.
         * \param coeff22 the coefficient of row \a row2 to use when
         * rewriting row \a row2.
         * \param fromCol the starting point in the rows from which the
         * operation will be performed.
         */
        void combRows(size_t row1, size_t row2, T coeff11, T coeff12,
                T coeff21, T coeff22, size_t fromCol = 0) {
            static_assert(ring, "Matrix<T>::combRows() requires "
                "type T to represent a ring.");

            for (size_t i = fromCol; i < this->cols_; ++i) {
                T tmp = coeff11 * this->data_[row1][i] +
                    coeff12 * this->data_[row2][i];
                this->data_[row2][i] = coeff21 * this->data_[row1][i] +
                    coeff22 * this->data_[row2][i];
                this->data_[row1][i] = std::move(tmp);
            }
        }
        /**
         * Rewrites two columns as linear combinations of those two columns.
         *
         * Specifically, if \a C1 and \a C2 are the original values of
         * columns \a col1 and \a col2 respectively, then:
         *
         * - Column \a col1 will become `coeff11 * C1 + coeff12 * C2`;
         * - Column \a col2 will become `coeff21 * C1 + coeff22 * C2`.
         *
         * The four coefficients are passed by value, in case they are
         * elements of the columns to be changed.
         *
         * If the optional argument \a fromRow is passed, then the
         * operation will only be performed for the elements from that
         * column down to the bottom of each column (inclusive).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() -1 inclusive.
         *
         * \param col1 the first column to operate on.
         * \param col2 the second column to operate on.
         * \param coeff11 the coefficient of column \a col1 to use when
         * rewriting column \a col1.
         * \param coeff12 the coefficient of column \a col2 to use when
         * rewriting column \a col1.
         * \param coeff21 the coefficient of column \a col1 to use when
         * rewriting column \a col2.
         * \param coeff22 the coefficient of column \a col2 to use when
         * rewriting column \a col2.
         * \param fromRow the starting point in the columns from which the
         * operation will be performed.
         */
        void combCols(size_t col1, size_t col2, T coeff11, T coeff12,
                T coeff21, T coeff22, size_t fromRow = 0) {
            static_assert(ring, "Matrix<T>::combCols() requires "
                "type T to represent a ring.");

            for (size_t i = fromRow; i < this->rows_; ++i) {
                T tmp = coeff11 * this->data_[i][col1] +
                    coeff12 * this->data_[i][col2];
                this->data_[i][col2] = coeff21 * this->data_[i][col1] +
                    coeff22 * this->data_[i][col2];
                this->data_[i][col1] = std::move(tmp);
            }
        }

        /**
         * Multiplies this by the given matrix, and returns the result.
         * This matrix is not changed.
         *
         * The two matrices being multiplied may use different underlying types
         * (e.g., you can multiply a matrix of LargeInteger objects with a
         * matrix of native C++ long integers).  The type of object that is
         * stored in the resulting matrix will be deduced accordingly
         * (specifically, it will be the type obtained by multiplying objects
         * of types \a T and \a U using the binary multiplication operator).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * \param other the other matrix to multiply this matrix by.
         * \return the product matrix `this * other`.
         */
        template <typename U>
        Matrix<decltype(T() * U())> operator * (
                const Matrix<U, true /* ring */>& other) const {
            static_assert(ring, "Matrix multiplication can only be "
                "used with matrices over rings.");

            using Ans = decltype(T() * U());
            Matrix<Ans> ans(this->rows_, other.cols_);

            size_t row, col, k;
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
         * Multiplies this matrix by the given vector, and returns the result.
         * The given vector is treated as a column vector.
         *
         * The matrix and vector may use different underlying types
         * (e.g., you can multiply a matrix of LargeInteger objects with a
         * vector of native C++ long integers).  The type of object that is
         * stored in the resulting vector will be deduced accordingly
         * (specifically, it will be the type obtained by multiplying objects
         * of types \a T and \a U using the binary multiplication operator).
         *
         * \pre The template argument \a ring is \c true.
         * \pre The length of the given vector is precisely the number of
         * columns in this matrix.
         *
         * \param other the vector to multiply this matrix by.
         * \return the product `this * other`, which will be a
         * vector whose length is the number of rows in this matrix.
         */
        template <typename U>
        Vector<decltype(T() * U())> operator * (const Vector<U>& other) const {
            static_assert(ring, "Matrix-vector multiplication can only be "
                "used with matrices over a ring.");

            using Ans = decltype(T() * U());
            Vector<Ans> ans(this->rows_);

            size_t row, col;
            for (row = 0; row < rows_; ++row) {
                Ans elt = 0;
                for (col = 0; col < cols_; ++col)
                    elt += (data_[row][col] * other[col]);
                ans[row] = elt;
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
         * Although the Matrix class does not formally support empty matrices,
         * if this _is_ found to be a 0-by-0 matrix then the determinant
         * returned will be 1.
         *
         * \pre The template argument \a ring is \c true.
         * \pre This is a square matrix.
         *
         * \exception FailedPrecondition This matrix is not square.
         *
         * \return the determinant of this matrix.
         */
        T det() const {
            static_assert(ring, "Matrix<T>::det() requires "
                "type T to represent a ring.");

            size_t n = this->rows_;
            if (n != this->cols_)
                throw FailedPrecondition("Determinants can only be "
                    "computed for square matrices.");
            if (n == 0)
                return 1;

            T* partial[2];
            partial[0] = new T[n * n];
            partial[1] = new T[n * n];

            size_t len, head, curr, prevHead, prevCurr;

            // Treat the smallest cases of len = 1 separately.
            int layer = 0; // always 0 or 1
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
         * Negates all elements in the given row.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * \param row the index of the row whose elements should be negated.
         */
        void negateRow(size_t row) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::negateRow() requires type T to be one of "
                "Regina's own integer types.");

            for (T* x = this->data_[row]; x != this->data_[row] + cols_; ++x)
                x->negate();
        }

        /**
         * Negates all elements in the given column.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * \param col the index of the column whose elements should be negated.
         */
        void negateCol(size_t col) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::negateCol() requires type T to be one of "
                "Regina's own integer types.");

            for (T** row = this->data_; row != this->data_ + rows_; ++row)
                (*row)[col].negate();
        }

        /**
         * Divides all elements of the given row by the given integer.
         * This can only be used when the given integer divides into all
         * row elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given row are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given row (i.e., it leaves no remainder).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * \param row the index of the row whose elements should be
         * divided by \a divBy.
         * \param divBy the integer to divide each row element by.
         */
        void divRowExact(size_t row, const T& divBy) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::divRowExact() requires type T to be one of "
                "Regina's own integer types.");

            for (T* x = this->data_[row]; x != this->data_[row] + cols_; ++x)
                x->divByExact(divBy);
        }

        /**
         * Divides all elements of the given column by the given integer.
         * This can only be used when the given integer divides into all
         * column elements exactly (with no remainder).  For the Integer class,
         * this may be much faster than ordinary division.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given column are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given column (i.e., it leaves no remainder).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * \param col the index of the column whose elements should be
         * divided by \a divBy.
         * \param divBy the integer to divide each column element by.
         */
        void divColExact(size_t col, const T& divBy) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::divColExact() requires type T to be one of "
                "Regina's own integer types.");

            for (T** row = this->data_; row != this->data_ + rows_; ++row)
                (*row)[col].divByExact(divBy);
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given row.  The value returned is guaranteed to be non-negative.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * \param row the index of the row whose gcd should be computed.
         * \return the greatest common divisor of all elements of this row.
         */
        T gcdRow(size_t row) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::gcdRow() requires type T to be one of "
                "Regina's own integer types.");

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
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * \param col the index of the column whose gcd should be computed.
         * \return the greatest common divisor of all elements of this column.
         */
        T gcdCol(size_t col) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::gcdCol() requires type T to be one of "
                "Regina's own integer types.");

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
         * changed at all, it will be divided by a _positive_ integer.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * \param row the index of the row to reduce.
         */
        void reduceRow(size_t row) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::reduceRow() requires type T to be one of "
                "Regina's own integer types.");

            T gcd = gcdRow(row);
            if (gcd != 0 && gcd != 1)
                divRowExact(row, gcd);
        }

        /**
         * Reduces the given column by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the column is
         * changed at all, it will be divided by a _positive_ integer.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * \param col the index of the column to reduce.
         */
        void reduceCol(size_t col) {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::reduceCol() requires type T to be one of "
                "Regina's own integer types.");

            T gcd = gcdCol(col);
            if (gcd != 0 && gcd != 1)
                divColExact(col, gcd);
        }

        /**
         * Transforms this matrix into row echelon form.  The transformation
         * will perform only row operations.
         *
         * This is simpler than the global routine regina::columnEchelonForm():
         * it does not return the change of basis matrices, and it processes
         * all columns in order from left to right (instead of passing a
         * custom column list).
         *
         * Our convention is that a matrix is in row echelon form if:
         *
         * - each row is either zero or there is a first non-zero entry which
         *   is positive;
         * - moving from the top row to the bottom, these first non-zero
         *   entries have strictly increasing column indices;
         * - for each first non-zero row entry, in that column all the elements
         *   above are smaller and non-negative (and all elements below are
         *   already zero by the previous condition);
         * - all the zero rows are at the bottom of the matrix.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         *
         * \return the rank of this matrix, i.e., the number of non-zero rows
         * remaining.
         */
        size_t rowEchelonForm() {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::rowEchelonForm() requires type T to be one of "
                "Regina's own integer types.");

            size_t i, j;

            // The current working row and column:
            // The entries to the left of currCol will not change, and
            // above currRow all that can happen is some reduction.
            size_t currRow = 0;
            size_t currCol = 0;

            // The algorithm works from left to right.
            while (currRow < rows_ && currCol < cols_) {
                // Identify the first non-zero entry in currCol.
                for (i = currRow; i < rows_; ++i)
                    if (data_[i][currCol] != 0)
                        break;

                if (i == rows_) {
                    // The column is entirely zero.  Nothing to do.
                    ++currCol;
                    continue;
                }

                if (i > currRow) {
                    // Swap rows so this first non-zero entry is currRow.
                    swapRows(currRow, i);
                }

                // Now our first non-zero entry is in currRow.

                // Zero out all entries in currCol that appear *below* currRow.
                for (i = currRow + 1; i < rows_; ++i)
                    if (data_[i][currCol] != 0) {
                        auto [gcd, u, v] = data_[currRow][currCol].
                            gcdWithCoeffs(data_[i][currCol]);
                        T a = data_[currRow][currCol].divExact(gcd);
                        T b = data_[i][currCol].divExact(gcd);
                        for (j = 0; j < cols_; ++j) {
                            T tmp = u * data_[currRow][j] + v * data_[i][j];
                            data_[i][j] = a * data_[i][j] -
                                b * data_[currRow][j];
                            data_[currRow][j] = tmp;
                        }
                    }

                // Ensure that our leading coefficient (currRow, currCol)
                // is positive.
                if (data_[currRow][currCol] < 0) {
                    multRow(currRow, -1);
                }

                // Finally, reduce the entries in currCol *above* currRow.
                for (i = 0; i < currRow; ++i) {
                    auto [d, r] = data_[i][currCol].divisionAlg(
                        data_[currRow][currCol]);
                    if (d != 0)
                        addRow(currRow /* source */, i /* dest */, -d);
                }

                ++currRow;
                ++currCol;
            }

            return currRow;
        }

        /**
         * Transforms this matrix into column echelon form.  The transformation
         * will perform only column operations.
         *
         * This is simpler than the global routine regina::columnEchelonForm():
         * it does not return the change of basis matrices, and it processes
         * all rows in order from left to right (instead of passing a
         * custom row list).
         *
         * Our convention is that a matrix is in column echelon form if:
         *
         * - each column is either zero or there is a first non-zero entry
         *   which is positive;
         * - moving from the left column to the right, these first non-zero
         *   entries have strictly increasing row indices;
         * - for each first non-zero column entry, in that row all the elements
         *   to the left are smaller and non-negative (and all elements to the
         *   right are already zero by the previous condition);
         * - all the zero columns are at the right hand end of the matrix.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         *
         * \return the rank of this matrix, i.e., the number of non-zero
         * columns remaining.
         */
        size_t columnEchelonForm() {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::columnEchelonForm() requires type T to be one of "
                "Regina's own integer types.");

            size_t i, j;

            // The current working row and column:
            // The entries above currRow will not change, and to the left of
            // currCol all that can happen is some reduction.
            size_t currRow = 0;
            size_t currCol = 0;

            // The algorithm works from top to bottom.
            while (currRow < rows_ && currCol < cols_) {
                // Identify the first non-zero entry in currRow.
                for (i = currCol; i < cols_; ++i)
                    if (data_[currRow][i] != 0)
                        break;

                if (i == cols_) {
                    // The row is entirely zero.  Nothing to do.
                    ++currRow;
                    continue;
                }

                if (i > currCol) {
                    // Swap columns so this first non-zero entry is currCol.
                    swapCols(currCol, i);
                }

                // Now our first non-zero entry is in currCol.

                // Zero out all entries in currRow that appear right of currCol.
                for (i = currCol + 1; i < cols_; ++i)
                    if (data_[currRow][i] != 0) {
                        auto [gcd, u, v] = data_[currRow][currCol].
                            gcdWithCoeffs(data_[currRow][i]);
                        T a = data_[currRow][currCol].divExact(gcd);
                        T b = data_[currRow][i].divExact(gcd);
                        for (j = 0; j < rows_; ++j) {
                            T tmp = u * data_[j][currCol] + v * data_[j][i];
                            data_[j][i] = a * data_[j][i] -
                                b * data_[j][currCol];
                            data_[j][currCol] = tmp;
                        }
                    }

                // Ensure that our leading coefficient (currRow, currCol)
                // is positive.
                if (data_[currRow][currCol] < 0) {
                    multCol(currCol, -1);
                }

                // Finally, reduce the entries in currRow left of currCol.
                for (i = 0; i < currCol; ++i) {
                    auto [d, r] = data_[currRow][i].divisionAlg(
                        data_[currRow][currCol]);
                    if (d != 0)
                        addCol(currCol /* source */, i /* dest */, -d);
                }

                ++currRow;
                ++currCol;
            }

            return currCol;
        }

        /**
         * A non-destructive routine that returns the rank of this matrix
         * whilst preserving the contents of the matrix.
         *
         * Normally, a rank computation would involve modifying the matrix
         * directly (e.g., by converting it to row echelon form).  In contrast,
         * this routine will leave the matrix unchanged.  The cost is an extra
         * deep copy in the implementation.
         *
         * If your matrix is disposable (i.e., you will never need to use it
         * again), then it is faster to use the rvalue reference version of
         * this routine, which will avoid the extra overhead of the deep copy.
         * To do this, replace `matrix.rank()` with `std::move(matrix).rank()`.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         *
         * \python Only the const version of rank() (i.e., this version)
         * is available for Python users.
         *
         * \return the rank of this matrix.
         */
        size_t rank() const& {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::rank() requires type T to be one of "
                "Regina's own integer types.");

            // Make a deep copy, which we can modify as we compute rank.
            return Matrix(*this).rowEchelonForm();
        }

        /**
         * A destructive routine that returns the rank of this matrix.
         *
         * Here "destructive" means that this routine modifies the matrix
         * directly as it performs the rank computation.  For this reason,
         * it is declared as an rvalue reference member function: it should
         * only be used if you do not care about the contents of the matrix
         * afterwards.
         *
         * To use this destructive rank computation, you can call
         * `std::move(matrix).rank()`.
         *
         * If you need to preserve the contents of the matrix, you should
         * instead call the const version of this function, which you can
         * simply access as `matrix.rank()`.  The (minor) cost of this
         * constness will be the extra overhead of an internal deep copy.
         *
         * \pre Type \a T is one of Regina's own integer classes (Integer,
         * LargeInteger, or NativeIntgeger).
         *
         * \nopython Only the const version of rank() is available for Python
         * users.
         *
         * \return the rank of this matrix.
         */
        size_t rank() && {
            static_assert(IsReginaInteger<T>::value,
                "Matrix<T>::rank() requires type T to be one of "
                "Regina's own integer types.");

            return rowEchelonForm();
        }
};

/**
 * Swaps the contents of the given matrices.
 *
 * This global routine simply calls Matrix<T>::swap(); it is provided
 * so that Matrix<T> meets the C++ Swappable requirements.
 *
 * \param a the first matrix whose contents should be swapped.
 * \param b the second matrix whose contents should be swapped.
 *
 * \ingroup maths
 */
template <typename T>
inline void swap(Matrix<T>& a, Matrix<T>& b) noexcept {
    a.swap(b);
}

/**
 * A matrix of arbitrary-precision integers.
 *
 * This is the most common class used by Regina when running algebraic
 * algorithms over integer matrices.  Since the underlying type is
 * Regina's Integer class, calculations will be exact regardless of
 * how large the integers become.
 *
 * \python This instance of the Matrix template class is made
 * available to Python.
 *
 * \ingroup maths
 */
using MatrixInt = Matrix<Integer>;

/**
 * A matrix of booleans.
 *
 * This is used in a handful of places in Regina to represent incidence or
 * adjacency matrices.
 *
 * \python This instance of the Matrix template class is made
 * available to Python.
 *
 * \ingroup maths
 */
using MatrixBool = Matrix<bool>;

} // namespace regina

#endif
