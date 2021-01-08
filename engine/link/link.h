
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

#ifndef __LINK_H
#ifndef __DOXYGEN
#define __LINK_H
#endif

#include <functional>
#include <vector>
#include "regina-core.h"
#include "maths/integer.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "packet/packet.h"
#include "progress/progresstracker.h"
#include "treewidth/treedecomposition.h"
#include "utilities/markedvector.h"
#include "utilities/property.h"

namespace regina {

/**
 * \addtogroup link Knots and Links
 * Knots and links in the 3-sphere
 * @{
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
 * These references are small enough to pass around by value.
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
 */
class REGINA_API StrandRef {
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
 */
class REGINA_API Crossing : public MarkedElement, public Output<Crossing> {
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
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

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

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_LINK> {
    typedef Link Class;
    inline static const char* name() {
        return "Link";
    }
};
#endif

/**
 * Represents a directed knot or link in the 3-sphere.
 *
 * This class supports links with any number of components (including zero),
 * and it also supports components with no crossings (which form additional
 * unknot components of the overall link).
 */
class REGINA_API Link : public Packet {
    REGINA_PACKET(Link, PACKET_LINK)

    private:
        MarkedVector<Crossing> crossings_;
            /**< The crossings in this link. */
        std::vector<StrandRef> components_;
            /**< For each component of the link, this array contains
                 a reference to one of its strands (which can then be
                 used to traverse the component).  If a component has no
                 crossings, then it is represented in this array by a
                 null reference. */

        mutable Property<Laurent<Integer>, StoreManagedPtr> jones_;
            /**< The Jones polynomial of the link. */
        mutable Property<Laurent2<Integer>, StoreManagedPtr> homflyLM_;
            /**< The HOMFLY polynomial of the link, as a polynomial in
                 \a l and \a m.  This property will be known if and only
                 if \a homflyAZ_ is known. */
        mutable Property<Laurent2<Integer>, StoreManagedPtr> homflyAZ_;
            /**< The HOMFLY polynomial of the link, as a polynomial in
                 \a alpha and \a z.  This property will be known if and
                 only if \a homflyLM_ is known. */
        mutable Property<Laurent<Integer>, StoreManagedPtr> bracket_;
            /**< The Kauffman bracket polynomial of the link diagram. */

        mutable Property<TreeDecomposition, StoreManagedPtr>
                niceTreeDecomposition_;
            /**< A nice tree decomposition of the planar 4-valent multigraph
                 formed by the link diagram. */

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
        static const char* jonesVar;

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
        static const char* homflyAZVarX;

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
        static const char* homflyAZVarY;

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
        static const char* homflyLMVarX;

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
        static const char* homflyLMVarY;

        /**
         * The name of the first variable used in the variant of the
         * HOMFLY polynomial as returned by homfly().
         *
         * This is simply an alias for homflyAZVarX.  See the documentation
         * for homflyAZVarX for further details.
         */
        static const char* homflyVarX;

        /**
         * The name of the second variable used in the variant of the
         * HOMFLY polynomial as returned by homfly().
         *
         * This is simply an alias for homflyAZVarY.  See the documentation
         * for homflyAZVarY for further details.
         */
        static const char* homflyVarY;

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
         * @param unknots the number of (unknotted) components in the
         * new unlink.
         */
        Link(size_t unknots);
        /**
         * Constructs a new copy of the given link.
         * The packet tree structure and packet label are \e not copied.
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
         *   used by fromDT().
         *
         * This list may grow in future versions of Regina.
         *
         * Regina will also set the packet label accordingly.
         *
         * If Regina cannot interpret the given string, this will be
         * left as the empty link.
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

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

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
         * @param other the link whose contents should be swapped with this.
         */
        void swapContents(Link& other);

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
         * routine will return immediately (as described below).
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
         * If a progress tracker is passed, then the exhaustive simplification
         * will take place in a new thread and this routine will return
         * immediately.  In this case, you will need to use some other
         * means to determine whether the knot diagram was eventually
         * simplified (e.g., by examining size() after the tracker
         * indicates that the operation has finished).
         *
         * To assist with performance, this routine can run in parallel
         * (multithreaded) mode; simply pass the number of parallel threads
         * in the argument \a nThreads.  Even in multithreaded mode, if no
         * progress tracker is passed then this routine will not return until
         * processing has finished (i.e., either the diagram was
         * simplified or the search was exhausted).
         *
         * If this routine is unable to simplify the knot diagram, then
         * this knot diagram will not be changed.
         *
         * If this link does not have precisely one component, then this
         * routine will do nothing.  If no progress tracker was passed then
         * it will immediately return \c false; otherwise the progress tracker
         * will immediately be marked as finished.
         *
         * @param height the maximum number of \e additional crossings to
         * allow beyond the number of crossings originally present in this
         * diagram, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return If a progress tracker is passed, then this routine
         * will return \c true or \c false immediately according to
         * whether a new thread could or could not be started.  If no
         * progress tracker is passed, then this routine will return \c true
         * if and only if this diagram was successfully simplified to
         * fewer crossings.
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
         * routine will return immediately (as described below).
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
         * - \a action must take at least one argument.  The first argument
         *   will be of type Link&, and will reference the
         *   knot diagram that has been found.  If there are any
         *   additional arguments supplied in the list \a args, then
         *   these will be passed as subsequent arguments to \a action.
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
         * - \a action may, if it chooses, make changes to the knot
         *   that is passed in its argument (though it must not delete it).
         *   This will likewise not affect the search, since the knot diagram
         *   that is passed to \a action will be destroyed immediately after
         *   \a action is called.
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
         * If a progress tracker is passed, then the exploration of
         * knot diagrams will take place in a new thread and this
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
         * If this link does not have precisely one component, then this
         * routine will do nothing.  If no progress tracker was passed then
         * it will immediately return \c false; otherwise the progress tracker
         * will immediately be marked as finished.
         *
         * \warning By default, the arguments \a args will be copied (or moved)
         * when they are passed to \a action.  If you need to pass some
         * argument(s) by reference, you must wrap them in std::ref or
         * std::cref.
         *
         * \apinotfinal
         *
         * \ifacespython Not present.
         *
         * @param height the maximum number of \e additional crossings to
         * allow beyond the number of crossings originally present in this
         * knot diagram, or a negative number if this should not be bounded.
         * @param nThreads the number of threads to use.  If this is
         * 1 or smaller then the routine will run single-threaded.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @param action a function (or other callable object) to call
         * upon each knot diagram that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial knot argument.
         * @return If a progress tracker is passed, then this routine
         * will return \c true or \c false immediately according to
         * whether a new thread could or could not be started.  If no
         * progress tracker is passed, then this routine will return \c true
         * if some call to \a action returned \c true (thereby terminating
         * the search early), or \c false if the search ran to completion.
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
         * Returns an ideal triangulation of the complement of this link
         * in the 3-sphere.
         *
         * The triangulation will have one ideal vertex for each link
         * component.  Assuming you pass \a simplify as \c true (the default),
         * there will typically be no internal vertices; however, this
         * is not guaranteed.
         *
         * Initially, the triangulation will be oriented.  In particular,
         * each tetrahedron will be oriented according to a right-hand rule:
         * the thumb of the right hand points from vertices 0 to 1, and
         * the fingers curl around to point from vertices 2 to 3.
         *
         * What happens next depends upon the argument \a simplify:
         *
         * - If you pass \a simplify as \c true, then Regina will attempt
         *   to simplify the triangulation to as few tetrahedra as possible.
         *   As a result, the orientation described above will be lost.
         *
         * - If you pass \a simplify as \c false, then Regina will leave the
         *   triangulation as is.  This will preserve the orientation, but
         *   it means that the triangulation will contain both ideal and
         *   internal vertices (and, in general, far more tetrahedra
         *   than are necessary).
         *
         * The triangulation will be newly created, and it is the
         * responsibility of the caller of this routine to destroy it.
         *
         * @param simplify \c true if and only if the triangulation of
         * the complement should be simplified (thereby losing information
         * about the orientation), as described above.
         * @return the complement of this link, as a newly-created object.
         */
        Triangulation<3>* complement(bool simplify = true) const;

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
         * The result will returned as a new link, and it is the
         * responsibility of the caller of this routine to destroy it.
         *
         * @param k the number of parallel copies to create.
         * This must be non-negative.
         * @param framing the framing under which these copies will be parallel.
         * @return \a k parallel copies of this link, as a newly-created object.
         */
        Link* parallel(int k, Framing framing = FRAMING_SEIFERT) const;

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
         * This (potentially) long computation can be managed by passing
         * a progress tracker:
         *
         * - If a progress tracker is passed and the polynomial has not yet
         *   been computed, then the calculation will take place in a
         *   new thread and this routine will return immediately.  Once the
         *   progress tracker indicates that the calculation has finished,
         *   you can call bracket() again to retrieve the polynomial.
         *
         * - If no progress tracker is passed and the polynomial has
         *   not yet been computed, the calculation will run in the current
         *   thread and this routine will not return until it is complete.
         *
         * - If the requested invariant has already been computed, then this
         *   routine will return immediately with the pre-computed value.  If
         *   a progress tracker is passed then it will be marked as finished.
         *
         * \warning The naive algorithm can only handle a limited number
         * of crossings (currently less than the number of bits in a long,
         * which on a typical machine is 64).  If you pass ALG_NAIVE and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
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
         * @return the bracket polynomial.  If a progress tracker was passed
         * then this return value must be ignored, and you should call
         * bracket() again once the tracker is marked as finished.
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
         * This (potentially) long computation can be managed by passing
         * a progress tracker:
         *
         * - If a progress tracker is passed and the polynomial has not yet
         *   been computed, then the calculation will take place in a
         *   new thread and this routine will return immediately.  Once the
         *   progress tracker indicates that the calculation has finished,
         *   you can call bracket() again to retrieve the polynomial.
         *
         * - If no progress tracker is passed and the polynomial has
         *   not yet been computed, the calculation will run in the current
         *   thread and this routine will not return until it is complete.
         *
         * - If the requested invariant has already been computed, then this
         *   routine will return immediately with the pre-computed value.  If
         *   a progress tracker is passed then it will be marked as finished.
         *
         * \warning The naive algorithm can only handle a limited number
         * of crossings (currently less than the number of bits in a long,
         * which on a typical machine is 64).  If you pass ALG_NAIVE and
         * you have too many crossings (which is not advised, since the
         * naive algorithm requires 2^<i>n</i> time), then this routine
         * will ignore your choice of algorithm and use the treewidth-based
         * algorithm regardless.
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
         * @return the Jones polynomial.  If a progress tracker was passed
         * then this return value must be ignored, and you should call
         * jones() again once the tracker is marked as finished.
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
         * If the HOMFLY polynomial has already been computed, then the result
         * will be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         * This (potentially) long computation can be managed by passing
         * a progress tracker:
         *
         * - If a progress tracker is passed and the polynomial has not yet
         *   been computed, then the calculation will take place in a
         *   new thread and this routine will return immediately.  Once the
         *   progress tracker indicates that the calculation has finished,
         *   you can call homflyAZ() again to retrieve the polynomial.
         *
         * - If no progress tracker is passed and the polynomial has
         *   not yet been computed, the calculation will run in the current
         *   thread and this routine will not return until it is complete.
         *
         * - If the HOMFLY polynomial has already been computed (either in
         *   terms of \a alpha and \a z or in terms of \a l and \a m), then
         *   this routine will return immediately with the pre-computed value.
         *   If a progress tracker is passed then it will be marked as finished.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial.  If a progress tracker was passed
         * then this return value must be ignored, and you should call
         * homflyAZ() again once the tracker is marked as finished.
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
         * If the HOMFLY polynomial has already been computed, then the result
         * will be cached and so this routine will be very fast (since it just
         * returns the previously computed result).  Otherwise the computation
         * could be quite slow, particularly for larger numbers of crossings.
         * This (potentially) long computation can be managed by passing
         * a progress tracker:
         *
         * - If a progress tracker is passed and the polynomial has not yet
         *   been computed, then the calculation will take place in a
         *   new thread and this routine will return immediately.  Once the
         *   progress tracker indicates that the calculation has finished,
         *   you can call homflyLM() again to retrieve the polynomial.
         *
         * - If no progress tracker is passed and the polynomial has
         *   not yet been computed, the calculation will run in the current
         *   thread and this routine will not return until it is complete.
         *
         * - If the HOMFLY polynomial has already been computed (either in
         *   terms of \a alpha and \a z or in terms of \a l and \a m), then
         *   this routine will return immediately with the pre-computed value.
         *   If a progress tracker is passed then it will be marked as finished.
         *
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial.  If a progress tracker was passed
         * then this return value must be ignored, and you should call
         * homflyLM() again once the tracker is marked as finished.
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
         * @param alg the algorithm with which to compute the polynomial.
         * If you are not sure, the default (ALG_DEFAULT) is a safe choice.
         * If you wish to specify a particular algorithm, there are
         * currently two choices: ALG_BACKTRACK will use Kauffman's
         * skein-template algorithm, and ALG_TREEWIDTH will use a
         * fixed-parameter tractable treewidth-based algorithm.
         * @param tracker a progress tracker through which progress will
         * be reported, or \c null if no progress reporting is required.
         * @return the HOMFLY polynomial.  If a progress tracker was passed
         * then this return value must be ignored, and you should call
         * homfly() again once the tracker is marked as finished.
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
         * Regina will not claim ownership of \e td, and will not edit
         * it in any way.  Instead, it will make a deep copy of \e td
         * and then modify this copy for its purposes.
         *
         * In particular, \e td does not need to be a \e nice tree
         * decomposition (indeed, it does not need to have any special
         * properties beyond the definition of a tree decomposition).
         * Regina will automatically create a nice tree decomposition
         * from it if \e td is not nice already.
         *
         * @param td a tree decomposition of the planar 4-valent
         * multigraph formed by this link diagram.
         */
        void useTreeDecomposition(const TreeDecomposition& td);

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const override;
        virtual void writeTextLong(std::ostream& out) const override;
        virtual bool dependsOnParent() const override;

        /*@}*/
        /**
         * \name Exporting Links
         */
        /*@{*/

        /**
         * Outputs this link in Regina's own brief format.
         * This format is concise, but contains enough information to
         * reconstruct the link.
         *
         * This format cannot (yet) be used to read links back into Regina,
         * and so it is not good for external storage, or for passing links
         * between different programs (or even different instances of Regina).
         * It was originally designed for use with the test suite, where it
         * was used to ensure that links with being created and/or manipulated
         * correctly.
         *
         * The output will contains the following elements, separated by
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
         * the format will not begin with a space; instead it will
         * simply be a sequence of the form <tt>( ) ( ) ... ( )</tt>.
         *
         * The string will not end in a newline.
         *
         * @return a description of this link in Regina's brief format.
         */
        std::string brief() const;

        /**
         * Outputs a classical Gauss code for this knot.
         *
         * In general, the classical Gauss code does not carry enough
         * information to uniquely reconstruct the knot.  For instance,
         * both a knot and its reflection can be described by the same
         * Gauss code; moreover, for composite knots, the Gauss code can
         * describe inequivalent knots (even when allowing for reflections).
         * If you need a code that specifies the knot uniquely, consider
         * using the \e oriented Gauss code instead.
         *
         * Currently Regina only supports Gauss codes for knots,
         * not multiple-component links.  If this link does not have
         * precisely one component then the empty string will be returned.
         *
         * The string will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of returning a string, writes directly to an output stream.
         *
         * @return a classical Gauss code for this knot, or the empty
         * string if this is a link with zero or multiple components.
         */
        std::string gauss() const;

        /**
         * Writes a classical Gauss code for this knot to the given output
         * stream.
         *
         * In general, the classical Gauss code does not carry enough
         * information to uniquely reconstruct the knot.  For instance,
         * both a knot and its reflection can be described by the same
         * Gauss code; moreover, for composite knots, the Gauss code can
         * describe inequivalent knots (even when allowing for reflections).
         * If you need a code that specifies the knot uniquely, consider
         * using the \e oriented Gauss code instead.
         *
         * Currently Regina only supports Gauss codes for knots,
         * not multiple-component links.  If this link does not have
         * precisely one component then nothing will be output at all.
         *
         * The output will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of using an output stream, simply returns a string.
         *
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant gauss(), which takes no
         * arguments and returns the output as a string.
         *
         * @param out the output stream to which to write.
         */
        void gauss(std::ostream& out) const;

        /**
         * Outputs an oriented Gauss code for this knot.
         *
         * The oriented Gauss code, based on a format used by Andreeva et al.,
         * is an extension of the classical Gauss code with additional
         * characters to describe the orientation of the other strand
         * passing by at each crossing.  For details of this format, see the
         * documentation for fromOrientedGauss(const std::string&), which
         * imports links in this format.
         *
         * The key advantage of using the oriented Gauss code (as opposed to
         * the classical Gauss code) is that an oriented Gauss code always
         * describes a unique knot, and moreover (for knots that are not
         * equivalent to their reflections) it describes a unique reflection
         * of that knot.
         *
         * Currently Regina only supports Gauss codes for knots,
         * not multiple-component links.  If this link does not have
         * precisely one component then the empty string will be returned.
         *
         * The string will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of returning a string, writes directly to an output stream.
         *
         * @return an oriented Gauss code for this knot, or the empty
         * string if this is a link with zero or multiple components.
         */
        std::string orientedGauss() const;

        /**
         * Writes an oriented Gauss code for this knot to the given output
         * stream.
         *
         * The oriented Gauss code, based on a format used by Andreeva et al.,
         * is an extension of the classical Gauss code with additional
         * characters to describe the orientation of the other strand
         * passing by at each crossing.  For details of this format, see the
         * documentation for fromOrientedGauss(const std::string&), which
         * imports links in this format.
         *
         * The key advantage of using the oriented Gauss code (as opposed to
         * the classical Gauss code) is that an oriented Gauss code always
         * describes a unique knot, and moreover (for knots that are not
         * equivalent to their reflections) it describes a unique reflection
         * of that knot.
         *
         * Currently Regina only supports Gauss codes for knots,
         * not multiple-component links.  If this link does not have
         * precisely one component then nothing will be output at all.
         *
         * The output will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of using an output stream, simply returns a string.
         *
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant orientedGauss(), which takes no
         * arguments and returns the output as a string.
         *
         * @param out the output stream to which to write.
         */
        void orientedGauss(std::ostream& out) const;

        /**
         * Exports this link as a string using the text representation
         * described by Bob Jenkins.  Jenkins uses this representation in his
         * HOMFLY polynomial software, which is available online from
         * <http://burtleburtle.net/bob/knot/homfly.html>.
         *
         * Jenkins' text format uses a sequence of integers separated by
         * whitespace.  For details of this format, see the documentation for
         * fromJenkins(const std::string&), which imports links in this format.
         *
         * The string will contain multiple lines, and will end in a newline.
         *
         * \note There is another variant of this routine that, instead
         * of returning a string, writes directly to an output stream.
         *
         * @return a description of this link using Jenkins' text format.
         */
        std::string jenkins() const;

        /**
         * Exports this link to the given output stream using the text
         * representation described by Bob Jenkins.  Jenkins uses this
         * representation in his HOMFLY polynomial software, which is available
         * online from <http://burtleburtle.net/bob/knot/homfly.html>.
         *
         * Jenkins' text format uses a sequence of integers separated by
         * whitespace.  For details of this format, see the documentation
         * from fromJenkins(), which imports links using this format.
         *
         * The output will contain multiple lines, and will end in a newline.
         *
         * \note There is another variant of this routine that, instead
         * of using an output stream, simply returns a string.
         *
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant jenkins(), which takes no
         * arguments and returns the output as a string.
         *
         * @param out the output stream to which to write.
         */
        void jenkins(std::ostream& out) const;

        /**
         * Outputs this knot using Dowker-Thistlethwaite notation.
         *
         * For an <i>n</i>-crossing knot, Regina supports two variants
         * of this notation:
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
         *   larger knots this routine will return the empty string if
         *   the alphabetical variant is requested.
         *
         * In general, Dowker-Thistlethwaite notation does not carry enough
         * information to uniquely reconstruct the knot.  For instance,
         * both a knot and its reflection can be described by the same
         * sequence of integers; moreover, for composite knots, the same
         * Dowker-Thistlethwaite notation can describe inequivalent knots
         * (even when allowing for reflections).  If you need notation that
         * specifies the knot uniquely, consider using the oriented Gauss code
         * instead, as output by orientedGauss().
         *
         * Currently Regina only supports Dowker-Thistlethwaite notation for
         * knots, not multiple-component links.  If this link does not have
         * precisely one component then the empty string will be returned.
         *
         * The string will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of returning a string, writes directly to an output stream.
         *
         * @param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         * @return the Dowker-Thistlethwaite notation for this knot diagram.
         * This routine will return the empty string if this link has zero or
         * multiple components, or if \a alpha is \c true and the knot
         * has more than 26 crossings.
         */
        std::string dt(bool alpha = false) const;

        /**
         * Writes this knot to the given output stream using
         * Dowker-Thistlethwaite notation.
         *
         * For an <i>n</i>-crossing knot, Regina supports two variants
         * of this notation:
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
         *   larger knots this routine will output nothing at all if
         *   the alphabetical variant is requested.
         *
         * In general, Dowker-Thistlethwaite notation does not carry enough
         * information to uniquely reconstruct the knot.  For instance,
         * both a knot and its reflection can be described by the same
         * sequence of integers; moreover, for composite knots, the same
         * Dowker-Thistlethwaite notation can describe inequivalent knots
         * (even when allowing for reflections).  If you need notation that
         * specifies the knot uniquely, consider using the oriented Gauss code
         * instead, as output by orientedGauss().
         *
         * Currently Regina only supports Dowker-Thistlethwaite notation for
         * knots, not multiple-component links.  If this link does not have
         * precisely one component then nothing will be output at all.
         *
         * The output will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of using an output stream, simply returns a string.
         *
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant dt(), which takes just the optional
         * \a alpha argument and returns the output as a string.
         *
         * @param out the output stream to which to write.
         * @param alpha \c true to use alphabetical notation, or \c false
         * (the default) to use numerical notation.
         */
        void dt(std::ostream& out, bool alpha = false) const;

        /**
         * Outputs the underlying planar 4-valent multigraph using the
         * PACE text format.
         * The text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ .
         *
         * In summary, the output will consist of several lines of text:
         *
         * - If this link has a packet label, then the output will begin with
         *   a descriptive comment line of the form <tt>c <i>label</i></tt>.
         *   Otherwise this initial comment line will be omitted.
         *
         * - Next will be a line of the form
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
           c Figure eight knot
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
         * This code will use the Link constructor that takes a series of
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
         * have precisely one component, then this routine will return
         * the empty string.
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
         * Creates a new link from hard-coded information about its
         * crossings and components.
         *
         * This constructor takes a series of C++11 initialiser lists
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
         * The topology of the link is defined precisely by this data, but the
         * precise embedding of the diagram in the plane remains ambiguous.
         * To be exact: the embedding of the diagram in the \e 2-sphere is
         * defined precisely, but there remains a choice of which 2-cell of
         * this embedding will contain the point at infinity (i.e., which
         * 2-cell becomes the exterior cell of the diagram in the plane).
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
         * that can reconstruct the link by calling this constructor.
         *
         * \ifacespython Not available.
         *
         * @param crossingSigns a list containing the signs of the
         * crossings; each sign must be either +1 or -1.
         * @param components one list for each link component that
         * describes the crossings that are visited along that component,
         * as described in the detailed notes above.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        template <typename... Args>
        static Link* fromData(std::initializer_list<int> crossingSigns,
            std::initializer_list<Args>... components);

        /**
         * Recovers a knot diagram from its signature.
         * See knotSig() for more information on knot signatures.
         *
         * The knot that is returned will be newly created, and it is
         * the responsibility of the caller of this routine to destroy it.
         *
         * Calling knotSig() followed by fromKnotSig() is not guaranteed to
         * produce an \e identical knot diagram to the original, but it
         * is guaranteed to produce one that is related by relabelling,
         * rotation, and optionally (according to the arguments that
         * were passed to knotSig()) reflection and/or reversal.
         *
         * @param sig the signature of the knot diagram to construct.
         * Note that signatures are case-sensitive.
         * @return a newly allocated knot if the reconstruction was
         * successful, or \c null if the given string was not a valid
         * knot signature.
         */
        static Link* fromKnotSig(const std::string& sig);

        /**
         * Creates a new knot from a classical Gauss code.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.  As
         * a result, there can be ambiguity in the orientation of the
         * diagram, and (for composite knots) even the topology of the
         * knot itself.  Furthermore, parsing a Gauss code is complex
         * since it requires an embedding to be deduced using some variant
         * of a planarity testing algorithm.  These issues are resolved
         * using \e oriented Gauss codes, as used by the routines
         * orientedGauss() and fromOrientedGauss().
         *
         * The Gauss code for an <i>n</i>-crossing knot is described by
         * a sequence of 2<i>n</i> positive and negative integers,
         * representing strands that pass over and under crossings
         * respectively.  Regina's implementation of Gauss codes comes
         * with the following restrictions:
         *
         * - It can only be used for knots (i.e., links with exactly one
         *   component).
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   (i.e., they cannot have be arbitrary natural numbers).
         *
         * The format works as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Start at some point on the knot and follow it around.
         *   Whenever you pass crossing \a k, write the integer
         *   <tt><i>k</i></tt> if you pass over the crossing,
         *   or <tt>-<i>k</i></tt> if you pass under the crossing.
         *
         * Be aware that, once the knot has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * As an example, you can construct the trefoil using the code:
         *
           \verbatim
           1 -2 3 -1 2 -3
           \endverbatim
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the integers have been combined together and
         * separated by whitespace.  The other variant takes a sequence of
         * integers, defined by a pair of iterators.
         *
         * In this variant (the string variant), the given string may
         * contain additional leading or trailing whitespace.
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
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * @author Adam Gowty
         *
         * @param str a classical Gauss code for a knot, as described above.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        static Link* fromGauss(const std::string& str);

        /**
         * Creates a new knot from a classical Gauss code.
         *
         * See fromGauss(const std::string&) for a detailed
         * description of this format as it is used in Regina.
         *
         * There are two variants of this routine.  The other variant
         * (fromGauss(const std::string&), which offers more
         * detailed documentation) takes a single string, where the integers
         * have been combined together and separated by whitespace.  This
         * variant takes a sequence of integers, defined by a pair of iterators.
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
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
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
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        template <typename Iterator>
        static Link* fromGauss(Iterator begin, Iterator end);

        /**
         * Creates a new knot from an "oriented" variant of the Gauss code.
         *
         * Classical Gauss codes essentially describe the 4-valent graph
         * of a knot but not the particular embedding in the plane.  As
         * a result, there can be ambiguity in the orientation of the
         * diagram, and (for composite knots) even the topology of the
         * knot itself.  Furthermore, parsing a Gauss code is complex
         * since it requires an embedding to be deduced using some variant
         * of a planarity testing algorithm.
         *
         * Andreeva et al. describe a variant of the Gauss code that includes
         * extra information about the embedding, so as to remove both the
         * ambiguity and the complexity in the conversion procedure.
         * With this extra information, the knot and its orientation are
         * well-defined (but the diagram is still ambiguous - see the
         * note below).
         *
         * This "oriented" format is described at
         * <http://www.javaview.de/services/knots/doc/description.html#gc>.
         * Regina adds two additional restrictions on this format:
         *
         * - It can only be used for knots (i.e., links with exactly one
         *   component).
         *
         * - The crossings of the knot must be labelled 1, 2, ..., \a n
         *   (i.e., they cannot have be arbitrary natural numbers).
         *
         * The format works as follows:
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
         * Be aware that, once the knot has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * As an example, you can construct the left-hand trefoil
         * using the following code:
         *
           \verbatim
           +>1 -<2 +>3 -<1 +>2 -<3
           \endverbatim
         *
         * The topology of the knot is defined precisely by this data, but the
         * precise embedding of the diagram in the plane remains ambiguous.
         * To be exact: the embedding of the diagram in the \e 2-sphere is
         * defined precisely, but there remains a choice of which 2-cell of
         * this embedding will contain the point at infinity (i.e., which
         * 2-cell becomes the exterior cell of the diagram in the plane).
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the tokens have been combined together and
         * separated by whitespace.  The other variant takes a sequence of
         * tokens, defined by a pair of iterators.
         *
         * In this variant (the string variant), the given string may
         * contain additional leading or trailing whitespace.
         *
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * @param str an "oriented" Gauss code for a knot, as described above.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        static Link* fromOrientedGauss(const std::string& str);

        /**
         * Creates a new knot from an "oriented" variant of the Gauss code.
         *
         * This format is described by Andreeva et al. at
         * <http://www.javaview.de/services/knots/doc/description.html#gc>,
         * though Regina limits its use to knots (i.e., one-component links),
         * and insists that the crossings be numbered 1, ..., \a n (not
         * arbitrary natural numbers).
         *
         * See fromOrientedGauss(const std::string&) for a detailed
         * description of this format as it is used in Regina.
         *
         * There are two variants of this routine.  The other variant
         * (fromOrientedGauss(const std::string&), which offers more
         * detailed documentation) takes a single string, where the tokens
         * have been combined together and separated by whitespace.  This
         * variant takes a sequence of tokens, defined by a pair of iterators.
         *
         * \pre \a Iterator is a random access iterator type.
         *
         * \pre Dereferencing such an iterator produces either a
         * C-style string (which can be cast to <tt>const char*</tt>) or a
         * C++-style string (which can be cast to <tt>const std::string&</tt>).
         *
         * \pre The tokens in the input sequence do not contain any whitespace.
         *
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of tokens for an "oriented" Gauss code.
         * @param end an iterator that points past the end of the
         * sequence of tokens for an "oriented" Gauss code.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        template <typename Iterator>
        static Link* fromOrientedGauss(Iterator begin, Iterator end);

        /**
         * Builds a link from the text representation described by
         * Bob Jenkins.  Jenkins uses this representation in his
         * HOMFLY polynomial software, which is available online from
         * <http://burtleburtle.net/bob/knot/homfly.html>.
         *
         * In this format, a link is described by a sequence of integers
         * separated by whitespace - the exact form of the whitespace
         * does not matter, and additional whitespace at the beginning
         * or end of this sequence is also allowed.
         *
         * We assume that there are \a n crossings in the link, labelled
         * arbitrarily as 0, 1, ..., <i>n</i>-1.  The sequence of
         * integers must contain, in order:
         *
         * - the number of components in the link;
         *
         * - for each link component:
         *   + the number of times you pass a crossing when traversing the
         *     component (i.e., the length of the component);
         *   + two integers for each crossing that you pass in such a traversal:
         *     the crossing label, and then either +1 or -1 according to
         *     whether you pass over or under the crossing respectively;
         *
         * - for each crossing:
         *   + the crossing label;
         *   + the sign of the crossing (either +1 or -1).
         *
         * As an example, you could construct the left-hand trefoil
         * using the following sequence:
         *
           \verbatim
           1
           6   0 1   1 -1   2 1   0 -1   1 1   2 -1
           0 -1   1 -1   2 -1
           \endverbatim
         *
         * Another example is the Hopf link, which you could construct
         * using the following sequence:
         *
           \verbatim
           2
           2   0 1   1 -1
           2   0 -1   1 1
           0 1   1 1
           \endverbatim
         *
         * The topology of the knot is defined precisely by this data, but the
         * precise embedding of the diagram in the plane remains ambiguous.
         * To be exact: the embedding of the diagram in the \e 2-sphere is
         * defined precisely, but there remains a choice of which 2-cell of
         * this embedding will contain the point at infinity (i.e., which
         * 2-cell becomes the exterior cell of the diagram in the plane).
         *
         * There are two variants of this routine.  This variant takes a
         * single string containing the integer sequence.  The other
         * variant takes an input stream, from which the sequence of
         * integers will be read.
         *
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \note You can export an existing link in Jenkins' format by
         * calling the routine jenkins().
         *
         * @param str a string containing a sequence of integers
         * separated by whitespace that describes a link, as detailed above.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        static Link* fromJenkins(const std::string& str);

        /**
         * Builds a link from the text representation described by
         * Bob Jenkins.  Jenkins uses this representation in his
         * HOMFLY polynomial software, which is available online from
         * <http://burtleburtle.net/bob/knot/homfly.html>.
         *
         * See fromJenkins(const std::string&) for a detailed
         * description of this format.
         *
         * There are two variants of this routine.  The other variant takes a
         * single string containing the integer sequence.  This variant takes
         * an input stream, from which the sequence of integers will be read.
         *
         * In this variant, this routine reads the integers that describe the
         * link and then leaves the remainder of the input stream untouched
         * (in particular, the stream may contain additional material,
         * which can be read by the user after this routine has finished).
         *
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \ifacespython Not present.
         *
         * @param in an input stream that begins with a sequence of integers
         * separated by whitespace that describes a link.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        static Link* fromJenkins(std::istream& in);

        /**
         * Creates a new knot from either alphabetical or numerical
         * Dowker-Thistlethwaite notation.
         *
         * For an <i>n</i>-crossing knot, the input may be in one of two
         * forms:
         *
         * - \e numerical Dowker-Thistlethwaite notation, which is a sequence
         *   of \a n even signed integers as described (amongst other places)
         *   in Section 2.2 of C. C. Adams, "The knot book",
         *   W. H. Freeman & Co., 1994;
         *
         * - \e alphabetical Dowker-Thistlethwaite notation, which transforms
         *   the numerical notation into a sequence of letters by replacing
         *   positive integers (2,4,6,...) with lower-case letters
         *   (\c a,\c b,\c c,...), and replacing negative integers
         *   (-2,-4,-6,...) with upper-case letters (\c A,\c B,\c C,...).
         *   This alphabetical variant can only be used to describe knots with
         *   26 crossings or fewer.
         *
         * Dowker-Thistlethwaite notation essentially describes the 4-valent
         * graph of a knot but not the particular embedding in the plane.
         * As a result, there can be ambiguity in the orientation of the
         * diagram, and (for composite knots) even the topology of the knot
         * itself.  Furthermore, parsing Dowker-Thistlethwaite notation is
         * complex since it requires an embedding to be deduced using some
         * variant of a planarity testing algorithm.  These issues are
         * resolved using oriented Gauss codes, as used by the routines
         * orientedGauss() and fromOrientedGauss().
         *
         * As an example, you can construct the trefoil using either of the
         * following variants of Dowker-Thistlethwaite notation:
         *
           \verbatim
           4 6 2
           bca
           \endverbatim
         *
         * There are two variants of this routine.  This variant takes a
         * single string, which is either the alphabetical notation (in which
         * any whitespace within the string will be ignored), or the numerical
         * notation where the integers have been combined together and
         * separated by whitespace.  The other variant of this routine
         * is only for the numerical variant, and it takes a sequence of
         * integers defined by a pair of iterators.
         *
         * In this variant (the string variant), the given string may
         * contain additional leading or trailing whitespace.
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
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * @author Much of the code for this routine is based on the
         * Dowker-Thistlethwaite implementation in the SnapPea/SnapPy kernel.
         *
         * @param str either the alphabetical or numerical
         * Dowker-Thistlethwaite notation for a knot, as described above.
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        static Link* fromDT(const std::string& str);

        /**
         * Creates a new knot from an integer sequence using
         * the numerical variant of Dowker-Thistlethwaite notation.
         *
         * For an <i>n</i>-crossing knot, this must be a sequence of \a n even
         * signed integers as described (amongst other places) in Section 2.2
         * of C. C. Adams, "The knot book", W. H. Freeman & Co., 1994.
         *
         * See fromDT(const std::string&) for a detailed
         * description of this format as it is used in Regina.
         *
         * Regina can also reconstruct a knot from \e alphabetical
         * Dowker-Thistlethwaite notation, but for this you must use the other
         * version of this routine that takes a single string argument.
         *
         * For numerical Dowker-Thistlethwaite notation, there are two variants
         * of this routine that you can use.  The other variant
         * (fromDT(const std::string&), which offers more
         * detailed documentation) takes a single string, where the integers
         * have been combined together and separated by whitespace.  This
         * variant takes a sequence of integers, defined by a pair of iterators.
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
         * input, it does \e not test for planarity of the diagram.
         * That is, if the input describes a knot diagram that must be
         * drawn on some higher-genus surface as opposed to the plane,
         * this will not be detected.  Of course such inputs are not
         * allowed, and it is currently up to the user to enforce this.
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
         * @return a newly constructed link, or \c null if the input was
         * found to be invalid.
         */
        template <typename Iterator>
        static Link* fromDT(Iterator begin, Iterator end);

        /*@}*/

        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const override;
        virtual void writeXMLPacketData(std::ostream& out) const override;

    private:
        /**
         * Clears any calculated properties, and declares them all unknown.
         * This must be called by any internal function that changes the link.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
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
         * @return \c true if and only if the components were successfully
         * processed without errors.
         */
        template <typename... Args>
        bool addComponents(size_t strandsRemaining,
            std::initializer_list<int> component,
            std::initializer_list<Args>... otherComponents);

        /**
         * Internal to fromData().
         *
         * This routine terminates the recursive calls to addComponents().
         * It is called when there are no more components remaining to
         * be processed.
         *
         * @param strandsRemaining the expected sum of the lengths of
         * all components that still need to be processed.  This expected
         * sum is computed as twice the number of crossings minus the
         * lengths of all components that have already been processed.
         * @return \c true if and only if no errors are detected, which
         * for this terminating call simply means \a strandsRemaining == 0.
         */
        bool addComponents(size_t strandsRemaining);

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
        Laurent<Integer>* bracketNaive(ProgressTracker* tracker) const;

        /**
         * Compute the Kauffman bracket polynomial using a fixed-parameter
         * tractable algorithm based on a tree decomposition.
         *
         * The given progress tracker may be \c null.
         * This routine does \e not mark the tracker as finished.
         *
         * See bracket() for further details.
         */
        Laurent<Integer>* bracketTreewidth(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z, using Kauffman's skein-template algorithm.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         */
        Laurent2<Integer>* homflyKauffman(ProgressTracker* tracker) const;

        /**
         * Compute the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z, using a fixed-parameter tractable algorithm
         * based on a tree decomposition.
         *
         * See homflyAZ() for further details.
         *
         * \pre This link contains at least one crossing.
         */
        Laurent2<Integer>* homflyTreewidth(ProgressTracker* tracker) const;

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
         * \a bracket, and the Jones polynomial will be derived from it.
         *
         * This link will claim ownership of the given polynomial.
         *
         * @param bracket the Kauffman bracket polynomial of this link.
         */
        void setPropertiesFromBracket(Laurent<Integer>* bracket) const;

        /**
         * A non-templated version of rewrite().
         *
         * This is identical to rewrite(), except that the action
         * function is given in the form of a std::function.
         * This routine contains the bulk of the implementation of
         * rewrite().
         *
         * Because this routine is not templated, its implementation
         * can be kept out of the main headers.
         */
        bool rewriteInternal(int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            const std::function<bool(Link&)>& action) const;

    friend class ModelLinkGraph;
    friend class Tangle;
    friend class XMLLinkCrossingsReader;
    friend class XMLLinkComponentsReader;
};

/**
 * Iterates through all crossings of a link.
 *
 * The order of iteration follows the indexing of the crossings
 * from 0 to Link::size()-1.
 *
 * This class implements the Boost multipass input iterator concept,
 * which is similar to the standard C++ forward iterator except that
 * the \a reference type may be the same as \a value_type (and so,
 * in particular, the dereference operator may return by value).
 * This header also specialises std::iterator_traits for this iterator type.
 *
 * \ifacespython Not present.
 */
class REGINA_API CrossingIterator {
    private:
        const Link* link_;
            /**< The underlying link. */
        size_t index_;
            /**< The index of the crossing that we are currently visiting. */

    public:
        /**
         * Creates a singular iterator.
         */
        CrossingIterator();
        /**
         * Default copy constructor.
         */
        CrossingIterator(const CrossingIterator&) = default;

        /**
         * Creates a new iterator pointing to the given crossing of the
         * given link.
         *
         * @param link the underlying knot/link.
         * @param index the index of the crossing to point to.  This must be
         * between 0 and link.size()-1 for a deferencable iterator,
         * or must be exactly link.size() for a past-the-end iterator.
         */
        CrossingIterator(const Link& link, size_t index = 0);

        /**
         * Preincrement operator.
         *
         * @return a reference to this iterator.
         */
        CrossingIterator& operator ++ ();

        /**
         * Postincrement operator.
         *
         * @return a copy of this iterator before it was incremented.
         */
        CrossingIterator operator ++ (int);

        /**
         * Returns the crossing to which this iterator points.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the crossing to which this iterator points.
         */
        Crossing* operator * () const;

        /**
         * Default assignment operator.
         *
         * @return a reference to this iterator.
         */
        CrossingIterator& operator = (const CrossingIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * \note This routine only compares the indices of the crossings.
         * It does not examine whether this and the given iterator refer
         * to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are equal.
         */
        bool operator == (const CrossingIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * \note This routine only compares the indices of the crossings.
         * It does not examine whether this and the given iterator refer
         * to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are different.
         */
        bool operator != (const CrossingIterator& rhs) const;
};

/**
 * Iterates through all directed arcs of a knot or link.
 *
 * The order of iteration is as follows.  The iterator works through
 * crossings 0,1,... of the underlying link in turn.  For each crossing,
 * it visits the arcs exiting the crossing from the lower strand and then
 * the upper strand, in that order.
 *
 * Zero-crossing unknot components are not visited at all by this iterator type.
 *
 * This class implements the Boost multipass input iterator concept,
 * which is similar to the standard C++ forward iterator except that
 * the \a reference type may be the same as \a value_type (and so,
 * in particular, the dereference operator may return by value).
 * This header also specialises std::iterator_traits for this iterator type.
 *
 * \ifacespython Not present.
 */
class REGINA_API ArcIterator {
    private:
        const Link* link_;
            /**< The underlying link. */
        size_t index_;
            /**< The index of the crossing that we are currently visiting. */
        bool upper_;
            /**< \c false if we are visiting the arc exiting the
                 crossing from the lower strand, or \c true if we
                 are visiting the arc exiting from the upper strand. */

    public:
        /**
         * Creates a singular iterator.
         */
        ArcIterator();
        /**
         * Default copy constructor.
         */
        ArcIterator(const ArcIterator&) = default;

        /**
         * Creates a new iterator pointing to the arc exiting the
         * given strand of the given crossing of the given link.
         *
         * @param link the underlying knot/link.
         * @param crossing the index of the given crossing.  This must be
         * between 0 and link.size()-1 for a deferencable iterator,
         * or must be exactly link.size() for a past-the-end iterator.
         * @param upper \c true or \c false according to whether the
         * iterator should point to the arc exiting the given crossing
         * from the upper or lower strand respectively.  For a
         * past-the-end iterator, this should always be \c false.
         */
        ArcIterator(const Link& link, size_t crossing = 0, bool upper = false);

        /**
         * Preincrement operator.
         *
         * @return a reference to this iterator.
         */
        ArcIterator& operator ++ ();

        /**
         * Postincrement operator.
         *
         * @return a copy of this iterator before it was incremented.
         */
        ArcIterator operator ++ (int);

        /**
         * Returns the directed arc to which this iterator points.
         *
         * See the StrandRef documentation for details on how a
         * StrandRef object is used to identify a directed arc.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the directed arc to which this iterator points.
         */
        StrandRef operator * () const;

        /**
         * Default assignment operator.
         *
         * @return a reference to this iterator.
         */
        ArcIterator& operator = (const ArcIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * \note This routine only compares the indices of the crossings
         * and the upper/lower strand markings.  It does not examine whether
         * this and the given iterator refer to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are equal.
         */
        bool operator == (const ArcIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * \note This routine only compares the indices of the crossings
         * and the upper/lower strand markings.  It does not examine whether
         * this and the given iterator refer to the same underlying link.
         *
         * @param rhs the iterator to compare with this.
         * @return \c true if and only if the two iterators are different.
         */
        bool operator != (const ArcIterator& rhs) const;
};

/*@}*/

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline void Link::clearAllProperties() {
    jones_.clear();
    homflyAZ_.clear();
    homflyLM_.clear();
    bracket_.clear();
    niceTreeDecomposition_.clear();
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
    out << "crossing " << index();
    if (sign_ == 1)
        out << " (+)";
    else
        out << " (-)";
}

inline void Crossing::writeTextLong(std::ostream& out) const {
    out << "Crossing " << index();
    if (sign_ == 1)
        out << " (+)";
    else
        out << " (-)";
    out << '\n';

    /*
    if (sign_ == 1) {
        out << "--\ /-->  " << next_[0] << '\n';
        out << "   \\n"
        out << "--/ \-->  " << next_[1] << std::endl;
    } else {
        out << "--\ /-->  " << next_[1] << '\n';
        out << "   /\n"
        out << "--/ \-->  " << next_[0] << std::endl;
    }
    */

    out << prev_[1] << "  ->  over  ->  " << next_[1] << '\n';
    out << prev_[0] << "  ->  under  ->  " << next_[0] << std::endl;
}

inline Crossing::Crossing() : sign_(0) {
}

inline Crossing::Crossing(int sign) : sign_(sign) {
}

// Inline functions for Link

inline Link::Link() {
}

inline Link::Link(size_t unknots) {
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

inline StrandRef Link::component(size_t index) const {
    return components_[index];
}

inline StrandRef Link::strand(int id) const {
    return (id >= 0 ? StrandRef(crossings_[id >> 1]->strand(id & 1)) :
        StrandRef());
}

inline long Link::writhe() const {
    long ans = 0;
    for (const Crossing* c : crossings_)
        ans += c->sign();
    return ans;
}

inline const Laurent2<Integer>& Link::homfly(Algorithm alg,
        ProgressTracker* tracker) const {
    return homflyAZ(alg, tracker);
}

inline bool Link::knowsBracket() const {
    return bracket_.known();
}
inline bool Link::knowsJones() const {
    return jones_.known();
}
inline bool Link::knowsHomfly() const {
    // Either both homflyAZ_ and homflyLM_ are known, or neither are known.
    return homflyAZ_.known();
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
    if (niceTreeDecomposition_.known())
        return *niceTreeDecomposition_.value();

    TreeDecomposition* ans = new TreeDecomposition(*this, TD_UPPER);
    prepareTreeDecomposition(*ans);
    return *(niceTreeDecomposition_ = ans);
}

inline void Link::useTreeDecomposition(const TreeDecomposition& td) {
    TreeDecomposition* use = new TreeDecomposition(td);
    prepareTreeDecomposition(*use);
    niceTreeDecomposition_ = use;
}

inline bool Link::dependsOnParent() const {
    return false;
}

inline Packet* Link::internalClonePacket(Packet*) const {
    return new Link(*this);
}

inline StrandRef Link::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(nullptr, other.strand()));
}

template <typename Action, typename... Args>
inline bool Link::rewrite(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker, Action&& action, Args&&... args) const {
    return rewriteInternal(height, nThreads, tracker,
        std::bind(action, std::placeholders::_1, args...));
}

inline void Link::join(const StrandRef& s, const StrandRef& t) {
    s.crossing_->next_[s.strand_] = t;
    t.crossing_->prev_[t.strand_] = s;
}

// Inline functions for CrossingIterator

inline CrossingIterator::CrossingIterator() : link_(nullptr), index_(0) {
}

inline CrossingIterator::CrossingIterator(const Link& link, size_t index) :
        link_(&link), index_(index) {
}

inline CrossingIterator& CrossingIterator::operator ++ () {
    ++index_;
    return *this;
}

inline CrossingIterator CrossingIterator::operator ++ (int) {
    return CrossingIterator(*link_, index_++);
}

inline Crossing* CrossingIterator::operator * () const {
    return link_->crossing(index_);
}

inline bool CrossingIterator::operator == (const CrossingIterator& rhs) const {
    return (index_ == rhs.index_);
}

inline bool CrossingIterator::operator != (const CrossingIterator& rhs) const {
    return (index_ != rhs.index_);
}

// Inline functions for ArcIterator

inline ArcIterator::ArcIterator() : link_(nullptr), index_(0), upper_(false) {
}

inline ArcIterator::ArcIterator(const Link& link, size_t index, bool upper) :
        link_(&link), index_(index), upper_(upper) {
}

inline ArcIterator& ArcIterator::operator ++ () {
    if (upper_) {
        ++index_;
        upper_ = false;
    } else
        upper_ = true;
    return *this;
}

inline ArcIterator ArcIterator::operator ++ (int) {
    ArcIterator ans(*this);
    if (upper_) {
        ++index_;
        upper_ = false;
    } else
        upper_ = true;
    return ans;
}

inline StrandRef ArcIterator::operator * () const {
    return StrandRef(link_->crossing(index_), upper_ ? 1 : 0);
}

inline bool ArcIterator::operator == (const ArcIterator& rhs) const {
    return (index_ == rhs.index_) && (upper_ == rhs.upper_);
}

inline bool ArcIterator::operator != (const ArcIterator& rhs) const {
    return (index_ != rhs.index_) || (upper_ != rhs.upper_);
}

} // namespace regina

namespace std {
    template <>
    struct iterator_traits<regina::CrossingIterator> {
        typedef int difference_type;
        typedef regina::Crossing* value_type;
        typedef regina::Crossing* const* pointer;
        typedef regina::Crossing* const& reference;
        typedef std::input_iterator_tag iterator_category;
    };

    template <>
    struct iterator_traits<regina::ArcIterator> {
        typedef int difference_type;
        typedef typename regina::StrandRef value_type;
        typedef typename regina::StrandRef const* pointer;
        typedef typename regina::StrandRef reference;
        typedef std::input_iterator_tag iterator_category;
    };
} // namespace std

#include "link/data-impl.h"
#include "link/dt-impl.h"
#include "link/gauss-impl.h"

#endif

