
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

/*! \file maths/matrix.h
 *  \brief Deals with matrices of elements of various types.
 */

#ifndef __REGINA_MATRIX_H
#ifndef __DOXYGEN
#define __REGINA_MATRIX_H
#endif

#include <array>
#include <initializer_list>
#include <iostream>
#include <memory>
#include "regina-core.h"
#include "concepts/core.h"
#include "concepts/io.h"
#include "core/output.h"
#include "maths/forward.h"
#include "maths/integer.h"
#include "maths/ring.h"
#include "utilities/fixedarray.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class Rational;
template <Ring T> requires Writeable<T> && IntegerCompatible<T> class Vector;

/**
 * Represents different algorithms for computing Smith normal forms of matrices.
 *
 * \ingroup maths
 */
enum class SNFAlgorithm {
    /**
     * Allow Regina to choose a sensible default.
     */
    Default = 0,
    /**
     * The Hafner-McCurley algorithm.
     *
     * This has historically been Regina's default Smith normal form algorithm.
     * It follows James L. Hafner and Kevin S. McCurley, "Asymptotically fast
     * triangularization of matrices over rings", SIAM J. Comput. 20 (1991),
     * no. 6, 1068–1083.
     *
     * This algorithm does not use modular arithmetic to control the
     * intermediate coefficient explosion.
     */
    HafnerMcCurley = 10,
    /**
     * An alternative Smith normal form algorithm that may be preferable
     * for extremely large matrices.
     *
     * This uses a variant of the Hafner-McCurley algorithm and Havas-Holt-Rees'
     * description of pivoting methods.  See James L. Hafner and
     * Kevin S. McCurley, "Asymptotically fast triangularization of matrices
     * over rings", SIAM J. Comput. 20 (1991), no. 6, 1068–1083, and
     * George Havas, Derek F. Holt and Sarah Rees, "Recognizing badly presented
     * Z-modules", Linear Algebra Appl. 192 (1993), 137–163.
     *
     * This algorithm does not use modular arithmetic to control the
     * intermediate coefficient explosion.
     */
    Metrical = 11,
};

/**
 * Represents different algorithms for computing adjugate matrices and
 * determinants.
 *
 * In all complexity notes, \a n denotes the side length of the input matrix,
 * and ω denotes the time complexity of matrix multiplication (which in Regina
 * is currently the naïve `n^3`).
 *
 * \ingroup maths
 */
enum class AdjugateAlgorithm {
    /**
     * Allow Regina to choose a sensible default.
     */
    Default = 0,
    /**
     * The Faddeev-Leverrier algorithm.  This runs in time `O(n^{ω+1})`, and
     * the bottleneck is \a n matrix multiplications.
     *
     * Every matrix multiplication is by the input matrix.  This is beneficial
     * if the input matrix is sparse and computing products of matrix elements
     * is expensive (e.g., when working with matrices of polynomials).
     * In particular, if the input matrix has a constant number of non-zero
     * entries per row or column, then each matrix multiplication will
     * involve `O(n^3)` element products (assuming naïve matrix multiplication),
     * but only `O(n^2)` of these products will be non-trivial.
     *
     * For a modern write-up of this algorithm, see Fredrik Johansson,
     * "On a fast and nearly division-free algorithm for the characteristic
     * polynomial", preprint, hal-03016034v3, November 2020.
     */
    FaddeevLeverrier = 10,
    /**
     * The Faddeev-Leverrier algorithm, with the baby-step/giant-step
     * improvement of Preparata and Sarwate.  This runs in time
     * `O(n^{ω+0.5} + n^3)`.  The bottlenecks are `√n` multiplications by the
     * input matrix, `√n` multiplications by arbitrary (typically dense)
     * matrices, and \a n "product trace" computations that compute `Tr(X * Y)`
     * without computing the full matrix product `X * Y`.
     *
     * Although the time complexity appears better than plain Faddeev-Leverrier,
     * one must be careful: if the input matrix is sparse and computing
     * products of matrix elements is expensive (e.g., when working with
     * matrices of polynomials), the need to multiply dense matrices here may
     * in fact make the overall time complexity worse once the cost of element
     * products is factored in.
     *
     * For a modern write-up of this algorithm, see Fredrik Johansson,
     * "On a fast and nearly division-free algorithm for the characteristic
     * polynomial", preprint, hal-03016034v3, November 2020.
     */
    PreparataSarwate = 11,
    /**
     * The Mahajan-Vinay dynamic programming algorithm, which is _only_
     * suitable for computing determinants (not adjugates).  This runs in time
     * `O(n^4)`.
     *
     * For further details, see Meena Mahajan and V. Vinay, "Determinant:
     * Combinatorics, algorithms, and complexity", Chicago J. Theor.
     * Comput. Sci., Vol. 1997, Article 5.
     */
    MahajanVinay = 100
};

/**
 * Represents a matrix of elements of the given type \a T.
 *
 * As of Regina 7.4, the extra boolean \a ring template parameter is gone;
 * instead the relevant functions are only enabled in scenarios where \a T
 * adheres to the Ring concept.  Nowadays you should always just use the type
 * `Matrix<T>`.
 *
 * As of Regina 8.0, empty matrices (where one or both of the matrix dimensions
 * are zero) are explicitly supported.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  If a matrix is moved from,
 * it can later be reused by assigning it a new value.
 *
 * \headers Some routines in this template class are implemented in a separate
 * header (matrix-impl.h), which is not included automatically by this file.
 * However, typical end users will not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for
 * `Matrix<Integer>`, which is where those routines are most commonly used.
 *
 * \python The C++ types `Matrix<Integer>`, `Matrix<bool>` and `Matrix<double>`
 * are available using the Python names `MatrixInt`, `MatrixBool` and
 * `MatrixReal` respectively.
 *
 * \ingroup maths
 */
template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writeable<T>
class Matrix : public Output<Matrix<T>> {
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
        T* data_;
            /**< The actual entries in the matrix.
                 The entry at position `(r, c)` is `data_[r * cols_ + c]`.
                 For an empty matrix (where \a rows_ and/or \a cols_ is 0),
                 this _may_ be `null` (but this is not required).
                 For a matrix that has been moved out of and as in an invalid
                 (but re-assignable) state, this may also be `null` regardless
                 of the matrix dimensions; however, we still insist that if
                 \a data_ is non-null then the array it points to has capacity
                 at least `rows_ * cols_`. */

    public:
        /**
         * Creates a new empty matrix.  The size of this matrix will be `0×0`.
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
         * \param size the number of rows and columns in the new matrix.
         */
        Matrix(size_t size) : rows_(size), cols_(size),
                data_(size ? new T[size * size] : nullptr) {
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
         * \param rows the number of rows in the new matrix.
         * \param cols the number of columns in the new matrix.
         */
        Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols),
                data_(rows && cols ? new T[rows * cols] : nullptr) {
        }
        /**
         * Creates a new matrix containing the given hard-coded entries.
         * This constructor can be used (for example) to create
         * hard-coded examples directly in C++ code.
         *
         * Each element of the initialiser list \a data describes a single row
         * of the matrix.
         *
         * Note that you cannot create a `0×k` matrix using this constructor
         * for positive \a k, but you can always use `Matrix(0, k)` instead.
         *
         * \pre All of the sub-lists within \a data have the same size (i.e.,
         * each row has the same number of columns).
         *
         * \python The argument \a data should be a Python list of
         * Python lists.
         *
         * \exception InvalidArgument The number of rows in the given list is
         * positive, but the individual rows are empty and/or have different
         * sizes.
         *
         * \param data the rows of the matrix, each given as a list of elements.
         */
        Matrix(std::initializer_list<std::initializer_list<T>> data) :
                rows_(data.size()) {
            if (rows_) {
                cols_ = data.begin()->size();
                if (cols_) {
                    data_ = new T[rows_ * cols_];
                    T* pos = data_;
                    for (auto row : data) {
                        if (row.size() != cols_) {
                            delete[] data_;
                            throw InvalidArgument("The matrix rows must all "
                                "have the same length");
                        }
                        for (auto elt : row)
                            *pos++ = elt;
                    }
                } else
                    data_ = nullptr;
            } else {
                cols_ = 0;
                data_ = nullptr;
            }
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * This constructor induces a deep copy of \a src.
         *
         * \param src the matrix to clone.
         */
        Matrix(const Matrix& src) : rows_(src.rows_), cols_(src.cols_) {
            if (size_t size = src.rows_ * src.cols_) {
                data_ = new T[size];
                std::copy(src.data_, src.data_ + size, data_);
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
         * This constructor is marked as explicit in the hope of avoiding
         * accidental (and unintentional) mixing of matrix classes.
         *
         * \nopython
         *
         * \param src the matrix to clone.
         */
        template <AssignableTo<T> U>
        explicit Matrix(const Matrix<U>& src) :
                rows_(src.rows_), cols_(src.cols_) {
            if (size_t size = src.rows_ * src.cols_) {
                data_ = new T[size];
                std::copy(src.data_, src.data_ + size, data_);
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
         * \param src the matrix to move.
         */
        Matrix(Matrix&& src) noexcept :
                rows_(src.rows_), cols_(src.cols_), data_(src.data_) {
            // If this leaves src in an invalid state, it will be with positive
            // size but null data (which makes it safe for re-assignment).
            src.data_ = nullptr;
        }
        /**
         * Destroys this matrix.
         */
        ~Matrix() {
            delete[] data_;
        }

        /**
         * Copies the given matrix into this matrix.
         *
         * It does not matter if this and the given matrix have different
         * sizes; if they do then this matrix will be resized as a result.
         *
         * This operator induces a deep copy of \a src.
         *
         * \param src the matrix to copy.
         * \return a reference to this matrix.
         */
        Matrix& operator = (const Matrix& src) {
            // std::copy() exhibits undefined behaviour with self-assignment.
            if (std::addressof(src) == this)
                return *this;

            if (size_t size = src.rows_ * src.cols_) {
                // We could keep data_ if rows_ * cols_ is larger than we need.
                // For now don't worry; we assume matrices won't change size
                // very often.
                if (rows_ != src.rows_ || cols_ != src.cols_ || ! data_) {
                    delete[] data_;
                    data_ = new T[size];
                }
                std::copy(src.data_, src.data_ + size, data_);
            } else {
                delete[] data_;
                data_ = nullptr;
            }
            rows_ = src.rows_;
            cols_ = src.cols_;
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
         * \param src the matrix to move.
         * \return a reference to this matrix.
         */
        Matrix& operator = (Matrix&& src) noexcept {
            // If this leaves src in an invalid state, it will be with positive
            // size but null data (which makes is safe for re-assignment).
            // To ensure this, we must delete any previous data now (as
            // opposed to handing it over to src for deletion later).
            delete[] data_;
            rows_ = src.rows_;
            cols_ = src.cols_;
            data_ = src.data_;
            src.data_ = nullptr;
            return *this;
        }

        /**
         * Sets every entry in this matrix to the given value.
         *
         * \param value the value to assign to each entry.
         */
        void fill(const T& value) {
            std::fill(data_, data_ + rows_ * cols_, value);
        }
        /**
         * Deprecated function that sets every entry in this matrix to the
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
         * 0 and `rows()-1` inclusive.
         * \param column the column of the desired entry; this must be
         * between 0 and `columns()-1` inclusive.
         * \return a reference to the entry in the given row and column.
         */
        T& entry(size_t row, size_t column) {
            return data_[row * cols_ + column];
        }
        /**
         * Returns a read-only reference to the entry at the given
         * row and column.  Rows and columns are numbered beginning at zero.
         *
         * \param row the row of the desired entry; this must be between
         * 0 and `rows()-1` inclusive.
         * \param column the column of the desired entry; this must be
         * between 0 and `columns()-1` inclusive.
         * \return a reference to the entry in the given row and column.
         */
        const T& entry(size_t row, size_t column) const {
            return data_[row * cols_ + column];
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
         * 0 and `rows()-1` inclusive.
         * \param column the column of the entry to set; this must be
         * between 0 and `columns()-1` inclusive.
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

            const T* pos = data_;
            for (size_t r = 0; r < rows_; r++)
                for (size_t c = 0; c < cols_; c++)
                    ans.data_[c * rows_ + r] = *pos++;

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
         * \param other the matrix to compare with this.
         * \return \c true if the matrices are equal as described above,
         * or \c false otherwise.
         */
        bool operator == (const Matrix& other) const
                requires std::equality_comparable<T> {
            return rows_ == other.rows_ && cols_ == other.cols_ &&
                std::equal(data_, data_ + rows_ * cols_, other.data_);
        }

        /**
         * Swaps the elements of the two given rows in the matrix.
         *
         * As of Regina 8.0, this operation is linear time (not constant time).
         *
         * If the optional argument \a fromCol is passed, then the
         * operation will only be performed for the elements from that
         * column to the rightmost end of each row (inclusive).
         *
         * \pre The two given rows are between 0 and `rows()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
         *
         * \param first the first row to swap.
         * \param second the second row to swap.
         * \param fromCol the starting point in each row from which the
         * operation will be performed.
         */
        void swapRows(size_t first, size_t second, size_t fromCol = 0) {
            if (first != second)
                std::swap_ranges(
                    data_ + first * cols_ + fromCol,
                    data_ + (first + 1) * cols_,
                    data_ + second * cols_ + fromCol);
        }
        /**
         * Swaps the elements of the two given columns in the matrix.
         *
         * Like swapRows(), this operation is linear time.
         *
         * If the optional argument \a fromRow is passed, then the
         * operation will only be performed for the elements from that
         * row down to the bottom of each column (inclusive).
         *
         * \pre The two given columns are between 0 and `columns()-1` inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() inclusive.
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
                ptrdiff_t gap = static_cast<ptrdiff_t>(second) -
                    static_cast<ptrdiff_t>(first);
                T* pos = data_ + fromRow * cols_ + first;
                for (size_t i = fromRow; i < rows_; ++i, pos += cols_)
                    swap(*pos, *(pos + gap));
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
            if (rows_ && cols_) {
                out << '[';
                const T* pos = data_;
                for (size_t r = 0; r < rows_; ++r) {
                    if (r > 0)
                        out << ' ';
                    out << '[';
                    for (size_t c = 0; c < cols_; ++c)
                        out << ' ' << *pos++;
                    out << " ]";
                }
                out << ']';
            } else
                out << "(empty " << rows_ << 'x' << cols_ << " matrix)";
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
            if (rows_ && cols_) {
                const T* pos = data_;
                for (size_t r = 0; r < rows_; r++) {
                    for (size_t c = 0; c < cols_; c++) {
                        if (c > 0) out << ' ';
                        out << *pos++;
                    }
                    out << '\n';
                }
            } else
                out << "(empty " << rows_ << 'x' << cols_ << " matrix)\n";
        }

        /**
         * Returns an identity matrix of the given size.
         * The matrix returned will have \a size rows and \a size columns.
         *
         * \param size the number of rows and columns of the matrix to build.
         * \return an identity matrix of the given size.
         */
        static Matrix identity(size_t size) requires Ring<T> {
            Matrix ans(size, size);
            if constexpr (! RingTraits<T>::zeroInitialised)
                ans.fill(RingTraits<T>::zero);
            T* pos = ans.data_;
            for (size_t i = 0; i < size; ++i, pos += (size + 1))
                *pos = RingTraits<T>::one;
            return ans;
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will have
         * `entry(r,c)` equal to 1 if `r == c` and 0 otherwise.
         */
        void makeIdentity() requires Ring<T> {
            fill(RingTraits<T>::zero);
            T* pos = data_;
            for (size_t i = 0; i < rows_ && i < cols_; ++i, pos += (cols_ + 1))
                *pos = RingTraits<T>::one;
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
         * \return \c true if and only if this is a square identity matrix.
         */
        bool isIdentity() const requires Ring<T> {
            if (rows_ != cols_)
                return false;

            const T* pos = data_;
            for (size_t r = 0; r < rows_; ++r)
                for (size_t c = 0; c < cols_; ++c)
                    if (r == c) {
                        if (*pos++ != RingTraits<T>::one)
                            return false;
                    } else {
                        if (*pos++ != RingTraits<T>::zero)
                            return false;
                    }

            return true;
        }

        /**
         * Determines whether this is the zero matrix.
         *
         * \return \c true if and only if all entries in the matrix are zero.
         */
        bool isZero() const requires Ring<T> {
            size_t size = rows_ * cols_;
            const T* pos = data_;
            for (size_t i = 0; i < size; ++i)
                if (*pos++ != RingTraits<T>::zero)
                    return false;
            return true;
        }

        /**
         * Adds the given source row to the given destination row.
         *
         * \pre The two given rows are distinct and between 0 and
         * `rows()-1` inclusive.
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
        void addRow(size_t source, size_t dest) requires Ring<T> {
            T* srcPos = data_ + source * cols_;
            T* destPos = data_ + dest * cols_;
            for (size_t i = 0; i < cols_; ++i)
                *destPos++ += *srcPos++;
        }
        /**
         * Adds a portion of the given source row to the given destination row.
         *
         * This is similar to addRow(), except that the operation will
         * only be performed for the elements from the column \a fromCol
         * to the rightmost end of the row (inclusive).
         *
         * \pre The two given rows are distinct and between 0 and
         * `rows()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
         *
         * \param source the row to add.
         * \param dest the row that will be added to.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void addRowFrom(size_t source, size_t dest, size_t fromCol)
                requires Ring<T> {
            T* srcPos = data_ + source * cols_ + fromCol;
            T* destPos = data_ + dest * cols_ + fromCol;
            for (size_t i = fromCol; i < cols_; ++i)
                *destPos++ += *srcPos++;
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
         * \pre The two given rows are distinct and between 0 and
         * `rows()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
         *
         * \param source the row to add.
         * \param dest the row that will be added to.
         * \param copies the number of copies of \a source to add to \a dest.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void addRow(size_t source, size_t dest, T copies, size_t fromCol = 0)
                requires Ring<T> {
            T* srcPos = data_ + source * cols_ + fromCol;
            T* destPos = data_ + dest * cols_ + fromCol;
            if constexpr (HasAddProduct<T>)
                for (size_t i = fromCol; i < cols_; ++i)
                    (destPos++)->addProduct(copies, *srcPos++);
            else
                for (size_t i = fromCol; i < cols_; ++i)
                    *destPos++ += copies * *srcPos++;
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
         * \pre The two given columns are distinct and between 0 and
         * `columns()-1` inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         */
        void addCol(size_t source, size_t dest) requires Ring<T> {
            ptrdiff_t gap = static_cast<ptrdiff_t>(source) -
                static_cast<ptrdiff_t>(dest);
            T* pos = data_ + dest;
            for (size_t i = 0; i < rows_; ++i, pos += cols_)
                *pos += *(pos + gap);
        }
        /**
         * Adds a portion of the given source column to the given destination
         * column.
         *
         * This is similar to addCol(), except that the operation will
         * only be performed for the elements from the row \a fromRow
         * down to the bottom of the column (inclusive).
         *
         * \pre The two given columns are distinct and between 0 and
         * `columns()-1` inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void addColFrom(size_t source, size_t dest, size_t fromRow = 0)
                requires Ring<T> {
            ptrdiff_t gap = static_cast<ptrdiff_t>(source) -
                static_cast<ptrdiff_t>(dest);
            T* pos = data_ + fromRow * cols_ + dest;
            for (size_t i = fromRow; i < rows_; ++i, pos += cols_)
                *pos += *(pos + gap);
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
         * \pre The two given columns are distinct and between 0 and
         * `columns()-1` inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() inclusive.
         *
         * \param source the columns to add.
         * \param dest the column that will be added to.
         * \param copies the number of copies of \a source to add to \a dest.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void addCol(size_t source, size_t dest, T copies, size_t fromRow = 0)
                requires Ring<T> {
            ptrdiff_t gap = static_cast<ptrdiff_t>(source) -
                static_cast<ptrdiff_t>(dest);
            T* pos = data_ + fromRow * cols_ + dest;
            if constexpr (HasAddProduct<T>)
                for (size_t i = fromRow; i < rows_; ++i, pos += cols_)
                    pos->addProduct(copies, *(pos + gap));
            else
                for (size_t i = fromRow; i < rows_; ++i, pos += cols_)
                    *pos += copies * *(pos + gap);
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
         * \pre The given row is between 0 and `rows()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
         *
         * \param row the row to work with.
         * \param factor the factor by which to multiply the given row.
         * \param fromCol the starting point in the row from which the
         * operation will be performed.
         */
        void multRow(size_t row, T factor, size_t fromCol = 0)
                requires Ring<T> {
            T* pos = data_ + row * cols_ + fromCol;
            for (size_t i = fromCol; i < cols_; ++i)
                *pos++ *= factor;
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
         * \pre The given column is between 0 and `columns()-1` inclusive.
         * \pre If passed, \a fromRow is between 0 and rows() inclusive.
         *
         * \param column the column to work with.
         * \param factor the factor by which to multiply the given column.
         * \param fromRow the starting point in the column from which the
         * operation will be performed.
         */
        void multCol(size_t column, T factor, size_t fromRow = 0)
                requires Ring<T> {
            T* pos = data_ + fromRow * cols_ + column;
            for (size_t i = fromRow; i < rows_; ++i, pos += cols_)
                *pos *= factor;
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
         * \pre The two given rows are distinct and between 0 and
         * `rows()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
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
                T coeff21, T coeff22, size_t fromCol = 0) requires Ring<T> {
            T* pos1 = data_ + row1 * cols_ + fromCol;
            T* pos2 = data_ + row2 * cols_ + fromCol;
            for (size_t i = fromCol; i < cols_; ++i, ++pos1, ++pos2) {
                T tmp = coeff11 * *pos1 + coeff12 * *pos2;
                *pos2 = coeff21 * *pos1 + coeff22 * *pos2;
                *pos1 = std::move(tmp);
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
         * \pre The two given columns are distinct and between 0 and
         * `columns()-1` inclusive.
         * \pre If passed, \a fromCol is between 0 and columns() inclusive.
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
                T coeff21, T coeff22, size_t fromRow = 0) requires Ring<T> {
            T* pos1 = data_ + fromRow * cols_ + col1;
            T* pos2 = data_ + fromRow * cols_ + col2;
            for (size_t i = fromRow; i < rows_;
                    ++i, pos1 += cols_, pos2 += cols_) {
                T tmp = coeff11 * *pos1 + coeff12 * *pos2;
                *pos2 = coeff21 * *pos1 + coeff22 * *pos2;
                *pos1 = std::move(tmp);
            }
        }

        /**
         * Adds the given matrix to this matrix in-place.
         *
         * \pre The given matrix has the same dimensions as this matrix.
         *
         * \param rhs the matrix to add to this.
         * \return a reference to this matrix.
         */
        Matrix& operator += (const Matrix& rhs) requires Ring<T> {
            size_t size = rows_ * cols_;
            const T* src = rhs.data_;
            T* dest = data_;
            for (size_t i = 0; i < size; ++i)
                *dest++ += *src++;
            return *this;
        }

        /**
         * Subtracts the given matrix from this matrix in-place.
         *
         * \pre The given matrix has the same dimensions as this matrix.
         *
         * \param rhs the matrix to subtract from this.
         * \return a reference to this matrix.
         */
        Matrix& operator -= (const Matrix& rhs) requires Ring<T> {
            size_t size = rows_ * cols_;
            const T* src = rhs.data_;
            T* dest = data_;
            for (size_t i = 0; i < size; ++i)
                *dest++ -= *src++;
            return *this;
        }

        /**
         * Multiplies this matrix by the given scalar and returns the result.
         *
         * This matrix and the given scalar may use different underlying types
         * (e.g., you can multiply a matrix of LargeInteger objects by a
         * native C++ integer).  The type of object that is stored in the
         * resulting matrix will be deduced accordingly (specifically, it will
         * be the type obtained by multiplying objects of types \a T and \a U
         * in that order using the binary multiplication operator).
         *
         * \python It is assumed that type \a U is the same as \a T.
         *
         * \param scalar the scalar to multiply this matrix by.
         * \return the matrix `this * scalar`.
         */
        template <typename U>
        requires requires(const T t, const U u) { { t * u }; }
        Matrix<decltype(T() * U())> operator * (const U& scalar) const& {
            Matrix<decltype(T() * U())> ans(rows_, cols_);
            size_t size = rows_ * cols_;
            const T* src = data_;
            T* dest = ans.data_;
            for (size_t i = 0; i < size; ++i)
                *dest++ = *src++ * scalar;
            return ans;
        }

        /**
         * Multiplies this matrix by the given scalar and returns the result.
         *
         * This matrix and the given scalar may use different underlying types.
         * However, be aware that (unlike the const version of this operator)
         * the resulting values will be remain as type \a T.  This means, for
         * example, that it is fine to multiply a matrix of LargeInteger
         * objects by a native C++ integer, but you should not try to multiply
         * a matrix of native C++ integers by a LargeInteger.
         *
         * \param scalar the scalar to multiply this matrix by.
         * \return the matrix `this * scalar`.
         */
        template <typename U>
        requires requires(T t, const U u) { { t *= u }; }
        Matrix operator * (const U& scalar) && {
            size_t size = rows_ * cols_;
            T* pos = data_;
            for (size_t i = 0; i < size; ++i)
                *pos++ *= scalar;
            return std::move(*this);
        }

        /**
         * Multiplies this matrix in-place by the given scalar.
         *
         * This matrix and the given scalar may use different underlying types
         * (e.g., you can multiply a matrix of LargeInteger objects by a
         * native C++ integer).  The type of object that is stored in the
         * resulting matrix will be deduced accordingly (specifically, it will
         * be the type obtained by multiplying objects of types \a T and \a U
         * in that order using the binary multiplication operator).
         *
         * \python It is assumed that type \a U is the same as \a T.
         *
         * \param scalar the scalar to multiply this matrix by.
         * \return a reference to this matrix.
         */
        template <typename U>
        requires requires(T t, const U u) { { t *= u }; }
        Matrix& operator *= (const U& scalar) {
            size_t size = rows_ * cols_;
            T* pos = data_;
            for (size_t i = 0; i < size; ++i)
                *pos++ *= scalar;
            return *this;
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
         * of types \a T and \a U in that order using the binary multiplication
         * operator).
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * \exception InvalidArgument The matrix dimensions are incompatible;
         * that is, `columns() ≠ rhs.rows()`.
         *
         * \param rhs the other matrix to multiply this matrix by.
         * \return the product matrix `this * rhs`.
         */
        template <typename U>
        Matrix<decltype(T() * U())> operator * (const Matrix<U>& rhs) const
                requires Ring<T> && Ring<U> && Ring<decltype(T() * U())> {
            if (cols_ != rhs.rows_)
                throw InvalidArgument("Incompatible matrix dimensions");

            using Ans = decltype(T() * U());
            Matrix<Ans> ans(rows_, rhs.cols_);

            T* dest = ans.data_;
            for (size_t row = 0; row < rows_; ++row)
                for (size_t col = 0; col < rhs.cols_; ++col) {
                    if constexpr (! RingTraits<Ans>::zeroInitialised)
                        *dest = RingTraits<Ans>::zero;
                    const T* lhsPos = data_ + row * cols_;
                    const T* rhsPos = rhs.data_ + col;
                    for (size_t k = 0; k < cols_;
                            ++k, ++lhsPos, rhsPos += rhs.cols_)
                        if constexpr (HasAddProduct<Ans>) {
                            dest->addProduct(*lhsPos, *rhsPos);
                        } else {
                            if (*lhsPos != RingTraits<T>::zero &&
                                    *rhsPos != RingTraits<U>::zero)
                                *dest += (*lhsPos * *rhsPos);
                        }
                    ++dest;
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
         * \pre The length of the given vector is precisely the number of
         * columns in this matrix.
         *
         * \exception InvalidArgument The matrix and vector dimensions are
         * incompatible; that is, `columns() ≠ rhs.size()`.
         *
         * \param rhs the vector to multiply this matrix by.
         * \return the product `this * rhs`, which will be a
         * vector whose length is the number of rows in this matrix.
         */
        template <typename U>
        Vector<decltype(T() * U())> operator * (const Vector<U>& rhs) const
                requires Ring<T> && Ring<U> && Ring<decltype(T() * U())> {
            if (cols_ != rhs.size())
                throw InvalidArgument("Incompatible matrix/vector dimensions");

            using Ans = decltype(T() * U());
            Vector<Ans> ans(rows_);

            const T* lhsPos = data_;
            for (auto& dest : ans) {
                if constexpr (! RingTraits<Ans>::zeroInitialised)
                    dest = RingTraits<Ans>::zero;
                for (const auto& rhsElt : rhs) {
                    if constexpr (HasAddProduct<Ans>) {
                        dest.addProduct(*lhsPos++, rhsElt);
                    } else {
                        if (*lhsPos != RingTraits<T>::zero &&
                                rhsElt != RingTraits<U>::zero)
                            dest += *lhsPos * rhsElt;
                        ++lhsPos;
                    }
                }
            }

            return ans;
        }

        /**
         * Returns the trace of this matrix.  The trace is simply the sum of
         * the elements along the main diagonal.
         *
         * The trace of an empty matrix (of size `0×0`) will be zero.
         *
         * \pre This is a square matrix.
         *
         * \exception FailedPrecondition This matrix is not square.
         *
         * \return the trace of this matrix.
         */
        T trace() const requires Ring<T> {
            if (rows_ != cols_)
                throw FailedPrecondition("The trace can only be computed for "
                    "a square matrix");

            T ans;
            if constexpr (! RingTraits<T>::zeroInitialised)
                ans = RingTraits<T>::zero;
            const T* pos = data_;
            for (size_t i = 0; i < rows_; ++i, pos += (rows_ + 1))
                ans += *pos;
            return ans;
        }

        /**
         * Evaluates the determinant of the matrix.
         *
         * The determinant of an empty matrix (of size `0×0`) will be 1.
         *
         * \pre This is a square matrix.
         *
         * \exception FailedPrecondition This matrix is not square.
         *
         * \param alg the algorithm to use for this computation.  If you do
         * not specify an algorithm, Regina will choose a sensible default.
         * \return the determinant of this matrix.
         */
        T det(AdjugateAlgorithm alg = AdjugateAlgorithm::Default) const
                requires IntegralDomain<T> {
            // Note: we requires an integral domain because the
            // Faddeev-Leverrier algorithm involves exact integer division.

            switch (alg) {
                case AdjugateAlgorithm::Default:
                case AdjugateAlgorithm::MahajanVinay:
                {
                    if (rows_ != cols_)
                        throw FailedPrecondition("Determinants can only be "
                            "computed for square matrices");
                    if (rows_ == 0)
                        return RingTraits<T>::one;

                    // Partial computations - each array describes an
                    // upper-diagonal matrix:
                    FixedArray<T> part[2] { rows_ * rows_, rows_ * rows_ };
                    size_t head, r;

                    // Treat the smallest cases of len = 1 separately.
                    // Here we just make part[0] the identity matrix.
                    typename FixedArray<T>::iterator pit = part[0].begin();
                    for (head = 0; head < rows_; ++head) {
                        // Currently pit points to the beginning of row #head.
                        // Put a one in the main diagonal.
                        pit += head;
                        *pit = RingTraits<T>::one;
                        // Fill the rest of the row to the right with zeroes.
                        std::fill(pit + 1, pit + (rows_ - head),
                            RingTraits<T>::zero);
                        pit += (rows_ - head);
                    }

                    // Work up through incrementing values of len.
                    int layer = 0; // always 0 or 1
                    typename FixedArray<T>::iterator pdiag;
                    const T* dit;
                    for (size_t len = 2; len <= rows_; ++len) {
                        layer ^= 1;
                        pit = part[layer].begin();
                        for (head = 0; head < rows_; ++head) {
                            pit += head;
                            // Now pit points to the diagonal on row #head.
                            // At this diagonal element we need to open a new
                            // clow, which requires computing many (negative)
                            // inner products of corresponding rows of
                            // part[layer ^ 1] and this matrix.
                            *pit = RingTraits<T>::zero;
                            for (r = 0, pdiag = part[layer ^ 1].begin(),
                                        dit = data_;
                                    r < head;
                                    ++r, dit += (rows_ + 1),
                                        pdiag += (rows_ + 1)) {
                                // Here dit and pdiag both point to the main
                                // diagonal on row #r.
                                *pit -= innerProduct(pdiag, dit, rows_ - r);
                            }

                            // Now walk pit along the row to the right, and at
                            // each position continue an existing clow.
                            // Right now, pdiag and dit both point to their
                            // respective diagonal element on row #head.
                            // We will leave pdiag fixed, and start stepping
                            // dit down in its current column.
                            for (r = head + 1, ++pit, dit += rows_;
                                    r < rows_; ++r, ++pit, dit += rows_)
                                *pit = innerProduct(pdiag, dit, rows_ - head);
                        }
                    }

                    // All done.  Sum up the determinant, which is now the
                    // inner product of _everything_ in part[layer] with this
                    // matrix.
                    T ans;
                    if constexpr (! RingTraits<T>::zeroInitialised)
                        ans = RingTraits<T>::zero;
                    for (r = 0, pit = part[layer].begin(), dit = data_;
                            r < rows_;
                            ++r, pit += (rows_ + 1), dit += (rows_ + 1)) {
                        // Here pit and dit point to their corresponding
                        // diagonal elements in row #r.
                        ans += innerProduct(pit, dit, rows_ - r);
                    }

                    return (rows_ % 2 == 0 ? -ans : ans);
                }
                default:
                    return adjugate(alg).second;
            }
        }

        /**
         * Computes both the adjugate matrix and the determinant of this
         * matrix.  The adjugate `adj` and the determinant `det` of a square
         * matrix `M` satisfy the relation `M * adj = adj * M = det * I`,
         * where `I` is the identity matrix of the same size.
         *
         * For an empty matrix (of size `0×0`), the adjugate will likewise be
         * empty, and the determinant will be 1.
         *
         * \pre This is a square matrix.
         *
         * \exception FailedPrecondition This matrix is not square.
         * \exception InvalidArgument The argument \a alg is an algorithm that
         * does not support adjugates (such as AdjugateAlgorithm::MahajanVinay).
         *
         * \param alg the algorithm to use for this computation.  If you do
         * not specify an algorithm, Regina will choose a sensible default.
         * \return a pair containing the adjugate matrix and the determinant.
         */
        std::pair<Matrix, T> adjugate(
                AdjugateAlgorithm alg = AdjugateAlgorithm::Default) const
                requires IntegralDomain<T> {
            if (rows_ != cols_)
                throw FailedPrecondition("The adjugate can only be "
                    "computed for a square matrix");
            if (rows_ == 0)
                return { 0 /* empty matrix */, RingTraits<T>::one };
            if (rows_ == 1)
                return { Matrix::identity(1), *data_ };

            switch (alg) {
                case AdjugateAlgorithm::Default:
                case AdjugateAlgorithm::FaddeevLeverrier:
                {
                    // We essentially follow Johansonn's formulation
                    // (https://inria.hal.science/hal-03016034v3, Algorithm 1),
                    // but we do not keep the coefficients of the
                    // characteristic polynomial.
                    Matrix b;
                    for (size_t k = 1; k < rows_; ++k) {
                        if (k == 1)
                            b = *this;
                        else
                            b = (*this) * b;

                        auto c = b.trace() / k;
                        // The following loop implements B -= c * I.
                        auto pos = b.data_;
                        for (size_t i = 0; i < rows_; ++i, pos += (rows_ + 1))
                            *pos -= c;
                    }

                    auto c = trace((*this), b) / rows_;
                    if (rows_ % 2 == 0) {
                        b.negate();
                        if constexpr (Negatable<T>)
                            c.negate();
                        else
                            c = -c;
                    }
                    return { std::move(b), std::move(c) };
                }
                case AdjugateAlgorithm::PreparataSarwate:
                {
                    // Again we essentially follow Johansonn's formulation
                    // (https://inria.hal.science/hal-03016034v3, Algorithm 2),
                    // but we do not keep the coefficients of the
                    // characteristic polynomial.
                    size_t m = isqrt(rows_);

                    FixedArray<Matrix> powA(m);
                    powA.front() = *this;
                    for (size_t i = 1; i < m; ++i)
                        powA[i] = (*this) * powA[i - 1];
                    FixedArray<T> traceA(m);
                    for (size_t i = 0; i < m; ++i)
                        traceA[i] = powA[i].trace();

                    FixedArray<T> charSlice(m);
                    Matrix b = identity(rows_);
                    size_t k = 1;

                    while (k < rows_) {
                        if (rows_ - k < m)
                            m = rows_ - k;
                        charSlice.front() = -trace((*this), b) / k;
                        for (size_t j = 1; j < m; ++j) {
                            charSlice[j] = trace(powA[j], b);
                            for (size_t i = 0; i < j; ++i)
                                if constexpr (HasAddProduct<T>)
                                    charSlice[j].addProduct(
                                        traceA[j - i - 1], charSlice[i]);
                                else
                                    charSlice[j] +=
                                        traceA[j - i - 1] * charSlice[i];
                            if constexpr (Negatable<T>) {
                                charSlice[j] /= (k + j);
                                charSlice[j].negate();
                            } else {
                                charSlice[j] = -charSlice[j] / (k + j);
                            }
                        }

                        // Note: when working with large types T (e.g., Laurent
                        // polynomials) and when the initial matrix is sparse
                        // (e.g., when computing Alexander polynomials or theta
                        // invariants), _this_ computation is the bottleneck
                        // (presumably because neither A^m nor B is sparse).
                        b = powA[m - 1] * b;

                        for (size_t j = 0; j < m - 1; ++j) {
                            auto bit = b.data_;
                            auto ait = powA[m - 2 - j].data_;
                            for (size_t row = 0; row < rows_; ++row)
                                for (size_t col = 0; col < cols_; ++col)
                                    if constexpr (HasAddProduct<T>)
                                        (bit++)->addProduct(*ait++,
                                            charSlice[j]);
                                    else
                                        *bit++ += *ait++ * charSlice[j];
                        }

                        // The following loop implements
                        // B += charSlice[m - 1] * I.
                        auto pos = b.data_;
                        for (size_t i = 0; i < rows_; ++i, pos += (rows_ + 1))
                            *pos += charSlice[m - 1];

                        k += m;
                    }

                    auto c = trace((*this), b) / rows_;
                    if (rows_ % 2 == 0) {
                        b.negate();
                        if constexpr (Negatable<T>)
                            c.negate();
                        else
                            c = -c;
                    }
                    return { std::move(b), std::move(c) };
                }
                case AdjugateAlgorithm::MahajanVinay:
                    throw InvalidArgument("The Mahajan-Vinay algorithm can "
                        "only be used for computing determinants, not adjugate "
                        "matrices");
            }
        }

        /**
         * Negates every entry in this matrix.
         */
        void negate() requires Ring<T> {
            size_t size = rows_ * cols_;
            T* pos = data_;
            for (size_t i = 0; i < size; ++i, ++pos)
                if constexpr (Negatable<T>)
                    pos->negate();
                else
                    *pos = -*pos;
        }

        /**
         * Negates all elements in the given row.
         *
         * \pre The given row number is between 0 and `rows()-1` inclusive.
         *
         * \param row the index of the row whose elements should be negated.
         */
        void negateRow(size_t row) requires Ring<T> {
            T* pos = data_ + row * cols_;
            for (size_t i = 0; i < cols_; ++i, ++pos)
                if constexpr (Negatable<T>)
                    pos->negate();
                else
                    *pos = -*pos;
        }

        /**
         * Negates all elements in the given column.
         *
         * \pre The given column number is between 0 and `columns()-1`
         * inclusive.
         *
         * \param col the index of the column whose elements should be negated.
         */
        void negateCol(size_t col) requires Ring<T> {
            T* pos = data_ + col;
            for (size_t i = 0; i < rows_; ++i, pos += cols_)
                if constexpr (Negatable<T>)
                    pos->negate();
                else
                    *pos = -*pos;
        }

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
         * \pre The given row number is between 0 and `rows()-1` inclusive.
         *
         * \param row the index of the row whose elements should be
         * divided by \a divBy.
         * \param divBy the integer to divide each row element by.
         */
        void divRowExact(size_t row, const T& divBy) requires ReginaInteger<T> {
            T* pos = data_ + row * cols_;
            for (size_t i = 0; i < cols_; ++i)
                (pos++)->divByExact(divBy);
        }

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
         * \pre The given column number is between 0 and `columns()-1`
         * inclusive.
         *
         * \param col the index of the column whose elements should be
         * divided by \a divBy.
         * \param divBy the integer to divide each column element by.
         */
        void divColExact(size_t col, const T& divBy) requires ReginaInteger<T> {
            T* pos = data_ + col;
            for (size_t i = 0; i < rows_; ++i, pos += cols_)
                pos->divByExact(divBy);
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given row.  The value returned is guaranteed to be non-negative.
         *
         * If this is a `k×0` matrix (i.e., the given row is empty), then
         * the return value will be zero.
         *
         * \pre The given row number is between 0 and `rows()-1` inclusive.
         *
         * \param row the index of the row whose gcd should be computed.
         * \return the greatest common divisor of all elements of this row.
         */
        T gcdRow(size_t row) requires ReginaInteger<T> {
            if (cols_ == 0)
                return {}; // zero

            T* pos = data_ + row * cols_;
            T gcd = *pos;
            for (size_t i = 1; i < cols_ && gcd != 1 && gcd != -1; ++i)
                gcd = gcd.gcd(*++pos); // advance pos, _then_ read

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given column.  The value returned is guaranteed to be non-negative.
         *
         * If this is a `0×k` matrix (i.e., the given column is empty), then
         * the return value will be zero.
         *
         * \pre The given column number is between 0 and `columns()-1`
         * inclusive.
         *
         * \param col the index of the column whose gcd should be computed.
         * \return the greatest common divisor of all elements of this column.
         */
        T gcdCol(size_t col) requires ReginaInteger<T> {
            if (rows_ == 0)
                return {}; // zero

            T* pos = data_ + col;
            T gcd = *pos;
            for (size_t i = 1; i < rows_ && gcd != 1 && gcd != -1; ++i)
                gcd = gcd.gcd(*(pos += cols_)); // advance pos, _then_ read

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Reduces the given row by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the row is
         * changed at all, it will be divided by a _positive_ integer.
         *
         * \pre The given row number is between 0 and `rows()-1` inclusive.
         *
         * \param row the index of the row to reduce.
         */
        void reduceRow(size_t row) requires ReginaInteger<T> {
            T gcd = gcdRow(row);
            if (gcd != 0 && gcd != 1)
                divRowExact(row, gcd);
        }

        /**
         * Reduces the given column by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the column is
         * changed at all, it will be divided by a _positive_ integer.
         *
         * \pre The given column number is between 0 and `columns()-1`
         * inclusive.
         *
         * \param col the index of the column to reduce.
         */
        void reduceCol(size_t col) requires ReginaInteger<T> {
            T gcd = gcdCol(col);
            if (gcd != 0 && gcd != 1)
                divColExact(col, gcd);
        }

        /**
         * Transforms this matrix into row echelon form.  The transformation
         * will perform only row operations.
         *
         * This matrix will be modified directly, and its size will not change
         * (even if the rank is less than the number of rows).
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
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the rank of this matrix, i.e., the number of non-zero rows
         * remaining.
         */
        size_t makeRowEchelon() requires ReginaInteger<T>;

        /**
         * Transforms this matrix into column echelon form.  The transformation
         * will perform only column operations.
         *
         * This matrix will be modified directly, and its size will not change
         * (even if the rank is less than the number of columns).
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
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the rank of this matrix, i.e., the number of non-zero
         * columns remaining.
         */
        size_t makeColumnEchelon() requires ReginaInteger<T>;

        /**
         * Returns the row echelon form of this matrix along with the rank.
         * This matrix itself will not be changed.
         *
         * The row echelon form will have the same dimensions as this matrix,
         * and will be obtained from this matrix by performing only row
         * operations.
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
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a pair consisting of the row echelon form \a E, and the
         * rank of this matrix (i.e., the number of non-zero rows in \a E).
         */
        std::pair<Matrix, size_t> rowEchelon() const&
                requires ReginaInteger<T> {
            Matrix ans(*this);
            size_t rank = ans.makeRowEchelon();
            return { std::move(ans), rank };
        }

        /**
         * Returns the row echelon form of this matrix along with the rank.
         *
         * This rvalue variant of rowEchelon() is faster than the const
         * variant, because it reduces this matrix directly instead of
         * working on a deep copy.
         *
         * See the const version of rowEchelon() for further information on
         * row echelon form.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a pair consisting of the row echelon form \a E, and the
         * rank of this matrix (i.e., the number of non-zero rows in \a E).
         */
        std::pair<Matrix, size_t> rowEchelon() && requires ReginaInteger<T> {
            size_t rank = makeRowEchelon();
            return { std::move(*this), rank };
        }

        /**
         * Returns the column echelon form of this matrix along with the rank.
         * This matrix itself will not be changed.
         *
         * The column echelon form will have the same dimensions as this matrix,
         * and will be obtained from this matrix by performing only column
         * operations.
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
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a pair consisting of the column echelon form \a E, and the
         * rank of this matrix (i.e., the number of non-zero columns in \a E).
         */
        std::pair<Matrix, size_t> columnEchelon() const&
                requires ReginaInteger<T> {
            Matrix ans(*this);
            size_t rank = ans.makeColumnEchelon();
            return { std::move(ans), rank };
        }

        /**
         * Returns the column echelon form of this matrix along with the rank.
         *
         * This rvalue variant of columnEchelon() is faster than the const
         * variant, because it reduces this matrix directly instead of
         * working on a deep copy.
         *
         * See the const version of columnEchelon() for further information on
         * column echelon form.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a pair consisting of the column echelon form \a E, and the
         * rank of this matrix (i.e., the number of non-zero columns in \a E).
         */
        std::pair<Matrix, size_t> columnEchelon() && requires ReginaInteger<T> {
            size_t rank = makeColumnEchelon();
            return { std::move(*this), rank };
        }

        /**
         * Returns the rank of this matrix.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the rank of this matrix.
         */
        size_t rank() const& requires ReginaInteger<T> {
            // Make a deep copy, which we can modify as we compute rank.
            return Matrix(*this).makeRowEchelon();
        }

        /**
         * Returns the rank of this matrix.
         *
         * A typical rank computation involves modifying the matrix (e.g., by
         * converting it to row echelon form).  This rvalue variant of rank()
         * is faster than the const variant, because it modifies this matrix
         * directly instead of working on a temporary copy.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the rank of this matrix.
         */
        size_t rank() && requires ReginaInteger<T> {
            return makeRowEchelon();
        }

        /**
         * Rearranges and/or deletes rows of this matrix so that whatever rows
         * remain form a basis of the original row space.  The main purpose of
         * this routine is to provide a way to remove redundant equations from
         * a system of linear equations.
         *
         * This routine may reorder rows and/or remove rows, but will perform no
         * other operations.  As a result, this routine _could_ change the
         * dimensions of the matrix.  After this routine returns, the rows of
         * this matrix will be linearly independent, and will together form a
         * basis (over the rationals) of the row space of the original matrix.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         */
        void makeRowBasis() requires ReginaInteger<T>;

        /**
         * Returns a subset of rows of this matrix which together form a basis
         * of the row space of this matrix.  The main purpose of this routine is
         * to provide a way to remove redundant equations from a system of
         * linear equations.
         *
         * Specifically: let \a B be the matrix that is returned.  Then the
         * rows of \a B will be a subset of the rows of this matrix, and may
         * possibly appear in a different order.  In particular, \a B might have
         * fewer rows than this matrix.  The rows of \a B will be linearly
         * independent, and will together form a basis (over the rationals) of
         * the row space of this matrix.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a basis of the row space of this matrix.
         */
        Matrix rowBasis() const& requires ReginaInteger<T> {
            Matrix ans(*this);
            ans.makeRowBasis();
            return ans;
        }

        /**
         * Returns a subset of rows of this matrix which together form a basis
         * of the row space of this matrix.  The main purpose of this routine is
         * to provide a way to remove redundant equations from a system of
         * linear equations.
         *
         * This rvalue variant of rowBasis() is faster than the const variant,
         * because it operates on this matrix directly instead of working on a
         * deep copy.
         *
         * See the const version of rowBasis() for further information on row
         * bases.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return a basis of the row space of this matrix.
         */
        Matrix rowBasis() && requires ReginaInteger<T> {
            makeRowBasis();
            return std::move(*this);
        }

        /**
         * Returns a subset of rows of this matrix which together form a basis
         * of its row space, along with an "incremental" basis for its
         * orthogonal complement.  This routine can help with larger procedures
         * that need to build up a row space and simultaneously cut down the
         * complement one dimension at a time.
         *
         * Specifically, this routine returns two matrices `(B, C)` as follows:
         *
         * - The matrix \a B will have dimensions `rank() × columns()`, and
         *   the matrix \a C will have dimensions `columns() × columns()`.
         *
         * - The rows of \a B will be a subset of the rows of this matrix, and
         *   may possibly appear in a different order.  In particular, \a B might
         *   have fewer rows than this matrix.
         *
         * - The rows of \a B will be linearly independent, and will together
         *   form a basis (over the rationals) of the row space of this matrix.
         *
         * - For each \a i between 0 and `rank()` inclusive, the final
         *   `(columns() - i)` rows of \a C will form a basis for the orthogonal
         *   complement of the first \a i rows of \a B.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the basis of the row space and the "incremental" basis for
         * its orthogonal complement, as described above.
         */
        std::pair<Matrix, Matrix> rowBasisAndOrthComp() const
                requires ReginaInteger<T>;

        /**
         * Computes the inverse of this matrix, if this exists as an integer
         * matrix.
         *
         * Currently this is only implemented for integer matrices.  If this
         * matrix does not have an inverse, or if the inverse is not an integer
         * matrix, then this routine will throw an exception.
         *
         * If your matrix might not have an integer inverse, or if you are
         * working over some other ring or field, you might wish to consider
         * calling adjugate() instead.
         *
         * \pre This is a square matrix.
         * \pre All entries in this matrix are finite.
         *
         * \exception FailedPrecondition This matrix is not square.
         * \exception NoSolution This matrix is not invertible over the
         * integers.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the inverse matrix.
         */
        Matrix inverse() const& requires ReginaInteger<T> {
            // Create a temporary copy and use the rvalue variant of inverse(),
            // which will reduce that copy in-place.
            return Matrix(*this).inverse();
        }

        /**
         * Computes the inverse of this matrix, if this exists as an integer
         * matrix.
         *
         * Currently this is only implemented for integer matrices.  If this
         * matrix does not have an inverse, or if the inverse is not an integer
         * matrix, then this routine will throw an exception.
         *
         * If your matrix might not have an integer inverse, or if you are
         * working over some other ring or field, you might wish to consider
         * calling adjugate() instead.
         *
         * A typical inverse computation involves modifying the matrix (e.g., by
         * converting it to row/column echelon form).  This rvalue variant of
         * inverse() is faster than the const variant, because it modifies this
         * matrix directly instead of working on a temporary copy.
         *
         * \pre This is a square matrix.
         * \pre All entries in this matrix are finite.
         *
         * \exception FailedPrecondition This matrix is not square.
         * \exception NoSolution This matrix is not invertible over the
         * integers.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the inverse matrix.
         */
        Matrix inverse() && requires ReginaInteger<T>;

        /**
         * Transforms this matrix into Smith normal form.
         *
         * Reading down the main diagonal, the final Smith normal form will
         * have a series of non-negative, non-decreasing invariant factors
         * followed by zeroes.  "Invariant factor" refers to the convention
         * that the `i`th term divides the (`i+1`)th term, and so these
         * invariant factors are unique.
         *
         * This routine will use Regina's default Smith normal form algorithm.
         * If you wish to use a different algorithm, you will need to call
         * makeSmithCoB() instead.
         *
         * This matrix need not be square and need not be of full rank.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         */
        void makeSmith() requires ReginaInteger<T>;

        /**
         * Returns the Smith normal form of this matrix.  This matrix itself
         * will not be changed.
         *
         * Reading down the main diagonal, the final Smith normal form will
         * have a series of non-negative, non-decreasing invariant factors
         * followed by zeroes.  "Invariant factor" refers to the convention
         * that the `i`th term divides the (`i+1`)th term, and so these
         * invariant factors are unique.
         *
         * This routine will use Regina's default Smith normal form algorithm.
         * If you wish to use a different algorithm, you will need to call
         * smithCoB() instead.
         *
         * This matrix need not be square and need not be of full rank.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the Smith normal form of this matrix.
         */
        Matrix smith() const& requires ReginaInteger<T> {
            Matrix ans(*this);
            ans.makeSmith();
            return ans;
        }

        /**
         * Returns the Smith normal form of this matrix.
         *
         * This rvalue variant of smith() is faster than the const variant,
         * because it reduces this matrix directly instead of working on a
         * deep copy.
         *
         * See the const version of smith() for further information on
         * Smith normal forms.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \return the Smith normal form of this matrix.
         */
        Matrix smith() && requires ReginaInteger<T> {
            makeSmith();
            return std::move(*this);
        }

        /**
         * Transforms this matrix into Smith normal form and returns
         * change-of-basis matrices.
         *
         * This routine is similar to makeSmith(); however, it allows you to
         * specify a custom algorithm, and it also computes and returns
         * change-of-basis matrices describing all of the row and column
         * operations that were performed.
         *
         * As with makeSmith(), the final Smith normal form will have a main
         * diagonal consisting of a series of non-negative, non-decreasing
         * invariant factors followed by zeroes.  "Invariant factor" refers to
         * the convention that the `i`th term divides the (`i+1`)th term,
         * and so these invariant factors are unique.
         *
         * The change-of-basis matrices work as follows.  Let \a M denote the
         * initial value of this matrix, let \a S denote its Smith normal form,
         * and let the returned array be `(rowSpaceBasis, rowSpaceBasisInv,
         * colSpaceBasis, colSpaceBasisInv)`.  Then:
         *
         * - this matrix will be converted from \a M into \a S;
         * - \a colSpaceBasis will be square with side length `rows()`, and
         *   \a colSpaceBasisInv will be its inverse matrix;
         * - \a rowSpaceBasis will be square with side length `columns()`, and
         *   \a rowSpaceBasisInv will be its inverse matrix;
         * - `S = colSpaceBasis * M * rowSpaceBasis`; and
         * - `M = colSpaceBasisInv * S * rowSpaceBasisInv`.
         *
         * This matrix need not be square and need not be of full rank.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \param alg the algorithm to use for this computation.  If you do
         * not specify an algorithm, Regina will choose a sensible default.
         * \return the four change-of-basis matrices `(rowSpaceBasis,
         * rowSpaceBasisInv, colSpaceBasis, colSpaceBasisInv)`, as described
         * above.
         *
         * \author B.B. and Ryan Budney
         */
        std::array<Matrix, 4> makeSmithCoB(
                SNFAlgorithm alg = SNFAlgorithm::Default)
                requires ReginaInteger<T> {
            switch (alg) {
                case SNFAlgorithm::Default:
                case SNFAlgorithm::HafnerMcCurley:
                    return makeSmithHafnerMcCurley();
                case SNFAlgorithm::Metrical:
                    return makeSmithMetrical();
            }
        }

        /**
         * Returns the Smith normal form of this matrix along with
         * change-of-basis matrices.  This matrix itself will not be changed.
         *
         * This routine is similar to smith(); however, it allows you to
         * specify a custom algorithm, and it also computes and returns
         * change-of-basis matrices describing all of the row and column
         * operations that were performed.
         *
         * As with smith(), the final Smith normal form will have a main
         * diagonal consisting of a series of non-negative, non-decreasing
         * invariant factors followed by zeroes.  "Invariant factor" refers to
         * the convention that the `i`th term divides the (`i+1`)th term,
         * and so these invariant factors are unique.
         *
         * The change-of-basis matrices work as follows.  Let \a M denote the
         * initial value of this matrix, and let the returned array be
         * `(S, rowSpaceBasis, rowSpaceBasisInv, colSpaceBasis,
         * colSpaceBasisInv)`.  Then:
         *
         * - \a S will be the Smith normal form of the original matrix \a M;
         * - \a colSpaceBasis will be square with side length `rows()`, and
         *   \a colSpaceBasisInv will be its inverse matrix;
         * - \a rowSpaceBasis will be square with side length `columns()`, and
         *   \a rowSpaceBasisInv will be its inverse matrix;
         * - `S = colSpaceBasis * M * rowSpaceBasis`; and
         * - `M = colSpaceBasisInv * S * rowSpaceBasisInv`.
         *
         * This matrix need not be square and need not be of full rank.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \param alg the algorithm to use for this computation.  If you do
         * not specify an algorithm, Regina will choose a sensible default.
         * \return the Smith normal form and the four change-of-basis matrices;
         * that is, the tuple `(S, rowSpaceBasis, rowSpaceBasisInv,
         * colSpaceBasis, colSpaceBasisInv)`, as described above.
         */
        std::array<Matrix, 5> smithCoB(SNFAlgorithm alg = SNFAlgorithm::Default)
                const& requires ReginaInteger<T> {
            Matrix ans(*this);
            auto cob = ans.makeSmithCoB(alg);
            return { std::move(ans), std::move(cob[0]), std::move(cob[1]),
                std::move(cob[2]), std::move(cob[3]) };
        }

        /**
         * Returns the Smith normal form of this matrix along with
         * change-of-basis matrices.
         *
         * This rvalue variant of smithCoB() is faster than the const variant,
         * because it reduces this matrix directly instead of working on a
         * deep copy.
         *
         * See the const version of smithCoB() for further information on
         * Smith normal forms and change-of-basis matrices.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \param alg the algorithm to use for this computation.  If you do
         * not specify an algorithm, Regina will choose a sensible default.
         * \return the Smith normal form \a S and the four change-of-basis
         * matrices; that is, the tuple `(S, rowSpaceBasis, rowSpaceBasisInv,
         * colSpaceBasis, colSpaceBasisInv)`.
         */
        std::array<Matrix, 5> smithCoB(SNFAlgorithm alg = SNFAlgorithm::Default)
                && requires ReginaInteger<T> {
            auto cob = makeSmithCoB(alg);
            return { std::move(*this), std::move(cob[0]), std::move(cob[1]),
                std::move(cob[2]), std::move(cob[3]) };
        }

        /**
         * Treats this matrix as a homomorphism from `Z^n` to `Z^k`, and computes
         * the preimage of a given sublattice of `Z^k` under this homomorphism.
         *
         * Suppose this matrix (which describes the homomorphism from
         * `Z^n` to `Z^k`) has dimensions `k × n`.  The sublattice is then of
         * the form `(p1 Z) * (p2 Z) * ... * (pk Z)`, where the non-negative
         * integers `p1,...,pk` are passed in the list \a sublattice.
         *
         * The preimage of the sublattice is some rank \a n lattice in `Z^n`.
         * This routine computes a basis for that lattice.
         *
         * An equivalent problem is to treat this matrix as a homomorphism from
         * `Z^n` to `Z_p1 + ... + Z_pk`; this routine then computes the kernel
         * of this homomorphism.
         *
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         * However, typical end users will not need this extra header, since
         * Regina's calculation engine already includes explicit instantiations
         * for `Matrix<Integer>`, which is where this routine is most commonly
         * used.
         *
         * \exception InvalidArgument The length of \a sublattice is different
         * from the number of rows in this matrix.  Note that the _contents_ of
         * \a sublattice (specifically, the signs of the integers it contains)
         * are not checked.
         *
         * \param sublattice a list of length rows() describing the sublattice
         * of `Z^k`; the elements of this list must be the non-negative integers
         * `p1,...,pk` as described above.
         * \return a matrix whose columns are a basis for the preimage lattice.
         * The number of rows of the returned matrix will be the same as the
         * number of _columns_ of this input matrix.
         *
         * \author Ryan Budney
         */
        Matrix preimageOfLattice(const std::vector<T>& sublattice) const
                requires ReginaInteger<T>;

        /**
         * A diagnostic routine that ensures that the internal representation
         * of this matrix is valid.
         *
         * If Regina is working correctly, this routine should do nothing.
         * If the internal representation of this matrix is _not_ valid,
         * this routine will throw an exception.
         *
         * This routine is provided for use within Regina's various test suites.
         * End users should not need to call it at all.
         *
         * \exception ImpossibleScenario The internal state of this matrix is
         * invalid.
         */
        void validate() const {
            if (rows_ > 0 && cols_ > 0 && ! data_)
                throw ImpossibleScenario("Matrix has positive dimensions "
                    "but a null element array");
        }

    private:
        /**
         * Returns the trace of the matrix product `lhs * rhs`.
         *
         * This routine is faster than computing `(lhs * rhs).trace()`, since
         * it only needs to evaluate the product along the main diagonal.
         *
         * \pre The dimensions of \a lhs are the same as the dimensions of the
         * transpose of \a rhs.  In other words, `lhs * rhs` is well-defined
         * and is a square matrix.
         *
         * \return the trace of `lhs * rhs`.
         */
        static T trace(const Matrix& lhs, const Matrix& rhs) requires Ring<T> {
            T ans;
            if constexpr (! RingTraits<T>::zeroInitialised)
                ans = RingTraits<T>::zero;
            const T* lhsPos = lhs.data_;
            for (size_t i = 0; i < lhs.rows_; ++i) {
                const T* rhsPos = rhs.data_ + i;
                for (size_t j = 0; j < lhs.cols_; ++j, rhsPos += rhs.cols_)
                    if constexpr (HasAddProduct<T>)
                        ans.addProduct(*lhsPos++, *rhsPos);
                    else
                        ans += *lhsPos++ * *rhsPos;
            }
            return ans;
        }

        /**
         * Returns the inner product of two vectors, given by a pair of
         * starting iterators and a vector length.
         */
        template <RandomAccessIteratorFor<T> Iterator1,
            RandomAccessIteratorFor<T> Iterator2>
        static T innerProduct(Iterator1 it1, Iterator2 it2, size_t len)
                requires Ring<T> {
            T ans;
            if constexpr (! RingTraits<T>::zeroInitialised)
                ans = RingTraits<T>::zero;

            Iterator1 end1 = it1 + len;
            while (it1 != end1)
                if constexpr (HasAddProduct<T>)
                    ans.addProduct(*it1++, *it2++);
                else
                    ans += *it1++ * *it2++;
            return ans;
        }

        /**
         * Transforms this matrix into column echelon form with respect to a
         * collection of rows.  The transformation will perform only column
         * operations.
         *
         * Given the list \a rowList of rows from this matrix, this algorithm
         * puts this matrix in column echelon form with respect to those rows.
         * The only purpose of \a rowList is to clarify and/or weaken precisely
         * what is meant by "column echelon form"; all rows of this matrix are
         * affected by the resulting column operations that take place.
         *
         * This routine also computes the corresponding change of coordinate
         * matrices through the arguments \a R and \a Ri:
         *
         * - The matrix \a R will have precisely the same column operations
         *   applied to it as this matrix.  The matrix \a Ri will have the
         *   inverse _row_ operations applied to it, thereby maintaining a
         *   constant value of the product `R * Ri` as the algorithm runs.
         *
         * - In particular, if \a R and \a Ri are passed into this routine as
         *   square identity matrices, then after the reduction is complete we
         *   will have `original * R = final` and `final * Ri = original`, where
         *   \a original and \a final refer to the original and final values of
         *   this matrix.
         *
         * Our convention is that a matrix is in column echelon form with
         * respect to \a rowList if:
         *
         * - each column is either zero or there is a first non-zero entry which
         *   is positive, following the note regarding \a rowList below;
         *
         * - moving from the leftmost column to the rightmost column, the rows
         *   containing the first non-zero entries for these columns have
         *   strictly increasing indices in \a rowList;
         *
         * - given a first non-zero column entry, in that row all the elements to
         *   the left are smaller and non-negative (all elements to the right are
         *   already zero by the previous condition);
         *
         * - all the zero columns are on the right hand side of the matrix.
         *
         * By a "zero column" here we simply mean "zero for every row in
         * \a rowList".  Likewise, by "first non-zero entry" we mean "first row
         * in \a rowList with a non-zero entry".
         *
         * \pre `R.columns() == Ri.rows() == columns()`.
         * \pre All entries in this matrix are finite.
         *
         * \headers This routine is implemented in a separate header
         * (matrix-impl.h), which is not included automatically by this file.
         *
         * \exception InvalidArgument Either `R.columns() ≠ columns()`, and/or
         * `Ri.rows() ≠ columns()`.
         *
         * \param R used to collect the column operations, as described above.
         * \param Ri used to collect the inverse row operations, again as
         * described above.
         * \param rowList the rows to pay attention to.  This list must contain
         * distinct integers, all between 0 and `rows()-1` inclusive (though it
         * need not contain _all_ of these integers).  The integers may appear in
         * any order (though changing the order will change the resulting column
         * echelon form).  For a "classical" column echelon form, this would be
         * the list of all rows: `0,...,(rows()-1)`.
         *
         * \author Ryan Budney
         */
        void makeColumnEchelonForRows(const std::vector<size_t>& rowList,
                Matrix& R, Matrix& Ri) requires ReginaInteger<T>;

        /**
         * Implements makeSmithCoB() for the case where the algorithm argument
         * is SNFAlgorithm::HafnerMcCurley.  See makeSmithCoB() for more
         * details.
         *
         * \pre All entries in this matrix are finite.
         *
         * \return the four change-of-basis matrices.
         */
        std::array<Matrix, 4> makeSmithHafnerMcCurley()
            requires ReginaInteger<T>;

        /**
         * Implements makeSmithCoB() for the case where the algorithm argument
         * is SNFAlgorithm::Metrical.  See makeSmithCoB() for more details.
         *
         * \pre All entries in this matrix are finite.
         *
         * \return the four change-of-basis matrices.
         *
         * \author Ryan Budney
         */
        std::array<Matrix, 4> makeSmithMetrical() requires ReginaInteger<T>;

    // Give internal access to Matrix<U> for conversion construction/assignment:
    template <typename U>
    requires std::default_initializable<U> && std::copyable<U> && Writeable<U>
    friend class Matrix;
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

/**
 * Deprecated routine that transforms the given integer matrix into
 * Smith normal form.
 *
 * This simply calls Matrix::makeSmith(); see that routine for further details.
 *
 * \deprecated You should now call `matrix.smithNormalForm()` instead.
 *
 * \pre All entries in the given matrix are finite.
 *
 * \param matrix the matrix to transform.
 *
 * \ingroup maths
 */
[[deprecated]] inline void smithNormalForm(MatrixInt& matrix) {
    matrix.makeSmith();
}

/**
 * Deprecated Smith normal form routine that also computes change-of-basis
 * matrices.
 *
 * This simply calls Matrix::makeSmithCoB(); see that routine for further
 * details.
 *
 * In this (deprecated) routine, the four change-of-basis matrices are given
 * as input and refilled by this routine.  Upon input, they may be of any size
 * (or may even be empty); upon return they will be square matrices of the
 * appropriate size.
 *
 * \deprecated You should now call `matrix.smithNormalFormCoB()` instead, and
 * collect the change-of-basis matrices from the return value of that function.
 *
 * \pre All entries in the given input matrix are finite.
 *
 * \param matrix the matrix to transform into Smith normal form.
 * \param rowSpaceBasis used to return the matrix of all row operations in the
 * conversion from the original matrix into its Smith normal form.
 * \param rowSpaceBasisInv used to return the inverse of \a rowSpaceBasis.
 * \param colSpaceBasis used to return the matrix of all column operations in
 * the conversion from the original matrix into its Smith normal form.
 * \param colSpaceBasisInv used to return the inverse of \a colSpaceBasis.
 *
 * \ingroup maths
 */
[[deprecated]] inline void smithNormalForm(MatrixInt& matrix,
        MatrixInt& rowSpaceBasis, MatrixInt& rowSpaceBasisInv,
        MatrixInt& colSpaceBasis, MatrixInt& colSpaceBasisInv) {
    auto cob = matrix.makeSmithCoB();
    rowSpaceBasis = std::move(cob[0]);
    rowSpaceBasisInv = std::move(cob[1]);
    colSpaceBasis = std::move(cob[2]);
    colSpaceBasisInv = std::move(cob[3]);
}

/**
 * Deprecated alternative Smith normal form routine that may be preferable for
 * extremely large matrices.
 *
 * This simply calls `Matrix::makeSmithCoB(SNFAlgorithm::Metrical)`; see that
 * routine for further details.
 *
 * In this (deprecated) routine, the four change-of-basis matrices are given
 * as input and refilled by this routine.  Upon input, they may be of any size
 * (or may even be empty); upon return they will be square matrices of the
 * appropriate size.
 *
 * \deprecated You should now call `matrix.metricalSmithNormalForm()` instead,
 * and collect the change-of-basis matrices from the return value of that
 * function.
 *
 * \param matrix the matrix to transform into Smith normal form.
 * \param rowSpaceBasis used to return the matrix of all row operations in the
 * conversion from the original matrix into its Smith normal form.
 * \param rowSpaceBasisInv used to return the inverse of \a rowSpaceBasis.
 * \param colSpaceBasis used to return the matrix of all column operations in
 * the conversion from the original matrix into its Smith normal form.
 * \param colSpaceBasisInv used to return the inverse of \a colSpaceBasis.
 *
 * \pre All entries in the given input matrix are finite.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
[[deprecated]] inline void metricalSmithNormalForm(MatrixInt& matrix,
        MatrixInt& rowSpaceBasis, MatrixInt& rowSpaceBasisInv,
        MatrixInt& colSpaceBasis, MatrixInt& colSpaceBasisInv) {
    auto cob = matrix.makeSmithCoB(SNFAlgorithm::Metrical);
    rowSpaceBasis = std::move(cob[0]);
    rowSpaceBasisInv = std::move(cob[1]);
    colSpaceBasis = std::move(cob[2]);
    colSpaceBasisInv = std::move(cob[3]);
}

/**
 * Deprecated function that, given a homomorphism from `Z^n` to `Z^k` and a
 * sublattice of `Z^k`, compute the preimage of this sublattice under this
 * homomorphism.
 *
 * The homomorphism from `Z^n` to `Z^k` is described by the given
 * `k × n` matrix \a hom.  The sublattice is of the form
 * `(p1 Z) * (p2 Z) * ... * (pk Z)`, where the non-negative integers
 * `p1,...,pk` are passed in the given list \a sublattice.
 *
 * The preimage of the sublattice is some rank \a n lattice in `Z^n`.
 * This routine computes a basis for that lattice.
 *
 * An equivalent problem is to treat \a hom as a homomorphism from
 * `Z^n` to `Z_p1 + ... + Z_pk`; this routine then computes the kernel of this
 * homomorphism.
 *
 * \deprecated You should now call `hom.preimageOfLattice(sublattice)` instead
 * (note also the change of case in the function name).
 *
 * \exception InvalidArgument The length of \a sublattice is different from
 * the number of rows of \a hom.  Note that the _contents_ of \a sublattice
 * (specifically, the signs of the integers it contains) are not checked.
 *
 * \param hom the `k × n` matrix representing the homomorphism from
 * `Z^n` to `Z^k`.
 * \param sublattice a list of length \a k describing the sublattice of `Z^k`;
 * the elements of this list must be the non-negative integers `p1,...,pk`
 * as described above.
 * \return a matrix whose columns are a basis for the preimage lattice.
 * This matrix will have precisely \a n rows.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
[[deprecated]] inline MatrixInt preImageOfLattice(const MatrixInt& hom,
        const std::vector<Integer>& sublattice) {
    return hom.preimageOfLattice(sublattice);
}

} // namespace regina

#endif
