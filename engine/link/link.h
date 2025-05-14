
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file link/link.h
 *  \brief Deals with classical and virtual knots and links.
 */

#ifndef __REGINA_LINK_H
#ifndef __DOXYGEN
#define __REGINA_LINK_H
#endif

#include <array>
#include <functional>
#include <optional>
#include <vector>
#include "regina-core.h"
#include "algebra/grouppresentation.h"
#include "maths/integer.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "packet/packet.h"
#include "progress/progresstracker.h"
#include "treewidth/treedecomposition.h"
#include "triangulation/dim3.h"
#include "triangulation/detail/retriangulate.h"
#include "utilities/exception.h"
#include "utilities/fixedarray.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"
#include "utilities/tightencoding.h"
#include "utilities/topologylock.h"

// Note: there are more includes after the main class definitions.

namespace regina {

class Crossing;
class Link;
class ModelLinkGraph;
class ProgressTracker;
class Tangle;
template <typename T> class Laurent;
template <typename T> class Laurent2;
template <int> class Triangulation;

/**
 * \defgroup link Knots and Links
 * Classical and virtual knots and links
 */

/**
 * Indicates one of the standard framings of a knot or link.
 * Here a _framing_ refers to a choice of normal vector field along the
 * knot or link.  Equivalently, a framing refers to a choice of longitude
 * on the torus bounding each component of the link.
 *
 * \ingroup link
 */
enum class Framing {
    /**
     * Indicates the _Seifert framing_, which is defined algebraically and is
     * independent of the knot/link projection.
     *
     * There are several ways in which the Seifert framing can be defined.
     * One simple definition that works for both classical and virtual links
     * is this: for each component of the link, the Seifert framing chooses
     * the unique longitude for the corresponding knot that has linking number
     * zero with the knot itself.
     *
     * Some alternative definitions for classical links:
     *
     * - For each component of the link, draw a Seifert surface (i.e., an
     *   orientable surface embedded in the 3-sphere that is bounded by the
     *   corresponding knot).  The Seifert framing is the vector field
     *   that points into the corresponding surface.
     *
     * - For each component of the link, the Seifert framing chooses the
     *   unique longitude for the corresponding knot that is trivial in the
     *   homology of the knot complement.
     */
    Seifert = 1,
    /**
     * Indicates the _blackboard framing_, which is specific to the
     * knot/link projection.
     *
     * For the blackboard framing, the normal vector field stays within
     * the projection surface.  Equivalently, the blackboard framing chooses
     * longitudes whose projections do not intersect the original link diagram.
     */
    Blackboard = 2
};

/**
 * A deprecated constant indicating one of the standard link framings.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Framing::Seifert.
 */
[[deprecated]] inline constexpr Framing FRAMING_SEIFERT = Framing::Seifert;
/**
 * A deprecated constant indicating one of the standard link framings.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Framing::Blackboard.
 */
[[deprecated]] inline constexpr Framing FRAMING_BLACKBOARD =
    Framing::Blackboard;

/**
 * A reference to one of the two strands of a link that pass each other
 * at a crossing.
 *
 * This class is a simple wrapper that stores (i) a pointer to the
 * relevant crossing; and (ii) an integer to denote whether this refers
 * to the upper strand (which passes over the crossing) or the lower strand
 * (which passes under the crossing).
 *
 * A "null reference" is one whose crossing is the null pointer.
 *
 * This class can also be used to refer to an _arc_ of a link; that is,
 * a section of the link that runs from one crossing to the next.
 * When used in this way:
 *
 * - The arc referred to is the arc beginning at the given strand of the
 *   given crossing, and moving forwards along the orientation of the link
 *   to the next crossing.
 *
 * - A null reference is used to refer to an entire zero-crossing unknot
 *   component (as opposed to a typical arc which has well-defined start
 *   and end points).  Of course the link in question may not have such a
 *   component, or may have many such components; the behaviour of the
 *   code in such circumstances is documented in the individual routines
 *   that take arc references as arguments.
 *
 * - The increment and decrement operators, as well as next() and prev(),
 *   behave as expected: they follow the link forward and backward
 *   respectively along its orientation.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup link
 */
class StrandRef {
    private:
        Crossing* crossing_;
            /**< The relevant crossing.  This may be null. */
        int strand_;
            /**< Either 0 for the lower strand, or 1 for the upper strand. */

    public:
        /**
         * Initialises this to a null reference.
         *
         * The pointer returned by crossing() will be \c null,
         * and the integer returned by strand() will be 0.
         */
        StrandRef();

        /**
         * Initialises this to the given strand of the given crossing.
         *
         * The given crossing may be \c null, in which case this will
         * become a null reference.  If you are creating a null reference,
         * then it is highly recommended that you pass \a strand as 0,
         * so that comparison tests treat this null reference as equal to
         * a null reference created by the zero-argument constructor.
         *
         * \param crossing the crossing being identified.
         * \param strand 0 to denote the strand running under the crossing, or
         * 1 to denote the strand running over the crossing.
         */
        StrandRef(Crossing* crossing, int strand);

        /**
         * Creates a new copy of the given strand reference.
         */
        StrandRef(const StrandRef&) = default;

        /**
         * The crossing that this reference points to.
         *
         * The information returned by crossing() and strand() together
         * pinpoint exactly which strand of the link this reference points to.
         *
         * \return the crossing, or \c null if this is a null reference.
         */
        Crossing* crossing() const;

        /**
         * Indicates whether this reference points to the upper or lower
         * strand of the relevant crossing.
         *
         * A value of 1 denotes the upper strand (which passes over the
         * crossing), and a value of 0 denotes the lower strand (which
         * passes under the crossing).
         *
         * The information returned by crossing() and strand() together
         * pinpoint exactly which strand of the link this reference points to.
         *
         * \return either 0 or 1 to indicate the strand.
         */
        int strand() const;

        /**
         * An integer that uniquely identifies this strand within the link.
         *
         * This integer will be 2<i>c</i>+<i>s</i>, where \e c is the
         * index of the crossing, and \e s is 0 or 1 for the lower or
         * upper strand respectively.
         *
         * If this is a null reference, then id() will return -1.
         *
         * A strand can be restored from its ID by calling Link::strand().
         *
         * \return the unique ID of this strand within the link.
         */
        ssize_t id() const;

        /**
         * Tests whether this and the given reference are identical.
         *
         * Two references are identical if and only if they return the
         * same values for both crossing() and strand().
         *
         * \warning If you create a null reference by calling
         * StrandRef(\c null, 1) then this will _not_ be considered
         * equal to the null reference created by calling StrandRef(),
         * since the latter is equivalent to calling StrandRef(\c null, 0).
         *
         * \return \c true if and only if this and the given reference are
         * identical.
         */
        bool operator == (const StrandRef&) const = default;
        /**
         * Sets this to be a copy of the given strand reference.
         *
         * \return a reference to this object.
         */
        StrandRef& operator = (const StrandRef&) = default;

        /**
         * Moves this reference forward along the direction of the link
         * until it reaches the next crossing.  (Of course, if the link
         * contains a trivial twist then this may in fact return to the
         * same crossing but the other strand).
         *
         * This is a preincrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \nopython The postincrement operator is available in Python
         * under the name inc().
         *
         * \return a reference to this object.
         */
        StrandRef& operator ++ ();
        /**
         * Moves this reference forward along the direction of the link
         * until it reaches the next crossing.  (Of course, if the link
         * contains a trivial twist then this may in fact return to the
         * same crossing but the other strand).
         *
         * This is a postincrement operator: the object will be changed,
         * but a copy of the original reference will be returned.
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \python This routine is available under the name inc().
         *
         * \return a copy of this object before the change took place.
         */
        StrandRef operator ++ (int);
        /**
         * Moves this reference backward against the direction of the link
         * until it reaches the previous crossing.  (Of course, if the link
         * contains a trivial twist then this may in fact return to the
         * same crossing but the other strand).
         *
         * This is a preincrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \nopython The postincrement operator is available in Python
         * under the name dec().
         *
         * \return a reference to this object.
         */
        StrandRef& operator -- ();
        /**
         * Moves this reference backward against the direction of the link
         * until it reaches the previous crossing.  (Of course, if the link
         * contains a trivial twist then this may in fact return to the
         * same crossing but the other strand).
         *
         * This is a postincrement operator: the object will be changed,
         * but a copy of the original reference will be returned.
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \python This routine is available under the name dec().
         *
         * \return a copy of this object before the change took place.
         */
        StrandRef operator -- (int);

        /**
         * Returns the crossing reference that comes immediately after
         * this when walking forward along the direction of the link.
         *
         * Equivalently, this routine returns the reference that would
         * be obtained by calling the increment (++) operator (but, unlike
         * the increment operator, this routine does not actually change
         * the current reference).
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \return the crossing reference that follows this.
         */
        StrandRef next() const;
        /**
         * Returns the crossing reference that comes immediately before
         * this when walking backward against the direction of the link.
         *
         * Equivalently, this routine returns the reference that would
         * be obtained by calling the decrement (--) operator (but, unlike
         * the decrement operator, this routine does not actually change
         * the current reference).
         *
         * \pre This is not a null reference, i.e., crossing() does not
         * return \c null.
         *
         * \return the crossing reference that precedes this.
         */
        StrandRef prev() const;

        /**
         * Jumps to the other strand at the same crossing.
         *
         * This reference will be changed directly.  The crossing will
         * remain the same, but the strand will switch from lower to
         * upper or vice versa.
         */
        void jump();

        /**
         * Tests whether this is a non-null reference.
         *
         * \return \c true if this is not a null reference (i.e., crossing()
         * does not return a null pointer), or \c false if this is a null
         * reference.
         */
        operator bool() const;

    friend class Link;
    friend class ModelLinkGraph;
    friend class Tangle;
};

/**
 * Writes a depiction of the given strand reference to the given output stream.
 *
 * The reference will be written in the form `^n` or `_n`,
 * denoting the upper or lower strand at crossing \a n respectively.
 * For example, the upper strand of crossing 7 will be written as `^7`.
 *
 * \param out the output stream to which to write.
 * \param s the reference to write.
 * \return a reference to the given output stream.
 *
 * \ingroup link
 */
std::ostream& operator << (std::ostream& out, const StrandRef& s);

/**
 * Represents a single crossing in a link diagram.
 * The two strands of the link that run over and under the crossing
 * respectively can be accessed through routines such as over(), under(),
 * upper(), lower(), and strand().
 *
 * Each crossing has a sign, which is either positive (denoted by +1)
 * or negative (denoted by -1):
 *
 * - In a positive crossing, the upper strand passes over the lower strand
 *   from left to right:
     \verbatim
     -----\ /----->
           \
     -----/ \----->
     \endverbatim
 *
 * - In a negative crossing, the upper strand passes over the lower strand
 *   from right to left:
     \verbatim
     -----\ /----->
           /
     -----/ \----->
     \endverbatim
 *
 * If a link has \a n crossings, then these are numbered 0,...,<i>n</i>-1.
 * The number assigned to this crossing can be accessed by calling index().
 * Note that crossings in a link may be reindexed when other crossings are
 * added or removed - if you wish to track a particular crossing through
 * such operations then you should use a pointer to the relevant Crossing
 * object instead.
 *
 * Crossings do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Link to which they belong.
 *
 * \ingroup link
 */
class Crossing : public MarkedElement, public ShortOutput<Crossing> {
    private:
        int sign_;
            /**< The sign of the crossing, which must be +1 or -1.
                 As an exception, the sign may be 0 whilst the link is
                 being constructed, as an indicator that the sign has
                 not yet been determined. */

        StrandRef next_[2];
            /**< next_[0] and next_[1] store the crossing references that
                 come immediately after this crossing when walking forward
                 along the lower and upper strands respectively. */
        StrandRef prev_[2];
            /**< prev_[0] and prev_[1] store the crossing references that
                 come immediately before this crossing when walking backward
                 along the lower and upper strands respectively. */

    public:
        /**
         * Returns the index of this crossing within the overall link.
         * If the link contains \a n crossings, then the index will be a
         * number between 0 and <i>n</i>-1 inclusive.
         *
         * \warning The index of this crossing might change if other
         * crossings are added or removed.
         *
         * \return the index of this crossing.
         */
        size_t index() const;
        /**
         * Returns the sign of this crossing.  This will be +1 for a
         * positive crossing, or -1 for a negative crossing.
         *
         * See the Crossing class notes for diagrams of positive and
         * negative crossings
         *
         * \return the sign of this crossing.
         */
        int sign() const;
        /**
         * Returns a reference to the strand running over this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 1).
         *
         * Note that upper() and over() are synonyms.
         *
         * \return a reference to the upper strand for this crossing.
         */
        StrandRef upper();
        /**
         * Returns a reference to the strand running under this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 0).
         *
         * Note that lower() and under() are synonyms.
         *
         * \return a reference to the lower strand for this crossing.
         */
        StrandRef lower();
        /**
         * Returns a reference to the strand running over this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 1).
         *
         * Note that upper() and over() are synonyms.
         *
         * \return a reference to the upper strand for this crossing.
         */
        StrandRef over();
        /**
         * Returns a reference to the strand running under this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 0).
         *
         * Note that lower() and under() are synonyms.
         *
         * \return a reference to the lower strand for this crossing.
         */
        StrandRef under();
        /**
         * Returns a reference to one of the two strands of the link that
         * pass each other at this crossing.  This is equivalent to directly
         * constructing StrandRef(\c this, \a which).
         *
         * Note that upper() and over() are synonyms for strand(1), and
         * lower() and under() are synonyms for strand(0).
         *
         * \param which either 1 to indicate the upper strand,
         * or 0 to indicate the lower strand.
         * \return a reference to the given strand at this crossing.
         */
        StrandRef strand(int which);
        /**
         * Returns the crossing reference that immediately follows this when
         * walking forward in the direction of the link along one of the two
         * strands that pass at this crossing.  Which strand we follow
         * is indicated by the argument \a strand.
         *
         * Note that for a crossing \a c, calling `c.next(s)`
         * is equivalent to calling `c.strand(s).next()`.
         *
         * \param strand either 1 to walk forward along the upper strand,
         * or 0 to walk forward along the lower strand.
         * \return a reference to the next crossing after this along the
         * given strand.
         */
        StrandRef next(int strand) const;
        /**
         * Returns the crossing reference that immediately precedes this when
         * walking backward against the direction of the link along one of the
         * two strands that pass at this crossing.  Which strand we
         * follow is indicated by the argument \a strand.
         *
         * Note that for a crossing \a c, calling `c.prev(s)`
         * is equivalent to calling `c.strand(s).prev()`.
         *
         * \param strand either 1 to walk backward along the upper strand,
         * or 0 to walk backward along the lower strand.
         * \return a reference to the previous crossing before this along the
         * given strand.
         */
        StrandRef prev(int strand) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Make this class non-copyable.
        Crossing(const Crossing&) = delete;
        Crossing& operator = (const Crossing&) = delete;

    private:
        /**
         * Initialises the \a next_ and \a prev_ arrays to contain null
         * references, and marks the sign of this crossing as 0
         * (which indicates an unknown sign).
         */
        Crossing();

        /**
         * Initialises the \a next_ and \a prev_ arrays to contain null
         * references, and sets the sign of this crossing according to
         * the given argument.
         *
         * \param sign the sign of this crossing; this must be +1 or -1.
         */
        Crossing(int sign);

    friend class Link;
    friend class ModelLinkGraph;
    friend class Tangle;
    friend class XMLLinkCrossingsReader;
    friend class XMLLinkConnectionsReader;
};

/**
 * Represents a combinatorial diagram of a directed knot or link.
 *
 * Regina uses the word _link_ to refer to links with any number of components,
 * including knots (which have exactly one component) and the empty link
 * (which has no components at all).
 *
 * Since Regina 7.4, this class supports both classical and virtual links:
 *
 * - A _classical_ link is a link in the 3-sphere (i.e., the type of link that
 *   one might typically read about in an undergraduate topology course).
 *   Classical links are considered equivalent under ambient isotopy.
 *
 * - A _virtual_ link is a link in some thickened orientable surface \a S.
 *   Virtual links are considered equivalent under ambient isotopy,
 *   orientation-preserving homeomorphisms of \a S, and the addition and/or
 *   removal of empty handles from \a S.
 *
 * This class stores a purely combinatorial representation of a 2-dimensional
 * link diagram, using just the combinatorics of the classical crossings and
 * the connections between them.  In particular:
 *
 * - The Link class does not store any geometric information about the
 *   specific placement of strands or crossings in the ambient 3-dimensional
 *   space.
 *
 * - For classical links, you can visualise a link using the SpatialLink class,
 *   which stores a specific embedding of the link in 3-dimensional Euclidean
 *   space, but which is based on floating-point arithmetic (and is therefore
 *   susceptible to floating-point errors).  For most mathematical purposes
 *   however, you should use this Link class, which has a rich set of
 *   mathematical features and uses exact discrete algorithms.
 *
 * - For virtual links, some authors like to use diagrams in the plane with
 *   "virtual crossings".  Regina does not use virtual crossings at all;
 *   instead it stores only the classical crossings in the thickened surface
 *   (where one strand passes over another).  Regina also does not store the
 *   surface itself; instead it uses the (unique) surface of smallest possible
 *   genus in which this diagram embeds (i.e., the surface in which the diagram
 *   embeds with no empty handles).  Put differently: Regina treats the
 *   crossings and strands of this diagram as defining a local embedding of the
 *   1-skeleton of some polygonal decomposition of the surface; the 2-cells of
 *   this decomposition are then assumed to be topological discs.
 *
 * This Link class supports links with any number of components (including
 * zero), and it also supports components with no crossings (which form
 * additional unknot components of the overall link).
 *
 * Since Regina 7.0, this is no longer a "packet type" that can be
 * inserted directly into the packet tree.  Instead a Link is now a
 * standalone mathematatical object, which makes it slimmer and faster
 * for ad-hoc use.  The consequences of this are:
 *
 * - If you create your own Link, it will not have any of the usual packet
 *   infrastructure.  You cannot add it into the packet tree, and it will
 *   not support a label, tags, child/parent packets, and/or event listeners.
 *
 * - To include a Link in the packet tree, you must create a new
 *   PacketOf<Link>.  This _is_ a packet type, and supports labels, tags,
 *   child/parent packets, and event listeners.  It derives from Link,
 *   and so inherits the full Link interface.
 *
 * If you are adding new functions to this class that edit the internal data
 * structures of the link, you must remember to surround these changes with
 * a ChangeAndClearSpan.  This manages bookkeeping such as clearing computed
 * properties, and (if this link _does_ belong to a packet) firing packet
 * change events.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class Link :
        public PacketData<Link>,
        public Output<Link>,
        public TightEncodable<Link>,
        protected TopologyLockable {
    private:
        MarkedVector<Crossing> crossings_;
            /**< The crossings in this link. */
        std::vector<StrandRef> components_;
            /**< For each component of the link, this array contains
                 a reference to one of its strands (which can then be
                 used to traverse the component).  If a component has no
                 crossings, then it is represented in this array by a
                 null reference. */

        mutable ssize_t virtualGenus_;
            /**< The virtual genus of the link diagram, or -1 if this has not
                 yet been computed. */
        mutable std::optional<Polynomial<Integer>> alexander_;
            /**< The Alexander polynomial of the link.
                 This is std::nullopt if it has not yet been computed,
                 or if this link does not have exactly one component. */
        mutable std::optional<Laurent<Integer>> jones_;
            /**< The Jones polynomial of the link.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent2<Integer>> homflyLM_;
            /**< The HOMFLY-PT polynomial of the link, as a polynomial in
                 \a l and \a m.  This property will be known if and only
                 if \a homflyAZ_ is known.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent2<Integer>> homflyAZ_;
            /**< The HOMFLY-PT polynomial of the link, as a polynomial in
                 \a alpha and \a z.  This property will be known if and
                 only if \a homflyLM_ is known.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent<Integer>> bracket_;
            /**< The Kauffman bracket polynomial of the link diagram.
                 This is std::nullopt if it has not yet been computed. */

        mutable std::optional<TreeDecomposition> niceTreeDecomposition_;
            /**< A nice tree decomposition of the 4-valent multigraph
                 formed by the link diagram.
                 This is std::nullopt if it has not yet been computed. */

    public:
        /**
         * The name of the variable used in the Alexander polynomial, as
         * returned by alexander().  This is provided to help with
         * pretty-printing Alexander polynomials for human consumption.
         *
         * To pretty-print the Alexander polynomial for human consumption,
         * you can call `Laurent::str(Link::alexanderVar)`.
         */
        static constexpr const char* alexanderVar = "t";

        /**
         * The name of the variable used in the Jones polynomial, as returned
         * by jones().  This is provided to help with pretty-printing
         * Jones polynomials for human consumption.
         *
         * Since jones() returns a Laurent polynomial in the square root
         * of \a t, this string is just a human-readable representation
         * of the square root of \a t (encoded in UTF-8).
         *
         * To pretty-print the Jones polynomial for human consumption,
         * you can call `Laurent::str(Link::jonesVar)`.
         */
        static constexpr const char* jonesVar = "\u221At"; // \u221A = root

        /**
         * The name of the first variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homflyAZ().  This is provided to help with
         * pretty-printing HOMFLY-PT polynomials for human consumption.
         *
         * Since homflyAZ() returns a Laurent polynomial in \a alpha and \a z,
         * this string just contains the mathematical symbol \a alpha
         * (encoded in UTF-8).
         *
         * To pretty-print this HOMFLY-PT polynomial for human consumption, you
         * can call `Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)`.
         */
        static constexpr const char* homflyAZVarX = "\u03B1"; // alpha

        /**
         * The name of the second variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homflyAZ().  This is provided to help with
         * pretty-printing HOMFLY-PT polynomials for human consumption.
         *
         * Since homflyAZ() returns a Laurent polynomial in \a alpha and \a z,
         * this string just contains the single character \a z.
         *
         * To pretty-print this HOMFLY-PT polynomial for human consumption, you
         * can call `Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)`.
         */
        static constexpr const char* homflyAZVarY = "z";

        /**
         * The name of the first variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homflyLM().  This is provided to help with
         * pretty-printing HOMFLY-PT polynomials for human consumption.
         *
         * Since homflyLM() returns a Laurent polynomial in \a l and \a m,
         * this string just contains the mathematical script symbol for \a l
         * (encoded in UTF-8).
         *
         * To pretty-print this HOMFLY-PT polynomial for human consumption, you
         * can call `Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)`.
         */
        static constexpr const char* homflyLMVarX = "\U0001D4C1"; // $\ell$

        /**
         * The name of the second variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homflyLM().  This is provided to help with
         * pretty-printing HOMFLY-PT polynomials for human consumption.
         *
         * Since homflyLM() returns a Laurent polynomial in \a l and \a m,
         * this string just contains the single character \a m.
         *
         * To pretty-print this HOMFLY-PT polynomial for human consumption, you
         * can call `Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)`.
         */
        static constexpr const char* homflyLMVarY = "m";

        /**
         * The name of the first variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homfly().
         *
         * This is simply an alias for homflyAZVarX.  See the documentation
         * for homflyAZVarX for further details.
         */
        static constexpr const char* homflyVarX = homflyAZVarX;

        /**
         * The name of the second variable used in the variant of the HOMFLY-PT
         * polynomial as returned by homfly().
         *
         * This is simply an alias for homflyAZVarY.  See the documentation
         * for homflyAZVarY for further details.
         */
        static constexpr const char* homflyVarY = homflyAZVarY;

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Constructs an empty link.  This will have zero components.
         */
        Link();
        /**
         * Constructs the unlink with the given number of components.
         *
         * \param unknots the number of (unknotted) components in the
         * new unlink.
         */
        Link(size_t unknots);
        /**
         * Constructs a new copy of the given link.
         *
         * This will clone any computed properties (such as Jones
         * polynomial and so on) of the given link also.  If you want a
         * "clean" copy that resets all properties to unknown, you can
         * use the two-argument copy constructor instead.
         *
         * \param copy the link to copy.
         */
        Link(const Link& copy);
        /**
         * Constructs a new copy of the given link, with the option of
         * whether or not to clone its computed properties also.
         *
         * \param copy the link to copy.
         * \param cloneProps \c true if this should also clone any
         * computed properties of the given link (such as Jones
         * polynomial and so on), or \c false if the new link should
         * have all properties marked as unknown.
         */
        Link(const Link& copy, bool cloneProps);
        /**
         * Moves the given link into this new link.
         * This is a fast (constant time) operation.
         *
         * All crossings that belong to \a src will be moved into this link,
         * and so any Crossing pointers or StrandRef object will remain valid.
         * Likewise, all cached properties will be moved into this link.
         *
         * The link that is passed (\a src) will no longer be usable.
         *
         * \note This operator is marked \c noexcept, and in particular
         * does not fire any change events.  This is because this link
         * is freshly constructed (and therefore has no listeners yet), and
         * because we assume that \a src is about to be destroyed (an action
         * that _will_ fire a packet destruction event).
         *
         * \param src the link to move.
         */
        Link(Link&& src) noexcept;
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a link.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - knot/link signatures, as used by fromSig();
         * - oriented Gauss codes, as used by fromOrientedGauss();
         * - signed Gauss codes, as used by fromSignedGauss();
         * - classical Gauss codes, as used by fromGauss();
         * - numeric or alphabetical Dowker-Thistlethwaite strings, as
         *   used by fromDT();
         * - planar diagram codes, as used by fromPD().
         *
         * This list may grow in future versions of Regina.
         *
         * \exception InvalidArgument Regina could not interpret the given
         * string as representing a link using any of the supported string
         * types.
         *
         * \param description a string that describes a knot or link.
         */
        Link(const std::string& description);

        /**
         * Destroys this link.
         *
         * The Crossing objects contained in this link will also be destroyed.
         */
        ~Link();

        /*@}*/
        /**
         * \name Crossings and Components
         */
        /*@{*/

        /**
         * Determines whether this link is empty.
         * An empty link is one with no components at all.
         *
         * \return \c true if and only if this link is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the number of crossings in this link.
         *
         * Note that a link can have more components than crossings
         * (since it may contain additional zero-crossing unknot components).
         *
         * \return the number of crossings.
         */
        size_t size() const;

        /**
         * Returns the number of components in this link.
         *
         * This is the number of circles embedded in the ambient 3-manifold
         * (it has nothing to do with the connectivity of the link diagram).
         * So, for example, the number of components in the Hopf link is two.
         *
         * \return the number of components.
         */
        size_t countComponents() const;

        /**
         * Returns a pointer to the crossing at the given index within
         * this link.
         *
         * For a link with \a n crossings, the crossings are numbered
         * from 0 to <i>n</i>-1 inclusive.
         *
         * \warning If some crossings are added or removed then the indices
         * of other crossings might change.  If you wish to track a particular
         * crossing through such operations then you should use the pointer
         * to the relevant Crossing object instead.
         *
         * \param index the index of the requested crossing.  This must
         * be between 0 and size()-1 inclusive.
         * \return the crossing at the given index.
         */
        Crossing* crossing(size_t index) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all crossings within this link.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (Crossing* c : link.crossings()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for as
         * long as the link exists: even as crossings are added and/or removed,
         * it will always reflect the crossings that are currently in the link.
         * Nevertheless, it is recommended to treat this object as temporary
         * only, and to call crossings() again each time you need it.
         *
         * \return access to the list of all crossings.
         */
        auto crossings() const;

        /**
         * Returns a strand in the given component of this link.
         *
         * Components are individual circles embedded in the ambient 3-manifold
         * (they have nothing to do with the connectivity of the link diagram).
         * So, for example, the Hopf link has two components.
         *
         * For each component of the link, this routine returns a
         * "starting strand".  You can traverse the entire component by
         * beginning at this starting strand and repeatedly incrementing
         * it through a routine such as StrandRef::operator++ or
         * StrandRef::next().
         *
         * If a component has no crossings (which means it must be a separate
         * unknot component), then this routine will return a null reference
         * (i.e., StrandRef::crossing() will return \c null).
         *
         * \param index the index of the requested component.  This must
         * be between 0 and countComponents()-1 inclusive.
         * \return a "starting strand" for traversing the component at
         * the given index, or a null reference if the requested component
         * has no crossings.
         */
        StrandRef component(size_t index) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all components of this link.
         *
         * Components are individual circles embedded in the ambient 3-manifold
         * (they have nothing to do with the connectivity of the link diagram).
         * So, for example, the Hopf link has two components.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Each element of the list will be
         * a starting strand for some component; more precisely, iterating
         * through this list is equivalent to calling `component(0)`,
         * `component(1)`, ..., `component(countComponents()-1)`
         * in turn.  As an example, your code might look like:
         *
         * \code{.cpp}
         * for (const StrandRef& c : link.components()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for as
         * long as the link exists: even as components are added and/or removed,
         * it will always reflect the components that are currently in the link.
         * Nevertheless, it is recommended to treat this object as temporary
         * only, and to call components() again each time you need it.
         *
         * \return access to the list of all components.
         */
        auto components() const;

        /**
         * Returns the number of zero-crossing unknot components in this link.
         *
         * \return the number of zero-crossing unknot components.
         */
        size_t countTrivialComponents() const;

        /**
         * Returns the strand in the link with the given integer ID.
         *
         * Each strand ID is of the form 2<i>c</i>+<i>s</i>, where \e c is the
         * index of the crossing, and \e s is 0 or 1 for the lower or
         * upper strand respectively.  A null strand reference (as used to
         * indicate 0-crossing unknot components) has an ID of -1.
         *
         * \param id an integer between -1 and 2*size()-1 inclusive.
         * \return the strand of this link with the corresponding ID.
         *
         * \see StrandRef::id()
         */
        StrandRef strand(ssize_t id) const;

        /**
         * Returns a sequence that maps strand IDs to link component numbers.
         *
         * This sequence will have length `2n`, where \a n is the number of
         * crossings in this link diagram.  If \a strand is a non-null strand
         * reference, \a map is the sequence that is returned, and
         * `map[strand.id()] == c`, then this indicates that \a strand is part
         * of the link component defined by `component(c)`.
         *
         * Null strand references are not handled by this map: they have a
         * negative ID (which means calling `map[strand.id()]` is an error),
         * and they could refer to any 0-crossing unknot component (so the
         * specific component might not be uniquely determined).
         *
         * The return type is deliberately not specified here.  It is
         * guaranteed to be a container whose elements have type `size_t`,
         * with value semantics, fast move construction and swap operations,
         * an array index operator, and random access iterators.  It is _not_
         * guaranteed to have a copy assignment operator (but it will support
         * fast move assignment).  At present the specific implementation
         * returns `FixedArray<size_t>`, though this is subject to change in
         * future versions of Regina and so end user code should always use
         * `auto`.
         *
         * \python This routine will return a Python list.
         *
         * \return a sequence mapping strand IDs to component numbers.
         */
        auto componentsByStrand() const;

        /**
         * Translates a crossing from some other link into the corresponding
         * crossing in this link.
         *
         * Typically this routine would be used when the given crossing comes
         * from a link that is combinatorially identical to this, and you wish
         * to obtain the corresponding crossing in this link.
         *
         * Specifically: if \a other refers to crossing number \a k of some
         * other link, then the return value will refer to crossing number \a k
         * of this link.
         *
         * This routine behaves correctly even if \a other is a null pointer.
         *
         * \pre This link contains at least as many crossings as the link
         * containing \a other (though, as noted above, in typical scenarios
         * both links would actually be combinatorially identical).
         *
         * \param other the crossing to translate.
         * \return the corresponding crossing in this link.
         */
        Crossing* translate(Crossing* other) const;

        /**
         * Translates a strand reference from some other link into the
         * corresponding strand reference from this link.
         *
         * Typically this routine would be used when the given strand comes
         * from a link that is combinatorially identical to this, and you wish
         * to obtain the corresponding strand in this link.
         *
         * Specifically: if \a other refers to some strand (upper or lower)
         * of crossing number \a k of some other link, then the return
         * value will refer to the same strand (upper or lower) of
         * crossing number \a k of this link.
         *
         * This routine behaves correctly even if \a other is a null reference.
         *
         * \pre This link contains at least as many crossings as the link
         * containing \a other (though, as noted above, in typical scenarios
         * both links would actually be combinatorially identical).
         *
         * \param other the strand reference to translate.
         * \return the corresponding strand reference for this link.
         */
        StrandRef translate(const StrandRef& other) const;

        /**
         * Determines whether this link diagram is connected, if we treat each
         * crossing as a 4-way intersection.
         *
         * This tests whether it is possible to travel from any part of the
         * link to any other part of the link by:
         *
         * - following the link around its components, and/or;
         * - jumping between upper and lower strands at crossings.
         *
         * In particular, the link diagram may be connected even if the link
         * has multiple components.
         *
         * Connectivity is a property of the diagram, not an invariant of the
         * link itself, since the locations of the crossings matter.
         * In particular:
         *
         * - a disconnected diagram _must_ describe a splittable link;
         *
         * - a splittable link, however, could be represented by either a
         *   connected or disconnected link diagram.
         *
         * This is almost, but not quite, equivalent to testing whether the
         * underlying 4-valent graph of the link diagram is connected.
         * Specifically, where `link.isConnected()` and
         * `link.graph().isConnected()` differ is in cases where the link has
         * zero-crossing components (i.e., unknotted circles disjoint from the
         * rest of the diagram).  Zero-crossing components are considered here
         * in `Link.isConnected()` but _not_ in `ModelLinkGraph.isConnected()`,
         * since such components cannot be represented by a 4-valent graph
         * (and so the ModelLinkGraph class ignores them completely).
         *
         * For the purposes of this routine, an empty link is considered to be
         * connected.
         *
         * Note: for knots and empty links, this routine is constant time.
         * For multiple-component links, it is linear in the link size.
         *
         * See also diagramComponents(), which extracts the connected
         * components of a link diagram as individual Link objects.
         *
         * \return \c true if and only if this link diagram is connected.
         */
        bool isConnected() const;

        /**
         * Determines whether the two given crossings are connected in the
         * link diagram, if we treat each crossing as a 4-way intersection.
         *
         * This tests whether it is possible to travel between the two given
         * crossings by:
         *
         * - following the link around its components, and/or;
         * - jumping between upper and lower strands at crossings.
         *
         * In particular, two crossings may be connected in the diagram even
         * if they involve entirely different components of the link.
         *
         * See isConnected() for further discussion on the connectivity of
         * link diagrams.
         *
         * Note: for knots and empty links, this routine is constant time.
         * For multiple-component links, it is linear in the link size.
         *
         * \param a the first of the two crossings to examine.
         * \param b the second of the two crossings to examine.
         * \return \c true if and only if the two given crossings are
         * connected.
         */
        bool connected(const Crossing* a, const Crossing* b) const;

        /**
         * Returns the connected components of this link diagram as individual
         * standalone links.
         *
         * Here _connected components_  are not the same as _link components_.
         * A connected component means a portion of the link diagram that
         * is connected when we treat each crossing as a 4-way intersection.
         * In other words, one can travel around the connected component by
         * following the link around, and/or jumping between upper and lower
         * strands at crossings.  A single connected component of the diagram
         * may contain multiple link components, and will always describe a
         * sublink for which isConnected() returns `true`.
         *
         * The connected components are a property of the diagram, not an
         * invariant of the link itself, since the locations of the crossings
         * matter.  In particular:
         *
         * - a diagram with multiple connected components _must_ describe a
         *   splittable link;
         *
         * - a splittable link, however, could be represented by a diagram
         *   with multiple connected components or with just one connected
         *   component.
         *
         * The connected components that are returned will be cloned from this
         * link (so even if this diagram is connected and there is just one
         * connected component, a deep copy will still take place).  The total
         * number of crossings across all of the links that are returned will
         * equal size(), and the total number of _link_ components across all
         * of the links that are returned will equal countComponents().
         *
         * If you simply wish to know whether this diagram is connected, you
         * should call isConnected() instead which is much more lightweight.
         *
         * \return a list containing the individual connected components of
         * this link diagram.
         */
        std::vector<Link> diagramComponents() const;

        /**
         * Locates an over-crossing within the same link component as the
         * given strand.  The choice of _which_ over-crossing is returned will
         * be arbitrary (i.e., it might not be the _first_ over-crossing).
         *
         * \param component a strand reference in this link, which may be a
         * null reference (indicating a zero-crossing component).
         * \return an over-crossing in the same link component, or a null
         * reference if the given link component contains only under-crossings
         * (which for classical links means it is a zero-crossing unknot placed
         * beneath the rest of the diagram).
         */
        StrandRef overForComponent(StrandRef component) const;
        /**
         * Locates an under-crossing within the same link component as the
         * given strand.  The choice of _which_ under-crossing is returned will
         * be arbitrary (i.e., it might not be the _first_ under-crossing).
         *
         * \param component a strand reference in this link, which may be a
         * null reference (indicating a zero-crossing component).
         * \return an under-crossing in the same link component, or a null
         * reference if the given link component contains only over-crossings
         * (which for classical links means it is a zero-crossing unknot placed
         * above the rest of the diagram).
         */
        StrandRef underForComponent(StrandRef component) const;

        /**
         * Determines if this link diagram is combinatorially identical to the
         * given link diagram.
         *
         * Here "identical" means that:
         *
         * - the link diagrams have the same number of crossings and the
         *   same number of components;
         *
         * - the same numbered crossings are positive and negative in both
         *   diagrams;
         *
         * - the same pairs of numbered crossings have their
         *   under/over-strands connected, with the same orientations;
         *
         * - for each \a i, the starting strand for the <i>th</i> component
         *   is the same (under/over) strand of the same numbered crossing
         *   in both diagrams.
         *
         * \param other the link diagram to compare with this.
         * \return \c true if and only if the two link diagrams are
         * combinatorially identical.
         */
        bool operator == (const Link& other) const;

        /**
         * Returns the 4-valent graph that models this link diagram, along with
         * the local embedding of the graph into the surface that contains the
         * diagram.
         *
         * Any zero-component unknot components of this link will be ignored.
         *
         * For classical links, the result will be a planar graph with a
         * specific planar embedding.  For virtual links, this may be an
         * embedding of the graph into some higher genus closed orientable
         * surface, depending on the virtual genus of the link.  See
         * ModelLinkGraph for further discussion on local embeddings.
         *
         * The nodes of the resulting graph will be numbered in the same way
         * as the crossings of this link.  For each node, arc 0 will represent
         * the outgoing lower strand of the corresponding crossing.
         *
         * Calling `link.graph()` is identical to creating a graph via
         * `ModelLinkGraph(link)`.
         *
         * \return the graph that models this link.
         */
        ModelLinkGraph graph() const;

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

        /**
         * Sets this to be a (deep) copy of the given link.
         *
         * \param src the link to copy.
         * \return a reference to this link.
         */
        Link& operator = (const Link& src);

        /**
         * Moves the contents of the given link into this link.
         * This is a fast (constant time) operation.
         *
         * All crossings that belong to \a src will be moved into this link,
         * and so any Crossing pointers or StrandRef object will remain valid.
         * Likewise, all cached properties will be moved into this link.
         *
         * The link that is passed (\a src) will no longer be usable.
         *
         * \note This operator is _not_ marked \c noexcept, since it fires
         * change events on this link which may in turn call arbitrary code
         * via any registered packet listeners.  It deliberately does _not_
         * fire change events on \a src, since it assumes that \a src is about
         * to be destroyed (which will fire a destruction event instead).
         *
         * \param src the link to move.
         * \return a reference to this link.
         */
        Link& operator = (Link&& src);

        /**
         * Swaps the contents of this and the given link.
         *
         * All crossings that belong to this link will be moved to \a other,
         * and all crossings that belong to \a other will be moved to this link.
         * Likewise, all cached properties (e.g., tree decompositions) will be
         * swapped.
         *
         * In particular, any Crossing pointers or references and any
         * StrandRef objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this link.
         *
         * \note This swap function is _not_ marked \c noexcept, since it
         * fires change events on both links which may in turn call arbitrary
         * code via any registered packet listeners.
         *
         * \param other the link whose contents should be swapped with this.
         */
        void swap(Link& other);

        /**
         * Inserts a copy of the given link into this link.
         *
         * The crossings and components of \a source will be copied into this
         * link, and placed after any pre-existing crossings and components.
         * Specifically, if the original number of crossings in this link was
         * \a N, then crossing number \a i of \a source will be copied to
         * a new crosssing `N+i` of this link; likewise for components.
         *
         * This routine behaves correctly when \a source is this link.
         *
         * \param source the link whose copy will be inserted.
         */
        void insertLink(const Link& source);

        /**
         * Moves the contents of the given link into this link.
         *
         * The crossings and components of \a source will be moved directly
         * into this link, and placed after any pre-existing crossings and
         * components.  Specifically, if the original number of crossings in
         * this link was \a N, then crossing number \a i of \a source will
         * become crosssing `N+i` of this link; likewise for components.
         *
         * As is normal for an rvalue reference, after calling this function
         * \a source will be unusable.  Any strand references or crossing
         * pointers that referred to either this link or \a source will remain
         * valid (and will all now refer to this link), though if they
         * originally referred to \a source then they will now return
         * different crossing indices and strand IDs.
         *
         * Calling `link.insertLink(source)` (where \a source is an rvalue
         * reference) is similar to calling `source.moveContentsTo(link)`,
         * but it is a little faster since it does not need to leave
         * \a source in a usable state.
         *
         * Regarding packet change events: this function does _not_ fire
         * a change event on \a source, since it assumes that \a source is
         * about to be destroyed (which will fire a destruction event instead).
         *
         * \pre \a source is not this link.
         *
         * \nopython Only the copying version of this function is available
         * (i.e., the version that takes \a source as a const reference).
         * If you want a fast move operation, call
         * `source.moveContentsTo(this)`.
         *
         * \param source the link whose contents should be moved.
         */
        void insertLink(Link&& source);

        /**
         * Moves the contents of this link into the given destination link,
         * leaving this link empty but otherwise usable.
         *
         * The crossings and components of this link will be moved directly
         * into \a dest, and placed after any pre-existing crossings and
         * components.  Specifically, if the original number of crossings in
         * \a dest was \a N, then crossing number \a i of this link will
         * become crosssing `N+i` of \a dest; likewise for components.
         *
         * This link will become empty as a result, but it will otherwise
         * remain a valid and usable Link object.  Any strand references or
         * crossing pointers that referred to either this link or \a dest will
         * remain valid (and will all now refer to \a dest), though if they
         * originally referred to this link then they will now return
         * different crossing indices and strand IDs.
         *
         * Calling `link.moveContentsTo(dest)` is similar to calling
         * `dest.insertLink(std::move(link))`; it is a little slower but it
         * comes with the benefit of leaving this link in a usable state.
         *
         * \pre \a dest is not this link.
         *
         * \param dest the link into which the contents of this link should be
         * moved.
         */
        void moveContentsTo(Link& dest);

        /**
         * Switches the upper and lower strands of the given crossing.
         *
         * \param c the crossing to change.
         */
        void change(Crossing* c);

        /**
         * Switches the upper and lower strands of every crossing in the
         * diagram.  As a result, the sign of every crossing will also change.
         *
         * This operation corresponds to reflecting the link diagram
         * through the surface on which it is drawn.
         *
         * In the language of Jeremy Green's virtual knot tables, this
         * operation is a _vertical_ mirror image.
         */
        void changeAll();

        /**
         * Resolves the given crossing.  The two incoming strands will
         * switch connections with the two outgoing strands, with the
         * result that the given crossing is removed entirely.
         *
         * \note The number of components in the link will change as a result
         * of this operation.
         *
         * \param c the crossing to resolve.
         */
        void resolve(Crossing* c);

        /**
         * Converts the given classical crossing into a virtual crossing.
         *
         * This essentially adds a handle to the surface in which the diagram
         * is embedded, so that the old upper and lower strands can use this
         * handle to pass by one another without actually crossing in the link
         * diagram.
         *
         * Note that the virtual genus of this link might actually go _down_ as
         * a result of this operation, since the operation might generate more
         * empty handles (which Regina implicitly removes, as explained in
         * the class notes).  A virtual link could even become classical as a
         * result of this operation.
         *
         * For the combinatorics of the link diagram, this operation simply
         * removes the given crossing entirely (recall that Regina does not
         * store virtual crossings explicitly).  The incoming and outgoing
         * upper strands will merge into one, and the incoming and outgoing
         * lower strands will merge into one.
         *
         * This routine is safe to call if \a crossing is \c null (in which
         * case this routine does nothing).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing the (classical) crossing that should be made virtual.
         */
        void makeVirtual(Crossing* crossing);

        /**
         * Grafts the two given arcs of this link together, possibly making
         * this a virtual link in the process.
         *
         * This routine is intended for use with virtual links and, unlike
         * composeWith(), it offers a way to build a composite knot with full
         * control over exactly which arcs are grafted together.
         *
         * This operation is simple: it reroutes the part of the link that
         * enters along the first arc to exit along the second, and it reroutes
         * the part of the link that enters along the second arc to exit along
         * the first.  As a result:
         *
         * - If \a first and \a second belong to _different_ components of this
         *   link then it will effectively combine those two components in an
         *   operation akin to knot composition.  The main difference is that,
         *   if the two components are already part of the same connected
         *   diagram component (e.g., they are already linked together), then
         *   this operation will make no attempt to separate them beforehand.
         *
         * - If \a first and \a second belong to the _same_ component of this
         *   link then this operation will effectively split that component
         *   into two.  It will not make any attempt to separate and/or unlink
         *   the two resulting components.
         *
         * The operation will never add or remove any crossings.  Therefore,
         * if the two given arcs belong to the same connected component of the
         * diagram but do not bound the same dual 2-cell with the same
         * orientation, this operation may increase the virtual genus.
         *
         * Regarding the two arguments:
         *
         * - It is allowed for \a first and \a second to refer to the same arc
         *   (in which case this operation will just split off a new
         *   zero-crossing component).
         *
         * - It is allowed for either \a first or \a second to be a null
         *   reference.  In this case it will be taken to refer to a
         *   zero-crossing component, and so this operation will effectively
         *   absorb the zero-crossing component into the other link component.
         *
         * - If \a first and \a second are both null references, then they will
         *   be assumed to refer to _different_ zero-crossing components.
         *
         * See the StrandRef documentation for the convention on how arcs are
         * represented using StrandRef objects.
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \exception InvalidArgument Either one of \a first or \a second is a
         * null reference but this link does not contain any zero-crossing
         * components, or _both_ \a first and \a second are null references but
         * this link does not contain at least two zero-crossing components.
         *
         * \param first the first of the two arcs to graft together.
         * \param second the second of the two arcs to graft together.
         */
        void graft(StrandRef first, StrandRef second);

        /**
         * Converts this link into its reflection.
         *
         * This routine changes the sign of every crossing, but leaves
         * the upper and lower strands the same.
         *
         * - For classical links, this operation corresponds to reflecting the
         *   link diagram about some axis in the plane.
         *
         * - For virtual links, this operation performs an orientation-reversing
         *   homeomorphism of the surface in which the link diagram embeds.
         *
         * In the language of Jeremy Green's virtual knot tables, this
         * operation is a _horizontal_ mirror image.
         */
        void reflect();

        /**
         * Rotates this link diagram, effectively flipping the surface that
         * contains it "upside-down".
         *
         * This routine keeps the sign of each crossing fixed, but
         * switches the upper and lower strands.
         *
         * - For classical links, this operation corresponds to a 3-dimensional
         *   rotation about some axis in the plane; the result will be a
         *   different diagram of the same link.
         *
         * - For virtual links, let \a S denote the closed orientable surface
         *   in which the link diagram embeds, and think of this as a link in
         *   the thickened surface `S x I`.  Then this operation performs an
         *   orientation-preserving homeomorphism of `S x I` that switches the
         *   boundaries `S x {0}` and `S x {1}`.
         *
         * Some authors refer to this operation as a _flip_.  In the language
         * of Jeremy Green's virtual knot tables, this is the composition of
         * both a vertical and a horizontal mirror image.
         */
        void rotate();

        /**
         * Reverses the orientation of every component of this link.
         *
         * This routine preserves both the sign and the upper/lower positions
         * at every crossing, but switches all incoming strands with outgoing
         * strands and vice versa (so next() becomes prev(), and prev()
         * becomes next()).
         */
        void reverse();

        /**
         * Reverses the orientation of just the link component that contains
         * the given strand.  Other components of the link will not be modified.
         *
         * For knots, this routine is identical to calling reverse().
         *
         * \param component a strand belonging to some component of this link.
         * This need not be the starting strand for the component (i.e., it
         * does not need to be the strand that is returned by `component()`).
         * This may be a null strand reference, in which case this routine
         * will do nothing.
         */
        void reverse(StrandRef component);

        /**
         * Changes a subset of crossings to convert this into an alternating
         * link diagram.  Here, "changing" a crossing means switching its
         * upper and lower strands (so this operation may change this into
         * a topologically different link).
         *
         * This is always possible for classical link diagrams; however, for
         * virtual link diagrams it might or might not be possibe.
         *
         * Any zero-crossing unknot components will be considered alternating;
         * likewise, the empty link is considered alternating.
         *
         * Assuming the diagram _can_ be made alternating, for each connected
         * piece of the link diagram (which may incorporate several link
         * components), one must choose between two possible alternating
         * diagrams.  Regina will choose the option that preserves the sign of
         * the lowest-index crossing in that connected piece of the diagram.
         *
         * If this diagram cannot be made alternating, or if it was already
         * alternating to begin with, then it will be left unchanged.
         *
         * \return \c true if this link diagram was successfully made
         * alternating (or was already alternating to begin with), or \c false
         * if this is a virtual link diagram that cannot be made alternating.
         */
        bool makeAlternating();

        /**
         * If possible, performs a type I Reidemeister move to remove a
         * crossing at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * The location of this move is specified by the argument \a crossing,
         * which indicates the crossing that will be removed.  Specifically,
         * this move involves undoing a trivial twist at the given crossing.
         *
         * You may pass a null pointer for \a crossing.  However, in this case
         * the move cannot be performed, which means this routine will do
         * nothing and simply return \c false.
         *
         * \warning A side-effect of this move is that, because one crossing
         * is being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the one involved in this move
         * will be destroyed.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing to be removed.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r1(Crossing* crossing);
        /**
         * If possible, performs a type I Reidemeister move to add a
         * new crossing at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * The location of this move is specified by the argument \a arc.
         * Specifically, this move involves adding a trivial twist to the
         * given arc; the arguments \a side and \a sign indicate on which side
         * of the arc and with which orientation the new twist will be made.
         * See the StrandRef documentation for the convention on how arcs
         * are represented using StrandRef objects.
         *
         * If \a arc is a null reference, then the new twist will be
         * added to a zero-crossing unknot component; it will be assumed
         * that this unknot component is oriented clockwise.  If \a arc
         * is null but there is no zero-crossing component then the move cannot
         * be performed, and if \a arc is null but there are multiple
         * zero-crossing components then the first such component will be used.
         *
         * This move is almost always able to be performed: the only situation
         * in which it _cannot_ be performed is if \a arc is a null reference
         * but this link contains no zero-crossing components, as discussed
         * above.
         *
         * The existing crossings in this link will keep the same indices,
         * and the new crossing will be given the next index that is available.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies the arc of the link in which the new
         * twist will be introduced, as described above.
         * \param side 0 if the twist should be introduced on the left
         * of the arc (when walking along the arc in the forward direction),
         * or 1 if the twist should be introduced on the right of the arc.
         * \param sign the sign of the new crossing that will be
         * introduced as part of the twist; this must be +1 or -1.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r1(StrandRef arc, int side, int sign);
        /**
         * If possible, performs a type II Reidemeister move to remove
         * two crossings at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  This variant, which takes
         * an arc, is more flexible (since either of the two arcs involved in
         * this move can be passed).  The other variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * The location of this move is specified by the argument \a arc.
         * Specifically, this move involves pulling apart two arcs of
         * the link that surround a bigon; the given arc must be one of
         * these two arcs.  See the StrandRef documentation for the
         * convention on how arcs are represented using StrandRef objects.
         *
         * You may pass a null reference for \a arc.  However, in this case
         * the move cannot be performed, which means this routine will do
         * nothing and simply return \c false.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the bigon about
         * which the move will be performed, as described above.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r2(StrandRef arc);
        /**
         * If possible, performs a type II Reidemeister move to remove
         * two crossings at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  The other variant, which takes
         * an arc, is more flexible (since either of the two arcs involved in
         * this move can be passed).  This variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * The location of this move is specified by the argument \a crossing,
         * Specifically, this move involves pulling apart two arcs of the link
         * (one upper, one lower) that both run between the same pair of
         * crossings.  The given crossing should be the start point of the
         * upper arc; that is, when following the upper arc forwards,
         * \a crossing should be the first of the two crossings that we
         * encounter.  Note that \a crossing is one of the two crossings that
         * will be removed by this move.
         *
         * You may pass a null pointer for \a crossing.  However, in this case
         * the move cannot be performed, which means this routine will do
         * nothing and simply return \c false.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of
         * the "upper" arc that features in this move, as described above.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r2(Crossing* crossing);
        /**
         * If possible, performs a classical type II Reidemeister move to add
         * two new crossings at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * By a _classical_ type II move, we mean that the move can be
         * performed without adding a handle to the surface \a S in which the
         * link diagram is embedded.  More precisely: the two "sides of strands"
         * that will be passed over one another either belong to different
         * connected components of the link diagram, or else both bound the same
         * 2-cell in the dual cell decomposition of \a S.  Performing a
         * classical type II move on a classical link diagram will always
         * result in a classical link diagram.
         *
         * If you are working with virtual links, you may wish to use
         * r2Virtual() instead, which does allow changing the surface \a S
         * (and which could therefore convert a classical link diagram into a
         * virtual diagram with positive virtual genus).
         *
         * The location of this move is specified by the arguments \a upperArc,
         * \a upperSide, \a lowerArc and \a lowerSide.  Specifically, this
         * move involves taking the arc \a upperArc and pushing it over
         * \a lowerArc so that the two arcs overlap.  The arguments
         * \a upperSide and \a lowerSide indicate on which side of each arc
         * the overlap takes place.  See the StrandRef documentation for the
         * convention on how arcs are represented using StrandRef objects.
         *
         * If either \a upperArc or \a lowerArc is a null reference, then
         * the move will be performed upon a zero-crossing unknot component;
         * it will be assumed that this unknot component is oriented clockwise.
         * If one of these arguments is a null reference but there is no
         * zero-crossing component then the move cannot be performed,
         * and if there are multiple zero-crossing components then the first
         * such component will be used.
         *
         * Likewise, if _both_ arcs are null references, then the move
         * will be performed upon two _different_ zero-crossing unknot
         * components.  In this case, if there are fewer than two such
         * components then the move cannot be performed, and otherwise
         * \a upperArc will be the first such component and \a lowerArc
         * will be the second.
         *
         * Currently, Regina cannot perform the move when \a upperArc and
         * \a lowerArc represent the same arc (or the same zero-crossing unknot
         * component).  In this case there is a workaround: you can achieve
         * the same effect by performing two type I Reidemeister moves
         * (i.e., by adding two twists).
         *
         * The existing crossings in this link will keep the same indices,
         * and the two new crossings will be given the next two indices
         * that are available.
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \warning The checks for this move are expensive (linear time).  If
         * you are certain that the move is legal and you wish to circumvent
         * this check, you can always call r2Virtual() instead.  If the move you
         * wish to perform is indeed classical and legal, then r2Virtual()
         * will have the same effect but will avoid the expensive planarity
         * check.
         *
         * \param upperArc identifies the arc of the link which will be
         * passed over the other, as described above.
         * \param upperSide 0 if the new overlap should take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a upperArc.
         * \param lowerArc identifies the arc of the link which will be
         * passed beneath the other, as described above.
         * \param lowerSide 0 if the new overlap should take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a lowerArc.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r2(StrandRef upperArc, int upperSide, StrandRef lowerArc,
            int lowerSide);
        /**
         * If possible, performs a virtual type II Reidemeister move to add
         * two new crossings at the given location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * By a _virtual_ type II move, we mean that the move can be performed
         * upon _any_ two "sides of strands", even if this requires adding a
         * handle to the surface in which the link diagram is embedded.
         * As a result, a virtual type II move could potentially change the
         * virtual genus of the link diagram; in particular, it could convert a
         * classical link diagram into a virtual diagram with positive virtual
         * genus.
         *
         * The location of this move is specified by passing two "sides of
         * strands", in the same way as for classical type II moves.
         * See r2(StrandRef, int, StrandRef, int) for details on how the
         * location arguments are interpreted, and in particular how this move
         * works with zero-crossing unknot components when passing null strand
         * references.
         *
         * The existing crossings in this link will keep the same indices,
         * and the two new crossings will be given the next two indices
         * that are available.
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param upperArc identifies the arc of the link which will be
         * passed over the other.  See r2(StrandRef, int, StrandRef, int)
         * for details on how this will be interpreted.
         * \param upperSide 0 if the new overlap should take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a upperArc.
         * \param lowerArc identifies the arc of the link which will be
         * passed beneath the other.  See r2(StrandRef, int, StrandRef, int)
         * for details on how this will be interpreted.
         * \param lowerSide 0 if the new overlap should take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a lowerArc.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r2Virtual(StrandRef upperArc, int upperSide, StrandRef lowerArc,
            int lowerSide);
        /**
         * If possible, performs a type III Reidemeister move at the given
         * location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  This variant, which takes
         * an arc, is more flexible (since any of the three arcs involved in
         * this move can be passed).  The other variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * The location of this move is specified by the arguments \a arc
         * and \a side.  Specifically, this move takes place around a
         * triangle; the given arc must form one of the three edges of this
         * triangle.  The argument \a side indicates on which side of the arc
         * the third crossing is located.  See the StrandRef documentation
         * for the convention on how arcs are represented using StrandRef
         * objects.
         *
         * You may pass a null reference for \a arc.  However, in this case
         * the move cannot be performed, which means this routine will do
         * nothing and simply return \c false.
         *
         * All crossings in this link will keep the same indices, and
         * no crossings will be created or destroyed.  Instead, the three
         * crossings involved in this move will simply be reordered
         * along the various segments of the link.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the triangle about
         * which the move will be performed, as described above.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the arc (when walking along the arc in the forward
         * direction), or 1 if the third crossing is located on the right of
         * the arc.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r3(StrandRef arc, int side);
        /**
         * If possible, performs a type III Reidemeister move at the given
         * location.
         * If such a move is not allowed, then this routine does nothing.
         *
         * This link diagram will be changed directly.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  The other variant, which takes
         * an arc, is more flexible (since any of the three arcs involved in
         * this move can be passed).  This variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * The location of this move is specified by the arguments \a crossing
         * and \a side.  Specifically, this move takes place around a
         * triangle, and one of the arcs of this triangle is _uppermost_ (in
         * that it passes above the other two arcs).  The given crossing
         * should be the start point of this uppermost arc; that is, when
         * following the arc forwards, \a crossing should be the first of the
         * two crossings that we encounter.  The additional argument \a side
         * indicates on which side of the uppermost arc the third crossing is
         * located.
         *
         * You may pass a null pointer for \a crossing.  However, in this case
         * the move cannot be performed, which means this routine will do
         * nothing and simply return \c false.
         *
         * All crossings in this link will keep the same indices, and
         * no crossings will be created or destroyed.  Instead, the three
         * crossings involved in this move will simply be reordered
         * along the various segments of the link.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of
         * the "uppermost" arc that features in this move, as described above.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the uppermost arc (when walking along the arc in the
         * forward direction), or 1 if the third crossing is located on the
         * right of the uppermost arc.
         * \return \c true if and only if the requested move was able to
         * be performed.
         */
        bool r3(Crossing* crossing, int side);

        /**
         * Determines whether it is possible to perform a type I Reidemeister
         * move at the given location to remove a crossing.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(Crossing*).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the candidate crossing to be removed.
         * See r1(Crossing*) for details on exactly how this will be
         * interpreted.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR1(Crossing* crossing) const;
        /**
         * Determines whether it is possible to perform a type I Reidemeister
         * move at the given location to add a new crossing.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(StrandRef, int, int).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies the arc of the link in which the new
         * candidate twist will be introduced.  See r1(StrandRef, int, int)
         * for details on exactly how this will be interpreted.
         * \param side 0 if the candidate twist should be introduced on the
         * left of the arc (when walking along the arc in the forward
         * direction), or 1 if the candidate twist should be introduced on the
         * right of the arc.
         * \param sign the sign of the new crossing that would be
         * introduced as part of the candidate twist; this must be +1 or -1.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR1(StrandRef arc, int side, int sign) const;
        /**
         * Determines whether it is possible to perform a type II Reidemeister
         * move at the given location to remove two crossings.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(StrandRef).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the bigon about which the
         * candidate move will be performed.  See r2(StrandRef) for details on
         * exactly how this will be interpreted.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR2(StrandRef arc) const;
        /**
         * Determines whether it is possible to perform a type II Reidemeister
         * move at the given location to remove two crossings.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(Crossing*).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of
         * the "upper" arc that features in this candidate move.  See
         * r2(Crossing*) for details on exactly how this will be interpreted.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR2(Crossing* crossing) const;
        /**
         * Determines whether it is possible to perform a classical type II
         * Reidemeister move at the given location to add two new crossings.
         *
         * For more detail on classical type II moves and when they can be
         * performed, see r2(StrandRef, int, StrandRef, int).
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \warning The check for this move is expensive (linear time).
         *
         * \param upperArc identifies which arc of the link would be passed
         * over another in this candidate move.  See
         * r2(StrandRef, int, StrandRef, int) for details on exactly how this
         * will be interpreted.
         * \param upperSide 0 if the new overlap would take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap would take place on the right
         * of \a upperArc.
         * \param lowerArc identifies which arc of the link would be passed
         * beneath another in this candidate move.  See
         * r2(StrandRef, int, StrandRef, int) for details on exactly how this
         * will be interpreted.
         * \param lowerSide 0 if the new overlap would take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap would take place on the right
         * of \a lowerArc.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR2(StrandRef upperArc, int upperSide,
            StrandRef lowerArc, int lowerSide) const;
        /**
         * Determines whether it is possible to perform a type III Reidemeister
         * move at the given location.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(StrandRef, int).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the triangle about which
         * the candidate move would be performed.  See r3(StrandRef, int) for
         * details on exactly how this will be interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the arc (when walking along the arc in the forward
         * direction), or 1 if the third crossing is located on the right of
         * the arc.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR3(StrandRef arc, int side) const;
        /**
         * Determines whether it is possible to perform a type III Reidemeister
         * move at the given location.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(Crossing*, int).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of the
         * "uppermost" arc that features in this candidate move.  See
         * r3(Crossing*, int) for details on exactly how this will be
         * interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the uppermost arc (when walking along the arc in the
         * forward direction), or 1 if the third crossing is located on the
         * right of the uppermost arc.
         * \return \c true if and only if the requested move can be performed.
         */
        bool hasR3(Crossing* crossing, int side) const;

        /**
         * If possible, returns the diagram obtained by performing a type I
         * Reidemeister move at the given location to remove a crossing.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(Crossing*).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing to be removed.  See
         * r1(Crossing*) for details on exactly how this will be interpreted.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR1(Crossing* crossing) const;
        /**
         * If possible, returns the diagram obtained by performing a type I
         * Reidemeister move at the given location to add a new crossing.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(StrandRef, int, int).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies the arc of the link in which the new twist
         * will be introduced.  See r1(StrandRef, int, int) for details on
         * exactly how this will be interpreted.
         * \param side 0 if the twist should be introduced on the left
         * of the arc (when walking along the arc in the forward direction),
         * or 1 if the twist should be introduced on the right of the arc.
         * \param sign the sign of the new crossing that will be
         * introduced as part of the twist; this must be +1 or -1.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR1(StrandRef arc, int side, int sign) const;
        /**
         * If possible, returns the diagram obtained by performing a type II
         * Reidemeister move at the given location to remove two crossings.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(StrandRef).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the bigon about which the
         * move will be performed.  See r2(StrandRef) for details on exactly
         * how this will be interpreted.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR2(StrandRef arc) const;
        /**
         * If possible, returns the diagram obtained by performing a type II
         * Reidemeister move at the given location to remove two crossings.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(Crossing*).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of the
         * "upper" arc that features in this move.  See r2(Crossing*) for
         * details on exactly how this will be interpreted.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR2(Crossing* crossing) const;
        /**
         * If possible, returns the diagram obtained by performing a classical
         * type II Reidemeister move at the given location to add two new
         * crossings.  If such a move is not allowed, then this routine
         * returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on classical type II moves and when they can be
         * performed, see r2(StrandRef, int, StrandRef, int).
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \warning The check for this move is expensive (linear time).
         *
         * \param upperArc identifies which arc of the link will be passed
         * over another.  See r2(StrandRef, int, StrandRef, int) for details
         * on exactly how this will be interpreted.
         * \param upperSide 0 if the new overlap should take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a upperArc.
         * \param lowerArc identifies which arc of the link will be passed
         * beneath another.  See r2(StrandRef, int, StrandRef, int) for details
         * on exactly how this will be interpreted.
         * \param lowerSide 0 if the new overlap should take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a lowerArc.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR2(StrandRef upperArc, int upperSide,
            StrandRef lowerArc, int lowerSide) const;
        /**
         * If possible, returns the diagram obtained by performing a type III
         * Reidemeister move at the given location.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(StrandRef, int).
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the triangle about which
         * the move will be performed.  See r3(StrandRef, int) for details on
         * exactly how this will be interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the arc (when walking along the arc in the forward
         * direction), or 1 if the third crossing is located on the right of
         * the arc.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR3(StrandRef arc, int side) const;
        /**
         * If possible, returns the diagram obtained by performing a type III
         * Reidemeister move at the given location.
         * If such a move is not allowed, then this routine returns no value.
         *
         * This link diagram will not be changed.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(Crossing*, int).
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of the
         * "uppermost" arc that features in this move.  See r3(Crossing*, int)
         * for details on exactly how this will be interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the uppermost arc (when walking along the arc in the
         * forward direction), or 1 if the third crossing is located on the
         * right of the uppermost arc.
         * \return The new link diagram obtained by performing the requested
         * move, or no value if the requested move cannot be performed.
         */
        std::optional<Link> withR3(Crossing* crossing, int side) const;

        /**
         * Deprecated routine that tests for and optionally performs a type I
         * Reidemeister move to remove a crossing.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(Crossing*).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR1().  If you wish to both check and perform the move,
         * call r1() without the two additional boolean arguments.
         *
         * \warning A side-effect of this move is that, because one crossing
         * is being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the one involved in this move
         * will be destroyed.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing to be removed.  See
         * r1(crossing*) for details on exactly how this will be interpreted.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r1(Crossing* crossing,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a type I
         * Reidemeister move to add a new crossing.
         *
         * For more detail on type I moves and when they can be performed,
         * see r1(StrandRef, int, int).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR1().  If you wish to both check and perform the move,
         * call r1() without the two additional boolean arguments.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies the arc of the link in which the new twist
         * will be introduced.  See r1(StrandRef, int, int) for details on
         * exactly how this will be interpreted.
         * \param side 0 if the twist should be introduced on the left
         * of the arc (when walking along the arc in the forward direction),
         * or 1 if the twist should be introduced on the right of the arc.
         * \param sign the sign of the new crossing that will be
         * introduced as part of the twist; this must be +1 or -1.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r1(StrandRef arc, int side, int sign,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a type II
         * Reidemeister move to remove two crossings.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(StrandRef).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR2().  If you wish to both check and perform the move,
         * call r2() without the two additional boolean arguments.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the bigon about which the
         * move will be performed.  See r2(StrandRef) for details on exactly
         * how this will be interpreted.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r2(StrandRef arc,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a type II
         * Reidemeister move to remove two crossings.
         *
         * For more detail on type II moves and when they can be performed,
         * see r2(Crossing*).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR2().  If you wish to both check and perform the move,
         * call r2() without the two additional boolean arguments.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of
         * the "upper" arc that features in this move.  See r2(Crossing*)
         * for details on exactly how this will be interpreted.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r2(Crossing* crossing,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a classical
         * type II Reidemeister move to add two new crossings.
         *
         * For more detail on classical type II moves and when they can be
         * performed, see r2(StrandRef, int, StrandRef, int).  This deprecated
         * routine will not perform virtual type II moves; for that you should
         * use the new routine r2Virtual() instead.
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether such a move is possible,
         * call hasR2().  If you wish to both check and perform the move,
         * call r2() without the two additional boolean arguments.
         *
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \warning The check for this move is expensive (linear time).
         *
         * \param upperArc identifies which arc of the link would be passed
         * over another in this move.  See r2(StrandRef, int, StrandRef, int)
         * for details on exactly how this will be interpreted.
         * \param upperSide 0 if the new overlap should take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a upperArc.
         * \param lowerArc identifies which arc of the link would be passed
         * beneath another in this move.  See r2(StrandRef, int, StrandRef, int)
         * for details on exactly how this will be interpreted.
         * \param lowerSide 0 if the new overlap should take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a lowerArc.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r2(StrandRef upperArc, int upperSide,
            StrandRef lowerArc, int lowerSide,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a type III
         * Reidemeister move.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(StrandRef, int).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR3().  If you wish to both check and perform the move,
         * call r3() without the two additional boolean arguments.
         *
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \param arc identifies one of the arcs of the triangle about which
         * the move would be performed.  See r3(StrandRef, int) for details on
         * exactly how this will be interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the arc (when walking along the arc in the forward
         * direction), or 1 if the third crossing is located on the right of
         * the arc.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r3(StrandRef arc, int side,
            bool ignored, bool perform = true);
        /**
         * Deprecated routine that tests for and optionally performs a type III
         * Reidemeister move.
         *
         * For more detail on type III moves and when they can be performed,
         * see r3(Crossing*, int).
         *
         * This routine will always _check_ whether the requested move is
         * allowed.  If it is, and if the argument \a perform is \c true,
         * this routine will also _perform_ the move.
         *
         * \deprecated If you just wish to test whether a such move is possible,
         * call hasR3().  If you wish to both check and perform the move,
         * call r3() without the two additional boolean arguments.
         *
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * \param crossing identifies the crossing at the beginning of the
         * "uppermost" arc that features in this move.  See r3(Crossing*, int)
         * for details on exactly how this will be interpreted.
         * \param side 0 if the third crossing of the triangle is located to
         * the left of the uppermost arc (when walking along the arc in the
         * forward direction), or 1 if the third crossing is located on the
         * right of the uppermost arc.
         * \param ignored an argument that is ignored.  In earlier versions of
         * Regina this argument controlled whether we check if the move can be
         * performed; however, now this check is done always.
         * \param perform \c true if we should actually perform the move,
         * assuming the move is allowed.
         * \return \c true if and only if the requested move could be performed.
         */
        [[deprecated]] bool r3(Crossing* crossing, int side,
            bool ignored, bool perform = true);

        /**
         * Tests whether this classical link has a pass move that will reduce
         * the number of crossings.
         *
         * A _pass_ move involves taking a section of some link component that
         * involves only over-crossings (or only under-crossings), and then
         * lifting that section above (or beneath respectively) the plane of
         * the diagram and placing it back again in a different location.
         * In particular, this routine searches for a different location that
         * will involve fewer crossings than the original location.
         *
         * In Regina, pass moves can only be used with classical links, not
         * the more general setting of virtual link diagrams.
         *
         * This routine does not actually _perform_ the pass move; it
         * simply determines whether one exists.
         *
         * The running time is cubic in the number of crossings.
         *
         * \pre This link diagram is classical (not virtual).
         *
         * \exception FailedPrecondition This is a virtual (not classical)
         * link diagram.
         *
         * \return \c true if and only if there is a pass move that
         * reduces the number of crossings.
         */
        bool hasReducingPass() const;

        /**
         * Adds trivial twists to this link to ensure that each component has
         * zero writhe.  Here the _writhe_ of a component \a c is the sum of
         * the signs of all crossings at which \a c crosses itself.
         *
         * Any component(s) that already have zero writhe will be left
         * unchanged.
         *
         * This link will be modified directly.
         *
         * \return \c true if the link diagram was changed, or \c false if
         * every component already had zero writhe to begin with.
         */
        bool selfFrame();

        /**
         * Attempts to simplify this link diagram as intelligently as possible
         * using fast and greedy heuristics.  Specifically, this routine tries
         * combinations of Reidemeister moves with the aim of reducing the
         * number of crossings.
         *
         * Currently this routine uses simplifyToLocalMinimum() in
         * combination with random type III Reidemeister moves.
         *
         * Although simplify() often works well, it can sometimes
         * get stuck.  If this link is a knot (i.e., it has precisely one
         * component), then in such cases you can try the more powerful but
         * (much) slower simplifyExhaustive() instead.
         *
         * This routine will never reflect or reverse the link.
         *
         * \warning Running this routine multiple times upon the same link may
         * return different results, since the implementation makes random
         * decisions.  More broadly, the implementation of this routine
         * (and therefore its results) may change between different releases
         * of Regina.
         *
         * \note For long-term users of Regina: this is the routine that was
         * for a long time called intelligentSimplify().  It was renamed to
         * simplify() in Regina 7.4.
         *
         * \return \c true if and only if the link diagram was successfully
         * simplified.
         */
        bool simplify();
        /**
         * Deprecated alias for simplify(), which attempts to simplify this
         * link diagram as intelligently as possible using fast and greedy
         * heuristics.
         *
         * \deprecated This routine has been renamed to simplify().
         * See simplify() for further details.
         *
         * \return \c true if and only if the link diagram was successfully
         * simplified.
         */
        [[deprecated]] bool intelligentSimplify();
        /**
         * Uses type I and II Reidemeister moves to reduce the link
         * monotonically to some local minimum number of crossings.
         *
         * End users will probably not want to call this routine.
         * You should call simplify() if you want a fast (and
         * usually effective) means of simplifying a link.  If this link is
         * a knot (i.e., it has precisely one component), then you can also
         * call simplifyExhaustive() if you are still stuck and you want to
         * try a slower but more powerful method instead.
         *
         * Type III Reidemeister moves (which do not reduce the number of
         * crossings) are not used in this routine.  Such moves do however
         * feature in simplify().
         *
         * This routine will never reflect or reverse the link.
         *
         * \warning The implementation of this routine (and therefore
         * its results) may change between different releases of Regina.
         *
         * \param perform \c true if we are to perform the
         * simplifications, or \c false if we are only to investigate
         * whether simplifications are possible (defaults to \c true).
         * \return if \a perform is \c true, this routine returns
         * \c true if and only if the link was changed to
         * reduce the number of crossings; if \a perform is \c false,
         * this routine returns \c true if and only if it determines
         * that it is capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Attempts to simplify this link diagram using a slow but
         * exhaustive search through the Reidemeister graph.  This routine is
         * more powerful but much slower than simplify().
         *
         * Unlike simplify(), this routine **could potentially
         * reflect or reverse the link**.
         *
         * As of Regina 7.4, this routine is now available for any connected
         * link diagram (classical or virtual) with fewer than 64 link
         * components.  If this link has 64 or more components then this
         * routine will throw an exception (as described below).
         *
         * This routine will iterate through all link diagrams that can be
         * reached from this via Reidemeister moves, without ever exceeding
         * \a height additional crossings beyond the original number.
         * (If this link diagram is disconnected, then there is an exception:
         * this routine will never use a type II move to merge distinct
         * diagram components together, which would never help with
         * simplification).
         *
         * If at any stage this routine finds a diagram with _fewer_ crossings
         * than the original, then it will call simplify() to simplify the
         * diagram further if possible and will then return \c true.
         * If it cannot find a diagram with fewer crossings then it will leave
         * this link diagram unchanged and return \c false.
         *
         * This routine can be very slow and very memory-intensive: the
         * number of link diagrams it visits may be exponential in
         * the number of crossings, and it records every diagram
         * that it visits (so as to avoid revisiting the same diagram
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if this fails then try increasing \a height one at a time until
         * either you find a simplification or the routine becomes
         * too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on
         * the number of additional crossings.  This means that the
         * routine will not terminate until a simpler diagram is found.
         * If no simpler diagram exists then the only way to terminate this
         * function is to cancel the operation via a progress tracker
         * (read on for details).
         *
         * If you want a _fast_ simplification routine, you should call
         * simplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn link diagrams where simplify()
         * finds itself stuck at a local minimum, simplifyExhaustive() is able
         * to "climb out" of such wells.
         *
         * Since Regina 7.0, this routine will not return until either the
         * link diagram is simplified or the exhaustive search is complete,
         * regardless of whether a progress tracker was passed.  If you
         * need the old behaviour (where passing a progress tracker caused
         * the exhaustive search to start in the background), simply call
         * this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a threads.  Even in multithreaded mode, this
         * routine will not return until processing has finished (i.e., either
         * the diagram was simplified or the search was exhausted), and any
         * change to this link diagram will happen in the calling thread.
         *
         * If this routine is unable to simplify the link diagram, then
         * this link diagram will not be changed.
         *
         * \pre This link has at most 64 link components.
         *
         * \exception FailedPrecondition This link has 64 or more link
         * components.  If a progress tracker was passed, it will be marked as
         * finished before the exception is thrown.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param height the maximum number of _additional_ crossings to
         * allow beyond the number of crossings originally present in this
         * diagram, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return \c true if and only if this diagram was successfully
         * simplified to fewer crossings.
         */
        bool simplifyExhaustive(int height = 1, unsigned threads = 1,
            ProgressTrackerOpen* tracker = nullptr);

        /**
         * Explores all link diagrams that can be reached from this via
         * Reidemeister moves, without exceeding a given number of additional
         * crossings.
         *
         * As of Regina 7.4, this routine is now available for any connected
         * link diagram (classical or virtual) with fewer than 64 link
         * components.  If this link has 64 or more components then this
         * routine will throw an exception (as described below).
         *
         * This routine iterates through all link diagrams that can be reached
         * from this via Reidemeister moves (with an important exception
         * involving disconnected diagrams), without ever exceeding
         * \a height additional crossings beyond the original number.
         * With the current implementation, these diagrams **could become
         * reflected and/or reversed**, and moreover each diagram will only be
         * considered once up to reflection and/or reversal; be aware that this
         * behaviour could change and/or become configurable in a future version
         * of Regina.
         *
         * For every such link diagram (including this starting diagram), this
         * routine will call \a action (which must be a function or some other
         * callable object).
         *
         * - \a action must take the following initial argument(s).
         *   Either (a) the first argument must be a link (the precise type
         *   is discussed below), representing the link diagram that has been
         *   found; or else (b) the first two arguments must be of types
         *   const std::string& followed by a link, representing both the
         *   link diagram and its signature (as returned by sig()).
         *   The second form is offered in order to avoid unnecessarily
         *   recomputation within the \a action function.
         *   If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - The link argument will be passed as an rvalue; a typical action
         *   could (for example) take it by const reference and query it,
         *   or take it by value and modify it, or take it by rvalue reference
         *   and move it into more permanent storage.
         *
         * - \a action must return a boolean.  If \a action ever returns
         *   \c true, then this indicates that processing should stop
         *   immediately (i.e., no more link diagrams will be processed).
         *
         * - \a action may, if it chooses, make changes to this link
         *   (i.e., the original link upon which rewrite() was called).
         *   This will not affect the search: all link diagrams
         *   that this routine visits will be obtained via Reidemeister moves
         *   from the original link diagram, before any subsequent changes
         *   (if any) were made.
         *
         * - \a action will only be called once for each link diagram
         *   (including this starting diagram).  In other words, no
         *   link diagram will be revisited a second time in a single call
         *   to rewrite().
         *
         * The exception for disconnected diagrams is this: if this link
         * diagram has more than one connected component, then this routine
         * will never use a type II move to merge those components together
         * (i.e., the diagram will always remain disconnected).  Of course, if
         * your link diagram is disconnected, then it will be _much_ more
         * efficient to call diagramComponents() and run rewrite() on each
         * component independently.
         *
         * This routine can be very slow and very memory-intensive, since the
         * number of link diagrams it visits may be exponential in
         * the number of crossings, and it records every link diagram
         * that it visits (so as to avoid revisiting the same diagram
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if necessary try increasing \a height one at a time until
         * this routine becomes too expensive to run.
         *
         * If \a height is negative, then there will be _no_ bound on
         * the number of additional crossings.  This means that the
         * routine will _never terminate_, unless \a action returns
         * \c true for some link diagram that is passed to it.
         *
         * Since Regina 7.0, this routine will not return until the exploration
         * of link diagrams is complete, regardless of whether a progress
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
         * \pre This link has fewer than 64 link components.
         *
         * \exception FailedPrecondition This link has 64 or more link
         * components.  If a progress tracker was passed, it will be marked as
         * finished before the exception is thrown.
         *
         * \apinotfinal
         *
         * \python This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the arguments \a tracker and \a args are
         * removed, so you simply call it as rewrite(height, threads, action).
         * Moreover, \a action must take exactly two arguments
         * (const std::string&, Link&&) representing the signature and
         * the link diagram, as described in option (b) above.
         *
         * \param height the maximum number of _additional_ crossings to
         * allow beyond the number of crossings originally present in this
         * link diagram, or a negative number if this should not be bounded.
         * \param threads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \param action a function (or other callable object) to call
         * for each link diagram that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial link argument(s).
         * \return \c true if some call to \a action returned \c true (thereby
         * terminating the search early), or \c false if the search ran to
         * completion.
         */
        template <typename Action, typename... Args>
        bool rewrite(int height, unsigned threads,
            ProgressTrackerOpen* tracker,
            Action&& action, Args&&... args) const;

        /**
         * Forms the composition of this with the given link.  This link will
         * be altered directly, and the given link will be left unchanged.
         *
         * Specifically, this routine will insert a copy of the given link
         * into this link, and will graft its first component into the first
         * component of this link in a way that preserves orientations and
         * crossing signs.  If either this and/or the given link has more than
         * one component, then any additional components will be left alone
         * (i.e., they will remain as different components in the final result).
         *
         * If either link is empty (i.e., contains no components at all),
         * then the result will simply be a clone of the other link
         * (with no composition operation performed).
         *
         * \note If you need to specify which components of the two links to
         * graft together, or if you need to choose the specific arcs at which
         * the graft takes place (which is important when working with _virtual_
         * links), you should use graft() instead.  Note that graft() assumes
         * that both components being grafted together already belong to this
         * link; you can use insertLink() to arrange this.
         *
         * It is allowed to pass this link as \a other.
         *
         * \param other the link with which this should be composed.
         */
        void composeWith(const Link& other);

        /*@}*/
        /**
         * \name Invariants and Related Properties
         */
        /*@{*/

        /**
         * Returns whether this link diagram is alternating.
         *
         * Note that this routine cannot tell whether the _link_ is
         * alternating (i.e., whether there _exists_ an alternating diagram).
         * Instead, it simply returns whether this specific diagram is
         * alternating or not.
         *
         * The empty diagram and any zero-crossing unknot components
         * will be considered alternating.
         *
         * \return \c true if this is an alternating diagram, or \c false
         * if this is a non-alternating diagram.
         */
        bool isAlternating() const;

        /**
         * Returns the linking number of this link, or throws an exception
         * if it is not an integer.
         *
         * The linking number is an invariant of the link, computed as half
         * the sum of the signs of all crossings that involve different link
         * components.
         *
         * For classical links, the linking number is always an integer, and
         * so linking() will always return successfully.
         *
         * For virtual links, the linking number might have a half-integer part;
         * if this happens then linking() will throw an exception.  If you are
         * working with virtual links then you should use linking2() instead,
         * which does not halve the sum of signs, and which therefore always
         * returns successfully with an integer result.
         *
         * The algorithm to compute linking number is linear time.
         *
         * \exception NotImplemented This is a virtual link whose linking
         * number is not an integer.
         *
         * \return the linking number.
         */
        long linking() const;

        /**
         * Returns twice the linking number of this link, which is always an
         * integer for both classical and virtual links.
         *
         * The linking number is an invariant of a link, computed as half
         * the sum of the signs of all crossings that involve different link
         * components.  For classical links the linking number is always an
         * integer, whereas for virtual links it might have a half-integer part.
         *
         * This routine returns _twice_ the linking number, which is always
         * guaranteed to be an integer.  If you are working with virtual links
         * then you should use linking2() instead of linking(), since linking()
         * will throw an exception if its result has a fractional part.
         *
         * The algorithm to compute linking number is linear time.
         *
         * \return twice the linking number.
         */
        long linking2() const;

        /**
         * Returns the writhe of this link diagram.
         *
         * This is _not_ an invariant of the link; instead it depends
         * on the particular link diagram.  It is computed as the sum
         * of the signs of all crossings.  It is preserved under
         * Reidemeister moves II and III, but not I.
         *
         * \return the writhe.
         */
        long writhe() const;

        /**
         * Returns the writhe of a single component of this link diagram.
         *
         * This is the writhe of the diagram when all _other_ components
         * are removed.  It is computed as the sum of the signs of all
         * crossings at which the given component crosses itself.
         *
         * In this version of writheOfComponent(), the component is
         * indicated by the argument \a strand, which may be any strand
         * along the component.  In particular, \a strand does not need to be
         * the "starting strand" returned by component().
         *
         * The given strand may be a null strand, in which case the
         * return value will be 0 (since Regina uses null strands to refer to
         * zero-crossing unknot components).  This is always allowed,
         * regardless of whether the link actually contains any zero-crossing
         * unknot components.
         *
         * \param component any strand along the component of interest.
         * \return the writhe of the component containing the given strand,
         * or 0 if the given strand is a null strand.
         */
        long writheOfComponent(StrandRef component) const;

        /**
         * Returns the writhe of a single component of this link diagram.
         *
         * This is the writhe of the diagram when all _other_ components
         * are removed.  It is computed as the sum of the signs of all
         * crossings at which the given component crosses itself.
         *
         * In this version of writheOfComponent(), the component is
         * indicated by its index.  This function is equivalent to calling
         * `writheOfComponent(component(index))`.
         *
         * \param index the index of the requested component.  This must
         * be between 0 and countComponents()-1 inclusive.
         * \return the writhe of the given component.
         */
        long writheOfComponent(size_t index) const;

        /**
         * Returns the odd writhe, or self-linking number, of this knot.
         *
         * The _odd writhe_ is an invariant of virtual knots, which sums the
         * signs of all odd crossings.  A crossing \a c is _odd_ if, when
         * traversing the knot, we pass through an odd number of crossings
         * between the over-strand and the under-strand of \a c.
         *
         * Some authors call this invariant the _self-linking number_ of the
         * knot.
         *
         * For a classical knot, every crossing will be even, and so the
         * odd writhe will always be zero.
         *
         * \pre This link has exactly one component (i.e., it is a knot).
         *
         * \exception FailedPrecondition This link is empty or has multiple
         * components.
         *
         * \return the odd writhe of this knot.
         */
        long oddWrithe() const;

        /**
         * Determines whether this link diagram is classical (that is, planar).
         * A link diagram that is _not_ classical cannot be drawn in the plane
         * without the addition of virtual crossings.
         *
         * Some notes:
         *
         * - Calling isClassical() is equivalent to testing whether
         *   virtualGenus() is zero.
         *
         * - This is a property of the link _diagram_, not the link itself.
         *   In particular, it is possible for a classical link to be
         *   represented using a non-classical diagram (i.e., a diagram that
         *   requires virtual crossings when drawn in the plane).
         *
         * - As mentioned in the class notes, the Link class does not actually
         *   store virtual crossings; instead it treats the link diagram as
         *   living within some closed orientable surface.  Any discussion of
         *   virtual crossings in the notes above is for exposition only.
         *
         * This routine runs in time linear in the size of the link diagram.
         * However, the virtual genus is cached, and so subsequent calls to
         * isClassical() or virtualGenus() will be instantaneous.
         *
         * \return \c true if and only if this link diagram is classical.
         * (i.e., planar).
         */
        bool isClassical() const;

        /**
         * Determines the virtual genus of this link diagram.  The virtual
         * genus is the smallest genus of closed orientable surface in which
         * the diagram embeds.
         *
         * Note that this is a property of the link _diagram_, not the link
         * itself.
         *
         * For classical link diagrams, the virtual genus will always be zero
         * (since classical link diagrams are by definition planar).
         *
         * This routine runs in time linear in the size of the link diagram.
         * However, the virtual genus is cached, and so subsequent calls to
         * virtualGenus() or isClassical() will be instantaneous.
         *
         * \return the virtual genus of this link diagram.
         */
        size_t virtualGenus() const;

        /**
         * Returns the number of Seifert circles for this link diagram.
         * This is the number of circles obtained when we smooth every
         * crossing in a way that respects the orientations of the strands.
         *
         * In other words: this routine returns the number of link components
         * that would be obtained if we called resolve() on every crossing
         * in the diagram.
         *
         * \return the number of Seifert circles.
         */
        size_t seifertCircles() const;

        /**
         * Returns an ideal triangulation of the complement of this link
         * diagram.  The triangulation will have one ideal vertex for each
         * link component.
         *
         * If this is a classical link diagram:
         *
         * - The triangulation will represent the complement of this link in
         *   the 3-sphere.  If the link diagram is disconnected, then the
         *   resulting 3-manifold will be the connected sum of the complements
         *   of each connected diagram component.
         *
         * If this is a virtual (non-classical) diagram:
         *
         * - A virtual link diagram is embedded in some closed orientable
         *   surface \a S with positive genus.  The triangulation that is
         *   returned will represent the complement of this link diagram in
         *   the thickened surface `S x I`.  There will be two additional
         *   ideal vertices, one for each copy of \a S on the boundary.
         *   If the link diagram is disconnected, then the surface \a S that
         *   is used will be the connected sum of the individual closed
         *   orientable surfaces that host each connected diagram component
         *   (i.e., the resulting triangulation will be connected).
         *
         * Note that for classical links, the complement is a topological
         * invariant of the link; however, for virtual (non-classical) links,
         * the complement (and indeed the genus of the surface \a S) is a
         * property of the specific link diagram.
         *
         * Assuming you pass \a simplify as \c true (the default), the
         * resulting triangulation will typically be no internal vertices;
         * however, this is not guaranteed.
         *
         * Initially, each tetrahedron will be oriented according to a
         * right-hand rule: the thumb of the right hand points from vertices
         * 0 to 1, and the fingers curl around to point from vertices 2 to 3.
         * If you pass \a simplify as \c true, then Regina will attempt to
         * simplify the triangulation to as few tetrahedra as possible:
         * this may relabel the tetrahedra, though their orientations will
         * be preserved.
         *
         * This is the same triangulation that would be produced by passing
         * this link to the Triangulation<3> constructor.
         *
         * \param simplify \c true if and only if the triangulation of the
         * complement should be simplified to use as few tetrahedra as possible.
         * \return the complement of this link diagram.
         */
        Triangulation<3> complement(bool simplify = true) const;

        /**
         * Returns the untwisted positive or negative Whitehead double of this
         * knot.
         *
         * This routine works only with knots, not multiple-component links.
         * If this link is empty or has more than one component, then this
         * routine will throw an exception.
         *
         * This routine creates a new link by (i) creating two parallel copies
         * of the original knot using the Seifert framing, and then (ii) cutting
         * open these two copies and re-connecting them using a clasp.
         * The signs of the two crossings in the clasp are determined by the
         * optional argument \a positive (the default is to use two positive
         * crossings).
         *
         * The two parallel copies of the original link will be oriented as
         * follows: when following the orientation of the original knot, the
         * left copy will have the same orientation, and the right copy will
         * have the reverse orientation.
         *
         * This link will not be modified.
         *
         * \pre This link has exactly one component (i.e., it is a knot).
         *
         * \exception FailedPrecondition This link is empty or has multiple
         * components.
         *
         * \param positive \c true if the clasp should use positive crossings
         * (which builds the _positive_ Whitehead double), or \c false if the
         * clasp should use negative crossings (which builds the _negative_
         * Whitehead double).
         * \return the requested untwisted Whitehead double of this knot.
         */
        Link whiteheadDouble(bool positive = true) const;

        /**
         * Returns \a k cables of this link, all parallel to each
         * other using the given framing.
         *
         * This routine creates a new link by:
         *
         * - treating each component of this link as a ribbon, using the
         *   given framing;
         *
         * - creating \a k parallel copies of the original link,
         *   following each other side-by-side along these ribbons.
         *
         * This link will not be modified.
         *
         * \param k the number of parallel copies to create.
         * This must be non-negative.
         * \param framing the framing under which these copies will be parallel.
         * \return \a k parallel copies of this link.
         */
        Link parallel(int k, Framing framing = Framing::Seifert) const;

        /**
         * Returns the Alexander polynomial of this classical knot.
         *
         * At present, Regina only computes Alexander polynomials for classical
         * knots, not multiple-component links or virtual knots.  If this link
         * is empty, has more than one component, or uses a virtual diagram,
         * then this routine will throw an exception.
         *
         * To pretty-print the Alexander polynomial for human consumption, you
         * can call `Polynomial::str(Link::alexanderVar)`.
         *
         * Bear in mind that each time a link changes, all of its
         * polynomials will be deleted.  Thus the reference that is returned
         * from this routine should not be kept for later use.  Instead,
         * alexander() should be called again; this will be instantaneous if
         * the Alexander polynomial has already been calculated.
         *
         * If this polynomial has already been computed, then the result will
         * be cached and so this routine will be instantaneous (since it just
         * returns the previously computed result).
         *
         * \pre This link diagram is classical (not virtual), and has exactly
         * one component (i.e., it is a knot).
         *
         * \exception FailedPrecondition This link is empty, has multiple
         * components, and/or uses a virtual (not classical) link diagram.
         *
         * \return the Alexander polynomial of this knot.
         */
        const Polynomial<Integer>& alexander() const;
        /**
         * Is the Alexander polynomial of this knot already known?
         * See alexander() for further details.
         *
         * If this property is already known, future calls to alexander() will
         * be very fast (simply returning the precalculated value).
         *
         * At present, Regina only computes Alexander polynomials for classical
         * knots.  If this link is empty, has multiple components, or uses a
         * virtual diagram, then this routine is still safe to call, and will
         * simply return \c false.
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsAlexander() const;

        /**
         * Returns the Kauffman bracket polynomial of this link diagram.
         *
         * Note that the bracket polynomial is not an invariant - it is
         * preserved under Reidemeister moves II and III, but not I.
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * Bear in mind that each time a link changes, all of its
         * polynomials will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, bracket() should be called again; this will be
         * instantaneous if the bracket polynomial has already been calculated.
         *
         * If this polynomial has already been computed, then the result will
         * be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \warning The naive algorithm can only handle a limited number of
         * crossings (currently at most 63).  If you pass Algorithm::Naive and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
         *
         * \exception NotImplemented This link is \e so large that the maximum
         * possible strand ID cannot fit into an \c int.  (On a typical machine
         * where \c int is 32-bit, this would require over a _billion_
         * crossings).  Note that, if you have such a link, then this function
         * (which is exponential time) would be intractably slow anyway.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (Algorithm::Default) is a safe
         * choice.  If you wish to specify a particular algorithm, there are
         * currently two choices: Algorithm::Naive is a slow algorithm that
         * computes the Kauffman bracket by resolving all crossings in all
         * possible ways, and Algorithm::Treewidth uses a fixed-parameter
         * tractable treewidth-based algorithm.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return the bracket polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent<Integer>& bracket(Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the Kauffman bracket polynomial of this link diagram
         * already known?  See bracket() for further details.
         *
         * If this property is already known, future calls to bracket() will
         * be very fast (simply returning the precalculated value).
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsBracket() const;

        /**
         * Returns the Jones polynomial of this link, but with all
         * exponents doubled.
         *
         * By "all exponents doubled", we are indicating that the
         * Jones polynomial is in fact a Laurent polynomial in the
         * square root of \a t.  So, for example:
         *
         * - The right-hand trefoil has Jones polynomial
         *   `1/t + 1/t^3 - 1/t^4`, and so this routine returns the
         *   Laurent polynomial `x^-2 + x^-6 - x^-8`.
         *
         * - The Hopf link has Jones polynomial
         *   `-1/sqrt(x) - 1/sqrt(x^5)`, and so this routine returns
         *   the Laurent polynomial `-x^-1 - x^-5`.
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * Regina follows the conventions described in C. C. Adams,
         * "The knot book", W. H. Freeman & Co., 1994.  If you wish to
         * convert to the conventions used by Khovanov as described in
         * Dror Bar-Natan, "On Khovanov's categorifiction of the Jones
         * polynomial", Algebraic & Geometric Topology 2 (2002), 337-370, you
         * can simply take the polynomial returned by this routine and replace
         * the variable \a x (which represents the square root of \a t)
         * with the expression -<i>q</i>.
         *
         * To pretty-print this polynomial for human consumption, you can
         * call `Laurent::str(Link::jonesVar)`.
         *
         * Bear in mind that each time a link changes, all of its
         * polynomials will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, jones() should be called again; this will be
         * instantaneous if the Jones polynomial has already been calculated.
         *
         * If this polynomial has already been computed, then the result will
         * be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \warning The naive algorithm can only handle a limited number of
         * crossings (currently at most 63).  If you pass Algorithm::Naive and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
         *
         * \exception NotImplemented This link is \e so large that the maximum
         * possible strand ID cannot fit into an \c int.  (On a typical machine
         * where \c int is 32-bit, this would require over a _billion_
         * crossings).  Note that, if you have such a link, then this function
         * (which is exponential time) would be intractably slow anyway.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (Algorithm::Default) is a safe
         * choice.  If you wish to specify a particular algorithm, there are
         * currently two choices: Algorithm::Naive is a slow algorithm that
         * computes the Kauffman bracket by resolving all crossings in all
         * possible ways, and Algorithm::Treewidth uses a fixed-parameter
         * tractable treewidth-based algorithm.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return the Jones polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent<Integer>& jones(Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the Jones polynomial of this link already known?
         * See jones() for further details.
         *
         * If this property is already known, future calls to jones() will be
         * very fast (simply returning the precalculated value).
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsJones() const;

        /**
         * Returns the HOMFLY-PT polynomial of this classical link, as a
         * polynomial in \a alpha and \a z.
         *
         * At present, Regina only computes HOMFLY-PT polynomials for classical
         * links.  If this is a virtual link diagram, then this routine will
         * throw an exception.
         *
         * This variant of the HOMFLY-PT polynomial is described (amongst other
         * places) in G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants of the HOMFLY-PT
         * polynomial are related by a simple transformation:
         * \a alpha = \a l \a i and \a z = -\a m \a i,
         * where \a i represents (as usual) a square root of -1.
         *
         * This routine returns a Laurent polynomial in the two variables
         * \a alpha and \a z (which are represented by \a x and \a y
         * respectively in the class Laurent2).
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * To pretty-print this polynomial for human consumption, you can call
         * `Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)`.
         *
         * The default implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * Bear in mind that each time a link changes, all of its polynomials
         * will be deleted.  Thus the reference that is returned from this
         * routine should not be kept for later use.  Instead, homflyAZ()
         * should be called again; this will be instantaneous if the HOMFLY-PT
         * polynomial has already been calculated.
         *
         * If the HOMFLY-PT polynomial has already been computed (either in
         * terms of \a alpha and \a z or in terms of \a l and \a m), then the
         * result will be cached and so this routine will be very fast (since
         * it just returns the previously computed result).  Otherwise the
         * computation could be quite slow, particularly for larger numbers of
         * crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \exception FailedPrecondition This is a virtual (not classical)
         * link diagram.
         *
         * \exception NotImplemented This link is \e so large that the maximum
         * possible strand ID cannot fit into an \c int.  (On a typical machine
         * where \c int is 32-bit, this would require over a _billion_
         * crossings).  Note that, if you have such a link, then this function
         * (which is exponential time) would be intractably slow anyway.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (Algorithm::Default) is a safe
         * choice.  If you wish to specify a particular algorithm, there are
         * currently two choices: Algorithm::Backtrack will use Kauffman's
         * skein-template algorithm, and Algorithm::Treewidth will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return the HOMFLY-PT polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homflyAZ(Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Returns the HOMFLY-PT polynomial of this classical link, as a
         * polynomial in \a l and \a m.
         *
         * At present, Regina only computes HOMFLY-PT polynomials for classical
         * links.  If this is a virtual link diagram, then this routine will
         * throw an exception.
         *
         * This variant of the HOMFLY-PT polynomial is described (amongst other
         * places) in C. C. Adams, "The knot book", W. H. Freeman & Co., 1994.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants of the HOMFLY-PT
         * polynomial are related by a simple transformation:
         * \a alpha = \a l \a i and \a z = -\a m \a i,
         * where \a i represents (as usual) a square root of -1.
         *
         * This routine returns a Laurent polynomial in the two variables
         * \a l and \a m (which are represented by \a x and \a y
         * respectively in the class Laurent2).
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * To pretty-print this polynomial for human consumption, you can call
         * `Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)`.
         *
         * The default implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * Bear in mind that each time a link changes, all of its polynomials
         * will be deleted.  Thus the reference that is returned from this
         * routine should not be kept for later use.  Instead, homflyLM()
         * should be called again; this will be instantaneous if the HOMFLY-PT
         * polynomial has already been calculated.
         *
         * If the HOMFLY-PT polynomial has already been computed (either in
         * terms of \a alpha and \a z or in terms of \a l and \a m), then the
         * result will be cached and so this routine will be very fast (since
         * it just returns the previously computed result).  Otherwise the
         * computation could be quite slow, particularly for larger numbers of
         * crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \exception FailedPrecondition This is a virtual (not classical)
         * link diagram.
         *
         * \exception NotImplemented This link is \e so large that the maximum
         * possible strand ID cannot fit into an \c int.  (On a typical machine
         * where \c int is 32-bit, this would require over a _billion_
         * crossings).  Note that, if you have such a link, then this function
         * (which is exponential time) would be intractably slow anyway.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (Algorithm::Default) is a safe
         * choice.  If you wish to specify a particular algorithm, there are
         * currently two choices: Algorithm::Backtrack will use Kauffman's
         * skein-template algorithm, and Algorithm::Treewidth will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return the HOMFLY-PT polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homflyLM(Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Returns the HOMFLY-PT polynomial of this classical link, as a
         * polynomial in \a alpha and \a z.
         *
         * This routine is simply an alias for homflyAZ().  See the
         * documentation for homflyAZ() for further details.
         *
         * At present, Regina only computes HOMFLY-PT polynomials for classical
         * links.  If this is a virtual link diagram, then this routine will
         * throw an exception.
         *
         * To pretty-print this polynomial for human consumption, you can call
         * `Laurent2::str(Link::homflyVarX, Link::homflyVarY)`.
         *
         * Bear in mind that each time a link changes, all of its polynomials
         * will be deleted.  Thus the reference that is returned from this
         * routine should not be kept for later use.  Instead, homfly() should
         * be called again; this will be instantaneous if the HOMFLY-PT
         * polynomial has already been calculated.
         *
         * \exception FailedPrecondition This is a virtual (not classical)
         * link diagram.
         *
         * \exception NotImplemented This link is \e so large that the maximum
         * possible strand ID cannot fit into an \c int.  (On a typical machine
         * where \c int is 32-bit, this would require over a _billion_
         * crossings).  Note that, if you have such a link, then this function
         * (which is exponential time) would be intractably slow anyway.
         *
         * \python The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * \param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (Algorithm::Default) is a safe
         * choice.  If you wish to specify a particular algorithm, there are
         * currently two choices: Algorithm::Backtrack will use Kauffman's
         * skein-template algorithm, and Algorithm::Treewidth will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * \param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * \return the HOMFLY-PT polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homfly(Algorithm alg = Algorithm::Default,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the HOMFLY-PT polynomial of this link already known?
         * See homflyAZ() and homflyLM() for further details.
         *
         * If this property is already known, future calls to homfly(),
         * homflyAZ() and homflyLM() will all be very fast (simply returning
         * the precalculated values).
         *
         * At present, Regina only computes HOMFLY-PT polynomials for classical
         * links.  If this is a virtual (not classical) link diagram, then
         * this routine is still safe to call, and will simply return \c false.
         *
         * \return \c true if and only if this property is already known.
         */
        bool knowsHomfly() const;

        /**
         * Converts between the (\a alpha, \a z) and (\a l, \a m)
         * representations of the HOMFLY-PT polynomial.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants are related by a
         * simple transformation: \a alpha = \a l \a i and \a z = -\a m \a i,
         * where \a i represents (as usual) a square root of -1.
         *
         * See homflyAZ() and homflyLM() for further details.
         *
         * \param homflyAZ the HOMFLY-PT polynomial of a link as a polynomial
         * in \a alpha and \a z, where (\a alpha, \a z) are represented by
         * (\a x, \a y) in the class Laurent2<Integer>.
         * \return the HOMFLY-PT polynomial of the same link as a polynomial
         * in \a l and \a m, where (\a l, \a m) are represented by (\a x, \a y)
         * in the class Laurent2<Integer>.
         */
        static Laurent2<Integer> homflyAZtoLM(Laurent2<Integer> homflyAZ);

        /**
         * Returns the group of this link, as constructed from the Wirtinger
         * presentation.
         *
         * In the Wirtinger presentation, each relation is some variant of the
         * form `xy=yz`, where \a y corresponds to the upper strand at some
         * crossing, and \a x and \a z correspond to the two sides of the
         * lower strand at that same crossing.
         *
         * - For classical links, this group will always be isomorphic to the
         *   fundamental group of the link exterior.
         *
         * - For a virtual link whose diagram is embedded in some closed
         *   orientable surface \a S, the group _could_ change depending upon
         *   which side of \a S you view the diagram from.  That is, switching
         *   the upper and lower strands at every crossing could yield
         *   non-isomorphic groups.  As a result, you may wish to call groups()
         *   instead, which builds _both_ group presentations.  See the groups()
         *   documentation for further discussion, or ExampleLink::gpv() for
         *   an example of a virtual knot for which these two groups are indeed
         *   non-isomorphic.
         *
         * If you pass \a simplify as \c false, this routine will keep the
         * Wirtinger presentation and not try to simplify it further.
         * If you pass \a simplify as \c true (the default), this routine will
         * attempt to simplify the group presentation before returning.
         *
         * \note If you have a classical link and you are finding the resulting
         * group presentation too large even after simplification, you could
         * also try calling complement() and computing the fundamental group of
         * the resulting 3-manifold triangulation instead.  Sometimes the
         * presentation obtained via the complement is better, and sometimes
         * it is worse.
         *
         * This group is _not_ cached; instead it is reconstructed every time
         * this function is called.  This behaviour may change in future
         * versions of Regina.
         *
         * \param simplify \c true if we should attempt to simplify the group
         * presentation before returning.
         * \return the group of this link.
         */
        GroupPresentation group(bool simplify = true) const;

        /**
         * Returns the two groups constructed from the Wirtinger presentation
         * for this link and its mirror image.  This function is intended for
         * use with virtual links, where these two groups might not be
         * isomorphic.
         *
         * As with group(), each Wirtinger presentation builds a group using
         * relations of the form `xy=yz`:
         *
         * - In first group that is returned, \a y corresponds to the upper
         *   strand at some crossing, and \a x and \a z correspond to the two
         *   sides of the lower strand at that same crossing.  This is exactly
         *   the same presentation constructed by group().
         *
         * - In the second group that is returned, we conceptually reflect the
         *   link diagram through the surface in which it is embedded (as
         *   though we had called changeAll(), though this link diagram will
         *   not actually be changed).  This means that \a y will correspond to
         *   the _lower_ strand at some crossing, and \a x and \a z correspond
         *   to the two sides of the _upper_ strand at that same crossing.
         *
         * For classical links, both groups will always be isomorphic, and so
         * there is little value in calling this function; you should just use
         * group() instead.
         *
         * For virtual links, these groups might _not_ be isomorphic, and so
         * this pair gives more information than you would obtain by just
         * calling group().  See ExampleLink::gpv() for an example of a virtual
         * knot whose "native" Wirtinger presentation (the first group) gives
         * the trefoil group, but whose "reflected" Wirtinger presentation
         * (the second group) gives the unknot group.
         *
         * If you pass \a simplify as \c false, this routine will keep both
         * Wirtinger presentations and not try to simplify them further.
         * If you pass \a simplify as \c true (the default), this routine will
         * attempt to simplify both group presentations before returning.
         *
         * These groups are _not_ cached; instead they are reconstructed
         * every time this function is called.  This behaviour may change in
         * future versions of Regina.
         *
         * \param simplify \c true if we should attempt to simplify the group
         * presentations before returning.
         * \return the groups of this link obtained by the "native" and
         * "reflected" Wirtinger presentations, as described above.
         */
        std::pair<GroupPresentation, GroupPresentation> groups(
            bool simplify = true) const;

        /**
         * Returns a nice tree decomposition of the 4-valent multigraph formed
         * by this link diagram.  This can (for example) be used in
         * implementing algorithms that are fixed-parameter tractable in the
         * treewidth of this graph.
         *
         * See TreeDecomposition for further details on tree decompositions,
         * and see TreeDecomposition::makeNice() for details on what it
         * means to be a _nice_ tree decomposition.
         *
         * This routine is fast: it will use a greedy algorithm to find
         * a tree decomposition with (hopefully) small width, but with
         * no guarantees that the width of this tree decomposition is
         * the smallest possible.
         *
         * The tree decomposition will be cached, so that if this
         * routine is called a second time (and the underlying link has
         * not been changed) then the same tree decomposition will be
         * returned immediately.
         *
         * If you wish to supply your own tree decomposition (as opposed
         * to relying on the greedy heuristics that Regina implements),
         * then you can supply it by calling useTreeDecomposition().
         *
         * \return a nice tree decomposition of this link diagram.
         */
        const TreeDecomposition& niceTreeDecomposition() const;

        /**
         * Instructs Regina to use the given tree decomposition as the
         * starting point whenever it needs a tree decomposition for this link.
         *
         * For some link routines, including niceTreeDecomposition() as
         * well as computations such as jones() that support the option
         * Algorithm::Treewidth, Regina needs a tree decomposition of the
         * 4-valent multigraph formed by this link diagram.
         *
         * By default, Regina will compute (and then cache) such a tree
         * decomposition itself, using in-built greedy heuristics.  This
         * routine allows you to supply your _own_ tree decomposition
         * (which, for example, might be a smaller-width tree decomposition
         * that you found using third-party software).  By supplying
         * your own tree decomposition \e td through this routine,
         * Regina will throw away any pre-computed tree decomposition
         * that it has cached, and will instead cache \e td for future
         * use instead.
         *
         * Regina may modify the given tree decomposition for its purposes.
         * In particular, \e td does not need to be a _nice_ tree
         * decomposition (indeed, it does not need to have any special
         * properties beyond the definition of a tree decomposition).
         * Regina will automatically create a nice tree decomposition
         * from it if \e td is not nice already.
         *
         * \param td a tree decomposition of the 4-valent multigraph formed
         * by this link diagram.
         */
        void useTreeDecomposition(TreeDecomposition td);

        /*@}*/
        /**
         * \name Exporting Links
         */
        /*@{*/

        /**
         * Outputs this link in Regina's own brief write-only format.
         * This format is concise, but contains enough information to
         * manually reconstruct the complete link diagram.
         *
         * This format cannot (yet) be used to read links back into Regina,
         * and so it is not good for external storage, or for passing links
         * between different programs (or even different instances of Regina).
         * It was originally designed for use with the test suite, where it
         * was used to ensure that links with being created and/or manipulated
         * correctly.
         *
         * The output will contain the following elements, separated by
         * single spaces:
         *
         * - a sequence of signs (`+` or `-`), concatenated
         *   together, giving the signs of the crossings in order from
         *   crossing 0 to crossing size()-1;
         *
         * - a description of each component of the link, in order from
         *   component 0 to component countComponents()-1.  Each component
         *   will be written in the form `( a b c ... )`, indicating
         *   the crossings that are encountered as we follow the component
         *   in the forward direction from its starting strand.  Each element
         *   \a a, \a b, \a c and so on will be written in the format used by
         *   the StrandRef class: either `^n` when passing over
         *   crossing \a n, or `_n` when passing under crossing \a n.
         *
         * For example, the Whitehead link as returned by
         * ExampleLink.whitehead() will give the following brief output:
         *
           \verbatim
           --++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )
           \endverbatim
         *
         * As a special case, if the link contains no crossings, then
         * the output will not begin with a space; instead it will
         * simply be a sequence of the form `( ) ( ) ... ( )`.
         *
         * The string will not end in a newline.
         *
         * There is also a variant of brief() that writes directly to an
         * output stream.
         *
         * \return a description of this link in Regina's brief format.
         */
        std::string brief() const;

        /**
         * Writes this link in Regina's own brief format to the given
         * output stream.
         *
         * See brief() for a full description of Regina's brief format,
         * as well as its limitations.
         *
         * The output from this routine is precisely the string that
         * would be returned by brief().  In particular, the output does
         * not contain any newlines.
         *
         * See also brief(), which returns the brief format as a string.
         *
         * \nopython Instead use the variant brief() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         */
        void brief(std::ostream& out) const;

        /**
         * Returns a classical Gauss code for this knot, presented as a string.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.
         * These codes come with major restrictions:
         *
         * - In general, they do not carry enough information to uniquely
         *   reconstruct a classical knot.  For instance, both a classical
         *   knot and its reflection can be described by the same Gauss code;
         *   moreover, for _composite_ knots, the same Gauss code can describe
         *   inequivalent knots even when allowing for reflections.
         *
         * - Parsing a Gauss code to reconstruct a classical knot is complex,
         *   since it requires an embedding to be deduced using some variant
         *   of a planarity testing algorithm.
         *
         * - Because Gauss codes rely on planarity, they are not suitable at
         *   all for working with virtual knots.
         *
         * If you need a code that specifies the knot uniquely, and/or is fast
         * to parse, and/or can work with both classical and virtual knots,
         * you should use the _oriented_ Gauss code instead, which resolves
         * all of these issues.
         *
         * The contents of a classical Gauss code are as follows.
         * A Gauss code for an <i>n</i>-crossing knot is described by
         * a sequence of 2<i>n</i> positive and negative integers,
         * representing strands that pass over and under crossings
         * respectively.  The code is constructed as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   Whenever you pass crossing \a k, write the integer `k` if you
         *   pass over the crossing, or `-k` if you pass under the crossing.
         *
         * As an example, you can represent the trefoil using the code:
         *
           \verbatim
           1 -2 3 -1 2 -3
           \endverbatim
         *
         * Currently Regina only supports Gauss codes for knots, not
         * empty or multiple component links.  If this link does not
         * have precisely one component, then this routine will throw an
         * exception.  It is possible that in future versions of Regina,
         * Gauss codes will be expanded to cover all possible link diagrams
         * (hence the choice of NotImplemented as the exception type).
         *
         * This routine formats the list of integers as a string.  The integers
         * will be separated by single spaces, and there will be no newlines.
         *
         * The routine gaussData() returns this same data in machine-readable
         * format (as a C++ vector), instead of the human-readable format
         * used here (a string).  There is also another variant of gauss()
         * that writes directly to an output stream.
         *
         * Although classical Gauss codes do not support virtual knots, if
         * this is a virtual link diagram then gauss() will still produce
         * correct output; the problem is simply that too much information is
         * lost, and you cannot _reconstruct_ your virtual link from this
         * output.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \return a classical Gauss code as described above.
         */
        std::string gauss() const;

        /**
         * Returns a classical Gauss code for this knot, presented as a
         * vector of integers.
         *
         * See gauss() for a full description of classical Gauss codes as
         * they are used in Regina, as well as their limitations.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, gauss() returns the same data in human-readable format
         * (as a string).
         *
         * \exception NotImplemented This link is empty, or has multiple
         * components, or has so many crossings that the Gauss code
         * cannot be expressed using native C++ integers.
         *
         * \return a classical Gauss code for this knot in machine-readable
         * form.
         */
        std::vector<int> gaussData() const;

        /**
         * Writes a classical Gauss code for this knot to the given output
         * stream.
         *
         * See gauss() for a full description of classical Gauss codes as
         * they are used in Regina, as well as their limitations.
         *
         * The output from this routine is precisely the string that
         * would be returned by gauss().  In particular, the output does
         * not contain any newlines.
         *
         * For a function that _returns_ the Gauss code (as opposed to writing
         * it to an output stream), you could use gauss() (which returns the
         * Gauss code as a human-readable string), or gaussData() (which
         * returns it as a machine-readable sequence of integers).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \nopython Instead use the variants gauss() or gaussData() that
         * take no arguments.
         *
         * \param out the output stream to which to write.
         */
        void gauss(std::ostream& out) const;

        /**
         * Returns an oriented Gauss code for this knot, presented as a string.
         *
         * The oriented Gauss code, based on a format used by Andreeva et al.,
         * is an extension of the classical Gauss code with additional
         * characters to describe the orientation of the other strand
         * passing by at each crossing.  This extra information removes
         * both the topological ambiguities and the complexity in the
         * reconstruction procedure for classical Gauss codes.  It also makes
         * the code suitable for both virtual and classical knots.
         *
         * This "oriented" format is described at
         * http://www.javaview.de/services/knots/doc/description.html#gc,
         * and it works as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   At every crossing that you pass, write a token of the form
         *   `+<k`, `-<k`, `+>k` or `->k`, where:
         *
         *     * the symbol `+` indicates that you are passing over the
         *       crossing labelled \a k, and the symbol `-` indicates
         *       that you are passing under the crossing labelled \a k;
         *
         *     * the symbol `<` indicates that the other strand of
         *       the crossing passes from right to left, and `>`
         *       indicates that the other strand passes from left to right;
         *
         *     * \a k is replaced with the integer crossing label.
         *
         * As an example, you can represent the left-hand trefoil using the
         * code:
         *
           \verbatim
           +>1 -<2 +>3 -<1 +>2 -<3
           \endverbatim
         *
         * Note that _oriented_ Gauss codes are different from _signed_ Gauss
         * codes.  Both formats improve upon classical Gauss codes by resolving
         * the topological ambiguities and making reconstruction easy; however,
         * they do so in different ways.
         *
         * Currently Regina only supports Gauss codes for knots, not
         * empty or multiple component links.  If this link does not
         * have precisely one component, then this routine will throw an
         * exception.  It is possible that in future versions of Regina,
         * Gauss codes will be expanded to cover all possible link diagrams
         * (hence the choice of NotImplemented as the exception type).
         *
         * This routine joins the tokens together as a single string.  The
         * tokens will be separated by single spaces, and there will be no
         * newlines.
         *
         * The routine orientedGaussData() returns this same data in
         * machine-readable format (as a C++ vector of string tokens), instead
         * of the human-readable format used here (a single long string).
         * There is also another variant of orientedGauss() that writes
         * directly to an output stream.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \return an oriented Gauss code as described above.
         */
        std::string orientedGauss() const;

        /**
         * Returns an oriented Gauss code for this knot, presented as a
         * vector of string tokens.
         *
         * See orientedGauss() for a full description of oriented Gauss codes
         * as they are used in Regina, as well as their limitations.
         *
         * For an <i>n</i>-crossing knot, the elements of the returned vector
         * will be the 2<i>n</i> individual tokens of the form
         * `+<k`, `-<k`, `+>k` or `->k` that would normally
         * be joined with whitespace to form a complete oriented Gauss code.
         * For example, for the left-hand trefoil, the vector might contain
         * the six tokens:
         *
           \verbatim
           { "+>1", "-<2", "+>3", "-<1", "+>2", "-<3" }
           \endverbatim
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, orientedGauss() returns the same data in
         * human-readable format (as a string).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \return an oriented Gauss code for this knot in machine-readable
         * form.
         */
        std::vector<std::string> orientedGaussData() const;

        /**
         * Writes an oriented Gauss code for this knot to the given output
         * stream.
         *
         * See orientedGauss() for a full description of oriented Gauss codes
         * as they are used in Regina, as well as their limitations.
         *
         * The output from this routine is precisely the string that
         * would be returned by orientedGauss().  In particular, the output
         * does not contain any newlines.
         *
         * For a function that _returns_ the oriented Gauss code (as opposed to
         * writing it to an output stream), you could use orientedGauss()
         * (which returns the oriented Gauss code as a human-readable string),
         * or orientedGaussData() (which returns it as a machine-readable
         * sequence of tokens).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \nopython Instead use the variants orientedGauss() or
         * orientedGaussData() that take no arguments.
         *
         * \param out the output stream to which to write.
         */
        void orientedGauss(std::ostream& out) const;

        /**
         * Returns a signed Gauss code for this knot, presented as a string.
         *
         * The signed Gauss code, as described by Kauffman, modifies the
         * classical Gauss code to indicate which crossings are positive
         * and which are negative.  This extra information removes both the
         * topological ambiguities and the complexity in the reconstruction
         * procedure for classical Gauss codes.  It also makes the code
         * suitable for both virtual and classical knots.
         *
         * Be warned that for signed Gauss codes, the signs `+/-` play a
         * very different role from classical Gauss codes: in signed Gauss
         * codes they indicate positive versus negative crossings, whereas in
         * classical Gauss codes they indicate upper versus lower strands.
         *
         * This format is used in Louis H. Kauffman, "Virtual knot theory",
         * European J. Combin. 20 (1999), no. 7, 663-690.  It works as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   At every crossing that you pass, write symbols of the form
         *   `Ok+`, `Ok-`, `Uk+` or `Uk-`, where:
         *
         *     * the symbol `O` indicates that you are passing over the
         *       crossing labelled \a k, and the symbol `U` indicates
         *       that you are passing under the crossing labelled \a k;
         *
         *     * the symbol `+` indicates that the crossing labelled \a k
         *       is positive, and the symbol `-` indicates that the crossing
         *       labelled \a k is negative;
         *
         *     * \a k is replaced with the integer crossing label.
         *
         * - All of the symbols should be concatenated together, without any
         *   separation by whitespace.
         *
         * As an example, you can represent the figure eight knot using the
         * code:
         *
           \verbatim
           U1+O2+U3-O4-U2+O1+U4-O3-
           \endverbatim
         *
         * Note that _signed_ Gauss codes are different from _oriented_ Gauss
         * codes.  Both formats improve upon classical Gauss codes by resolving
         * the topological ambiguities and making reconstruction easy; however,
         * they do so in different ways.
         *
         * Currently Regina only supports Gauss codes for knots, not
         * empty or multiple component links.  If this link does not
         * have precisely one component, then this routine will throw an
         * exception.  It is possible that in future versions of Regina,
         * Gauss codes will be expanded to cover all possible link diagrams
         * (hence the choice of NotImplemented as the exception type).
         *
         * The routine signedGaussData() returns this same data in
         * machine-readable format (as a C++ vector of shorter string tokens,
         * one for each crossing that you pass), instead of the single long
         * string that is returned here.  There is also another variant of
         * signedGauss() that writes directly to an output stream.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \return a signed Gauss code as described above.
         */
        std::string signedGauss() const;

        /**
         * Returns a signed Gauss code for this knot, presented as a
         * vector of string tokens.
         *
         * See signedGauss() for a full description of signed Gauss codes
         * as they are used in Regina, as well as their limitations.
         *
         * For an <i>n</i>-crossing knot, the elements of the returned vector
         * will be the 2<i>n</i> individual tokens of the form
         * `Ok+`, `Ok-`, `Uk+` or `Uk-` that would normally be concatenated
         * together to form a complete signed Gauss code.  For example, for
         * the figure eight knot, the vector might contain the eight tokens:
         *
           \verbatim
           { "U1+", "O2+", "U3-", "O4-", "U2+", "O1+", "U4-", "O3-" }
           \endverbatim
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, signedGauss() returns the same data in
         * human-readable format (as a single long string).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \return a signed Gauss code for this knot in machine-readable form.
         */
        std::vector<std::string> signedGaussData() const;

        /**
         * Writes a signed Gauss code for this knot to the given output stream.
         *
         * See signedGauss() for a full description of signed Gauss codes
         * as they are used in Regina, as well as their limitations.
         *
         * The output from this routine is precisely the string that
         * would be returned by signedGauss().  In particular, the output
         * does not contain any newlines.
         *
         * For a function that _returns_ the signed Gauss code (as opposed to
         * writing it to an output stream), you could use signedGauss()
         * (which returns the signed Gauss code as a human-readable string),
         * or signedGaussData() (which returns it as a machine-readable
         * sequence of tokens).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \nopython Instead use the variants signedGauss() or signedGaussData()
         * that take no arguments.
         *
         * \param out the output stream to which to write.
         */
        void signedGauss(std::ostream& out) const;

        /**
         * Exports this link using Bob Jenkins' text format, returning a
         * single string.
         *
         * Jenkins' format is lengthy.  However, in contrast to classical
         * Gauss codes or Dowker-Thistlethwaite notation, there are no
         * topological ambiguities in the format, and reconstructing a link
         * from Jenkins' format is simple.  Moreover, the format is suitable
         * for links with any number of components, and can be used with both
         * virtual and classical links.
         *
         * Jenkins' format is described in his HOMFLY-PT polynomial software,
         * which is available online from
         * http://burtleburtle.net/bob/knot/homfly.html.
         * The format consists of a sequence of integers separated by
         * whitespace, constructed as follows:
         *
         * - Label the crossings arbitrarily as 0, 1, ..., <i>n</i>-1.
         *
         * - Write the number of components in the link.
         *
         * - Next, for each link component:
         *   + write the number of times you pass a crossing when traversing
         *     the component (i.e., the length of the component);
         *   + write two integers for each crossing that you pass in such a
         *     traversal: the crossing label, and then +1 if you pass over
         *     the crossing or -1 if you pass under the crossing.
         *
         * - Finally, for each crossing:
         *   + write the crossing label;
         *   + write the sign of the crossing (either +1 or -1).
         *
         * As an example, you could describe the left-hand trefoil
         * using the following sequence:
         *
           \verbatim
           1
           6   0 1   1 -1   2 1   0 -1   1 1   2 -1
           0 -1   1 -1   2 -1
           \endverbatim
         *
         * Another example is the Hopf link, which you could describe
         * using the following sequence:
         *
           \verbatim
           2
           2   0 1   1 -1
           2   0 -1   1 1
           0 1   1 1
           \endverbatim
         *
         * The string that is returned will contain multiple lines,
         * and will end in a newline.  The specific choice of whitespace
         * (i.e., the "formatting" of the sequence) may change in future
         * versions of Regina.
         *
         * The routine jenkinsData() returns this same data in machine-readable
         * format (as a C++ vector), instead of the human-readable format
         * used here (a string).  There is also another variant of jenkins()
         * that writes directly to an output stream.
         *
         * \return a description of this link using Jenkins' text format.
         */
        std::string jenkins() const;

        /**
         * Exports this link using Bob Jenkins' text format, returning a
         * vector of integers.
         *
         * See jenkins() for a full description of Jenkins' format as it
         * is used in Regina.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, jenkins() returns the same data in human-readable
         * format (as a string).
         *
         * \exception NotImplemented This link has so many crossings
         * and/or components that its description in Jenkins' format
         * cannot be expressed using native C++ integers.
         *
         * \return a description of this link using Jenkins' format
         * in machine-readable form.
         */
        std::vector<int> jenkinsData() const;

        /**
         * Exports this link to the given output stream using Bob Jenkins'
         * text format.
         *
         * See jenkins() for a full description of Jenkins' format as it
         * is used in Regina.
         *
         * The output from this routine is precisely the string that
         * would be returned by jenkins().  In particular, the output will
         * typically span multiple lines, and will finish with a newline.
         *
         * For a function that _returns_ the link in Jenkins' format (as
         * opposed to writing it to an output stream), you could use jenkins()
         * (which returns the description as a human-readable string), or
         * jenkinsData() (which returns it as a machine-readable sequence of
         * integers).
         *
         * \nopython Instead use the variants jenkins() or jenkinsData() that
         * take no arguments.
         *
         * \param out the output stream to which to write.
         */
        void jenkins(std::ostream& out) const;

        /**
         * Exports this classical knot in either numerical or alphabetical
         * Dowker-Thistlethwaite notation, returning a string.
         *
         * Like classical Gauss codes, Dowker-Thistlethwaite notation
         * essentially describes the 4-valent graph of a knot but not the
         * particular embedding in the plane.  It comes with major restrictions:
         *
         * - It relies on parity properties that only hold for classical knots.
         *   As a result, Dowker-Thistlethwaite notation cannot be used with
         *   virtual knots at all.
         *
         * - Even for classical knots, it does not carry enough information to
         *   uniquely reconstruct a knot.  For instance, both a knot and its
         *   reflection can be described by the same Dowker-Thistlethwaite
         *   notation; moreover, for _composite_ knots, the same notation can
         *   describe inequivalent knots even when allowing for reflections.
         *
         * - Parsing Dowker-Thistlethwaite notation to reconstruct a classical
         *   knot is complex, since it requires an embedding to be deduced
         *   using some variant of a planarity testing algorithm.
         *
         * If you need a code that specifies the knot uniquely, and/or is fast
         * to parse, and/or can work with both classical and virtual knots,
         * you should use the _oriented_ Gauss code instead, which resolves
         * all of these issues.
         *
         * For an <i>n</i>-crossing knot, Regina supports two variants
         * of Dowker-Thistlethwaite notation:
         *
         * - a _numerical_ variant (the default), which is a sequence of
         *   \a n even signed integers as described (amongst other places) in
         *   Section 2.2 of C. C. Adams, "The knot book", W. H. Freeman & Co.,
         *   1994;
         *
         * - an _alphabetical_ variant, which transforms the numerical
         *   notation into a sequence of letters by replacing positive
         *   integers (2,4,6,...) with lower-case letters (\c a,\c b,\c c,...),
         *   and replacing negative integers (-2,-4,-6,...) with upper-case
         *   letters (\c A,\c B,\c C,...).  This alphabetical variant
         *   can only be used for knots with 26 crossings or fewer; for
         *   larger knots this routine will throw an exception if the
         *   alphabetical variant is requested.
         *
         * As an example, you can describe the trefoil using numerical
         * Dowker-Thistlethwaite notation as:
         *
           \verbatim
           4 6 2
           \endverbatim
         *
         * In alphabetical Dowker-Thistlethwaite notation, this becomes:
         *
           \verbatim
           bca
           \endverbatim
         *
         * Currently Regina only supports Dowker-Thistlethwaite codes for
         * knots, not empty or multiple component links.  If this link does not
         * have precisely one component, then this routine will throw an
         * exception.  It is possible that in future versions of Regina,
         * Dowker-Thistlethwaite codes will be expanded to cover all possible
         * link diagrams (hence the choice of NotImplemented as the exception
         * type).
         *
         * For numerical Dowker-Thistlethwaite notation, this routine will
         * format the list of integers as a string.  The integers will be
         * separated by single spaces, and there will be no newlines.
         * For alphabetical Dowker-Thistlethwaite notation, the string that
         * is returned will not contain any whitespace at all.
         *
         * For the numerical variant, the routine dtData() returns this same
         * data in machine-readable format (as a C++ vector), instead of the
         * human-readable format used here (a string).  There is also another
         * variant of dt() that can write either the numerical or the
         * alphabetical variant directly to an output stream.
         *
         * \exception NotImplemented Either this link is empty or has multiple
         * components, or this is a virtual (not classical) link diagram, or
         * \a alpha is true and this link diagram has more than 26 crossings.
         *
         * \param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         * \return the Dowker-Thistlethwaite notation for this knot diagram.
         */
        std::string dt(bool alpha = false) const;

        /**
         * Exports this classical knot in numerical Dowker-Thistlethwaite
         * notation, returning a vector of integers.
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, as well as its limitations.
         *
         * Although Regina can work with both the numerical and alphabetical
         * variants of Dowker-Thistlethwaite notation, this dtData() routine
         * exports the numerical variant only.  If you wish to export the
         * alphabetical variant, you can call `dt(true)`.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, calling `dt()` returns the same integer
         * sequence in human-readable format (as a string).
         *
         * \exception NotImplemented Either this link is empty or has multiple
         * components, or this is a virtual (not classical) link diagram, or
         * this diagram has so many crossings that the Dowker-Thistlethwaite
         * notation cannot be expressed using native C++ integers.
         *
         * \return the numerical Dowker-Thistlethwaite notation in
         * machine-readable form.
         */
        std::vector<int> dtData() const;

        /**
         * Writes this classical knot to the given output stream using
         * Dowker-Thistlethwaite notation.
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, as well as its limitations.
         *
         * This routine can write either numerical or alphabetical
         * Dowker-Thistlethwaite notation, as indicated by the optional
         * argument \a alpha.
         *
         * The output from this routine is precisely the string that
         * would be returned by dt(bool).  In particular, the output does
         * not contain any newlines.
         *
         * For a function that _returns_ the Dowker-Thistlethwaite notation
         * (as opposed to writing it to an output stream), you could use
         * dt(bool) (which returns the Dowker-Thistlethwaite notation as a
         * human-readable string), or dtData() (which returns the numerical
         * Dowker-Thistlethwaite notation as a machine-readable sequence of
         * integers).
         *
         * \exception NotImplemented Either this link is empty or has multiple
         * components, or this is a virtual (not classical) link diagram, or
         * \a alpha is true and this link diagram has more than 26 crossings.
         *
         * \nopython Instead use the variants dt(bool) or dtData() that take
         * no arguments.
         *
         * \param out the output stream to which to write.
         * \param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         */
        void dt(std::ostream& out, bool alpha = false) const;

        /**
         * Returns a planar diagram code for this link, presented as a string.
         *
         * Planar diagram codes encode the local information at each crossing,
         * and present this information as a list of 4-tuples.  These codes
         * are available for links as well as knots.  Moreover (despite their
         * name) they are available for virtual as well as classical links.
         * However, they do come with some minor restrictions:
         *
         * - They cannot encode zero-crossing unknot components (i.e.,
         *   components for which the component() function returns a null
         *   strand).  Any such components will simply be omitted from the code.
         *   You can detect such components by calling countTrivialComponents().
         *
         * - If a link has any components that consist entirely of
         *   over-crossings (that is, zero-crossing components that are "placed
         *   on top of" the rest of the link diagram), then a planar diagram
         *   code does not carry enough data to reconstruct the _orientation_
         *   of these components.  For classical links, the topology will still
         *   be preserved (since such components must be topological unknots),
         *   but in general the combinatorics of such a link diagram cannot be
         *   reconstructed faithfully.  For virtual links, the problems are
         *   more serious (since such components may traverse handles in the
         *   surface in which the link diagram is embedded).  In all cases,
         *   you can detect such components by calling pdAmbiguous().
         *
         * If you need a text code that can work with these types of
         * link diagrams, you can always use Jenkins' format instead.
         *
         * Regina adheres to a tight specification for the planar diagram codes
         * that it outputs, in order to ensure compatibility with other
         * software.  In particular, Regina's codes are compatible with the
         * Knot Atlas, as seen at http://katlas.org/wiki/Planar_Diagrams.
         *
         * In detail: a planar diagram code for an <i>n</i>-crossing link
         * is formed from a sequence of \a n 4-tuples of integers.
         * Regina constructs this sequence as follows:
         *
         * - Throw away any zero-crossing unknot components.
         *
         * - Let \a n denote the number of crossings.
         *
         * - Number the strands from 1 to 2<i>n</i> in order as we walk along
         *   each component, in order from the first component to the last.
         *
         * - For each crossing \a c, construct a 4-tuple that lists the four
         *   strands that meet at that \a c, in counter-clockwise order,
         *   beginning from the incoming lower strand.
         *
         * - Return the resulting list of \a n 4-tuples.
         *
         * An example, you can represent the right-hand trefoil using the code:
         *
           \verbatim
           [[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
           \endverbatim
         *
         * Some points to be aware of:
         *
         * - When building the list of 4-tuples, Regina orders the
         *   crossings as follows: again we walk along each component,
         *   in order from the first component to the last, and process
         *   each crossing when we enter it at the lower strand.
         *
         * - When building each individual 4-tuple, some sources order the
         *   strands clockwise instead of counter-clockwise.  Regina follows
         *   the same counter-clockwise convention that is used by the
         *   Knot Atlas and SnapPy.
         *
         * This routine formats the list of 4-tuples as a string, in a way
         * that is consistent with the description in the Knot Atlas at
         * http://katlas.org/wiki/Planar_Diagrams.
         *
         * In particular, each 4-tuple will be formatted with square
         * brackets, commas, and the prefix \c X, and the main list will
         * be formatted with square brackets, commas, and the prefix \c PD.
         * An example (for the right-handed trefoil) is:
         *
           \verbatim
           PD[X[1, 5, 2, 4], X[3, 1, 4, 6], X[5, 3, 6, 2]]
           \endverbatim
         *
         * The routine pdData() returns this same data in machine-readable
         * format (as a C++ vector of 4-tuples of integers), instead of the
         * human-readable format used here (a single string).  There is also
         * another variant of pd() that writes directly to an output stream.
         *
         * \return the planar diagram code, as described above.
         */
        std::string pd() const;

        /**
         * Returns a planar diagram code for this link, presented as
         * vector of 4-tuples.
         *
         * See pd() for a full description of planar diagram codes as they are
         * used in Regina, as well as their limitations.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, pd() returns the same data in human-readable format
         * (as a string).
         *
         * \exception NotImplemented This link has so many crossings that the
         * planar diagram code cannot be expressed using native C++ integers.
         *
         * \return the planar diagram code in machine-readable form.
         */
        std::vector<std::array<int, 4>> pdData() const;

        /**
         * Writes a planar diagram code for this link to the given output
         * stream.
         *
         * See pd() for a full description of planar diagram codes as they are
         * used in Regina, as well as their limitations.
         *
         * The output from this routine is precisely the string that would be
         * returned by pd().  In particular, the output does not contain any
         * newlines.
         *
         * For a function that _returns_ the planar diagram code (as opposed to
         * writing it to an output stream), you could use pd() (which returns
         * the code as a human-readable string), or pdData() (which returns it
         * as a machine-readable sequence of 4-tuples of integers).
         *
         * \nopython Instead use the variants pd() or pdData() that take no
         * arguments.
         *
         * \param out the output stream to which to write.
         */
        void pd(std::ostream& out) const;

        /**
         * Determines whether this link has any components whose orientations
         * cannot be recovered from a planar diagram code.
         *
         * Such components must have at least one crossing, and must consist
         * _entirely_ of over-crossings.  See pd() for a detailed discussion
         * on such components (which must be trivial for classical links,
         * but which could be more interesting for virtual links).
         *
         * Note that planar diagram codes have another limitation, which is
         * that they cannot represent zero-crossing components at all (any such
         * components are omitted from planar diagram codes entirely).
         * Zero-crossing components are _not_ recognised by this routine, but
         * can be recognised instead by calling countTrivialComponents().
         *
         * \return \c true if and only if some component of this link has at
         * least one crossing and consists entirely of over-crossings.
         */
        bool pdAmbiguous() const;

        /**
         * Outputs the underlying 4-valent multigraph for this link diagram
         * using the PACE text format.  This format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/,
         * and is documented in detail by the routine pace().
         *
         * Calling `link.writePACE(out)` is equivalent to `out << link.pace()`.
         * However, this routine is more efficient.
         *
         * See the pace() documentation for further details.
         *
         * \nopython Instead use the variant pace() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        void writePACE(std::ostream& out) const;
        /**
         * Returns a text representation of the underlying 4-valent multigraph
         * for this link diagram, using the PACE text format.
         * This format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/.
         *
         * In summary, the PACE text representation will consist of several
         * lines of text:
         *
         * - The first line will be of the form
         *   `p tw <num_vertices> <num_edges>`.
         *   Note that, since the underlying graph comes from a link diagram,
         *   we will always have \a num_edges equal to twice \a num_vertices.
         *
         * - Following this will be \a num_edges lines, one for each
         *   edge, each of the form `<u> <v>`, indicating
         *   an edge from vertex number \a u to vertex number \a v.
         *   In this format, vertices are numbered 1,2,...,\a num_vertices.
         *
         * An example of this text format is as follows:
         *
           \verbatim
           p tw 4 8
           1 2
           1 4
           1 2
           2 3
           3 4
           1 3
           3 4
           2 4
           \endverbatim
         *
         * If you are writing this text representation to an output stream
         * then you should call writePACE() instead, which is more efficient.
         *
         * \return the PACE text representation of the underlying 4-valent
         * multigraph, as outlined above.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        std::string pace() const;

        /**
         * Returns C++ or Python source code that can be used to reconstruct
         * this link.
         *
         * This code will call Link::fromData(), passing a series of
         * hard-coded C++ initialiser lists or Python lists (depending on the
         * requested language).
         *
         * The main purpose of this routine is to generate these hard-coded
         * lists, which can be tedious and error-prone to write by hand.
         *
         * \param language the language in which the source code should be
         * written.
         * \return the source code that was generated.
         */
        std::string source(Language language = Language::Current) const;

        /**
         * Deprecated routine that returns C++ code to reconstruct this link.
         *
         * \deprecated This is equivalent to calling `source(Language::Cxx)`,
         * for compatibility with older versions of Regina.  In particular,
         * it is _not_ equivalent to calling `source()` (which defaults to the
         * programming language currently being used).  See source() for
         * further details.
         *
         * \return the C++ code that was generated.
         */
        [[deprecated]] std::string dumpConstruction() const;

        /**
         * Constructs the _signature_ for this knot or link diagram.
         *
         * A _signature_ is a compact text representation of a link diagram
         * that uniquely determines the diagram up to any combination of:
         *
         * - relabelling;
         *
         * - (optionally) reflecting the entire diagram, which changes the sign
         *   of every crossing but leaves the upper and lower strands the same;
         *
         * - (optionally) reversing some or all link components;
         *
         * - (optionally) rotating the entire diagram, which preserves the sign
         *   of every crossing but switches the upper and lower strands.
         *
         * Signatures are now supported for all link diagrams with fewer than
         * 64 link components.  Specifically:
         *
         * - Regina 7.3 and earlier only offered signatures for knots.
         *   As of Regina 7.4, signatures are now supported for arbitrary
         *   link diagrams (but see the next point), and for knots the new
         *   signatures are identical to the old.
         *
         * - The implementation uses bitmasks, and a side-effect of this is
         *   that it can only support fewer than 64 link components.  However,
         *   since the running time is exponential in the number of components
         *   (if we allow reversal, which is the default) then it would be
         *   completely infeasible to use this routine in practice with _more_
         *   components than this.  If there are 64 or more link components
         *   then this routine will throw an exception.
         *
         * The signature is constructed entirely of printable characters,
         * and has length proportional to `n log n`, where \a n
         * is the number of crossings.
         *
         * The routine fromSig() can be used to recover a link diagram from
         * its signature.  The resulting diagram might not be identical to
         * the original, but it will be related by zero or more applications
         * of relabelling, and (according to the arguments) reflection of the
         * diagram, rotation of the diagram, and/or reversal of individual
         * link components.
         *
         * The running time is quadratic in the number of crossings and (if we
         * allow reversal, which is the default) exponential in the number of
         * link components.  For this reason, signatures should not be used
         * for links with a large number of components.
         *
         * This routine runs in quadratic time.
         *
         * \exception NotImplemented This link diagram has 64 or more link
         * components.
         *
         * \param allowReflection \c true if reflecting the entire link diagram
         * should preserve the signature, or \c false if the signature should
         * distinguish between a diagram and its reflection (unless of course
         * there is a symmetry).
         * \param allowReversal \c true if reversing some or all link components
         * should preserve the signature, or \c false if the signature should
         * distinguish between different orientations (again, unless of course
         * there are symmetries).
         * \param allowRotation \c true if rotating the entire link diagram
         * should preserve the signature, or \c false if the signature should
         * distinguish between a diagram and its rotation (again, unless there
         * is a symmetry).
         * \return the signature for this link diagram.
         */
        std::string sig(bool allowReflection = true, bool allowReversal = true,
            bool allowRotation = true) const;

        /**
         * Alias for sig(), which constructs the signature for this
         * knot or link diagram.
         *
         * This alias knotSig() has been kept to reflect the fact that, in
         * older versions of Regina, these signatures were only available for
         * single-component knots; moreover the old name "knot signatures" can
         * still be found in the literature.  While this routine is not
         * deprecated, it is recommended to use sig() in new code.
         *
         * See sig() for further details.
         *
         * \exception NotImplemented This link diagram has 64 or more link
         * components.
         *
         * \param allowReflection \c true if reflecting the entire link diagram
         * should preserve the signature, or \c false if the signature should
         * distinguish between a diagram and its reflection (unless of course
         * there is a symmetry).
         * \param allowReversal \c true if reversing some or all link components
         * should preserve the signature, or \c false if the signature should
         * distinguish between different orientations (again, unless of course
         * there are symmetries).
         * \param allowRotation \c true if rotating the entire link diagram
         * should preserve the signature, or \c false if the signature should
         * distinguish between a diagram and its rotation (again, unless there
         * is a symmetry).
         * \return the signature for this link diagram.
         */
        std::string knotSig(bool allowReflection = true,
            bool allowReversal = true, bool allowRotation = true) const;

        /**
         * Writes the tight encoding of this link to the given output stream.
         * See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Writes a short text representation of this link to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this link to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /*@}*/
        /**
         * \name Building Links
         */
        /*@{*/

        /**
         * Inserts a new (\a p, \a q) torus link into this link.
         *
         * The parameters \a p and \a q must be non-negative, but they do
         * not need to be coprime.
         *
         * All of the crossings in the new torus link component(s) will be
         * positive if the argument \a positive is \c true, or negative
         * otherwise.
         *
         * The new crossings and components will be inserted at the
         * end of the respective lists in this link.
         *
         * If your aim is to create a new torus link (as opposed to
         * inserting one into an existing link), it is simpler to just call
         * ExampleLink::torus().
         *
         * \param p the first parameter of the new torus link; this must be
         * non-negative.
         * \param q the second parameter of the new torus link; this must
         * also be non-negative.
         * \param positive \c true if the crossings in the new torus link
         * should be positive, or \c false if they should be negative.
         */
        void insertTorusLink(int p, int q, bool positive = true);

        /**
         * Creates a new classical or virtual link from hard-coded information
         * about its crossings and components.
         *
         * This routine takes a series of C++ initialiser lists
         * (each a list of integers), which makes it useful for creating
         * hard-coded examples directly in C++ code.
         *
         * For the purposes of this routine, we number the crossings
         * 1, 2, ..., \a n.  The lists that you must pass to this routine
         * are as follows:
         *
         * - The first list contains the signs of crossings 1, ..., \a n
         *   in order, where each sign is either +1 or -1.
         *
         * - Each subsequent list describes a single component of the link.
         *   The list identifies which crossings you visit in order when
         *   traversing the component; a positive entry \a i indicates
         *   that you pass over crossing \a i, and a negative entry -\a i
         *   indicates that you pass under crossing \a i.  Empty lists
         *   are allowed (these denote separate unknot components).
         *
         * - If a component has no crossings, then you should pass the
         *   list { 0 }, not the empty list.  (This is because the
         *   C++ compiler cannot deduce the type of an empty list.)
         *
         * Be aware that, once the link has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * As an example, you can construct the left-hand trefoil and
         * the Hopf link as follows:
         *
         * \code
         * trefoil = Link::fromData({ -1, -1, -1 }, { 1, -2, 3, -1, 2, -3 });
         * hopf = Link::fromData({ +1, +1 }, { 1, -2 }, { -1, 2 });
         * \endcode
         *
         * \note If you have an existing link that you would like to
         * hard-code, the routine source() will output source code that
         * reconstructs the link by calling this routine.
         *
         * \exception InvalidArgument A link could not be reconstructed from
         * the given data.
         *
         * \nopython Instead, use the variant of fromData() that takes this
         * same data using two Python lists (which need not be constant).
         *
         * \param crossingSigns a list containing the signs of the
         * crossings; each sign must be either +1 or -1.
         * \param components one list for each link component that
         * describes the crossings that are visited along that component,
         * as described in the detailed notes above.
         * \return the reconstructed link.
         */
        template <typename... Args>
        static Link fromData(std::initializer_list<int> crossingSigns,
            std::initializer_list<Args>... components);

        /**
         * Creates a new classical or virtual link from information about its
         * crossings and components.
         *
         * This routine is an analogue to the variant of fromData() that
         * takes C++ initialiser lists; however, here the input data may be
         * constructed at runtime (which makes it accessible to Python,
         * amongst other things).
         *
         * For the purposes of this routine, we number the crossings
         * 1, 2, ..., \a n.  The information that you must pass to this
         * routine is the following:
         *
         * - The first iterator range (\a beginSigns, \a endSigns)
         *   encodes the signs of crossings 1, ..., \a n in order.
         *   Each iterator in this range must dereference to either +1 or -1.
         *
         * - The second iterator range (\a beginComponents, \a endComponents)
         *   identifies the individual components of the link.
         *   Each iterator in this range must dereference to a container
         *   that has a size() function and supports range-based \c for loops
         *   (so standard C++ container classes such as std::vector<int> and
         *   std::list<int> are fine).
         *
         * - The container for each component must be filled with integers,
         *   which identify the crossings you visit in order when traversing
         *   the component.  A positive entry \a i indicates that you pass
         *   over crossing \a i, and a negative entry -\a i indicates that you
         *   pass under crossing \a i.
         *
         * - To encode a component with no crossings, you may use either an
         *   empty container or a container containing the single integer 0.
         *
         * Be aware that, once the link has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * As an example, Python users can construct the left-hand trefoil and
         * the Hopf link as follows:
         *
         * \code{.py}
         * trefoil = Link.fromData([ -1, -1, -1 ], [[ 1, -2, 3, -1, 2, -3 ]])
         * hopf = Link.fromData([ +1, +1 ], [[ 1, -2 ], [ -1, 2 ]])
         * \endcode
         *
         * \exception InvalidArgument A link could not be reconstructed from
         * the given data.
         *
         * \python The signs should be passed as a single Python list of
         * integers (not an iterator pair).  Likewise, the components should be
         * passed as a Python list of lists of integers (not an iterator pair).
         * In the case of a knot (which has only one component), you are
         * welcome to replace the list of lists `[[...]]` with a
         * single list `[...]`; however, be aware that a single empty list `[ ]`
         * will be interpreted as an empty link (not a zero-crossing unknot).
         *
         * \param beginSigns the beginning of the list of crossing signs.
         * \param endSigns a past-the-end iterator indicating the end of
         * the list of crossing signs.
         * \param beginComponents the beginning of the list of containers
         * describing each link component.
         * \param endComponents a past-the-end iterator indicating the
         * end of the list of link components.
         * \return the reconstructed link.
         */
        template <typename SignIterator, typename ComponentIterator>
        static Link fromData(SignIterator beginSigns, SignIterator endSigns,
            ComponentIterator beginComponents, ComponentIterator endComponents);

        /**
         * Recovers a classical or virtual link diagram from its knot/link
         * signature.  See sig() for more information on these signatures.
         *
         * Calling sig() followed by fromSig() is not guaranteed to produce
         * an _identical_ link diagram to the original, but it is guaranteed
         * to produce one that is related by zero or more applications of
         * relabelling, and (according to the arguments that were passed
         * to sig()) reflection of the diagram, rotation of the diagram,
         * and/or reversal of individual link components.
         *
         * \exception InvalidArgument The given string was not a valid
         * knot/link signature.
         *
         * \param sig the signature of the link diagram to construct.
         * Note that signatures are case-sensitive.
         * \return the reconstructed link diagram.
         */
        static Link fromSig(const std::string& sig);

        /**
         * Alias for fromSig(), to recover a classical or virtual link diagram
         * from its knot/link signature.
         *
         * This alias fromKnotSig() has been kept to reflect the fact that, in
         * older versions of Regina, these signatures were only available for
         * single-component knots; moreover the old name "knot signatures" can
         * still be found in the literature.  While this routine is not
         * deprecated, it is recommended to use fromSig() in new code.
         *
         * See fromSig() for further details.
         *
         * \exception InvalidArgument The given string was not a valid
         * knot/link signature.
         *
         * \param sig the signature of the link diagram to construct.
         * Note that signatures are case-sensitive.
         * \return the reconstructed link diagram.
         */
        static Link fromKnotSig(const std::string& sig);

        /**
         * Reconstructs a classical or virtual link from its given tight
         * encoding.  See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of a link.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a link.
         * \return the link represented by the given tight encoding.
         */
        static Link tightDecode(std::istream& input);

        /**
         * Creates a new classical knot from a classical Gauss code,
         * presented as a string.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.  As a
         * result, there can be topological ambiguities when a classical knot
         * is reconstructed from a Gauss code; these are described in the
         * warnings below.  For virtual (not classical) knots, the ambiguities
         * inherent in classical Gauss codes are even more severe, and so
         * Regina will not attempt to reconstruct a virtual knot from its
         * classical Gauss code at all.
         *
         * The Gauss code for an <i>n</i>-crossing knot is described by
         * a sequence of 2<i>n</i> positive and negative integers.
         * As an example, you can construct the trefoil using the code:
         *
           \verbatim
           1 -2 3 -1 2 -3
           \endverbatim
         *
         * See gauss() for a full description of classical Gauss codes as
         * they are used in Regina, as well as their limitations.
         *
         * Regina imposes the following restrictions when reconstructing
         * a knot from a classical Gauss code:
         *
         * - This can only be done for _knots_ (i.e., links with exactly one
         *   component), and only for _classical_ knots (not the more general
         *   virtual knot diagrams).
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   in some order.
         *
         * Be aware that, once the knot has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the integers have been combined together and
         * separated by whitespace.  The other variant takes a sequence of
         * integers, defined by a pair of iterators.
         *
         * In this variant (the string variant), the exact form of the
         * whitespace does not matter, and additional whitespace at the
         * beginning or end of the string is allowed.
         *
         * \warning In general, the classical Gauss code does not contain
         * enough information to uniquely reconstruct a classical knot.
         * For prime knots, both a knot and its reflection can be described by
         * the same Gauss code; for composite knots, the same Gauss code can
         * describe knots that are topologically inequivalent, even when
         * allowing for reflection.  If you need to reconstruct a knot uniquely,
         * consider using the _oriented_ Gauss code instead.
         *
         * \exception InvalidArgument The given string was not a valid
         * classical Gauss code for a classical knot.
         *
         * \author Adam Gowty
         *
         * \param str a classical Gauss code for a classical knot, as
         * described above.
         * \return the reconstructed knot.
         */
        static Link fromGauss(const std::string& str);

        /**
         * Creates a new classical knot from a classical Gauss code,
         * presented as an integer sequence.
         *
         * See gauss() for a full description of classical Gauss codes as
         * they are used in Regina, and see fromGauss(const std::string&)
         * for a detailed discussion of how Regina reconstructs classical knots
         * from such codes.
         *
         * This routine is a variant of fromGauss(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of integers.  This sequence is given by passing a
         * pair of begin/end iterators.
         *
         * \pre \a Iterator is a random access iterator type, and
         * dereferencing such an iterator produces a native C++ integer.
         * (The specific native C++ integer type being used will be deduced
         * from the type \a Iterator.)
         *
         * \warning In general, the classical Gauss code does not contain
         * enough information to uniquely reconstruct a classical knot.
         * For prime knots, both a knot and its reflection can be described by
         * the same Gauss code; for composite knots, the same Gauss code can
         * describe knots that are topologically inequivalent, even when
         * allowing for reflection.  If you need to reconstruct a knot uniquely,
         * consider using the _oriented_ Gauss code instead.
         *
         * \exception InvalidArgument The given sequence was not a valid
         * classical Gauss code for a classical knot.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * \author Adam Gowty
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of integers for a classical Gauss code.
         * \param end an iterator that points past the end of the
         * sequence of integers for a classical Gauss code.
         * \return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromGauss(Iterator begin, Iterator end);

        /**
         * Creates a new classical or virtual knot from an "oriented" variant
         * of the Gauss code, presented as string.
         *
         * Oriented gauss codes overcome the limitations of classical Gauss
         * codes by encoding all of the data needed to quickly and correctly
         * reconstruct a knot diagram.
         *
         * The oriented Gauss code for an <i>n</i>-crossing knot is described
         * by a sequence of 2<i>n</i> string tokens.  As an example, you can
         * construct the left-hand trefoil using the code:
         *
           \verbatim
           +>1 -<2 +>3 -<1 +>2 -<3
           \endverbatim
         *
         * See orientedGauss() for a full description of oriented Gauss codes
         * as they are used in Regina (and in particular, what these tokens
         * represent).  Also note that _oriented_ Gauss codes are different
         * from _signed_ Gauss codes: see orientedGauss() versus signedGauss()
         * for details.
         *
         * Regina imposes the following restrictions when reconstructing
         * a knot from an oriented Gauss code:
         *
         * - This can only be done for knots (i.e., links with exactly one
         *   component).  Both classical and virtual knots are supported.
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   in some order.
         *
         * Be aware that, once the knot has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the tokens have been combined together and
         * separated by whitespace.  The other variant takes a sequence of
         * tokens, defined by a pair of iterators.
         *
         * In this variant (the string variant), the exact form of the
         * whitespace does not matter, and additional whitespace at the
         * beginning or end of the string is allowed.
         *
         * \exception InvalidArgument The given string was not a valid
         * oriented Gauss code for a classical or virtual knot.
         *
         * \param str an "oriented" Gauss code for a knot, as described above.
         * \return the reconstructed knot.
         */
        static Link fromOrientedGauss(const std::string& str);

        /**
         * Creates a new classical or virtual knot from an "oriented" variant
         * of the Gauss code, presented as a sequence of string tokens.
         *
         * See orientedGauss() for a full description of oriented Gauss codes
         * as they are used in Regina, and see
         * fromOrientedGauss(const std::string&) for a detailed discussion
         * of how Regina reconstructs knots from such codes.
         *
         * This routine is a variant of fromOrientedGauss(const std::string&)
         * which, instead of taking a human-readable string, takes a
         * machine-readable sequence of string tokens.  This sequence is
         * given by passing a pair of begin/end iterators.
         *
         * The tokens in the input sequence should be the individual tokens of
         * the form `+<k`, `-<k`, `+>k` or `->k` that would normally
         * be joined with whitespace to form a complete oriented Gauss code.
         * For example, to describe the left-hand trefoil, the input sequence
         * could be a vector containing the six tokens:
         *
           \verbatim
           { "+>1", "-<2", "+>3", "-<1", "+>2", "-<3" }
           \endverbatim
         *
         * Each individual token should _not_ contain any whitespace;
         * otherwise this routine may fail to parse the token(s) and
         * could throw an exception as a result.
         *
         * \pre \a Iterator is a random access iterator type.
         *
         * \pre Dereferencing such an iterator produces a C++-style string
         * (i.e., something that can be cast to `const std::string&`).
         *
         * \exception InvalidArgument The given sequence was not a valid
         * oriented Gauss code for a classical or virtual knot.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of tokens for an "oriented" Gauss code.
         * \param end an iterator that points past the end of the
         * sequence of tokens for an "oriented" Gauss code.
         * \return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromOrientedGauss(Iterator begin, Iterator end);

        /**
         * Creates a new classical or virtual knot from a "signed" variant
         * of the Gauss code, presented as string.
         *
         * Signed gauss codes overcome the limitations of classical Gauss
         * codes by encoding all of the data needed to quickly and correctly
         * reconstruct a knot diagram.
         *
         * The signed Gauss code for an <i>n</i>-crossing knot is described
         * by a sequence of 2<i>n</i> string tokens, all concatenated together
         * with no internal whitespace.  As an example, you can construct the
         * figure eight knot using the code:
         *
           \verbatim
           U1+O2+U3-O4-U2+O1+U4-O3-
           \endverbatim
         *
         * See signedGauss() for a full description of signed Gauss codes
         * as they are used in Regina (and in particular, what these tokens
         * represent).  Also note that _signed_ Gauss codes are different from
         * _oriented_ Gauss codes: see signedGauss() versus orientedGauss()
         * for details.
         *
         * Regina imposes the following restrictions when reconstructing
         * a knot from a signed Gauss code:
         *
         * - This can only be done for knots (i.e., links with exactly one
         *   component).  Both classical and virtual knots are supported.
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   in some order.
         *
         * Be aware that, once the knot has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the tokens have been concatenated together
         * with no internal whitespace.  The other variant takes a sequence of
         * 2<i>n</i> individual tokens, defined by a pair of iterators.
         *
         * In this variant (the string variant), the code should not contain
         * any internal whitespace; however, whitespace at the beginning or
         * end of the string is allowed.  The symbols `U` and `O` may be
         * either upper-case or lower-case (or you may use some mix of both).
         *
         * \exception InvalidArgument The given string was not a valid
         * signed Gauss code for a classical or virtual knot.
         *
         * \param str a "signed" Gauss code for a knot, as described above.
         * \return the reconstructed knot.
         */
        static Link fromSignedGauss(const std::string& str);

        /**
         * Creates a new classical or virtual knot from a "signed" variant
         * of the Gauss code, presented as a sequence of string tokens.
         *
         * See signedGauss() for a full description of signed Gauss codes
         * as they are used in Regina, and see
         * fromSignedGauss(const std::string&) for a detailed discussion
         * of how Regina reconstructs knots from such codes.
         *
         * This routine is a variant of fromSignedGauss(const std::string&)
         * which, instead of taking a human-readable string, takes a
         * machine-readable sequence of smaller string tokens (one for each
         * crossing that you pass through when traversing the knot).
         * This sequence is given by passing a pair of begin/end iterators.
         *
         * The tokens in the input sequence should be the individual tokens of
         * the form `Ok+`, `Ok-`, `Uk+` or `Uk-` that would normally be
         * concatenated together to form a complete signed Gauss code.
         * For example, to describe the figure eight knot, the input sequence
         * could be a vector containing the eight tokens:
         *
           \verbatim
           { "U1+", "O2+", "U3-", "O4-", "U2+", "O1+", "U4-", "O3-" }
           \endverbatim
         *
         * None of the tokens should contain any whitespace; otherwise this
         * routine may fail to parse the token(s) and could throw an exception
         * as a result.  The symbols `U` and `O` that begin each token may be
         * either upper-case or lower-case (or you may use some mix of both).
         *
         * \pre \a Iterator is a random access iterator type.
         *
         * \pre Dereferencing such an iterator produces a C++-style string
         * (i.e., something that can be cast to `const std::string&`).
         *
         * \exception InvalidArgument The given sequence was not a valid
         * signed Gauss code for a classical or virtual knot.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of tokens for a "signed" Gauss code.
         * \param end an iterator that points past the end of the
         * sequence of tokens for a "signed" Gauss code.
         * \return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromSignedGauss(Iterator begin, Iterator end);

        /**
         * Creates a new classical or virtual link from Bob Jenkins' format,
         * presented as a string.
         *
         * Jenkins' format overcomes the limitations of classical Gauss
         * codes by encoding all of the data needed to quickly and correctly
         * reconstruct a link diagram.  It can work with links as well as knots.
         *
         * In Jenkins' format, a link is described by a sequence of integers.
         * As an example, you could construct the Hopf link using the sequence:
         *
           \verbatim
           2
           2   0 1   1 -1
           2   0 -1   1 1
           0 1   1 1
           \endverbatim
         *
         * See jenkins() for a full description of Jenkins' format (and
         * in particular, what these integers represent).
         *
         * There are three variants of this routine.  This variant takes a
         * single string, where the integers have been combined together and
         * separated by whitespace.  The other variants take (i) a sequence of
         * integers defined by a pair of iterators, or (ii) an input stream
         * from which the integers will be read.
         *
         * In this variant (the string variant), the exact form of the
         * whitespace does not matter, and additional whitespace at the
         * beginning or end of the string is allowed.
         *
         * \exception InvalidArgument The given string was not a valid
         * encoding of a classical or virtual link in Jenkins' format.
         *
         * \param str a string describing a link in Jenkins' format,
         * as described above.
         * \return the reconstructed link.
         */
        static Link fromJenkins(const std::string& str);

        /**
         * Creates a new classical or virtual link from Bob Jenkins' format,
         * read directly from an input stream.
         *
         * See jenkins() for a full description of Bob Jenkins' format as
         * it is used in Regina, and see fromJenkins(const std::string&)
         * for a detailed discussion of how Regina reconstructs links
         * from this format.
         *
         * This routine is a variant of fromJenkins(const std::string&) which,
         * instead of taking a string as input, takes an input stream from
         * which the sequence of integers describing the link will be read.
         *
         * Once this routine reads the integers that describe the link, or as
         * soon as it encounters an error (e.g., invalid input data), it will
         * stop reading and leave the remainder of the input stream untouched.
         * This means that the stream may contain additional material,
         * which can be read by the user after this routine has finished.
         *
         * \exception InvalidArgument The given input was not a valid
         * encoding of a classical or virtual link in Jenkins' format.
         *
         * \nopython Instead use the variant fromJenkins(const std::string&),
         * which takes the input as a string.
         *
         * \param in an input stream that begins with a sequence of integers
         * separated by whitespace that describes a link.
         * \return the reconstructed link.
         */
        static Link fromJenkins(std::istream& in);

        /**
         * Creates a new classical or virtual link from Bob Jenkins' format,
         * presented as an integer sequence.
         *
         * See jenkins() for a full description of Bob Jenkins' format as
         * it is used in Regina, and see fromJenkins(const std::string&)
         * for a detailed discussion of how Regina reconstructs links
         * from this format.
         *
         * This routine is a variant of fromJenkins(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of integers.  This sequence is given by passing a
         * pair of begin/end iterators.
         *
         * \pre \a Iterator is a forward iterator type, and
         * dereferencing such an iterator produces a native C++ integer.
         * (The specific native C++ integer type being used will be deduced
         * from the type \a Iterator.)
         *
         * \exception InvalidArgument The given sequence was not a valid
         * encoding of a classical or virtual link in Jenkins' format.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of integers that describes a link.
         * \param end an iterator that points past the end of the
         * sequence of integers that describes a link.
         * \return the reconstructed link.
         */
        template <typename Iterator>
        static Link fromJenkins(Iterator begin, Iterator end);

        /**
         * Creates a new classical knot from either alphabetical or numerical
         * Dowker-Thistlethwaite notation, presented as a string.
         *
         * Dowker-Thistlethwaite notation essentially describes the 4-valent
         * graph of a knot but not its particular embedding in the plane.
         * As a result, there can be topological ambiguities when a
         * classical knot is reconstructed from Dowker-Thistlethwaite notation;
         * these are described in the warnings below.  Dowker-Thistlethwaite
         * notation cannot be used with virtual (not classical) knots at all.
         *
         * Dowker-Thistlethwaite notation comes in two forms: numerical
         * and alphabetical.  For an <i>n</i>-crossing knot, the numerical
         * form is a sequence of \a n even signed integers, and the
         * alphabetical form is a sequence of \a n case-sensitive letters.
         * As an example, you can construct the trefoil using either of
         * the following strings:
         *
           \verbatim
           4 6 2
           bca
           \endverbatim
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, as well as its limitations.
         *
         * There are two variants of this routine.  This variant takes a single
         * string, which is either (i) the alphabetical notation, in which any
         * whitespace within the string will be ignored; or (ii) the numerical
         * notation, in which the integers are combined together and
         * separated by whitespace.  The other variant of this routine
         * is only for numerical Dowker-Thistlethwaite notation, and it takes
         * a sequence of integers defined by a pair of iterators.
         *
         * In this variant (the string variant), the string may
         * contain additional leading or trailing whitespace; moreover,
         * for numerical Dowker-Thistlethwaite notation, the exact form
         * of the whitespace that separates the integers does not matter.
         *
         * \warning In general, Dowker-Thistlethwaite notation does not contain
         * enough information to uniquely reconstruct a classical knot.i
         * For prime knots, both a knot and its reflection can be described by
         * the same notation; for composite knots, the same notation can
         * describe knots that are topologically inequivalent, even when
         * allowing for reflection.  If you need to reconstruct a knot uniquely,
         * consider using the oriented Gauss code instead.
         *
         * \exception InvalidArgument The given string was not a valid
         * Dowker-Thistlethwaite code for a classical knot.
         *
         * \param str either the alphabetical or numerical
         * Dowker-Thistlethwaite notation for a classical knot, as described
         * above.
         * \return the reconstructed knot.
         */
        static Link fromDT(const std::string& str);

        /**
         * Creates a new classical knot from numerical Dowker-Thistlethwaite
         * notation, presented as an integer sequence.
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, and see fromDT(const std::string&)
         * for a detailed discussion of how Regina reconstructs classical knots
         * from such notation.
         *
         * This routine is a variant of fromDT(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of integers.  This sequence is given by passing a
         * pair of begin/end iterators.
         *
         * This variant of fromDT() can only work with _numerical_
         * Dowker-Thistlethwaite notation.  Regina does understand alphabetic
         * Dowker-Thistlethwaite notation, but for this you will need to use
         * the string-based variant of fromDT().
         *
         * \pre \a Iterator is a random access iterator type, and
         * dereferencing such an iterator produces a native C++ integer.
         * (The specific native C++ integer type being used will be deduced
         * from the type \a Iterator.)
         *
         * \warning In general, Dowker-Thistlethwaite notation does not contain
         * enough information to uniquely reconstruct a classical knot.
         * For prime knots, both a knot and its reflection can be described by
         * the same notation; for composite knots, the same notation can
         * describe knots that are topologically inequivalent, even when
         * allowing for reflection.  If you need to reconstruct a knot uniquely,
         * consider using the oriented Gauss code instead.
         *
         * \exception InvalidArgument The given sequence was not a valid
         * Dowker-Thistlethwaite code for a classical knot.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of integers for the Dowker-Thistlethwaite notation
         * for a classical knot.
         * \param end an iterator that points past the end of the
         * sequence of integers for the Dowker-Thistlethwaite notation
         * for a classical knot.
         * \return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromDT(Iterator begin, Iterator end);

        /**
         * Creates a new classical or virtual link from a planar diagram code,
         * presented as a string.
         *
         * Planar diagram codes overcome the limitations of classical Gauss
         * codes by encoding the local information at each crossing, though
         * they do introduce their own (less severe) ambiguities and
         * computational difficulties, as described in the warnings below.
         * They can work with links as well as knots, though they cannot
         * encode zero-crossing unknot components.  They can also (despite
         * their name) work with virtual links as well as classical links.
         *
         * A planar diagram code for an <i>n</i>-crossing link is formed
         * from a sequence of \a n 4-tuples of integers.  An example,
         * you can construct the right-handed trefoil using the sequence:
         *
           \verbatim
           [[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
           \endverbatim
         *
         * See pd() for a full description of planar diagram codes (and
         * in particular, what these integers represent).
         *
         * Regina imposes the following restrictions when reconstructing
         * a link from a planar diagram code:
         *
         * - The integers used in the input sequence (which denote the
         *   2<i>n</i> strands in the link diagram) must be in the range
         *   `1,2,...,2n`, with each of these numbers used exactly twice.
         *
         * When Regina builds the resulting link, it numbers the crossings
         * and components (but not the strands).  It will do this as follows:
         *
         * - Each 4-tuple in the given sequence represents a single crossing.
         *   Regina will number the crossings 0, 1, ..., \a n in the same
         *   order as the corresponding 4-tuples appear in the sequence.
         *
         * - The integers in the given sequence represent strands in the
         *   link diagram.  The strand numbered 1 will become the starting
         *   point of component 0 in the final link.  Of the strands not
         *   in that component, the lowest numbered strand remaining
         *   will become the starting point of component 1, and so on.
         *
         * - In particular be aware that StrandRef::id() will in general
         *   have no relation to the strand numbers used in the planar
         *   diagram code.
         *
         * There are two variants of this routine.  This variant takes a
         * single string containing all 4<i>n</i> integers (see below
         * for how this string may be formatted).  The other variant takes a
         * sequence of 4-tuples of integers, defined by a pair of iterators.
         *
         * In this variant (the string variant), the integers may be
         * separated by any combination of the following:
         *
         * - any whitespace;
         * - commas;
         * - open or close round brackets, square brackets and/or braces;
         * - the special symbols \c PD, \c X, \c Xp, \c Xm and \c P, which are
         *   used by other sources (such as the Knot Atlas), but which
         *   are ignored here.
         *
         * Thus the following strings all describe the same sequence:
         *
           \verbatim
           [[1, 5, 2, 4], [3, 1, 4, 6], [5, 3, 6, 2]]
           PD[X[1, 5, 2, 4], X[3, 1, 4, 6], X[5, 3, 6, 2]]
           1 5 2 4 3 1 4 6 5 3 6 2
           \endverbatim
         *
         * The string may containin separators (as defined above) at the
         * beginning and/or the end; these will be ignored.
         *
         * Note that some sources (again, such as the Knot Atlas) use the
         * special symbols \c Xp, \c Xm and \c P to change the meaning of the
         * tuples.  Regina does _not_ attribute any meaning to these symbols,
         * and will treat them as nothing more than separators.
         *
         * \warning If the link contains any components that sit completely
         * above all other link components (in other words, link components
         * that consist entirely of over-crossings), then the orientations of
         * these components might not be reconstructed correctly.
         * This is unavoidable: the planar diagram code simply does not
         * contain this information.
         *
         * \exception InvalidArgument The given string was not a valid
         * planar diagram code for a classical or virtual link.
         *
         * \param str a planar diagram code for a link, as described above.
         * \return the reconstructed link.
         */
        static Link fromPD(const std::string& str);

        /**
         * Creates a new classical or virtual link from a planar diagram code,
         * presented as a sequence of 4-tuples.
         *
         * See pd() for a full description of planar diagram codes as
         * they are used in Regina, and see fromPD(const std::string&)
         * for a detailed discussion of how Regina reconstructs links
         * from such codes.
         *
         * This routine is a variant of fromPD(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of 4-tuples of integers.  This sequence is given by
         * passing a pair of begin/end iterators.
         *
         * \pre \a Iterator is a random access iterator type.
         *
         * \pre If \a it is such an iterator, then `(*it)[0]`,
         * `(*it)[1]`, `(*it)[2]` and `(*it)[3]`
         * will give the elements of the corresponding 4-tuple, which
         * can then be treated as native C++ integers.  (The specific native
         * C++ integer type being used will be deduced from the type
         * \a Iterator.)
         *
         * \warning If the link contains any components that sit completely
         * above all other link components (in other words, link components
         * that consist entirely of over-crossings), then the orientations of
         * these components might not be reconstructed correctly.
         * This is unavoidable: the planar diagram code simply does not
         * contain this information.
         *
         * \exception InvalidArgument The given sequence was not a valid
         * planar diagram code for a classical or virtual link.
         *
         * \python Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list.  Each element
         * of the list should be convertible to a tuple of integers.
         * In particular, a list of Python lists is fine, and a list of
         * Python tuples is fine also.
         *
         * \param begin an iterator that points to the beginning of the
         * sequence of 4-tuples for a planar diagram code.
         * \param end an iterator that points past the end of the
         * sequence of 4-tuples for a planar diagram code.
         * \return the reconstructed link.
         */
        template <typename Iterator>
        static Link fromPD(Iterator begin, Iterator end);

        /*@}*/

    private:
        /**
         * Clears any calculated properties, and declares them all unknown.
         * This must be called by any internal function that changes the link.
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
         * Indicates that strand \a s is followed immediately by strand \a t
         * when traversing a link.  The relevant \a next_ and \a prev_ arrays
         * of the two crossings will be adjusted accordingly.
         *
         * There is no sanity checking to ensure that these two
         * crossings do not already have conflicting connections in place.
         *
         * \pre Neither \a s nor \a t is a null strand reference.
         */
        static void join(const StrandRef& s, const StrandRef& t);

        /**
         * Returns the location in \a components_ of the link component
         * containing \a s, or `components_.end()` if \a s does not belong to
         * any component of this link.
         *
         * The given strand may be a null strand reference (in which case this
         * routine will look for a zero-crossing component).
         */
        std::vector<StrandRef>::iterator componentFor(const StrandRef& s);

        /**
         * Used with fromEnhancedGauss() to indicate which kind of Gauss code
         * we are reconstructing a knot from.
         */
        enum class GaussEnhancement {
            Oriented = 1, Signed = 2
        };
        /**
         * Implements either fromOrientedGauss() or fromSignedGauss(),
         * depending on the value of \a type_.  See fromOrientedGauss() and
         * fromSignedGauss() for details.
         *
         * This routine exists because fromOrientedGauss() and fromSignedGauss()
         * share much of the same source code.
         */
        template <GaussEnhancement type_, typename Iterator>
        static Link fromEnhancedGauss(Iterator begin, Iterator end);
        /**
         * Internal to fromOrientedGauss().
         *
         * This routine parses a single token in an "oriented" Gauss code.
         * The token must not contain any whitespace.
         *
         * \param s the token to parse.
         * \param nCross the total number of crossings in the knot.
         * \param crossing used to return the label of the crossing
         * described by this token.
         * \param strand used to return either 1 or 0 according to
         * whether this token describes the upper or lower strand of the
         * crossing respectively.
         * \param sign used to return the sign of this crossing (either
         * +1 or -1).
         * \return \c true if and only if no errors were detected when
         * parsing the token.
         */
        static bool parseOrientedGaussTerm(const std::string& s,
            size_t nCross, size_t& crossing, int& strand, int& sign);
        /**
         * Internal to fromSignedGauss().
         *
         * This routine parses a single token in a "signed" Gauss code.
         * The token must not contain any whitespace.
         *
         * \param s the token to parse.
         * \param nCross the total number of crossings in the knot.
         * \param crossing used to return the label of the crossing
         * described by this token.
         * \param strand used to return either 1 or 0 according to
         * whether this token describes the upper or lower strand of the
         * crossing respectively.
         * \param sign used to return the sign of this crossing (either
         * +1 or -1).
         * \return \c true if and only if no errors were detected when
         * parsing the token.
         */
        static bool parseSignedGaussTerm(const std::string& s,
            size_t nCross, size_t& crossing, int& strand, int& sign);

        /**
         * Internal to fromData().
         *
         * This routine processes one link component, and then recursively
         * calls itself to process the remaining components.
         *
         * \exception InvalidArgument An error or inconsistency is found in
         * the given data.
         *
         * \param strandsRemaining the expected sum of the lengths of
         * all components that still need to be processed.  This expected
         * sum is computed as twice the number of crossings minus the
         * lengths of all components that have already been processed.
         * In an error-free scenario, \a strandsRemaining should equal
         * the sum of the lengths of the lists passed in \a component and
         * \a otherComponents.
         * \param component the component to process now.
         * \param otherComponents the remaining components still to be
         * processed; these will be processed by a recursive call to this
         * routine.
         */
        template <typename... Args>
        void addComponents(size_t strandsRemaining,
            std::initializer_list<int> component,
            std::initializer_list<Args>... otherComponents);

        /**
         * Internal to fromData().
         *
         * This routine terminates the recursive calls to addComponents().
         * It is called when there are no more components remaining to
         * be processed.
         *
         * \exception InvalidArgument An error or inconsistency was found in
         * the given data (which for this terminating call simply means
         * \a strandsRemaining != 0).
         *
         * \param strandsRemaining the expected sum of the lengths of
         * all components that still need to be processed.  This expected
         * sum is computed as twice the number of crossings minus the
         * lengths of all components that have already been processed.
         */
        void addComponents(size_t strandsRemaining);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r1() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         *
         * \param check indicates whether we should check whether the move can
         * be performed.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalR1(Crossing* crossing, bool check, bool perform);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r1() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         *
         * \param check indicates whether we should check whether the move can
         * be performed.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalR1(StrandRef arc, int side, int sign, bool check,
            bool perform);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r2() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         *
         * \param check indicates whether we should check whether the move can
         * be performed.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalR2(StrandRef arc, bool check, bool perform);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r2() for details on what the location arguments mean.
         *
         * The argument \a classicalOnly controls what kinds of checks are made:
         *
         * - This routine will always check that any null strand reference
         *   that is passed has a corresponding zero-crossing unknot component
         *   to operate upon.  This check is extremely fast (linear in the
         *   number of link components).
         *
         * - If \a classicalOnly is \c true, this routine will also ensure that
         *   the move is planar; that is, the two strands being moved over one
         *   another either both push into the same dual 2-cell, or belong to
         *   different connected components of the link diagram.  This check is
         *   more expensive (linear in the number of crossings).
         *
         * Be aware that, if \a classicalOnly is \c false, this move could
         * potentially convert a classical link diagram into a virtual link
         * diagram.
         *
         * \param classicalOnly indicates whether we should ensure that the
         * requested move is planar.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if and only if the all of the checks pass.
         */
        bool internalR2(StrandRef upperArc, int upperSide,
            StrandRef lowerArc, int lowerSide,
            bool classicalOnly, bool perform);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r3() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         *
         * \param check indicates whether we should check whether the move can
         * be performed.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalR3(StrandRef arc, int side, bool check, bool perform);

        /**
         * Implements testing for and/or performing Reidemeister moves.
         * See r3() for details on what the location arguments mean.
         *
         * \pre The arguments \a check and \a perform are not both \c false.
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         *
         * \param check indicates whether we should check whether the move can
         * be performed.
         * \param perform indicates whether we should actually perform the
         * move, assuming any requested checks are successful.
         * \return \c true if the requested checks pass, or if \a check was
         * \c false (which means no checks were performed at all).
         */
        bool internalR3(Crossing* crossing, int side, bool check, bool perform);

        /**
         * Internal to bracketNaive().
         *
         * Returns the number of loops in the link produced by resolving
         * each crossing according to the given bitmask:
         *
         * - If the <i>i</i>th bit in \a mask is 0, this indicates that
         *   crossing \a i should be resolved by turning _left_ when
         *   entering along the upper strand.
         *
         * - If the <i>i</i>th bit in \a mask is 1, this indicates that
         *   crossing \a i should be resolved by turning _right_ when
         *   entering along the upper strand.
         *
         * If the array \a loopIDs is non-null, then it will be filled
         * with an identifier for each loop.  Each identifier will be
         * the minimum of the following values that are computed as you
         * follow the loop: when passing through crossing \a i, if we
         * encounter the half of the upper strand that _exits_ the crossing
         * then we take the value \a i, and if we encounter the half of
         * the upper strand that _enters_ the crossing then we take the
         * value (\a i + \a n).  These identifiers will be returned in the
         * array \a loopIDs in sorted order.
         *
         * If the array \a loopLengths is non-null, then it will be
         * filled with the number of strands in each loop (so these
         * should sum to twice the number of crossings).  These loop
         * lengths will be placed in the array in the same order as the
         * loop IDs as described above.
         *
         * \pre The number of crossings is less than 64 (the length of
         * the bitmask type).
         *
         * \pre If either or both the arrays \a loopIDs and \a loopLengths
         * are not null, then they are arrays whose size is at least the
         * return value (i.e., the number of loops).  This typically means
         * that the caller must put an upper bound on the number of loops
         * in advance, before calling this routine.
         *
         * \return the resulting number of loops after all crossings are
         * resolved.
         */
        size_t resolutionLoops(uint64_t mask, size_t* loopIDs = nullptr,
            size_t* loopLengths = nullptr) const;

        /**
         * Compute the Kauffman bracket polynomial using a naive
         * algorithm that sums over all resolutions of all crossings.
         *
         * The given progress tracker may be \c null.
         * This routine does _not_ mark the tracker as finished.
         *
         * See bracket() for further details.
         */
        Laurent<Integer> bracketNaive(ProgressTracker* tracker) const;

        /**
         * Compute the Kauffman bracket polynomial using a fixed-parameter
         * tractable algorithm based on a tree decomposition.
         *
         * The given progress tracker may be \c null.
         * This routine does _not_ mark the tracker as finished.
         *
         * See bracket() for further details.
         *
         * \pre The maximum possible strand ID can fit into an \c int.
         * In other words, if an \c int contains \a b bits, then the
         * number of crossings is less than 2^(<i>b</i>-2).
         */
        Laurent<Integer> bracketTreewidth(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY-PT polynomial of this link, as a polynomial
         * in \a alpha and \a z, using Kauffman's skein-template algorithm.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         */
        Laurent2<Integer> homflyKauffman(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY-PT polynomial of this link, as a polynomial
         * in \a alpha and \a z, using a fixed-parameter tractable algorithm
         * based on a tree decomposition.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         *
         * \pre The maximum possible strand ID can fit into an \c int.
         * In other words, if an \c int contains \a b bits, then the
         * number of crossings is less than 2^(<i>b</i>-2).
         */
        Laurent2<Integer> homflyTreewidth(ProgressTracker* tracker) const;

        /**
         * Optimises the given tree decomposition for computing the
         * Jones polynomial.
         *
         * This optimisation may involve compressing and/or rerooting the
         * given tree decomposition.  The aim is to minimise the estimated
         * processing time and memory consumption of calling
         * `jones(Algorithm::Treewidth)` and/or `bracket(Algorithm::Treewidth)`.
         *
         * The rerooting procedure essentially estimates the number of
         * partial solutions that are expected at each bag in the
         * treewidth-based dynamic programming algorithm, and chooses a
         * root that minimises the maximum such estimate over all bags.
         *
         * \param td the tree decomposition to optimise.
         */
        void optimiseForJones(TreeDecomposition& td) const;

        /**
         * Returns the group of this link as constructed from the Wirtinger
         * presentation, possibly switching the roles of the upper and lower
         * strands at every crossing.
         *
         * \param flip \c false if we should build the "native" Wirtinger
         * presentation, as constructed by the public function group(), or
         * \c true if we should build the "reflection" Wirtinger presentation,
         * which would be obtained by group() if we had called changeAll()
         * beforehand.
         * \param simplify \c true if we should attempt to simplify the group
         * presentation before returning.
         * \return the requested link group.
         */
        GroupPresentation internalGroup(bool flip, bool simplify) const;

        /**
         * Takes an arbitrary tree decomposition for this link, and
         * modifies and optimises it so that it is ready for use as a
         * nice tree decomposition for the internal treewidth-based
         * algorithms in the Link class.
         */
        void prepareTreeDecomposition(TreeDecomposition& td) const;

        /**
         * Sets the cached Kauffman bracket polynomial and Jones polynomial.
         *
         * The Kauffman bracket polynomial will be set to the argument
         * \a bracket (using a move, not a deep copy), and the
         * Jones polynomial will be derived from it.
         *
         * \param bracket the Kauffman bracket polynomial of this link.
         */
        void setPropertiesFromBracket(Laurent<Integer>&& bracket) const;

        /**
         * An object that facilitates both firing change events and calling
         * clearAllProperties().
         *
         * An object of type Link::ChangeAndClearSpan has three possible
         * effects upon the link that is passed to its constructor:
         *
         * - If the link is actually part of a PacketOf<Link>, then the packet
         *   events PacketListener::packetToBeChanged() and
         *   PacketListener::packetWasChanged() will be fired upon this
         *   object's construction and destruction respectively.
         *
         * - On destruction, this object also calls Link::clearAllProperties(),
         *   _unless_ the template argument \a changeType is
         *   ChangeType::Cosmetic.  This call will happen just before the
         *   final change event is fired.
         *
         * - Finally, if the template argument \a changeType is
         *   ChangeType::PreserveTopology, then this object will effectively
         *   create a new TopologyLock for the link that lasts for the
         *   full lifespan of this object, _excluding_ the firing of packet
         *   change events.  Specifically, the TopologyLock will be created in
         *   the constructor after the initial change event is fired, and will
         *   be removed in the destructor immediately after the call to
         *   Link::clearAllProperties().  In particular, this means that
         *   topological properties of the link that have been computed and
         *   cached (such as Jones and HOMFLY-PT polynomials) will be preserved
         *   when clearAllProperties() is called in the destructor.
         *
         * The use of ChangeAndClearSpan is similar to Packet::PacketChangeSpan
         * (and indeed, this class is intended to _replace_ PacketChangeSpan
         * when writing Link member functions): objects of this type would
         * typically be created on the stack, just before the internal data
         * within a link is changed, and have a lifespan that covers all of
         * your changes to the link.
         *
         * Like PacketChangeSpan, these objects can be safely nested with other
         * ChangeAndClearSpan and/or PacketChangeSpan objects, and only the
         * outermost object will fire packet change events.  However, unlike
         * PacketChangeSpan, this comes with a cost: as always, only one
         * set of change events will be fired; however, if there are multiple
         * ChangeAndClearSpan objects then Link::clearAllProperties() will be
         * called multiple times.  This is harmless but inefficient.
         *
         * Likewise, if \a changeType is ChangeType::PreserveTopology then these
         * objects will behave in the expected way when nested with other
         * TopologyLock objects (i.e., topological properties will be preserved
         * as long as any such object is alive).
         *
         * Note: we would normally use a deduction guide so that, for the
         * default case, you can just write `ChangeAndClearSpan` instead of
         * `ChangeAndClearSpan<>`.  Unfortunately this is not possible due to
         * a gcc bug (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79501),
         * which affects gcc versions 11 and earlier (but not clang).
         *
         * ChangeAndClearSpan objects are not copyable, movable or swappable.
         * In particular, Regina does not offer any way for a ChangeAndClearSpan
         * to transfer its outstanding duties (i.e., firing events and calling
         * clearAllProperties() upon destruction) to another object.
         *
         * \tparam changeType controls which computed properties of the link
         * will be cleared upon the destruction of this object.  See the notes
         * above for details.  Currently ChangeType::PreserveTopology is not
         * yet supported for links (this is planned for a future release of
         * Regina).  If unsure, the default value of ChangeType::General
         * (which clears _all_ computed properties) is always safe to use.
         *
         * \nopython
         */
        template <ChangeType changeType = ChangeType::General>
        class ChangeAndClearSpan : public PacketData<Link>::PacketChangeSpan {
            public:
                /**
                 * Performs all initial tasks before the link is modified.
                 * See the class notes for precisely what tasks are performed.
                 *
                 * \param link the link whose data is about to change.
                 */
                ChangeAndClearSpan(Link& link) :
                        PacketData<Link>::PacketChangeSpan(link) {
                    if constexpr (changeType == ChangeType::PreserveTopology)
                        ++link.topologyLock_;
                }

                /**
                 * Performs all follow-up tasks after the link has
                 * been modified.  See the class notes for precisely what
                 * tasks are performed.
                 */
                ~ChangeAndClearSpan() {
                    if constexpr (changeType != ChangeType::Cosmetic)
                        static_cast<Link&>(data_).clearAllProperties();

                    if constexpr (changeType == ChangeType::PreserveTopology)
                        --static_cast<Link&>(data_).topologyLock_;
                }

                // Make this class non-copyable.
                ChangeAndClearSpan(const ChangeAndClearSpan&) = delete;
                ChangeAndClearSpan& operator = (const ChangeAndClearSpan&) =
                    delete;
        };

    friend class ModelLinkGraph;
    friend class Tangle;
    friend class XMLLinkCrossingsReader;
    friend class XMLLinkComponentsReader;
    friend class XMLWriter<Link>;
};

/**
 * Swaps the contents of the two given links.
 *
 * This global routine simply calls Link::swap(); it is provided so
 * that Link meets the C++ Swappable requirements.
 *
 * See Link::swap() for more details.
 *
 * \note This swap function is _not_ marked \c noexcept, since it
 * fires change events on both links which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * \param lhs the link whose contents should be swapped with \a rhs.
 * \param rhs the link whose contents should be swapped with \a lhs.
 *
 * \ingroup link
 */
void swap(Link& lhs, Link& rhs);

} // namespace regina

// Headers that cannot be included until after the Link class is defined:
#include "link/modellinkgraph.h"

namespace regina {

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline void Link::clearAllProperties() {
    if (! topologyLocked()) {
        alexander_.reset();
        jones_.reset();
        homflyAZ_.reset();
        homflyLM_.reset();
    }

    virtualGenus_ = -1;
    bracket_.reset();
    niceTreeDecomposition_.reset();
}

inline Link::~Link() {
    clearAllProperties();
    for (Crossing* c : crossings_)
        delete c;
}

inline size_t Crossing::index() const {
    return markedIndex();
}

inline StrandRef Crossing::next(int strand) const {
    return next_[strand];
}

inline StrandRef Crossing::prev(int strand) const {
    return prev_[strand];
}

// Inline functions for StrandRef

inline StrandRef::StrandRef() : crossing_(nullptr), strand_(0) {
}

inline StrandRef::StrandRef(Crossing* crossing, int strand) :
        crossing_(crossing), strand_(strand) {
}

inline Crossing* StrandRef::crossing() const {
    return crossing_;
}

inline int StrandRef::strand() const {
    return strand_;
}

inline ssize_t StrandRef::id() const {
    if (crossing_)
        return (crossing()->index() << 1) | static_cast<size_t>(strand_);
    else
        return -1;
}

inline StrandRef& StrandRef::operator ++ () {
    return (*this = crossing_->next(strand_));
}

inline StrandRef StrandRef::operator ++ (int) {
    StrandRef old = *this;
    *this = crossing_->next(strand_);
    return old;
}

inline StrandRef& StrandRef::operator -- () {
    return (*this = crossing_->prev(strand_));
}

inline StrandRef StrandRef::operator -- (int) {
    StrandRef old = *this;
    *this = crossing_->prev(strand_);
    return old;
}

inline StrandRef StrandRef::next() const {
    return crossing_->next(strand_);
}

inline StrandRef StrandRef::prev() const {
    return crossing_->prev(strand_);
}

inline void StrandRef::jump() {
    strand_ ^= 1;
}

inline StrandRef::operator bool() const {
    return (crossing_ != nullptr);
}

inline std::ostream& operator << (std::ostream& out, const StrandRef& s) {
    if (s.crossing())
        return out << (s.strand() == 1 ? '^' : '_') << s.crossing()->index();
    else
        return out << "(null)";
}

// Inline functions for Crossing

inline int Crossing::sign() const {
    return sign_;
}

inline StrandRef Crossing::upper() {
    return StrandRef(this, 1);
}

inline StrandRef Crossing::lower() {
    return StrandRef(this, 0);
}

inline StrandRef Crossing::over() {
    return StrandRef(this, 1);
}

inline StrandRef Crossing::under() {
    return StrandRef(this, 0);
}

inline StrandRef Crossing::strand(int which) {
    return StrandRef(this, which);
}

inline void Crossing::writeTextShort(std::ostream& out) const {
    out << "Crossing " << index() << " (" << (sign_ == 1 ? '+' : '-')
        << "): over " << prev_[1] << " -+-> " << next_[1]
        << ", under " << prev_[0] << " -+-> " << next_[0];
}

inline Crossing::Crossing() : sign_(0) {
}

inline Crossing::Crossing(int sign) : sign_(sign) {
}

// Inline functions for Link

inline Link::Link() : virtualGenus_(-1) {
}

inline Link::Link(size_t unknots) : virtualGenus_(-1) {
    components_.resize(unknots);
    std::fill(components_.begin(), components_.end(), StrandRef());
}

inline Link::Link(const Link& cloneMe) : Link(cloneMe, true) {
}

inline size_t Link::size() const {
    return crossings_.size();
}

inline bool Link::isEmpty() const {
    return components_.empty();
}

inline size_t Link::countComponents() const {
    return components_.size();
}

inline Crossing* Link::crossing(size_t index) const {
    return crossings_[index];
}

inline auto Link::crossings() const {
    return ListView(crossings_);
}

inline StrandRef Link::component(size_t index) const {
    return components_[index];
}

inline auto Link::components() const {
    return ListView(components_);
}

inline StrandRef Link::strand(ssize_t id) const {
    return (id >= 0 ? StrandRef(crossings_[id >> 1]->strand(id & 1)) :
        StrandRef());
}

inline auto Link::componentsByStrand() const {
    // We implement this here in the header because the return type is auto.
    FixedArray<size_t> ans(2 * crossings_.size());
    for (size_t c = 0; c < components_.size(); ++c) {
        StrandRef start = components_[c];
        if (! start)
            continue;
        StrandRef s = start;
        do {
            ans[s.id()] = c;
            ++s;
        } while (s != start);
    }
    return ans;
}

inline ModelLinkGraph Link::graph() const {
    return ModelLinkGraph(*this);
}

inline Triangulation<3> Link::complement(bool simplify) const {
    return Triangulation<3>(*this, simplify);
}

inline long Link::writhe() const {
    long ans = 0;
    for (const Crossing* c : crossings_)
        ans += c->sign();
    return ans;
}

inline long Link::writheOfComponent(size_t index) const {
    return writheOfComponent(components_[index]);
}

inline size_t Link::virtualGenus() const {
    if (virtualGenus_ < 0)
        virtualGenus_ = graph().genus();
    return virtualGenus_;
}

inline bool Link::isClassical() const {
    return virtualGenus() == 0;
}

inline long Link::linking() const {
    long twice = linking2();
    if (twice & 1)
        throw NotImplemented("This is a virtual link whose linking number "
            "is not an integer: use linking2() instead");
    else
        return twice >> 1;
}

inline const Laurent2<Integer>& Link::homfly(Algorithm alg,
        ProgressTracker* tracker) const {
    return homflyAZ(alg, tracker);
}

inline bool Link::knowsBracket() const {
    return bracket_.has_value();
}
inline bool Link::knowsAlexander() const {
    return alexander_.has_value();
}
inline bool Link::knowsJones() const {
    return jones_.has_value();
}
inline bool Link::knowsHomfly() const {
    // Either both homflyAZ_ and homflyLM_ are known, or neither are known.
    return homflyAZ_.has_value();
}

inline bool Link::r1(Crossing* crossing) {
    return internalR1(crossing, true, true);
}

inline bool Link::r1(StrandRef arc, int side, int sign) {
    return internalR1(arc, side, sign, true, true);
}

inline bool Link::r2(StrandRef arc) {
    return internalR2(arc, true, true);
}

inline bool Link::r2(Crossing* crossing) {
    return internalR2(StrandRef(crossing, 1), true, true);
}

inline bool Link::r2(StrandRef upperArc, int upperSide,
        StrandRef lowerArc, int lowerSide) {
    return internalR2(upperArc, upperSide, lowerArc, lowerSide,
        true /* classical only */, true);
}

inline bool Link::r2Virtual(StrandRef upperArc, int upperSide,
        StrandRef lowerArc, int lowerSide) {
    return internalR2(upperArc, upperSide, lowerArc, lowerSide,
        false /* allow virtual */, true);
}

inline bool Link::r3(StrandRef arc, int side) {
    return internalR3(arc, side, true, true);
}

inline bool Link::r3(Crossing* crossing, int side) {
    return internalR3(crossing, side, true, true);
}

inline bool Link::hasR1(Crossing* crossing) const {
    return const_cast<Link*>(this)->internalR1(crossing, true, false);
}

inline bool Link::hasR1(StrandRef arc, int side, int sign) const {
    return const_cast<Link*>(this)->internalR1(arc, side, sign, true, false);
}

inline bool Link::hasR2(StrandRef arc) const {
    return const_cast<Link*>(this)->internalR2(arc, true, false);
}

inline bool Link::hasR2(Crossing* crossing) const {
    return const_cast<Link*>(this)->internalR2(StrandRef(crossing, 1),
        true, false);
}

inline bool Link::hasR2(StrandRef upperArc, int upperSide,
        StrandRef lowerArc, int lowerSide) const {
    return const_cast<Link*>(this)->internalR2(upperArc, upperSide,
        lowerArc, lowerSide, true, false);
}

inline bool Link::hasR3(StrandRef arc, int side) const {
    return const_cast<Link*>(this)->internalR3(arc, side, true, false);
}

inline bool Link::hasR3(Crossing* crossing, int side) const {
    return const_cast<Link*>(this)->internalR3(crossing, side, true, false);
}

inline std::optional<Link> Link::withR1(Crossing* crossing) const {
    if (! hasR1(crossing))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR1(ans->translate(crossing), false, true);
    return ans;
}

inline std::optional<Link> Link::withR1(StrandRef arc, int side, int sign)
        const {
    if (! hasR1(arc, side, sign))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR1(ans->translate(arc), side, sign, false, true);
    return ans;
}

inline std::optional<Link> Link::withR2(StrandRef arc) const {
    if (! hasR2(arc))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR2(ans->translate(arc), false, true);
    return ans;
}

inline std::optional<Link> Link::withR2(Crossing* crossing) const {
    if (! hasR2(crossing))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR2(StrandRef(ans->translate(crossing), 1), false, true);
    return ans;
}

inline std::optional<Link> Link::withR2(StrandRef upperArc, int upperSide,
        StrandRef lowerArc, int lowerSide) const {
    if (! hasR2(upperArc, upperSide, lowerArc, lowerSide))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    // We already know that the move will be planar.
    // There is no need to run the expensive planarity check again.
    ans->internalR2(ans->translate(upperArc), upperSide,
        ans->translate(lowerArc), lowerSide,
            false /* allow non-planar moves */, true);
    return ans;
}

inline std::optional<Link> Link::withR3(StrandRef arc, int side) const {
    if (! hasR3(arc, side))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR3(ans->translate(arc), side, false, true);
    return ans;
}

inline std::optional<Link> Link::withR3(Crossing* crossing, int side) const {
    if (! hasR3(crossing, side))
        return {};

    std::optional<Link> ans(std::in_place, *this);
    ans->internalR3(ans->translate(crossing), side, false, true);
    return ans;
}

inline bool Link::r1(Crossing* crossing, bool, bool perform) {
    return internalR1(crossing, true, perform);
}

inline bool Link::r1(StrandRef arc, int side, int sign, bool, bool perform) {
    return internalR1(arc, side, sign, true, perform);
}

inline bool Link::r2(StrandRef arc, bool, bool perform) {
    return internalR2(arc, true, perform);
}

inline bool Link::r2(Crossing* crossing, bool, bool perform) {
    return internalR2(StrandRef(crossing, 1), true, perform);
}

inline bool Link::r2(StrandRef upperArc, int upperSide,
        StrandRef lowerArc, int lowerSide, bool, bool perform) {
    return internalR2(upperArc, upperSide, lowerArc, lowerSide,
        true /* classical only */, perform);
}

inline bool Link::r3(StrandRef arc, int side, bool, bool perform) {
    return internalR3(arc, side, true, perform);
}

inline bool Link::r3(Crossing* crossing, int side, bool, bool perform) {
    return internalR3(crossing, side, true, perform);
}

inline bool Link::internalR3(Crossing* crossing, int side, bool check,
        bool perform) {
    StrandRef s(crossing, 1);

    // If we are testing, then make sure this is an uppermost arc.
    if (check) {
        // We already know the start of the arc is an over-crossing, by
        // construction; we only need to check the end.
        if ((! crossing) || (s.next().strand() != 1))
            return false;
    }

    return internalR3(s, side, check, perform);
}

inline GroupPresentation Link::group(bool simplify) const {
    return internalGroup(false, simplify);
}

inline std::pair<GroupPresentation, GroupPresentation> Link::groups(
        bool simplify) const {
    return { internalGroup(false, simplify), internalGroup(true, simplify) };
}

inline const TreeDecomposition& Link::niceTreeDecomposition() const {
    if (niceTreeDecomposition_)
        return *niceTreeDecomposition_;

    TreeDecomposition ans(*this, TreeDecompositionAlg::Upper);
    prepareTreeDecomposition(ans);
    niceTreeDecomposition_ = ans;

    return *niceTreeDecomposition_;
}

inline void Link::useTreeDecomposition(TreeDecomposition td) {
    prepareTreeDecomposition(td);
    niceTreeDecomposition_ = std::move(td);
}

inline Crossing* Link::translate(Crossing* other) const {
    return (other ? crossings_[other->index()] : nullptr);
}

inline StrandRef Link::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(nullptr, other.strand()));
}

inline bool Link::intelligentSimplify() {
    return simplify();
}

template <typename Action, typename... Args>
inline bool Link::rewrite(int height, unsigned threads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    if (components_.size() >= 64) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "rewrite() requires fewer than 64 link components");
    }

    // Use RetriangulateActionTraits to deduce whether the given action takes
    // a link or both a signature and link as its initial argument(s).
    using Traits = regina::detail::RetriangulateActionTraits<Link, Action>;
    static_assert(Traits::valid,
        "The action that is passed to rewrite() does not take the correct initial argument type(s).");
    if constexpr (Traits::withSig) {
        return regina::detail::retriangulateInternal<Link, true>(
            *this, height, threads, tracker,
            [&](const std::string& sig, Link&& obj) {
                return action(sig, std::move(obj), std::forward<Args>(args)...);
            });
    } else {
        return regina::detail::retriangulateInternal<Link, false>(
            *this, height, threads, tracker,
            [&](Link&& obj) {
                return action(std::move(obj), std::forward<Args>(args)...);
            });
    }
}

inline bool Link::simplifyExhaustive(int height, unsigned threads,
        ProgressTrackerOpen* tracker) {
    if (components_.size() >= 64) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "simplifyExhaustive() requires fewer than 64 link components");
    }

    return regina::detail::simplifyExhaustiveInternal<Link>(
        *this, height, threads, tracker);
}

inline void Link::join(const StrandRef& s, const StrandRef& t) {
    s.crossing_->next_[s.strand_] = t;
    t.crossing_->prev_[t.strand_] = s;
}

inline std::string Link::knotSig(bool allowReflection, bool allowReversal,
        bool allowRotation) const {
    return sig(allowReflection, allowReversal, allowRotation);
}

inline Link Link::fromKnotSig(const std::string& sig) {
    return Link::fromSig(sig);
}

template <typename Iterator>
inline Link Link::fromOrientedGauss(Iterator begin, Iterator end) {
    return fromEnhancedGauss<GaussEnhancement::Oriented, Iterator>(begin, end);
}

template <typename Iterator>
inline Link Link::fromSignedGauss(Iterator begin, Iterator end) {
    return fromEnhancedGauss<GaussEnhancement::Signed, Iterator>(begin, end);
}

inline std::string Link::dumpConstruction() const {
    return source(Language::Cxx);
}

inline void swap(Link& lhs, Link& rhs) {
    lhs.swap(rhs);
}

} // namespace regina

#include "link/data-impl.h"
#include "link/dt-impl.h"
#include "link/gauss-impl.h"
#include "link/jenkins-impl.h"
#include "link/pd-impl.h"

#endif

