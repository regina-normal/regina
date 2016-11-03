
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file surfaces/normalsurfaces.h
 *  \brief Contains a packet representing a collection of normal
 *  surfaces in a 3-manifold.
 */

#ifndef __NORMALSURFACES_H
#ifndef __DOXYGEN
#define __NORMALSURFACES_H
#endif

#include <algorithm>
#include <iterator>
#include <vector>
#include "regina-core.h"
#include "packet/packet.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalflags.h"
#include "surfaces/normalcoords.h"
#include "utilities/memutils.h"

namespace regina {

class NormalSurfaces;
class ProgressTracker;
class XMLPacketReader;

template <typename> class MatrixIntDomain;
typedef MatrixIntDomain<Integer> MatrixInt;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Used to describe a field, or a set of fields, that can be exported
 * alongside a normal surface list.  This enumeration type is used with
 * export routines such as NormalSurfaces::saveCSVStandard() or
 * NormalSurfaces::saveCSVEdgeWeight().
 *
 * This type describes fields in addition to normal coordinates, not the
 * normal coordinates themselves (which are always exported).  Each field
 * describes some property of a single normal surface, and corresponds to a
 * single column in a table of normal surfaces.
 *
 * This type should be treated as a bitmask: you can describe a set of fields
 * by combining the values for individual fields using bitwise \e or.
 *
 * The list of available fields may grow with future releases of Regina.
 */
enum SurfaceExportFields {
    surfaceExportName = 0x0001,
        /**< Represents the user-assigned surface name. */
    surfaceExportEuler = 0x0002,
        /**< Represents the calculated Euler characteristic of a
             surface.  This will be an integer, and will be left empty
             if the Euler characteristic cannot be computed. */
    surfaceExportOrient = 0x0004,
        /**< Represents the calculated property of whether a surface is
             orientable.  This will be the string \c TRUE or \c FALSE, or
             will be left empty if the orientability cannot be computed. */
    surfaceExportSides = 0x0008,
        /**< Represents the calculated property of whether a surface is
             one-sided or two-sided.  This will be the integer 1 or 2,
             or will be left empty if the "sidedness" cannot be computed. */
    surfaceExportBdry = 0x0010,
        /**< Represents the calculated property of whether a surface is
             bounded.  In most cases, this will be one of the strings "closed",
             "real bdry" or "infinite" (where "infinite" indicates a
             surface with infinitely many discs).  For spun-normal
             surfaces in certain ideal triangulations, this string will
             be followed by the boundary slopes of the surface at the
             cusps: these written as a list of pairs (\a p, \a q),
             one for each cusp, indicating that the boundary curves of
             the surface run \a p times around the meridian and \a q times
             around the longitude.  See NormalSurface::boundaryIntersections()
             for further information on interpreting these values. */
    surfaceExportLink = 0x0020,
        /**< Represents whether a surface is a single vertex link or a
             thin edge link.  See NormalSurface::isVertexLink() and
             NormalSurface::isThinEdgeLink() for details.  This will be
             written as a human-readable string. */
    surfaceExportType = 0x0040,
        /**< Represents any additional high-level properties of a
             surface, such as whether it is a splitting surface or a
             central surface.  This will be written as a human-readable
             string.  This field is somewhat arbitrary, and the precise
             properties it describes are subject to change in future
             releases of Regina. */

    surfaceExportNone = 0,
        /**< Indicates that no additional fields should be exported. */
    surfaceExportAllButName = 0x007e,
        /**< Indicates that all available fields should be exported,
             except for the user-assigned surface name.  Since the list
             of available fields may grow with future releases, the numerical
             value of this constant may change as a result. */
    surfaceExportAll = 0x007f
        /**< Indicates that all available fields should be exported,
             including the user-assigned surface name.  Since the list
             of available fields may grow with future releases, the numerical
             value of this constant may change as a result. */
};

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_NORMALSURFACES> {
    typedef NormalSurfaces Class;
    inline static const char* name() {
        return "Normal Surface List";
    }
};
#endif

/**
 * A packet representing a collection of normal surfaces in a 3-manifold.
 * Such a packet must always be a child packet of the triangulation from
 * which the surfaces were obtained.  If this triangulation changes, the
 * information contained in this packet will become invalid.
 *
 * See the NormalSurfaceVector class notes for details of what to do
 * when introducing a new coordinate system.
 *
 * Normal surface lists should be created using the routine enumerate().
 *
 * \todo \feature Allow custom matching equations.
 * \todo \feature Allow enumeration with some coordinates explicitly set
 * to zero.
 * \todo \feature Allow generating only closed surfaces.
 * \todo \feature Generate facets of the solution space representing
 * embedded surfaces.
 */
class REGINA_API NormalSurfaces : public Packet {
    REGINA_PACKET(NormalSurfaces, PACKET_NORMALSURFACES)

    public:
        class VectorIterator;

    protected:
        std::vector<NormalSurface*> surfaces;
            /**< Contains the normal surfaces stored in this packet. */
        NormalCoords coords_;
            /**< Stores which coordinate system is being
                 used by the normal surfaces in this packet. */
        NormalList which_;
            /**< Indicates which normal surfaces these represent within
                 the underlying triangulation. */
        NormalAlg algorithm_;
            /**< Stores the details of the enumeration algorithm that
                 was used to generate this list.  This might not be the
                 same as the \a algorithmHints flag passed to the
                 corresponding enumeration routine (e.g., if invalid
                 or inappropriate flags were passed). */

    public:
        /**
         * Destroys this list and all the surfaces within.
         */
        virtual ~NormalSurfaces();

        /**
         * A unified routine for enumerating various classes of normal
         * surfaces within a given triangulation.
         *
         * The NormalCoords argument allows you to specify an underlying
         * coordinate system (e.g., standard coordinates,
         * quadrilateral coordinates or almost normal coordinates).
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
         * The enumerated surfaces will be stored in a new normal
         * surface list, and their representations will be scaled down
         * to use the smallest possible integer coordinates.
         * This normal surface list will be inserted into the packet tree as
         * the last child of the given triangulation.  This triangulation
         * \b must remain the parent of this normal surface list, and must not
         * change while this normal surface list remains in existence.
         *
         * If a progress tracker is passed, the normal surface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the normal surface list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress tracking,
         * this routine will declare and work through a series of stages
         * whose combined weights sum to 1; typically this means that the
         * given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param coords the coordinate system to be used.
         * @param which indicates which normal surfaces should be enumerated.
         * @param algHints passes requests to Regina for which specific
         * enumeration algorithm should be used.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created normal surface list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal surface list is created).
         */
        static NormalSurfaces* enumerate(Triangulation<3>* owner,
            NormalCoords coords,
            NormalList which = NS_LIST_DEFAULT,
            NormalAlg algHints = NS_ALG_DEFAULT,
            ProgressTracker* tracker = 0);

        /**
         * Returns the coordinate system being used by the
         * surfaces stored in this set.
         *
         * @return the coordinate system used.
         */
        NormalCoords coords() const;
        /**
         * Returns details of which normal surfaces this list represents
         * within the underlying triangulation.
         *
         * This may not be the same NormalList that was passed to
         * enumerate().  In particular, default values will have been
         * explicitly filled in (such as NS_VERTEX and/or NS_EMBEDDED_ONLY),
         * and invalid and/or redundant values will have been removed.
         *
         * @return details of what this list represents.
         */
        NormalList which() const;
        /**
         * Returns details of the algorithm that was used to enumerate
         * this list.
         *
         * These may not be the same NormalAlg flags that were passed to
         * enumerate().  In particular, default values will have been
         * explicitly filled in, invalid and/or redundant values will have
         * been removed, and unavailable and/or unsupported combinations
         * of algorithm flags will be replaced with whatever algorithm was
         * actually used.
         *
         * @return details of the algorithm used to enumerate this list.
         */
        NormalAlg algorithm() const;
        /**
         * Determines if the coordinate system being used
         * allows for almost normal surfaces, that is, allows for
         * octagonal discs.
         *
         * @return \c true if and only if almost normal surfaces are
         * allowed.
         */
        bool allowsAlmostNormal() const;
        /**
         * Determines if the coordinate system being used
         * allows for spun normal surfaces.
         *
         * @return \c true if and only if spun normal surface are
         * supported.
         */
        bool allowsSpun() const;
        /**
         * Determines if the coordinate system being used
         * allows for transversely oriented normal surfaces.
         *
         * @return \c true if and only if transverse orientations are
         * supported.
         */
        bool allowsOriented() const;
        /**
         * Returns whether this list was constructed to contain only
         * properly embedded surfaces.
         *
         * If this returns \c false, it does not guarantee that immersed
         * and/or singular surfaces are present; it merely indicates
         * that they were not deliberately excluded (for instance, the
         * quadrilateral constraints were not enforced).
         *
         * @return \c true if this list was constructed to contain only
         * properly embedded surfaces, or \c false otherwise.
         */
        bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal surfaces live.
         * 
         * @return the triangulation in which these surfaces live.
         */
        Triangulation<3>* triangulation() const;
        /**
         * Returns the number of surfaces stored in this list.
         *
         * @return the number of surfaces.
         */
        size_t size() const;
        /**
         * Returns the surface at the requested index in this set.
         *
         * @param index the index of the requested surface in this set;
         * this must be between 0 and size()-1 inclusive.
         *
         * @return the normal surface at the requested index in this set.
         */
        const NormalSurface* surface(size_t index) const;
        /**
         * Writes the number of surfaces in this set followed by the
         * details of each surface to the given output stream.  Output
         * will be over many lines.
         *
         * \ifacespython Parameter \a out is not present and is assumed
         * to be standard output.
         *
         * @param out the output stream to which to write.
         */
        void writeAllSurfaces(std::ostream& out) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

        /**
         * Converts the set of all embedded vertex normal surfaces in
         * quadrilateral space to the set of all embedded vertex normal
         * surfaces in standard (tri-quad) space.  The initial list in
         * quadrilateral space is taken to be this normal surface list;
         * the final list in standard space will be inserted as a new
         * child packet of the underlying triangulation (specifically, as
         * the final child).  As a convenience, the final list will also
         * be returned from this routine.
         *
         * This routine can only be used with normal surfaces, not almost
         * normal surfaces.  For almost normal surfaces, see the similar
         * routine quadOctToStandardAN().
         *
         * This procedure is available for any triangulation whose vertex
         * links are all spheres and/or discs, and is \e much faster than
         * enumerating surfaces directly in standard tri-quad coordinates.
         * The underlying algorithm is described in detail in "Converting
         * between quadrilateral and standard solution sets in normal
         * surface theory", Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009),
         * 2121-2174.
         *
         * Typically users do not need to call this routine directly,
         * since the standard enumerate() routine will use it implicitly
         * where possible.  That is, when asked for standard vertex surfaces,
         * enumerate() will first find all \e quadrilateral vertex surfaces
         * and then use this procedure to convert them to standard vertex
         * surfaces; this is generally orders of magnitude faster than
         * enumerating surfaces directly in standard coordinates.
         *
         * Nevertheless, this standalone routine is provided as a convenience
         * for users who already have a set of quadrilateral vertex surfaces,
         * and who simply wish to convert them to a set of standard
         * vertex surfaces without the cost of implicitly enumerating the
         * quadrilateral vertex surfaces again.
         *
         * It should be noted that this routine does \e not simply convert
         * vectors from one form to another; instead it converts a full
         * solution set of vertex surfaces in quadrilateral coordinates to a
         * full solution set of vertex surfaces in standard coordinates.
         * Typically there are many more vertex surfaces in standard
         * coordinates (all of which this routine will find).
         *
         * This routine will run some very basic sanity checks before
         * starting.  Specifically, it will check the validity and vertex
         * links of the underlying triangulation, and will verify that
         * the coordinate system and embedded-only flag are set to
         * NS_QUAD and \c true respectively.  If any of
         * these checks fails, this routine will do nothing and return 0.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex normal surfaces in quadrilateral space; no more,
         * no less.  Moreover, these vectors are stored using quadrilateral
         * coordinates.  Typically this means that it was obtained through
         * enumerate(), with the coordinate system set to NS_QUAD and
         * with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex normal surfaces in standard (tri-quad)
         * coordinates, or 0 if any of the basic sanity checks failed.
         */
        NormalSurfaces* quadToStandard() const;

        /**
         * Converts the set of all embedded vertex almost normal surfaces in
         * quadrilateral-octagon space to the set of all embedded vertex
         * almost normal surfaces in the standard tri-quad-oct space.
         *
         * This routine is the almost normal analogue to the
         * quadToStandard() conversion routine; see the quadToStandard()
         * documentation for further information.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This surface list is precisely the set of all embedded vertex
         * almost normal surfaces in quadrilateral-octagon space; no more,
         * no less.  Moreover, these vectors are stored using
         * quadrilateral-octagon coordinates.  Typically this means that it
         * was obtained through enumerate(), with the coordinate system set to
         * NS_AN_QUAD_OCT and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex almost normal surfaces in standard
         * tri-quad-oct coordinates, or 0 if any of the basic sanity checks
         * failed.
         */
        NormalSurfaces* quadOctToStandardAN() const;

        /**
         * Converts the set of all embedded vertex normal surfaces in
         * standard (tri-quad) space to the set of all embedded vertex
         * normal surfaces in quadrilateral space.  The initial list in
         * standard space is taken to be this normal surface list;
         * the final list in quadrilateral space will be inserted as a new
         * child packet of the underlying triangulation (specifically, as
         * the final child).  As a convenience, the final list will also
         * be returned from this routine.
         *
         * This routine can only be used with normal surfaces, not almost
         * normal surfaces.  For almost normal surfaces, see the similar
         * routine standardANToQuadOct().
         *
         * This procedure is available for any triangulation whose vertex
         * links are all spheres and/or discs.  The underlying algorithm
         * is described in detail in "Converting between quadrilateral and
         * standard solution sets in normal surface theory",
         * Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.
         *
         * It should be noted that this routine does \e not simply convert
         * vectors from one form to another; instead it converts a full
         * solution set of vertex surfaces in standard coordinates to a
         * full solution set of vertex surfaces in quadrilateral coordinates.
         * Typically there are far fewer vertex surfaces in quadrilateral
         * coordinates (all of which this routine will find).
         *
         * This routine will run some very basic sanity checks before
         * starting.  Specifically, it will check the validity and vertex
         * links of the underlying triangulation, and will verify that
         * the coordinate system and embedded-only flag are set to
         * NS_STANDARD and \c true respectively.  If any of
         * these checks fails, this routine will do nothing and return 0.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex normal surfaces in standard (tri-quad) space;
         * no more, no less.  Moreover, these vectors are stored using
         * standard coordinates.  Typically this means that this list was
         * obtained through enumerate(), with the coordinate system set to
         * NS_STANDARD and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex normal surfaces in quadrilateral
         * coordinates, or 0 if any of the basic sanity checks failed.
         */
        NormalSurfaces* standardToQuad() const;

        /**
         * Converts the set of all embedded vertex almost normal surfaces in
         * standard tri-quad-oct space to the set of all embedded vertex
         * almost normal surfaces in the smaller quadrilateral-octagon space.
         *
         * This routine is the almost normal analogue to the
         * standardToQuad() conversion routine; see the standardToQuad()
         * documentation for further information.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex almost normal surfaces in standard tri-quad-oct
         * space; no more, no less.  Typically this means that it was obtained
         * through enumerate(), with the coordinate system set to
         * NS_AN_STANDARD and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex almost normal surfaces in
         * quadrilateral-octagon coordinates, or 0 if any of the basic
         * sanity checks failed.
         */
        NormalSurfaces* standardANToQuadOct() const;

        /**
         * Creates a new list filled with the surfaces from this list
         * that have at least one locally compatible partner.
         * In other words, a surface \a S from this list will be placed
         * in the new list if and only if there is some other surface \a T
         * in this list for which \a S and \a T are locally compatible.
         * See NormalSurface::locallyCompatible() for further details on
         * compatibility testing.
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         *
         * \warning If this list contains a vertex link (plus at least
         * one other surface), then the new list will be identical to
         * the old (i.e., every surface will be copied across).
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NormalSurfaces* filterForLocallyCompatiblePairs() const;

        /**
         * Creates a new list filled with the surfaces from this list
         * that have at least one disjoint partner.
         * In other words, a surface \a S from this list will be placed
         * in the new list if and only if there is some other surface \a T
         * in this list for which \a S and \a T can be made to intersect
         * nowhere at all, without changing either normal isotopy class.
         * See NormalSurface::disjoint() for further details on disjointness
         * testing.
         *
         * This routine cannot deal with empty, disconnected or
         * non-compact surfaces.  Such surfaces will be silently
         * ignored, and will not be used in any disjointness tests (in
         * particular, they will never be considered as a "disjoint partner"
         * for any other surface).
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         * \pre All surfaces within this list are stored using the same
         * coordinate system (i.e., the same subclass of NormalSurfaceVector).
         *
         * \warning If this list contains a vertex link (plus at least
         * one other surface), then the new list will be identical to
         * the old (i.e., every surface will be copied across).
         *
         * \todo Deal properly with surfaces that are too large to handle.
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NormalSurfaces* filterForDisjointPairs() const;

        /**
         * Creates a new list filled with only the surfaces from this list
         * that "might" represent two-sided incompressible surfaces.
         * More precisely, we consider all two-sided surfaces in this list,
         * as well as the two-sided double covers of all one-sided surfaces
         * in this list (see below for details on how one-sided surfaces
         * are handled).  Each of these surfaces is examined using
         * relatively fast heuristic tests for incompressibility.  Any
         * surface that is definitely \e not incompressible is thrown
         * away, and all other surfaces are placed in the new list.
         *
         * Therefore, it is guaranteed that every incompressible surface
         * from the old list will be placed in the new list.  However,
         * it is not known whether any given surface in the new list is
         * indeed incompressible.
         *
         * See NormalSurface::isIncompressible() for the definition of
         * incompressibility that is used here.  Note in particular that
         * spheres are \e never considered incompressible.
         *
         * As indicated above, this filter works exclusively with two-sided
         * surfaces.  If a surface in this list is one-sided, the heuristic
         * incompressibility tests will be run on its two-sided double cover.
         * Nevertheless, if the tests pass, the original one-sided surface
         * (not the double cover) will be added to the new list.
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * Currently the heuristic tests include (i) throwing away
         * all vertex links and thin edge links, and then
         * (ii) cutting along the remaining surfaces and running
         * Triangulation<3>::hasSimpleCompressingDisc() on the resulting
         * bounded triangulations.  For more details on these tests
         * see "The Weber-Seifert dodecahedral space is non-Haken",
         * Benjamin A. Burton, J. Hyam Rubinstein and Stephan Tillmann,
         * Trans. Amer. Math. Soc. 364:2 (2012), pp. 911-932.
         *
         * \pre The underlying 3-manifold triangulation is valid and closed.
         * In particular, it has no ideal vertices.
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         * \pre This list contains only compact, connected normal surfaces.
         * \pre No surfaces in this list contain any octagonal discs.
         *
         * \warning The behaviour of this routine is subject to change
         * in future versions of Regina, since additional tests may be
         * added to improve the power of this filtering.
         *
         * \todo Add progress tracking.
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NormalSurfaces* filterForPotentiallyIncompressible() const;

        /**
         * Returns a newly created matrix containing the matching
         * equations that were used to create this normal surface list.
         * The destruction of this matrix is the responsibility of the
         * caller of this routine.  Multiple calls to this routine will
         * result in the construction of multiple matrices.  This
         * routine in fact merely calls makeMatchingEquations() with the
         * appropriate parameters.
         *
         * The format of the matrix is identical to that returned by
         * makeMatchingEquations().
         * 
         * @return the matching equations used to create this normal
         * surface list.
         */
        MatrixInt* recreateMatchingEquations() const;

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
         * select precisely which properties to include by passing a bitmask,
         * formed as a bitwise \e or combination of constants from
         * the regina::SurfaceExportFields enumeration type.
         *
         * The CSV format used here begins with a header row, and uses commas
         * as field separators.  Text fields with arbitrary contents are
         * placed inside double quotes, and the double quote character itself
         * is represented by a pair of double quotes.  Thus the string
         * <tt>my "normal" surface's name</tt> would be stored as
         * <tt>"my ""normal"" surface's name"</tt>.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  Any user strings such as surface names will be written
         * in UTF-8.
         *
         * @param filename the name of the CSV file to export to.
         * @param additionalFields a bitwise combination of constants from
         * regina::SurfaceExportFields indicating which additional properties
         * of surfaces should be included in the export.
         * @return \c true if the export was successful, or \c false otherwise.
         */
        bool saveCSVStandard(const char* filename,
            int additionalFields = regina::surfaceExportAll);

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
         * select precisely which properties to include by passing a bitmask,
         * formed as a bitwise \e or combination of constants from
         * the regina::SurfaceExportFields enumeration type.
         *
         * The CSV format used here begins with a header row, and uses commas
         * as field separators.  Text fields with arbitrary contents are
         * placed inside double quotes, and the double quote character itself
         * is represented by a pair of double quotes.  Thus the string
         * <tt>my "normal" surface's name</tt> would be stored as
         * <tt>"my ""normal"" surface's name"</tt>.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  Any user strings such as surface names will be written
         * in UTF-8.
         *
         * @param filename the name of the CSV file to export to.
         * @param additionalFields a bitwise combination of constants from
         * regina::SurfaceExportFields indicating which additional properties
         * of surfaces should be included in the export.
         * @return \c true if the export was successful, or \c false otherwise.
         */
        bool saveCSVEdgeWeight(const char* filename,
            int additionalFields = regina::surfaceExportAll);

        /**
         * An iterator that gives access to the raw vectors for surfaces in
         * this list, pointing to the beginning of this surface list.
         *
         * \ifacespython Not present.
         *
         * @return an iterator at the beginning of this surface list.
         */
        VectorIterator beginVectors() const;

        /**
         * An iterator that gives access to the raw vectors for surfaces in
         * this list, pointing past the end of this surface list.
         * This iterator is not dereferenceable.
         *
         * \ifacespython Not present.
         *
         * @return an iterator past the end of this surface list.
         */
        VectorIterator endVectors() const;

        /**
         * A bidirectional iterator that runs through the raw vectors for
         * surfaces in this list.
         *
         * \ifacespython Not present.
         */
        class VectorIterator : public std::iterator<
                std::bidirectional_iterator_tag, Ray> {
            private:
                std::vector<NormalSurface*>::const_iterator it_;
                    /**< An iterator into the underlying list of surfaces. */

            public:
                /**
                 * Creates a new uninitialised iterator.
                 */
                VectorIterator();

                /**
                 * Creates a copy of the given iterator.
                 *
                 * @param cloneMe the iterator to clone.
                 */
                VectorIterator(const VectorIterator& cloneMe);

                /**
                 * Makes this a copy of the given iterator.
                 *
                 * @param cloneMe the iterator to clone.
                 * @return a reference to this iterator.
                 */
                VectorIterator& operator = (const VectorIterator& cloneMe);

                /**
                 * Compares this with the given operator for equality.
                 *
                 * @param other the iterator to compare this with.
                 * @return \c true if the iterators point to the same
                 * element of the same normal surface list, or \c false
                 * if they do not.
                 */
                bool operator == (const VectorIterator& other) const;

                /**
                 * Compares this with the given operator for inequality.
                 *
                 * @param other the iterator to compare this with.
                 * @return \c false if the iterators point to the same
                 * element of the same normal surface list, or \c true
                 * if they do not.
                 */
                bool operator != (const VectorIterator& other) const;

                /**
                 * Returns the raw vector for the normal surface that this
                 * iterator is currently pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * @return the corresponding normal surface vector.
                 */
                const Ray& operator *() const;

                /**
                 * The preincrement operator.
                 *
                 * @return a reference to this iterator after the increment.
                 */
                VectorIterator& operator ++();

                /**
                 * The postincrement operator.
                 *
                 * @return a copy of this iterator before the
                 * increment took place.
                 */
                VectorIterator operator ++(int);

                /**
                 * The predecrement operator.
                 *
                 * @return a reference to this iterator after the decrement.
                 */
                VectorIterator& operator --();

                /**
                 * The postdecrement operator.
                 *
                 * @return a copy of this iterator before the
                 * decrement took place.
                 */
                VectorIterator operator --(int);

            private:
                /**
                 * Initialise a new vector iterator using an iterator for
                 * the internal list of normal surfaces.
                 */
                VectorIterator(
                    const std::vector<NormalSurface*>::const_iterator& i);

            friend class NormalSurfaces;
        };

    protected:
        /**
         * Creates an empty list of normal surfaces with the given
         * parameters.
         *
         * @param coords the coordinate system to be used
         * for filling this list.
         * @param which indicates which normal surfaces these will
         * represent within the underlying triangulation.
         * @param algorithm details of the enumeration algorithm that
         * will be used to fill this list.
         */
        NormalSurfaces(NormalCoords coords, NormalList which,
            NormalAlg algorithm);

        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * An output iterator used to insert surfaces into an
         * NormalSurfaces.
         *
         * Objects of type <tt>NormalSurface*</tt> and
         * <tt>NormalSurfaceVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NormalSurface
         * will be automatically created.
         *
         * \warning The behaviour of this class has changed!
         * As of Regina 4.6, this class happily inserts every surface or
         * vector that it is given.  In previous versions it checked
         * almost normal surface vectors for multiple octagonal discs;
         * this check has been removed to support conversions between
         * quad-oct space and standard almost normal space, and to support
         * the enumeration of \e all almost normal surfaces (as opposed to
         * just vertex surfaces).  Such checks are now left to the user
         * interface (and indeed are now optional, at the user's discretion).
         */
        struct SurfaceInserter : public std::iterator<
                std::output_iterator_tag, NormalSurfaceVector*> {
            NormalSurfaces* list;
                /**< The list into which surfaces will be inserted. */
            Triangulation<3>* owner;
                /**< The triangulation in which the surfaces to be
                 *   inserted are contained. */

            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param newList the list into which surfaces will be inserted.
             * @param newOwner the triangulation in which the surfaces
             * to be inserted are contained.
             */
            SurfaceInserter(NormalSurfaces& newList,
                Triangulation<3>* newOwner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            SurfaceInserter(const SurfaceInserter& cloneMe);

            /**
             * Sets this iterator to be a clone of the given output iterator.
             *
             * @param cloneMe the output iterator to clone.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(const SurfaceInserter& cloneMe);

            /**
             * Appends a normal surface to the end of the appropriate
             * surface list.
             *
             * The given surface will be deallocated with the other
             * surfaces in this list when the list is eventually destroyed.
             *
             * @param surface the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NormalSurface* surface);
            /**
             * Appends the normal surface corresponding to the given
             * vector to the end of the appropriate surface list.
             *
             * The given vector will be owned by the newly created
             * normal surface and will be deallocated with the other
             * surfaces in this list when the list is eventually destroyed.
             *
             * \warning The behaviour of this routine has changed!
             * As of Regina 4.6, this routine no longer checks for
             * multiple octagonal discs.  See the SurfaceInserter
             * class notes for details.
             *
             * @param vector the vector of the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NormalSurfaceVector* vector);

            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator *();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++(int);
        };

    private:
        /**
         * A helper class containing constants, typedefs and operations
         * for working with normal (as opposed to almost normal) surfaces.
         *
         * This class and its partner AlmostNormalSpec can be used to
         * write generic template code that works with both normal
         * \e and almost normal surfaces.
         *
         * The full definition of this class is in the file normalspec-impl.h,
         * which is included automatically by this header.
         */
        struct NormalSpec;

        /**
         * A helper class containing constants, typedefs and operations
         * for working with almost normal (as opposed to normal) surfaces.
         *
         * This class and its partner NormalSpec can be used to
         * write generic template code that works with both normal
         * \e and almost normal surfaces.
         *
         * The full definition of this class is in the file normalspec-impl.h,
         * which is included automatically by this header.
         */
        struct AlmostNormalSpec;

        /**
         * Converts a set of embedded vertex normal surfaces in
         * (quad or quad-oct) space to a set of embedded vertex normal
         * surfaces in (standard normal or standard almost normal) space.
         * The original (quad or quad-oct) space surfaces are passed in
         * the argument \a quadList, and the resulting (standard normal
         * or standard almost normal) space surfaces will be inserted
         * directly into this list.
         *
         * See quadToStandard() and quadOctToStandardAN() for full details
         * and preconditions for this procedure.
         *
         * This routine is designed to work with surface lists that are
         * still under construction.  As such, it ignores the packet
         * tree completely.  The parent packet is ignored (and not changed);
         * instead the underlying triangulation is passed explicitly as
         * the argument \a owner.
         *
         * Although this routine takes a vector of non-const pointers, it
         * guarantees not to modify (or destroy) any of the contents.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns.
         *
         * Although this is a template function, it is a private
         * template function and so is only defined in the one <tt>.cpp</tt>
         * file that needs it.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are doing the
         * work for quadToStandard() or quadOctToStandardAN() respectively.
         * \pre The coordinate system for this surface list is set to
         * NS_STANDARD or NS_AN_STANDARD, according to whether we are doing
         * the work for quadToStandard() or quadOctToStandardAN() respectively,
         * and the embedded-only flag is set to \c true.
         * \pre The given triangulation is valid and non-empty, and the link
         * of every vertex is either a sphere or a disc.
         *
         * @param owner the triangulation upon which this list of
         * surfaces is to be based.
         * @param reducedList a full list of vertex surfaces in
         * (quad or quad-oct) coordinates for the given triangulation.
         * @param tracker a progress tracker to be used for progress reporting
         * and cancellation requests, or 0 if this is not required.
         */
        template <class Variant>
        void buildStandardFromReduced(Triangulation<3>* owner,
            const std::vector<NormalSurface*>& reducedList,
            ProgressTracker* tracker = 0);

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
         * the number of tetrahedra in the given triangulation.
         */
        template <class Variant, class BitmaskType>
        void buildStandardFromReducedUsing(Triangulation<3>* owner,
            const std::vector<NormalSurface*>& reducedList,
            ProgressTracker* tracker);

        /**
         * Converts a set of embedded vertex surfaces in (quad or quad-oct)
         * space to a set of embedded vertex surfaces in (standard normal or
         * standard almost normal) space.
         *
         * This is a generic implementation that performs the real work
         * for both quadToStandard() and quadOctToStandardAN().  See each
         * of those routines for further details as well as relevant
         * preconditions and postconditions.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are implementing
         * quadToStandard() or quadOctToStandardAN() accordingly.
         */
        template <class Variant>
        NormalSurfaces* internalReducedToStandard() const;

        /**
         * Converts a set of embedded vertex surfaces in
         * (standard normal or standard almost normal) space to a set of
         * embedded vertex surfaces in (quad or quad-oct) space.
         *
         * This is a generic implementation that performs the real work
         * for both standardToQuad() and standardANToQuadOct().  See each
         * of those routines for further details as well as relevant
         * preconditions and postconditions.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are implementing
         * standardToQuad() or standardANToQuadOct() accordingly.
         */
        template <class Variant>
        NormalSurfaces* internalStandardToReduced() const;

        /**
         * A functor that performs all normal surface enumeration.
         */
        class Enumerator {
            private:
                NormalSurfaces* list_;
                    /**< The surface list to be filled. */
                Triangulation<3>* triang_;
                    /**< The triangulation in which these surfaces lie. */
                ProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or 0 if no progress tracker is in use. */

            public:
                /**
                 * Creates a new functor with the given parameters.
                 *
                 * @param list the surface list to be filled.
                 * @param triang the triangulation in which these surfaces lie.
                 * @param tracker the progress tracker to use for progress
                 * reporting and cancellation polling, or 0 if these
                 * capabilities are not required.
                 */
                Enumerator(NormalSurfaces* list,
                    Triangulation<3>* triang, ProgressTracker* tracker);

                /**
                 * Performs the real enumeration work, in a setting
                 * where the underlying coordinate system is
                 * a compile-time constant.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ into the packet
                 * tree as a child of \a triang_.
                 *
                 * \tparam Coords an instance of the NormalInfo<> template
                 * class.
                 */
                template <typename Coords>
                void operator() ();

            private:
                /**
                 * The enumeration code for enumerating vertex surfaces.
                 * This is internal to operator().
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
                template <typename Coords>
                void fillVertex();

                /**
                 * The enumeration code for enumerating fundamental surfaces.
                 * This is internal to operator().
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
                template <typename Coords>
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
                template <typename Coords>
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
                template <typename Coords>
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
                template <typename Coords, typename Integer>
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
                template <typename Coords>
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
                template <typename Coords>
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
                template <typename Coords>
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
                template <typename Coords>
                void fillFundamentalFullCone();
        };

    friend class XMLNormalSurfacesReader;
};

/**
 * Returns a new normal surface vector of the appropriate length for the
 * given triangulation and the given coordinate system.
 * All elements of this vector will be initialised to zero.
 *
 * The new vector will be of the subclass of NormalSurfaceVector
 * corresponding to the given coordinate system.  The caller
 * of this routine is responsible for destroying the new vector.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param coords the coordinate system to be used.
 * @return a new zero vector of the correct class and length.
 */
REGINA_API NormalSurfaceVector* makeZeroVector(
    const Triangulation<3>* triangulation, NormalCoords coords);
/**
 * Creates a new set of normal surface matching equations for the
 * given triangulation using the given coordinate system.
 * The returned matrix will be newly allocated and its destruction will
 * be the responsibility of the caller of this routine.
 *
 * Each equation will be represented as a row of the matrix.
 * Each column of the matrix represents a coordinate in the given
 * coordinate system.
 *
 * @param triangulation the triangulation upon which these matching equations
 * will be based.
 * @param coords the coordinate system to be used.
 * @return a newly allocated set of matching equations.
 */
REGINA_API MatrixInt* makeMatchingEquations(
    const Triangulation<3>* triangulation, NormalCoords coords);
/**
 * Creates a new set of validity constraints representing the condition that
 * normal surfaces be embedded.  The validity constraints will be expressed
 * relative to the given coordinate system.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which these validity constraints
 * will be based.
 * @param coords the coordinate system to be used.
 * @return a newly allocated set of constraints.
 */
REGINA_API EnumConstraints* makeEmbeddedConstraints(
    const Triangulation<3>* triangulation, NormalCoords coords);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalSurfaceList has now been renamed to
 * NormalSurfaces.
 */
REGINA_DEPRECATED typedef NormalSurfaces NNormalSurfaceList;

/*@}*/

// Inline functions for NormalSurfaces

inline NormalSurfaces::~NormalSurfaces() {
    for_each(surfaces.begin(), surfaces.end(), FuncDelete<NormalSurface>());
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

inline size_t NormalSurfaces::size() const {
    return surfaces.size();
}

inline const NormalSurface* NormalSurfaces::surface(size_t index) const {
    return surfaces[index];
}

inline bool NormalSurfaces::dependsOnParent() const {
    return true;
}

inline NormalSurfaces::VectorIterator::VectorIterator() {
}

inline NormalSurfaces::VectorIterator::VectorIterator(
        const NormalSurfaces::VectorIterator& cloneMe) :
        it_(cloneMe.it_) {
}

inline NormalSurfaces::VectorIterator& NormalSurfaces::VectorIterator::
        operator =(const NormalSurfaces::VectorIterator& cloneMe) {
    it_ = cloneMe.it_;
    return *this;
}

inline bool NormalSurfaces::VectorIterator::operator ==(
        const NormalSurfaces::VectorIterator& other) const {
    return (it_ == other.it_);
}

inline bool NormalSurfaces::VectorIterator::operator !=(
        const NormalSurfaces::VectorIterator& other) const {
    return (it_ != other.it_);
}

inline const Ray& NormalSurfaces::VectorIterator::
        operator *() const {
    return (*it_)->rawVector();
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
        const std::vector<NormalSurface*>::const_iterator& i) : it_(i) {
}

inline NormalSurfaces::VectorIterator NormalSurfaces::beginVectors()
        const {
    return VectorIterator(surfaces.begin());
}

inline NormalSurfaces::VectorIterator NormalSurfaces::endVectors()
        const {
    return VectorIterator(surfaces.end());
}

inline NormalSurfaces::SurfaceInserter::SurfaceInserter(
        NormalSurfaces& newList, Triangulation<3>* newOwner) :
        list(&newList), owner(newOwner) {
}

inline NormalSurfaces::SurfaceInserter::SurfaceInserter(
        const SurfaceInserter& cloneMe) : list(cloneMe.list),
        owner(cloneMe.owner) {
}


inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator =(
        const SurfaceInserter& cloneMe) {
    list = cloneMe.list;
    owner = cloneMe.owner;
    return *this;
}

inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator =(
        NormalSurface* surface) {
    list->surfaces.push_back(surface);
    return *this;
}

inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator =(
        NormalSurfaceVector* vector) {
    list->surfaces.push_back(new NormalSurface(owner, vector));
    return *this;
}

inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator *() {
    return *this;
}

inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator ++() {
    return *this;
}

inline NormalSurfaces::SurfaceInserter&
        NormalSurfaces::SurfaceInserter::operator ++(int) {
    return *this;
}

inline NormalSurfaces::NormalSurfaces(NormalCoords coords,
        NormalList which, NormalAlg algorithm) :
        coords_(coords), which_(which), algorithm_(algorithm) {
}

inline NormalSurfaces::Enumerator::Enumerator(NormalSurfaces* list,
        Triangulation<3>* triang, ProgressTracker* tracker) :
        list_(list), triang_(triang), tracker_(tracker) {
}

} // namespace regina

// Import the full definitions of NormalSpec and AlmostNormalSpec.
#include "surfaces/normalspec-impl.h"

#endif
