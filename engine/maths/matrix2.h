
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_MATRIX2_H
#ifndef __DOXYGEN
#define __REGINA_MATRIX2_H
#endif

#include <array>
#include <iostream>
#include "regina-core.h"
#include "maths/ring.h"

/*! \file maths/matrix2.h
 *  \brief Deals with 2x2 integer matrices.
 */

namespace regina {

/**
 * Represents a 2-by-2 integer matrix.  The advantage of using this class
 * over the larger Matrix class template (e.g., MatrixInt) is that this class
 * has less overhead.
 *
 * This class only contains four long integers, and so it may be considered
 * small enough to pass about by value.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the Matrix classes.
 *
 * \ingroup maths
 */
class Matrix2 {
    private:
        std::array<std::array<long, 2>, 2> data_;
            /**< The four entries in this matrix, indexed by row and
                 then by column. */

    public:
        /**
         * Initialises to the zero matrix.
         */
        Matrix2();
        /**
         * Initialises to a copy of the given matrix.
         */
        Matrix2(const Matrix2&) = default;
        /**
         * Initialises to the given integer values.
         *
         * \param val00 the value to place in row 0, column 0.
         * \param val01 the value to place in row 0, column 1.
         * \param val10 the value to place in row 1, column 0.
         * \param val11 the value to place in row 1, column 1.
         */
        Matrix2(long val00, long val01, long val10, long val11);

        /**
         * Sets this matrix to be a copy of the given matrix.
         *
         * \return a reference to this matrix.
         */
        Matrix2& operator = (const Matrix2&) = default;

        /**
         * Swaps the contents of this and the given matrix.
         *
         * \param other the matrix whose contents should be swapped with this.
         */
        void swap(Matrix2& other) noexcept;

        /**
         * Gives read-only access to a single row of this matrix.
         *
         * This means that the integer in row \a r, column \a c can be
         * accessed as `myMatrix[r][c]` (where \a r and \a c are
         * each 0 or 1).
         *
         * \param row the index of the requested row; this must be 0 or 1.
         * \return a two-integer array containing the elements of the
         * requested row.
         */
        const std::array<long, 2>& operator [] (int row) const;
        /**
         * Gives read-write access to a single row of this matrix.
         *
         * This means that the integer in row \a r, column \a c can be
         * accessed as `myMatrix[r][c]` (where \a r and \a c are
         * each 0 or 1).  Each such element may be modified directly.
         *
         * \param row the index of the requested row; this must be 0 or 1.
         * \return a reference to the two-integer array containing the
         * elements of the requested row.
         */
        std::array<long, 2>& operator [] (int row);

        /**
         * Calculates the matrix product of this and the given matrix.
         * Neither this nor the given matrix is changed.
         *
         * \param other the matrix that this should be multiplied by.
         * \return the product \a this * \a other.
         */
        Matrix2 operator * (const Matrix2& other) const;
        /**
         * Calculates the scalar product of this matrix and the given
         * integer.  This matrix is not changed.
         *
         * \param scalar the integer that this matrix should be multiplied by.
         * \return the product \a this * \a scalar.
         */
        Matrix2 operator * (long scalar) const;
        /**
         * Calculates the sum of two matrices.
         * Neither this nor the given matrix is changed.
         *
         * \param other the matrix to add to this.
         * \return the sum \a this + \a other.
         */
        Matrix2 operator + (const Matrix2& other) const;
        /**
         * Calculates the difference of two matrices.
         * Neither this nor the given matrix is changed.
         *
         * \param other the matrix to subtract from this.
         * \return the difference \a this - \a other.
         */
        Matrix2 operator - (const Matrix2& other) const;
        /**
         * Determines the negative of this matrix.
         * This matrix is not changed.
         *
         * \return the negative of this matrix.
         */
        Matrix2 operator - () const;
        /**
         * Returns the transpose of this matrix.
         * This matrix is not changed.
         *
         * \return the transpose of this matrix.
         */
        Matrix2 transpose() const;
        /**
         * Calculates the inverse of this matrix.
         * This matrix is not changed.
         *
         * This routine only works for integer matrices whose determinant is
         * either +1 or -1.
         *
         * \return the inverse of this matrix.  If this matrix does not
         * have determinant +1 or -1, the zero matrix will be returned
         * instead.
         */
        Matrix2 inverse() const;

        /**
         * Adds the given matrix to this.
         * This matrix is changed to reflect the result.
         *
         * \param other the matrix to add to this.
         * \return a reference to this matrix with its new value.
         */
        Matrix2& operator += (const Matrix2& other);
        /**
         * Subtracts the given matrix from this.
         * This matrix is changed to reflect the result.
         *
         * \param other the matrix to subtract from this.
         * \return a reference to this matrix with its new value.
         */
        Matrix2& operator -= (const Matrix2& other);
        /**
         * Multiplies this by the given matrix.
         * This matrix is changed to reflect the result.
         *
         * \param other the matrix by which this should be multiplied.
         * \return a reference to this matrix with its new value.
         */
        Matrix2& operator *= (const Matrix2& other);
        /**
         * Multiplies this by the given scalar.
         * This matrix is changed to reflect the result.
         *
         * \param scalar the scalar by which this should be multiplied.
         * \return a reference to this matrix with its new value.
         */
        Matrix2& operator *= (long scalar);
        /**
         * Negates this matrix.
         * This matrix is changed to reflect the result.
         */
        void negate();
        /**
         * Inverts this matrix.
         *
         * This routine only works for integer matrices whose determinant is
         * either +1 or -1.  Otherwise this matrix is left unchanged.
         *
         * \return \c true if this matrix was successfully inverted
         * (i.e., its determinant was +1 or -1), or \c false otherwise.
         */
        bool invert();

        /**
         * Determines if this is equal to the given matrix.
         *
         * \return \c true if and only if this matrix is equal to the given
         * matrix.
         */
        bool operator == (const Matrix2&) const = default;

        /**
         * Returns the determinant of this matrix.
         *
         * \return the determinant of this matrix.
         */
        long determinant() const;
        /**
         * Determines if this is the 2-by-2 identity matrix.
         *
         * \return \c true if this is the identity matrix, or \c false
         * otherwise.
         */
        bool isIdentity() const;
        /**
         * Determines if this is the 2-by-2 zero matrix.
         *
         * \return \c true if this is the zero matrix, or \c false
         * otherwise.
         */
        bool isZero() const;

    friend std::ostream& operator << (std::ostream& out, const Matrix2& mat);
};

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <>
struct RingTraits<Matrix2> {
    inline static const Matrix2 zero;
    inline static const Matrix2 one { 1, 0, 0, 1 };
};
#endif // __DOXYGEN

/**
 * Swaps the contents of the two given matrices.
 *
 * This global routine simply calls Matrix2::swap(); it is provided so
 * that Matrix2 meets the C++ Swappable requirements.
 *
 * \param a the first matrix whose contents should be swapped.
 * \param b the second matrix whose contents should be swapped.
 *
 * \ingroup maths
 */
void swap(Matrix2& a, Matrix2& b) noexcept;

/**
 * Writes the given matrix to the given output stream.  The matrix will
 * be written entirely on a single line, with the first row followed by the
 * second row.
 *
 * \param out the output stream to which to write.
 * \param mat the matrix to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
std::ostream& operator << (std::ostream& out, const Matrix2& mat);

/**
 * Compare two matrices to determine which is more aesthetically pleasing.
 * The way in which this judgement is made is purely aesthetic on the part
 * of the author, and is subject to change in future versions of Regina.
 *
 * \python Instead of a `std::strong_ordering`, this routine returns an
 * integer -1, 0 or 1 (representing `less`, `equal` or `greater` respectively).
 *
 * \param m1 the first matrix to examine.
 * \param m2 the second matrix to examine.
 * \return `less` if \a m1 is deemed to be more pleasing than \a m2,
 * `greater` if \a m2 is more pleasing than \a m1, or `equal` if both matrices
 * are equal.
 *
 * \ingroup maths
 */
std::strong_ordering simplerThreeWay(const Matrix2& m1, const Matrix2& m2);

/**
 * Deprecated routine that determines whether the first given matrix is
 * more aesthetically pleasing than the second.
 *
 * \deprecated This routine is implemented using simplerThreeWay(), and new
 * code should use that routine instead.  See simplerThreeWay() for further
 * discussion.
 *
 * \param m1 the first matrix to examine.
 * \param m2 the second matrix to examine.
 * \return \c true if \a m1 is deemed to be more pleasing than \a m2, or
 * \c false if either the matrices are equal or \a m2 is more pleasing than
 * \a m1.
 *
 * \ingroup maths
 */
[[deprecated]] bool simpler(const Matrix2& m1, const Matrix2& m2);

/**
 * Compares two ordered pairs of matrices to determine which pair is more
 * aesthetically pleasing.  The way in which this judgement is made is purely
 * aesthetic on the part of the author, and is subject to change in future
 * versions of Regina.
 *
 * Note that pairs are ordered, so the pair (\a M, \a N) may be more
 * (or perhaps less) pleasing than the pair (\a N, \a M).
 *
 * \python Instead of a `std::strong_ordering`, this routine returns an
 * integer -1, 0 or 1 (representing `less`, `equal` or `greater` respectively).
 *
 * \param pair1first the first matrix of the first pair to examine.
 * \param pair1second the second matrix of the first pair to examine.
 * \param pair2first the first matrix of the second pair to examine.
 * \param pair2second the second matrix of the second pair to examine.
 * \return `less` if the first pair is deemed to be more pleasing than the
 * second pair, `greater` if the second pair is more pleasing than the first,
 * or `equal` if both ordered pairs are equal.
 *
 * \ingroup maths
 */
std::strong_ordering simplerThreeWay(
        const Matrix2& pair1first, const Matrix2& pair1second,
        const Matrix2& pair2first, const Matrix2& pair2second);

/**
 * Deprecated routine that determines whether the first given pair of matrices
 * is more aesthetically pleasing than the second pair.
 *
 * \deprecated This routine is implemented using simplerThreeWay(), and new
 * code should use that routine instead.  See simplerThreeWay() for further
 * discussion.
 *
 * \param pair1first the first matrix of the first pair to examine.
 * \param pair1second the second matrix of the first pair to examine.
 * \param pair2first the first matrix of the second pair to examine.
 * \param pair2second the second matrix of the second pair to examine.
 * \return \c true if the first pair is deemed to be more pleasing than
 * the second pair, or \c false if either the ordered pairs are equal or
 * the second pair is more pleasing than the first.
 *
 * \ingroup maths
 */
[[deprecated]] bool simpler(
        const Matrix2& pair1first, const Matrix2& pair1second,
        const Matrix2& pair2first, const Matrix2& pair2second);

// Inline functions for Matrix2

inline Matrix2::Matrix2() : data_ {{{ 0, 0 }, { 0, 0 }}} {
}

inline Matrix2::Matrix2(long val00, long val01, long val10, long val11) :
        data_ {{{ val00, val01 }, { val10, val11 }}} {
}

inline void Matrix2::swap(Matrix2& other) noexcept {
    data_.swap(other.data_);
}

inline const std::array<long, 2>& Matrix2::operator [] (int row) const {
    return data_[row];
}

inline std::array<long, 2>& Matrix2::operator [] (int row) {
    return data_[row];
}

inline Matrix2 Matrix2::operator * (const Matrix2& other) const {
    return Matrix2(
        data_[0][0] * other.data_[0][0] + data_[0][1] * other.data_[1][0],
        data_[0][0] * other.data_[0][1] + data_[0][1] * other.data_[1][1],
        data_[1][0] * other.data_[0][0] + data_[1][1] * other.data_[1][0],
        data_[1][0] * other.data_[0][1] + data_[1][1] * other.data_[1][1]);
}

inline Matrix2 Matrix2::operator * (long scalar) const {
    return Matrix2(
        data_[0][0] * scalar, data_[0][1] * scalar,
        data_[1][0] * scalar, data_[1][1] * scalar);
}

inline Matrix2 Matrix2::operator + (const Matrix2& other) const {
    return Matrix2(
        data_[0][0] + other.data_[0][0], data_[0][1] + other.data_[0][1],
        data_[1][0] + other.data_[1][0], data_[1][1] + other.data_[1][1]);
}

inline Matrix2 Matrix2::operator - (const Matrix2& other) const {
    return Matrix2(
        data_[0][0] - other.data_[0][0], data_[0][1] - other.data_[0][1],
        data_[1][0] - other.data_[1][0], data_[1][1] - other.data_[1][1]);
}

inline Matrix2 Matrix2::operator - () const {
    return Matrix2(-data_[0][0], -data_[0][1], -data_[1][0], -data_[1][1]);
}

inline Matrix2 Matrix2::transpose() const {
    return Matrix2(data_[0][0], data_[1][0], data_[0][1], data_[1][1]);
}

inline Matrix2& Matrix2::operator += (const Matrix2& other) {
    data_[0][0] += other.data_[0][0]; data_[0][1] += other.data_[0][1];
    data_[1][0] += other.data_[1][0]; data_[1][1] += other.data_[1][1];
    return *this;
}

inline Matrix2& Matrix2::operator -= (const Matrix2& other) {
    data_[0][0] -= other.data_[0][0]; data_[0][1] -= other.data_[0][1];
    data_[1][0] -= other.data_[1][0]; data_[1][1] -= other.data_[1][1];
    return *this;
}

inline Matrix2& Matrix2::operator *= (long scalar) {
    data_[0][0] *= scalar; data_[0][1] *= scalar;
    data_[1][0] *= scalar; data_[1][1] *= scalar;
    return *this;
}

inline void Matrix2::negate() {
    data_[0][0] = -data_[0][0]; data_[0][1] = -data_[0][1];
    data_[1][0] = -data_[1][0]; data_[1][1] = -data_[1][1];
}

inline long Matrix2::determinant() const {
    return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
}

inline bool Matrix2::isIdentity() const {
    return (data_[0][0] == 1 && data_[0][1] == 0 &&
            data_[1][0] == 0 && data_[1][1] == 1);
}

inline bool Matrix2::isZero() const {
    return (data_[0][0] == 0 && data_[0][1] == 0 &&
            data_[1][0] == 0 && data_[1][1] == 0);
}

inline std::ostream& operator << (std::ostream& out, const Matrix2& mat) {
    return out << "[[ " << mat.data_[0][0] << ' ' << mat.data_[0][1]
        << " ] [ " << mat.data_[1][0] << ' ' << mat.data_[1][1] << " ]]";
}

inline void swap(Matrix2& a, Matrix2& b) noexcept {
    a.swap(b);
}

inline bool simpler(const Matrix2& m1, const Matrix2& m2) {
    return simplerThreeWay(m1, m2) == std::strong_ordering::less;
}

inline bool simpler(const Matrix2& pair1first, const Matrix2& pair1second,
        const Matrix2& pair2first, const Matrix2& pair2second) {
    return simplerThreeWay(pair1first, pair1second, pair2first, pair2second) ==
        std::strong_ordering::less;
}

} // namespace regina

#endif
