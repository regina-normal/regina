
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2016, Ben Burton                                   *
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

/*! \file enumerate/treelp.h
 *  \brief Linear programming code for tree traversal enumeration methods.
 */

#ifndef __TREELP_H
#ifndef __DOXYGEN
#define __TREELP_H
#endif

#include "maths/integer.h"
#include "maths/matrix.h"
#include "surfaces/normalcoords.h"
#include "triangulation/forward.h"
#include <algorithm>

/**
 * Define REGINA_VERIFY_LPDATA to check invariants as the algorithm runs.
 * This checking is slow and can increase the running time significantly.
 */
// #define REGINA_VERIFY_LPDATA

namespace regina {

class AngleStructureVector;
class NormalSurfaceVector;
class Ray;

template <typename> class MatrixIntDomain;
typedef MatrixIntDomain<Integer> MatrixInt;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * A matrix class for use with linear programming.
 *
 * This class is used in the tree traversal algorithms for enumerating
 * and locating vertex normal surfaces, as described in "A tree traversal
 * algorithm for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 * It is also used for locating a single strict angle structure, and for
 * enumerating all taut angle structures.
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
 * This matrix is stored in dense form.  All elements are of the integer class
 * \a IntType, which is supplied as a template argument.
 *
 * \pre The default constructor for the template class IntType must intialise
 * each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <typename IntType>
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
        inline LPMatrix();

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
        inline LPMatrix(unsigned rows, unsigned cols);

        /**
         * Destroys this matrix and all of the data it contains.
         *
         * You can safely destroy a matrix that is uninitialised
         * or only partially initialised (i.e., space has been reserved but
         * the matrix size is not set).
         */
        inline ~LPMatrix();

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
        inline void reserve(unsigned maxRows, unsigned maxCols);

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
        inline void initClone(const LPMatrix& clone);

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
        void initIdentity(unsigned size);

        /**
         * Returns a read-write reference to the given element of this
         * matrix.
         *
         * @param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * @param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline IntType& entry(unsigned row, unsigned col);

        /**
         * Returns a read-only reference to the given element of this
         * matrix.
         *
         * @param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * @param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline const IntType& entry(unsigned row, unsigned col) const;

        /**
         * Returns the number of rows in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * @return the number of rows.
         */
        inline unsigned rows() const;

        /**
         * Returns the number of columns in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * @return the number of columns.
         */
        inline unsigned columns() const;

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
        inline void swapRows(unsigned r1, unsigned r2);

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
                const IntType& div);

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
                unsigned dest, const IntType& srcCoeff, unsigned src);

        /**
         * Negates all elements in the given row of this matrix.
         *
         * @param row the row whose elements should be negated.
         * This must be between 0 and rows()-1 inclusive.
         */
        inline void negateRow(unsigned row);

        /**
         * Writes this matrix to the given output stream.
         * The output is "rough" and wasteful, and is intended for
         * debugging purposes only.  The precise output format is
         * subject to change in future versions of Regina.
         *
         * @param out the output stream to write to.
         */
        void dump(std::ostream& out) const;
};

/**
 * Used by LPInitialTableaux<LPConstraint> to store a single column of the
 * adjusted matching equation matrix in sparse form.
 *
 * See the LPInitialTableaux class notes for details on what the
 * "adjusted matching equation matrix" means.
 *
 * Specifically, an LPCol object stores the location of each +1 entry,
 * and the location of each -1 entry.  If some entry in the matrix
 * is greater than +1 or less than -1, we represent it using
 * multiple +1 or -1 entries in the same matrix location.
 *
 * For any additional rows that represent extra linear constraints,
 * we inherit the coefficients directly from LPConstraint::Coefficients.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint>
struct LPCol : public LPConstraint::Coefficients {
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
    inline LPCol();

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
    inline void push(unsigned row, int val);
};

/**
 * Stores an adjusted matrix of homogeneous linear matching equations based on
 * a given triangulation, in sparse form.  Typically these will be
 * the normal surface matching equations in some coordinate system,
 * or the angle structure equations.
 *
 * This class forms part of the tree traversal algorithms for enumerating
 * and locating normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 * It is also used for locating a single strict angle structure, and for
 * enumerating all taut angle structures.
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
 * There is also optional support for adding extra linear constraints (such as
 * a constraint on Euler characteristic for normal surfaces).  These extra
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
 * This class can only work in quadrilateral normal coordinates (NS_QUAD),
 * standard normal coordinates (NS_STANDARD), or angle structure coordinates
 * (NS_ANGLE).  No other coordinate systems are supported.
 *
 * \warning The implementation of this class relies on the fact that the
 * sum of <i>absolute values</i> of all coefficients in each column is
 * at most four (not counting the rows for any optional extra constraints).
 * If you are extending this class to work with more general matching
 * equation matrices, you may need to change the implementation accordingly.
 *
 * \pre The template parameter LPConstraint must be one of the subclasses of
 * LPConstraintBase.  See the LPConstraintBase class notes for further details.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint>
class LPInitialTableaux {
    private:
        const Triangulation<3>* tri_;
            /**< The underlying triangulation. */
        NormalCoords coords_;
            /**< The coordinate system used for the matrix of matching
                 equations; this must be one of NS_QUAD, NS_STANDARD,
                 or NS_ANGLE. */
        MatrixInt* eqns_;
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

        int scaling_;
            /**< In angle structure coordinates, the final coordinate is a
                 scaling coordinate used to projectivise the angle structure
                 polytope to become a polyhedral cone.  As a result, the
                 final coordinate is given the same coefficient \a scaling_
                 in every equation.  In other words, every entry in column
                 coordinateColumns()-1 of the matrix is equal to \a scaling_.
                 In all normal surface coordinate systems (which do not
                 need to be projectivised), \a scaling_ will be zero. */
        LPCol<LPConstraint>* col_;
            /**< An array of size \a cols_ that stores the individual columns
                 of this adjusted matrix in sparse form.  In angle structure
                 coordinates, the column col_[coordinateColumns()-1] will be
                 ignored, since this column of the matrix is described by the
                 \a scaling_ member instead. */

        int* columnPerm_;
            /**< A permutation of 0,...,cols_-1 that maps column numbers
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
         * matching equations; this must be one of NS_QUAD, NS_STANDARD,
         * or NS_ANGLE.
         * @param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the tableaux for an existence test
         * (such as searching for a non-trivial normal disc or sphere, or
         * a strict angle structure).
         */
        LPInitialTableaux(const Triangulation<3>* tri,
            NormalCoords coords, bool enumeration);

        /**
         * Destroys this matrix.
         */
        inline ~LPInitialTableaux();

        /**
         * Returns the underlying 3-manifold triangulation from which the
         * matching equations were derived.
         *
         * @return the underlying triangulation.
         */
        inline const Triangulation<3>* tri() const;

        /**
         * Returns the coordinate system that is used for the matrix of
         * matching equations.  This will be the same coordinate system
         * that was passed to the LPInitialTableaux constructor; in particular,
         * it will always be one of NS_QUAD, NS_STANDARD, or NS_ANGLE.
         *
         * @return the coordinate system.
         */
        NormalCoords coords() const;

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
        inline unsigned rank() const;

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
        inline unsigned columns() const;

        /**
         * Returns the number of columns that correspond to normal coordinates
         * or angle structure coordinates.  This is precisely the number of
         * columns in the original matrix of matching equations.
         *
         * @return the number of normal or angle structure coordinate columns.
         */
        inline unsigned coordinateColumns() const;

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
        inline bool constraintsBroken() const;

        /**
         * Returns the permutation that describes how the columns of
         * the matching equation matrix were reordered.  This permutation maps
         * column numbers in this adjusted matching equation matrix to
         * column numbers in the original (unmodified) matching equation
         * matrix that was originally derived from the triangulation.
         *
         * The permutation is returned as an array of columns() integers,
         * such that column \a i of this adjusted matrix corresponds to
         * column <tt>columnPerm()[i]</tt> of the original matrix.
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
         *   standard normal coordinate system, the remaining 4<i>n</i>
         *   triangle coordinate columns must appear last.
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
         * - The triangle coordinate columns (if we are working in standard
         *   normal coordinates) must likewise be grouped by tetrahedron,
         *   and these tetrahedra must appear in the same order as for the
         *   quadrilateral types.  In other words, for each
         *   \a i = 0,...,\a n-1, the quadrilateral columns
         *   3<i>i</i>, 3<i>i</i>+1 and 3<i>i</i>+2 and the triangle columns
         *   3<i>n</i>+4<i>i</i>, 3<i>n</i>+4<i>i</i>+1, 3<i>n</i>+4<i>i</i>+2 
         *   and 3<i>n</i>+4<i>i</i>+3 all refer to the same tetrahedron.
         *
         * - For angle structure coordinates, the constraints are analogous to
         *   those for quadrilateral coordinates: the angle coordinates
         *   must be grouped by tetrahedron and ordered by angle type,
         *   and the final scaling coordinate must remain last.
         *
         * @return details of the permutation describing how columns
         * were reordered.
         */
        inline const int* columnPerm() const;

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
        template <typename IntType>
        inline IntType multColByRow(const LPMatrix<IntType>& m, unsigned mRow,
                unsigned thisCol) const;

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
         * This routine is not used with angle structure coordinates.
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
        template <typename IntType>
        inline IntType multColByRowOct(const LPMatrix<IntType>& m,
                unsigned mRow, unsigned thisCol) const;

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
        template <typename IntType>
        void fillInitialTableaux(LPMatrix<IntType>& m) const;

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
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the ordering for an existence test
         * (such as searching for a non-trivial normal disc or sphere, or
         * a strict angle structure).
         */
        void reorder(bool enumeration);
};

/**
 * Stores an intermediate tableaux for the dual simplex method, and
 * contains all of the core machinery for using the dual simplex method.
 *
 * This class forms part of the tree traversal algorithms for enumerating
 * and locating normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801,
 * and "A fast branching algorithm for unknot recognition with
 * experimental polynomial-time behaviour", Burton and Ozlen, arXiv:1211.1079.
 * It is also used for locating a single strict angle structure, and for
 * enumerating all taut angle structures.
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
 * and Ozlen, we store the matrix M_beta^{-1}, where M is the original
 * matrix stored in the class LPInitialTableaux, and beta is the current
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
 * In the context of normal surfaces (not angle structures):
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
 * All tableaux elements are of the integer class \a IntType, which is
 * supplied as a template argument.  This same integer class will be
 * used as a template argument for \a LPConstraint.
 *
 * \pre The template parameter LPConstraint must be one of the subclasses of
 * LPConstraintBase.  See the LPConstraintBase class notes for further details.
 *
 * \pre The default constructor for the template class IntType must intialise
 * each new integer to zero.  The classes Integer and NativeInteger,
 * for instance, have this property.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <class LPConstraint, typename IntType>
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
                 Ozlen, this is the column vector M_beta^{-1} * b.
                 If \a rank_ is smaller than origTableaux_->rank() then
                 the "extra" entries rhs_[rank_, rank_+1, ...] may have
                 undefined values, and should simply be ignored. */
        LPMatrix<IntType> rowOps_;
            /**< The matrix of row operations that we apply to the
                 original starting tableaux, as described in the class
                 notes.  In the notation of Burton and Ozlen, this is
                 the matrix M_beta^{-1}.  This is a square matrix of
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
                 If we have not declared an octagon type (or if we are
                 working with angle structures), this is -1. */
        int octSecondary_;
            /**< If we have declared an octagon type, this stores the
                 second of the two quadrilateral columns that together
                 "represent" the octagon type, as described in the class
                 notes.  This is the quadrilateral column that we set to
                 zero and deactivate (as opposed to \a octPrimary_, which
                 we keep to count the number of octagons).
                 If we have not declared an octagon type (or if we are
                 working with angle structures), this variable is undefined. */

    public:
        /**
         * Constructs a new tableaux.  You \e must call reserve() before
         * doing anything else with this tableaux.
         */
        inline LPData();

        /**
         * Destroys this tableaux.  This is safe even if reserve() was
         * never called.
         */
        inline ~LPData();

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
        void reserve(const LPInitialTableaux<LPConstraint>* origTableaux);

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
        void initStart();

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
        void initClone(const LPData& parent);

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
        inline unsigned columns() const;

        /**
         * Returns the number of columns in this tableaux that correspond to
         * normal coordinates or angle structure coordinates.  This is
         * precisely the number of columns in the original matrix of
         * matching equations.
         *
         * @return the number of normal or angle structure coordinate columns.
         */
        inline unsigned coordinateColumns() const;

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
        inline bool isFeasible() const;

        /**
         * Determines whether the given variable is currently active.
         * See the LPData class notes for details.
         *
         * @param pos the index of the variable to query.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         */
        inline bool isActive(unsigned pos) const;

        /**
         * Returns the sign of the given variable under the current
         * basis.  This does \e not attempt to "undo" any changes of variable
         * caused by prior calls to constrainPositive() or constrainOct();
         * it simply tests the sign of the variable in the given column
         * of the tableaux in its current form.
         *
         * Specifically: if the given variable is inactive or non-basic,
         * this routine returns zero.  If the given variable is in the
         * basis, this routine returns the sign of the corresponding
         * integer on the right-hand side of the tableaux.
         *
         * @param pos the index of the variable to query.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         * @return the sign of the variable as described above;
         * this will be either 1, 0 or -1.
         */
        inline int sign(unsigned pos) const;

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
        void constrainZero(unsigned pos);

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
        void constrainPositive(unsigned pos);

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
         * This routine is not used with angle structure coordinates.
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
        void constrainOct(unsigned quad1, unsigned quad2);

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
        void dump(std::ostream& out) const;

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
         *   (This is why the output class is Ray and not Vector.)
         *
         * This routine is not used as an internal part of the tree traversal
         * algorithm; instead it is offered as a helper routine for
         * reconstructing the normal surfaces or angle structures that result.
         *
         * \pre The given vector \a v has been initialised to the zero vector
         * of length origTableaux_->columns().  Note that the Ray constructor
         * will automatically initialise all elements to zero as required.
         *
         * \pre No individual coordinate column has had more than one call
         * to either of constrainPositive() or constrainOct() (otherwise
         * the coordinate will not be correctly reconstructed).  Any
         * additional columns arising from LPConstraint are exempt from
         * this requirement.
         *
         * \tparam RayClass the class used to hold the output vector \a v.
         * This should either be Ray, or some other class that provides
         * analogous functions size(), setElement() and scaleDown().
         *
         * @param v the vector into which the values of the variables
         * will be placed.
         * @param type the type vector corresponding to the current state of
         * this tableaux, indicating which variables were previously fixed as
         * positive via calls to constrainPositive().  This is necessary
         * because LPData does not keep such historical data on its own.
         * As a special case, when extracting a strict angle structure
         * one may pass \a type = 0, in which case this routine will
         * assume that \e every coordinate was constrained as positive.
         */
        template <class RayClass>
        void extractSolution(RayClass& v, const char* type) const;

    private:
        /**
         * Returns the given entry in this tableaux.
         *
         * Since we do not store the full tableaux, this entry is
         * computed on the fly.  However, this computation is fast
         * because the computations use sparse vector multiplication.
         *
         * There is an alternate version of this function that avoids
         * creating spurious temporaries (which may help with performance).
         *
         * @param the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * @param the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * @return the requested entry in this tableaux.
         */
        inline IntType entry(unsigned row, unsigned col) const;

        /**
         * Sets \a ans to the given entry in this tableaux.
         *
         * Since we do not store the full tableaux, this entry is
         * computed on the fly.  However, this computation is fast
         * because the computations use sparse vector multiplication.
         *
         * There is an alternate version of this function that is more
         * natural (it returns its answer), but creates an additional
         * temporary variable (which may hinder performance).
         *
         * @param the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * @param the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * @param ans an integer that will be set to the requested entry
         * in this tableaux.
         */
        inline void entry(unsigned row, unsigned col, IntType& ans) const;

        /**
         * Determines the sign of the given entry in this tableaux.
         *
         * Since we do not store the full tableaux, the entry is
         * computed on the fly.  However, this computation is fast
         * because the computations use sparse vector multiplication.
         *
         * @param the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * @param the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * @return +1, -1 or 0 according to whether the requested entry
         * is positive, negative or zero.
         */
        inline int entrySign(unsigned row, unsigned col) const;

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
        void pivot(unsigned outCol, unsigned inCol);

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
         * In particular, it performs the entire Gauss-Jordan elimination
         * using the arbitrary-precision Integer class, so there is no need
         * to worry about the magnitudes of any intermediate matrix
         * entries that might appear during the process.  The final
         * row operation matrix will of course be copied into rowOps_
         * using the IntType class specified in the template arguments.
         *
         * \pre The current tableaux is precisely the original starting
         * tableaux; in particular, rhs_ is the zero vector, rowOps_ is the
         * identity matrix, and rank_ is precisely origTableaux_->rank().
         */
        void findInitialBasis();

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
        void makeFeasible();

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
        void makeFeasibleAntiCycling();

        /**
         * Runs some tests to ensure that the tableaux is in a
         * consistent state.  This is for use in debugging only, since
         * running these tests might have a severe impact on performance.
         *
         * If any tests fail, this routine reports the error and exits
         * the program immediately.
         */
        void verify() const;
};

/*@}*/

}

#include "enumerate/treeconstraint.h"

namespace regina {

extern template class REGINA_API LPMatrix<Integer>;
extern template class REGINA_API LPMatrix<NNativeLong>;

extern template class REGINA_API LPCol<LPConstraintNone>;
extern template class REGINA_API LPCol<LPConstraintEuler>;
extern template class REGINA_API LPCol<LPConstraintNonSpun>;

extern template class REGINA_API LPInitialTableaux<LPConstraintNone>;
extern template class REGINA_API LPInitialTableaux<LPConstraintEuler>;
extern template class REGINA_API LPInitialTableaux<LPConstraintNonSpun>;

extern template class REGINA_API LPData<LPConstraintNone, Integer>;
extern template REGINA_API void LPData<LPConstraintNone, Integer>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintNone, Integer>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintNone, NNativeLong>;
extern template REGINA_API void LPData<LPConstraintNone, NNativeLong>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintNone, NNativeLong>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintEuler, Integer>;
extern template REGINA_API void LPData<LPConstraintEuler, Integer>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintEuler, Integer>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintEuler, NNativeLong>;
extern template REGINA_API void LPData<LPConstraintEuler, NNativeLong>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintEuler, NNativeLong>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintNonSpun, Integer>;
extern template REGINA_API void LPData<LPConstraintNonSpun, Integer>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintNonSpun, Integer>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintNonSpun, NNativeLong>;
extern template REGINA_API void LPData<LPConstraintNonSpun, NNativeLong>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintNonSpun, NNativeLong>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

#ifdef INT128_AVAILABLE
extern template class REGINA_API LPData<LPConstraintNone, NativeInteger<16>>;
extern template REGINA_API void LPData<LPConstraintNone, NativeInteger<16>>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void REGINA_API LPData<LPConstraintNone, NativeInteger<16>>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintEuler, NativeInteger<16>>;
extern template REGINA_API void LPData<LPConstraintEuler, NativeInteger<16>>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintEuler, NativeInteger<16>>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;

extern template class REGINA_API LPData<LPConstraintNonSpun, NativeInteger<16>>;
extern template REGINA_API void LPData<LPConstraintNonSpun, NativeInteger<16>>::
    extractSolution<AngleStructureVector>(
    AngleStructureVector&, const char*) const;
extern template REGINA_API void LPData<LPConstraintNonSpun, NativeInteger<16>>::
    extractSolution<NormalSurfaceVector>(
    NormalSurfaceVector&, const char*) const;
#endif

// Inline functions for LPMatrix

template <typename IntType>
inline LPMatrix<IntType>::LPMatrix() : dat_(0) {
}

template <typename IntType>
inline LPMatrix<IntType>::LPMatrix(unsigned rows, unsigned cols) :
        dat_(new IntType[rows * cols]),
        rows_(rows), cols_(cols) {
}

template <typename IntType>
inline LPMatrix<IntType>::~LPMatrix() {
    delete[] dat_;
}

template <typename IntType>
inline void LPMatrix<IntType>::reserve(unsigned maxRows, unsigned maxCols) {
    dat_ = new IntType[maxRows * maxCols];
}

template <typename IntType>
inline void LPMatrix<IntType>::initClone(const LPMatrix& clone) {
    rows_ = clone.rows_;
    cols_ = clone.cols_;
    std::copy(clone.dat_, clone.dat_ + clone.rows_ * clone.cols_, dat_);
}

template <typename IntType>
inline void LPMatrix<IntType>::initIdentity(unsigned size) {
    // Don't fuss about optimising this, since we only call it once
    // in the entire tree traversal algorithm.
    rows_ = cols_ = size;

    unsigned r, c;
    for (r = 0; r < rows_; ++r)
        for (c = 0; c < cols_; ++c)
            entry(r, c) = (r == c ? 1 : long(0));
}

template <typename IntType>
inline IntType& LPMatrix<IntType>::entry(unsigned row, unsigned col) {
    return dat_[row * cols_ + col];
}

template <typename IntType>
inline const IntType& LPMatrix<IntType>::entry(unsigned row, unsigned col)
        const {
    return dat_[row * cols_ + col];
}

template <typename IntType>
inline unsigned LPMatrix<IntType>::rows() const {
    return rows_;
}

template <typename IntType>
inline unsigned LPMatrix<IntType>::columns() const {
    return cols_;
}

template <typename IntType>
inline void LPMatrix<IntType>::swapRows(unsigned r1, unsigned r2) {
    if (r1 != r2)
        std::swap_ranges(dat_ + r1 * cols_, dat_ + r1 * cols_ + cols_,
            dat_ + r2 * cols_);
}

template <typename IntType>
inline void LPMatrix<IntType>::negateRow(unsigned row) {
    IntType *p = dat_ + row * cols_;
    for (unsigned i = 0; i < cols_; ++p, ++i)
        p->negate();
}

// Inline functions for LPCol

template <class LPConstraint>
inline LPCol<LPConstraint>::LPCol() : nPlus(0), nMinus(0) {
}

template <class LPConstraint>
inline void LPCol<LPConstraint>::push(unsigned row, int val) {
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

// Inline functions for LPInitialTableaux

template <class LPConstraint>
inline LPInitialTableaux<LPConstraint>::~LPInitialTableaux() {
    delete eqns_;
    delete[] col_;
    delete[] columnPerm_;
}

template <class LPConstraint>
inline const Triangulation<3>* LPInitialTableaux<LPConstraint>::tri() const {
    return tri_;
}

template <class LPConstraint>
inline NormalCoords LPInitialTableaux<LPConstraint>::coords() const {
    return coords_;
}

template <class LPConstraint>
inline unsigned LPInitialTableaux<LPConstraint>::rank() const {
    return rank_;
}

template <class LPConstraint>
inline unsigned LPInitialTableaux<LPConstraint>::columns() const {
    return cols_;
}

template <class LPConstraint>
inline unsigned LPInitialTableaux<LPConstraint>::coordinateColumns() const {
    return eqns_->columns();
}

template <class LPConstraint>
inline bool LPInitialTableaux<LPConstraint>::constraintsBroken() const {
    return constraintsBroken_;
}

template <class LPConstraint>
inline const int* LPInitialTableaux<LPConstraint>::columnPerm() const {
    return columnPerm_;
}

template <class LPConstraint>
template <typename IntType>
inline IntType LPInitialTableaux<LPConstraint>::multColByRow(
        const LPMatrix<IntType>& m, unsigned mRow, unsigned thisCol) const {
    if (scaling_ && thisCol == coordinateColumns() - 1) {
        // Multiply the entire row by the scaling coefficient.
        IntType ans; // Initialised to zero.
        for (unsigned i = 0; i < rank_; ++i)
            ans += m.entry(mRow, i);
        ans *= scaling_;
        return ans;
    } else {
        // Just pick out individual coefficients using the sparse
        // representation of the column.
        IntType ans = col_[thisCol].innerProduct(m, mRow);

        unsigned i;
        for (i = 0; i < col_[thisCol].nPlus; ++i)
            ans += m.entry(mRow, col_[thisCol].plus[i]);
        for (i = 0; i < col_[thisCol].nMinus; ++i)
            ans -= m.entry(mRow, col_[thisCol].minus[i]);
        return ans;
    }
}

template <class LPConstraint>
template <typename IntType>
inline IntType LPInitialTableaux<LPConstraint>::multColByRowOct(
        const LPMatrix<IntType>& m, unsigned mRow, unsigned thisCol) const {
    // By the preconditions of this routine, we must be working in some normal
    // or almost normal coordinate system, and so there is no scaling
    // coordinate to worry about.
    IntType ans = col_[thisCol].innerProductOct(m, mRow);

    unsigned i;
    for (i = 0; i < col_[thisCol].nPlus; ++i)
        ans += m.entry(mRow, col_[thisCol].plus[i]);
    for (i = 0; i < col_[thisCol].nMinus; ++i)
        ans -= m.entry(mRow, col_[thisCol].minus[i]);
    return ans;
}

template <class LPConstraint>
template <typename IntType>
inline void LPInitialTableaux<LPConstraint>::fillInitialTableaux(
        LPMatrix<IntType>& m) const {
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

    if (scaling_)
        for (i = 0; i < rank_; ++i)
            m.entry(i, coordinateColumns() - 1) = scaling_;
}

// Template functions for LPData

template <class LPConstraint, typename IntType>
inline LPData<LPConstraint, IntType>::LPData() :
        rhs_(0), basis_(0), basisRow_(0) {
}

template <class LPConstraint, typename IntType>
inline LPData<LPConstraint, IntType>::~LPData() {
    delete[] rhs_;
    delete[] basis_;
    delete[] basisRow_;
}

template <class LPConstraint, typename IntType>
inline void LPData<LPConstraint, IntType>::reserve(
        const LPInitialTableaux<LPConstraint>* origTableaux) {
    origTableaux_ = origTableaux;
    rhs_ = new IntType[origTableaux->rank()];
    rowOps_.reserve(origTableaux->rank(), origTableaux->rank());
    basis_ = new int[origTableaux->rank()];
    basisRow_ = new int[origTableaux->columns()];
}

template <class LPConstraint, typename IntType>
inline unsigned LPData<LPConstraint, IntType>::columns() const {
    return origTableaux_->columns();
}

template <class LPConstraint, typename IntType>
inline unsigned LPData<LPConstraint, IntType>::coordinateColumns() const {
    return origTableaux_->coordinateColumns();
}

template <class LPConstraint, typename IntType>
inline bool LPData<LPConstraint, IntType>::isFeasible() const {
    return feasible_;
}

template <class LPConstraint, typename IntType>
inline bool LPData<LPConstraint, IntType>::isActive(unsigned pos) const {
    // If basisRow_[pos] < 0, the variable is active and non-basic.
    // If basisRow_[pos] > 0, the variable is active and basic.
    // If basisRow_[pos] == 0, then:
    //   - if rank_ > 0 and basis_[0] == pos, then the variable
    //     is active and basic;
    //   - otherwise the variable is not active.
    return ! (basisRow_[pos] == 0 &&
        (rank_ == 0 || basis_[0] != pos));
}

template <class LPConstraint, typename IntType>
inline int LPData<LPConstraint, IntType>::sign(unsigned pos) const {
    // If basisRow_[pos] < 0, the variable is active and non-basic.
    // If basisRow_[pos] > 0, the variable is active and basic.
    // If basisRow_[pos] == 0, then:
    //   - if rank_ > 0 and basis_[0] == pos, then the variable
    //     is active and basic;
    //   - otherwise the variable is not active.
    return ((basisRow_[pos] > 0 || (rank_ > 0 && basis_[0] == pos)) ?
        rhs_[basisRow_[pos]].sign() : 0);
}

template <class LPConstraint, typename IntType>
inline IntType LPData<LPConstraint, IntType>::entry(unsigned row, unsigned col)
        const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != col)
        return origTableaux_->multColByRow(rowOps_, row, col);
    else {
        IntType ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
        return ans;
    }
}

template <class LPConstraint, typename IntType>
inline void LPData<LPConstraint, IntType>::entry(unsigned row, unsigned col,
        IntType& ans) const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != col)
        ans = origTableaux_->multColByRow(rowOps_, row, col);
    else {
        ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
    }
}

template <class LPConstraint, typename IntType>
inline int LPData<LPConstraint, IntType>::entrySign(unsigned row, unsigned col)
        const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != col)
        return origTableaux_->multColByRow(rowOps_, row, col).sign();
    else {
        IntType ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
        return ans.sign();
    }
}

} // namespace regina

#endif

