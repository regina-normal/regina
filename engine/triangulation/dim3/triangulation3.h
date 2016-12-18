
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

/*! \file triangulation/dim3/triangulation3.h
 *  \brief Internal header for 3-dimensional triangulations.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

#ifndef __TRIANGULATION3_H
#ifndef __DOXYGEN
#define __TRIANGULATION3_H
#endif

#include <map>
#include <memory>
#include <vector>
#include <set>

#include "regina-core.h"
#include "angle/anglestructure.h"
#include "maths/cyclotomic.h"
#include "treewidth/treedecomposition.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/boolset.h"
#include "utilities/markedvector.h"

// NOTE: More #includes for faces, components and boundary components
// follow after the class declarations.

namespace regina {

class AngleStructure;
class GroupPresentation;
class NormalSurface;
class ProgressTracker;
class ProgressTrackerOpen;
class XMLPacketReader;

template <int> class XMLTriangulationReader;

/**
 * \addtogroup dim3 3-Manifold Triangulations
 * Details for implementing triangulations of 3-manifolds.
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_TRIANGULATION3> {
    typedef Triangulation<3> Class;
    inline static const char* name() {
        return "3-Manifold Triangulation";
    }
};
#endif

/**
 * Represents the various algorithms available for computing Turaev-Viro
 * invariants.
 */
enum TuraevViroAlg {
    /**
     * The default algorithm.  Here Regina will choose whichever
     * algorithm it thinks (rightly or wrongly) is most appropriate.
     */
    TV_DEFAULT = 0,
    /**
     * An optimised backtracking algorithm.  This enumerates edge colourings
     * and sums their corresponding weights.  This can be slow in general
     * (since there could be exponentially many edge colourings),
     * but it has very small memory usage.
     */
    TV_BACKTRACK = 1,
    /**
     * A treewidth-based algorithm.  This uses dynamic programming over
     * a tree decomposition of the face pairing graph.  This can be fast
     * for triangulations whose face pairing graphs have small treewidth,
     * but it may require extremely large amounts of memory.
     */
    TV_TREEWIDTH = 2,
    /**
     * An unoptimised backtracking algorithm.  Like TV_BACKTRACK, this
     * enumerates edge colourings and sums weights.  However, the
     * implementation is more naive.
     *
     * \warning This algorithm should only be used for comparison and
     * experimentation.  Due to its slow performance, it is not suitable
     * for "real" applications.
     */
    TV_NAIVE = 3
};

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
 * \todo \feature Is the boundary incompressible?
 * \todo \featurelong Am I obviously a handlebody?  (Simplify and see
 * if there is nothing left).  Am I obviously not a handlebody?
 * (Compare homology with boundary homology).
 * \todo \featurelong Is the triangulation Haken?
 * \todo \featurelong What is the Heegaard genus?
 * \todo \featurelong Have a subcomplex as a child packet of a
 * triangulation.  Include routines to crush a subcomplex or to expand a
 * subcomplex to a normal surface.
 * \todo \featurelong Implement writeTextLong() for skeletal objects.
 *
 * \headerfile triangulation/dim3.h
 */
template <>
class REGINA_API Triangulation<3> :
        public Packet,
        public detail::TriangulationBase<3> {
    REGINA_PACKET(Triangulation<3>, PACKET_TRIANGULATION3)

    public:
        typedef std::vector<Tetrahedron<3>*>::const_iterator TetrahedronIterator;
            /**< A dimension-specific alias for SimplexIterator,
                 used to iterate through tetrahedra. */
        typedef FaceList<3, 2>::Iterator TriangleIterator;
            /**< Used to iterate through triangles. */
        typedef FaceList<3, 1>::Iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef FaceList<3, 0>::Iterator VertexIterator;
            /**< Used to iterate through vertices. */

        typedef std::map<std::pair<unsigned long, bool>, Cyclotomic>
                TuraevViroSet;
            /**< A map from (\a r, \a parity) pairs to Turaev-Viro invariants,
                 as described by turaevViro(). */

    private:
        bool ideal_;
            /**< Is the triangulation ideal? */
        bool standard_;
            /**< Is the triangulation standard? */

        mutable Property<AbelianGroup, StoreManagedPtr> H1Rel_;
            /**< Relative first homology group of the triangulation
             *   with respect to the boundary. */
        mutable Property<AbelianGroup, StoreManagedPtr> H1Bdry_;
            /**< First homology group of the boundary. */
        mutable Property<AbelianGroup, StoreManagedPtr> H2_;
            /**< Second homology group of the triangulation. */

        mutable Property<bool> twoSphereBoundaryComponents_;
            /**< Does the triangulation contain any 2-sphere boundary
                 components? */
        mutable Property<bool> negativeIdealBoundaryComponents_;
            /**< Does the triangulation contain any boundary components
                 that are ideal and have negative Euler characteristic? */

        mutable Property<bool> zeroEfficient_;
            /**< Is the triangulation zero-efficient? */
        mutable Property<bool> splittingSurface_;
            /**< Does the triangulation have a normal splitting surface? */

        mutable Property<bool> threeSphere_;
            /**< Is this a triangulation of a 3-sphere? */
        mutable Property<bool> threeBall_;
            /**< Is this a triangulation of a 3-dimensional ball? */
        mutable Property<bool> solidTorus_;
            /**< Is this a triangulation of the solid torus? */
        mutable Property<bool> irreducible_;
            /**< Is this 3-manifold irreducible? */
        mutable Property<bool> compressingDisc_;
            /**< Does this 3-manifold contain a compressing disc? */
        mutable Property<bool> haken_;
            /**< Is this 3-manifold Haken?
                 This property must only be stored for triangulations
                 that are known to represent closed, connected,
                 orientable, irreducible 3-manifolds. */

        mutable Property<AngleStructure, StoreManagedPtr>
                strictAngleStructure_;
            /**< A strict angle structure on this triangulation, or the
                 null pointer if none exists. */

        mutable Property<TreeDecomposition, StoreManagedPtr>
                niceTreeDecomposition_;
            /**< A nice tree decomposition of the face pairing graph of
                 this triangulation. */

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
         * The packet tree structure and packet label are \e not copied.
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
         * Regina will also set the packet label accordingly.
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
        /**
         * Destroys this triangulation.
         *
         * The constituent tetrahedra, the cellular structure and all other
         * properties will also be destroyed.
         */
        REGINA_INLINE_REQUIRED virtual ~Triangulation();

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual bool dependsOnParent() const;

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
         * This (potentially) long computation can be managed by passing
         * a progress tracker:
         *
         * - If a progress tracker is passed and the requested invariant has
         *   not yet been computed, then the calculation will take place in a
         *   new thread and this routine will return immediately.  Once the
         *   progress tracker indicates that the calculation has finished,
         *   you can call turaevViro() again with the same arguments for
         *   \a r and \a parity to retrieve the value of the invariant.
         *
         * - If no progress tracker is passed and the requested invariant has
         *   not yet been computed, the calculation will run in the current
         *   thread and this routine will not return until it is complete.
         *
         * - If the requested invariant has already been computed, then this
         *   routine will return immediately with the pre-computed value.  If
         *   a progress tracker is passed then it will be marked as finished.
         *
         * \pre This triangulation is valid, closed and non-empty.
         *
         * @param r the integer \a r as described above; this must be at
         * least 3.
         * @param parity determines for odd \a r whether \a q0 is a primitive
         * <i>2r</i>th or <i>r</i>th root of unity, as described above.
         * @param alg the algorithm with which to compute the invariant.
         * If you are not sure, the default value (TV_DEFAULT) is a safe choice.
         * @param tracker a progress tracker through will progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the requested Turaev-Viro invariant.  As an exception,
         * if \a tracker is non-null and the value of this invariant has
         * not been computed before, then (since the calculation will
         * still be running in a new thread) the return value is undefined.
         *
         * @see allCalculatedTuraevViro
         */
        Cyclotomic turaevViro(unsigned long r, bool parity = true,
            TuraevViroAlg alg = TV_DEFAULT, ProgressTracker* tracker = 0) const;
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
         * @param alg the algorithm with which to compute the invariant.
         * If you are not sure, the default value (TV_DEFAULT) is a safe choice.
         * @return the requested Turaev-Viro invariant.
         *
         * @see allCalculatedTuraevViro
         */
        double turaevViroApprox(unsigned long r, unsigned long whichRoot = 1,
            TuraevViroAlg alg = TV_DEFAULT) const;
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
         * \ifacespython Not present.
         *
         * @return the cache of all Turaev-Viro invariants that have
         * already been calculated.
         *
         * @see turaevViro
         */
        const TuraevViroSet& allCalculatedTuraevViro() const;

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
        bool isZeroEfficient();
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
         * \pre This triangulation is connected.  If the triangulation
         * is not connected, this routine will still return a result but
         * that result will be unreliable.
         *
         * @return \c true if and only if this triangulation has a
         * normal splitting surface.
         */
        bool hasSplittingSurface();
        /**
         * Is it already known whether or not this triangulation has a
         * splitting surface?
         * See hasSplittingSurface() for further details.
         *
         * If this property is already known, future calls to
         * hasSplittingSurface() will be very fast (simply returning the
         * precalculated value).
         *
         * \warning This routine does not actually tell you \e whether
         * this triangulation has a splitting surface; it merely tells you
         * whether the answer has already been computed.
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsSplittingSurface() const;
        /**
         * Searches for a non-vertex-linking normal sphere or disc
         * within this triangulation.  If such a surface exists within
         * this triangulation, this routine is guaranteed to find one.
         *
         * Note that the surface returned (if any) depends upon this
         * triangulation, and so the surface must be destroyed before this
         * triangulation is destroyed.
         *
         * \warning This routine may, in some scenarios, temporarily modify the
         * packet tree by creating and then destroying a normal surface list.
         *
         * @return a newly allocated non-vertex-linking normal sphere or
         * disc, or 0 if none exists.
         */
        NormalSurface* hasNonTrivialSphereOrDisc();
        /**
         * Searches for an octagonal almost normal 2-sphere within this
         * triangulation.  If such a surface exists, this routine is
         * guaranteed to find one.
         *
         * Note that the surface returned (if any) depends upon this
         * triangulation, and so the surface must be destroyed before this
         * triangulation is destroyed.
         *
         * \pre This triangulation is valid, closed, orientable, connected,
         * and 0-efficient.  These preconditions are almost certainly more
         * restrictive than they need to be, but we stay safe for now.
         *
         * \warning This routine may, in some scenarios, temporarily modify the
         * packet tree by creating and then destroying a normal surface list.
         *
         * @return a newly allocated non-vertex-linking normal sphere or
         * disc, or 0 if none exists.
         */
        NormalSurface* hasOctagonalAlmostNormalSphere();
        /**
         * Searches for a strict angle structure on this triangulation.
         * Recall that a \e strict angle structure is one in which every
         * angle is strictly between 0 and &pi;.  If a strict angle structure
         * does exist, then this routine is guaranteed to find one.
         *
         * The underlying algorithm runs a single linear program (it does
         * \e not enumerate all vertex angle structures).  This means
         * that it is likely to be fast even for large triangulations.
         *
         * If you are only interested in \e whether a strict angle structure
         * exists (i.e., you are not interested in the specific angles
         * themselves), then you may call hasStrictAngleStructure() instead.
         *
         * The angle structure returned (if any) is cached internally
         * alongside this triangulation.  This means that, as long as
         * the triangulation does not change, subsequent calls to
         * findStrictAngleStructure() will return identical pointers
         * and will be essentially instantaneous.
         *
         * If the triangulation changes however, then the cached angle
         * structure will be deleted.  This means that you should not
         * store the returned pointer for later use; instead you should
         * just call findStrictAngleStructure() again.
         *
         * @return a strict angle structure on this triangulation, or 0 if
         * none exists.
         */
        const AngleStructure* findStrictAngleStructure() const;
        /**
         * Determines whether this triangulation supports a strict angle
         * structure.  Recall that a \e strict angle structure is one
         * in which every angle is strictly between 0 and &pi;.
         *
         * This routine is equivalent to calling findStrictAngleStructure()
         * and testing whether the return value is non-null.
         *
         * The underlying algorithm runs a single linear program (it does
         * \e not enumerate all vertex angle structures).  This means
         * that it is likely to be fast even for large triangulations.
         *
         * @return \c true if a strict angle structure exists on this
         * triangulation, or 0 if not.
         */
        bool hasStrictAngleStructure() const;
        /**
         * Is it already known (or trivial to determine) whether or not
         * this triangulation supports a strict angle structure?
         * See hasStrictAngleStructure() for further details.
         *
         * If this property is indeed already known, future calls to
         * findStrictAngleStructure() and hasStrictAngleStructure() will be
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

        /*@}*/
        /**
         * \name Skeletal Transformations
         */
        /*@{*/

        /**
         * Produces a maximal forest in the 1-skeleton of the
         * triangulation boundary.
         * Both given sets will be emptied and the edges and vertices of
         * the maximal forest will be placed into them.
         * A vertex that forms its own boundary component (such as an
         * ideal vertex) will still be placed in \c vertexSet.
         *
         * Note that the edge and vertex pointers returned will become
         * invalid once the triangulation has changed.
         *
         * \ifacespython Not present.
         *
         * @param edgeSet the set to be emptied and into which the edges
         * of the maximal forest will be placed.
         * @param vertexSet the set to be emptied and into which the
         * vertices of the maximal forest will be placed.
         */
        void maximalForestInBoundary(std::set<Edge<3>*>& edgeSet,
                std::set<Vertex<3>*>& vertexSet) const;
        /**
         * Produces a maximal forest in the triangulation's 1-skeleton.
         * The given set will be emptied and will have the edges of the
         * maximal forest placed into it.  It can be specified whether
         * or not different boundary components may be joined by the
         * maximal forest.
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
         * \ifacespython Not present.
         *
         * @param edgeSet the set to be emptied and into which the edges
         * of the maximal forest will be placed.
         * @param canJoinBoundaries \c true if and only if different
         * boundary components are allowed to be joined by the maximal
         * forest.
         */
        void maximalForestInSkeleton(std::set<Edge<3>*>& edgeSet,
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
         * \warning The specific behaviour of this routine may well
         * change between releases.
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
         * \warning The specific behaviour of this routine is
         * very likely to change between releases.
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
         * If you want a \e fast simplification routine, you should call
         * intelligentSimplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn triangulations where intelligentSimplify()
         * finds itself stuck at a local minimum, simplifyExhaustive() is able
         * to "climb out" of such wells.
         *
         * If a progress tracker is passed, then the exhaustive simplification
         * will take place in a new thread and this routine will return
         * immediately.  In this case, you will need to use some other
         * means to determine whether the triangulation was eventually
         * simplified (e.g., by examining size() after the tracker
         * indicates that the operation has finished).
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a nThreads.  Even in multithreaded mode, if no
         * progress tracker is passed then this routine will not return until
         * processing has finished (i.e., either the triangulation was
         * simplified or the search was exhausted).
         *
         * If this routine is unable to simplify the triangulation, then
         * the triangulation will not be changed.
         *
         * \pre This triangulation is connected.
         *
         * @param height the maximum number of \e additional tetrahedra to
         * allow, beyond the number of tetrahedra originally present in the
         * triangulation.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return If a progress tracker is passed, then this routine
         * will return \c true or \c false immediately according to
         * whether a new thread could or could not be started.  If no
         * progress tracker is passed, then this routine will return \c true
         * if and only if the triangulation was successfully simplified to
         * fewer tetrahedra.
         */
        bool simplifyExhaustive(int height = 1, unsigned nThreads = 1,
            ProgressTrackerOpen* tracker = 0);

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
         * - \a action must take at least one argument.  The first argument
         *   will be of type Triangulation<3>&, and will reference the
         *   triangulation that has been found.  If there are any
         *   additional arguments supplied in the list \a args, then
         *   these will be passed as subsequent arguments to \a action.
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
         * - \a action may, if it chooses, make changes to the triangulation
         *   that is passed in its argument (though it must not delete it).
         *   This will likewise not affect the search, since the triangulation
         *   that is passed to \a action will be destroyed immediately after
         *   \a action is called.
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
         * If a progress tracker is passed, then the exploration of
         * triangulations will take place in a new thread and this
         * routine will return immediately.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a nThreads.  Even in multithreaded mode, if no
         * progress tracker is passed then this routine will not return until
         * processing has finished (i.e., either \a action returned \c true,
         * or the search was exhausted).  All calls to \a action will be
         * protected by a mutex (i.e., different threads will never be
         * calling \a action at the same time).
         *
         * If \a height is negative, then this routine will do nothing
         * and immediately return \c false, and any progress tracker
         * that was passed will immediately be marked as finished.
         *
         * \warning By default, the arguments \a args will be copied (or moved)
         * when they are passed to \a action.  If you need to pass some
         * argument(s) by reference, you must wrap them in std::ref or
         * std::cref.
         *
         * \pre This triangulation is connected.
         *
         * \apinotfinal
         *
         * \ifacespython Not present.
         *
         * @param height the maximum number of \e additional tetrahedra to
         * allow, beyond the number of tetrahedra originally present in the
         * triangulation.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @param action a function (or other callable object) to call
         * upon each triangulation that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial triangulation argument.
         * @return If a progress tracker is passed, then this routine
         * will return \c true or \c false immediately according to
         * whether a new thread could or could not be started.  If no
         * progress tracker is passed, then this routine will return \c true
         * if some call to \a action returned \c true (thereby terminating
         * the search early), or \c false if the search ran to completion.
         */
        template <typename Action, typename... Args>
        bool retriangulate(int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            Action&& action, Args&&... args) const;

        /**
         * Checks the eligibility of and/or performs a 3-2 move
         * about the given edge.
         * This involves replacing the three tetrahedra joined at that
         * edge with two tetrahedra joined by a triangle.
         * This can be done iff (i) the edge is valid and non-boundary,
         * and (ii) the three tetrahedra are distinct.
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
        bool threeTwoMove(Edge<3>* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-3 move
         * about the given triangle.
         * This involves replacing the two tetrahedra joined at that
         * triangle with three tetrahedra joined by an edge.
         * This can be done iff the two tetrahedra are distinct.
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
        bool twoThreeMove(Triangle<3>* t, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 1-4 move
         * upon the given tetrahedron.
         * This involves replacing one tetrahedron with four tetrahedra:
         * each new tetrahedron runs from one face of
         * the original tetrahedron to a new common internal degree four vertex.
         *
         * This move can always be performed.  The \a check argument is
         * present (as for other moves), but is simply ignored (since
         * the move is always legal).  The \a perform argument is also
         * present for consistency with other moves, but if it is set to
         * \c false then this routine does nothing and returns no useful
         * information.
         *
         * Note that after performing this move, all skeletal objects
         * (edges, components, etc.) will be reconstructed, which means
         * any pointers to old skeletal objects (such as the argument \a t)
         * can no longer be used.
         *
         * \pre The given tetrahedron is a tetrahedron of this triangulation.
         *
         * @param t the tetrahedron about which to perform the move.
         * @param check this argument is ignored, since this move is
         * always legal (see the notes above).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return \c true always.
         */
        bool oneFourMove(Tetrahedron<3>* t, bool check = true,
            bool perform = true);
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
         * - the two boundary triangles that it joins are distinct;
         *
         * - the two vertices opposite \a e in each of these boundary triangles
         *   are valid and distinct;
         *
         * - if edges \a e1 and \a e2 of one boundary triangle are to be
         *   folded onto edges \a f1 and \a f2 of the other boundary
         *   triangle respectively, then we do not have both \a e1 = \a e2
         *   and \a f1 = \a f2.
         *
         * There are in fact several other "distinctness" conditions on
         * the edges \a e1, \a e2, \a f1 and \a f2, but they follow
         * automatically from the "distinct vertices" condition above.
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
         * Checks the eligibility of and/or performs a collapse of
         * an edge in such a way that the topology of the manifold
         * does not change and the number of vertices of the triangulation
         * decreases by one.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
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
         * Splits this triangulation into its connected sum
         * decomposition.  The individual prime 3-manifold triangulations
         * that make up this decomposition will be inserted as children
         * of the given parent packet.  The original triangulation will
         * be left unchanged.
         *
         * For non-orientable triangulations, this routine is only guaranteed
         * to succeed if the original manifold contains no embedded two-sided
         * projective planes.  If the manifold \e does contain embedded
         * two-sided projective planes, then this routine might still succeed
         * but it might fail; however, such a failure will always be detected,
         * and in such a case this routine will return -1 instead (without
         * building any prime summands at all).
         *
         * Note that this routine is currently only available for closed
         * triangulations; see the list of preconditions for full details.
         *
         * If the given parent packet is 0, the new prime summand
         * triangulations will be inserted as children of this triangulation.
         *
         * This routine can optionally assign unique (and sensible)
         * packet labels to each of the new prime summand triangulations.
         * Note however that uniqueness testing may be slow, so this
         * assignment of labels should be disabled if the summand
         * triangulations are only temporary objects used as part
         * of a larger routine.
         *
         * If this is a triangulation of a 3-sphere then no prime summand
         * triangulations will be created at all, and this routine will
         * return 0.
         *
         * The underlying algorithm appears in "A new approach to crushing
         * 3-manifold triangulations", Discrete and Computational
         * Geometry 52:1 (2014), pp. 116-139.  This algorithm is based on the
         * Jaco-Rubinstein 0-efficiency algorithm, and works in both
         * orientable and non-orientable settings.
         *
         * \warning Users are strongly advised to check the return value if
         * embedded two-sided projective planes are a possibility, since in
         * such a case this routine might fail (as explained above).
         * Note however that this routine might still succeed, and so success
         * is not a proof that no embedded two-sided projective planes exist.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so can be very slow for larger triangulations.
         * For 3-sphere testing, see the routine isThreeSphere() which
         * uses faster methods where possible.
         *
         * \pre This triangulation is valid, closed and connected.
         *
         * @param primeParent the packet beneath which the new prime
         * summand triangulations will be inserted, or 0 if they
         * should be inserted directly beneath this triangulation.
         * @param setLabels \c true if the new prime summand triangulations
         * should be assigned unique packet labels, or \c false if
         * they should be left without labels at all.
         * @return the number of prime summands created, 0 if this
         * triangulation is a 3-sphere, or -1 if this routine failed
         * because this is a non-orientable triangulation with embedded
         * two-sided projective planes.
         */
        long connectedSumDecomposition(Packet* primeParent = 0,
            bool setLabels = true);
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
         * The routine knowsThreeSphere() can be called to see if this
         * property is already known or if it happens to be very fast to
         * calculate for this triangulation.
         *
         * @return \c true if and only if this is a 3-sphere triangulation.
         */
        bool isThreeSphere() const;
        /**
         * Is it already known (or trivial to determine) whether or not this
         * is a triangulation of a 3-sphere?  See isThreeSphere() for
         * further details.
         *
         * If this property is indeed already known, future calls to
         * isThreeSphere() will be very fast (simply returning the
         * precalculated value).
         *
         * If this property is not already known, this routine will
         * nevertheless run some very fast preliminary tests to see if the
         * answer is obviously no.  If so, it will store \c false as the
         * precalculated value for isThreeSphere() and this routine will
         * return \c true.
         *
         * Otherwise a call to isThreeSphere() may potentially require more
         * significant work, and so this routine will return \c false.
         *
         * \warning This routine does not actually tell you \e whether
         * this triangulation forms a 3-sphere; it merely tells you whether
         * the answer has already been computed (or is very easily computed).
         *
         * @return \c true if and only if this property is already known
         * or trivial to calculate.
         */
        bool knowsThreeSphere() const;
        /**
         * Determines whether this is a triangulation of a 3-dimensional ball.
         *
         * This routine is based on isThreeSphere(), which in turn combines
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
         * Converts this into a 0-efficient triangulation of the same
         * underlying 3-manifold.  A triangulation is 0-efficient if its
         * only normal spheres and discs are vertex linking, and if it has
         * no 2-sphere boundary components.
         *
         * Note that this routine is currently only available for
         * closed orientable triangulations; see the list of
         * preconditions for details.  The 0-efficiency algorithm of
         * Jaco and Rubinstein is used.
         *
         * If the underlying 3-manifold is prime, it can always be made
         * 0-efficient (with the exception of the special cases RP3 and
         * S2xS1 as noted below).  In this case the original triangulation
         * will be modified directly and 0 will be returned.
         *
         * If the underyling 3-manifold is RP3 or S2xS1, it cannot
         * be made 0-efficient; in this case the original triangulation
         * will be reduced to a two-tetrahedron minimal triangulation
         * and 0 will again be returned.
         *
         * If the underlying 3-manifold is not prime, it cannot be made
         * 0-efficient.  In this case the original triangulation will
         * remain unchanged and a new connected sum decomposition will
         * be returned.  This will be presented as a newly allocated
         * container packet with one child triangulation for each prime
         * summand.
         *
         * \warning The algorithms used in this routine rely on normal
         * surface theory and so can be very slow for larger triangulations.
         *
         * \pre This triangulation is valid, closed, orientable and
         * connected.
         *
         * @return 0 if the underlying 3-manifold is prime (in which
         * case the original triangulation was modified directly), or
         * a newly allocated connected sum decomposition if the
         * underlying 3-manifold is composite (in which case the
         * original triangulation was not changed).
         */
        Packet* makeZeroEfficient();
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
         * Drills out a regular neighbourhood of the given edge of the
         * triangulation.
         *
         * This is done by (i) performing two barycentric subdivisions,
         * (ii) removing all tetrahedra that touch the original edge,
         * and (iii) simplifying the resulting triangulation.
         *
         * \warning The second barycentric subdivision will multiply the
         * number of tetrahedra by 576; as a result this routine might
         * be slow, and the number of tetrahedra at the end might be
         * large (even taking the simplification into account).
         *
         * @param e the edge to drill out.
         */
        void drillEdge(Edge<3>* e);

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
        void puncture(Tetrahedron<3>* tet = 0);

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
         * \pre The given edge is a boundary edge of this triangulation,
         * and the two boundary triangles on either side of it are distinct.
         *
         * @param edge the boundary edge upon which to layer.
         * @return the new tetrahedron provided by the layering.
         */
        Tetrahedron<3>* layerOn(Edge<3>* edge);

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
         * \pre \a cuts1 is non-zero;
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
         * Inserts a new layered lens space L(p,q) into the triangulation.
         * The lens space will be created by gluing together two layered
         * solid tori in a way that uses the fewest possible tetrahedra.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \pre \a p \> \a q \>= 0 unless (<i>p</i>,<i>q</i>) = (0,1);
         * \pre gcd(\a p, \a q) = 1.
         *
         * @param p a parameter of the desired lens space.
         * @param q a parameter of the desired lens space.
         *
         * @see LayeredLensSpace
         */
        void insertLayeredLensSpace(unsigned long p, unsigned long q);
        /**
         * Inserts a layered loop of the given length into this triangulation.
         * Layered loops are described in more detail in the LayeredLoop
         * class notes.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * @param length the length of the new layered loop; this must
         * be strictly positive.
         * @param twisted \c true if the new layered loop should be twisted,
         * or \c false if it should be untwisted.
         *
         * @see LayeredLoop
         */
        void insertLayeredLoop(unsigned long length, bool twisted);
        /**
         * Inserts an augmented triangular solid torus with the given
         * parameters into this triangulation.  Almost all augmented
         * triangular solid tori represent Seifert fibred spaces with three
         * or fewer exceptional fibres.  Augmented triangular solid tori
         * are described in more detail in the AugTriSolidTorus class notes.
         *
         * The resulting Seifert fibred space will be
         * SFS((<i>a1</i>,<i>b1</i>) (<i>a2</i>,<i>b2</i>)
         * (<i>a3</i>,<i>b3</i>) (1,1)), where the parameters
         * <i>a1</i>, ..., <i>b3</i> are passed as arguments to this
         * routine.  The three layered solid tori that are attached to
         * the central triangular solid torus will be
         * LST(|<i>a1</i>|, |<i>b1</i>|, |-<i>a1</i>-<i>b1</i>|), ...,
         * LST(|<i>a3</i>|, |<i>b3</i>|, |-<i>a3</i>-<i>b3</i>|).
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
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
        void insertAugTriSolidTorus(long a1, long b1, long a2, long b2,
            long a3, long b3);
        /**
         * Inserts an orientable Seifert fibred space with at most three
         * exceptional fibres over the 2-sphere into this triangulation.
         *
         * The inserted Seifert fibred space will be
         * SFS((<i>a1</i>,<i>b1</i>) (<i>a2</i>,<i>b2</i>)
         * (<i>a3</i>,<i>b3</i>) (1,1)), where the parameters
         * <i>a1</i>, ..., <i>b3</i> are passed as arguments to this
         * routine.
         *
         * The three pairs of parameters (<i>a</i>,<i>b</i>) do not need
         * to be normalised, i.e., the parameters can be positive or
         * negative and <i>b</i> may lie outside the range [0..<i>a</i>).
         * There is no separate twisting parameter; each additional
         * twist can be incorporated into the existing parameters
         * by replacing some pair (<i>a</i>,<i>b</i>) with the pair
         * (<i>a</i>,<i>a</i>+<i>b</i>).  For Seifert fibred
         * spaces with less than three exceptional fibres, some or all
         * of the parameter pairs may be (1,<i>k</i>) or even (1,0).
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
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
        void insertSFSOverSphere(long a1 = 1, long b1 = 0,
            long a2 = 1, long b2 = 0, long a3 = 1, long b3 = 0);
        /**
         * Forms the connected sum of this triangulation with the given
         * triangulation.  This triangulation will be altered directly.
         *
         * If this and the given triangulation are both oriented, then
         * the result will be oriented also, and the connected sum will
         * respect these orientations.
         *
         * This and/or the given triangulation may be bounded or ideal, or
         * even invalid; in all cases the connected sum will be formed
         * correctly.  Note, however, that the result might possibly
         * contain internal vertices (even if the original triangulations
         * do not).
         *
         * \pre This triangulation is connected and non-empty.
         *
         * @param other the triangulation to sum with this.
         */
        void connectedSumWith(const Triangulation& other);
        /**
         * Inserts the rehydration of the given string into this triangulation.
         * If you simply wish to convert a dehydration string into a
         * new triangulation, use the static routine rehydrate() instead.
         * See dehydrate() for more information on dehydration strings.
         *
         * This routine will first rehydrate the given string into a proper
         * triangulation.  The tetrahedra from the rehydrated triangulation
         * will then be inserted into this triangulation in the same order in
         * which they appear in the rehydrated triangulation, and the
         * numbering of their vertices (0-3) will not change.
         *
         * The routine dehydrate() can be used to extract a dehydration
         * string from an existing triangulation.  Dehydration followed
         * by rehydration might not produce a triangulation identical to
         * the original, but it is guaranteed to produce an isomorphic
         * copy.  See dehydrate() for the reasons behind this.
         *
         * For a full description of the dehydrated triangulation
         * format, see <i>A Census of Cusped Hyperbolic 3-Manifolds</i>,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * @param dehydration a dehydrated representation of the
         * triangulation to insert.  Case is irrelevant; all letters
         * will be treated as if they were lower case.
         * @return \c true if the insertion was successful, or
         * \c false if the given string could not be rehydrated.
         *
         * @see dehydrate
         * @see rehydrate
         */
        bool insertRehydration(const std::string& dehydration);

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
         *
         * @see rehydrate
         * @see insertRehydration
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
         * - If you are calling this from one of Regina's own Triangulation<3>
         *   objects, then only the tetrahedron gluings and the manifold name
         *   will be stored (the name will be derived from the packet label).
         *   All other SnapPea-specific information (such as peripheral curves)
         *   will be marked as unknown (since Regina does not track such
         *   information itself), and of course other Regina-specific
         *   information (such as the Turaev-Viro invariants) will not
         *   be written to the SnapPea file at all.
         *
         * - If you are calling this from the subclass SnapPeaTriangulation,
         *   then all additional SnapPea-specific information will be written
         *   to the file (indeed, the SnapPea kernel itself will be used to
         *   produce the file contents).
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
        virtual std::string snapPea() const;

        /**
         * Writes the full contents of a SnapPea data file describing this
         * triangulation to the given output stream.
         *
         * Regarding what gets stored in the SnapPea data file:
         *
         * - If you are calling this from one of Regina's own Triangulation<3>
         *   objects, then only the tetrahedron gluings and the manifold name
         *   will be stored (the name will be derived from the packet label).
         *   All other SnapPea-specific information (such as peripheral curves)
         *   will be marked as unknown (since Regina does not track such
         *   information itself), and of course other Regina-specific
         *   information (such as the Turaev-Viro invariants) will not
         *   be written to the SnapPea file at all.
         *
         * - If you are calling this from the subclass SnapPeaTriangulation,
         *   then all additional SnapPea-specific information will be written
         *   to the file (indeed, the SnapPea kernel itself will be used to
         *   produce the file contents).
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
         * \ifacespython Not present.
         *
         * @param out the output stream to which the SnapPea data file
         * will be written.
         */
        virtual void snapPea(std::ostream& out) const;

        /**
         * Writes this triangulation to the given file using SnapPea's
         * native file format.
         *
         * Regarding what gets stored in the SnapPea data file:
         *
         * - If you are calling this from one of Regina's own Triangulation<3>
         *   objects, then only the tetrahedron gluings and the manifold name
         *   will be stored (the name will be derived from the packet label).
         *   All other SnapPea-specific information (such as peripheral curves)
         *   will be marked as unknown (since Regina does not track such
         *   information itself), and of course other Regina-specific
         *   information (such as the Turaev-Viro invariants) will not
         *   be written to the SnapPea file at all.
         *
         * - If you are calling this from the subclass SnapPeaTriangulation,
         *   then all additional SnapPea-specific information will be written
         *   to the file (indeed, the SnapPea kernel itself will be used to
         *   produce the file contents).
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
        virtual bool saveSnapPea(const char* filename) const;

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
         * \ifacespython Not present.
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
         * \ifacespython Not present.
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
         * Allows the user to interactively enter a triangulation in
         * plain text.  Prompts will be sent to the given output stream
         * and information will be read from the given input stream.
         *
         * \ifacespython This routine is a member of class Engine.
         * It takes no parameters; \a in and \a out are always assumed
         * to be standard input and standard output respectively.
         *
         * @param in the input stream from which text will be read.
         * @param out the output stream to which prompts will be
         * written.
         * @return the triangulation entered in by the user.
         */
        static Triangulation<3>* enterTextTriangulation(std::istream& in,
                std::ostream& out);
        /**
         * Rehydrates the given alphabetical string into a new triangulation.
         * See dehydrate() for more information on dehydration strings.
         *
         * This routine will rehydrate the given string into a new
         * triangulation, and return this new triangulation.
         *
         * The converse routine dehydrate() can be used to extract a
         * dehydration string from an existing triangulation.  Dehydration
         * followed by rehydration might not produce a triangulation identical
         * to the original, but it is guaranteed to produce an isomorphic
         * copy.  See dehydrate() for the reasons behind this.
         *
         * For a full description of the dehydrated triangulation
         * format, see <i>A Census of Cusped Hyperbolic 3-Manifolds</i>,
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999.
         *
         * @param dehydration a dehydrated representation of the
         * triangulation to construct.  Case is irrelevant; all letters
         * will be treated as if they were lower case.
         * @return a newly allocated triangulation if the rehydration was
         * successful, or null if the given string could not be rehydrated.
         *
         * @see dehydrate
         * @see insertRehydration
         */
        static Triangulation<3>* rehydrate(const std::string& dehydration);

        /**
         * Extracts the tetrahedron gluings from a string that contains the
         * full contents of a SnapPea data file.  All other SnapPea-specific
         * information (such as peripheral curves) will be ignored, since
         * Regina's Triangulation<3> class does not track such information itself.
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
         * The triangulation that is returned will be newly created.
         * If the SnapPea data is not in the correct format, this
         * routine will return 0 instead.
         *
         * \warning This routine is "lossy", in that drops SnapPea-specific
         * information (as described above).  Unless you specifically need an
         * Triangulation<3> (not an SnapPeaTriangulation) or you need to avoid
         * calling routines from the SnapPea kernel, it is highly recommended
         * that you create a SnapPeaTriangulation from the given file
         * contents instead.  See the string-based SnapPeaTriangulation
         * constructor for how to do this.
         *
         * @param snapPeaData a string containing the full contents of a
         * SnapPea data file.
         * @return a new triangulation extracted from the given data,
         * or 0 on error.
         */
        static Triangulation<3>* fromSnapPea(const std::string& snapPeaData);

        /*@}*/

        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        void clearAllProperties();
        /**
         * Swaps all calculated properties, including skeletal data,
         * with the given triangulation.  This is called by
         * TriangulationBase::swapContents(), and by nothing else.
         */
        void swapAllProperties(Triangulation<3>& other);

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

        /**
         * A non-templated version of retriangulate().
         *
         * This is identical to retriangulate(), except that the action
         * function is given in the form of a std::function.
         * This routine contains the bulk of the implementation of
         * retriangulate().
         *
         * Because this routine is not templated, its implementation
         * can be kept out of the main headers.
         */
        bool retriangulateInternal(int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            const std::function<bool(Triangulation<3>&)>& action) const;

        void stretchBoundaryForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInBoundary(). */
        bool stretchForestFromVertex(Vertex<3>*, std::set<Edge<3>*>&,
                std::set<Vertex<3>*>&, std::set<Vertex<3>*>&) const;
            /**< Internal to maximalForestInSkeleton(). */

        /**
         * Reads the contents of a SnapPea data file from the given input
         * stream, and converts the result to a new Triangulation<3>.  Since
         * this returns a Triangulation<3>, it will lose some SnapPea-specific
         * information in the process (such as peripheral curves).
         *
         * If the input stream could not be read or if the data was not in the
         * correct format, 0 will be returned.  Otherwise a newly allocated
         * triangulation will be returned, and it is the user's responsibility
         * to deallocate this when it is finished with.
         *
         * Unlike the SnapPeaTriangulation constructor, this routine uses
         * Regina's own SnapPea input code - it does not call any functions
         * from the SnapPea kernel.
         *
         * \ifacespython Not present.
         */
        static Triangulation<3>* readSnapPea(std::istream& in);

    friend class regina::Face<3, 3>;
    friend class regina::detail::SimplexBase<3>;
    friend class regina::detail::TriangulationBase<3>;
    friend class regina::XMLTriangulationReader<3>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NTriangulation, you should use
 * the templated class name Triangulation<3>.
 */
REGINA_DEPRECATED typedef Triangulation<3> NTriangulation;

// Additional face typedefs that do not have their own headers:

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NEdgeEmbedding, you should
 * use either the new alias EdgeEmbedding<3>, or the full class name
 * FaceEmbedding<3, 1>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<3, 1> NEdgeEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NEdge, you should use
 * either the new alias Edge<3>, or the full class name Face<3, 1>.
 */
REGINA_DEPRECATED typedef Face<3, 1> NEdge;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/dim3/tetrahedron3.h"
#include "triangulation/dim3/triangle3.h"
#include "triangulation/dim3/vertex3.h"
#include "triangulation/dim3/component3.h"
#include "triangulation/dim3/boundarycomponent3.h"
namespace regina {

// Inline functions for Triangulation<3>

inline Triangulation<3>::Triangulation() {
}

inline Triangulation<3>::Triangulation(const Triangulation<3>& copy) :
        Triangulation<3>(copy, true) {
}

inline Triangulation<3>::~Triangulation() {
    clearAllProperties();
}

inline Packet* Triangulation<3>::internalClonePacket(Packet*) const {
    return new Triangulation<3>(*this);
}

inline bool Triangulation<3>::dependsOnParent() const {
    return false;
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

inline bool Triangulation<3>::hasTwoSphereBoundaryComponents() const {
    if (! twoSphereBoundaryComponents_.known())
        calculateBoundaryProperties();
    return twoSphereBoundaryComponents_.value();
}

inline bool Triangulation<3>::hasNegativeIdealBoundaryComponents() const {
    if (! negativeIdealBoundaryComponents_.known())
        calculateBoundaryProperties();
    return negativeIdealBoundaryComponents_.value();
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

inline bool Triangulation<3>::knowsZeroEfficient() const {
    return zeroEfficient_.known();
}

inline bool Triangulation<3>::knowsSplittingSurface() const {
    return splittingSurface_.known();
}

inline bool Triangulation<3>::hasStrictAngleStructure() const {
    if (! strictAngleStructure_.known())
        findStrictAngleStructure();
    return (strictAngleStructure_.value() != 0);
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
    return retriangulateInternal(height, nThreads, tracker,
        std::bind(action, std::placeholders::_1, args...));
}

inline const TreeDecomposition& Triangulation<3>::niceTreeDecomposition()
        const {
    if (niceTreeDecomposition_.known())
        return *niceTreeDecomposition_.value();

    TreeDecomposition* ans = new TreeDecomposition(*this, TD_UPPER);
    ans->makeNice();
    return *(niceTreeDecomposition_ = ans);
}

inline void Triangulation<3>::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << simplices_.size()
        << (simplices_.size() == 1 ? " tetrahedron" : " tetrahedra");
}

inline void Triangulation<3>::recognizer(std::ostream& out) const {
    recogniser(out);
}

inline bool Triangulation<3>::saveRecognizer(const char* filename) const {
    return saveRecogniser(filename);
}

} // namespace regina

#endif

