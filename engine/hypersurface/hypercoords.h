
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

/*! \file hypersurface/hypercoords.h
 *  \brief Defines constants for normal hypersurface coordinate systems.
 */

#ifndef __REGINA_HYPERCOORDS_H
#ifndef __DOXYGEN
#define __REGINA_HYPERCOORDS_H
#endif

#include "regina-core.h"
#include <cstddef> // for size_t

namespace regina {

/**
 * Represents different coordinate systems that can be used for enumerating
 * and/or displaying normal hypersurfaces within 4-manifold triangulations.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own coordinate system, you should choose
 * an ID ≥ 10000.
 *
 * \ingroup hypersurface
 */
enum class HyperCoords {
    /**
     * Represents standard tetrahedron-prism coordinates for
     * normal hypersurfaces.
     *
     * Regina can both enumerate and view hypersurfaces in this
     * coordinate system.
     */
    Standard = 0,

    /**
     * Represents prism coordinates for normal hypersurfaces.
     *
     * Regina can both enumerate and view hypersurfaces in this
     * coordinate system.
     */
    Prism = 1,

    /**
     * Represents edge weight coordinates for normal hypersurfaces.
     *
     * This coordinate system is for display only: Regina can view hypersurfaces
     * in this coordinate system, but it cannot use it to enumerate or create
     * hypersurfaces.
     */
    Edge = 200
};

/**
 * An alias for standard tetrahedron-prism coordinates for normal
 * hypersurfaces.
 *
 * The preferred way to access this constant is using the scoped enum value
 * HyperCoords::Standard.  However, these old names for the coordinate system
 * constants have been kept in the regina:: namespace for convenience and
 * compatibility with older code.
 */
inline constexpr HyperCoords HS_STANDARD = HyperCoords::Standard;

/**
 * An alias for prism coordinates for normal hypersurfaces.
 *
 * The preferred way to access this constant is using the scoped enum value
 * HyperCoords::Prism.  However, these old names for the coordinate system
 * constants have been kept in the regina:: namespace for convenience and
 * compatibility with older code.
 */
inline constexpr HyperCoords HS_PRISM = HyperCoords::Prism;

/**
 * An alias for edge weight coordinates for normal hypersurfaces.
 *
 * The preferred way to access this constant is using the scoped enum value
 * HyperCoords::Edge.  However, these old names for the coordinate system
 * constants have been kept in the regina:: namespace for convenience and
 * compatibility with older code.
 */
inline constexpr HyperCoords HS_EDGE_WEIGHT = HyperCoords::Edge;

/**
 * Indicates precisely how a normal hypersurface within a 4-manifold
 * triangulation is encoded by an integer vector.
 *
 * Normal hypersurfaces do not always store their internal vectors in the same
 * coordinate system that was used to enumerate the surfaces, and indeed
 * an isolated hypersurface does not know _how_ it was originally created.
 *
 * Therefore each normal hypersurface keeps a small amount of data,
 * represented by this class, so that it knows how to interpret its
 * internal integer vector.  This data also remembers properties of the
 * enumeration process that can be used for optimisations (e.g., the
 * knowledge that, even if the vector stores tetrahedron coordinates, the
 * surface cannot contain any vertex linking components).
 *
 * Encodings have the important property that any rational multiple of a
 * normal hypersurface \a s can always be stored using the same encoding as
 * is used for \a s.  (In particular, taking a rational multiple will not
 * invalidate any of the property flags in the encoding.)
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup hypersurface
 */
class HyperEncoding {
    private:
        // Note: the "true/false" interpretations of the bit flags below
        // are chosen so that, where possible, adding two hypersurfaces with
        // different encodings corresponds to a boolean OR of the flags.
        // This is why (for instance) we use COULD_BE_VERTEX_LINK as opposed
        // to something more intuitive like IS_VERTEX_LINK.

        static constexpr int SIZE_MASK = 0x000F;
            /**< Masks the initial bits of \a flags_ that store the
                 number of coordinates per pentachoron. */

        static constexpr int PROPERTY_MASK = 0x7FF0;
            /**< Masks the higher-order bits of \a flags_ that store
                 boolean properties of the encoding method. */

        static constexpr int STORES_TETRAHEDRA = 0x0010;
            /**< The bit of \a flags_ that indicates whether tetrahedron
                 coordinates are included in the vector. */

        static constexpr int COULD_BE_VERTEX_LINK = 0x0100;
            /**< A bit of \a flags_ that, if \c false, indicates that it is
                 known from elsewhere (e.g., the enumeration process) that
                 this hypersurface cannot contain any vertex linking components.
                 This flags does not affect the layout of the vector,
                 but may be used to optimise certain algorithms. */

        static constexpr int COULD_BE_NON_COMPACT= 0x0200;
            /**< A bit of \a flags_ that, if \c false, indicates that it is
                 known from elsewhere (e.g., the enumeration process) that
                 this hypersurface must be compact (i.e., contain finitely
                 many normal pieces).
                 This flags does not affect the layout of the vector,
                 but may be used to optimise certain algorithms. */

        static constexpr int INVALID = 0x1000;
            /**< A bit of \a flags that, if \c true, indicates that this does
                 not represent a valid encoding method. */

        int flags_;
            /**< Holds (1) the number of coordinates per pentachoron in
                 the bits indicated by SIZE_MASK; and (2) additional
                 bit flags that describe other properties of the encoding
                 and/or known properties of the surface. */

        /**
         * Creates a new encoding with the given value of \a flags_.
         */
        constexpr HyperEncoding(int flags) : flags_(flags) {}

    public:
        /**
         * Returns an encoding that precisely describes the given
         * normal hypersurface coordinate system.
         * This is the encoding that you would use with a "pen and paper"
         * enumeration of hypersurfaces in the given coordinate system, with
         * no further conversion, optimisation, or post-processing.
         *
         * Note that, when Regina enumerates hypersurfaces in the given
         * coordinate system, it might choose to post-process the resulting
         * vectors to use a different encoding.  (In the 3-dimensional world,
         * for example, normal surfaces enumerated in quad coordinates
         * are post-processed and stored in standard coordinates.)
         *
         * If \a coords is not one of the coordinate systems that Regina
         * can use to enumerate normal hypersurfaces, then the resulting
         * encoding will be invalid, and valid() will return \c false.
         *
         * \param coords one of Regina's normal hypersurface coordinate systems.
         */
        constexpr HyperEncoding(HyperCoords coords) : flags_(INVALID) {
            switch (coords) {
                case HyperCoords::Standard:
                    flags_ = 15 | COULD_BE_VERTEX_LINK | STORES_TETRAHEDRA;
                    break;

                case HyperCoords::Prism:
                    flags_ = 10 | COULD_BE_NON_COMPACT;
                    break;

                default:
                    break;
            }
        }
        /**
         * Creates a new copy of the given encoding.
         */
        constexpr HyperEncoding(const HyperEncoding&) = default;
        /**
         * Sets this to be a copy of the given encoding.
         *
         * \return a reference to this encoding.
         */
        HyperEncoding& operator = (const HyperEncoding&) = default;

        /**
         * Determines whether this and the given encoding are identical.
         *
         * \param other the encoding to compare with this.
         * \return \c true if and only if both encodings are identical.
         */
        constexpr bool operator == (const HyperEncoding& other) const {
            return flags_ == other.flags_;
        }

        /**
         * Returns whether this describes a vector encoding of normal
         * hypersurfaces.
         *
         * The only case where this would be \c false is if you create
         * an encoding using either (1) a coordinate system that Regina
         * does not use for enumeration; or (2) another invalid encoding.
         *
         * \return \c true if and only if this is a valid encoding.
         */
        constexpr bool valid() const {
            return ! (flags_ & INVALID);
        }
        /**
         * Returns the number of coordinates stored for each pentachoron.
         *
         * \return The number of coordinates per pentachoron.
         */
        constexpr int block() const {
            return flags_ & SIZE_MASK;
        }
        /**
         * Returns whether this encoding explicitly stores tetrahedron
         * coordinates.
         *
         * \return \c true if tetrahedron coordinates are stored.
         */
        constexpr bool storesTetrahedra() const {
            return flags_ & STORES_TETRAHEDRA;
        }
        /**
         * Returns whether it is possible for a normal hypersurface using this
         * encoding to include one or more vertex linking components.
         *
         * This is typically information that is inherited from whoever
         * enumerated or created the hypersurface that uses this encoding.
         *
         * If this returns \c true, it does not mean that the hypersurface
         * does actually contain vertex linking components; it simply
         * means that the user will need to test this themselves.
         * If this returns \c false, however, it is guaranteed that the
         * hypersurface does _not_ contain any vertex linking components,
         * with no further testing required.
         *
         * \return \c true if it is possible that the hypersurface might
         * contain one or more vertex linking components.
         */
        constexpr bool couldBeVertexLink() const {
            return flags_ & COULD_BE_VERTEX_LINK;
        }
        /**
         * Returns whether it is possible for a normal hypersurface using this
         * encoding to be non-compact.  Here "non-compact" refers to a
         * surface with infinitely many normal pieces.
         * See NormalHypersurface::isCompact() for details.
         *
         * This is typically information that is inherited from whoever
         * enumerated or created the hypersurface that uses this encoding.
         *
         * If this returns \c true, it does not mean that the hypersurface
         * does actually contain infinitely many normal pieces; it simply
         * means that the user will need to test this themselves.
         * If this returns \c false, however, it is guaranteed that the
         * hypersurface is compact, with no further testing required.
         *
         * \return \c true if it is possible that the hypersurface might
         * be non-compact.
         */
        constexpr bool couldBeNonCompact() const {
            return flags_ & COULD_BE_NON_COMPACT;
        }
        /**
         * Returns an extension of this encoding that explicitly stores
         * tetrahedron coordinates.
         *
         * All other properties of the encoding will be copied across without
         * changes (including "theoretical" properties such as whether the
         * surface could have vertex linking components, or whether it
         * could be non-compact).
         *
         * If this encoding already stores tetrahedron coordinates, then
         * the result will be identical to this.
         *
         * \return an extension of this encoding that stores tetrahedron
         * coordinates.
         */
        constexpr HyperEncoding withTetrahedra() const {
            if (storesTetrahedra())
                return HyperEncoding(flags_);
            else
                return HyperEncoding((flags_ | STORES_TETRAHEDRA) + 5);
        }
        /**
         * Returns an encoding that could hold the sum of hypersurfaces that
         * use this and the given encoding.
         *
         * More precisely, the encoding that is returned is the "simplest"
         * possible encoding that is capable of holding the sum of two normal
         * hypersurfaces that use this and the given encoding respectively.
         *
         * \param rhs the encoding to combine with this.
         * \return the "sum" of this and the given encoding, as defined above.
         */
        constexpr HyperEncoding operator + (HyperEncoding rhs) const {
            int propFlags = ((flags_ | rhs.flags_) & PROPERTY_MASK);
            int blockSize = ((propFlags & STORES_TETRAHEDRA) ? 15 : 10);
            return HyperEncoding(propFlags | blockSize);
        }
        /**
         * Returns an encoding that is suitable for representing the empty
         * hypersurface, whose normal coordinates are all zero.
         *
         * \return a suitable encoding for the empty hypersurface.
         */
        static constexpr HyperEncoding empty() {
            return { 15 | STORES_TETRAHEDRA };
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
         * reading and writing hypersurfaces to/from Regina's XML file format.
         *
         * It is guaranteed that 0 will never be the integer value of
         * a (correctly constructed) encoding.
         *
         * \return an integer that represents this encoding.
         */
        constexpr int intValue() const {
            return flags_;
        }
        /**
         * Reconstructs an encoding from an integer value.
         *
         * This is a partner routine to intValue(): for any encoding \a enc,
         * the encoding `fromIntValue(enc.intValue())` is always equal
         * to \a enc.
         *
         * The main purpose of intValue() and fromIntValue() is to support
         * reading and writing hypersurfaces to/from Regina's XML file format.
         *
         * \param value an integer that represents an encoding, typically
         * obtained by calling intValue().
         * \return the corresponding encoding.
         */
        constexpr static HyperEncoding fromIntValue(int value) {
            return HyperEncoding(value);
        }
};

/**
 * A class used to query general information about different normal
 * hypersurface coordinate systems.
 *
 * This class has become much simpler and more stripped-down since Regina 7.0.
 * Much of the functionality that was available in this class in Regina 6.0.1
 * and earlier can now be accessed through the new HyperEncoding class.
 *
 * \ingroup hypersurface
 */
class HyperInfo {
    public:
        /**
         * Returns the human-readable name of the given coordinate system.
         *
         * The first letter of the returned string will be upper-case,
         * and all subsequent letters will be lower-case.
         *
         * This routine is guaranteed to return a non-null string, even
         * if \a coordSystem is not one of the HyperCoords enum values.
         *
         * \param coordSystem the coordinate system being queried; this
         * may be any of the HyperCoords enumeration values.
         * \return the name of the given coordinate system.
         */
        constexpr static const char* name(HyperCoords coordSystem) {
            switch (coordSystem) {
                case HyperCoords::Standard:
                    return "Standard normal (tet-prism)";
                case HyperCoords::Prism:
                    return "Prism normal";
                case HyperCoords::Edge:
                    return "Edge weight";
                default:
                    return "Unknown";
            }
        }
};

} // namespace regina

#endif

