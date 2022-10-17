
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file link/tangle.h
 *  \brief Deals with 2-tangles in the 3-ball.
 */

#ifndef __REGINA_TANGLE_H
#ifndef __DOXYGEN
#define __REGINA_TANGLE_H
#endif

#include "link/link.h"
#include "utilities/listview.h"

namespace regina {

/**
 * Represents a 2-tangle in the 3-ball.  Regina does not allow closed
 * components in a tangle; in other words, a tangle in Regina is a
 * proper embedding of exactly two arcs in the 3-ball with the
 * corresponding four endpoints attached to four marked points on
 * the 3-ball boundary.
 *
 * Regina stores tangles as projections, with the four endpoints
 * fixed at the top-left, top-right, bottom-left and bottom-right
 * corners of the diagram.
 *
 * Each tangles has a \e type, indicating how the four endpoints are
 * connected.  The three possible types are:
 *
 * - \e horizontal, indicating that the two top endpoints are connected,
 *   and the two bottom endpoints are connected;
 *
 * - \e vertical, indicating that the two left endpoints are connected,
 *   and the two right endpoints are connected;
 *
 * - \e diagonal, indicating that the top-left and bottom-right endpoints
 *   are connected, and the bottom-left and top-right endpoints are connected.
 *
 * Internally, Regina numbers the two strings 0 and 1: string 0 will
 * always be the one attached to the top-left endpoint.
 * Regina also assigns each string an orientation: for a
 * horizontal or diagonal tangle this will always be from left to right,
 * and for a vertical tangle this will always be from top to bottom.
 *
 * When traversing a tangle, if you reach one of the endpoints of a string
 * then the corresponding return value of Crossing::next() or
 * Crossing::prev() (whichever is relevant) will be a null strand reference.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class Tangle : public Output<Tangle> {
    private:
        char type_;
            /**< Indicates how the four endpoints connect; this will be
                 one of the symbols <tt>-</tt>, <tt>|</tt> or <tt>x</tt>,
                 representing a horizontal, vertical or diagonal type as
                 described in the class notes. */
        MarkedVector<Crossing> crossings_;
            /**< The crossings in this tangle. */
        StrandRef end_[2][2];
            /**< The member <tt>end_[s][i]</tt> store the crossings
                 closest to the two endpoints of string \a s, where
                 endpoint <i>i</i>=0 is at the beginning of the string
                 (following its orientation), and the endpoint <i>i</i>=1
                 is at the end of the string.
                 If a string has no crossings at all, then the two
                 endpoints in this array will be null references. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Constructs the zero tangle.
         *
         * This is the horizontal tangle with no crossings.
         */
        Tangle();
        /**
         * Constructs a tangle from the given number of twists.
         *
         * If \a twists is positive, then the new tangle will consist of
         * \a twists positive twists, stacked from left to right.
         * If \a twists is negative, then the new tangle will consist of
         * -(\a twists) negative twists, likewise stacked from left to right.
         * If \a twists is zero, then the new tangle will be a
         * horizontal tangle with no crossings at all.
         *
         * In all cases, this is equivalent to calling the rational
         * tangle constructor Tangle(\a twists, 1).
         *
         * @param twists the number of twists to perform; this may be
         * positive, negative or zero.
         */
        Tangle(int twists);
        /**
         * Constructs a rational tangle with the given parameters.
         * Here we use the following convention (following the
         * description that Adams gives in <i>The Knot Book</i>):
         *
         * - the zero tangle is horizontal with no crossings;
         * - the infinity tangle is vertical with no crossings;
         * - the +1 tangle is diagonal with one crossing, where
         *   the upper string runs from bottom-left to top-right.
         *
         * \pre The given arguments are coprime.
         *
         * @param num the numerator of the rational number that
         * describes this tangle.
         * @param den the denominator of the rational number that
         * describes this tangle; this may be 0 (representing the
         * infinity tangle).
         */
        Tangle(int num, int den);
        /**
         * Creates a tangle from two parallel copies of the given knot.
         *
         * Specifically, the tangle will consist of two parallel copies
         * of the given knot diagram, which will be broken just before
         * the starting strand as returned by <tt>knot.component(0)</tt>.
         *
         * The two resulting endpoints that appear just before the
         * starting strand will form the top-left and bottom-left
         * endpoints of this tangle, and the endpoints on the other side
         * of the break (which will be just after the parallel copies of the
         * final strand <tt>knot.component(0).prev()</tt>) will form the
         * top-right and bottom-right endpoints of this tangle.
         *
         * The tangle will contain <tt>4 * knot.size()</tt> crossings in total.
         *
         * \pre The argument contains exactly one component (i.e., it
         * is actually a knot, and not empty or a multiple-component link).
         *
         * @param knot the knot to break and duplicate to form this tangle.
         */
        Tangle(const Link& knot);
        /**
         * Constructs a new copy of the given tangle.
         *
         * @param copy the tangle to copy.
         */
        Tangle(const Tangle& copy);
        /**
         * Moves the given tangle into this new tangle.
         * This is a fast (constant time) operation.
         *
         * All crossings that belong to \a src will be moved into this tangle,
         * and so any Crossing pointers or StrandRef object will remain valid.
         * Likewise, all cached properties will be moved into this tangle.
         *
         * The tangle that is passed (\a src) will no longer be usable.
         *
         * @param src the tangle to move.
         */
        Tangle(Tangle&& src) noexcept;

        /**
         * Destroys this tangle.
         *
         * The Crossing objects contained in this tangle will also be destroyed.
         */
        ~Tangle();

        /*@}*/
        /**
         * \name Crossings and Strings
         */
        /*@{*/

        /**
         * Returns the type of this tangle.
         *
         * This will be one of the characters <tt>-</tt>, <tt>|</tt> or
         * <tt>x</tt>, indicating a horizontal, vertical or diagonal type as
         * described in the class notes.
         *
         * @return the type of this crossing.
         */
        char type() const;

        /**
         * Returns the number of crossings in this tangle.
         *
         * @return the number of crossings.
         */
        size_t size() const;

        /**
         * Returns a pointer to the crossing at the given index within
         * this tangle.
         *
         * For a tangle with \a n crossings, the crossings are numbered
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
         * to all crossings within this tangle.
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
         * for (Crossing* c : tangle.crossings()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for as
         * long as the tangle exists: even as crossings are added and/or
         * removed, it will always reflect the crossings that are currently in
         * the tangle.  Nevertheless, it is recommended to treat this object as
         * temporary only, and to call crossings() again each time you need it.
         *
         * @return access to the list of all crossings.
         */
        auto crossings() const;

        /**
         * Returns the crossing closest to the beginning of the given string.
         *
         * Recall from the class notes that string 0 is always attached
         * to the top-left endpoint.  Recall also that strings are
         * oriented from left-to-right for a horizontal or diagonal tangle,
         * and from top-to-bottom for a vertical tangle.
         *
         * @param string indicates which of the two strings in this
         * tangle to query; this must be either 0 or 1.
         * @return the crossing closest to the beginning of the given string,
         * or a null reference if the given string contains no crossings.
         */
        StrandRef begin(int string) const;

        /**
         * Returns the crossing closest to the end of the given string.
         *
         * Recall from the class notes that string 0 is always attached
         * to the top-left endpoint.  Recall also that strings are
         * oriented from left-to-right for a horizontal or diagonal tangle,
         * and from top-to-bottom for a vertical tangle.
         *
         * @param string indicates which of the two strings in this
         * tangle to query; this must be either 0 or 1.
         * @return the crossing closest to the end of the given string,
         * or a null reference if the given string contains no crossings.
         */
        StrandRef end(int string) const;

        /**
         * Determines if this tangle is combinatorially identical to the
         * given tangle.
         *
         * Here "identical" means that:
         *
         * - the tangles are of the same type and have the same number of
         *   crossings;
         *
         * - the same numbered crossings are positive and negative in both
         *   tangles;
         *
         * - the corresponding strings in each tangle pass through the same
         *   under/over-strands of the same numbered crossings in the same
         *   order.
         *
         * @param other the tangle to compare with this.
         * @return \c true if and only if the two tangles are
         * combinatorially identical.
         */
        bool operator == (const Tangle& other) const;

        /**
         * Determines if this tangle is not combinatorially identical
         * to the given tangle.
         *
         * Here "identical" means that:
         *
         * - the tangles are of the same type and have the same number of
         *   crossings;
         *
         * - the same numbered crossings are positive and negative in both
         *   tangles;
         *
         * - the corresponding strings in each tangle pass through the same
         *   under/over-strands of the same numbered crossings in the same
         *   order.
         *
         * @param other the tangle to compare with this.
         * @return \c true if and only if the two tangles are
         * not combinatorially identical.
         */
        bool operator != (const Tangle& other) const;

        /**
         * Translates a strand reference from some other tangle into the
         * corresponding strand reference from this tangle.
         *
         * Typically this routine would be used when the given strand comes
         * from a tangle that is combinatorially identical to this, and you wish
         * to obtain the corresponding strand in this tangle.
         *
         * Specifically: if \a other refers to some strand (upper or lower)
         * of crossing number \a k of some other tangle, then the return
         * value will refer to the same strand (upper or lower) of
         * crossing number \a k of this tangle.
         *
         * This routine behaves correctly even if \a other is a null reference.
         *
         * \pre This tangle contains at least as many crossings as the tangle
         * containing \a other (though, as noted above, in typical scenarios
         * both tangles would actually be combinatorially identical).
         *
         * @param other the strand reference to translate.
         * @return the corresponding strand reference for this tangle.
         */
        StrandRef translate(const StrandRef& other) const;

        /*@}*/
        /**
         * \name Editing
         */
        /*@{*/

        /**
         * Sets this to be a (deep) copy of the given tangle.
         *
         * @param src the tangle to copy.
         * @return a reference to this tangle.
         */
        Tangle& operator = (const Tangle& src);

        /**
         * Moves the contents of the given tangle into this tangle.
         * This is a fast (constant time) operation.
         *
         * All crossings that belong to \a src will be moved into this tangle,
         * and so any Crossing pointers or StrandRef object will remain valid.
         * Likewise, all cached properties will be moved into this tangle.
         *
         * The tangle that is passed (\a src) will no longer be usable.
         *
         * @param src the tangle to move.
         * @return a reference to this tangle.
         */
        Tangle& operator = (Tangle&& src) noexcept;

        /**
         * Swaps the contents of this and the given tangle.
         *
         * All crossings that belong to this tangle will be moved to \a other,
         * and all crossings that belong to \a other will be moved to this
         * tangle.  Likewise, all cached properties will be swapped.
         *
         * In particular, any Crossing pointers or references and any
         * StrandRef objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this tangle.
         *
         * @param other the tangle whose contents should be swapped with this.
         */
        void swap(Tangle& other) noexcept;

        /**
         * Adds a twist to the right-hand end of this tangle.
         *
         * @param sign either 1 if we should perform a positive twist
         * (dragging the bottom-right endpoint up over the top-right endpoint),
         * or -1 if we should perform a negative twist (dragging the
         * bottom-right endpoint up beneath the top-right endpoint).
         */
        void twist(int sign = 1);

        /**
         * Rotates this tangle by 90 degrees.
         *
         * @param direction either 1 if the tangle should be rotated
         * clockwise, or -1 if the tangle should be rotated anticlockwise.
         */
        void turn(int direction = 1);

        /**
         * Switches the upper and lower strands of every crossing in the 
         * tangle.
         *
         * This operation corresponds to reflecting the tangle through
         * the plane on which the diagram is drawn.
         */
        void changeAll();

        /**
         * Tests for and/or performs a type I Reidemeister move to remove a
         * crossing.
         *
         * Unlike links, which implement the full suite of Reidemeister
         * moves, tangles (at present) only offer the simplifying versions
         * of Reidemeister moves I and II.
         *
         * The behaviour of this routine is identical to the r1()
         * routine in the Link class; see Link::r1() for further details.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this tangle.
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
         * Tests for and/or performs a type II Reidemeister move to remove
         * two crossings.
         *
         * Unlike links, which implement the full suite of Reidemeister
         * moves, tangles (at present) only offer the simplifying versions
         * of Reidemeister moves I and II.
         *
         * The behaviour of this routine is identical to the r2()
         * routine in the Link class; see Link::r2() for further details.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given strand reference is either a null reference,
         * or else refers to some strand of some crossing in this tangle.
         *
         * @param arc identifies one of the arcs of the bigon about
         * which the move will be performed.
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
         * Unlike links, which implement the full suite of Reidemeister
         * moves, tangles (at present) only offer the simplifying versions
         * of Reidemeister moves I and II.
         *
         * The behaviour of this routine is identical to the r2()
         * routine in the Link class; see Link::r2() for further details.
         *
         * \pre If \a perform is \c true but \a check is \c false, then
         * it must be known in advance that this move can be performed
         * at the given location.
         * \pre The given crossing is either a null pointer, or else some
         * crossing in this tangle.
         *
         * @param crossing identifies the crossing at the beginning of
         * the "upper" arc that features in this move.
         * @param check \c true if we are to check whether the move is legal.
         * @param perform \c true if we should actually perform the move.
         * @return If \a check is \c true, this function returns \c true
         * if and only if the requested move is legal.  If \a check is \c false,
         * this function always returns \c true.
         */
        bool r2(Crossing* crossing, bool check = true, bool perform = true);

        /**
         * Uses type I and II Reidemeister moves to reduce the tangle
         * monotonically to some local minimum number of crossings.
         * Type III Reidemeister moves (which do not reduce the number of
         * crossings) are not used in this routine.
         *
         * Unlike links, tangle do not (at present) offer stronger
         * simplification routines (such as the much better
         * Link::intelligentSimplify() and Link::simplifyExhaustive()).
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

        /*@}*/
        /**
         * \name Algebra on Tangles
         */
        /*@{*/

        /**
         * Adds the given tangle to the right-hand side of this tangle.
         *
         * In Conway's notation, if this tangle is \a t, then this
         * routine converts this into (\a t + \a other).
         *
         * Specifically: this routine will attach the two right-hand
         * endpoints of this tangle to the two left-hand endpoints of a
         * copy of \a other.
         *
         * This tangle will be changed directly.  The tangle \a other
         * (passed as the argumet) will be left unchanged.
         *
         * It is allowed to pass this tangle as \a other.
         *
         * \pre It is not the case that both this and \e other are
         * vertical tangles (which would cause the addition to create a
         * closed link component).
         *
         * @param other the tangle to add to this.
         */
        void add(const Tangle& other);

        /**
         * Reflects this tangle through the diagonal axis running from
         * the top-left to bottom-right corners of the diagram.
         *
         * In Conway's notation, this negates the tangle.
         */
        void negate();

        /**
         * Encloses this tangle with the four given tangles in a box
         * configuration.
         *
         * The five tangles will be connected as shown, with this tangle
         * in the centre:
           \verbatim
            \     /
             O---O
            / \ / \
            |  O  |
            \ / \ /
             O---O
            /     \
           \endverbatim
         *
         * The top-left corner of the argument \a topLeft will become
         * the top-left corner of the resulting tangle, and so on for
         * the other three corners.
         *
         * This tangle will be changed directly.  The other four other tangles
         * (passed as arguments) will be left unchanged.
         *
         * You may use the same tangle for multiple arguments, and you
         * may even use this tangle for one or more arguments.
         *
         * \pre Every string in all five tangles (the four arguments and
         * this) has at least one crossing.
         * \pre None of the five tangles (the four arguments and this)
         * have types that would result in a closed link component after
         * this operation is performed.
         *
         * @param topLeft the tangle to connect to the top-left corner of this.
         * @param topRight the tangle to connect to the top-right corner of
         * this.
         * @param bottomLeft the tangle to connect to the bottom-left corner
         * of this.
         * @param bottomRight the tangle to connect to the bottom-right corner
         * of this.
         */
        void box(const Tangle& topLeft, const Tangle& topRight,
            const Tangle& bottomLeft, const Tangle& bottomRight);

        /**
         * Forms the numerator closure of this tangle.
         *
         * This is the link created by joining the two top endpoints of
         * this tangle, and also joining the two bottom endpoints.
         *
         * @return the numerator closure of this tangle.
         */
        Link numClosure() const;

        /**
         * Forms the denominator closure of this tangle.
         *
         * This is the link created by joining the two left endpoints of
         * this tangle, and also joining the two right endpoints.
         *
         * @return the denominator closure of this tangle.
         */
        Link denClosure() const;

        /*@}*/
        /**
         * \name Output
         */
        /*@{*/

        /**
         * Writes a short text representation of this tangle to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this tangle to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /*@}*/
        /**
         * \name Exporting Tangles
         */
        /*@{*/

        /**
         * Outputs this tangle in Regina's own brief write-only format.
         * This format is concise, but contains enough information to
         * manually reconstruct the complete tangle.
         *
         * This format cannot (yet) be used to read tangles back into Regina,
         * and so it is not good for external storage, or for passing tangles
         * between different programs (or even different instances of Regina).
         * It was originally designed for use with the test suite, where it
         * was used to ensure that tangles with being created and/or manipulated
         * correctly.
         *
         * The output will contain the following elements, separated by
         * single spaces:
         *
         * - one of the symbols <tt>-</tt>, <tt>|</tt> or <tt>x</tt>,
         *   indicating that the tangle is of horizontal, vertical or
         *   diagonal type respectively (as described in the class notes);
         *
         * - a sequence of signs (<tt>+</tt> or <tt>-</tt>), concatenated
         *   together, giving the signs of the crossings in order from
         *   crossing 0 to crossing size()-1;
         *
         * - a description of string 0 and then string 1.  Each string
         *   will be written in the form <tt>( a b c ... )</tt>, indicating
         *   the crossings that are encountered as we follow the string
         *   in the forward direction from its starting endpoint.  Each element
         *   \a a, \a b, \a c and so on will be written in the format used by
         *   the StrandRef class: either <tt>^n</tt> when passing over
         *   crossing \a n, or <tt>_n</tt> when passing under crossing \a n.
         *
         * For example, the rational tangle 3/2 as returned by
         * Tangle(3,2) will give the following brief output:
         *
           \verbatim
           | --+ ( _0 ^1 ) ( ^2 _1 ^0 _2 )
           \endverbatim
         *
         * As a special case, if the tangle contains no crossings then
         * the output will contain just one space, not two consecutive spaces,
         * between the type symbol and the string descriptions (since
         * the sequence of crossing signs that would normally sit between them
         * will be empty).
         *
         * The string will not end in a newline.
         *
         * There is also a variant of brief() that writes directly to an
         * output stream.
         *
         * @return a description of this tangle in Regina's brief format.
         */
        std::string brief() const;

        /**
         * Writes this tangle in Regina's own brief format to the given
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
         * \nopython Instead use the variant brief(), which takes no arguments
         * and returns a string.
         *
         * @param out the output stream to which to write.
         */
        void brief(std::ostream& out) const;

        /**
         * Outputs an oriented Gauss code for this tangle.
         *
         * Oriented Gauss codes for tangles are an extension of oriented
         * Gauss codes for knots.  Whilst oriented Gauss codes for knots
         * are used elsewhere (they are based on a format used by
         * Andreeva et al.), these codes for tangles are specific to Regina
         * (so you should not expect other software to understand them).
         *
         * For a full explanation of how oriented Gauss codes work for tangles,
         * see the documentation for fromOrientedGauss(const std::string&),
         * which imports tangles in this format.
         *
         * The string that is returned will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of returning a string, writes directly to an output stream.
         *
         * @return an oriented Gauss code for this tangle.
         */
        std::string orientedGauss() const;

        /**
         * Writes an oriented Gauss code for this tangle to the given output
         * stream.
         *
         * Oriented Gauss codes for tangles are an extension of oriented
         * Gauss codes for knots.  Whilst oriented Gauss codes for knots
         * are used elsewhere (they are based on a format used by
         * Andreeva et al.), these codes for tangles are specific to Regina
         * (so you should not expect other software to understand them).
         *
         * For a full explanation of how oriented Gauss codes work for tangles,
         * see the documentation for fromOrientedGauss(const std::string&),
         * which imports tangles in this format.
         *
         * The output will not contain any newlines.
         *
         * \note There is another variant of this routine that, instead
         * of using an output stream, simply returns a string.
         *
         * \nopython Instead use the variant orientedGauss(), which takes no
         * arguments and returns a string.
         *
         * @param out the output stream to which to write.
         */
        void orientedGauss(std::ostream& out) const;

        /*@}*/
        /**
         * \name Building Tangles
         */
        /*@{*/

        /**
         * Creates a new tangle from an oriented Gauss code.
         *
         * Oriented Gauss codes for tangles are an extension of oriented
         * Gauss codes for knots.  Whilst oriented Gauss codes for knots
         * are used elsewhere (they are based on a format used by
         * Andreeva et al.), these codes for tangles are specific to Regina
         * (so you should not expect other software to understand them).
         *
         * The format works as follows:
         *
         * - Label the crossings arbitrarily as 1, 2, ..., \a n.
         *
         * - Write one of the tokens <tt>-</tt>, <tt>|</tt> or <tt>x</tt> to
         *   represent a horizontal, vertical or diagonal tangle respectively.
         *
         * - Start at the top-left endpoint and follow this string to
         *   its other endpoint.  At every crossing that you pass, write a
         *   token of the form <tt>+&lt;<i>k</i></tt>, <tt>-&lt;<i>k</i></tt>,
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
         * - Write the token <tt>_</tt> to indicate that the first string has
         *   finished.
         *
         * - Start at the beginning of the other string (for horizontal
         *   or diagonal tangles, this is the bottom-left endpoint, and
         *   for vertical tangles this is the top-right endpoint).  As
         *   before, follow this string to its other endpoint, writing a
         *   token of the form <tt>+&lt;<i>k</i></tt>, <tt>-&lt;<i>k</i></tt>,
         *   <tt>+&gt;<i>k</i></tt> or <tt>-&gt;<i>k</i></tt> at every
         *   crossing that you pass.
         *
         * Be aware that, once the tangle has been constructed, the crossings
         * 1, ..., \a n will have been reindexed as 0, ..., <i>n</i>-1
         * (since every Tangle object numbers its crossings starting from 0).
         *
         * As an example, you can construct the rational tangle -3/4
         * using the following code:
         *
           \verbatim
           | -<1 +>2 -<3 +>4 _ -<5 -<4 +>3 -<2 +>1 +>5
           \endverbatim
         *
         * There are two variants of this routine.  This variant takes a
         * single string, where the tokens have been combined together and
         * separated by whitespace.  The other variant takes a sequence of
         * tokens, defined by a pair of iterators.
         *
         * In this variant (the string variant), the given string may
         * contain additional leading or trailing whitespace.
         *
         * \warning While this routine does some error checking on the input,
         * these checks are not exhaustive.  In particular, it does \e not test
         * for the viability of the diagram (i.e., whether the given crossings
         * with the given signs actually produce a tangle of the given type
         * with the correct endpoints).  Of course non-viable inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument The input was not a valid oriented
         * Gauss code.  As noted above, the checks performed here are
         * not exhaustive.
         *
         * @param str an oriented Gauss code for a tangle, as described above.
         * @return the resulting tangle.
         */
        static Tangle fromOrientedGauss(const std::string& str);

        /**
         * Creates a new tangle from an oriented Gauss code.
         *
         * Oriented Gauss codes for tangles are an extension of oriented
         * Gauss codes for knots.  Whilst oriented Gauss codes for knots
         * are used elsewhere (they are based on a format used by
         * Andreeva et al.), these codes for tangles are specific to Regina
         * (so you should not expect other software to understand them).
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
         * \warning While this routine does some error checking on the input,
         * these checks are not exhaustive.  In particular, it does \e not test
         * for the viability of the diagram (i.e., whether the given crossings
         * with the given signs actually produce a tangle of the given type
         * with the correct endpoints).  Of course non-viable inputs are not
         * allowed, and it is currently up to the user to enforce this.
         *
         * \exception InvalidArgument The input did not describe a valid
         * oriented Gauss code.  As noted above, the checks performed here
         * are not exhaustive.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of tokens for an oriented Gauss code.
         * @param end an iterator that points past the end of the
         * sequence of tokens for an oriented Gauss code.
         * @return the resulting tangle.
         */
        template <typename Iterator>
        static Tangle fromOrientedGauss(Iterator begin, Iterator end);

        /*@}*/

    private:
        /**
         * Reverses the orientation of the given string.
         * This will make all necessary edits to all Crossing objects,
         * but will not touch the internal \a end_ array.
         */
        void reverse(int string);

        /**
         * Indicates that the strand immediately before \a oldDest should
         * now be followed by \a newDest.  This does the correct thing
         * even if \a oldDest is at the beginning of a string, and/or if
         * \a newDest is a null reference.  The relevant \a next_ array
         * (or \a end_[i][0] if necessary) will be adjusted accordingly.
         *
         * Note that that the \a prev_ array at \a newDest (or \a end_[i][1]
         * if \a newDest is null) will not be touched.  That is, this routine
         * may result in inconsistent connections.
         *
         * \pre The argument \a oldDest is not a null strand reference.
         */
        void rerouteTo(const StrandRef& oldDest, const StrandRef& newDest);

        /**
         * Indicates that the strand immediately after \a oldSrc should
         * now be preceded by \a newSrc.  This does the correct thing
         * even if \a oldSrc is at the end of a string, and/or if
         * \a newSrc is a null reference.  The relevant \a prev_ array
         * (or \a end_[i][1] if necessary) will be adjusted accordingly.
         *
         * Note that that the \a next_ array at \a newSrc (or \a end_[i][0]
         * if \a newSrc is null) will not be touched.  That is, this routine
         * may result in inconsistent connections.
         *
         * \pre The argument \a oldSrc is not a null strand reference.
         */
        void rerouteFrom(const StrandRef& oldSrc, const StrandRef& newSrc);

        /**
         * Internal to box().
         *
         * Coverts a corner of this tangle into a (string, end) pair.
         */
        void endForCorner(int corner, int& string, int& end);

        /**
         * Internal to box().
         *
         * Coverts a (string, end) pair into a corner of this tangle.
         */
        int cornerForEnd(int string, int end);

        /**
         * Internal to fromOrientedGauss().
         *
         * This routine examines a single token in an oriented Gauss code.
         * If the token contains exactly one character, then it returns
         * that character; otherwise it returns 0.
         */
        static char extractChar(const char* s);

        /**
         * Internal to fromOrientedGauss().
         *
         * This routine examines a single token in an oriented Gauss code.
         * If the token contains exactly one character, then it returns
         * that character; otherwise it returns 0.
         */
        static char extractChar(const std::string& s);
};

/**
 * Swaps the contents of the two given tangles.
 *
 * This global routine simply calls Tangle::swap(); it is provided so
 * that Tangle meets the C++ Swappable requirements.
 *
 * See Tangle::swap() for more details.
 *
 * @param lhs the tangle whose contents should be swapped with \a rhs.
 * @param rhs the tangle whose contents should be swapped with \a lhs.
 *
 * \ingroup link
 */
void swap(Tangle& lhs, Tangle& rhs) noexcept;

// Inline functions for Tangle

inline Tangle::Tangle() : type_('-') {
    // By default, crossings_ is empty and each end_[i][j] is a null reference.
}

inline Tangle::~Tangle() {
    for (Crossing* c : crossings_)
        delete c;
}

inline char Tangle::type() const {
    return type_;
}

inline size_t Tangle::size() const {
    return crossings_.size();
}

inline Crossing* Tangle::crossing(size_t index) const {
    return crossings_[index];
}

inline auto Tangle::crossings() const {
    return ListView(crossings_);
}

inline StrandRef Tangle::begin(int string) const {
    return end_[string][0];
}

inline StrandRef Tangle::end(int string) const {
    return end_[string][1];
}

inline bool Tangle::operator != (const Tangle& other) const {
    return ! ((*this) == other);
}

inline StrandRef Tangle::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(nullptr, other.strand()));
}

inline bool Tangle::r2(Crossing* crossing, bool check, bool perform) {
    return r2(StrandRef(crossing, 1), check, perform);
}

inline void swap(Tangle& lhs, Tangle& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace regina

#include "link/gauss-tangle-impl.h"

#endif

