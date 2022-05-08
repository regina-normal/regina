
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

/*! \file surface/normalcoords.h
 *  \brief Defines constants for normal surface coordinate systems.
 */

#ifndef __REGINA_NORMALCOORDS_H
#ifndef __DOXYGEN
#define __REGINA_NORMALCOORDS_H
#endif

#include "regina-core.h"
#include <cstddef> // for size_t

namespace regina {

/**
 * Represents different coordinate systems that can
 * be used for enumerating and/or displaying normal surfaces.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own coordinate system, you should choose
 * an ID >= 10000.
 *
 * \ingroup surfaces
 */
enum NormalCoords {
    /**
     * Represents standard triangle-quadrilateral coordinates for
     * normal surfaces.
     *
     * Regina can both enumerate and view surfaces in this coordinate system.
     */
    NS_STANDARD = 0,
    /**
     * Represents quadrilateral coordinates for normal surfaces.
     * For details, see "Normal surface Q-theory", Jeffrey L. Tollefson,
     * Pacific J. Math. 183 (1998), no. 2, 359--374.
     *
     * Regina can both enumerate and view surfaces in this coordinate system.
     */
    NS_QUAD = 1,
    /**
     * Represents quadrilateral coordinates in ideal triangulations for
     * enumerating closed surfaces only (thus excluding spun-normal surfaces).
     * The coordinates themselves are identical to quadrilateral
     * coordinates, as described by NS_QUAD; however, the enumeration
     * procedure introduces additional constraints.  The resulting
     * solution space is the space Q_0 as described in "Computing closed
     * essential surfaces in knot complements", by Burton, Coward and Tillmann,
     * in SCG ’13: Proceedings of the 29th Annual Symposium on Computational
     * Geometry, ACM, 2013, pp. 405–414.
     *
     * Note that, if a vertex surface in quad coordinates is closed, it will
     * always be a vertex surface in this system of "closed quad coordinates".
     * However, the converse is not true: a vertex surface in closed quad
     * coordinates need not be a vertex in "plain" quad coordinates.
     *
     * Regina can enumerate surfaces in this coordinate system, but it is not
     * for viewing.  You can just view the surfaces in quad coordinates
     * (NS_QUAD) instead.
     *
     * \pre Regina can only create matching equations in this coordinate system
     * for a limited class of triangulations.  Currently, such triangulations
     * \e must be oriented and ideal, with precisely one torus cusp and no
     * other boundary components or internal vertices.  These conditions
     * will be checked when building the matching equations, and Regina
     * will throw an InvalidArgument exception if they are not met.
     *
     * \pre SnapPea must be able to work with the underlying triangulation,
     * without retriangulating.  This should follow from the previous
     * constraints; however, it will also be checked when building the
     * matching equations, and Regina will throw an UnsolvedCase exception
     * if this requirement is not met.
     */
    NS_QUAD_CLOSED = 10,
    /**
     * Indicates that a list of almost normal surfaces was created
     * using Regina 4.5.1 or earlier, where surfaces with more than
     * one octagon of the same type were stripped out of the final
     * solution set.  As of Regina 4.6 such surfaces are now
     * included in the solution set, since we need them if we
     * wish to enumerate \e all almost normal surfaces (not just
     * the \e vertex almost normal surfaces).
     *
     * Regina cannot enumerate or view surfaces in this coordinate system.
     * It is only used for reading legacy data files.
     * If you have a list that uses this system, you can just view the
     * surfaces in standard almost normal coordinates (NS_AN_STANDARD).
     */
    NS_AN_LEGACY = 100,
    /**
     * Represents quadrilateral-octagon coordinates for octagonal
     * almost normal surfaces.  For details, see
     * "Quadrilateral-octagon coordinates for almost normal surfaces",
     * Benjamin A. Burton, Experiment. Math. 19 (2010), 285-315.
     *
     * Regina can both enumerate and view surfaces in this coordinate system.
     */
    NS_AN_QUAD_OCT = 101,
    /**
     * Represents standard triangle-quadrilateral-octagon coordinates
     * for octagonal almost normal surfaces.
     *
     * Regina can both enumerate and view surfaces in this coordinate system.
     */
    NS_AN_STANDARD = 102,
    /**
     * Represents quadrilateral-octagon coordinates in ideal triangulations for
     * enumerating closed surfaces only (thus excluding spun-almost normal
     * surfaces).  The coordinates themselves are identical to
     * quadrilateral-octagon coordinates, as described by NS_AN_QUAD_OCT;
     * however, the enumeration procedure introduces additional constraints.
     *
     * Note that, if a vertex surface in quad-oct coordinates is closed, it will
     * always be a vertex surface in this system of "closed quad-oct
     * coordinates".  However, the converse is not true: a vertex surface in
     * closed quad-oct coordinates need not be a vertex in "plain" quad-oct
     * coordinates.
     *
     * Regina can enumerate surfaces in this coordinate system, but it is not
     * for viewing.  You can just view the surfaces in quad-oct coordinates
     * (NS_AN_QUAD_OCT) instead.
     *
     * \pre Regina can only create matching equations in this coordinate system
     * for a limited class of triangulations.  Currently, such triangulations
     * \e must be oriented and ideal, with precisely one torus cusp and no
     * other boundary components or internal vertices.  These conditions
     * will be checked when building the matching equations, and Regina
     * will throw an InvalidArgument exception if they are not met.
     *
     * \pre SnapPea must be able to work with the underlying triangulation,
     * without retriangulating.  This should follow from the previous
     * constraints; however, it will also be checked when building the
     * matching equations, and Regina will throw an UnsolvedCase exception
     * if this requirement is not met.
     */
    NS_AN_QUAD_OCT_CLOSED = 110,
    /**
     * Represents edge weight coordinates for normal surfaces.
     *
     * This coordinate system is for display only: Regina can view surfaces
     * in this coordinate system, but it cannot use it to enumerate or create
     * surfaces.
     */
    NS_EDGE_WEIGHT = 200,
    /**
     * Represents triangle arc coordinates for normal surfaces.
     *
     * This coordinate system is for display only: Regina can view surfaces
     * in this coordinate system, but it cannot use it to enumerate or create
     * surfaces.
     */
    NS_TRIANGLE_ARCS = 201,
    /**
     * Represents angle structure coordinates.
     *
     * This coordinate system is \e not for use with normal surfaces:
     * it cannot be used either to display them or enumerate them.
     * Instead it is for use with angle structures on triangulations.
     * Because the combinatorics and linear algebra of angle strutures
     * are tightly related to those of normal surfaces, we include
     * NS_ANGLE here so that angle structure routines can make use of
     * some of Regina's existing normal surface machinery.
     *
     * For a triangulation with \a n tetrahedra, this system has
     * 3<i>n</i>+1 coordinates.  The first 3<i>n</i> are analogous
     * to quadrilateral coordinates (specifically, for each
     * quadrilateral type \a Q, the corresponding angle structure coordinate
     * represents the pair of angles in the same tetrahedron that \a Q does
     * not meet).  The final coordinate is a scaling coordinate, used to
     * projectivise the angle structure polytope so that it becomes a
     * polyhedral cone that is invariant under (positive) scaling.
     * If the final scaling coordinate is \a s, then a rational value of \a x
     * in any other coordinate position should be interpreted as the angle
     * <i>x</i>.&pi;/<i>s</i>.
     *
     * \pre This coordinate system must not be used with any of Regina's
     * routines unless they explicitly declare that NS_ANGLE is allowed.
     */
    NS_ANGLE = 400
};

/**
 * Indicates precisely how a normal surface is encoded by an integer vector.
 *
 * Normal surfaces do not always store their internal vectors in the same
 * coordinate system that was used to enumerate the surfaces, and indeed
 * an isolated surface does not know \e how it was originally created.
 *
 * Therefore each normal surface keeps a small amount of data,
 * represented by this class, so that it knows how to interpret its
 * internal integer vector.  This data also remembers properties of the
 * enumeration process that can be used for optimisations (e.g., the
 * knowledge that, even if the vector stores triangle coordinates, the
 * surface cannot contain any vertex linking components).
 *
 * For convenience, there is also a special encoding that identifies an angle
 * structure vector; this can be created via <tt>NormalEncoding(NS_ANGLE)</tt>,
 * and can be recognised via storesAngles().  However, like NS_ANGLE itself,
 * this special angle structure encoding does \e not represent a normal surface,
 * cannot be combined with other encodings, and must not be used with any of
 * Regina's routines unless the documentation explicitly allows it.
 * Specifically, any code that accepts a NormalEncoding argument may silently
 * assume that the encoding is \e not the special angle structure encoding,
 * unless the documentation explicitly says otherwise.
 *
 * Encodings have the important property that any rational multiple of a
 * normal surface \a s can always be stored using the same encoding as is
 * used for \a s.  (In particular, taking a rational multiple will not
 * invalidate any of the property flags in the encoding.)
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup surfaces
 */
class NormalEncoding {
    private:
        // Note: the "true/false" interpretations of the bit flags below
        // are chosen so that, where possible, adding two surfaces with
        // different encodings corresponds to a boolean OR of the flags.
        // This is why (for instance) we use COULD_BE_NON_COMPACT as opposed
        // to something more intuitive like IS_COMPACT.

        static constexpr int SIZE_MASK = 0x000F;
            /**< Masks the initial bits of \a flags_ that store the
                 number of coordinates per tetrahedron. */

        static constexpr int PROPERTY_MASK = 0x7FF0;
            /**< Masks the higher-order bits of \a flags_ that store
                 boolean properties of the encoding method. */

        static constexpr int STORES_TRIANGLES = 0x0010;
            /**< The bit of \a flags_ that indicates whether triangle
                 coordinates are included in the vector. */

        static constexpr int STORES_OCTAGONS = 0x0020;
            /**< The bit of \a flags_ that indicates whether octagon
                 coordinates are included in the vector. */

        static constexpr int STORES_ANGLES = 0x0080;
            /**< A bit of \a flags that, if \c true, indicates that this
                 is the special encoding that corresponds to NS_ANGLE.
                 See the NormalEncoding class notes for details. */

        static constexpr int COULD_BE_VERTEX_LINK = 0x0100;
            /**< A bit of \a flags_ that, if \c false, indicates that it is
                 known from elsewhere (e.g., the enumeration process) that
                 this surface cannot contain any vertex linking components.
                 This flags does not affect the layout of the vector,
                 but may be used to optimise certain algorithms. */

        static constexpr int COULD_BE_NON_COMPACT= 0x0200;
            /**< A bit of \a flags_ that, if \c false, indicates that it is
                 known from elsewhere (e.g., the enumeration process) that
                 this surface must be compact (i.e., not spun-normal).
                 This flags does not affect the layout of the vector,
                 but may be used to optimise certain algorithms. */

        static constexpr int INVALID = 0x1000;
            /**< A bit of \a flags that, if \c true, indicates that this does
                 not represent a valid encoding method. */

        int flags_;
            /**< Holds (1) the number of coordinates per tetrahedron in
                 the bits indicated by SIZE_MASK; and (2) additional
                 bit flags that describe other properties of the encoding
                 and/or known properties of the surface. */

        /**
         * Creates a new encoding with the given value of \a flags_.
         */
        constexpr NormalEncoding(int flags) : flags_(flags) {}

    public:
        /**
         * Returns an encoding that precisely describes the given
         * normal or almost normal coordinate system.
         * This is the encoding that you would use with a "pen and paper"
         * enumeration of surfaces in the given coordinate system, with
         * no further conversion, optimisation, or post-processing.
         *
         * Note that, when Regina enumerates surfaces in the given coordinate
         * system, it might choose to post-process the resulting vectors to
         * use a different encoding.  For example, when enumerating surfaces
         * in quad or quad-oct coordinates, Regina computes and stores
         * triangle coordinates also, and so for its own \e internal choice of
         * encoding, storesTriangles() will return \c true.  In contrast,
         * if you simply create a <tt>NormalEncoding(NS_QUAD)</tt>, then the
         * resulting encoding will have storesTriangles() return \c false.
         *
         * If \a coords is not one of the coordinate systems that Regina
         * can use to enumerate or reconstruct surfaces (or NS_ANGLE, as
         * discussed below), then the resulting encoding will be invalid,
         * and valid() will return \c false.  (Here "reconstruct" refers
         * to the special case of NS_AN_LEGACY, which is used only when
         * reading surfaces from very old data files).
         *
         * As a special case, you may pass NS_ANGLE to this constructor;
         * however, the resulting encoding does not represent a normal surface
         * and must not be used anywhere in Regina unless the documentation
         * explicitly allows it.  See the class notes for further details.
         *
         * @param coords one of Regina's normal or almost normal coordinate
         * systems.
         */
        constexpr NormalEncoding(NormalCoords coords) : flags_(INVALID) {
            switch (coords) {
                case NS_STANDARD:
                    flags_ = 7 | COULD_BE_VERTEX_LINK | STORES_TRIANGLES;
                    break;
                case NS_QUAD:
                    flags_ = 3 | COULD_BE_NON_COMPACT;
                    break;
                case NS_QUAD_CLOSED:
                    flags_ = 3;
                    break;
                case NS_AN_STANDARD:
                case NS_AN_LEGACY:
                    flags_ = 10 | COULD_BE_VERTEX_LINK |
                        STORES_TRIANGLES | STORES_OCTAGONS;
                    break;
                case NS_AN_QUAD_OCT:
                    flags_ = 6 | COULD_BE_NON_COMPACT | STORES_OCTAGONS;
                    break;
                case NS_AN_QUAD_OCT_CLOSED:
                    flags_ = 6 | STORES_OCTAGONS;
                    break;
                case NS_ANGLE:
                    flags_ = 3 | STORES_ANGLES;
                default:
                    break;
            }
        }
        /**
         * Creates a new copy of the given encoding.
         */
        constexpr NormalEncoding(const NormalEncoding&) = default;
        /**
         * Sets this to be a copy of the given encoding.
         *
         * @return a reference to this encoding.
         */
        NormalEncoding& operator = (const NormalEncoding&) = default;

        /**
         * Determines whether this and the given encoding are identical.
         *
         * @param other the encoding to compare with this.
         * @return \c true if and only if both encodings are identical.
         */
        constexpr bool operator == (const NormalEncoding& other) const {
            return flags_ == other.flags_;
        }
        /**
         * Determines whether this and the given encoding are different.
         *
         * @param other the encoding to compare with this.
         * @return \c true if and only if both encodings are different.
         */
        constexpr bool operator != (const NormalEncoding& other) const {
            return flags_ != other.flags_;
        }

        /**
         * Returns whether this describes a vector encoding of normal
         * or almost normal surfaces.
         *
         * The only case where this would be \c false is if you create
         * an encoding using either (1) a coordinate system that Regina
         * does not use for enumeration or reconstruction; or (2) another
         * invalid encoding.
         *
         * For the special angle structure encoding (described in the class
         * notes), this routine will return \c true.
         *
         * @return \c true if and only if this is a valid encoding.
         */
        constexpr bool valid() const {
            return ! (flags_ & INVALID);
        }
        /**
         * Returns the number of coordinates stored for each tetrahedron.
         *
         * @return The number of coordinates per tetrahedron.
         */
        constexpr int block() const {
            return flags_ & SIZE_MASK;
        }
        /**
         * Returns whether this encoding explicitly stores triangle coordinates.
         *
         * For the special angle structure encoding (described in the class
         * notes), this routine will return \c false.
         *
         * @return \c true if triangle coordinates are stored.
         */
        constexpr bool storesTriangles() const {
            return flags_ & STORES_TRIANGLES;
        }
        /**
         * Returns whether this encoding explicitly stores octagon coordinates.
         *
         * For the special angle structure encoding (described in the class
         * notes), this routine will return \c false.
         *
         * @return \c true if octagon coordinates are stored.
         */
        constexpr bool storesOctagons() const {
            return flags_ & STORES_OCTAGONS;
        }
        /**
         * Identifies whether this is the special angle structure encoding.
         *
         * This routine is used to recognise the "special case" encoding
         * <tt>NormalEncoding(NS_ANGLE)</tt>.  Such an encoding does not
         * represent a normal surface, and cannot be used anywhere in Regina
         * unless explicitly allowed in the documentation.  See the class
         * notes for further details.
         */
        constexpr bool storesAngles() const {
            return flags_ & STORES_ANGLES;
        }
        /**
         * Returns whether it is possible for a surface using this
         * encoding to include one or more vertex linking components.
         *
         * This is typically information that is inherited from whoever
         * enumerated or created the normal surface that uses this encoding.
         *
         * If this returns \c true, it does not mean that the surface
         * does actually contain vertex linking components; it simply
         * means that the user will need to test this themselves.
         * If this returns \c false, however, it is guaranteed that the
         * surface does \e not contain any vertex linking components,
         * with no further testing required.
         *
         * For the special angle structure encoding (described in the class
         * notes), this routine will return \c false.
         *
         * @return \c true if it is possible that the surface might
         * contain one or more vertex linking components.
         */
        constexpr bool couldBeVertexLink() const {
            return flags_ & COULD_BE_VERTEX_LINK;
        }
        /**
         * Returns whether it is possible for a surface using this
         * encoding to be non-compact.  Here "non-compact" refers to a
         * surface with infinitely many discs, such as a spun-normal surface.
         * See NormalSurface::isCompact() for details.
         *
         * This is typically information that is inherited from whoever
         * enumerated or created the normal surface that uses this encoding.
         *
         * If this returns \c true, it does not mean that the surface
         * does actually contain infinitely many discs; it simply
         * means that the user will need to test this themselves.
         * If this returns \c false, however, it is guaranteed that the
         * surface is compact, with no further testing required.
         *
         * For the special angle structure encoding (described in the class
         * notes), this routine will return \c false.
         *
         * @return \c true if it is possible that the surface might
         * be non-compact.
         */
        constexpr bool couldBeNonCompact() const {
            return flags_ & COULD_BE_NON_COMPACT;
        }
        /**
         * Returns an extension of this encoding that explicitly stores
         * triangle coordinates.
         *
         * All other properties of the encoding will be copied across without
         * changes (including "theoretical" properties such as whether the
         * surface could have vertex linking components, or whether it
         * could be spun-normal).
         *
         * If this encoding already stores triangle coordinates, then
         * the result will be identical to this.
         *
         * \pre This is not the special angle structure encoding (see the
         * class notes for details).
         *
         * @return an extension of this encoding that stores triangle
         * coordinates.
         */
        constexpr NormalEncoding withTriangles() const {
            if (storesTriangles())
                return NormalEncoding(flags_);
            else
                return NormalEncoding((flags_ | STORES_TRIANGLES) + 4);
        }
        /**
         * Returns an encoding that could hold the sum of surfaces that
         * use this and the given encoding.
         *
         * More precisely, the encoding that is returned is the "simplest"
         * possible encoding that is capable of holding the sum of two
         * surfaces that use this and the given encoding respectively.
         *
         * \pre Neither this encoding nor \a rhs is the special angle
         * structure encoding (see the class notes for details).
         *
         * @param rhs the encoding to combine with this.
         * @return the "sum" of this and the given encoding, as defined above.
         */
        constexpr NormalEncoding operator + (NormalEncoding rhs) const {
            int propFlags = ((flags_ | rhs.flags_) & PROPERTY_MASK);
            int blockSize = 3 /* quads always present */ +
                ((propFlags & STORES_TRIANGLES) ? 4 : 0) +
                ((propFlags & STORES_OCTAGONS) ? 3 : 0);
            return NormalEncoding(propFlags | blockSize);
        }
        /**
         * Exports this encoding as an integer.
         *
         * The exact value of the integer is meant to be opaque, in the sense
         * that it can only be used with fromIntValue().  End users are
         * not meant to pull it apart and analyse it (this is what the
         * other query routines in this class are for).
         *
         * The main purpose of intValue() and fromIntValue() is to support
         * reading and writing surfaces to/from Regina's XML file format.
         *
         * It is guaranteed that 0 will never be the integer value of
         * a (correctly constructed) encoding.
         *
         * @return an integer that represents this encoding.
         */
        constexpr int intValue() const {
            return flags_;
        }
        /**
         * Reconstructs an encoding from an integer value.
         *
         * This is a partner routine to intValue(): for any encoding \a enc,
         * the encoding <tt>fromIntValue(enc.intValue())</tt> is always equal
         * to \a enc.
         *
         * The main purpose of intValue() and fromIntValue() is to support
         * reading and writing surfaces to/from Regina's XML file format.
         *
         * @param value an integer that represents an encoding, typically
         * obtained by calling intValue().
         * @return the corresponding encoding.
         */
        constexpr static NormalEncoding fromIntValue(int value) {
            return NormalEncoding(value);
        }
};

/**
 * A class used to query general information about different normal
 * coordinate systems.
 *
 * This class has become much simpler and more stripped-down since Regina 7.0.
 * Much of the functionality that was available in this class in Regina 6.0.1
 * and earlier can now be accessed through the new NormalEncoding class.
 *
 * \ingroup surfaces
 */
class NormalInfo {
    public:
        /**
         * Returns the human-readable name of the given coordinate system.
         *
         * The first letter of the returned string will be upper-case,
         * and all subsequent letters will be lower-case.
         *
         * This routine is guaranteed to return a non-null string, even
         * if \a coordSystem is not one of the NormalCoords enum values.
         *
         * @param coordSystem the coordinate system being queried; this
         * may be any of the NormalCoords enumeration values.
         * @return the name of the given coordinate system.
         */
        constexpr static const char* name(NormalCoords coordSystem) {
            switch (coordSystem) {
                case NS_STANDARD:
                    return "Standard normal (tri-quad)";
                case NS_QUAD:
                    return "Quad normal";
                case NS_AN_STANDARD:
                    return "Standard almost normal (tri-quad-oct)";
                case NS_AN_QUAD_OCT:
                    return "Quad-oct almost normal";
                case NS_QUAD_CLOSED:
                    return "Closed quad (non-spun)";
                case NS_AN_QUAD_OCT_CLOSED:
                    return "Closed quad-oct (non-spun)";
                case NS_EDGE_WEIGHT:
                    return "Edge weight";
                case NS_TRIANGLE_ARCS:
                    return "Triangle arc";
                case NS_AN_LEGACY:
                    return "Legacy almost normal (pruned tri-quad-oct)";
                case NS_ANGLE:
                    return "Angle structure";
                default:
                    return "Unknown";
            }
        }
};

} // namespace regina

#endif

