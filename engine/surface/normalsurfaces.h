
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

/*! \file surface/normalsurfaces.h
 *  \brief Implements a collection of normal surfaces in a 3-manifold
 *  triangulation.
 */

#ifndef __REGINA_NORMALSURFACES_H
#ifndef __DOXYGEN
#define __REGINA_NORMALSURFACES_H
#endif

#include <algorithm>
#include <iterator>
#include <optional>
#include <vector>
#include "regina-core.h"
#include "enumerate/validityconstraints.h"
#include "maths/matrix.h"
#include "packet/packet.h"
#include "progress/progresstracker.h"
#include "surface/normalsurface.h"
#include "surface/normalflags.h"
#include "surface/normalcoords.h"
#include "utilities/exception.h"

namespace regina {

class NormalSurfaces;
class ProgressTracker;
class SurfaceFilter;

/**
 * Used to describe a field, or a set of fields, that can be exported
 * alongside a normal surface list.  This enumeration type, and the
 * corresponding flags class SurfaceExport, is used with export routines such
 * as NormalSurfaces::saveCSVStandard() or NormalSurfaces::saveCSVEdgeWeight().
 *
 * This type describes fields in addition to normal coordinates, not the
 * normal coordinates themselves (which are always exported).  Each field
 * describes some property of a single normal surface, and corresponds to a
 * single column in a table of normal surfaces.
 *
 * You can describe a set of fields by combining the values for individual
 * fields using the bitwise OR operator.
 *
 * The list of available fields may grow with future releases of Regina.
 *
 * \ingroup surfaces
 */
enum SurfaceExportFields {
    /**
     * Represents the user-assigned surface name.
     */
    surfaceExportName = 0x0001,
    /**
     * Represents the calculated Euler characteristic of a
     * surface.  This will be an integer, and will be left empty
     * if the Euler characteristic cannot be computed.
     */
    surfaceExportEuler = 0x0002,
    /**
     * Represents the calculated property of whether a surface is
     * orientable.  This will be the string \c TRUE or \c FALSE, or
     * will be left empty if the orientability cannot be computed.
     */
    surfaceExportOrient = 0x0004,
    /**
     * Represents the calculated property of whether a surface is
     * one-sided or two-sided.  This will be the integer 1 or 2,
     * or will be left empty if the "sidedness" cannot be computed.
     */
    surfaceExportSides = 0x0008,
    /**
     * Represents the calculated property of whether a surface is
     * bounded.  In most cases, this will be one of the strings "closed",
     * "real bdry" or "infinite" (where "infinite" indicates a
     * surface with infinitely many discs).  For spun-normal
     * surfaces in certain ideal triangulations, this string will
     * be followed by the boundary slopes of the surface at the
     * cusps: these written as a list of pairs (\a p, \a q),
     * one for each cusp, indicating that the boundary curves of
     * the surface run \a p times around the meridian and \a q times
     * around the longitude.  See NormalSurface::boundaryIntersections()
     * for further information on interpreting these values.
     */
    surfaceExportBdry = 0x0010,
    /**
     * Represents whether a surface is a single vertex link or a
     * thin edge link.  See NormalSurface::isVertexLink() and
     * NormalSurface::isThinEdgeLink() for details.  This will be
     * written as a human-readable string.
     */
    surfaceExportLink = 0x0020,
    /**
     * Represents any additional high-level properties of a
     * surface, such as whether it is a splitting surface or a
     * central surface.  This will be written as a human-readable
     * string.  This field is somewhat arbitrary, and the precise
     * properties it describes are subject to change in future
     * releases of Regina.
     */
    surfaceExportType = 0x0040,

    /**
     * Indicates that no additional fields should be exported.
     */
    surfaceExportNone = 0,
    /**
     * Indicates that all available fields should be exported,
     * except for the user-assigned surface name.  Since the list
     * of available fields may grow with future releases, the numerical
     * value of this constant may change as a result.
     */
    surfaceExportAllButName = 0x007e,
    /**
     * Indicates that all available fields should be exported,
     * including the user-assigned surface name.  Since the list
     * of available fields may grow with future releases, the numerical
     * value of this constant may change as a result.
     */
    surfaceExportAll = 0x007f
};

/**
 * A set of fields to export alongside a normal surface list.
 *
 * If a function requires a SurfaceExport object as an argument, you can
 * pass a single SurfaceExportFields constant, or a combination of such
 * constants using the bitwise OR operator, or empty braces {} to indicate
 * no fields at all.
 *
 * \ingroup surfaces
 */
using SurfaceExport = regina::Flags<SurfaceExportFields>;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup surfaces
 */
inline SurfaceExport operator | (
        SurfaceExportFields lhs, SurfaceExportFields rhs) {
    return SurfaceExport(lhs) | rhs;
}

/**
 * A collection of normal surfaces in a 3-manifold triangulation.
 *
 * There are some important changes to this class as of Regina 7.0:
 *
 * - A normal surface list does _not_ need to be a child packet of the
 *   underlying triangulation, and indeed does not need to interact with
 *   the packet tree at all.
 *
 * - You are welcome to modify or even destroy the original triangulation;
 *   if you do then this list will automatically make a private copy of the
 *   original triangulation as an ongoing reference.  Different normal
 *   surface lists (and angle structure lists) can all share the same
 *   private copy, so this is not an expensive process.
 *
 * - You should now create normal surface lists using the class constructor
 *   (but which, unlike the old enumerate(), does not insert the list
 *   into the packet tree).  There is no need to use enumerate() any more.
 *
 * Since Regina 7.0, this is no longer a "packet type" that can be
 * inserted directly into the packet tree.  Instead a normal surface list
 * is now a standalone mathematatical object, which makes it slimmer and
 * faster for ad-hoc use.  The consequences of this are:
 *
 * - If you create your own NormalSurfaces object, it will not have any of
 *   the usual packet infrastructure.  You cannot add it into the packet tree,
 *   and it will not support a label, tags, child/parent packets, and/or
 *   event listeners.
 *
 * - To include an NormalSurfaces object in the packet tree, you must create
 *   a new PacketOf<NormalSurfaces>.  This _is_ a packet type, and supports
 *   labels, tags, child/parent packets, and event listeners.  It derives from
 *   NormalSurfaces, and so inherits the full NormalSurfaces interface.
 *
 * - If you are adding new functions to this class that edit the list,
 *   you must still remember to create a PacketChangeSpan.  This will
 *   ensure that, if the list is being managed by a PacketOf<NormalSurfaces>,
 *   then the appropriate packet change events will be fired.
 *   All other events (aside from packetToBeChanged() and packetWasChanged()
 *   are managed directly by the PacketOf<NormalSurfaces> wrapper class.
 *
 * See the NormalSurface class notes for details of what to do
 * when introducing a new coordinate system.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \feature Allow custom matching equations.
 * \todo \feature Allow enumeration with some coordinates explicitly set
 * to zero.
 * \todo \feature Allow generating only closed surfaces.
 * \todo \feature Generate facets of the solution space representing
 * embedded surfaces.
 *
 * \ingroup surfaces
 */
class NormalSurfaces :
        public PacketData<NormalSurfaces>, public Output<NormalSurfaces> {
    public:
        class VectorIterator;

    protected:
        std::vector<NormalSurface> surfaces_;
            /**< Contains all normal surfaces in this list. */
        SnapshotRef<Triangulation<3>> triangulation_;
            /**< The triangulation in which these normal surfaces lie. */
        NormalCoords coords_;
            /**< The coordinate system that was originally used to enumerate
                 the normal surfaces in this list. */
        NormalList which_;
            /**< Indicates which normal surfaces these represent within
                 the underlying triangulation. */
        NormalAlg algorithm_;
            /**< Stores the details of the enumeration algorithm that
                 was used to generate this list.  This might not be the
                 same as the \a algHints flag that was originally
                 passed to the enumeration routine (e.g., if invalid
                 or inappropriate flags were passed). */

    public:
        /**
         * A unified "enumeration constructor" for enumerating various classes
         * of normal surfaces within a given triangulation.
         *
         * The NormalCoords argument allows you to specify an underlying
         * coordinate system in which to do the enumeration (e.g., standard
         * coordinates, quadrilateral coordinates or almost normal coordinates).
         * This choice of coordinate system will affect which surfaces are
         * produced, since vertex/fundamental surfaces in one system are not
         * necessarily vertex/fundamental in another.
         *
         * The NormalList argument is a combination of flags that
         * allows you to specify exactly which normal surfaces you require.
         * This includes (i) whether you want all vertex surfaces
         * or all fundamental surfaces, which defaults to NS_VERTEX
         * if you specify neither or both; and (ii) whether you want only
         * properly embedded surfaces or you also wish to include
         * immersed and/or singular surfaces, which defaults to
         * NS_EMBEDDED_ONLY if you specify neither or both.
         *
         * The NormalAlg argument is a combination of flags that allows
         * you to control the underlying enumeration algorithm.  These
         * flags are treated as hints only: if your selection of
         * algorithm is invalid, unavailable or unsupported then Regina
         * will choose something more appropriate.  Unless you have
         * some specialised need, the default NS_ALG_DEFAULT (which
         * makes no hints at all) will allow Regina to choose what it
         * thinks will be the most efficient method.
         *
         * The enumerated surfaces will be stored in this new normal
         * surface list, and their representations will be scaled down
         * to use the smallest possible integer coordinates.
         *
         * Unlike the old enumerate() function, the new normal surface
         * list will _not_ be inserted into the packet tree.  Moreover,
         * the given triangulation may change or even be destroyed
         * without causing problems.  See the class notes for details.
         *
         * If a progress tracker is passed, this routine will declare and
         * work through a series of stages whose combined weights sum to 1;
         * typically this means that the given tracker must not have been
         * used before.
         *
         * This constructor will not return until the enumeration of surfaces
         * is complete, regardless of whether a progress tracker was passed.
         * If you need the behaviour of the old enumerate() (where passing a
         * progress tracker caused the enumeration to start in the background),
         * simply call this constructor in a new detached thread.
         * Note that this enumeration can be extremely slow for larger
         * triangulations, and so there could be good reasons to do this.
         *
         * If an error occurs, then this routine will thrown an exception.
         * In this case, no normal surface list will be created, and the
         * progress tracker (if passed) will be marked as finished.
         * See the exception specifications below for details.
         *
         * \exception InvalidArgument The matching equations could not
         * be created for the given triangulation in the given coordinate
         * system, due to an error that should have been preventable
         * with the right checks in advance.  This can only happen in certain
         * coordinate systems, and for all such coordinate systems this is
         * explicitly described in the NormalCoords enum documentation.
         *
         * \exception UnsolvedCase The list of hypersurfaces could not be
         * be created for the given triangulation in the given coordinate
         * system, due to an error that was "genuinely" unforseeable.
         * Currently there are two scenarios in which this could happen:
         * (i) the matching equations could not be constructed, which can
         * only happen in certain coordinate systems where this is explicitly
         * described in the NormalCoords enum documentation; or
         * (ii) the arguments require enumerating _fundamental_ normal
         * surfaces using the primal Hilbert basis algorithm, and Normaliz
         * was unable to complete its portion of the task, which in theory
         * should never happen at all.
         *
         * \python The global interpreter lock will be released while
         * this constructor runs, so you can use it with Python-based
         * multithreading.
         *
         * \param triangulation the triangulation upon which this list of
         * normal surfaces will be based.
         * \param coords the coordinate system to be used.  This must be
         * one of the system that Regina is able to use for enumeration;
         * this is documented alongside each NormalCoords enum value.
         * \param which indicates which normal surfaces should be enumerated.
         * \param algHints passes requests to Regina for which specific
         * enumeration algorithm should be used.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         */
        NormalSurfaces(
            const Triangulation<3>& triangulation,
            NormalCoords coords,
            NormalList which = NS_LIST_DEFAULT,
            NormalAlg algHints = NS_ALG_DEFAULT,
            ProgressTracker* tracker = nullptr);

        /**
         * A unified "transform constructor" for transforming one normal
         * surface list into another.
         *
         * The available transformations include:
         *
         * - conversions between vertex surfaces in different coordinate
         *   systems (e.g., between the set of all standard vertex surfaces
         *   and the typically smaller set of all quad vertex surfaces);
         *
         * - filters that select a subset of surfaces (e.g., only surfaces
         *   that have a locally compatible or disjoint partner, or only
         *   surfaces that could potentially be incompressible).
         *
         * Each transformation comes with its own set of preconditions,
         * as documented alongside the various NormalTransform enumeration
         * constants.  These preconditions will be checked, and if any of them
         * fails then this constructor will throw an exception (see below).
         *
         * Unlike the old conversion and filter functions, this constructor
         * will _not_ insert the new normal surface list into the packet tree.
         *
         * \exception FailedPrecondition The preconditions for the given
         * transformation were not met.  See each NormalTransform enum
         * constant for the corresponding set of preconditions.
         *
         * \param src the normal surface list that we wish to transform;
         * this will not be modified.
         * \param transform the specific transformation to apply.
         */
        NormalSurfaces(const NormalSurfaces& src, NormalTransform transform);

        /**
         * A "filter constructor" that creates a new list filled with those
         * surfaces from the given list that pass the given filter.
         *
         * Unlike the old filter() function, this constructor will _not_
         * insert the new normal surface list into the packet tree.
         *
         * For this new filtered list, which() will include the NS_CUSTOM
         * flag, and algorithm() will include the NS_ALG_CUSTOM flag.
         *
         * \param src the normal surface list that we wish to filter;
         * this will not be modified.
         * \param filter the filter to apply to the given list.
         */
        NormalSurfaces(const NormalSurfaces& src, const SurfaceFilter& filter);

        /**
         * Constructs a new copy of the given list.
         */
        NormalSurfaces(const NormalSurfaces&) = default;

        /**
         * Moves the given list into this new list.
         * This is a fast (constant time) operation.
         *
         * The list that is passed will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this list
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the list to move.
         */
        NormalSurfaces(NormalSurfaces&& src) noexcept = default;

        /**
         * Sets this to be a (deep) copy of the given list.
         *
         * \param src the list to copy.
         * \return a reference to this list.
         */
        NormalSurfaces& operator = (const NormalSurfaces& src);

        /**
         * Moves the contents of the given list into this list.
         * This is a fast (constant time) operation.
         *
         * The list that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this list which may in turn call arbitrary code
         * via any registered packet listeners.  It deliberately does _not_
         * fire change events on \a src, since it assumes that \a src is about
         * to be destroyed (which will fire a destruction event instead).
         *
         * \param src the list to move.
         * \return a reference to this list.
         */
        NormalSurfaces& operator = (NormalSurfaces&& src);

        /**
         * Swaps the contents of this and the given list.
         *
         * This routine will behave correctly if \a other is in fact
         * this list.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both lists which may in turn call arbitrary
         * code via any registered packet listeners.
         *
         * \param other the list whose contents should be swapped with this.
         */
        void swap(NormalSurfaces& other);

        /**
         * Returns the coordinate system that was originally used to enumerate
         * the surfaces in this list.
         *
         * \return the coordinate system used.
         */
        NormalCoords coords() const;
        /**
         * Returns details of which normal surfaces this list represents
         * within the underlying triangulation.
         *
         * This may not be the same NormalList that was passed to the
         * class constructor.  In particular, default values will have been
         * explicitly filled in (such as NS_VERTEX and/or NS_EMBEDDED_ONLY),
         * and invalid and/or redundant values will have been removed.
         *
         * \return details of what this list represents.
         */
        NormalList which() const;
        /**
         * Returns details of the algorithm that was used to enumerate
         * this list.
         *
         * These may not be the same NormalAlg flags that were passed to the
         * class constructor.  In particular, default values will have been
         * explicitly filled in, invalid and/or redundant values will have
         * been removed, and unavailable and/or unsupported combinations
         * of algorithm flags will be replaced with whatever algorithm was
         * actually used.
         *
         * \return details of the algorithm used to enumerate this list.
         */
        NormalAlg algorithm() const;
        /**
         * Determines if the coordinate system that was used for enumeration
         * allows for almost normal surfaces.
         *
         * This does not test whether any of the surfaces in this list
         * actually contain octagons: it simply returns a basic property
         * of the coordinate system that was used for enumeration.
         *
         * \return \c true if and only if almost normal surfaces are supported.
         */
        bool allowsAlmostNormal() const;
        /**
         * Determines if the coordinate system that was used for enumeration
         * allows for non-compact normal surfaces.
         *
         * This does not test whether any of the surfaces in this list
         * are actually non-compact: it simply returns a basic property
         * of the coordinate system that was used for enumeration.
         *
         * \return \c true if and only if non-compact normal surfaces are
         * supported.
         */
        bool allowsNonCompact() const;
        /**
         * Returns whether this list was constructed to contain only
         * properly embedded surfaces.
         *
         * If this returns \c false, it does not guarantee that immersed
         * and/or singular surfaces are present; it merely indicates
         * that they were not deliberately excluded (for instance, the
         * quadrilateral constraints were not enforced).
         *
         * \return \c true if this list was constructed to contain only
         * properly embedded surfaces, or \c false otherwise.
         */
        bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal surfaces live.
         *
         * This will be a snapshot frozen in time of the triangulation
         * that was originally passed to the NormalSurfaces constructor.
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
         * \warning As of Regina 7.0, you _cannot_ access this triangulation
         * via the packet tree as Packet::parent().  This is because normal
         * surface lists can now be kept anywhere in the packet tree, or can
         * be kept as standalone objects outside the packet tree entirely.
         *
         * \return a reference to the underlying triangulation.
         */
        const Triangulation<3>& triangulation() const;
        /**
         * Returns the number of surfaces stored in this list.
         *
         * \python This is also used to implement the Python special
         * method __len__().
         *
         * \return the number of surfaces.
         */
        size_t size() const;
        /**
         * Returns the surface at the requested index in this list.
         * This is identical to using the square bracket operator.
         *
         * \param index the index of the requested surface in this list;
         * this must be between 0 and size()-1 inclusive.
         *
         * \return the normal surface at the requested index in this list.
         */
        const NormalSurface& surface(size_t index) const;
        /**
         * Returns the surface at the requested index in this list.
         * This is identical to calling surface().
         *
         * \param index the index of the requested surface in this list;
         * this must be between 0 and size()-1 inclusive.
         *
         * \return the normal surface at the requested index in this list.
         */
        const NormalSurface& operator [](size_t index) const;
        /**
         * Returns a C++ iterator at the beginning of this list of surfaces.
         *
         * These begin() and end() functions allow you to iterate through all
         * surfaces in this list using a range-based \c for loop:
         *
         * \code{.cpp}
         * NormalSurfaces list(...);
         * for (const NormalSurface& s : list) { ... }
         * \endcode
         *
         * The type that is returned will be a lightweight iterator type,
         * guaranteed to satisfy the C++ LegacyRandomAccessIterator requirement.
         * The precise C++ type of the iterator is subject to change, so
         * C++ users should use \c auto (just like this declaration does).
         *
         * \nopython For Python users, NormalSurfaces implements the Python
         * iterable interface.  You can iterate over the normal surfaces in
         * this list in the same way that you would iterate over any native
         * Python container.
         *
         * \return an iterator at the beginning of this list.
         */
        auto begin() const;
        /**
         * Returns a C++ iterator beyond the end of this list of surfaces.
         *
         * These begin() and end() routines allow you to iterate through all
         * surfaces in this list using a range-based \c for loop.
         * See the begin() documentation for further details.
         *
         * \nopython For Python users, NormalSurfaces implements the Python
         * iterable interface.  You can iterate over the normal surfaces in
         * this list in the same way that you would iterate over any native
         * Python container.
         *
         * \return an iterator beyond the end of this list.
         */
        auto end() const;
#ifdef __APIDOCS
        /**
         * Returns a Python iterator over the normal surfaces in this list.
         *
         * In Python, a normal surface list can be treated as an iterable
         * object:
         *
         * \code{.py}
         * list = NormalSurfaces(...)
         * for s in list:
         *     ...
         * \endcode
         *
         * \nocpp For C++ users, NormalSurfaces provides the usual begin()
         * and end() functions instead.  In particular, you can iterate over
         * the normal surfaces in this list in the usual way using a
         * range-based \c for loop.
         *
         * \return an iterator over the normal surfaces in this list.
         */
        auto __iter__() const;
#endif

        /**
         * Determines whether this and the given list contain the same
         * set of normal (or almost normal) surfaces.
         *
         * The lists will be compared as multisets: the order of the
         * surfaces in each list does not matter; however, in the unusual
         * scenario where a list the same surface multiple times,
         * multiplicity does matter.
         *
         * Like the comparison operators for NormalSurface, it does not
         * matter whether the lists work with different triangulations,
         * or different encodings, or if one but not the other supports
         * almost normal and/or spun-normal surfaces.  The individual
         * surfaces will simply be compared by examining or computing
         * the number of discs of each type.
         *
         * In particular, this routine is safe to call even if this and the
         * given list work with different triangulations:
         *
         * - If the two triangulations have the same size, then this routine
         *   will compare surfaces as though they were transplanted into the
         *   same triangulation using the same tetrahedron numbering and the
         *   same disc types.
         *
         * - If the two triangulations have different sizes, then this
         *   comparison will return \c false.
         *
         * \param other the list to be compared with this list.
         * \return \c true if both lists represent the same multiset of
         * normal or almost normal surfaces, or \c false if not.
         */
        bool operator == (const NormalSurfaces& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Sorts the surfaces in this list according to the given criterion.
         *
         * This sort is stable, i.e., surfaces that are equivalent under the
         * given criterion will remain in the same relative order.
         *
         * The implementation of this routine uses std::stable_sort.
         *
         * \python This is available in Python, and \a comp may be
         * a pure Python function.
         *
         * \param comp a binary function (or other callable object) that
         * accepts two const NormalSurface references, and returns \c true
         * if and only if the first surface should appear before the second
         * in the sorted list.
         */
        template <typename Comparison>
        void sort(Comparison&& comp);

        /**
         * Returns the matching equations that were used to create this
         * normal surface list.  The matrix is not cached: multiple calls to
         * this routine will result in the construction of multiple matrices.
         * This routine in fact merely calls makeMatchingEquations() with the
         * appropriate parameters.
         *
         * The format of the matrix is identical to that returned by
         * makeMatchingEquations().
         *
         * Note that there are situations in which makeMatchingEquations()
         * throws an exception (because the triangulation is not supported
         * by the chosen coordinate system).  However, this routine will
         * always succeed, because if makeMatchingEquations() had failed
         * then this normal surface list would not have been created
         * in the first place.
         *
         * \return the matching equations used to create this normal
         * surface list.
         */
        MatrixInt recreateMatchingEquations() const;

        /**
         * Exports this list of normal surfaces as a plain text CSV
         * (comma-separated value) file, using standard coordinates.
         * CSV files are human-readable and human-editable, and are
         * suitable for importing into spreadsheets and databases.
         *
         * The surfaces will be exported in standard coordinates (tri-quad
         * coordinates for normal surfaces, or tri-quad-oct coordinates for
         * almost normal surfaces).  Each coordinate will become a separate
         * field in the CSV file.
         *
         * As well as the normal surface coordinates, additional properties
         * of the normal surfaces (such as Euler characteristic, orientability,
         * and so on) can be included as extra fields in the export.  Users can
         * select precisely which properties to include by passing a
         * bitwise OR combination of constants from the
         * regina::SurfaceExportFields enumeration type.
         *
         * The CSV format used here begins with a header row, and uses commas
         * as field separators.  Text fields with arbitrary contents are
         * placed inside double quotes, and the double quote character itself
         * is represented by a pair of double quotes.  Thus the string
         * ``my "normal" surface's name`` would be stored as
         * ``"my ""normal"" surface's name"``.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file _name_, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  Any user strings such as surface names will be written
         * in UTF-8.
         *
         * \param filename the name of the CSV file to export to.
         * \param additionalFields a bitwise OR combination of constants from
         * regina::SurfaceExportFields indicating which additional properties
         * of surfaces should be included in the export.
         * \return \c true if the export was successful, or \c false otherwise.
         */
        bool saveCSVStandard(const char* filename,
            SurfaceExport additionalFields = regina::surfaceExportAll) const;

        /**
         * Exports the given list of normal surfaces as a plain text CSV
         * (comma-separated value) file, using edge weight coordinates.
         * CSV files are human-readable and human-editable, and are
         * suitable for importing into spreadsheets and databases.
         *
         * The surfaces will be exported in edge weight coordinates.  Thus
         * there will be one coordinate for each edge of the underlying
         * triangulation; each such coordinate will become a separate field
         * in the CSV file.
         *
         * As well as the normal surface coordinates, additional properties
         * of the normal surfaces (such as Euler characteristic, orientability,
         * and so on) can be included as extra fields in the export.  Users can
         * select precisely which properties to include by passing a
         * bitwise OR combination of constants from the
         * regina::SurfaceExportFields enumeration type.
         *
         * The CSV format used here begins with a header row, and uses commas
         * as field separators.  Text fields with arbitrary contents are
         * placed inside double quotes, and the double quote character itself
         * is represented by a pair of double quotes.  Thus the string
         * ``my "normal" surface's name`` would be stored as
         * ``"my ""normal"" surface's name"``.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file _name_, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  Any user strings such as surface names will be written
         * in UTF-8.
         *
         * \param filename the name of the CSV file to export to.
         * \param additionalFields a bitwise OR combination of constants from
         * regina::SurfaceExportFields indicating which additional properties
         * of surfaces should be included in the export.
         * \return \c true if the export was successful, or \c false otherwise.
         */
        bool saveCSVEdgeWeight(const char* filename,
            SurfaceExport additionalFields = regina::surfaceExportAll) const;

        /**
         * A C++ iterator that gives access to the raw vectors for surfaces
         * in this list, pointing to the beginning of this surface list.
         *
         * \nopython Use vectors() instead, which returns an iterable object
         * for iterating over these same raw vectors.
         *
         * \return an iterator at the beginning of this surface list.
         */
        VectorIterator beginVectors() const;

        /**
         * A C++ iterator that gives access to the raw vectors for surfaces
         * in this list, pointing past the end of this surface list.
         * This iterator is not dereferenceable.
         *
         * \nopython Use vectors() instead, which returns an iterable object
         * for iterating over these same raw vectors.
         *
         * \return an iterator past the end of this surface list.
         */
        VectorIterator endVectors() const;

#ifdef __APIDOCS
        /**
         * Returns a Python iterable object that iterates over the raw
         * vectors for all surfaces in this list.  For example:
         *
         * \code{.py}
         * list = NormalSurfaces(...)
         * for v in list.vectors():
         *     ...
         * \endcode
         *
         * \nocpp For C++ users, NormalHypersurfaces provides beginVectors()
         * and endVectors() instead, which together define an iterator range
         * over these same raw vectors.
         *
         * \return an iterator over the normal surfaces in this list.
         */
        auto vectors() const;
#endif
        /**
         * A bidirectional iterator that runs through the raw vectors for
         * surfaces in this list.
         *
         * \nopython Instead NormalSurfaces::vectors() returns an object of a
         * different (hidden) class that supports the Python iterable/iterator
         * interface.
         */
        class VectorIterator {
            public:
                using iterator_category = std::bidirectional_iterator_tag;
                    /**< Declares this to be a bidirectional iterator type. */
                using value_type = Vector<LargeInteger>;
                    /**< Indicates what type the iterator points to. */
                using difference_type = typename
                    std::vector<NormalSurface>::const_iterator::difference_type;
                    /**< The type obtained by subtracting iterators. */
                using pointer = const Vector<LargeInteger>*;
                    /**< A pointer to \a value_type. */
                using reference = const Vector<LargeInteger>&;
                    /**< The type obtained when dereferencing iterators. */

            private:
                std::vector<NormalSurface>::const_iterator it_;
                    /**< An iterator into the underlying list of surfaces. */

            public:
                /**
                 * Creates a new uninitialised iterator.
                 */
                VectorIterator() = default;

                /**
                 * Creates a copy of the given iterator.
                 */
                VectorIterator(const VectorIterator&) = default;

                /**
                 * Makes this a copy of the given iterator.
                 *
                 * \return a reference to this iterator.
                 */
                VectorIterator& operator = (const VectorIterator&) = default;

                /**
                 * Compares this with the given iterator for equality.
                 *
                 * \return \c true if the iterators point to the same
                 * element of the same normal surface list, or \c false
                 * if they do not.
                 */
                bool operator == (const VectorIterator&) const = default;

                /**
                 * Returns the raw vector for the normal surface that this
                 * iterator is currently pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * \return the corresponding normal surface vector.
                 */
                const Vector<LargeInteger>& operator *() const;

                /**
                 * The preincrement operator.
                 *
                 * \return a reference to this iterator after the increment.
                 */
                VectorIterator& operator ++();

                /**
                 * The postincrement operator.
                 *
                 * \return a copy of this iterator before the
                 * increment took place.
                 */
                VectorIterator operator ++(int);

                /**
                 * The predecrement operator.
                 *
                 * \return a reference to this iterator after the decrement.
                 */
                VectorIterator& operator --();

                /**
                 * The postdecrement operator.
                 *
                 * \return a copy of this iterator before the
                 * decrement took place.
                 */
                VectorIterator operator --(int);

            private:
                /**
                 * Initialise a new vector iterator using an iterator for
                 * the internal list of normal surfaces.
                 */
                VectorIterator(
                    const std::vector<NormalSurface>::const_iterator& i);

            friend class NormalSurfaces;
        };

    protected:
        /**
         * Creates an empty list of normal surfaces with the given
         * parameters.
         */
        NormalSurfaces(NormalCoords coords, NormalList which,
            NormalAlg algorithm, const Triangulation<3>& triangulation);

        /**
         * Creates an empty list of normal surfaces with the given
         * parameters.
         */
        NormalSurfaces(NormalCoords coords, NormalList which,
            NormalAlg algorithm,
            const SnapshotRef<Triangulation<3>>& triangulation);

    private:
        /**
         * Converts a set of embedded vertex normal surfaces in
         * (quad or quad-oct) space to a set of embedded vertex normal
         * surfaces in (standard normal or standard almost normal) space.
         * The original surfaces are passed in the argument \a reducedList,
         * and the resulting surfaces will be inserted directly into this list.
         *
         * See NormalTransform::NS_CONV_REDUCED_TO_STD for full details
         * and preconditions for this procedure.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns.
         *
         * Although this is a template function, it is a private
         * template function and so is only defined in the one `.cpp`
         * file that needs it.
         *
         * \pre The coordinate system for this surface list is set to
         * NormalCoords::Standard or NormalCoords::AlmostNormal, and the
         * embedded-only flag is \c true.
         * \pre The underlying triangulation is valid, and the link
         * of every vertex is either a sphere or a disc.
         *
         * \param reducedList a full list of vertex surfaces in
         * (quad or quad-oct) coordinates for the underlying triangulation.
         * \param tracker a progress tracker to be used for progress reporting
         * and cancellation requests, or \c null if this is not required.
         */
        void buildStandardFromReduced(
            const std::vector<NormalSurface>& reducedList,
            ProgressTracker* tracker = nullptr);

        /**
         * Implements the one-template-argument version of
         * buildStandardFromReduced() using the specified bitmask type
         * to store zero sets.  See the one-template-argument
         * buildStandardFromReduced() for further information on this routine,
         * including important preconditions.
         *
         * The one-template-argument buildStandardFromReduced() simply
         * chooses an appropriate bitmask type and then calls this
         * routine, which does the real work.
         *
         * \pre The template argument \a BitmaskType can support
         * bitmasks of size 7 \a n (if we are using normal surfaces) or size
         * 10 \a n (if we are using almost normal surfaces), where \a n is
         * the number of tetrahedra in the underlying triangulation.
         * \pre The underlying triangulation (in addition to the other
         * preconditions) is non-empty.
         */
        template <class BitmaskType>
        void buildStandardFromReducedUsing(
            const std::vector<NormalSurface>& reducedList,
            ProgressTracker* tracker);

        /**
         * Converts a set of embedded vertex normal surfaces in
         * (standard normal or almost normal) space to a set of embedded
         * vertex normal surfaces in (quad or quad-oct) space.
         * The original surfaces are passed in the argument \a stdList, and the
         * resulting surfaces will be inserted directly into this list.
         *
         * See NormalTransform::NS_CONV_STD_TO_REDUCED for full details
         * and preconditions for this procedure.
         *
         * \pre The coordinate system for this surface list is set to
         * NormalCoords::Quad or NormalCoords::QuadOct, and the embedded-only
         * flag is set to \c true.
         * \pre The underlying triangulation is valid, and the link
         * of every vertex is either a sphere or a disc.
         *
         * \param stdList a full list of vertex surfaces in standard normal or
         * almost normal coordinates for the underlying triangulation.
         */
        void buildReducedFromStandard(
            const std::vector<NormalSurface>& stdList);

        /**
         * Contains the code responsible for all normal surface enumeration.
         */
        class Enumerator {
            private:
                NormalSurfaces* list_;
                    /**< The surface list to be filled. */
                const MatrixInt& eqns_;
                    /**< The matching equations for the given triangulation in
                         the coordinate system corresponding to list_. */
                ProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or \c null if no progress tracker is in use. */
                Packet* treeParent_;
                    /**< The parent packet in the tree, if we should insert the
                         finished list into the packet tree once enumeration
                         has finished, or \c null if we should not. */

            public:
                /**
                 * Creates a new functor with the given parameters.
                 *
                 * \pre If \a treeParent is non-null, then list is actually
                 * the inherited interface of a PacketOf<NormalSurfaces>.
                 */
                Enumerator(NormalSurfaces* list, const MatrixInt& eqns,
                    ProgressTracker* tracker, Packet* treeParent);

                /**
                 * Default move constructor.
                 */
                Enumerator(Enumerator&&) = default;

                /**
                 * Performs the real enumeration work.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ beneath \a treeParent_
                 * if \a treeParent_ is non-null.
                 */
                void enumerate();

                // Make this class non-copyable.
                Enumerator(const Enumerator&) = delete;
                Enumerator& operator = (const Enumerator&) = delete;

            private:
                /**
                 * The enumeration code for enumerating vertex surfaces.
                 * This is internal to enumerate().
                 *
                 * We assume that the flag set which_ is set correctly,
                 * and we do not alter it here.
                 * We make no assumptions about the state of algorithm_,
                 * and we set this during the course of this routine.
                 *
                 * This routine only fills \a list_ with surfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 */
                void fillVertex();

                /**
                 * The enumeration code for enumerating fundamental surfaces.
                 * This is internal to enumerate().
                 *
                 * We assume that the flag set which_ is set correctly,
                 * and we do not alter it here.
                 * We make no assumptions about the state of algorithm_,
                 * and we set this during the course of this routine.
                 *
                 * This routine only fills \a list_ with surfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 */
                void fillFundamental();

                /**
                 * The enumeration code for enumerating vertex surfaces
                 * using the double description method.
                 * This is internal to fillVertex().
                 *
                 * This routine assumes that \a algorithm_ has been set
                 * correctly, and does not alter it.
                 *
                 * If \a tracker_ is non-null, this routine assumes that
                 * an appropriate tracker stage has already been
                 * declared, and works through that stage only.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillVertexDD();

                /**
                 * The enumeration code for enumerating vertex surfaces
                 * using the tree traversal method.
                 * This is internal to fillVertex().
                 *
                 * This routine assumes that \a algorithm_ has been set
                 * correctly, and does not alter it.
                 *
                 * If \a tracker_ is non-null, this routine assumes that
                 * an appropriate tracker stage has already been
                 * declared, and works through that stage only.
                 *
                 * \pre We are enumerating embedded surfaces only.
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillVertexTree();

                /**
                 * Internal code for fillVertexTree() in which the underlying
                 * integer type for the tree traversal method is fixed.
                 *
                 * This does all of the work for fillVertexTree(), aside
                 * from the initial selection of an integer type.  See
                 * the nodes for fillVertexTree() for further details.
                 *
                 * \pre We are enumerating embedded surfaces only.
                 * \pre The underlying triangulation is non-empty.
                 * \pre The given integer type is known to be sufficient
                 * (i.e., will not overflow) for the enumeration problem
                 * under consideration.
                 */
                template <typename Integer>
                void fillVertexTreeWith();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the primal method.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillFundamentalPrimal();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the dual method.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillFundamentalDual();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using a slow Contejean-Devie method.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillFundamentalCD();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using a slow full cone enumeration.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                void fillFundamentalFullCone();
        };

    friend class XMLNormalSurfacesReader;
    friend class XMLLegacyNormalSurfacesReader;
    friend class XMLWriter<NormalSurfaces>;
};

/**
 * Swaps the contents of the two given lists.
 *
 * This global routine simply calls NormalSurfaces::swap(); it is provided so
 * that NormalSurfaces meets the C++ Swappable requirements.
 *
 * See NormalSurfaces::swap() for more details.
 *
 * \note This swap function is _not_ marked \c noexcept, since it
 * fires change events on both lists which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * \param lhs the list whose contents should be swapped with \a rhs.
 * \param rhs the list whose contents should be swapped with \a lhs.
 *
 * \ingroup surfaces
 */
void swap(NormalSurfaces& lhs, NormalSurfaces& rhs);

/**
 * Generates the set of normal surface matching equations for the
 * given triangulation using the given coordinate system.
 *
 * These are the matching equations that will be used when enumerating
 * normal surfaces in the coordinate system \a coords.
 *
 * Each equation will be represented as a row of the resulting matrix.
 * Each column of the matrix represents a coordinate in the given
 * coordinate system.
 *
 * \exception InvalidArgument The matching equations could not be created for
 * the given triangulation in the given coordinate system, due to an error
 * that should have been preventable with the right checks in advance.  This
 * can only happen in certain coordinate systems, and for all such coordinate
 * systems this is explicitly described in the NormalCoords enum documentation.
 *
 * \exception UnsolvedCase The matching equations could not be created for the
 * given triangulation in the given coordinate system, due to an error that was
 * "genuinely" unforseeable.  Again this can only happen in certain coordinate
 * systems, where this is explicitly described in the NormalCoords enum
 * documentation.
 *
 * \param triangulation the triangulation upon which these matching equations
 * will be based.
 * \param coords the coordinate system to be used.
 * \return the resulting set of matching equations.
 *
 * \ingroup surfaces
 */
MatrixInt makeMatchingEquations(const Triangulation<3>& triangulation,
    NormalCoords coords);

/**
 * Generates the validity constraints representing the condition that
 * normal surfaces be embedded.  The validity constraints will be expressed
 * relative to the given coordinate system.
 *
 * For some coordinate systems, these will include additional constraints
 * of a similar nature (i.e., restricting which combinations of
 * coordinates may be non-zero).  For instance, in almost normal coordinates,
 * there will typically be an extra constraint insisting that at most
 * one octagon type is non-zero across the entire triangulation.
 *
 * These are the constraints that will be used when enumerating embedded
 * surfaces in the given coordinate system (i.e., when the default
 * NS_EMBEDDED_ONLY flag is used).  They will not be used when the enumeration
 * allows for immersed and/or singular surfaces.
 *
 * \param triangulation the triangulation upon which these validity constraints
 * will be based.
 * \param coords the coordinate system to be used.
 * \return the set of validity constraints.
 *
 * \ingroup surfaces
 */
ValidityConstraints makeEmbeddedConstraints(
    const Triangulation<3>& triangulation, NormalCoords coords);

// Inline functions for NormalSurfaces

inline NormalSurfaces::NormalSurfaces(const Triangulation<3>& triangulation,
        NormalCoords coords, NormalList which, NormalAlg algHints,
        ProgressTracker* tracker) :
        triangulation_(triangulation), coords_(coords), which_(which),
        algorithm_(algHints) {
    try {
        Enumerator(this, makeMatchingEquations(triangulation, coords),
            tracker, nullptr).enumerate();
    } catch (const ReginaException&) {
        if (tracker)
            tracker->setFinished();
        throw;
    }
}

inline NormalSurfaces& NormalSurfaces::operator = (const NormalSurfaces& src) {
    PacketChangeSpan span(*this);

    surfaces_ = src.surfaces_;
    triangulation_ = src.triangulation_;
    coords_ = src.coords_;
    which_ = src.which_;
    algorithm_ = src.algorithm_;

    return *this;
}

inline NormalSurfaces& NormalSurfaces::operator = (NormalSurfaces&& src) {
    PacketChangeSpan span(*this);

    surfaces_ = std::move(src.surfaces_);
    triangulation_ = std::move(src.triangulation_);

    // Trivial data members:
    coords_ = src.coords_;
    which_ = src.which_;
    algorithm_ = src.algorithm_;

    return *this;
}

inline NormalCoords NormalSurfaces::coords() const {
    return coords_;
}

inline NormalList NormalSurfaces::which() const {
    return which_;
}

inline NormalAlg NormalSurfaces::algorithm() const {
    return algorithm_;
}

inline bool NormalSurfaces::isEmbeddedOnly() const {
    return which_.has(NS_EMBEDDED_ONLY);
}

inline const Triangulation<3>& NormalSurfaces::triangulation() const {
    return *triangulation_;
}

inline size_t NormalSurfaces::size() const {
    return surfaces_.size();
}

inline const NormalSurface& NormalSurfaces::surface(size_t index) const {
    return surfaces_[index];
}

inline const NormalSurface& NormalSurfaces::operator [](size_t index) const {
    return surfaces_[index];
}

inline auto NormalSurfaces::begin() const {
    return surfaces_.begin();
}

inline auto NormalSurfaces::end() const {
    return surfaces_.end();
}

inline bool NormalSurfaces::allowsAlmostNormal() const {
    return NormalEncoding(coords_).storesOctagons();
}

inline bool NormalSurfaces::allowsNonCompact() const {
    return NormalEncoding(coords_).couldBeNonCompact();
}

template <typename Comparison>
inline void NormalSurfaces::sort(Comparison&& comp) {
    PacketChangeSpan span(*this);
    std::stable_sort(surfaces_.begin(), surfaces_.end(), comp);
}

inline const Vector<LargeInteger>& NormalSurfaces::VectorIterator::
        operator *() const {
    return it_->vector();
}

inline NormalSurfaces::VectorIterator& NormalSurfaces::VectorIterator::
        operator ++() {
    ++it_;
    return *this;
}

inline NormalSurfaces::VectorIterator NormalSurfaces::VectorIterator::
        operator ++(int) {
    return NormalSurfaces::VectorIterator(it_++);
}

inline NormalSurfaces::VectorIterator& NormalSurfaces::VectorIterator::
        operator --() {
    --it_;
    return *this;
}

inline NormalSurfaces::VectorIterator NormalSurfaces::VectorIterator::
        operator --(int) {
    return NormalSurfaces::VectorIterator(it_--);
}

inline NormalSurfaces::VectorIterator::VectorIterator(
        const std::vector<NormalSurface>::const_iterator& i) : it_(i) {
}

inline NormalSurfaces::VectorIterator NormalSurfaces::beginVectors() const {
    return VectorIterator(surfaces_.begin());
}

inline NormalSurfaces::VectorIterator NormalSurfaces::endVectors() const {
    return VectorIterator(surfaces_.end());
}

inline NormalSurfaces::NormalSurfaces(NormalCoords coords, NormalList which,
        NormalAlg algorithm, const Triangulation<3>& triangulation) :
        triangulation_(triangulation), coords_(coords), which_(which),
        algorithm_(algorithm) {
}

inline NormalSurfaces::NormalSurfaces(NormalCoords coords, NormalList which,
        NormalAlg algorithm,
        const SnapshotRef<Triangulation<3>>& triangulation) :
        triangulation_(triangulation), coords_(coords), which_(which),
        algorithm_(algorithm) {
}

inline void swap(NormalSurfaces& lhs, NormalSurfaces& rhs) {
    lhs.swap(rhs);
}

inline NormalSurfaces::Enumerator::Enumerator(NormalSurfaces* list,
        const MatrixInt& eqns, ProgressTracker* tracker, Packet* treeParent) :
        list_(list), eqns_(eqns), tracker_(tracker), treeParent_(treeParent) {
}

} // namespace regina

#endif
