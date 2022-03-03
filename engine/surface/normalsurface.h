
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
#include "maths/perm.h"
#include "maths/vector.h"
#include "packet/packet.h"
#include "surface/disctype.h"
#include "surface/normalcoords.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/snapshot.h"

namespace regina {

/**
 * \defgroup surfaces Normal Surfaces
 * Normal surfaces in 3-manifold triangulations.
 */

class NormalSurfaces;

/**
 * Lists which quadrilateral types separate which pairs of vertices in a
 * tetrahedron.
 * As outlined in NormalSurface::quads(), there are three quadrilateral types
 * in a tetrahedron, numbered 0, 1 and 2.  Each quadrilateral type separates
 * the four tetrahedron vertices 0,1,2,3 into two pairs.
 * <tt>quadSeparating[i][j]</tt> is the number of the quadrilateral type that
 * keeps vertices <tt>i</tt> and <tt>j</tt> together.
 *
 * It is guaranteed that quadrilateral type \a i will keep the vertices of
 * edge \a i together (and will therefore also keep the vertices of edge
 * \a 5-i together).
 *
 * \ingroup surfaces
 */
inline constexpr int quadSeparating[4][4] = {
    { -1, 0, 1, 2 }, {  0,-1, 2, 1 }, {  1, 2,-1, 0 }, {  2, 1, 0,-1 }
};

/**
 * Lists which quadrilateral types meet which edges in a tetrahedron.
 * See regina::quadSeparating and NormalSurface::quads() for more
 * information on quadrilateral types.
 *
 * <tt>quadMeeting[i][j][0,1]</tt> are the numbers of the two
 * quadrilateral types that meet the edge joining tetrahedron vertices
 * <tt>i</tt> and <tt>j</tt>.
 *
 * \ingroup surfaces
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
 * <tt>quadDefn[i][0,1]</tt> and
 * <tt>quadDefn[i][2,3]</tt>.
 *
 * It is guaranteed that:
 *
 * - <tt>quadDefn[i][0] < quadDefn[i][1]</tt>;
 * - <tt>quadDefn[i][2] < quadDefn[i][3]</tt>;
 * - <tt>quadDefn[i][0] < quadDefn[i][2]</tt>.
 *
 * This array contains similar information to the function Edge<3>::ordering().
 * Instead of quadDefn[\a i][\a j], you can call Edge<3>::ordering(\a i)[\a j];
 * this will give the same results for \a j = 0 and 1, but it might
 * switch the results for \a j = 2 and 3.
 *
 * \ingroup surfaces
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
 * vertex <tt>quadPartner[i][v]</tt>.
 *
 * \ingroup surfaces
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
 * The string describing quadrilateral type \c i is <tt>quadString[i]</tt> and
 * is of the form <tt>02/13</tt>, which in this case is the quadrilateral type
 * that splits vertices 0,2 from vertices 1,3.
 *
 * \ingroup surfaces
 */
inline constexpr char quadString[3][6] = { "01/23", "02/13", "03/12" };

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of triangular normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>triDiscArcs[i]</tt> lists the boundary arcs of the
 * triangular disc of type <i>i</i>.  See NormalSurface::triangles()
 * for further details.
 *
 * Note that every permutation in this array is even.
 *
 * \ingroup surfaces
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
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>quadDiscArcs[i]</tt> lists the boundary arcs of the
 * quadrilateral disc of type <i>i</i>.  See NormalSurface::quads()
 * for further details.
 *
 * Note that permutation <tt>quadDiscArcs[i][j]</tt> will be even
 * precisely when <tt>j</tt> is even.
 *
 * \ingroup surfaces
 */
inline constexpr Perm<4> quadDiscArcs[3][4] = {
    Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(1,3,2,0), Perm<4>(2,1,0,3),
    Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(2,1,3,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(3,2,1,0), Perm<4>(1,3,0,2)
};

/**
 * Lists in consecutive order the directed normal arcs that form the
 * boundary of each type of octagonal normal disc.  Each permutation \a p
 * represents an arc about vertex <tt>p[0]</tt> parallel to the directed
 * edge from <tt>p[1]</tt> to <tt>p[2]</tt>.
 *
 * Array <tt>octDiscArcs[i]</tt> lists the boundary arcs of the
 * octagonal disc of type <i>i</i>.  See NormalSurface::octs()
 * for further details.
 *
 * Note that permutation <tt>octDiscArcs[i][j]</tt> will be even
 * precisely when <tt>j</tt> is 0, 1, 4 or 5.
 *
 * \ingroup surfaces
 */
inline constexpr Perm<4> octDiscArcs[3][8] = {
    Perm<4>(0,3,1,2), Perm<4>(0,1,2,3), Perm<4>(2,0,3,1), Perm<4>(2,3,1,0),
    Perm<4>(1,2,0,3), Perm<4>(1,0,3,2), Perm<4>(3,1,2,0), Perm<4>(3,2,0,1),
    Perm<4>(0,1,2,3), Perm<4>(0,2,3,1), Perm<4>(3,0,1,2), Perm<4>(3,1,2,0),
    Perm<4>(2,3,0,1), Perm<4>(2,0,1,3), Perm<4>(1,2,3,0), Perm<4>(1,3,0,2),
    Perm<4>(0,2,3,1), Perm<4>(0,3,1,2), Perm<4>(1,0,2,3), Perm<4>(1,2,3,0),
    Perm<4>(3,1,0,2), Perm<4>(3,0,2,1), Perm<4>(2,3,1,0), Perm<4>(2,1,0,3)
};

template <typename, bool> class Matrix;
using MatrixInt = Matrix<Integer, true>;

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
 * \ingroup surfaces
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
         * @param src the normal surface to copy.
         * @param triangulation the triangulation in which this new surface
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
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation.
         *
         * @param src the normal surface to copy.
         * @param triangulation a snapshot, frozen in time, of the
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
         * \ifacespython Instead of a Vector<LargeInteger>, you may (if
         * you prefer) pass a Python list of integers.
         *
         * @param triang the triangulation in which this normal surface resides.
         * @param enc indicates precisely how the given vector encodes a normal
         * surface.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const Triangulation<3>& triang, NormalEncoding enc,
            const Vector<LargeInteger>& vector);

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
         * \ifacespython Not present, but you can use the version that
         * copies \a vector.
         *
         * @param triang the triangulation in which this normal surface resides.
         * @param enc indicates precisely how the given vector encodes a normal
         * surface.
         * @param vector a vector containing the coordinates of the normal
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
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation.
         *
         * @param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * @param enc indicates precisely how the given vector encodes a normal
         * surface.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalEncoding enc, const Vector<LargeInteger>& vector);

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
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation and copies \a vector.
         *
         * @param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * @param enc indicates precisely how the given vector encodes a normal
         * surface.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalEncoding enc, Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by <tt>NormalEncoding(coords)</tt>.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use <tt>NormalEncoding(coords)</tt> as its internal encoding method.
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
         * <tt>NormalEncoding(coords)</tt>.  This will not be checked!
         *
         * \ifacespython Instead of a Vector<LargeInteger>, you may (if
         * you prefer) pass a Python list of integers.
         *
         * @param triang the triangulation in which this normal surface resides.
         * @param coords the coordinate system from which the vector
         * encoding will be deduced.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const Triangulation<3>& triang, NormalCoords coords,
            const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by <tt>NormalEncoding(coords)</tt>.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use <tt>NormalEncoding(coords)</tt> as its internal encoding method.
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
         * <tt>NormalEncoding(coords)</tt>.  This will not be checked!
         *
         * \ifacespython Not present, but you can use the version that
         * copies \a vector.
         *
         * @param triang the triangulation in which this normal surface resides.
         * @param coords the coordinate system from which the vector
         * encoding will be deduced.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const Triangulation<3>& triang, NormalCoords coords,
            Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by <tt>NormalEncoding(coords)</tt>.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use <tt>NormalEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * <tt>NormalEncoding(coords)</tt>.  This will not be checked!
         *
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation.
         *
         * @param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * @param coords the coordinate system from which the vector
         * encoding will be deduced.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalCoords coords, const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal surface inside the given triangulation with the
         * given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this surface uses the vector encoding described
         * by <tt>NormalEncoding(coords)</tt>.  Be careful with this if you
         * are extracting the vector from some other normal surface, since
         * Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this surface will
         * use <tt>NormalEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include triangle coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * surface inside the given triangulation, using the encoding
         * <tt>NormalEncoding(coords)</tt>.  This will not be checked!
         *
         * \ifacespython Not present, but you can use the version that
         * takes a "pure" triangulation and copies \a vector.
         *
         * @param triang a snapshot, frozen in time, of the
         * triangulation in which this normal surface resides.
         * @param coords the coordinate system from which the vector
         * encoding will be deduced.
         * @param vector a vector containing the coordinates of the normal
         * surface.
         */
        NormalSurface(const SnapshotRef<Triangulation<3>>& triang,
            NormalCoords coords, Vector<LargeInteger>&& vector);

        /**
         * Deprecated routine that creates a newly allocated clone of this
         * normal surface.
         *
         * The name of the normal surface will \e not be copied to the
         * clone; instead the clone will have an empty name.
         *
         * \deprecated Simply use the copy constructor instead.
         *
         * @return a clone of this normal surface.
         */
        [[deprecated]] NormalSurface* clone() const;

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
         * @return a reference to this normal surface.
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
         * @return a reference to this normal surface.
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
         * @param other the normal surface whose contents should be swapped
         * with this.
         */
        void swap(NormalSurface& other) noexcept;

        /**
         * Returns the double of this surface.
         *
         * @return the double of this normal surface.
         */
        NormalSurface doubleSurface() const;

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
         * @param rhs the surface to sum with this.
         * @return the sum of both normal surfaces.
         */
        NormalSurface operator + (const NormalSurface& rhs) const;

        /**
         * Returns the number of triangular discs of the given type in
         * this normal surface.
         * A triangular disc type is identified by specifying a
         * tetrahedron and a vertex of that tetrahedron that the
         * triangle surrounds.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested triangles reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * @param vertex the vertex of the given tetrahedron around
         * which the requested triangles lie; this should be between 0
         * and 3 inclusive.
         * @return the number of triangular discs of the given type.
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
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested quadrilaterals reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * @param quadType the type of this quadrilateral in the given
         * tetrahedron; this should be 0, 1 or 2, as described above.
         * @return the number of quadrilateral discs of the given type.
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
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested octagons reside;
         * this should be between 0 and Triangulation<3>::size()-1 inclusive.
         * @param octType the type of this octagon in the given tetrahedron;
         * this should be 0, 1 or 2, as described above.
         * @return the number of octagonal discs of the given type.
         */
        LargeInteger octs(size_t tetIndex, int octType) const;
        /**
         * Returns the number of times this normal surface crosses the
         * given edge.
         *
         * @param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Triangulation<3>::countEdges()-1 inclusive.
         * @return the number of times this normal surface crosses the
         * given edge.
         */
        LargeInteger edgeWeight(size_t edgeIndex) const;
        /**
         * Returns the number of arcs in which this normal surface
         * intersects the given triangle in the given direction.
         *
         * @param triIndex the index in the triangulation of the triangle
         * in which we are interested; this should be between 0 and
         * Triangulation<3>::countTriangles()-1 inclusive.
         * @param triVertex the vertex of the triangle (0, 1 or 2) around
         * which the arcs of intersection that we are interested in lie;
         * only these arcs will be counted.
         * @return the number of times this normal surface intersect the
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
         * @return the position of the first non-zero octagonal coordinate,
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
         * @return a reference to the underlying triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * @return the name of associated with this surface.
         */
        const std::string& name() const;
        /**
         * Sets the name associated with this normal surface.
         * Names are optional and need not be unique.
         * The default name for a surface is the empty string.
         *
         * @param name the new name to associate with this surface.
         */
        void setName(const std::string& name);

        /**
         * Writes this surface to the given output stream, using
         * standard triangle-quad-oct coordinates.  Octagonal coordinates
         * will only be written if the surface is stored using an encoding
         * that supports almost normal surfaces.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Deprecated routine that writes the underlying coordinate vector
         * to the given output stream in text format.
         * No indication will be given as to which coordinate
         * system is being used or what each coordinate means.
         * No newline will be written.
         *
         * \deprecated Just write vector() directly to the output stream.
         *
         * \ifacespython Not present; instead just write vector() to the
         * appropriate output stream.
         *
         * @param out the output stream to which to write.
         */
        [[deprecated]] void writeRawVector(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this normal surface and all
         * of its properties.  This routine will be called from within
         * NormalSurfaces::writeXMLPacketData().
         *
         * \ifacespython The argument \a out should be an open Python file
         * object.
         *
         * @param out the output stream to which the XML should be written.
         * @param format indicates which of Regina's XML file formats to write.
         * @param list the enclosing normal hypersurface list.  Currently this
         * is only relevant when writing to the older REGINA_XML_GEN_2 format;
         * it will be ignored (and may be \c null) for newer file formats.
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
         * It may be assumed that at most one octagonal disc \e type exists in
         * this surface.  This routine will return \c true if an octagonal
         * type does exist and its coordinate is greater than one.
         *
         * \pre At most one octagonal disc \e type exists in this surface.
         *
         * @return \c true if and only if there is an octagonal disc type
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
         * @return \c true if and only if this normal surface is compact.
         */
        bool isCompact() const;
        /**
         * Returns the Euler characteristic of this surface.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal surface is compact (has finitely many discs).
         *
         * @return the Euler characteristic.
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
         * @return \c true if this surface is orientable, or \c false if
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
         * @return \c true if this surface is two-sided, or \c false if
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
         * @return \c true if this surface is connected, or \c false if
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
         * @return \c true if and only if this surface has real boundary.
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
         * @return the list of connected components.
         */
        std::vector<NormalSurface> components() const;

        /**
         * Determines whether or not this surface is vertex linking.
         * A <i>vertex linking</i> surface contains only triangles.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this surface is vertex linking.
         */
        bool isVertexLinking() const;
        /**
         * Determines whether or not a rational multiple of this surface
         * is the link of a single vertex.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return the vertex linked by this surface, or \c null if this
         * surface is not the link of a single vertex.
         */
        const Vertex<3>* isVertexLink() const;
        /**
         * Determines whether or not a rational multiple of this surface
         * is the thin link of a single edge.
         *
         * If there are two different edges <i>e1</i> and <i>e2</i> for
         * which this surface could be expressed as the thin link of
         * either <i>e1</i> or <i>e2</i>, the pair
         * (<i>e1</i>, <i>e2</i>) will be returned.
         * If this surface is the thin link of only one edge <i>e</i>,
         * the pair (<i>e</i>, \c null) will be returned.
         * If this surface is not the thin link of any edges, the pair
         * (\c null, \c null) will be returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \todo \opt Cache results.
         *
         * @return a pair containing the edge(s) linked by this surface,
         * as described above.
         */
        std::pair<const Edge<3>*, const Edge<3>*> isThinEdgeLink() const;
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
         * \todo \opt Cache results.
         *
         * @return \c true if and only if this is a splitting surface.
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
         * \todo \opt Cache results.
         *
         * @return the number of tetrahedra that this surface meets if it
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
         * \warning This routine explicitly builds the normal arcs on
         * the boundary.  If the normal coordinates are extremely large,
         * (in particular, of a similar order of magnitude as the
         * largest possible long integer), then the behaviour of this
         * routine is undefined.
         *
         * @author Alex He
         *
         * @return the number of disjoint boundary curves.
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
         * @param knownConnected \c true if this normal surface is
         * already known to be connected (for instance, if it came from
         * an enumeration of vertex normal surfaces), or \c false if
         * we should not assume any such information about this surface.
         * @return \c true if this surface is a compressing disc, or \c false if
         * this surface is not a compressing disc.
         */
        bool isCompressingDisc(bool knownConnected = false) const;

        /**
         * Determines whether this is an incompressible surface within
         * the surrounding 3-manifold.  At present, this routine is only
         * implemented for surfaces embedded within \e closed and
         * \e irreducible 3-manifold triangulations.
         *
         * Let \a D be some disc embedded in the underlying 3-manifold,
         * and let \a B be the boundary of \a D.  We call \a D a
         * <i>compressing disc</i> for this surface if (i) the intersection
         * of \a D with this surface is the boundary \a B, and
         * (ii) although \a B bounds a disc within the 3-manifold, it
         * does not bound a disc within this surface.
         *
         * We declare this surface to be \e incompressible if there are
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
         * @return \c true if this surface is incompressible, or \c false if
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
         * \warning The number of tetrahedra in the new triangulation
         * can be <i>very</i> large.
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * @return the resulting cut-open triangulation.
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
         * \warning This routine can have unintended topological
         * side-effects, as described above.
         * \warning In exceptional cases with non-orientable
         * 3-manifolds, these side-effects might lead to invalid edges
         * (edges whose midpoints are projective plane cusps).
         *
         * \pre This normal surface is compact and embedded.
         * \pre This normal surface contains no octagonal discs.
         *
         * @return the resulting crushed triangulation.
         */
        Triangulation<3> crush() const;

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
         * @param other the surface to be compared with this surface.
         * @return \c true if both surfaces represent the same normal or
         * almost normal surface, or \c false if not.
         */
        bool operator == (const NormalSurface& other) const;

        /**
         * Determines whether this and the given surface represent
         * different normal (or almost normal) surfaces.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal or almost normal discs of each type, and returns \c true
         * if and only if these counts are not the same for both surfaces.
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
         *   same normal disc types, would be different from \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c true.
         *
         * @param other the surface to be compared with this surface.
         * @return \c true if both surfaces represent different normal or
         * almost normal surface, or \c false if not.
         */
        bool operator != (const NormalSurface& other) const;

        /**
         * Imposes a total order on all normal and almost normal surfaces.
         *
         * This order is not mathematically meaningful; it is merely
         * provided for scenarios where you need to be able to sort
         * surfaces (e.g., when using them as keys in a map).
         *
         * The order \e is well-defined, and will be preserved across
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
         * @param other the surface to be compared with this surface.
         * @return \c true if and only if this appears before the given
         * surface in the total order.
         */
        bool operator < (const NormalSurface& other) const;

        /**
         * Deprecated routine that determines whether this and the given
         * surface in fact represent the same normal (or almost normal) surface.
         *
         * \deprecated This routine has been renamed to the comparison
         * operator (==).
         *
         * @param other the surface to be compared with this surface.
         * @return \c true if both surfaces represent the same normal or
         * almost normal surface, or \c false if not.
         */
        [[deprecated]] bool sameSurface(const NormalSurface& other) const;

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
         * @return \c true if and only if this surface contains only
         * triangles and/or quadrilaterals.
         */
        bool normal() const;

        /**
         * Determines whether this surface is embedded.  This is true if
         * and only if the surface contains no conflicting quadrilateral
         * and/or octagon types.
         *
         * @return \c true if and only if this surface is embedded.
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
         * tetrahedra \e simultaneously.  To test the global constraint,
         * see the (much slower) routine disjoint() instead.
         *
         * Local compatibility can be formulated in terms of normal disc types.
         * Two normal (or almost normal) surfaces are locally compatible if
         * and only if they together have at most one quadrilateral or
         * octagonal disc type per tetrahedron.
         *
         * Note again that this is a local constraint only.  In particular,
         * for almost normal surfaces, it does \e not insist that there is
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
         * @param other the other surface to test for local compatibility with
         * this surface.
         * @return \c true if the two surfaces are locally compatible, or
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
         * @param other the other surface to test alongside this surface
         * for potential intersections.
         * @return \c true if both surfaces can be embedded without
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
         * cusp.  These meridians and longitudes are \e only available through
         * the SnapPea kernel, since Regina does not use or store peripheral
         * curves for its own Triangulation<3> class.  Therefore:
         *
         * - If the underlying triangulation (as returned by triangulation())
         *   is not of the subclass SnapPeaTriangulation, this routine will
         *   throw an exception (see below).
         *
         * - In particular, this \e will happen if you have edited or deleted
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
         * The rational boundary slope is therefore <tt>-L/M</tt>, and
         * there are <tt>gcd(L,M)</tt> boundary curves with this slope.
         *
         * The orientations of the boundary curves of a
         * spun-normal surface are chosen so that \e if meridian and
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
         * \exception SnapPeaIsNull this is a null SnapPea triangulation.
         *
         * \exception FailedPrecondition one or more of the preconditions
         * listed above was not met.
         *
         * @author William Pettersson and Stephan Tillmann
         *
         * @return a matrix with \a number_of_vertices rows and two columns
         * as described above.
         */
        MatrixInt boundaryIntersections() const;

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
         * @return the underlying integer vector.
         */
        const Vector<LargeInteger>& vector() const;

        /**
         * A deprecated alias for vector().
         *
         * \deprecated This routine has been renamed to vector().
         *
         * @return the underlying integer vector.
         */
        [[deprecated]] const Vector<LargeInteger>& rawVector() const;

        /**
         * Returns the specific integer vector encoding that this surface
         * uses internally.  This is the encoding that should be used
         * to interpret vector().
         *
         * Note that this might differ from the encoding originally
         * passed to the class constructor.
         *
         * @return the internal vector encoding.
         */
        NormalEncoding encoding() const;

        /**
         * Indicates whether the internal vector encoding for this
         * surface supports almost normal surfaces.
         *
         * If this routine returns \c true, it does not mean that the surface
         * actually \e contains one or more octagons; you should use normal()
         * to test for that.  This routine simply queries a basic property
         * of the vector encoding that is being used, and this property is
         * often inherited from whatever coordinate system was used to
         * perform the normal surface enumeration.
         *
         * On the other hand, if this routine returns \c false, it is a
         * guarantee that this surface is normal.
         *
         * @return \c true if the internal encoding supports almost
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
         * actually \e is non-compact; you should use isCompact() to test for
         * that.  This routine simply queries a basic property of the vector
         * encoding that is being used, and this property is often inherited
         * from whatever coordinate system was used to perform the normal
         * surface enumeration.
         *
         * On the other hand, if this routine returns \c false, it is a
         * guarantee that this surface is compact.
         *
         * @return \c true if the internal encoding supports almost
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
         * @param tri the triangulation in which the normal surface lives.
         * @param vector an integer vector that encodes a normal (or almost
         * normal) surface within \a tri; this will be modified directly.
         * @param enc the encoding used by the given integer vector.
         * @return the new encoding used by the modified \a vector.
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
         */
        void calculateBoundaries() const;

    friend class XMLNormalSurfaceReader;
};

/**
 * Swaps the contents of the given normal surfaces.
 * This is a fast (constant time) operation.
 *
 * This global routine simply calls NormalSurface::swap(); it is provided
 * so that NormalSurface meets the C++ Swappable requirements.
 *
 * @param a the first normal surface whose contents should be swapped.
 * @param b the second normal surface whose contents should be swapped.
 *
 * \ingroup surfaces
 */
void swap(NormalSurface& a, NormalSurface& b) noexcept;

// Inline functions for NormalSurface

inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalEncoding enc, const Vector<LargeInteger>& vector) :
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

inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalEncoding enc, const Vector<LargeInteger>& vector) :
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

inline NormalSurface::NormalSurface(const Triangulation<3>& tri,
        NormalCoords coords, const Vector<LargeInteger>& vector) :
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

inline NormalSurface::NormalSurface(
        const SnapshotRef<Triangulation<3>>& tri,
        NormalCoords coords, const Vector<LargeInteger>& vector) :
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

inline NormalSurface* NormalSurface::clone() const {
    return new NormalSurface(*this);
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

inline void NormalSurface::writeRawVector(std::ostream& out) const {
    out << vector_;
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
    if (! boundaries_.has_value())
        calculateBoundaries();
    return *boundaries_;
}

inline bool NormalSurface::operator != (const NormalSurface& other) const {
    return ! ((*this) == other);
}

inline bool NormalSurface::sameSurface(const NormalSurface& other) const {
    return (*this) == other;
}

inline bool NormalSurface::normal() const {
    return ! octPosition();
}

inline const Vector<LargeInteger>& NormalSurface::vector() const {
    return vector_;
}

inline const Vector<LargeInteger>& NormalSurface::rawVector() const {
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

inline void swap(NormalSurface& a, NormalSurface& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

