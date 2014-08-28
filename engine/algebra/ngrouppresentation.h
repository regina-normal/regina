
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file algebra/ngrouppresentation.h
 *  \brief Deals with finite presentations of groups.
 */

#ifndef __NGROUPPRESENTATION_H
#ifndef __DOXYGEN
#define __NGROUPPRESENTATION_H
#endif

#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>

#include "regina-core.h"
#include "shareableobject.h"
#include "utilities/memutils.h"
#include "utilities/ptrutils.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/nabeliangroup.h"

namespace regina {

class NAbelianGroup;
class NHomGroupPresentation;
class NMarkedAbelianGroup;

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents a power of a generator in a group presentation.
 */
struct REGINA_API NGroupExpressionTerm {
    unsigned long generator;
        /**< The number that identifies the generator in this term. */
    long exponent;
        /**< The exponent to which the generator is raised. */

    /**
     * Creates a new uninitialised term.
     */
    NGroupExpressionTerm();
    /**
     * Creates a new term initialised to the given value.
     *
     * @param newGen the number that identifies the generator in the new term.
     * @param newExp the exponent to which this generator is raised.
     */
    NGroupExpressionTerm(unsigned long newGen, long newExp);
    /**
     * Creates a new term initialised to the given value.
     *
     * @param cloneMe a term whose data will be copied to the new term.
     */
    NGroupExpressionTerm(const NGroupExpressionTerm& cloneMe);

    /**
     * Makes this term identical to the given term.
     *
     * @param cloneMe the term whose data will be copied to this term.
     * @return a reference to this term.
     */
    NGroupExpressionTerm& operator = (const NGroupExpressionTerm& cloneMe);
    /**
     * Determines whether this and the given term contain identical data.
     *
     * @param other the term with which this term will be compared.
     * @return \c true if and only if this and the given term have both the
     * same generator and exponent.
     */
    bool operator == (const NGroupExpressionTerm& other) const;

    /**
     * Imposes an ordering on terms.
     * Terms are ordered lexigraphically as (generator, exponent) pairs.
     *
     * @param other the term to compare with this.
     * @return true if and only if this term is lexicographically
     * smaller than \a other.
     */
    bool operator < (const NGroupExpressionTerm& other) const;

    /**
     * Returns the inverse of this term.  The inverse has the same
     * generator but a negated exponent.
     *
     * Note that this term will remain unchanged.
     *
     * @return the inverse of this term.
     */
    NGroupExpressionTerm inverse() const;

    /**
     * Attempts to merge this term with the given term.
     * If both terms have the same generator, the two exponents will be
     * added and stored in this term.  If the generators are different,
     * this routine will do nothing.
     *
     * Note that this term might be changed but the given term will remain
     * unchanged.
     *
     * @param other the term to merge with this term.
     * @return \c true if the two terms were merged into this term, or
     * \c false if the two terms have different generators.
     */
    bool operator += (const NGroupExpressionTerm& other);
};

/**
 * Writes the given term to the given output stream.
 * The term will be written in the format <tt>g3^-7</tt>, where in this
 * example the term represents generator number 3 raised to the -7th power.
 *
 * If the term has exponent 0 or 1, the output format will be
 * appropriately simplified.
 *
 * @param out the output stream to which to write.
 * @param term the term to write.
 * @return a reference to the given output stream.
 */
REGINA_API std::ostream& operator << (std::ostream& out,
    const NGroupExpressionTerm& term);

/**
 * Represents an expression involving generators from a group presentation
 * or a free group.  An expression is represented as word, i.e, a sequence
 * of powers of generators all of which are multiplied in order.  Each power
 * of a generator corresponds to an individual NGroupExpressionTerm.
 *
 * For instance, the expression <tt>g1^2 g3^-1 g6</tt> contains the
 * three terms <tt>g1^2</tt>, <tt>g3^-1</tt> and <tt>g6^1</tt> in that
 * order.
 */
class REGINA_API NGroupExpression : public ShareableObject {
    private:
        std::list<NGroupExpressionTerm> terms;
            /** The terms that make up this expression. */

    public:
        /**
         * Creates a new expression with no terms.
         */
        NGroupExpression();
        /**
         * Creates a new expression that is a clone of the given
         * expression.
         *
         * @param cloneMe the expression to clone.
         */
        NGroupExpression(const NGroupExpression& cloneMe);
        /**
         * Attempts to interpret the given input string as a word in a group.
         * Regina can recognise strings in the following four basic forms:
         *
         *  - \c a^7b^-2
         *  - \c aaaaaaaBB
         *  - \c a^7B^2
         *  - \c g0^7g1^-2
         *
         * The string may contain whitespace, which will simply be ignored.
         *
         * The argument \a valid may be \c null, but if it is non-null
         * then the boolean it points to will be used for error reporting.
         * This routine sets valid to \c true if the string was successfully
         * interpreted, or \c false if the algorithm failed to interpret the
         * string.
         *
         * Regardless of whether \a valid is \c null, if the string
         * could not be interpreted then this expression will be initialised
         * to the trivial word.
         *
         * @param input the input string that is to be interpreted.
         * @param valid used for error reporting as described above, or
         * \c null if no error reporting is required.
         */
        NGroupExpression(const std::string &input, bool* valid=NULL);

        /**
         * Makes this expression a clone of the given expression.
         *
         * @param cloneMe the expression to clone.
         * @return a reference to this expression.
         */
        NGroupExpression& operator = (const NGroupExpression& cloneMe);

        /**
         * Equality operator. Checks to see whether or not these two words
         * represent the same literal string.
         *
         * @param comp the expression to compare against this.
         * @return \c true if this and the given string literal are identical.
         */
        bool operator == (const NGroupExpression& comp) const;

        /**
         * Returns the list of terms in this expression.
         * These are the actual terms stored internally; any
         * modifications made to this list will show up in the
         * expression itself.
         *
         * For instance, the expression <tt>g1^2 g3^-1 g6</tt> has list
         * consisting of three terms <tt>g1^2</tt>, <tt>g3^-1</tt> and
         * <tt>g6^1</tt> in that order.
         *
         * \ifacespython Not present; only the const version of this
         * routine is available.
         *
         * @return the list of terms.
         */
        std::list<NGroupExpressionTerm>& getTerms();
        /**
         * Returns a constant reference to the list of terms in this
         * expression.
         *
         * For instance, the expression <tt>g1^2 g3^-1 g6</tt> has list
         * consisting of three terms <tt>g1^2</tt>, <tt>g3^-1</tt> and
         * <tt>g6^1</tt> in that order.
         *
         * \ifacespython This routine returns a python list of copied
         * NGroupExpressionTerm objects.  In particular, modifying this
         * list or the terms within it will not modify the group
         * expression from which they came.
         *
         * @return the list of terms.
         */
        const std::list<NGroupExpressionTerm>& getTerms() const;
        /**
         * Returns the number of terms in this expression.
         *
         * For instance, the expression <tt>g1^2 g3^-1 g6</tt> contains three
         * terms.  See also getWordLength().
         *
         * @return the number of terms.
         */
        unsigned long getNumberOfTerms() const;
        /**
         * Returns the length of the word, i.e. the number of letters
         * with exponent +1 or -1 for which this word is expressable as a
         * product.
         *
         * For instance, the expression <tt>g1^2 g3^-1 g6</tt> is a word of
         * length four.  See also getNumberOfTerms().
         *
         * No attempt is made to remove redundant terms (so the word
         * <tt>g g^-1</tt> will count as length two).
         *
         * @return the length of the word.
         */
        unsigned long wordLength() const;
        /**
         * Tests whether this is the trivial (unit) word.
         *
         * No attempt is made to remove redundant terms (so the word
         * <tt>g g^-1</tt> will be treated as non-trivial).
         *
         * @return \c true if and only if this is the trivial word.
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
         * \warning This routine is <i>O(n)</i> where \a n is the number
         * of terms in this expression.
         *
         * @param index the index of the term to return; this must be
         * between 0 and getNumberOfTerms()-1 inclusive.
         * @return the requested term.
         */
        NGroupExpressionTerm& getTerm(unsigned long index);
        /**
         * Returns a constant reference to the term at the given
         * index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is <i>O(n)</i> where \a n is the number
         * of terms in this expression.
         *
         * \ifacespython Not present; only the non-const version of this
         * routine is available.
         *
         * @param index the index of the term to return; this must be
         * between 0 and getNumberOfTerms()-1 inclusive.
         * @return the requested term.
         */
        const NGroupExpressionTerm& getTerm(unsigned long index) const;
        /**
         * Returns the generator corresonding to the
         * term at the given index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is <i>O(n)</i> where \a n is the number
         * of terms in this expression.
         *
         * @param index the index of the term to return; this must be
         * between 0 and getNumberOfTerms()-1 inclusive.
         * @return the number of the requested generator.
         */
        unsigned long getGenerator(unsigned long index) const;
        /**
         * Returns the exponent corresonding to the
         * term at the given index in this expression.
         * Index 0 represents the first term, index 1
         * represents the second term and so on.
         *
         * \warning This routine is <i>O(n)</i> where \a n is the number
         * of terms in this expression.
         *
         * @param index the index of the term to return; this must be
         * between 0 and getNumberOfTerms()-1 inclusive.
         * @return the requested exponent.
         */
        long getExponent(unsigned long index) const;

        /**
         * Adds the given term to the beginning of this expression.
         *
         * @param term the term to add.
         */
        void addTermFirst(const NGroupExpressionTerm& term);
        /**
         * Adds the given term to the beginning of this expression.
         *
         * @param generator the number of the generator corresponding to
         * the new term.
         * @param exponent the exponent to which the given generator is
         * raised.
         */
        void addTermFirst(unsigned long generator, long exponent);
        /**
         * Adds the given term to the end of this expression.
         *
         * @param term the term to add.
         */
        void addTermLast(const NGroupExpressionTerm& term);
        /**
         * Adds the given term to the end of this expression.
         *
         * @param generator the number of the generator corresponding to
         * the new term.
         * @param exponent the exponent to which the given generator is
         * raised.
         */
        void addTermLast(unsigned long generator, long exponent);

        /**
         * Multiplies this expression on the left by the given word.
         * This expression will be modified directly.
         *
         * @param word the word to multiply with this expression.
         */
        void addTermsFirst(const NGroupExpression& word);
        /**
         * Multiplies this expression on the right by the given word.
         * This expression will be modified directly.
         *
         * @param word the word to multiply with this expression.
         */
        void addTermsLast(const NGroupExpression& word);

        /**
         * Multiplies this expression on the left by the word
         * respresented by the given string.
         *
         * See the string-based constructor
         * NGroupExpression(const std::string&, bool*) for further
         * information on how this string should be formatted.
         *
         * If the given string cannot be interpreted as a word in a group,
         * then this expression will be left untouched.
         *
         * @param a string representation of the word to multiply with
         * this expression.
         * @return \c true if the given string could interpreted
         * (and therefore the multiplication was completed successfully), or
         * \c false if the given string could not be interpreted
         * (in which case this expression will be left untouched).
         */
        bool addStringFirst(const std::string& input);

        /**
         * Multiplies this expression on the right by the word
         * respresented by the given string.
         *
         * See the string-based constructor
         * NGroupExpression(const std::string&, bool*) for further
         * information on how this string should be formatted.
         *
         * If the given string cannot be interpreted as a word in a group,
         * then this expression will be left untouched.
         *
         * @param a string representation of the word to multiply with
         * this expression.
         * @return \c true if the given string could interpreted
         * (and therefore the multiplication was completed successfully), or
         * \c false if the given string could not be interpreted
         * (in which case this expression will be left untouched).
         */
        bool addStringLast(const std::string& input);

        /**
         *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
         * converts the word into g_i2^j2 ... g_in^jn g_i1^j1
         */
        void cycleRight();

        /**
         *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
         * converts the word into g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1
         */
        void cycleLeft();

        /**
         * Returns a newly created expression that is the inverse of
         * this expression.  The terms will be reversed and the
         * exponents negated.
         *
         * @return the inverse of this expression.
         */
        NGroupExpression* inverse() const;

        /**
         * Inverts this expression.  Does not allocate or deallocate anything.
         */
        void invert();

        /**
         * Returns a newly created expression that is
         * this expression raised to the given power.
         * Note that the given exponent may be positive, zero or negative.
         *
         * @param exponent the power to which this expression should be raised.
         * @return this expression raised to the given power.
         */
        NGroupExpression* power(long exponent) const;
        /**
         * Simplifies this expression.
         * Adjacent powers of the same generator will be combined, and
         * terms with an exponent of zero will be removed.
         * Note that it is \e not assumed that the underlying group is
         * abelian.
         *
         * You may declare that the expression is cyclic, in which case
         * it is assumed that terms may be moved from the back to the
         * front and vice versa.  Thus expression <tt>g1 g2 g1 g2 g1</tt>
         * simplifies to <tt>g1^2 g2 g1 g2</tt> if it is cyclic, but does not
         * simplify at all if it is not cyclic.
         *
         * @param cyclic \c true if and only if the expression may be
         * assumed to be cyclic.
         * @return \c true if and only if this expression was changed.
         */
        bool simplify(bool cyclic = false);
        /**
         * Replaces every occurrence of the given generator with the
         * given substite expression.  If the given generator was found,
         * the expression will be simplified once the substitution is
         * complete.
         *
         * @param generator the generator to be replaced.
         * @param expansion the substitute expression that will replace
         * every occurrence of the given generator.
         * @param cyclic \c true if and only if the expression may be
         * assumed to be cyclic; see simplify() for further details.
         * @return \c true if and only if any substitutions were made.
         */
        bool substitute(unsigned long generator,
            const NGroupExpression& expansion, bool cyclic = false);

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
         * @param other the word to compare against this.
         * @param cyclic if \c false we get a list of exact relabellings from
         * this word to \a other.  If \c true, it can be up to cyclic
         * permutation and inversion.
         * @return a list of permutations, implemented as maps from
         * generator indices of this word to generator indices of \a other.
         */
        std::list< std::map< unsigned long, NGroupExpressionTerm > >
            relabellingsThisToOther( const NGroupExpression &other,
            bool cyclic=false ) const;

        /**
         * Writes a chunk of XML containing this expression.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * Returns a TeX representation of this expression.
         * See writeTeX() for details on how this is formed.
         *
         * @return a TeX representation of this expression.
         */
        std::string toTeX() const;

        /**
         * Writes a TeX represesentation of this expression to the given
         * output stream.
         *
         * The text representation will be of the form
         * <tt>g_2^4 g_{13}^{-5} g_4</tt>.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         */
        void writeTeX(std::ostream& out) const;

        /**
         * Writes a text representation of this expression to the given
         * output stream, using either numbered generators or
         * alphabetic generators.
         *
         * The text representation will be of the form
         * <tt>g2^4 g13^-5 g4</tt>. If the shortword flag is
         * true, it will assume your word is in an alphabet of
         * no more than 26 letters, and will write the word using
         * lower-case ASCII, i.e. <tt>c^4 n^-5 e</tt>.
         *
         * \pre If \a shortword is \c true, the number of generators in
         * the corresponding group must be 26 or fewer.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         * @param shortword indicates whether to use numbered or
         * alphabetic generators, as described above.
         */
        void writeText(std::ostream& out, bool shortword=false) const;

        /**
         * The text representation will be of the form
         * <tt>g2^4 g13^-5 g4</tt>.
         */
        virtual void writeTextShort(std::ostream& out) const;
};

/**
 * Represents a finite presentation of a group.
 *
 * A presentation consists of a number of generators and a set of
 * relations between these generators that together define the group.
 *
 * If there are \a g generators, they will be numbered 0, 1, ..., <i>g</i>-1.
 *
 * \todo Let's make intelligent simplify a tad more intelligent, and the GUI
 * call a bit more safe.  Perhaps parallelize the GUI call, and give users
 * parameters to ensure it won't crash the computer.  Also look at the FPGroup
 * package. We should also have a simple way of creating NGroupPresentation
 * objects directly from text strings.  We would like to have something like
 * NGroupPresentation( numGens, "abAAB", "bccd" ) etc., with arbitrary
 * numbers of relators. Maybe std::tuple.  Or "variadic templates"?
 */
class REGINA_API NGroupPresentation : public ShareableObject {
    protected:
        unsigned long nGenerators;
            /**< The number of generators. */
        std::vector<NGroupExpression*> relations;
            /**< The relations between the generators. */

    public:
        /**
         * Creates a new presentation with no generators and no
         * relations.
         */
        NGroupPresentation();
        /**
         * Creates a clone of the given group presentation.
         *
         * @param cloneMe the presentation to clone.
         */
        NGroupPresentation(const NGroupPresentation& cloneMe);
        /**
         * Constructor that allows you to directly pass an arbitrary number
         * of relators in string format.
         *
         * The first argument \a nGens is the number of generators one wants
         * the group to have. The second argument \a rels is a vector
         * of strings, where each string gives a single relator.  See
         * the NGroupExpression::NGroupExpression(const std::string&, bool*)
         * constructor notes for information on what format these strings
         * can take.
         *
         * If you are compiling Regina against C++11, you can use the
         * C++11 initializer_list construction to construct an
         * NGroupPresentation directly using syntax of the form
         * <tt>NGroupPresentation(nGens, { "rel1", "rel2", ... })</tt>.
         *
         * @param nGens the number of generators.
         * @param rels a vector of relations each given in string form,
         * as outlined above.
         */
        NGroupPresentation(unsigned long nGens, std::vector<std::string> &rels);
        /**
         * Destroys the group presentation.
         * All relations that are stored will be deallocated.
         */
        virtual ~NGroupPresentation();

        /**
         * Assignment operator.
         *
         * @param cloneMe the group presentation that this will become a
         * copy of.
         * @return a reference to this group presentation.
         */
        NGroupPresentation& operator=(const NGroupPresentation& cloneMe);

        /**
         * Adds one or more generators to the group presentation.
         * If the new presentation has \a g generators, the new
         * generators will be numbered <i>g</i>-1, <i>g</i>-2 and so on.
         *
         * @param numToAdd the number of generators to add.
         * @return the number of generators in the new presentation.
         */
        unsigned long addGenerator(unsigned long numToAdd = 1);
        /**
         * Adds the given relation to the group presentation.
         * The relation must be of the form <tt>expression = 1</tt>.
         *
         * This presentation will take ownership of the given
         * expression, may change it and will be responsible for its
         * deallocation.
         *
         * \warning This routine does not check whether or not your relation
         * is a word only in the generators of this group.  In other
         * words, it does not stop you from using generators beyond the
         * getNumberOfGenerators() bound.
         *
         * \ifacespython Since this group presentation takes ownership
         * of the given expression, the python object containing the
         * given expression becomes a null object and should no longer
         * be used.
         *
         * @param rel the expression that the relation sets to 1; for
         * instance, if the relation is <tt>g1^2 g2 = 1</tt> then this
         * parameter should be the expression <tt>g1^2 g2</tt>.
         */
        void addRelation(NGroupExpression* rel);

        /**
         * Returns the number of generators in this group presentation.
         *
         * @return the number of generators.
         */
        unsigned long getNumberOfGenerators() const;
        /**
         * Returns the number of relations in this group presentation.
         *
         * @return the number of relations.
         */
        unsigned long getNumberOfRelations() const;
        /**
         * Returns the relation at the given index in this group
         * presentation.  The relation will be of the form <tt>expresson
         * = 1</tt>.
         *
         * @param index the index of the desired relation; this must be
         * between 0 and getNumberOfRelations()-1 inclusive.
         *
         * @return the expression that the requested relation sets to 1;
         * for instance, if the relation is <tt>g1^2 g2 = 1</tt> then
         * this will be the expression <tt>g1^2 g2</tt>.
         */
        const NGroupExpression& getRelation(unsigned long index) const;

        /**
         *  Returns true if and only if the relations for the group are words
         * in the generators.  Returns false if at least one relator uses an
         * out of bound generator.
         */
        bool isValid() const;

        /**
         * Attempts to simplify the group presentation as intelligently
         * as possible without further input.
         *
         * See intelligentSimplifyDetail() for further details on how
         * the simplification is done.
         *
         * @return \c true if and only if the group presentation was changed.
         */
        bool intelligentSimplify();

        /**
         * Attempts to simplify the group presentation as intelligently
         * as possible without further input.
         *
         * The current simplification method is based on the Dehn algorithm
         * for hyperbolic groups, i.e. small cancellation theory.   This means
         * we look to see if part of one relator can be used to simplify
         * others.  If so, make the substitution and simplify.  We continue
         * until no more presentation-shortening substitutions are available.
         * We follow that by killing any available generators using words
         * where generators appear a single time.
         *
         * \todo \optlong This routine could use some small tweaks --
         * recognition of utility of some score==0 moves, such as
         * commutators, for example.
         *
         * @return a newly allocated homomorphism describing the
         * reduction map from the original presentation to the new
         * presentation, or a null pointer if this presentation was not
         * changed.
         */
        std::auto_ptr<NHomGroupPresentation> intelligentSimplifyDetail();

        /**
         * Attempts to simplify the group presentation using only small
         * cancellation theory.
         *
         * See smallCancellationDetail() for further details on how
         * the simplification is done.
         *
         * @return \c true if and only if the group presentation was changed.
         */
        bool smallCancellation();
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
         * \todo \optlong This routine could use some small tweaks --
         * recognition of utility of some score==0 moves, such as
         * commutators, for example.
         *
         * @return a newly allocated homomorphism describing the
         * reduction map from the original presentation to the new
         * presentation, or a null pointer if this presentation was not
         * changed.
         */
        std::auto_ptr<NHomGroupPresentation> smallCancellationDetail();

        /**
         * Using the current presentation of the group, this routine uses
         * small cancellation theory to reduce the input word.
         *
         * @input is the word you would like to simplify.  It must be a word
         *  in the generators of this group.
         *
         * \warning this routine is only as good as the relator table for the
         *  group.  You might want to consider running intelligentSimplify()
         *  and possibly proliferateRelators() before using this routine for
         *  any significant tasks.
         *
         * @return true if the input word has been modified, false otherwise.
         */
        bool simplifyWord( NGroupExpression &input ) const;

        /**
         * A routine to help escape local wells when simplifying
         * presentations, which may be useful when small cancellation theory
         * can't find the simplest relators.
         *
         * Given a presentation &lt;g_i | r_i&gt;, this routine appends
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
         * @param depth controls the depth of the proliferation, as
         * described above; this must be strictly positive.
         */
        void proliferateRelators(unsigned long depth=1);

        /**
         * Attempts to recognise the group corresponding to this
         * presentation.  This routine is much more likely to be
         * successful if you have already called intelligentSimplify().
         *
         * Note that the presentation might be simplified a little
         * during the execution of this routine, although not nearly as
         * much as would be done by intelligentSimplify().
         *
         * Currently, if successful the only groups this routine
         * recognises is the trivial group, cyclic groups, free groups,
         * and the free abelian group of rank two.
         *
         * Return strings have the form "0" for the trivial
         * group, "Z_n" for cyclic groups with n > 1, "Free(n generators)"
         * for free groups with n>1, and "Z" and "Z + Z (abelian)"
         * are the only two free abelian groups supported at present.
         *
         * \todo \featurelong Make this recognition more effective.
         *
         * @return a simple string representation of the group if it is
         * recognised, or an empty string if the group is not
         * recognised.
         */
        std::string recogniseGroup() const;

        /**
         * Writes a chunk of XML containing this group presentation.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        /**
         * The sum of the word lengths of the relators.
         * Word lengths are computing using NGroupExpression::wordLength().
         * Used as a coarse measure of the complexity of the presentation.
         *
         * @return the sum of word lengths.
         */
        unsigned long relatorLength() const;

        /**
         * Computes the abelianisation of this group.
         *
         * @return a newly allocated abelianisation of this group.
         */
        std::auto_ptr<NAbelianGroup> abelianisation() const;

        /**
         * Computes the abelianisation of this group.
         * The coordinates in the chain complex correspond
         * to the generators and relators for this group.
         *
         * @return a newly allocated abelianisation of this group.
         */
        std::auto_ptr<NMarkedAbelianGroup> markedAbelianisation() const;

        /**
         * Attempts to determine if the group is abelian.   If the group is
         * abelian, markedAbelianization() is the easiest way to see precisely
         * which abelian group it is, and how the generators sit in that group.
         * You will have better luck using this algorithm provided the
         * presentation has been simplified, as this algorithm uses small
         * cancellation theory in an attempt to reduce the commutators of all
         * pairs of generators.
         *
         * \warning if one has not adequately simplified this presentation this
         *  routine will return false.  Consider running intelligentSimplify
         *  and perhaps even proliferateRelators in order to discover adequately
         *  many commutators.
         *
         * @return true if the group is shown to be abelian.  False if either
         *  the group is not abelian, or the algorithm fails.
         */
        bool identifyAbelian() const;

        /**
         *  This switches the generators in the presentation indexed by i
         * and j respectively, and recompute the appropriate presentation.
         * It is one of the standard Nielsen moves, which is the first of
         * three generator types of the automorphism group of a
         * free group.
         *
         * @returns true if and only if the nielsen automorphism had an effect
         *  on at least one relation.
         */
        bool nielsenTransposition(const unsigned long &i,
                                  const unsigned long &j);

        /**
         *  This replaces a generator in a presentation by its inverse, and
         * recomputes the appropriate presentation. This is the second
         * generator type of the automorphism group of a free group.
         *
         * @returns true if and only if the nielsen automorphism had an effect
         *  on at least one relation.
         */
        bool nielsenInvert(const unsigned long &i);

        /**
         *  This replaces a generator gi by (gi)(gj)^k in the presentation. It
         * it is the third type of Nielsen move one can apply to a presentation.
         * If the flag is set false, it uses left multiplication, replacing
         * gi by (gj)^k(gi). So if the new generator Gi is the old (gi)(gj)^k,
         * this means we  construct the new presentation from the old by
         * replacing occurances of gi by (Gi)(gj)^(-k).
         *
         * @returns true if and only if the nielsen automorphism had an effect
         *  on at least one relation.
         */
        bool nielsenCombine(const unsigned long &i, const unsigned long &j,
                const signed long &k, const bool &flag=true);

        /**
         *  Looks for Nielsen moves that will simplify the presentation.
         * Performs one of the most-effective ones, if it can find any.
         *
         * @returns true if and only if it performed a Nielsen move.
         */
        bool intelligentNielsen();

        /**
         *  Looks for Nielsen moves that will simplify the presentation.
         * Performs one of the most-effective ones, if it can find any.
         *
         * @returns the allocated NHomGroupPresentation auto pointer
         *  which is the isomorphism from the old presentation to the
         *  new one.
         */
        std::auto_ptr<NHomGroupPresentation> intelligentNielsenDetail();

        /**
         * This routine attempts to rewrite the presentation so that generators
         * of the group map to generators of the abelianisation, with any
         * left-over generators mapping to zero (if possible).  Consider this a
         * homological-alignment of the presentation.
         *
         * If the abelianisation of this group has rank N and M invariant
         * factors d0 | d2 | ... | d(M-1), this routine applies Nielsen moves
         * to the presentation to ensure that under the markedAbelianisation
         * routine, generators 0 through M-1 are mapped to generators of the
         * relevant Z_di group.  Similarly, generators M through M+N-1 are
         * mapped to +/-1 in the appropriate factor. All further generators
         * will be mapped to zero.
         *
         * @returns true if presentation has changed
         */
        bool homologicalAlignment();

        /**
         * Same a the previous homologicalAlignment() routine but returns an
         * allocated NHomGroupPresentation auto_ptr giving the reduction map
         * from the old presentation to the new, if any change is detected.
         *
         * @returns allocated auto_ptr if presentation has changed.
         */
        std::auto_ptr<NHomGroupPresentation> homologicalAlignmentDetail();

        /**
         * This is an entirely cosmetic re-writing of the presentation, is
         * fast and superficial.
         *  1) If there are any length 1 relators, those generators are
         *     deleted, and the remaining relators simplified.
         *  2) It sorts the relators by number of generator indices that
         *     appear, followed by relator numbers (lexico) followed by
         *     relator length.
         *  3) inverts relators if net sign of the generators is negative.
         *  4) Given each generator, it looks for the smallest word where that
         *     generator appears with non-zero weight.  If negative weight,
         *     it inverts that generator.
         *  5) It cyclically permutes relators to start with smallest gen.
         *  6) TODO: Makes elementary simplifications to aid in seeing standard
         *     relators like commutators.
         *
         * @returns true if and only if the choice of generators for the group
         *  has changed.  You can call prettyRewritingDetail to get the
         *  the isomorphism.
         */
        bool prettyRewriting();

        std::auto_ptr<NHomGroupPresentation> prettyRewritingDetail();

        /**
         *  This routine attempts to re-write the presentation as a group
         *  extension of the form: < a, r1,...,rn | R1,...,RM, ar1a^-1 = w1,
         *    ... arna^-1 = wn > i.e. as a semi-direct product of the integers
         *  and another finitely-presented group.  This is an algorithmic
         *  implementation of the Reidemeister-Schrier algorithm, which isn't
         *  actually an algorithm.  So sometimes this procedure works, and
         *  sometimes it does not.  The return value is an allocated auto_ptr
         *  if and only if the algorithm is successful.  Even if the algorithm
         *  is unsuccessful, its application will likely result in a
         *  modification of the presentation.
         *
         *  \apinotfinal - this routine may very well either be eliminated
         *   in future versions of this software, perhaps incorporated into a
         *   bigger-and-better future algorithm.
         *
         *  @return an allocated auto_ptr< NHomGroupPresentation > if and only
         *   if the algorithm is successful.  When this pointer is allocated
         *   it will be an automorphism of a presentation of the kernel of the
         *   map this to the integers.
         */
        std::auto_ptr< NHomGroupPresentation > identifyExtensionOverZ();

        /**
         * Attempts to prove that this and the given group presentation are
         * <i>simply isomorphic</i>.
         *
         * A <i>simple isomorphism</i> is an isomorphism where each generator
         * <i>g<sub>i</sub></i> of this presentation is sent to
         * some generator <i>g<sub>j</sub></i><sup>+/-1</sup> of the
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
         * @param other the group presentation to compare with this.
         * @return \c true if this routine could certify that the two group
         * presentations are simply isomorphic, or \c false if it could not.
         */
        bool identifySimplyIsomorphicTo(const NGroupPresentation& other) const;

        /**
         * Routine attempts to determine if this groups is clearly a free
         * product of other groups.  This is an unsophisticated algorithm
         * and will likely only have success if one has pre-processed the
         * presentation with simplification routines beforehand.
         *
         * @return a list of NGroupPresentation's.  This will be an empty
         *  list if the routine fails, and a list of allocated pointers
         *  otherwise.
         *
         * \apinotfinal Reconsider how the end-user should see this
         *  routine.
         */
        std::list< NGroupPresentation* > identifyFreeProduct() const;

        /**
         * Returns a TeX representation of this group presentation.
         * See writeTeX() for details on how this is formed.
         *
         * @return a TeX representation of this group presentation.
         */
        std::string toTeX() const;

        /**
         * Writes a TeX represesentation of this group presentation
         * to the given output stream.
         *
         * The output will be of the form &lt; generators | relators &gt;.
         * There will be no final newline.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         */
        void writeTeX(std::ostream& out) const;

        /**
         * A deprecated alias for compact(), which returns a
         * compact one-line representation of this group presentation.
         *
         * \deprecated This routine has been deprecated; use the
         * simpler-to-type compact() instead.
         *
         * @return a compact representation of this group presentation.
         */
        std::string toStringCompact() const;

        /**
         * Returns a compact one-line representation of this group presentation,
         * including details of all generators and relations.
         * See writeTextCompact() for details on how this is formed.
         *
         * @return a compact representation of this group presentation.
         */
        std::string compact() const;

        /**
         * Writes a compact represesentation of this group to the given
         * output stream.
         *
         * The output will be of the form &lt; generators | relators &gt;.
         * The full relations will be included, and the entire output
         * will be written on a single line.  There will be no final newline.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         */
        void writeTextCompact(std::ostream& out) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;

    private:
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
        struct NWordSubstitutionData {
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
                bool operator<( const NWordSubstitutionData &other ) const {
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
                void writeTextShort(std::ostream& out) const
                {
                        out<<"Target position "<<start_sub_at<<
                        " length of substitution "<<sub_length<<(invertB ?
                         " inverse reducer position " : " reducer position ")
                        <<start_from<<" score "<<score;
                }
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
            const NGroupExpression &this_word,
            const NGroupExpression &that_word,
            std::set< NWordSubstitutionData > &sub_list,
            unsigned long step=1 );

        /**
         *  A routine internal to the small cancellation simplification
         *  algorithm.
         *
         *  Given a word this_word and that_word, apply the substitution
         *  specified by sub_data to *this. See dehnAlgorithm() and struct
         *  NWordSubstitutionData.  In particular sub_data needs to be a
         *  valid substitution, usually it will be generated by
         *  dehnAlgorithmSubMetric.
         */
        static void applySubstitution(
            NGroupExpression& this_word,
            const NGroupExpression &that_word,
            const NWordSubstitutionData &sub_data );

};

/*@}*/

// Inline functions for NGroupExpressionTerm

inline NGroupExpressionTerm::NGroupExpressionTerm() {
}
inline NGroupExpressionTerm::NGroupExpressionTerm(unsigned long newGen,
        long newExp) : generator(newGen), exponent(newExp) {
}
inline NGroupExpressionTerm::NGroupExpressionTerm(
        const NGroupExpressionTerm& cloneMe) :
        generator(cloneMe.generator), exponent(cloneMe.exponent) {
}

inline NGroupExpressionTerm& NGroupExpressionTerm::operator = (
        const NGroupExpressionTerm& cloneMe) {
    generator = cloneMe.generator;
    exponent = cloneMe.exponent;
    return *this;
}

inline bool NGroupExpressionTerm::operator == (
        const NGroupExpressionTerm& other) const {
    return (generator == other.generator) && (exponent == other.exponent);
}

inline NGroupExpressionTerm NGroupExpressionTerm::inverse() const {
    return NGroupExpressionTerm(generator, -exponent);
}

inline bool NGroupExpressionTerm::operator += (
        const NGroupExpressionTerm& other) {
    if (generator == other.generator) {
        exponent += other.exponent;
        return true;
    } else
        return false;
}

inline bool NGroupExpressionTerm::operator < (
        const NGroupExpressionTerm& other) const {
    return ( (generator < other.generator) ||
             ( (generator == other.generator) &&
               ( exponent < other.exponent ) ) );
}

// Inline functions for NGroupExpression

inline NGroupExpression::NGroupExpression() {
}

inline NGroupExpression::NGroupExpression(const NGroupExpression& cloneMe) :
        ShareableObject(), terms(cloneMe.terms) {
}

inline bool NGroupExpression::operator==(const NGroupExpression& comp) const {
    return terms == comp.terms;
}

inline NGroupExpression& NGroupExpression::operator=(
        const NGroupExpression& cloneMe) {
    terms = cloneMe.terms;
    return *this;
}

inline std::list<NGroupExpressionTerm>& NGroupExpression::getTerms() {
    return terms;
}

inline const std::list<NGroupExpressionTerm>& NGroupExpression::getTerms()
        const {
    return terms;
}

inline unsigned long NGroupExpression::getNumberOfTerms() const {
    return terms.size();
}

inline bool NGroupExpression::isTrivial() const {
    return terms.empty();
}

inline unsigned long NGroupExpression::wordLength() const {
    unsigned long retval(0);
    std::list<NGroupExpressionTerm>::const_iterator it;
    for (it = terms.begin(); it!=terms.end(); it++)
        retval += labs((*it).exponent);
    return retval;
}

inline unsigned long NGroupExpression::getGenerator(unsigned long index)
        const {
    return getTerm(index).generator;
}

inline long NGroupExpression::getExponent(unsigned long index) const {
    return getTerm(index).exponent;
}

inline void NGroupExpression::addTermFirst(const NGroupExpressionTerm& term) {
    terms.push_front(term);
}

inline void NGroupExpression::addTermFirst(unsigned long generator,
        long exponent) {
    terms.push_front(NGroupExpressionTerm(generator, exponent));
}

inline void NGroupExpression::addTermLast(const NGroupExpressionTerm& term) {
    terms.push_back(term);
}

inline void NGroupExpression::addTermLast(unsigned long generator,
        long exponent) {
    terms.push_back(NGroupExpressionTerm(generator, exponent));
}

inline void NGroupExpression::erase() {
    terms.clear();
}

// Inline functions for NGroupPresentation

inline NGroupPresentation::NGroupPresentation() : nGenerators(0) {
}

inline NGroupPresentation::~NGroupPresentation() {
    for_each(relations.begin(), relations.end(),
        FuncDelete<NGroupExpression>());
}

inline unsigned long NGroupPresentation::addGenerator(unsigned long num) {
    return (nGenerators += num);
}

inline void NGroupPresentation::addRelation(NGroupExpression* rel) {
    relations.push_back(rel);
}

inline unsigned long NGroupPresentation::getNumberOfGenerators() const {
    return nGenerators;
}

inline unsigned long NGroupPresentation::getNumberOfRelations() const {
    return relations.size();
}

inline const NGroupExpression& NGroupPresentation::getRelation(
        unsigned long index) const {
    return *relations[index];
}

inline void NGroupPresentation::writeTextShort(std::ostream& out) const {
    out << "Group presentation: " << nGenerators << " generators, "
        << relations.size() << " relations";
}

inline unsigned long NGroupPresentation::relatorLength() const {
    unsigned long retval(0);
    for (unsigned long i=0; i<relations.size(); i++)
        retval += relations[i]->wordLength();
    return retval;
}

} // namespace regina

#endif

