
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

/*! \file algebra/grouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

#ifndef __REGINA_GROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __REGINA_GROUPPRESENTATION_H
#endif

#include <algorithm>
#include <functional>
#include <list>
#include <optional>
#include <vector>
#include <set>
#include <map>

#include "regina-core.h"
#include "core/output.h"
#include "algebra/markedabeliangroup.h"
#include "algebra/abeliangroup.h"
#include "maths/matrix.h"

// There are more includes at the end of this file.

namespace regina {

class AbelianGroup;
class HomGroupPresentation;
class MarkedAbelianGroup;

/**
 * Represents a power of a generator in a group presentation.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup algebra
 */
struct GroupExpressionTerm {
    unsigned long generator;
        /**< The number that identifies the generator in this term. */
    long exponent;
        /**< The exponent to which the generator is raised. */

    /**
     * Creates a new uninitialised term.
     */
    GroupExpressionTerm() = default;
    /**
     * Creates a new term initialised to the given value.
     *
     * \param gen the number that identifies the generator in the new term.
     * \param exp the exponent to which this generator is raised.
     */
    GroupExpressionTerm(unsigned long gen, long exp);
    /**
     * Creates a new term initialised to the given value.
     */
    GroupExpressionTerm(const GroupExpressionTerm&) = default;

    /**
     * Makes this term identical to the given term.
     *
     * \return a reference to this term.
     */
    GroupExpressionTerm& operator = (const GroupExpressionTerm&) = default;
    /**
     * Determines whether this and the given term contain identical data.
     *
     * \param other the term with which this term will be compared.
     * \return \c true if and only if this and the given term have both the
     * same generator and exponent.
     */
    bool operator == (const GroupExpressionTerm& other) const;
    /**
     * Determines whether this and the given term do not contain identical data.
     *
     * \param other the term with which this term will be compared.
     * \return \c true if and only if this and the given term do not have
     * both the same generator and exponent.
     */
    bool operator != (const GroupExpressionTerm& other) const;

    /**
     * Imposes an ordering on terms.
     * Terms are ordered lexigraphically as (generator, exponent) pairs.
     *
     * \param other the term to compare with this.
     * \return true if and only if this term is lexicographically
     * smaller than \a other.
     */
    bool operator < (const GroupExpressionTerm& other) const;

    /**
     * Returns the inverse of this term.  The inverse has the same
     * generator but a negated exponent.
     *
     * Note that this term will remain unchanged.
     *
     * \return the inverse of this term.
     */
    GroupExpressionTerm inverse() const;

    /**
     * Attempts to merge this term with the given term.
     * If both terms have the same generator, the two exponents will be
     * added and stored in this term.  If the generators are different,
     * this routine will do nothing.
     *
     * Note that this term might be changed but the given term will remain
     * unchanged.
     *
     * \param other the term to merge with this term.
     * \return \c true if the two terms were merged into this term, or
     * \c false if the two terms have different generators.
     */
    bool operator += (const GroupExpressionTerm& other);
};

/**
 * Writes the given term to the given output stream.
 * The term will be written in the format `g3^-7`, where in this
 * example the term represents generator number 3 raised to the -7th power.
 *
 * Note that generators are indexed start from 0 (so `g3` is in fact the
 * _fourth_ generator in the group presentation, not the third).
 *
 * If the term has exponent 0 or 1, the output format will be
 * appropriately simplified.
 *
 * \param out the output stream to which to write.
 * \param term the term to write.
 * \return a reference to the given output stream.
 *
 * \ingroup algebra
 */
std::ostream& operator << (std::ostream& out, const GroupExpressionTerm& term);

/**
 * Represents an expression involving generators from a group presentation
 * or a free group.  An expression is represented as word, i.e, a sequence
 * of powers of generators all of which are multiplied in order.  Each power
 * of a generator corresponds to an individual GroupExpressionTerm.
 *
 * For instance, the expression `g1^2 g3^-1 g6` contains the
 * three terms `g1^2`, `g3^-1` and `g6^1` in that order.
 *
 * Note that generators are indexed starting from 0 (so, for example, `g3`
 * represents the _fourth_ generator in the group presentation, not the third).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup algebra
 */
class GroupExpression : public ShortOutput<GroupExpression, true> {
    private:
        std::list<GroupExpressionTerm> terms_;
            /** The terms that make up this expression. */

    public:
        /**
         * Creates a new expression with no terms.
         */
        GroupExpression() = default;
        /**
         * Creates a new expression containing a single term.
         *
         * \param term the term to use as the new expression.
         */
        GroupExpression(const GroupExpressionTerm& term);
        /**
         * Creates a new expression containing a single term.
         *
         * \param generator the number of the generator to use in the term.
         * \param exponent the exponent to which the given generator is
         * raised in the term.
         */
        GroupExpression(unsigned long generator, long exponent);
        /**
         * Creates a new expression that is a clone of the given expression.
         */
        GroupExpression(const GroupExpression&) = default;
        /**
         * Moves the contents of the given expression to this new expression.
         * This is a fast (constant time) operation.
         *
         * The expression that was passed will no longer be usable.
         */
        GroupExpression(GroupExpression&&) noexcept = default;
        /**
         * Attempts to interpret the given input string as a word in a group.
         * Regina can recognise strings in the following four basic forms:
         *
         * - `a^7b^-2`
         * - `aaaaaaaBB`
         * - `a^7B^2`
         * - `g0^7g1^-2`
         *
         * The string may contain whitespace, which will simply be ignored.
         * The empty string will be treated as an expression with no terms.
         *
         * Note that generators are numbered starting from 0.  This means,
         * for example, that `a`, `b` and `c` correspond to `g0`, `g1` and `g2`
         * respectively.
         *
         * If the optional argument \a nGens is passed and is positive, then
         * this constructor will explicitly check that the given string only
         * uses generators 0,...,(\a nGens-1).
         *
         * \exception InvalidArgument The given string could not be
         * interpreted as a group expression, or else \a nGens was positive
         * and the given string contains an out-of-range generator.
         *
         * \param input the input string that is to be interpreted.
         * \param nGens the number of generators in the group presentation.
         * If this is 0 (the default), then this argument will be ignored and
         * this constructor will not check whether generators are within range.
         */
        GroupExpression(const char* input, unsigned long nGens = 0);
        /**
         * Attempts to interpret the given input string as a word in a group.
         * Regina can recognise strings in the following four basic forms:
         *
         * - `a^7b^-2`
         * - `aaaaaaaBB`
         * - `a^7B^2`
         * - `g0^7g1^-2`
         *
         * The string may contain whitespace, which will simply be ignored.
         * The empty string will be treated as an expression with no terms.
         *
         * Note that generators are numbered starting from 0.  This means,
         * for example, that `a`, `b` and `c` correspond to `g0`, `g1` and `g2`
         * respectively.
         *
         * If the optional argument \a nGens is passed and is positive, then
         * this constructor will explicitly check that the given string only
         * uses generators 0,...,(\a nGens-1).
         *
         * \exception InvalidArgument The given string could not be
         * interpreted as a group expression, or else \a nGens was positive
         * and the given string contains an out-of-range generator.
         *
         * \param input the input string that is to be interpreted.
         * \param nGens the number of generators in the group presentation.
         * If this is 0 (the default), then this argument will be ignored and
         * this constructor will not check whether generators are within range.
         */
        GroupExpression(const std::string &input, unsigned long nGens = 0);

        /**
         * Makes this expression a clone of the given expression.
         *
         * \return a reference to this expression.
         */
        GroupExpression& operator = (const GroupExpression&) = default;
        /**
         * Moves the contents of the given expression to this expression.
         * This is a fast (constant time) operation.
         *
         * The expression that was passed will no longer be usable.
         *
         * \return a reference to this expression.
         */
        GroupExpression& operator = (GroupExpression&&) noexcept = default;

        /**
         * Swaps the contents of this and the given expression.
         *
         * \param other the expression whose contents should be swapped with
         * this.
         */
        void swap(GroupExpression& other) noexcept;

        /**
         * Equality operator. Checks to see whether or not these two words
         * represent the same literal string.
         *
         * \param comp the expression to compare against this.
         * \return \c true if this and the given string literal are identical.
         */
        bool operator == (const GroupExpression& comp) const;

        /**
         * Inequality operator. Checks to see whether or not these two words
         * represent different literal strings.
         *
         * \param comp the expression to compare against this.
         * \return \c true if this and the given string literal are not
         * identical.
         */
        bool operator != (const GroupExpression& comp) const;

        /**
         * Returns the list of terms in this expression.
         * These are the actual terms stored internally; any
         * modifications made to this list will show up in the
         * expression itself.
         *
         * For instance, the expression `g1^2 g3^-1 g6` has list
         * consisting of three terms `g1^2`, `g3^-1` and
         * `g6^1` in that order.
         *
         * \python The list itself is not returned by reference
         * (instead this routine returns a new Python list).  However,
         * the terms within this list are still returned by reference
         * (i.e., you can use the elements of this list to modify each
         * term individually).
         *
         * \return the list of terms.
         */
        std::list<GroupExpressionTerm>& terms();
        /**
         * Returns a constant reference to the list of terms in this
         * expression.
         *
         * For instance, the expression `g1^2 g3^-1 g6` has list
         * consisting of three terms `g1^2`, `g3^-1` and
         * `g6^1` in that order.
         *
         * \python The list itself is not returned by reference
         * (instead this routine returns a new Python list).  However,
         * the terms within this list are still returned by reference.
         *
         * \return the list of terms.
         */
        const std::list<GroupExpressionTerm>& terms() const;
        /**
         * Returns the number of terms in this expression.
         *
         * For instance, the expression `g1^2 g3^-1 g6` contains three
         * terms.  See also wordLength().
         *
         * \return the number of terms.
         */
        size_t countTerms() const;
        /**
         * Returns the length of the word, i.e. the number of letters
         * with exponent +1 or -1 for which this word is expressable as a
         * product.
         *
         * For instance, the expression `g1^2 g3^-1 g6` is a word of
         * length four.  See also countTerms().
         *
         * No attempt is made to remove redundant terms (so the word
         * `g g^-1` will count as length two).
         *
         * \return the length of the word.
         */
        size_t wordLength() const;
        /**
         * Tests whether this is the trivial (unit) word.
         *
         * No attempt is made to remove redundant terms (so the word
         * `g g^-1` will be treated as non-trivial).
         *
         * \return \c true if and only if this is the trivial word.
         */
        bool isTrivial() const;

        /**
         * Erases all terms from this this word.
         * This effectively turns this word into the identity element.
         */
        void erase();

        /**
         * Returns the term at the given index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is O(\a n) where \a n is the number
         * of terms in this expression.
         *
         * \param index the index of the term to return; this must be
         * between 0 and countTerms()-1 inclusive.
         * \return the requested term.
         */
        GroupExpressionTerm& term(size_t index);
        /**
         * Returns a constant reference to the term at the given
         * index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is O(\a n) where \a n is the number
         * of terms in this expression.
         *
         * \param index the index of the term to return; this must be
         * between 0 and countTerms()-1 inclusive.
         * \return the requested term.
         */
        const GroupExpressionTerm& term(size_t index) const;
        /**
         * Returns the generator corresonding to the
         * term at the given index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is O(\a n) where \a n is the number
         * of terms in this expression.
         *
         * \param index the index of the term to return; this must be
         * between 0 and countTerms()-1 inclusive.
         * \return the number of the requested generator.
         */
        unsigned long generator(size_t index) const;
        /**
         * Returns the exponent corresonding to the
         * term at the given index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is O(\a n) where \a n is the number
         * of terms in this expression.
         *
         * \param index the index of the term to return; this must be
         * between 0 and countTerms()-1 inclusive.
         * \return the requested exponent.
         */
        long exponent(size_t index) const;

        /**
         * Adds the given term to the beginning of this expression.
         *
         * \param term the term to add.
         */
        void addTermFirst(const GroupExpressionTerm& term);
        /**
         * Adds the given term to the beginning of this expression.
         *
         * \param generator the number of the generator corresponding to
         * the new term.
         * \param exponent the exponent to which the given generator is
         * raised.
         */
        void addTermFirst(unsigned long generator, long exponent);
        /**
         * Adds the given term to the end of this expression.
         *
         * \param term the term to add.
         */
        void addTermLast(const GroupExpressionTerm& term);
        /**
         * Adds the given term to the end of this expression.
         *
         * \param generator the number of the generator corresponding to
         * the new term.
         * \param exponent the exponent to which the given generator is
         * raised.
         */
        void addTermLast(unsigned long generator, long exponent);

        /**
         * Multiplies this expression on the left by the given word.
         * This expression will be modified directly.
         *
         * \param word the word to multiply with this expression.
         */
        void addTermsFirst(GroupExpression word);
        /**
         * Multiplies this expression on the right by the given word.
         * This expression will be modified directly.
         *
         * \param word the word to multiply with this expression.
         */
        void addTermsLast(GroupExpression word);

        /**
         * Cycles this word by moving the leftmost term around to the rightmost.
         * All other terms shift one step to the left.
         *
         * If the word is of the form
         * `g_i1^j1 g_i2^j2 ... g_in^jn`,
         * this converts it into the word
         * `g_i2^j2 ... g_in^jn g_i1^j1`.
         */
        void cycleRight();

        /**
         * Cycles this word by moving the rightmost term around to the leftmost.
         * All other terms shift one step to the right.
         *
         * If the word is of the form
         * `g_i1^j1 g_i2^j2 ... g_in^jn`,
         * this converts it into the word
         * `g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1`.
         */
        void cycleLeft();

        /**
         * Returns the inverse of this expression.
         * The terms will be reversed and the exponents negated.
         *
         * \return the inverse of this expression.
         */
        GroupExpression inverse() const;

        /**
         * Inverts this expression.  Does not allocate or deallocate anything.
         */
        void invert();

        /**
         * Returns this expression raised to the given power.
         * The given exponent may be positive, zero or negative.
         *
         * \param exponent the power to which this expression should be raised.
         * \return this expression raised to the given power.
         */
        GroupExpression power(long exponent) const;
        /**
         * Simplifies this expression.
         * Adjacent powers of the same generator will be combined, and
         * terms with an exponent of zero will be removed.
         * Note that it is _not_ assumed that the underlying group is
         * abelian.
         *
         * You may declare that the expression is cyclic, in which case
         * it is assumed that terms may be moved from the back to the
         * front and vice versa.  Thus expression `g1 g2 g1 g2 g1`
         * simplifies to `g1^2 g2 g1 g2` if it is cyclic, but does not
         * simplify at all if it is not cyclic.
         *
         * \param cyclic \c true if and only if the expression may be
         * assumed to be cyclic.
         * \return \c true if and only if this expression was changed.
         */
        bool simplify(bool cyclic = false);
        /**
         * Replaces every occurrence of the given generator with the
         * given substitute expression.  If the given generator was found,
         * the expression will be simplified once the substitution is
         * complete.
         *
         * \pre The given expansion is not the same GroupExpression
         * object as this.
         *
         * \param generator the generator to be replaced.
         * \param expansion the substitute expression that will replace
         * every occurrence of the given generator.
         * \param cyclic \c true if and only if the expression may be
         * assumed to be cyclic; see simplify() for further details.
         * \return \c true if and only if any substitutions were made.
         */
        bool substitute(unsigned long generator,
            const GroupExpression& expansion, bool cyclic = false);
        /**
         * Replaces every generator in this expression with the
         * corresponding substitute expression from the given map.
         *
         * Specifically, each generator \a i will be replaced with the
         * expression `expansions[i]`.
         *
         * The expression will be simplified once all substitutions are
         * complete.
         *
         * Unlike the single-generator verison of substitute(), it is
         * perfectly fine if this GroupExpression object appears in the
         * \a expansions list, and/or if the same GroupExpression object
         * appears several times in the given list.
         *
         * \pre The length of \a expansions is at least <i>g</i>+1,
         * where \a g is the largest generator that appears in this expression.
         * In other words, `expansions[i]` exists for every generator
         * \a i that appears in this expression.
         *
         * \param expansions the list of substitutes for all generators in
         * this expression.
         * \param cyclic \c true if and only if the expression may be
         * assumed to be cyclic; see simplify() for further details.
         */
        void substitute(const std::vector<GroupExpression>& expansions,
            bool cyclic = false);

        /**
         * Determines whether or not one can relabel the generators in
         * this word to obtain the given other word. If so, returns a non-empty
         * list of all such relabellings.  If not, returns an empty list.
         *
         * Relabellings are partially-defined permutations on the
         * generator set, also allowing for possible inversions if
         * cyclic is \c true.
         *
         * \apinotfinal
         *
         * \todo Change this to use less heavyweight types and less deep
         * copying.
         *
         * \pre If \a cyclic is \c true, then both this word and \a other
         * have been cyclically reduced.
         *
         * \nopython
         *
         * \param other the word to compare against this.
         * \param cyclic if \c false we get a list of exact relabellings from
         * this word to \a other.  If \c true, it can be up to cyclic
         * permutation and inversion.
         * \return a list of permutations, implemented as maps from
         * generator indices of this word to generator indices of \a other.
         */
        std::list< std::map< unsigned long, GroupExpressionTerm > >
            relabellingsThisToOther( const GroupExpression &other,
            bool cyclic=false ) const;

        /**
         * Writes a chunk of XML containing this expression.
         *
         * \python The argument \a out should be an open Python file
         * object.
         *
         * \param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * Returns a TeX representation of this expression.
         *
         * The text representation will be of the form `g_2^4 g_{13}^{-5} g_4`.
         *
         * \return a TeX representation of this expression.
         */
        std::string tex() const;

        /**
         * Writes a TeX represesentation of this expression to the given
         * output stream.
         *
         * The text representation will be of the form `g_2^4 g_{13}^{-5} g_4`.
         *
         * \nopython Instead use the variant tex() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         */
        void writeTeX(std::ostream& out) const;

        using ShortOutput<GroupExpression, true>::str;
        using ShortOutput<GroupExpression, true>::utf8;

        /**
         * Returns a short text representation of this group expression, with
         * a choice of either numbered generators or alphabetic generators.
         *
         * If \a alphaGen is \c false, the text representation will be of
         * the form `g2^4 g13^-5 g4`.  If \a alphaGen is \c true,
         * this routine will assume your word is in an alphabet of no more
         * than 26 letters, and will format the word using lower-case ASCII,
         * i.e., `c^4 n^-5 e`.
         *
         * Note that there is also a zero-argument version of str(), inherited
         * through the ShortOutput base class.  This zero-argument str()
         * gives the same output as `str(false)`.
         *
         * Note that generators are numbered starting from 0.  This means,
         * for example, that `a`, `b` and `c` correspond to `g0`, `g1` and `g2`
         * respectively.
         *
         * \pre If \a alphaGen is \c true, the number of generators in
         * the corresponding group must be 26 or fewer.
         *
         * \param alphaGen indicates whether to use numbered or
         * alphabetic generators, as described above.
         * \return a short text representation of this group expression.
         */
        std::string str(bool alphaGen) const;

        /**
         * Returns a short text representation of this group expression using
         * unicode characters, with a choice of either numbered generators or
         * alphabetic generators.
         *
         * This outputs a similar text representation to str(bool), except that
         * all exponents will be written using superscript characters encoded
         * in UTF-8.  See str(bool) for further details.
         *
         * Note that there is also a zero-argument version of utf8(), inherited
         * through the ShortOutput base class.  This zero-argument utf8()
         * gives the same output as `utf8(false)`.
         *
         * \pre If \a alphaGen is \c true, the number of generators in
         * the corresponding group must be 26 or fewer.
         *
         * \param alphaGen indicates whether to use numbered or
         * alphabetic generators, as described above.
         * \return a short text representation of this group expression.
         */
        std::string utf8(bool alphaGen) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream, using either numbered generators or
         * alphabetic generators.
         *
         * The text representation will be of the form
         * `g2^4 g13^-5 g4`. If the \a alphaGen flag is \c true, it
         * will assume your word is in an alphabet of no more than 26 letters,
         * and will write the word using lower-case ASCII, i.e.,
         * `c^4 n^-5 e`.  If the \a utf8 flag is \c true, all exponents
         * will be written using superscript characters encoded in UTF-8.
         *
         * Note that generators are numbered starting from 0.  This means,
         * for example, that `a`, `b` and `c` correspond to `g0`, `g1` and `g2`
         * respectively.
         *
         * \pre If \a alphaGen is \c true, the number of generators in
         * the corresponding group must be 26 or fewer.
         *
         * \nopython Use str() or utf8() instead.
         *
         * \param out the output stream to which to write.
         * \param utf8 \c true if exponents should be written using
         * unicode superscript characters, or \c false if they should be
         * written using a caret (^) symbol.
         * \param alphaGen indicates whether to use numbered or
         * alphabetic generators, as described above.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
            bool alphaGen = false) const;
};

/**
 * Swaps the contents of the two given expressions.
 *
 * This global routine simply calls GroupExpression::swap(); it is provided
 * so that GroupExpression meets the C++ Swappable requirements.
 *
 * \param lhs the expression whose contents should be swapped with \a rhs.
 * \param rhs the expression whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(GroupExpression& lhs, GroupExpression& rhs) noexcept;

/**
 * Represents a finite presentation of a group.
 *
 * A presentation consists of a number of generators and a set of
 * relations between these generators that together define the group.
 *
 * If there are \a g generators, they will be numbered 0, 1, ..., <i>g</i>-1.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo Let's make intelligent simplify a tad more intelligent, and the GUI
 * call a bit more safe.  Perhaps parallelize the GUI call, and give users
 * parameters to ensure it won't crash the computer.  Also look at the FPGroup
 * package. We should also have a simple way of creating GroupPresentation
 * objects directly from text strings.  We would like to have something like
 * GroupPresentation( numGens, "abAAB", "bccd" ) etc., with arbitrary
 * numbers of relators. Maybe std::tuple.  Or "variadic templates"?
 *
 * \ingroup algebra
 */
class GroupPresentation : public Output<GroupPresentation> {
    protected:
        unsigned long nGenerators_;
            /**< The number of generators. */
        std::vector<GroupExpression> relations_;
            /**< The relations between the generators. */

    public:
        /**
         * Creates a new presentation with no generators and no relations.
         */
        GroupPresentation();
        /**
         * Creates a clone of the given group presentation.
         *
         * \param src the group presentation to clone.
         */
        GroupPresentation(const GroupPresentation& src) = default;
        /**
         * Moves the contents of the given group presentation to this new
         * group presentation.  This is a fast (constant time) operation.
         *
         * The group presentation that was passed (\a src) will no longer be
         * usable.
         *
         * \param src the group presentation to move.
         */
        GroupPresentation(GroupPresentation&& src) noexcept = default;
        /**
         * Creates the free group on the given number of generators.
         *
         * \param nGenerators the number of generators.
         */
        GroupPresentation(unsigned long nGenerators);
        /**
         * Constructor that allows you to directly pass an arbitrary number
         * of relators in string format.
         *
         * The first argument \a nGens is the number of generators one wants
         * the group to have. The second argument \a rels is a vector
         * of strings, where each string gives a single relator.  See
         * the GroupExpression::GroupExpression(const std::string&)
         * constructor notes for information on what format these strings
         * can take.
         *
         * If you wish to create a group presentation from a hard-coded list
         * of relations, you can use this constructor with an initialiser list,
         * via syntax of the form
         * `GroupPresentation(nGens, { "rel1", "rel2", ... })`.
         *
         * \exception InvalidArgument One or more of the given strings
         * could not be interpreted as a group expression, and/or contains
         * an out-of-range generator.
         *
         * \param nGens the number of generators.
         * \param rels a vector of relations each given in string form,
         * as outlined above.
         */
        GroupPresentation(unsigned long nGens,
                const std::vector<std::string> &rels);

        /**
         * Sets this to be a clone of the given group presentation.
         *
         * \param src the group presentation to copy.
         * \return a reference to this group presentation.
         */
        GroupPresentation& operator = (const GroupPresentation& src) = default;
        /**
         * Moves the contents of the given group presentation to this
         * group presentation.  This is a fast (constant time) operation.
         *
         * The group presentation that was passed (\a src) will no longer be
         * usable.
         *
         * \param src the group presentation to move.
         * \return a reference to this group presentation.
         */
        GroupPresentation& operator = (GroupPresentation&& src) noexcept =
            default;

        /**
         * Swaps the contents of this and the given group presentation.
         *
         * \param other the group presentation whose contents should be
         * swapped with this.
         */
        void swap(GroupPresentation& other) noexcept;

        /**
         * Adds one or more generators to the group presentation.
         * If the new presentation has \a g generators, the new
         * generators will be numbered <i>g</i>-1, <i>g</i>-2 and so on.
         *
         * \param numToAdd the number of generators to add.
         * \return the number of generators in the new presentation.
         */
        unsigned long addGenerator(unsigned long numToAdd = 1);
        /**
         * Adds the given relation to the group presentation.
         * The relation must be of the form `expression = 1`.
         *
         * \warning This routine does not check whether or not your relation
         * is a word only in the generators of this group.  In other
         * words, it does not stop you from using generators beyond the
         * countGenerators() bound.
         *
         * \param rel the expression that the relation sets to 1; for
         * instance, if the relation is `g1^2 g2 = 1` then this
         * parameter should be the expression `g1^2 g2`.
         */
        void addRelation(GroupExpression rel);

        /**
         * Returns the number of generators in this group presentation.
         *
         * \return the number of generators.
         */
        unsigned long countGenerators() const;
        /**
         * Returns the number of relations in this group presentation.
         *
         * \return the number of relations.
         */
        size_t countRelations() const;
        /**
         * Returns the relation at the given index in this group
         * presentation.  The relation will be of the form `expresson = 1`.
         *
         * \param index the index of the desired relation; this must be
         * between 0 and countRelations()-1 inclusive.
         *
         * \return the expression that the requested relation sets to 1;
         * for instance, if the relation is `g1^2 g2 = 1` then
         * this will be the expression `g1^2 g2`.
         */
        const GroupExpression& relation(size_t index) const;
        /**
         * Returns the list of all relations in this group presentation.
         *
         * \python This routine returns a python list of copied
         * GroupExpression objects.  In particular, modifying this
         * list or the relations within it will not modify the group
         * presentation from which they came.
         *
         * \python The list itself is not returned by reference
         * (instead this routine returns a new Python list).  However,
         * the relations within this list are still returned by reference.
         *
         * \return the list of relations.
         */
        const std::vector<GroupExpression>& relations() const;

        /**
         * Tests whether all of the relations for the group are indeed words
         * in the generators.  This routine returns \c false if at least
         * one relator uses an out-of-bound generator, and \c true otherwise.
         *
         * This routine is intended only for sanity checking: you should
         * never have an invalid group presentation in the first place.
         *
         * \return \c true if and only if all of the relations are words
         * in the generators.
         */
        bool isValid() const;

        /**
         * Attempts to simplify the group presentation as intelligently
         * as possible without further input.
         *
         * The current simplification method uses a combination of small
         * cancellation theory and Nielsen moves.
         *
         * If this routine does return a homomorphism (because the
         * presentation was changed), then this homomorphsm will in fact be
         * a declared isomorphism.  See the HomGroupPresentation class
         * notes for details on what this means.
         *
         * \note If you all care about is whether the presentation changed,
         * you can simply cast the return value to a \c bool.  This will
         * then mirror the behaviour of intelligentSimplify() from Regina 6.0
         * and earlier, when the return type was simply \c bool.
         *
         * \return an isomorphism describing the reduction map from the
         * original presentation to the new presentation, or \nullopt if
         * this presentation was not changed.
         */
        std::optional<HomGroupPresentation> intelligentSimplify();

        /**
         * Attempts to simplify the group presentation using small cancellation
         * theory. The simplification method is based on the Dehn algorithm
         * for hyperbolic groups, i.e. small cancellation theory.   This means
         * we look to see if part of one relator can be used to simplify
         * others.  If so, make the substitution and simplify.  We continue
         * until no more presentation-shortening substitutions are available.
         * We follow that by killing any available generators using words
         * where generators appear a single time.
         *
         * If this routine does return a homomorphism (because the
         * presentation was changed), then this homomorphsm will in fact be
         * a declared isomorphism.  See the HomGroupPresentation class
         * notes for details on what this means.
         *
         * \note If you all care about is whether the presentation changed,
         * you can simply cast the return value to a \c bool.  This will
         * then mirror the behaviour of smallCancellation() from Regina 6.0
         * and earlier, when the return type was simply \c bool.
         *
         * \todo \optlong This routine could use some small tweaks -
         * recognition of utility of some score==0 moves, such as
         * commutators, for example.
         *
         * \return an isomorphism describing the reduction map from the
         * original presentation to the new presentation, or \nullopt if
         * this presentation was not changed.
         */
        std::optional<HomGroupPresentation> smallCancellation();

        /**
         * Uses small cancellation theory to reduce the input word,
         * modulo conjugation, using the current presentation of the group.
         * The input word will be modified directly.
         *
         * By "modulo conjugation", we mean: if \a w represents the input word,
         * then this routine might (as part of the reduction process) transform
         * \a w into a different group element of the form `g w g^-1`.
         *
         * \warning This routine is only as good as the relator table for the
         * group.  You might want to consider running intelligentSimplify(),
         * possibly in concert with proliferateRelators(), before using this
         * routine for any significant tasks.
         *
         * \param input is the word you would like to simplify.
         * This must be a word in the generators of this group.
         * \return \c true if and only if the input word was modified.
         */
        bool simplifyWord(GroupExpression &input) const;

        /**
         * A routine to help escape local wells when simplifying
         * presentations, which may be useful when small cancellation theory
         * can't find the simplest relators.
         *
         * Given a presentation `<g_i | r_i>`, this routine appends
         * consequences of the relators {r_i} to the presentation that
         * are of the form ab, where both a and b are cyclic permutations
         * of relators from the collection {r_i}.
         *
         * Passing depth=1 means it will only form products of two
         * relators.  Depth=2 means products of three, etc.  Depth=4 is
         * typically the last depth before the exponential growth of
         * the operation grows out of hand.  It also conveniently trivializes
         * all the complicated trivial group presentations that we've come
         * across so far.
         *
         * \warning Do not call this routine with depth n before having called
         * it at depth n-1 first.  Depth=0 is invalid, and depth=1 should be
         * your first call to this routine.  This routine gobbles up an
         * exponential amount of memory (exponential in your presentation
         * size times n).  So do be careful when using it.
         *
         * \param depth controls the depth of the proliferation, as
         * described above; this must be strictly positive.
         */
        void proliferateRelators(unsigned long depth=1);

        /**
         * Attempts to recognise the group corresponding to this
         * presentation.  This routine is much more likely to be
         * successful if you have already called intelligentSimplify().
         *
         * Currently, the groups this routine recognises include:
         * the trivial group, abelian groups, free groups,
         * extensions over the integers, and free products of any group
         * the algorithm can recognise (inductively).
         *
         * The string returned from this routine may use some unicode
         * characters, which will be encoding using UTF-8.  If \a moreUtf8
         * is passed as \c false then unicode will be used sparingly;
         * if \a moreUtf8 is \c true then unicode will be use more liberally,
         * resulting in strings that look nicer but require more complex
         * fonts to be available on the user's machine.
         *
         * Examples of the format of the returned string are:
         *
         * - `0` for the trivial group;
         * - `Z_n` for cyclic groups with \a n > 1;
         * - `Free(n)` for free groups with \a n > 1 generators - see
         *   AbelianGroup::str() for how abelian groups are presented;
         * - `FreeProduct(G1, G2, ... , Gk)` for free products, where
         *   one replaces \a G1 through \a Gk by text strings representing the
         *   free summands;
         * - `Z~G w/ monodromy H` for extensions over Z,
         *   where \a G is a description of the kernel of the homomorphism
         *   to the integers, and \a H is a text string representing the
         *   monodromy - see HomMarkedAbelianGroup.str() for details on
         *   how these are presented.
         *
         * \todo \featurelong Make this recognition more effective.
         *
         * \return a simple string representation of the group if it is
         * recognised, or an empty string if the group is not
         * recognised.
         */
        std::string recogniseGroup(bool moreUtf8 = false) const;

        /**
         * Writes a chunk of XML containing this group presentation.
         *
         * \python The argument \a out should be an open Python file
         * object.
         *
         * \param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * The sum of the word lengths of the relators.
         * Word lengths are computing using GroupExpression::wordLength().
         * Used as a coarse measure of the complexity of the presentation.
         *
         * \return the sum of word lengths.
         */
        size_t relatorLength() const;

        /**
         * Computes the abelianisation of this group.
         *
         * \return the abelianisation of this group.
         */
        AbelianGroup abelianisation() const;

        /**
         * Computes the rank of the abelianisation of this group.
         * This is the number of \a Z summands in the abelianisation
         * (i.e., ignoring any torsion summands).
         *
         * This is much less informative than computing the full
         * abelianisation, but in some cases it might be significantly faster
         * (since it involves just a matrix rank computation as opposed to a
         * Smith normal form).
         *
         * The result of this routine should be the same as the output
         * of `abelianisation().rank()`.
         *
         * \return the rank of the abelianisation of this group.
         */
        unsigned long abelianRank() const;

        /**
         * Computes the abelianisation of this group.
         * The coordinates in the chain complex correspond
         * to the generators and relators for this group.
         *
         * \return the abelianisation of this group.
         */
        MarkedAbelianGroup markedAbelianisation() const;

        /**
         * Attempts to determine if the group is abelian.
         *
         * A return value of \c true indicates that this routine
         * successfully certified that the group is abelian.
         * A return value of \c false indicates an inconclusive result:
         * either the group is non-abelian, or the group
         * is abelian but this routine could not prove so.
         *
         * If the group is abelian, then markedAbelianization() is the easiest
         * way to see precisely
         * which abelian group it is, and how the generators sit in that group.
         *
         * You will have better results from this algorithm if the
         * presentation has been simplified, since this algorithm uses small
         * cancellation theory in an attempt to reduce the commutators of all
         * pairs of generators.
         *
         * \warning If you have not adequately simplified this presentation
         * this routine will most likely return \c false.  Consider running
         * intelligentSimplify, possibly in concert with proliferateRelators(),
         * in order to discover adequately many commutators.
         *
         * \return \c true if the group is shown to be abelian, or
         * \c false if the result is inconclusive.
         */
        bool identifyAbelian() const;

        /**
         * Switches the generators in the presentation indexed by \a i
         * and \a j respectively, and recomputes the appropriate presentation.
         * It is one of the standard Nielsen moves, which is the first of
         * three generator types of the automorphism group of a free group.
         *
         * \pre Both \a i and \a j are strictly less than
         * countGenerators().
         *
         * \param i indicates the first of the two generators to switch.
         * \param j indicates the second of the two generators to switch.
         * \return \c true if and only if the Nielsen automorphism had an
         * effect on at least one relation.
         */
        bool nielsenTransposition(unsigned long i, unsigned long j);

        /**
         * Replaces a generator in a presentation by its inverse, and
         * recomputes the appropriate presentation. This is the second
         * generator type of the automorphism group of a free group.
         *
         * \pre \a i is strictly less than countGenerators().
         *
         * \param i indicates the generator to invert.
         * \return \c true if and only if the Nielsen automorphism had an
         * effect on at least one relation.
         */
        bool nielsenInvert(unsigned long i);

        /**
         * Replaces a generator \c gi by either
         * `(gi)(gj)^k` or `(gj)^k(gi)` in the presentation. It
         * it is the third type of Nielsen move one can apply to a presentation.
         *
         * This means that, if the new generator \c Gi is the old
         * `(gi)(gj)^k` or `(gj)^k(gi)`, then we can construct
         * the new presentation from the old by replacing occurrences of \c Gi
         * by `(Gi)(gj)^(-k)` or `(gj)^(-k)(Gi)` respectively.
         *
         * \pre Both \a i and \a j are strictly less than countGenerators().
         *
         * \param i indicates the generator to replace.
         * \param j indicates the generator to combine with \c gi.
         * \param k indicates the power to which we raise \c gj when
         * performing the replacement; this may be positive or negative
         * (or zero, but this will have no effect).
         * \param rightMult \c true if we should replace \c gi by
         * `(gi)(gj)^k`, or \c false if we should replace \c gi by
         * `(gj)^k(gi)`.
         * \return \c true if and only if the nielsen automorphism had an
         * effect on at least one relation.
         */
        bool nielsenCombine(unsigned long i, unsigned long j,
                long k, bool rightMult=true);

        /**
         * Looks for Nielsen moves that will simplify the presentation.
         * Performs one of the most-effective moves, if it can find any.
         *
         * If this routine does return a homomorphism (because some
         * move was performed), then this homomorphsm will in fact be
         * a declared isomorphism.  See the HomGroupPresentation class
         * notes for details on what this means.
         *
         * \note If you all care about is whether the presentation changed,
         * you can simply cast the return value to a \c bool.  This will
         * then mirror the behaviour of intelligentNielsen() from Regina 6.0
         * and earlier, when the return type was simply \c bool.
         *
         * \return an isomorphism describing the map from the
         * original presentation to the new presentation, or \nullopt if
         * this presentation was not changed.
         */
        std::optional<HomGroupPresentation> intelligentNielsen();

        /**
         * Rewrites the presentation so that generators
         * of the group map to generators of the abelianisation, with any
         * left-over generators mapping to zero (if possible).  Consider this a
         * _homological-alignment_ of the presentation.
         *
         * If the abelianisation of this group has rank \a N and \a M invariant
         * factors `d0 | d2 | ... | d(M-1)`,
         * this routine applies Nielsen moves
         * to the presentation to ensure that under the markedAbelianisation()
         * routine, generators 0 through \a M-1 are mapped to generators of the
         * relevant \c Z_di group.  Similarly, generators \a M through
         * <i>M</i>+<i>N</i>-1 are mapped to ±1 in the appropriate factor.
         * All further generators will be mapped to zero.
         *
         * If this routine does return a homomorphism (because the
         * presentation was changed), then this homomorphsm will in fact be
         * a declared isomorphism.  See the HomGroupPresentation class
         * notes for details on what this means.
         *
         * \note If you all care about is whether the presentation changed,
         * you can simply cast the return value to a \c bool.  This will
         * then mirror the behaviour of homologicalAlignment() from Regina 6.0
         * and earlier, when the return type was simply \c bool.
         *
         * \return an isomorphism describing the reduction map from the
         * original presentation to the new presentation, or \nullopt if
         * this presentation was not changed.
         */
        std::optional<HomGroupPresentation> homologicalAlignment();

        /**
         * An entirely cosmetic re-writing of the presentation, which is
         * fast and superficial.
         *
         * -# If there are any length 1 relators, those generators are
         *    deleted, and the remaining relators simplified.
         * -# It sorts the relators by number of generator indices that
         *    appear, followed by relator numbers (lexico) followed by
         *    relator length.
         * -# It inverts relators if the net sign of the generators is
         *    negative.
         * -# Given each generator, it looks for the smallest word where that
         *    generator appears with non-zero weight.  If negative weight,
         *    it inverts that generator.
         * -# It cyclically permutes relators to start with smallest gen.
         *
         * If this routine does return a homomorphism (because the choice of
         * generators was changed), then this homomorphsm will in fact be
         * a declared isomorphism.  See the HomGroupPresentation class
         * notes for details on what this means.
         *
         * \note If you all care about is whether the presentation changed,
         * you can simply cast the return value to a \c bool.  This will
         * then mirror the behaviour of prettyRewriting() from Regina 6.0
         * and earlier, when the return type was simply \c bool.
         *
         * \todo As a final step, make elementary simplifications to aid in
         * seeing standard relators like commutators.
         *
         * \return an isomorphism describing the map from the
         * original presentation to the new presentation, or \nullopt if
         * this presentation was not changed.
         */
        std::optional<HomGroupPresentation> prettyRewriting();

        /**
         * Determines whether this and the given group presentation are
         * identical.
         *
         * This routine does _not_ test for isomorphism (which in
         * general is an undecidable problem).  Instead it tests whether
         * this and the given presentation use exactly the same generators
         * and exactly the same relations, presented in exactly the same order.
         *
         * \param other the group presentation to compare with this.
         * \return \c true if and only if this and the given group presentation
         * are identical.
         */
        bool operator == (const GroupPresentation& other) const;

        /**
         * Determines whether this and the given group presentation are
         * not identical.
         *
         * This routine does _not_ test for isomorphism (which in
         * general is an undecidable problem).  Instead it tests whether
         * this and the given presentation use exactly the same generators
         * and exactly the same relations, presented in exactly the same order.
         *
         * \param other the group presentation to compare with this.
         * \return \c true if and only if this and the given group presentation
         * are not identical.
         */
        bool operator != (const GroupPresentation& other) const;

        /**
         * Attempts to prove that this and the given group presentation are
         * _simply isomorphic_.
         *
         * A _simple isomorphism_ is an isomorphism where each generator
         * <i>g<sub>i</sub></i> of this presentation is sent to
         * some generator <i>g<sub>j</sub></i><sup>±1</sup> of the
         * other presentation.  Moreover, at present this routine only
         * looks for maps where both presentations have the same number
         * of generators, and where distinct generators <i>g<sub>i</sub></i>
         * of this presentation correspond to distinct generators
         * <i>g<sub>j</sub></i> of the other presentation (possibly with
         * inversion, as noted above).
         *
         * If this routine returns \c true, it means that the two
         * presentations are indeed simply isomorphic.
         *
         * If this routine returns \c false, it could mean one of many
         * things:
         *
         * - The groups are not isomorphic;
         * - The groups are isomorphic, but not simply isomorphic;
         * - The groups are simply isomorphic but this routine could not
         *   prove it, due to difficulties with the word problem.
         *
         * \param other the group presentation to compare with this.
         * \return \c true if this routine could certify that the two group
         * presentations are simply isomorphic, or \c false if it could not.
         */
        bool identifySimplyIsomorphicTo(const GroupPresentation& other) const;

        /**
         * Enumerates all transitive representations of this group into
         * the symmetric group <i>S(k)</i>.  Each representation is
         * produced exactly once up to conjugacy.
         *
         * Each such representation corresponds to an index \a k subgroup,
         * and the multiset of the abelianisations of all these subgroups is
         * a group invariant that (for small enough \a k) can be computed
         * in reasonable time.
         *
         * If this is the fundamental group of a manifold, then each
         * such representation also corresponds to a connected
         * <i>k</i>-sheeted cover.
         *
         * For each representation that is produced, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a group presentation.
         *   This will be the index \a k subgroup corresponding to the
         *   representation.  This argument will be passed as an rvalue;
         *   a typical action could (for example) take it by const reference
         *   and query it, or take it by value and modify it, or take it by
         *   rvalue reference and move it into more permanent storage.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * - It is completely safe for \a action to (if you wish) make changes
         *   to the original presentation (i.e., the group presentation upon
         *   which enumerateCovers() is being called).  This will not interfere
         *   with the enumeration or change the results in any way.
         *
         * This routine produces a constant stream of output (i.e., it
         * calls \a action as soon as each representation is found).
         *
         * \warning The running time is `(k!)^g`, where \a k is the subgroup
         * index described above, and \a g is the number of generators of this
         * group presentation.  In particular, the running time grows
         * _extremely_ quickly with \a k.  Moreover, for larger indices this
         * routine may precompute some tables that will never be released
         * (but which only need to be computed the first time that index is
         * used); for index 10 these tables will consume roughly 30MB, and
         * for index 11 they will consume around 320MB.
         *
         * \warning This routine does _not_ simplify the group presentation
         * before it runs.  You should make sure that you have simplified
         * the presentation, either using Regina or some other tool, before
         * running this routine, since (as noted above) the running time
         * is exponential in the number of generators.
         *
         * \warning Likewise, this routine does not simplify the subgroup
         * presentations before passing them to \a action.  These presentations
         * can be quite large, and (for example) if all you care about is their
         * abelianisations then you are better off using the _abelian_ group
         * simplification / computation instead (which is much faster).
         *
         * \pre Arrays on this system can be large enough to store `2⋅(n-2)!`
         * objects.  This is a technical condition on the bit-size of \c size_t
         * that will be explicitly checked (with an exception thrown if it
         * fails).  On a 64-bit system this condition will be true for all
         * supported \a n, but on a 32-bit system or smaller it will mean that
         * enumerateCovers() cannot be used for larger values of \a n.
         *
         * \exception FailedPrecondition A signed integer of the same bit-size
         * as \c size_t cannot hold `2⋅(n-2)!`.  See the precondition above
         * for further discussion on this constraint.
         *
         * \apinotfinal
         *
         * \python There are two versions of this function available in
         * Python.  The first form is `enumerateCovers(index, action)`,
         * which mirrors the C++ function: it takes \a action which may
         * be a pure Python function, it returns the number of representations
         * found, but it does _not_ take an addition argument list (\a args).
         * The second form is `enumerateCovers(index)`, which returns
         * a Python list containing all of the corresponding subgroups, each
         * given as a GroupPresentation.  In both forms, the template
         * parameter \a index becomes the first argument to the Python function.
         *
         * \tparam index the number \a k in the description above; in other
         * words, the index of the resulting subgroups.  Currently this
         * must be between 2 and 11 inclusive; this range is limited because
         * some of the cached precomputations can consume a _lot_ of space for
         * larger indices.
         * \param action a function (or other callable object) to call
         * for each representation that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial subgroup presentation argument.
         * \return the total number of representations found.
         */
        template <int index, typename Action, typename... Args>
        size_t enumerateCovers(Action&& action, Args&&... args) const;

        /**
         * Returns a matrix indicating which generators are used by
         * which relations.
         *
         * The rows of the matrix correspond to the relations 0,1,...,
         * and the columns correspond to the generators 0,1,....
         * The matrix entry in row \a r, column \a g will be \c true
         * if and only if relation \a r uses generator \a g.
         *
         * \pre The numbers of generators and relations are both non-zero.
         *
         * \return the incidence matrix between relators and generators.
         */
        Matrix<bool> incidence() const;

        /**
         * Returns a TeX representation of this group presentation.
         *
         * The output will be of the form `< generators | relators >`.
         * There will be no final newline.
         *
         * \return a TeX representation of this group presentation.
         */
        std::string tex() const;

        /**
         * Writes a TeX represesentation of this group presentation to the
         * given output stream.  See tex() for details on how this is formed.
         *
         * \nopython Instead use the variant tex() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         */
        void writeTeX(std::ostream& out) const;

        /**
         * Returns a compact one-line representation of this group presentation,
         * including details of all generators and relations.
         *
         * The output will be of the form `< generators | relators >`.
         * The full relations will be included, and the entire output
         * will be written on a single line.  There will be no final newline.
         *
         * Currently str() and compact() are identical functions, though the
         * output from str() may change in future versions of Regina.
         *
         * \return a compact representation of this group presentation.
         */
        std::string compact() const;

        /**
         * Writes a compact one-line represesentation of this group to the given
         * output stream.  See compact() for details on how this is formed.
         *
         * Currently writeTextShort() and writeTextCompact() are identical
         * functions, though the output from writeTextShort() may change in
         * future versions of Regina.
         *
         * \nopython Instead use the variant compact() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         */
        void writeTextCompact(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * Currently writeTextShort() and writeTextCompact() are identical
         * functions, though the output from writeTextShort() may change in
         * future versions of Regina.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Returns a sequence of GAP commands that create this group.
         *
         * GAP is a widely-used computational algebra system, and can be
         * downloaded from http://gap-system.org/ .
         *
         * Other than the variable for the group itself, the commands
         * returned will not use or modify any other GAP variables with the
         * current GAP scope.
         *
         * The string that is returned will be presented as a single
         * (possibly very long) GAP function call, and will not contain
         * any newlines.
         *
         * \param groupVariable the name of the GAP variable to which
         * this group will be assigned.
         * \return a sequence of commands to create this group in GAP.
         */
        std::string gap(const std::string& groupVariable = "g") const;

#ifdef __DOXYGEN
        /**
         * A SageMath-only routine that returns a copy of this group
         * presentation in a format native to SageMath.
         *
         * \nocpp
         * \python Only present when run within SageMath.
         *
         * \return a copy of this group as a mathematical object native to
         * SageMath.
         */
        FinitelyPresentedGroup sage() const;
#endif

    private:
        /**
         * Attempts to rewrite the presentation as a group extension.
         * In particular, this routine attempts to rewrite this group
         * as a semi-direct product of the integers and another
         * finitely-presented group, i.e., an extension of the form:
         *
         *   < a, r1,...,rn | R1,...,RM, ar1a^-1 = w1, ... arna^-1 = wn >
         *
         * This is an algorithmic
         * implementation of the Reidemeister-Schrier algorithm, which isn't
         * actually an algorithm.  So sometimes this procedure works, and
         * sometimes it does not.  This function will return a value
         * if and only if the algorithm is successful.  Even if the algorithm
         * is unsuccessful, its application will likely result in a
         * modification of the presentation.
         *
         * \apinotfinal This routine may very well either be eliminated
         * in future versions of this software, perhaps incorporated into a
         * bigger-and-better future algorithm.
         *
         * \return a homomorphism if the algorithm was successful, or \nullopt
         * if it was not.  If a homomorphism is returned, it will be an
         * automorphism of a presentation of the kernel of the map this to the
         * integers.
         */
        std::optional<HomGroupPresentation> identifyExtensionOverZ();

        /**
         * Attempts to determine if this group is clearly a free
         * product of other groups.  This is an unsophisticated algorithm
         * and will likely only have success if one has pre-processed the
         * presentation with simplification routines beforehand.
         *
         * If this routine succeeds then the group is definitely a free
         * product.  If this routine fails (by returning an empty list)
         * then the result is inconclusive: the group might not be a
         * free product, or it might be a free product but this routine
         * could not prove so.
         *
         * \apinotfinal Reconsider how the end-user should see this routine.
         *
         * \return a list of group presentations giving the factors of this
         * free product, or an empty list if this routine fails (i.e., the
         * result is inconclusive).
         */
        std::list<GroupPresentation> identifyFreeProduct() const;

        /**
         * A structure internal to the small cancellation simplification
         * algorithm.
         *
         * Given two words, A and B, one wants to know how one can make
         * substitutions into A using variants of the word B.  This
         * structure holds that data.  For example, if:
         *
         *  A == a^5b^2abababa^4b^1  and  B == bababa^-1
         *    == aaaaabbabababaaaab
         * start_sub_at == 6, start_from == 0, sub_length == 5 makes sense,
         *  this singles out the subword aaaaab[babab]abaaaab. Since it would
         *  reduce the length by four, the score is 4.
         *
         * Similarly, if    A == baba^4b^1a^5b^2aba == babaaaabaaaaabbaba
         *   and    B == baba^-1ba start_sub_at == 14, start_from == 5,
         *   sub_length == 5 makes sense, and is a cyclic variation
         *   on the above substitution, so the score is also 4.
         */
        struct WordSubstitutionData {
            unsigned long start_sub_at;
                /**< Where in A do we start? */
            unsigned long start_from;
                /**< Where in B do we start? */
            unsigned long sub_length;
                /**< The number of letters from B to use. */
            bool invertB;
                /**< Invert B before making the substitution? */
            long int score;
                /**< The score, i.e., the decrease in the word letter count
                     provided this substitution is made. */

            bool operator<( const WordSubstitutionData &other ) const {
                if (score < other.score) return false;
                if (score > other.score) return true;
                if (sub_length < other.sub_length) return false;
                if (sub_length > other.sub_length) return true;
                if ( (invertB == true)  && (other.invertB == false) )
                        return false;
                if ( (invertB == false) && (other.invertB == true)  )
                        return true;
                if (start_from < other.start_from) return false;
                if (start_from > other.start_from) return true;
                if (start_sub_at < other.start_sub_at) return false;
                if (start_sub_at > other.start_sub_at) return true;
                return false;
            }
            void writeTextShort(std::ostream& out) const {
                out<<"Target position "<<start_sub_at<<
                    " length of substitution "<<sub_length<<(invertB ?
                     " inverse reducer position " : " reducer position ")
                    <<start_from<<" score "<<score;
            }
            /**
             * Gives a string that describes the substitution.
             */
            std::string substitutionString(const GroupExpression &word) const;
        };
        /**
         *  A routine internal to the small cancellation simplification
         *  algorithm.
         *
         *  This is the core of the Dehn algorithm for hyperbolic groups.
         *  Given two words, this_word and that_word, this routine searches for
         *  subwords of that_word (in the cyclic sense), and builds a table
         *  of substitutions one can make from that_word into this_word.  The
         *  table is refined so that one knows the "value" of each
         *  substitution -- the extent to which the substitution would shorten
         *  this_word.   This is to allow for intelligent choices of
         *  substitutions by whichever algorithms call this one.
         *
         *  This algorithm assumes that this_word and that_word are cyclically
         *  reduced words.  If you feed it non-cyclically reduced words it
         *  will give you suggestions although they will not be as strong
         *  as if the words were cyclically reduced.  It also only adds
         *  to sub_list, so in normal usage one would pass it an empty sub-list.
         *
         *  The default argument step==1 assumes you are looking for
         *  substitutions that shorten the length of a word, and that
         *  you only want to make an immediate substitution.  Setting
         *  step==2 assumes after you make your first substitution you
         *  will want to attempt a further substitution, etc.  step>1
         *  is used primarily when building relator tables for group
         *  recognition.
         */
        static void dehnAlgorithmSubMetric(
            const GroupExpression &this_word,
            const GroupExpression &that_word,
            std::set<WordSubstitutionData> &sub_list,
            unsigned long step=1 );

        /**
         *  A routine internal to the small cancellation simplification
         *  algorithm.
         *
         *  Given a word this_word and that_word, apply the substitution
         *  specified by sub_data to *this. See dehnAlgorithm() and struct
         *  WordSubstitutionData.  In particular sub_data needs to be a
         *  valid substitution, usually it will be generated by
         *  dehnAlgorithmSubMetric.
         */
        static void applySubstitution(
            GroupExpression& this_word,
            const GroupExpression &that_word,
            const WordSubstitutionData &sub_data );

        /**
         * Contains the bulk of the implementation for enumerateCovers().
         *
         * Unlike enumerateCovers(), this routine is designed to work on
         * a temporary non-const copy of the original group.  This means that
         * it is free to simplify the group, reorder the generators, and/or
         * make other changes it thinks may help speed up the enumeration.
         *
         * The arguments are similar to enumerateCovers(), except that the
         * type of the action function is now known precisely.  This means
         * that the implementation can be kept out of the main headers.
         */
        template <int index>
        size_t enumerateCoversInternal(
            std::function<void(GroupPresentation&&)>&& action);

        /**
         * Relabels the generators and reorders the relations in the
         * hope that an initial subset of generators will cover a large
         * initial subset of relations.
         *
         * This routine works by:
         *
         * - reordering the relations so that the relations that appear first
         *   use a smaller subset of distinct generators; and then
         *
         * - relabelling the generators so that these initial relations
         *   use generators with smaller labels.
         *
         * There are non-obvious trade-offs to be made here, and so the
         * precise algorithm (and hence the precise reordering/relabelling)
         * is subject to change in future versions of Regina.
         *
         * It is guaranteed that, after this routine finishes, the maximum
         * generator label used in each relation is a (non-strict) monotonic
         * increasing function of the relation number (with all empty
         * relations appearing at the beginning of the relation list).
         *
         * This routine will also cycle the relations around so that
         * the last term of each relation uses the relation's corresponding
         * maximum generator label.
         */
        void minimaxGenerators();
};

/**
 * Swaps the contents of the two given group presentations.
 *
 * This global routine simply calls GroupPresentation::swap(); it is provided
 * so that GroupPresentation meets the C++ Swappable requirements.
 *
 * \param lhs the presentation whose contents should be swapped with \a rhs.
 * \param rhs the presentation whose contents should be swapped with \a lhs.
 *
 * \ingroup algebra
 */
void swap(GroupPresentation& lhs, GroupPresentation& rhs) noexcept;

// Inline functions for GroupExpressionTerm

inline GroupExpressionTerm::GroupExpressionTerm(unsigned long gen, long exp) :
        generator(gen), exponent(exp) {
}

inline bool GroupExpressionTerm::operator == (
        const GroupExpressionTerm& other) const {
    return (generator == other.generator) && (exponent == other.exponent);
}

inline bool GroupExpressionTerm::operator != (
        const GroupExpressionTerm& other) const {
    return (generator != other.generator) || (exponent != other.exponent);
}

inline GroupExpressionTerm GroupExpressionTerm::inverse() const {
    return GroupExpressionTerm(generator, -exponent);
}

inline bool GroupExpressionTerm::operator += (
        const GroupExpressionTerm& other) {
    if (generator == other.generator) {
        exponent += other.exponent;
        return true;
    } else
        return false;
}

inline bool GroupExpressionTerm::operator < (
        const GroupExpressionTerm& other) const {
    return ( (generator < other.generator) ||
             ( (generator == other.generator) &&
               ( exponent < other.exponent ) ) );
}

// Inline functions for GroupExpression

inline GroupExpression::GroupExpression(const GroupExpressionTerm& term) {
    terms_.push_back(term);
}

inline GroupExpression::GroupExpression(unsigned long generator,
        long exponent) {
    terms_.emplace_back(generator, exponent);
}

inline GroupExpression::GroupExpression(const std::string &input,
        unsigned long nGens) : GroupExpression(input.c_str(), nGens) {
}

inline void GroupExpression::swap(GroupExpression& other) noexcept {
    terms_.swap(other.terms_);
}

inline bool GroupExpression::operator ==(const GroupExpression& comp) const {
    return terms_ == comp.terms_;
}

inline bool GroupExpression::operator !=(const GroupExpression& comp) const {
    return terms_ != comp.terms_;
}

inline std::list<GroupExpressionTerm>& GroupExpression::terms() {
    return terms_;
}

inline const std::list<GroupExpressionTerm>& GroupExpression::terms()
        const {
    return terms_;
}

inline size_t GroupExpression::countTerms() const {
    return terms_.size();
}

inline bool GroupExpression::isTrivial() const {
    return terms_.empty();
}

inline size_t GroupExpression::wordLength() const {
    size_t retval(0);
    std::list<GroupExpressionTerm>::const_iterator it;
    for (it = terms_.begin(); it!=terms_.end(); it++)
        retval += labs((*it).exponent);
    return retval;
}

inline unsigned long GroupExpression::generator(size_t index) const {
    return term(index).generator;
}

inline long GroupExpression::exponent(size_t index) const {
    return term(index).exponent;
}

inline void GroupExpression::addTermFirst(const GroupExpressionTerm& term) {
    terms_.push_front(term);
}

inline void GroupExpression::addTermFirst(unsigned long generator,
        long exponent) {
    terms_.push_front(GroupExpressionTerm(generator, exponent));
}

inline void GroupExpression::addTermLast(const GroupExpressionTerm& term) {
    terms_.push_back(term);
}

inline void GroupExpression::addTermLast(unsigned long generator,
        long exponent) {
    terms_.emplace_back(generator, exponent);
}

inline void GroupExpression::addTermsLast(GroupExpression word) {
    terms_.splice(terms_.end(), std::move(word.terms_));
}

inline void GroupExpression::addTermsFirst(GroupExpression word) {
    terms_.splice(terms_.begin(), std::move(word.terms_));
}

inline void GroupExpression::erase() {
    terms_.clear();
}

inline std::string GroupExpression::str(bool alphaGen) const {
    std::ostringstream out;
    writeTextShort(out, false, alphaGen);
    return out.str();
}

inline std::string GroupExpression::utf8(bool alphaGen) const {
    std::ostringstream out;
    writeTextShort(out, true, alphaGen);
    return out.str();
}

inline void swap(GroupExpression& lhs, GroupExpression& rhs) noexcept {
    lhs.swap(rhs);
}

// Inline functions for GroupPresentation

inline GroupPresentation::GroupPresentation() : nGenerators_(0) {
}

inline GroupPresentation::GroupPresentation(unsigned long nGenerators) :
        nGenerators_(nGenerators) {
}

inline void GroupPresentation::swap(GroupPresentation& other) noexcept {
    std::swap(nGenerators_, other.nGenerators_);
    relations_.swap(other.relations_);
}

inline unsigned long GroupPresentation::addGenerator(unsigned long num) {
    return (nGenerators_ += num);
}

inline void GroupPresentation::addRelation(GroupExpression rel) {
    relations_.push_back(std::move(rel));
}

inline unsigned long GroupPresentation::countGenerators() const {
    return nGenerators_;
}

inline size_t GroupPresentation::countRelations() const {
    return relations_.size();
}

inline const GroupExpression& GroupPresentation::relation(size_t index) const {
    return relations_[index];
}

inline const std::vector<GroupExpression>& GroupPresentation::relations()
        const {
    return relations_;
}

inline void GroupPresentation::writeTextShort(std::ostream& out) const {
    writeTextCompact(out);
}

inline size_t GroupPresentation::relatorLength() const {
    size_t retval(0);
    for (const auto& r : relations_)
        retval += r.wordLength();
    return retval;
}

inline bool GroupPresentation::operator == (const GroupPresentation& other)
        const {
    return nGenerators_ == other.nGenerators_ && relations_ == other.relations_;
}

inline bool GroupPresentation::operator != (const GroupPresentation& other)
        const {
    return nGenerators_ != other.nGenerators_ || relations_ != other.relations_;
}

inline void swap(GroupPresentation& lhs, GroupPresentation& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace regina

// Several of our functions have a return type that requres a fully defined
// HomGroupPresentation.
#include "algebra/homgrouppresentation.h"

namespace regina {

template <int index, typename Action, typename... Args>
inline size_t GroupPresentation::enumerateCovers(
        Action&& action, Args&&... args) const {
    // Do the real work on a temporary copy of this presentation that we
    // can be free to modify as we see fit.
    return GroupPresentation(*this).enumerateCoversInternal<index>(
        [&](GroupPresentation&& g) {
            action(std::move(g), std::forward<Args>(args)...);
        });
}

} // namespace regina

#endif

