/**
 * Supporting code for "Computing closed essential surfaces in knot
 * complements", B.A. Burton, A. Coward and S. Tillmann, arXiv:1212.1531.
 *
 * Copyright (c) 2012, Benjamin A. Burton.
 *
 * Released under the GNU General Public License, version 2 or greater.
 *
 * This header file contains the full branching algorithms for
 * enumerating and identifying normal surfaces under various constraints.
 *
 * These algorithms are used by candidate.cc and compress.cc, which are
 * two small wrapper programs that use the branching algorithms to
 * enumerate and test candidate essential surfaces respectively.
 *
 * All of the code in this header file is thoroughly documented.
 *
 * This code needs to be built against Regina 4.94 or later.  Until
 * Regina 4.94 is released, you can build it against the current trunk in
 * Regina's online source code repository, which you can check out from
 * http://regina.sourceforge.net/ .
 *
 * Please check again for future versions of this software bundle at
 * http://www.maths.uq.edu.au/~bab/code/ .
 *
 * - Ben Burton, 15 December 2012.
 */

#include <file/nxmlfile.h>
#include <maths/matrixops.h>
#include <maths/nmatrixint.h>
#include <maths/ninteger.h>
#include <snappea/nsnappeatriangulation.h>
#include <surfaces/nnormalsurfacelist.h>
#include <surfaces/nsanstandard.h>
#include <surfaces/nsquad.h>
#include <surfaces/nsquadoct.h>
#include <surfaces/nsstandard.h>
#include <triangulation/ntriangulation.h>
#include <utilities/nbitmask.h>
#include <utilities/nthread.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <list>

typedef regina::NInteger<> IntType;

/**
 * Optimisation flags:
 * Define any combination of the following flags to switch \e off
 * various optimisations.
 * This is for diagnostic purposes only.
 */
// #define REGINA_NOOPT_REORDER_COLUMNS
// #define REGINA_NOOPT_MIN_FEASIBLE

/**
 * Define REGINA_VERIFY_LPDATA to check invariants as the algorithm runs.
 * This checking is slow and can increase the running time significantly.
 */
// #define REGINA_VERIFY_LPDATA

/**
 * Define REGINA_TREE_TRACE to output details of the type vectors as we
 * walk through the search tree.  This is for diagnostic purposes only.
 */
// #define REGINA_TREE_TRACE

/**
 * Define REGINA_SURFACE_TRACE to output details of full normal surfaces
 * (possibly immersed, or branched), as well as their corresponding
 * type vectors, as we walk through the search tree.
 * This is for diagnostic purposes only.
 */
// #define REGINA_SURFACE_TRACE

using namespace regina;

/**
 * A trie that stores a set of type vectors of a fixed length.
 *
 * This class forms part of the tree traversal algorithm for enumerating
 * vertex normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3.
 *
 * A type vector is a sequence of digits, each between 0 and \a nTypes-1
 * inclusive.  Type vectors are represented as arrays of characters:
 * these are not strings, but simply sequences of one-byte integers.
 * In particular, you cannot print them (since they use raw integer
 * values, not ASCII digits).  The length of a type vector must be
 * passed alongside it (i.e., there is no special terminating character).
 *
 * A type vector \a v is said to \e dominate \a u if, for each position
 * \a i, either v[i] == u[i] or else u[i] == 0.  So, for instance,
 * (1,0,2,3) dominates (1,0,2,0), which in turn dominates (1,0,0,0).
 * Domination is a partial order, not a total order: for instance,
 * neither of (1,0,2,0) or (1,0,3,0) dominates the other.
 *
 * We assume that all type vectors used in this trie have the same
 * length.  This is important, since we optimise the implementation by
 * ignoring trailing zeroes, which means that this trie cannot distinguish
 * between a vector \a v and the same vector with additional zeroes
 * appended to its end.
 *
 * Internally, each node of the trie is represented by a separate
 * TypeTrie object, each of which is responsible for managing the
 * lifespan of its descendant nodes.  Externally, a user only needs
 * to create and manage a single TypeTrie object (which becomes
 * the root of the trie).
 *
 * \pre \a nTypes is at most 256.  The typical value for \a nTypes for
 * normal surface enumeration is \a nTypes = 4.
 *
 * \ifacespython Not present.
 */
template <int nTypes>
class TypeTrie {
    private:
        TypeTrie<nTypes>* child_[nTypes];
            /**< If this node is \a k levels deeper than the root of
                 the trie (that is, it corresponds to the \a kth position
                 in the type vector), then child_[i] stores the subtrie
                 of type vectors \a v for which v[k] == i. */
        bool elementHere_;
            /**< \c true if the path from the root of the trie to this
                 node precisely describes the elements of some type
                 vector in the set, ignoring any trailing zeroes.
                 (In particular, the zero vector is in the set if and
                 only if \a elementHere_ is \c true at the root node.)
                 If this is \c false at a non-root node, then the fact
                 that the node was ever constructed means that the path
                 from the root to this node describes some \e prefix of
                 a longer type vector in the set that has additional
                 subsequent non-zero elements. */

    public:
        /**
         * Initialises an empty trie.
         */
        inline TypeTrie() : elementHere_(false) {
            memset(child_, 0, sizeof(TypeTrie<nTypes>*) * nTypes);
        }

        /**
         * Destroys this trie.
         */
        inline ~TypeTrie() {
            for (int i = 0; i < nTypes; ++i)
                delete child_[i];
        }

        /**
         * Resets this to the empty trie.
         */
        inline void clear() {
            for (int i = 0; i < nTypes; ++i) {
                delete child_[i];
                child_[i] = 0;
            }
            elementHere_ = false;
        }

        /**
         * Inserts the given type vector into this trie.
         *
         * \pre The given length \a len is non-zero, and is fixed throughout
         * the life of this trie; that is, it is the same every time
         * insert() or dominates() is called.
         *
         * @param entry the type vector to insert.
         * @param len the number of elements in the given type vector.
         */
        void insert(const char* entry, unsigned len) {
            // Strip off trailing zeroes.
            while (len > 0 && ! entry[len - 1])
                --len;

            // Insert this type vector, creating new nodes only when required.
            TypeTrie<nTypes>* node = this;
            const char* next = entry;
            for (int pos = 0; pos < len; ++pos, ++next) {
                if (! node->child_[*next])
                    node->child_[*next] = new TypeTrie<nTypes>();
                node = node->child_[*next];
            }
            node->elementHere_ = true;
        }

        /**
         * Determines whether the given type vector dominates any vector
         * in this trie.
         *
         * \pre The given length \a len is non-zero, and is fixed throughout
         * the life of this trie; that is, it is the same every time
         * insert() or dominates() is called.
         *
         * @param vec the type vector to test.
         * @param len the number of elements in the given type vector.
         * @return \c true if and only if \a vec dominates some type
         * vector stored in this trie.
         */
        bool dominates(const char* vec, unsigned len) const {
            // Strip off trailing zeroes.
            while (len > 0 && ! vec[len - 1])
                --len;

            // At worst we have a recursive O(2^len) search on our hands.
            // Create a stack of options that describe which branch of the
            // trie we follow at each stage of the search.
            //
            // Here node[i] will store the next candidate node to try at
            // depth i in the tree (where the root is at depth 0), or 0
            // if we have exhausted our options at that level of the search.
            const TypeTrie<nTypes>** node =
                new const TypeTrie<nTypes>*[len + 2];

            int level = 0;
            node[0] = this;
            while (level >= 0) {
                if ((! node[level]) || level > len) {
                    // If node[level] is 0, then we ran out of siblings
                    // at this level.
                    // If level > len, then any vector in this subtree
                    // must have non-zero elements where vec only has zeros.
                    // Either way, we need to backtrack.

                    // Move back up one level...
                    --level;
                    // ... and then move to the next sibling at this (higher)
                    // level.
                    if (level > 0 &&
                            node[level] == node[level - 1]->child_[0] &&
                            vec[level - 1])
                        node[level] = node[level - 1]->child_[vec[level - 1]];
                    else if (level >= 0)
                        node[level] = 0;
                    continue;
                }

                // Process the node at the current level.
                if (node[level]->elementHere_) {
                    // This node (padded with trailing zeroes) is
                    // dominated by the given type vector.
                    delete[] node;
                    return true;
                }

                // Descend further into the tree.
                //
                // If vec[level] == 0, we must descend to child_[0].
                // Otherwise we try child_[0] and then child_[type].
                //
                // The following code sets node[level + 1] to the first non-zero
                // child in this selection, or to 0 if all such children are 0.
                if (node[level]->child_[0])
                    node[level + 1] = node[level]->child_[0];
                else
                    node[level + 1] = node[level]->child_[vec[level]];
                ++level;
            }

            delete[] node;
            return false;
        }
};

/**
 * A matrix class for use with linear programming.
 *
 * This class is used in the tree traversal algorithms for enumerating
 * and locating vertex normal surfaces, as described in "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * The operations on this matrix class are tailored and optimised
 * specifically for use with the dual simplex method in the context
 * of a repetitive backtracking search.  As a result, the API is
 * cumbersome and highly specialised, which makes this matrix class
 * inappropriate for general use.
 *
 * It is \e critical that, before using such a matrix, you reserve space
 * for its elements, and then fix a specific size.  A matrix for which
 * both tasks have been done will be called \a initialised.  You can
 * initialise a matrix in one of two ways:
 *
 * - by using the (\a rows, \a columns) constructor, which does
 *   everything for you;
 *
 * - by using the default (no-arguments) constructor, then calling reserve(),
 *   and then calling one of the initialisation routines initClone() or
 *   initIdentity().
 *
 * You may call the initialisation initClone() and initIdentity() routines
 * more than once (e.g., during a backtracking search), and you may use
 * different matrix sizes each time.  However, you may never use more
 * elements than you originally reserved space for.
 *
 * This matrix is stored in dense form.  All elements are of the class
 * IntType.
 *
 * \ifacespython Not present.
 */
class LPMatrix {
    private:
        IntType* dat_;
            /**< The elements of this matrix as a single long array,
                 stored in row-major order.  This array stores as many
                 elements as were originally reserved, which might be
                 more than (but can never be less than) the current size
                 of the matrix according to \a rows_ and \a cols_. */
        unsigned rows_;
            /**< The number of rows in this matrix. */
        unsigned cols_;
            /**< The number of columns in this matrix. */

    public:
        /**
         * Creates an uninitialised matrix with no memory storage.
         *
         * You \e must call reserve() and then either initClone() or
         * initIdentity() before this matrix will become initialised.
         */
        inline LPMatrix() : dat_(0) {
        }

        /**
         * Creates a fully initialised \a rows by \a cols matrix
         * with all elements set to zero.
         *
         * This routine reserves space for precisely \a rows * \a cols
         * elements.  In other words, you may later re-initialise the matrix
         * to become smaller if you like, but you cannot re-initialise the
         * matrix to become larger.
         *
         * @param rows the number of rows in the new matrix.  This must
         * be strictly positive.
         * @param cols the number of columns in the new matrix.  This must
         * be strictly positive.
         */
        inline LPMatrix(unsigned rows, unsigned cols) :
                dat_(new IntType[rows * cols]),
                rows_(rows), cols_(cols) {
            // The IntType default constructor sets all elements to zero.
        }

        /**
         * Destroys this matrix and all of the data it contains.
         *
         * You can safely destroy a matrix that is uninitialised
         * or only partially initialised (i.e., space has been reserved but
         * the matrix size is not set).
         */
        inline ~LPMatrix() {
            delete[] dat_;
        }

        /**
         * Reserves enough space to store the elements of a
         * \a maxRows by \a maxCols matrix.  This is just an upper
         * bound: your matrix may end up using fewer elements than this,
         * but it cannot use more.
         *
         * This matrix will still not be initialised until you call
         * either initClone() or initIdentity().  See the class notes for
         * details.
         *
         * \pre This matrix was created using the default (no-argument)
         * constructor, and you have not called any other routines on
         * this matrix since.
         *
         * \warning To elaborate on the precondition above: you can only
         * call reserve() once, and if you did not use the default
         * LPMatrix constructor then you cannot call it at all.
         * Any additional calls to reserve() will result in a memory leak.
         *
         * @param maxRows an upper bound on the number of rows that you
         * will need for this matrix.  This must be strictly positive.
         * @param maxCols an upper bound on the number of columns that
         * you will need for this matrix.  This must be strictly positive.
         */
        inline void reserve(unsigned maxRows, unsigned maxCols) {
            dat_ = new IntType[maxRows * maxCols];
        }

        /**
         * Initialises this matrix to a copy of the given matrix.
         *
         * This matrix does not yet need to be initialised, but it does
         * need to have enough space reserved.
         *
         * You may call this routine on an already-initialised matrix,
         * and you may use this routine to assign it a different size
         * (as long as enough space was originally reserved).
         *
         * \pre If this matrix has not been initialised before, then
         * reserve() must have already been called.
         *
         * \pre This matrix has enough space reserved for at least
         * clone.rows() * clone.columns() elements.
         *
         * @param clone the matrix to copy.
         */
        inline void initClone(const LPMatrix& clone) {
            rows_ = clone.rows_;
            cols_ = clone.cols_;
            std::copy(clone.dat_, clone.dat_ + clone.rows_ * clone.cols_, dat_);
        }

        /**
         * Initialises this matrix to the identity matrix of the given size.
         *
         * This matrix does not yet need to be initialised, but it does
         * need to have enough space reserved.
         *
         * You may call this routine on an already-initialised matrix,
         * and you may use this routine to assign it a different size
         * (as long as enough space was originally reserved).
         *
         * \pre If this matrix has not been initialised before, then
         * reserve() must have already been called.
         *
         * \pre This matrix has enough space reserved for at least
         * \a size * \a size elements.
         *
         * @param size the number of rows, and also the number of
         * columns, that will be assigned to this matrix.
         * This must be strictly positive.
         */
        void initIdentity(unsigned size) {
            // Don't fuss about optimising this, since we only call it once
            // in the entire tree traversal algorithm.
            rows_ = cols_ = size;

            unsigned r, c;
            for (r = 0; r < rows_; ++r)
                for (c = 0; c < cols_; ++c)
                    entry(r, c) = (r == c ? 1 : long(0));
        }

        /**
         * Returns a read-write reference to the given element of this
         * matrix.
         *
         * @param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * @param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline IntType& entry(unsigned row, unsigned col) {
            return dat_[row * cols_ + col];
        }

        /**
         * Returns a read-only reference to the given element of this
         * matrix.
         *
         * @param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * @param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline const IntType& entry(unsigned row, unsigned col) const {
            return dat_[row * cols_ + col];
        }

        /**
         * Returns the number of rows in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * @return the number of rows.
         */
        inline unsigned rows() const {
            return rows_;
        }

        /**
         * Returns the number of columns in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * @return the number of columns.
         */
        inline unsigned columns() const {
            return cols_;
        }

        /**
         * Swaps the two given rows of this matrix.
         * The two arguments \a r1 and \a r2 may be equal (in which case
         * the matrix will be left unchanged).
         *
         * @param r1 the index of the first row to swap.  This must be
         * between 0 and rows()-1 inclusive.
         * @param r2 the index of the second row to swap.  This must be
         * between 0 and rows()-1 inclusive.
         */
        inline void swapRows(unsigned r1, unsigned r2) {
            if (r1 != r2)
                std::swap_ranges(dat_ + r1 * cols_, dat_ + r1 * cols_ + cols_,
                    dat_ + r2 * cols_);
        }

        /**
         * Applies a particular row operation to this matrix.
         *
         * Specifically, row \a dest will be replaced with the linear
         * combination:
         * (\a destCoeff * row \a dest - \a srcCoeff * row \a src) / \a div.
         *
         * \pre \a dest and \a src are not equal.
         * \pre It is known in advance that every integer in
         * (\a destCoeff * row \a dest - \a srcCoeff * row \a src)
         * will be divisible by \a div.  In other words, it is known in
         * advance that we can use exact integer division without remainders.
         *
         * @param destCoeff the coefficient applied to row \a dest in
         * the linear combination.
         * @param dest the index of the row to replace.  This must be
         * between 0 and rows()-1 inclusive.
         * @param srcCoeff the coefficient applied to row \a src in
         * the linear combination.
         * @param src the index of the other row used in this linear
         * combination.  This must be between 0 and rows()-1 inclusive.
         * @param div the integer to divide the final row by.  This must
         * be non-zero.
         */
        inline void combRow(const IntType& destCoeff, unsigned dest,
                const IntType& srcCoeff, unsigned src,
                const IntType& div) {
            IntType* ps = dat_ + src * cols_;
            IntType* pd = dat_ + dest * cols_;
            if (div > 1)
                for (unsigned i = 0; i < cols_; ++i) {
                    *pd *= destCoeff;
                    *pd -= (srcCoeff * *ps++);
                    (*pd++).divByExact(div);
                }
            else
                for (unsigned i = 0; i < cols_; ++i) {
                    *pd *= destCoeff;
                    *pd++ -= (srcCoeff * *ps++);
                }
        }

        /**
         * Applies a particular row operation to this matrix, and then
         * normalises.
         *
         * Specifically, row \a dest will be replaced with the linear
         * combination:
         * (\a destCoeff * row \a dest - \a srcCoeff * row \a src);
         * then, if row \a dest is non-zero, it will be normalised by
         * dividing through by the gcd of its elements.  Note that this gcd
         * is always taken to be positive (i.e., the final normalisation
         * will never change the signs of the elements in the row).
         *
         * \pre \a dest and \a src are not equal.
         *
         * @param destCoeff the coefficient applied to row \a dest in
         * the linear combination.
         * @param dest the index of the row to replace.  This must be
         * between 0 and rows()-1 inclusive.
         * @param srcCoeff the coefficient applied to row \a src in
         * the linear combination.
         * @param src the index of the other row used in this linear
         * combination.  This must be between 0 and rows()-1 inclusive.
         * @return the positive gcd that row \a dest was scaled down by,
         * or 0 if row \a dest is entirely zero.
         */
        inline IntType combRowAndNorm(const IntType& destCoeff,
                unsigned dest, const IntType& srcCoeff, unsigned src) {
            IntType gcdRow; // Initialised to zero.
            IntType* ps = dat_ + src * cols_;
            IntType* pd = dat_ + dest * cols_;
            unsigned i;
            for (i = 0; i < cols_; ++i, pd++, ps++) {
                *pd *= destCoeff;
                *pd -= (srcCoeff * *ps);
                if (gcdRow != 1)
                    gcdRow = gcdRow.gcd(*pd); // gcd() guarantees to be >= 0.
            }
            if (gcdRow > 1) {
                pd = dat_ + dest * cols_;
                for (i = 0; i < cols_; ++i)
                    (*pd++).divByExact(gcdRow);
            }
            return gcdRow;
        }

        /**
         * Negates all elements in the given row of this matrix.
         *
         * @param row the row whose elements should be negated.
         * This must be between 0 and rows()-1 inclusive.
         */
        inline void negateRow(unsigned row) {
            IntType *p = dat_ + row * cols_;
            for (unsigned i = 0; i < cols_; ++p, ++i)
                p->negate();
        }

        /**
         * Writes this matrix to the given output stream.
         * The output is "rough" and wasteful, and is intended for
         * debugging purposes only.  The precise output format is
         * subject to change in future versions of Regina.
         *
         * @param out the output stream to write to.
         */
        void dump(std::ostream& out) const {
            out << "---------------------------------" << std::endl;
            unsigned r, c;
            for (r = 0; r < rows_; ++r) {
                for (c = 0; c < cols_; ++c)
                    out << entry(r, c) << ' ';
                out << std::endl;
            }
            out << "---------------------------------" << std::endl;
        }
};

/**
 * Stores an adjusted matrix of matching equations from the
 * underlying triangulation, in sparse form.
 *
 * This class forms part of the tree traversal algorithms for enumerating
 * and locating normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * The adjustments (which are all carried out in the LPInitialTableaux
 * class constructor) are as follows:
 *
 * - the rows of the matching equation matrix have been reordered so
 *   that the first rank() rows are full rank;

 * - the columns of the matching equation matrix have been reordered
 *   according to the permutation returned by columnPerm().  This is a
 *   heuristic reordering designed to improve the performance of the tree
 *   traversal algorithm; see columnPerm() for a list of constraints that
 *   such a reordering must satisfy.
 *
 * There is also optional support for adding extra linear constraints
 * (such as a constraint on Euler characteristic).  These extra
 * constraints are supplied by the template parameter \a LPConstraint,
 * and will generate LPConstraint::nConstraints additional rows and columns
 * (used by the additional variables that evaluate the corresponding linear
 * functions).  If there are no additional constraints, simply use the
 * template parameter LPConstraintNone.
 *
 * In some cases, it may be impossible to add the extra linear constraints
 * that you would like (for instance, the constraints might require some
 * preconditions on the underlying triangulation that are not met).  If this
 * is a possibility in your setting, you should call constraintsBroken() to
 * test this as soon as the LPInitialTableaux has been constructed.  Even if
 * the constraints could not be added correctly, the tableaux will be left in a
 * consistent state (the constraints will just be treated as zero functions
 * instead).
 *
 * This class is optimised for working with \e columns of the matrix
 * (in particular, multiplying columns of this matrix by rows of some
 * other matrix).
 *
 * This class can only work in quadrilateral normal coordinates
 * (NNormalSurfaceList::QUAD) or standard normal coordinates
 * (NNormalSurfaceList::STANDARD).  No other coordinate systems are
 * supported.
 *
 * \warning The implementation of this class relies on the fact that the
 * sum of <i>absolute values</i> of all coefficients in each column is
 * at most four (not counting the rows for any optional extra constraints).
 * If you are extending this class to work with more general matching
 * equation matrices, you may need to change the implementation accordingly.
 *
 * \pre The template parameter LPConstraint must be one of the subclasses of
 * LPConstraintBase.  See the LPConstraintBase class notes for further details.
 */
template <typename LPConstraint>
class LPInitialTableaux {
    private:
        /**
         * Stores a single column of the adjusted matching equation matrix
         * in sparse form.
         *
         * Specifically, this stores the location of each +1 entry,
         * and the location of each -1 entry.  If some entry in the matrix
         * is greater than +1 or less than -1, we represent it using
         * multiple +1 or -1 entries in the same matrix location.
         *
         * For any additional rows that represent extra linear constraints,
         * we inherit the coefficients directly from LPConstraint::Coefficients.
         */
        struct Col : public LPConstraint::Coefficients {
            unsigned nPlus;
                /**< The total number of +1 entries in this column. */
            unsigned plus[4];
                /**< The rows containing these +1 entries, in any order.
                     The same row may appear in this list more than once
                     (indicating a +2, +3 or +4 entry in the matrix). */
            unsigned nMinus;
                /**< The total number of -1 entries in this column. */
            unsigned minus[4];
                /**< The rows containing these -1 entries, in any order.
                     The same row may appear in this list more than once
                     (indicating a -2, -3 or -4 entry in the matrix). */

            /**
             * Initialises an empty column.
             */
            inline Col() : nPlus(0), nMinus(0) {
            }

            /**
             * Adds the given entry in the given row to this column.
             *
             * \pre No entry in the given row has been added to this column
             * yet.
             *
             * \pre The sum of absolute values of all entries in this
             * column must never exceed 4.
             *
             * @param row the row containing the given value.
             * @param val the value at this location in the matrix.
             */
            inline void push(unsigned row, int val) {
#ifdef REGINA_VERIFY_LPDATA
                if ((val > 0 && val + nPlus > 4) ||
                        (val < 0 && val - nMinus < -4)) {
                    std::cerr << "BAD MATRIX" << std::endl;
                    ::exit(1);
                }
#endif
                for (; val > 0; --val)
                    plus[nPlus++] = row;
                for (; val < 0; ++val)
                    minus[nMinus++] = row;
            }
        };

        NTriangulation* tri_;
            /**< The underlying triangulation. */
        int coords_;
            /**< The coordinate system used for the matrix of matching
                 equations; this must be one of NNormalSurfaceList::QUAD
                 or NNormalSurfaceList::STANDARD. */
        NMatrixInt* eqns_;
            /**< The adjusted matching equation matrix, in dense form.
                 The precise adjustments that we make are described in the
                 LPInitialTableaux class notes. */
        unsigned rank_;
            /**< The rank of this tableaux, taking into account any additional
                 constraints from the template parameter LPConstraint. */
        unsigned cols_;
            /**< The number of columns in this tableaux, taking into account
                 any additional constraints from the template parameter
                 LPConstraint. */

        Col* col_;
            /**< An array of size \a cols_, storing the individual
                 columns of this adjusted matrix in sparse form. */

        int* columnPerm_;
            /**< A permutation of 0,...,cols_ - 1 that maps column numbers
                 in the adjusted matrix to column numbers in the original
                 (unmodified) matrix of matching equations that was originally
                 derived from the triangulation.  See columnPerm() for
                 more details on what this permutation means and what
                 constraints it must adhere to. */

        bool constraintsBroken_;
            /**< Indicates whether or not the extra constraints from the
                 template parameter \a LPConstraints were added successfully.
                 See the LPInitialTableaux class notes for details. */

    public:
        /**
         * Construts this adjusted sparse matrix of matching equations.
         *
         * \pre The given triangulation is non-empty.
         *
         * @param tri the underlying 3-manifold triangulation.
         * @param coords the coordinate system to use for the matrix of
         * matching equations; this must be one of NNormalSurfaceList::QUAD
         * or NNormalSurfaceList::STANDARD.
         * @param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces, or \c false if we
         * should optimise the tableaux for an existence test (such as
         * searching for a non-trivial normal disc or sphere).
         */
        LPInitialTableaux(NTriangulation* tri, int coords, bool enumeration) :
                tri_(tri), coords_(coords) {
            // Fetch the original (unadjusted) matrix of matching equations.
            eqns_ = regina::makeMatchingEquations(tri, coords);

            // Compute the rank of the matrix, and reorder its rows so
            // the first \a rank_ rows are full rank.
            rank_ = regina::rowBasis(*eqns_);

            // Reorder the columns using a good heuristic.
            cols_ = eqns_->columns() + LPConstraint::nConstraints;
            columnPerm_ = new int[cols_];
            reorder(enumeration);

            // Create and fill the sparse columns.
            col_ = new Col[cols_];
            unsigned r, c;
            for (c = 0; c < eqns_->columns(); ++c)
                for (r = 0; r < rank_; ++r)
                    if (eqns_->entry(r, c) != 0)
                        col_[c].push(r, eqns_->entry(r, c).longValue());

            // Add in the final row(s) for any additional constraints.
            constraintsBroken_ =
                ! LPConstraint::addRows(col_, columnPerm_, tri);
            rank_ += LPConstraint::nConstraints;
        }

        /**
         * Destroys this matrix.
         */
        inline ~LPInitialTableaux() {
            delete eqns_;
            delete[] col_;
            delete[] columnPerm_;
        }

        /**
         * Returns the underlying 3-manifold triangulation from which the
         * matching equations were derived.
         *
         * @return the underlying triangulation.
         */
        inline NTriangulation* tri() const {
            return tri_;
        }

        /**
         * Returns the rank of this matrix.
         *
         * Note that, if we are imposing extra constraints through the
         * template parameter LPConstraint, then there will be extra variables
         * to enforce these, and so the rank will be larger than the rank of
         * the original matching equation matrix.
         *
         * @return the matrix rank.
         */
        inline unsigned rank() const {
            return rank_;
        }

        /**
         * Returns the number of columns in this matrix.
         *
         * Note that, if we are imposing extra constraints through the
         * template parameter LPConstraint, then there will be extra variables
         * to enforce these, and so the number of columns will be larger than
         * in the original matching equation matrix.
         *
         * @return the number of columns.
         */
        inline unsigned columns() const {
            return cols_;
        }

        /**
         * Returns the number of columns that correspond to normal
         * coordinates.  This is precisely the number of columns in the
         * original matrix of matching equations.
         *
         * @return the number of normal coordinate columns.
         */
        inline unsigned coordinateColumns() const {
            return eqns_->columns();
        }

        /**
         * Indicates whether or not the extra constraints from the template
         * parameter \a LPConstraints were added successfully.
         * This query function is important because some constraints require
         * additional preconditions on the underlying triangulation, and
         * cannot be added if these preconditions are not satisfied.
         *
         * Even if the extra constraints were not added successfully, this
         * tableaux will be left in a consistent state (the extra constraints
         * will be treated as zero functions).  See the LPInitialTableaux class
         * notes for further details.
         *
         * @return \c true if the constraints were \e not added
         * successfully, or \c false if the constraints were added successfully.
         */
        inline bool constraintsBroken() const {
            return constraintsBroken_;
        }

        /**
         * Returns the permutation that describes how the columns of
         * the matching equation matrix were reordered.  This permutation maps
         * column numbers in this adjusted matching equation matrix to
         * column numbers in the original (unmodified) matching equation
         * matrix that was originally derived from the triangulation.
         *
         * The permutation is returned as an array of columns()
         * integers, such that column \a i of this adjusted matrix corresponds
         * to column <tt>columnPerm()[i]</tt> of the original matrix.
         *
         * If you are imposing additional constraints through the
         * template parameter LPConstraint, then the corresponding extra
         * variables will be included in the permutation; however, these are
         * never moved and will always remain the rightmost variables in
         * this system (i.e., the columns of highest index).
         *
         * As well as the requirement that this is a genuine permutation of
         * 0,...,columns()-1, this array will also adhere to the
         * following constraints.  In the following discussion, \a n refers
         * to the number of tetrahedra in the underlying triangulation.
         *
         * - The quadrilateral coordinate columns must appear as the
         *   first 3<i>n</i> columns of the adjusted matrix.
         *   In particular, when working in the 7<i>n</i>-dimensional
         *   standard coordinate system, the remaining 4<i>n</i> triangle
         *   coordinate columns must appear last.
         *
         * - The quadrilateral coordinate columns must be grouped by
         *   tetrahedron and ordered by quadrilateral type.  In other
         *   words, for each \a i = 0,...,\a n-1, there will be some
         *   tetrahedron \a j for which the three columns
         *   3<i>i</i>, 3<i>i</i>+1 and 3<i>i</i>+2 refer to the
         *   quadrilaterals in tetrahedron \a j of types 0, 1 and 2
         *   respectively.  Phrased loosely, we are allowed to reorder
         *   the tetrahedra, but not the quadrilateral coordinates
         *   within each tetrahedron.
         *
         * - The triangle coordinate columns (if we have them) must likewise
         *   be grouped by tetrahedron, and these tetrahedra must appear in
         *   the same order as for the quadrilateral types.  In other
         *   words, for each \a i = 0,...,\a n-1, the quadrilateral columns
         *   3<i>i</i>, 3<i>i</i>+1 and 3<i>i</i>+2 and the triangle columns
         *   3<i>n</i>+4<i>i</i>, 3<i>n</i>+4<i>i</i>+1, 3<i>n</i>+4<i>i</i>+2 
         *   and 3<i>n</i>+4<i>i</i>+3 all refer to the same tetrahedron.
         *
         * @return details of the permutation describing how columns
         * were reordered.
         */
        inline const int* columnPerm() const {
            return columnPerm_;
        }

        /**
         * Computes the inner product of (i) the given row of the given
         * matrix with (ii) the given column of this matrix.
         *
         * This routine is optimised to use the sparse representation of
         * columns in this matrix.
         *
         * \pre The given matrix \a m has precisely rank() columns.
         *
         * @param m the matrix whose row we will use in the inner product.
         * @param mRow the row of the matrix \a m to use in the inner product.
         * @param thisCol the column of this matrix to use in the inner product.
         * @return the resulting inner product.
         */
        inline IntType multColByRow(const LPMatrix& m, unsigned mRow,
                unsigned thisCol) const {
            IntType ans = col_[thisCol].innerProduct(m, mRow);

            unsigned i;
            for (i = 0; i < col_[thisCol].nPlus; ++i)
                ans += m.entry(mRow, col_[thisCol].plus[i]);
            for (i = 0; i < col_[thisCol].nMinus; ++i)
                ans -= m.entry(mRow, col_[thisCol].minus[i]);
            return ans;
        }

        /**
         * A variant of multColByRow() that takes into account any adjustments
         * to the tableaux that are required when this is a quadrilateral
         * column being used to represent an octagon type.
         *
         * The LPData class offers support for octagonal almost normal
         * surfaces, in which exactly one tetrahedron is allowed to have
         * exactly one octagon type.  We represent such an octagon as a
         * \e pair of incompatible quadrilaterals within the same tetrahedron.
         * See the LPData class notes for details on how this works.
         *
         * In some settings where we are using additional constraints
         * through the template parameter LPConstraint, these extra
         * constraints behave differently in the presence of octagons
         * (i.e., the coefficient of the octagon type is not just the
         * sum of coefficients of the two constituent quadrilateral types).
         * This routine effectively allows us to adjust the tableaux
         * accordingly.
         *
         * Specifically: this routine computes the inner product of
         * (i) the given row of the given matrix with (ii) the given
         * column of this matrix.  We assume that the given column of
         * this matrix describes one of the two quadrilateral coordinates
         * in some tetrahedron that together form an octagon type, and
         * (via the helper routine LPConstraint::Coefficients::innerProductOct)
         * we implicitly adjust the coefficients of our extra constraints
         * accordingly.
         *
         * This routine is optimised to use the sparse representation of
         * columns in this matrix.
         *
         * \pre The given matrix \a m has precisely rank() columns.
         *
         * \pre Column \a thisCol of this matrix describes one of the
         * two quadrilateral coordinates that are being combined to form
         * an octagon type within some tetrahedron.
         *
         * @param m the matrix whose row we will use in the adjusted
         * inner product.
         * @param mRow the row of the matrix \a m to use in the adjusted
         * inner product.
         * @param thisCol the column of this matrix to use in the adjusted
         * inner product.
         * @return the resulting adjusted inner product.
         */
        inline IntType multColByRowOct(const LPMatrix& m,
                unsigned mRow, unsigned thisCol) const {
            IntType ans = col_[thisCol].innerProductOct(m, mRow);

            unsigned i;
            for (i = 0; i < col_[thisCol].nPlus; ++i)
                ans += m.entry(mRow, col_[thisCol].plus[i]);
            for (i = 0; i < col_[thisCol].nMinus; ++i)
                ans -= m.entry(mRow, col_[thisCol].minus[i]);
            return ans;
        }

        /**
         * Fills the given matrix with the contents of this matrix.
         * This effectively copies this sparse
         * but highly specialised matrix representation into a dense
         * but more flexible matrix representation.
         *
         * \pre The given matrix has already been initialised to size
         * rank() * columns(), and all of its elements have already been
         * set to zero.  Note that this can all be arranged by calling
         * the constructor LPMatrix::LPMatrix(unsigned, unsigned).
         *
         * @param m the matrix to fill.
         */
        void fillInitialTableaux(LPMatrix& m) const {
            unsigned c, i;
            for (c = 0; c < cols_; ++c) {
                for (i = 0; i < col_[c].nPlus; ++i)
                    ++m.entry(col_[c].plus[i], c);
                for (i = 0; i < col_[c].nMinus; ++i)
                    --m.entry(col_[c].minus[i], c);

                // Don't forget any additional constraints that we added
                // as final rows to the matrix.
                col_[c].fillFinalRows(m, c);
            }
        }

    private:
        /**
         * Reorders the columns of the matching equation matrix.
         * This is a heuristic reordering that aims to reduce the number
         * of dead ends in the tree traversal algorithm, and thereby
         * improve the running time.
         *
         * Details of the reordering will be stored in the array
         * columnPerm_; see the columnPerm() notes for more information
         * on the constraints that this reordering is required to satisfy.
         *
         * This routine is called before any additional constraints are
         * added from the template parameter LPConstraint; that is, the
         * rows of the matrix are just the matching equations.  However,
         * we do already have the extra placeholder columns for the new
         * variables that correspond to these extra constraint(s).
         *
         * @param enumeration \c true if we should optimise the ordering
         * for a full enumeration of vertex surfaces, or \c false if we
         * should optimise the ordering for an existence test (such as
         * searching for a non-trivial normal disc or sphere).
         */
#ifdef REGINA_NOOPT_REORDER_COLUMNS
        void reorder(bool) {
            // This is a "do-nothing" version of reorder().
            int i, j;
            if (coords_ == NNormalSurfaceList::QUAD) {
                // Leave the columns exactly as they were.
                for (i = 0; i < cols_; ++i)
                    columnPerm_[i] = i;
            } else {
                // Keep the tetrahedra in the same order, but move
                // quadrilaterals to the front and triangles to the back
                // as required by columnPerm().
                int n = tri_->getNumberOfTetrahedra();
                for (i = 0; i < n; ++i) {
                    columnPerm_[3 * i] = 7 * i + 4;
                    columnPerm_[3 * i + 1] = 7 * i + 5;
                    columnPerm_[3 * i + 2] = 7 * i + 6;
                    columnPerm_[3 * n + 4 * i] = 7 * i;
                    columnPerm_[3 * n + 4 * i + 1] = 7 * i + 1;
                    columnPerm_[3 * n + 4 * i + 2] = 7 * i + 2;
                    columnPerm_[3 * n + 4 * i + 3] = 7 * i + 3;
                }
            }

            // This fills the columnPerm_ array; now we need to move the
            // columns of eqns_ around accordingly, and then finish off
            // columnPerm_ with the columns for additional constraints
            // from LPConstraint (if we have any).
            //
            // From here on we copy code directly from the "real" reorder()
            // below.
            int* tmp = new int[eqns_->columns()];
            std::copy(columnPerm_, columnPerm_ + eqns_->columns(), tmp);
            for (i = 0; i < eqns_->columns(); ++i) {
                // Column tmp[i] of the matrix should be moved to
                // column i.
                if (tmp[i] == i)
                    continue;

                eqns_->swapColumns(i, tmp[i]);

                // Adjust links to the old column i, which is now column tmp[i].
                for (j = i + 1; j < eqns_->columns(); ++j)
                    if (tmp[j] == i)
                        break; // This is the link we need to change.
#ifdef REGINA_VERIFY_LPDATA
                if (j == eqns_->columns()) {
                    std::cerr << "ERROR: Sorting error." << std::endl;
                    ::exit(1);
                }
#endif
                tmp[j] = tmp[i];
                tmp[i] = i;
            }
            delete[] tmp;

            // If we have extra variables for additional constraints or
            // objectives, append the corresponding entries to the end of
            // the permutation for completeness.
            for (i = 0; i < LPConstraint::nConstraints; ++i)
                columnPerm_[cols_ - i - 1] = cols_ - i - 1;
        }
#else
        void reorder(bool enumeration) {
            int n = tri_->getNumberOfTetrahedra();
            int i, j, k;

            // Fill the columnPerm_ array according to what kind of
            // problem we're trying to solve.
            if (coords_ == NNormalSurfaceList::STANDARD && enumeration) {
                // We're doing vertex enumeration in standard coordinates.
                //
                // Use exactly the same ordering of quadrilaterals that we
                // use in quadrilateral coordinates, and then just fill
                // in the triangles at the end.
                LPInitialTableaux quad(tri_, NNormalSurfaceList::QUAD,
                    true /* enumeration */);
                for (i = 0; i < n; ++i) {
                    k = quad.columnPerm()[3 * i] / 3;
                    columnPerm_[3 * i] = 7 * k + 4;
                    columnPerm_[3 * i + 1] = 7 * k + 5;
                    columnPerm_[3 * i + 2] = 7 * k + 6;
                    columnPerm_[3 * n + 4 * i] = 7 * k;
                    columnPerm_[3 * n + 4 * i + 1] = 7 * k + 1;
                    columnPerm_[3 * n + 4 * i + 2] = 7 * k + 2;
                    columnPerm_[3 * n + 4 * i + 3] = 7 * k + 3;
                }
            } else {
                // We're doing vertex enumeration in quad coordinates,
                // or we're in standard coordinates but just searching
                // for a single solution under some constraints.
                //
                // Process the rows in increasing order by number of tetrahedra
                // touched, and place the columns for each tetrahedron in the
                // order that we see them.  We place columns at the "back" of
                // the matrix, so that we fill the matrix in "reverse" order
                // from the last column to the first.

                // Track which rows have been processed so far.
                bool* used = new bool[rank_];
                std::fill(used, used + rank_, false);

                // Also track which tetrahedra have been used so far.
                bool* touched = new bool[n];
                std::fill(touched, touched + n, false);
                int nTouched = 0;

                // Off we go, one row at a time.
                int bestRow, best, curr;
                for (i = 0; i < rank_; ++i) {
                    // Seek out the ith row to process.
                    // Because the first rank_ rows of the matrix are full rank,
                    // we are guaranteed that this row will be non-zero.

                    best = n + 1; // No row touches more than n tetrahedra.

                    for (j = 0; j < rank_; ++j) {
                        if (used[j])
                            continue;
                        curr = 0;
                        for (k = 0; k < n; ++k) {
                            if (touched[k])
                                continue;
                            if (coords_ == NNormalSurfaceList::QUAD) {
                                // We're in quadrilateral coordinates.
                                if (eqns_->entry(j, 3 * k) != 0 ||
                                        eqns_->entry(j, 3 * k + 1) != 0 ||
                                        eqns_->entry(j, 3 * k + 2) != 0)
                                    ++curr;
                            } else {
                                // We're in standard coordinates.
                                if (eqns_->entry(j, 7 * k + 4) != 0 ||
                                        eqns_->entry(j, 7 * k + 5) != 0 ||
                                        eqns_->entry(j, 7 * k + 6) != 0)
                                    ++curr;
                            }
                            if (curr >= best)
                                break; // We cannot beat best from here.
                        }
                        if (curr < best) {
                            bestRow = j;
                            best = curr;
                        }
                    }

                    // The next row to process is bestRow.
                    // Find all the tetrahedra that it touches that we
                    // haven't already used yet, and place the corresponding
                    // columns at the end of the matrix.
                    used[bestRow] = true;
                    for (k = 0; k < n; ++k) {
                        if (touched[k])
                            continue;
                        if (coords_ == NNormalSurfaceList::QUAD) {
                            // We're in quadrilateral coordinates.
                            if ((eqns_->entry(bestRow, 3 * k) != 0 ||
                                    eqns_->entry(bestRow, 3 * k + 1) != 0 ||
                                    eqns_->entry(bestRow, 3 * k + 2) != 0)) {
                                touched[k] = true;

                                columnPerm_[3 * (n - nTouched) - 3] = 3 * k;
                                columnPerm_[3 * (n - nTouched) - 2] = 3 * k + 1;
                                columnPerm_[3 * (n - nTouched) - 1] = 3 * k + 2;

                                ++nTouched;
                            }
                        } else {
                            // We're in standard coordinates.
                            if ((eqns_->entry(bestRow, 7 * k + 4) != 0 ||
                                    eqns_->entry(bestRow, 7 * k + 5) != 0 ||
                                    eqns_->entry(bestRow, 7 * k + 6) != 0)) {
                                touched[k] = true;

                                // The quadrilateral columns...
                                columnPerm_[3 * (n - nTouched) - 3] = 7 * k + 4;
                                columnPerm_[3 * (n - nTouched) - 2] = 7 * k + 5;
                                columnPerm_[3 * (n - nTouched) - 1] = 7 * k + 6;
                                // ... and the triangle columns.
                                columnPerm_[3 * n + 4 * (n - nTouched) - 4] =
                                    7 * k;
                                columnPerm_[3 * n + 4 * (n - nTouched) - 3] =
                                    7 * k + 1;
                                columnPerm_[3 * n + 4 * (n - nTouched) - 2] =
                                    7 * k + 2;
                                columnPerm_[3 * n + 4 * (n - nTouched) - 1] =
                                    7 * k + 3;

                                ++nTouched;
                            }
                        }
                    }
                }

                // We have now processed all rows.  However, there may be some
                // tetrahedra that appear in no rows at all.
                // Make sure we catch these tetrahedra as well.
                for (k = 0; k < n; ++k) {
                    if (touched[k])
                        continue;
                    touched[k] = true;
                    if (coords_ == NNormalSurfaceList::QUAD) {
                        // We're in quadrilateral coordinates.
                        columnPerm_[3 * (n - nTouched) - 3] = 3 * k;
                        columnPerm_[3 * (n - nTouched) - 2] = 3 * k + 1;
                        columnPerm_[3 * (n - nTouched) - 1] = 3 * k + 2;
                    } else {
                        // We're in standard coordinates.
                        columnPerm_[3 * (n - nTouched) - 3] = 7 * k + 4;
                        columnPerm_[3 * (n - nTouched) - 2] = 7 * k + 5;
                        columnPerm_[3 * (n - nTouched) - 1] = 7 * k + 6;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 4] = 7 * k;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 3] = 7 * k + 1;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 2] = 7 * k + 2;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 1] = 7 * k + 3;
                    }
                    ++nTouched;
                }

                delete[] touched;
                delete[] used;
            }

            // At this point we have filled the columnPerm_ array
            // (except for the final columns for additional constraints
            // from LPConstraint, which we will deal with later).
            //
            // Now go ahead and actually move the columns around accordingly.
            int* tmp = new int[eqns_->columns()];
            std::copy(columnPerm_, columnPerm_ + eqns_->columns(), tmp);
            for (i = 0; i < eqns_->columns(); ++i) {
                // Column tmp[i] of the matrix should be moved to
                // column i.
                if (tmp[i] == i)
                    continue;

                eqns_->swapColumns(i, tmp[i]);

                // Adjust links to the old column i, which is now column tmp[i].
                for (j = i + 1; j < eqns_->columns(); ++j)
                    if (tmp[j] == i)
                        break; // This is the link we need to change.
#ifdef REGINA_VERIFY_LPDATA
                if (j == eqns_->columns()) {
                    std::cerr << "ERROR: Sorting error." << std::endl;
                    ::exit(1);
                }
#endif
                tmp[j] = tmp[i];
                tmp[i] = i;
            }
            delete[] tmp;

            // If we have extra variables for additional constraints or
            // objectives, append the corresponding entries to the end of
            // the permutation for completeness.
            for (i = 0; i < LPConstraint::nConstraints; ++i)
                columnPerm_[cols_ - i - 1] = cols_ - i - 1;
        }
#endif
};

/**
 * Stores an intermediate tableaux for the dual simplex method, and
 * contains all of the core machinery for using the dual simplex method.
 *
 * This class forms part of the tree traversal algorithms for enumerating
 * and locating normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * This class is designed to represent a state partway through the tree
 * traversal algorithm, where the tableaux has been altered to
 * constrain some variables:
 *
 * - Some variables have been "deactivated".  This means we fix them to
 *   zero permanently, and pretend that the corresponding columns do not
 *   exist in the matrix.  As a result, the rank of the matrix may
 *   smaller than it was when we began the tree traversal.
 *
 * - Some variables have been constrained to be positive; as described
 *   in Burton and Ozlen, it is safe to do this using the non-strict
 *   inequality x_i >= 1 (instead of the strict inequality x_i > 0, which
 *   is more difficult to enforce).  We enforce this constraing using a
 *   change of variable: we replace the variable x_i with (x_i - 1),
 *   which is then constrained to be non-negative as usual.  The new
 *   variable (x_i - 1) uses the same column in the tableaux (we perform
 *   the actual change of variable by editing the tableaux itself using
 *   column operations).  Be warned: as a result, when we arrive at a
 *   final solution and collect the values of the variables, we must
 *   remember to \e increment the values of any such variables by one.
 *
 * We do not store the full tableaux (which is dense and slow to work
 * with).  Instead we store the matrix of row operations that were
 * applied to the original starting tableaux (in the notation of Burton
 * and Ozlen, we store the matrix M_\beta^{-1}, where M is the original
 * matrix stored in the class LPInitialTableaux, and \beta is the current
 * basis).
 *
 * If the system is infeasible (because the constraints on variables as
 * described above are too severe), then the contents of the internal
 * data members are undefined (other than the data member \a feasible_,
 * which is guaranteed to be \c false).  This is because the code
 * is optimised to abort any operation as soon as infeasibility is detected,
 * which may leave the data members in a broken state.  If you are not sure,
 * you should always call isFeasible() before performing any other query
 * or operation on this tableaux.
 *
 * This class is designed to be used in a backtracking search, which
 * means the API is cumbersome but we can quickly rewrite and copy data.
 * The rules are as follows:
 *
 * - Before using an LPData object, you must reserve the necessary memory
 *   by calling reserve() and passing the original starting tableaux.
 *
 * - After this, you can reset the data by calling one of the initialisation
 *   routines initStart() or initClone(), and you can call these
 *   initialisation routines as often as you like.
 *
 * Like LPInitialTableaux, this class can enforce additional linear
 * constraints (such as positive Euler characteristic) through the template
 * parameter LPConstraint.  If there are no such constraints, simply use
 * the template parameter LPConstraintNone.
 *
 * Although the underlying coordinate system is based on quadrilaterals
 * and (optionally) triangles, this class has elementary support for
 * octagons also, as seen in \e almost normal surface theory.  For the
 * purposes of this class, an octagon is represented as a pair of
 * quadrilaterals of different types in the same tetrahedron: these meet
 * the boundary of the tetrahedron in the same arcs as a single octagon,
 * and therefore interact with the matching equations in the same way.
 *
 * To declare that you will be using octagons in some tetrahedron, you
 * must call constrainOct(quad1, quad2), where \a quad1 and \a quad2 are the
 * two corresponding quadrilateral columns.  This will have the following
 * effects, all of which may alter the tableaux:
 *
 * - There will be some changes of variable.  One of the two variables
 *   x_i will be replaced with (x_i - 1), forcing the number of octagons
 *   to be positive.  The other variable x_j will be replaced with
 *   (x_j - x_i), which will be set to zero and deactivated.  There is no
 *   guarantee as to which of the two variables \a quad1 and \a quad2
 *   will be kept and which will be deactivated: this will depend on the
 *   layout of the tableaux when constrainOct() is called.
 *
 * - If you are imposing additional constraints through the \a LPConstraint
 *   template parameter, the corresponding linear constraint functions
 *   may change their values (since the coefficients they use for
 *   octagon types need not be related to the coefficients for the two
 *   corresponding quadrilateral columns).  Any such changes are managed
 *   through the function LPConstraint::Coefficients::innerProductOct.
 *
 * This class has been optimised to ensure that you only have one
 * octagon type declared at any given time (which is consistent with the
 * constraints of almost normal surface theory).
 *
 * \pre The template parameter LPConstraint must be one of the subclasses of
 * LPConstraintBase.  See the LPConstraintBase class notes for further details.
 */
template <typename LPConstraint>
class LPData {
    private:
        const LPInitialTableaux<LPConstraint>* origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm began. */
        IntType* rhs_;
            /**< An array of length origTableaux_->rank() that stores
                 the column vector of constants on the right-hand side
                 of the current tableaux.  In the notation of Burton and
                 Ozlen, this is the column vector M_\beta^{-1} * b.
                 If \a rank_ is smaller than origTableaux_->rank() then
                 the "extra" entries rhs_[rank_, rank_+1, ...] may have
                 undefined values, and should simply be ignored. */
        LPMatrix rowOps_;
            /**< The matrix of row operations that we apply to the
                 original starting tableaux, as described in the class
                 notes.  In the notation of Burton and Ozlen, this is
                 the matrix M_\beta^{-1}.  This is a square matrix of
                 side length origTableaux_->rank(). */
        unsigned rank_;
            /**< The rank of the current tableaux, taking into account
                 any changes such as deactivation of variables.  This
                 will be at most (but quite possibly less than)
                 origTableaux_->rank().  We guarantee that the first
                 \a rank_ rows of the current tableaux are full rank (and so
                 any subsequent rows should simply be ignored from here on). */
        int* basis_;
            /**< An array of length origTableaux_->rank() that stores the
                 \a rank_ variables that form the current basis.  In
                 particular, for each i = 0,...,rank_-1, basis_[i] is the
                 basis variable whose defining row is row i.
                 If \a rank_ is smaller than origTableaux_->rank() then any
                 trailing entries in this array have undefined values,
                 and should simply be ignored. */
        int* basisRow_;
            /**< An array of length origTableaux_->columns() that indicates
                 which row of the current tableaux holds the defining equation
                 for each basis variable.  Specifically:
                 - if column \a i corresponds to a basic variable, then the
                   defining row for this basis variable is row basisRow_[i];
                 - if column \a i corresponds to an active non-basic variable,
                   then basisRow_[i] will be strictly negative;
                 - if column \a i has been deactivated, then basisRow_[i]
                   will be zero.

                 For each i = 0,...,rank_-1, basisRow_[basis_[i]] == i. */
        bool feasible_;
            /**< Indicates whether or not the current system of
                 constraints is feasible. */
        int octPrimary_;
            /**< If we have declared an octagon type, this stores the
                 column that we use to count the octagons.
                 This will be one of the two quadrilateral columns that
                 together "represent" the octagon type, as described in
                 the class notes.
                 If we have not declared an octagon type, this is -1. */
        int octSecondary_;
            /**< If we have declared an octagon type, this stores the
                 second of the two quadrilateral columns that together
                 "represent" the octagon type, as described in the class
                 notes.  This is the quadrilateral column that we set to
                 zero and deactivate (as opposed to \a octPrimary_, which
                 we keep to count the number of octagons).
                 If we have not declared an octagon type, this variable
                 is undefined. */

    public:
        /**
         * Constructs a new tableaux.  You \e must call reserve() before
         * doing anything else with this tableaux.
         */
        inline LPData() : rhs_(0), basis_(0), basisRow_(0) {
        }

        /**
         * Destroys this tableaux.  This is safe even if reserve() was
         * never called.
         */
        inline ~LPData() {
            delete[] rhs_;
            delete[] basis_;
            delete[] basisRow_;
        }

        /**
         * Reserves enough memory for this tableaux to work with.
         * You \e must call this routine before doing anything else with
         * this tableaux.
         *
         * The data in this tableaux will not be initialised, and the
         * contents and behaviour of this tableaux will remain undefined
         * until you call one of the initialisation routines initStart()
         * or initClone().
         *
         * @param origTableaux the original starting tableaux that holds the
         * adjusted matrix of matching equations, before the tree traversal
         * algorithm began.
         */
        void reserve(const LPInitialTableaux<LPConstraint>* origTableaux) {
            origTableaux_ = origTableaux;
            rhs_ = new IntType[origTableaux->rank()];
            rowOps_.reserve(origTableaux->rank(), origTableaux->rank());
            basis_ = new int[origTableaux->rank()];
            basisRow_ = new int[origTableaux->columns()];
        }

        /**
         * Initialises this tableaux by beginning at the original
         * starting tableaux and working our way to any feasible basis.
         *
         * This routine also explicitly enforces the additional constraints
         * from the template parameter LPConstraint (i.e., this routine
         * is responsible for forcing the corresponding linear
         * function(s) to be zero or strictly positive as appropriate).
         *
         * It is possible that a feasible basis cannot be found; you
         * should test isFeasible() after running this routine to see
         * whether this is the case.
         *
         * \pre reserve() has already been called.
         */
        void initStart() {
            // In this routine we rely on the fact that the
            // LPInitialTableaux constructor ensures that the original
            // tableaux has full rank.

            // Begin at the original tableaux, with no row operations performed
            // and with all equations having a right-hand side of zero.
            rowOps_.initIdentity(origTableaux_->rank());
            std::fill(rhs_, rhs_ + origTableaux_->rank(), 0);
            rank_ = origTableaux_->rank();
            octPrimary_ = -1;

            // From here, find any feasible basis.
            findInitialBasis();

            // Since RHS = 0, this basis is already feasible.
            feasible_ = true;

            // Finally, enforce our additional linear constraints.
            // This might break feasibility.
            LPConstraint::constrain(*this, origTableaux_->columns());
        }

        /**
         * Initialises this tableaux to be a clone of the given tableaux.
         * This is used in the tree traversal algorithm as we work our way
         * down the search tree, and child nodes "inherit" tableaux from
         * their parent nodes.
         *
         * \pre reserve() has already been called.
         *
         * @param parent the tableaux to clone.
         */
        void initClone(const LPData& parent) {
            // If the parent tableaux is infeasible, mark this tableaux as
            // infeasible also and abort.
            feasible_ = parent.feasible_;
            if (! feasible_)
                return;

            // The parent tableaux is feasible: clone all of its data.
            std::copy(parent.rhs_, parent.rhs_ + parent.rank_, rhs_);
            rowOps_.initClone(parent.rowOps_);
            rank_ = parent.rank_;
            memcpy(basis_, parent.basis_, parent.rank_ * sizeof(int));
            memcpy(basisRow_, parent.basisRow_,
                origTableaux_->columns() * sizeof(int));
            octPrimary_ = parent.octPrimary_;
            octSecondary_ = parent.octSecondary_;
        }

        /**
         * Returns the number of columns in this tableaux.
         *
         * Note that, if we are imposing extra constraints through the
         * template parameter LPConstraint, then there will be extra variables
         * to enforce these, and so the number of columns will be larger than
         * in the original matching equation matrix.
         *
         * @return the number of columns.
         */
        inline unsigned columns() const {
            return origTableaux_->columns();
        }

        /**
         * Returns the number of columns in this tableaux that correspond to
         * normal coordinates.  This is precisely the number of columns in the
         * original matrix of matching equations.
         *
         * @return the number of normal coordinate columns.
         */
        inline unsigned coordinateColumns() const {
            return origTableaux_->coordinateColumns();
        }

        /**
         * Returns whether or not this system is feasible.
         *
         * A system may become infeasible when we add too many extra
         * constraints on the variables (such as forcing them to be
         * positive, or setting them to zero); see the LPData class
         * notes for details on these constraints.
         *
         * \warning As explained in the class notes, if this system is
         * infeasible then any queries or operations (other than calling
         * isFeasible() itself) are undefined.
         *
         * @return \c true if this system is feasible, or \c false if it
         * is infeasible.
         */
        inline bool isFeasible() const {
            return feasible_;
        }

        /**
         * Determines whether the given variable is currently active.
         * See the LPData class notes for details.
         *
         * @param pos the index of the variable to query.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         */
        inline bool isActive(unsigned pos) const {
            // If basisRow_[pos] < 0, the variable is active and non-basic.
            // If basisRow_[pos] > 0, the variable is active and basic.
            // If basisRow_[pos] == 0, then:
            //   - if rank_ > 0 and basis_[0] == pos, then the variable
            //     is active and basic;
            //   - otherwise the variable is not active.
            return ! (basisRow_[pos] == 0 &&
                (rank_ == 0 || basis_[0] != pos));
        }

        /**
         * Constrains this system further by setting the given variable
         * to zero and deactivating it.  See the LPData class notes for
         * details.
         *
         * This routine will work even if the given variable has already
         * been deactivated (and it will do nothing in this case).
         *
         * \warning If you have previously called constrainPositive()
         * or constrainOct() on this variable, then these prior routines
         * will have performed a change of variable.  Any new call to
         * constraintZero() on this same variable will constraint the
         * \e new variable, not the original, and so might not have the
         * intended effect.
         *
         * @param pos the index of the variable that is to be set to
         * zero.  This must be between 0 and origTableaux_->columns()-1
         * inclusive.
         */
        void constrainZero(unsigned pos) {
            // If the variable has already been deactivated, there is
            // nothing to do.
            if (! isActive(pos))
                return;

            // If the system is infeasible beforehand, it will certainly
            // be infeasible afterwards.  In this case, abort.
            if (! feasible_)
                return;

            // If we ever do something that *might* make the basis
            // infeasible, we will set perhapsInfeasible to true as a
            // reminder to fix things later.
            bool perhapsInfeasible = false;

            // Is the variable currently in the basis?  If so, get it out.
            if (basisRow_[pos] >= 0) {
                int r = basisRow_[pos];
                int c;
                if (rhs_[r] == 0) {
                    // We can pivot in any other variable that appears in
                    // this basis row.  Choose the one with largest index.
                    for (c = origTableaux_->columns() - 1; c >= 0; --c)
                        if (basisRow_[c] < 0 /* c is active and non-basic */ &&
                                entry(r, c) != 0)
                            break;
                    if (c >= 0) {
                        pivot(pos, c);
                        // Because rhs_[r] == 0, this pivot can never create
                        // infeasibility.
                    } else {
                        // There are no other variables in this basis row!
                        // Our equation just looks like x_pos = 0.
                        //
                        // This means that, if we deactivate pos, we lose rank
                        // and we need to delete the corresponding row entirely.
                        --rank_;

                        // "Delete" the row by moving it to index rank_, which
                        // is now outside our scope of interest (since we are
                        // now only interested in rows 0,...,rank_-1).
                        if (r != rank_) {
                            std::swap(rhs_[r], rhs_[rank_]);
                            rowOps_.swapRows(r, rank_);
                            basis_[r] = basis_[rank_];
                            basisRow_[basis_[r]] = r;
                        }

                        // This column is already filled with zeroes
                        // from row 0 to rank_-1, because pos was in the basis.
                        // If we're in paranoid mode, check this.
#ifdef REGINA_VERIFY_LPDATA
                        for (r = 0; r < rank_; ++r)
                            if (! entry(r, pos).isZero()) {
                                std::cerr << "VERIFY: Drop error." << std::endl;
                                ::exit(1);
                            }
#endif
                    }
                } else {
                    // Because the system is feasible, we have rhs_[r] > 0.
                    // This means we can only pivot in a variable with positive
                    // coefficient in this basis row.  If there is one,
                    // choose the one with largest index.  If there is
                    // no such variable, the entire system becomes infeasible.
                    for (c = origTableaux_->columns() - 1; c >= 0; --c)
                        if (basisRow_[c] < 0 /* c is active and non-basic */ &&
                                entry(r, c) > 0)
                            break;
                    if (c < 0) {
                        // There is no possible variable to pivot in.
                        // The system must be infeasible.
                        feasible_ = false;
                        return;
                    }
                    pivot(pos, c);

                    // The pivot *might* have made the new basis infeasible.
                    // Remember this so we can fix things afterwards.
                    perhapsInfeasible = true;
                }
            }

            // The variable is out of the basis.  Deactivate the column
            // (which simply means setting basisRow to some non-negative
            // integer).
            basisRow_[pos] = 0;

#ifdef REGINA_VERIFY_LPDATA
            verify();
#endif

            // The variable is gone, but we might have pivoted to an
            // infeasible basis.  If this is a possibility, then move to a
            // feasible basis if we can.  If we cannot, then makeFeasible()
            // will detect this and set \a feasible_ to \c false.
            if (perhapsInfeasible)
                makeFeasible();

#ifdef REGINA_VERIFY_LPDATA
            verify();
#endif
        }

        /**
         * Constrains this system further by constraining the given variable
         * to be strictly positive.  We do this using a change of variable
         * that effectively replaces x_pos with the new variable
         * x'_pos = x_pos - 1 (which we simply constrain to be non-negative
         * as usual).  See the LPData class notes for details.
         *
         * This routine will work even if the given variable has already
         * been deactivated, but in this case the routine will
         * immediately set the system to infeasible and return.
         *
         * \warning If you have previously called constrainPositive()
         * or constrainOct() on this variable, then these prior routines
         * will have performed a change of variable.  Any new call to
         * constrainPositive() on this same variable will constrain the
         * \e new variable, not the original, and so might not have the
         * intended effect.
         *
         * @param pos the index of the variable that is to be constrained as
         * positive.  This must be between 0 and origTableaux_->columns()-1
         * inclusive.
         */
        void constrainPositive(unsigned pos) {
            // If the variable has already been deactivated, it cannot
            // be positive.
            if (! isActive(pos)) {
                feasible_ = false;
                return;
            }

            // If the system is infeasible beforehand, it will certainly
            // be infeasible afterwards.  In this case, abort.
            if (! feasible_)
                return;

            // Just replace x with (1+x'), where now x' must be non-negative.
            // This corresponds to subtracting column pos of this tableaux
            // from the right-hand side.
            // If there is any possibility that some entry on the
            // right-hand side could become negative, we must remember to
            // pivot back to feasibility.
            int r = basisRow_[pos];
            if (r >= 0) {
                // This variable is in the basis, and so there is only
                // one non-zero entry in column pos.
                // This makes subtracting column pos from rhs_ very easy
                // (just a single operation):
                if ((rhs_[r] -= entry(r, pos)) < 0)
                    makeFeasible();
            } else {
                // This variable is not in the basis.
                // We know nothing about the column, so just do a full
                // element-by-element column subtraction.
                for (r = 0; r < rank_; ++r)
                    rhs_[r] -= entry(r, pos);
                makeFeasible();
            }
        }

        /**
         * Declares that two quadrilateral coordinates within a tetrahedron
         * are to be combined into a single octagon coordinate, for use
         * with almost normal surfaces, and constrains the system accordingly.
         *
         * This constrains the system in several ways, as discussed in detail
         * in the LPData class notes.  In theory, we set the two quadrilateral
         * coordinates to be equal, and also insist that the number of octagons
         * be strictly positive.  In practice, we do this through several
         * changes of variable; see the LPData class notes for a detailed
         * discussion of precisely how the variables and tableaux will change.
         *
         * This routine will work even if one of the given quadrilateral
         * variables has already been deactivated, but in this case the
         * routine will immediately set the system to infeasible and return.
         *
         * \pre This is the first time constrainOct() has been called on
         * this tableaux.  This is because this class can only handle one
         * octagon type in the entire system.
         *
         * \pre Variables \a quad1 and \a quad2 represent different
         * quadrilateral coordinates in the same tetrahedron of the
         * underlying triangulation.
         *
         * \warning If you have previously called constrainPositive() or
         * constrainOct() on one of the given variables, then these prior
         * routines will have performed a change of variable.  Any new call
         * to constrainOct() involving this same variable will constrain the
         * \e new variable, not the original, and so might not have the
         * intended effect.
         *
         * @param quad1 one of the two quadrilateral types that we
         * combine to form the new octagon type.
         * @param quad2 the other of the two quadrilateral types that we
         * combine to form the new octagon type.
         */
        void constrainOct(unsigned quad1, unsigned quad2) {
            // If either variable has already been deactivated, it cannot
            // be positive.
            if (! (isActive(quad1) && isActive(quad2))) {
                feasible_ = false;
                return;
            }

            // If the system is infeasible beforehand, it will certainly
            // be infeasible afterwards.  In this case, abort.
            if (! feasible_)
                return;

            // Suppose we choose to count octagons using column i, and
            // to zero out and deactivate column j (where i and j are
            // quad1 and quad2 in some order).  Then our tasks are to:
            //
            // (i) Set x_i = x_j, by replacing the variable x_j with
            //     x_j' = x_j - x_i;
            // (ii) If we have any additional linear constraints through
            //      the template parameter LPConstraints, adjust the
            //      coefficients in columns i and/or j if necessary to
            //      reflect the presence of octagons (recalling that the
            //      coefficients for an octagon type need not be the sum
            //      of coefficients for the corresponding two
            //      quadrilateral types);
            // (iii) Set x_i >= 1 and x_j' = 0.
            //
            // We do this as follows:
            //
            // (i) Add column j to column i;
            // (ii) Add or subtract further multiples of the final column(s)
            //      to/from column i to reflect any change in coefficients;
            // (iii) Call constrainZero(j) and constrainPositive(i).
            //
            // We perform steps (i) and (ii) just by setting octPrimary_ and
            // octSecondary_ (this works because the implementation of entry()
            // adjusts its results according to the current values of
            // octPrimary_ and octSecondary_).  However, if we change a
            // column corresponding to a basic variable then we must
            // remember to reorganise the tableaux so that the column once
            // again contains all zeroes except for a single positive entry.

            int row1 = basisRow_[quad1];
            int row2 = basisRow_[quad2];

            if (row1 < 0) {
                if (row2 < 0) {
                    // Both variables are non-basic.
                    // We will use quad1 to count octagons.

                    // First adjust the columns in the tableaux...
                    octPrimary_ = quad1;
                    octSecondary_ = quad2;

                    // ... and then constrain variables as required.
                    // Since quad2 is non-basic, it is already zero so
                    // we can simply deactivate it.
                    basisRow_[quad2] = 0;
                    constrainPositive(quad1);
                } else {
                    // quad1 is non-basic, but quad2 is basic.
                    // Once again we will use quad1 to count octagons.

                    // First adjust the columns in the tableaux...
                    octPrimary_ = quad1;
                    octSecondary_ = quad2;

                    // ... and then constrain variables as required.
                    // This time quad2 might be non-zero, so we need to
                    // call the more expensive constrainZero(quad2).
                    constrainZero(quad2);
                    constrainPositive(quad1);
                }
            } else if (row2 < 0) {
                // quad2 is non-basic, but quad1 is basic.
                // This time we will use quad2 to count octagons.
                // Do what we did in the previous case, but the other way
                // around.

                // First adjust the columns in the tableaux...
                octPrimary_ = quad2;
                octSecondary_ = quad1;

                // ... and then constrain variables as required.
                constrainZero(quad1);
                constrainPositive(quad2);
            } else {
                // Both quad1 and quad2 are basic.
                //
                // Because we might need to adjust columns to reflect changes
                // in our additional linear constraints, whichever column we
                // keep could change in any crazy way.  We will need to adjust
                // things to make sure it looks like a basis column once again.

                // For no particular reason, let's choose to count
                // octagons using quad1, and eventually drop quad2.
                // It's going to be messy whichever we choose.

                // Adjust column quad1 now.
                octPrimary_ = quad1;
                octSecondary_ = quad2;

                // Although quad1 is in the basis, its column could now
                // look like anything.  We need to repair it so it
                // contains all zeroes except for cell (row1, quad1),
                // which must be strictly positive.
                IntType e1 = entry(row1, quad1);
                if (! e1.isZero()) {
                    // The (row1, quad1) entry is non-zero.
                    // It's clear what to do from here: make sure
                    // this entry is positive, perform row operations to
                    // clear out the rest of column quad1, and then restore
                    // feasibility.
                    if (e1 < 0) {
                        e1.negate();
                        rhs_[row1].negate();
                        rowOps_.negateRow(row1);
                    }

                    IntType coeff;
                    IntType gcdRow;
                    for (int r = 0; r < rank_; ++r) {
                        if (r == row1)
                            continue;

                        coeff = entry(r, quad1);
                        if (! coeff.isZero()) {
                            gcdRow = rowOps_.combRowAndNorm(e1, r,
                                coeff, row1);

                            // As usual, we already know in advance that
                            // gcdRow must divide into rhs_[r].
                            rhs_[r] *= e1;
                            rhs_[r] -= (coeff * rhs_[row1]);
                            rhs_[r].divByExact(gcdRow);
                        }
                    }

                    makeFeasible();

                    // Right: that takes care of the column adjustments.
                    // Now constrain the variables as required.
                    constrainZero(quad2);
                    constrainPositive(quad1);
                } else {
                    // The (row1, quad1) entry is now zero.
                    // Our solution here is to get quad1 out of the basis.
                    // Try to find some other non-zero coefficient in row1;
                    // note that the only possible locations for another
                    // non-zero coefficient are in non-basic columns.

                    // Choose the column with largest index.
                    int c;
                    for (c = origTableaux_->columns() - 1; c >= 0; --c)
                        if (basisRow_[c] < 0 /* active and non-basic */ &&
                                ! entry(row1, c).isZero())
                            break;
                    if (c >= 0) {
                        // We've found an alternative.
                        // Pivot quad1 out of the basis, and put column c
                        // in its place.
                        pivot(quad1, c);

                        // We now have a basis again with a corresponding
                        // tableaux, but the pivot may have broken feasibility.
                        makeFeasible();

                        // This takes care of the column adjustments.
                        // Now constrain the variables as required.
                        constrainZero(quad2);
                        constrainPositive(quad1);
                    } else {
                        // Every single entry in this row is zero!
                        //
                        // If rhs_[row1] == 0, this is a tautology.
                        // If rhs_[row1] != 0, this is an impossibility.
                        if (rhs_[row1] != 0) {
                            feasible_ = false;
                        } else {
                            // Just pull quad1 out of the basis.  Since
                            // the rank drops, we don't need another
                            // variable to replace it.
                            basisRow_[quad1] = -1;

                            // Move the empty row out of the active area
                            // of the matrix.
                            --rank_;
                            if (row1 != rank_) {
                                std::swap(rhs_[row1], rhs_[rank_]);
                                rowOps_.swapRows(row1, rank_);
                                basis_[row1] = basis_[rank_];
                                basisRow_[basis_[row1]] = row1;
                            }

                            // Since the RHS did not change, the system
                            // is still feasible.
                            // Constrain the variables as required.
                            constrainZero(quad2);
                            constrainPositive(quad1);
                        }
                    }
                }
            }
        }

        /**
         * Writes details of this tableaux to the given output stream.
         * The output is "rough" and wasteful, and is intended for
         * debugging purposes only.
         *
         * The precise output is subject to change in future versions
         * of Regina.
         *
         * @param out the output stream to write to.
         */
        void dump(std::ostream& out) const {
            unsigned r, c;
            out << "========================" << std::endl;
            for (r = 0; r < rank_; ++r)
                out << basis_[r] << ' ';
            out << std::endl;
            out << "========================" << std::endl;
            for (r = 0; r < rank_; ++r) {
                for (c = 0; c < origTableaux_->columns(); ++c)
                    out << entry(r, c) << ' ';
                out << std::endl;
            }
            out << "========================" << std::endl;
        }

        /**
         * Extracts the values of the individual variables from the
         * current basis, with some modifications (as described below).
         * The values of the variables are store in the given vector \a v.
         *
         * The modifications are as follows:
         *
         * - We extract variables that correspond to the original
         *   matching equations obtained from the underlying
         *   triangulation, \e not the current tableaux and \e not even
         *   the original starting tableaux stored in origTableaux_.
         *   In other words, when we fill the vector \a v we undo the
         *   column permutation described by LPInitialTableaux::columnPerm(),
         *   and we undo any changes of variable that were caused by
         *   calls to constrainPositive() and/or constrainOct().
         *
         * - To ensure that the variables are all integers, we scale the
         *   final vector by the smallest positive rational multiple
         *   for which all elements of the vector are integers.
         *   (This is why the output class is NRay and not NVector.)
         *
         * This routine is not used as an internal part of the tree traversal
         * algorithm; instead it is offered as a helper routine for
         * reconstructing the normal surfaces that result.
         *
         * \pre The given vector \a v has been initialised to the zero vector
         * of length origTableaux_->columns().  Note that the NRay constructor
         * will automatically initialise all elements to zero as required.
         *
         * \pre No individual coordinate column has had more than one call
         * to either of constrainPositive() or constrainOct() (otherwise
         * the coordinate will not be correctly reconstructed).  Any
         * additional columns arising from LPConstraint are exempt from
         * this requirement.
         *
         * @param v the vector into which the values of the variables
         * will be placed.
         * @param the type vector corresponding to the current state of
         * this tableaux, indicating which variables were previously fixed as
         * positive via calls to constrainPositive().  This is necessary
         * because LPData does not keep such historical data on its own.
         */
        void extractSolution(NRay& v, const char* type) const {
            // Fetch details on how to undo the column permutation.
            const int* columnPerm = origTableaux_->columnPerm();

            // We will multiply the solution vector by
            // lcm(basis coefficients in the tableaux), which will
            // ensure that the variables will all be integers.
            // This multiple might be too large, but we will shrink the
            // vector down again at the end of this routine.
            //
            // First compute this lcm.
            unsigned i;
            IntType lcm(1);
            for (i = 0; i < rank_; ++i)
                lcm = lcm.lcm(entry(i, basis_[i]));

            // Now compute (lcm * the solution vector).  We do not yet
            // take into account the change of variables x_i -> x_i - 1
            // that occurred each time we called constrainPositive(),
            // or the more complex changes of variables that occurred
            // each time we called constrainOct().
            //
            // All non-basic variables will be zero (and so we do
            // nothing, since the precondition states that they are
            // already zero in \a v).
            //
            // For basic variables, compute the values from the tableaux.
            // Because we are multiplying everything by lcm, the
            // divisions in the following code are all perfectly safe
            // (and give precise integer results).
            IntType coord;
            for (i = 0; i < rank_; ++i) {
                if (basis_[i] >= v.size())
                    continue;
                coord = lcm;
                coord *= rhs_[i];
                coord /= entry(i, basis_[i]);
                // Here we convert from the faster IntType back to the
                // old NLargeInteger, which is still used with normal surfaces.
                v.setElement(columnPerm[basis_[i]], NLargeInteger(coord));
            }

            // Now we take into account the changes of variable due
            // to past calls to constrainPositive(), as described above.
            // Since we have multiplied everything by lcm, instead of
            // adding +1 to each relevant variable we must add +lcm.
            unsigned pos;
            const unsigned nTets =
                origTableaux_->tri()->getNumberOfTetrahedra();
            NLargeInteger lcmAsLarge(lcm); // IntType -> NLargeInteger

            // First take into account the quadrilateral types...
            for (i = 0; i < nTets; ++i)
                if (type[i] && type[i] < 4) {
                    pos = columnPerm[3 * i + type[i] - 1];
                    v.setElement(pos, v[pos] + lcmAsLarge);
                }
            // ... and then the triangle types.
            for (i = 3 * nTets; i < v.size(); ++i)
                if (type[i - 2 * nTets]) {
                    pos = columnPerm[i];
                    v.setElement(pos, v[pos] + lcmAsLarge);
                }

            // Next take into account the changes of variable due to
            // past calls to constrainOct().
            if (octPrimary_ >= 0) {
                pos = columnPerm[octPrimary_];
                v.setElement(pos, v[pos] + lcmAsLarge);
                v.setElement(columnPerm[octSecondary_], v[pos]);
            }

            // To finish, divide through by the gcd so we have the
            // smallest multiple that is an integer vector.
            v.scaleDown();
        }

    private:
        /**
         * Returns the given entry in this tableaux.
         *
         * Since we do not store the full tableaux, this entry is
         * computed on the fly.  However, this computation is fast
         * because the computations use sparse vector multiplication.
         *
         * @param the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * @param the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * @return the requested entry in this tableaux.
         */
        inline IntType entry(unsigned row, unsigned col) const {
            // Remember to take into account any changes of variable due
            // to previous calls to constrainOct().
            if (octPrimary_ != col)
                return origTableaux_->multColByRow(rowOps_, row, col);
            else {
                IntType ans = origTableaux_->multColByRowOct(rowOps_,
                    row, col);
                ans += origTableaux_->multColByRowOct(rowOps_,
                    row, octSecondary_);
                return ans;
            }
        }

        /**
         * Performs a pivot in the dual simplex method.
         *
         * The column \a outCol is pivoted out of the current basis, and
         * the column \a inCol is pivoted in.
         *
         * No assumptions are made about the current state of column \a outCol;
         * in particular, it may be in a state "under construction"
         * whereby it has more than the one expected non-zero element.
         * However, assumptions \a are made about the current state of
         * column \a inCol, as noted in the preconditions below.
         *
         * \pre \a outCol represents an active basic variable, and
         * \a inCol represents an active non-basic variable.
         *
         * \pre The non-basic variable \a inCol has a non-zero entry in the
         * row of the tableaux that defines the basic variable \a outCol.
         *
         * @param outCol the index of the variable to pivot out of the basis.
         * @param inCol the index of the variable to pivot into the basis.
         */
        void pivot(unsigned outCol, unsigned inCol) {
            unsigned defRow = basisRow_[outCol];
            basisRow_[outCol] = -1;
            basisRow_[inCol] = defRow;
            basis_[defRow] = inCol;

            // Make sure that inCol has a positive coefficient in row defRow.
            IntType base = entry(defRow, inCol);
            if (base < 0) {
                base.negate();
                rhs_[defRow].negate();
                rowOps_.negateRow(defRow);
            }

            // Walk through the entire tableaux and perform row operations
            // to ensure that the only non-zero entry in column \a inCol
            // is the entry base in row defRow (as extracted above).
            IntType coeff;
            IntType gcdRow;
            unsigned r;
            for (r = 0; r < rank_; ++r) {
                if (r == defRow)
                    continue;

                coeff = entry(r, inCol);
                if (! coeff.isZero()) {
                    // Perform the row operation on the matrix...
                    gcdRow = rowOps_.combRowAndNorm(base, r, coeff, defRow);

                    // ... and on the right-hand side also.
                    // We already know that gcdRow must divide into rhs_[r],
                    // since rhs_ is obtained by multiplying the integer
                    // matrix rowOps_ with an integer vector.
                    rhs_[r] *= base;
                    rhs_[r] -= (coeff * rhs_[defRow]);
                    rhs_[r].divByExact(gcdRow);
                }
            }
        }

        /**
         * Finds an initial basis for the system using Gauss-Jordan
         * elimination.
         *
         * This routine is only ever called from initStart(), and
         * assumes that the current tableaux is just the original
         * starting tableaux (i.e., no changes have been made to the
         * tableaux at all).
         *
         * The implementation of this routine is a little naive and
         * heavy-handed, but since we only call it once in the entire
         * tree traversal algorithm, this does not really matter.
         *
         * \pre The current tableaux is precisely the original starting
         * tableaux; in particular, rhs_ is the zero vector, and rowOps_
         * is the identity matrix.
         */
        void findInitialBasis() {
            // Start with all variables active but non-basic.
            std::fill(basisRow_, basisRow_ + origTableaux_->columns(), -1);

            // Build a dense copy of the starting tableaux, which we
            // will work with as we perform our Gauss-Jordan elimination.
            LPMatrix tmp(origTableaux_->rank(), origTableaux_->columns());
            origTableaux_->fillInitialTableaux(tmp);

            // Off we go with our Gauss-Jordan elimination.
            // Since the original tableaux is full rank, we know in
            // advance that every row will define some basic variable.
            unsigned row;
            unsigned r, c;
            IntType base, coeff;
            IntType gcdRow;
            for (row = 0; row < rank_; ++row) {
                // Find the first non-zero entry in this row.
                // The corresponding column will become our next basic variable.
                for (c = 0; c < origTableaux_->columns(); ++c)
                    if (basisRow_[c] < 0 /* non-basic variable */ &&
                            ! tmp.entry(row, c).isZero())
                        break;

                // Since the original tableaux has full rank, we must
                // have found a non-zero entry.  However, for sanity,
                // add some code to deal with the situation where we did not.
                if (c == origTableaux_->columns()) {
                    // Impossible, assuming the matrix had the correct rank...
#ifdef REGINA_VERIFY_LPDATA
                    std::cerr << "ERROR: No initial basis, bad rank."
                        << std::endl;
                    ::exit(1);
#endif
                    // ... but deal with it anyway by just dropping rank.
                    --rank_;
                    if (row != rank_) {
                        tmp.swapRows(row, rank_);
                        rowOps_.swapRows(row, rank_);
                        std::swap(rhs_[row], rhs_[rank_]);
                    }
                    --row; // We will ++row again for the next loop iteration.
                    continue;
                }

                // Here is our non-zero entry.
                // Make this a basis variable.
                basis_[row] = c;
                basisRow_[c] = row;

                // Make the corresponding non-zero entry positive.
                base = tmp.entry(row, c);
                if (base < 0) {
                    base.negate();
                    tmp.negateRow(row);
                    rowOps_.negateRow(row);
                    rhs_[row].negate();
                }

                // Make sure this basis variable has zero coefficients
                // in all other rows.
                for (r = 0; r < rank_; ++r) {
                    if (r == row)
                        continue;
                    coeff = tmp.entry(r, c);
                    if (! coeff.isZero()) {
                        gcdRow = rowOps_.combRowAndNorm(base, r, coeff, row);
                        tmp.combRow(base, r, coeff, row, gcdRow);

                        // We already know that gcdRow must divide into rhs_[r],
                        // since rhs_ is obtained by multiplying the integer
                        // matrix rowOps_ with an integer vector.
                        rhs_[r] *= base;
                        rhs_[r] -= (coeff * rhs_[row]);
                        rhs_[r].divByExact(gcdRow);
                    }
                }
            }
        }

        /**
         * Pivots from the current basis to a feasible basis, or else
         * marks the entire system as infeasible if this is not possible.
         * This uses a heuristic pivot rule with good performance but
         * also includes safety checks to break cycling; see below for details.
         *
         * This routine is called after we take a feasible basis and make
         * some modification that might (or might not) cause some basis
         * variables to become negative.
         *
         * The pivot rule that we use is greedy: the variable that exits
         * the basis is the one with largest magnitude negative value.
         * This rule is fast to test and in most cases leads to a small
         * number of pivots, giving good performance overall.  However,
         * this rule can lead to cycling, and so we include cycle-detection
         * code that falls back to Bland's rule to break cycling in the
         * rare cases when it occurs.
         *
         * \pre feasible_ is currently marked as \c true (as a leftover
         * from the feasible basis before our recent modification).
         */
        void makeFeasible() {
            int r, c, outCol, outRow;
            IntType outEntry, tmp;

            // Variables for detecting cycling.
            //
            // The bits in oldBasis are a snapshot of which variables were in
            // the basis at some point in the past, and the bits in currBasis
            // indicate which variables are in the basis right now.
            //
            // We use Brent's method for detecting cycles:
            // We store a snapshot in oldBasis after 2^k pivots, for all k.
            // This means that, regardless of the length of the cycle or
            // the number of pivots that precede the cycle, we will
            // detect oldBasis == currBasis shortly after cycling occurs
            // (in particular, the total number of pivots that we take
            // overall is at most three times the total number of pivots
            // before the first repeated basis).
            unsigned nCols = origTableaux_->columns();
            NBitmask currBasis(nCols);
            for (r = 0; r < rank_; ++r)
                currBasis.set(basis_[r], true);
            NBitmask oldBasis(currBasis);
            unsigned long pow2 = 1;
            unsigned long nPivots = 0;

            while (true) {
                // Locate a variable in the basis with negative value.
                // If there are many, choose the variable with largest 
                // magnitude negative value.
                outCol = -1;
                for (r = 0; r < rank_; ++r)
                    if (rhs_[r] < 0) {
                        if (outCol < 0) {
                            // First candidate we've seen.
                            // Use it until we find something better.
                            outRow = r;
                            outCol = basis_[r];
                            outEntry = entry(r, outCol);
                            continue;
                        }
                        // Compare which variable is most negative.
                        tmp = entry(r, basis_[r]);
                        if (rhs_[r] * outEntry < rhs_[outRow] * tmp) {
                            outRow = r;
                            outCol = basis_[r];
                            outEntry = tmp;
                        }
                    }
                if (outCol < 0) {
                    // All basis variables are non-negative!
                    // This is a feasible basis; we're done.
                    return;
                }

                // Fix this bad variable by pivoting it out.
                // The pivot-in variable must be the largest-index
                // column with negative coefficient in this row.
                for (c = nCols - 1; c >= 0; --c)
                    if (basisRow_[c] < 0 /* active, non-basic variable */ &&
                            entry(outRow, c) < 0)
                        break;
                if (c < 0) {
                    // There is no possible variable to pivot in.
                    // The system is infeasible.
                    feasible_ = false;
                    return;
                }
                pivot(outCol, c);

                // Run our cycle-detection machinery.
                currBasis.set(outCol, false);
                currBasis.set(c, true);

                if (currBasis == oldBasis) {
                    // We've cycled!
                    // Switch to a slower but cycle-free pivot rule.
                    makeFeasibleAntiCycling();
                    return;
                }

                if (++nPivots == pow2) {
                    oldBasis = currBasis;
                    pow2 <<= 1;
                    // On a modern (64-bit) system, pow2 will only overflow
                    // after something like 10^19 pivots, and the human
                    // will have given up in frustration long before this.
                    // Nevertheless, make sure things work even in this case:
                    if (! pow2) {
                        makeFeasibleAntiCycling();
                        return;
                    }
                }
            }
        }

        /**
         * Pivots from the current basis to a feasible basis, or else
         * marks the entire system as infeasible if this is not possible.
         * The pivot rule has poor performance but guarantees to avoid
         * cycling; see below for details.
         *
         * This routine is called after we take a feasible basis and make
         * some modification that might (or might not) cause some basis
         * variables to become negative.
         *
         * This routine uses a variant of Bland's rule (but without an
         * objective function) to avoid cycling.  The rule to decide on
         * each individual pivot is fast to run, but the total number of
         * pivots required to reach feasibility is often very large, leading
         * to a poor performance overall.  It is recommended that you
         * use a different pivoting rule in general, and only call this
         * function to break cycles when they occur.
         *
         * \pre feasible_ is currently marked as \c true (as a leftover
         * from the feasible basis before our recent modification).
         */
        void makeFeasibleAntiCycling() {
            int r, c, outCol;
            while (true) {
                // Locate a variable in the basis with negative value.
                // If there are many, choose the one with largest index.
                outCol = -1;
                for (r = 0; r < rank_; ++r)
                    if (rhs_[r] < 0) {
                        if (basis_[r] > outCol)
                            outCol = basis_[r];
                    }
                if (outCol < 0) {
                    // All basis variables are non-negative!
                    // This is a feasible basis; we're done.
                    return;
                }

                // Fix this bad variable by pivoting it out.
                // The pivot-in variable must be the largest-index
                // column with negative coefficient in this row.
                for (c = origTableaux_->columns() - 1; c >= 0; --c)
                    if (basisRow_[c] < 0 /* active, non-basic variable */ &&
                            entry(basisRow_[outCol], c) < 0)
                        break;
                if (c < 0) {
                    // There is no possible variable to pivot in.
                    // The system is infeasible.
                    feasible_ = false;
                    return;
                }
                pivot(outCol, c);
            }
        }

        /**
         * Runs some tests to ensure that the tableaux is in a
         * consistent state.  This is for use in debugging only, since
         * running these tests might have a severe impact on performance.
         *
         * If any tests fail, this routine reports the error and exits
         * the program immediately.
         */
        void verify() const {
            unsigned r, c;
            for (r = 0; r < rank_; ++r) {
                // Check that rowOps_ is an inverse matrix.
                for (c = 0; c < rank_; ++c)
                    if (r != c && ! entry(r, basis_[c]).isZero()) {
                        std::cerr << "VERIFY: Inverse error" << std::endl;
                        ::exit(1);
                    }

                // Check that each row has gcd = 1.
                IntType g; // Initialised to zero.
                for (c = 0; c < rowOps_.columns(); ++c)
                    g = g.gcd(rowOps_.entry(r, c));
                if (g != 1) {
                    std::cerr << "VERIFY: GCD error" << std::endl;
                    ::exit(1);
                }
            }
        }
};

/**
 * A base class for additional linear constraints that we can add to the
 * tableaux of normal surface matching equations.  This is used with
 * TreeEnumeration, TreeSingleSoln and related algorithms for enumerating and
 * locating normal surfaces in a 3-manifold triangulation.  See the
 * LPInitialTableaux class notes for details on how these constraints
 * interact with the tableaux of matching equations.
 *
 * The linear constraints may be equalities or inequalities, and there
 * may be more than one such constraint.  If all constraints are
 * homogeneous equalities, the class should derive from LPConstraintSubspace
 * instead (not this base class).
 *
 * This base class provides no functionality.  For documentation's sake
 * only, the notes here describe the functionality that any subclass
 * \e must implement.  We note again that LPConstraintBase does not
 * provide any implementations at all, and subclasses are completely
 * responsible for their own implementations.
 */
class LPConstraintBase {
#ifdef __DOXYGEN
    public:
        enum {
            /**
             * The number of additional linear constraints that we impose.
             * Each constraint will generate one new variable (column)
             * and one new equation (row) in the tableaux.
             */
            nConstraints
        };

        /**
         * Stores the extra coefficients in a single column for the
         * \a nConstraints additional rows that we add to the tableaux
         * to describe the \a nConstraints additional linear equations
         * or inequalities.
         *
         * Subclasses may store these coefficients however they like
         * (in particular, they may optimise for sparse coefficients,
         * binary coefficients, and so on).  They will only ever be
         * accessed through the member functions of this Coefficients class.
         */
        struct Coefficients {
            /**
             * Creates an uninitialised set of coefficients for a single
             * column.  These cofficients must be initialised through a
             * call to addRows() before they can be used.
             */
            Coefficients();

            /**
             * Explicitly fills the final row(s) of the given tableaux matrix 
             * with the coefficients stored in this Coefficients structure.
             * In essence, this routine simply copies this sparse and/or
             * specialised representation of the final row(s) into a
             * more standard dense matrix representation.
             *
             * This routine should only affect the final \a nConstraints
             * entries in the given column of the matrix.  It may assume
             * that these final row(s) have already been initialised to zero.
             *
             * \pre The given matrix has at least \a nConstraints rows
             * and at least \a col + 1 columns.
             * \pre The final \a nConstraints entries in column \a col
             * of the given matrix have already been set to zero.
             *
             * @param m the matrix in which to place these column
             * coefficients.
             * @param col the column of the given matrix in which to
             * place these coefficients.
             */
            void fillFinalRows(LPMatrix& m, unsigned col) const;

            /**
             * Computes the inner product of (i) the final \a nConstraints
             * entries in the given row of the given matrix with (ii) the
             * \a nConstraints column coefficients stored in this data
             * structure.
             *
             * \pre The given matrix has at least \a nConstraints columns
             * and at least \a mRow + 1 rows.
             *
             * @param m the matrix whose row we will use in the inner product.
             * @param mRow the row of the matrix \a m to use in the inner
             * product.
             * @return the resulting portion of the inner product.
             */
            IntType innerProduct(const LPMatrix& m, unsigned mRow) const;

            /**
             * A variant of innerProduct() that takes into account any
             * adjustments to these linear constraint(s) that are required when
             * this is a quadrilateral column being used to represent an
             * octagon type.
             *
             * The LPData class offers support for octagonal almost normal
             * surfaces, in which exactly one tetrahedron is allowed to have
             * exactly one octagon type.  We represent such an octagon as a
             * \e pair of incompatible quadrilaterals within the same
             * tetrahedron.  See the LPData class notes for details on how
             * this works.
             *
             * In some settings, our extra linear constraints must behave
             * differently in the presence of octagons (i.e., the coefficient
             * of the octagon type is not just the sum of coefficients of the
             * two constituent quadrilateral types).  This routine effectively
             * allows us to adjust the tableaux accordingly.
             *
             * Specifically: this routine computes the inner product of (i) the
             * final \a nConstraints entries in the given row of the given
             * matrix with (ii) the \a nConstraints column coefficients
             * stored in this data structure.  We assume that this column
             * in the underlying tableaux describes one of the two
             * quadrilateral coordinates in some tetrahedron that together
             * form an octagon type, and if necessary we implicitly adjust
             * the coefficients stored in this data structure accordingly.
             *
             * \pre The given matrix has at least \a nConstraints columns
             * and at least \a mRow + 1 rows.
             *
             * \pre This column of the underlying tableaux describes one
             * of the two quadrilateral coordinates that are being
             * combined to form an octagon type within some tetrahedron.
             *
             * @param m the matrix whose row we will use in the inner product.
             * @param mRow the row of the matrix \a m to use in the inner
             * product.
             * @return the resulting portion of the inner product.
             */
            IntType innerProductOct(const LPMatrix& m, unsigned mRow)
                const;
        };

        /**
         * Explicitly constructs equations for the linear function(s)
         * constrained by this class.  Specifically, this routine takes an
         * array of Coefficients objects (one for each column of the initial
         * tableaux) and fills in the necessary coefficient data.
         *
         * The precise form of the linear function(s) will typically
         * depend upon the underlying triangulation.  For this reason,
         * the triangulation is explicitly passed, along with the
         * permutation that indicates which columns of the initial tableaux
         * correspond to which normal coordinates.
         *
         * More precisely: recall that, for each linear function, the initial
         * tableaux acquires one new variable \a x_i that evaluates this linear
         * function f(x).  This routine must create the corresponding row that
         * sets <tt>f(x) - x_i = 0</tt>.  Thus it must construct the
         * coefficients of f(x) in the columns corresponding to normal
         * coordinates, and it must also set a coefficient of -1 in the
         * column for the corresponding new variable.
         *
         * This function is templated, since in reality we typically
         * pass an array of full tableaux columns (of type
         * LPInitialTableaux::Col), which are larger subclasses of the
         * Coefficients class.  This templating is necessary because the
         * compiler must know how large each column object is in
         * order to correct access each element of the given array.
         *
         * As described in the LPInitialTableaux class notes, it might
         * not be possible to construct the linear functions (since the
         * triangulation might not satisfy the necessary requirements).
         * In this case, this routine should ensure that the linear
         * functions are in fact the zero functions, and should return
         * \c false (but it must still set -1 coefficients for the new
         * variables as described above).  Otherwise (if the linear function
         * were successfully constructed) this routine should return \c true.
         *
         * \pre The template class \a ColClass is a subclass of
         * Coefficients.
         *
         * \pre For all coefficients in the array \a col, the
         * Coefficients substructures have all been initialised with the
         * default constructor and not modified since.
         *
         * @param col the array of columns as stored in the initial
         * tableaux (i.e., the data member LPInitialTableaux::col_).
         * @param columnPerm the corresponding permutation of columns
         * that describes how columns of the tableaux correspond to
         * normal coordinates in the underlying triangulation (i.e., the
         * data member LPInitialTableaux::columnPerm_).
         * @param tri the underlying triangulation.
         * @return \c true if the linear functions were successfully
         * constructed, or \c false if not (in which case they will be
         * replaced with the zero functions instead).
         */
        template <typename ColClass>
        static bool addRows(ColClass* col, const int* columnPerm,
            NTriangulation* tri);

        /**
         * Explicitly constraints each of these linear functions to an
         * equality or inequality in the underlying tableaux.  This will
         * typically consist of a series of calls to LPData::constrainZero()
         * and/or LPData::constrainPositive().
         *
         * The variables for these extra linear functions are stored in
         * columns <tt>numCols - nConstraints</tt>, ..., <tt>numCols - 1</tt>
         * of the given tableaux, and so your calls to LPData::constrainZero()
         * and/or LPData::constrainPositive() should operate on these
         * (and only these) columns.
         * 
         * \pre These column coefficients belong to the initial starting
         * tableaux (LPInitialTableaux) from which the given tableaux is
         * derived.
         *
         * @param lp the tableaux in which to constrain these linear
         * functions.
         * @param numCols the number of columns in the given tableaux.
         */
        static void constrain(LPData<LPConstraintNone>& lp, unsigned numCols);

        /**
         * Ensures that the given normal surface satisfies the extra
         * constraints described by this class.
         *
         * Ideally this test is not based on explicitly recomputing the
         * linear function(s), but instead runs independent tests.
         * For instance, if this class is used to constraint Euler
         * characteristic, then ideally this routine would call
         * s->getEulerCharacteristic() and test the return value of that
         * routine instead.
         *
         * @param s the surface to test.
         * @return \c true if the given surface satisfies these linear
         * constraints, or \c false if it does not.
         */
        static bool verify(const NNormalSurface* s);
#endif
};

/**
 * A subclass of LPConstraintBase used for constraints defined entirely
 * by homogeneous linear equations.
 *
 * Any set of constraints defined entirely by homogeneous linear
 * equations should derive from LPConstraintSubspace, not LPConstraintBase.
 * In other words, any set of constraints derived from LPConstraintSubspace
 * should simply restrict our attention to a vector subspace of the
 * normal surface coordinate system.
 *
 * This class does not provide any additional functionality.  It is
 * merely a convenience to help describe and enforce preconditions.
 */
class LPConstraintSubspace : public LPConstraintBase {
};

/**
 * A do-nothing class that imposes no additional linear constraints on
 * the tableaux of normal surface matching equations.
 */
class LPConstraintNone : public LPConstraintSubspace {
    public:
        enum { nConstraints = 0 };

        struct Coefficients {
            inline Coefficients() {}

            inline void fillFinalRows(LPMatrix& m, unsigned col) const {}

            inline IntType innerProduct(const LPMatrix&, unsigned) const {
                return IntType(); // Returns zero.
            }

            inline IntType innerProductOct(const LPMatrix&, unsigned)
                    const {
                return IntType(); // Returns zero.
            }
        };

        template <typename ColClass>
        inline static bool addRows(ColClass*, const int*, NTriangulation*) {
            return true;
        }

        inline static void constrain(LPData<LPConstraintNone>&, unsigned) {
        }

        inline static bool verify(const NNormalSurface*) {
            return true;
        }
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that Euler characteristic is strictly positive.
 *
 * There are many ways of writing Euler characteritic as a linear
 * function.  The function constructed here has integer coefficients,
 * but otherwise has no special properties of note.
 *
 * This constraint can work with either normal or almost normal
 * coordinates.  In the case of almost normal coordinates, the function
 * is modified to measure Euler characteristic minus the number of
 * octagons (a technique of Casson, also employed by Jaco and Rubinstein, that
 * is used to ensure we do not have more than two octagons when searching for
 * a normal or almost normal sphere in the 3-sphere recognition algorithm).
 *
 * \pre We are working in standard normal or almost normal coordinates
 * (not quadrilateral or quadrilateral-octagon coordinates).  In
 * particular, the coordinate system passed to the corresponding
 * LPInitialTableaux class constructor must be NNormalSurfaceList::STANDARD.
 */
class LPConstraintEuler : public LPConstraintBase {
    public:
        enum { nConstraints = 1 };

        struct Coefficients {
            int euler;
                /**< The coefficient of the Euler characteristic
                     function for the corresponding column of the matching
                     equation matrix. */

            inline Coefficients() : euler(0) {}

            inline void fillFinalRows(LPMatrix& m, unsigned col) const {
                m.entry(m.rows() - 1, col) = euler;
            }

            inline IntType innerProduct(const LPMatrix& m,
                    unsigned mRow) const {
                IntType ans(m.entry(mRow, m.rows() - 1));
                ans *= euler;
                return ans;
            }

            inline IntType innerProductOct(const LPMatrix& m,
                    unsigned mRow) const {
                // This is called for *two* quad columns (the two quads
                // that combine to give a single octagon).
                //
                // The adjustment in this case is to subtract two from
                // the overall Euler characteristic coefficient for this
                // octagon type (-1 because an octagon has lower Euler
                // characteristic than two quads, and -1 again because
                // we are measuring Euler - #octagons.
                //
                // Happily we can do this by subtracting one from the
                // coefficient in each of the two columns, as
                // implemented below.
                IntType ans(m.entry(mRow, m.rows() - 1));
                ans *= (euler - 1);
                return ans;
            }
        };

        template <typename ColClass>
        static bool addRows(ColClass* col, const int* columnPerm,
                NTriangulation* tri) {
            int* obj = new int[7 * tri->getNumberOfTetrahedra()];
            unsigned tet, i;
            NPerm4 p;
            for (i = 0; i < 7 * tri->getNumberOfTetrahedra(); ++i)
                obj[i] = 1;
            for (i = 0; i < tri->getNumberOfFaces(); ++i) {
                tet = tri->tetrahedronIndex(
                    tri->getFace(i)->getEmbedding(0).getTetrahedron());
                p = tri->getFace(i)->getEmbedding(0).getVertices();
                --obj[7 * tet + p[0]];
                --obj[7 * tet + p[1]];
                --obj[7 * tet + p[2]];
                --obj[7 * tet + 4];
                --obj[7 * tet + 5];
                --obj[7 * tet + 6];
            }
            for (i = 0; i < tri->getNumberOfEdges(); ++i) {
                tet = tri->tetrahedronIndex(
                    tri->getEdge(i)->getEmbedding(0).getTetrahedron());
                p = tri->getEdge(i)->getEmbedding(0).getVertices();
                ++obj[7 * tet + p[0]];
                ++obj[7 * tet + p[1]];
                ++obj[7 * tet + 4 + vertexSplitMeeting[p[0]][p[1]][0]];
                ++obj[7 * tet + 4 + vertexSplitMeeting[p[0]][p[1]][1]];
            }

            for (i = 0; i < 7 * tri->getNumberOfTetrahedra(); ++i)
                col[i].euler = obj[columnPerm[i]];

            col[7 * tri->getNumberOfTetrahedra()].euler = -1;

            delete[] obj;
            return true;
        }

        inline static void constrain(LPData<LPConstraintEuler>& lp,
                unsigned numCols) {
            lp.constrainPositive(numCols - 1);
        }

        inline static bool verify(const NNormalSurface* s) {
            return (s->getEulerCharacteristic() > 0);
        }
};

/**
 * A class that constraints the tableaux of normal surface matching equations
 * to ensure that normal surfaces in an ideal triangulation are compact
 * (thereby avoiding spun normal surfaces with infinitely many triangles).
 *
 * At present this class can only work with oriented triangulations that have
 * precisely one vertex, which is ideal with torus link.  These
 * constraints are explicitly checked by addRows(), which returns \c false
 * if they are not satisfied.  Moreover, this constraint calls on
 * SnapPea for some calculations: in the unexpected situation where
 * SnapPea retriangulates, the linear function cannot be constructed and
 * addRows() will again return \c false.  You should always test
 * LPInitialTableaux::constraintsBroken() to verify that the linear
 * functions have been constructed correctly.
 *
 * Also, at present this class can only work with quadrilateral normal
 * coordinates (and cannot handle almost normal coordinates at all).
 * This is \e not explicitly checked; instead it appears as a
 * precondition (see below).
 *
 * \pre We are working in quadrilateral normal coordinates.  In particular,
 * the coordinate system passed to the corresponding LPInitialTableaux class
 * must be NNormalSurfaceList::QUAD, and constrainOct() must never be
 * called on any of the corresponding LPData tableaux.
 */
class LPConstraintNonSpun : public LPConstraintSubspace {
    public:
        enum { nConstraints = 2 };

        struct Coefficients {
            int meridian;
            int longitude;

            inline Coefficients() : meridian(0), longitude(0) {}

            inline void fillFinalRows(LPMatrix& m, unsigned col) const {
                m.entry(m.rows() - 2, col) = meridian;
                m.entry(m.rows() - 1, col) = longitude;
            }

            inline IntType innerProduct(const LPMatrix& m,
                    unsigned mRow) const {
                IntType ans1(m.entry(mRow, m.rows() - 2));
                ans1 *= meridian;
                IntType ans2(m.entry(mRow, m.rows() - 1));
                ans2 *= longitude;
                ans1 += ans2;
                return ans1;
            }

            inline IntType innerProductOct(const LPMatrix& m,
                    unsigned mRow) const {
                // This should never be called, since we never use this
                // constraint with almost normal surfaces.
                // For compilation's sake though, just return the usual
                // inner product.
                return innerProduct(m, mRow);
            }
        };

        template <typename ColClass>
        static bool addRows(ColClass* col, const int* columnPerm,
                NTriangulation* tri) {
            // Regardless of whether the constraints are broken,
            // we need to ensure that the matrix has full rank.
            // Therefore add the coefficients for the two new variables now.
            col[3 * tri->getNumberOfTetrahedra()].meridian = -1;
            col[3 * tri->getNumberOfTetrahedra() + 1].longitude = -1;

            // For the time being we insist on one vertex, which must be
            // ideal with torus link.
            if (tri->getNumberOfVertices() != 1 ||
                    (! tri->getVertex(0)->isIdeal()) ||
                    (! tri->getVertex(0)->isLinkOrientable()) ||
                    tri->getVertex(0)->getLinkEulerCharacteristic() != 0)
                return false;

            // Compute the two slope equations for the torus cusp, if we can.
            NSnapPeaTriangulation snapPea(*tri, false);
            NMatrixInt* coeffs = snapPea.slopeEquations();
            if (! coeffs)
                return false;

            // Check that SnapPea hasn't changed the triangulation on us.
            if (! snapPea.verifyTriangulation(*tri)) {
                delete coeffs;
                return false;
            }

            // All good!  Add the two slope equations as extra rows to
            // our constraint matrix.
            //
            // The coefficients here are differences of terms from
            // SnapPy's get_cusp_equation(), which works in native
            // integers; therefore we will happily convert them back to
            // native integers now.
            for (int i = 0; i < 3 * tri->getNumberOfTetrahedra(); ++i) {
                col[i].meridian = coeffs->entry(0, columnPerm[i]).longValue();
                col[i].longitude = coeffs->entry(1, columnPerm[i]).longValue();
            }

            delete coeffs;
            return true;
        }

        inline static void constrain(LPData<LPConstraintNonSpun>& lp,
                unsigned numCols) {
            lp.constrainZero(numCols - 2);
            lp.constrainZero(numCols - 1);
        }

        inline static bool verify(const NNormalSurface* s) {
            return s->isCompact();
        }
};

/**
 * A base class for additional banning and marking constraints that we
 * can place on tree traversal algorithms.  This is used with
 * TreeEnumeration, TreeSingleSoln and related algorithms for
 * enumerating and locating normal surfaces in a 3-manifold triangulation.
 *
 * This class adds constraints of two types:
 *
 * - \e Banning constraints, which ensure that certain normal coordinates
 *   are set to zero;
 *
 * - \e Marking constraints, which are more flexible and can be used in
 *   different ways by different algorithms.
 *
 * All of these constraints operate only on normal coordinates in the
 * underlying tableaux (and in particular not the additional variables
 * introduced by additional linear constraints, as described by
 * LPConstraintBase and its subclasses).
 *
 * Currently marking is used in the following ways:
 *
 * - The TreeEnumeration algorithm does not use marking at all.
 *
 * - In the TreeSingleSoln algorithm, marking affects what is considered
 *   a non-trivial surface.  Normally, a non-trivial surface is defined
 *   to be one in which some triangle coordinate is zero.  With marking,
 *   a non-trivial surface is redefined to be one in which some \e unmarked
 *   triangle coordinate is zero.  In other words, marked triangle types
 *   are effectively ignored when determining whether a surface is non-trivial
 *   or not.
 *
 * At present, marking is not used at all for quadrilateral coordinates.
 * Howver, marking is a very new feature, and this concept may be expanded
 * in future versions of Regina.
 *
 * This class does not record disc types in the order of their normal
 * coordinates; instead it records them in the order of their columns in
 * a tableaux for linear programming (as used in LPInitialTableaux).
 * This means that there is a little more work required in setting up
 * the initial lists of banned and marked columns, but then these lists are
 * easy to use on the fly during tree traversal algorithms.
 *
 * This base class provides limited functionality (as documented below).
 * Subclasses \e must implement a constructor (which, like this base
 * class, takes a triangulation and a coordinate system), and must also
 * implement the routine init() which determines which normal coordinates
 * are banned and/or marked.
 */
class BanConstraintBase {
    protected:
        NTriangulation* tri_;
            /**< The triangulation with which we are working. */
        int coords_;
            /**< The normal or almost normal coordinate system in which
                 we are working.  This must be one of NNormalSurfaceList::QUAD,
                 NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT,
                 or NNormalSurfaceList::AN_STANDARD. */
        bool* banned_;
            /**< Indicates which columns of a tableaux correspond to banned
                 disc types.
                 The size of this array is the number of normal coordinates
                 (so we explicitly exclude extra columns that arise from the
                 template parameter LPConstraint. */
        bool* marked_;
            /**< Indicates which columns of a tableaux correspond to marked
                 disc types.
                 The size of this array is the number of normal coordinates
                 (so we explicitly exclude extra columns that arise from the
                 template parameter LPConstraint. */

    protected:
        /**
         * Constructs and initialises the \a banned_ and \a marked_ arrays
         * to be entirely \c false.  The only purpose of passing the
         * triangulation and coordinate system is to determine how many
         * normal coordinates we are dealing with.
         *
         * \warning Before you use this object, the routine init() must be
         * called to fill in the \a banned_ and \a marked_ arrays with the
         * correct data.  Otherwise you will have no banned or marked disc
         * types at all.
         *
         * @param tri the triangulation with which we are working.
         * @param coords the normal or almost normal coordinate system in
         * which we are working.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline BanConstraintBase(NTriangulation* tri, int coords) :
                tri_(tri), coords_(coords) {
            unsigned nCols = (coords == NNormalSurfaceList::QUAD ||
                coords == NNormalSurfaceList::AN_QUAD_OCT ?
                3 * tri->getNumberOfTetrahedra() :
                7 * tri->getNumberOfTetrahedra());
            banned_ = new bool[nCols];
            marked_ = new bool[nCols];
            std::fill(banned_, banned_ + nCols, false);
            std::fill(marked_, marked_ + nCols, false);
        }

        /**
         * Destroys this object and all associated data.
         */
        inline ~BanConstraintBase() {
            delete[] banned_;
            delete[] marked_;
        }

        /**
         * Enforces all bans described by this class in the given
         * tableaux.  Specifically, for each banned disc type, this
         * routine calls LPData::constrainZero() on the corresponding
         * normal coordinate column.
         *
         * @param lp the tableaux in which to enforce the bans.
         */
        template <typename LPConstraint>
        void enforceBans(LPData<LPConstraint>& lp) const {
            for (unsigned i = 0; i < lp.coordinateColumns(); ++i)
                if (banned_[i])
                    lp.constrainZero(i);
        }

#ifdef __DOXYGEN
        /**
         * Idetifies which disc types to ban and mark, and records the
         * corresponding tableaux columns in the \a banned_ and \a marked_
         * arrays respectively.
         *
         * @param columnPerm the permutation of columns that describes how
         * columns of the tableaux correspond to normal coordinates in
         * the underlying triangulation.  Specifically, this permutation must
         * be the same permutation returned by LPInitialTableaux::columnPerm().
         */
        void init(const int* columnPerm);
#endif
};

/**
 * A do-nothing class that bans no disc types and marks no disc types.
 */
class BanNone : public BanConstraintBase {
    protected:
        /**
         * Constructs and initialises the \a banned_ and \a marked_ arrays
         * to be entirely \c false, as described in the BanConstraintBase
         * superclass constructor.
         *
         * Although one should normally call the routine init() before
         * using this object, for BanNone this is not strictly necessary
         * since there are no disc types to ban or mark.
         *
         * @param tri the triangulation with which we are working.
         * @param coords the normal or almost normal coordinate system in
         * which we are working.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline BanNone(NTriangulation* tri, int coords) :
                BanConstraintBase(tri, coords) {
        }

        inline void init(const int*) {
        }
};

/**
 * A class that bans normal disc types that meet the boundary of the
 * underlying triangulation.  No disc types are marked at all.
 *
 * \warning This class only works as expected in \e standard normal or
 * almost normal coordinates.  In quadrilateral or quadrilateral-octagon
 * coordinates it will only ban quadrilaterals or octagons that touch
 * the boundary, but it will still allow \e triangles that meet the boundary
 * (since triangle types are not counted in these coordinate systems).
 */
class BanBoundary : public BanConstraintBase {
    protected:
        /**
         * Constructs and initialises the \a banned_ and \a marked_ arrays
         * to be entirely \c false, as described in the BanConstraintBase
         * superclass constructor.
         *
         * \warning Before you use this object, the routine init() must be
         * called to fill in the \a banned_ and \a marked_ arrays with the
         * correct data.  Otherwise you will have no banned or marked disc
         * types at all.
         *
         * @param tri the triangulation with which we are working.
         * @param coords the normal or almost normal coordinate system in
         * which we are working.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline BanBoundary(NTriangulation* tri, int coords) :
                BanConstraintBase(tri, coords) {
        }

        void init(const int* columnPerm) {
            unsigned n = tri_->getNumberOfTetrahedra();
            unsigned tet, type, i, k;

            bool quadOnly = (coords_ == NNormalSurfaceList::QUAD ||
                coords_ == NNormalSurfaceList::AN_QUAD_OCT);

            // The implementation here is a little inefficient (we repeat tests
            // three or four times over), but this routine is only called at
            // the beginning of the enumeration process so no need to worry.

            // Ban quadrilaterals in tetrahedra that meet the boundary
            // (every such quadrilateral meets a boundary face).
            for (i = 0; i < 3 * n; ++i) {
                if (quadOnly)
                    tet = columnPerm[i] / 3;
                else
                    tet = columnPerm[i] / 7;

                for (k = 0; k < 4; ++k)
                    if (! tri_->getTetrahedron(tet)->adjacentTetrahedron(k)) {
                        banned_[i] = true;
                        break;
                    }
            }

            // Ban triangles in tetrahedra that meet the boundary (but
            // only those triangles that meet the boundary faces).
            if (! quadOnly)
                for (i = 3 * n; i < 7 * n; ++i) {
                    tet = columnPerm[i] / 7;
                    type = columnPerm[i] % 7;

                    for (k = 0; k < 4; ++k)
                        if (k != type &&
                                ! tri_->getTetrahedron(tet)->
                                adjacentTetrahedron(k)) {
                            banned_[i] = true;
                            break;
                        }
                }
        }
};

/**
 * A class that bans and marks disc types associated with torus boundary
 * components.  Here we refer exclusively to real torus boundary
 * components (not ideal vertices with torus cusps).  Specifically:
 *
 * - this class bans any normal triangle or quadrilateral that meets a
 *   torus boundary;
 *
 * - this class marks any normal triangle in the link of a vertex on a
 *   torus boundary.
 *
 * \warning As with BanBoundary, this class only works as expected in
 * \e standard normal or almost normal coordinates.  In quadrilateral or
 * quadrilateral-octagon coordinates it will only ban quadrilaterals or
 * octagons that touch torus boundaries, but it will still allow \e triangles
 * that meet torus boundaries (since triangle types are not counted in these
 * coordinate systems).
 */
class BanTorusBoundary : public BanConstraintBase {
    protected:
        /**
         * Constructs and initialises the \a banned_ and \a marked_ arrays
         * to be entirely \c false, as described in the BanConstraintBase
         * superclass constructor.
         *
         * \warning Before you use this object, the routine init() must be
         * called to fill in the \a banned_ and \a marked_ arrays with the
         * correct data.  Otherwise you will have no banned or marked disc
         * types at all.
         *
         * @param tri the triangulation with which we are working.
         * @param coords the normal or almost normal coordinate system in
         * which we are working.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline BanTorusBoundary(NTriangulation* tri, int coords) :
                BanConstraintBase(tri, coords) {
        }

        void init(const int* columnPerm) {
            unsigned n = tri_->getNumberOfTetrahedra();
            unsigned tet, type, i, k;

            // Which boundary faces are we banning?
            unsigned nFaces = tri_->getNumberOfFaces();
            bool* banFace = new bool[nFaces];
            std::fill(banFace, banFace + nFaces, false);

            // Which vertex links are we marking triangles around?
            unsigned nVertices = tri_->getNumberOfVertices();
            bool* markVtx = new bool[nVertices];
            std::fill(markVtx, markVtx + nVertices, false);

            NBoundaryComponent* bc;
            for (i = 0; i < tri_->getNumberOfBoundaryComponents(); ++i) {
                bc = tri_->getBoundaryComponent(i);
                if ((! bc->isIdeal()) && bc->isOrientable() &&
                        bc->getEulerCharacteristic() == 0) {
                    // We've found a real torus boundary.
                    for (k = 0; k < bc->getNumberOfFaces(); ++k)
                        banFace[bc->getFace(k)->markedIndex()] = true;
                    for (k = 0; k < bc->getNumberOfVertices(); ++k)
                        markVtx[bc->getVertex(k)->markedIndex()] = true;
                }
            }

            bool quadOnly = (coords_ == NNormalSurfaceList::QUAD ||
                coords_ == NNormalSurfaceList::AN_QUAD_OCT);

            // The implementation here is a little inefficient (we repeat tests
            // three or four times over), but this routine is only called at
            // the beginning of the enumeration process so no need to worry.

            // Ban quadrilaterals that touch torus boundaries.
            for (i = 0; i < 3 * n; ++i) {
                if (quadOnly)
                    tet = columnPerm[i] / 3;
                else
                    tet = columnPerm[i] / 7;

                for (k = 0; k < 4; ++k)
                    if (banFace[tri_->getTetrahedron(tet)->getFace(k)->
                            markedIndex()]) {
                        banned_[i] = true;
                        break;
                    }
            }

            // Ban triangles that touch torus boundaries, and mark all
            // triangles that surround vertices on torus boundaries
            // (even if the triangles do not actually touch the boundary).
            if (! quadOnly)
                for (i = 3 * n; i < 7 * n; ++i) {
                    tet = columnPerm[i] / 7;
                    type = columnPerm[i] % 7;

                    if (markVtx[tri_->getTetrahedron(tet)->getVertex(type)->
                            markedIndex()])
                        marked_[i] = true;

                    for (k = 0; k < 4; ++k)
                        if (k != type &&
                                banFace[tri_->getTetrahedron(tet)->getFace(k)->
                                markedIndex()]) {
                            banned_[i] = true;
                            break;
                        }
                }

            delete[] banFace;
            delete[] markVtx;
        }
};

/**
 * A base class for searches that employ the tree traversal algorithm for
 * enumerating and locating vertex normal surfaces.  Users should not use this
 * base class directly; instead use one of the subclasses TreeEnumeration (for
 * enumerating all vertex normal surfaces) or TreeSingleSoln (for
 * locating a single non-trivial solution under additional constraints,
 * such as positive Euler characteristic).
 *
 * The full algorithms are described respectively in "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 *
 * This base class provides the infrastructure for the search tree, and the
 * subclasses handle the mechanics of the moving through the tree according
 * to the backtracking search.  The domination test is handled separately by
 * the class TypeTrie, and the feasibility test is handled separately by the
 * class LPData.
 *
 * This class holds the particular state of the tree traversal
 * at any point in time, as described by the current \e level (indicating
 * our current depth in the search tree) and <i>type vector</i>
 * (indicating which branches of the search tree we have followed).
 * For details on these concepts, see the Algorithmica paper cited above.
 * The key details are summarised below; throughout this discussion
 * \a n represents the number of tetrahedra in the underlying triangulation.
 *
 * - In quadrilateral coordinates, the type vector is a sequence of \a n
 *   integers, each equal to 0, 1, 2 or 3, where the <i>i</i>th integer
 *   describes the choice of quadrilateral location in the <i>i</i>th
 *   tetrahedron.
 *
 * - In standard coordinates, the type vector begins with the \a n
 *   quadrilateral choices outlined above.  This is then followed by an
 *   additional 4<i>n</i> integers, each equal to 0 or 1; these
 *   correspond to the 4<i>n</i> triangle coordinates, and indicate
 *   whether each coordinate is zero or non-zero.
 *
 * In the original Algorithmica paper, we choose types in the order
 * type_[0], type_[1] and so on, working from the root of the tree down
 * to the leaves.  Here we support a more flexible system: there is an
 * internal permutation \a typeOrder_, and we choose types in the order
 * type_[typeOrder_[0]], type_[typeOrder_[1]] and so on.  This
 * permutation may mix quadrilateral and triangle processing, and may
 * even change as the algorithm runs.
 *
 * This class can also support octagon types in almost normal surfaces.
 * However, we still do our linear programming in standard or quadrilateral
 * coordinates, where we represent an octagon using two conflicting 
 * quadrilaterals in the same tetrahedron (which meet the tetrahedron boundary
 * in the same set of arcs as a single octagon would).  As with the almost
 * normal coordinate systems in NNormalSurfaceList, we allow multiple octagons
 * of the same type, but only one octagon type in the entire tetrahedron.
 * In the type vector, octagons are indicated by setting a quadrilateral
 * type to 4, 5 or 6.
 *
 * There is optional support for adding extra linear constraints
 * (such as a constraint on Euler characteristic), supplied by the
 * template parameter \a LPConstraint.  If there are no additional
 * constraints, simply use the template parameter LPConstraintNone.
 *
 * Also, there is optional support for banning normal disc types (so the
 * corresponding coordinates must be set to zero), and/or marking normal
 * disc types (which currently affects what is meant by a "non-trivial"
 * surface for the TreeSingleSoln algorithm, though this concept may be
 * expanded in future versions of Regina).  These options are supplied
 * by the template parameter \a BanConstraint.  If there are no disc
 * types to ban or mark, simply use the template parameter \a BanNone.
 *
 * In some cases, it is impossible to add the extra linear constraints
 * that we would like (for instance, if they require additional
 * preconditions on the underlying triangulation).  If this is a possibility
 * in your setting, you should call constraintsBroken() to test for this
 * once the TreeTraversal object has been constructed.
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintBase and BanConstraintBase respectively.  See the
 * LPConstraintBase and BanConstraintBase class notes for further details.
 */
template <typename LPConstraint, typename BanConstraint>
class TreeTraversal : public BanConstraint {
    protected:
        // Global information about the search:
        const LPInitialTableaux<LPConstraint> origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm begins. */
        const int coords_;
            /**< The coordinate system in which we are enumerating or
                 searching for normal or almost normal surfaces.
                 This must be one of NNormalSurfaceList::QUAD or
                 NNormalSurfaceList::STANDARD if we are only supporting
                 normal surfaces, or one of NNormalSurfaceList::AN_QUAD_OCT
                 or NNormalSurfaceList::AN_STANDARD if we are allowing
                 octagons in almost normal surfaces. */
        const int nTets_;
            /**< The number of tetrahedra in the underlying triangulation. */
        const int nTypes_;
            /**< The total length of a type vector. */
        const int nTableaux_;
            /**< The maximum number of tableaux that we need to keep in memory
                 at any given time during the backtracking search. */

        // Details of the current state of the backtracking search:
        char* type_;
            /**< The current working type vector.  As the search runs,
                 we modify this type vector in-place.  Any types beyond
                 the current level in the search tree will always be set
                 to zero. */
        int* typeOrder_;
            /**< A permutation of 0,...,\a nTypes_-1 that indicates in
                 which order we select types: the first type we select
                 (at the root of the tree) is type_[typeOrder_[0]], and the
                 last type we select (at the leaves of the tree) is
                 type_[typeOrder_[nTypes_-1]].  This permutation is
                 allowed to change as the algorithm runs (though of
                 course you can only change sections of the permutation
                 that correspond to types not yet selected). */
        int level_;
            /**< The current level in the search tree.
                 As the search runs, this holds the index into
                 typeOrder_ corresponding to the last type that we chose. */
        int octLevel_;
            /**< The level at which we are enforcing an octagon type
                 (with a strictly positive number of octagons).  If we
                 are working with normal surfaces only (and so we do not
                 allow any octagons at all), then \a octLevel_ = \a nTypes_.
                 If we are allowing almost normal surfaces but we have not yet
                 chosen an octagon type, then \a octLevel_ = -1. */
        LPData<LPConstraint>* lp_;
            /**< Stores tableaux for linear programming at various nodes
                 in the search tree.  We only store a limited number of
                 tableaux at any given time, and as the search
                 progresses we overwrite old tableaux with new tableaux.

                 More precisely, we store a linear number of tableaux,
                 essentially corresponding to the current node in the
                 search tree and all of its ancestores, all the way up
                 to the root node.  In addition to these tableaux, we
                 also store other immediate children of these ancestores
                 that we have pre-prepared for future processing.  See the
                 documentation within TreeEnumeration::next() for details of
                 when and how these tableaux are constructed. */
        LPData<LPConstraint>** lpSlot_;
            /**< Recall from above that the array \a lp_ stores tableaux
                 for the current node in the search tree and all of its
                 ancestors.  This means we have one tableaux for the
                 root node, as well as additional tableaux at each level
                 0,1,...,\a level_.

                 The array lpSlot_ indicates which element of the array \a lp_
                 holds each of these tableaux.  Specifically: lpSlot_[0]
                 points to the tableaux for the root node, and for each level
                 \a i in the range 0,...,\a level_, the corresponding
                 tableaux is *lpSlot_[i+1].  Again, see the documentation
                 within TreeEnumeration::next() for details of when and how
                 these tableaux are constructed and later overwritten. */
        LPData<LPConstraint>** nextSlot_;
            /**< Points to the next available tableaux in lp_ that is free to
                 use at each level of the search tree.  Specifically:
                 nextSlot_[0] points to the next free tableaux at the root
                 node, and for each level \a i in the range 0,...,\a level_,
                 the corresponding next free tableaux is *nextSlot_[i+1].

                 The precise layout of the nextSlot_ array depends on the
                 order in which we process quadrilateral and triangle types. */
        unsigned long nVisited_;
            /**< Counts the total number of nodes in the search tree that we
                 have visited thus far.  This may grow much faster than the
                 number of solutions, since it also counts traversals
                 through "dead ends" in the search tree. */
        LPData<LPConstraint> tmpLP_[4];
            /**< Temporary tableaux used by the function feasibleBranches()
                 to determine which quadrilateral types have good potential
                 for pruning the search tree.

                 Other routines are welcome to use these temporary tableaux
                 also (as "scratch space"); however, be aware that any
                 call to feasibleBranches() will overwrite them. */

    private:
        bool cancelled_;
            /**< Has the search been cancelled by another thread?
                 See the cancel() and cancelled() routines for details. */
        regina::NMutex mCancel_;
            /**< A mutex used to serialise cancellation tests and requests. */

    public:
        /**
         * Indicates whether or not the extra constraints from the template
         * parameter \a LPConstraints were added successfully to the
         * infrastructure for the search tree.
         *
         * This query function is important because some constraints require
         * additional preconditions on the underlying triangulation, and
         * so these constraints cannot be added in some circumstances.
         * If it is possible that the constraints
         * might not be added successfully, this function should be
         * tested as soon as the TreeTraversal object has been created.
         *
         * If the extra constraints were not added successfully, the search
         * tree will be left in a consistent state but will give incorrect
         * results (specifically, the extra constraints will be treated as
         * zero functions).
         *
         * @return \c true if the constraints were \e not added
         * successfully, or \c false if the constraints were added successfully.
         */
        inline bool constraintsBroken() const {
            return origTableaux_.constraintsBroken();
        }

        /**
         * Returns the total number of nodes in the search tree that we
         * have visited thus far in the tree traversal.
         * This figure might grow much faster than the number of solutions,
         * since it also counts traversals through "dead ends" in the
         * search tree.
         *
         * This counts all nodes that we visit, including those that fail
         * any or all of the domination, feasibility and zero tests.
         * The precise way that this number is calculated is subject to
         * change in future versions of Regina.
         *
         * If you called TreeEnumeration::run() or TreeSingleSoln::find(),
         * then this will be the total number of nodes that were visited
         * in the entire tree traversal.  If you are calling
         * TreeEnumeration::next() one surface at time, this will be the
         * partial count of how many nodes have been visited so far.
         *
         * @return the number of nodes visited so far.
         */
        inline unsigned long nVisited() const {
            return nVisited_;
        }

        /**
         * Writes the current type vector to the given output stream.
         * There will be no spaces between the types, and there will be
         * no final newline.
         *
         * @param out the output stream to which to write.
         */
        inline void dumpTypes(std::ostream& out) const {
            for (unsigned i = 0; i < nTypes_; ++i)
                out << static_cast<int>(type_[i]);
        }

        /**
         * Reconstructs the full normal surface that is represented by
         * the type vector at the current stage of the search.
         *
         * The surface that is returned will be newly constructed, and
         * it is the caller's responsibility to destroy it when it is no
         * longer required.
         *
         * If the current type vector does not represent a \e vertex
         * normal surface (which may be the case when calling
         * TreeSingleSoln::find()), then there may be many normal surfaces
         * all represented by the same type vector; in this case there are
         * no further guarantees about \e which of these normal surfaces
         * you will get.
         *
         * \pre This tree traversal is at a point in the search where
         * it has found a feasible solution that represents a normal surface
         * (though this need not be a vertex surface).
         * This condition is always true after TreeEnumeration::next()
         * or TreeSingleSoln::find() returns \c true, or any time that
         * TreeEnumeration::run() calls its callback function.
         *
         * @return a normal surface that has been found at the current stage
         * of the search.
         */
        NNormalSurface* buildSurface() const {
            // Note that the vector constructors automatically set all
            // elements to zero, as required by LPData::extractSolution().
            NNormalSurfaceVector* v;
            if (coords_ == NNormalSurfaceList::QUAD ||
                    coords_ == NNormalSurfaceList::AN_QUAD_OCT)
                v = new NNormalSurfaceVectorQuad(3 * nTets_);
            else if (coords_ == NNormalSurfaceList::STANDARD ||
                    coords_ == NNormalSurfaceList::AN_STANDARD)
                v = new NNormalSurfaceVectorStandard(7 * nTets_);
            else
                return 0;

            lpSlot_[nTypes_]->extractSolution(*v, type_);

            if (coords_ == NNormalSurfaceList::QUAD ||
                    coords_ == NNormalSurfaceList::STANDARD)
                return new NNormalSurface(origTableaux_.tri(), v);

            // We have an almost normal surface: restore the octagon
            // coordinates.
            NNormalSurfaceVector* an;
            unsigned i, j;
            if (coords_ == NNormalSurfaceList::AN_QUAD_OCT) {
                an = new NNormalSurfaceVectorQuadOct(6 * nTets_);
                for (i = 0; i < nTets_; ++i)
                    for (j = 0; j < 3; ++j)
                        an->setElement(6 * i + j, (*v)[3 * i + j]);
                if (octLevel_ >= 0) {
                    unsigned octTet = (origTableaux_.columnPerm()[
                        3 * typeOrder_[octLevel_]] / 3);
                    unsigned octType = type_[typeOrder_[octLevel_]] - 4;
                    an->setElement(6 * octTet + 3 + octType,
                        (*v)[3 * octTet + (octType + 1) % 3]);
                    for (j = 0; j < 3; ++j)
                        an->setElement(6 * octTet + j, 0);
                }
            } else {
                an = new NNormalSurfaceVectorANStandard(10 * nTets_);
                for (i = 0; i < nTets_; ++i)
                    for (j = 0; j < 7; ++j)
                        an->setElement(10 * i + j, (*v)[7 * i + j]);
                if (octLevel_ >= 0) {
                    unsigned octTet = (origTableaux_.columnPerm()[
                        3 * typeOrder_[octLevel_]] / 7);
                    unsigned octType = type_[typeOrder_[octLevel_]] - 4;
                    an->setElement(10 * octTet + 7 + octType,
                        (*v)[7 * octTet + 4 + (octType + 1) % 3]);
                    for (j = 0; j < 3; ++j)
                        an->setElement(10 * octTet + 4 + j, 0);
                }
            }
            delete v;
            return new NNormalSurface(origTableaux_.tri(), an);
        }

        /**
         * Ensures that the given normal or almost normal surface satisfies
         * the matching equations, as well as any additional constraints
         * from the template parameter LPConstraint.
         *
         * This routine is provided for diagnostic, debugging and verification
         * purposes.
         *
         * Instead of using the initial tableaux to verify the matching
         * equations, this routine goes back to the original matching
         * equations matrix as constructed by regina::makeMatchingEquations().
         * This ensures that the test is independent of any potential
         * problems with the tableaux.  You are not required to pass
         * your own matching equations (if you don't, they will be temporarily
         * reconstructed for you); however, you may pass your own if you
         * wish to use a non-standard matching equation matrix, and/or
         * reuse the same matrix to avoid the overhead of reconstructing it
         * every time this routine is called.
         *
         * \pre The normal or almost normal surface \a s uses the same
         * coordinate system as was passed to the TreeTraversal constructor.
         *
         * \pre If \a matchingEqns is non-null, then the number of columns
         * in \a matchingEqns is equal to the number of coordinates in the
         * underlying normal or almost normal coordinate system.
         *
         * @param s the normal surface to verify.
         * @param matchingEqns the matching equations to check against
         * the given surface; this may be 0, in which case the matching
         * equations will be temporarily reconstructed for you using
         * regina::makeMatchingEquations().
         * @return \c true if the given surfaces passes all of the test
         * described above, or \c false if it fails one or more tests
         * (indicating a problem or error).
         */
        bool verify(const NNormalSurface* s,
                const NMatrixInt* matchingEqns = 0) const {
            // Rebuild the matching equations if necessary.
            NMatrixInt* tmpEqns = 0;
            if (! matchingEqns) {
                tmpEqns = regina::makeMatchingEquations(
                    origTableaux_.tri(), coords_);
                matchingEqns = tmpEqns;
            }

            // Verify the matching equations.
            unsigned row, col;
            for (row = 0; row < matchingEqns->rows(); ++row) {
                NLargeInteger ans; // Initialised to zero.
                for (col = 0; col < matchingEqns->columns(); ++col)
                    ans += (matchingEqns->entry(row, col) *
                        (*s->rawVector())[col]);
                if (ans != 0) {
                    delete tmpEqns;
                    return false;
                }
            }
            delete tmpEqns;

            // Verify any additional constraints.
            return LPConstraint::verify(s);
        }

        /**
         * TODO: Document.
         */
        inline void cancel() {
            regina::NMutex::MutexLock lock(mCancel_);
            cancelled_ = true;
        }

        /**
         * TODO: Document.
         */
        inline bool cancelled() const {
            regina::NMutex::MutexLock lock(mCancel_);
            return cancelled_;
        }

    protected:
        /**
         * Initialises a new base object for running the tree traversal
         * algorithm.  This routine may only be called by subclass constructors;
         * for more information on how to create and run a tree
         * traversal, see the subclasses TreeEnumeration and
         * TreeSingleSoln instead.
         *
         * \pre The given triangulation is non-empty.
         *
         * @param tri the triangulation in which we wish to search for
         * normal surfaces.
         * @param coords the coordinate system in which wish to search for
         * normal surfaces.  This must be one of
         * NNormalSurfaceList::QUAD, NNormalSurfaceList::STANDARD,
         * NNormalSurfaceList::AN_QUAD_OCT, or NNormalSurfaceList::AN_STANDARD.
         * @param branchesPerQuad the maximum number of branches we
         * spawn in the search tree for each quadrilateral type
         * (e.g., 4 for a vanilla normal surface tree traversal algorithm).
         * @param branchesPerTri the maximum number of branches we
         * spawn in the search tree for each triangle type
         * (e.g., 2 for a vanilla normal surface tree traversal algorithm).
         * @param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces, or \c false if we
         * should optimise the tableaux for an existence test (such as
         * searching for a non-trivial normal disc or sphere).
         */
        TreeTraversal(NTriangulation* tri, int coords,
                int branchesPerQuad, int branchesPerTri, bool enumeration) :
                BanConstraint(tri, coords),
                origTableaux_(tri,
                    (coords == NNormalSurfaceList::QUAD ||
                     coords == NNormalSurfaceList::AN_QUAD_OCT ?
                     NNormalSurfaceList::QUAD : NNormalSurfaceList::STANDARD),
                     enumeration),
                coords_(coords),
                nTets_(tri->getNumberOfTetrahedra()),
                nTypes_(coords == NNormalSurfaceList::QUAD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ?
                    nTets_ : 5 * nTets_),
                /* Each time we branch, one LP can be solved in-place:
                   therefore we use branchesPerQuad-1 and branchesPerTri-1.
                   The final +1 is for the root node. */
                nTableaux_(coords == NNormalSurfaceList::QUAD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ?
                    (branchesPerQuad - 1) * nTets_ + 1 :
                    (branchesPerQuad - 1) * nTets_ +
                        (branchesPerTri - 1) * nTets_ * 4 + 1),
                type_(new char[nTypes_ + 1]),
                typeOrder_(new int[nTypes_]),
                level_(0),
                octLevel_(coords == NNormalSurfaceList::AN_STANDARD ||
                    coords == NNormalSurfaceList::AN_QUAD_OCT ? -1 : nTypes_),
                lp_(new LPData<LPConstraint>[nTableaux_]),
                lpSlot_(new LPData<LPConstraint>*[nTypes_ + 1]),
                nextSlot_(new LPData<LPConstraint>*[nTypes_ + 1]),
                nVisited_(0),
                cancelled_(false) {
            // Initialise the type vector to the zero vector.
            std::fill(type_, type_ + nTypes_ + 1, 0);

            // Set a default type order.
            unsigned i;
            for (i = 0; i < nTypes_; ++i)
                typeOrder_[i] = i;

            // Reserve space for all the tableaux that we will ever need.
            for (i = 0; i < nTableaux_; ++i)
                lp_[i].reserve(&origTableaux_);

            // Mark the location of the initial tableaux at the root node.
            lpSlot_[0] = lp_;
            nextSlot_[0] = lp_ + 1;

            // Set up the ban list.
            BanConstraint::init(origTableaux_.columnPerm());

            // Reserve space for our additional temporary tableaux.
            tmpLP_[0].reserve(&origTableaux_);
            tmpLP_[1].reserve(&origTableaux_);
            tmpLP_[2].reserve(&origTableaux_);
            tmpLP_[3].reserve(&origTableaux_);
        }

        /**
         * Destroys this object.
         */
        inline ~TreeTraversal() {
            delete[] type_;
            delete[] typeOrder_;
            delete[] lp_;
            delete[] lpSlot_;
            delete[] nextSlot_;
        }

        /**
         * Rearranges the search tree so that \a nextType becomes the next
         * type that we process.
         *
         * Specifically, this routine will set typeOrder_[level_ + 1]
         * to \a nextType_, and will move other elements of typeOrder_
         * back by one position to make space as required.
         *
         * \pre \a nextType is in the range 0,...,\a nTypes-1 inclusive.
         * \pre \a nextType is still waiting to be processed; that is,
         * \a nextType does not appear in the list
         * typeOrder_[0,...,level_].
         *
         * @param nextType the next type to process.
         */
        void setNext(int nextType) {
            int* pos = std::find(typeOrder_ + level_ + 1,
                typeOrder_ + nTypes_, nextType);
            if (pos != typeOrder_ + level_ + 1) {
                // Use memmove(), which is safe when the source and
                // destination ranges overlap.
                memmove(typeOrder_ + level_ + 2 /* dest */,
                    typeOrder_ + level_ + 1 /* src */,
                    (pos - (typeOrder_ + level_ + 1)) * sizeof(int));
                typeOrder_[level_ + 1] = nextType;
            }
        }

        /**
         * Returns the next unmarked triangle type from a given starting
         * point.  Specifically, this routine returns the first unmarked
         * triangle type whose type number is greater than or equal to
         * \a startFrom.  For more information on marking, see the
         * BanConstraintBase class notes.
         *
         * This routine simply searches through types by increasing index
         * into the type vector; in particular, it does \e not make any use
         * of the reordering defined by the \a typeOrder_ array.
         *
         * \pre We are working in standard normal or almost normal
         * coordinates.  That is, the coordinate system passed to the
         * TreeTraversal constructor was one of NNormalSurfaceList::STANDARD
         * or NNormalSurfaceList::AN_STANDARD.
         *
         * \pre The argument \a startFrom is at least \a nTets_ (i.e.,
         * it is at least as large as the index of the first triangle type).
         *
         * @param startFrom the index into the type vector of the triangle type
         * from which we begin searching.
         * @return the index into the type vector of the next unmarked
         * triangle type from \a startFrom onwards, or -1 if there are no
         * more remaining.
         */
        int nextUnmarkedTriangleType(int startFrom) {
            while (startFrom < nTypes_ &&
                    BanConstraint::marked_[2 * nTets_ + startFrom])
                ++startFrom;
            return (startFrom == nTypes_ ? -1 : startFrom);
        }

        /**
         * Determines how many different values we could assign to the given
         * quadrilateral type and still obtain a feasible system.
         *
         * This will involve solving four linear programs, all based on
         * the current state of the tableaux at the current level of the
         * search tree.  These assign 0, 1, 2 and 3 to the given
         * quadrilateral type and enforce the corresponding constraints;
         * here types 0 and 1 are counted separately as in TreeEnumeration,
         * not merged together as in TreeSingleSoln.
         *
         * \pre The given quadrilateral type has not yet been processed in
         * the search tree (i.e., it has not had an explicit value selected).
         *
         * @param quadType the quadrilateral type to examine.
         * @return the number of type values 0, 1, 2 or 3 that yield a
         * feasible system; this will be between 0 and 4 inclusive.
         */
        int feasibleBranches(int quadType) {
            // Spin off clones for the new linear programs (reusing as much
            // work as possible).
            tmpLP_[0].initClone(*lpSlot_[level_ + 1]);

            tmpLP_[1].initClone(tmpLP_[0]);
            tmpLP_[1].constrainZero(3 * quadType + 1);
            tmpLP_[1].constrainZero(3 * quadType + 2);
            tmpLP_[1].constrainPositive(3 * quadType);

            tmpLP_[0].constrainZero(3 * quadType);
            if (! tmpLP_[0].isFeasible()) {
                // Branches 0, 2 and 3 will all be infeasible.
                // Save some work and jump straight to the solution.
                return (tmpLP_[1].isFeasible() ? 1 : 0);
            }

            tmpLP_[2].initClone(tmpLP_[0]);
            tmpLP_[2].constrainZero(3 * quadType + 2);
            tmpLP_[2].constrainPositive(3 * quadType + 1);

            tmpLP_[0].constrainZero(3 * quadType + 1);

            tmpLP_[3].initClone(tmpLP_[0]);
            tmpLP_[3].constrainPositive(3 * quadType + 2);

            tmpLP_[0].constrainZero(3 * quadType + 2);

            // Determine which of these systems are feasible.
            return ((tmpLP_[0].isFeasible() ? 1 : 0) +
                (tmpLP_[1].isFeasible() ? 1 : 0) +
                (tmpLP_[2].isFeasible() ? 1 : 0) +
                (tmpLP_[3].isFeasible() ? 1 : 0));
        }
};

/**
 * The main entry point for the tree traversal algorithm to enumerate all
 * vertex normal or almost normal surfaces in a 3-manifold triangulation.
 *
 * This class essentially implements the algorithm from "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica (to appear), DOI 10.1007/s00453-012-9645-3.
 *
 * To enumerate all vertex surfaces for a given 3-manifold
 * triangulation, simply construct a TreeTraversal object and call run().
 *
 * Alternatively, you can have more fine-grained control over the search.
 * Instead of calling run(), you can construct a TreeTraversal object and
 * repeatedly call next() to step through each vertex surface one at
 * a time.  This allows you to pause and resume the search as you please.
 *
 * If you simply wish to detect a single non-trivial solution under
 * additional constraints (such as positive Euler characteristic), then
 * use the class TreeSingleSoln instead, which is optimised for this purpose.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NNormalSurfaceList::QUAD), standard normal coordinates
 * (NNormalSurfaceList::STANDARD), quadrilateral-octagon almost normal
 * coordinates (NNormalSurfaceList::AN_QUAD_OCT), or standard almost
 * normal coordinates (NNormalSurfaceList::AN_STANDARD).  For almost
 * normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon \e type in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * By using appropriate template parameters \a LPConstraint and/or
 * \a BanConstraint, it is possible to impose additional linear
 * constraints on the normal surface solution cone, and/or explicitly force
 * particular normal coordinates to zero.  In this case, the notion of
 * "vertex surface" is modified to mean a normal surface whose coordinates
 * lie on an extreme ray of the restricted solution cone under these additional
 * constraints (and whose coordinates are integers with no common divisor).
 * See the LPConstraintBase and BanConstraintBase class notes for
 * details.
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintSubspace and BanConstraintBase respectively.  Note in
 * particular that the base class LPConstraintBase is not enough here.
 * See the LPConstraintBase, LPConstraintSubspace and BanConstraintBase
 * class notes for further details.
 *
 * \warning Although the tree traversal algorithm can run in standard
 * normal or almost normal coordinates, this is not recommended: it is likely
 * to be \e much slower than in quadrilateral or quadrilateral-octagon
 * coordinates respectively.  Instead you should enumerate vertex
 * solutions using quadrilateral or quadrilateral-octagon coordinates, and
 * then run the conversion procedure NNormalSurfaceList::quadToStandard()
 * or NNormalSurfaceList::quadOctToStandardAN().
 */
template <typename LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone>
class TreeEnumeration : public TreeTraversal<LPConstraint, BanConstraint> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint>::dumpTypes;

    protected:
        // Since we have a template base class, we need to explicitly
        // list the inherited member variables that we use.
        // Note that these are all protected in the base class, and so
        // we are not changing any access restrictions here.
        using TreeTraversal<LPConstraint, BanConstraint>::level_;
        using TreeTraversal<LPConstraint, BanConstraint>::lp_;
        using TreeTraversal<LPConstraint, BanConstraint>::lpSlot_;
        using TreeTraversal<LPConstraint, BanConstraint>::nextSlot_;
        using TreeTraversal<LPConstraint, BanConstraint>::nTets_;
        using TreeTraversal<LPConstraint, BanConstraint>::nTypes_;
        using TreeTraversal<LPConstraint, BanConstraint>::nVisited_;
        using TreeTraversal<LPConstraint, BanConstraint>::octLevel_;
        using TreeTraversal<LPConstraint, BanConstraint>::type_;
        using TreeTraversal<LPConstraint, BanConstraint>::typeOrder_;

        using TreeTraversal<LPConstraint, BanConstraint>::cancelled;

    private:
        TypeTrie<7> solns_;
            /**< A trie that holds the type vectors for all vertex
                 surfaces found so far.
                 We wastefully allow for 7 possible types always (which
                 are required for almost normal surfaces); the
                 performance loss from changing 4 to 7 is negligible. */
        unsigned long nSolns_;
            /**< The number of vertex surfaces found so far. */

        int lastNonZero_;
            /**< The index into typeOrder_ corresponding to the last non-zero
                 type that was selected, or -1 if we still have the zero
                 vector.  Here "last" means "last chosen"; that is, the
                 highest index into typeOrder_ that gives a non-zero type. */

    public:
        /**
         * Creates a new object for running the tree traversal algorithm.
         *
         * This prepares the algorithm; in order to run the algorithm
         * and enumerate vertex surfaces, you can either:
         *
         * - call run(), which enumerates all vertex surfaces
         *   with a single function call;
         *
         * - repeatedly call next(), which will step to the next vertex
         *   surface each time you call it.
         *
         * \warning Although it is supported, it is highly recommended that you
         * do \e not run a full vertex enumeration in standard normal
         * or almost normal coordinates (this is for performance reasons).
         * See the class notes for further discussion and better alternatives.
         * In normal circumstances you should run a full vertex enumeration
         * in quadrilateral or quadrilateral-octagon coordinates only.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre Both the trianglation and the given coordinate system
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * @param tri the triangulation in which we wish to enumerate
         * vertex surfaces.
         * @param coords the coordinate system in which wish to enumerate
         * vertex surfaces.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline TreeEnumeration(NTriangulation* tri, int coords) :
                TreeTraversal<LPConstraint, BanConstraint>(tri, coords,
                    (coords == NNormalSurfaceList::AN_QUAD_OCT ||
                     coords == NNormalSurfaceList::AN_STANDARD ?
                     7 : 4) /* branches per quad */,
                    2 /* branches per triangle */,
                    true /* enumeration */),
                nSolns_(0),
                lastNonZero_(-1) {
        }

        /**
         * Returns the total number of vertex normal or almost normal surfaces
         * found thus far in the tree traversal search.
         *
         * If you called run(), then this will simply be the total number of
         * vertex surfaces.  If you are calling next() one
         * surface at time, this will be the partial count of how many
         * vertex surfaces have been found until now.
         *
         * @return the number of solutions found so far.
         */
        inline unsigned long nSolns() const {
            return nSolns_;
        }

        /**
         * Runs the complete tree traversal algorithm to enumerate
         * vertex normal or almost normal surfaces.
         *
         * For each vertex surface that is found, this routine
         * will call the function \a useSoln.  It will pass two
         * arguments to this function: (i) this tree enumeration object,
         * and (ii) an arbitrary piece of data that you can supply via
         * the argument \a arg.
         *
         * You can extract details of the solution directly from the
         * tree enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full normal or
         * almost normal surface using buildSurface() and perform some other
         * operations upon it.  If you do call buildSurface(), remember
         * to delete the normal surface once you are finished with it.
         *
         * The tree traversal will block until your callback function
         * \a useSoln returns.  If the callback function returns \c true,
         * then run() will continue the tree traversal.  If it returns
         * \c false, then run() will abort the search and return immediately.
         *
         * The usual way of using this routine is to construct a
         * TreeEnumeration object and then immediately call run().  However,
         * if you prefer, you may call run() after one or more calls to next().
         * In this case, run() will continue the search from the current point
         * and run it to its completion.  In other words, run() will locate
         * and call \a useSoln for all vertex surfaces that had not yet
         * been found, but it will not call \a useSoln on those surfaces
         * that had previously been found during earlier calls to next().
         *
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @param useSoln a callback function that will be called each
         * time we locate a vertex surface, as described above.
         * @param arg the second argument to pass to the callback
         * function; this may be any type of data that you like.
         */
        void run(bool (*useSoln)(const TreeEnumeration&, void*),
                void* arg = 0) {
            while (next())
                if (! useSoln(*this, arg))
                    return;
        }

        /**
         * An incremental step in the tree traversal algorithm that
         * runs forward until it finds the next solution.
         * Specifically: this continues the tree traversal from the
         * current point until either it finds the next vertex normal
         * or almost normal surface (in which case it returns \c true), or
         * until the tree traversal is completely finished with no more
         * solutions to be found (in which case it returns \c false).
         *
         * If you simply wish to find and process all vertex surfaces,
         * you may wish to consider the all-in-one routine
         * run() instead.  By using next() to step through one solution
         * at a time however, you obtain more fine-grained control: for
         * instance, you can "pause" and restart the search, or have
         * tighter control over multithreading.
         *
         * If next() does return \c true because it found a solution,
         * you can extract details of the solution directly from this
         * tree enumeration object: for instance, you can dump the type
         * vector using dumpTypes(), or you can reconstruct the full normal or
         * almost normal surface using buildSurface() and perform some other
         * operations upon it.  If you do call buildSurface(), remember
         * to delete the normal surface once you are finished with it.
         *
         * \pre The tree traversal algorithm has not yet finished.
         * That is, you have not called run() before, and if you have
         * called next() then it has always returned \c true (indicating
         * that it has not yet finished the search).
         *
         * @return \c true if we found another vertex surface, or
         * \c false if the search has now finished and no more vertex
         * surfaces were found.
         */
        bool next() {
            if (lastNonZero_ < 0) {
                // Our type vector is the zero vector.
                // This means we are starting the search from the very
                // beginning.
                //
                // Prepare the root node by finding an initial basis
                // from the original starting tableaux.
                lp_[0].initStart();
                BanConstraint::enforceBans(lp_[0]);
                ++nVisited_;

                // Is the system feasible at the root node?
                // If not, there can be no solutions at all.
                if (! lp_[0].isFeasible())
                    return false;
            } else {
                // We are starting the search from a previous solution.
                // Make the next incremental change to the type vector
                // to continue the search.
                //
                // We *should* increment type_[typeOrder_[nTypes_-1]].
                // However, if type_[typeOrder_[nTypes_-1]] is zero then this
                // will fail the domination test.  In fact, incrementing *any*
                // trailing zeroes in our type vector will likewise fail
                // the domination test (since it will dominate the
                // previous solution that we are now stepping away from).
                // We can therefore shortcut the search and wind our way
                // back to the last *non-zero* element in the type
                // vector, and increment that instead.
                //
                // Note: we must have *some* non-zero element in the
                // type vector, because we know that lastNonZero_ >= 0.
                level_ = lastNonZero_;
                ++type_[typeOrder_[level_]];
            }

            // And... continue the search!
            unsigned idx; /* Index of the type we are currently choosing. */
            bool outOfRange;
            while (! cancelled()) {
#ifdef REGINA_TREE_TRACE
                dumpTypes(std::cout);
                std::cout << std::endl;
#endif
                /* INVARIANTS:
                 *
                 * - 0 <= level_ < nTypes_.
                 *
                 * - We have explicitly set type_[typeOrder_[0,...,level_]],
                 *   though it is possible that type_[typeOrder_[level_]] is
                 *   out of range (too large).  All later elements
                 *   type_[typeOrder_[level_+1,...,nTypes_-1]] are 0.
                 *
                 * - The parent node in the tree (where
                 *   type_[typeOrder_[level_]] == 0) passes the feasibility
                 *   and domination tests; however we do not yet know whether
                 *   this node in the tree (with our new value for
                 *   type_[typeOrder_[level_]]) passes these tests.
                 */
                idx = typeOrder_[level_];

                // Check whether type_[idx] is out of range,
                // and if so then backtrack further up the tree.
                outOfRange = false;
                if (type_[idx] == 4) {
                    // This quadrilateral column is out of range.
                    if (octLevel_ < 0) {
                        // But... we can make it an octagon column instead.
                        octLevel_ = level_;
                    } else
                        outOfRange = true;
                } else if (type_[idx] == 7) {
                    // This octagon column is out of range.
                    // Clear octLevel_ again so that some other level can
                    // claim the one and only octagon column if it likes.
                    octLevel_ = -1;
                    outOfRange = true;
                } else if (idx >= nTets_ && type_[idx] == 2) {
                    // This triangle column is out of range.
                    outOfRange = true;
                }

                if (outOfRange) {
                    // Backtrack.
                    type_[idx] = 0;
                    --level_;
                    if (level_ < 0) {
                        // Out of options: the tree traversal is finished.
                        return false;
                    }
                    ++type_[typeOrder_[level_]];
                    lastNonZero_ = level_;
                    continue;
                }

                // This is a node in the search tree that we need to
                // examine.
                ++nVisited_;

                // Check the domination test.
                // If this fails then abandon this subtree, increment the
                // type at the current level, and continue searching.
                //
                // Note that if type_[idx] == 0 then we do not
                // need to check the domination test, since we know the parent
                // node passed the domination test and setting
                // type_[idx] = 0 will not change the result.
                if (type_[idx] && solns_.dominates(type_, nTypes_)) {
                    ++type_[idx];
                    lastNonZero_ = level_;
                    continue;
                }

                // Leave the zero test until a bit later; there is some
                // dual simplex work we need to do with the zero vector
                // even though we know we don't want it as a solution.

                // All that's left is the feasibility test.
                // Bring on the linear programming.

                // First, prepare the tableaux for our new type at this level
                // of the tree.
                if (! type_[idx]) {
                    // This is the first time we have visited this node.
                    // Since the parent node already passes the
                    // feasibility test, we will simply overwrite the
                    // parent tableaux "in place", avoiding the need for
                    // an expensive copy operation.
                    lpSlot_[level_ + 1] = lpSlot_[level_];

                    // Since type_[idx] = 0, we will be adding
                    // up to three new constraints x_i = x_{i+1} = x_{i+2} = 0.
                    // So that we can reuse as much work as possible, we
                    // will gradually spin off clones of this tableaux
                    // that we can later use with
                    // type_[idx] == 1, 2 or 3 (and 4, 5 or 6 if we allow
                    // octagons).
                    //
                    // How we add these constraints and spin off clones
                    // depends on whether are working with quadrilateral
                    // columns or triangle columns.
                    if (idx < nTets_) {
                        // We're working with a quadrilateral column.
                        if (octLevel_ < 0) {
                            // We must support both quadrilaterals and octagons.
                            nextSlot_[level_ + 1] = nextSlot_[level_] + 6;

                            // Here we have three constraints
                            // x_{3k} = x_{3k+1} = x_{3k+2} = 0,
                            // and later we will be trying quadrilateral types
                            // 1, 2 and 3, and octagon types 4, 5 and 6.

                            // First spin off clones that we will later use with
                            // type_[idx] = 1, 5 and 6.  These clones inherit
                            // no extra constraints: for type 1 we will need to
                            // fix x_{3k} >= 1 and x_{3k+1} = x_{3k+2} = 0
                            // later, and for types 5 and 6 we must likewise
                            // fix all constraints later on.
                            nextSlot_[level_]->initClone(*lpSlot_[level_]);
                            (nextSlot_[level_] + 4)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 5)->initClone(
                                *lpSlot_[level_]);

                            // Now we can fix x_{3k} = 0.
                            lpSlot_[level_]->constrainZero(3 * idx);

                            // Next spin off clones that we will later use with
                            // type_[idx] = 2 and 4.  These clones already
                            // inherit the constraint x_{3k} = 0, and we
                            // will add the other constraints later (for
                            // instance, for type 2 we will add x_{3k+1} >= 1
                            // and x_{3k+2} = 0 later).
                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 3)->initClone(
                                *lpSlot_[level_]);

                            // Now we can fix x_{3k+1} = 0.
                            lpSlot_[level_]->constrainZero(3 * idx + 1);

                            // Finally spin off a clone that we will later use
                            // with type_[idx] = 3.  This clone already
                            // inherits the constraint x_{3k} = x_{3k+1} = 0,
                            // which only leaves us x_{3k+2} >= 1 to add later.
                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);

                            // At last we add the final constraint x_{3k+2} = 0
                            // for this node.
                            lpSlot_[level_]->constrainZero(3 * idx + 2);

                            // This node now has all of the necessary
                            // constraints x_{3k} = x_{3k+1} = x_{3k+2} = 0.
                        } else {
                            // We are supporting quadrilaterals only.
                            // As above, but with types 1, 2 and 3 only.
                            nextSlot_[level_ + 1] = nextSlot_[level_] + 3;

                            nextSlot_[level_]->initClone(*lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx);

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 1);

                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 2);
                        }
                    } else {
                        // We're working with a triangle column.
                        nextSlot_[level_ + 1] = nextSlot_[level_] + 1;

                        // Here we only have one constraint x_k = 0,
                        // and later we only have type 1 to try.

                        // Spin off a clone that we will later use with
                        // type_[idx] = 1.  This clone inherits
                        // no extra constraints.
                        nextSlot_[level_]->initClone(*lpSlot_[level_]);

                        // Now add the one and only constraint x_k = 0 for
                        // this node.
                        lpSlot_[level_]->constrainZero(2 * nTets_ + idx);
                    }
                } else {
                    // This is not the first time we have visited this node.
                    // Find the appropriate clone that we spun off earlier when
                    // type_[idx] was 0, and add the missing
                    // constraints that we did not enforce during the
                    // cloning process.
                    //
                    // We always enforce constraints of the form x_i = 0
                    // before constraints of the form x_i >= 1.
                    // This is in the hope that x_i = 0 is easier to
                    // deal with, and if we break feasibility earlier then
                    // this saves us some work later on.
                    //
                    // (Note that we did indeed spin off clones earlier,
                    // since if we pass the domination test now for
                    // type_[idx] != 0 then we must have passed
                    // it earlier for type_[idx] == 0.)
                    if (idx < nTets_) {
                        // Quadrilateral columns (type is 1, 2 or 3,
                        // or 4, 5 or 6 if we allow octagons):
                        lpSlot_[level_ + 1] =
                            nextSlot_[level_] + type_[idx] - 1;

                        switch (type_[idx]) {
                            case 1:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx);
                                break;
                            case 2:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 1);
                                break;
                            case 3:
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 2);
                                break;
                            case 4:
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx + 1, 3 * idx + 2);
                                break;
                            case 5:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 2);
                                break;
                            case 6:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 2);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 1);
                                break;
                        }
                    } else {
                        // Triangle columns (type is 1):
                        lpSlot_[level_ + 1] = nextSlot_[level_];

                        lpSlot_[level_ + 1]->constrainPositive(
                            2 * nTets_ + idx);
                    }
                }

                // *Now* we can enforce the zero test.
                // We could not do this earlier because, even if we have
                // the zero vector, we still needed to spin off clones
                // for type_[idx] = 1, 2 and 3.
                if (lastNonZero_ < 0 && level_ == nTypes_ - 1) {
                    // We failed the zero test.
                    // Abandon this subtree, increment the type at the
                    // current level, and continue searching.
                    ++type_[idx];
                    lastNonZero_ = level_;
                    continue;
                }

                // Now all our constraints are enforced, and we can
                // simply test the tableaux for feasibility.
                if (lpSlot_[level_ + 1]->isFeasible()) {
                    if (level_ < nTypes_ - 1) {
                        // We pass the feasibility test, but we're
                        // not at a leaf node.
                        // Head deeper into the tree.
                        ++level_;
                    } else {
                        // We pass the feasibility test, *and* we're at
                        // a leaf node.  This means we've found a solution!
                        solns_.insert(type_, nTypes_);
                        ++nSolns_;
                        return true;
                    }
                } else {
                    // We failed the feasibility test.
                    // Abandon this subtree, increment the type at the
                    // current level, and continue searching.
                    ++type_[idx];
                    lastNonZero_ = level_;
                }
            }

            // If we ever make it out here, it's because some other
            // thread cancelled the search.  The result should be ignored.
            return false;
        }

        /**
         * A callback function that writes to standard output the type vector
         * at the current point in the given tree traversal search.
         * You can use this as the callback function \a useSoln that is
         * passed to run().
         *
         * The type vector will be written on a single line, with no
         * spaces between types, with a prefix indicating which solution
         * we are up to, and with a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current type vector.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeTypes(const TreeEnumeration& tree, void*) {
            std::cout << "SOLN #" << tree.nSolns() << ": ";
            tree.dumpTypes(std::cout);
            std::cout << std::endl;
            return true;
        }

        /**
         * A callback function that writes to standard output the full
         * triangle-quadrilateral coordinates of the vertex normal
         * or almost normal surface at the current point in the given
         * tree traversal search.  You can use this as the callback function
         * \a useSoln that is passed to run().
         *
         * The normal surface coordinates will be written on a single line, with
         * spaces and punctuation separating them, a prefix indicating which
         * solution we are up to, and a final newline appended.
         * This output format is subject to change in future versions of Regina.
         *
         * The second (void*) argument is ignored.  It is only present
         * for compatibility with run().
         *
         * \pre The given tree traversal is at a point in the search where
         * it has reached the deepest level of the search tree and found a
         * feasible solution that represents a vertex normal or almost
         * normal surface.  This is always the case any time after next()
         * returns \c true, or any time that run() calls its callback function.
         *
         * @param tree the tree traversal object from which we are
         * extracting the current vertex normal or almost normal surface.
         * @return \c true (which indicates to run() that we should
         * continue the tree traversal).
         */
        static bool writeSurface(const TreeEnumeration& tree, void*) {
            std::cout << "SOLN #" << tree.nSolns() << ": ";
            NNormalSurface* f = tree.buildSurface();
            std::cout << f->toString() << std::endl;
            delete f;
            return true;
        }
};

/**
 * The main entry point for the tree traversal / branching algorithm to locate
 * a single non-trivial normal surface satisfying given constraints within
 * a 3-manifold triangulation.  The constraints are passed using a
 * combination of the template arguments LPConstraint and BanConstraint.
 *
 * A common application of this algorithm is to find a surface of positive
 * Euler characteristic, using the template argument LPConstraintEuler.
 * This is useful for tasks such as 0-efficiency testing
 * and prime decomposition (when this is done in standard normal coordinates),
 * and also 3-sphere recognition (when this is done in standard almost normal
 * coordinates).  Indeed, the underlying algorithm is optimised for
 * precisely this application.
 *
 * By a "non-trivial" surface, we mean that at least one triangle coordinate
 * is zero.  Philosophically this is to avoid vertex linking surfaces,
 * though if the triangulation has more than one vertex then this takes
 * on a different meaning.  See the warning on this matter below.
 *
 * Be warned that this routine does not eliminate the zero vector, and so
 * the template argument LPConstraint should include at least one
 * constraint that eliminates the zero vector (e.g., positive Euler
 * characteristic).  Otherwise this algorithm may simply return the zero
 * vector, and the information gained will not be very useful.
 *
 * For any given normal coordinate, this routine will always try setting
 * that coordinate to zero before it tries setting it to non-zero.  In
 * other words, if it does find a surface satisfying the given constraints,
 * then it is guaranteed that the set of non-zero coordinate positions
 * will be minimal (though not necessary a global \e minimum).
 * In many settings (such as when using LPConstraintEuler), this guarantees
 * that the final surface (if it exists) will be a vertex normal or
 * almost normal surface.
 *
 * The underlying algorithm is described in "A fast branching algorithm for
 * unknot recognition with experimental polynomial-time behaviour",
 * Burton and Ozlen, arXiv:1211.1079, and uses significant material from
 * "A tree traversal algorithm for decision problems in knot theory and
 * 3-manifold topology", Burton and Ozlen, Algorithmica (to appear),
 * DOI 10.1007/s00453-012-9645-3.
 *
 * To use this class, i.e., to locate a non-trivial normal or almost normal
 * surface under the given constraints or to prove that no such surface exists,
 * you can simply construct a TreeSingleSoln object and call find().  You can
 * then call buildSurface() to extract the details of the surface that was
 * found.
 *
 * If you wish to enumerate \e all vertex surfaces in a 3-manifold
 * triangulation (instead of finding just one), you should use the class
 * TreeEnumeration instead.
 *
 * This tree traversal can only enumerate surfaces in quadrilateral normal
 * coordinates (NNormalSurfaceList::QUAD), standard normal coordinates
 * (NNormalSurfaceList::STANDARD), quadrilateral-octagon almost normal
 * coordinates (NNormalSurfaceList::AN_QUAD_OCT), or standard almost
 * normal coordinates (NNormalSurfaceList::AN_STANDARD).  For almost
 * normal surfaces, we allow any number of octagons (including zero),
 * but we only allow at most one octagon \e type in the entire triangulation.
 * No coordinate systems other than these are supported.
 *
 * \warning Typically one should only use this class with \e one-vertex
 * triangulations (since otherwise, setting at least one triangle coordinate
 * to zero is not enough to rule out trivial vertex linking surfaces).
 * Of course there may be settings in which multiple vertices make sense
 * (for instance, in ideal triangulations with multiple cusps, or when
 * using ban constraints), and in such settings this class will still work
 * precisely as described.
 * 
 * \warning If you examine the type vector (for instance, by calling
 * dumpTypes()), be aware that this class merges the old types 0 and 1
 * together into a single branch of the search tree.  This means that
 * type 0 never appears, and that type 1 could indicate \e either positive
 * quadrilaterals in the first position, or else no quadrilaterals at all.
 *
 * \pre The parameters LPConstraint and BanConstraint must be subclasses of
 * LPConstraintBase and BanConstraintBase respectively.  See the
 * LPConstraintBase and BanConstraintBase class notes for further details.
 */
template <typename LPConstraint = LPConstraintNone,
          typename BanConstraint = BanNone>
class TreeSingleSoln : public TreeTraversal<LPConstraint, BanConstraint> {
    public:
        using TreeTraversal<LPConstraint, BanConstraint>::dumpTypes;

    protected:
        using TreeTraversal<LPConstraint, BanConstraint>::level_;
        using TreeTraversal<LPConstraint, BanConstraint>::lp_;
        using TreeTraversal<LPConstraint, BanConstraint>::lpSlot_;
        using TreeTraversal<LPConstraint, BanConstraint>::nextSlot_;
        using TreeTraversal<LPConstraint, BanConstraint>::nTets_;
        using TreeTraversal<LPConstraint, BanConstraint>::nTypes_;
        using TreeTraversal<LPConstraint, BanConstraint>::nVisited_;
        using TreeTraversal<LPConstraint, BanConstraint>::octLevel_;
        using TreeTraversal<LPConstraint, BanConstraint>::origTableaux_;
        using TreeTraversal<LPConstraint, BanConstraint>::tmpLP_;
        using TreeTraversal<LPConstraint, BanConstraint>::type_;
        using TreeTraversal<LPConstraint, BanConstraint>::typeOrder_;

        using TreeTraversal<LPConstraint, BanConstraint>::cancelled;
        using TreeTraversal<LPConstraint, BanConstraint>::feasibleBranches;
        using TreeTraversal<LPConstraint, BanConstraint>::
            nextUnmarkedTriangleType;
        using TreeTraversal<LPConstraint, BanConstraint>::setNext;

    private:
        int nextZeroLevel_;
            /**< The next level in the search tree at which we will force some
                 triangle coordinate to zero.  We use this to avoid vertex
                 links by dynamically reorganising the search tree as we run
                 to ensure that at least one relevant triangle coordinate is
                 set to zero at all stages and all levels of the search. */

    public:
        /**
         * Creates a new object for running the tree traversal / branching
         * algorithm to locate a non-trivial surface that satisfies the
         * chosen constraints.
         *
         * This constructor prepares the algorithm; in order to run the
         * algorithm you should call find(), which returns \c true or \c false
         * according to whether or not such a surface was found.
         *
         * \pre The given triangulation is non-empty.
         *
         * \pre Both the trianglation and the given coordinate system
         * adhere to any preconditions required by the template
         * parameters LPConstraint and BanConstraint.
         *
         * @param tri the triangulation in which we wish to search for a
         * non-trivial surface.
         * @param coords the normal or almost normal coordinate system in
         * which to work.  This must be one of NNormalSurfaceList::QUAD,
         * NNormalSurfaceList::STANDARD, NNormalSurfaceList::AN_QUAD_OCT, or
         * NNormalSurfaceList::AN_STANDARD.
         */
        inline TreeSingleSoln(NTriangulation* tri, int coords) :
                TreeTraversal<LPConstraint, BanConstraint>(tri, coords,
                    (coords == NNormalSurfaceList::AN_QUAD_OCT ||
                     coords == NNormalSurfaceList::AN_STANDARD ?
                     6 : 3) /* branches per quad */,
                    2 /* branches per triangle */,
                    false /* enumeration */),
                nextZeroLevel_(0) {
        }

        /**
         * Runs the tree traversal algorithm until it finds some non-trivial
         * surface that satisfies the chosen constraints, or else proves that
         * no such solution exists.
         *
         * Note that, if a solution is found, it will have a maximal
         * (but not necessarily maximum) set of zero coordinates, which
         * in some settings is enough to guarantee a vertex normal surface.
         * See the TreeSingleSoln class notes for details.
         *
         * If find() does return \c true, you can extract details of the
         * corresponding surface directly from this tree enumeration
         * object: for instance, you can dump the type vector using
         * dumpTypes(), or you can reconstruct the full surface using
         * buildSurface().  Be warned that this class defines the type
         * vector in an unusual way (see the TreeSingleSoln class notes
         * for details).  If you call buildSurface(), remember
         * to delete the surface once you are finished with it.
         *
         * \pre The algorithm has not yet been run, i.e., you have not called
         * find() before.
         *
         * @return \c true if we found a non-trivial solution as described
         * in the class notes, or \c false if no such solution exists.
         */
        bool find() {
            // This code is similar to next(), but makes some changes to
            // account for the facts that:
            // - we only need a single solution that satisfies our
            //   constraints, not all solutions;
            // - this single solution does not need to be a vertex solution.
            //
            // Amongst other things, we make the following key changes:
            // - there is no domination test (since we stop at the first
            //   solution);
            // - we insist that at least one unmarked triangle coordinate is
            //   zero at all stages of the search, so that we can avoid surfaces
            //   with trivial (and unwanted) vertex linking components.
            //
            // Furthermore, we only take three branches for each quadrilateral
            // type, not four.  We do this by combining types 0 and 1,
            // so the three branches are:
            //   ( >= 0,    0,    0 ) --> type 1
            //   (    0, >= 1,    0 ) --> type 2
            //   (    0,    0, >= 1 ) --> type 3

            // Start the search from the very beginning.
            //
            // Prepare the root node by finding an initial basis from
            // the original starting tableaux.
            lp_[0].initStart();
            BanConstraint::enforceBans(lp_[0]);

            ++nVisited_;
            if (! lp_[0].isFeasible())
                return false;

            // To kick off our vertex-link-avoiding regime, make
            // nextZeroLevel_ the first level in the search tree, and
            // choose an appropriate triangle type to branch on.  We will
            // return to the quadrilateral types as soon as some triangle
            // coordinate is safely constrained to zero.
            //
            // Since setNext() works on typeOrder_[level_ + 1], we must
            // temporarily set level_ = -1 before we call it.
            int useTriangle = nextUnmarkedTriangleType(nTets_);
            if (useTriangle < 0) {
                // There are no triangle types available to set to zero!
                return false;
            }
            level_ = -1;
            setNext(useTriangle);
            level_ = 0;

            // Run the search!
            unsigned idx; /* Index of the type we are currently choosing. */
            bool outOfRange;
            while (! cancelled()) {
                // We can safely return from this point.
#ifdef REGINA_TREE_TRACE
                dumpTypes(std::cout);
                std::cout << std::endl;
#endif
                // This code is based on TreeEnumeration::next().
                // For details on how it works, see the implementation of
                // TreeEnumeration::next(), which is very thoroughly documented.
                idx = typeOrder_[level_];

                // Check whether type_[idx] is out of range,
                // and if so then backtrack further up the tree.
                outOfRange = false;
                if (type_[idx] == 4) { // Quadrilateral column
                    if (octLevel_ < 0)
                        octLevel_ = level_; // Make it an octagon column
                    else
                        outOfRange = true;
                } else if (type_[idx] == 7) { // Octagon column
                    octLevel_ = -1;
                    outOfRange = true;
                } else if (idx >= nTets_ && type_[idx] == 2) // Triangle column
                    outOfRange = true;

                if (outOfRange) {
                    // Backtrack.
                    type_[idx] = 0;
                    --level_;
                    if (level_ < 0) {
                        // Out of options: there is no solution.
                        return false;
                    }
                    ++type_[typeOrder_[level_]];
                    continue;
                }

                ++nVisited_;

                // Skip the domination test and the zero test (which
                // are irrelevant here).  Note in particular that, if we
                // are searching for a surface with positive Euler
                // characteristic, the zero vector will not be a solution.

                // Prepare the tableaux for our new type at this level
                // of the tree.
                if (! type_[idx]) {
                    // This is the first time we have visited this node.
                    // Spin off clones and add some preliminary x_i = 0
                    // constraints as we go, reusing as much work as possible.
                    if (idx < nTets_) {
                        // We're working with a quadrilateral column.
                        //
                        // First, ignore type 0; instead step directly to
                        // type 1 (which merges the old types 0 and 1 together).
                        ++type_[idx];

                        // Now spin off clones for type 2 and 3, and
                        // overwrite the parent tableaux in-place for type 1.
                        //
                        // As we go, we add the following constraints:
                        // - type 1: x_{3k+1} = x_{3k+2} = 0
                        // - type 2: x_{3k+2} = 0
                        // - type 3: none
                        //
                        // If we allow octagons, we also spin off:
                        // - type 4: none
                        // - type 5: none
                        // - type 6: x_{3k+2} = 0
                        if (octLevel_ < 0) {
                            // We must support both quadrilaterals and octagons.
                            nextSlot_[level_ + 1] = nextSlot_[level_] + 5;

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 2)->initClone(
                                *lpSlot_[level_]);
                            (nextSlot_[level_] + 3)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 2);

                            nextSlot_[level_]->initClone(*lpSlot_[level_]);
                            (nextSlot_[level_] + 4)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 1);
                        } else {
                            // We only support quadrilaterals.
                            nextSlot_[level_ + 1] = nextSlot_[level_] + 2;

                            (nextSlot_[level_] + 1)->initClone(
                                *lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 2);

                            nextSlot_[level_]->initClone(*lpSlot_[level_]);

                            lpSlot_[level_]->constrainZero(3 * idx + 1);
                        }
                    } else {
                        // We're working with a triangle column.
                        //
                        // Since we are going to process type 0 right now,
                        // mark which tableaux we will use.
                        lpSlot_[level_ + 1] = lpSlot_[level_];

                        // Spin off a clone for type 1, and overwrite
                        // the parent tableaux in-place for type 0.
                        //
                        // As we go, we add the following constraints:
                        // - type 0: x_k = 0
                        // - type 1: none
                        nextSlot_[level_ + 1] = nextSlot_[level_] + 1;

                        nextSlot_[level_]->initClone(*lpSlot_[level_]);

                        lpSlot_[level_]->constrainZero(2 * nTets_ + idx);
                    }
                }

                if (type_[idx]) {
                    // Find the appropriate clone that we spun off earlier when
                    // type_[idx] was 0, and add any missing constraints.
                    if (idx < nTets_) {
                        // Quadrilateral columns (type is 1, 2 or 3,
                        // or 4, 5 or 6 if we allow octagons):
                        lpSlot_[level_ + 1] =
                            (type_[idx] == 1 ? lpSlot_[level_] :
                             nextSlot_[level_] + type_[idx] - 2);

                        switch (type_[idx]) {
                            // Nothing required for type 1, which already
                            // has all necessary constraints.  Since we merge
                            // types 0 and 1 together, there is not even a
                            // positivity constraint to add.
                            case 2:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 1);
                                break;
                            case 3:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainPositive(
                                    3 * idx + 2);
                                break;
                            case 4:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx + 1, 3 * idx + 2);
                                break;
                            case 5:
                                lpSlot_[level_ + 1]->constrainZero(
                                    3 * idx + 1);
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 2);
                                break;
                            case 6:
                                lpSlot_[level_ + 1]->constrainOct(
                                    3 * idx, 3 * idx + 1);
                                break;
                        }
                    } else {
                        // Triangle columns (type is 1):
                        lpSlot_[level_ + 1] = nextSlot_[level_];

                        lpSlot_[level_ + 1]->constrainPositive(
                            2 * nTets_ + idx);
                    }
                }

                // Now all our constraints are enforced, and we can
                // simply test the tableaux for feasibility.
                if (lpSlot_[level_ + 1]->isFeasible()) {
#ifdef REGINA_SURFACE_TRACE
                    {
                        dumpTypes(std::cout);
                        std::cout << " (" << idx << " -> " << (int)type_[idx]
                            << ")" << std::endl;

                        NNormalSurfaceVector* v =
                            new NNormalSurfaceVectorStandard(7 * nTets_);
                        lpSlot_[level_ + 1]->extractSolution(*v, type_);
                        NNormalSurface* f = new NNormalSurface(
                            origTableaux_.tri(), v);
                        std::cout << f->toString() << std::endl;
                        delete f;
                    }
#endif
                    if (level_ < nTypes_ - 1) {
                        // We pass the feasibility test, but we're
                        // not at a leaf node.
                        // Head deeper into the tree.

                        if (level_ == nextZeroLevel_) {
                            // We're avoiding vertex links, and we're
                            // still in the upper region of the search
                            // tree where we force triangles to be zero
                            // as early as possible in the search.
                            //
                            // Either we've just started setting the current
                            // triangle type to 0, or we've just finished
                            // setting the current triangle type to 0.
                            // Either way, we need to do some rearrangement
                            // of the search tree now.
                            if (! type_[idx]) {
                                // We've just started setting this triangle
                                // type to 0.
                                // This means that we're happy, and we
                                // can move onto quadrilaterals.
                                // Make sure the type we process next is the
                                // corresponding quadrilateral type.
                                setNext((idx - nTets_) / 4);
                            } else {
                                // We've just finished setting this
                                // triangle type to 0, and so we need to try
                                // setting a new triangle type to 0 instead.
                                // Find the next candidate triangle type,
                                // and make sure we process it next.
                                useTriangle = nextUnmarkedTriangleType(idx + 1);
                                if (useTriangle >= 0) {
                                    setNext(useTriangle);
                                    ++nextZeroLevel_;
                                } else {
                                    // There are no more triangles types left
                                    // to try setting to 0.
                                    // The only solutions remaining have all
                                    // unmarked triangle coordinates
                                    // positive, i.e., they involve multiples
                                    // of undesirable vertex links.  We don't
                                    // want such solutions,
                                    // so abandon the search now.
                                    return false;
                                }
                            }
                        } else if (typeOrder_[level_ + 1] < nTets_) {
                            // The next level is a quadrilateral type.
                            // See if we can't find a better quadrilateral
                            // type to branch on instead.  We will choose the
                            // quadrilateral type that branches into the
                            // fewest possible feasible subtrees.
                            // Here we measure the old types 0, 1, 2, 3 as
                            // four separate branches (not the three merged
                            // branches 0=1, 2, 3 that we use in the actual
                            // search).
                            int bestQuad = -1;
                            int minBranches = 5; // Greater than any soln.
                            int tmp;
                            for (int i = level_ + 1; i < nTypes_; ++i) {
                                if (typeOrder_[i] < nTets_) {
                                    // It's an available quad type.
#ifdef REGINA_NOOPT_MIN_FEASIBLE
                                    bestQuad = typeOrder_[i];
                                    break;
#else
                                    tmp = feasibleBranches(typeOrder_[i]);
                                    if (tmp < minBranches) {
                                        minBranches = tmp;
                                        bestQuad = typeOrder_[i];
                                        if (tmp == 0)
                                            break; // Can't get any better!
                                    }
#endif
                                }
                            }
                            if (bestQuad >= 0)
                                setNext(bestQuad);
                        }

                        ++level_;
                    } else {
                        // We pass the feasibility test, *and* we're at
                        // a leaf node.  This means we've found a solution!

                        // However: we have no guarantee that it's a
                        // vertex solution, since we merged quad types 0/1
                        // together.  Explicitly try setting each type 1
                        // quadrilateral coordinate to zero, and change
                        // the type to 0 if the system is still feasible.
                        // If the system is not feasible, call
                        // constrainPositive() to enact the change of
                        // variable so that we can reconstruct the
                        // surface correctly.
                        for (int i = 0; i < nTets_; ++i) {
                            if (type_[i] == 1) {
                                tmpLP_[0].initClone(*lpSlot_[level_ + 1]);
                                tmpLP_[0].constrainZero(3 * i);
                                // TODO: Use initClone() here instead?
                                if (tmpLP_[0].isFeasible()) {
                                    lpSlot_[level_ + 1]->constrainZero(3 * i);
                                    type_[i] = 0;
                                } else {
                                    lpSlot_[level_ + 1]->constrainPositive(
                                        3 * i);
                                }
                            }
                        }
                        return true;
                    }
                } else {
                    // We failed the feasibility test.
                    // Abandon this subtree, increment the type at the
                    // current level, and continue searching.
                    ++type_[idx];
                }
            }

            // If we ever make it out here, it's because some other
            // thread cancelled the search.  The result should be ignored.
            return false;
        }
};

/**
 * Output information on how to use this program, and then terminate the
 * entire program with return value 1.
 *
 * @param program the name of this program (typically argv[0]).
 */
void usage(const char* program) {
    std::cerr << "Usage:\n\n"
        << program << " ( -s | -q | -a | -o | -e | -3 ) [--] <isosig> ...\n\n"
        "    -s : Use standard coordinates (7n dimensions)\n"
        "    -q : Use quadrilateral coordinates (3n dimensions)\n"
        "    -a : Use standard almost normal coordinates (10n dimensions)\n"
        "    -o : Use quadrilateral-octagon coordinates (6n dimensions)\n"
        "    -e : Only detect 0-efficiency (not full vertex enumeration)\n"
        "    -3 : Detect an almost normal sphere (not full vertex enumeration)\n"
        "\n";
    ::exit(1);
}

/**
 * A variant of TreeTraversal::writeTypes() that also performs sanity
 * checking on the normal surface.
 */
template <typename LPConstraint, typename BanConstraint>
bool writeTypesAndVerify(
        const TreeEnumeration<>& tree, void* eqns) {
    /*
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    */

    NNormalSurface* s = tree.buildSurface();
    if (! tree.verify(s, static_cast<const NMatrixInt*>(eqns)))
        std::cout << "ERROR: Verification failed." << std::endl;

    return true;
}

/**
 * The main body of the program.
 *
 * For each isomorphism signature passed on the command line, we build
 * the corresponding triangulation and then run away to perform the tree
 * traversal algorithm.
 */
int main(int argc, char* argv[]) {
    char mode = 0;

    if (argc == 1)
        usage(argv[0]);

    bool found = false;
    bool noMoreSwitches = false;
    for (int i = 1; i < argc; ++i) {
        // Handle command-line arguments:
        if (argv[i][0] == '-' && ! noMoreSwitches) {
            if (found)
                usage(argv[0]);
            switch (argv[i][1]) {
                case '-':
                    noMoreSwitches = true;
                    break;
                case 'q':
                case 's':
                case 'a':
                case 'o':
                case 'e':
                case '3':
                    if (mode && mode != argv[i][1])
                        usage(argv[0]);
                    mode = argv[i][1];
                    break;
                default:
                    usage(argv[0]);
            }
            continue;
        }
        if (! mode)
            usage(argv[0]);

        // Process the next triangulation on the command line:
        NTriangulation* t = NTriangulation::fromIsoSig(argv[i]);
        if (t) {
            found = true;

            if (t->isOrientable())
                t->orient();

            std::cout << "PROCESSING: " << argv[i] << std::endl;
            if (mode == 's' || mode == 'q' || mode == 'a' || mode == 'o') {
                int coords = (mode == 's' ? NNormalSurfaceList::STANDARD :
                    mode == 'q' ? NNormalSurfaceList::QUAD :
                    mode == 'a' ? NNormalSurfaceList::AN_STANDARD :
                    NNormalSurfaceList::AN_QUAD_OCT);
                TreeEnumeration<> search(t, coords);
                if (search.constraintsBroken())
                    std::cerr << "ERROR: Constraints broken." << std::endl;
                else {
                    // Build the matching equations for sanity checking.
                    NMatrixInt* eqns = regina::makeMatchingEquations(t, coords);

                    search.run(&writeTypesAndVerify<LPConstraintNone,
                        BanNone>, eqns);
                    std::cout << "# solutions = " << search.nSolns()
                        << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited() 
                        << std::endl;

                    delete eqns;
                }
            } else {
                TreeSingleSoln<LPConstraintEuler>
                    search(t, mode == '3');
                if (search.constraintsBroken())
                    std::cerr << "ERROR: Constraints broken." << std::endl;
                else if (search.find()) {
                    std::cout << "Found non-trivial Euler > 0:" << std::endl;
                    search.dumpTypes(std::cout);
                    std::cout << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited()
                        << std::endl;

                    // Sanity checking:
                    NNormalSurface* s = search.buildSurface();
                    if (! search.verify(s))
                        std::cout << "ERROR: Verification failed." << std::endl;
                } else {
                    std::cout << "No non-trivial solution with Euler > 0"
                        << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited()
                        << std::endl;
                }
            }
        } else
            std::cerr << "ERROR: Could not reconstruct " << argv[i]
                << std::endl;
        delete t;
    }

    if (! found)
        usage(argv[0]);

    return 0;
}
