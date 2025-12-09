
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

/*! \file maths/3d.h
 *  \brief Deals with floating-point 3-dimensional geometry.
 */

#ifndef __REGINA_3D
#ifndef __DOXYGEN
#define __REGINA_3D
#endif

#include <array>
#include <cmath>
#include <iostream>

namespace regina {

/**
 * Represents a vector in real three-dimensional space.  This class is also
 * used to represent a single 3-D point.
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for important information,
 * including the inexact floating-point nature of the Vector3D class, and the
 * right-handedness of Regina's coordinate system.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python The template parameter \a Real is \c double.
 *
 * \tparam Real the floating-point type to use for all storage and computation.
 *
 * \ingroup maths
 */
template <typename Real = double>
struct Vector3D {
    /**
     * The first (x) coordinate of the vector.
     */
    Real x;
    /**
     * The second (y) coordinate of the vector.
     */
    Real y;
    /**
     * The third (z) coordinate of the vector.
     */
    Real z;

    /**
     * Creates a new vector whose coordinates are uninitialised.
     */
    Vector3D() = default;

    /**
     * Creates a new copy of the given vector.
     */
    constexpr Vector3D(const Vector3D&) = default;

    /**
     * Creates a new vector with the given coordinates.
     *
     * \param x the first (x) coordinate.
     * \param y the second (y) coordinate.
     * \param z the third (z) coordinate.
     */
    constexpr Vector3D(Real x, Real y, Real z) : x(x), y(y), z(z) {
    }

    /**
     * Creates a new vector with the given coordinates.
     *
     * \param coordinates array whose three elements are the \a x,
     * \a y and \a z coordinate respectively.
     */
    constexpr Vector3D(const std::array<Real, 3>& coordinates) :
            x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    }

    /**
     * Sets this to be a copy of the given vector.
     *
     * \return a reference to this vector.
     */
    constexpr Vector3D& operator = (const Vector3D&) = default;

    /**
     * Determines if this and the given vector have the same coordinates.
     *
     * \warning Equality and inequailty testing, while supported, is
     * extremely fragile, since it relies on floating-point comparisons.
     *
     * \param other the vector to compare with this.
     * \return \c true if and only if the two vectors are equal.
     */
    constexpr bool operator == (const Vector3D& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    /**
     * Returns the sum of this and the given vector.
     *
     * \param rhs the vector to add to this vector.
     * \return the sum of this and the given vector.
     */
    constexpr Vector3D operator + (const Vector3D& rhs) const {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    };

    /**
     * Returns the difference obtained from this vector minus the given vector.
     *
     * If both this and \a rhs represent 3-D points, then the result is the
     * vector pointing from \a rhs to this point.
     *
     * \param rhs the vector to subtract from this vector.
     * \return the difference obtained from this minus the given vector.
     */
    constexpr Vector3D operator - (const Vector3D& rhs) const {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    };

    /**
     * Returns a copy of this vector scaled up by the given factor.
     * Specifically, the coordinates of the vector that is returned will
     * be the coordinates of this vector multiplied by \a scale.
     *
     * \param scale the scaling factor to apply.
     * \return a rescaled copy of this vector.
     */
    constexpr Vector3D operator * (Real scale) const {
        return { x * scale, y * scale, z * scale };
    }

    /**
     * Returns a copy of this vector scaled down by the given factor.
     * Specifically, the coordinates of the vector that is returned will
     * be the coordinates of this vector divided by \a scale.
     *
     * \param scale the scaling factor to apply; this must be non-zero.
     * \return a rescaled copy of this vector.
     */
    constexpr Vector3D operator / (Real scale) const {
        return { x / scale, y / scale, z / scale };
    }

    /**
     * Adds the given vector to this vector.
     *
     * \param rhs the vector to add to this vector.
     * \return a reference to this vector.
     */
    constexpr Vector3D& operator += (const Vector3D& rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    /**
     * Subtracts the given vector from this vector.
     *
     * \param rhs the vector to subtract from this vector.
     * \return a reference to this vector.
     */
    constexpr Vector3D& operator -= (const Vector3D& rhs) {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }

    /**
     * Scales this vector up by the given factor.
     * Specifically, all coordinates of this vector will be multiplied
     * by \a scale.
     *
     * \param scale the scaling factor to apply.
     * \return a reference to this vector.
     */
    constexpr Vector3D& operator *= (Real scale) {
        x *= scale; y *= scale; z *= scale;
        return *this;
    }

    /**
     * Scales this vector down by the given factor.
     * Specifically, all coordinates of this vector will be divided by \a scale.
     *
     * \param scale the scaling factor to apply; this must be non-zero.
     * \return a reference to this vector.
     */
    constexpr Vector3D& operator /= (Real scale) {
        x /= scale; y /= scale; z /= scale;
        return *this;
    }

    /**
     * Returns the inner product of this and the given vector.
     *
     * \param rhs the other vector to use in computing the inner product.
     * \return the inner product of this vector and \a rhs.
     */
    constexpr Real operator * (const Vector3D& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    /**
     * Returns the length of this vector.
     *
     * If this object represents a single 3-D point, then this function
     * returns the distance of this point from the origin.
     *
     * \return the length of this vector.
     */
    constexpr Real length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * Returns the distance between this and the given point in 3-D space.
     *
     * The naming and description of this function assume that both this and
     * \a other represent individual points in 3-D space.
     *
     * Computing `a.distance(b)` is equivalent to computing `(a-b).length()`.
     *
     * \return the distance between this and the given point.
     */
    constexpr Real distance(const Vector3D& other) const {
        Real dx = x - other.x;
        Real dy = y - other.y;
        Real dz = z - other.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    /**
     * Returns the midpoint between this and the given point.
     *
     * The naming and description of this function assume that both this and
     * \a other represent individual points in 3-D space.
     *
     * Computing `a.midpoint(b)` is equivalent to computing `(a+b)/2`.
     *
     * \return the midpoint between this and the given point.
     */
    constexpr Vector3D midpoint(const Vector3D& other) const {
        return { (x + other.x) / 2,
                 (y + other.y) / 2,
                 (z + other.z) / 2};
    }

    /**
     * Casts this point to a standard C++ array.
     *
     * \return a three-element array containing \a x, \a y and \a z
     * (in that order).
     */
    constexpr operator std::array<Real, 3>() const {
        return { x, y, z };
    }
};

/**
 * Writes the given vector to the given output stream.
 * The vector will be written as a triple `(x, y, z)`.
 *
 * \param out the output stream to which to write.
 * \param v the vector to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Vector3D<Real>& v) {
    return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

/**
 * Represents a line segment in 3-dimensional space, defined by its two
 * endpoints \a u and \a v.
 *
 * The points on this line segment are precisely those points of the form
 * `𝜆v + (1-𝜆)u`, where 𝜆 is any real number between 0 and 1 inclusive.
 *
 * Degenerate segments (whose two endpoints are the same) are explicitly
 * supported by this class.
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for important information,
 * including the inexact floating-point nature of the Vector3D class, and the
 * right-handedness of Regina's coordinate system.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python The template parameter \a Real is \c double.
 *
 * \tparam Real the floating-point type to use for all storage and computation.
 *
 * \ingroup maths
 */
template <typename Real = double>
struct Segment3D {
    /**
     * The first endpoint (u) of this line segment.
     */
    Vector3D<Real> u;
    /**
     * The second endpoint (v) of this line segment.
     */
    Vector3D<Real> v;

    /**
     * Creates a new line segment whose endpoints are uninitialised.
     */
    Segment3D() = default;

    /**
     * Creates a new copy of the given line segment.
     */
    constexpr Segment3D(const Segment3D&) = default;

    /**
     * Creates a new line segment with the given endpoints.
     *
     * \param u the first endpoint.
     * \param v the second endpoint.
     */
    constexpr Segment3D(const Vector3D<Real>& u, const Vector3D<Real>& v) :
            u(u), v(v) {
    }

    /**
     * Sets this to be a copy of the given line segment.
     *
     * \return a reference to this line segment.
     */
    constexpr Segment3D& operator = (const Segment3D&) = default;

    /**
     * Determines if this and the given line segment have the same endpoints,
     * in the same order.
     *
     * \warning Equality and inequailty testing, while supported, is
     * extremely fragile, since it relies on floating-point comparisons.
     *
     * \param other the line segment to compare with this.
     * \return \c true if and only if both segments have identical first
     * endpoints \a u, and both segments have identical second endpoints \a v.
     */
    constexpr bool operator == (const Segment3D& other) const {
        return (u == other.u && v == other.v);
    }

    /**
     * Returns the length of this line segment.
     *
     * \return the length of this line segment.
     */
    constexpr Real length() const {
        return (u - v).length();
    }

    /**
     * Returns the point on this line segment represented by the given real
     * number 𝜆.
     *
     * As outlined in the class notes, this line segment contains all points
     * of the form `𝜆v + (1-𝜆)u`, where 𝜆 is any real number between 0 and 1
     * inclusive.  This routine returns the exact point corresponding to the
     * given argument 𝜆.  In particular `point(0)` will return the first
     * endpoint \a u, and `point(1)` will return the second endpoint \a v.
     *
     * \param lambda the real number 𝜆 as described above.  Typically this
     * would be between 0 and 1 inclusive; however, there is no problem
     * passing a value of 𝜆 outside this range (which, for non-degenerate
     * segments, means the resulting point will be outside the bounds of
     * this line segment).
     * \return the corresponding point `𝜆v + (1-𝜆)u`.
     */
    constexpr Vector3D<Real> point(Real lambda) const {
        return v * lambda + u * (1 - lambda);
    };

    /**
     * Returns the translation of this line segment by the given vector.
     *
     * \param translation the vector to add to both endpoints of this
     * line segment.
     * \return the translated copy of this line segment.
     */
    constexpr Segment3D operator + (const Vector3D<Real>& translation) const {
        return { u + translation, v + translation };
    };

    /**
     * Translates this line segment by the given vector.
     *
     * \param translation the vector to add to both endpoints of this
     * line segment.
     * \return a reference to this line segment, which will be modified
     * directly.
     */
    constexpr Segment3D& operator += (const Vector3D<Real>& translation) {
        u += translation;
        v += translation;
        return *this;
    }

    /**
     * Returns the translation of this line segment by the negative of the
     * given vector.
     *
     * \param translation the vector to subtract from both endpoints of this
     * line segment.
     * \return the translated copy of this line segment.
     */
    constexpr Segment3D operator - (const Vector3D<Real>& translation) const {
        return { u - translation, v - translation };
    };

    /**
     * Translates this line segment by the negative of the given vector.
     *
     * \param translation the vector to subtract from both endpoints of this
     * line segment.
     * \return a reference to this line segment, which will be modified
     * directly.
     */
    constexpr Segment3D& operator -= (const Vector3D<Real>& translation) {
        u -= translation;
        v -= translation;
        return *this;
    }

    /**
     * Returns the midpoint of this line segment.
     *
     * \return the midpoint of this line segment.
     */
    constexpr Vector3D<Real> midpoint() const {
        return { (u.x + v.x) / 2,
                 (u.y + v.y) / 2,
                 (u.z + v.z) / 2};
    }

    /**
     * Computes the closest point on this line segment to the given point.
     *
     * This routine does respect the limits defined by the two endpoints of
     * this line segment.  That is, the resulting closest point will always
     * lie between the two endpoints of this segment inclusive.
     *
     * This routine does behave correctly if this segment is degenerate
     * (i.e., its two endpoints are the same); however, the real number that
     * is returned could be anywhere between 0 and 1 inclusive.
     *
     * \param p the point whose proximity we are interested in.
     * \return a real number 𝜆 between 0 and 1 inclusive, where the closest
     * point to \a p on this segment is the point `𝜆v + (1-𝜆)u`; in other
     * words, `point(𝜆)`.
     */
    constexpr Real closest(const Vector3D<Real>& p) const {
        auto uv = v - u;
        auto norm = uv * uv;
        if (norm > 0) {
            auto ans = (v - u) * (p - u) / norm;
            return (ans > 1 ? 1 : ans < 0 ? 0 : ans);
        } else
            return 0; // degenerate segment
    }
};

/**
 * Writes the given line segment to the given output stream.
 * The segment will be written in the form `[(...), (...)]`.
 *
 * \param out the output stream to which to write.
 * \param s the line segment to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Segment3D<Real>& s) {
    return out << '[' << s.u << ", " << s.v << ']';
}

/**
 * Represents a linear transformation in three-dimensional space, as
 * represented by a real 3-by-3 matrix.
 *
 * These matrices act on _column_ vectors.  Specifically, a transformation
 * represented by the 3-by-3 matrix `M` will transform the column vector
 * `v` into the vector `M * v`.
 *
 * If you are interested specifically in rotations, then you should use the
 * Rotation3D class instead, which uses a more compact and numerically stable
 * representation (quaternions).
 *
 * This class is designed specifically to work with transformations, and so it
 * focuses more on operations such as composition and inverse, and less on
 * other more general matrix operations.  For a general numerical matrix class
 * you can always use `Matrix<double>` (or `MatrixReal` in Python) instead.
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for important information,
 * including the inexact floating-point nature of the Vector3D class, and the
 * right-handedness of Regina's coordinate system.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python The template parameter \a Real is \c double.
 *
 * \tparam Real the floating-point type to use for all storage and computation.
 *
 * \ingroup maths
 */
template <typename Real = double>
class Matrix3D {
    private:
        std::array<std::array<Real, 3>, 3> m_ {
                std::array<Real, 3>{ 1.0, 0.0, 0.0 },
                std::array<Real, 3>{ 0.0, 1.0, 0.0 },
                std::array<Real, 3>{ 0.0, 0.0, 1.0 } };
            /**< The individual matrix elements, indexed by row and then by
                 column. */

    public:
        /**
         * Creates the identity matrix.
         */
        constexpr Matrix3D() = default;

        /**
         * Creates a new copy of the given matrix.
         */
        constexpr Matrix3D(const Matrix3D&) = default;

        /**
         * Creates a new matrix containin the given entries.
         *
         * \param m00 the entry in row 0, column 0.
         * \param m01 the entry in row 0, column 1.
         * \param m02 the entry in row 0, column 2.
         * \param m10 the entry in row 1, column 0.
         * \param m11 the entry in row 1, column 1.
         * \param m12 the entry in row 1, column 2.
         * \param m20 the entry in row 2, column 0.
         * \param m21 the entry in row 2, column 1.
         * \param m22 the entry in row 2, column 2.
         */
        constexpr Matrix3D(Real m00, Real m01, Real m02,
            Real m10, Real m11, Real m12, Real m20, Real m21, Real m22) : m_{
                std::array<Real, 3>{ m00, m01, m02 },
                std::array<Real, 3>{ m10, m11, m12 },
                std::array<Real, 3>{ m20, m21, m22 }} {
        }

        /**
         * Sets this to be a copy of the given matrix.
         *
         * \return a reference to this matrix.
         */
        constexpr Matrix3D& operator = (const Matrix3D&) = default;

        /**
         * Gives read-write access to a single row of this matrix.
         *
         * This means that the entry in row \a r, column \a c can be accessed
         * as `matrix[r][c]` (where \a r and \a c are each 0, 1 or 2).
         *
         * \param row the index of the requested row; this must be 0, 1 or 2.
         * \return a reference to the three-element array containing the
         * elements of the requested row.
         */
        constexpr std::array<Real, 3>& operator [] (int row) {
            return m_[row];
        }

        /**
         * Gives read-only access to a single row of this matrix.
         *
         * This means that the entry in row \a r, column \a c can be accessed
         * as `matrix[r][c]` (where \a r and \a c are each 0, 1 or 2).
         *
         * \param row the index of the requested row; this must be 0, 1 or 2.
         * \return a three-element array containing the elements of the
         * requested row.
         */
        constexpr const std::array<Real, 3>& operator [] (int row) const {
            return m_[row];
        }

        /**
         * Determines if this and the given matrix are equal.
         *
         * \warning Equality and inequailty testing, while supported, is
         * extremely fragile, since it relies on floating-point comparisons.
         *
         * \param other the matrix to compare with this.
         * \return \c true if and only if the two matrices are equal.
         */
        constexpr bool operator == (const Matrix3D& other) const {
            return m_ == other.m_;
        }

        /**
         * Returns the composition of this and the given transformation.
         *
         * Composition of transformations is _not_ commutative.  Here we follow
         * the same convention as used elsewhere in Regina (e.g., by Regina's
         * permutation classes): the product `s * t` indicates that we should
         * apply transformation `t` first, followed by transformation `s`.
         * This is also consistent with the order in which we multiply the
         * underlying 3-by-3 matrices.
         *
         * \param rhs the transformation to apply before this.
         * \return the composition of this and the given transformation.
         */
        constexpr Matrix3D operator * (const Matrix3D& rhs) const {
            // Maybe this should be in a loop. *shrug*
            return Matrix3D {
                m_[0][0]*rhs[0][0] + m_[0][1]*rhs[1][0] + m_[0][2]*rhs[2][0],
                m_[0][0]*rhs[0][1] + m_[0][1]*rhs[1][1] + m_[0][2]*rhs[2][1],
                m_[0][0]*rhs[0][2] + m_[0][1]*rhs[1][2] + m_[0][2]*rhs[2][2],
                m_[1][0]*rhs[0][0] + m_[1][1]*rhs[1][0] + m_[1][2]*rhs[2][0],
                m_[1][0]*rhs[0][1] + m_[1][1]*rhs[1][1] + m_[1][2]*rhs[2][1],
                m_[1][0]*rhs[0][2] + m_[1][1]*rhs[1][2] + m_[1][2]*rhs[2][2],
                m_[2][0]*rhs[0][0] + m_[2][1]*rhs[1][0] + m_[2][2]*rhs[2][0],
                m_[2][0]*rhs[0][1] + m_[2][1]*rhs[1][1] + m_[2][2]*rhs[2][1],
                m_[2][0]*rhs[0][2] + m_[2][1]*rhs[1][2] + m_[2][2]*rhs[2][2]
            };
        }

        /**
         * Composes this with the given transformation, which is to be applied
         * first.  This transformation will be changed directly.
         *
         * Composition of transformations is _not_ commutative.  Here we follow
         * the same convention as used elsewhere in Regina (e.g., by Regina's
         * permutation classes): writing `s *= t` indicates that we should
         * apply transformation `t` first, followed by transformation `s`,
         * and then change `s` to store the resulting composition.  This is
         * consistent with the order in which we multiply the underlying
         * 3-by-3 matrices.
         *
         * \param rhs the transformation to apply before this.
         * \return a reference to this transformation.
         */
        constexpr Matrix3D& operator *= (const Matrix3D& rhs) {
            // No particular optimisations to perform here.
            return (*this = (*this) * rhs);
        }

        /**
         * Returns the image of the given vector under this transformation.
         *
         * Recall that vectors are treated as _column_ vectors.  That is, if
         * this transformation has matrix `M` and the given vector represents
         * the column vector `v`, then the result will be the column vector
         * `M * v`.
         *
         * \param vector the 3-D vector to rotate.
         * \return the result of applying this transformation to the given
         * vector.
         */
        constexpr Vector3D<Real> operator * (const Vector3D<Real>& vector)
                const {
            return Vector3D<Real> {
                m_[0][0]*vector.x + m_[0][1]*vector.y + m_[0][2]*vector.z,
                m_[1][0]*vector.x + m_[1][1]*vector.y + m_[1][2]*vector.z,
                m_[2][0]*vector.x + m_[2][1]*vector.y + m_[2][2]*vector.z
            };
        }

        /**
         * Returns the inverse of this transformation.
         *
         * \pre This transformation is invertible; that is, the underlying
         * 3-by-3 matrix does not have determinant zero.
         *
         * \return the inverse transformation.
         */
        constexpr Matrix3D inverse() const {
            Real det = (
                  m_[0][0] * m_[1][1] * m_[2][2]
                + m_[0][1] * m_[1][2] * m_[2][0]
                + m_[0][2] * m_[1][0] * m_[2][1]
                - m_[0][0] * m_[1][2] * m_[2][1]
                - m_[0][1] * m_[1][0] * m_[2][2]
                - m_[0][2] * m_[1][1] * m_[2][0]
            );

            // This will be the _transpose_ of the inverse matrix.
            Real a[3][3];
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    a[i][j] = (
                            m_[(i+1)%3][(j+1)%3] * m_[(i+2)%3][(j+2)%3] -
                            m_[(i+2)%3][(j+1)%3] * m_[(i+1)%3][(j+2)%3]
                        ) / det;

            return Matrix3D(
                a[0][0], a[1][0], a[2][0],
                a[0][1], a[1][1], a[2][1],
                a[0][2], a[1][2], a[2][2]);
        }
};

/**
 * Writes the given matrix to the given output stream.
 * The matrix will be written row by row, in the form
 * `[[ m00 m01 m02 ] [ m10 m11 m12 ] [ m20 m21 m22 ]]`.
 *
 * \param out the output stream to which to write.
 * \param m the matrix to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Matrix3D<Real>& m) {
    return out << "[[ " << m[0][0] << ' ' << m[0][1] << ' ' << m[0][2]
        << " ] [ " << m[1][0] << ' ' << m[1][1] << ' ' << m[1][2]
        << " ] [ " << m[2][0] << ' ' << m[2][1] << ' ' << m[2][2] << " ]]";
}

/**
 * Represents a rotation about the origin in real three-dimensional space.
 *
 * Regina stores a rotation using a _quaternion_, which consists of four real
 * numbers.  We refer to these four numbers as the _quaternion coordinates_.
 *
 * Specifically, suppose we rotate by an angle of θ around the axis pointing
 * from the origin to the unit vector `(x,y,z)`, and this rotation follows a
 * right-hand rule (the thumb of the right hand points from the origin out
 * towards `(x,y,z)`, and the fingers follow the direction of the rotation).
 * Then the four real numbers that make up the quaternion are
 * `(cos θ/2, x sin θ/2, y sin θ/2, z sin θ/2)`.
 *
 * Some notes about the uniqueness of quaternions:
 *
 * - A non-zero rotation can be described in two ways (by simultaneously
 *   negating both the axis vector and the angle of rotation).  Both
 *   descriptions yield identical quaternion coordinates.
 *
 * - A zero rotation (i.e., the identity operation) can be described using any
 *   axis of rotation.  Again, regardless of the choice of axis, we obtain
 *   identical quaternion coordinates (specifically, `(1,0,0,0)`).
 *
 * Regarding normalisation:
 *
 * - In theory, a quaternion that describes a rotation must be _normalised_;
 *   that is, its four real quaternion coordinates must form a unit vector in
 *   4-D.  This follows immediately from the discussion above, using the fact
 *   that the axis vector `(x,y,z)` is a unit vector in 3-D.
 *
 * - In this class, however, we do _not_ require the quaternion coordinates to
 *   be normalised, since this may allow quaternions to be constructed more
 *   easily.  Instead we allow the quaternion coordinates `(λa, λb, λc, λd)`
 *   to represent the same rotation as `(a,b,c,d)` for any positive λ (and,
 *   if we ignore the _direction_ of rotation and just consider its endpoint,
 *   then for any negative λ also).
 *
 * - If you wish (though this is not actually necessary), you can normalise
 *   the coordinates yourself by calling normalise().
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for important information,
 * including the inexact floating-point nature of the Vector3D class, and the
 * right-handedness of Regina's coordinate system.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python The template parameter \a Real is \c double.
 *
 * \tparam Real the floating-point type to use for all storage and computation.
 *
 * \ingroup maths
 */
template <typename Real = double>
class Rotation3D {
    private:
        std::array<Real, 4> q_ { 1.0, 0.0, 0.0, 0.0 };
            /**< The four quaternion coordinates. */

    public:
        /**
         * Creates the identity rotation.  This is the operation that does not
         * rotate at all.
         */
        constexpr Rotation3D() = default;

        /**
         * Creates a new copy of the given rotation.
         */
        constexpr Rotation3D(const Rotation3D&) = default;

        /**
         * Creates a new rotation from the given quaternion coordinates.
         *
         * As described in the class notes, these coordinates do not need to
         * be normalised.
         *
         * \pre The given coordinates are not all zero.
         *
         * \param a the first quaternion coordinate; that is, the coordinate
         * corresponding to `cos θ/2` from the discussion in the class notes.
         * \param b the second quaternion coordinate; that is, the coordinate
         * corresponding to `x sin θ/2` from the discussion in the class notes.
         * \param c the third quaternion coordinate; that is, the coordinate
         * corresponding to `y sin θ/2` from the discussion in the class notes.
         * \param d the fourth quaternion coordinate; that is, the coordinate
         * corresponding to `z sin θ/2` from the discussion in the class notes.
         */
        constexpr Rotation3D(Real a, Real b, Real c, Real d) : q_{a, b, c, d} {
        }

        /**
         * Sets this to be a copy of the given rotation.
         *
         * \return a reference to this rotation.
         */
        constexpr Rotation3D& operator = (const Rotation3D&) = default;

        /**
         * Returns the given quaternion coordinate for this rotation.
         *
         * \param index indicates which coordinate to return; this must be
         * between 0 and 3 inclusive.
         * \return the corresponding quaternion coordinate.
         */
        constexpr Real operator[] (int index) const {
            return q_[index];
        }

        /**
         * Determines if this and the given rotation have the same quaternion
         * coordinates.
         *
         * \warning Equality and inequailty testing, while supported, is
         * extremely fragile, since it relies on floating-point comparisons.
         *
         * \param other the rotation to compare with this.
         * \return \c true if and only if the two rotations have the same
         * quaternion coordinates.
         */
        constexpr bool operator == (const Rotation3D& other) const {
            return q_ == other.q_;
        }

        /**
         * Rescales all four quaternion coordinates by the same positive
         * constant so that the quaternion coordinates become normalised.
         *
         * Specifically, after this operation:
         *
         * - each quaternion coordinate will have the same sign as it did
         *   before this operation;
         *
         * - the four quaternion coordinates `(a,b,c,d)` will satisfy
         *   `a^2 + b^2 + c^2 + d^2 = 1`.
         */
        constexpr void normalise() {
            Real scale = 1.0 / std::sqrt(q_[0] * q_[0] + q_[1] * q_[1] +
                q_[2] * q_[2] + q_[3] * q_[3]);
            for (int i = 0; i < 4; ++i)
                q_[i] *= scale;
        }

        /**
         * Returns the composition of this and the given rotation.
         *
         * Composition of 3-D rotations is _not_ commutative.  Here we follow
         * the same convention as used elsewhere in Regina (e.g., by Regina's
         * permutation classes): the product `q * r` indicates that we should
         * apply rotation `r` first, followed by rotation `q`.  This is also
         * consistent with the matrix() function, which produces matrices that
         * act on column vectors (`matrix * vector`), and which therefore
         * compose using the same convention.
         *
         * This function does not require the quaternion coordinates of either
         * this or \a rhs to be normalised.  If the two rotations have
         * coordinates scaled by λ and μ respectively, then the resulting
         * composition will have its coordinates scaled by λμ.
         *
         * \param rhs the rotation to apply before this.
         * \return the composition of this and the given rotation.
         */
        constexpr Rotation3D operator * (const Rotation3D& rhs) const {
            return {
                q_[0] * rhs.q_[0] - q_[1] * rhs.q_[1]
                    - q_[2] * rhs.q_[2] - q_[3] * rhs.q_[3],
                q_[0] * rhs.q_[1] + q_[1] * rhs.q_[0] +
                    q_[2] * rhs.q_[3] - q_[3] * rhs.q_[2],
                q_[0] * rhs.q_[2] + q_[2] * rhs.q_[0] +
                    q_[3] * rhs.q_[1] - q_[1] * rhs.q_[3],
                q_[0] * rhs.q_[3] + q_[3] * rhs.q_[0] +
                    q_[1] * rhs.q_[2] - q_[2] * rhs.q_[1]
            };
        }

        /**
         * Composes this with the given rotation, which is to be applied first.
         * This rotation will be changed directly.
         *
         * Composition of 3-D rotations is _not_ commutative.  Here we follow
         * the same convention as used elsewhere in Regina (e.g., by Regina's
         * permutation classes): writing `q *= r` indicates that we should
         * apply rotation `r` first, followed by rotation `q`, and then change
         * `q` to store the resulting composition.  This is also consistent
         * with the matrix() function, which produces matrices that act on
         * column vectors (`matrix * vector`), and which therefore compose
         * using the same convention.
         *
         * This function does not require the quaternion coordinates of either
         * this or \a rhs to be normalised.  If the two original rotations have
         * coordinates scaled by λ and μ respectively, then the resulting
         * composition will have its coordinates scaled by λμ.
         *
         * \param rhs the rotation to apply before this.
         * \return a reference to this rotation.
         */
        constexpr Rotation3D& operator *= (const Rotation3D& rhs) {
            // No particular optimisations to perform here.
            return (*this = (*this) * rhs);
        }

        /**
         * Returns the image of the given vector under this rotation.
         *
         * \warning This operator is only recommended for occasional ad-hoc
         * calculations.  If you need to transform a large number of points
         * using the same rotation matrix, it is faster to call matrix() once
         * and then multiply each vector by the resulting matrix in turn.
         * (The implementation of this operator does exactly this, but does
         * not allow for caching the transformation matrix.)
         *
         * \param vector the 3-D vector to rotate.
         * \return the result of applying this rotation to the given vector.
         */
        constexpr Vector3D<Real> operator * (const Vector3D<Real>& vector)
                const {
            return matrix() * vector;
        }

        /**
         * Returns the inverse to this rotation.
         *
         * This rotates around the same axis by negative the original angle.
         *
         * This function does not require the quaternion coordinates to be
         * normalised.  If this rotation has its coordinates scaled by λ, then
         * the resulting rotation will have its coordinates scaled by λ also.
         *
         * \return the inverse rotation.
         */
        constexpr Rotation3D inverse() const {
            return { q_[0], -q_[1], -q_[2], -q_[3] };
        }

        /**
         * Returns the 3-dimensional transformation matrix for this rotation.
         *
         * The result will be a 3-by-3 matrix `M`, which can be used to rotate
         * column vectors by matrix multiplication.  Specifically, this rotation
         * transforms the column vector `v` into the vector `M * v`.
         *
         * This function does not require the quaternion coordinates to be
         * normalised.  The result will be the same as though normalise() had
         * been called beforehand.
         *
         * \return the corresponding 3-dimensional rotation matrix.
         */
        constexpr Matrix3D<Real> matrix() const {
            // Here we follow the formula from Wikipedia, taken from Watt and
            // Watt (1992), ISBN 978-0201544121, which optimises the number of
            // floating-point operations.
            Real s = 2.0 / (q_[0] * q_[0] + q_[1] * q_[1] +
                q_[2] * q_[2] + q_[3] * q_[3]);
            Real bs = q_[1] * s; Real cs = q_[2] * s; Real ds = q_[3] * s;
            Real ab = q_[0] * bs; Real ac = q_[0] * cs; Real ad = q_[0] * ds;
            Real bb = q_[1] * bs; Real bc = q_[1] * cs; Real bd = q_[1] * ds;
            Real cc = q_[2] * cs; Real cd = q_[2] * ds; Real dd = q_[3] * ds;
            return Matrix3D {
                1 - cc - dd, bc - ad, bd + ac,
                bc + ad, 1 - bb - dd, cd - ab,
                bd - ac, cd + ab, 1 - bb - cc };
        }
};

/**
 * Writes the given rotation to the given output stream.
 * The rotation will be written using its quaternion coordinates, as a
 * tuple `(a, b, c, d)`.
 *
 * \param out the output stream to which to write.
 * \param rot the rotation to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Rotation3D<Real>& rot) {
    return out << '(' << rot[0] << ", " << rot[1] << ", " << rot[2] << ", "
        << rot[3] << ')';
}

} // namespace regina

#endif

