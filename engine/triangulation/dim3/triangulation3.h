
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

/*! \file triangulation/dim3/triangulation3.h
 *  \brief Internal header for 3-dimensional triangulations.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

#ifndef __REGINA_TRIANGULATION3_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION3_H
#endif

#include <map>
#include <variant>
#include <vector>
#include <set>

#include "regina-core.h"
#include "angle/anglestructure.h"
#include "maths/cyclotomic.h"
#include "progress/progresstracker.h"
#include "surface/normalsurface.h"
#include "treewidth/treedecomposition.h"
#include "triangulation/detail/retriangulate.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/boolset.h"
#include "utilities/exception.h"
#include "utilities/markedvector.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

class AngleStructure;
class GroupPresentation;
class Link;
class NormalSurface;
class SnapPeaTriangulation;

template <int> class XMLTriangulationReader;

#ifdef __DOCSTRINGS
// Declare SnapPy types that appear in the Python-only functions below,
// so that the docstring generator does not complain.
namespace snappy {
    class Manifold;
    class Triangulation;
}
#endif

/**
 * \defgroup dim3 3-Manifold Triangulations
 * Details for implementing triangulations of 3-manifolds.
 */

/**
 * Represents a 3-dimensional triangulation, typically of a 3-manifold.
 *
 * This is a specialisation of the generic Triangulation class template;
 * see the generic Triangulation documentation for a general overview of how
 * the triangulation classes work.  In Python, you can read this generic
 * documentation by looking at a higher dimension: try `help(Triangulation5)`.
 *
 * This 3-dimensional specialisation offers significant extra functionality,
 * including many functions specific to 3-manifolds.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Triangulation<3> : public detail::TriangulationBase<3> {
    public:
        using TuraevViroSet =
                std::map<std::pair<unsigned long, bool>, Cyclotomic>;
            /**< A map from (\a r, \a parity) pairs to Turaev-Viro invariants,
                 as described by turaevViro(). */

    private:
        bool ideal_;
            /**< Is the triangulation ideal? */
        bool standard_;
            /**< Is the triangulation standard? */

        /**
         * A struct that holds all of our calculated properties.
         * This is a convenience so we can use its implicitly defined
         * assignment operators and copy constructors.  It is mutable so that
         * expensive read-only calculations can cache their results.
         *
         * All std::optional properties are std::nullopt if they have
         * not yet been computed.
         */
        mutable struct {
            std::optional<AbelianGroup> H1Rel_;
                /**< Relative first homology group of the triangulation
                     with respect to the boundary. */
            std::optional<AbelianGroup> H1Bdry_;
                /**< First homology group of the boundary. */
            std::optional<AbelianGroup> H2_;
                /**< Second homology group of the triangulation. */

            std::optional<bool> twoSphereBoundaryComponents_;
                /**< Does the triangulation contain any 2-sphere boundary
                     components? */
            std::optional<bool> negativeIdealBoundaryComponents_;
                /**< Does the triangulation contain any boundary components
                     that are ideal and have negative Euler characteristic? */

            std::optional<bool> zeroEfficient_;
                /**< Is the triangulation zero-efficient? */
            std::optional<bool> oneEfficient_;
                /**< Is the triangulation one-efficient? */
            std::optional<bool> splittingSurface_;
                /**< Does the triangulation have a normal splitting surface? */

            std::optional<bool> threeSphere_;
                /**< Is this a triangulation of a 3-sphere? */
            std::optional<ssize_t> handlebody_;
                /**< Is this a triangulation of an orientable handlebody?
                     If so, this stores the genus; if not, this stores -1. */
            std::optional<bool> TxI_;
                /**< Is this a triangulation of the product TxI? */
            std::optional<bool> irreducible_;
                /**< Is this 3-manifold irreducible? */
            std::optional<bool> compressingDisc_;
                /**< Does this 3-manifold contain a compressing disc? */
            std::optional<bool> haken_;
                /**< Is this 3-manifold Haken?
                     This property must only be stored for triangulations
                     that are known to represent closed, connected,
                     orientable, irreducible 3-manifolds. */

            std::optional<TreeDecomposition> niceTreeDecomposition_;
                /**< A nice tree decomposition of the face pairing graph of
                     this triangulation. */

            TuraevViroSet turaevViroCache_;
                /**< The set of Turaev-Viro invariants that have already
                     been calculated.  See allCalculatedTuraevViro() for
                     details. */
        } prop_;

        // Regarding cached normal surfaces and angle structures:
        // When move constructing/assigning triangulations, we do *not* need to
        // adjust the triangulation references in these surfaces/structures.
        // This is because surfaces/structures reference their triangulations
        // via a SnapshotRef, and the triangulation's inherited Snapshottable
        // move construction/assignment preserves the underlying Snapshot.

        mutable std::variant<bool, AngleStructure>
            strictAngleStructure_ { false };
                /**< A strict angle structure on this triangulation, or a
                     bool if we do not have one.  The bool will be \c false
                     if the computation has not yet been attempted, or \c true
                     if it is confirmed that no such angle structure exists. */

        mutable std::variant<bool, AngleStructure>
            generalAngleStructure_ { false };
                /**< A generalised angle structure on this triangulation, or a
                     bool if we do not have one.  The bool will be \c false
                     if the computation has not yet been attempted, or \c true
                     if it is confirmed that no such angle structure exists. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Triangulation() = default;
        /**
         * Creates a new copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, the same numbering and
         * labelling will be used for all skeletal objects.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be copied across.
         *
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * \param src the triangulation to copy.
         */
        Triangulation(const Triangulation& src);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties and/or locks also.
         *
         * If \a cloneProps is \c true, then this constructor will also clone
         * any computed properties (such as homology, fundamental group, and
         * so on).  If \a cloneProps is \c false, then these properties
         * will be marked as unknown in the new triangulation, and will be
         * recomputed on demand if/when they are required.
         *
         * Regardless of \a cloneProps, the skeleton (vertices, edges,
         * components, etc.) will _always_ be cloned.  This is to ensure that
         * the same numbering and labelling will be used for all skeletal
         * objects in both triangulations.
         *
         * If \a cloneLocks is \c true then any locks on the top-dimensional
         * simplices and/or facets of \a src will be copied across.
         * If \a cloneLocks is \c false then the new triangulation will have
         * no locks at all.
         *
         * \param src the triangulation to copy.
         * \param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation, or \c false if the new
         * triangulation should have such properties marked as unknown.
         * \param cloneLocks \c true if this should also clone any simplex
         * and/or facet locks from the given triangulation, or \c false if
         * the new triangulation should have no locks at all.
         */
        Triangulation(const Triangulation& src, bool cloneProps,
            bool cloneLocks = true);
        /**
         * Moves the given triangulation into this new triangulation.
         *
         * This is much faster than the copy constructor, but is still linear
         * time.  This is because every tetrahedron must be adjusted to point
         * back to this new triangulation instead of \a src.
         *
         * All tetrahedra and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Tetrahedron<3>, Face<3, subdim>, Component<3> or
         * BoundaryComponent<3> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be moved across.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the triangulation to move.
         */
        Triangulation(Triangulation&& src) noexcept = default;
        /**
         * Creates a new ideal triangulation representing the complement
         * of the given link in the 3-sphere.
         *
         * The triangulation will have one ideal vertex for each link
         * component.  Assuming you pass \a simplify as \c true (the default),
         * there will typically be no internal vertices; however, this
         * is not guaranteed.
         *
         * Initially, each tetrahedron will be oriented according to a
         * right-hand rule: the thumb of the right hand points from vertices
         * 0 to 1, and the fingers curl around to point from vertices 2 to 3.
         * If you pass \a simplify as \c true, then Regina will attempt to
         * simplify the triangulation to as few tetrahedra as possible:
         * this may relabel the tetrahedra, though their orientations will
         * be preserved.
         *
         * This is the same triangulation that is produced by
         * Link::complement().
         *
         * \param link the link whose complement we should build.
         * \param simplify \c true if and only if the triangulation
         * should be simplified to use as few tetrahedra as possible.
         */
        Triangulation(const Link& link, bool simplify = true);
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a triangulation.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - isomorphism signatures (see fromIsoSig());
         * - dehydration strings (see rehydrate());
         * - the filename or contents of a SnapPea data file (see
         *   fromSnapPea()).
         *
         * This list may grow in future versions of Regina.
         *
         * \warning If you pass the filename or contents of a SnapPea data
         * file, then only the tetrahedron gluings will be read; all other
         * SnapPea-specific information (such as peripheral curves) will
         * be lost.  See fromSnapPea() for details, and for other
         * alternatives that do preserve SnapPea-specific data.
         *
         * \exception InvalidArgument Regina could not interpret the given
         * string as representing a triangulation using any of the supported
         * string types.
         *
         * \param description a string that describes a 3-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
#ifdef __APIDOCS
        /**
         * Python-only constructor that copies the given SnapPy manifold.
         *
         * Although the argument \a m would typically be a `SnapPy.Manifold`,
         * it could in fact be anything with a `_to_string()` method (so
         * you could instead pass a `SnapPy.Triangulation`, for example).
         * Regina will then call `m._to_string()` and pass the result to
         * the "magic" string constructor for Regina's Triangulation3 class.
         * Typically, if \a m is a SnapPy object, this means that
         * `m._to_string()` would need to return the contents of a
         * SnapPy/SnapPea data file.
         *
         * \warning Only the tetrahedron gluings will be copied; all other
         * SnapPy-specific information (such as peripheral curves) will
         * be lost.  See fromSnapPea() for details, and for other
         * alternatives that do preserve SnapPy-specific data.
         *
         * \nocpp
         *
         * \param m a SnapPy object of type snappy.Manifold.
         */
        Triangulation(snappy::Manifold m);
#endif
        /**
         * Destroys this triangulation.
         *
         * The constituent tetrahedra, the cellular structure and all other
         * properties will also be destroyed.
         */
        ~Triangulation();

        /**
         * Returns the packet that holds this data, even if it is held
         * indirectly via a SnapPea triangulation.
         *
         * This routine is similar to PacketOf<Triangulation<3>>::packet().
         * In particular, if this triangulation is held directly by a
         * 3-dimensional triangulation packet \a p, then this routine
         * will return \a p.
         *
         * The difference is when this triangulation is held "indirectly" by a
         * SnapPea triangulation packet \a q (i.e., this is the inherited
         * Triangulation<3> data belonging to the SnapPea triangulation).
         * In such a scenario, Triangulation<3>::packet() will return \c null
         * (since there is no "direct" 3-dimensional triangulation packet),
         * but inAnyPacket() will return \a q (since the triangulation is
         * still "indirectly" held by a different type of packet).
         *
         * \return the packet that holds this data (directly or indirectly),
         * or \c null if this data is not held by either a 3-dimensional
         * triangulation packet or a SnapPea triangulation packet.
         */
        std::shared_ptr<Packet> inAnyPacket();
        /**
         * Returns the packet that holds this data, even if it is held
         * indirectly via a SnapPea triangulation.
         *
         * This routine is similar to PacketOf<Triangulation<3>>::packet().
         * In particular, if this triangulation is held directly by a
         * 3-dimensional triangulation packet \a p, then this routine
         * will return \a p.
         *
         * The difference is when this triangulation is held "indirectly" by a
         * SnapPea triangulation packet \a q (i.e., this is the inherited
         * Triangulation<3> data belonging to the SnapPea triangulation).
         * In such a scenario, Triangulation<3>::packet() will return \c null
         * (since there is no "direct" 3-dimensional triangulation packet),
         * but inAnyPacket() will return \a q (since the triangulation is
         * still "indirectly" held by a different type of packet).
         *
         * \return the packet that holds this data (directly or indirectly),
         * or \c null if this data is not held by either a 3-dimensional
         * triangulation packet or a SnapPea triangulation packet.
         */
        std::shared_ptr<const Packet> inAnyPacket() const;

        /**
         * Returns the SnapPea triangulation that holds this data, if
         * there is one.
         *
         * This routine essentially replaces a dynamic_cast, since the
         * class Triangulation<3> is not polymorphic.
         *
         * If this object in fact belongs to a SnapPeaTriangulation \a t
         * (through its inherited Triangulation<3> interface), then this
         * routine will return \a t.  Otherwise it will return \c null.
         *
         * \return the SnapPea triangulation that holds this data, or
         * \c null if this data is not part of a SnapPea triangulation.
         */
        SnapPeaTriangulation* isSnapPea();
        /**
         * Returns the SnapPea triangulation that holds this data, if
         * there is one.
         *
         * This routine essentially replaces a dynamic_cast, since the
         * class Triangulation<3> is not polymorphic.
         *
         * If this object in fact belongs to a SnapPeaTriangulation \a t
         * (through its inherited Triangulation<3> interface), then this
         * routine will return \a t.  Otherwise it will return \c null.
         *
         * \return the SnapPea triangulation that holds this data, or
         * \c null if this data is not part of a SnapPea triangulation.
         */
        const SnapPeaTriangulation* isSnapPea() const;

        /*@}*/
        /**
         * \name Tetrahedra
         */
        /*@{*/

        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Tetrahedron<3>* newTetrahedron();
        /**
         * A dimension-specific alias for newSimplex().
         *
         * See newSimplex() for further information.
         */
        Tetrahedron<3>* newTetrahedron(const std::string& desc);
        /**
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        template <int k>
        std::array<Tetrahedron<3>*, k> newTetrahedra();
        /**
         * A dimension-specific alias for newSimplices().
         *
         * See newSimplices() for further information.
         */
        void newTetrahedra(size_t k);
        /**
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         *
         * \exception LockViolation The given tetrahedron and/or one of its
         * facets is currently locked.  This exception will be thrown
         * before any changes are made.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on how such locks
         * work and what their implications are.
         *
         * \param tet the tetrahedron to remove.
         */
        void removeTetrahedron(Tetrahedron<3>* tet);
        /**
         * A dimension-specific alias for removeSimplexAt().
         *
         * See removeSimplexAt() for further information.
         *
         * \exception LockViolation The requested tetrahedron and/or one of its
         * facets is currently locked.  This exception will be thrown
         * before any changes are made.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on how such locks
         * work and what their implications are.
         *
         * \param index specifies which tetrahedron to remove; this
         * must be between 0 and size()-1 inclusive.
         */
        void removeTetrahedronAt(size_t index);
        /**
         * A dimension-specific alias for removeAllSimplices().
         *
         * See removeAllSimplices() for further information.
         *
         * \exception LockViolation This triangulation contains at least one
         * locked tetrahedron and/or facet.  This exception will be
         * thrown before any changes are made.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on how such locks
         * work and what their implications are.
         */
        void removeAllTetrahedra();

        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * This will also clone any computed properties (such as homology,
         * fundamental group, and so on), as well as the skeleton (vertices,
         * edges, components, etc.).  In particular, this triangulation
         * will use the same numbering and labelling for all skeletal objects
         * as in the source triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be copied across.
         *
         * \param src the triangulation to copy.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (const Triangulation& src);

        /**
         * Moves the contents of the given triangulation into this
         * triangulation.
         *
         * This is much faster than copy assignment, but is still linear
         * time.  This is because every tetrahedron must be adjusted to point
         * back to this triangulation instead of \a src.
         *
         * All tetrahedra and skeletal objects (faces, components and
         * boundary components) that belong to \a src will be moved into
         * this triangulation, and so any pointers or references to
         * Tetrahedron<3>, Face<3, subdim>, Component<3> or
         * BoundaryComponent<3> objects will remain valid.  Likewise, all
         * cached properties will be moved into this triangulation.
         *
         * If \a src has any locks on top-dimensional simplices and/or their
         * facets, these locks will also be moved across.
         *
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this triangulation which may in turn call arbitrary
         * code via any registered packet listeners.  It deliberately does
         * _not_ fire change events on \a src, since it assumes that \a src is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * \param src the triangulation to move.
         * \return a reference to this triangulation.
         */
        Triangulation& operator = (Triangulation&& src);

        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All tetrahedra that belong to this triangulation
         * will be moved to \a other, and all tetrahedra
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * (such as homology and fundamental group) will be swapped.
         *
         * In particular, any pointers or references to Tetrahedron<3> and/or
         * Face<3, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both triangulations which may in turn call
         * arbitrary code via any registered packet listeners.
         *
         * \param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<3>& other);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTriangles() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTriangles() const;

        /**
         * Determines if this triangulation contains any two-sphere
         * boundary components.
         *
         * \return \c true if and only if there is at least one
         * two-sphere boundary component.
         */
        bool hasTwoSphereBoundaryComponents() const;
        /**
         * Determines if this triangulation contains any ideal boundary
         * components with negative Euler characteristic.
         *
         * \return \c true if and only if there is at least one such
         * boundary component.
         */
        bool hasNegativeIdealBoundaryComponents() const;

        /**
         * Determines whether the boundary of this triangulation contains
         * the smallest possible number of triangles.
         *
         * This is true if and only if, amongst all real boundary components,
         * every sphere or projective plane boundary component has precisely
         * two triangles, and every other boundary component has precisely
         * one vertex.
         *
         * For the purposes of this routine, ideal boundary components
         * are ignored.
         *
         * If this routine returns \c false, you can call minimiseBoundary()
         * to make the number of boundary triangles minimal.
         *
         * \pre This triangulation is valid.
         *
         * \return \c true if and only if the boundary contains the
         * smallest possible number of triangles.
         */
        bool hasMinimalBoundary() const;

        /**
         * Determines whether this triangulation contains the smallest possible
         * number of vertices for the 3-manifold that it represents.
         *
         * This is true if and only if:
         *
         * - amongst all real boundary components, every sphere or projective
         *   plane boundary component has precisely two triangles, and every
         *   other boundary component has precisely one vertex;
         *
         * - amongst all connected components, every closed component
         *   has precisely one vertex, and every component with real or
         *   ideal boundary has no internal vertices at all.
         *
         * If this routine returns \c false, you can call minimiseVertices()
         * to make the number of vertices minimal.
         *
         * \pre This triangulation is valid.
         *
         * \return \c true if and only if this triangulation contains
         * the smallest possible number of vertices.
         */
        bool hasMinimalVertices() const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Returns the Euler characteristic of the corresponding compact
         * 3-manifold.
         *
         * Instead of simply calculating `V-E+F-T`, this routine also:
         *
         * - treats ideal vertices as surface boundary components
         *   (i.e., effectively truncates them);
         * - truncates invalid boundary vertices (i.e., boundary vertices
         *   whose links are not discs);
         * - truncates the projective plane cusps at the midpoints of invalid
         *   edges (edges identified with themselves in reverse).
         *
         * For ideal triangulations, this routine therefore computes
         * the proper Euler characteristic of the manifold (unlike
         * eulerCharTri(), which does not).
         *
         * For triangulations whose vertex links are all spheres or discs,
         * this routine and eulerCharTri() give identical results.
         *
         * \return the Euler characteristic of the corresponding compact
         * manifold.
         */
        long eulerCharManifold() const;

        /**
         * Determines if this triangulation is ideal.
         * This is the case if and only if one of the vertex links
         * is closed and not a 2-sphere.
         * Note that the triangulation is not required to be valid.
         *
         * \return \c true if and only if this triangulation is ideal.
         */
        bool isIdeal() const;
        /**
         * Determines if this triangulation is standard.
         * This is the case if and only if every vertex is standard.
         * See Vertex<3>::isStandard() for further details.
         *
         * \return \c true if and only if this triangulation is
         * standard.
         */
        bool isStandard() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal triangulations are not closed.
         *
         * \return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;

        /**
         * Determines if this triangulation is ordered; that is, if
         * tetrahedron vertices are labelled so that all gluing
         * permutations are order-preserving on the tetrahedron faces.
         * Equivalently, this tests whether the edges of the triangulation
         * can all be oriented such that they induce a consistent ordering
         * on the vertices of each tetrahedron.
         *
         * Triangulations are not ordered by default, and indeed some
         * cannot be ordered at all.  The routine order() will attempt
         * to relabel tetrahedron vertices to give an ordered triangulation.
         *
         * \return \c true if and only if all gluing permutations are
         * order preserving on the tetrahedron faces.
         *
         * \author Matthias Goerner
         */
        bool isOrdered() const;

        /*@}*/
        /**
         * \name Algebraic Properties
         */
        /*@{*/

        /**
         * Returns the relative first homology group with
         * respect to the boundary for this triangulation.
         * Note that ideal vertices are considered part of the boundary.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homologyRel() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * \exception FailedPrecondition This triangulation is invalid.
         *
         * \return the relative first homology group with respect to the
         * boundary.
         */
        const AbelianGroup& homologyRel() const;
        /**
         * Returns the first homology group of the
         * boundary for this triangulation.
         * Note that ideal vertices are considered part of the boundary.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homologyBdry() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * This routine is fairly fast, since it deduces the homology of
         * each boundary component through knowing what kind of surface
         * it is.
         *
         * \pre This triangulation is valid.
         *
         * \exception FailedPrecondition This triangulation is invalid.
         *
         * \return the first homology group of the boundary.
         */
        const AbelianGroup& homologyBdry() const;
        /**
         * Returns the second homology group with coefficients in Z_2
         * for this triangulation.
         * If this triangulation contains any ideal vertices,
         * the homology group will be
         * calculated as if each such vertex had been truncated.
         * The algorithm used calculates the relative first homology group
         * with respect to the boundary and uses homology and cohomology
         * theorems to deduce the second homology group.
         *
         * This group will simply be the direct sum of several copies of
         * Z_2, so the number of Z_2 terms is returned.
         *
         * \pre This triangulation is valid.
         *
         * \exception FailedPrecondition This triangulation is invalid.
         *
         * \return the number of Z_2 terms in the second homology group
         * with coefficients in Z_2.
         */
        unsigned long homologyH2Z2() const;
        /**
         * Computes the given Turaev-Viro state sum invariant of this
         * 3-manifold using exact arithmetic.
         *
         * The initial data for the Turaev-Viro invariant is as described in
         * the paper of Turaev and Viro, "State sum invariants of 3-manifolds
         * and quantum 6j-symbols", Topology, vol. 31, no. 4, 1992, pp 865-902.
         * In particular, Section 7 of this paper describes the initial data
         * as determined by an integer r ≥ 3, and a root of unity q₀ of
         * degree 2r for which q₀² is a primitive root of unity of
         * degree r.  There are several cases to consider:
         *
         * - \a r may be even.  In this case \a q₀ must be a primitive
         *   (<i>2r</i>)th root of unity, and the invariant is computed as an
         *   element of the cyclotomic field of order \a 2r.  There is no need
         *   to specify _which_ root of unity is used, since switching between
         *   different roots of unity corresponds to an automorphism of
         *   the underlying cyclotomic field (i.e., it does not yield
         *   any new information).  Therefore, if \a r is even, the
         *   additional argument \a parity is ignored.
         *
         * - \a r may be odd, and \a q₀ may be a primitive (2\a r)th
         *   root of unity.  This case corresponds to passing the argument
         *   \a parity as \c true.  Here the invariant is again computed
         *   as an element of the cyclotomic field of order \a 2r.  As before,
         *   there is no need to give further information as to which
         *   root of unity is used, since switching between roots of unity
         *   does not yield new information.
         *
         * - \a r may be odd, and \a q₀ may be a primitive (\a r)th
         *   root of unity.  This case corresponds to passing the argument
         *   \a parity as \c false.  In this case the invariant is computed
         *   as an element of the cyclotomic field of order \a r.  Again,
         *   there is no need to give further information as to which
         *   root of unity is used.
         *
         * This routine works entirely within the relevant cyclotomic field,
         * which yields exact results but adds a significant overhead to the
         * running time.  If you want a fast floating-point approximation,
         * you can call turaevViroApprox() instead.
         *
         * Unlike this routine, turaevViroApprox() requires a precise
         * specification of which root of unity is used (since it
         * returns a numerical real value).  The numerical value
         * obtained by calling
         * `turaevViroApprox(r, whichRoot)`
         * should be the same as
         * `turaevViro(r, parity).evaluate(whichRoot)`,
         * where \a parity is \c true or \c false according to whether
         * \a whichRoot is odd or even respectively.  Of course in practice the
         * numerical values might be very different, since turaevViroApprox()
         * performs significantly more floating-point operations, and so is
         * subject to a much larger potential numerical error.
         *
         * If the requested Turaev-Viro invariant has already been computed,
         * then the result will be cached and so this routine will be
         * very fast (since it just returns the previously computed result).
         * Otherwise the computation could be quite slow, particularly
         * for larger triangulations and/or larger values of \a r.
         *
         * Since Regina 7.0, this routine will not return until the
         * Turaev-Viro computation is complete, regardless of whether a progress
         * tracker was passed.  If you need the old behaviour (where passing a
         * progress tracker caused the computation to start in the background),
         * simply call this routine in a new detached thread.
         *
         * \pre This triangulation is valid, closed and non-empty.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \exception FailedPrecondition This triangulation is not valid,
         * closed and non-empty.
         *
         * \exception InvalidArgument The argument \a r is less than 3.
         *
         * \param r the integer \a r as described above; this must be at
         * least 3.
         * \param parity determines for odd \a r whether \a q₀ is a primitive
         * <i>2r</i>th or <i>r</i>th root of unity, as described above.
         * \param alg the algorithm with which to compute the invariant.  If
         * you are not sure, the default value (Algorithm::Default) is a safe
         * choice.  This should be treated as a hint only: if the algorithm
         * you choose is not supported for the given parameters (\a r and
         * \a parity), then Regina will use another algorithm instead.
         * \param tracker a progress tracker through will progress will
         * be reported, or \c nullptr if no progress reporting is required.
         * \return the requested Turaev-Viro invariant, or an uninitialised
         * field element if the calculation was cancelled via the given
         * progress tracker.
         *
         * \see allCalculatedTuraevViro
         */
        Cyclotomic turaevViro(unsigned long r, bool parity = true,
            Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Computes the given Turaev-Viro state sum invariant of this
         * 3-manifold using a fast but inexact floating-point approximation.
         *
         * The initial data for the Turaev-Viro invariant is as described in
         * the paper of Turaev and Viro, "State sum invariants of 3-manifolds
         * and quantum 6j-symbols", Topology, vol. 31, no. 4, 1992, pp 865-902.
         * In particular, Section 7 describes the initial data as
         * determined by an integer \a r ≥ 3 and a root of unity \a q₀ of
         * degree \a 2r for which \a q₀² is a primitive root of unity of
         * degree \a r.
         *
         * The argument \a whichRoot specifies which root of unity is
         * used for \a q₀.  Specifically, \a q₀ will be the root of unity
         * `e^(2πi * whichRoot / 2r)`.  There are additional
         * preconditions on \a whichRoot to ensure that \a q₀² is a
         * _primitive_ root of unity of degree \a r; see below for details.
         *
         * This same invariant can be computed by calling
         * `turaevViro(r, parity).evaluate(whichRoot)`,
         * where \a parity is \c true or \c false according to whether
         * \a whichRoot is odd or even respectively.
         * Calling turaevViroApprox() is significantly faster (since it avoids
         * the overhead of working in cyclotomic fields), but may also
         * lead to a much larger numerical error (since this routine might
         * perform an exponential number of floating-point operations,
         * whereas the alternative only uses floating-point for
         * the final call to Cyclotomic::evaluate()).
         *
         * These invariants, although computed in the complex field,
         * should all be reals.  Thus the return type is an ordinary
         * double.
         *
         * \pre This triangulation is valid, closed and non-empty.
         * \pre The argument \a whichRoot is strictly between 0 and `2r`,
         * and has no common factors with \a r.
         *
         * \exception FailedPrecondition This triangulation is not valid,
         * closed and non-empty.
         *
         * \exception InvalidArgument the argument \a r is less than 3, or
         * the argument \a whichRoot is not both coprime to \a r and between
         * 0 and `2r`.
         *
         * \param r the integer \a r as described above; this must be at
         * least 3.
         * \param whichRoot specifies which root of unity is used for \a q₀,
         * as described above.
         * \param alg the algorithm with which to compute the invariant.  If
         * you are not sure, the default value (Algorithm::Default) is a safe
         * choice.  This should be treated as a hint only: if the algorithm
         * you choose is not supported for the given parameters (\a r and
         * \a whichRoot), then Regina will use another algorithm instead.
         * \return the requested Turaev-Viro invariant.
         *
         * \see allCalculatedTuraevViro
         */
        double turaevViroApprox(unsigned long r, unsigned long whichRoot = 1,
            Algorithm alg = Algorithm::Default) const;
        /**
         * Returns the cache of all Turaev-Viro state sum invariants that
         * have been calculated for this 3-manifold.
         * This cache is updated every time turaevViro() is called, and
         * is emptied whenever the triangulation is modified.
         *
         * Turaev-Viro invariants are identified by an (\a r, \a parity)
         * pair as described in the turaevViro() documentation.  The
         * cache is just a set that maps (\a r, \a parity) pairs to the
         * corresponding invariant values.
         *
         * For even values of \a r, the parity is ignored when calling
         * turaevViro() (since the even and odd versions of the invariant
         * contain essentially the same information).  Therefore, in this
         * cache, all even values of \a r will have the corresponding parities
         * set to \c false.
         *
         * \note All invariants in this cache are now computed using exact
         * arithmetic, as elements of a cyclotomic field.  This is a
         * change from Regina 4.96 and earlier, which computed
         * floating-point approximations instead.
         *
         * \python This routine returns a Python dictionary.
         * It also returns by value, not by reference (i.e., if more
         * Turaev-Viro invariants are computed later on, the dictionary
         * that was originally returned will not change as a result).
         *
         * \return the cache of all Turaev-Viro invariants that have
         * already been calculated.
         *
         * \see turaevViro
         */
        const TuraevViroSet& allCalculatedTuraevViro() const;

        /**
         * Identifies the algebraic longitude as a curve on the boundary of a
         * triangulated knot complement.
         *
         * Specifically, assuming that this triangulation represents the
         * complement of a knot in the 3-sphere, this routine identifies the
         * non-trivial simple closed curve on the boundary whose homology in
         * the 3-manifold is trivial.
         *
         * The curve will be returned as a triple of integers, indicating how
         * many times the longitude intersects each of the three boundary edges.
         * It is always true that the largest of these three integers will be
         * the sum of the other two.
         *
         * At present this routine is fairly restrictive in what triangulations
         * it can work with: it requires the triangulation to be one-vertex
         * and have real (not ideal) boundary.
         * These restrictions may be eased in future versions of Regina.
         *
         * \pre The underlying 3-manifold is known to be the complement
         * of a knot in the 3-sphere.
         * \pre This triangulation has precisely one vertex, and its
         * (unique) boundary component is formed from two triangles.
         *
         * \warning If you have an _ideal_ triangulation of a knot
         * complement, you _must_ first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not a valid
         * one-vertex orientable triangulation with homology \a Z, and with a
         * two-triangle torus as its one and only boundary component.
         * Note that this does not capture all of the preconditions for
         * this routine, but it does capture those that are easy to test.
         *
         * \exception UnsolvedCase An integer overflow occurred during
         * the computation.
         *
         * \return a triple of non-negative integers indicating how many
         * times the longitude intersects each of the three boundary edges.
         * Specifically, if the returned tuple is \a t and the unique boundary
         * component is \a bc, then for each \a k = 0,1,2, the element
         * `t[k]` indicates the (absolute) number of times that the
         * longitude intersects the edge `bc->edge(k)`.
         */
        std::array<long, 3> longitudeCuts() const;

        /**
         * Modifies a triangulated knot complement so that the algebraic
         * longitude follows a single boundary edge, and returns this edge.
         *
         * Assuming that this triangulation represents the complement of
         * a knot in the 3-sphere, this routine:
         *
         * - identifies the algebraic longitude of the knot complement;
         *   that is, identifies the non-trivial simple closed curve on
         *   the boundary whose homology in the 3-manifold is trivial;
         *
         * - layers additional tetrahedra on the boundary if necessary
         *   so that this curve is represented by a single boundary edge;
         *
         * - returns that (possibly new) boundary edge.
         *
         * Whilst this routine returns less information than
         * meridianLongitude(), it (1) runs much faster since it is based
         * on fast algebraic calculations, and (2) guarantees to terminate.
         * In contrast, meridianLongitude() must repeatedly try to test for
         * 3-spheres, and (as a result of only using fast 3-sphere
         * recognition heuristics) does not guarantee to terminate.
         *
         * At present this routine is fairly restrictive in what triangulations
         * it can work with: it requires the triangulation to be one-vertex
         * and have real (not ideal) boundary.
         * These restrictions may be eased in future versions of Regina.
         *
         * If the algebraic longitude is already represented by a single
         * boundary edge, then it is guaranteed that this routine will
         * _not_ modify the triangulation, and will simply return this
         * boundary edge.
         *
         * \pre The underlying 3-manifold is known to be the complement
         * of a knot in the 3-sphere.
         * \pre This triangulation has precisely one vertex, and its
         * (unique) boundary component is formed from two triangles.
         *
         * \warning This routine may modify the triangluation, as
         * explained above, which will have the side-effect of
         * invalidating any existing Vertex, Edge or Triangle references.
         *
         * \warning If you have an _ideal_ triangulation of a knot
         * complement, you _must_ first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not a valid
         * one-vertex orientable triangulation with homology \a Z, and with a
         * two-triangle torus as its one and only boundary component.
         * Note that this does not capture all of the preconditions for
         * this routine, but it does capture those that are easy to test.
         *
         * \exception UnsolvedCase An integer overflow occurred during
         * the computation.
         *
         * \exception LockViolation At least one of the two boundary triangles
         * is currently locked.  This exception will be thrown before any
         * changes are made.  See Simplex<3>::lockFacet() for further details
         * on how such locks work and what their implications are.
         *
         * \return the boundary edge representing the algebraic
         * longitude of the knot (after this triangulation has
         * been modified if necessary).
         */
        Edge<3>* longitude();

        /**
         * Modifies a triangulated knot complement so that the meridian
         * follows a single boundary edge, and returns this edge.
         *
         * Assuming that this triangulation represents the complement of
         * a knot in the 3-sphere, this routine:
         *
         * - identifies the meridian of the knot complement;
         *
         * - layers additional tetrahedra on the boundary if necessary
         *   so that this curve is represented by a single boundary edge;
         *
         * - returns that (possibly new) boundary edge.
         *
         * This routine uses fast heuristics to locate the meridian; as a
         * result, **it does not guarantee to terminate** (but if you find
         * a case where it does not, please let the Regina developers know!).
         * If it does return then it guarantees that the result is correct.
         *
         * This routine uses a similar algorithm to meridianLongitude(),
         * with the same problem that it could be slow and might not terminate.
         * However, meridian() has the advantage that it might produce a
         * smaller triangulation, since there is no need to arrange for the
         * longitude to be a boundary edge also.
         *
         * At present this routine is fairly restrictive in what triangulations
         * it can work with: it requires the triangulation to be one-vertex
         * and have real (not ideal) boundary.
         * These restrictions may be eased in future versions of Regina.
         *
         * If the meridian is already represented by a single boundary edge,
         * then it is guaranteed that, if this routine does terminate,
         * it will _not_ modify the triangulation, and will simply return
         * this boundary edge.
         *
         * \pre The underlying 3-manifold is known to be the complement
         * of a knot in the 3-sphere.
         * \pre This triangulation has precisely one vertex, and its
         * (unique) boundary component is formed from two triangles.
         *
         * \warning This routine may modify the triangluation, as
         * explained above, which will have the side-effect of
         * invalidating any existing Vertex, Edge or Triangle references.
         *
         * \warning If you have an _ideal_ triangulation of a knot
         * complement, you _must_ first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not a valid
         * one-vertex orientable triangulation with homology \a Z, and with a
         * two-triangle torus as its one and only boundary component.
         * Note that this does not capture all of the preconditions for
         * this routine, but it does capture those that are easy to test.
         *
         * \exception UnsolvedCase An integer overflow occurred during
         * the computation.
         *
         * \exception LockViolation At least one of the two boundary triangles
         * is currently locked.  This exception will be thrown before any
         * changes are made.  See Simplex<3>::lockFacet() for further details
         * on how such locks work and what their implications are.
         *
         * \return the boundary edge representing the meridian (after this
         * triangulation has been modified if necessary).
         */
        Edge<3>* meridian();

        /**
         * Modifies a triangulated knot complement so that the meridian and
         * algebraic longitude each follow a single boundary edge, and returns
         * these two edges.
         *
         * Assuming that this triangulation represents the complement of
         * a knot in the 3-sphere, this routine:
         *
         * - identifies the meridian of the knot complement, and also the
         *   algebraic longitude (i.e., the non-trivial simple closed curve on
         *   the boundary whose homology in the 3-manifold is trivial);
         *
         * - layers additional tetrahedra on the boundary if necessary so that
         *   each of these curves is represented by a single boundary edge;
         *
         * - returns these two (possibly new) boundary edges.
         *
         * This routine uses fast heuristics to locate the meridian; as a
         * result, **it does not guarantee to terminate** (but if you find
         * a case where it does not, please let the Regina developers know!).
         * If it does return then it guarantees that the result is correct.
         *
         * Whilst this routine returns more information than longitude(),
         * note that longitude() (1) runs much faster since it is based
         * on fast algebraic calculations, and (2) guarantees to terminate.
         *
         * At present this routine is fairly restrictive in what triangulations
         * it can work with: it requires the triangulation to be one-vertex
         * and have real (not ideal) boundary.
         * These restrictions may be eased in future versions of Regina.
         *
         * If the meridian and algebraic longitude are already both represented
         * by single boundary edges, then it is guaranteed that, if this
         * routine does terminate, it will _not_ modify the triangulation,
         * and will simply return these two boundary edges.
         *
         * \pre The underlying 3-manifold is known to be the complement
         * of a knot in the 3-sphere.
         * \pre This triangulation has precisely one vertex, and its
         * (unique) boundary component is formed from two triangles.
         *
         * \warning This routine may modify the triangluation, as
         * explained above, which will have the side-effect of
         * invalidating any existing Vertex, Edge or Triangle references.
         *
         * \warning If you have an _ideal_ triangulation of a knot
         * complement, you _must_ first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not a valid
         * one-vertex orientable triangulation with homology \a Z, and with a
         * two-triangle torus as its one and only boundary component.
         * Note that this does not capture all of the preconditions for
         * this routine, but it does capture those that are easy to test.
         *
         * \exception UnsolvedCase An integer overflow occurred during
         * the computation.
         *
         * \exception LockViolation At least one of the two boundary triangles
         * is currently locked.  This exception will be thrown before any
         * changes are made.  See Simplex<3>::lockFacet() for further details
         * on how such locks work and what their implications are.
         *
         * \return a pair (\a m, \a l), where \a m is the boundary edge
         * representing the meridian and \a l is the boundary edge representing
         * the algebraic longitude of the knot complement (after this
         * triangulation has been modified if necessary).
         */
        std::pair<Edge<3>*, Edge<3>*> meridianLongitude();

        /*@}*/
        /**
         * \name Normal Surfaces and Angle Structures
         */
        /*@{*/

        /**
         * Returns the link of the given face as a normal surface.
         *
         * Constructing the link of a face begins with building the frontier
         * of a regular neighbourhood of the face.  If this is already a
         * normal surface, then then link is called _thin_.  Otherwise
         * the usual normalisation steps are performed until the surface
         * becomes normal; note that these normalisation steps could
         * change the topology of the surface, and in some pathological
         * cases could even reduce it to the empty surface.
         *
         * \tparam subdim the dimension of the face to link; this must be
         * between 0 and 2 inclusive.
         *
         * \pre The given face is a face of this triangulation.
         *
         * \return a pair (\a s, \a thin), where \a s is the face linking
         * normal surface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         */
        template <int subdim>
        std::pair<NormalSurface, bool> linkingSurface(
            const Face<3, subdim>& face) const;

        /**
         * Determines if this triangulation is 0-efficient.
         * A triangulation is _0-efficient_ if its only normal spheres and
         * discs are vertex linking, and if it has no 2-sphere boundary
         * components.
         *
         * \return \c true if and only if this triangulation is
         * 0-efficient.
         */
        bool isZeroEfficient() const;
        /**
         * Is it already known whether or not this triangulation is
         * 0-efficient?  See isZeroEfficient() for further details.
         *
         * If this property is already known, future calls to isZeroEfficient()
         * will be very fast (simply returning the precalculated value).
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation is 0-efficient; it merely tells you whether
         * the answer has already been computed.
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsZeroEfficient() const;
        /**
         * Determines if this triangulation is 1-efficient.
         *
         * For now, 1-efficiency testing is _only_ available for ideal
         * triangulations in which every vertex link is a torus or Klein bottle.
         * In this setting, an ideal triangulation \a T is _1-efficient_ if,
         * amongst all closed embedded normal surfaces in \a T, there are no
         * surfaces at all of positive Euler characteristic, and the only
         * surfaces with zero Euler characteristic are vertex linking.
         *
         * The scope of 1-efficiency testing might be expanded to a broader
         * class of triangulations in future versions of Regina; what is
         * currently holding this back is the need to choose from the several
         * different definitions available in the literature.
         *
         * \pre This is a valid ideal triangulation in which the link of every
         * vertex is a torus or Klein bottle.
         *
         * \exception FailedPrecondition This triangulation is invalid, empty,
         * and/or has some vertex whose link is not a torus or Klein bottle.
         *
         * \return \c true if and only if this triangulation is 1-efficient.
         */
        bool isOneEfficient() const;
        /**
         * Is it already known whether or not this triangulation is
         * 1-efficient?  See isOneEfficient() for further details.
         *
         * If this property is already known, future calls to isOneEfficient()
         * will be very fast (simply returning the precalculated value).
         *
         * \pre This is a valid ideal triangulation in which the link of every
         * vertex is a torus or Klein bottle.
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation is 1-efficient; it merely tells you whether
         * the answer has already been computed.
         *
         * \exception FailedPrecondition This triangulation is invalid, empty,
         * and/or has some vertex whose link is not a torus or Klein bottle.
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsOneEfficient() const;
        /**
         * Determines whether this triangulation has a normal splitting
         * surface.  See NormalSurface::isSplitting() for details
         * regarding normal splitting surfaces.
         *
         * In the special case where this is the empty triangulation,
         * this routine returns \c false.
         *
         * As of Regina 6.0, this routine is now fast (small polynomial
         * time), and works even for triangulations with more than one
         * connected component.  Thanks to Robert Haraway.
         *
         * \return \c true if and only if this triangulation has a
         * normal splitting surface.
         */
        bool hasSplittingSurface() const;
        /**
         * Searches for a non-vertex-linking normal sphere or disc
         * within this triangulation.  If such a surface exists within
         * this triangulation, this routine is guaranteed to find one.
         *
         * \return a non-vertex-linking normal sphere or disc, or \nullopt if
         * none exists.
         */
        std::optional<NormalSurface> nonTrivialSphereOrDisc() const;
        /**
         * Searches for an octagonal almost normal 2-sphere within this
         * triangulation.  If such a surface exists, this routine is
         * guaranteed to find one.
         *
         * \pre This triangulation is valid, closed, orientable, connected,
         * and 0-efficient.  These preconditions are almost certainly more
         * restrictive than they need to be, but we stay safe for now.
         *
         * \return an octagonal almost normal 2-sphere, or \nullopt if
         * none exists.
         */
        std::optional<NormalSurface> octagonalAlmostNormalSphere() const;
        /**
         * Returns a strict angle structure on this triangulation, if one
         * exists.  Recall that a _strict_ angle structure is one in which
         * every angle is strictly between 0 and π.  If a strict angle
         * structure does exist, then this routine is guaranteed to return one.
         *
         * This routine is designed for scenarios where you already know
         * that a strict angle structure exists.  This means:
         *
         * - If no strict angle structure exists, this routine will throw an
         *   exception, which will incur a significant overhead.
         *
         * - If you do _not_ know in advance whether a strict angle structure
         *   exists, you should call hasStrictAngleStructure() first.  If the
         *   answer is no, this will avoid the overhead of throwing and catching
         *   exceptions.  If the answer is yes, this will have the side-effect
         *   of caching the strict angle structure, which means your subsequent
         *   call to strictAngleStructure() will be essentially instantaneous.
         *
         * The underlying algorithm runs a single linear program (it does
         * _not_ enumerate all vertex angle structures).  This means
         * that it is likely to be fast even for large triangulations.
         *
         * The result of this routine is cached internally: as long as
         * the triangulation does not change, multiple calls to
         * strictAngleStructure() will return identical angle structures,
         * and every call after the first be essentially instantaneous.
         *
         * If the triangulation does change, however, then the cached angle
         * structure will be deleted, and any reference that was returned
         * before will become invalid.
         *
         * As of Regina 7.4, this routine has changed its behaviour for the
         * empty triangulation: it now returns the empty angle structure,
         * which is (strictly speaking) both strict and taut.
         *
         * \exception NoSolution No strict angle structure exists on
         * this triangulation.
         *
         * \return a strict angle structure on this triangulation, if
         * one exists.
         */
        const AngleStructure& strictAngleStructure() const;
        /**
         * Determines whether this triangulation supports a strict angle
         * structure.  Recall that a _strict_ angle structure is one
         * in which every angle is strictly between 0 and π.
         *
         * This routine returns \c false if and only if strictAngleStructure()
         * throws an exception.  However, if you do not _know_ whether a
         * strict angle structure exists, then this routine is faster:
         *
         * - If there is _no_ strict angle structure, this routine will
         *   avoid the overhead of throwing and catching exceptions.
         *
         * - If there _is_ a strict angle structure, this routine will find
         *   and cache this angle structure, which means that any subsequent
         *   call to strictAngleStructure() to retrieve its details will
         *   be essentially instantaneous.
         *
         * The underlying algorithm runs a single linear program (it does
         * _not_ enumerate all vertex angle structures).  This means
         * that it is likely to be fast even for large triangulations.
         *
         * As of Regina 7.4, this routine has changed its behaviour for the
         * empty triangulation: it now returns \c true, since the empty angle
         * structure is (strictly speaking) both strict and taut.
         *
         * \return \c true if and only if a strict angle structure exists on
         * this triangulation.
         */
        bool hasStrictAngleStructure() const;
        /**
         * Is it already known (or trivial to determine) whether or not
         * this triangulation supports a strict angle structure?
         * See hasStrictAngleStructure() for further details.
         *
         * If this property is indeed already known, future calls to
         * strictAngleStructure() and hasStrictAngleStructure() will be
         * very fast (simply returning the precalculated solution).
         *
         * \warning This routine does not actually tell you _whether_
         * the triangulation supports a strict angle structure; it merely
         * tells you whether the answer has already been computed (or is
         * very easily computed).
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsStrictAngleStructure() const;
        /**
         * Returns a generalised angle structure on this triangulation,
         * if one exists.  A _generalised_ angle structure must satisfy the
         * same matching equations as all angle structures do, but there is no
         * constraint on the signs of the angles; in particular, negative
         * angles are allowed.  If a generalised angle structure does exist,
         * then this routine is guaranteed to return one.
         *
         * This routine is designed for scenarios where you already know
         * that a generalised angle structure exists.  This means:
         *
         * - If no generalised angle structure exists, this routine will
         *   throw an exception, which will incur a significant overhead.
         *
         * - It should be rare that you do not know in advance whether a
         *   generalised angle structure exists (see the simple conditions in
         *   the note below).  However, if you don't yet know, you should call
         *   hasGeneralAngleStructure() first.  If the answer is no, this will
         *   avoid the overhead of throwing and catching exceptions.  If the
         *   answer is yes, this will have the side-effect of caching the
         *   angle structure, which means your subsequent call to
         *   generalAngleStructure() will be essentially instantaneous.
         *
         * The underlying algorithm simply solves a system of linear equations,
         * and so should be fast even for large triangulations.
         *
         * The result of this routine is cached internally: as long as
         * the triangulation does not change, multiple calls to
         * generalAngleStructure() will return identical angle structures,
         * and every call after the first be essentially instantaneous.
         *
         * If the triangulation does change, however, then the cached angle
         * structure will be deleted, and any reference that was returned
         * before will become invalid.
         *
         * As of Regina 7.4, this routine has changed its behaviour for the
         * empty triangulation: it now returns the empty angle structure.
         *
         * \note For a valid triangulation with no boundary faces, a
         * generalised angle structure exists if and only if every vertex link
         * is a torus or Klein bottle.  The "only if" direction is a simple
         * Euler characteristic calculation; for the "if" direction see
         * "Angle structures and normal surfaces", Feng Luo and Stephan
         * Tillmann, Trans. Amer. Math. Soc. 360:6 (2008), pp. 2849-2866).
         *
         * \exception NoSolution No generalised angle structure exists on
         * this triangulation.
         *
         * \return a generalised angle structure on this triangulation, if
         * one exists.
         */
        const AngleStructure& generalAngleStructure() const;
        /**
         * Determines whether this triangulation supports a generalised angle
         * structure.  A _generalised_ angle structure must satisfy the
         * same matching equations as all angle structures do, but there is no
         * constraint on the signs of the angles; in particular, negative
         * angles are allowed.
         *
         * This routine returns \c false if and only if generalAngleStructure()
         * throws an exception.  However, if you do not _know_ whether a
         * generalised angle structure exists, then this routine is faster:
         *
         * - If there is _no_ generalised angle structure, this routine will
         *   avoid the overhead of throwing and catching exceptions.
         *
         * - If there _is_ a generalised angle structure, this routine will
         *   find and cache this angle structure, which means that any
         *   subsequent call to generalAngleStructure() to retrieve its
         *   details will be essentially instantaneous.
         *
         * The underlying algorithm simply solves a system of linear equations,
         * and so should be fast even for large triangulations.
         *
         * As of Regina 7.4, this routine has changed its behaviour for the
         * empty triangulation: it now returns \c true, and caches the empty
         * angle structure as its solution.
         *
         * \note For a valid triangulation with no boundary faces, a
         * generalised angle structure exists if and only if every vertex link
         * is a torus or Klein bottle.  The "only if" direction is a simple
         * Euler characteristic calculation; for the "if" direction see
         * "Angle structures and normal surfaces", Feng Luo and Stephan
         * Tillmann, Trans. Amer. Math. Soc. 360:6 (2008), pp. 2849-2866).
         *
         * \note Even if the condition above is true and it is clear that a
         * generalised angle structure should exist, this routine will still do
         * the extra work to compute an explicit solution (in order to fulfil
         * the promise made in the generalAngleStructure() documentation).
         *
         * \return \c true if and only if a generalised angle structure exists
         * on this triangulation.
         */
        bool hasGeneralAngleStructure() const;

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Produces a maximal forest in the 1-skeleton of the triangulation
         * boundary.
         *
         * Note that the edge pointers returned will become invalid once the
         * triangulation has changed.
         *
         * \return a set containing the edges of the maximal forest.
         */
        std::set<Edge<3>*> maximalForestInBoundary() const;
        /**
         * Produces a maximal forest in the triangulation's 1-skeleton.
         * It can be specified whether or not different boundary components
         * may be joined by the maximal forest.
         *
         * An edge leading to an ideal vertex is still a
         * candidate for inclusion in the maximal forest.  For the
         * purposes of this algorithm, any ideal vertex will be treated
         * as any other vertex (and will still be considered part of its
         * own boundary component).
         *
         * Note that the edge pointers returned will become
         * invalid once the triangulation has changed.
         *
         * \param canJoinBoundaries \c true if and only if different
         * boundary components are allowed to be joined by the maximal forest.
         * \return a set containing the edges of the maximal forest.
         */
        std::set<Edge<3>*> maximalForestInSkeleton(
                bool canJoinBoundaries = true) const;

        /**
         * Attempts to simplify this triangulation as intelligently as possible
         * using fast and greedy heuristics.  This routine will attempt to
         * reduce the number of tetrahedra, the number of vertices and the
         * number of boundary triangles (with the number of tetrahedra as its
         * priority).
         *
         * Currently this routine uses simplifyToLocalMinimum() and
         * minimiseVertices() in combination with random 4-4 moves,
         * book opening moves and book closing moves.
         *
         * Although simplify() works very well most of the time,
         * it can occasionally get stuck; in such cases you may wish to try
         * the more powerful but (much) slower simplifyExhaustive() instead.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If any tetrahedra and/or triangles are locked, these locks will be
         * respected: that is, the simplification operation will avoid any
         * moves that would violate these locks (and in particular, no
         * LockViolation exceptions should be thrown).  Of course, however,
         * having locks may make the simplification less effective in reducing
         * the number of tetrahedra.
         *
         * \warning Running this routine multiple times upon the same
         * triangulation may return different results, since the implementation
         * makes random decisions.  More broadly, the implementation of this
         * routine (and therefore its results) may change between different
         * releases of Regina.
         *
         * \note For long-term users of Regina: this is the routine that was
         * for a long time called intelligentSimplify().  It was renamed to
         * simplify() in Regina 7.4.
         *
         * \todo \opt Include random 2-3 moves to get out of wells.
         *
         * \return \c true if and only if the triangulation was successfully
         * simplified.  Otherwise this triangulation will not be changed.
         */
        bool simplify();
        /**
         * Deprecated alias for simplify(), which attempts to simplify this
         * triangulation as intelligently as possible using fast and greedy
         * heuristics.
         *
         * \deprecated This routine has been renamed to simplify().
         * See simplify() for further details.
         *
         * \return \c true if and only if the triangulation was successfully
         * simplified.  Otherwise this triangulation will not be changed.
         */
        [[deprecated]] bool intelligentSimplify();
        /**
         * Uses all known simplification moves to reduce the triangulation
         * monotonically to some local minimum number of tetrahedra.
         *
         * End users will probably not want to call this routine.
         * You should call simplify() if you want a fast (and
         * usually effective) means of simplifying a triangulation, or
         * you should call simplifyExhaustive() if you are still stuck
         * and you want to try a slower but more powerful method instead.
         *
         * The moves used by this routine include 3-2, 2-0 (edge and vertex),
         * 2-1 and boundary shelling moves.
         *
         * Moves that do not reduce the number of tetrahedra
         * (such as 4-4 moves or book opening moves) are not used in this
         * routine.  Such moves do however feature in simplify().
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If any tetrahedra and/or triangles are locked, these locks will be
         * respected: that is, the simplification operation will avoid any
         * moves that would violate these locks (and in particular, no
         * LockViolation exceptions should be thrown).  Of course, however,
         * having locks may make the simplification less effective in reducing
         * the number of tetrahedra.
         *
         * \warning The implementation of this routine (and therefore
         * its results) may change between different releases of Regina.
         *
         * \param perform \c true if we are to perform the
         * simplifications, or \c false if we are only to investigate
         * whether simplifications are possible (defaults to \c true).
         * \return if \a perform is \c true, this routine returns
         * \c true if and only if the triangulation was changed to
         * reduce the number of tetrahedra; if \a perform is \c false,
         * this routine returns \c true if and only if it determines
         * that it is capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Attempts to simplify this triangulation using a slow but
         * exhaustive search through the Pachner graph.  This routine is
         * more powerful but much slower than simplify().
         *
         * Specifically, this routine will iterate through all
         * triangulations that can be reached from this triangulation
         * via 2-3 and 3-2 Pachner moves, without ever exceeding
         * \a height additional tetrahedra beyond the original number.
         *
         * If at any stage it finds a triangulation with _fewer_
         * tetrahedra than the original, then this routine will call
         * simplify() to shrink the triangulation further if
         * possible and will then return \c true.  If it cannot find a
         * triangulation with fewer tetrahedra then it will leave this
         * triangulation unchanged and return \c false.
         *
         * This routine can be very slow and very memory-intensive: the
         * number of triangulations it visits may be superexponential in
         * the number of tetrahedra, and it records every triangulation
         * that it visits (so as to avoid revisiting the same triangulation
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if this fails then try increasing \a height one at a time until
         * either you find a simplification or the routine becomes
         * too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on the
         * number of additional tetrahedra.  This means that the routine will
         * not terminate until a simpler triangulation is found (unless there
         * are so many locks that the number of reachable triangulations is
         * finite).  This means that, if no simpler triangulation exists, the
         * only way to terminate this function is to cancel the operation via
         * a progress tracker (read on for details).
         *
         * If any tetrahedra and/or triangles are locked, these locks will be
         * respected: that is, the retriangulation will avoid any moves that
         * would violate these locks (and in particular, no LockViolation
         * exceptions should be thrown).  Of course, however, having locks may
         * reduce the number of distinct triangulations that can be reached.
         *
         * If you want a _fast_ simplification routine, you should call
         * simplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn triangulations where simplify()
         * finds itself stuck at a local minimum, simplifyExhaustive() is able
         * to "climb out" of such wells.
         *
         * Since Regina 7.0, this routine will not return until either the
         * triangulation is simplified or the exhaustive search is complete,
         * regardless of whether a progress tracker was passed.  If you
         * need the old behaviour (where passing a progress tracker caused
         * the exhaustive search to start in the background), simply call
         * this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a threads.  Even in multithreaded mode, this
         * routine will not return until processing has finished (i.e., either
         * the triangulation was simplified or the search was exhausted), and
         * any change to this triangulation will happen in the calling thread.
         *
         * If this routine is unable to simplify the triangulation, then
         * the triangulation will not be changed.
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition This triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param height the maximum number of _additional_ tetrahedra to
         * allow beyond the number of tetrahedra originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c nullptr if no progress reporting is required.
         * \return \c true if and only if the triangulation was successfully
         * simplified to fewer tetrahedra.
         */
        bool simplifyExhaustive(int height = 1, unsigned threads = 1,
            ProgressTrackerOpen* tracker = nullptr);

        /**
         * Explores all triangulations that can be reached from this via
         * Pachner moves, without exceeding a given number of additional
         * tetrahedra.
         *
         * Specifically, this routine will iterate through all
         * triangulations that can be reached from this triangulation
         * via 2-3 and 3-2 Pachner moves, without ever exceeding
         * \a height additional tetrahedra beyond the original number.
         *
         * For every such triangulation (including this starting
         * triangulation), this routine will call \a action (which must
         * be a function or some other callable object).
         *
         * - \a action must take the following initial argument(s).
         *   Either (a) the first argument must be a triangulation (the precise
         *   type is discussed below), representing the triangluation that has
         *   been found; or else (b) the first two arguments must be of types
         *   const std::string& followed by a triangulation, representing both
         *   the triangulation and _an_ isomorphism signature.
         *   The second form is offered in order to avoid unnecessary
         *   recomputation within the \a action function; however, note that
         *   the signature might not be of the IsoSigClassic type (i.e., it
         *   might not match the output from the default version of isoSig()).
         *   If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - The triangulation argument will be passed as an rvalue; a typical
         *   action could (for example) take it by const reference and query it,
         *   or take it by value and modify it, or take it by rvalue reference
         *   and move it into more permanent storage.
         *
         * - \a action must return a boolean.  If \a action ever returns
         *   \c true, then this indicates that processing should stop
         *   immediately (i.e., no more triangulations will be processed).
         *
         * - \a action may, if it chooses, make changes to this triangulation
         *   (i.e., the original triangulation upon which retriangulate()
         *   was called).  This will not affect the search: all triangulations
         *   that this routine visits will be obtained via Pachner moves
         *   from the original form of this triangulation, before any
         *   subsequent changes (if any) were made.
         *
         * - \a action will only be called once for each triangulation
         *   (including this starting triangulation).  In other words, no
         *   triangulation will be revisited a second time in a single call
         *   to retriangulate().
         *
         * This routine can be very slow and very memory-intensive, since the
         * number of triangulations it visits may be superexponential in
         * the number of tetrahedra, and it records every triangulation
         * that it visits (so as to avoid revisiting the same triangulation
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if necessary try increasing \a height one at a time until
         * this routine becomes too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on the
         * number of additional tetrahedra.  This means that the routine will
         * _never terminate_, unless \a action returns \c true for some
         * triangulation that is passed to it (or unless there are so many
         * locks that the number of reachable triangulations becomes finite).
         *
         * If any tetrahedra and/or triangles are locked, these locks will be
         * respected: that is, the retriangulation will avoid any moves that
         * would violate these locks (and in particular, no LockViolation
         * exceptions should be thrown).  Of course, however, having locks may
         * reduce the number of distinct triangulations that can be reached.
         *
         * Since Regina 7.0, this routine will not return until the exploration
         * of triangulations is complete, regardless of whether a progress
         * tracker was passed.  If you need the old behaviour (where passing a
         * progress tracker caused the enumeration to start in the background),
         * simply call this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads in
         * the argument \a threads.  Even in multithreaded mode, this routine
         * will not return until processing has finished (i.e., either \a action
         * returned \c true, or the search was exhausted).  All calls to
         * \a action will be protected by a mutex (i.e., different threads will
         * never be calling \a action at the same time); as a corollary, the
         * action should avoid expensive operations where possible (otherwise
         * it will become a serialisation bottleneck in the multithreading).
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition This triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * \apinotfinal
         *
         * \python This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the arguments \a tracker and \a args are
         * removed, so you call it as retriangulate(height, threads, action).
         * Moreover, \a action must take exactly two arguments
         * (const std::string&, Triangulation<3>&&) representing a signature
         * and the triangulation, as described in option (b) above.
         *
         * \param height the maximum number of _additional_ tetrahedra to
         * allow beyond the number of tetrahedra originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \param action a function (or other callable object) to call
         * for each triangulation that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial triangulation argument(s).
         * \return \c true if some call to \a action returned \c true (thereby
         * terminating the search early), or \c false if the search ran to
         * completion.
         */
        template <typename Action, typename... Args>
        bool retriangulate(int height, unsigned threads,
            ProgressTrackerOpen* tracker,
            Action&& action, Args&&... args) const;

        /**
         * Ensures that the boundary contains the smallest possible
         * number of triangles, potentially adding tetrahedra to do this.
         *
         * This routine is for use with algorithms that require minimal
         * boundaries (e.g., torus boundaries must contain exactly two
         * triangles).  As noted above, it may in fact increase the
         * total number of tetrahedra in the triangulation (though the
         * implementation does make efforts not to do this).
         *
         * Once this routine is finished, every boundary component will
         * have exactly one vertex, except for sphere and projective
         * plane boundaries which will have exactly two triangles
         * (but three and two vertices respectively).
         *
         * The changes that this routine performs can always be
         * expressed using only close book moves and/or layerings.
         * In particular, this routine never creates new vertices,
         * and it never creates a non-vertex-linking normal disc or sphere
         * if there was not one before.
         *
         * Although this routine only modifies real boundary components,
         * it is fine if the triangulation also contains ideal boundary
         * components (and these simply will be left alone).  If the
         * triangulation contains internal vertices, these will likewise
         * be left untouched.  If you wish to remove internal vertices
         * also, then you should call minimiseVertices() instead.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If this triangle has any locked tetrahedra or locked _internal_
         * triangles, such locks will not prevent this operation from occuring
         * (since none of the moves would violate such locks).  However, this
         * operation does not try to avoid violating locks on _boundary_
         * triangles (and indeed, in some scenarios this would be impossible).
         * Therefore we require as a precondition that no boundary triangles
         * are locked.
         *
         * \pre This triangulation is valid.
         *
         * \pre This triangulation does not have any locked boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not valid.
         *
         * \exception LockViolation This operation attempted a move that would
         * violate a lock on a boundary triangle.  Note that some moves might
         * have been performed already before this exception is thrown,
         * though the topology of the manifold should remain safely unchanged.
         * See Simplex<3>::lockFacet() for further details on how facet locks
         * work and what their implications are.
         *
         * \return \c true if the triangulation was changed, or \c false if
         * every boundary component was already minimal to begin with.
         */
        bool minimiseBoundary();

        /**
         * A deprecated synonym for minimiseBoundary().
         * This ensures that the boundary contains the smallest possible
         * number of triangles, potentially adding tetrahedra to do this.
         *
         * See minimiseBoundary() for further details.
         *
         * \deprecated Regina uses British English throughout its API.
         * This synonym was a special case where Regina used to offer both
         * British and American alternatives, but this will be removed in a
         * future release.  See the page on
         * \ref spelling "spelling throughout Regina" for further details.
         *
         * \pre This triangulation is valid.
         *
         * \pre This triangulation does not have any locked boundary triangles.
         *
         * \exception LockViolation This operation attempted a move that would
         * violate a lock on a boundary triangle.  Note that some moves might
         * have been performed already before this exception is thrown,
         * though the topology of the manifold should remain safely unchanged.
         * See Simplex<3>::lockFacet() for further details on how facet locks
         * work and what their implications are.
         *
         * \return \c true if the triangulation was changed, or \c false if
         * every boundary component was already minimal to begin with.
         */
        [[deprecated]] bool minimizeBoundary();

        /**
         * Ensures that this triangulation contains the smallest possible
         * number of vertices for the 3-manifold that it represents,
         * potentially adding tetrahedra to do this.
         *
         * This routine is for use with algorithms that require a minimal
         * number of vertices (e.g., one-vertex triangulations of closed
         * manifolds, or <i>k</i>-vertex triangulations of the complements
         * of <i>k</i>-component links).  As noted above, this routine may
         * in fact increase the total number of tetrahedra in the triangulation
         * (though the implementation does make efforts not to do this).
         *
         * Once this routine is finished:
         *
         * - every real boundary component will have exactly one vertex, except
         *   for sphere and projective plane boundaries which will have three
         *   and two vertices respectively (i.e., the minimum possible);
         *
         * - for each component of the triangulation that contains one or more
         *   boundary components (either real and/or ideal), there will be
         *   no internal vertices at all;
         *
         * - for each component of the triangulation that has no boundary
         *   components (i.e., that represents a closed 3-manifold), there
         *   will be precisely one vertex.
         *
         * The changes that this routine performs can always be expressed
         * using only close book moves, layerings, collapse edge moves,
         * and/or pinch edge moves.  In particular, this routine never
         * creates new vertices.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * If this triangle has any locked tetrahedra or locked _internal_
         * triangles, such locks will not prevent this operation from occuring
         * (in particular, this routine will use pinch edge moves instead of
         * collapse edge moves where necessary to avoid violating such locks).
         * However, this operation does not try to avoid violating locks on
         * _boundary_ triangles (and indeed, in some scenarios this would be
         * impossible).  Therefore we require as a precondition that no
         * boundary triangles are locked.
         *
         * \pre This triangulation is valid.
         *
         * \pre This triangulation does not have any locked boundary triangles.
         *
         * \exception FailedPrecondition This triangulation is not valid.
         *
         * \exception LockViolation This operation attempted a move that would
         * violate a lock on a boundary triangle.  Note that some moves might
         * have been performed already before this exception is thrown,
         * though the topology of the manifold should remain safely unchanged.
         * See Simplex<3>::lockFacet() for further details on how facet locks
         * work and what their implications are.
         *
         * \return \c true if the triangulation was changed, or \c false if
         * the number of vertices was already minimal to begin with.
         */
        bool minimiseVertices();

        /**
         * A deprecated synonym for minimiseVertices().
         * This ensures that the triangulation contains the smallest possible
         * number of vertices, potentially adding tetrahedra to do this.
         *
         * See minimiseVertices() for further details.
         *
         * \deprecated Regina uses British English throughout its API.
         * This synonym was a special case where Regina used to offer both
         * British and American alternatives, but this will be removed in a
         * future release.  See the page on
         * \ref spelling "spelling throughout Regina" for further details.
         *
         * \pre This triangulation is valid.
         *
         * \pre This triangulation does not have any locked boundary triangles.
         *
         * \exception LockViolation This operation attempted a move that would
         * violate a lock on a boundary triangle.  Note that some moves might
         * have been performed already before this exception is thrown,
         * though the topology of the manifold should remain safely unchanged.
         * See Simplex<3>::lockFacet() for further details on how facet locks
         * work and what their implications are.
         *
         * \return \c true if the triangulation was changed, or \c false if
         * the number of vertices was already minimal to begin with.
         */
        [[deprecated]] bool minimizeVertices();

        /**
         * If possible, performs a 4-4 move about the given edge of this
         * triangulation.  This involves replacing the four tetrahedra joined
         * along the given degree four edge with four new tetrahedra joined
         * along a different (and new) degree four edge instead.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * simplex and/or facet locks.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on locks.
         *
         * In order for this move to make sense and to not to change the
         * topology, we require that:
         *
         * - the given edge is valid and non-boundary; and
         *
         * - the given edge has degree four, and in particular belongs to
         *   four distinct tetrahedra.
         *
         * The move then works as follows.  Consider the octahedron made up of
         * the four original tetrahedra.  This octahedron has three internal
         * axes: one axis is the given edge, and the other two axes are not
         * represented by edges of the triangulation at all.  The move simply
         * retriangulates this octahedron using four new tetrahedra, joined
         * along a new degree four edge that follows one of the other two axes.
         * The argument \a axis indicates which of these other two axes should
         * be used (as described below).
         *
         * If this triangulation is currently oriented, then this 4-4 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param axis indicates which axis of the octahedron the four new
         * tetrahedra should meet along; this must be 0 or 1.  Specifically:
         * consider the four original tetrahedra in the order described by
         * `Edge<3>::embedding(0,...,3)`; call these tetrahedra 0, 1, 2 and 3.
         * If \a axis is 0, then the new axis will separate tetrahedra 0 and 1
         * from 2 and 3.  If \a axis is 1, then the new axis will separate
         * tetrahedra 1 and 2 from 3 and 0.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool move44(Edge<3>* e, int axis);
        /**
         * If possible, performs a 2-1 move at the given end of the given
         * edge of this triangulation.  This involves taking the given degree
         * one edge and effectively merging the (unique) tetrahedron that
         * contains it with an adjacent tetrahedron.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * simplex and/or facet locks.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on locks.
         *
         * In order for this move to make sense and to not to change the
         * topology, we require that:
         *
         * - the given edge \a e is valid and non-boundary;
         *
         * - the given edge \a e has degree one, whereupon we let \a t denote
         *   the unique tetrahedron containing \a e;
         *
         * - the two faces of \a t that do _not_ contain \a e are not joined
         *   to each other;
         *
         * - the face of \a t opposite the given endpoint of \a e is not
         *   boundary, whereupon we let \a f denote this face and we let \a s
         *   denote the adjacent tetrahedron connected to \a t along \a f;
         *
         * - if we consider the two edges of \a s that run from (i) the two
         *   (identified) vertices of \a t opposite \a e to (ii) the vertex
         *   of \a s opposite face \a f, then these two edges are distinct and
         *   not both boundary.
         *
         * The move essentially flattens two triangular faces of \a s together,
         * and merges the remnants of \a s with all of \a t to form a single
         * new tetrahedron which again is folded onto itself to form a new edge
         * of degree one.
         *
         * There are additional "distinct and not both boundary" conditions
         * on faces of the second tetrahedron \a s, but those follow
         * automatically from the final condition above.
         *
         * If this triangulation is currently oriented, then this 2-1 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param edgeEnd the end of the edge _opposite_ that at which the
         * second tetrahedron (to be merged) is joined.  This argument must be
         * 0 or 1, corresponding to the labelling (0,1) of the vertices of the
         * edge as described by EdgeEmbedding<3>::vertices().
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool move21(Edge<3>* e, int edgeEnd);
        /**
         * If possible, performs a 0-2 move about the two specified triangles.
         * This involves fattening these two triangles (which should share a
         * common edge, and which need not be distinct) into a new pair of
         * tetrahedra surrounding a new degree two edge.  This is, in essence,
         * an inverse to the 2-0 edge move.
         *
         * The different variants of move02() allow the two triangles and their
         * common edge to be specified in different ways.  For this variant,
         * the common edge is referenced by both the embedding objects
         * \a e0 and \a e1, and the the two triangles are
         * `e0.tetrahedron()->triangle(e0.vertices()[t0])` and
         * `e1.tetrahedron()->triangle(e1.vertices()[t1])`.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<3>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * To be able to perform this move, we require that:
         *
         * - \a e0 and \a e1 are both embeddings of the same edge \a e;
         *
         * - this common edge \a e is valid;
         *
         * - \a t0 and \a t1 are both either 2 or 3 (which means that the two
         *   triangles listed above do indeed contain \a e).
         *
         * A note regarding facet locks: since this move pries open a _pair_ of
         * adjacent triangles and not just a single triangle, a lock on either
         * of the two triangles involved in this move will prevent the move
         * from taking place.
         *
         * If this triangulation is currently oriented, then this 0-2 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects can no longer be used.
         * However, the arguments \a e0 and \a e1 _can_ still be used since
         * a FaceEmbedding can happily outlive the face that it refers to;
         * see the FaceEmbedding class notes for further details.
         *
         * \pre The edge \a e is an edge of this triangulation.
         *
         * \param e0 an embedding of the common edge \a e of the two
         * triangles about which to perform the move.
         * \param t0 indicates one of the triangles about which to perform the
         * move, with respect to the edge embedding \a e0; this must be 2 or 3.
         * \param e1 another embedding of the edge \a e.
         * \param t1 indicates the other triangle about which to perform the
         * move, with respect to the edge embedding \a e1; this must be 2 or 3.
         * \return \c true if and only if the requested move was able to be
         * performed.
         *
         * \author Alex He
         */
        bool move02(EdgeEmbedding<3> e0, int t0, EdgeEmbedding<3> e1, int t1);
        /**
         * If possible, performs a 0-2 move about the two specified triangles.
         * This involves fattening these two triangles (which should share a
         * common edge, and which need not be distinct) into a new pair of
         * tetrahedra surrounding a new degree two edge.  This is, in essence,
         * an inverse to the 2-0 edge move.
         *
         * The different variants of move02() allow the two triangles and their
         * common edge to be specified in different ways.  For this variant,
         * the common edge is given as the argument \a e, and the two triangles
         * are the triangles incident to \a e that are numbered \a t0 and \a t1
         * (see below for how this numbering scheme works).
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<3>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * To be able to perform this move, we require that:
         *
         * - the given edge \a e is valid;
         *
         * - the numbers \a t0 and \a t1 are both less than or equal to
         *   `e->degree()`, and strictly less than `e->degree()` if \a e is
         *   non-boundary (as required by our numbering scheme for triangles).
         *
         * Our numbering scheme for triangles incident to \a e works as follows:
         *
         * - For each \a i in the range `0 ≤ i < e->degree()`, we assign
         *   the number \a i to the triangle
         *   `emb.tetrahedron()->triangle(emb.vertices()[3])`,
         *   where \a emb denotes `e->embedding(i)`.
         *
         * - If \a e is a boundary edge, then we additionally assign the
         *   number `e->degree()` to the boundary triangle
         *   `emb.tetrahedron()->triangle(emb.vertices()[2])`,
         *   where this time \a emb denotes `e->back()`.
         *
         * A note regarding facet locks: since this move pries open a _pair_ of
         * adjacent triangles and not just a single triangle, a lock on either
         * of the two triangles involved in this move will prevent the move
         * from taking place.
         *
         * If this triangulation is currently oriented, then this 0-2 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * The implementation of this routine simply translates its arguments
         * to call `move02(EdgeEmbedding<3>, int, EdgeEmbedding<3>, int)`.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the common edge of the two triangles about which to
         * perform the move.
         * \param t0 the number assigned to one of two triangles about which
         * to perform the move, as described above.
         * \param t1 the number assigned to the other triangle about which
         * to perform the move, as described above.
         * \return \c true if and only if the requested move was able to be
         * performed.
         *
         * \author Alex He
         */
        bool move02(Edge<3>* e, size_t t0, size_t t1);
        /**
         * If possible, performs a 0-2 move about the two specified triangles.
         * This involves fattening these two triangles (which should share a
         * common edge, and which need not be distinct) into a new pair of
         * tetrahedra surrounding a new degree two edge.  This is, in essence,
         * an inverse to the 2-0 edge move.
         *
         * The different variants of move02() allow the two triangles and their
         * common edge to be specified in different ways.  For this variant,
         * the two triangles are given as the arguments \a t0 and \a t1,
         * and their common edge is `t0->edge(e0)` and `t1->edge(e1)`.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<3>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * To be able to perform this move, we require that:
         *
         * - the edges `t0->edge(e0)` and `t1->edge(e1)` are the same edge
         *   of this triangulation;
         *
         * - moreover, that common edge is valid.
         *
         * A note regarding facet locks: since this move pries open a _pair_ of
         * adjacent triangles and not just a single triangle, a lock on either
         * of the two triangles involved in this move will prevent the move
         * from taking place.
         *
         * If this triangulation is currently oriented, then this 0-2 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * The implementation of this routine simply translates its arguments
         * to call `move02(EdgeEmbedding<3>, int, EdgeEmbedding<3>, int)`.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the arguments
         * \a t0 and \a t1) can no longer be used.
         *
         * \pre The given triangles are both triangles of this triangulation.
         *
         * \param t0 one of the two triangles about which to perform the move.
         * \param e0 the edge at which \a t0 meets the other triangle \a t1;
         * this must be 0, 1 or 2.
         * \param t1 the other triangle about which to perform the move.
         * \param e1 the edge at which \a t1 meets the other triangle \a t0;
         * this must be 0, 1 or 2.
         * \return \c true if and only if the requested move was able to be
         * performed.
         *
         * \author Alex He
         */
        bool move02(Triangle<3>* t0, int e0, Triangle<3>* t1, int e1);
        /**
         * If possible, performs a book opening move about the given triangle.
         * This involves taking a triangle that meets the boundary along
         * precisely two edges, and ungluing it to create two new boundary
         * triangles.  The resulting effect is to expose the tetrahedra it
         * initially joined, and hopefully open the way for subsequent
         * boundary shelling moves.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<3>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * In order to not change the topology, we impose the following
         * requirements:
         *
         * - the given triangle meets the boundary in precisely two edges (and
         *   therefore also joins two tetrahedra;
         *
         * - the vertex between these two edges is a standard boundary vertex
         *   (i.e., its link is a disc);
         *
         * - the remaining edge of the triangle (which is internal to the
         *   triangulation) is valid.
         *
         * Whilst we do not (currently) support the case where the given
         * triangle meets the boundary in just one edge, those triangulations
         * that we miss out on can typically be simplified in other ways
         * (since they would typically have spurious internal vertices).
         *
         * If this triangulation is currently oriented, then this operation
         * will (trivially) preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a t)
         * can no longer be used.
         *
         * See closeBook() for an inverse to this move.
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * \param t the triangle about which to perform the move.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool openBook(Triangle<3>* t);
        /**
         * If possible, performs a book closing move about the given edge.
         * This involves taking a boundary edge of the triangulation and
         * folding together the two boundary triangles on either side.
         * The resulting effect is to simplify the boundary of the
         * triangulation.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * facet locks.  See Simplex<3>::lockFacet() for further details on
         * facet locks.  Note that simplex locks are never a concern for this
         * type of move.
         *
         * In order for this move to make sense and to not to change the
         * topology, we require that:
         *
         * - the given edge \a e is a boundary edge;
         *
         * - the two vertices opposite \a e in the boundary triangles
         *   that contain \a e are valid and distinct;
         *
         * - the boundary component containing \a e contains more than
         *   two triangles.
         *
         * There are several additional distinctness conditions on the nearby
         * edges and triangles, but they follow automatically from the
         * conditions listed above.
         *
         * If this triangulation is currently oriented, then this operation
         * will (trivially) preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * See openBook() for an inverse to this move.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool closeBook(Edge<3>* e);
        /**
         * If possible, performs an edge collapse move upon the given edge.
         * This involves collapsing the edge to a point, merging its two
         * endpoints together, and flattening all of the tetrahedra that
         * contain it.  The resulting effect is to reduce the number of
         * vertices in this triangulation by one.
         *
         * This triangulation will be changed directly.
         *
         * This move will only be performed if it will not change the topology
         * of the manifold (as discussed below), _and_ it will not violate any
         * simplex and/or facet locks.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on locks.
         *
         * The requirements for this move to not change the topology are
         * complex, and are discussed in detail in the collapseEdge() source
         * code for those who are interested.  The most important requirement
         * is that the given edge should join two distinct vertices.  It is
         * also important to note that checking the full requirements is
         * expensive (amongst other things, we need to build a union-find
         * structure to implement the test).
         *
         * If you are trying to reduce the number of vertices without changing
         * the topology, and if \a e is an edge connecting an internal vertex
         * with some different vertex, then either collapseEdge() or pinchEdge()
         * may be more appropriate for your situation (though you may find it
         * easier just to call minimiseVertices() instead).
         *
         * - The advantage of collapseEdge() is that it decreases the
         *   number of tetrahedra, whereas pinchEdge() increases this
         *   number (but only by two).
         *
         * - The disadvantages of collapseEdge() are that it cannot always be
         *   performed, and its validity tests are expensive; pinchEdge() on
         *   the other hand can always be used for edges \a e of the
         *   type described above.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         * \return \c true if and only if the requested move was able to be
         * performed.
         */
        bool collapseEdge(Edge<3>* e);

        /**
         * Determines whether it is possible to perform a 4-4 move about the
         * given edge of this triangulation, without violating any simplex
         * and/or facet locks.
         *
         * For more detail on 4-4 moves and when they can be performed,
         * see move44().
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the candidate edge about which to perform the move.
         * \param axis indicates which axis of the enclosing octahedron the
         * four new proposed tetrahedra should meet along; this must be 0 or 1.
         * See move44() for details on exactly what this means.
         * \return \c true if and only if the requested move can be performed.
         */
        bool has44(Edge<3>* e, int axis) const;
        /**
         * Determines whether it is possible to perform a 2-1 move at the
         * given end of the given edge of this triangulation, without
         * violating any simplex and/or facet locks.
         *
         * For more detail on 2-1 moves and when they can be performed,
         * see move21().
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the candidate edge about which to perform the move.
         * \param edgeEnd indicates at which end of the edge \a e the move
         * does _not_ involve the adjacent tetrahedron; this should be 0 or 1.
         * See move21() for details on exactly what this means.
         * \return \c true if and only if the requested move can be performed.
         */
        bool has21(Edge<3>* e, int edgeEnd) const;
        /**
         * Determines whether it is possible to perform a 0-2 move about the
         * two specified triangles of this triangulation, without violating any
         * facet locks.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(EdgeEmbedding<3>, int, EdgeEmbedding<3>, int).
         *
         * \pre The given embeddings refer to edges of this triangulation.
         *
         * \param e0 an embedding of the common edge \a e of the two
         * candidate triangles about which to perform the move.
         * \param t0 indicates one of the candidate triangles about which to
         * perform the move, with respect to the edge embedding \a e0; this
         * must be 2 or 3.
         * \param e1 another embedding of the edge \a e.
         * \param t1 indicates the other candidate triangle about which to
         * perform the move, with respect to the edge embedding \a e1; this
         * must be 2 or 3.
         * \return \c true if and only if the requested move can be performed.
         *
         * \author Alex He
         */
        bool has02(EdgeEmbedding<3> e0, int t0, EdgeEmbedding<3> e1, int t1)
            const;
        /**
         * Determines whether it is possible to perform a 0-2 move about the
         * two specified triangles of this triangulation, without violating any
         * facet locks.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Edge<3>*, size_t, size_t).
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the common edge of the two candidate triangles about which
         * to perform the move.
         * \param t0 the number assigned to one of two candidate triangles
         * about which to perform the move.
         * \param t1 the number assigned to the other candidate triangle
         * about which to perform the move.
         * \return \c true if and only if the requested move can be performed.
         *
         * \author Alex He
         */
        bool has02(Edge<3>* e, size_t t0, size_t t1) const;
        /**
         * Determines whether it is possible to perform a 0-2 move about the
         * two given triangles of this triangulation, without violating any
         * facet locks.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Triangle<3>*, int, Triangle<3>*, int).
         *
         * \pre The given triangles are both triangles of this triangulation.
         *
         * \param t0 one of the two candidate triangles about which to perform
         * the move.
         * \param e0 the edge at which \a t0 meets the other triangle \a t1;
         * this must be 0, 1 or 2.
         * \param t1 the other candidate triangle about which to perform the
         * move.
         * \param e1 the edge at which \a t1 meets the other triangle \a t0;
         * this must be 0, 1 or 2.
         * \return \c true if and only if the requested move can be performed.
         *
         * \author Alex He
         */
        bool has02(Triangle<3>* t0, int e0, Triangle<3>* t1, int e1) const;
        /**
         * Determines whether it is possible to perform a book opening move
         * about the given triangle of this triangulation, without violating
         * any facet locks.
         *
         * For more detail on book opening moves and when they can be
         * performed, see openBook().
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * \param t the candidate triangle about which to perform the move.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasOpenBook(Triangle<3>* t) const;
        /**
         * Determines whether it is possible to perform a book closing move
         * about the given edge of this triangulation, without violating any
         * facet locks.
         *
         * For more detail on book closing moves and when they can be
         * performed, see closeBook().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the candidate edge about which to perform the move.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasCloseBook(Edge<3>* e) const;
        /**
         * Determines whether it is possible to collapse the given edge of
         * this triangulation, without violating any simplex and/or facet locks.
         *
         * For more detail on edge collapse moves and when they can be
         * performed, see collapseEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the candidate edge to collapse.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasCollapseEdge(Edge<3>* e) const;

        /**
         * If possible, returns the triangulation obtained by performing a
         * 4-4 move about the given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * simplex and/or facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 4-4 moves and when they can be performed,
         * see move44().
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param axis indicates which axis of the enclosing octahedron the
         * four new tetrahedra should meet along; this must be 0 or 1.  See
         * move44() for details on exactly what this means.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<3>> with44(Edge<3>* e, int axis) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * 2-1 move at the given end of the given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * simplex and/or facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 2-1 moves and when they can be performed,
         * see move21().
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param edgeEnd indicates at which end of the edge \a e the move
         * does _not_ involve the adjacent tetrahedron; this should be 0 or 1.
         * See move21() for details on exactly what this means.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<3>> with21(Edge<3>* e, int edgeEnd) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * 0-2 move about the two specified triangles of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(EdgeEmbedding<3>, int, EdgeEmbedding<3>, int).
         *
         * \pre The given embeddings refer to edges of this triangulation.
         *
         * \param e0 an embedding of the common edge \a e of the two
         * triangles about which to perform the move.
         * \param t0 indicates one of the triangles about which to perform the
         * move, with respect to the edge embedding \a e0; this must be 2 or 3.
         * \param e1 another embedding of the edge \a e.
         * \param t1 indicates the other triangle about which to perform the
         * move, with respect to the edge embedding \a e1; this must be 2 or 3.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         *
         * \author Alex He
         */
        std::optional<Triangulation<3>> with02(EdgeEmbedding<3> e0, int t0,
            EdgeEmbedding<3> e1, int t1) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * 0-2 move about the two specified triangles of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Edge<3>*, size_t, size_t).
         *
         * \pre The given edge is a edge of this triangulation.
         *
         * \param e the common edge of the two triangles about which to
         * perform the move.
         * \param t0 the number assigned to one of two triangles about which
         * to perform the move.
         * \param t1 the number assigned to the other triangle about which
         * to perform the move.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         *
         * \author Alex He
         */
        std::optional<Triangulation<3>> with02(Edge<3>* e,
            size_t t0, size_t t1) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * 0-2 move about the two given triangles of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Triangle<3>*, int, Triangle<3>*, int).
         *
         * \pre The given triangles are both triangles of this triangulation.
         *
         * \param t0 one of the two triangles about which to perform the move.
         * \param e0 the edge at which \a t0 meets the other triangle \a t1;
         * this must be 0, 1 or 2.
         * \param t1 the other triangle about which to perform the move.
         * \param e1 the edge at which \a t1 meets the other triangle \a t0;
         * this must be 0, 1 or 2.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         *
         * \author Alex He
         */
        std::optional<Triangulation<3>> with02(Triangle<3>* t0, int e0,
            Triangle<3>* t1, int e1) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * book opening move about the given triangle of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on book opening moves and when they can be
         * performed, see openBook().
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * \param t the triangle about which to perform the move.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<3>> withOpenBook(Triangle<3>* t) const;
        /**
         * If possible, returns the triangulation obtained by performing a
         * book closing move about the given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on book closing moves and when they can be
         * performed, see closeBook().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<3>> withCloseBook(Edge<3>* e) const;
        /**
         * If possible, returns the triangulation obtained by collapsing the
         * given edge of this triangulation.
         * If such a move is not allowed, or if such a move would violate any
         * simplex and/or facet locks, then this routine returns no value.
         *
         * This triangulation will not be changed.
         *
         * For more detail on edge collapse moves and when they can be
         * performed, see collapseEdge().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         * \return The new triangulation obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Triangulation<3>> withCollapseEdge(Edge<3>* e) const;

        /**
         * Deprecated routine that tests for and optionally performs a
         * 4-4 move about the given edge of this triangulation.
         *
         * For more details on 4-4 moves and when they can be performed,
         * see move44().
         *
         * This routine will always _check_ whether the requested move is
         * legal and will not violate any simplex and/or facet locks (see
         * Simplex<3>::lock() and Simplex<3>::lockFacet() for further details
         * on locks).  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has44().  If you wish to both check and perform the move, call
         * move44().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param axis indicates which axis of the enclosing octahedron the
         * four new tetrahedra should meet along; this must be 0 or 1.  See
         * move44() for details on exactly what this means.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool fourFourMove(Edge<3>* e, int axis, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * 2-1 move at the given end of the given edge of this triangulation.
         *
         * For more details on 2-1 moves and when they can be performed,
         * see move21().
         *
         * This routine will always _check_ whether the requested move is
         * legal and will not violate any simplex and/or facet locks (see
         * Simplex<3>::lock() and Simplex<3>::lockFacet() for further details
         * on locks).  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has21().  If you wish to both check and perform the move, call
         * move21().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param edgeEnd indicates at which end of the edge \a e the move
         * does _not_ involve the adjacent tetrahedron; this should be 0 or 1.
         * See move21() for details on exactly what this means.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool twoOneMove(Edge<3>* e, int edgeEnd, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * 0-2 move about the two specified triangles of this triangulation.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(EdgeEmbedding<3>, int, EdgeEmbedding<3>, int).
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<3>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has02().  If you wish to both check and perform the move, call
         * move02().
         *
         * \pre The two given edge embeddings both refer to the same edge,
         * which must be an edge of this triangulation.
         *
         * \param e0 an embedding of the common edge \a e of the two
         * triangles about which to perform the move.
         * \param t0 indicates one of the triangles about which to perform the
         * move, with respect to the edge embedding \a e0; this must be 2 or 3.
         * \param e1 another embedding of the edge \a e.
         * \param t1 indicates the other triangle about which to perform the
         * move, with respect to the edge embedding \a e1; this must be 2 or 3.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         *
         * \author Alex He
         */
        [[deprecated]] bool zeroTwoMove(EdgeEmbedding<3> e0, int t0,
            EdgeEmbedding<3> e1, int t1, bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * 0-2 move about the two specified triangles of this triangulation.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Edge<3>*, size_t, size_t).
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<3>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has02().  If you wish to both check and perform the move, call
         * move02().
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the common edge of the two triangles about which to
         * perform the move.
         * \param t0 the number assigned to one of two triangles about which
         * to perform the move.
         * \param t1 the number assigned to the other triangle about which
         * to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         *
         * \author Alex He
         */
        [[deprecated]] bool zeroTwoMove(Edge<3>* e, size_t t0, size_t t1,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * 0-2 move about the two given triangles of this triangulation.
         *
         * For more detail on 0-2 moves and when they can be performed, and
         * for full details on what the arguments to this function mean, see
         * move02(Triangle<3>*, int, Triangle<3>*, int).
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<3>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call has02().  If you wish to both check and perform the move, call
         * move02().
         *
         * \pre The given triangles are both triangles of this triangulation.
         *
         * \param t0 one of the two triangles about which to perform the move.
         * \param e0 the edge at which \a t0 meets the other triangle \a t1;
         * this must be 0, 1 or 2.
         * \param t1 the other triangle about which to perform the move.
         * \param e1 the edge at which \a t1 meets the other triangle \a t0;
         * this must be 0, 1 or 2.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         *
         * \author Alex He
         */
        [[deprecated]] bool zeroTwoMove(Triangle<3>* t0, int e0,
            Triangle<3>* t1, int e1, bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * book opening move about the given triangle of this triangulation.
         *
         * For more details on book opening moves and when they can be
         * performed, see the variant of openBook() without the extra
         * boolean arguments.
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<3>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasOpenBook().  If you wish to both check and perform the move,
         * call openBook() without the two extra boolean arguments.
         *
         * \pre The given triangle is a triangle of this triangulation.
         *
         * \param t the triangle about which to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool openBook(Triangle<3>* t, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a
         * book closing move about the given edge of this triangulation.
         *
         * For more details on book closing moves and when they can be
         * performed, see the variant of closeBook() without the extra
         * boolean arguments.
         *
         * This routine will always _check_ whether the requested move is legal
         * and will not violate any facet locks (see Simplex<3>::lockFacet()
         * for further details on facet locks).  Note that this type of move can
         * never violate a simplex lock, and so there is no need to check for
         * those at all.  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasCloseBook().  If you wish to both check and perform the move,
         * call closeBook() without the two extra boolean arguments.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge about which to perform the move.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool closeBook(Edge<3>* e, bool ignored,
            bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs an
         * edge collapse move upon the given edge of this triangulation.
         *
         * For more details on edge collapse moves and when they can be
         * performed, as well as the difference between edge collapse and
         * edge pinch moves, see the variant of collapseEdge() without the
         * extra boolean arguments.
         *
         * This routine will always _check_ whether the requested move is
         * legal and will not violate any simplex and/or facet locks (see
         * Simplex<3>::lock() and Simplex<3>::lockFacet() for further details
         * on locks).  If the move _is_ allowed, and if the argument
         * \a perform is \c true, this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasCollapseEdge().  If you wish to both check and perform the
         * move, call collapseEdge() without the two extra boolean arguments.
         *
         * \pre The given edge is an edge of this triangulation.
         *
         * \param e the edge to collapse.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool collapseEdge(Edge<3>* e, bool ignored,
            bool perform = true);

        /**
         * Deprecated alias for reorderBFS(), which reorders the tetrahedra
         * of this triangulation using a breadth-first search.
         *
         * \deprecated This routine has been renamed reorderBFS() (and is now
         * available for triangulations in all dimension).  See reorderBFS()
         * for further details.
         *
         * \param reverse \c true if the new tetrahedron numbers should
         * be assigned in reverse order, as described above.
         */
        [[deprecated]] void reorderTetrahedraBFS(bool reverse = false);

        /**
         * Relabels tetrahedron vertices in this triangulation to give
         * an ordered triangulation, if possible.
         *
         * To be an ordered triangulation, all face gluings (when restricted
         * to the tetrahedron face) must be order preserving. In other words,
         * it must be possible to orient all edges of the triangulation in
         * such a fashion that they are consistent with the ordering of the
         * vertices in each tetrahedron.
         *
         * If it is possible to order this triangulation, the vertices
         * of each tetrahedron will be relabelled accordingly and this
         * routine will return \c true.  Otherwise, this routine will
         * return \c false and the triangulation will not be changed.
         *
         * If this triangulation has locks on any tetrahedra and/or their
         * facets, these will not prevent the ordering from taking place.
         * Instead, any locks will be transformed accordingly (i.e., the
         * facets of each tetrahedron will exchange their lock states according
         * to how the vertices of that tetrahedron have been relabelled).
         *
         * \warning This routine may be slow, since it backtracks
         * through all possible edge orientations until a consistent one
         * has been found.
         *
         * \param forceOriented \c true if the triangulation must be
         * both ordered and _oriented_, in which case this routine will
         * return \c false if the triangulation cannot be oriented and
         * ordered at the same time.  See orient() for further details.
         * \return \c true if the triangulation has been successfully ordered
         * as described above, or \c false if not.
         *
         * \author Matthias Goerner
         */
        bool order(bool forceOriented = false);

        /*@}*/
        /**
         * \name Decompositions
         */
        /*@{*/

        /**
         * Computes the connected sum decomposition of this triangulation.
         *
         * The prime summands will be returned as a vector of triangulations;
         * this triangulation will not be modified.
         *
         * As far as possible, the summands will be represented using
         * 0-efficient triangulations (i.e., triangulations that contain
         * no non-vertex-linking normal spheres).  Specifically, for
         * every summand, either:
         *
         * - the triangulation of the summand that is produced will be
         *   0-efficient; or
         *
         * - the summand is one of RP3, the product S2xS1, or the twisted
         *   product S2x~S1.  In each of these cases there is no possible
         *   0-efficient triangulation of the summand, and so the triangulation
         *   that is produced will just be minimal (i.e., two tetrahedra).
         *
         * For non-orientable triangulations, this routine is only guaranteed
         * to succeed if the original manifold contains no embedded two-sided
         * projective planes.  If the manifold _does_ contain embedded
         * two-sided projective planes, then this routine might still succeed
         * but it might fail; however, such a failure will always be detected,
         * and in such a case this routine will throw an exception (as
         * detailed below).
         *
         * Note that this routine is currently only available for closed
         * triangulations; see the list of preconditions for full details.
         * If this triangulation is a 3-sphere then this routine will return
         * an empty list.
         *
         * This function is new to Regina 7.0, and it has some important
         * changes of behaviour from the old connectedSumDecomposition() from
         * Regina 6.0.1 and earlier:
         *
         * - This function does not insert the resulting components into
         *   the packet tree.
         *
         * - If this routine fails because of an embedded two-sided projective
         *   plane, then it throws an exception instead of returning -1.
         *
         * - This function does not assign labels to the new summands.
         *
         * The underlying algorithm appears in "A new approach to crushing
         * 3-manifold triangulations", Discrete and Computational
         * Geometry 52:1 (2014), pp. 116-139.  This algorithm is based on the
         * Jaco-Rubinstein 0-efficiency algorithm, and works in both
         * orientable and non-orientable settings.
         *
         * If any tetrahedra and/or triangles in this triangulation are locked,
         * this will not prevent summands() from doing its work (since the
         * original triangulation will not be changed).  The triangulations
         * that are returned (i.e., the prime summands) will have no simplex
         * and/or facet locks at all.
         *
         * \warning Users are strongly advised to check for exceptions if
         * embedded two-sided projective planes are a possibility, since in
         * such a case this routine might fail (as explained above).
         * Note however that this routine might still succeed, and so success
         * is not a proof that no embedded two-sided projective planes exist.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so can be very slow for larger triangulations.
         * For 3-sphere testing, see the routine isSphere() which
         * uses faster methods where possible.
         *
         * \pre This triangulation is valid, closed and connected.
         *
         * \exception UnsolvedCase The original manifold is non-orientable
         * and contains one or more embedded two-sided projective planes,
         * and this routine was not able to recover from this situation.
         *
         * \return a list of triangulations of prime summands.
         */
        std::vector<Triangulation<3>> summands() const;

        /**
         * Determines whether this is a triangulation of a 3-sphere.
         *
         * This routine relies upon a combination of Rubinstein's 3-sphere
         * recognition algorithm and Jaco and Rubinstein's 0-efficiency
         * prime decomposition algorithm.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so can be very slow for larger
         * triangulations (although faster tests are used where possible).
         * The routine knowsSphere() can be called to see if this
         * property is already known or if it happens to be very fast to
         * calculate for this triangulation.
         *
         * \return \c true if and only if this is a 3-sphere triangulation.
         */
        bool isSphere() const;
        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of a 3-sphere?  See isSphere() for
         * further details.
         *
         * If this property is indeed already known, future calls to isSphere()
         * will be very fast (simply returning the precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for isSphere() and this routine will
         * return \c true.
         *
         * Otherwise a call to isSphere() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation forms a 3-sphere; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsSphere() const;
        /**
         * Determines whether this is a triangulation of a 3-dimensional ball.
         *
         * This routine is based on isSphere(), which in turn combines
         * Rubinstein's 3-sphere recognition algorithm with Jaco and
         * Rubinstein's 0-efficiency prime decomposition algorithm.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so can be very slow for larger
         * triangulations (although faster tests are used where possible).
         * The routine knowsBall() can be called to see if this
         * property is already known or if it happens to be very fast to
         * calculate for this triangulation.
         *
         * \return \c true if and only if this is a triangulation of a
         * 3-dimensional ball.
         */
        bool isBall() const;
        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of a 3-dimensional ball?  See isBall() for
         * further details.
         *
         * If this property is indeed already known, future calls to isBall()
         * will be very fast (simply returning the precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for isBall() and this routine will
         * return \c true.
         *
         * Otherwise a call to isBall() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation forms a ball; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsBall() const;
        /**
         * Determines whether this is a triangulation of the solid
         * torus; that is, the unknot complement.  This routine can be
         * used on a triangulation with real boundary triangles, or on an
         * ideal triangulation (in which case all ideal vertices will
         * be assumed to be truncated).
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so might be very slow for larger
         * triangulations (although faster tests are used where possible).
         * The routine knowsSolidTorus() can be called to see if this
         * property is already known or if it happens to be very fast to
         * calculate for this triangulation.
         *
         * \return \c true if and only if this is either a real (compact)
         * or ideal (non-compact) triangulation of the solid torus.
         */
        bool isSolidTorus() const;
        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of a solid torus (that is, the unknot
         * complement)?  See isSolidTorus() for further details.
         *
         * If this property is indeed already known, future calls to
         * isSolidTorus() will be very fast (simply returning the
         * precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for isSolidTorus() and this routine will
         * return \c true.
         *
         * Otherwise a call to isSolidTorus() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation forms a solid torus; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsSolidTorus() const;

        /**
         * Determines whether this is a triangulation of an orientable
         * handlebody, and if so, which genus.  Specifically, this routine
         * returns the genus if this is indeed a handlebody, and returns
         * -1 otherwise. This routine can be used on a triangulation with
         * real boundary triangles, or on an ideal triangulation (in which
         * case all ideal vertices will be assumed to be truncated).
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so might be very slow for larger triangulations
         * (although faster tests are used where possible). The routine
         * knowsHandlebody() can be called to see if this property is already
         * known or if it happens to be very fast to calculate for this
         * triangulation.
         *
         * \return the genus if this is a triangulation of an orientable
         * handlebody, or -1 otherwise.
         *
         * \author Alex He
         */
        ssize_t recogniseHandlebody() const;
        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of an orientable handlebody? See
         * recogniseHandlebody() for further details.
         *
         * If this property is indeed already known, future calls to
         * recogniseHandlebody() will be very fast (simply returning the
         * precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no. If so, it will store \c false as the
         * precalculated value for recogniseHandlebody() and this routine will
         * return \c true.
         *
         * Otherwise a call to recogniseHandlebody() may potentially require
         * more significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_ this
         * triangulation forms an orientable handlebody; it merely tells you
         * whether the answer has already been computed (or is very easily
         * computed).
         *
         * \return \c true if and only if this property is already known or
         * trivial to calculate.
         *
         * \author Alex He
         */
        bool knowsHandlebody() const;

        /**
         * Determines whether or not the underlying 3-manifold is
         * the product of a torus with an interval.
         *
         * This routine can be used on a triangulation with real boundary
         * triangles, or ideal boundary components, or a mix of both.
         * If there are any ideal vertices, they will be treated as though
         * they were truncated.
         *
         * The underlying algorithm is due to Robert C. Haraway, III; see
         * https://journals.carleton.ca/jocg/index.php/jocg/article/view/433
         * for details.
         *
         * \warning This algorithm ultimately relies on isSolidTorus(),
         * which might run slowly for large triangulations.
         *
         * \return \c true if and only if this is a triangulation (either
         * real, ideal or a combination) of the product of the torus with an
         * interval.
         */
        bool isTxI() const;

        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of a the product of a torus with an interval?
         * See isTxI() for further details.
         *
         * If this property is indeed already known, future calls to
         * isTxI() will be very fast (simply returning the precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for isTxI() and this routine will return \c true.
         *
         * Otherwise a call to isTxI() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_
         * this triangulation forms the product of the torus with an interval;
         * it merely tells you whether the answer has already been computed
         * (or is very easily computed).
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsTxI() const;

        /**
         * Determines whether the underlying 3-manifold (which must be
         * closed) is irreducible.  In other words, this routine determines
         * whether every embedded sphere in the underlying 3-manifold
         * bounds a ball.
         *
         * If the underlying 3-manifold is orientable, this routine will
         * use fast crushing and branch-and-bound methods.  If the
         * underlying 3-manifold is non-orientable, it will use a
         * (much slower) full enumeration of vertex normal surfaces.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and might be slow for larger triangulations.
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * \return \c true if and only if the underlying 3-manifold is
         * irreducible.
         */
        bool isIrreducible() const;
        /**
         * Is it already known (or trivial to determine) whether or not the
         * underlying 3-manifold is irreducible?  See isIrreducible() for
         * further details.
         *
         * If this property is indeed already known, future calls to
         * isIrreducible() will be very fast (simply returning the
         * precalculated value).
         *
         * \warning This routine does not actually tell you _whether_
         * the underlying 3-manifold is irreducible; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsIrreducible() const;

        /**
         * Searches for a compressing disc within the underlying
         * 3-manifold.
         *
         * Let \a M be the underlying 3-manifold and let \a B be its
         * boundary.  By a _compressing disc_, we mean a disc \a D
         * properly embedded in \a M, where the boundary of \a D
         * lies in \a B but does not bound a disc in \a B.
         *
         * This routine will first call the heuristic routine
         * hasSimpleCompressingDisc() in the hope of obtaining a fast
         * answer.  If this fails, it will do one of two things:
         *
         * - If the triangulation is orientable and 1-vertex, it will
         *   use the linear programming and crushing machinery outlined in
         *   "Computing closed essential surfaces in knot complements",
         *   Burton, Coward and Tillmann, SCG '13, p405-414, 2013.
         *   This is often extremely fast, even for triangulations with
         *   many tetrahedra.
         *
         * - If the triangulation is non-orientable or has multiple vertices
         *   then it will run a full enumeration of
         *   vertex normal surfaces, as described in "Algorithms for the
         *   complete decomposition of a closed 3-manifold",
         *   Jaco and Tollefson, Illinois J. Math. 39 (1995), 358-406.
         *   As the number of tetrahedra grows, this can become extremely slow.
         *
         * This routine will work on a copy of this triangulation, not
         * the original.  In particular, the copy will be simplified, which
         * means that there is no harm in calling this routine on an
         * unsimplified triangulation.
         *
         * If this triangulation has no boundary components, this
         * routine will simply return \c false.
         *
         * \pre This triangulation is valid and is not ideal.
         * \pre The underlying 3-manifold is irreducible.
         *
         * \warning This routine can be infeasibly slow for large
         * triangulations (particularly those that are non-orientable
         * or have multiple vertices), since it may need to perform a
         * full enumeration of vertex normal surfaces, and since it might
         * perform "large" operations on these surfaces such as cutting along
         * them.  See hasSimpleCompressingDisc() for a "heuristic shortcut"
         * that is faster but might not give a definitive answer.
         *
         * \return \c true if the underlying 3-manifold contains a
         * compressing disc, or \c false if it does not.
         */
        bool hasCompressingDisc() const;
        /**
         * Is it already known (or trivial to determine) whether or not
         * the underlying 3-manifold contains a compressing disc?
         * See hasCompressingDisc() for further details.
         *
         * If this property is indeed already known, future calls to
         * hasCompressingDisc() will be very fast (simply returning the
         * precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for hasCompressingDisc() and this routine will
         * return \c true.
         *
         * Otherwise a call to hasCompressingDisc() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you _whether_
         * the underlying 3-manifold has a compressing disc; it merely tells
         * you whether the answer has already been computed (or is very
         * easily computed).
         *
         * \pre This triangulation is valid and is not ideal.
         * \pre The underlying 3-manifold is irreducible.
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsCompressingDisc() const;

        /**
         * Determines whether the underlying 3-manifold (which
         * must be closed and orientable) is Haken.  In other words, this
         * routine determines whether the underlying 3-manifold contains an
         * embedded closed two-sided incompressible surface.
         *
         * Currently Hakenness testing is available only for irreducible
         * manifolds.  This routine will first test whether the manifold is
         * irreducible and, if it is not, will return \c false immediately.
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * \warning This routine could be very slow for larger triangulations.
         *
         * \return \c true if and only if the underlying 3-manifold is
         * irreducible and Haken.
         */
        bool isHaken() const;
        /**
         * Is it already known (or trivial to determine) whether or not the
         * underlying 3-manifold is Haken?  See isHaken() for further details.
         *
         * If this property is indeed already known, future calls to
         * isHaken() will be very fast (simply returning the
         * precalculated value).
         *
         * \warning This routine does not actually tell you _whether_
         * the underlying 3-manifold is Haken; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * \return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsHaken() const;

        /**
         * Searches for a "simple" compressing disc inside this
         * triangulation.
         *
         * Let \a M be the underlying 3-manifold and let \a B be its
         * boundary.  By a _compressing disc_, we mean a disc \a D
         * properly embedded in \a M, where the boundary of \a D
         * lies in \a B but does not bound a disc in \a B.
         *
         * By a \a simple compressing disc, we mean a compressing disc
         * that has a very simple combinatorial structure (here "simple"
         * is subject to change; see the warning below).  Examples
         * include the compressing disc inside a 1-tetrahedron solid
         * torus, or a compressing disc formed from a single internal triangle
         * surrounded by three boundary edges.
         *
         * The purpose of this routine is to avoid working with normal
         * surfaces within a large triangulation where possible.  This
         * routine is relatively fast, and if it returns \c true then this
         * 3-manifold definitely contains a compressing disc.  If this
         * routine returns \c false then there might or might not be a
         * compressing disc; the user will need to perform a full normal
         * surface enumeration using hasCompressingDisc() to be sure.
         *
         * This routine will work on a copy of this triangulation, not
         * the original.  In particular, the copy will be simplified, which
         * means that there is no harm in calling this routine on an
         * unsimplified triangulation.
         *
         * If this triangulation has no boundary components, this
         * routine will simply return \c false.
         *
         * For further information on this test, see "The Weber-Seifert
         * dodecahedral space is non-Haken", Benjamin A. Burton,
         * J. Hyam Rubinstein and Stephan Tillmann,
         * Trans. Amer. Math. Soc. 364:2 (2012), pp. 911-932.
         *
         * \warning The definition of "simple" is subject to change in
         * future releases of Regina.  That is, this routine may be
         * expanded over time to identify more types of compressing discs
         * (thus making it more useful as a "heuristic shortcut").
         *
         * \pre This triangulation is valid and is not ideal.
         *
         * \return \c true if a simple compressing disc was found,
         * or \c false if not.  Note that even with a return value of
         * \c false, there might still be a compressing disc (just not
         * one with a simple combinatorial structure).
         */
        bool hasSimpleCompressingDisc() const;

        /**
         * Returns a nice tree decomposition of the face pairing graph
         * of this triangulation.  This can (for example) be used in
         * implementing algorithms that are fixed-parameter tractable
         * in the treewidth of the face pairing graph.
         *
         * See TreeDecomposition for further details on tree
         * decompositions, and see TreeDecomposition::makeNice() for
         * details on what it means to be a _nice_ tree decomposition.
         *
         * This routine is fast: it will use a greedy algorithm to find a
         * tree decomposition with (hopefully) small width, but with
         * no guarantees that the width of this tree decomposition is the
         * smallest possible.
         *
         * The tree decomposition will be cached, so that if this routine is
         * called a second time (and the underlying triangulation has not
         * been changed) then the same tree decomposition will be returned
         * immediately.
         *
         * \return a nice tree decomposition of the face pairing graph
         * of this triangulation.
         */
        const TreeDecomposition& niceTreeDecomposition() const;

        /*@}*/
        /**
         * \name Subdivisions, Extensions and Covers
         */
        /*@{*/

        /**
         * Converts an ideal triangulation into a finite triangulation.
         * All ideal or invalid vertices are truncated and thus
         * converted into real boundary components made from unglued
         * faces of tetrahedra.
         *
         * Note that this operation is a loose converse of finiteToIdeal().
         *
         * If this triangulation has any invalid edges, then these will remain
         * invalid after this operation (in contrast to barycentric subdivision,
         * which converts invalid edges into projective plane cusps).  As of
         * Regina 7.4, the presence of invalid edges will no longer cause the
         * triangulation to be subdivided if there are no vertices to truncate.
         *
         * \warning Currently, this routine subdivides all tetrahedra as
         * if <i>all</i> vertices (not just some) were ideal.
         * This may lead to more tetrahedra than are necessary.
         *
         * \exception LockViolation This triangulation contains at least one
         * locked top-dimensional simplex and/or facet.  This exception will be
         * thrown before any changes are made.  See Simplex<3>::lock() and
         * Simplex<3>::lockFacet() for further details on how such locks work
         * and what their implications are.
         *
         * \todo \optlong Have this routine only use as many tetrahedra
         * as are necessary, leaving finite vertices alone.
         *
         * \return \c true if and only if the triangulation was changed.
         * \author David Letscher
         */
        bool idealToFinite();

        /**
         * Pinches an internal edge to a point.  Topologically, this collapses
         * the edge to a point with no further side-effects, and it increases
         * the number of tetrahedra by two.
         *
         * This operation can be performed on _any_ internal edge,
         * without further constraints.  Two particularly useful settings are:
         *
         * - If the edge joins an internal vertex with some different vertex
         *   (which may be internal, boundary, ideal or invalid), then
         *   this move does not change the topology of the manifold at all,
         *   and it reduces the total number of vertices by one.  In
         *   this sense, it acts as an alternative to collapseEdge(),
         *   and unlike collapseEdge() it can _always_ be performed.
         *
         * - If the edge runs from an internal vertex back to itself,
         *   then this move effectively drills out the edge, leaving
         *   an ideal torus or Klein bottle boundary component.
         *
         * We do not allow \a e to lie entirely on the triangulation boundary,
         * because the implementation actually collapses an internal curve
         * _parallel_ to \a e, not the edge \a e itself (and so if \a e is a
         * boundary edge then the topological effect would not be as intended).
         * We do allow \a e to be an internal edge with both endpoints on the
         * boundary, but note that in this case the resulting topological
         * operation would render the triangulation invalid.
         *
         * If you are trying to reduce the number of vertices without changing
         * the topology, and if \a e is an edge connecting an internal vertex
         * with some different vertex, then either collapseEdge() or pinchEdge()
         * may be more appropriate for your situation (though you may find it
         * easier just to call minimiseVertices() instead).
         *
         * - The advantage of collapseEdge() is that it decreases the
         *   number of tetrahedra, whereas pinchEdge() increases this
         *   number (but only by two).
         *
         * - The disadvantages of collapseEdge() are that it cannot always be
         *   performed, and its validity tests are expensive; pinchEdge() on
         *   the other hand can always be used for edges \a e of the
         *   type described above.
         *
         * This operation works by prying open a triangle \a t and inserting a
         * two-tetrahedron gadget \a g within the resulting triangular pillow.
         * In particular, this means that simplex and/or facet locks will
         * never prevent this operation from taking place: if the triangle \a t
         * happens to be locked, then this lock will simply move across to one
         * of the two triangles bounding the gadget \a g.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an internal edge of this triangulation
         * (that is, \a e does not lie entirely within the boundary).
         *
         * \exception InvalidArgument The given edge lies entirely within the
         * boundary of the triangulation.
         *
         * \param e the edge to collapse.
         */
        void pinchEdge(Edge<3>* e);

        /**
         * Punctures this manifold by thickening the given triangle into a
         * triangular pillow and then removing a 3-ball from its interior.
         * If no triangle is specified (i.e., the triangle pointer is \c null),
         * then the triangle used will be facet 0 of tetrahedron 0.
         *
         * The puncture will not meet the boundary of the pillow, and so
         * nothing will go wrong if the given triangle is boundary or has
         * ideal vertices.  A side-effect of this, however, is that the
         * resulting triangulation will contain additional vertices, and will
         * almost certainly be far from minimal.  It is highly recommended
         * that you run simplify() if you do not need to preserve
         * the combinatorial structure of the new triangulation.
         *
         * If this triangulation was originally oriented, then it will also be
         * oriented after this routine has been called.  See isOriented() for
         * further details on oriented triangulations.
         *
         * The new sphere boundary will be formed from two triangles;
         * specifically, face 0 of the last and second-last tetrahedra
         * of the triangulation.  These two triangles will be joined so
         * that vertex 1 of each tetrahedron coincides, and vertices 2,3
         * of one map to vertices 3,2 of the other.
         *
         * Tetrahedron and/or facet locks will not prevent the puncture
         * from taking place.  If the given triangle was locked, then
         * this lock will be moved to one of the two triangles surrounding
         * the triangular pillow.  In particular, if the given triangle
         * is boundary, then the lock will be moved to the corresponding
         * boundary triangle.
         *
         * \pre This triangulation is non-empty, and if \c location is non-null
         * then it is in fact a triangle belonging to this triangulation.
         *
         * \exception InvalidArgument The given triangle is non-null but
         * not a triangle of this triangulation, or the given triangle
         * is null but this triangulation is empty.
         *
         * \param location the triangle indicating where the puncture
         * should be taken.  This may be \c null (the default), in which case
         * facet 0 of tetrahedron 0 will be used.
         */
        void puncture(Triangle<3>* location = nullptr);

        /**
         * Deprecated routine that punctures this manifold by removing a
         * 3-ball from the interior of the given tetrahedron.
         *
         * \deprecated Since the operation in fact involves prying open a
         * triangle, puncture() now takes a triangle instead of a tetrahedron
         * to indicate the location for the operation.  If \a tet is null,
         * then this function is equivalent to calling `puncture()`; otherwise
         * it is equivalent to calling `puncture(tet->triangle(0))`.  See
         * puncture(Triangle<3>*) for further details.
         *
         * \pre This triangulation is non-empty, and if \c tet is non-null
         * then it is in fact a tetrahedron of this triangulation.
         *
         * \exception InvalidArgument The given tetrahedron is non-null but
         * not a tetrahedron of this triangulation, or the given tetrahedron
         * is null but this triangulation is empty.
         *
         * \param tet the tetrahedron indicating where the puncture will be
         * taken.  This may be \c null, in which case tetrahedron 0 will be
         * used.
         */
        [[deprecated]] void puncture(Tetrahedron<3>* tet);

        /*@}*/
        /**
         * \name Building Triangulations
         */
        /*@{*/

        /**
         * Performs a layering upon the given boundary edge of the
         * triangulation.  See the Layering class notes for further
         * details on what a layering entails.
         *
         * The new tetrahedron will be returned, and the new boundary
         * edge that it creates will be edge 5 (i.e., the edge joining
         * vertices 2 and 3) of this tetrahedron.
         *
         * \pre The given edge is a boundary edge of this triangulation,
         * and the two boundary triangles on either side of it are distinct.
         *
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, either the given edge is non-boundary, or the same
         * boundary triangles lie on both sides of it.
         *
         * \exception LockViolation At least one of the two boundary triangles
         * on either side of the given edge is currently locked.  This
         * exception will be thrown before any changes are made.  See
         * Simplex<3>::lockFacet() for further details on how such locks
         * work and what their implications are.
         *
         * \param edge the boundary edge upon which to layer.
         * \return the new tetrahedron provided by the layering.
         */
        Tetrahedron<3>* layerOn(Edge<3>* edge);

        /**
         * Fills a two-triangle torus boundary component by attaching a
         * solid torus along a given curve.
         *
         * The boundary component to be filled should be passed as the
         * argument \a bc; if the triangulation has exactly one
         * boundary component then you may omit \a bc (i.e., pass \c null),
         * and the (unique) boundary component will be inferred.
         *
         * If the boundary component cannot be inferred, and/or if the
         * selected boundary component is not a two-triangle torus, then
         * this routine will do nothing and return \c false.
         *
         * Otherwise the given boundary component will be filled with a
         * solid torus whose meridional curve cuts the edges
         * `bc->edge(0)`, `bc->edge(1)` and `bc->edge(2)`
         * a total of \a cuts0, \a cuts1 and \a cuts2 times respectively.
         *
         * For the filling to be performed successfully, the integers
         * \a cuts0, \a cuts1 and \a cuts2 must be coprime, and two of
         * them must add to give the third.  Otherwise, as above, this
         * routine will do nothing and return \c false.
         *
         * The triangulation will be simplified before returning.
         *
         * There are two versions of fillTorus(); the other takes three
         * explicit edges instead of a boundary component.  You should
         * use the other version if you know how the filling curve cuts
         * each boundary edge but you do not know how these edges are
         * indexed in the boundary component.
         *
         * \param cuts0 the number of times that the meridional curve of
         * the new solid torus should cut the edge `bc->edge(0)`.
         * \param cuts1 the number of times that the meridional curve of
         * the new solid torus should cut the edge `bc->edge(1)`.
         * \param cuts2 the number of times that the meridional curve of
         * the new solid torus should cut the edge `bc->edge(2)`.
         * \param bc the boundary component to fill.  If the triangulation
         * has precisely one boundary component then this may be \c null.
         * \return \c true if the boundary component was filled successfully,
         * or \c false if one of the required conditions as described
         * above is not satisfied.
         */
        bool fillTorus(size_t cuts0, size_t cuts1, size_t cuts2,
            BoundaryComponent<3>* bc = nullptr);

        /**
         * Fills a two-triangle torus boundary component by attaching a
         * solid torus along a given curve.
         *
         * The three edges of the boundary component should be passed as
         * the arguments \a e0, \a e1 and \a e2.
         * The boundary component will then be filled with a
         * solid torus whose meridional curve cuts these three edges
         * \a cuts0, \a cuts1 and \a cuts2 times respectively.
         *
         * For the filling to be performed successfully, the three given
         * edges must belong to the same boundary component, and this boundary
         * component must be a two-triangle torus.  Moreover, the integers
         * \a cuts0, \a cuts1 and \a cuts2 must be coprime, and two of
         * them must add to give the third.  If any of these conditions
         * are not met, then this routine will do nothing and return \c false.
         *
         * The triangulation will be simplified before returning.
         *
         * There are two versions of fillTorus(); the other takes a boundary
         * component, and sets \a e0, \a e1 and \a e2 to its three edges
         * according to Regina's own edge numbering.
         * This version of fillTorus() should be used when you know how the
         * filling curve cuts each boundary edge but you do not know how these
         * edges are indexed in the corresponding boundary component.
         *
         * \param e0 one of the three edges of the boundary component to fill.
         * \param e1 the second of the three edges of the boundary component
         * to fill.
         * \param e2 the second of the three edges of the boundary component
         * to fill.
         * \param cuts0 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e0.
         * \param cuts1 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e1.
         * \param cuts2 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e2.
         * \return \c true if the boundary component was filled successfully,
         * or \c false if one of the required conditions as described
         * above is not satisfied.
         */
        bool fillTorus(Edge<3>* e0, Edge<3>* e1, Edge<3>* e2,
            size_t cuts0, size_t cuts1, size_t cuts2);

        /**
         * Inserts a new layered solid torus into the triangulation.
         * The meridinal disc of the layered solid torus will intersect
         * the three edges of the boundary torus in \a cuts0, \a cuts1
         * and (\a cuts0 + \a cuts1) points respectively.
         *
         * The boundary torus will always consist of faces 012 and 013 of the
         * tetrahedron containing this boundary torus (this tetrahedron will be
         * returned).  In face 012, edges 12, 02 and 01 will meet the meridinal
         * disc \a cuts0, \a cuts1 and (\a cuts0 + \a cuts1) times respectively.
         * The only exceptions are if these three intersection numbers are
         * (1,1,2) or (0,1,1), in which case edges 12, 02 and 01 will meet the
         * meridinal disc (1, 2 and 1) or (1, 1 and 0) times respectively.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \pre 0 ≤ \a cuts0 ≤ \a cuts1;
         * \pre gcd(\a cuts0, \a cuts1) = 1.
         *
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, either \a cuts0 > \a cuts1, and/or \a cuts0 and \a cuts1
         * are not coprime.
         *
         * \param cuts0 the smallest of the three desired intersection numbers.
         * \param cuts1 the second smallest of the three desired intersection
         * numbers.
         * \return the tetrahedron containing the boundary torus.
         *
         * \see LayeredSolidTorus
         */
        Tetrahedron<3>* insertLayeredSolidTorus(size_t cuts0, size_t cuts1);
        /**
         * Forms the connected sum of this triangulation with the given
         * triangulation.  This triangulation will be altered directly.
         *
         * If this and the given triangulation are both oriented, then
         * the result will be oriented also, and the connected sum will
         * respect these orientations.
         *
         * If one or both triangulations contains multiple connected components,
         * this routine will connect the components containing tetrahedron 0
         * of each triangulation, and will copy any additional
         * components across with no modification.
         *
         * If either triangulation is empty, then the result will simply be a
         * clone of the other triangulation.
         *
         * This and/or the given triangulation may be bounded or ideal, or
         * even invalid; in all cases the connected sum will be formed
         * correctly.  Note, however, that the result might possibly
         * contain internal vertices (even if the original triangulations
         * do not).
         *
         * Tetrahedron and/or facet locks will not prevent the connected sum
         * from taking place.  The operation essentially involves prying open
         * two triangles (one from each triangulation) and joining them with a
         * connector gadget; if some original triangle \a t is locked then the
         * lock will be pushed across to one of two triangles that results
         * when \a t is pried open.  In particular, if \a t is a boundary
         * triangle then the lock will be kept on the boundary (as expected).
         *
         * It is allowed to pass this triangulation as \a other.
         *
         * \param other the triangulation to sum with this.
         */
        void connectedSumWith(const Triangulation& other);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Dehydrates this triangulation into an alphabetical string.
         *
         * A _dehydration string_ is a compact text representation
         * of a triangulation, introduced by Callahan, Hildebrand and Weeks
         * for their cusped hyperbolic census (see below).  The dehydration
         * string of an <i>n</i>-tetrahedron triangulation consists of
         * approximately (but not precisely) 5<i>n</i>/2 lower-case letters.
         *
         * Dehydration strings come with some restrictions:
         * - They rely on the triangulation being "canonical" in some
         *   combinatorial sense.  This is not enforced here; instead
         *   a combinatorial isomorphism is applied to make the
         *   triangulation canonical, and this isomorphic triangulation
         *   is dehydrated instead.  Note that the original triangulation
         *   is not changed.
         * - They require the triangulation to be connected.
         * - They require the triangulation to have no boundary triangles
         *   (though ideal triangulations are fine).
         * - They can only support triangulations with at most 25 tetrahedra.
         *
         * The routine rehydrate() can be used to recover a
         * triangulation from a dehydration string.  Note that the
         * triangulation recovered might not be identical to the
         * original, but it is guaranteed to be an isomorphic copy.
         *
         * For a full description of the dehydrated triangulation
         * format, see _A Census of Cusped Hyperbolic 3-Manifolds_,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * \exception NotImplemented Either this triangulation is disconnected,
         * it has boundary triangles, or it contains more than 25 tetrahedra.
         *
         * \return a dehydrated representation of this triangulation
         * (or an isomorphic variant of this triangulation).
         */
        std::string dehydrate() const;

        /**
         * Returns a string containing the full contents of a SnapPea data
         * file that describes this triangulation.  In particular, this string
         * can be used in a Python session to pass the triangulation directly
         * to SnapPy (without writing to the filesystem).
         *
         * Regarding what gets stored in the SnapPea data file:
         *
         * - Since this function is defined by Regina's own Triangulation<3>
         *   class, only the tetrahedron gluings will be included in the SnapPea
         *   data file.  All other SnapPea-specific information (such as
         *   peripheral curves) will be marked as unknown, since Regina does not
         *   track such information itself, and of course Regina-specific
         *   information (such as the Turaev-Viro invariants) will not be
         *   included in the SnapPea file either.
         *
         * - The subclass SnapPeaTriangulation implements its own version
         *   of this function, which writes all additional SnapPea-specific
         *   information to the file (in fact it uses the SnapPea kernel itself
         *   to produce the file contents).  However, to access that function
         *   you must explicitly call SnapPeaTriangulation::snapPea()
         *   (since Triangulation<3> is not a polymorphic class, and in
         *   particular this function is not virtual).
         *
         * If you wish to export a triangulation to a SnapPea _file_, you
         * should call saveSnapPea() instead (which has better performance, and
         * does not require you to construct an enormous intermediate string).
         *
         * SnapPea cannot represent triangulations that are empty, invalid,
         * or contain boundary triangles.  If any of these conditions is
         * true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either empty,
         * invalid, or has boundary triangles.
         *
         * \return a string containing the contents of the corresponding
         * SnapPea data file.
         */
        std::string snapPea() const;

        /**
         * Writes the full contents of a SnapPea data file describing this
         * triangulation to the given output stream.
         *
         * Regarding what gets stored in the SnapPea data file:
         *
         * - Since this function is defined by Regina's own Triangulation<3>
         *   class, only the tetrahedron gluings will be included in the SnapPea
         *   data file.  All other SnapPea-specific information (such as
         *   peripheral curves) will be marked as unknown, since Regina does not
         *   track such information itself, and of course Regina-specific
         *   information (such as the Turaev-Viro invariants) will not be
         *   included in the SnapPea file either.
         *
         * - The subclass SnapPeaTriangulation implements its own version
         *   of this function, which writes all additional SnapPea-specific
         *   information to the file (in fact it uses the SnapPea kernel itself
         *   to produce the file contents).  However, to access that function
         *   you must explicitly call SnapPeaTriangulation::snapPea()
         *   (since Triangulation<3> is not a polymorphic class, and in
         *   particular this function is not virtual).
         *
         * If you wish to extract the SnapPea data file as a string, you should
         * call the zero-argument routine snapPea() instead.  If you wish to
         * write to a real SnapPea data file on the filesystem, you should call
         * saveSnapPea() (which is also available in Python).
         *
         * SnapPea cannot represent triangulations that are empty, invalid,
         * or contain boundary triangles.  If any of these conditions is
         * true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either empty,
         * invalid, or has boundary triangles.
         *
         * \nopython Instead call snapPea() with no arguments, which returns
         * this data as a string.
         *
         * \param out the output stream to which the SnapPea data file
         * will be written.
         */
        void snapPea(std::ostream& out) const;

        /**
         * Writes this triangulation to the given file using SnapPea's
         * native file format.
         *
         * Regarding what gets stored in the SnapPea data file:
         *
         * - Since this function is defined by Regina's own Triangulation<3>
         *   class, only the tetrahedron gluings will be included in the SnapPea
         *   data file.  All other SnapPea-specific information (such as
         *   peripheral curves) will be marked as unknown, since Regina does not
         *   track such information itself, and of course Regina-specific
         *   information (such as the Turaev-Viro invariants) will not be
         *   included in the SnapPea file either.
         *
         * - The subclass SnapPeaTriangulation implements its own version
         *   of this function, which writes all additional SnapPea-specific
         *   information to the file (in fact it uses the SnapPea kernel itself
         *   to produce the file contents).  However, to access that function
         *   you must explicitly call SnapPeaTriangulation::saveSnapPea()
         *   (since Triangulation<3> is not a polymorphic class, and in
         *   particular this function is not virtual).
         *
         * SnapPea cannot represent triangulations that are empty, invalid,
         * or contain boundary triangles.  If any of these conditions is
         * true then the file will not be written and this routine will
         * return \c false.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file _name_, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The _contents_ of the file will be written using UTF-8.
         *
         * \param filename the name of the SnapPea file to which to write.
         * \return \c true if and only if the file was successfully written.
         */
        bool saveSnapPea(const char* filename) const;

        /**
         * Returns a string that expresses this triangulation in
         * Matveev's 3-manifold recogniser format.
         *
         * Recogniser exports are currently not available for triangulations
         * that are invalid or contain boundary triangles.  If either of these
         * conditions is true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either invalid
         * or has boundary triangles.
         *
         * \return a string containing the 3-manifold recogniser data.
         */
        std::string recogniser() const;

        /**
         * A synonym for recogniser().  This returns a string that
         * expresses this triangulation in Matveev's 3-manifold
         * recogniser format.
         *
         * Recogniser exports are currently not available for triangulations
         * that are invalid or contain boundary triangles.  If either of these
         * conditions is true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either invalid
         * or has boundary triangles.
         *
         * \return a string containing the 3-manifold recogniser data.
         */
        std::string recognizer() const;

        /**
         * Writes a string expressing this triangulation in Matveev's
         * 3-manifold recogniser format to the given output stream.
         *
         * Recogniser exports are currently not available for triangulations
         * that are invalid or contain boundary triangles.  If either of these
         * conditions is true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either invalid
         * or has boundary triangles.
         *
         * \nopython Instead call recogniser() with no arguments, which
         * returns this data as a string.
         *
         * \param out the output stream to which the recogniser data file
         * will be written.
         */
        void recogniser(std::ostream& out) const;

        /**
         * A synonym for recognizer(std::ostream&).  This writes
         * a string expressing this triangulation in Matveev's
         * 3-manifold recogniser format to the given output stream.
         *
         * Recogniser exports are currently not available for triangulations
         * that are invalid or contain boundary triangles.  If either of these
         * conditions is true then this routine will throw an exception.
         *
         * \exception NotImplemented This triangulation is either invalid
         * or has boundary triangles.
         *
         * \nopython Instead call recognizer() with no arguments, which
         * returns this data as a string.
         *
         * \param out the output stream to which the recogniser data file
         * will be written.
         */
        void recognizer(std::ostream& out) const;

        /**
         * Writes this triangulation to the given file in Matveev's
         * 3-manifold recogniser format.
         *
         * Recogniser exports are currently not available for triangulations
         * that are invalid or contain boundary triangles.  If either of these
         * conditions is true then the file will not be written, and
         * this routine will return \c false.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file _name_, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The _contents_ of the file will be written using UTF-8.
         *
         * \param filename the name of the Recogniser file to which to write.
         * \return \c true if and only if the file was successfully written.
         */
        bool saveRecogniser(const char* filename) const;

        /**
         * A synonym for saveRecogniser().  This writes this triangulation to
         * the given file in Matveev's 3-manifold recogniser format.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file _name_, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The _contents_ of the file will be written using UTF-8.
         *
         * \param filename the name of the Recogniser file to which to write.
         * \return \c true if and only if the file was successfully written.
         */
        bool saveRecognizer(const char* filename) const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Rehydrates the given alphabetical string into a 3-dimensional
         * triangulation.
         *
         * For a full description of the dehydrated triangulation
         * format, see _A Census of Cusped Hyperbolic 3-Manifolds_,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * The converse routine dehydrate() can be used to extract a
         * dehydration string from an existing triangulation.  Dehydration
         * followed by rehydration might not produce a triangulation identical
         * to the original, but it is guaranteed to produce an isomorphic
         * copy.  See dehydrate() for the reasons behind this.
         *
         * \exception InvalidArgument The given string could not be rehydrated.
         *
         * \param dehydration a dehydrated representation of the
         * triangulation to construct.  Case is irrelevant; all letters
         * will be treated as if they were lower case.
         * \return the rehydrated triangulation.
         */
        static Triangulation<3> rehydrate(const std::string& dehydration);

        /**
         * Extracts the tetrahedron gluings from the contents of a SnapPea
         * data file.  The argument may be the _name_ of a SnapPea file, or
         * it may also be the _contents_ of a SnapPea file (so the file itself
         * need not actually exist on the filesystem).
         *
         * Aside from the tetrahedron gluings, all other SnapPea-specific
         * information (such as peripheral curves, and the manifold name) will
         * be ignored, since Regina's Triangulation<3> class does not track
         * such information itself.
         *
         * If you wish to preserve all SnapPea-specific information from the
         * data file, you should work with the SnapPeaTriangulation class
         * instead (which uses the SnapPea kernel directly, and can therefore
         * store anything that SnapPea can).
         *
         * One reason for working with this function as opposed to using
         * SnapPeaTriangulation is if you need to preserve the specific
         * triangulation.  For example, if the SnapPea data file describes a
         * closed triangulation (where all vertices are finite), then the
         * SnapPea kernel will convert this into an ideal triangulation with
         * filling coefficients, whereas this routine will return the original
         * closed triangulation.
         *
         * \warning This routine is "lossy", in that drops SnapPea-specific
         * information (as described above).  Unless you specifically need an
         * Triangulation<3> (not an SnapPeaTriangulation) or you need to avoid
         * calling routines from the SnapPea kernel, it is highly recommended
         * that you create a SnapPeaTriangulation from the given file
         * contents instead.  See the string-based SnapPeaTriangulation
         * constructor for how to do this.
         *
         * \warning If (for some reason) you pass a filename that begins
         * with "% Triangulation", then Regina will interpret this as
         * the contents of a SnapPea file (not a filename).
         *
         * \i18n If the given argument is a filename, then this routine makes
         * no assumptions about the \ref i18n "character encoding" used in the
         * filename, and simply passes it through unchanged to low-level C/C++
         * file I/O routines.  This routine assumes that the file _contents_,
         * however, are in UTF-8 (the standard encoding used throughout Regina).
         *
         * \exception InvalidArgument The given string does not provide either
         * the filename or contents of a correctly formatted SnapPea data file.
         * \exception FileError An error occurred with file I/O (such as
         * testing whether the given file exists, or reading its contents).
         *
         * \param filenameOrContents either the name of a SnapPea data
         * file, or the full contents of a SnapPea data file (which need not
         * actually exist on the filesystem).
         * \return a native Regina triangulation extracted from the given
         * SnapPea data.
         */
        static Triangulation<3> fromSnapPea(
            const std::string& filenameOrContents);

        /*@}*/

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a change event.
         *
         * It is recommended that you use a local ChangeAndClearSpan object
         * to manage both of these tasks (calling clearAllProperties() and
         * firing change events), rather than calling this function manually.
         */
        void clearAllProperties();

        /**
         * Checks that the permutations on face gluings are valid and
         * that adjacent face gluings match.  That is, if face \a A of
         * tetrahedron \a X is glued to face \a B of tetrahedron \a Y,
         * then face \a B of tetrahedron \a Y is also glued to face \a A
         * of tetrahedron \a X using the inverse permutation.
         *
         * If any of these tests fail, this routine writes a message to
         * standard error and marks this triangulations as invalid.
         * Any such failure indicates a likely programming error, since
         * adjacent face gluings should always match if the Tetrahedron<3>
         * gluing routines have been used correctly.
         *
         * \author Matthias Goerner
         */
        void checkPermutations();

        void calculateSkeleton();
        void cloneSkeleton(const Triangulation& src);

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertexLinks();

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateBoundaryProperties() const;

        /**
         * Implements testing for and/or performing 4-4 moves.
         * See move44() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any simplex and/or facet locks.
         *
         * \exception LockViolation This move would violate a simplex or facet
         * lock, and \a check was passed as \c false.  This exception will be
         * thrown before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internal44(Edge<3>* e, int axis, bool check, bool perform);
        /**
         * Implements testing for and/or performing 2-1 moves.
         * See move21() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any simplex and/or facet locks.
         *
         * \exception LockViolation This move would violate a simplex or facet
         * lock, and \a check was passed as \c false.  This exception will be
         * thrown before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internal21(Edge<3>* e, int edgeEnd, bool check, bool perform);
        /**
         * Implements testing for and/or performing 2-0 moves.
         * See the various forms of move02() for details on what the location
         * arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         *
         * \author Alex He
         */
        bool internal02(EdgeEmbedding<3> e0, int t0,
            EdgeEmbedding<3> e1, int t1, bool check, bool perform);
        /**
         * Implements testing for and/or performing 2-0 moves.
         * See the various forms of move02() for details on what the location
         * arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         *
         * \author Alex He
         */
        bool internal02(Edge<3>* e, size_t t0, size_t t1,
            bool check, bool perform);
        /**
         * Implements testing for and/or performing 2-0 moves.
         * See the various forms of move02() for details on what the location
         * arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         *
         * \author Alex He
         */
        bool internal02(Triangle<3>* t0, int e0, Triangle<3>* t1, int e1,
            bool check, bool perform);
        /**
         * Implements testing for and/or performing book opening moves.
         * See openBook() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalOpenBook(Triangle<3>* t, bool check, bool perform);
        /**
         * Implements testing for and/or performing book closing moves.
         * See closeBook() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any facet locks.
         *
         * \exception LockViolation This move would violate a facet lock, and
         * \a check was passed as \c false.  This exception will be thrown
         * before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalCloseBook(Edge<3>* e, bool check, bool perform);
        /**
         * Implements testing for and/or performing edge collapse moves.
         * See collapseEdge() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then it must
         * be known in advance that the requested move is legal and will not
         * violate any simplex and/or facet locks.
         *
         * \exception LockViolation This move would violate a simplex or facet
         * lock, and \a check was passed as \c false.  This exception will be
         * thrown before any changes are made.
         *
         * \param check indicates whether we should check whether the move is
         * legal and will not violate any locks.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalCollapseEdge(Edge<3>* e, bool check, bool perform);

        void stretchBoundaryForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInBoundary(). */
        bool stretchForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&, std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInSkeleton(). */

        /**
         * Called before changing the Regina data for a SnapPea triangulation.
         *
         * The functions snapPeaPreChange() and snapPeaPostChange() together
         * manage change events on the SnapPea triangulation packet (if there
         * is one), as well as the nullification of the SnapPea triangulation.
         *
         * \pre This Regina triangulation is in fact the inherited
         * Triangulation<3> data for the derived class SnapPeaTriangulation.
         */
        void snapPeaPreChange();

        /**
         * Called after changing the Regina data for a SnapPea triangulation.
         *
         * The functions snapPeaPreChange() and snapPeaPostChange() together
         * manage change events on the SnapPea triangulation packet (if there
         * is one), as well as the nullification of the SnapPea triangulation.
         *
         * \pre This Regina triangulation is in fact the inherited
         * Triangulation<3> data for the derived class SnapPeaTriangulation.
         */
        void snapPeaPostChange();

        /**
         * Implements fromSnapPea() where the contents of the SnapPea data
         * file are to be read from the given input stream.
         *
         * See fromSnapPea(const std::string&) for further details.
         *
         * \exception InvalidArgument The given input stream does not contain
         * the contents of a correctly formatted SnapPea data file.
         * \exception FileError An error occurred in attempting to read data
         * from the given input stream.
         *
         * \param in an input stream containing the full contents of a SnapPea
         * data file.
         * \return a native Regina triangulation extracted from the given
         * SnapPea data.
         */
        static Triangulation<3> fromSnapPea(std::istream& in);

    friend class regina::Face<3, 3>;
    friend class regina::detail::SimplexBase<3>;
    friend class regina::detail::TriangulationBase<3>;
    friend class PacketData<Triangulation<3>>;
    friend class regina::XMLTriangulationReader<3>;
    friend class regina::XMLWriter<Triangulation<3>>;
};

/**
 * Casts a reference from Packet to Triangulation<3>, allowing for
 * the packet to hold either a Triangulation<3> or a SnapPeaTriangulation.
 *
 * The behaviour of this routine is analogous to
 * static_cast<Triangulation<3&>>() and
 * regina::static_packet_cast<Triangulation<3>>().  It is provided because
 * these other routines cannot simultaneously support packets that hold a
 * Triangulation<3> and packets that hold a SnapPeaTriangulation - these
 * two cases use separate (and unrelated) paths through the class hierarchy to
 * get from Packet to Triangulation<3>.
 *
 * In particular, attempting to use regina::static_packet_cast<Triangulation<3>>
 * with a packet holding a SnapPea triangulation is not allowed, and will result
 * in undefined behaviour.  In contrast, calling
 * regina::static_triangulation3_cast on such a packet is allowed and
 * will return the expected Triangulation<3> reference.
 *
 * \nopython Casting is unnecessary in Python.
 *
 * \param p a reference, presented as a packet.
 * \return the same reference, presented using the type \a Held.
 *
 * \ingroup dim3
 */
Triangulation<3>& static_triangulation3_cast(Packet& p);

/**
 * Casts a const reference from Packet to Triangulation<3>, allowing for
 * the packet to hold either a Triangulation<3> or a SnapPeaTriangulation.
 *
 * The behaviour of this routine is analogous to
 * static_cast<const Triangulation<3>&>() and
 * regina::static_packet_cast<Triangulation<3>>().  It is provided because
 * these other routines cannot simultaneously support packets that hold a
 * Triangulation<3> and packets that hold a SnapPeaTriangulation - these
 * two cases use separate (and unrelated) paths through the class hierarchy to
 * get from Packet to Triangulation<3>.
 *
 * In particular, attempting to use regina::static_packet_cast<Triangulation<3>>
 * with a packet holding a SnapPea triangulation is not allowed, and will result
 * in undefined behaviour.  In contrast, calling
 * regina::static_triangulation3_cast on such a packet is allowed and
 * will return the expected Triangulation<3> reference.
 *
 * \nopython Casting is unnecessary in Python.
 *
 * \param p a reference, presented as a packet.
 * \return the same reference, presented using the type \a Held.
 *
 * \ingroup dim3
 */
const Triangulation<3>& static_triangulation3_cast(const Packet& p);

// Doxygen struggles with specialisations; hide them from it.
#ifndef __DOXYGEN
template <>
inline PacketData<Triangulation<3>>::PacketChangeSpan::PacketChangeSpan(
        PacketData& data) : data_(data) {
    switch (data_.heldBy_) {
        case PacketHeldBy::SnapPea: {
            static_cast<Triangulation<3>&>(data_).snapPeaPreChange();
            break;
        }
        case PacketHeldBy::Packet: {
            auto& p = static_cast<PacketOf<Triangulation<3>>&>(data_);
            if (! p.packetChangeSpans_)
                p.fireEvent(&PacketListener::packetToBeChanged);
            ++p.packetChangeSpans_;
            break;
        }
        default:
            break;
    }
}

template <>
inline PacketData<Triangulation<3>>::PacketChangeSpan::~PacketChangeSpan() {
    switch (data_.heldBy_) {
        case PacketHeldBy::SnapPea: {
            static_cast<Triangulation<3>&>(data_).snapPeaPostChange();
            break;
        }
        case PacketHeldBy::Packet: {
            auto& p = static_cast<PacketOf<Triangulation<3>>&>(data_);
            --p.packetChangeSpans_;
            if (! p.packetChangeSpans_)
                p.fireEvent(&PacketListener::packetWasChanged);
            break;
        }
        default:
            break;
    }
}
#endif // ! __DOXYGEN

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline Triangulation<3>::~Triangulation() {
    // There is some difficulty over what to do with properties that
    // hold normal surfaces and/or angle structures, since such properties
    // will hold a snapshot reference to this triangulation.
    //
    // If these are the *only* snapshot references to the triangulation,
    // then we should clear the properties now, so that takeSnapshot()
    // does not trigger a deep copy of the triangulation which is then
    // immediately destroyed.
    //
    // However, if there are *other* snapshot references to the triangulation,
    // then clearing the properties now will mean they do not get cloned
    // with the deep copy, losing the benefit of having cached them.
    //
    // For now, we take the following approach: currently the only cached
    // properties of this type are polynomial-time to compute, and so we clear
    // them now and let the deep copy recompute them again later if needed.

    if (std::holds_alternative<AngleStructure>(strictAngleStructure_))
        strictAngleStructure_ = false;
    if (std::holds_alternative<AngleStructure>(generalAngleStructure_))
        generalAngleStructure_ = false;

    Snapshottable<Triangulation<3>>::takeSnapshot();
    clearAllProperties();
}

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim3/tetrahedron3.h"
#include "triangulation/dim3/triangle3.h"
#include "triangulation/dim3/edge3.h"
#include "triangulation/dim3/vertex3.h"
#include "triangulation/dim3/component3.h"
namespace regina {

// Inline functions for Triangulation<3>

inline Triangulation<3>::Triangulation(const Triangulation& src) :
        Triangulation<3>(src, true, true) {
}

inline Tetrahedron<3>* Triangulation<3>::newTetrahedron() {
    return newSimplex();
}

inline Tetrahedron<3>* Triangulation<3>::newTetrahedron(const std::string& desc) {
    return newSimplex(desc);
}

template <int k>
inline std::array<Tetrahedron<3>*, k> Triangulation<3>::newTetrahedra() {
    return newSimplices<k>();
}

inline void Triangulation<3>::newTetrahedra(size_t k) {
    newSimplices(k);
}

inline void Triangulation<3>::removeTetrahedronAt(size_t index) {
    removeSimplexAt(index);
}

inline void Triangulation<3>::removeTetrahedron(Tetrahedron<3>* tet) {
    removeSimplex(tet);
}

inline void Triangulation<3>::removeAllTetrahedra() {
    removeAllSimplices();
}

inline Triangulation<3>& Triangulation<3>::operator = (
        const Triangulation& src) {
    // We need to implement copy assignment ourselves because it all
    // needs to be wrapped in a PacketChangeSpan.  This is so that the
    // final packetWasChanged event is fired *after* we modify the
    // properties specific to dimension 3.
    //
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to clone them, not clear them.
    PacketChangeSpan span(*this);

    TriangulationBase<3>::operator = (src);

    ideal_ = src.ideal_;
    standard_ = src.standard_;
    prop_ = src.prop_;

    // Any cached angle structures must be remade to live in this triangulation.
    if (std::holds_alternative<AngleStructure>(src.strictAngleStructure_))
        strictAngleStructure_ = AngleStructure(
            std::get<AngleStructure>(src.strictAngleStructure_), *this);
    else
        strictAngleStructure_ = std::get<bool>(src.strictAngleStructure_);
    if (std::holds_alternative<AngleStructure>(src.generalAngleStructure_))
        generalAngleStructure_ = AngleStructure(
            std::get<AngleStructure>(src.generalAngleStructure_), *this);
    else
        generalAngleStructure_ = std::get<bool>(src.generalAngleStructure_);

    return *this;
}

inline Triangulation<3>& Triangulation<3>::operator = (Triangulation&& src) {
    // Like copy assignment, we implement this ourselves because it all
    // needs to be wrapped in a PacketChangeSpan.
    //
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to move them, not clear them.
    PacketChangeSpan span(*this);

    // The parent class assignment goes last, since its move invalidates src.

    ideal_ = src.ideal_;
    standard_ = src.standard_;
    prop_ = std::move(src.prop_);

    strictAngleStructure_ = std::move(src.strictAngleStructure_);
    generalAngleStructure_ = std::move(src.generalAngleStructure_);

    TriangulationBase<3>::operator = (std::move(src));

    return *this;
}

inline bool Triangulation<3>::hasBoundaryTriangles() const {
    return hasBoundaryFacets();
}

inline size_t Triangulation<3>::countBoundaryTriangles() const {
    return countBoundaryFacets();
}

inline bool Triangulation<3>::hasTwoSphereBoundaryComponents() const {
    if (! prop_.twoSphereBoundaryComponents_.has_value())
        calculateBoundaryProperties();
    return *prop_.twoSphereBoundaryComponents_;
}

inline bool Triangulation<3>::hasNegativeIdealBoundaryComponents() const {
    if (! prop_.negativeIdealBoundaryComponents_.has_value())
        calculateBoundaryProperties();
    return *prop_.negativeIdealBoundaryComponents_;
}

inline bool Triangulation<3>::isIdeal() const {
    ensureSkeleton();
    return ideal_;
}

inline bool Triangulation<3>::isStandard() const {
    ensureSkeleton();
    return standard_;
}

inline bool Triangulation<3>::isClosed() const {
    ensureSkeleton();
    return boundaryComponents().empty();
}

inline void Triangulation<3>::reorderTetrahedraBFS(bool reverse) {
    reorderBFS(reverse);
}

inline bool Triangulation<3>::knowsZeroEfficient() const {
    return prop_.zeroEfficient_.has_value();
}

inline bool Triangulation<3>::knowsOneEfficient() const {
    // Check the preconditions before examining the cached value, since it's
    // possible the 1-efficiency value was cached from a newer calculation
    // engine that supports 1-efficiency testing in more settings.
    if (! isValid()) {
        throw FailedPrecondition(
            "1-efficiency testing requires a valid triangulation");
    }
    if (! isIdeal()) {
        // The empty triangulation is eliminated here.
        throw FailedPrecondition(
            "1-efficiency testing requires an ideal triangulation");
    }
    for (auto v : vertices()) {
        if (v->linkType() != Vertex<3>::Link::Torus &&
                v->linkType() != Vertex<3>::Link::KleinBottle)
            throw FailedPrecondition(
                "1-efficiency testing requires a triangulation whose "
                "vertex links are all tori and/or Klein bottles");
    }

    if (prop_.oneEfficient_.has_value())
        return true;

    // We might already know the answer from the 0-efficiency property,
    // since for the settings in which we are able to test 1-efficiency,
    // 0-efficiency is a necessary condition.
    if (prop_.zeroEfficient_.has_value() && ! *prop_.zeroEfficient_) {
        prop_.oneEfficient_ = false;
        return true;
    }

    // We don't know.
    return false;
}

inline const AngleStructure& Triangulation<3>::strictAngleStructure() const {
    if (hasStrictAngleStructure())
        return std::get<AngleStructure>(strictAngleStructure_);
    else
        throw NoSolution();
}

inline const AngleStructure& Triangulation<3>::generalAngleStructure() const {
    // Optimise for the common case where a solution is known to exist,
    // so we can inline it.
    if (std::holds_alternative<AngleStructure>(generalAngleStructure_))
        return std::get<AngleStructure>(generalAngleStructure_);

    // Either there is no solution or we don't know yet.
    if (hasGeneralAngleStructure())
        return std::get<AngleStructure>(generalAngleStructure_);
    else
        throw NoSolution();
}

inline unsigned long Triangulation<3>::homologyH2Z2() const {
    // The call to homologyRel() will test the validity precondition.
    const AbelianGroup& h1Rel = homologyRel();
    return h1Rel.rank() + h1Rel.torsionRank(2);
}

inline const Triangulation<3>::TuraevViroSet&
        Triangulation<3>::allCalculatedTuraevViro() const {
    return prop_.turaevViroCache_;
}

inline bool Triangulation<3>::intelligentSimplify() {
    return simplify();
}

template <typename Action, typename... Args>
inline bool Triangulation<3>::retriangulate(int height, unsigned threads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    if (countComponents() > 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "retriangulate() requires a connected triangulation");
    }

    // Use RetriangulateActionTraits to deduce whether the given action
    // takes a triangulation or both an isomorphism signature and triangulation
    // as its initial argument(s).
    using Traits =
        regina::detail::RetriangulateActionTraits<Triangulation<3>, Action>;
    static_assert(Traits::valid,
        "The action that is passed to retriangulate() does not take the correct initial argument type(s).");
    if constexpr (Traits::withSig) {
        return regina::detail::retriangulateInternal<Triangulation<3>, true>(
            *this, height, threads, tracker,
            [&](const std::string& sig, Triangulation<3>&& obj) {
                return action(sig, std::move(obj), std::forward<Args>(args)...);
            });
    } else {
        return regina::detail::retriangulateInternal<Triangulation<3>, false>(
            *this, height, threads, tracker,
            [&](Triangulation<3>&& obj) {
                return action(std::move(obj), std::forward<Args>(args)...);
            });
    }
}

inline bool Triangulation<3>::minimizeBoundary() {
    return minimiseBoundary();
}

inline bool Triangulation<3>::minimizeVertices() {
    return minimiseVertices();
}

inline bool Triangulation<3>::move44(Edge<3>* e, int axis) {
    return internal44(e, axis, true, true);
}

inline bool Triangulation<3>::move21(Edge<3>* e, int edgeEnd) {
    return internal21(e, edgeEnd, true, true);
}

inline bool Triangulation<3>::move02(EdgeEmbedding<3> e0, int t0,
        EdgeEmbedding<3> e1, int t1) {
    return internal02(e0, t0, e1, t1, true, true);
}

inline bool Triangulation<3>::move02(Edge<3>* e, size_t t0, size_t t1) {
    return internal02(e, t0, t1, true, true);
}

inline bool Triangulation<3>::move02(Triangle<3>* t0, int e0,
        Triangle<3>* t1, int e1) {
    return internal02(t0, e0, t1, e1, true, true);
}

inline bool Triangulation<3>::openBook(Triangle<3>* t) {
    return internalOpenBook(t, true, true);
}

inline bool Triangulation<3>::closeBook(Edge<3>* e) {
    return internalCloseBook(e, true, true);
}

inline bool Triangulation<3>::collapseEdge(Edge<3>* e) {
    return internalCollapseEdge(e, true, true);
}

inline bool Triangulation<3>::has44(Edge<3>* e, int axis) const {
    return const_cast<Triangulation<3>*>(this)->internal44(e, axis,
        true, false);
}

inline bool Triangulation<3>::has21(Edge<3>* e, int edgeEnd) const {
    return const_cast<Triangulation<3>*>(this)->internal21(e, edgeEnd,
        true, false);
}

inline bool Triangulation<3>::has02(EdgeEmbedding<3> e0, int t0,
        EdgeEmbedding<3> e1, int t1) const {
    return const_cast<Triangulation<3>*>(this)->internal02(e0, t0, e1, t1,
        true, false);
}

inline bool Triangulation<3>::has02(Edge<3>* e, size_t t0, size_t t1) const {
    return const_cast<Triangulation<3>*>(this)->internal02(e, t0, t1,
        true, false);
}

inline bool Triangulation<3>::has02(Triangle<3>* t0, int e0,
        Triangle<3>* t1, int e1) const {
    return const_cast<Triangulation<3>*>(this)->internal02(t0, e0, t1, e1,
        true, false);
}

inline bool Triangulation<3>::hasOpenBook(Triangle<3>* t) const {
    return const_cast<Triangulation<3>*>(this)->internalOpenBook(t,
        true, false);
}

inline bool Triangulation<3>::hasCloseBook(Edge<3>* e) const {
    return const_cast<Triangulation<3>*>(this)->internalCloseBook(e,
        true, false);
}

inline bool Triangulation<3>::hasCollapseEdge(Edge<3>* e) const {
    return const_cast<Triangulation<3>*>(this)->internalCollapseEdge(e,
        true, false);
}

inline std::optional<Triangulation<3>> Triangulation<3>::with44(
        Edge<3>* e, int axis) const {
    if (! has44(e, axis))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internal44(ans->translate(e), axis, false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::with21(
        Edge<3>* e, int edgeEnd) const {
    if (! has21(e, edgeEnd))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internal21(ans->translate(e), edgeEnd, false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::with02(
        EdgeEmbedding<3> e0, int t0, EdgeEmbedding<3> e1, int t1) const {
    if (! has02(e0, t0, e1, t1))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internal02(ans->translate(e0), t0, ans->translate(e1), t1,
        false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::with02(
        Edge<3>* e, size_t t0, size_t t1) const {
    if (! has02(e, t0, t1))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internal02(ans->translate(e), t0, t1, false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::with02(
        Triangle<3>* t0, int e0, Triangle<3>* t1, int e1) const {
    if (! has02(t0, e0, t1, e1))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internal02(ans->translate(t0), e0, ans->translate(t1), e1,
        false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::withOpenBook(
        Triangle<3>* t) const {
    if (! hasOpenBook(t))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internalOpenBook(ans->translate(t), false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::withCloseBook(
        Edge<3>* e) const {
    if (! hasCloseBook(e))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internalCloseBook(ans->translate(e), false, true);
    return ans;
}

inline std::optional<Triangulation<3>> Triangulation<3>::withCollapseEdge(
        Edge<3>* e) const {
    if (! hasCollapseEdge(e))
        return {};

    std::optional<Triangulation<3>> ans(std::in_place, *this);
    ans->internalCollapseEdge(ans->translate(e), false, true);
    return ans;
}

inline bool Triangulation<3>::fourFourMove(Edge<3>* e, int axis,
        bool, bool perform) {
    return internal44(e, axis, true, perform);
}

inline bool Triangulation<3>::twoOneMove(Edge<3>* e, int edgeEnd,
        bool, bool perform) {
    return internal21(e, edgeEnd, true, perform);
}

inline bool Triangulation<3>::zeroTwoMove(EdgeEmbedding<3> e0, int t0,
        EdgeEmbedding<3> e1, int t1, bool, bool perform) {
    return internal02(e0, t0, e1, t1, true, perform);
}

inline bool Triangulation<3>::zeroTwoMove(Edge<3>* e, size_t t0, size_t t1,
        bool, bool perform) {
    return internal02(e, t0, t1, true, perform);
}

inline bool Triangulation<3>::zeroTwoMove(Triangle<3>* t0, int e0,
        Triangle<3>* t1, int e1, bool, bool perform) {
    return internal02(t0, e0, t1, e1, true, perform);
}

inline bool Triangulation<3>::openBook(Triangle<3>* t, bool, bool perform) {
    return internalOpenBook(t, true, perform);
}

inline bool Triangulation<3>::closeBook(Edge<3>* e, bool, bool perform) {
    return internalCloseBook(e, true, perform);
}

inline bool Triangulation<3>::collapseEdge(Edge<3>* e, bool, bool perform) {
    return internalCollapseEdge(e, true, perform);
}

inline void Triangulation<3>::puncture(Tetrahedron<3>* tet) {
    if (tet)
        puncture(tet->triangle(0));
    else
        puncture(); // use the default location
}

inline const TreeDecomposition& Triangulation<3>::niceTreeDecomposition()
        const {
    if (prop_.niceTreeDecomposition_)
        return *prop_.niceTreeDecomposition_;

    TreeDecomposition ans(*this, TreeDecompositionAlg::Upper);
    ans.makeNice();
    prop_.niceTreeDecomposition_ = ans;

    return *prop_.niceTreeDecomposition_;
}

inline void Triangulation<3>::recognizer(std::ostream& out) const {
    recogniser(out);
}

inline bool Triangulation<3>::saveRecognizer(const char* filename) const {
    return saveRecogniser(filename);
}

} // namespace regina

#endif

