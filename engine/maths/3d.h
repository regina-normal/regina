
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
 * \param vector the vector to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Vector3D<Real>& v) {
    return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

} // namespace regina

#endif

