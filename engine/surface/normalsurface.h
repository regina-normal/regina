
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

/*! \file surface/normalsurface.h
 *  \brief Deals with an individual normal surface in a 3-manifold
 *  triangulation.
 */

#ifndef __REGINA_NORMALSURFACE_H
#ifndef __DOXYGEN
#define __REGINA_NORMALSURFACE_H
#endif

#include <optional>
#include <utility>
#include "regina-core.h"
#include "core/output.h"
#include "maths/forward.h"
#include "maths/perm.h"
#include "maths/vector.h"
#include "packet/packet.h"
#include "surface/disctype.h"
#include "surface/normalcoords.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/snapshot.h"

namespace regina {

class NormalSurfaces;

/**
 * \defgroup surface Normal Surfaces
 * Normal surfaces in 3-manifold triangulations.
 */

/**
 * Lists which quadrilateral types separate which pairs of vertices in a
 * tetrahedron.
 * As outlined in NormalSurface::quads(), there are three quadrilateral types
 * in a tetrahedron, numbered 0, 1 and 2.  Each quadrilateral type separates
 * the four tetrahedron vertices 0,1,2,3 into two pairs.
 * `quadSeparating[i][j]` is the number of the quadrilateral type that
 * keeps vertices `i` and `j` together.
 *
 * It is guaranteed that quadrilateral type \a i will keep the vertices of
 * edge \a i together (and will therefore also keep the vertices of edge
 * \a 5-i together).
 *
 * \ingroup surface
 */
inline constexpr int quadSeparating[4][4] = {
    { -1, 0, 1, 2 }, {  0,-1, 2, 1 }, {  1, 2,-1, 0 }, {  2, 1, 0,-1 }
};

/**
 * Lists which quadrilateral types meet which edges in a tetrahedron.
 * See regina::quadSeparating and NormalSurface::quads() for more
 * information on quadrilateral types.
 *
 * `quadMeeting[i][j][0,1]` are the numbers of the two
 * quadrilateral types that meet the edge joining tetrahedron vertices
 * `i` and `j`.
 *
 * \ingroup surface
 */
inline constexpr int quadMeeting[4][4][2] = {
    { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
    { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
    { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
    { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
};

/**
 * Lists which vertices each quadrilateral type separates in a tetrahedron.
 * See regina::quadSeparating and NormalSurface::quads() for more
 * information on quadrilateral types.
 *
 * Quadrilateral type \c i splits the vertex pairs
 * `quadDefn[i][0,1]` and
 * `quadDefn[i][2,3]`.
 *
 * It is guaranteed that:
 *
 * - `quadDefn[i][0] < quadDefn[i][1]`;
 * - `quadDefn[i][2] < quadDefn[i][3]`;
 * - `quadDefn[i][0] < quadDefn[i][2]`.
 *
 * This array contains similar information to the function Edge<3>::ordering().
 * Instead of quadDefn[\a i][\a j], you can call Edge<3>::ordering(\a i)[\a j];
 * this will give the same results for \a j = 0 and 1, but it might
 * switch the results for \a j = 2 and 3.
 *
 * \ingroup surface
 */
inline constexpr int quadDefn[3][4] = {
    { 0, 1, 2, 3 }, { 0, 2, 1, 3 }, { 0, 3, 1, 2 }
};

/**
 * Lists the second vertex with which each vertex is paired under each
 * quadrilateral type in a tetrahedron.
 * See regina::quadSeparating and NormalSurface::quads() for more
 * information on quadrilateral types.
 *
 * Quadrilateral type \c i pairs vertex \c v with
 * vertex `quadPartner[i][v]`.
 *
 * \ingroup surface
 */
inline constexpr int quadPartner[3][4] = {
    { 1, 0, 3, 2}, { 2, 3, 0, 1}, { 3, 2, 1, 0}
};

/**
 * Contains strings that can be used to represent each quadrilateral type
 * in a tetrahedron.
 * See regina::quadSeparating and NormalSurface::quads() for more
 * information on quadrilateral types.
 *
 * The string describing quadrilateral type \c i is `quadString[i]` and
 * is of the form `02/13`, which in this case is the quadrilateral type
 * that splits vertices 0,2 from vertices 1,3.
 *
 * Some older compilers (such as GCC 10 and GCC 11) do not support constexpr
 * strings.  In such cases, this constant will be marked `inline const`
 * but not `constexpr`.
 *
 * \ingroup surface
 */
#if defined(__APIDOCS) || __cpp_lib_constexpr_string >= 201907L
inline constexpr std::string quadString[3] = { "01/23", "02/13", "03/12" };
#else
// The compiler does not support constexpr strings. Fall back to const.
inline const std::string quadString[3] = { "01/23", "02/13", "03/12" };
#endif

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of triangular normal disc.  Each permutation \a p
 * represents an arc about vertex `p[0]` parallel to the directed
 * edge from `p[1]` to `p[2]`.
 *
 * Array `triDiscArcs[i]` lists the boundary arcs of the
 * triangular disc of type \a i.  See NormalSurface::triangles()
 * for further details.
 *
 * Note that every permutation in this array is even.
 *
 * \ingroup surface
 */
inline constexpr Perm<4> triDiscArcs[4][3] = {
    Perm<4>(0,1,2,3), Perm<4>(0,2,3,1), Perm<4>(0,3,1,2),
    Perm<4>(1,0,3,2), Perm<4>(1,3,2,0), Perm<4>(1,2,0,3),
    Perm<4>(2,3,0,1), Perm<4>(2,0,1,3), Perm<4>(2,1,3,0),
    Perm<4>(3,2,1,0), Perm<4>(3,1,0,2), Perm<4>(3,0,2,1)
};

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of quadrilateral normal disc.  Each permutation \a p
 * represents an arc about vertex `p[0]` parallel to the directed
 * edge from `p[1]` to `p[2]`.
 *
 * Array `quadDiscArcs[i]` lists the boundary arcs of the
 * quadrilateral disc of type \a i.  See NormalSurface::quads()
 * for further details.
 *
 * Note that permutation `quadDiscArcs[i][j]` will be even
 * precisely when `j` is even.
 *
 * \ingroup surface
 */
inline constexpr Perm<4> quadDiscArcs[3][4] = {
    Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(1,3,2,0), Perm<4>(2,1,0,3),
    Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(2,1,3,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(3,2,1,0), Perm<4>(1,3,0,2)
};

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of octagonal normal disc.  Each permutation \a p
 * represents an arc about vertex `p[0]` parallel to the directed
 * edge from `p[1]` to `p[2]`.
 *
 * Array `octDiscArcs[i]` lists the boundary arcs of the
 * octagonal disc of type \a i.  See NormalSurface::octs()
 * for further details.
 *
 * Note that permutation `octDiscArcs[i][j]` will be even
 * precisely when `j` is 0, 1, 4 or 5.
 *
 * \ingroup surface
 */
inline constexpr Perm<4> octDiscArcs[3][8] = {
    Perm<4>(0,3,1,2), Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(2,3,1,0),
    Perm<4>(1,2,0,3), Perm<4>(1,0,3,2), Perm<4>(3,1,2,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(3,1,2,0),
    Perm<4>(2,3,0,1), Perm<4>(2,0,1,3), Perm<4>(1,2,3,0), Perm<4>(1,3,0,2),
    Perm<4>(0,2,3,1), Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(1,2,3,0),
    Perm<4>(3,1,0,2), Perm<4>(3,0,2,1), Perm<4>(2,3,1,0), Perm<4>(2,1,0,3)
};

/**
 * Represents a single normal surface in a 3-manifold triangulation.
 *
 * The normal surface is described internally by an integer vector
 * (discussed in more detail below).  Since different surfaces may use
 * different vector encodings, you should not rely on the raw vector
 * entries unless absolutely necessary.  Instead, the query routines
 * such as triangles(), quads(), edgeWeight() and so on are independent
 * of the underlying vector encoding being used.
 *
 * Note that non-compact surfaces (surfaces with infinitely many discs,
 * such as spun-normal surfaces) are allowed; in these cases, the
 * corresponding lookup routines (such as triangles()) will return
 * LargeInteger::infinity where appropriate.
 *
 * Since Regina 7.0, you can modify or even destroy the original
 * triangulation that was used to create this normal surface.  If you do,
 * then this normal surface will automatically make a private copy of
 * the original triangulation as an ongoing reference.  Different normal
 * surfaces (and angle structures) can all share the same private copy,
 * so this is not an expensive process.
 *
 * Internally, a normal surface is represented by a Vector<LargeInteger>
 * (possibly using a different coordinate system from the one in which
 * the surfaces were originally enumerated).  This contains a block of
 * coordinates for each tetrahedron, in order from the first tetrahedron
 * to the last.  Each block begins with four triangle coordinates (always),
 * then three quadrilateral coordinates (always), and finally three octagon
 * coordinates (only for some coordinate systems).  Therefore the vector
 * that is stored will always have length 7<i>n</i> or 10<i>n</i> where \a n
 * is the number of tetrahedra in the underlying triangulation.
 *
 * When adding support for a new coordinate system:
 *
 * - The file normalcoords.h must be updated.  This includes a new enum
 *   value for NormalCoords, a new case for the NormalEncoding constructor,
 *   and new cases for the functions in NormalInfo.  Do not forget to
 *   update the python bindings for NormalCoords also.
 *
 * - The global routines makeEmbeddedConstraints() and makeMatchingEquations()
 *   should be updated to incorporate the new coordinate system.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \feature Calculation of Euler characteristic and orientability
 * for non-compact surfaces.
 * \todo \featurelong Determine which faces in the solution space a
 * normal surface belongs to.
 *
 * \ingroup surface
 */
class NormalSurface : public ShortOutput<NormalSurface> {
    protected:
        NormalEncoding enc_;
            /**< The specific encoding of a normal surface used by the
                 coordinate vector. */
        Vector<LargeInteger> vector_;
            /**< Contains the coordinates of the normal surface. */
        SnapshotRef<Triangulation<3>> triangulation_;
            /**< The triangulation in which this normal surface resides. */

        std::string name_;
            /**< An optional name associated with this surface. */

        mutable std::optional<DiscType> octPosition_;
            /**< The position of the first non-zero octagonal coordinate,
                 or a null disc type if there is no non-zero octagonal
                 coordinate.  Here DiscType::type is an octagon type
                 between 0 and 2 inclusive.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<LargeInteger> eulerChar_;
            /**< The Euler characteristic of this surface.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<size_t> boundaries_;
            /**< The number of disjoint boundary curves on this surface.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> orientable_;
            /**< Is this surface orientable?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> twoSided_;
            /**< Is this surface two-sided?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> connected_;
            /**< Is this surface connected?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> realBoundary_;
            /**< Does this surface have real boundary (i.e. does it meet
                 any boundary triangles)?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> compact_;
            /**< Is this surface compact (i.e. does it only contain
                 finitely many discs)?
                 This is std::nullopt if it has not yet been computed. */
        mutable uint8_t linkOf_ { 0 };
            /**< Indicates which dimensions of face a positive rational multiple
                 of this surface is a thin or normalised link of.  This is
                 treated as a bitmask: for each i=0,1,2, the (2i+1)th bit
                 indicates whether this surface scales to the link of an
                 i-face, and the (2i)th bit indicates whether this information
                 has actually been computed yet; if it has not been computed,
                 then the (2i+1)th bit will be zero. */

    public:
        /**
         * Creates a new copy of the given normal surface.
         */
        NormalSurface(const NormalSurface&) = default;

        /**
         * Creates a new copy of the given normal surface, but relocated
         * to the given triangulation.
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal surface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation in which
         * \a src resides.
         *
         * \param src the normal surface to copy.
         * \param triangulation the triangulation in which this new surface
         * will reside.
         */
        NormalSurface(const NormalSurface& src,
            const Triangulation<3>& triangulation);

        /**
         * Creates a new copy of the given normal surface, but relocated
         * to the given triangulation.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation in which
         * \a src resides.
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param src the normal surface to copy.
         * \param triangulation a snapshot, frozen in time, of the
         * triangulation in which this new surface will reside.
         */
        NormalSurface(const NormalSurface& src,
            const SnapshotRef<Triangulation<3>>& triangulation);

        /**
         * Moves the given surface into this new normal surface.
         * This is a fast (constant time) operation.
         *
         * The surface that is passed will no longer be usable.
         */
        NormalSurface(NormalSurface&&) noexcept = default;

        /**
         * Create the empty surface within the given triangulation.
         *
         * All normal coordinates will be zero.
         *
         * \param triang the triangulation in which this normal surface resides.
         */
        NormalSurface(const Triangulation<3>& triang);

        /**
         * Create the empty surface within the given triangulation.
         *
         * All normal coordinates will be zero.
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this surface will keep the given
         * encoding: NormalSurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal surface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the given encoding.
         * This will not be checked!
         *
         * \python The supported types for the template parameter \a U are
         * regina::Integer and regina::LargeInteger.  You may also, if you
         * prefer, pass \a vector as a Python list of integers.
         *
         * \tparam U the type of object held by the given vector.  It must be
         * possible to assign an object of type \a U to a regina::LargeInteger.
         *
         * \param triang the triangulation in which this normal surface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * surface.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        template <typename U>
        NormalSurface(const Triangulation<3>& triang, NormalEncoding enc,
            const Vector<U>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this surface will keep the given
         * encoding: NormalSurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal surface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the given encoding.
         * This will not be checked!
         *
         * \nopython Instead use the version that copies \a vector.
         *
         * \param triang the triangulation in which this normal surface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * surface.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const Triangulation<3>& triang, NormalEncoding enc,
            Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this surface will keep the given
         * encoding: NormalSurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the given encoding.
         * This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \tparam U the type of object held by the given vector.  It must be
         * possible to assign an object of type \a U to a regina::LargeInteger.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * surface.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        template <typename U>
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalEncoding enc, const Vector<U>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this surface will keep the given
         * encoding: NormalSurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the given encoding.
         * This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation
         * and copies \a vector.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * surface.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalEncoding enc, Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by `NormalEncoding(coords)`.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use `NormalEncoding(coords)` as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal surface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * `NormalEncoding(coords)`.  This will not be checked!
         *
         * \python The supported types for the template parameter \a U are
         * regina::Integer and regina::LargeInteger.  You may also, if you
         * prefer, pass \a vector as a Python list of integers.
         *
         * \tparam U the type of object held by the given vector.  It must be
         * possible to assign an object of type \a U to a regina::LargeInteger.
         *
         * \param triang the triangulation in which this normal surface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        template <typename U>
        NormalSurface(const Triangulation<3>& triang, NormalCoords coords,
            const Vector<U>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by `NormalEncoding(coords)`.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use `NormalEncoding(coords)` as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal surface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * `NormalEncoding(coords)`.  This will not be checked!
         *
         * \nopython Instead use the version that copies \a vector.
         *
         * \param triang the triangulation in which this normal surface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const Triangulation<3>& triang, NormalCoords coords,
            Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by `NormalEncoding(coords)`.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use `NormalEncoding(coords)` as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * `NormalEncoding(coords)`.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \tparam U the type of object held by the given vector.  It must be
         * possible to assign an object of type \a U to a regina::LargeInteger.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        template <typename U>
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalCoords coords, const Vector<U>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by `NormalEncoding(coords)`.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use `NormalEncoding(coords)` as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * `NormalEncoding(coords)`.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation
         * and copies \a vector.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalCoords coords, Vector<LargeInteger>&& vector);

        /**
         * Sets this to be a copy of the given normal surface.
         *
         * This and the given normal surface do not need to live in the same
         * underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then this surface will be adjusted
         * accordingly.
         *
         * This operator induces a deep copy of the given normal surface.
         *
         * \return a reference to this normal surface.
         */
        NormalSurface& operator = (const NormalSurface&) = default;

        /**
         * Moves the contents of the given normal surface to this surface.
         * This is a fast (constant time) operation.
         *
         * This and the given normal surface do not need to live in the same
         * underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then this surface will be adjusted
         * accordingly.
         *
         * The surface that was passed will no longer be usable.
         *
         * \return a reference to this normal surface.
         */
        NormalSurface& operator = (NormalSurface&&) noexcept = default;

        /**
         * Swaps the contents of this and the given normal surface.
         * This is a fast (constant time) operation.
         *
         * This and the given normal surface do not need to live in the
         * same underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then the two surfaces will be adjusted
         * accordingly.
         *
         * \param other the normal surface whose contents should be swapped
         * with this.
         */
        void swap(NormalSurface& other) noexcept;

        /**
         * Deprecated routine that returns the double of this surface.
         *
         * \deprecated Normal surfaces can now be multiplied by integer
         * constants.  In particular, this routine has exactly the same
         * effect as multiplying the surface by 2.
         *
         * \return the double of this normal surface.
         */
        [[deprecated]] NormalSurface doubleSurface() const;

        /**
         * Returns the sum of this and the given surface.  This will combine
         * all triangles, quadrilaterals and/or octagons from both surfaces.
         *
         * The two surfaces do not need to use the same coordinate system
         * and/or internal vector encodings.  Moreover, the resulting
         * surface might well use an encoding different from both of these,
         * or even a hybrid encoding that does not come from one of
         * Regina's ready-made coordinate systems.
         *
         * \pre Both this and the given normal surface use the same
         * underlying triangulation.
         *
         * \param rhs the surface to sum with this.
         * \return the sum of both normal surfaces.
         */
        NormalSurface operator + (const NormalSurface& rhs) const;

        /**
         * Returns the given integer multiple of this surface.
         *
         * The resulting surface will use the same internal vector encoding
         * as this surface.
         *
         * \param coeff the coefficient to multiply this surface by;
         * this must be non-negative.
         * \return the resulting multiple of this surface.
         */
        NormalSurface operator * (const LargeInteger& coeff) const;

        /**
         * Converts this surface into the given integer multiple of itself.
         *
         * The internal vector encoding used by this surface will not change.
         *
         * \param coeff the coefficient to multiply this surface by;
         * this must be non-negative.
         * \return a reference to this surface.
         */
        NormalSurface& operator *= (const LargeInteger& coeff);

        /**
         * Converts this surface into its smallest positive rational multiple
         * with integer coordinates.
         *
         * Note that the scaling factor will be independent of which
         * internal vector encoding is used.  This is essentially because
         * integer quad coordinates (which are stored in every encoding)
         * and integer octagon coordinates (which are stored in every
         * almost normal encoding) are enough to guarantee integer triangle
         * coordinates (which might or might not be stored).
         *
         * \return the integer by which the original surface was divided
         * (i.e., the gcd of all normal coordinates in the original surface).
         * This will always be strictly positive.
         */
        LargeInteger scaleDown();

        /**
         * Returns the number of triangular discs of the given type in
         * this normal surface.
         * A triangular disc type is identified by specifying a
         * tetrahedron and a vertex of that tetrahedron that the
         * triangle surrounds.
         *
         * \param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested triangles reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * \param vertex the vertex of the given tetrahedron around
         * which the requested triangles lie; this should be between 0
         * and 3 inclusive.
         * \return the number of triangular discs of the given type.
         */
        LargeInteger triangles(size_t tetIndex, int vertex) const;

        /**
         * Returns the number of quadrilateral discs of the given
         * type in this normal surface.
         *
         * In each tetrahedron, there are three types of quadrilaterals,
         * defined by how they separate the four tetrahedron vertices into
         * two pairs.  Quadrilateral type \a i (for \a i = 0, 1 or 2)
         * is defined to separate edge \a i of the tetrahedron from edge
         * (5-\a i).  That is:
         *
         * - type 0 separates vertices 0,1 of the tetrahedron from vertices 2,3;
         * - type 1 separates vertices 0,2 of the tetrahedron from vertices 1,3;
         * - type 2 separates vertices 0,3 of the tetrahedron from vertices 1,2.
         *
         * \param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested quadrilaterals reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * \param quadType the type of this quadrilateral in the given
         * tetrahedron; this should be 0, 1 or 2, as described above.
         * \return the number of quadrilateral discs of the given type.
         */
        LargeInteger quads(size_t tetIndex, int quadType) const;
        /**
         * Returns the number of octagonal discs of the given type
         * in this normal surface.
         *
         * In each tetrahedron, there are three types of octagons,
         * defined by how they separate the four tetrahedron vertices into
         * two pairs.  Octagon type \a i (for \a i = 0, 1 or 2) is defined to
         * intersect edges \a i and (5-\a i) of the tetrahedron twice each,
         * and to intersect the remaining edges once each.  This means:
         *
         * - type 0 separates vertices 0,1 of the tetrahedron from vertices 2,3;
         * - type 1 separates vertices 0,2 of the tetrahedron from vertices 1,3;
         * - type 2 separates vertices 0,3 of the tetrahedron from vertices 1,2.
         *
         * \param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octagons reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * \param octType the type of this octagon in the given tetrahedron;
         * this should be 0, 1 or 2, as described above.
         * \return the number of octagonal discs of the given type.
         */
        LargeInteger octs(size_t tetIndex, int octType) const;
        /**
         * Returns the number of times this normal surface crosses the
         * given edge.
         *
         * \param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Triangulation<3>::countEdges()-1 inclusive.
         * \return the number of times this normal surface crosses the
         * given edge.
         */
        LargeInteger edgeWeight(size_t edgeIndex) const;
        /**
         * Returns the number of arcs in which this normal surface
         * intersects the given triangle in the given direction.
         *
         * \param triIndex the index in the triangulation of the triangle
         * in which we are interested; this should be between 0 and
         * Triangulation<3>::countTriangles()-1 inclusive.
         * \param triVertex the vertex of the triangle (0, 1 or 2) around
         * which the arcs of intersection that we are interested in lie;
         * only these arcs will be counted.
         * \return the number of times this normal surface intersect the
         * given triangle with the given arc type.
         */
        LargeInteger arcs(size_t triIndex, int triVertex) const;

        /**
         * Determines the first coordinate position at which this surface
         * has a non-zero octagonal coordinate.  In other words, if this
         * routine returns the disc type \a t, then the octagonal
         * coordinate returned by octs(t.tetIndex, t.type) is non-zero.
         * Here DiscType::type represents an octagon type within a
         * tetrahedron, and takes values between 0 and 2 inclusive.
         *
         * If this surface does not contain any octagons, this routine
         * returns a null disc type instead.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.  Moreover, if the underlying coordinate
         * system does not support almost normal surfaces, then even the
         * first call is fast (it returns a null disc type immediately).
         *
         * \return the position of the first non-zero octagonal coordinate,
         * or a null disc type if there is no such coordinate.
         */
        DiscType octPosition() const;

        /**
         * Returns the triangulation in which this normal surface resides.
         *
         * This will be a snapshot frozen in time of the triangulation
         * that was originally passed to the NormalSurface constructor.
         *
         * This will return a correct result even if the original triangulation
         * has since been modified or destroyed.  However, in order to ensure
         * this behaviour, it is possible that at different points in time
         * this function may return references to different C++ objects.
         *
         * The rules for using the triangulation() reference are:
         *
         * - Do not keep the resulting reference as a long-term reference or
         *   pointer of your own, since in time you may find yourself referring
         *   to the wrong object (see above).  Just call this function again.
         *
         * - You must respect the read-only nature of the result (i.e.,
         *   you must not cast the constness away).  The snapshotting
         *   process detects modifications, and modifying the frozen
         *   snapshot may result in an exception being thrown.
         *
         * \return a reference to the underlying triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * \return the name of associated with this surface.
         */
        const std::string& name() const;
        /**
         * Sets the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * \param name the new name to associate with this surface.
         */
        void setName(const std::string& name);

        /**
         * Writes this surface to the given output stream, using
         * standard triangle-quad-oct coordinates.  Octagonal coordinates
         * will only be written if the surface is stored using an encoding
         * that supports almost normal surfaces.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this normal surface and all
         * of its properties.  This routine will be called from within
         * NormalSurfaces::writeXMLPacketData().
         *
         * \python The argument \a out should be an open Python file
         * object.
         *
         * \param out the output stream to which the XML should be written.
         * \param format indicates which of Regina's XML file formats to write.
         * \param list the enclosing normal surface list.  Currently this
         * is only relevant when writing to the older FileFormat::XmlGen2
         * format; it will be ignored (and may be \c null) for newer file
         * formats.
         */
        void writeXMLData(std::ostream& out, FileFormat format,
            const NormalSurfaces* list) const;

        /**
         * Determines if this normal surface is empty (has no discs
         * whatsoever).
         */
        bool isEmpty() const;
        /**
         * Determines if this normal surface has more than one octagonal disc.
         * It may be assumed that at most one octagonal disc _type_ exists in
         * this surface.  This routine will return \c true if an octagonal
         * type does exist and its coordinate is greater than one.
         *
         * \pre At most one octagonal disc _type_ exists in this surface.
         *
         * \return \c true if and only if there is an octagonal disc type
         * present and its coordinate is greater than one.
         */
        bool hasMultipleOctDiscs() const;
        /**
         * Determines if this normal surface is compact (has
         * finitely many discs).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \return \c true if and only if this normal surface is compact.
         */
        bool isCompact() const;
        /**
         * Returns the Euler characteristic of this surface.
         *
         * For properly embedded surfaces, this is of course just the ordinary
         * Euler characteristic of the surface.
         *
         * For immersed or singular surfaces, the situation is more complex
         * since Regina does not know how many branch points there are (if any).
         * Regina's approach is to compute everything locally, assuming that
         * the surface is an immersion.  This means that eulerChar() will
         * report the correct result for an immersed surface, but for singular
         * surfaces it will report a _larger_ number than it should since it
         * essentially counts each branch point as multiple vertices.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \return the Euler characteristic.
         */
        LargeInteger eulerChar() const;
        /**
         * Returns whether or not this surface is orientable.
         *
         * For our purposes, the empty surface is considered to be orientable.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \warning This routine explicitly builds the normal discs,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this surface is orientable, or \c false if
         * this surface is non-orientable.
         */
        bool isOrientable() const;
        /**
         * Returns whether or not this surface is two-sided.
         *
         * For our purposes, the empty surface is considered to be two-sided.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \warning This routine explicitly builds the normal discs,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this surface is two-sided, or \c false if
         * this surface is one-sided.
         */
        bool isTwoSided() const;
        /**
         * Returns whether or not this surface is connected.
         *
         * For our purposes, the empty surface is considered to be connected.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \warning This routine explicitly builds the normal discs,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this surface is connected, or \c false if
         * this surface is disconnected.
         */
        bool isConnected() const;
        /**
         * Determines if this surface has any real boundary, that is,
         * whether it meets any boundary triangles of the triangulation.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \return \c true if and only if this surface has real boundary.
         */
        bool hasRealBoundary() const;

        /**
         * Splits this surface into connected components.
         *
         * A list of connected components will be returned.
         * These components will always be encoded using standard
         * (tri-quad or tri-quad-oct) coordinates, regardless of the
         * internal vector encoding that is used by this surface.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \warning This routine explicitly builds the normal discs,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return the list of connected components.
         */
        std::vector<NormalSurface> components() const;

        /**
         * Determines whether or not this surface is vertex linking.
         * A _vertex linking_ surface contains only triangles.
         *
         * This behaves differently from isVertexLink(), which only detects
         * the link of a single vertex (or a multiple of such a link).
         * In contrast, this routine will also detect the union of
         * several _different_ vertex links.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return \c true if and only if this surface is vertex linking.
         */
        bool isVertexLinking() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * surface is the link of a single vertex.
         *
         * This behaves differently from isVertexLinking(), which will also
         * detect a union of several different vertex links.  In contrast,
         * this routine will only identify the link of a _single_ vertex
         * (or a multiple of such a link).
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return the vertex linked by a positive rational multiple of this
         * surface,
         * or \c null if this surface is not a multiple of a single vertex link.
         */
        const Vertex<3>* isVertexLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * surface is the thin link of a single edge.
         *
         * Here a _thin_ edge link is a normal surface which appears naturally
         * as the frontier of a regular neighbourhood of an edge, with no need
         * for any further normalisation.
         *
         * This behaves differently from isNormalEdgeLink(), which tests for a
         * _normalised_ edge link (which could end up far away from the
         * edge, or could be normalised into a surface with different
         * topology, or could even be normalised away to nothing).
         * Although isNormalEdgeLink() will also indicate thin edge links,
         * this test has significantly less overhead (and so should be faster).
         *
         * A surface (or its positive rational multiple) can be the _thin_ edge
         * link of at most two edges.  If there are indeed two different edges
         * \a e1 and \a e2 for which a multiple of this surface can be expressed
         * as the thin edge link, then the pair (\a e1, \a e2) will be returned.
         * If there is only one such edge \a e, then the pair (\a e, \c null)
         * will be returned.  If no positive rational multiple of this surface
         * is the thin link of any edge, then the pair (\c null, \c null) will
         * be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a pair containing the edge(s) linked by a positive rational
         * multiple of this surface, as described above.
         */
        std::pair<const Edge<3>*, const Edge<3>*> isThinEdgeLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * surface is the normalised link of a single edge.
         *
         * Here the phrase _normalised_ link of an edge \a e means the
         * frontier of a regular neighbourhood of \a e, converted into a
         * normal surface by expanding away from the edge using the
         * normalisation process.  It could be that there is no normalisation
         * required at all (in which case it is also a _thin_ edge link).
         * However, it could be that the normalisation process expands
         * the surface far away from the edge itself, or changes its
         * topology, or disconnects the surface, or even normalises it
         * away to an empty surface.
         *
         * In particular, this test behaves differently from isThinEdgeLink(),
         * which tests for thin edge links only (where no additional
         * normalisation is required).  If you are only interested in thin
         * edge links, then you should call isThinEdgeLink(), which has much
         * less overhead.
         *
         * A surface (or its positive rational multiple) could be the
         * normalised link of many edges.  The return value will be a pair
         * (\a v, \a thin), where:
         *
         * - \a v is a vector containing all such edges.  This will begin
         *   with the edges for which this surface is a thin link, followed by
         *   the edges where normalisation was required; within each category
         *   the edges will be ordered by their index within the triangulation.
         *
         * - \a thin is either 0, 1 or 2, indicating how many edges this
         *   surface is a thin link for.  This uses an unsigned type, since
         *   it will often be compared to `v.size()`.
         *
         * If no positive rational multiple of this surface is the normalised
         * link of any edge, then \a link will be 0 and \a v will be the
         * empty vector.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a vector containing the edge(s) linked by a positive rational
         * multiple of this surface and an integer indicating how many
         * of these links are thin, as described above.
         */
        std::pair<std::vector<const Edge<3>*>, unsigned> isNormalEdgeLink()
            const;
        /**
         * Determines whether or not a positive rational multiple of this
         * surface is the thin link of a single triangle.
         *
         * Here a _thin_ triangle link is a normal surface which appears
         * naturally as the frontier of a regular neighbourhood of a
         * triangle, with no need for any further normalisation.
         *
         * This behaves differently from isNormalTriangleLink(), which tests
         * for a _normalised_ triangle link (which could end up far away from
         * the triangle, or could be normalised into a surface with different
         * topology, or could even be normalised away to nothing).  Unlike the
         * tests for edge links, the routines isThinTriangleLink() and
         * isNormalTriangleLink() use essentially the same implementation (so
         * testing for only thin links may be a little faster, but not by much).
         *
         * A surface (or its positive rational multiple) can be the _thin_ link
         * of at most two triangles.  If there are indeed two different
         * triangles \a t1 and \a t2 for which a multiple of this surface can
         * be expressed as the thin triangle link, then the pair (\a t1, \a t2)
         * will be returned.  If there is only one such triangle \a t, then the
         * pair (\a t, \c null) will be returned.  If no positive rational
         * multiple of this surface is the thin link of any triangle, then the
         * pair (\c null, \c null) will be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a pair containing the triangle(s) linked by a positive
         * rational multiple of this surface, as described above.
         */
        std::pair<const Triangle<3>*, const Triangle<3>*> isThinTriangleLink()
            const;
        /**
         * Determines whether or not a positive rational multiple of this
         * surface is the normalised link of a single triangle.
         *
         * Here the phrase _normalised_ link of a triangle \a t means the
         * frontier of a regular neighbourhood of \a t, converted into a
         * normal surface by expanding away from the triangle using the
         * normalisation process.  It could be that there is no normalisation
         * required at all (in which case it is also a _thin_ triangle link).
         * However, it could be that the normalisation process expands
         * the surface far away from the triangle itself, or changes its
         * topology, or disconnects the surface, or even normalises it
         * away to an empty surface.
         *
         * In particular, this test behaves differently from
         * isThinTriangleLink(), which tests for thin triangle links only
         * (where no additional normalisation is required).  Unlike the
         * tests for edge links, the routines isThinTriangleLink() and
         * isNormalTriangleLink() use essentially the same implementation (so
         * testing for only thin links may be a little faster, but not by much).
         *
         * A surface (or its positive rational multiple) could be the
         * normalised link of many triangles.  The return value will be a pair
         * (\a v, \a thin), where:
         *
         * - \a v is a vector containing all such triangles.  This will begin
         *   with the triangles for which this surface is a thin link, followed
         *   by the triangles where normalisation was required; within each
         *   category the triangles will be ordered by their index within the
         *   triangulation.
         *
         * - \a thin is either 0, 1 or 2, indicating how many triangles this
         *   surface is a thin link for.  This uses an unsigned type, since
         *   it will often be compared to `v.size()`.
         *
         * If no positive rational multiple of this surface is the normalised
         * link of any triangle, then \a link will be 0 and \a v will be the
         * empty vector.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a vector containing the triangle(s) linked by a positive
         * rational multiple of this surface and an integer indicating how many
         * of these links are thin, as described above.
         */
        std::pair<std::vector<const Triangle<3>*>, unsigned>
            isNormalTriangleLink() const;
        /**
         * Determines whether or not this surface is a splitting surface.
         * A \a splitting surface is a compact surface containing
         * precisely one quad per tetrahedron and no other normal (or
         * almost normal) discs.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return \c true if and only if this is a splitting surface.
         */
        bool isSplitting() const;
        /**
         * Determines whether or not this surface is a central surface.
         * A \a central surface is a compact surface containing
         * at most one normal or almost normal disc per tetrahedron.
         * If this surface is central, the number of tetrahedra that it meets
         * (i.e., the number of discs in the surface) will be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return the number of tetrahedra that this surface meets if it
         * is a central surface, or 0 if it is not a central surface.
         */
        size_t isCentral() const;

        /**
         * Returns the number of disjoint boundary curves on this surface.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         *
         * \warning This routine explicitly builds all of the normal arcs on
         * the boundary.  If the normal coordinates are extremely large, this
         * could lead to performance problems.  In extreme cases where these
         * arc counts cannot even fit into the relevant native C++ integer
         * type, this routine will throw an exception (see below).
         *
         * \author Alex He
         *
         * \exception UnsolvedCase This surface has so many normal arcs on the
         * boundary that it will be impossible to explicitly build these arcs
         * in memory.  Specifically, this means that some arc count cannot fit
         * into a native C++ \c size_t; in practice on a typical 64-bit machine
         * this means that some arc count is at least `2^64`.
         *
         * \return the number of disjoint boundary curves.
         */
        size_t countBoundaries() const;

        /**
         * Determines whether this surface represents a compressing disc
         * in the underlying 3-manifold.
         *
         * Let this surface be \a D and let the underlying 3-manifold
         * be \a M with boundary \a B.  To be a compressing disc, \a D must
         * be a properly embedded disc in \a M, and the boundary of \a D
         * must not bound a disc in \a B.
         *
         * The implementation of this routine is somewhat inefficient at
         * present, since it cuts along the disc, retriangulates and then
         * examines the resulting boundary components.
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * \todo \opt Reimplement this to avoid cutting along surfaces.
         * \todo \prob Check for absurdly large numbers of discs and bail
         * accordingly.
         *
         * \warning This routine might cut along the surface and
         * retriangulate, and so may run out of memory if the normal
         * coordinates are extremely large.
         *
         * \param knownConnected \c true if this normal surface is
         * already known to be connected (for instance, if it came from
         * an enumeration of vertex normal surfaces), or \c false if
         * we should not assume any such information about this surface.
         * \return \c true if this surface is a compressing disc, or \c false if
         * this surface is not a compressing disc.
         */
        bool isCompressingDisc(bool knownConnected = false) const;

        /**
         * Determines whether this is an incompressible surface within
         * the surrounding 3-manifold.  At present, this routine is only
         * implemented for surfaces embedded within _closed_ and
         * _irreducible_ 3-manifold triangulations.
         *
         * Let \a D be some disc embedded in the underlying 3-manifold,
         * and let \a B be the boundary of \a D.  We call \a D a
         * _compressing disc_ for this surface if (i) the intersection
         * of \a D with this surface is the boundary \a B, and
         * (ii) although \a B bounds a disc within the 3-manifold, it
         * does not bound a disc within this surface.
         *
         * We declare this surface to be _incompressible_ if there are
         * no such compressing discs.  For our purposes, spheres are never
         * considered incompressible (so if this surface is a sphere then
         * this routine will always return \c false).
         *
         * This test is designed exclusively for two-sided surfaces.
         * If this surface is one-sided, the incompressibility test will
         * be run on its two-sided double cover.
         *
         * \warning This routine may in some circumstances be extremely slow.
         * This is because the underlying algorithm cuts along this surface,
         * retriangulates (possibly using a very large number of tetrahedra),
         * and then searches for a normal compressing disc in each
         * component of the cut-open triangulation.
         *
         * \pre The underlying triangulation is valid and closed, and
         * represents an irreducible 3-manifold.
         * \pre This normal surface is compact, embedded and connected,
         * and contains no octagonal discs.
         *
         * \return \c true if this surface is incompressible, or \c false if
         * this surface is not incompressible (or if it is a sphere).
         */
        bool isIncompressible() const;

        /**
         * Cuts the underlying triangulation along this surface and
         * returns the result as a new triangulation.  The original
         * triangulation is not changed.
         *
         * Note that, unlike crushing a surface to a point, this
         * operation will not change the topology of the underlying
         * 3-manifold beyond simply slicing along this surface.
         *
         * As of Regina 7.1, this routine can happily cut along _almost_
         * normal surfaces as well as normal surfaces.  That is, it can
         * now handle octagons, including cases with multiple octagons in the
         * same tetrahedron and/or octagons in multiple tetrahedra.
         *
         * This operation does _not_ preserve orientation.  That is,
         * even if the original triangulation is oriented, there are no
         * guarantees regarding the orientation of the tetrahedra in the
         * cut-open triangulation.  The cut-open triangulation might have the
         * same or opposite orientation as the original, or it might not be
         * oriented at all.  If you need to preserve orientation, consider
         * whether crush() (which _does_ preserve orientation) might work for
         * you instead.
         *
         * This routine will ignore any locks on tetrahedra and/or triangles
         * of the original triangulation (and of course the original
         * triangulation will be left safely unchanged).  The triangulation
         * that is returned will not have any locks at all.
         *
         * \warning The number of tetrahedra in the new triangulation
         * can be _very_ large.
         *
         * \pre This normal surface is compact and embedded.
         *
         * \return the resulting cut-open triangulation.
         */
        Triangulation<3> cutAlong() const;

        /**
         * Crushes this surface to a point in the underlying triangulation
         * and returns the result as a new triangulation.  The original
         * triangulation is not changed.
         *
         * Crushing the surface will produce a number of tetrahedra,
         * triangular pillows and/or footballs.  The pillows and
         * footballs will then be flattened to triangles and edges
         * respectively (resulting in the possible
         * changes mentioned below) to produce a proper triangulation.
         *
         * Note that the new triangulation will have at most the same
         * number of tetrahedra as the old triangulation, and will have
         * strictly fewer tetrahedra if this surface is not vertex
         * linking.
         *
         * The act of flattening pillows and footballs as described
         * above can lead to unintended topological side-effects, beyond
         * the effects of merely cutting along this surface and
         * identifying the new boundary surface(s) to points.
         * Examples of these unintended side-effects can include
         * connected sum decompositions, removal of 3-spheres and
         * small Lens spaces and so on; a full list of possible changes
         * is beyond the scope of this API documentation.
         *
         * If the original triangulation is oriented, then the crushing
         * operation will preserve this orientation.
         *
         * This routine will ignore any locks on tetrahedra and/or triangles
         * of the original triangulation (and of course the original
         * triangulation will be left safely unchanged).  The triangulation
         * that is returned will not have any locks at all.
         *
         * \warning This routine can have unintended topological
         * side-effects, as described above.
         *
         * \warning In exceptional cases with non-orientable
         * 3-manifolds, these side-effects might lead to invalid edges
         * (edges whose midpoints are projective plane cusps).
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * \return the resulting crushed triangulation.
         */
        Triangulation<3> crush() const;

        /**
         * Returns an isotopic normal surface that contains no octagons,
         * placed within a homeomorphic (but possibly different) triangulation.
         *
         * Specifically: this routine returns a surface \a s within a
         * triangulation \a t, where \a t is homeomorphic to the triangulation
         * containing this surface, and where \a s is a normal (not
         * almost normal) surface isotopic to this.  Only the surface \a s is
         * returned; you can access \a t by calling `s.triangulation()`.
         *
         * If this surface is already normal (i.e., it does not contain
         * any octagons), then the surface returned will simply be a copy
         * of this surface (but possibly using a different vector encoding),
         * living within the same triangulation.
         *
         * If this surface does contain octagons, then the triangulation \a t
         * will be obtained from the original by replacing each tetrahedron
         * containing octagons with _three_ tetrahedra (essentially
         * performing a 0-2 move).  Each octagon can then be subdivided
         * into a quadrilateral and four triangles.  If the original
         * triangulation is oriented, then the new triangulation \a t will
         * preserve this orientation.
         *
         * In all cases, the surface that is returned will use a vector
         * encoding that does not store octagons.
         *
         * Note: this routine can happily cope with multiple octagons in the
         * same tetrahedron, and/or multiple tetrahedra containing octagons.
         *
         * This routine ignores tetrahedron locks entirely. If there are
         * octagons present and the triangulation does change, then the new
         * triangulation will have no tetrahedron or triangle locks at all.
         * In particular, such locks will not stop this routine from
         * subdividing those tetrahedra that contain octagons.
         * Of course, the original triangulation will remain untouched,
         * with all of its locks intact.
         *
         * \pre This surface is embedded.
         *
         * \return an isotopic normal (not almost normal) surface \a s,
         * as described above.
         */
        NormalSurface removeOcts() const;

        /**
         * Determines whether this and the given surface in fact
         * represent the same normal (or almost normal) surface.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal or almost normal discs of each type, and returns \c true
         * if and only if these counts are the same for both surfaces.
         *
         * It does not matter what vector encodings the two surfaces
         * use.  In particular, it does not matter if the two surfaces
         * use different encodings, or if one but not the other supports
         * almost normal and/or spun-normal surfaces.
         *
         * This routine is safe to call even if this and the given
         * surface do not belong to the same triangulation:
         *
         * - If the two triangulations have the same size, then this routine
         *   will test whether this surface, if transplanted into the
         *   other triangulation using the same tetrahedron numbering and the
         *   same normal disc types, would be the same as \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c false.
         *
         * \param other the surface to be compared with this surface.
         * \return \c true if both surfaces represent the same normal or
         * almost normal surface, or \c false if not.
         */
        bool operator == (const NormalSurface& other) const;

        /**
         * Compares this against the given surface under a total
         * ordering of all normal and almost normal surfaces.
         *
         * This ordering is not mathematically meaningful; it is merely
         * provided for scenarios where you need to be able to sort
         * surfaces (e.g., when using them as keys in a map).
         *
         * The order _is_ well-defined, and will be preserved across
         * copy/move operations, different program executions, and different
         * platforms (since it is defined purely in terms of the normal
         * coordinates, and does not use transient properties such as
         * locations in memory).
         *
         * This operation is consistent with the equality test.  In particular,
         * it does not matter whether the two surfaces belong to different
         * triangulations, or use different encodings, or if one but not the
         * other supports non-compact or almost normal surfaces.
         * See the equality test operator==() for further details.
         *
         * This routine generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the surface to compare this surface with.
         * \return The result of the comparison between this and the given
         * surface.  This is marked as a weak ordering (not a strong
         * ordering) to reflect the fact that (for example) surfaces in
         * different triangulations or using different encodings could be
         * considered equal under this comparison.
         */
        std::weak_ordering operator <=> (const NormalSurface& rhs) const;

        /**
         * Determines whether this surface contains only triangle and/or
         * quadrilateral discs.  This is to distinguish normal surfaces
         * from more general surfaces such as almost normal surfaces
         * (which also contain octagonal pieces).
         *
         * Even if the underlying vector encoding supports other disc types
         * (such as octagons), this routine will still return \c true if this
         * particular surface does not use them.  This is in contrast to the
         * routine NormalSurfaces::allowsAlmostNormal(), which only examines
         * the underlying coordinate system.
         *
         * \return \c true if and only if this surface contains only
         * triangles and/or quadrilaterals.
         */
        bool normal() const;

        /**
         * Determines whether this surface is embedded.  This is true if
         * and only if the surface contains no conflicting quadrilateral
         * and/or octagon types.
         *
         * \return \c true if and only if this surface is embedded.
         */
        bool embedded() const;

        /**
         * Determines whether this and the given surface are locally compatible.
         * Local compatibility means that, within each individual tetrahedron
         * of the triangulation, it is possible to arrange the normal
         * discs of both surfaces so that none intersect.
         *
         * This is a local constraint, not a global constraint.  That is,
         * we do not insist that we can avoid intersections within all
         * tetrahedra _simultaneously_.  To test the global constraint,
         * see the (much slower) routine disjoint() instead.
         *
         * Local compatibility can be formulated in terms of normal disc types.
         * Two normal (or almost normal) surfaces are locally compatible if
         * and only if they together have at most one quadrilateral or
         * octagonal disc type per tetrahedron.
         *
         * Note again that this is a local constraint only.  In particular,
         * for almost normal surfaces, it does _not_ insist that there is
         * at most one octagonal disc type anywhere within the triangulation.
         *
         * If one of the two surfaces breaks the local compatibility
         * constraints on its own (for instance, it contains two different
         * quadrilateral disc types within the same tetrahedron), then this
         * routine will return \c false regardless of what the other surface
         * contains.
         *
         * \pre Both this and the given normal surface live within the
         * same 3-manifold triangulation.
         *
         * \param other the other surface to test for local compatibility with
         * this surface.
         * \return \c true if the two surfaces are locally compatible, or
         * \c false if they are not.
         */
        bool locallyCompatible(const NormalSurface& other) const;

        /**
         * Determines whether this and the given surface can be placed
         * within the surrounding triangulation so that they do not intersect
         * anywhere at all, without changing either normal isotopy class.
         *
         * This is a global constraint, and therefore gives a stronger test
         * than locallyCompatible().  However, this global constraint is
         * also much slower to test; the running time is proportional to
         * the total number of normal discs in both surfaces.
         *
         * \pre Both this and the given normal surface live within the
         * same 3-manifold triangulation.
         * \pre Both this and the given surface are compact (have
         * finitely many discs), embedded, non-empty and connected.
         *
         * \warning This routine is slow, since it performs a depth-first
         * search over the entire set of normal discs.
         *
         * \param other the other surface to test alongside this surface
         * for potential intersections.
         * \return \c true if both surfaces can be embedded without
         * intersecting anywhere, or \c false if this and the given
         * surface are forced to intersect at some point.
         */
        bool disjoint(const NormalSurface& other) const;

        /**
         * Computes the information about the boundary slopes of this surface
         * at each cusp of the triangulation.  This is for use with spun-normal
         * surfaces (since for closed surfaces all boundary slopes are zero).
         *
         * This routine is only available for use with SnapPea triangulations,
         * since it needs to know the specific meridian and longitude on each
         * cusp.  These meridians and longitudes are _only_ available through
         * the SnapPea kernel, since Regina does not use or store peripheral
         * curves for its own Triangulation<3> class.  Therefore:
         *
         * - If the underlying triangulation (as returned by triangulation())
         *   is not of the subclass SnapPeaTriangulation, this routine will
         *   throw an exception (see below).
         *
         * - In particular, this _will_ happen if you have edited or deleted
         *   the original triangulation that was used to construct this normal
         *   surface.  This is because such a modification will trigger an
         *   internal deep copy of the original, and this will only copy
         *   Regina's native Triangulation<3> data.
         *
         * All cusps are treated as complete.  That is, any Dehn fillings
         * stored in the SnapPea triangulation will be ignored.
         *
         * The results are returned in a matrix with \a V rows and two
         * columns, where \a V is the number of vertices in the triangulation.
         * If row \a i of the matrix contains the integers
         * \a M and \a L, this indicates that at the <i>i</i>th cusp,
         * the boundary curves have algebraic intersection number \a M
         * with the meridian and \a L with the longitude.  Equivalently,
         * the boundary curves pass \a L times around the meridian and
         * <i>-M</i> times around the longitude.
         * The rational boundary slope is therefore `-L/M`, and
         * there are `gcd(L,M)` boundary curves with this slope.
         *
         * The orientations of the boundary curves of a
         * spun-normal surface are chosen so that _if_ meridian and
         * longitude are a positive basis as vieved from the cusp, then
         * as one travels along an oriented boundary curve, the
         * spun-normal surface spirals into the cusp to one's right and
         * down into the manifold to one's left.
         *
         * If the triangulation contains more than one vertex, the rows
         * in the resulting matrix are ordered by cusp index (as stored by
         * SnapPea).  You can call SnapPeaTriangulation::cuspVertex() to
         * map these to Regina's vertex indices if needed.
         *
         * \pre As noted above, the underlying triangulation must be a
         * SnapPeaTriangulation; this will be checked, and this routine
         * will throw an exception if this requirement is not met.
         *
         * \pre At present, Regina can only compute boundary slopes if the
         * triangulation is oriented, if every vertex link in the
         * triangulation is a torus, and if the underlying coordinate system
         * is for normal surfaces only (not almost normal surfaces).
         * These conditions will likewise be checked, and this routine
         * will throw an exception if they are not met.
         *
         * \exception SnapPeaIsNull This is a null SnapPea triangulation.
         *
         * \exception FailedPrecondition One or more of the preconditions
         * listed above was not met.
         *
         * \author William Pettersson and Stephan Tillmann
         *
         * \return a matrix with \a number_of_vertices rows and two columns
         * as described above.
         */
        Matrix<Integer> boundaryIntersections() const;

        /**
         * Gives read-only access to the integer vector that Regina uses
         * internally to represent this surface.
         *
         * Note that this vector might not use the same coordinate system
         * in which the surfaces were originally enumerated.  (For example,
         * this vector will always include triangle coordinates, even if
         * the surfaces were originally enumerated in quad or quad-oct
         * coordinates.)  You can call encoding() to find out precisley
         * how the coordinates of this vector should be interpreted.
         *
         * See the NormalSurface class notes for information on how this
         * vector is structured.
         *
         * \note If you wish to access the numbers of triangles, quads and
         * so on, you should use the functions triangles(), quads(), etc.,
         * which do not require any knowledge of the internal vector
         * encoding that this surface uses.
         *
         * \return the underlying integer vector.
         */
        const Vector<LargeInteger>& vector() const;

        /**
         * Returns the specific integer vector encoding that this surface
         * uses internally.  This is the encoding that should be used
         * to interpret vector().
         *
         * Note that this might differ from the encoding originally
         * passed to the class constructor.
         *
         * \return the internal vector encoding.
         */
        NormalEncoding encoding() const;

        /**
         * Indicates whether the internal vector encoding for this
         * surface supports almost normal surfaces.
         *
         * If this routine returns \c true, it does not mean that the surface
         * actually _contains_ one or more octagons; you should use normal()
         * to test for that.  This routine simply queries a basic property
         * of the vector encoding that is being used, and this property is
         * often inherited from whatever coordinate system was used to
         * perform the normal surface enumeration.
         *
         * On the other hand, if this routine returns \c false, it is a
         * guarantee that this surface is normal.
         *
         * \return \c true if the internal encoding supports almost
         * normal surfaces.
         */
        bool couldBeAlmostNormal() const;

        /**
         * Indicates whether the internal vector encoding for this
         * surface supports non-compact surfaces.  Non-compact surfaces
         * are surfaces that contain infinitely many discs
         * (i.e., spun-normal surfaces).
         *
         * If this routine returns \c true, it does not mean that the surface
         * actually _is_ non-compact; you should use isCompact() to test for
         * that.  This routine simply queries a basic property of the vector
         * encoding that is being used, and this property is often inherited
         * from whatever coordinate system was used to perform the normal
         * surface enumeration.
         *
         * On the other hand, if this routine returns \c false, it is a
         * guarantee that this surface is compact.
         *
         * \return \c true if the internal encoding supports almost
         * normal surfaces.
         */
        bool couldBeNonCompact() const;

        /**
         * Reconstructs the triangle coordinates in the given integer vector.
         *
         * The given vector must represent a normal surface within the
         * given triangulation, using the given vector encoding.
         *
         * - If the given encoding does not already store triangle coordinates,
         *   then the vector will be modified directly to use a new encoding
         *   that does, and this new encoding will be returned.
         *
         * - If the given encoding does already store triangles, then
         *   this routine will do nothing and immediately return \a enc.
         *
         * \param tri the triangulation in which the normal surface lives.
         * \param vector an integer vector that encodes a normal (or almost
         * normal) surface within \a tri; this will be modified directly.
         * \param enc the encoding used by the given integer vector.
         * \return the new encoding used by the modified \a vector.
         */
        static NormalEncoding reconstructTriangles(const Triangulation<3>& tri,
            Vector<LargeInteger>& vector, NormalEncoding enc);

    private:
        /**
         * Calculates the position of the first non-zero octagon
         * coordinate and stores it as a property.
         */
        void calculateOctPosition() const;

        /**
         * Calculates the Euler characteristic of this surface and
         * stores it as a property.
         *
         * \pre This normal surface is compact (has finitely many discs).
         */
        void calculateEulerChar() const;
        /**
         * Calculates whether this surface is orientable and/or
         * two-sided and stores the results as properties.
         *
         * \pre This normal surface is embedded (not singular or immersed).
         * \pre This normal surface is compact (has finitely many discs).
         */
        void calculateOrientable() const;
        /**
         * Calculates whether this surface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary() const;
        /**
         * Computes the number of disjoint boundary curves and stores the
         * result as a property.
         *
         * \exception IntegerOverflow Some normal arc count does not fit into a
         * standard C++ \c size_t.  Be aware that this exception will need to
         * be converted to something more user-friendly before being passed on
         * to the end user.
         */
        void calculateBoundaries() const;

        /**
         * Determines whether or not a positive rational multiple of this
         * surface _could_ be the normalised link of a face of positive
         * dimension.
         *
         * A non-null return value is _not_ a guarantee that this surface
         * _is_ such a link; however, if this routine returns \nullopt then
         * this _is_ a guarantee that the surface is not such a link.
         *
         * The precise tests that this routine carries out involve a trade-off
         * between speed and mathematical power, and so are subject to change
         * in future versions of Regina.
         *
         * \pre This surface is non-empty.
         *
         * \return the precise multiple of this surface that _could_ be a
         * normalised non-vertex face link, or \nullopt if we can prove
         * that this surface is not such a link.
         */
        std::optional<NormalSurface> couldLinkFace() const;

    friend class XMLNormalSurfaceReader;
};

/**
 * Swaps the contents of the given normal surfaces.
 * This is a fast (constant time) operation.
 *
 * This global routine simply calls NormalSurface::swap(); it is provided
 * so that NormalSurface meets the C++ Swappable requirements.
 *
 * \param a the first normal surface whose contents should be swapped.
 * \param b the second normal surface whose contents should be swapped.
 *
 * \ingroup surface
 */
void swap(NormalSurface& a, NormalSurface& b) noexcept;

// Inline functions for NormalSurface

template <typename U>
inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalEncoding enc, const Vector<U>& vector) :
        enc_(enc), vector_(vector), triangulation_(tri) {
    // This call to storesTriangles() is unnecessary, but we'd like it
    // accessible to the inline version.  (Same goes for the similar
    // occurrences of storesTriangles() in the constructors below.)
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(tri, vector_, enc_);
}

inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalEncoding enc, Vector<LargeInteger>&& vector) :
        enc_(enc), vector_(std::move(vector)), triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(tri, vector_, enc_);
}

template <typename U>
inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalEncoding enc, const Vector<U>& vector) :
        enc_(enc), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(*tri, vector_, enc_);
}

inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalEncoding enc, Vector<LargeInteger>&& vector) :
        enc_(enc), vector_(std::move(vector)), triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(*tri, vector_, enc_);
}

template <typename U>
inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalCoords coords, const Vector<U>& vector) :
        enc_(coords), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(tri, vector_, enc_);
}

inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalCoords coords, Vector<LargeInteger>&& vector) :
        enc_(coords), vector_(std::move(vector)),
        triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(tri, vector_, enc_);
}

template <typename U>
inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalCoords coords, const Vector<U>& vector) :
        enc_(coords), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(*tri, vector_, enc_);
}

inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalCoords coords, Vector<LargeInteger>&& vector) :
        enc_(coords), vector_(std::move(vector)),
        triangulation_(tri) {
    if (! enc_.storesTriangles())
        enc_ = reconstructTriangles(*tri, vector_, enc_);
}

// NOLINTNEXTLINE(modernize-pass-by-value)
inline NormalSurface::NormalSurface(const NormalSurface& src,
        const Triangulation<3>& triangulation) :
        NormalSurface(src) {
    // We will happily accept one redundant SnapshotRef assignment as the
    // cost of removing many lines of code.
    triangulation_ = triangulation;
}

// NOLINTNEXTLINE(modernize-pass-by-value)
inline NormalSurface::NormalSurface(const NormalSurface& src,
        const SnapshotRef<Triangulation<3>>& triangulation) :
        NormalSurface(src) {
    // We will happily accept one redundant SnapshotRef assignment as the
    // cost of removing many lines of code.
    triangulation_ = triangulation;
}

inline void NormalSurface::swap(NormalSurface& other) noexcept {
    std::swap(enc_, other.enc_);
    vector_.swap(other.vector_);
    triangulation_.swap(other.triangulation_);

    name_.swap(other.name_);
    octPosition_.swap(other.octPosition_);
    eulerChar_.swap(other.eulerChar_);
    boundaries_.swap(other.boundaries_);
    orientable_.swap(other.orientable_);
    twoSided_.swap(other.twoSided_);
    connected_.swap(other.connected_);
    realBoundary_.swap(other.realBoundary_);
    compact_.swap(other.compact_);
    std::swap(linkOf_, other.linkOf_);
}

inline LargeInteger NormalSurface::triangles(size_t tetIndex, int vertex)
        const {
    return vector_[enc_.block() * tetIndex + vertex];
}
inline LargeInteger NormalSurface::quads(size_t tetIndex, int quadType) const {
    return vector_[enc_.block() * tetIndex + 4 + quadType];
}
inline LargeInteger NormalSurface::octs(size_t tetIndex, int octType) const {
    if (! enc_.storesOctagons())
        return 0;
    else
        return vector_[enc_.block() * tetIndex + 7 + octType];
}

inline DiscType NormalSurface::octPosition() const {
    if (! octPosition_.has_value())
        calculateOctPosition();
    return *octPosition_;
}

inline const Triangulation<3>& NormalSurface::triangulation() const {
    return *triangulation_;
}

inline const std::string& NormalSurface::name() const {
    return name_;
}
inline void NormalSurface::setName(const std::string& name) {
    name_ = name;
}

inline LargeInteger NormalSurface::eulerChar() const {
    if (! eulerChar_.has_value())
        calculateEulerChar();
    return *eulerChar_;
}

inline bool NormalSurface::isEmpty() const {
    // All vector encodings store the empty surface as the zero vector.
    return vector_.isZero();
}

inline bool NormalSurface::isOrientable() const {
    if (! orientable_.has_value())
        calculateOrientable();
    return *orientable_;
}

inline bool NormalSurface::isTwoSided() const {
    if (! twoSided_.has_value())
        calculateOrientable();
    return *twoSided_;
}

inline bool NormalSurface::isConnected() const {
    if (! connected_.has_value())
        calculateOrientable();
    return *connected_;
}

inline bool NormalSurface::hasRealBoundary() const {
    if (! realBoundary_.has_value())
        calculateRealBoundary();
    return *realBoundary_;
}

inline size_t NormalSurface::countBoundaries() const {
    if (! boundaries_.has_value()) {
        try {
            calculateBoundaries();
        } catch (const IntegerOverflow&) {
            throw UnsolvedCase("This surface has too many boundary arcs "
                "for this computation to proceed");
        }
    }
    return *boundaries_;
}

inline bool NormalSurface::normal() const {
    return ! octPosition();
}

inline const Vector<LargeInteger>& NormalSurface::vector() const {
    return vector_;
}

inline NormalEncoding NormalSurface::encoding() const {
    return enc_;
}

inline bool NormalSurface::couldBeAlmostNormal() const {
    return enc_.storesOctagons();
}

inline bool NormalSurface::couldBeNonCompact() const {
    return enc_.couldBeNonCompact();
}

inline NormalSurface NormalSurface::doubleSurface() const {
    return (*this) * 2;
}

inline void swap(NormalSurface& a, NormalSurface& b) noexcept {
    a.swap(b);
}

} // namespace regina

// If we haven't yet seen the full definition of Triangulation<3>, include it
// now - the SnapshotRef constructor (used inline above) needs it.
#include "triangulation/dim3.h"

#endif

