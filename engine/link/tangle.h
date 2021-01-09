
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

/*! \file link/tangle.h
 *  \brief Deals with 2-tangles in the 3-ball.
 */

#ifndef __TANGLE_H
#ifndef __DOXYGEN
#define __TANGLE_H
#endif

#include "link/link.h"

namespace regina {

/**
 * \weakgroup link
 * @{
 */

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
 */
class REGINA_API Tangle : public Output<Tangle> {
    private:
        char type_;
            /**< Indicates how the four endpoints connect; this will be
                 one of the symbols \c -, \c | or \c x, representing a
                 horizontal, vertical or diagonal type as described in
                 the class notes. */
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
         * This will be one of the characters \c -, \c | or \c x,
         * indicating a horizontal, vertical or diagonal type as
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
         * Translates a strand reference for some other tangle into the
         * corresponding strand reference for this tangle.
         *
         * Specifically: if \a other refers to some strand (upper or lower)
         * of crossing number \a k of some other tangle, then the return
         * value will refer to the same strand (upper or lower) of
         * crossing number \a k of this tangle.
         *
         * This routine behaves correctly even if \a other is a null reference.
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
         * Swaps the contents of this and the given tangle.
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
        void swapContents(Tangle& other);

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
         * @return a newly created link that is the numerator closure of
         * this tangle.
         */
        Link* numClosure() const;

        /**
         * Forms the denominator closure of this tangle.
         *
         * This is the link created by joining the two left endpoints of
         * this tangle, and also joining the two right endpoints.
         *
         * @return a newly created link that is the denominator closure of
         * this tangle.
         */
        Link* denClosure() const;

        /*@}*/
        /**
         * \name Output
         */
        /*@{*/

        /**
         * Writes a short text representation of this tangle to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this tangle to the
         * given output stream.
         *
         * \ifacespython Not present.
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
         * \ifacespython This routine is not available in Python.  Instead,
         * Python users can use the variant orientedGauss(), which takes no
         * arguments and returns the output as a string.
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
         * - Write one of the tokens \c -, \c | or \c x to represent a
         *   horizontal, vertical or diagonal tangle respectively.
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
         * - Write the token \c _ to indicate that the first string has
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
         * \warning While this routine does some error checking on the
         * input, it does \e not test for the viability of the diagram
         * (i.e., whether the given crossings with the given signs actually
         * produce a tangle of the given type with the correct endpoints).
         * Of course non-viable inputs are not allowed, and it is currently
         * up to the user to enforce this.
         *
         * @param str an oriented Gauss code for a tangle, as described above.
         * @return a newly constructed tangle, or \c null if the input was
         * found to be invalid.
         */
        static Tangle* fromOrientedGauss(const std::string& str);

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
         * \warning While this routine does some error checking on the
         * input, it does \e not test for the viability of the diagram
         * (i.e., whether the given crossings with the given signs actually
         * produce a tangle of the given type with the correct endpoints).
         * Of course non-viable inputs are not allowed, and it is currently
         * up to the user to enforce this.
         *
         * \ifacespython Instead of a pair of begin and past-the-end
         * iterators, this routine takes a Python list of strings.
         *
         * @param begin an iterator that points to the beginning of the
         * sequence of tokens for an oriented Gauss code.
         * @param end an iterator that points past the end of the
         * sequence of tokens for an oriented Gauss code.
         * @return a newly constructed tangle, or \c null if the input was
         * found to be invalid.
         */
        template <typename Iterator>
        static Tangle* fromOrientedGauss(Iterator begin, Iterator end);

        /*@}*/

        // Make this class non-assignable.
        Tangle& operator = (const Tangle&) = delete;

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

/*@}*/

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

inline StrandRef Tangle::begin(int string) const {
    return end_[string][0];
}

inline StrandRef Tangle::end(int string) const {
    return end_[string][1];
}

inline StrandRef Tangle::translate(const StrandRef& other) const {
    return (other.crossing() ?
        crossings_[other.crossing()->index()]->strand(other.strand()) :
        StrandRef(nullptr, other.strand()));
}

inline bool Tangle::r2(Crossing* crossing, bool check, bool perform) {
    return r2(StrandRef(crossing, 1), check, perform);
}

} // namespace regina

#include "link/gauss-tangle-impl.h"

#endif

