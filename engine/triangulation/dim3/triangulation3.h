
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
#include "surfaces/normalsurface.h"
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

/**
 * \defgroup dim3 3-Manifold Triangulations
 * Details for implementing triangulations of 3-manifolds.
 */

/**
 * Represents a 3-dimensional triangulation, typically of a 3-manifold.
 *
 * This is a specialisation of the generic Triangulation class template;
 * see the Triangulation documentation for a general overview of how
 * the triangulation classes work.
 *
 * This 3-dimensional specialisation offers significant extra functionality,
 * including many functions specific to 3-manifolds.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \feature Is the boundary incompressible?
 * \todo \featurelong Am I obviously a handlebody?  (Simplify and see
 * if there is nothing left).  Am I obviously not a handlebody?
 * (Compare homology with boundary homology).
 * \todo \featurelong Is the triangulation Haken?
 * \todo \featurelong What is the Heegaard genus?
 * \todo \featurelong Have a subcomplex as a new type.  Include routines to
 * crush a subcomplex or to expand a subcomplex to a normal surface.
 * \todo \featurelong Implement writeTextLong() for skeletal objects.
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Triangulation<3> : public detail::TriangulationBase<3> {
    public:
        using TetrahedronIterator =
                std::vector<Tetrahedron<3>*>::const_iterator;
            /**< A dimension-specific alias for SimplexIterator,
                 used to iterate through tetrahedra. */
        using TriangleIterator =
                decltype(detail::TriangulationBase<3>().faces<2>().begin());
            /**< Used to iterate through triangles. */
        using EdgeIterator =
                decltype(detail::TriangulationBase<3>().faces<1>().begin());
            /**< Used to iterate through edges. */
        using VertexIterator =
                decltype(detail::TriangulationBase<3>().faces<0>().begin());
            /**< Used to iterate through vertices. */

        using TuraevViroSet =
                std::map<std::pair<unsigned long, bool>, Cyclotomic>;
            /**< A map from (\a r, \a parity) pairs to Turaev-Viro invariants,
                 as described by turaevViro(). */

    private:
        bool ideal_;
            /**< Is the triangulation ideal? */
        bool standard_;
            /**< Is the triangulation standard? */

        mutable std::optional<AbelianGroup> H1Rel_;
            /**< Relative first homology group of the triangulation
                 with respect to the boundary.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<AbelianGroup> H1Bdry_;
            /**< First homology group of the boundary.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<AbelianGroup> H2_;
            /**< Second homology group of the triangulation.
                 This is std::nullopt if it has not yet been computed. */

        mutable std::optional<bool> twoSphereBoundaryComponents_;
            /**< Does the triangulation contain any 2-sphere boundary
                 components?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> negativeIdealBoundaryComponents_;
            /**< Does the triangulation contain any boundary components
                 that are ideal and have negative Euler characteristic?
                 This is std::nullopt if it has not yet been computed. */

        mutable std::optional<bool> zeroEfficient_;
            /**< Is the triangulation zero-efficient?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> splittingSurface_;
            /**< Does the triangulation have a normal splitting surface?
                 This is std::nullopt if it has not yet been computed. */

        mutable std::optional<bool> threeSphere_;
            /**< Is this a triangulation of a 3-sphere?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> threeBall_;
            /**< Is this a triangulation of a 3-dimensional ball?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> solidTorus_;
            /**< Is this a triangulation of the solid torus?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> TxI_;
            /**< Is this a triangulation of the product TxI?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> irreducible_;
            /**< Is this 3-manifold irreducible?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> compressingDisc_;
            /**< Does this 3-manifold contain a compressing disc?
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<bool> haken_;
            /**< Is this 3-manifold Haken?
                 This property must only be stored for triangulations
                 that are known to represent closed, connected,
                 orientable, irreducible 3-manifolds.
                 This is std::nullopt if it has not yet been computed. */

        mutable std::variant<bool, AngleStructure> strictAngleStructure_;
            /**< A strict angle structure on this triangulation, or a
                 boolean if we do not have one.  The boolean will be \c false
                 if the computation has not yet been attempted, or \c true if
                 it is confirmed that no such angle structure exists. */

        mutable std::variant<bool, AngleStructure> generalAngleStructure_;
            /**< A generalised angle structure on this triangulation, or a
                 boolean if we do not have one.  The boolean will be \c false
                 if the computation has not yet been attempted, or \c true if
                 it is confirmed that no such angle structure exists. */

        mutable std::optional<TreeDecomposition> niceTreeDecomposition_;
            /**< A nice tree decomposition of the face pairing graph of
                 this triangulation.
                 This is std::nullopt if it has not yet been computed. */

        mutable TuraevViroSet turaevViroCache_;
            /**< The set of Turaev-Viro invariants that have already
                 been calculated.  See allCalculatedTuraevViro() for
                 details. */

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
        Triangulation();
        /**
         * Creates a new copy of the given triangulation.
         *
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation<3>& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * @param copy the triangulation to copy.
         * @param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
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
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this triangulation
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that \e will fire a packet destruction event).
         *
         * @param src the triangulation to move.
         */
        Triangulation(Triangulation&& src) noexcept = default;
        /**
         * Deprecated routine that creates a new ideal triangulation
         * representing the complement of the given link in the 3-sphere.
         *
         * This is the same triangulation that is produced by
         * Link::complement(); however, this routine is slightly less
         * efficient because it induces an additional move.
         * See Link::complemenet() for further details.
         *
         * The triangulation will be simplified, but be aware that it
         * might still contain internal vertices (i.e., vertices whose
         * links are spheres).  This should, however, be a rare occurrence.
         *
         * \deprecated Just call the more efficient Link::complement() instead.
         *
         * @param link the link whose complement we should build.
         */
        [[deprecated]] Triangulation(const Link& link);
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a triangulation.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - isomorphism signatures (see fromIsoSig());
         * - dehydration strings (see rehydrate());
         * - the contents of a SnapPea data file (see fromSnapPea()).
         *
         * This list may grow in future versions of Regina.
         *
         * If Regina cannot interpret the given string, this will be
         * left as the empty triangulation.
         *
         * \warning If you pass the contents of a SnapPea data file,
         * then only the tetrahedron gluings will be read; all other
         * SnapPea-specific information (such as peripheral curves) will
         * be lost.  See fromSnapPea() for details, and for other
         * alternatives that preserve SnapPea-specific data.
         *
         * @param description a string that describes a 3-manifold
         * triangulation.
         */
        Triangulation(const std::string& description);
#ifdef __DOXYGEN
        /**
         * Python-only constructor that copies the given SnapPy manifold.
         *
         * \warning Only the tetrahedron gluings will be copied; all other
         * SnapPy-specific information (such as peripheral curves) will
         * be lost.  See fromSnapPea() for details, and for other
         * alternatives that preserve SnapPy-specific data.
         *
         * \ifacescpp Not present.
         *
         * @param m a SnapPy object of type snappy.Manifold.
         */
        Triangulation(snappy.Manifold m);
        /**
         * Python-only constructor that copies the given SnapPy triangulation.
         *
         * \warning Only the tetrahedron gluings will be copied; all other
         * SnapPy-specific information (such as peripheral curves) will
         * be lost.  See fromSnapPea() for details, and for other
         * alternatives that preserve SnapPy-specific data.
         *
         * \ifacescpp Not present.
         *
         * @param t a SnapPy object of type snappy.Triangulation.
         */
        Triangulation(snappy.Triangulation t);
#endif
        /**
         * Destroys this triangulation.
         *
         * The constituent tetrahedra, the cellular structure and all other
         * properties will also be destroyed.
         */
        ~Triangulation();

        using Snapshottable<Triangulation<3>>::isReadOnlySnapshot;

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
         * @return the packet that holds this data (directly or indirectly),
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
         * @return the packet that holds this data (directly or indirectly),
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
         * @return the SnapPea triangulation that holds this data, or
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
         * @return the SnapPea triangulation that holds this data, or
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
         * A dimension-specific alias for removeSimplex().
         *
         * See removeSimplex() for further information.
         */
        void removeTetrahedron(Tetrahedron<3>* tet);
        /**
         * A dimension-specific alias for removeSimplexAt().
         *
         * See removeSimplexAt() for further information.
         */
        void removeTetrahedronAt(size_t index);
        /**
         * A dimension-specific alias for removeAllSimplices().
         *
         * See removeAllSimplices() for further information.
         */
        void removeAllTetrahedra();

        /**
         * Sets this to be a (deep) copy of the given triangulation.
         *
         * @return a reference to this triangulation.
         */
        Triangulation& operator = (const Triangulation&) = default;

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
         * The triangulation that is passed (\a src) will no longer be usable.
         *
         * \note This operator is \e not marked \c noexcept, since it fires
         * change events on this triangulation which may in turn call arbitrary
         * code via any registered packet listeners.  It deliberately does
         * \e not fire change events on \a src, since it assumes that \a src is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * @param src the triangulation to move.
         * @return a reference to this triangulation.
         */
        Triangulation& operator = (Triangulation&& src) = default;

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
         * \note This swap function is \e not marked \c noexcept, since it
         * fires change events on both triangulations which may in turn call
         * arbitrary code via any registered packet listeners.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<3>& other);
        /**
         * Deprecated routine that swaps the contents of this and the
         * given triangulation.
         *
         * \deprecated Use swap() instead.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        [[deprecated]] void swapContents(Triangulation<3>& other);

        /*@}*/
        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * Determines if this triangulation contains any two-sphere
         * boundary components.
         *
         * @return \c true if and only if there is at least one
         * two-sphere boundary component.
         */
        bool hasTwoSphereBoundaryComponents() const;
        /**
         * Determines if this triangulation contains any ideal boundary
         * components with negative Euler characteristic.
         *
         * @return \c true if and only if there is at least one such
         * boundary component.
         */
        bool hasNegativeIdealBoundaryComponents() const;

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Returns the Euler characteristic of the corresponding compact
         * 3-manifold.
         *
         * Instead of simply calculating \a V-E+F-T, this routine also:
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
         * @return the Euler characteristic of the corresponding compact
         * manifold.
         */
        long eulerCharManifold() const;

        /**
         * Determines if this triangulation is ideal.
         * This is the case if and only if one of the vertex links
         * is closed and not a 2-sphere.
         * Note that the triangulation is not required to be valid.
         *
         * @return \c true if and only if this triangulation is ideal.
         */
        bool isIdeal() const;
        /**
         * Determines if this triangulation is standard.
         * This is the case if and only if every vertex is standard.
         * See Vertex<3>::isStandard() for further details.
         *
         * @return \c true if and only if this triangulation is
         * standard.
         */
        bool isStandard() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal triangulations are not closed.
         *
         * @return \c true if and only if this triangulation is closed.
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
         * @return \c true if and only if all gluing permutations are
         * order preserving on the tetrahedron faces.
         *
         * @author Matthias Goerner
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
         * @return the relative first homology group with respect to the
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
         * @return the first homology group of the boundary.
         */
        const AbelianGroup& homologyBdry() const;
        /**
         * Returns the second homology group for this triangulation.
         * If this triangulation contains any ideal vertices,
         * the homology group will be
         * calculated as if each such vertex had been truncated.
         * The algorithm used calculates various first homology groups
         * and uses homology and cohomology theorems to deduce the
         * second homology group.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homologyH2() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the second homology group.
         */
        const AbelianGroup& homologyH2() const;
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
         * @return the number of Z_2 terms in the second homology group
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
         * as determined by an integer r >= 3, and a root of unity q0 of
         * degree 2r for which q0^2 is a primitive root of unity of
         * degree r.  There are several cases to consider:
         *
         * - \a r may be even.  In this case \a q0 must be a primitive
         *   (<i>2r</i>)th root of unity, and the invariant is computed as an
         *   element of the cyclotomic field of order \a 2r.  There is no need
         *   to specify \e which root of unity is used, since switching between
         *   different roots of unity corresponds to an automorphism of
         *   the underlying cyclotomic field (i.e., it does not yield
         *   any new information).  Therefore, if \a r is even, the
         *   additional argument \a parity is ignored.
         *
         * - \a r may be odd, and \a q0 may be a primitive (<i>2r</i>)th
         *   root of unity.  This case corresponds to passing the argument
         *   \a parity as \c true.  Here the invariant is again computed
         *   as an element of the cyclotomic field of order \a 2r.  As before,
         *   there is no need to give further information as to which
         *   root of unity is used, since switching between roots of unity
         *   does not yield new information.
         *
         * - \a r may be odd, and \a q0 may be a primitive (<i>r</i>)th
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
         * <tt>turaevViroApprox(r, whichRoot)</tt>
         * should be the same as
         * <tt>turaevViro(r, parity).evaluate(whichRoot)</tt>,
         * where \a parity is \c true or \c false according to whether
         * \a whichRoot is odd or even respectively.  Of course in practice the
         * numerical values might be very different, since turaevViroApprox()
         * performs significantly more floating point operations, and so is
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
         * @param r the integer \a r as described above; this must be at
         * least 3.
         * @param parity determines for odd \a r whether \a q0 is a primitive
         * <i>2r</i>th or <i>r</i>th root of unity, as described above.
         * @param alg the algorithm with which to compute the invariant.  If
         * you are not sure, the default value (ALG_DEFAULT) is a safe choice.
         * @param tracker a progress tracker through will progress will
         * be reported, or \c nullptr if no progress reporting is required.
         * @return the requested Turaev-Viro invariant, or an uninitialised
         * field element if the calculation was cancelled via the given
         * progress tracker.
         *
         * @see allCalculatedTuraevViro
         */
        Cyclotomic turaevViro(unsigned long r, bool parity = true,
            Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Computes the given Turaev-Viro state sum invariant of this
         * 3-manifold using a fast but inexact floating-point approximation.
         *
         * The initial data for the Turaev-Viro invariant is as described in
         * the paper of Turaev and Viro, "State sum invariants of 3-manifolds
         * and quantum 6j-symbols", Topology, vol. 31, no. 4, 1992, pp 865-902.
         * In particular, Section 7 describes the initial data as
         * determined by an integer \a r >= 3 and a root of unity \a q0 of
         * degree \a 2r for which \a q0^2 is a primitive root of unity of
         * degree \a r.
         *
         * The argument \a whichRoot specifies which root of unity is
         * used for \a q0.  Specifically, \a q0 will be the root of unity
         * <tt>e^(2i * Pi * whichRoot / 2r)</tt>.  There are additional
         * preconditions on \a whichRoot to ensure that \a q0^2 is a
         * \e primitive root of unity of degree \a r; see below for details.
         *
         * This same invariant can be computed by calling
         * <tt>turaevViro(r, parity).evaluate(whichRoot)</tt>,
         * where \a parity is \c true or \c false according to whether
         * \a whichRoot is odd or even respectively.
         * Calling turaevViroApprox() is significantly faster (since it avoids
         * the overhead of working in cyclotomic fields), but may also
         * lead to a much larger numerical error (since this routine might
         * perform an exponential number of floating point operations,
         * whereas the alternative only uses floating point for
         * the final call to Cyclotomic::evaluate()).
         *
         * These invariants, although computed in the complex field,
         * should all be reals.  Thus the return type is an ordinary
         * double.
         *
         * \pre This triangulation is valid, closed and non-empty.
         * \pre The argument \a whichRoot is strictly between 0 and <i>2r</i>,
         * and has no common factors with \a r.
         *
         * @param r the integer \a r as described above; this must be at
         * least 3.
         * @param whichRoot specifies which root of unity is used for \a q0,
         * as described above.
         * @param alg the algorithm with which to compute the invariant.  If
         * you are not sure, the default value (ALG_DEFAULT) is a safe choice.
         * @return the requested Turaev-Viro invariant.
         *
         * @see allCalculatedTuraevViro
         */
        double turaevViroApprox(unsigned long r, unsigned long whichRoot = 1,
            Algorithm alg = ALG_DEFAULT) const;
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
         * \ifacespython This routine returns a Python dictionary.
         * It also returns by value, not by reference (i.e., if more
         * Turaev-Viro invariants are computed later on, the dictionary
         * that was originally returned will not change as a result).
         *
         * @return the cache of all Turaev-Viro invariants that have
         * already been calculated.
         *
         * @see turaevViro
         */
        const TuraevViroSet& allCalculatedTuraevViro() const;

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
         * \e not modify the triangulation, and will simply return this
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
         * \warning If you have an \e ideal triangulation of a knot
         * complement, you \e must first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * @return the boundary edge representing the algebraic
         * longitude of the knot (after this triangulation has
         * been modified if necessary), or \c null if an error (such as
         * an integer overflow) occurred during the computation.
         */
        Edge<3>* longitude();

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
         * result, <b>it does not guarantee to terminate</b> (but if you find
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
         * by single boundary edges, then it is guaranteed that this routine
         * will \e not modify the triangulation, and will simply return
         * these two boundary edges.
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
         * \warning If you have an \e ideal triangulation of a knot
         * complement, you \e must first run idealToFinite() and then simplify
         * the resulting triangulation to have two boundary triangles.
         *
         * @return a pair (\a m, \a l), where \a m is the boundary edge
         * representing the meridian and \a l is the boundary edge representing
         * the algebraic longitude of the knot complement (after this
         * triangulation has been modified if necessary).  If an error (such as
         * an integer overflow) occurs during the computation, then this
         * routine will return (\c null, \c null).
         */
        std::pair<Edge<3>*, Edge<3>*> meridianLongitude();

        /*@}*/
        /**
         * \name Normal Surfaces and Angle Structures
         */
        /*@{*/

        /**
         * Determines if this triangulation is 0-efficient.
         * A triangulation is 0-efficient if its only normal spheres and
         * discs are vertex linking, and if it has no 2-sphere boundary
         * components.
         *
         * @return \c true if and only if this triangulation is
         * 0-efficient.
         */
        bool isZeroEfficient() const;
        /**
         * Is it already known whether or not this triangulation is
         * 0-efficient?
         * See isZeroEfficient() for further details.
         *
         * If this property is already known, future calls to
         * isZeroEfficient() will be very fast (simply returning the
         * precalculated value).
         *
         * \warning This routine does not actually tell you \e whether
         * this triangulation is 0-efficient; it merely tells you whether
         * the answer has already been computed.
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsZeroEfficient() const;
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
         * @return \c true if and only if this triangulation has a
         * normal splitting surface.
         */
        bool hasSplittingSurface() const;
        /**
         * Searches for a non-vertex-linking normal sphere or disc
         * within this triangulation.  If such a surface exists within
         * this triangulation, this routine is guaranteed to find one.
         *
         * @return a non-vertex-linking normal sphere or disc, or no value if
         * none exists.
         */
        std::optional<NormalSurface> nonTrivialSphereOrDisc() const;
        /**
         * A deprecated alias for nonTrivialSphereOrDisc(), which searches for
         * a non-vertex-linking normal sphere or disc within this triangulation.
         *
         * \deprecated This routine has been renamed to
         * nonTrivialSphereOrDisc().  See that routine for further details.
         *
         * @return a non-vertex-linking normal sphere or disc, or no value if
         * none exists.
         */
        [[deprecated]] std::optional<NormalSurface> hasNonTrivialSphereOrDisc()
            const;
        /**
         * Searches for an octagonal almost normal 2-sphere within this
         * triangulation.  If such a surface exists, this routine is
         * guaranteed to find one.
         *
         * \pre This triangulation is valid, closed, orientable, connected,
         * and 0-efficient.  These preconditions are almost certainly more
         * restrictive than they need to be, but we stay safe for now.
         *
         * @return an octagonal almost normal 2-sphere, or no value if
         * none exists.
         */
        std::optional<NormalSurface> octagonalAlmostNormalSphere() const;
        /**
         * A deprecated alias for octagonalAlmostNormalSphere(), which searches
         * for an octagonal almost normal 2-sphere within this triangulation.
         *
         * \deprecated This routine has been renamed to
         * octagonalAlmostNormalSphere().  See that routine for further details.
         *
         * \pre This triangulation is valid, closed, orientable, connected,
         * and 0-efficient.
         *
         * @return an octagonal almost normal 2-sphere, or no value if
         * none exists.
         */
        [[deprecated]] std::optional<NormalSurface>
            hasOctagonalAlmostNormalSphere() const;
        /**
         * Returns a strict angle structure on this triangulation, if one
         * exists.  Recall that a \e strict angle structure is one in which
         * every angle is strictly between 0 and &pi;.  If a strict angle
         * structure does exist, then this routine is guaranteed to return one.
         *
         * This routine is designed for scenarios where you already know
         * that a strict angle structure exists.  This means:
         *
         * - If no strict angle structure exists, this routine will throw an
         *   exception, which will incur a significant overhead.
         *
         * - If you do \e not know in advance whether a strict angle structure
         *   exists, you should call hasStrictAngleStructure() first.  If the
         *   answer is no, this will avoid the overhead of throwing and catching
         *   exceptions.  If the answer is yes, this will have the side-effect
         *   of caching the strict angle structure, which means your subsequent
         *   call to strictAngleStructure() will be essentially instantaneous.
         *
         * The underlying algorithm runs a single linear program (it does
         * \e not enumerate all vertex angle structures).  This means
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
         * \exception NoSolution no strict angle structure exists on
         * this triangulation.
         *
         * @return a strict angle structure on this triangulation, if
         * one exists.
         */
        const AngleStructure& strictAngleStructure() const;
        /**
         * A deprecated alias for strictAngleStructure(), which returns
         * a strict angle structure on this triangulation if one exists.
         *
         * \deprecated This routine has been renamed to strictAngleStructure().
         * See that routine for further details.
         *
         * \exception NoSolution no strict angle structure exists on
         * this triangulation.
         *
         * @return a strict angle structure on this triangulation, if
         * one exists.
         */
        [[deprecated]] const AngleStructure& findStrictAngleStructure() const;
        /**
         * Determines whether this triangulation supports a strict angle
         * structure.  Recall that a \e strict angle structure is one
         * in which every angle is strictly between 0 and &pi;.
         *
         * This routine returns \c false if and only if strictAngleStructure()
         * throws an exception.  However, if you do not \e know whether a
         * strict angle structure exists, then this routine is faster:
         *
         * - If there is \e no strict angle structure, this routine will
         *   avoid the overhead of throwing and catching exceptions.
         *
         * - If there \e is a strict angle structure, this routine will find
         *   and cache this angle structure, which means that any subsequent
         *   call to strictAngleStructure() to retrieve its details will
         *   be essentially instantaneous.
         *
         * The underlying algorithm runs a single linear program (it does
         * \e not enumerate all vertex angle structures).  This means
         * that it is likely to be fast even for large triangulations.
         *
         * @return \c true if and only if a strict angle structure exists on
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
         * \warning This routine does not actually tell you \e whether
         * the triangulation supports a strict angle structure; it merely
         * tells you whether the answer has already been computed (or is
         * very easily computed).
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsStrictAngleStructure() const;
        /**
         * Returns a generalised angle structure on this triangulation,
         * if one exists.  A \e generalised angle structure must satisfy the
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
         * \note For a valid triangulation with no boundary faces, a
         * generalised angle structure exists if and only if every vertex link
         * is a torus or Klein bottle.  The "only if" direction is a simple
         * Euler characteristic calculation; for the "if" direction see
         * "Angle structures and normal surfaces", Feng Luo and Stephan
         * Tillmann, Trans. Amer. Math. Soc. 360:6 (2008), pp. 2849-2866).
         *
         * \exception NoSolution no generalised angle structure exists on
         * this triangulation.
         *
         * @return a generalised angle structure on this triangulation, if
         * one exists.
         */
        const AngleStructure& generalAngleStructure() const;
        /**
         * Determines whether this triangulation supports a generalised angle
         * structure.  A \e generalised angle structure must satisfy the
         * same matching equations as all angle structures do, but there is no
         * constraint on the signs of the angles; in particular, negative
         * angles are allowed.
         *
         * This routine returns \c false if and only if generalAngleStructure()
         * throws an exception.  However, if you do not \e know whether a
         * generalised angle structure exists, then this routine is faster:
         *
         * - If there is \e no generalised angle structure, this routine will
         *   avoid the overhead of throwing and catching exceptions.
         *
         * - If there \e is a generalised angle structure, this routine will
         *   find and cache this angle structure, which means that any
         *   subsequent call to generalAngleStructure() to retrieve its
         *   details will be essentially instantaneous.
         *
         * The underlying algorithm simply solves a system of linear equations,
         * and so should be fast even for large triangulations.
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
         * @return \c true if and only if a generalised angle structure exists
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
         * @return a set containing the edges of the maximal forest.
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
         * @param canJoinBoundaries \c true if and only if different
         * boundary components are allowed to be joined by the maximal forest.
         * @return a set containing the edges of the maximal forest.
         */
        std::set<Edge<3>*> maximalForestInSkeleton(
                bool canJoinBoundaries = true) const;

        /**
         * Attempts to simplify the triangulation using fast and greedy
         * heuristics.  This routine will attempt to reduce both the number
         * of tetrahedra and the number of boundary triangles (with the
         * number of tetrahedra as its priority).
         *
         * Currently this routine uses simplifyToLocalMinimum() in
         * combination with random 4-4 moves, book opening moves and
         * book closing moves.
         *
         * Although intelligentSimplify() works very well most of the time,
         * it can occasionally get stuck; in such cases you may wish to try
         * the more powerful but (much) slower simplifyExhaustive() instead.
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * \warning Running this routine multiple times upon the same
         * triangulation may return different results, since the implementation
         * makes random decisions.  More broadly, the implementation of this
         * routine (and therefore its results) may change between different
         * releases of Regina.
         *
         * \todo \opt Include random 2-3 moves to get out of wells.
         *
         * @return \c true if and only if the triangulation was successfully
         * simplified.  Otherwise this triangulation will not be changed.
         */
        bool intelligentSimplify();
        /**
         * Uses all known simplification moves to reduce the triangulation
         * monotonically to some local minimum number of tetrahedra.
         *
         * End users will probably not want to call this routine.
         * You should call intelligentSimplify() if you want a fast (and
         * usually effective) means of simplifying a triangulation, or
         * you should call simplifyExhaustive() if you are still stuck
         * and you want to try a slower but more powerful method instead.
         *
         * The moves used by this routine include 3-2, 2-0 (edge and vertex),
         * 2-1 and boundary shelling moves.
         *
         * Moves that do not reduce the number of tetrahedra
         * (such as 4-4 moves or book opening moves) are not used in this
         * routine.  Such moves do however feature in intelligentSimplify().
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * \warning The implementation of this routine (and therefore
         * its results) may change between different releases of Regina.
         *
         * @param perform \c true if we are to perform the
         * simplifications, or \c false if we are only to investigate
         * whether simplifications are possible (defaults to \c true).
         * @return if \a perform is \c true, this routine returns
         * \c true if and only if the triangulation was changed to
         * reduce the number of tetrahedra; if \a perform is \c false,
         * this routine returns \c true if and only if it determines
         * that it is capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Attempts to simplify this triangulation using a slow but
         * exhaustive search through the Pachner graph.  This routine is
         * more powerful but much slower than intelligentSimplify().
         *
         * Specifically, this routine will iterate through all
         * triangulations that can be reached from this triangulation
         * via 2-3 and 3-2 Pachner moves, without ever exceeding
         * \a height additional tetrahedra beyond the original number.
         *
         * If at any stage it finds a triangulation with \e fewer
         * tetrahedra than the original, then this routine will call
         * intelligentSimplify() to shrink the triangulation further if
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
         * If \a height is negative, then there will be \e no bound on
         * the number of additional tetrahedra.  This means that the
         * routine will not terminate until a simpler triangulation is found.
         * If no simpler diagram exists then the only way to terminate this
         * function is to cancel the operation via a progress tracker
         * (read on for details).
         *
         * If you want a \e fast simplification routine, you should call
         * intelligentSimplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn triangulations where intelligentSimplify()
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
         * in the argument \a nThreads.  Even in multithreaded mode, this
         * routine will not return until processing has finished (i.e., either
         * the triangulation was simplified or the search was exhausted).
         *
         * If this routine is unable to simplify the triangulation, then
         * the triangulation will not be changed.
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition this triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * @param height the maximum number of \e additional tetrahedra to
         * allow beyond the number of tetrahedra originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c nullptr if no progress reporting is required.
         * @return \c true if and only if the triangulation was successfully
         * simplified to fewer tetrahedra.
         */
        bool simplifyExhaustive(int height = 1, unsigned nThreads = 1,
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
         *   the triangulation and its isomorphism signature.
         *   The second form is offered in order to avoid unnecessarily
         *   recomputation within the \a action function.
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
         * If \a height is negative, then there will be \e no bound on
         * the number of additional tetrahedra.  This means that the
         * routine will <i>never terminate</i>, unless \a action returns
         * \c true for some triangulation that is passed to it.
         *
         * Since Regina 7.0, this routine will not return until the exploration
         * of triangulations is complete, regardless of whether a progress
         * tracker was passed.  If you need the old behaviour (where passing a
         * progress tracker caused the enumeration to start in the background),
         * simply call this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads in
         * the argument \a nThreads.  Even in multithreaded mode, this routine
         * will not return until processing has finished (i.e., either \a action
         * returned \c true, or the search was exhausted).  All calls to
         * \a action will be protected by a mutex (i.e., different threads will
         * never be calling \a action at the same time); as a corollary, the
         * action should avoid expensive operations where possible (otherwise
         * it will become a serialisation bottleneck in the multithreading).
         *
         * \pre This triangulation is connected.
         *
         * \exception FailedPrecondition this triangulation has more
         * than one connected component.  If a progress tracker was passed,
         * it will be marked as finished before the exception is thrown.
         *
         * \apinotfinal
         *
         * \ifacespython This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the arguments \a tracker and \a args are
         * removed, so you call it as retriangulate(height, threads, action).
         * Moreover, \a action must take exactly two arguments
         * (const std::string&, Triangulation<3>&&) representing the signature
         * and the triangulation, as described in option (b) above.
         *
         * @param height the maximum number of \e additional tetrahedra to
         * allow beyond the number of tetrahedra originally present in the
         * triangulation, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @param action a function (or other callable object) to call
         * for each triangulation that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial triangulation argument(s).
         * @return \c true if some call to \a action returned \c true (thereby
         * terminating the search early), or \c false if the search ran to
         * completion.
         */
        template <typename Action, typename... Args>
        bool retriangulate(int height, unsigned nThreads,
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
         * components (and these simply will be left alone).
         *
         * If this triangulation is currently oriented, then this operation
         * will preserve the orientation.
         *
         * \pre This triangulation is valid.
         *
         * @return \c true if the triangulation was changed, or \c false if
         * every boundary component was already minimal to begin with.
         */
        bool minimiseBoundary();

        /**
         * A synonym for minimiseBoundary().
         * This ensures that the boundary contains the smallest possible
         * number of triangles, potentially adding tetrahedra to do this.
         *
         * See minimiseBoundary() for further details.
         *
         * \pre This triangulation is valid.
         *
         * @return \c true if the triangulation was changed, or \c false if
         * every boundary component was already minimal to begin with.
         */
        bool minimizeBoundary();

        /**
         * Checks the eligibility of and/or performs a 4-4 move
         * about the given edge.
         * This involves replacing the four tetrahedra joined at that
         * edge with four tetrahedra joined along a different edge.
         * Consider the octahedron made up of the four original
         * tetrahedra; this has three internal axes.  The initial four
         * tetrahedra meet along the given edge which forms one of these
         * axes; the new tetrahedra will meet along a different axis.
         * This move can be done iff (i) the edge is valid and non-boundary,
         * and (ii) the four tetrahedra are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * If this triangulation is currently oriented, then this 4-4 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param newAxis Specifies which axis of the octahedron the new
         * tetrahedra should meet along; this should be 0 or 1.
         * Consider the four original tetrahedra in the order described
         * by Edge<3>::embedding(0,...,3); call these tetrahedra 0, 1, 2 and
         * 3.  If \a newAxis is 0, the new axis will separate tetrahedra
         * 0 and 1 from 2 and 3.  If \a newAxis is 1, the new axis will
         * separate tetrahedra 1 and 2 from 3 and 0.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool fourFourMove(Edge<3>* e, int newAxis, bool check = true,
                bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-0 move
         * about the given edge of degree 2.
         * This involves taking the two tetrahedra joined at that edge
         * and squashing them flat.  This can be done if:
         *
         * - the edge is valid and non-boundary;
         *
         * - the two tetrahedra are distinct;
         *
         * - the edges opposite \c e in each tetrahedron are distinct and
         *   not both boundary;
         *
         * - if triangles \a f1 and \a f2 from one tetrahedron are to be
         *   flattened onto triangles \a g1 and \a g2 of the other
         *   respectively, then
         *   (a) \a f1 and \a g1 are distinct,
         *   (b) \a f2 and \a g2 are distinct,
         *   (c) we do not have both \a f1 = \a g2 and \a g1 = \a f2,
         *   (d) we do not have both \a f1 = \a f2 and \a g1 = \a g2, and
         *   (e) we do not have two of the triangles boundary and the other
         *   two identified.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(Edge<3>* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-0 move
         * about the given vertex of degree 2.
         * This involves taking the two tetrahedra joined at that vertex
         * and squashing them flat.
         * This can be done if:
         *
         * - the vertex is non-boundary and has a 2-sphere vertex link;
         *
         * - the two tetrahedra are distinct;
         *
         * - the triangles opposite \c v in each tetrahedron are distinct and
         *   not both boundary;
         *
         * - the two tetrahedra meet each other on all three faces touching
         *   the vertex (as opposed to meeting each other on one face and
         *   being glued to themselves along the other two).
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a v)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given vertex is a vertex of this triangulation.
         *
         * @param v the vertex about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(Vertex<3>* v, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-1 move
         * about the given edge.
         * This involves taking an edge meeting only one tetrahedron
         * just once and merging that tetrahedron with one of the
         * tetrahedra joining it.
         *
         * This can be done assuming the following conditions:
         *
         * - The edge must be valid and non-boundary.
         *
         * - The two remaining faces of the tetrahedron are not joined, and
         *   the tetrahedron face opposite the given endpoint of the edge is
         *   not boundary.
         *
         * - Consider the second tetrahedron to be merged (the one
         *   joined along the face opposite the given endpoint of the edge).
         *   Moreover, consider the two edges of this second tetrahedron
         *   that run from the (identical) vertices of the original
         *   tetrahedron not touching \c e to the vertex of the second
         *   tetrahedron not touching the original tetrahedron.  These edges
         *   must be distinct and may not both be in the boundary.
         *
         * There are additional "distinct and not both boundary" conditions
         * on faces of the second tetrahedron, but those follow automatically
         * from the final condition above.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * If this triangulation is currently oriented, then this 2-1 move
         * will label the new tetrahedra in a way that preserves the
         * orientation.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param edgeEnd the end of the edge \e opposite that at
         * which the second tetrahedron (to be merged) is joined.
         * The end is 0 or 1, corresponding to the labelling (0,1) of
         * the vertices of the edge as described in
         * EdgeEmbedding<3>::vertices().
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoOneMove(Edge<3>* e, int edgeEnd,
                bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a book opening move
         * about the given triangle.
         * This involves taking a triangle meeting the boundary along two
         * edges, and ungluing it to create two new boundary triangles
         * (thus exposing the tetrahedra it initially joined).
         * This move is the inverse of the closeBook() move, and is
         * used to open the way for new shellBoundary() moves.
         *
         * This move can be done if:
         *
         * - the triangle meets the boundary in precisely two edges (and thus
         *   also joins two tetrahedra);
         *
         * - the vertex between these two edges is a standard boundary
         *   vertex (its link is a disc);
         *
         * - the remaining edge of the triangle (which is internal to the
         *   triangulation) is valid.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given triangle is a triangle of this triangulation.
         *
         * @param t the triangle about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool openBook(Triangle<3>* t, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a book closing move
         * about the given boundary edge.
         * This involves taking a boundary edge of the triangulation and
         * folding together the two boundary triangles on either side.  This
         * move is the inverse of the openBook() move, and is used to
         * simplify the boundary of the triangulation.
         * This move can be done if:
         *
         * - the edge \a e is a boundary edge;
         *
         * - the two vertices opposite \a e in the boundary triangles
         *   that contain \a e are valid and distinct;
         *
         * - the boundary component containing \a e contains more than
         *   two triangles.
         *
         * There are in fact several other distinctness conditions on
         * the nearby edges and triangles, but they follow automatically
         * from the conditions above.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a f)
         * can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool closeBook(Edge<3>* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a boundary shelling
         * move on the given tetrahedron.
         * This involves simply popping off a tetrahedron that touches
         * the boundary.
         * This can be done if:
         *
         * - all edges of the tetrahedron are valid;
         *
         * - precisely one, two or three faces of the tetrahedron lie in
         *   the boundary;
         *
         * - if one face lies in the boundary, then the opposite vertex
         *   does not lie in the boundary, and no two of the remaining three
         *   edges are identified;
         *
         * - if two faces lie in the boundary, then the remaining edge does
         *   not lie in the boundary, and the remaining two faces of the
         *   tetrahedron are not identified.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects can no longer be used.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * @param t the tetrahedron upon which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool shellBoundary(Tetrahedron<3>* t,
                bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a collapse of an edge
         * between two distinct vertices.  This operation (when it is allowed)
         * does not change the topology of the manifold, decreases the
         * number of vertices by one, and also decreases the number of
         * tetrahedra.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * If you are trying to reduce the number of vertices without changing
         * the topology, and if \a e is an edge connecting an internal vertex
         * with some different vertex, then either collapseEdge() or pinchEdge()
         * may be more appropriate for your situation.
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
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * The eligibility requirements for this move are somewhat
         * involved, and are discussed in detail in the collapseEdge()
         * source code for those who are interested.
         *
         * \pre If the move is being performed and no check is being run,
         * it must be known in advance that the move is legal.
         * \pre The given edge is an edge of this triangulation.
         *
         * @param e the edge to collapse.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the given edge may be collapsed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool collapseEdge(Edge<3>* e, bool check = true, bool perform = true);

        /**
         * Reorders the tetrahedra of this triangulation using a
         * breadth-first search, so that small-numbered tetrahedra are
         * adjacent to other small-numbered tetrahedra.
         *
         * Specifically, the reordering will operate as follows.
         * Tetrahedron 0 will remain tetrahedron 0.  Its immediate
         * neighbours will be numbered 1, 2, 3 and 4 (though if these
         * neighbours are not distinct then of course fewer labels will
         * be required).  Their immediate neighbours will in turn be
         * numbered 5, 6, and so on, ultimately following a breadth-first
         * search throughout the entire triangulation.
         *
         * If the optional argument \a reverse is \c true, then tetrahedron
         * numbers will be assigned in reverse order.  That is, tetrahedron 0
         * will become tetrahedron \a n-1, its neighbours will become
         * tetrahedra \a n-2 down to \a n-5, and so on.
         *
         * @param reverse \c true if the new tetrahedron numbers should
         * be assigned in reverse order, as described above.
         */
        void reorderTetrahedraBFS(bool reverse = false);

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
         * \warning This routine may be slow, since it backtracks
         * through all possible edge orientations until a consistent one
         * has been found.
         *
         * @param forceOriented \c true if the triangulation must be
         * both ordered and \e oriented, in which case this routine will
         * return \c false if the triangulation cannot be oriented and
         * ordered at the same time.  See orient() for further details.
         * @return \c true if the triangulation has been successfully ordered
         * as described above, or \c false if not.
         *
         * @author Matthias Goerner
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
         * projective planes.  If the manifold \e does contain embedded
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
         * \exception UnsolvedCase the original manifold is non-orientable
         * and contains one or more embedded two-sided projective planes,
         * and this routine was not able to recover from this situation.
         *
         * @return a list of triangulations of prime summands.
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
         * @return \c true if and only if this is a 3-sphere triangulation.
         */
        bool isSphere() const;
        /**
         * Deprecated function to test if this is a 3-sphere triangulation.
         *
         * \deprecated This routine has been renamed isSphere().
         *
         * @return \c true if and only if this is the 3-sphere.
         */
        [[deprecated]] bool isThreeSphere() const;
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
         * \warning This routine does not actually tell you \e whether
         * this triangulation forms a 3-sphere; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsSphere() const;
        /**
         * Deprecated function to determine if it is already known (or
         * trivial to determine) whether this is a 3-sphere triangulation.
         *
         * \deprecated This routine has been renamed knowsSphere().
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        [[deprecated]] bool knowsThreeSphere() const;
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
         * @return \c true if and only if this is a triangulation of a
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
         * \warning This routine does not actually tell you \e whether
         * this triangulation forms a ball; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * @return \c true if and only if this property is already known
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
         * @return \c true if and only if this is either a real (compact)
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
         * \warning This routine does not actually tell you \e whether
         * this triangulation forms a solid torus; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsSolidTorus() const;

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
         * @return \c true if and only if this is a triangulation (either
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
         * \warning This routine does not actually tell you \e whether
         * this triangulation forms the product of the torus with an interval;
         * it merely tells you whether the answer has already been computed
         * (or is very easily computed).
         *
         * @return \c true if and only if this property is already known
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
         * @return \c true if and only if the underlying 3-manifold is
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
         * \warning This routine does not actually tell you \e whether
         * the underlying 3-manifold is irreducible; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsIrreducible() const;

        /**
         * Searches for a compressing disc within the underlying
         * 3-manifold.
         *
         * Let \a M be the underlying 3-manifold and let \a B be its
         * boundary.  By a <i>compressing disc</i>, we mean a disc \a D
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
         * @return \c true if the underlying 3-manifold contains a
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
         * \warning This routine does not actually tell you \e whether
         * the underlying 3-manifold has a compressing disc; it merely tells
         * you whether the answer has already been computed (or is very
         * easily computed).
         *
         * \pre This triangulation is valid and is not ideal.
         * \pre The underlying 3-manifold is irreducible.
         *
         * @return \c true if and only if this property is already known
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
         * @return \c true if and only if the underlying 3-manifold is
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
         * \warning This routine does not actually tell you \e whether
         * the underlying 3-manifold is Haken; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * \pre This triangulation is valid, closed, orientable and connected.
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsHaken() const;

        /**
         * Searches for a "simple" compressing disc inside this
         * triangulation.
         *
         * Let \a M be the underlying 3-manifold and let \a B be its
         * boundary.  By a <i>compressing disc</i>, we mean a disc \a D
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
         * @return \c true if a simple compressing disc was found,
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
         * details on what it means to be a \e nice tree decomposition.
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
         * @return a nice tree decomposition of the face pairing graph
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
         * \warning Currently, this routine subdivides all tetrahedra as
         * if <i>all</i> vertices (not just some) were ideal.
         * This may lead to more tetrahedra than are necessary.
         *
         * \warning Currently, the presence of an invalid edge will force
         * the triangulation to be subdivided regardless of the value of
         * parameter \a forceDivision.  The final triangulation will
         * still have the projective plane cusp caused by the invalid
         * edge.
         *
         * \todo \optlong Have this routine only use as many tetrahedra
         * as are necessary, leaving finite vertices alone.
         *
         * @return \c true if and only if the triangulation was changed.
         * @author David Letscher
         */
        bool idealToFinite();

        /**
         * Pinches an internal edge to a point.  Topologically, this collapses
         * the edge to a point with no further side-effects, and it increases
         * the number of tetrahedra by two.
         *
         * This operation can be performed on \e any internal edge,
         * without further constraints.  Two particularly useful settings are:
         *
         * - If the edge joins an internal vertex with some different vertex
         *   (which may be internal, boundary, ideal or invalid), then
         *   this move does not change the topology of the manifold at all,
         *   and it reduces the total number of vertices by one.  In
         *   this sense, it acts as an alternative to collapseEdge(),
         *   and unlike collapseEdge() it can \e always be performed.
         *
         * - If the edge runs from an internal vertex back to itself,
         *   then this move effectively drills out the edge, leaving
         *   an ideal torus or Klein bottle boundary component.
         *
         * We do not allow \a e to lie entirely on the triangulation boundary,
         * because the implementation actually collapses an internal curve
         * \e parallel to \a e, not the edge \a e itself (and so if \a e is a
         * boundary edge then the topological effect would not be as intended).
         * We do allow \a e to be an internal edge with both endpoints on the
         * boundary, but note that in this case the resulting topological
         * operation would render the triangulation invalid.
         *
         * If you are trying to reduce the number of vertices without changing
         * the topology, and if \a e is an edge connecting an internal vertex
         * with some different vertex, then either collapseEdge() or pinchEdge()
         * may be more appropriate for your situation.
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
         * Note that after performing this move, all skeletal objects
         * (triangles, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a e)
         * can no longer be used.
         *
         * \pre The given edge is an internal edge of this triangulation
         * (that is, \a e does not lie entirely within the boundary).
         *
         * @param e the edge to collapse.
         */
        void pinchEdge(Edge<3>* e);

        /**
         * Punctures this manifold by removing a 3-ball from the interior of
         * the given tetrahedron.  If no tetrahedron is specified (i.e.,
         * the tetrahedron pointer is \c null), then the puncture will be
         * taken from the interior of tetrahedron 0.
         *
         * The puncture will not meet the boundary of the tetrahedron,
         * so nothing will go wrong if the tetrahedron has boundary facets
         * and/or ideal vertices.  A side-effect of this, however, is
         * that the resulting triangulation will contain additional vertices,
         * and will almost certainly be far from minimal.
         * It is highly recommended that you run intelligentSimplify()
         * if you do not need to preserve the combinatorial structure of
         * the new triangulation.
         *
         * The puncturing is done by subdividing the original tetrahedron.
         * The new tetrahedra will have orientations consistent with the
         * original tetrahedra, so if the triangulation was originally oriented
         * then it will also be oriented after this routine has been called.
         * See isOriented() for further details on oriented triangulations.
         *
         * The new sphere boundary will be formed from two triangles;
         * specifically, face 0 of the last and second-last tetrahedra
         * of the triangulation.  These two triangles will be joined so
         * that vertex 1 of each tetrahedron coincides, and vertices 2,3
         * of one map to vertices 3,2 of the other.
         *
         * \pre This triangulation is non-empty, and if \c tet is non-null
         * then it is in fact a tetrahedron of this triangulation.
         *
         * @param tet the tetrahedron inside which the puncture will be
         * taken.  This may be \c null (the default), in which case the
         * first tetrahedron will be used.
         */
        void puncture(Tetrahedron<3>* tet = nullptr);

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
         * @param edge the boundary edge upon which to layer.
         * @return the new tetrahedron provided by the layering.
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
         * <tt>bc->edge(0)</tt>, <tt>bc->edge(1)</tt> and <tt>bc->edge(2)</tt>
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
         * @param cuts0 the number of times that the meridional curve of
         * the new solid torus should cut the edge <tt>bc->edge(0)</tt>.
         * @param cuts1 the number of times that the meridional curve of
         * the new solid torus should cut the edge <tt>bc->edge(1)</tt>.
         * @param cuts2 the number of times that the meridional curve of
         * the new solid torus should cut the edge <tt>bc->edge(2)</tt>.
         * @param bc the boundary component to fill.  If the triangulation
         * has precisely one boundary component then this may be \c null.
         * @return \c true if the boundary component was filled successfully,
         * or \c false if one of the required conditions as described
         * above is not satisfied.
         */
        bool fillTorus(unsigned long cuts0, unsigned long cuts1,
            unsigned long cuts2, BoundaryComponent<3>* bc = nullptr);

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
         * @param e0 one of the three edges of the boundary component to fill.
         * @param e1 the second of the three edges of the boundary component
         * to fill.
         * @param e2 the second of the three edges of the boundary component
         * to fill.
         * @param cuts0 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e0.
         * @param cuts1 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e1.
         * @param cuts2 the number of times that the meridional curve of
         * the new solid torus should cut the edge \a e2.
         * @return \c true if the boundary component was filled successfully,
         * or \c false if one of the required conditions as described
         * above is not satisfied.
         */
        bool fillTorus(Edge<3>* e0, Edge<3>* e1, Edge<3>* e2,
            unsigned long cuts0, unsigned long cuts1, unsigned long cuts2);

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
         * \pre 0 \<= \a cuts0 \<= \a cuts1;
         * \pre gcd(\a cuts0, \a cuts1) = 1.
         *
         * @param cuts0 the smallest of the three desired intersection numbers.
         * @param cuts1 the second smallest of the three desired intersection
         * numbers.
         * @return the tetrahedron containing the boundary torus.
         *
         * @see LayeredSolidTorus
         */
        Tetrahedron<3>* insertLayeredSolidTorus(unsigned long cuts0,
            unsigned long cuts1);
        /**
         * Deprecated routine that inserts a new layered lens space L(p,q)
         * into this triangulation.
         * The lens space will be created by gluing together two layered
         * solid tori in a way that uses the fewest possible tetrahedra.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \deprecated If you just wish to create a layered lens space,
         * call <tt>Example<3>::lens(p, q)</tt>.  If you wish to insert a
         * copy of it into an existing triangulation, call
         * <tt>insertTriangulation(Example<3>::lens(p, q))</tt>.
         *
         * \pre \a p \> \a q \>= 0 unless (<i>p</i>,<i>q</i>) = (0,1);
         * \pre gcd(\a p, \a q) = 1.
         *
         * @param p a parameter of the desired lens space.
         * @param q a parameter of the desired lens space.
         *
         * @see LayeredLensSpace
         */
        [[deprecated]] void insertLayeredLensSpace(size_t p, size_t q);
        /**
         * Deprecated routine that inserts a layered loop of the given length
         * into this triangulation.  Layered loops are described in more
         * detail in the LayeredLoop class notes.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * @param length the length of the new layered loop; this must
         * be strictly positive.
         * @param twisted \c true if the new layered loop should be twisted,
         * or \c false if it should be untwisted.
         *
         * \deprecated If you just wish to create a layered loop, call
         * <tt>Example<3>::layeredLoop(...)</tt>.  If you wish to
         * insert a copy of it into an existing triangulation, call
         * <tt>insertTriangulation(Example<3>::layeredLoop(...))</tt>.
         *
         * @see LayeredLoop
         */
        [[deprecated]] void insertLayeredLoop(size_t length, bool twisted);
        /**
         * Deprecated routine that inserts an augmented triangular solid torus
         * with the given parameters into this triangulation.  Almost all
         * augmented triangular solid tori represent Seifert fibred spaces with
         * three or fewer exceptional fibres.  Augmented triangular solid tori
         * are described in more detail in the AugTriSolidTorus class notes.
         *
         * The resulting Seifert fibred space will be
         * SFS((\a a1, \a b1), (\a a2, \a b2), (\a a3, \a b3), (1,1)),
         * where the parameters \a a1, ..., \a b3 are passed as arguments to
         * this routine.  The three layered solid tori that are attached to
         * the central triangular solid torus will be
         * LST(|<i>a1</i>|, |<i>b1</i>|, |-<i>a1</i>-<i>b1</i>|), ...,
         * LST(|<i>a3</i>|, |<i>b3</i>|, |-<i>a3</i>-<i>b3</i>|).
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \deprecated If you just wish to create an augmented triangular solid
         * torus, call <tt>Example<3>::augTriSolidTorus(...)</tt>.  If you wish
         * to insert a copy of it into an existing triangulation, call
         * <tt>insertTriangulation(Example<3>::augTriSolidTorus(...))</tt>.
         *
         * \pre gcd(\a a1, \a b1) = 1.
         * \pre gcd(\a a2, \a b2) = 1.
         * \pre gcd(\a a3, \a b3) = 1.
         *
         * @param a1 a parameter describing the first layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         * @param b1 a parameter describing the first layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         * @param a2 a parameter describing the second layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         * @param b2 a parameter describing the second layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         * @param a3 a parameter describing the third layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         * @param b3 a parameter describing the third layered solid
         * torus in the augmented triangular solid torus; this may be
         * either positive or negative.
         */
        [[deprecated]] void insertAugTriSolidTorus(long a1, long b1,
            long a2, long b2, long a3, long b3);
        /**
         * Deprecated routine that inserts an orientable Seifert fibred space
         * with at most three exceptional fibres over the 2-sphere into this
         * triangulation.
         *
         * The Seifert fibred space will be
         * SFS((\a a1, \a b1), (\a a2, \a b2), (\a a3, \a b3)), where the
         * parameters \a a1, ..., \a b3 are passed as arguments to this routine.
         *
         * The three pairs of parameters (\a a, \a b) do not need
         * to be normalised, i.e., the parameters can be positive or
         * negative and \a b may lie outside the range [0..\a a).
         * There is no separate twisting parameter; each additional
         * twist can be incorporated into the existing parameters by replacing
         * some pair (\a a>, \a b) with the pair (\a a, \a a + \a b).
         * For Seifert fibred spaces with less than three exceptional fibres,
         * some or all of the parameter pairs may be (1, \a k) or even (1, 0).
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \deprecated If you just wish to triangulate the given Seifert fibred
         * space, call <tt>Example<3>::sfsOverSphere(...)</tt>.  If you wish
         * to insert a copy of it into an existing triangulation, call
         * <tt>insertTriangulation(Example<3>::sfsOverSphere(...))</tt>.
         *
         * \pre None of \a a1, \a a2 or \a a3 are 0.
         * \pre gcd(\a a1, \a b1) = 1.
         * \pre gcd(\a a2, \a b2) = 1.
         * \pre gcd(\a a3, \a b3) = 1.
         *
         * @param a1 a parameter describing the first exceptional fibre.
         * @param b1 a parameter describing the first exceptional fibre.
         * @param a2 a parameter describing the second exceptional fibre.
         * @param b2 a parameter describing the second exceptional fibre.
         * @param a3 a parameter describing the third exceptional fibre.
         * @param b3 a parameter describing the third exceptional fibre.
         */
        [[deprecated]] void insertSFSOverSphere(long a1 = 1, long b1 = 0,
            long a2 = 1, long b2 = 0, long a3 = 1, long b3 = 0);
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
         * It is allowed to pass this triangulation as \a other.
         *
         * @param other the triangulation to sum with this.
         */
        void connectedSumWith(const Triangulation& other);
        /**
         * Deprecated routine that inserts the rehydration of the given string
         * into this triangulation.  See dehydrate() and rehydrate() for
         * more information on dehydration strings.
         *
         * This routine will first rehydrate the given string into a proper
         * triangulation.  The tetrahedra from the rehydrated triangulation
         * will then be inserted into this triangulation in the same order in
         * which they appear in the rehydrated triangulation, and the
         * numbering of their vertices (0-3) will not change.
         *
         * \deprecated If you just wish to rehydrate a dehydration string,
         * simply call the static routine rehydrate().  If you wish to insert
         * the rehydration into an existing triangulation, call rehydrate()
         * followed by insertTriangulation().
         *
         * \exception InvalidArgument the given string could not be rehydrated.
         *
         * @param dehydration a dehydrated representation of the
         * triangulation to insert.  Case is irrelevant; all letters
         * will be treated as if they were lower case.
         */
        [[deprecated]] void insertRehydration(const std::string& dehydration);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Dehydrates this triangulation into an alphabetical string.
         *
         * A <i>dehydration string</i> is a compact text representation
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
         * triangulation recovered <b>might not be identical</b> to the
         * original, but it is guaranteed to be an isomorphic copy.
         *
         * For a full description of the dehydrated triangulation
         * format, see <i>A Census of Cusped Hyperbolic 3-Manifolds</i>,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * @return a dehydrated representation of this triangulation
         * (or an isomorphic variant of this triangulation), or the
         * empty string if dehydration is not possible because the
         * triangulation is disconnected, has boundary triangles or contains
         * too many tetrahedra.
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
         * If you wish to export a triangulation to a SnapPea \e file, you
         * should call saveSnapPea() instead (which has better performance, and
         * does not require you to construct an enormous intermediate string).
         *
         * If this triangulation is empty, invalid, or contains boundary
         * triangles (which SnapPea cannot represent), then the resulting
         * string will be empty.
         *
         * @return a string containing the contents of the corresponding
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
         * If this triangulation is empty, invalid, or contains boundary
         * triangles (which SnapPea cannot represent), then nothing will
         * be written to the output stream.
         *
         * \ifacespython Not present, but you can call snapPea() with
         * no arguments which returns this data as a string.
         *
         * @param out the output stream to which the SnapPea data file
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
         * If this triangulation is empty, invalid, or contains boundary
         * triangles (which SnapPea cannot represent), then the file
         * will not be written and this routine will return \c false.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The \e contents of the file will be written using UTF-8.
         *
         * @param filename the name of the SnapPea file to which to write.
         * @return \c true if and only if the file was successfully written.
         */
        bool saveSnapPea(const char* filename) const;

        /**
         * Returns a string that expresses this triangulation in
         * Matveev's 3-manifold recogniser format.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * @return a string containing the 3-manifold recogniser data.
         */
        std::string recogniser() const;

        /**
         * A synonym for recogniser().  This returns a string that
         * expresses this triangulation in Matveev's 3-manifold
         * recogniser format.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * @return a string containing the 3-manifold recogniser data.
         */
        std::string recognizer() const;

        /**
         * Writes a string expressing this triangulation in Matveev's
         * 3-manifold recogniser format to the given output stream.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * \ifacespython Not present, but you can call recogniser() with
         * no arguments which returns this data as a string.
         *
         * @param out the output stream to which the recogniser data file
         * will be written.
         */
        void recogniser(std::ostream& out) const;

        /**
         * A synonym for recognizer(std::ostream&).  This writes
         * a string expressing this triangulation in Matveev's
         * 3-manifold recogniser format to the given output stream.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * \ifacespython Not present, but you can call recognizer() with
         * no arguments which returns this data as a string.
         *
         * @param out the output stream to which the recogniser data file
         * will be written.
         */
        void recognizer(std::ostream& out) const;

        /**
         * Writes this triangulation to the given file in Matveev's
         * 3-manifold recogniser format.
         *
         * \pre This triangulation is not invalid, and does not contain
         * any boundary triangles.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The \e contents of the file will be written using UTF-8.
         *
         * @param filename the name of the Recogniser file to which to write.
         * @return \c true if and only if the file was successfully written.
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
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  The \e contents of the file will be written using UTF-8.
         *
         * @param filename the name of the Recogniser file to which to write.
         * @return \c true if and only if the file was successfully written.
         */
        bool saveRecognizer(const char* filename) const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Deprecated function that allows the user to interactively enter a
         * triangulation in plain text.  Prompts will be sent to the given
         * output stream and information will be read from the given input
         * stream.
         *
         * \deprecated This is essentially an ancient piece of an interactive
         * user interface, buried inside what is otherwise a mathematical
         * library, and dating back to before Regina offered Python bindings.
         * Nowadays there are better ways of interacting with Regina at a low
         * level (e.g., Python), and so this routine will soon be removed
         * completely.
         *
         * \ifacespython This routine takes no parameters; \a in and \a out
         * are always assumed to be standard input and standard output
         * respectively.
         *
         * @param in the input stream from which text will be read.
         * @param out the output stream to which prompts will be written.
         * @return the triangulation entered in by the user.
         */
        [[deprecated]] static Triangulation<3> enterTextTriangulation(
                std::istream& in, std::ostream& out);
        /**
         * Rehydrates the given alphabetical string into a 3-dimensional
         * triangulation.
         *
         * For a full description of the dehydrated triangulation
         * format, see <i>A Census of Cusped Hyperbolic 3-Manifolds</i>,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * The converse routine dehydrate() can be used to extract a
         * dehydration string from an existing triangulation.  Dehydration
         * followed by rehydration might not produce a triangulation identical
         * to the original, but it is guaranteed to produce an isomorphic
         * copy.  See dehydrate() for the reasons behind this.
         *
         * \exception InvalidArgument the given string could not be rehydrated.
         *
         * @param dehydration a dehydrated representation of the
         * triangulation to construct.  Case is irrelevant; all letters
         * will be treated as if they were lower case.
         * @return the rehydrated triangulation.
         */
        static Triangulation<3> rehydrate(const std::string& dehydration);

        /**
         * Extracts the tetrahedron gluings from a string that contains the
         * full contents of a SnapPea data file.  All other SnapPea-specific
         * information (such as peripheral curves, and the manifold name) will
         * be ignored, since Regina's Triangulation<3> class does not track
         * such information itself.
         *
         * If you wish to preserve all SnapPea-specific information from the
         * data file, you should work with the SnapPeaTriangulation class
         * instead (which uses the SnapPea kernel directly, and can therefore
         * store anything that SnapPea can).
         *
         * If you wish to read a triangulation from a SnapPea \e file, you
         * should likewise call the SnapPeaTriangulation constructor, giving
         * the filename as argument.  This will read all SnapPea-specific
         * information (as described above), and also avoids constructing an
         * enormous intermediate string.
         *
         * \warning This routine is "lossy", in that drops SnapPea-specific
         * information (as described above).  Unless you specifically need an
         * Triangulation<3> (not an SnapPeaTriangulation) or you need to avoid
         * calling routines from the SnapPea kernel, it is highly recommended
         * that you create a SnapPeaTriangulation from the given file
         * contents instead.  See the string-based SnapPeaTriangulation
         * constructor for how to do this.
         *
         * \exception InvalidArgument the given SnapPea data was not in
         * the correct format.
         *
         * @param snapPeaData a string containing the full contents of a
         * SnapPea data file.
         * @return a native Regina triangulation extracted from the given
         * SnapPea data.
         */
        static Triangulation<3> fromSnapPea(const std::string& snapPeaData);

        /*@}*/

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a change event.
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
         * @author Matthias Goerner
         */
        void checkPermutations();

        void calculateSkeleton();

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

        void stretchBoundaryForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInBoundary(). */
        bool stretchForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&, std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInSkeleton(). */

        /**
         * Nullifies this SnapPea triangulation.
         *
         * \pre This Regina triangulation is in fact the inherited
         * Triangulation<3> data for the derived class SnapPeaTriangulation.
         *
         * \note This trivial function exists simply so that we can make some
         * other Triangulation<3> functions inline without needing to include
         * the full SnapPea triangulation headers here in this file.
         */
        void nullifySnapPea();

    friend class regina::Face<3, 3>;
    friend class regina::detail::SimplexBase<3>;
    friend class regina::detail::TriangulationBase<3>;
    friend class PacketData<Triangulation<3>>;
    friend class regina::XMLTriangulationReader<3>;
    friend class regina::XMLWriter<Triangulation<3>>;
};

template <>
inline PacketData<Triangulation<3>>::ChangeEventSpan::ChangeEventSpan(
        PacketData& data) : data_(data) {
    switch (data_.heldBy_) {
        case HELD_BY_SNAPPEA: {
            // We will not nullify the triangulation until after the change,
            // since the routine performing the change probably expects
            // the original (non-empty) Triangulation<3> data.
            break;
        }
        case HELD_BY_PACKET: {
            auto& p = static_cast<PacketOf<Triangulation<3>>&>(data_);
            if (! p.changeEventSpans_)
                p.fireEvent(&PacketListener::packetToBeChanged);
            ++p.changeEventSpans_;
            break;
        }
        default:
            break;
    }
}

template <>
inline PacketData<Triangulation<3>>::ChangeEventSpan::~ChangeEventSpan() {
    switch (data_.heldBy_) {
        case HELD_BY_SNAPPEA: {
            static_cast<Triangulation<3>&>(data_).nullifySnapPea();
            break;
        }
        case HELD_BY_PACKET: {
            auto& p = static_cast<PacketOf<Triangulation<3>>&>(data_);
            --p.changeEventSpans_;
            if (! p.changeEventSpans_)
                p.fireEvent(&PacketListener::packetWasChanged);
            break;
        }
        default:
            break;
    }
}

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline Triangulation<3>::~Triangulation() {
    Snapshottable<Triangulation<3>>::takeSnapshot();
    clearAllProperties();
}

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim3/tetrahedron3.h"
#include "triangulation/dim3/triangle3.h"
#include "triangulation/dim3/vertex3.h"
#include "triangulation/dim3/component3.h"
namespace regina {

// Inline functions for Triangulation<3>

inline Triangulation<3>::Triangulation() :
        strictAngleStructure_(false), generalAngleStructure_(false) {
}

inline Triangulation<3>::Triangulation(const Triangulation<3>& copy) :
        Triangulation<3>(copy, true) {
}

inline Tetrahedron<3>* Triangulation<3>::newTetrahedron() {
    return newSimplex();
}

inline Tetrahedron<3>* Triangulation<3>::newTetrahedron(const std::string& desc) {
    return newSimplex(desc);
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

inline void Triangulation<3>::swapContents(Triangulation<3>& other) {
    swap(other);
}

inline bool Triangulation<3>::hasTwoSphereBoundaryComponents() const {
    if (! twoSphereBoundaryComponents_.has_value())
        calculateBoundaryProperties();
    return *twoSphereBoundaryComponents_;
}

inline bool Triangulation<3>::hasNegativeIdealBoundaryComponents() const {
    if (! negativeIdealBoundaryComponents_.has_value())
        calculateBoundaryProperties();
    return *negativeIdealBoundaryComponents_;
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

inline bool Triangulation<3>::isThreeSphere() const {
    return isSphere();
}

inline bool Triangulation<3>::knowsThreeSphere() const {
    return knowsSphere();
}

inline bool Triangulation<3>::knowsZeroEfficient() const {
    return zeroEfficient_.has_value();
}

inline std::optional<NormalSurface>
        Triangulation<3>::hasNonTrivialSphereOrDisc() const {
    return nonTrivialSphereOrDisc();
}

inline std::optional<NormalSurface>
        Triangulation<3>::hasOctagonalAlmostNormalSphere() const {
    return octagonalAlmostNormalSphere();
}

inline const AngleStructure& Triangulation<3>::strictAngleStructure() const {
    if (hasStrictAngleStructure())
        return std::get<AngleStructure>(strictAngleStructure_);
    else
        throw NoSolution();
}

inline const AngleStructure& Triangulation<3>::findStrictAngleStructure()
        const {
    return strictAngleStructure();
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
    return homologyRel().rank() + homologyRel().torsionRank(2);
}

inline const Triangulation<3>::TuraevViroSet&
        Triangulation<3>::allCalculatedTuraevViro() const {
    return turaevViroCache_;
}

template <typename Action, typename... Args>
inline bool Triangulation<3>::retriangulate(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    if (countComponents() != 1) {
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
            *this, height, nThreads, tracker,
            [&](const std::string& sig, Triangulation<3>&& obj) {
                return action(sig, std::move(obj), std::forward<Args>(args)...);
            });
    } else {
        return regina::detail::retriangulateInternal<Triangulation<3>, false>(
            *this, height, nThreads, tracker,
            [&](Triangulation<3>&& obj) {
                return action(std::move(obj), std::forward<Args>(args)...);
            });
    }
}

inline bool Triangulation<3>::simplifyExhaustive(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker) {
    return retriangulate(height, nThreads, tracker,
        [](Triangulation<3>&& alt, Triangulation<3>& original, size_t minSimp) {
            if (alt.size() < minSimp) {
                ChangeEventSpan span(original);
                original = std::move(alt);
                original.intelligentSimplify();
                return true;
            } else
                return false;
        }, *this, size());
}

inline bool Triangulation<3>::minimizeBoundary() {
    return minimiseBoundary();
}

inline const TreeDecomposition& Triangulation<3>::niceTreeDecomposition()
        const {
    if (niceTreeDecomposition_)
        return *niceTreeDecomposition_;

    TreeDecomposition ans(*this, TD_UPPER);
    ans.makeNice();
    niceTreeDecomposition_ = ans;

    return *niceTreeDecomposition_;
}

inline void Triangulation<3>::recognizer(std::ostream& out) const {
    recogniser(out);
}

inline bool Triangulation<3>::saveRecognizer(const char* filename) const {
    return saveRecogniser(filename);
}

inline void Triangulation<3>::insertRehydration(
        const std::string& dehydration) {
    insertTriangulation(rehydrate(dehydration));
}

} // namespace regina

#endif

