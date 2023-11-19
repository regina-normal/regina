
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

/*! \file maths/3d.h
 *  \brief Deals with floating-point 3-dimensional geometry.
 */

#ifndef __REGINA_3D
#ifndef __DOXYGEN
#define __REGINA_3D
#endif

#include <cmath>
#include <iostream>

namespace regina {

/**
 * Represents a vector in real three-dimensional space.  This class is also
 * used to represent a single 3-D point.
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for importing information,
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
     * \param coordinates array whose three elements are the \z x,
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
    Vector3D& operator = (const Vector3D&) = default;

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
     * Determines if this and the given vector have different coordinates.
     *
     * \warning Equality and inequailty testing, while supported, is
     * extremely fragile, since it relies on floating-point comparisons.
     *
     * \param other the vector to compare with this.
     * \return \c true if and only if the two vectors are different.
     */
    constexpr bool operator != (const Vector3D& other) const {
        return (x != other.x || y != other.y || z != other.z);
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
    Vector3D& operator += (const Vector3D& rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    /**
     * Subtracts the given vector from this vector.
     *
     * \param rhs the vector to subtract from this vector.
     * \return a reference to this vector.
     */
    Vector3D& operator -= (const Vector3D& rhs) {
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
    Vector3D& operator *= (Real scale) {
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
    Vector3D& operator /= (Real scale) {
        x /= scale; y /= scale; z /= scale;
        return *this;
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
 * `(cos θ/2, x sin θ/2, y sin θ/2, z sin θ/2)`.  Since the axis vector
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
 * - Geometric operations, such as matrix(), will produce the same results as
 *   though this quaternion had been normalised beforehand.  This is explicitly
 *   noted in the documentation for each relevant function.
 *
 * - If you wish (though this is not actually necessary), you can normalise
 *   the coordinates yourself by calling normalise().
 *
 * See Regina's \ref 3d "notes on 3-D geometry" for importing information,
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
        Real q_[4] { 1.0, 0.0, 0.0, 0.0 };
            /**< The four quaternion coordinates. */

    public:
        /**
         * Creates the identity rotation.  This is the operation that does not
         * rotate at all.
         */
        Rotation3D() = default;

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
        Rotation3D& operator = (const Rotation3D&) = default;

        /**
         * Returns the given quaternion coordinate for this rotation.
         *
         * \param index indicates which coordinate to return; this must be
         * between 0 and 3 inclusive.
         * \return the corresponding quaternion coordinate.
         */
        constexpr double operator[] (int index) const {
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
            return std::equal(q_, q_ + 4, other.q_);
        }

        /**
         * Determines if this and the given rotation have different quaternion
         * coordinates.
         *
         * \warning Equality and inequailty testing, while supported, is
         * extremely fragile, since it relies on floating-point comparisons.
         *
         * \param other the rotation to compare with this.
         * \return \c true if and only if the two rotations have different
         * quaternion coordinates.
         */
        constexpr bool operator != (const Rotation3D& other) const {
            return ! std::equal(q_, q_ + 4, other.q_);
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
        void normalise() {
            Real scale = 1.0 / std::sqrt(q_[0] * q_[0] + q_[1] * q_[1] +
                q_[2] * q_[2] + q_[3] * q_[3]);
            for (int i = 0; i < 4; ++i)
                q_[i] *= scale;
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
std::ostream& operator << (std::ostream& out, const Rotation3D<Real>& r) {
    return out << '(' << r[0] << ", " << r[1] << ", " << r[2] << ", "
        << r[3] << ')';
}

} // namespace regina

#endif

