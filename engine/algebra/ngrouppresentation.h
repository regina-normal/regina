
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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
#include "utilities/memutils.h"
#include "shareableobject.h"
#include "utilities/ptrutils.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/nabeliangroup.h"
//#include "algebra/nhomgrouppresentation.h"

namespace regina {

class NFile;
class NHomGroupPresentation; 

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents a power of a generator in a group presentation.
 */
struct NGroupExpressionTerm {
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

    /**
     * Writes this term to the given old-style binary file.
     *
     * \deprecated For the preferred way to write data to file, see
     * NGroupExpression::writeXMLData() instead.
     *
     * \pre The given file is currently opened for writing.
     *
     * \ifacespython Not present.
     *
     * @param out the file to which to write.
     */
    void writeToFile(NFile& out) const;
    /**
     * Reads a term from the given old-style binary file.
     *
     * \deprecated For the preferred way of reading group
     * presentations from file, see class NXMLGroupPresentationReader
     * instead.
     *
     * \pre The given file is currently opened for reading.
     *
     * \ifacespython Not present.
     *
     * @param in the file from which to read.
     * @return the term read from the given file.
     */
    static NGroupExpressionTerm readFromFile(NFile& in);

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
std::ostream& operator << (std::ostream& out, const NGroupExpressionTerm& term);

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
class NGroupExpression : public ShareableObject {
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
         * Returns the list of terms in this expression.
         * These are the actual terms stored internally; any
         * modifications made to this list will show up in the
         * expression itself.
         *
	 * For instance, the expression <tt>g1^2 g3^-1 g6</tt> has list consisting
	 * of three terms <tt>g1^2</tt>, <tt>g3^-1</tt> and <tt>g6^1</tt> in that
	 * order.
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
	 * For instance, the expression <tt>g1^2 g3^-1 g6</tt> has list consisting
	 * of three terms <tt>g1^2</tt>, <tt>g3^-1</tt> and <tt>g6^1</tt> in that
	 * order.
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
	 * For instance, the expression <tt>g1^2 g3^-1 g6</tt> contains three terms.	
	 *  See also getWordLength(). 
         *
         * @return the number of terms.
         */
        unsigned long getNumberOfTerms() const;
        /**
         * Returns the length of the word, i.e. the minimal number of letters with exponent
	 *  +1 or -1 for which this word is expressable as a product. 
	 *
	 * For instance, the expression <tt>g1^2 g3^-1 g6</tt> is a word of length
	 *  four.  See also getNumberOfTerms(). 
         *
         * @return the length of the word. 
         */
	unsigned long wordLength() const;

        /**
         * Erases this word. Turns it into the identity element.
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
	 * Multiplies *this on the right by word. 
	 */
	void addTermsLast( const NGroupExpression& word);
	/**
	 * Multiplies *this on the left by word. 
	 */
	void addTermsFirst( const NGroupExpression& word);

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
	 * Given two words, A and B, one wants to know how one can make 
         *  substitutions into A using variants of the word B.  This 
         *  structure holds that data.  For example, if:
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
		unsigned long start_sub_at; // where in A do we start?
		unsigned long start_from;   // where in B do we start?
		unsigned long sub_length;   // how many letters from B do we use?
	        bool invertB;               // do we invert B before making the substitution?
		unsigned long score;        // what is the "score" of this substitution.
		// needed to create std::set< NWordSubstitutionData > objects
	 	// we set up the ordering so that highest score objects are at begin()
		bool operator<( const NWordSubstitutionData &other ) const
		{
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
	 *  This is the core of the Dehn algorithm for hyperbolic groups.       
         *  Given two words, *this and that_word, this routine searches for 
         *  subwords of that_word (in the cyclic sense), and builds a table 
         *  of substitutions one can make from that_word into *this.  The 
         *  table is refined so that one knows the "value" of each 
         *  substitution -- the extent to which the substitution would shorten
         *  the word *this.   This is to allow for intelligent choices of
	 *  substitutions by whichever algorithms call this one.  
	 *
	 *  This algorithm assumes that *this and that_word are cyclically      
         *  reduced words.  If you feed it non-cyclically reduced words it 
         *  will give you suggestions although they will not be as strong
         *  as if the words were cyclically reduced.  It also only adds 
         *  to sub_list, so for best results pass it an empty sub-list.
	 */
	void dehnAlgorithmSubMetric( const NGroupExpression &that_word, 
               std::set< NWordSubstitutionData > &sub_list ) const;

	/**  
	 *  Given a word *this and that_word, apply the substitution specified
         *  by sub_data to *this. See dehnAlgorithm() and struct 
         *  NWordSubstitutionData.  In particular sub_data needs to be a 
	 *  valid substitution, usually it will be generated by 
         *  dehnAlgorithmSubMetric.  
	 */
	void applySubstitution( const NGroupExpression &that_word, const 
             NWordSubstitutionData &sub_data );

        /**
         * Writes a chunk of XML containing this expression.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;
        /**
         * Writes this expression to the given old-style binary file.
         *
         * \deprecated For the preferred way to write data to file, see
         * writeXMLData() instead.
         *
         * \pre The given file is currently opened for writing.
         *
         * \ifacespython Not present.
         *
         * @param out the file to which to write.
         */
        void writeToFile(NFile& out) const;
        /**
         * Reads an expression from the given old-style binary file.
         * The expression will be newly allocated, and responsibility for
         * its destruction lies with the caller of this routine.
         *
         * \deprecated For the preferred way of reading group
         * presentations from file, see class NXMLGroupPresentationReader
         * instead.
         *
         * \pre The given file is currently opened for reading.
         *
         * \ifacespython Not present.
         *
         * @param in the file from which to read.
         * @return a newly allocated expression read from the given file, or
         * 0 if problems arose.
         */
        static NGroupExpression* readFromFile(NFile& in);

        /**
         * The text representation will be of the form
         * <tt>g2^4 g13^-5 g4</tt>.
	 *
	 * @return a std::string representation of the word.
         */
	std::string stringOutput() const;

        /**
         * The text representation will be of the form
         * <tt>g_2^4 g_{13}^{-5} g_4</tt>, i.e. suitable for TeX.
	 *
	 * @return a std::string representation of the word.
         */
	std::string TeXOutput() const;

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
 * \testpart
 */
class NGroupPresentation : public ShareableObject {
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
         * Destroys the group presentation.
         * All relations that are stored will be deallocated.
         */
        virtual ~NGroupPresentation();

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
         * @return the expression that the requested relation sets to 1;
         * for instance, if the relation is <tt>g1^2 g2 = 1</tt> then
         * this will be the expression <tt>g1^2 g2</tt>.
         */
        const NGroupExpression& getRelation(unsigned long index) const;

        /**
         * See intelligentSimplify(NHomGroupPresentation*&)
         *
         * @return \c true if and only if the group presentation was changed.
         */
        bool intelligentSimplify();
	/**
	 * Reduces this presentation using the Dehn algorithm for hyperbolic 
         * groups, i.e. small cancellation theory.   This means we look to see 
         * if part of one relator can be used to simplify others.  If so, 
	 * make the substitution and simplify.  We continue until no more 
         * presentation-shortening substitutions are available.  We follow that
         *  by killing any available generators using words where generators
         * appear a single time. 
         *
         * @param must be a null pointer, the algorithm initializes the pointer
         *  and reductionMap. This means the user is responsible for 
         *  de-allocating reductionMap whenever they're done with it. 
         *
         * @return \c true if and only if the presentation was changed. 
         * 
         * \todo \optlong This routine could use some small tweaks -- 
         *   recognition of utility of some score==0 moves, such as 
         *   commutators, for example. 
	 */
	bool intelligentSimplify(NHomGroupPresentation*& reductionMap);

        /**
         * Attempts to recognise the group corresponding to this
         * presentation.  This routine is much more likely to be
         * successful if you have already called intelligentSimplify().
         *
         * Note that the presentation might be simplified a little
         * during the execution of this routine, although not nearly as
         * much as would be done by intelligentSimplify().
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
         * Writes this group presentation to the given old-style binary file.
         *
         * \deprecated For the preferred way to write data to file, see
         * writeXMLData() instead.
         *
         * \pre The given file is currently opened for writing.
         *
         * \ifacespython Not present.
         *
         * @param out the file to which to write.
         */
        void writeToFile(NFile& out) const;
        /**
         * Reads a group presentation from the given old-style binary file.
         * The presentation will be newly allocated, and responsibility for
         * its destruction lies with the caller of this routine.
         *
         * \deprecated For the preferred way of reading group
         * presentations from file, see class NXMLGroupPresentationReader
         * instead.
         *
         * \pre The given file is currently opened for reading.
         *
         * \ifacespython Not present.
         *
         * @param in the file from which to read.
         * @return a newly allocated presentation read from the given file, or
         * 0 if problems arose.
         */
        static NGroupPresentation* readFromFile(NFile& in);

	/**
	 *  The sum of the wordLength()s of the relators.  Used as a coarse measure of the complexity
	 * of the presentation.
	 */
	unsigned long relatorLength() const;

        /**
         * Computes the abelianization of this group. 
         * @return a pointer to the abelianization. 
         */
        std::auto_ptr<NAbelianGroup> unMarkedAbelianization() const;

        /**
         * Computes the abelianization of this group. 
         * The coordinates in the chain complex correspond
         * to the generators and relators for this group. 
         *
         * @return a pointer to the abelianization. 
         */
        std::auto_ptr<NMarkedAbelianGroup> markedAbelianization() const;

	/**
	 * Writes a single string description of the group presentation. No endlines!
	 *
	 * @return a std::string describing the presentation, < generators | relators >
	 */
	std::string stringOutput() const;

	/**
	 * Writes a single string description of the group presentation. No endlines. 
         * In TeX 
	 *
	 * @return a std::string describing the presentation, < generators | relators >
	 */
	std::string TeXOutput() const;

        /**
         * Assignment operator.
         */
        void operator=(const NGroupPresentation& copyMe);

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
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

// Inline functions for NGroupExpression

inline NGroupExpression::NGroupExpression() {
}

inline NGroupExpression::NGroupExpression(const NGroupExpression& cloneMe) :
        ShareableObject(), terms(cloneMe.terms) {
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

inline unsigned long NGroupExpression::wordLength() const {
 unsigned long retval(0); 
 std::list<NGroupExpressionTerm>::const_iterator it; 
 for (it = terms.begin(); it!=terms.end(); it++) retval += abs((*it).exponent);
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

inline void NGroupExpression::erase()
{
        terms.resize(0);
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

inline unsigned long NGroupPresentation::relatorLength() const
{
unsigned long retval(0);
for (unsigned long i=0; i<relations.size(); i++)
 retval += relations[i]->wordLength();
return retval;
}


} // namespace regina

#endif

