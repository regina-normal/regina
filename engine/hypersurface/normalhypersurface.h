
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file hypersurface/normalhypersurface.h
 *  \brief Deals with an individual normal hypersurface in a 4-manifold
 *  triangulation.
 */

#ifndef __REGINA_NORMALHYPERSURFACE_H
#ifndef __DOXYGEN
#define __REGINA_NORMALHYPERSURFACE_H
#endif

#include <optional>
#include <utility>
#include "regina-core.h"
#include "core/output.h"
#include "algebra/abeliangroup.h"
#include "hypersurface/hypercoords.h"
#include "maths/vector.h"
#include "packet/packet.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/snapshot.h"

namespace regina {

class NormalHypersurfaces;

template <typename, bool> class Matrix;
using MatrixInt = Matrix<Integer, true>;

/**
 * \defgroup hypersurface Normal Hypersurfaces
 * Normal hypersurfaces in 4-manifold triangulations.
 */

/**
 * Represents a single normal hypersurface in a 4-manifold triangulation.
 *
 * The normal hypersurface is described internally by an integer vector
 * (discussed in more detail below).  Since different hypersurfaces may use
 * different vector encodings, you should not rely on the raw vector
 * entries unless absolutely necessary.  Instead, the query routines
 * such as tetrahedra(), prisms(), edgeWeight() and so on are independent
 * of the underlying vector encoding being used.
 *
 * Note that non-compact hypersurfaces (surfaces with infinitely many pieces)
 * are allowed; in these cases, the corresponding coordinate lookup routines
 * will return LargeInteger::infinity where appropriate.
 *
 * Since Regina 7.0, you can modify or even destroy the original
 * triangulation that was used to create this normal hypersurface.  If you do,
 * then this normal hypersurface will automatically make a private copy of
 * the original triangulation as an ongoing reference.  Different normal
 * hypersurfaces can all share the same private copy, so this is not an
 * expensive process.
 *
 * Internally, a normal hypersurface is represented by a Vector<LargeInteger>
 * (possibly using a different coordinate system from the one in which
 * the hypersurfaces were originally enumerated).  This contains a block of
 * coordinates for each pentachoron, in order from the first pentachoron
 * to the last.  Each block begins with five tetrahedron coordinates (always),
 * followed by ten prisms coordinates (always) - unlike the 3-dimensional
 * world, there are currently no optional coordinates that might or might not
 * be stored (though this could change in future versions of Regina).
 * Therefore the vector that is stored will always have length 15<i>n</i>,
 * where \a n is the number of pentachora in the underlying triangulation.
 *
 * When adding support for a new coordinate system:
 *
 * - The file hypercoords.h must be updated.  This includes a new enum
 *   value for HyperCoords, a new case for the HyperEncoding constructor,
 *   and new cases for the functions in HyperInfo.  Do not forget to
 *   update the python bindings for HyperCoords also.
 *
 * - The global routines makeEmbeddedConstraints() and makeMatchingEquations()
 *   should be updated to incorporate the new coordinate system.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup hypersurface
 */
class NormalHypersurface : public ShortOutput<NormalHypersurface> {
    protected:
        HyperEncoding enc_;
            /**< The specific encoding of a normal hypersurface used by the
                 coordinate vector. */
        Vector<LargeInteger> vector_;
            /**< Contains the coordinates of the normal hypersurface. */
        SnapshotRef<Triangulation<4>> triangulation_;
            /**< The triangulation in which this normal hypersurface resides. */

        std::string name_;
            /**< An optional name associated with this hypersurface. */

        mutable std::optional<bool> orientable_;
            /**< Is this hypersurface orientable?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> twoSided_;
            /**< Is this hypersurface two-sided?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> connected_;
            /**< Is this hypersurface connected?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> realBoundary_;
            /**< Does this hypersurface have real boundary (i.e. does it meet
                 any boundary facets)?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> compact_;
            /**< Is this hypersurface compact (i.e., does it only
                 contain finitely many pieces)?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<AbelianGroup> H1_;
            /**< First homology group of the hypersurface.
                 This is std::nullopt if it has not yet been computed. */
        mutable uint8_t linkOf_ { 0 };
            /**< Indicates which dimensions of face a positive rational multiple
                 of this hypersurface is a thin or normalised link of.  This is
                 treated as a bitmask: for each i=0,1,2,3, the (2i+1)th bit
                 indicates whether this hypersurface scales to the link of an
                 i-face, and the (2i)th bit indicates whether this information
                 has actually been computed yet; if it has not been computed,
                 then the (2i+1)th bit will be zero. */

    public:
        /**
         * Creates a new copy of the given normal hypersurface.
         */
        NormalHypersurface(const NormalHypersurface&) = default;

        /**
         * Creates a new copy of the given normal hypersurface, but
         * relocated to the given triangulation.
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal hypersurface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation in which
         * \a src resides.
         *
         * \param src the normal hypersurface to copy.
         * \param triangulation the triangulation in which this new
         * hypersurface will reside.
         */
        NormalHypersurface(const NormalHypersurface& src,
            const Triangulation<4>& triangulation);

        /**
         * Creates a new copy of the given normal hypersurface, but
         * relocated to the given triangulation.
         *
         * \pre The given triangulation is either the same as, or is
         * combinatorially identical to, the triangulation in which
         * \a src resides.
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param src the normal hypersurface to copy.
         * \param triangulation a snapshot, frozen in time, of the
         * triangulation in which this new hypersurface will reside.
         */
        NormalHypersurface(const NormalHypersurface& src,
            const SnapshotRef<Triangulation<4>>& triangulation);

        /**
         * Moves the given hypersurface into this new normal hypersurface.
         * This is a fast (constant time) operation.
         *
         * The hypersurface that is passed will no longer be usable.
         */
        NormalHypersurface(NormalHypersurface&&) noexcept = default;

        /**
         * Create the empty hypersurface within the given triangulation.
         *
         * All normal coordinates will be zero.
         *
         * \param triang the triangulation in which this normal hypersurface
         * resides.
         */
        NormalHypersurface(const Triangulation<4>& triang);

        /**
         * Create the empty hypersurface within the given triangulation.
         *
         * All normal coordinates will be zero.
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal hypersurface resides.
         */
        NormalHypersurface(const SnapshotRef<Triangulation<4>>& triang);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this hypersurface will keep the given
         * encoding: NormalHypersurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal hypersurface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the given
         * encoding.  This will not be checked!
         *
         * \ifacespython Instead of a Vector<LargeInteger>, you may (if
         * you prefer) pass a Python list of integers.
         *
         * \param triang the triangulation in which this normal hypersurface
         * resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * hypersurface.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const Triangulation<4>& triang, HyperEncoding enc,
            const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this hypersurface will keep the given
         * encoding: NormalHypersurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal hypersurface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the given
         * encoding.  This will not be checked!
         *
         * \nopython Instead use the version that copies \a vector.
         *
         * \param triang the triangulation in which this normal hypersurface
         * resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * hypersurface.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const Triangulation<4>& triang, HyperEncoding enc,
            Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this hypersurface will keep the given
         * encoding: NormalHypersurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the given
         * encoding.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal hypersurface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * hypersurface.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const SnapshotRef<Triangulation<4>>& triang,
            HyperEncoding enc, const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given vector encoding.
         *
         * There is no guarantee that this hypersurface will keep the given
         * encoding: NormalHypersurface will sometimes convert the vector to
         * use a different encoding for its own internal storage.
         *
         * Despite what is said in the class notes, it is okay if the
         * given vector encoding does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the given
         * encoding.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation
         * and copies \a vector.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal hypersurface resides.
         * \param enc indicates precisely how the given vector encodes a normal
         * hypersurface.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const SnapshotRef<Triangulation<4>>& triang,
            HyperEncoding enc, Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this hypersurface uses the vector encoding
         * described by <tt>HyperEncoding(coords)</tt>.  Be careful with this
         * if you are extracting the vector from some other normal hypersurface,
         * since Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this hypersurface will
         * use <tt>HyperEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal hypersurface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the encoding
         * <tt>HyperEncoding(coords)</tt>.  This will not be checked!
         *
         * \ifacespython Instead of a Vector<LargeInteger>, you may (if
         * you prefer) pass a Python list of integers.
         *
         * \param triang the triangulation in which this normal hypersurface
         * resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const Triangulation<4>& triang, HyperCoords coords,
            const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this hypersurface uses the vector encoding
         * described by <tt>HyperEncoding(coords)</tt>.  Be careful with this
         * if you are extracting the vector from some other normal hypersurface,
         * since Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this hypersurface will
         * use <tt>HyperEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * A snapshot will be taken of the given triangulation as it appears
         * right now.  You may change or even delete the triangulation later
         * on; if so, then this normal hypersurface will still refer to the
         * frozen snapshot that was taken at the time of construction.
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the encoding
         * <tt>HyperEncoding(coords)</tt>.  This will not be checked!
         *
         * \nopython Instead use the version that copies \a vector.
         *
         * \param triang the triangulation in which this normal hypersurface
         * resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const Triangulation<4>& triang, HyperCoords coords,
            Vector<LargeInteger>&& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this hypersurface uses the vector encoding
         * described by <tt>HyperEncoding(coords)</tt>.  Be careful with this
         * if you are extracting the vector from some other normal hypersurface,
         * since Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this hypersurface will
         * use <tt>HyperEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the encoding
         * <tt>HyperEncoding(coords)</tt>.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal hypersurface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const SnapshotRef<Triangulation<4>>& triang,
            HyperCoords coords, const Vector<LargeInteger>& vector);

        /**
         * Creates a new normal hypersurface inside the given triangulation
         * with the given coordinate vector, using the given coordinate system.
         *
         * It is assumed that this hypersurface uses the vector encoding
         * described by <tt>HyperEncoding(coords)</tt>.  Be careful with this
         * if you are extracting the vector from some other normal hypersurface,
         * since Regina may internally convert to use a different encoding from
         * whatever was used during enumeration and/or read from file.
         * In the same spirit, there is no guarantee that this hypersurface will
         * use <tt>HyperEncoding(coords)</tt> as its internal encoding method.
         *
         * Despite what is said in the class notes, it is okay if the
         * given coordinate system does not include tetrahedron coordinates.
         * (If this is the case, the vector will be converted automatically.)
         *
         * \pre The given coordinate vector does indeed represent a normal
         * hypersurface inside the given triangulation, using the encoding
         * <tt>HyperEncoding(coords)</tt>.  This will not be checked!
         *
         * \nopython Instead use the version that takes a "pure" triangulation
         * and copies \a vector.
         *
         * \param triang a snapshot, frozen in time, of the
         * triangulation in which this normal hypersurface resides.
         * \param coords the coordinate system from which the vector
         * encoding will be deduced.
         * \param vector a vector containing the coordinates of the normal
         * hypersurface.
         */
        NormalHypersurface(const SnapshotRef<Triangulation<4>>& triang,
            HyperCoords coords, Vector<LargeInteger>&& vector);

        /**
         * Sets this to be a copy of the given normal hypersurface.
         *
         * This and the given normal hypersurface do not need to live in the
         * same underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then this hypersurface will be adjusted
         * accordingly.
         *
         * This operator induces a deep copy of the given normal hypersurface.
         *
         * \return a reference to this normal hypersurface.
         */
        NormalHypersurface& operator = (const NormalHypersurface&) = default;

        /**
         * Moves the contents of the given normal hypersurface to this
         * hypersurface.  This is a fast (constant time) operation.
         *
         * This and the given normal hypersurface do not need to live in the
         * same underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then this hypersurface will be adjusted
         * accordingly.
         *
         * The hypersurface that was passed will no longer be usable.
         *
         * \return a reference to this normal hypersurface.
         */
        NormalHypersurface& operator = (NormalHypersurface&&) noexcept =
            default;

        /**
         * Swaps the contents of this and the given normal hypersurface.
         * This is a fast (constant time) operation.
         *
         * This and the given normal hypersurface do not need to live in the
         * same underlying triangulation, and they do not need to have the same
         * length vectors or use the same normal coordinate system - if any of
         * these properties differs then the two hypersurfaces will be adjusted
         * accordingly.
         *
         * \param other the normal hypersurface whose contents should be swapped
         * with this.
         */
        void swap(NormalHypersurface& other) noexcept;

        /**
         * Deprecated routine that returns the double of this hypersurface.
         *
         * \deprecated Normal hypersurfaces can now be multiplied by integer
         * constants.  In particular, this routine has exactly the same
         * effect as multiplying the hypersurface by 2.
         *
         * \return the double of this normal hypersurface.
         */
        [[deprecated]] NormalHypersurface doubleHypersurface() const;

        /**
         * Returns the sum of this and the given hypersurface.  This will
         * combine all tetrahedra and/or prisms from both surfaces.
         *
         * The two hypersurfaces do not need to use the same coordinate system
         * and/or internal vector encodings.  Moreover, the resulting
         * hypersurface might well use an encoding different from both of these,
         * or even a hybrid encoding that does not come from one of
         * Regina's ready-made coordinate systems.
         *
         * \pre Both this and the given normal hypersurface use the same
         * underlying triangulation.
         *
         * \param rhs the hypersurface to sum with this.
         * \return the sum of both normal hypersurfaces.
         */
        NormalHypersurface operator + (const NormalHypersurface& rhs) const;

        /**
         * Returns the given integer multiple of this hypersurface.
         *
         * The resulting hypersurface will use the same internal vector
         * encoding as this hypersurface.
         *
         * \param coeff the coefficient to multiply this hypersurface by;
         * this must be non-negative.
         * \return the resulting multiple of this hypersurface.
         */
        NormalHypersurface operator * (const LargeInteger& coeff) const;

        /**
         * Converts this hypersurface into the given integer multiple of itself.
         *
         * The internal vector encoding used by this hypersurface will not
         * change.
         *
         * \param coeff the coefficient to multiply this hypersurface by;
         * this must be non-negative.
         * \return a reference to this hypersurface.
         */
        NormalHypersurface& operator *= (const LargeInteger& coeff);

        /**
         * Converts this hypersurface into its smallest positive rational
         * multiple with integer coordinates.
         *
         * Note that the scaling factor will be independent of which
         * internal vector encoding is used.  This is essentially because
         * integer prism coordinates (which are stored in every encoding)
         * are enough to guarantee integer tetrahedron coordinates (which
         * might or might not be stored).
         *
         * \return the integer by which the original hypersurface was divided
         * (i.e., the gcd of all normal coordinates in the original
         * hypersurface).  This will always be strictly positive.
         */
        LargeInteger scaleDown();

        /**
         * Returns the number of tetrahedron pieces of the given type in
         * this normal hypersurface.
         * A tetrahedron piece type is identified by specifying a
         * pentachoron and a vertex of that pentachoron that the
         * tetrahedron surrounds.
         *
         * \param pentIndex the index in the triangulation of the
         * pentachoron in which the requested pieces reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * \param vertex the vertex of the given pentachoron around
         * which the requested pieces lie; this should be between 0
         * and 4 inclusive.
         * \return the number of tetrahedron pieces of the given type.
         */
        LargeInteger tetrahedra(size_t pentIndex, int vertex) const;
        /**
         * Returns the number of prism pieces of the given type
         * in this normal hypersurface.
         * A prism piece type is identified by specifying a
         * pentachoron and an edge of that pentachoron; prisms of this
         * type will then separate edge \a i of the pentachoron from
         * triangle \a i of the pentachoron.
         *
         * \param pentIndex the index in the triangulation of the
         * pentachoron in which the requested prisms reside;
         * this should be between 0 and Triangulation<4>::size()-1 inclusive.
         * \param prismType specifies the edge of the given pentachoron that
         * this prism separates from the opposite triangle;
         * this should be between 0 and 9 inclusive.
         * \return the number of prism pieces of the given type.
         */
        LargeInteger prisms(size_t pentIndex, int prismType) const;
        /**
         * Returns the number of times this normal hypersurface crosses the
         * given edge.
         *
         * \param edgeIndex the index in the triangulation of the edge
         * in which we are interested; this should be between 0 and
         * Triangulation<4>::countEdges()-1 inclusive.
         * \return the number of times this normal hypersurface crosses the
         * given edge.
         */
        LargeInteger edgeWeight(size_t edgeIndex) const;

        /**
         * Returns the triangulation in which this normal hypersurface resides.
         *
         * This will be a snapshot frozen in time of the triangulation
         * that was originally passed to the NormalHypersurface constructor.
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
        const Triangulation<4>& triangulation() const;

        /**
         * Returns the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * \return the name of associated with this hypersurface.
         */
        const std::string& name() const;
        /**
         * Sets the name associated with this normal hypersurface.
         * Names are optional and need not be unique.
         * The default name for a hypersurface is the empty string.
         *
         * \param name the new name to associate with this hypersurface.
         */
        void setName(const std::string& name);

        /**
         * Writes this hypersurface to the given output stream, using
         * standard tetrahedron-prism coordinates.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this normal hypersurface and all
         * of its properties.  This routine will be called from within
         * NormalHypersurfaces::writeXMLPacketData().
         *
         * \ifacespython The argument \a out should be an open Python file
         * object.
         *
         * \param out the output stream to which the XML should be written.
         * \param format indicates which of Regina's XML file formats to write.
         * \param list the enclosing normal hypersurface list.  Currently this
         * is only relevant when writing to the older REGINA_XML_GEN_2 format;
         * it will be ignored (and may be \c null) for newer file formats.
         */
        void writeXMLData(std::ostream& out, FileFormat format,
            const NormalHypersurfaces* list) const;

        /**
         * Determines if this normal hypersurface is empty (has no
         * pieces whatsoever).
         */
        bool isEmpty() const;
        /**
         * Determines if this normal hypersurface is compact (has
         * finitely many pieces).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \return \c true if and only if this normal hypersurface is compact.
         */
        bool isCompact() const;
        /**
         * Returns whether or not this hypersurface is orientable.
         *
         * For our purposes, the empty hypersurface is considered to be
         * orientable.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this hypersurface is orientable, or \c false if
         * this hypersurface is non-orientable.
         */
        bool isOrientable() const;
        /**
         * Returns whether or not this hypersurface is two-sided.
         *
         * For our purposes, the empty hypersurface is considered to be
         * two-sided.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this hypersurface is two-sided, or \c false if
         * this hypersurface is one-sided.
         */
        bool isTwoSided() const;
        /**
         * Returns whether or not this hypersurface is connected.
         *
         * For our purposes, the empty hypersurface is considered to be
         * connected.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return \c true if this hypersurface is connected, or \c false if
         * this hypersurface is disconnected.
         */
        bool isConnected() const;
        /**
         * Determines if this hypersurface has any real boundary, that is,
         * whether it meets any boundary tetrahedra of the triangulation.
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \return \c true if and only if this hypersurface has real boundary.
         */
        bool hasRealBoundary() const;
        /**
         * Determines whether or not this hypersurface is vertex linking.
         * A <i>vertex linking</i> hypersurface contains only tetrahedra.
         *
         * This behaves differently from isVertexLink(), which only detects
         * the link of a single vertex (or a multiple of such a link).
         * In contrast, this routine will also detect the union of
         * several \e different vertex links.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return \c true if and only if this hypersurface is vertex linking.
         */
        bool isVertexLinking() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the link of a single vertex.
         *
         * This behaves differently from isVertexLinking(), which will also
         * detect a union of several different vertex links.  In contrast,
         * this routine will only identify the link of a \e single vertex
         * (or a multiple of such a link).
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return the vertex linked by a positive rational multiple of this
         * hypersurface, or \c null if this hypersurface is not a multiple of
         * a single vertex link.
         */
        const Vertex<4>* isVertexLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the thin link of a single edge.
         *
         * Here a \e thin edge link is a normal hypersurface which appears
         * naturally as the frontier of a regular neighbourhood of an edge,
         * with no need for any further normalisation.
         *
         * This behaves differently from isNormalEdgeLink(), which tests for a
         * \e normalised edge link (which could end up far away from the
         * edge, or could be normalised into a hypersurface with different
         * topology, or could even be normalised away to nothing).
         * Although isNormalEdgeLink() will also indicate thin edge links,
         * this test has significantly less overhead (and so should be faster).
         *
         * A hypersurface (or its positive rational multiple) can be the thin
         * edge link of at most one edge.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return the edge linked by a positive rational multiple of this
         * hypersurface, or \c null if this hypersurface is not a multiple
         * of a single thin edge link.
         */
        const Edge<4>* isThinEdgeLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the normalised link of a single edge.
         *
         * Here the phrase \e normalised link of an edge \a e means the
         * frontier of a regular neighbourhood of \a e, converted into a
         * normal hypersurface by expanding away from the edge using some basic
         * normalisation moves.  It could be that there is no normalisation
         * required at all (in which case it is also a \e thin edge link).
         * However, it could be that the normalisation process expands
         * the hypersurface far away from the edge itself, or changes its
         * topology, or disconnects the hypersurface, or even normalises it
         * away to an empty hypersurface.
         *
         * In particular, this test behaves differently from isThinEdgeLink(),
         * which tests for thin edge links only (where no additional
         * normalisation is required).  If you are only interested in thin
         * edge links, then you should call isThinEdgeLink(), which has much
         * less overhead.
         *
         * A hypersurface (or its positive rational multiple) could be the
         * normalised link of many edges.  The return value will be a pair
         * (\a v, \a thin), where:
         *
         * - \a v is a vector containing all such edges.  This will begin with
         *   the edges for which this hypersurface is a thin link, followed by
         *   the edges where normalisation was required; within each category
         *   the edges will be ordered by their index within the triangulation.
         *
         * - \a thin is either 0 or 1, indicating how many edges this
         *   hypersurface is a thin link for.  This uses an unsigned type,
         *   since it will often be compared to <tt>v.size()</tt>.
         *
         * If no positive rational multiple of this hypersurface is the
         * normalised link of any edge, then \a link will be 0 and \a v will be
         * the empty vector.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a vector containing the edge(s) linked by a positive rational
         * multiple of this hypersurface and an integer indicating how many
         * of these links are thin, as described above.
         */
        std::pair<std::vector<const Edge<4>*>, unsigned> isNormalEdgeLink()
            const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the thin link of a single triangle.
         *
         * Here a \e thin triangle link is a normal hypersurface which appears
         * naturally as the frontier of a regular neighbourhood of a triangle,
         * with no need for any further normalisation.
         *
         * This behaves differently from isNormalTriangleLink(), which tests
         * for a \e normalised triangle link (which could end up far away from
         * the triangle, or could be normalised into a hypersurface with
         * different topology, or could even be normalised away to nothing).
         * Unlike the tests for edge links, the routines isThinTriangleLink()
         * and isNormalTriangleLink() use essentially the same implementation
         * (so testing for only thin links may be a little faster, but not by
         * much).
         *
         * A hypersurface (or its positive rational multiple) can be the thin
         * triangle link of at most two triangles.  If there are indeed two
         * different triangles \a t1 and \a t2 for which a multiple of this
         * hypersurface can be expressed as the thin triangle link, then
         * the pair (\a t1, \a t2) will be returned.  If there is only one
         * such triangle \a t, then the pair (\a t, \c null) will be returned.
         * If no positive rational multiple of this hypersurface is the thin
         * link of any triangle, then the pair (\c null, \c null) will be
         * returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a pair containing the triangle(s) linked by a positive
         * rational multiple of this hypersurface, as described above.
         */
        std::pair<const Triangle<4>*, const Triangle<4>*> isThinTriangleLink()
            const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the normalised link of a single triangle.
         *
         * Here the phrase \e normalised link of a triangle \a t means the
         * frontier of a regular neighbourhood of \a t, converted into a normal
         * hypersurface by expanding away from the triangle using some basic
         * normalisation moves.  It could be that there is no normalisation
         * required at all (in which case it is also a \e thin triangle link).
         * However, it could be that the normalisation process expands
         * the hypersurface far away from the triangle itself, or changes its
         * topology, or disconnects the hypersurface, or even normalises it
         * away to an empty hypersurface.
         *
         * In particular, this test behaves differently from
         * isThinTriangleLink(), which tests for thin triangle links only
         * (where no additional normalisation is required).  Unlike the
         * tests for edge links, the routines isThinTriangleLink() and
         * isNormalTriangleLink() use essentially the same implementation (so
         * testing for only thin links may be a little faster, but not by much).
         *
         * A hypersurface (or its positive rational multiple) could be the
         * normalised link of many triangles.  The return value will be a pair
         * (\a v, \a thin), where:
         *
         * - \a v is a vector containing all such triangles.  This will begin
         *   with the triangles for which this hypersurface is a thin link,
         *   followed by the triangles where normalisation was required;
         *   within each category the triangles will be ordered by their
         *   index within the triangulation.
         *
         * - \a thin is either 0, 1 or 2, indicating how many triangles this
         *   hypersurface is a thin link for.  This uses an unsigned type,
         *   since it will often be compared to <tt>v.size()</tt>.
         *
         * If no positive rational multiple of this hypersurface is the
         * normalised link of any triangle, then \a link will be 0 and \a v
         * will be the empty vector.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a vector containing the triangle(s) linked by a positive
         * rational multiple of this hypersurface and an integer indicating
         * how many of these links are thin, as described above.
         */
        std::pair<std::vector<const Triangle<4>*>, unsigned>
            isNormalTriangleLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the thin link of a single tetrahedron.
         *
         * Here a \e thin tetrahedron link is a normal hypersurface which
         * appears naturally as the frontier of a regular neighbourhood of a
         * tetrahedron, with no need for any further normalisation.
         *
         * This behaves differently from isNormalTetrahedronLink(), which tests
         * for a \e normalised tetrahedron link (which could end up far away
         * from the tetrahedron, or could be normalised into a hypersurface with
         * different topology, or could even be normalised away to nothing).
         * Unlike the tests for edge links, the routines isThinTetrahedronLink()
         * and isNormalTetrahedronLink() use essentially the same implementation
         * (so testing for only thin links may be a little faster, but not by
         * much).
         *
         * A hypersurface (or its positive rational multiple) can be the thin
         * link of at most two tetrahedra.  If there are indeed two different
         * tetrahedra \a t1 and \a t2 for which a multiple of this
         * hypersurface can be expressed as the thin tetrahedron link, then
         * the pair (\a t1, \a t2) will be returned.  If there is only one such
         * tetrahedron \a t, then the pair (\a t, \c null) will be returned.
         * If no positive rational multiple of this hypersurface is the thin
         * link of any tetrahedron, then the pair (\c null, \c null) will be
         * returned.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a pair containing the tetrahedra linked by a positive
         * rational multiple of this hypersurface, as described above.
         */
        std::pair<const Tetrahedron<4>*, const Tetrahedron<4>*>
            isThinTetrahedronLink() const;
        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface is the normalised link of a single tetrahedron.
         *
         * Here the phrase \e normalised link of a tetrahedron \a t means the
         * frontier of a regular neighbourhood of \a t, converted into a normal
         * hypersurface by expanding away from the tetrahedron using some basic
         * normalisation moves.  It could be that there is no normalisation
         * required at all (in which case it is also a \e thin tetrahedron
         * link).  However, it could be that the normalisation process expands
         * the hypersurface far away from the tetrahedron itself, or changes its
         * topology, or disconnects the hypersurface, or even normalises it
         * away to an empty hypersurface.
         *
         * In particular, this test behaves differently from
         * isThinTetrahedronLink(), which tests for thin tetrahedron links only
         * (where no additional normalisation is required).  Unlike the
         * tests for edge links, the routines isThinTetrahedronLink() and
         * isNormalTetrahedronLink() use essentially the same implementation (so
         * testing for only thin links may be a little faster, but not by much).
         *
         * A hypersurface (or its positive rational multiple) could be the
         * normalised link of many tetrahedra.  The return value will be a
         * pair (\a v, \a thin), where:
         *
         * - \a v is a vector containing all such tetrahedra.  This will begin
         *   with the tetrahedra for which this hypersurface is a thin link,
         *   followed by the tetrahedra where normalisation was required;
         *   within each category the tetrahedra will be ordered by their
         *   index within the triangulation.
         *
         * - \a thin is either 0, 1 or 2, indicating how many tetrahedra this
         *   hypersurface is a thin link for.  This uses an unsigned type,
         *   since it will often be compared to <tt>v.size()</tt>.
         *
         * If no positive rational multiple of this hypersurface is the
         * normalised link of any tetrahedron, then \a link will be 0 and \a v
         * will be the empty vector.
         *
         * Note that the results of this routine are not cached.
         * Thus the results will be reevaluated every time this routine is
         * called.
         *
         * \return a vector containing the tetrahedra linked by a positive
         * rational multiple of this hypersurface and an integer indicating
         * how many of these links are thin, as described above.
         */
        std::pair<std::vector<const Tetrahedron<4>*>, unsigned>
            isNormalTetrahedronLink() const;

        /**
         * Returns the first homology group of this hypersurface.
         *
         * There is an important caveat regarding invalid 4-manifold
         * triangulations.  If the underlying triangulation has edge
         * links that are not spheres, then it is possible that this
         * normal hypersurface is not a compact 3-manifold.  In such a
         * case, this routine will compute homology in the same way as
         * Triangulation<3>::homology() - that is, by effectively truncating
         * ideal points of the hypersurface (which may arise where the
         * hypersurface meets an invalid edge).
         *
         * This routine caches its results, which means that once it has
         * been called for a particular surface, subsequent calls return
         * the answer immediately.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \warning This routine explicitly builds the normal pieces,
         * and so may run out of memory if the normal coordinates
         * are extremely large.
         *
         * \return the first homology group.
         */
        const AbelianGroup& homology() const;

        /**
         * Returns a 3-manifold triangulation describing this normal
         * hypersurface.
         *
         * The triangulation will be simplified via
         * Triangulation<3>::intelligentSimplify(), which means that the
         * tetrahedra of the final triangulation are not likely to
         * correspond to any particular tetrahedron/prism pieces of
         * this normal hypersurface.
         *
         * \todo \prob Check for absurdly large numbers of pieces and
         * return \c null accordingly.
         *
         * \pre This normal hypersurface is compact and embedded.
         *
         * \return a triangulation of this normal hypersurface.
         */
        Triangulation<3> triangulate() const;

        /**
         * Determines whether this and the given hypersurface in fact
         * represent the same normal hypersurface.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal pieces of each type, and returns \c true
         * if and only if these counts are the same for both hypersurfaces.
         *
         * It does not matter what vector encodings the two hypersurfaces
         * use.  In particular, it does not matter if this and the
         * given hypersurface use different encodings, or if one but not
         * the other supports non-compact hypersurfaces.
         *
         * This routine is safe to call even if this and the given
         * hypersurface do not belong to the same triangulation:
         *
         * - If the two triangulations have the same size, then this routine
         *   will test whether this hypersurface, if transplanted into the
         *   other triangulation using the same pentachoron numbering and the
         *   same normal piece types, would be the same as \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c false.
         *
         * \param other the hypersurface to be compared with this hypersurface.
         * \return \c true if both hypersurfaces represent the same normal
         * hypersurface, or \c false if not.
         */
        bool operator == (const NormalHypersurface& other) const;

        /**
         * Determines whether this and the given hypersurface represent
         * different normal hypersurfaces.
         *
         * Specifically, this routine examines (or computes) the number of
         * normal pieces of each type, and returns \c true if and only if
         * these counts are not the same for both hypersurfaces.
         *
         * It does not matter what vector encodings the two hypersurfaces
         * use.  In particular, it does not matter if this and the
         * given hypersurface use different encodings, or if one but not
         * the other supports non-compact hypersurfaces.
         *
         * This routine is safe to call even if this and the given
         * hypersurface do not belong to the same triangulation:
         *
         * - If the two triangulations have the same size, then this routine
         *   will test whether this hypersurface, if transplanted into the
         *   other triangulation using the same pentachoron numbering and the
         *   same normal piece types, would be different from \a other.
         *
         * - If the two triangulations have different sizes, then this
         *   routine will return \c true.
         *
         * \param other the hypersurface to be compared with this hypersurface.
         * \return \c true if both hypersurfaces represent different normal
         * hypersurface, or \c false if not.
         */
        bool operator != (const NormalHypersurface& other) const;

        /**
         * Imposes a total order on all normal hypersurfaces.
         *
         * This order is not mathematically meaningful; it is merely
         * provided for scenarios where you need to be able to sort
         * hypersurfaces (e.g., when using them as keys in a map).
         *
         * The order \e is well-defined, and will be preserved across
         * copy/move operations, different program executions, and different
         * platforms (since it is defined purely in terms of the normal
         * coordinates, and does not use transient properties such as
         * locations in memory).
         *
         * This operation is consistent with the equality test.  In
         * particular, it does not matter whether the two hypersurfaces
         * belong to different triangulations, or use different encodings,
         * or if one but not the other supports non-compact hypersurfaces.
         * See the equality test operator==() for further details.
         *
         * \param other the hypersurface to be compared with this hypersurface.
         * \return \c true if and only if this appears before the given
         * hypersurface in the total order.
         */
        bool operator < (const NormalHypersurface& other) const;

        /**
         * Determines whether this hypersurface is embedded.  This is true if
         * and only if the surface contains no conflicting prism types.
         *
         * \return \c true if and only if this hypersurface is embedded.
         */
        bool embedded() const;

        /**
         * Determines whether this and the given hypersurface are locally
         * compatible.  Local compatibility means that, within each individual
         * pentachoron of the triangulation, it is possible to arrange the
         * normal discs of both hypersurfaces so that none intersect.
         *
         * This is a local constraint, not a global constraint.  That is,
         * we do not insist that we can avoid intersections within all
         * pentachora \e simultaneously.
         *
         * Local compatibility can be formulated in terms of normal piece
         * types.  Two normal hypersurfaces are locally compatible if
         * and only if they together use at most two prism piece types per
         * pentachoron; moreover, if there \e are two prism piece types within
         * a single pentachoron then these prism types are non-intersecting.
         *
         * If one of the two hypersurfaces breaks the local compatibility
         * constraints on its own (for instance, it contains two conflicting
         * prism types within the same pentachoron), then this
         * routine will return \c false regardless of what the other
         * hypersurface contains.
         *
         * \pre Both this and the given normal hypersurface live within the
         * same 4-manifold triangulation.
         *
         * \param other the other hypersurface to test for local compatibility
         * with this hypersurface.
         * \return \c true if the two hypersurfaces are locally compatible, or
         * \c false if they are not.
         */
        bool locallyCompatible(const NormalHypersurface& other) const;

        /**
         * Gives read-only access to the integer vector that Regina uses
         * internally to represent this hypersurface.
         *
         * Note that this vector might not use the same coordinate system
         * in which the hypersurfaces were originally enumerated.  (For example,
         * this vector will always include tetrahedron coordinates, even if
         * the surfaces were originally enumerated in prism coordinates.)
         * You can call encoding() to find out precisley how the coordinates
         * of this vector should be interpreted.
         *
         * See the NormalHypersurface class notes for information on how this
         * vector is structured.
         *
         * \note If you wish to access the numbers of tetrahedra, prisms and
         * so on, you should use the functions tetrahedra(), prisms(), etc.,
         * which do not require any knowledge of the internal vector
         * encoding that this hypersurface uses.
         *
         * \return the underlying integer vector.
         */
        const Vector<LargeInteger>& vector() const;

        /**
         * Returns the specific integer vector encoding that this hypersurface
         * uses internally.  This is the encoding that should be used
         * to interpret vector().
         *
         * Note that this might differ from the encoding originally
         * passed to the class constructor.
         *
         * \return the internal vector encoding.
         */
        HyperEncoding encoding() const;

        /**
         * Reconstructs the tetrahedron coordinates in the given integer vector.
         *
         * The given vector must represent a normal hypersurface within the
         * given triangulation, using the given vector encoding.
         *
         * - If the given encoding does not already store tetrahedron
         *   coordinates, then the vector will be modified directly to use a
         *   new encoding that does, and this new encoding will be returned.
         *
         * - If the given encoding does already store tetrahedra, then
         *   this routine will do nothing and immediately return \a enc.
         *
         * \param tri the triangulation in which the normal hypersurface lives.
         * \param vector an integer vector that encodes a normal hypersurface
         * within \a tri; this will be modified directly.
         * \param enc the encoding used by the given integer vector.
         * \return the new encoding used by the modified \a vector.
         */
        static HyperEncoding reconstructTetrahedra(const Triangulation<4>& tri,
            Vector<LargeInteger>& vector, HyperEncoding enc);

    private:
        /**
         * Calculates whether this hypersurface has any real boundary and
         * stores the result as a property.
         */
        void calculateRealBoundary() const;
        /**
         * Calculate and store all properties that we derive from the
         * 3-manifold triangulation of this hypersurface.
         */
        void calculateFromTriangulation() const;

        /**
         * Determines whether or not a positive rational multiple of this
         * hypersurface \e could be the normalised link of a face of positive
         * dimension.
         *
         * A non-null return value is \e not a guarantee that this hypersurface
         * \e is such a link; however, if this routine returns no value then
         * this \e is a guarantee that the hypersurface is not such a link.
         *
         * The precise tests that this routine carries out involve a trade-off
         * between speed and mathematical power, and so are subject to change
         * in future versions of Regina.
         *
         * \pre This hypersurface is non-empty.
         *
         * \return the precise multiple of this hypersurface that \e could be a
         * normalised non-vertex face link, or no value if we can prove
         * that this hypersurface is not such a link.
         */
        std::optional<NormalHypersurface> couldLinkFace() const;

    friend class XMLNormalHypersurfaceReader;
};

/**
 * Swaps the contents of the given normal hypersurfaces.
 * This is a fast (constant time) operation.
 *
 * This global routine simply calls NormalHypersurface::swap(); it is provided
 * so that NormalHypersurface meets the C++ Swappable requirements.
 *
 * \param a the first normal hypersurface whose contents should be swapped.
 * \param b the second normal hypersurface whose contents should be swapped.
 *
 * \ingroup hypersurface
 */
void swap(NormalHypersurface& a, NormalHypersurface& b) noexcept;

// Inline functions for NormalHypersurface

inline NormalHypersurface::NormalHypersurface(const Triangulation<4>& tri,
        HyperEncoding enc, const Vector<LargeInteger>& vector) :
        enc_(enc), vector_(vector), triangulation_(tri) {
    // This call to storesTetrahedra() is unnecessary, but we'd like it
    // accessible to the inline version.  (Same goes for the similar
    // occurrences of storesTetrahedra() in the constructors below.)
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(const Triangulation<4>& tri,
        HyperEncoding enc, Vector<LargeInteger>&& vector) :
        enc_(enc), vector_(std::move(vector)), triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(
        const SnapshotRef<Triangulation<4>>& tri,
        HyperEncoding enc, const Vector<LargeInteger>& vector) :
        enc_(enc), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(*tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(
        const SnapshotRef<Triangulation<4>>& tri,
        HyperEncoding enc, Vector<LargeInteger>&& vector) :
        enc_(enc), vector_(std::move(vector)), triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(*tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(const Triangulation<4>& tri,
        HyperCoords coords, const Vector<LargeInteger>& vector) :
        enc_(coords), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(const Triangulation<4>& tri,
        HyperCoords coords, Vector<LargeInteger>&& vector) :
        enc_(coords), vector_(std::move(vector)),
        triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(
        const SnapshotRef<Triangulation<4>>& tri,
        HyperCoords coords, const Vector<LargeInteger>& vector) :
        enc_(coords), vector_(vector), triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(*tri, vector_, enc_);
}

inline NormalHypersurface::NormalHypersurface(
        const SnapshotRef<Triangulation<4>>& tri,
        HyperCoords coords, Vector<LargeInteger>&& vector) :
        enc_(coords), vector_(std::move(vector)),
        triangulation_(tri) {
    if (! enc_.storesTetrahedra())
        enc_ = reconstructTetrahedra(*tri, vector_, enc_);
}

// NOLINTNEXTLINE(modernize-pass-by-value)
inline NormalHypersurface::NormalHypersurface(const NormalHypersurface& src,
        const Triangulation<4>& triangulation) :
        NormalHypersurface(src) {
    // We will happily accept one redundant SnapshotRef assignment as the
    // cost of removing many lines of code.
    triangulation_ = triangulation;
}

// NOLINTNEXTLINE(modernize-pass-by-value)
inline NormalHypersurface::NormalHypersurface(const NormalHypersurface& src,
        const SnapshotRef<Triangulation<4>>& triangulation) :
        NormalHypersurface(src) {
    // We will happily accept one redundant SnapshotRef assignment as the
    // cost of removing many lines of code.
    triangulation_ = triangulation;
}

inline void NormalHypersurface::swap(NormalHypersurface& other) noexcept {
    std::swap(enc_, other.enc_);
    vector_.swap(other.vector_);
    triangulation_.swap(other.triangulation_);

    name_.swap(other.name_);
    orientable_.swap(other.orientable_);
    twoSided_.swap(other.twoSided_);
    connected_.swap(other.connected_);
    realBoundary_.swap(other.realBoundary_);
    compact_.swap(other.compact_);
    H1_.swap(other.H1_);
    std::swap(linkOf_, other.linkOf_);
}

inline LargeInteger NormalHypersurface::tetrahedra(size_t pentIndex, int vertex)
        const {
    return vector_[enc_.block() * pentIndex + vertex];
}
inline LargeInteger NormalHypersurface::prisms(
        size_t pentIndex, int prismType) const {
    return vector_[enc_.block() * pentIndex + 5 + prismType];
}

inline const Triangulation<4>& NormalHypersurface::triangulation() const {
    return *triangulation_;
}

inline const std::string& NormalHypersurface::name() const {
    return name_;
}
inline void NormalHypersurface::setName(const std::string& name) {
    name_ = name;
}

inline bool NormalHypersurface::isEmpty() const {
    // All vector encodings store the empty hypersurface as the zero vector.
    return vector_.isZero();
}

inline bool NormalHypersurface::isOrientable() const {
    if (! orientable_.has_value())
        calculateFromTriangulation();
    return *orientable_;
}

inline bool NormalHypersurface::isTwoSided() const {
    if (! twoSided_.has_value())
        calculateFromTriangulation();
    return *twoSided_;
}

inline bool NormalHypersurface::isConnected() const {
    if (! connected_.has_value())
        calculateFromTriangulation();
    return *connected_;
}

inline bool NormalHypersurface::hasRealBoundary() const {
    if (! realBoundary_.has_value())
        calculateRealBoundary();
    return *realBoundary_;
}

inline const AbelianGroup& NormalHypersurface::homology() const {
    if (! H1_.has_value())
        calculateFromTriangulation();
    return *H1_;
}

inline const Vector<LargeInteger>& NormalHypersurface::vector() const {
    return vector_;
}

inline HyperEncoding NormalHypersurface::encoding() const {
    return enc_;
}

inline bool NormalHypersurface::operator != (const NormalHypersurface& other)
        const {
    return ! ((*this) == other);
}

inline NormalHypersurface NormalHypersurface::operator + (
        const NormalHypersurface& rhs) const {
    // Given our current conditions on vector storage, both underlying
    // integer vectors should store both tetrahedra and prisms.
    // This means that we can just add the vectors directly.
    return NormalHypersurface(triangulation_, enc_ + rhs.enc_,
            vector_ + rhs.vector_);
}

inline NormalHypersurface NormalHypersurface::doubleHypersurface() const {
    return (*this) * 2;
}

inline void swap(NormalHypersurface& a, NormalHypersurface& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

