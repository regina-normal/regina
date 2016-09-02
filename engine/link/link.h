
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

/*! \file link/nlink.h
 *  \brief Deals with knots and links in the 3-sphere.
 */

#ifndef __LINK_H
#ifndef __DOXYGEN
#define __LINK_H
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "packet/npacket.h"
#include "maths/ninteger.h"
#include "utilities/nmarkedvector.h"

namespace regina {

/**
 * \addtogroup link Knots and Links
 * Knots and links in the 3-sphere
 * @{
 */

class Crossing;
class Link;
template <typename T> class NLaurent;
template <typename T> class NLaurent2;
template <int> class Triangulation;

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
         * Initialises this to a copy of the given reference.
         *
         * @param rhs the reference to copy.
         */
        StrandRef(const StrandRef& rhs);

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
         * Sets this to be a copy of the given reference.
         *
         * @param rhs the reference to clone.
         * @return a reference to this object.
         */
        StrandRef& operator = (const StrandRef& rhs);

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
         * \ifacespython This routine is available under the name inc().
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
         * \ifacespython This routine is not available; however, the
         * preincrement operator is available under the name inc().
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
         * \ifacespython This routine is available under the name dec().
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
         * \ifacespython This routine is not available; however, the
         * preincrement operator is available under the name dec().
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
class REGINA_API Crossing : public NMarkedElement,
        public Output<Crossing>, public boost::noncopyable {
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
class REGINA_API Link : public NPacket {
    REGINA_PACKET(Link, PACKET_LINK)

    private:
        NMarkedVector<Crossing> crossings_;
            /**< The crossings in this link. */
        std::vector<StrandRef> components_;
            /**< For each component of the link, this array contains
                 a reference to one of its strands (which can then be
                 used to traverse the component).  If a component has no
                 crossings, then it is represented in this array by a
                 null reference. */

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
         * you can call <tt>NLaurent::str(Link::jonesVar)</tt>.
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
         * call <tt>NLaurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
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
         * call <tt>NLaurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
         */
        static const char* homflyAZVarY;

        /**
         * The name of the first variable used in the variant of the
         * HOMFLY polynomial as returned by homflyLM().  This is provided to
         * help with pretty-printing HOMFLY polynomials for human consumption.
         *
         * Since homflyLM() returns a Laurent polynomial in \a l and \a m,
         * this string just contains the single character \a l.
         *
         * To pretty-print this HOMFLY polynomial for human consumption, you can
         * call <tt>NLaurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
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
         * call <tt>NLaurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
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
         * @param copy the link to copy.
         */
        Link(const Link& copy);

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
        Crossing* crossing(size_t index);
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
        const Crossing* crossing(size_t index) const;

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

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

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
         * crossings.  When following the upper arc along the orientation of
         * the link, \a crossing should be the first of the two crossings that
         * we encounter (i.e., the start point of the upper arc).  Note
         * that \a crossing is one of the two crossings that will be removed
         * by this move.
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
         * It is allowed to have \a upperArc and \a lowerArc represent
         * the same arc (though the same effect can be achieved by performing
         * two type I Reidemeister moves, i.e., adding two twists).
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
         * will be the second.  There is currently no way to have
         * \a upperArc and \a lowerArc represent the \e same zero-crossing
         * unknot component, but in this case you can achieve the same
         * effect by adding two twists as described above.
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

        /*@}*/
        /**
         * \name Invariants and Related Properties
         */
        /*@{*/

        /**
         * Returns the writhe of this link diagram.
         *
         * The \e writhe sums the signs of all crossings.  It is
         * preserved under Reidemeister moves II and III, but not I.
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
         * @return the complement of this link, as a newly-created object.
         */
        Triangulation<3>* complement(bool simplify = true) const;

        /**
         * Returns the Kauffman bracket polynomial of this link diagram.
         *
         * Note that the bracket polynomial is not an invariant - it is
         * preserved under Reidemeister moves II and III, but not I.
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * The polynomial that is returned will be newly created, and it
         * is the responsibility of the caller of this routine to destroy it.
         *
         * \warning If there are too many crossings for the algorithm to
         * handle, then this routine will return \c null.  Currently this
         * happens when the link contains at least 2^<i>b</i> crossings,
         * where \a b is the number of bits in a long integer (typically 64
         * on a modern machine).  The intention is for this constraint to be
         * removed when Regina switches to a more sophisticated algorithm.
         *
         * \warning The current implementation is naive, with a running
         * time of O(<i>n</i> 2^<i>n</i>) where \a n is the number of
         * crossings.  This will eventually be replaced with a more
         * efficient implementation.
         *
         * @return the bracket polynomial, as a newly-created object.
         */
        NLaurent<NInteger>* bracket() const;

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
         * The polynomial that is returned will be newly created, and it
         * is the responsibility of the caller of this routine to destroy it.
         * To pretty-print this polynomial for human consumption, you can
         * call <tt>NLaurent::str(Link::jonesVar)</tt>.
         *
         * \warning If there are too many crossings for the algorithm to
         * handle, then this routine will return \c null.  Currently this
         * happens when the link contains at least 2^<i>b</i> crossings,
         * where \a b is the number of bits in a long integer (typically 64
         * on a modern machine).  The intention is for this constraint to be
         * removed when Regina switches to a more sophisticated algorithm.
         *
         * \warning The current implementation is naive, with a running
         * time of O(<i>n</i> 2^<i>n</i>) where \a n is the number of
         * crossings.  This will eventually be replaced with a more
         * efficient implementation.
         *
         * @return the Jones polynomial, as a newly-created object.
         */
        NLaurent<NInteger>* jones() const;

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
         * respectively in the class NLaurent2).
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * The polynomial that is returned will be newly created, and it
         * is the responsibility of the caller of this routine to destroy it.
         * To pretty-print this polynomial for human consumption, you can call
         * <tt>NLaurent2::str(Link::homflyAZVarX, Link::homflyAZVarY)</tt>.
         *
         * The current implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * @return the HOMFLY polynomial, as a newly-created object.
         */
        NLaurent2<NInteger>* homflyAZ() const;

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
         * respectively in the class NLaurent2).
         *
         * If this is the empty link, then this routine will return the zero
         * polynomial.
         *
         * The polynomial that is returned will be newly created, and it
         * is the responsibility of the caller of this routine to destroy it.
         * To pretty-print this polynomial for human consumption, you can call
         * <tt>NLaurent2::str(Link::homflyLMVarX, Link::homflyLMVarY)</tt>.
         *
         * The current implementation uses Kauffman's skein-template algorithm;
         * see L. H. Kauffman, "State models for link polynomials",
         * L'enseignement mathematique 36 (1990), 1-37, or for a more recent
         * summary see G. Gouesbet et al., "Computer evaluation of Homfly
         * polynomials by using Gauss codes, with a skein-template algorithm",
         * Applied Mathematics and Computation 105 (1999), 271-289.
         *
         * @return the HOMFLY polynomial, as a newly-created object.
         */
        NLaurent2<NInteger>* homflyLM() const;

        /**
         * Returns the HOMFLY polynomial of this link, as a polynomial
         * in \a alpha and \a z.
         *
         * This routine is simply an alias for homflyAZ().  See the
         * documentation for homflyAZ() for further details.
         *
         * The polynomial that is returned will be newly created, and it
         * is the responsibility of the caller of this routine to destroy it.
         * To pretty-print this polynomial for human consumption, you can call
         * <tt>NLaurent2::str(Link::homflyVarX, Link::homflyVarY)</tt>.
         *
         * @return the HOMFLY polynomial, as a newly-created object.
         */
        NLaurent2<NInteger>* homfly() const;

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
         * \name Building Links
         */
        /*@{*/

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
         * Be aware that, once the link has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Link object numbers its crossings starting from 0).
         *
         * As an example, you can construct the left-hand trefoil and
         * the Hopf link as follows:
         *
         * \pre
         * trefoil = Link::fromData({ -1, -1, -1 }, { 1, -2, 3, -1, 2, -3 });
         * hopf = Link::fromData({ +1, +1 }, { 1, -2 }, { -1, 2 });
         * \endpre
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
         * iterators, this routine takes a Python list of tokens.
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

        /*@}*/

        static NXMLPacketReader* xmlReader(NPacket* parent,
            NXMLTreeResolver& resolver);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
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

    friend class XMLLinkCrossingsReader;
    friend class XMLLinkComponentsReader;
};

/*@}*/

// Inline functions for StrandRef

inline StrandRef::StrandRef() : crossing_(0), strand_(0) {
}

inline StrandRef::StrandRef(Crossing* crossing, int strand) :
        crossing_(crossing), strand_(strand) {
}

inline StrandRef::StrandRef(const StrandRef& rhs) :
        crossing_(rhs.crossing_), strand_(rhs.strand_) {
}

inline Crossing* StrandRef::crossing() const {
    return crossing_;
}

inline int StrandRef::strand() const {
    return strand_;
}

inline bool StrandRef::operator == (const StrandRef& rhs) const {
    return (crossing_ == rhs.crossing_ && strand_ == rhs.strand_);
}

inline bool StrandRef::operator != (const StrandRef& rhs) const {
    return (crossing_ != rhs.crossing_ || strand_ != rhs.strand_);
}

inline StrandRef& StrandRef::operator = (const StrandRef& rhs) {
    crossing_ = rhs.crossing_;
    strand_ = rhs.strand_;
    return *this;
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
    return (crossing_ != 0);
}

inline std::ostream& operator << (std::ostream& out, const StrandRef& s) {
    if (s.crossing())
        return out << (s.strand() == 1 ? '^' : '_') << s.crossing()->index();
    else
        return out << "(null)";
}

// Inline functions for Crossing

inline int Crossing::index() const {
    return markedIndex();
}

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

inline StrandRef Crossing::next(int strand) const {
    return next_[strand];
}

inline StrandRef Crossing::prev(int strand) const {
    return prev_[strand];
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

inline Link::~Link() {
    for (Crossing* c : crossings_)
        delete c;
}

inline size_t Link::size() const {
    return crossings_.size();
}

inline size_t Link::countComponents() const {
    return components_.size();
}

inline Crossing* Link::crossing(size_t index) {
    return crossings_[index];
}

inline const Crossing* Link::crossing(size_t index) const {
    return crossings_[index];
}

inline StrandRef Link::component(size_t index) const {
    return components_[index];
}

inline long Link::writhe() const {
    long ans = 0;
    for (const Crossing* c : crossings_)
        ans += c->sign();
    return ans;
}

inline NLaurent2<NInteger>* Link::homfly() const {
    return homflyAZ();
}

inline bool Link::dependsOnParent() const {
    return false;
}

inline NPacket* Link::internalClonePacket(NPacket*) const {
    return new Link(*this);
}

inline StrandRef Link::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(0, other.strand()));
}

} // namespace regina

#include "link/data-impl.h"
#include "link/gauss-impl.h"

#endif

