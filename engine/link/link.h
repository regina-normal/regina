
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

/*! \file link/link.h
 *  \brief Deals with knots and links in the 3-sphere.
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
#include "utilities/listview.h"
#include "utilities/markedvector.h"

namespace regina {

/**
 * \defgroup link Knots and Links
 * Knots and links in the 3-sphere
 */

class Crossing;
class Link;
class ProgressTracker;
class Tangle;
template <typename T> class Laurent;
template <typename T> class Laurent2;
template <int> class Triangulation;

/**
 * Indicates one of the standard framings of a knot or link.
 * Here a \e framing refers to a choice of normal vector field along the
 * knot or link.  Equivalently, a framing refers to a choice of longitude
 * on the torus bounding each component of the link.
 *
 * \ingroup link
 */
enum Framing {
    /**
     * Indicates the <i>Seifert framing</i>, which is defined
     * algebraically and is independent of the knot/link projection.
     *
     * For each component of the link, draw a Seifert surface (i.e., an
     * orientable surface embedded in the 3-sphere that is bounded by the
     * corresponding knot).  The Seifert framing is the vector field
     * that points into the corresponding surface.
     *
     * Equivalently, for each component of the link, the Seifert framing
     * chooses the unique longitude for the corresponding knot that is
     * trivial in the homology of the knot complement.
     */
    FRAMING_SEIFERT = 1,
    /**
     * Indicates the <i>blackboard framing</i>, which is specific to the
     * knot/link projection.
     *
     * For the blackboard framing, the normal vector field stays within
     * the projection plane.  Equivalently, the blackboard framing chooses
     * longitudes whose projections do not intersect the original link diagram.
     */
    FRAMING_BLACKBOARD = 2
};

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
 * This class can also be used to refer to an \e arc of a link; that is,
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
         * @param crossing the crossing being identified.
         * @param strand 0 to denote the strand running under the crossing, or
         * 1 to denote the strand running over the crossing.
         */
        StrandRef(Crossing* crossing, int strand);

        /**
         * Default copy constructor.
         */
        StrandRef(const StrandRef&) = default;

        /**
         * The crossing that this reference points to.
         *
         * The information returned by crossing() and strand() together
         * pinpoint exactly which strand of the link this reference points to.
         *
         * @return the crossing, or \c null if this is a null reference.
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
         * @return either 0 or 1 to indicate the strand.
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
         * @return the unique ID of this strand within the link.
         */
        int id() const;

        /**
         * Tests whether this and the given reference are identical.
         *
         * Two references are identical if and only if they return the
         * same values for both crossing() and strand().
         *
         * \warning If you create a null reference by calling
         * StrandRef(\c null, 1) then this will \e not be considered
         * equal to the null reference created by calling StrandRef(),
         * since the latter is equivalent to calling StrandRef(\c null, 0).
         *
         * \c true if and only if this and \a rhs are identical.
         */
        bool operator == (const StrandRef& rhs) const;
        /**
         * Tests whether this and the given reference are not identical.
         *
         * Two references are identical if and only if they return the
         * same values for both crossing() and strand().
         *
         * \warning If you create a null reference by calling
         * StrandRef(\c null, 1) then this will \e not be considered
         * equal to the null reference created by calling StrandRef(),
         * since the latter is equivalent to calling StrandRef(\c null, 0).
         *
         * \c true if and only if this and \a rhs are not identical.
         */
        bool operator != (const StrandRef& rhs) const;
        /**
         * Default assignment operator.
         *
         * @return a reference to this object.
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
         * \ifacespython This routine is not available; however, the
         * postincrement operator is available under the name inc().
         *
         * @return a reference to this object.
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
         * \ifacespython This routine is available under the name inc().
         *
         * @return a copy of this object before the change took place.
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
         * \ifacespython This routine is not available; however, the
         * postincrement operator is available under the name dec().
         *
         * @return a reference to this object.
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
         * \ifacespython This routine is available under the name dec().
         *
         * @return a copy of this object before the change took place.
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
         * @return the crossing reference that follows this.
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
         * @return the crossing reference that precedes this.
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
         * \ifacespython This is not available to python users.
         * Instead you can simply test whether <tt>crossing() == None</tt>.
         *
         * @return \c true if this is not a null reference (i.e., crossing()
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
 * The reference will be written in the form <tt>^n</tt> or <tt>_n</tt>,
 * denoting the upper or lower strand at crossing \a n respectively.
 * For example, the upper strand of crossing 7 will be written as <tt>^7</tt>.
 *
 * @param out the output stream to which to write.
 * @param s the reference to write.
 * @return a reference to the given output stream.
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
         * @return the index of this crossing.
         */
        int index() const;
        /**
         * Returns the sign of this crossing.  This will be +1 for a
         * positive crossing, or -1 for a negative crossing.
         *
         * See the Crossing class notes for diagrams of positive and
         * negative crossings
         *
         * @return the sign of this crossing.
         */
        int sign() const;
        /**
         * Returns a reference to the strand running over this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 1).
         *
         * Note that upper() and over() are synonyms.
         *
         * @return a reference to the upper strand for this crossing.
         */
        StrandRef upper();
        /**
         * Returns a reference to the strand running under this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 0).
         *
         * Note that lower() and under() are synonyms.
         *
         * @return a reference to the lower strand for this crossing.
         */
        StrandRef lower();
        /**
         * Returns a reference to the strand running over this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 1).
         *
         * Note that upper() and over() are synonyms.
         *
         * @return a reference to the upper strand for this crossing.
         */
        StrandRef over();
        /**
         * Returns a reference to the strand running under this crossing.
         * This is equivalent to directly constructing StrandRef(\c this, 0).
         *
         * Note that lower() and under() are synonyms.
         *
         * @return a reference to the lower strand for this crossing.
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
         * @param which either 1 to indicate the upper strand,
         * or 0 to indicate the lower strand.
         * @return a reference to the given strand at this crossing.
         */
        StrandRef strand(int which);
        /**
         * Returns the crossing reference that immediately follows this when
         * walking forward in the direction of the link along one of the two
         * strands that pass at this crossing.  Which strand we follow
         * is indicated by the argument \a strand.
         *
         * Note that for a crossing \a c, calling <tt>c.next(s)</tt>
         * is equivalent to calling <tt>c.strand(s).next()</tt>.
         *
         * @param strand either 1 to walk forward along the upper strand,
         * or 0 to walk forward along the lower strand.
         * @return a reference to the next crossing after this along the
         * given strand.
         */
        StrandRef next(int strand) const;
        /**
         * Returns the crossing reference that immediately precedes this when
         * walking backward against the direction of the link along one of the
         * two strands that pass at this crossing.  Which strand we
         * follow is indicated by the argument \a strand.
         *
         * Note that for a crossing \a c, calling <tt>c.prev(s)</tt>
         * is equivalent to calling <tt>c.strand(s).prev()</tt>.
         *
         * @param strand either 1 to walk backward along the upper strand,
         * or 0 to walk backward along the lower strand.
         * @return a reference to the previous crossing before this along the
         * given strand.
         */
        StrandRef prev(int strand) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
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
         * @param sign the sign of this crossing; this must be +1 or -1.
         */
        Crossing(int sign);

    friend class Link;
    friend class ModelLinkGraph;
    friend class Tangle;
    friend class XMLLinkCrossingsReader;
    friend class XMLLinkConnectionsReader;
};

/**
 * Represents a directed knot or link in the 3-sphere.
 *
 * This class supports links with any number of components (including zero),
 * and it also supports components with no crossings (which form additional
 * unknot components of the overall link).
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
 *   PacketOf<Link>.  This \e is a packet type, and supports labels, tags,
 *   child/parent packets, and event listeners.  It derives from Link,
 *   and so inherits the full Link interface.
 *
 * - If you are adding new functions to this class that edit the link,
 *   you must still remember to create a ChangeEventSpan.  This will
 *   ensure that, if the link is being managed by a PacketOf<Link>,
 *   then the appropriate packet change events will be fired.
 *   All other events (aside from packetToBeChanged() and packetWasChanged()
 *   are managed directly by the PacketOf<Link> wrapper class.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class Link : public PacketData<Link>, public Output<Link> {
    private:
        MarkedVector<Crossing> crossings_;
            /**< The crossings in this link. */
        std::vector<StrandRef> components_;
            /**< For each component of the link, this array contains
                 a reference to one of its strands (which can then be
                 used to traverse the component).  If a component has no
                 crossings, then it is represented in this array by a
                 null reference. */

        mutable std::optional<Laurent<Integer>> jones_;
            /**< The Jones polynomial of the link.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent2<Integer>> homflyLM_;
            /**< The HOMFLY polynomial of the link, as a polynomial in
                 \a l and \a m.  This property will be known if and only
                 if \a homflyAZ_ is known.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent2<Integer>> homflyAZ_;
            /**< The HOMFLY polynomial of the link, as a polynomial in
                 \a alpha and \a z.  This property will be known if and
                 only if \a homflyLM_ is known.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::optional<Laurent<Integer>> bracket_;
            /**< The Kauffman bracket polynomial of the link diagram.
                 This is std::nullopt if it has not yet been computed. */

        mutable std::optional<TreeDecomposition> niceTreeDecomposition_;
            /**< A nice tree decomposition of the planar 4-valent multigraph
                 formed by the link diagram.
                 This is std::nullopt if it has not yet been computed. */

    public:
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
         * you can call <tt>Laurent::str(Link::jonesVar)</tt>.
         */
        static constexpr const char* jonesVar = "\u221At"; // \u221A = root

        /**
         * The name of the first variable used in the variant of the
         * HOMFLY polynomial as returned by homflyAZ().  This is provided to
         * help with pretty-printing HOMFLY polynomials for human consumption.
         *
         * Since homflyAZ() returns a Laurent polynomial in \a alpha and \a z,
         * this string just contains the mathematical symbol \a alpha
         * (encoded in UTF-8).
         *
         * To pretty-print this HOMFLY polynomial for human consumption, you can
         * call <tt>Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
         */
        static constexpr const char* homflyAZVarX = "\u03B1"; // alpha

        /**
         * The name of the second variable used in the variant of the
         * HOMFLY polynomial as returned by homflyAZ().  This is provided to
         * help with pretty-printing HOMFLY polynomials for human consumption.
         *
         * Since homflyAZ() returns a Laurent polynomial in \a alpha and \a z,
         * this string just contains the single character \a z.
         *
         * To pretty-print this HOMFLY polynomial for human consumption, you can
         * call <tt>Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
         */
        static constexpr const char* homflyAZVarY = "z";

        /**
         * The name of the first variable used in the variant of the
         * HOMFLY polynomial as returned by homflyLM().  This is provided to
         * help with pretty-printing HOMFLY polynomials for human consumption.
         *
         * Since homflyLM() returns a Laurent polynomial in \a l and \a m,
         * this string just contains the mathematical script symbol for \a l
         * (encoded in UTF-8).
         *
         * To pretty-print this HOMFLY polynomial for human consumption, you can
         * call <tt>Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
         */
        static constexpr const char* homflyLMVarX = "\U0001D4C1"; // $\ell$

        /**
         * The name of the second variable used in the variant of the
         * HOMFLY polynomial as returned by homflyLM().  This is provided to
         * help with pretty-printing HOMFLY polynomials for human consumption.
         *
         * Since homflyLM() returns a Laurent polynomial in \a l and \a m,
         * this string just contains the single character \a m.
         *
         * To pretty-print this HOMFLY polynomial for human consumption, you can
         * call <tt>Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
         */
        static constexpr const char* homflyLMVarY = "m";

        /**
         * The name of the first variable used in the variant of the
         * HOMFLY polynomial as returned by homfly().
         *
         * This is simply an alias for homflyAZVarX.  See the documentation
         * for homflyAZVarX for further details.
         */
        static constexpr const char* homflyVarX = homflyAZVarX;

        /**
         * The name of the second variable used in the variant of the
         * HOMFLY polynomial as returned by homfly().
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
        Link() = default;
        /**
         * Constructs the unlink with the given number of components.
         *
         * @param unknots the number of (unknotted) components in the
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
         * @param copy the link to copy.
         */
        Link(const Link& copy);
        /**
         * Constructs a new copy of the given link, with the option of
         * whether or not to clone its computed properties also.
         *
         * @param copy the link to copy.
         * @param cloneProps \c true if this should also clone any
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
         * that \e will fire a packet destruction event).
         *
         * @param src the link to move.
         */
        Link(Link&& src) noexcept;
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a link.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - knot signatures, as used by fromKnotSig();
         * - oriented Gauss codes, as used by fromOrientedGauss();
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
         * @param description a string that describes a knot or link.
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
         * @return \c true if and only if this link is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the number of crossings in this link.
         *
         * Note that a link can have more components than crossings
         * (since it may contain additional zero-crossing unknot components).
         *
         * @return the number of crossings.
         */
        size_t size() const;

        /**
         * Returns the number of components in this link.
         *
         * @return the number of components.
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
         * @param index the index of the requested crossing.  This must
         * be between 0 and size()-1 inclusive.
         * @return the crossing at the given index.
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
         * C++11 range-based \c for loops.  Note that the elements of the list
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
         * @return access to the list of all crossings.
         */
        auto crossings() const;

        /**
         * Returns a strand in the given component of this link.
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
         * @param index the index of the requested component.  This must
         * be between 0 and countComponents()-1 inclusive.
         * @return a "starting strand" for traversing the component at
         * the given index, or a null reference if the requested component
         * has no crossings.
         */
        StrandRef component(size_t index) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all components of this link.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  Each element of the list will be
         * a starting strand for some components; more precisely, iterating
         * through this list is equivalent to calling <tt>component(0)</tt>,
         * <tt>component(1)</tt>, ..., <tt>component(countComponents()-1)</tt>
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
         * @return access to the list of all components.
         */
        auto components() const;

        /**
         * Returns the strand in the link with the given integer ID.
         *
         * Each strand ID is of the form 2<i>c</i>+<i>s</i>, where \e c is the
         * index of the crossing, and \e s is 0 or 1 for the lower or
         * upper strand respectively.  A null strand reference (as used to
         * indicate 0-crossing unknot components) has an ID of -1.
         *
         * @param id an integer between -1 and 2*size()-1 inclusive.
         * @return the strand of this link with the corresponding ID.
         *
         * @see StrandRef::id()
         */
        StrandRef strand(int id) const;

        /**
         * Translates a strand reference for some other link into the
         * corresponding strand reference for this link.
         *
         * Specifically: if \a other refers to some strand (upper or lower)
         * of crossing number \a k of some other link, then the return
         * value will refer to the same strand (upper or lower) of
         * crossing number \a k of this link.
         *
         * This routine behaves correctly even if \a other is a null reference.
         *
         * @param other the strand reference to translate.
         * @return the corresponding strand reference for this link.
         */
        StrandRef translate(const StrandRef& other) const;

        /**
         * Determines whether the two given crossings are connected in the
         * underlying 4-valent graph of the link diagram.
         *
         * Here "the underlying 4-valent graph" means the multigraph
         * whose vertices are the crossings and whose edges are the arcs
         * between crossings.  In particular
         *
         * - two crossings may be connected even if they involve
         *   entirely different components of the link;
         *
         * - if two crossings are not connected then the underlying link
         *   must be splittable (though this need not happen in the
         *   other direction: one can have a diagram of a splittable
         *   link in which all crossings are connected with each other).
         *
         * \warning This routine is slow (linear time), since it may
         * need to perform a depth-first search through the graph.
         *
         * @param a the first of the two crossings to examine.
         * @param b the second of the two crossings to examine.
         * @return \c true if and only if the two given crossings are
         * connected.
         */
        bool connected(const Crossing* a, const Crossing* b) const;

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
         * @param other the link diagram to compare with this.
         * @return \c true if and only if the two link diagrams are
         * combinatorially identical.
         */
        bool operator == (const Link& other) const;

        /**
         * Determines if this link diagram is not combinatorially identical
         * to the given link diagram.
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
         * @param other the link diagram to compare with this.
         * @return \c true if and only if the two link diagrams are
         * not combinatorially identical.
         */
        bool operator != (const Link& other) const;

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

        /**
         * Sets this to be a (deep) copy of the given link.
         *
         * @param src the link to copy.
         * @return a reference to this link.
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
         * \note This operator is \e not marked \c noexcept, since it fires
         * change events on this link which may in turn call arbitrary code
         * via any registered packet listeners.  It deliberately does \e not
         * fire change events on \a src, since it assumes that \a src is about
         * to be destroyed (which will fire a destruction event instead).
         *
         * @param src the link to move.
         * @return a reference to this link.
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
         * \note This swap function is \e not marked \c noexcept, since it
         * fires change events on both links which may in turn call arbitrary
         * code via any registered packet listeners.
         *
         * @param other the link whose contents should be swapped with this.
         */
        void swap(Link& other);

        /**
         * Deprecated routine that swaps the contents of this and the
         * given link.
         *
         * \deprecated Use swap() instead.
         *
         * @param other the link whose contents should be swapped with this.
         */
        [[deprecated]] void swapContents(Link& other);

        /**
         * Switches the upper and lower strands of the given crossing.
         *
         * @param c the crossing to change.
         */
        void change(Crossing* c);

        /**
         * Switches the upper and lower strands of every crossing in the
         * diagram.
         *
         * This operation corresponds to reflecting the link diagram
         * through the plane on which it is drawn.
         */
        void changeAll();

        /**
         * Resolves the given crossing.  The two incoming strands will
         * switch connections with the two outgoing strands, with the
         * result that the given crossing is removed entirely.
         *
         * \note The number of components in the link \e will change
         * as a result of this operation.
         *
         * @param c the crossing to resolve.
         */
        void resolve(Crossing* c);

        /**
         * Converts this link into its reflection.
         *
         * This routine changes the sign of every crossing, but leaves
         * the upper and lower strands the same.  This operation
         * corresponds to reflecting the link diagram about some axis in
         * the plane.
         */
        void reflect();

        /**
         * Rotates this link diagram, converting it into a different
         * diagram of the same link.
         *
         * This routine keeps the sign of each crossing fixed, but
         * switches the upper and lower strands.  This operation corresponds
         * to a 3-dimensional rotation about some axis in the plane.
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
         * Tests for and/or performs a type I Reidemeister move to remove a
         * crossing.
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
         *
         * The location of this move is specified by the argument \a crossing,
         * which indicates the crossing that will be removed.  Specifically,
         * this move involves undoing a trivial twist at the given crossing.
         *
         * You may pass a null pointer for \a crossing.  However, in this case
         * the move cannot be performed, which means (i) \a check must be
         * \c true, and therefore (ii) this routine will do nothing and return
         * \c false.
         *
         * \warning A side-effect of this move is that, because one crossing
         * is being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the one involved in this move
         * will be destroyed.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * @param crossing identifies the crossing to be removed.
         * @param check \c true if we are to check whether the move can
         * be performed at the given location.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the move can be performed.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r1(Crossing* crossing, bool check = true, bool perform = true);
        /**
         * Tests for and/or performs a type I Reidemeister move to add a
         * new crossing.
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
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
         * in which it \e cannot be performed is if \a arc is a null reference
         * but this link contains no zero-crossing components, as discussed
         * above.
         *
         * The existing crossings in this link will keep the same indices,
         * and the new crossing will be given the next index that is available.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * @param arc identifies the arc of the link in which the new
         * twist will be introduced, as described above.
         * @param side 0 if the twist should be introduced on the left
         * of the arc (when walking along the arc in the forward direction),
         * or 1 if the twist should be introduced on the right of the arc.
         * @param sign the sign of the new crossing that will be
         * introduced as part of the twist; this must be +1 or -1.
         * @param check \c true if we are to check whether the move can
         * be performed at the given location.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the move can be performed.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r1(StrandRef arc, int side, int sign,
            bool check = true, bool perform = true);
        /**
         * Tests for and/or performs a type II Reidemeister move to remove
         * two crossings.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  This variant, which takes
         * an arc, is more flexible (since either of the two arcs involved in
         * this move can be passed).  The other variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
         *
         * The location of this move is specified by the argument \a arc.
         * Specifically, this move involves pulling apart two arcs of
         * the link that surround a bigon; the given arc must be one of
         * these two arcs.  See the StrandRef documentation for the
         * convention on how arcs are represented using StrandRef objects.
         *
         * You may pass a null reference for \a arc.  However, in this case
         * the move cannot be performed, which means (i) \a check must be
         * \c true, and therefore (ii) this routine will do nothing and return
         * \c false.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * @param arc identifies one of the arcs of the bigon about
         * which the move will be performed, as described above.
         * @param check \c true if we are to check whether the move is legal.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the requested move is legal.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r2(StrandRef arc, bool check = true, bool perform = true);
        /**
         * Tests for and/or performs a type II Reidemeister move to remove
         * two crossings.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  The other variant, which takes
         * an arc, is more flexible (since either of the two arcs involved in
         * this move can be passed).  This variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
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
         * the move cannot be performed, which means (i) \a check must be
         * \c true, and therefore (ii) this routine will do nothing and return
         * \c false.
         *
         * \warning A side-effect of this move is that, because two crossings
         * are being removed, the other crossings in the link may be reindexed.
         * However, no crossings other than the two involved in this move
         * will be destroyed.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * @param crossing identifies the crossing at the beginning of
         * the "upper" arc that features in this move, as described above.
         * @param check \c true if we are to check whether the move is legal.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the requested move is legal.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r2(Crossing* crossing, bool check = true, bool perform = true);
        /**
         * Tests for and/or performs a type II Reidemeister move to add
         * two new crossings.
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
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
         * Likewise, if \e both arcs are null references, then the move
         * will be performed upon two \e different zero-crossing unknot
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
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre Each of the given strand references is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * \warning The check for this move is expensive (linear time),
         * since it includes testing whether both sides-of-arcs belong
         * to the same 2-cell of the knot diagram.
         *
         * @param upperArc identifies the arc of the link which will be
         * passed over the other, as described above.
         * @param upperSide 0 if the new overlap should take place on the left
         * of \a upperArc (when walking along \a upperArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a upperArc.
         * @param lowerArc identifies the arc of the link which will be
         * passed beneath the other, as described above.
         * @param lowerSide 0 if the new overlap should take place on the left
         * of \a lowerArc (when walking along \a lowerArc in the forward
         * direction), or 1 if the new overlap should take place on the right
         * of \a lowerArc.
         * @param check \c true if we are to check whether the move can
         * be performed at the given location.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the move can be performed.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r2(StrandRef upperArc, int upperSide, StrandRef lowerArc,
            int lowerSide, bool check = true, bool perform = true);
        /**
         * Tests for and/or performs a type III Reidemeister move.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  This variant, which takes
         * an arc, is more flexible (since any of the three arcs involved in
         * this move can be passed).  The other variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
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
         * the move cannot be performed, which means (i) \a check must be
         * \c true, and therefore (ii) this routine will do nothing and return
         * \c false.
         *
         * All crossings in this link will keep the same indices, and
         * no crossings will be created or destroyed.  Instead, the three
         * crossings involved in this move will simply be reordered
         * along the various segments of the link.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this link.
         *
         * @param arc identifies one of the arcs of the triangle about
         * which the move will be performed, as described above.
         * @param side 0 if the third crossing of the triangle is located to
         * the left of the arc (when walking along the arc in the forward
         * direction), or 1 if the third crossing is located on the right of
         * the arc.
         * @param check \c true if we are to check whether the move can
         * be performed at the given location.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the move can be performed.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r3(StrandRef arc, int side, bool check = true,
            bool perform = true);
        /**
         * Tests for and/or performs a type III Reidemeister move.
         *
         * There are two variants of this routine: one that takes an
         * arc, and one that takes a crossing.  The other variant, which takes
         * an arc, is more flexible (since any of the three arcs involved in
         * this move can be passed).  This variant, which takes a
         * crossing, offers a canonical way of performing the move (since for
         * each move there is exactly one crossing that describes it).
         *
         * There are two boolean arguments that control the behaviour of
         * this routine: \a check and \a perform.
         *
         * - If \a check and \a perform are both \c true (the default), then
         *   this routine will first check whether this move can be performed
         *   at the given location.  If so, it will perform the move and
         *   return \c true.  If not, it will do nothing and return \c false.
         *
         * - If \a check is \c true but \a perform is \c false, then this
         *   routine will simply check whether this move can be performed at
         *   the given location and return \c true or \c false accordingly.
         *
         * - If \a check is \c false but \a perform is \c true, then this
         *   routine will perform the move without any prior checks, and will
         *   always return \c true.  In this case, it must be known in advance
         *   that the move can be performed at the given location.
         *
         * - If \a check and \a perform are both \c false, then this
         *   routine does nothing and just returns \c true.  (There is
         *   no reason to use this combination of arguments.)
         *
         * The location of this move is specified by the arguments \a crossing
         * and \a side.  Specifically, this move takes place around a
         * triangle, and one of the arcs of this triangle is \e uppermost (in
         * that it passes above the other two arcs).  The given crossing
         * should be the start point of this uppermost arc; that is, when
         * following the arc forwards, \a crossing should be the first of the
         * two crossings that we encounter.  The additional argument \a side
         * indicates on which side of the uppermost arc the third crossing is
         * located.
         *
         * You may pass a null pointer for \a crossing.  However, in this case
         * the move cannot be performed, which means (i) \a check must be
         * \c true, and therefore (ii) this routine will do nothing and return
         * \c false.
         *
         * All crossings in this link will keep the same indices, and
         * no crossings will be created or destroyed.  Instead, the three
         * crossings involved in this move will simply be reordered
         * along the various segments of the link.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this link.
         *
         * @param crossing identifies the crossing at the beginning of
         * the "uppermost" arc that features in this move, as described above.
         * @param side 0 if the third crossing of the triangle is located to
         * the left of the uppermost arc (when walking along the arc in the
         * forward direction), or 1 if the third crossing is located on the
         * right of the uppermost arc.
         * @param check \c true if we are to check whether the move can
         * be performed at the given location.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the move can be performed.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r3(Crossing* crossing, int side, bool check = true,
            bool perform = true);

        /**
         * Tests whether this knot has a pass move that will reduce the
         * number of crossings.
         *
         * Currently this routine is only available for knots, not
         * multiple-component links.
         *
         * A \e pass move involves taking a section of the knot that
         * involves only over-crossings (or only under-crossings), and
         * then lifting that section above (or beneath respectively) the
         * diagram and placing it back again in a different location.
         * In particular, this routine searches for a different location
         * that will involve fewer crossings than the original location.
         *
         * This routine does not actually \e perform the pass move; it
         * simply determines whether one exists.
         *
         * The running time is cubic in the number of crossings.
         *
         * \pre This link is actually a knot (i.e., it contains exactly
         * one component).
         *
         * @return \c true if and only if there is a pass move that
         * reduces the number of crossings.
         */
        bool hasReducingPass() const;

        /**
         * Adds trivial twists to this link to ensure that each component has
         * zero writhe.  Here the \e writhe of a component \a c is the sum of
         * the signs of all crossings at which \a c crosses itself.
         *
         * Any component(s) that already have zero writhe will be left
         * unchanged.
         *
         * This link will be modified directly.
         */
        void selfFrame();

        /**
         * Attempts to simplify the link diagram using fast and greedy
         * heuristics.  Specifically, this routine tries combinations of
         * Reidemeister moves with the aim of reducing the number of
         * crossings.
         *
         * Currently this routine uses simplifyToLocalMinimum() in
         * combination with random type III Reidemeister moves.
         *
         * Although intelligentSimplify() often works well, it can sometimes
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
         */
        bool intelligentSimplify();
        /**
         * Uses type I and II Reidemeister moves to reduce the link
         * monotonically to some local minimum number of crossings.
         *
         * End users will probably not want to call this routine.
         * You should call intelligentSimplify() if you want a fast (and
         * usually effective) means of simplifying a link.  If this link is
         * a knot (i.e., it has precisely one component), then you can also
         * call simplifyExhaustive() if you are still stuck and you want to
         * try a slower but more powerful method instead.
         *
         * Type III Reidemeister moves (which do not reduce the number of
         * crossings) are not used in this routine.  Such moves do however
         * feature in intelligentSimplify().
         *
         * This routine will never reflect or reverse the link.
         *
         * \warning The implementation of this routine (and therefore
         * its results) may change between different releases of Regina.
         *
         * @param perform \c true if we are to perform the
         * simplifications, or \c false if we are only to investigate
         * whether simplifications are possible (defaults to \c true).
         * @return if \a perform is \c true, this routine returns
         * \c true if and only if the link was changed to
         * reduce the number of crossings; if \a perform is \c false,
         * this routine returns \c true if and only if it determines
         * that it is capable of performing such a change.
         */
        bool simplifyToLocalMinimum(bool perform = true);

        /**
         * Attempts to simplify this knot diagram using a slow but
         * exhaustive search through the Reidemeister graph.  This routine is
         * more powerful but much slower than intelligentSimplify().
         *
         * Unlike intelligentSimplify(), this routine <b>could potentially
         * reflect or reverse the link</b>.
         *
         * This routine is only available for knots at the present time.
         * If this link has multiple (or zero) components, then this
         * routine will throw an exception (as described below).
         *
         * This routine will iterate through all knot diagrams that can be
         * reached from this via Reidemeister moves, without ever exceeding
         * \a height additional crossings beyond the original number.
         *
         * If at any stage it finds a diagram with \e fewer
         * crossings than the original, then this routine will call
         * intelligentSimplify() to simplify the diagram further if
         * possible and will then return \c true.  If it cannot find a
         * diagram with fewer crossings then it will leave this
         * knot diagram unchanged and return \c false.
         *
         * This routine can be very slow and very memory-intensive: the
         * number of knot diagrams it visits may be exponential in
         * the number of crossings, and it records every diagram
         * that it visits (so as to avoid revisiting the same diagram
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if this fails then try increasing \a height one at a time until
         * either you find a simplification or the routine becomes
         * too expensive to run.
         *
         * If \a height is negative, then there will be \e no bound on
         * the number of additional crossings.  This means that the
         * routine will not terminate until a simpler diagram is found.
         * If no simpler diagram exists then the only way to terminate this
         * function is to cancel the operation via a progress tracker
         * (read on for details).
         *
         * If you want a \e fast simplification routine, you should call
         * intelligentSimplify() instead.  The benefit of simplifyExhaustive()
         * is that, for very stubborn knot diagrams where intelligentSimplify()
         * finds itself stuck at a local minimum, simplifyExhaustive() is able
         * to "climb out" of such wells.
         *
         * Since Regina 7.0, this routine will not return until either the
         * knot diagram is simplified or the exhaustive search is complete,
         * regardless of whether a progress tracker was passed.  If you
         * need the old behaviour (where passing a progress tracker caused
         * the exhaustive search to start in the background), simply call
         * this routine in a new detached thread.
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a nThreads.  Even in multithreaded mode, this
         * routine will not return until processing has finished (i.e., either
         * the diagram was simplified or the search was exhausted).
         *
         * If this routine is unable to simplify the knot diagram, then
         * this knot diagram will not be changed.
         *
         * \pre This link has at most one component (i.e., it is empty
         * or it is a knot).
         *
         * \exception FailedPrecondition this link has more than one component.
         * If a progress tracker was passed, it will be marked as finished
         * before the exception is thrown.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param height the maximum number of \e additional crossings to
         * allow beyond the number of crossings originally present in this
         * diagram, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return \c true if and only if this diagram was successfully
         * simplified to fewer crossings.
         */
        bool simplifyExhaustive(int height = 1, unsigned nThreads = 1,
            ProgressTrackerOpen* tracker = nullptr);

        /**
         * Explores all knot diagrams that can be reached from this via
         * Reidemeister moves, without exceeding a given number of additional
         * crossings.
         *
         * This routine is only available for knots at the present time.
         * If this link has multiple (or zero) components, then this
         * routine will throw an exception (as described below).
         *
         * This routine iterates through all knot diagrams that can be reached
         * from this via Reidemeister moves, without ever exceeding
         * \a height additional crossings beyond the original number.
         * With the current implementation, these diagrams <b>could become
         * reflected and/or reversed</b>, and moreover each diagram will only be
         * considered once up to reflection and/or reversal; be aware that this
         * behaviour could change and/or become configurable in a future version
         * of Regina.
         *
         * For every such knot diagram (including this starting
         * diagram), this routine will call \a action (which must
         * be a function or some other callable object).
         *
         * - \a action must take the following initial argument(s).
         *   Either (a) the first argument must be a link (the precise type
         *   is discussed below), representing the knot diagram that has been
         *   found; or else (b) the first two arguments must be of types
         *   const std::string& followed by a link, representing both the
         *   knot diagram and its knot signature.
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
         *   immediately (i.e., no more knot diagrams will be processed).
         *
         * - \a action may, if it chooses, make changes to this knot
         *   (i.e., the original knot upon which rewrite() was called).
         *   This will not affect the search: all knot diagrams
         *   that this routine visits will be obtained via Reidemeister moves
         *   from the original knot diagram, before any subsequent changes
         *   (if any) were made.
         *
         * - \a action will only be called once for each knot diagram
         *   (including this starting diagram).  In other words, no
         *   knot diagram will be revisited a second time in a single call
         *   to rewrite().
         *
         * This routine can be very slow and very memory-intensive, since the
         * number of knot diagrams it visits may be exponential in
         * the number of crossings, and it records every knot diagram
         * that it visits (so as to avoid revisiting the same diagram
         * again).  It is highly recommended that you begin with \a height = 1,
         * and if necessary try increasing \a height one at a time until
         * this routine becomes too expensive to run.
         *
         * If \a height is negative, then there will be \e no bound on
         * the number of additional crossings.  This means that the
         * routine will <i>never terminate</i>, unless \a action returns
         * \c true for some knot diagram that is passed to it.
         *
         * Since Regina 7.0, this routine will not return until the exploration
         * of knot diagrams is complete, regardless of whether a progress
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
         * \pre This link has precisely one component (i.e., it is a knot).
         *
         * \exception FailedPrecondition this link is empty or has more than
         * one component.  If a progress tracker was passed, it will be marked
         * as finished before the exception is thrown.
         *
         * \apinotfinal
         *
         * \ifacespython This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the arguments \a tracker and \a args are
         * removed, so you simply call it as rewrite(height, threads, action).
         * Moreover, \a action must take exactly two arguments
         * (const std::string&, Link&&) representing the knot signature and
         * the knot diagram, as described in option (b) above.
         *
         * @param height the maximum number of \e additional crossings to
         * allow beyond the number of crossings originally present in this
         * knot diagram, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @param action a function (or other callable object) to call
         * for each knot diagram that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial knot argument(s).
         * @return \c true if some call to \a action returned \c true (thereby
         * terminating the search early), or \c false if the search ran to
         * completion.
         */
        template <typename Action, typename... Args>
        bool rewrite(int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            Action&& action, Args&&... args) const;

        /**
         * Forms the composition of this with the given link.
         * This link will be altered directly.
         *
         * Specifically, the first component of the given link will be
         * grafted into the first component of this link, in a way that
         * preserves orientations and crossing signs.  If the given link
         * has any additional components, then they will be copied into
         * this link directly with no modification.
         *
         * This routine may be expanded in future versions of Regina to
         * allow more flexibility (in particular, to allow you to choose
         * which components of the two links to graft together, and/or
         * at which strands to graft them).
         *
         * If either link is empty (i.e., contains no components at all),
         * then the result will simply be a clone of the other link
         * (with no composition operation performed).
         *
         * It is allowed to pass this link as \a other.
         *
         * @param other the link with which this should be composed.
         */
        void composeWith(const Link& other);

        /*@}*/
        /**
         * \name Invariants and Related Properties
         */
        /*@{*/

        /**
         * Returns whether this knot diagram is alternating.
         *
         * Note that this routine cannot tell whether the \e knot is
         * alternating (i.e., whether there \e exists an alternating diagram).
         * Instead, it simply returns whether this specific diagram is
         * alternating or not.
         *
         * The empty diagram and any zero-crossing unknot components
         * will be considered alternating.
         *
         * @return \c true if this is an alternating diagram, or \c false
         * if this is a non-alternating diagram.
         */
        bool isAlternating() const;

        /**
         * Returns the linking number of this link.
         *
         * This is an invariant of the link, computed as half the sum of the
         * signs of all crossings that involve different link components.
         *
         * The algorithm to compute linking number is linear time.
         *
         * @return the linking number.
         */
        long linking() const;

        /**
         * Returns the writhe of this link diagram.
         *
         * This is \e not an invariant of the link; instead it depends
         * on the particular link diagram.  It is computed as the sum
         * of the signs of all crossings.  It is preserved under
         * Reidemeister moves II and III, but not I.
         *
         * @return the writhe.
         */
        long writhe() const;

        /**
         * Returns the writhe of a single component of this link diagram.
         *
         * This is the writhe of the diagram when all \e other components
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
         * @param component any strand along the component of interest.
         * @return the writhe of the component containing the given strand,
         * or 0 if the given strand is a null strand.
         */
        long writheOfComponent(StrandRef component) const;

        /**
         * Returns the writhe of a single component of this link diagram.
         *
         * This is the writhe of the diagram when all \e other components
         * are removed.  It is computed as the sum of the signs of all
         * crossings at which the given component crosses itself.
         *
         * In this version of writheOfComponent(), the component is
         * indicated by its index.  This function is equivalent to calling
         * <tt>writheOfComponent(component(index))</tt>.
         *
         * @param index the index of the requested component.  This must
         * be between 0 and countComponents()-1 inclusive.
         * @return the writhe of the given component.
         */
        long writheOfComponent(size_t index) const;

        /**
         * Returns an ideal triangulation of the complement of this link
         * in the 3-sphere.
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
         * This is the same triangulation that would be produced by passing
         * this link to the Triangulation<3> constructor.
         *
         * @param simplify \c true if and only if the triangulation of the
         * complement should be simplified to use as few tetrahedra as possible.
         * @return the complement of this link.
         */
        Triangulation<3> complement(bool simplify = true) const;

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
         * @param k the number of parallel copies to create.
         * This must be non-negative.
         * @param framing the framing under which these copies will be parallel.
         * @return \a k parallel copies of this link.
         */
        Link parallel(int k, Framing framing = FRAMING_SEIFERT) const;

        /**
         * Returns the Kauffman bracket polynomial of this link diagram.
         *
         * Note that the bracket polynomial is not an invariant - it is
         * preserved under Reidemeister moves II and III, but not I.
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * Bear in mind that each time the link changes, all of its
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
         * \warning The naive algorithm can only handle a limited number
         * of crossings (currently less than the number of bits in a long,
         * which on a typical machine is 64).  If you pass ALG_NAIVE and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_NAIVE is a slow algorithm that computes
         * the Kauffman bracket by resolving all crossings in all possible
         * ways, and ALG_TREEWIDTH uses a fixed-parameter tractable
         * treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the bracket polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent<Integer>& bracket(Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the Kauffman bracket polynomial of this link diagram
         * already known?  See bracket() for further details.
         *
         * If this property is already known, future calls to bracket() will
         * be very fast (simply returning the precalculated value).
         *
         * @return \c true if and only if this property is already known.
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
         *   <tt>1/t + 1/t^3 - 1/t^4</tt>, and so this routine returns the
         *   Laurent polynomial <tt>x^-2 + x^-6 - x^-8</tt>.
         *
         * - The Hopf link has Jones polynomial
         *   <tt>-1/sqrt(x) - 1/sqrt(x^5)</tt>, and so this routine returns
         *   the Laurent polynomial <tt>-x^-1 - x^-5</tt>.
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
         * the variable <i>x</i> (which represents the square root of \a t)
         * with the expression -<i>q</i>.
         *
         * To pretty-print this polynomial for human consumption, you can
         * call <tt>Laurent::str(Link::jonesVar)</tt>.
         *
         * Bear in mind that each time the link changes, all of its
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
         * \warning The naive algorithm can only handle a limited number
         * of crossings (currently less than the number of bits in a long,
         * which on a typical machine is 64).  If you pass ALG_NAIVE and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_NAIVE is a slow algorithm that computes
         * the Kauffman bracket by resolving all crossings in all possible
         * ways, and ALG_TREEWIDTH uses a fixed-parameter tractable
         * treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the Jones polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent<Integer>& jones(Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the Jones polynomial of this link diagram already known?
         * See jones() for further details.
         *
         * If this property is already known, future calls to jones() will be
         * very fast (simply returning the precalculated value).
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsJones() const;

        /**
         * Returns the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z.
         *
         * This variant of the HOMFLY polynomial is described (amongst other
         * places) in G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants of the HOMFLY
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
         * <tt>Laurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
         *
         * The default implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * Bear in mind that each time the link changes, all of its
         * polynomials will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homflyAZ() should be called again; this will be
         * instantaneous if the HOMFLY polynomial has already been calculated.
         *
         * If the HOMFLY polynomial has already been computed (either in terms
         * of \a alpha and \a z or in terms of \a l and \a m), then the result
         * will be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homflyAZ(Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Returns the HOMFLY polynomial of this link, as a polynomial
         * in \a l and \a m.
         *
         * This variant of the HOMFLY polynomial is described (amongst other
         * places) in C. C. Adams, "The knot book", W. H. Freeman & Co., 1994.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants of the HOMFLY
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
         * <tt>Laurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
         *
         * The default implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * Bear in mind that each time the link changes, all of its
         * polynomials will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homflyLM() should be called again; this will be
         * instantaneous if the HOMFLY polynomial has already been calculated.
         *
         * If the HOMFLY polynomial has already been computed (either in terms
         * of \a alpha and \a z or in terms of \a l and \a m), then the result
         * will be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         *
         * Since Regina 7.0, this routine will not return until the polynomial
         * computation is complete, regardless of whether a progress tracker
         * was passed.  If you need the old behaviour (where passing a progress
         * tracker caused the computation to start in the background), simply
         * call this routine in a new detached thread.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homflyLM(Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Returns the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z.
         *
         * This routine is simply an alias for homflyAZ().  See the
         * documentation for homflyAZ() for further details.
         *
         * To pretty-print this polynomial for human consumption, you can call
         * <tt>Laurent2::str(Link::homflyVarX, Link::homflyVarY)</tt>.
         *
         * Bear in mind that each time the link changes, all of its
         * polynomials will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, homfly() should be called again; this will be
         * instantaneous if the HOMFLY polynomial has already been calculated.
         *
         * \ifacespython The global interpreter lock will be released while
         * this function runs, so you can use it with Python-based
         * multithreading.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial, or the zero polynomial if the
         * calculation was cancelled via the given progress tracker.
         */
        const Laurent2<Integer>& homfly(Algorithm alg = ALG_DEFAULT,
            ProgressTracker* tracker = nullptr) const;
        /**
         * Is the HOMFLY polynomial of this link diagram already known?
         * See homflyAZ() and homflyLM() for further details.
         *
         * If this property is already known, future calls to homfly(),
         * homflyAZ() and homflyLM() will all be very fast (simply returning
         * the precalculated values).
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsHomfly() const;

        /**
         * Converts between the (\a alpha, \a z) and (\a l, \a m)
         * representations of the HOMFLY polynomial.
         *
         * The (\a alpha, \a z) and (\a l, \a m) variants are related by a
         * simple transformation: \a alpha = \a l \a i and \a z = -\a m \a i,
         * where \a i represents (as usual) a square root of -1.
         *
         * See homflyAZ() and homflyLM() for further details.
         *
         * @param homflyAZ the HOMFLY polynomial of a link as a polynomial in
         * \a alpha and \a z, where (\a alpha, \a z) are represented by
         * (\a x, \a y) in the class Laurent2<Integer>.
         * @return the HOMFLY polynomial of the same link as a polynomial in
         * \a l and \a m, where (\a l, \a m) are represented by (\a x, \a y)
         * in the class Laurent2<Integer>.
         */
        static Laurent2<Integer> homflyAZtoLM(Laurent2<Integer> homflyAZ);

        /**
         * Returns the group of this link; that is, the fundamental group of
         * the link exterior.
         *
         * This routine builds the Wirtinger presentation, where all relations
         * are some variant of the form <tt>xy=yz</tt>.
         *
         * If you pass \a simplify as \c false, it will leave the presentation
         * in exactly this form (i.e., the Wirtinger presentation), and not
         * simplify it further.  If you pass \a simplify as \c true (the
         * default), this routine will attempt to simplify the group
         * presentation before returning.
         *
         * \note If you are finding the resulting group presentation too large
         * for your liking even after simplification, then you could also try
         * calling complement() and computing the fundamental group of the
         * resulting 3-manifold triangulation.  Sometimes the presentation
         * obtained via the complement is better, and sometimes it is worse.
         *
         * Currently this group is \e not cached; instead it is reconstructed
         * every time this function is called.  This behaviour may change in
         * future versions of Regina.
         *
         * @return the group of this link.
         */
        GroupPresentation group(bool simplify = true) const;

        /**
         * Returns a nice tree decomposition of the planar 4-valent
         * multigraph formed by this link diagram.  This can (for example)
         * be used in implementing algorithms that are fixed-parameter
         * tractable in the treewidth of this graph.
         *
         * See TreeDecomposition for further details on tree decompositions,
         * and see TreeDecomposition::makeNice() for details on what it
         * means to be a \e nice tree decomposition.
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
         * @return a nice tree decomposition of this link diagram.
         */
        const TreeDecomposition& niceTreeDecomposition() const;

        /**
         * Instructs Regina to use the given tree decomposition as the
         * starting point whenever it needs a tree decomposition for this link.
         *
         * For some link routines, including niceTreeDecomposition() as
         * well as computations such as jones() that support the option
         * ALG_TREEWIDTH, Regina needs a tree decomposition of the
         * planar 4-valent multigraph formed by this link diagram.
         *
         * By default, Regina will compute (and then cache) such a tree
         * decomposition itself, using in-built greedy heuristics.  This
         * routine allows you to supply your \e own tree decomposition
         * (which, for example, might be a smaller-width tree decomposition
         * that you found using third-party software).  By supplying
         * your own tree decomposition \e td through this routine,
         * Regina will throw away any pre-computed tree decomposition
         * that it has cached, and will instead cache \e td for future
         * use instead.
         *
         * Regina may modify the given tree decomposition for its purposes.
         * In particular, \e td does not need to be a \e nice tree
         * decomposition (indeed, it does not need to have any special
         * properties beyond the definition of a tree decomposition).
         * Regina will automatically create a nice tree decomposition
         * from it if \e td is not nice already.
         *
         * @param td a tree decomposition of the planar 4-valent
         * multigraph formed by this link diagram.
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
         * - a sequence of signs (<tt>+</tt> or <tt>-</tt>), concatenated
         *   together, giving the signs of the crossings in order from
         *   crossing 0 to crossing size()-1;
         *
         * - a description of each component of the link, in order from
         *   component 0 to component countComponents()-1.  Each component
         *   will be written in the form <tt>( a b c ... )</tt>, indicating
         *   the crossings that are encountered as we follow the component
         *   in the forward direction from its starting strand.  Each element
         *   \a a, \a b, \a c and so on will be written in the format used by
         *   the StrandRef class: either <tt>^n</tt> when passing over
         *   crossing \a n, or <tt>_n</tt> when passing under crossing \a n.
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
         * simply be a sequence of the form <tt>( ) ( ) ... ( )</tt>.
         *
         * The string will not end in a newline.
         *
         * There is also a variant of brief() that writes directly to an
         * output stream.
         *
         * @return a description of this link in Regina's brief format.
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
         * \ifacespython Not present; instead use the variant
         * brief() that takes no arguments and returns a string.
         *
         * @param out the output stream to which to write.
         */
        void brief(std::ostream& out) const;

        /**
         * Returns a classical Gauss code for this knot, presented as a string.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.
         * These codes come with two major restrictions:
         *
         * - In general, they do not carry enough information to uniquely
         *   reconstruct a knot.  For instance, both a knot and its reflection
         *   can be described by the same Gauss code; moreover, for composite
         *   knots, the Gauss code can describe inequivalent knots (even when
         *   allowing for reflections).
         *
         * - Parsing a Gauss code is complex, since it requires an embedding
         *   to be deduced using some variant of a planarity testing algorithm.
         *
         * If you need a code that specifies the knot uniquely and/or that
         * is fast to parse, consider using the \e oriented Gauss code instead,
         * which resolves both of these issues.
         *
         * A Gauss code for an <i>n</i>-crossing knot is described by
         * a sequence of 2<i>n</i> positive and negative integers,
         * representing strands that pass over and under crossings
         * respectively.  The code is constructed as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   Whenever you pass crossing \a k, write the integer
         *   <tt><i>k</i></tt> if you pass over the crossing,
         *   or <tt>-<i>k</i></tt> if you pass under the crossing.
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
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * @return a classical Gauss code as described above.
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
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * @return a classical Gauss code for this knot in machine-readable
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
         * See also gauss(), which returns the Gauss code as a
         * human-readable string, and gaussData(), which returns it
         * as a machine-readable sequence of integers.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \ifacespython Not present; instead use the variants
         * gauss() or gaussData() that take no arguments.
         *
         * @param out the output stream to which to write.
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
         * reconstruction procedure for classical Gauss codes.
         *
         * This "oriented" format is described at
         * <http://www.javaview.de/services/knots/doc/description.html#gc>,
         * and it works as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   At every crossing that you pass, write a token of the form
         *   <tt>+&lt;<i>k</i></tt>, <tt>-&lt;<i>k</i></tt>,
         *   <tt>+&gt;<i>k</i></tt> or <tt>-&gt;<i>k</i></tt>, where:
         *
         *     * the symbol <tt>+</tt> indicates that you are passing over the
         *       crossing labelled \a k, and the symbol <tt>-</tt> indicates
         *       that you are passing under the crossing labelled \a k;
         *
         *     * the symbol <tt>&lt;</tt> indicates that the other strand of
         *       the crossing passes from right to left, and <tt>&gt;</tt>
         *       indicates that the other strand passes from left to right.
         *
         * As an example, you can represent the left-hand trefoil using the
         * code:
         *
           \verbatim
           +>1 -<2 +>3 -<1 +>2 -<3
           \endverbatim
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
         * @return an oriented Gauss code as described above.
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
         * <tt>+&lt;<i>k</i></tt>, <tt>-&lt;<i>k</i></tt>,
         * <tt>+&gt;<i>k</i></tt> or <tt>-&gt;<i>k</i></tt> that would normally
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
         * @return an oriented Gauss code for this knot in machine-readable
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
         * See also orientedGauss(), which returns the oriented Gauss code as
         * a human-readable string, and orientedGaussData(), which returns it
         * as a machine-readable sequence of tokens.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * \ifacespython Not present; instead use the variants
         * orientedGauss() or orientedGaussData() that take no arguments.
         *
         * @param out the output stream to which to write.
         */
        void orientedGauss(std::ostream& out) const;

        /**
         * Exports this link using Bob Jenkins' text format, returning a
         * single string.
         *
         * Jenkins' format is lengthy.  However, in contrast to classical
         * Gauss codes or Dowker-Thistlethwaite notation, there are no
         * topological ambiguities in the format, and reconstructing a link
         * from Jenkins' format is simple.  Moreover, the format is suitable
         * for links with any number of components.
         *
         * Jenkins' format is described in his HOMFLY polynomial software,
         * which is available online from
         * <http://burtleburtle.net/bob/knot/homfly.html>.
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
         * @return a description of this link using Jenkins' text format.
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
         * @return a description of this link using Jenkins' format
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
         * See also jenkins(), which exports this link in Jenkins' format
         * as a human-readable string, and jenkinsData(), which exports it
         * as a machine-readable sequence of integers.
         *
         * \ifacespython Not present; instead use the variants
         * jenkins() or jenkinsData() that take no arguments.
         *
         * @param out the output stream to which to write.
         */
        void jenkins(std::ostream& out) const;

        /**
         * Exports this knot in either numerical or alphabetical
         * Dowker-Thistlethwaite notation, returning a string.
         *
         * Like classical Gauss codes, Dowker-Thistlethwaite notation
         * essentially describes the 4-valent graph of a knot but not the
         * particular embedding in the plane.  It comes with two major
         * restrictions:
         *
         * - In general, it does not carry enough information to uniquely
         *   reconstruct a knot.  For instance, both a knot and its reflection
         *   can be described by the same Dowker-Thistlethwaite notation;
         *   moreover, for composite knots, the same notation can describe
         *   inequivalent knots (even when allowing for reflections).
         *
         * - Parsing Dowker-Thistlethwaite notation is complex, since it
         *   requires an embedding to be deduced using some variant of a
         *   planarity testing algorithm.
         *
         * If you need a code that specifies the knot uniquely and/or that
         * is fast to parse, consider using the \e oriented Gauss code instead,
         * which resolves both of these issues.
         *
         * For an <i>n</i>-crossing knot, Regina supports two variants
         * of Dowker-Thistlethwaite notation:
         *
         * - a \e numerical variant (the default), which is a sequence of
         *   \a n even signed integers as described (amongst other places) in
         *   Section 2.2 of C. C. Adams, "The knot book", W. H. Freeman & Co.,
         *   1994;
         *
         * - an \e alphabetical variant, which transforms the numerical
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
         * components, or \a alpha is true and it has more than 26 crossings.
         *
         * @param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         * @return the Dowker-Thistlethwaite notation for this knot diagram.
         */
        std::string dt(bool alpha = false) const;

        /**
         * Exports this knot in numerical Dowker-Thistlethwaite notation,
         * returning a vector of integers.
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, as well as its limitations.
         *
         * Although Regina can work with both the numerical and alphabetical
         * variants of Dowker-Thistlethwaite notation, this dtData() routine
         * exports the numerical variant only.  If you wish to export the
         * alphabetical variant, you can call <tt>dt(true)</tt>.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, calling <tt>dt()</tt> returns the same integer
         * sequence in human-readable format (as a string).
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * @return the numerical Dowker-Thistlethwaite notation in
         * machine-readable form.
         */
        std::vector<int> dtData() const;

        /**
         * Writes this knot to the given output stream using
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
         * See also dtBool(bool), which can export either the numerical
         * or alphabetical variant of Dowker-Thistlethwaite notation as a
         * human-readable string, and dtData(), which exports the numerical
         * variant only as a machine-readable sequence of integers.
         *
         * \exception NotImplemented Either this link is empty or has multiple
         * components, or \a alpha is true and it has more than 26 crossings.
         *
         * \ifacespython Not present; instead use the variants
         * dt(bool) or dtData() that take no arguments.
         *
         * @param out the output stream to which to write.
         * @param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         */
        void dt(std::ostream& out, bool alpha = false) const;

        /**
         * Returns a planar diagram code for this link, presented as a string.
         *
         * Planar diagram codes encode the local information at each
         * crossing, and present this information as a list of 4-tuples.
         * These codes are available for links as well as knots, but they do
         * come with some minor restrictions:
         *
         * - They cannot encode zero-crossing unknot components (i.e.,
         *   components for which the component() function returns a null
         *   strand).  Any such components will simply be omitted from the code.
         *
         * - If a link has any components that consist entirely of
         *   over-crossings (which must be unknots "placed on top of" the link
         *   diagram), a planar diagram code does not carry enough data to
         *   reconstruct the \e orientation of these components.  The topology
         *   will be preserved, but in general the combinatorics of such a link
         *   diagram cannot be reconstructed faithfully.
         *
         * If you need a text code that can work with these types of
         * link diagrams, you can always use Jenkins' format instead.
         *
         * Regina adheres to a tight specification for the planar diagram codes
         * that it outputs, in order to ensure compatibility with other
         * software.  In particular, Regina's codes are compatible with the
         * Knot Atlas, as seen at http://katlas.org/wiki/Planar_Diagrams .
         *
         * In detail: a planar diagram code for an <i>n</i>-crossing link
         * is formed from a sequence of <i>n</i> 4-tuples of integers.
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
         * http://katlas.org/wiki/Planar_Diagrams .
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
         * @return the planar diagram code, as described above.
         */
        std::string pd() const;

        /**
         * Returns a planar diagram code for this link, presented as
         * vector of 4-tuples.
         *
         * See pd() for a full description of planar diagram codes as
         * they are used in Regina, as well as their limitations.
         *
         * This routine returns machine-readable data (as a C++ vector);
         * in contrast, pd() returns the same data in human-readable format
         * (as a string).
         *
         * @return the planar diagram code in machine-readable form.
         */
        std::vector<std::array<int, 4>> pdData() const;

        /**
         * Writes a planar diagram code for this link to the given output
         * stream.
         *
         * See pd() for a full description of planar diagram codes as
         * they are used in Regina, as well as their limitations.
         *
         * The output from this routine is precisely the string that
         * would be returned by pd().  In particular, the output does
         * not contain any newlines.
         *
         * See also pd(), which returns the planar diagram code as a
         * human-readable string, and pdData(), which returns it
         * as a machine-readable sequence of 4-tuples of integers.
         *
         * \ifacespython Not present; instead use the variants
         * pd() or pdData() that take no arguments.
         *
         * @param out the output stream to which to write.
         */
        void pd(std::ostream& out) const;

        /**
         * Outputs the underlying planar 4-valent multigraph using the
         * PACE text format.
         * The text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ .
         *
         * In summary, the output will consist of several lines of text:
         *
         * - The first line will be of the form
         *   <tt>p&nbsp;tw&nbsp;<i>num_vertices</i>&nbsp;<i>num_edges</i></tt>.
         *   Note that, since the underlying graph comes from a link diagram,
         *   we will always have \e num_edges equal to twice \e num_vertices.
         *
         * - Following this will be \e num_edges lines, one for each
         *   edge, each of the form <tt><i>u&nbsp;v</i></tt>, indicating
         *   an edge from vertex number \e u to vertex number \e v.
         *   In this format, vertices are numbered 1,2,...,\e num_vertices.
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
         * \ifacespython The \a out argument is not present; instead
         * standard output is assumed.
         *
         * \ifacespython Not present; instead use the variant pace() that
         * takes no arguments and returns a string.
         *
         * @param out the output stream to which to write.
         *
         * @see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        void writePACE(std::ostream& out) const;
        /**
         * Returns a text representation of the underlying planar
         * 4-valent multigraph, using the PACE text format.
         * The text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ ,
         * and is documented in detail by the routine writePACE().
         *
         * This routine simply returns the output of writePACE() as a
         * string, instead of writing it to an output stream.
         *
         * See the writePACE() notes for further details.
         *
         * @return the output of writePACE(), as outlined above.
         *
         * @see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        std::string pace() const;

        /**
         * Returns C++ code that can be used to reconstruct this link.
         *
         * This code will call Link::fromData(), passing a series of
         * hard-coded C++11 initialiser lists.
         *
         * The main purpose of this routine is to generate these hard-coded
         * initialiser lists, which can be tedious and error-prone to write
         * by hand.
         *
         * @return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /**
         * Constructs the \e signature for this knot diagram.
         *
         * A <i>signature</i> is a compact text representation of a knot
         * diagram that unique determines the knot up to relabelling,
         * rotation, and (optionally) reflection and/or reversal.
         *
         * Currently signatures are only implemented for knots, not
         * empty or multiple component links.  If this link does not
         * have precisely one component, then this routine will throw an
         * exception.  It is possible that in future versions of Regina,
         * knot signatures will be expanded to cover all possible link
         * diagrams (hence the choice of NotImplemented as the exception type).
         *
         * The signature is constructed entirely of printable characters,
         * and has length proportional to <tt>n log n</tt>, where \a n
         * is the number of crossings.
         *
         * The routine fromKnotSig() can be used to recover a knot from
         * its signature.  The resulting knot might not be identical to
         * the original, but it will be related by zero or more applications
         * of relabelling, rotation, and/or (according to the arguments)
         * reflection and reversal.
         *
         * This routine runs in quadratic time.
         *
         * \exception NotImplemented This link is empty or has multiple
         * components.
         *
         * @param useReflection \c true if the reflection of a knot diagram
         * should have the same signature as the original, or \c false
         * if these should be distinct (assuming the diagram is not symmetric
         * under reflection).
         * @param useReversal \c true if the reversal of a knot diagram
         * should have the same signature as the original, or \c false
         * if these should be distinct (assuming the diagram is not symmetric
         * under reversal).
         * @return the signature for this knot diagram.
         */
        std::string knotSig(bool useReflection = true, bool useReversal = true)
            const;

        /**
         * Writes a short text representation of this link to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this link to the
         * given output stream.
         *
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
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
         * @param p the first parameter of the new torus link; this must be
         * non-negative.
         * @param q the second parameter of the new torus link; this must
         * also be non-negative.
         * @param positive \c true if the crossings in the new torus link
         * should be positive, or \c false if they should be negative.
         */
        void insertTorusLink(int p, int q, bool positive = true);

        /**
         * Creates a link from hard-coded information about its crossings
         * and components.
         *
         * This routine takes a series of C++11 initialiser lists
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
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \note If you have an existing link that you would like to
         * hard-code, the routine dumpConstruction() will output C++ code
         * that can reconstruct the link by calling this routine.
         *
         * \exception InvalidArgument a link could not be reconstructed from
         * the given data.
         *
         * \ifacespython Not available, but there is a variant of fromData()
         * that takes the same data using two Python lists (which need not
         * be constant).
         *
         * @param crossingSigns a list containing the signs of the
         * crossings; each sign must be either +1 or -1.
         * @param components one list for each link component that
         * describes the crossings that are visited along that component,
         * as described in the detailed notes above.
         * @return the reconstructed link.
         */
        template <typename... Args>
        static Link fromData(std::initializer_list<int> crossingSigns,
            std::initializer_list<Args>... components);

        /**
         * Creates a new link from information about its crossings and
         * components.
         *
         * This routine is an analogue to the variant of fromData() that
         * takes C++11 initialiser lists; however, here the input data may be
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
         *   std::list<int> are be fine).
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
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument a link could not be reconstructed from
         * the given data.
         *
         * \ifacespython The signs should be passed as a single Python list of
         * integers (not an iterator pair).  Likewise, the components should be
         * passed as a Python list of lists of integers (not an iterator pair).
         * In the case of a knot (which has only one component), you are
         * welcome to replace the list of lists <tt>[[...]]</tt> with a
         * single list <tt>[...]</tt>.
         *
         * @param beginSigns the beginning of the list of crossing signs.
         * @param endSigns a past-the-end iterator indicating the end of
         * the list of crossing signs.
         * @param beginComponents the beginning of the list of containers
         * describing each link component.
         * @param endComponents a past-the-end iterator indicating the
         * end of the list of link components.
         * @return the reconstructed link.
         */
        template <typename SignIterator, typename ComponentIterator>
        static Link fromData(SignIterator beginSigns, SignIterator endSigns,
            ComponentIterator beginComponents, ComponentIterator endComponents);

        /**
         * Recovers a knot diagram from its signature.
         * See knotSig() for more information on knot signatures.
         *
         * Calling knotSig() followed by fromKnotSig() is not guaranteed to
         * produce an \e identical knot diagram to the original, but it
         * is guaranteed to produce one that is related by relabelling,
         * rotation, and optionally (according to the arguments that
         * were passed to knotSig()) reflection and/or reversal.
         *
         * \exception InvalidArgument the given string was not a valid
         * knot signature.
         *
         * @param sig the signature of the knot diagram to construct.
         * Note that signatures are case-sensitive.
         * @return the reconstructed knot.
         */
        static Link fromKnotSig(const std::string& sig);

        /**
         * Alias for fromKnotSig(), to recover a knot diagram from its
         * signature.
         *
         * This alias fromSig() is provided to assist with generic code
         * that can work with both knots and triangulations.
         *
         * See fromKnotSig() for further details.
         *
         * \exception InvalidArgument the given string was not a valid
         * knot signature.
         *
         * @param sig the signature of the knot diagram to construct.
         * Note that signatures are case-sensitive.
         * @return the reconstructed knot.
         */
        static Link fromSig(const std::string& sig);

        /**
         * Creates a new knot from a classical Gauss code, presented as
         * a string.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.  As
         * a result, there can be topological ambiguities when a knot is
         * reconstructed from a gauss code; these are described in the
         * warnings below.
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
         * - This can only be done for knots (i.e., links with exactly one
         *   component).
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   (i.e., they cannot be arbitrary natural numbers with "gaps",
         *   and the numbering cannot use a different starting point).
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
         * enough information to uniquely reconstruct a knot.  For prime knots,
         * both a knot and its reflection can be described by the same Gauss
         * code; for composite knots, the same Gauss code can describe
         * knots that are topologically inequivalent, even when allowing for
         * reflection.  If you need to reconstruct a knot uniquely, consider
         * using the \e oriented Gauss code instead.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given string was not a valid
         * classical Gauss code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * @author Adam Gowty
         *
         * @param str a classical Gauss code for a knot, as described above.
         * @return the reconstructed knot.
         */
        static Link fromGauss(const std::string& str);

        /**
         * Creates a new knot from a classical Gauss code, presented as
         * an integer sequence.
         *
         * See gauss() for a full description of classical Gauss codes as
         * they are used in Regina, and see fromGauss(const std::string&)
         * for a detailed discussion of how Regina reconstructs knots
         * from such codes.
         *
         * This routine is a variant of fromGauss(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of integers.  This sequence is given by passing a
         * pair of begin/end iterators.
         *
         * \pre \a Iterator is a random access iterator type, and
         * dereferencing such an iterator produces an integer.
         *
         * \warning In general, the classical Gauss code does not contain
         * enough information to uniquely reconstruct a knot.  For prime knots,
         * both a knot and its reflection can be described by the same Gauss
         * code; for composite knots, the same Gauss code can describe
         * knots that are topologically inequivalent, even when allowing for
         * reflection.  If you need to reconstruct a knot uniquely, consider
         * using the \e oriented Gauss code instead.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given sequence was not a valid
         * classical Gauss code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * @author Adam Gowty
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of integers for a classical Gauss code.
         * @param end an iterator that points past the end of the
         * sequence of integers for a classical Gauss code.
         * @return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromGauss(Iterator begin, Iterator end);

        /**
         * Creates a new knot from an "oriented" variant of the Gauss code,
         * presented as string.
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
         * represent).
         *
         * Regina imposes the following restrictions when reconstructing
         * a knot from an oriented Gauss code:
         *
         * - This can only be done for knots (i.e., links with exactly one
         *   component).
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   (i.e., they cannot be arbitrary natural numbers with "gaps",
         *   and the numbering cannot use a different starting point).
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
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given string was not a valid
         * oriented Gauss code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * @param str an "oriented" Gauss code for a knot, as described above.
         * @return the reconstructed knot.
         */
        static Link fromOrientedGauss(const std::string& str);

        /**
         * Creates a new knot from an "oriented" variant of the Gauss code,
         * presented as a sequence of string tokens.
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
         * the form <tt>+&lt;<i>k</i></tt>, <tt>-&lt;<i>k</i></tt>,
         * <tt>+&gt;<i>k</i></tt> or <tt>-&gt;<i>k</i></tt> that would normally
         * be joined with whitespace to form a complete oriented Gauss code.
         * For example, to describe the left-hand trefoil, the input sequence
         * could be a vector containing the six tokens:
         *
           \verbatim
           { "+>1", "-<2", "+>3", "-<1", "+>2", "-<3" }
           \endverbatim
         *
         * Each individual token should \e not contain any whitespace;
         * otherwise this routine may fail to parse the token(s) and
         * could return \c null as a result.
         *
         * \pre \a Iterator is a random access iterator type.
         *
         * \pre Dereferencing such an iterator produces either a
         * C-style string (which can be cast to <tt>const char*</tt>) or a
         * C++-style string (which can be cast to <tt>const std::string&</tt>).
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given sequence was not a valid
         * oriented Gauss code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of tokens for an "oriented" Gauss code.
         * @param end an iterator that points past the end of the
         * sequence of tokens for an "oriented" Gauss code.
         * @return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromOrientedGauss(Iterator begin, Iterator end);

        /**
         * Creates a new link from Bob Jenkins' format, presented as a string.
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
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given string was not a valid
         * encoding of a link in Jenkins' format.  As noted above, the
         * checks performed here are not exhaustive.
         *
         * @param str a string describing a link in Jenkins' format,
         * as described above.
         * @return the reconstructed link.
         */
        static Link fromJenkins(const std::string& str);

        /**
         * Creates a new link from Bob Jenkins' format, read directly
         * from an input stream.
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
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given input was not a valid
         * encoding of a link in Jenkins' format.  As noted above, the
         * checks performed here are not exhaustive.
         *
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant fromJenkins(const std::string&),
         * which takes the input as a string.
         *
         * @param in an input stream that begins with a sequence of integers
         * separated by whitespace that describes a link.
         * @return the reconstructed link.
         */
        static Link fromJenkins(std::istream& in);

        /**
         * Creates a new link from Bob Jenkins' format, presented as an
         * integer sequence.
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
         * dereferencing such an iterator produces an integer.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given sequence was not a valid
         * encoding of a link in Jenkins' format.  As noted above, the
         * checks performed here are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of integers that describes a link.
         * @param end an iterator that points past the end of the
         * sequence of integers that describes a link.
         * @return the reconstructed link.
         */
        template <typename Iterator>
        static Link fromJenkins(Iterator begin, Iterator end);

        /**
         * Creates a new knot from either alphabetical or numerical
         * Dowker-Thistlethwaite notation, presented as a string.
         *
         * Dowker-Thistlethwaite notation essentially describes the 4-valent
         * graph of a knot but not the particular embedding in the plane.
         * As a result, there can be topological ambiguities when a knot is
         * reconstructed from Dowker-Thistlethwaite notation; these are
         * described in the warnings below.
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
         * enough information to uniquely reconstruct a knot.  For prime knots,
         * both a knot and its reflection can be described by the same notation;
         * for composite knots, the same notation can describe knots that are
         * topologically inequivalent, even when allowing for reflection.
         * If you need to reconstruct a knot uniquely, consider
         * using the oriented Gauss code instead.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given string was not a valid
         * Dowker-Thistlethwaite code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * @author Much of the code for this routine is based on the
         * Dowker-Thistlethwaite implementation in the SnapPea/SnapPy kernel.
         *
         * @param str either the alphabetical or numerical
         * Dowker-Thistlethwaite notation for a knot, as described above.
         * @return the reconstructed knot.
         */
        static Link fromDT(const std::string& str);

        /**
         * Creates a new knot from numerical Dowker-Thistlethwaite notation,
         * presented as an integer sequence.
         *
         * See dt(bool) for a full description of Dowker-Thistlethwaite
         * notation as it is used in Regina, and see fromDT(const std::string&)
         * for a detailed discussion of how Regina reconstructs knots
         * from such notation.
         *
         * This routine is a variant of fromDT(const std::string&) which,
         * instead of taking a human-readable string, takes a machine-readable
         * sequence of integers.  This sequence is given by passing a
         * pair of begin/end iterators.
         *
         * This variant of fromDT() can only work with \e numerical
         * Dowker-Thistlethwaite notation.  Regina does understand alphabetic
         * Dowker-Thistlethwaite notation, but for this you will need to use
         * the string-based variant of fromDT().
         *
         * \pre \a Iterator is a random access iterator type, and
         * dereferencing such an iterator produces an integer.
         *
         * \warning In general, Dowker-Thistlethwaite notation does not contain
         * enough information to uniquely reconstruct a knot.  For prime knots,
         * both a knot and its reflection can be described by the same notation;
         * for composite knots, the same notation can describe knots that are
         * topologically inequivalent, even when allowing for reflection.
         * If you need to reconstruct a knot uniquely, consider
         * using the oriented Gauss code instead.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given sequence was not a valid
         * Dowker-Thistlethwaite code for a knot.  As noted above, the checks
         * performed here are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of integers.
         *
         * @author Much of the code for this routine is based on the
         * Dowker-Thistlethwaite implementation in the SnapPea/SnapPy kernel.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of integers for the Dowker-Thistlethwaite notation
         * for a knot.
         * @param end an iterator that points past the end of the
         * sequence of integers for the Dowker-Thistlethwaite notation
         * for a knot.
         * @return the reconstructed knot.
         */
        template <typename Iterator>
        static Link fromDT(Iterator begin, Iterator end);

        /**
         * Creates a new link from a planar diagram code, presented as a string.
         *
         * Planar diagram codes overcome the limitations of classical Gauss
         * codes by encoding the local information at each crossing, though
         * they do introduce their own (less severe) ambiguities and
         * computational difficulties, as described in the warnings below.
         * They can work with links as well as knots, though they cannot
         * encode zero-crossing unknot components.
         *
         * A planar diagram code for an <i>n</i>-crossing link is formed
         * from a sequence of <i>n</i> 4-tuples of integers.  An example,
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
         *   1, 2, ..., 2<i>n</i>.  That is, they cannot be arbitrary natural
         *   numbers with "gaps", and the numbering of strands cannot use a
         *   different starting point.
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
         * Thus the follow strings all describe the same sequence:
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
         * tuples.  Regina does \e not attribute any meaning to these symbols,
         * and will treat them as nothing more than separators.
         *
         * \warning If the link contains an unknotted loop that sits
         * completely above all other link components (in other words,
         * a link components that consists entirely of over-crossings), then
         * the orientation of this loop might not be reconstructed correctly.
         * This is unavoidable: the planar diagram code simply does not
         * contain this information.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given string was not a valid
         * planar diagram code.  As noted above, the checks performed here
         * are not exhaustive.
         *
         * @param str a planar diagram code for a link, as described above.
         * @return the reconstructed link.
         */
        static Link fromPD(const std::string& str);

        /**
         * Creates a new link from a planar diagram code, presented as a
         * sequence of 4-tuples.
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
         * \pre If \a it is such an iterator, then <tt>(*it)[0]</tt>,
         * <tt>(*it)[1]</tt>, <tt>(*it)[2]</tt> and <tt>(*it)[3]</tt>
         * will give the elements of the corresponding 4-tuple, which
         * can then be treated as native C++ integers.
         *
         * \warning If the link contains an unknotted loop that sits
         * completely above all other link components (in other words,
         * a link components that consists entire of over-crossings), then
         * the orientation of this loop might not be reconstructed correctly.
         * This is unavoidable: the planar diagram code simply does not
         * contain this information.
         *
         * \warning While this routine does some error checking on the
         * input, these checks are not exhaustive.  In particular,
         * it does \e not test for planarity of the diagram.
         * That is, if the input describes a link diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument the given sequence was not a valid
         * planar diagram code.  As noted above, the checks performed here
         * are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list.  Each element
         * of the list should be convertible to a tuple of integers.
         * In particular, a list of Python lists is fine, and a list of
         * Python tuples is fine also.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of 4-tuples for a planar diagram code.
         * @param end an iterator that points past the end of the
         * sequence of 4-tuples for a planar diagram code.
         * @return the reconstructed link.
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
         * Internal to fromOrientedGauss().
         *
         * This routine parses a single token in an "oriented" Gauss code.
         * The token must not contain any whitespace.
         *
         * @param s the token to parse.
         * @param nCross the total number of crossings in the knot.
         * @param crossing used to return the label of the crossing
         * described by this token.
         * @param strand used to return either 1 or 0 according to
         * whether this token describes the upper or lower strand of the
         * crossing respectively.
         * @param sign used to return the sign of this crossing (either
         * +1 or -1).
         * @return \c true if and only if no errors were detected when
         * parsing the token.
         */
        static bool parseOrientedGaussTerm(const std::string& s,
            size_t nCross, size_t& crossing, int& strand, int& sign);
        /**
         * Internal to fromOrientedGauss().
         *
         * This routine parses a single token in an "oriented" Gauss code.
         * The token must not contain any whitespace.
         *
         * @param s the token to parse.
         * @param nCross the total number of crossings in the knot.
         * @param crossing used to return the label of the crossing
         * described by this token.
         * @param strand used to return either 1 or 0 according to
         * whether this token describes the upper or lower strand of the
         * crossing respectively.
         * @param sign used to return the sign of this crossing (either
         * +1 or -1).
         * @return \c true if and only if no errors were detected when
         * parsing the token.
         */
        static bool parseOrientedGaussTerm(const char* s,
            size_t nCross, size_t& crossing, int& strand, int& sign);

        /**
         * Internal to fromDT().
         *
         * This routine attempts to deduce the orientation of each crossing
         * in order to make a planar embedding of the 4-valent graph
         * described by the Dowker-Thistlethwaite notation.
         *
         * See the source code for further documentation.
         *
         * @return \c true if and only if a planar embedding was found.
         *
         * @author This routine is based on the Dowker-Thistlethwaite
         * implementation from the SnapPea/SnapPy kernel.
         */
        static bool realizeDT(int* anInvolution, bool* aRealization,
            int aNumCrossings);

        /**
         * Internal to fromData().
         *
         * This routine processes one link component, and then recursively
         * calls itself to process the remaining components.
         *
         * \exception InvalidArgument an error or inconsistency is found in
         * the given data.
         *
         * @param strandsRemaining the expected sum of the lengths of
         * all components that still need to be processed.  This expected
         * sum is computed as twice the number of crossings minus the
         * lengths of all components that have already been processed.
         * In an error-free scenario, \a strandsRemaining should equal
         * the sum of the lengths of the lists passed in \a component and
         * \a otherComponents.
         * @param component the component to process now.
         * @param otherComponents the remaining components still to be
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
         * \exception InvalidArgument an error or inconsistency was found in
         * the given data (which for this terminating call simply means
         * \a strandsRemaining != 0).
         *
         * @param strandsRemaining the expected sum of the lengths of
         * all components that still need to be processed.  This expected
         * sum is computed as twice the number of crossings minus the
         * lengths of all components that have already been processed.
         */
        void addComponents(size_t strandsRemaining);

        /**
         * Internal to bracketNaive().
         *
         * Returns the number of loops in the link produced by resolving
         * each crossing according to the given bitmask:
         *
         * - If the <i>i</i>th bit in \a mask is 0, this indicates that
         *   crossing \a i should be resolved by turning \e left when
         *   entering along the upper strand.
         *
         * - If the <i>i</i>th bit in \a mask is 1, this indicates that
         *   crossing \a i should be resolved by turning \e right when
         *   entering along the upper strand.
         *
         * If the array \a loopIDs is non-null, then it will be filled
         * with an identifier for each loop.  Each identifier will be
         * the minimum of the following values that are computed as you
         * follow the loop: when passing through crossing \a i, if we
         * encounter the half of the upper strand that \e exits the crossing
         * then we take the value \a i, and if we encounter the half of
         * the upper strand that \e enters the crossing then we take the
         * value (\a i + \a n).  These identifiers will be returned in the
         * array \a loopIDs in sorted order.
         *
         * If the array \a loopLengths is non-null, then it will be
         * filled with the number of strands in each loop (so these
         * should sum to twice the number of crossings).  These loop
         * lengths will be placed in the array in the same order as the
         * loop IDs as described above.
         *
         * \pre The number of crossings is at most the number of bits in
         * an unsigned long.
         *
         * \pre If either or both the arrays \a loopIDs and \a loopLengths
         * are not null, then they are arrays whose size is at least the
         * return value (i.e., the number of loops).  This typically means
         * that the caller must put an upper bound on the number of loops
         * in advance, before calling this routine.
         *
         * @return the resulting number of loops after all crossings are
         * resolved.
         */
        size_t resolutionLoops(unsigned long mask, size_t* loopIDs = nullptr,
            size_t* loopLengths = nullptr) const;

        /**
         * Compute the Kauffman bracket polynomial using a naive
         * algorithm that sums over all resolutions of all crossings.
         *
         * The given progress tracker may be \c null.
         * This routine does \e not mark the tracker as finished.
         *
         * See bracket() for further details.
         */
        Laurent<Integer> bracketNaive(ProgressTracker* tracker) const;

        /**
         * Compute the Kauffman bracket polynomial using a fixed-parameter
         * tractable algorithm based on a tree decomposition.
         *
         * The given progress tracker may be \c null.
         * This routine does \e not mark the tracker as finished.
         *
         * See bracket() for further details.
         */
        Laurent<Integer> bracketTreewidth(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z, using Kauffman's skein-template algorithm.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         */
        Laurent2<Integer> homflyKauffman(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z, using a fixed-parameter tractable algorithm
         * based on a tree decomposition.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         */
        Laurent2<Integer> homflyTreewidth(ProgressTracker* tracker) const;

        /**
         * Optimises the given tree decomposition for computing the
         * Jones polynomial.
         *
         * This optimisation may involve compressing and/or rerooting the
         * given tree decomposition.  The aim is to minimise the estimated
         * processing time and memory consumption of calling
         * <tt>jones(ALG_TREEWIDTH)</tt> and/or <tt>bracket(ALG_TREEWIDTH)</tt>.
         *
         * The rerooting procedure essentially estimates the number of
         * partial solutions that are expected at each bag in the
         * treewidth-based dynamic programming algorithm, and chooses a
         * root that minimises the maximum such estimate over all bags.
         *
         * @param td the tree decomposition to optimise.
         */
        void optimiseForJones(TreeDecomposition& td) const;

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
         * @param bracket the Kauffman bracket polynomial of this link.
         */
        void setPropertiesFromBracket(Laurent<Integer>&& bracket) const;

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
 * \note This swap function is \e not marked \c noexcept, since it
 * fires change events on both links which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * @param lhs the link whose contents should be swapped with \a rhs.
 * @param rhs the link whose contents should be swapped with \a lhs.
 *
 * \ingroup link
 */
void swap(Link& lhs, Link& rhs);

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline void Link::clearAllProperties() {
    jones_.reset();
    homflyAZ_.reset();
    homflyLM_.reset();
    bracket_.reset();
    niceTreeDecomposition_.reset();
}

inline Link::~Link() {
    clearAllProperties();
    for (Crossing* c : crossings_)
        delete c;
}

inline int Crossing::index() const {
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

inline int StrandRef::id() const {
    return (crossing_ ? ((crossing()->index() << 1) | strand_) : -1);
}

inline bool StrandRef::operator == (const StrandRef& rhs) const {
    return (crossing_ == rhs.crossing_ && strand_ == rhs.strand_);
}

inline bool StrandRef::operator != (const StrandRef& rhs) const {
    return (crossing_ != rhs.crossing_ || strand_ != rhs.strand_);
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

inline Link::Link(size_t unknots) {
    components_.resize(unknots);
    std::fill(components_.begin(), components_.end(), StrandRef());
}

inline Link::Link(const Link& cloneMe) : Link(cloneMe, true) {
}

inline void Link::swapContents(Link& other) {
    swap(other);
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

inline StrandRef Link::strand(int id) const {
    return (id >= 0 ? StrandRef(crossings_[id >> 1]->strand(id & 1)) :
        StrandRef());
}

inline bool Link::operator != (const Link& other) const {
    return ! ((*this) == other);
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

inline const Laurent2<Integer>& Link::homfly(Algorithm alg,
        ProgressTracker* tracker) const {
    return homflyAZ(alg, tracker);
}

inline bool Link::knowsBracket() const {
    return bracket_.has_value();
}
inline bool Link::knowsJones() const {
    return jones_.has_value();
}
inline bool Link::knowsHomfly() const {
    // Either both homflyAZ_ and homflyLM_ are known, or neither are known.
    return homflyAZ_.has_value();
}
inline bool Link::r2(Crossing* crossing, bool check, bool perform) {
    return r2(StrandRef(crossing, 1), check, perform);
}

inline bool Link::r3(Crossing* crossing, int side, bool check, bool perform) {
    StrandRef s(crossing, 1);

    // If we are testing, then make sure this is an uppermost arc.
    if (check) {
        // We already know the start of the arc is an over-crossing, by
        // construction; we only need to check the end.
        if ((! crossing) || (s.next().strand() != 1))
            return false;
    }

    return r3(s, side, check, perform);
}

inline const TreeDecomposition& Link::niceTreeDecomposition() const {
    if (niceTreeDecomposition_)
        return *niceTreeDecomposition_;

    TreeDecomposition ans(*this, TD_UPPER);
    prepareTreeDecomposition(ans);
    niceTreeDecomposition_ = ans;

    return *niceTreeDecomposition_;
}

inline void Link::useTreeDecomposition(TreeDecomposition td) {
    prepareTreeDecomposition(td);
    niceTreeDecomposition_ = std::move(td);
}

inline StrandRef Link::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(nullptr, other.strand()));
}

template <typename Action, typename... Args>
inline bool Link::rewrite(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    if (countComponents() != 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "rewrite() requires a link with exactly one component");
    }

    // Use RetriangulateActionTraits to deduce whether the given action takes
    // a link or both a knot signature and link as its initial argument(s).
    using Traits = regina::detail::RetriangulateActionTraits<Link, Action>;
    static_assert(Traits::valid,
        "The action that is passed to rewrite() does not take the correct initial argument type(s).");
    if constexpr (Traits::withSig) {
        return regina::detail::retriangulateInternal<Link, true>(
            *this, height, nThreads, tracker,
            [&](const std::string& sig, Link&& obj) {
                return action(sig, std::move(obj), std::forward<Args>(args)...);
            });
    } else {
        return regina::detail::retriangulateInternal<Link, false>(
            *this, height, nThreads, tracker,
            [&](Link&& obj) {
                return action(std::move(obj), std::forward<Args>(args)...);
            });
    }
}

inline bool Link::simplifyExhaustive(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker) {
    if (isEmpty()) {
        if (tracker)
            tracker->setFinished();
        return false;
    }
    return rewrite(height, nThreads, tracker,
        [](Link&& alt, Link& original, size_t minCrossings) {
            if (alt.size() < minCrossings) {
                ChangeEventSpan span(original);
                original = std::move(alt);
                original.intelligentSimplify();
                return true;
            } else
                return false;
        }, *this, size());
}

inline void Link::join(const StrandRef& s, const StrandRef& t) {
    s.crossing_->next_[s.strand_] = t;
    t.crossing_->prev_[t.strand_] = s;
}

inline Link Link::fromSig(const std::string& sig) {
    return Link::fromKnotSig(sig);
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

