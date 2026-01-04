
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file enumerate/treelp.h
 *  \brief Linear programming code for tree traversal enumeration methods.
 */

#ifndef __REGINA_TREELP_H
#ifndef __DOXYGEN
#define __REGINA_TREELP_H
#endif

#include "regina-core.h"
#include "concepts/array.h"
#include "concepts/maths.h"
#include "maths/integer.h"
#include "maths/matrix.h"
#include "surface/normalcoords.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"
#include "utilities/fixedarray.h"
#include <algorithm>

/**
 * Define REGINA_VERIFY_LPDATA to check invariants as the algorithm runs.
 * This checking is slow and can increase the running time significantly.
 */
// #define REGINA_VERIFY_LPDATA

namespace regina {

class AngleStructure;
class BanConstraintBase;
class BanNone;
class NormalSurface;
namespace detail { template <int, typename> class LPCol; }

/**
 * Indicates whether a linear constraint describes an equality or an inequality.
 * This is used with Regina's linear programming machinery.
 *
 * \ingroup enumerate
 */
enum class LPConstraintType {
    /**
     * Indicates a constraint that requires some linear function to be zero.
     *
     * A constraint of this type would typically be enforced by calling
     * LPData::constrainZero().
     */
    Zero = 0,
    /**
     * Indicates a constraint that requires some linear function to be
     * strictly positive.
     *
     * A constraint of this type would typically be enforced by calling
     * LPData::constrainPositive().
     */
    Positive = 1
};

/**
 * Represents a set of additional linear constraints that we can add to the
 * tableaux of normal surface or angle structure matching equations.
 * This concept is used with Regina's linear programming machinery.
 *
 * See LPConstraintAPI for further information.
 *
 * \ingroup enumerate
 */
template <typename T>
concept LPConstraint =
    requires(const Triangulation<3> tri, const NormalSurface surface,
            const AngleStructure structure, const NormalEncoding enc) {
        { T::constraints } -> ConstRefArrayOf<LPConstraintType>;
        typename T::Coefficient;
        requires SignedCppInteger<typename T::Coefficient>;
        { T::octAdjustment } -> std::same_as<const typename T::Coefficient&>;

        { T::addRows(
            (detail::LPCol<T::constraints.size(),
                typename T::Coefficient>*)(nullptr),
            tri, (const size_t*)(nullptr)) };
        { T::verify(surface) } -> std::same_as<bool>;
        { T::verify(structure) } -> std::same_as<bool>;
        { T::supported(enc) } -> std::same_as<bool>;
    };

/**
 * Represents a set of additional homogeneous linear equality constraints that
 * we can add to the tableaux of normal surface or angle structure matching
 * equations.  This concept is used with Regina's linear programming machinery.
 *
 * The concept LPSubspace essentially refines LPConstraint to ensure that the
 * additional linear constraints carve out a linear subspace of `R^n`.
 *
 * See LPConstraintAPI for further information.
 *
 * \ingroup enumerate
 */
template <typename T>
concept LPSubspace =
    LPConstraint<T> &&
    std::count(T::constraints.begin(), T::constraints.end(),
        LPConstraintType::Zero) == T::constraints.size();

/**
 * A type used to force certain normal coordinates or angle structure
 * coordinates to be zero.  This concept is used with Regina's linear
 * programming machinery when enumerating or locating normal surfaces or
 * angle structures.
 *
 * See BanConstraintBase for further information.
 *
 * \ingroup enumerate
 */
template <typename T>
concept BanConstraint =
    std::same_as<T, BanNone> ||
    std::derived_from<T, BanConstraintBase>;

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
 * inappropriate for general use.  If you just want a general-use
 * integer matrix class, use MatrixInt instead.
 *
 * It is _critical_ that, before using an LPMatrix, you reserve space
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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  However, due to the unusual create-reserve-initialise
 * procedure, it does not support copying (either by copy construction or
 * copy assignment).  Because of the move semantics, this class avoids deep
 * copies, even when passing or returning objects by value.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \python The template argument \a IntType is taken to be regina::Integer.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <ReginaInteger IntType>
class LPMatrix : public Output<LPMatrix<IntType>> {
    private:
        IntType* dat_;
            /**< The elements of this matrix as a single long array,
                 stored in row-major order.  This array stores as many
                 elements as were originally reserved, which might be
                 more than (but can never be less than) the current size
                 of the matrix according to \a rows_ and \a cols_. */
        size_t rows_;
            /**< The number of rows in this matrix. */
        size_t cols_;
            /**< The number of columns in this matrix. */

    public:
        /**
         * Creates an uninitialised matrix with no memory storage.
         *
         * You _must_ call reserve() and then either initClone() or
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
         * \param rows the number of rows in the new matrix.  This must
         * be strictly positive.
         * \param cols the number of columns in the new matrix.  This must
         * be strictly positive.
         */
        inline LPMatrix(size_t rows, size_t cols);

        /**
         * Moves the contents of the given matrix into this new matrix.
         * This is a fast (constant time) operation.
         *
         * If the given matrix is uninitialised, then this new matrix
         * will be uninitialised also.
         *
         * The matrix that is passed (\a src) will no longer be usable.
         *
         * \param src the matrix to move.
         */
        inline LPMatrix(LPMatrix&& src) noexcept;

        /**
         * Destroys this matrix and all of the data it contains.
         *
         * You can safely destroy a matrix that is uninitialised
         * or only partially initialised (i.e., space has been reserved but
         * the matrix size is not set).
         */
        inline ~LPMatrix();

        /**
         * Moves the contents of the given matrix into this matrix.
         * This is a fast (constant time) operation.
         *
         * If the given matrix is uninitialised, then this matrix
         * will become uninitialised also.
         *
         * The matrix that is passed (\a src) will no longer be usable.
         *
         * \param src the matrix to move.
         * \return a reference to this matrix.
         */
        inline LPMatrix& operator = (LPMatrix&& src) noexcept;

        /**
         * Swaps the contents of this and the given matrix.
         *
         * It does not matter if the two matrices have different sizes,
         * or if one or both is unintialised; if so then these properties
         * will be swapped also.
         *
         * \param other the matrix whose contents should be swapped with this.
         */
        inline void swap(LPMatrix& other) noexcept;

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
         * \param maxRows an upper bound on the number of rows that you
         * will need for this matrix.  This must be strictly positive.
         * \param maxCols an upper bound on the number of columns that
         * you will need for this matrix.  This must be strictly positive.
         */
        inline void reserve(size_t maxRows, size_t maxCols);

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
         * \param clone the matrix to copy.
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
         * \param size the number of rows, and also the number of
         * columns, that will be assigned to this matrix.
         * This must be strictly positive.
         */
        void initIdentity(size_t size);

        /**
         * Returns a read-write reference to the given element of this matrix.
         *
         * \python In general, to assign values to matrix elements you
         * should use the Python-only set() routine.  This entry() routine does
         * give read-write access to matrix elements in Python, but it does
         * not allow them to be set using the assignment operator.
         * In other words, code such as `matrix.entry(r, c).negate()`
         * will work, but `matrix.entry(r, c) = value` will not; instead
         * you will need to call `matrix.set(r, c, value)`.
         *
         * \param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * \param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline IntType& entry(size_t row, size_t col);

        /**
         * Returns a read-only reference to the given element of this matrix.
         *
         * \param row the row of the requested element.  This must be
         * between 0 and rows()-1 inclusive.
         * \param col the column of the requested element.  This must be
         * between 0 and columns()-1 inclusive.
         */
        inline const IntType& entry(size_t row, size_t col) const;

#ifdef __APIDOCS
        /**
         * Python-only routine that sets the given element of this matrix.
         *
         * \nocpp For C++ users, entry() is used for both reading and
         * writing: just write `entry(row, column) = value`.
         *
         * \python In general, to assign values to matrix elements you
         * should use the syntax `matrix.set(row, column, value)`.  The entry()
         * routine does give read-write access to matrix elements in Python,
         * but it does not allow them to be set using the assignment operator.
         * In other words, code such as `matrix.entry(r, c).negate()`
         * will work, but `matrix.entry(r, c) = value` will not.
         *
         * \param row the row of the entry to set; this must be between
         * 0 and rows()-1 inclusive.
         * \param col the column of the entry to set; this must be
         * between 0 and columns()-1 inclusive.
         * \param value the new entry to place in the given row and column.
         */
        void set(size_t row, size_t col, const IntType& value);
#endif

        /**
         * Returns the number of rows in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * \return the number of rows.
         */
        inline size_t rows() const;

        /**
         * Returns the number of columns in this matrix.  This relates to
         * the currently assigned matrix size, not the total amount of
         * memory that was originally reserved.
         *
         * \return the number of columns.
         */
        inline size_t columns() const;

        /**
         * Determines whether this and the given matrix are equal.
         *
         * Two matrices are equal if and only if their dimensions are the same,
         * and the corresponding elements of each matrix are equal.
         *
         * It is safe to compare matrices of different dimensions, and
         * it is safe to compare matrices that might not yet be initialised.
         * Two uninitialised matrices will compare as equal.
         *
         * \param other the matrix to compare with this.
         * \return \c true if and only if the two matrices are equal.
         */
        inline bool operator == (const LPMatrix& other) const;

        /**
         * Swaps the two given rows of this matrix.
         * The two arguments \a r1 and \a r2 may be equal (in which case
         * the matrix will be left unchanged).
         *
         * \param r1 the index of the first row to swap.  This must be
         * between 0 and rows()-1 inclusive.
         * \param r2 the index of the second row to swap.  This must be
         * between 0 and rows()-1 inclusive.
         */
        inline void swapRows(size_t r1, size_t r2);

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
         * \param destCoeff the coefficient applied to row \a dest in
         * the linear combination.
         * \param dest the index of the row to replace.  This must be
         * between 0 and rows()-1 inclusive.
         * \param srcCoeff the coefficient applied to row \a src in
         * the linear combination.
         * \param src the index of the other row used in this linear
         * combination.  This must be between 0 and rows()-1 inclusive.
         * \param div the integer to divide the final row by.  This must
         * be non-zero.
         */
        void combRow(const IntType& destCoeff, size_t dest,
                const IntType& srcCoeff, size_t src,
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
         * \param destCoeff the coefficient applied to row \a dest in
         * the linear combination.
         * \param dest the index of the row to replace.  This must be
         * between 0 and rows()-1 inclusive.
         * \param srcCoeff the coefficient applied to row \a src in
         * the linear combination.
         * \param src the index of the other row used in this linear
         * combination.  This must be between 0 and rows()-1 inclusive.
         * \return the positive gcd that row \a dest was scaled down by,
         * or 0 if row \a dest is entirely zero.
         */
        IntType combRowAndNorm(const IntType& destCoeff,
                size_t dest, const IntType& srcCoeff, size_t src);

        /**
         * Negates all elements in the given row of this matrix.
         *
         * \param row the row whose elements should be negated.
         * This must be between 0 and rows()-1 inclusive.
         */
        inline void negateRow(size_t row);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
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

        // Mark this class as non-copyable:
        LPMatrix(const LPMatrix&) = delete;
        LPMatrix& operator = (const LPMatrix&) = delete;
};

/**
 * Swaps the contents of the given matrices.
 *
 * This global routine simply calls LPMatrix<IntType>::swap(); it is provided
 * so that LPMatrix<IntType> meets the C++ Swappable requirements.
 *
 * \param a the first matrix whose contents should be swapped.
 * \param b the second matrix whose contents should be swapped.
 *
 * \ingroup enumerate
 */
template <ReginaInteger IntType>
inline void swap(LPMatrix<IntType>& a, LPMatrix<IntType>& b) noexcept;

namespace detail {

/**
 * Internal class used by `LPInitialTableaux` to store a single column of the
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
 * For any additional rows that represent extra linear constraints (typically
 * described by an LPConstraint type), an LPCol object stores the coefficients
 * for those rows explicitly.  The number of such rows is assumed to be very
 * small (at the time of writing, this is no larger than 2 for all of Regina's
 * constraint classes).
 *
 * These column objects have full value semantics.  Assuming the number of
 * rows from any extra linear constraints is very small (as noted above), these
 * column objects are cheap to move, cheap to copy, and cheap to swap via
 * std::swap().
 *
 * \warning End users should not use this class at all.  It was designed purely
 * for use as internal storage for LPInitialTableaux, and at some point in the
 * future this class will most likely become private to LPInitialTableaux.
 *
 * \apinotfinal
 *
 * \nopython
 *
 * \tparam nConstraints the number of additional rows that represent extra
 * linear constraints (as typically described by an LPConstraint type).
 *
 * \tparam Coefficient the integer type used for coefficients in these extra
 * linear constraints.
 *
 * \ingroup detail
 */
template <int nConstraints, typename Coefficient>
struct LPCol {
    int nPlus;
        /**< The total number of +1 entries in this column. */
    size_t plus[4];
        /**< The rows containing these +1 entries, in any order.
             The same row may appear in this list more than once
             (indicating a +2, +3 or +4 entry in the matrix). */
    int nMinus;
        /**< The total number of -1 entries in this column. */
    size_t minus[4];
        /**< The rows containing these -1 entries, in any order.
             The same row may appear in this list more than once
             (indicating a -2, -3 or -4 entry in the matrix). */

    std::array<Coefficient, nConstraints> extra;
        /**< The coefficients for this column that appear in each extra
             linear constraint (typically described by an LPConstraint type). */

    /**
     * Initialises an empty column, with all entries equal to zero.
     */
    inline LPCol() : nPlus(0), nMinus(0) {
        if constexpr (nConstraints > 0) {
            // I'm not sure how well the compiler will optimise this for us.
            // Hard-code the first couple of cases without using std::fill().
            if constexpr (nConstraints == 1) {
                extra[0] = 0;
            } else if constexpr (nConstraints == 2) {
                extra[0] = 0;
                extra[1] = 0;
            } else
                std::fill(extra.begin(), extra.end(), 0);
        }
    }

    /**
     * Creates a new copy of the given column.
     */
    LPCol(const LPCol&) = default;

    /**
     * Moves the contents of the given column into this new column.
     *
     * This move operation is not marked \c noexcept, since this depends
     * upon the underlying \a Coefficient type.
     *
     * After this operation, the given column will no longer be usable.
     */
    LPCol(LPCol&&) = default;

    /**
     * Sets this to be a copy of the given column.
     *
     * \return a reference to this column.
     */
    LPCol& operator = (const LPCol&) = default;

    /**
     * Moves the contents of the given column into this column.
     *
     * This move operation is not marked \c noexcept, since this depends
     * upon the underlying \a Coefficient type.
     *
     * After this operation, the given column will no longer be usable.
     *
     * \return a reference to this column.
     */
    LPCol& operator = (LPCol&&) = default;

    /**
     * Adds the given entry in the given row to this column.
     *
     * \pre No entry in the given row has been added to this column
     * yet.
     *
     * \pre The sum of absolute values of all entries in this
     * column must never exceed 4.
     *
     * \param row the row containing the given value.
     * \param val the value at this location in the matrix.
     */
    inline void push(size_t row, int val) {
        #ifdef REGINA_VERIFY_LPDATA
        if ((val > 0 && val + nPlus > 4) || (val < 0 && val - nMinus < -4))
            throw ImpossibleScenario("Bad matrix");
        #endif
        for (; val > 0; --val)
            plus[nPlus++] = row;
        for (; val < 0; ++val)
            minus[nMinus++] = row;
    }
};

} // namespace detail

/**
 * Indicates which broad class of vector encodings a particular tableaux
 * is designed to work with.  This type is used by Regina's linear
 * programming machinery, and in particular by the LPInitialTableaux class.
 *
 * By "broad class of vector encodings", we allow only three options:
 *
 * - _standard_ encodings, which cover all normal surface encodings
 *   that include triangle coordinates, and where the tableaux holds
 *   triangle and quadrilateral columns but nothing else;
 *
 * - _quad_ encodings, which cover all normal surface encodings that do not
 *   include triangle coordinates, and where the tableaux holds
 *   quadrilateral columns but nothing else;
 *
 * - _angle_ encodings, which cover angle structure encodings, and
 *   where the tableaux holds angle columns as well as a single scaling
 *   column.
 *
 * When working with almost normal coordinate systems, we represent octagons as
 * pairs of intersecting quadrilaterals; see the LPData class notes for more
 * information on how this works.  This means, for example, that the coordinate
 * system NormalCoords::AlmostNormal will fall under the class of standard
 * encodings, and NormalCoords::QuadOct will fall under the class of quad
 * encodings.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup enumerate
 */
class LPSystem : public ShortOutput<LPSystem> {
    private:
        /**
         * The three possible classes of vector encodings.
         */
        enum class System {
            Standard = 1,
            Quad = 2,
            Angle = 4
        } system_;

    public:
        /**
         * Identifies which class of vector encodings the given encoding
         * falls into.
         *
         * \param enc a normal surface vector encoding; this may be any
         * valid NormalEncoding object, including the special angle
         * structure encoding.
         */
        constexpr LPSystem(NormalEncoding enc) : system_(
                enc.storesAngles() ? System::Angle :
                enc.storesTriangles() ? System::Standard :
                System::Quad) {
        }
        /**
         * Creates a new copy of the given class of vector encodings.
         */
        constexpr LPSystem(const LPSystem&) = default;
        /**
         * Sets this to be a copy of the given class of vector encodings.
         *
         * \return a reference to this object.
         */
        constexpr LPSystem& operator = (const LPSystem&) = default;
        /**
         * Determines whether this and the given object represent the
         * same class of vector encodings.
         *
         * \param other the object to compare with this.
         * \return \c true if and only if both objects represent the same
         * class of encodings.
         */
        constexpr bool operator == (const LPSystem& other) const {
            return system_ == other.system_;
        }
        /**
         * Identifies whether this is one of the two classes of
         * encodings that represent normal or almost normal surfaces.
         *
         * This will be \c true if and only if either standard() or quad()
         * returns \c true.
         *
         * Exactly one of normal() and angle() will return \c true.
         *
         * \return \c true if this is a class of normal or almost normal
         * surface encodings.
         */
        constexpr bool normal() const {
            return (system_ != System::Angle);
        };
        /**
         * Identifies whether this is the class of encodings that represent
         * angle structures.
         *
         * Exactly one of normal() and angle() will return \c true.
         *
         * \return \c true if this is the class of angle encodings.
         */
        constexpr bool angle() const {
            return (system_ == System::Angle);
        }
        /**
         * Identifies whether this is the class of standard encodings.
         *
         * Exactly one of standard(), quad() and angle() will return \c true.
         *
         * \return \c true if this is the class of standard encodings.
         */
        constexpr bool standard() const {
            return (system_ == System::Standard);
        }
        /**
         * Identifies whether this is the class of quad encodings.
         *
         * Exactly one of standard(), quad() and angle() will return \c true.
         *
         * \return \c true if this is the class of quad encodings.
         */
        constexpr bool quad() const {
            return (system_ == System::Quad);
        }
        /**
         * Returns the number of coordinate columns that a tableaux will use
         * for this class of vector encodings, with respect to a particular
         * triangulation.
         *
         * \param nTet the number of tetrahedra in the triangulation.
         * \return the corresponding number of coordinate columns in the
         * tableaux.
         */
        constexpr size_t coords(size_t nTet) const {
            switch (system_) {
                case System::Standard: return 7 * nTet;
                case System::Quad: return 3 * nTet;
                case System::Angle: return 3 * nTet + 1;
                default: return 0;
            }
        }

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            switch (system_) {
                case System::Standard: out << "standard"; return;
                case System::Quad: out << "quad"; return;
                case System::Angle: out << "angle"; return;
                default: out << "invalid"; return;
            }
        }
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
 * constraints are supplied by the template parameter \a Constraint, and will
 * generate additional rows and columns (one for each element of the array
 * `Constraint::constraints`).  These additional rows and columns will be
 * used by the additional variables that evaluate the corresponding linear
 * functions.  If there are no additional constraints, simply use the
 * template parameter LPConstraintNone.
 *
 * For some \a Constraint template arguments, Regina may discover at runtime
 * that it is impossible to add the corresponding extra linear constraints
 * (e.g., the constraints might require some preconditions on the underlying
 * triangulation that are not met).  In this case, the LPInitialTableaux
 * class constructor will throw an exception, as noted in the constructor
 * documentation below.
 *
 * This class is optimised for working with _columns_ of the matrix
 * (in particular, multiplying columns of this matrix by rows of some
 * other matrix).
 *
 * This class works with a broad class of vector encodings for normal
 * surfaces or angle structures, as described by the LPSystem class,
 * and within that broad class it does not know _which_ particular
 * encoding or underlying coordinate system is being used.  In particular,
 * the matching equations it uses will _always_ be one of the standard
 * tri-quad normal matching equations (if LPSystem::standard() is \c true),
 * the quad normal matching equations (if LPSystem::quad() is \c true),
 * or the homogeneous angle equations (if LPSystem::angles() is true).
 * If you need to add extra matching equations beyond these, use the
 * Constraint template argument as outlined above.  If you need to support
 * more exotic vector encodings (e.g., for octagonal almost normal surfaces),
 * you will need to find a way to represent it using one of these three broad
 * classes; see the LPData class notes for how this is done with octagons.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \warning The implementation of this class relies on the fact that the
 * sum of _absolute values_ of all coefficients in each column is
 * at most four (not counting the rows for any optional extra constraints).
 * If you are extending this class to work with more general matching
 * equation matrices, you may need to change the implementation accordingly.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \python This is a heavily templated class; nevertheless, many variants
 * are now made available to Python users.  Each class name is of the form
 * LPInitialTableaux_<i>Constraint</i>, where the suffix \a Constraint
 * is an abbreviated version of the Constraint template parameter;
 * this suffix is omitted entirely for the common case LPConstraintNone.
 * An example of such a Python class name is \c LPInitialTableaux_NonSpun.
 * You are encouraged to look through the Regina namespace to see which
 * constraint classes are supported under Python.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <LPConstraint Constraint>
class LPInitialTableaux : public Output<LPInitialTableaux<Constraint>> {
    private:
        using Col = detail::LPCol<Constraint::constraints.size(),
                typename Constraint::Coefficient>;
            /**< The type used to store an individual column of this adjusted
                 matrix in sparse form. */

        const Triangulation<3>* tri_;
            /**< The underlying triangulation.  This is stored by pointer
                 in order to support assignment; it must never be \c null. */
        LPSystem system_;
            /**< The broad class of vector encodings that this tableaux
                 works with. */
        MatrixInt eqns_;
            /**< The adjusted matching equation matrix, in dense form.
                 The precise adjustments that we make are described in the
                 LPInitialTableaux class notes. */
        size_t rank_;
            /**< The rank of this tableaux, taking into account any additional
                 constraints from the template parameter Constraint. */
        size_t cols_;
            /**< The number of columns in this tableaux, taking into account
                 any additional constraints from the template parameter
                 Constraint. */

        int scaling_;
            /**< In angle structure coordinates, the final coordinate is a
                 scaling coordinate used to projectivise the angle structure
                 polytope to become a polyhedral cone.  As a result, the
                 final coordinate is given the same coefficient \a scaling_
                 in every equation.  In other words, every entry in column
                 coordinateColumns()-1 of the matrix is equal to \a scaling_.
                 In all normal surface coordinate systems (which do not
                 need to be projectivised), \a scaling_ will be zero. */
        FixedArray<Col> col_;
            /**< An array of size \a cols_ that stores the individual columns
                 of this adjusted matrix in sparse form.  In angle structure
                 coordinates, the column col_[coordinateColumns()-1] will be
                 ignored, since this column of the matrix is described by the
                 \a scaling_ member instead. */

        FixedArray<size_t> columnPerm_;
            /**< A permutation of 0,...,cols_-1 that maps column numbers
                 in the adjusted matrix to column numbers in the original
                 (unmodified) matrix of matching equations that was originally
                 derived from the triangulation.  See columnPerm() for
                 more details on what this permutation means and what
                 constraints it must adhere to. */

    public:
        /**
         * Construts this adjusted sparse matrix of matching equations.
         *
         * Note that LPInitialTableaux does not copy the given triangulation;
         * it merely keeps a reference to it.  The triangulation should
         * not change during the lifespan of this object.
         *
         * \pre The given triangulation is non-empty.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error which should have been preventable with the right checks
         * in advance.  Such exceptions are generated by the \a Constraint
         * class, and so you should consult the class documentation for your
         * chosen \a Constraint template argument to see if this is a
         * possibility.
         *
         * \exception InvalidArgument It was not possible to add the extra
         * constraints from the LPConstraint template argument, due to an
         * error that was "genuinely" unforseeable.  Again, such exceptions
         * are generated by your chosen \a Constraint class, and you should
         * consult its documentation to see if this is a possibility.
         *
         * \param tri the underlying 3-manifold triangulation.
         * \param enc the normal surface vector encoding that we are using
         * for our enumeration task.  This may be any valid NormalEncoding
         * object, including the special angle structure encoding.
         * \param enumeration \c true if we should optimise the tableaux
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the tableaux for an existence test
         * (such as searching for a non-trivial normal disc or sphere, or
         * a strict angle structure).
         */
        LPInitialTableaux(const Triangulation<3>& tri, NormalEncoding enc,
            bool enumeration = true);

        /**
         * Creates a new copy of the given matrix.
         */
        LPInitialTableaux(const LPInitialTableaux&) = default;

        /**
         * Moves the contents of the given matrix into this new matrix.
         * This is a fast (constant time) operation.
         *
         * The matrix that is passed will no longer be usable.
         */
        LPInitialTableaux(LPInitialTableaux&& src) noexcept = default;

        // Disable copy assignment, since FixedArray does not support this.
        LPInitialTableaux& operator = (const LPInitialTableaux&) = delete;

        /**
         * Moves the contents of the given matrix into this matrix.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given matrix have different
         * sizes and/or work with different vector encodings; if so then these
         * properties will be moved across also.
         *
         * The matrix that is passed will no longer be usable.
         *
         * \return a reference to this matrix.
         */
        LPInitialTableaux& operator = (LPInitialTableaux&&) noexcept = default;

        /**
         * Swaps the contents of this and the given matrix.
         *
         * It does not matter if the two matrices have different sizes,
         * and/or work with different vector encodings; if so then these
         * properties will be swapped also.
         *
         * \param other the matrix whose contents should be swapped with this.
         */
        inline void swap(LPInitialTableaux& other) noexcept;

        /**
         * Returns the underlying 3-manifold triangulation from which the
         * matching equations were derived.
         *
         * \return the underlying triangulation.
         */
        inline const Triangulation<3>& tri() const;

        /**
         * Returns the broad class of vector encodings that this tableaux
         * works with.  This broad class is deduced from the vector encoding
         * that was passed to the class constructor, and it completely
         * determines which matching equations were generated as a result.
         *
         * See the LPInitialTableaux class notes for more information on
         * these three broad classes and how they affect the tableaux.
         *
         * \return the class of vector encodings used by this tableaux.
         */
        LPSystem system() const;

        /**
         * Returns the rank of this matrix.
         *
         * Note that, if we are imposing extra constraints through the
         * LPConstraint template parameter, then there will be extra variables
         * to enforce these, and so the rank will be larger than the rank of
         * the original matching equation matrix.
         *
         * \return the matrix rank.
         */
        inline size_t rank() const;

        /**
         * Returns the number of columns in this matrix.
         *
         * Note that, if we are imposing extra constraints through the
         * LPConstraint template parameter, then there will be extra variables
         * to enforce these, and so the number of columns will be larger than
         * in the original matching equation matrix.
         *
         * \return the number of columns.
         */
        inline size_t columns() const;

        /**
         * Returns the number of columns that correspond to normal coordinates
         * or angle structure coordinates.  This is precisely the number of
         * columns in the original matrix of matching equations.
         *
         * \return the number of normal or angle structure coordinate columns.
         */
        inline size_t coordinateColumns() const;

        /**
         * Returns the permutation that describes how the columns of
         * the matching equation matrix were reordered.  This permutation maps
         * column numbers in this adjusted matching equation matrix to
         * column numbers in the original (unmodified) matching equation
         * matrix that was originally derived from the triangulation.
         *
         * The permutation is returned as an array of columns() integers,
         * such that column \a i of this adjusted matrix corresponds to
         * column `columnPerm()[i]` of the original matrix.
         *
         * If you are imposing additional constraints through the
         * LPConstraint template parameter, then the corresponding extra
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
         * \python This routine returns a Python list.
         *
         * \return details of the permutation describing how columns
         * were reordered.
         */
        inline const size_t* columnPerm() const;

        /**
         * Computes the inner product of (i) the given row of the given
         * matrix with (ii) the given column of this matrix.
         *
         * This routine is optimised to use the sparse representation of
         * columns in this matrix.
         *
         * \pre The given matrix \a m has precisely rank() columns.
         *
         * \param m the matrix whose row we will use in the inner product.
         * \param mRow the row of the matrix \a m to use in the inner product.
         * \param thisCol the column of this matrix to use in the inner product.
         * \return the resulting inner product.
         */
        template <ReginaInteger IntType>
        inline IntType multColByRow(const LPMatrix<IntType>& m, size_t mRow,
                size_t thisCol) const;

        /**
         * A variant of multColByRow() that takes into account any adjustments
         * to the tableaux that are required when this is a quadrilateral
         * column being used to represent an octagon type.
         *
         * The LPData class offers support for octagonal almost normal
         * surfaces, in which exactly one tetrahedron is allowed to have
         * exactly one octagon type.  We represent such an octagon as a
         * _pair_ of incompatible quadrilaterals within the same tetrahedron.
         * See the LPData class notes for details on how this works.
         *
         * In some settings where we are using additional constraints
         * through the LPConstraint template parameter, these extra
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
         * (via the information given by Constraint::octAdjustment)
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
         * \param m the matrix whose row we will use in the adjusted
         * inner product.
         * \param mRow the row of the matrix \a m to use in the adjusted
         * inner product.
         * \param thisCol the column of this matrix to use in the adjusted
         * inner product.
         * \return the resulting adjusted inner product.
         */
        template <ReginaInteger IntType>
        inline IntType multColByRowOct(const LPMatrix<IntType>& m,
                size_t mRow, size_t thisCol) const;

        /**
         * Fills the given matrix with the contents of this matrix.
         * This effectively copies this sparse
         * but highly specialised matrix representation into a dense
         * but more flexible matrix representation.
         *
         * \pre The given matrix has already been initialised to size
         * rank() * columns(), and all of its elements have already been
         * set to zero.  Note that this can all be arranged by calling
         * the constructor LPMatrix::LPMatrix(size_t, size_t).
         *
         * \param m the matrix to fill.
         */
        template <ReginaInteger IntType>
        void fillInitialTableaux(LPMatrix<IntType>& m) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
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
         * added from the LPConstraint template parameter; that is, the
         * rows of the matrix are just the matching equations.  However,
         * we do already have the extra placeholder columns for the new
         * variables that correspond to these extra constraint(s).
         *
         * \param enumeration \c true if we should optimise the ordering
         * for a full enumeration of vertex surfaces or taut angle structures,
         * or \c false if we should optimise the ordering for an existence test
         * (such as searching for a non-trivial normal disc or sphere, or
         * a strict angle structure).
         */
        void reorder(bool enumeration);
};

/**
 * Swaps the contents of the given matrices.
 *
 * This global routine simply calls LPInitialTableaux<Constraint>::swap();
 * it is provided so that LPInitialTableaux<Constraint> meets the C++
 * Swappable requirements.
 *
 * \param a the first matrix whose contents should be swapped.
 * \param b the second matrix whose contents should be swapped.
 *
 * \ingroup enumerate
 */
template <LPConstraint Constraint>
inline void swap(LPInitialTableaux<Constraint>& a,
    LPInitialTableaux<Constraint>& b) noexcept;

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
 *   inequality x_i ≥ 1 (instead of the strict inequality x_i > 0, which
 *   is more difficult to enforce).  We enforce this constraing using a
 *   change of variable: we replace the variable x_i with (x_i - 1),
 *   which is then constrained to be non-negative as usual.  The new
 *   variable (x_i - 1) uses the same column in the tableaux (we perform
 *   the actual change of variable by editing the tableaux itself using
 *   column operations).  Be warned: as a result, when we arrive at a
 *   final solution and collect the values of the variables, we must
 *   remember to _increment_ the values of any such variables by one.
 *
 * We do not store the full tableaux (which is dense and slow to work
 * with).  Instead we store the matrix of row operations that were
 * applied to the original starting tableaux (in the notation of Burton
 * and Ozlen, we store the matrix M_β^{-1}, where M is the original
 * matrix stored in the class LPInitialTableaux, and β is the current
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
 * parameter Constraint.  If there are no such constraints, simply use
 * the template parameter LPConstraintNone.
 *
 * In the context of normal surfaces (not angle structures):
 * Although the underlying coordinate system is based on quadrilaterals
 * and (optionally) triangles, this class has elementary support for
 * octagons also, as seen in _almost_ normal surface theory.  For the
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
 * - If you are imposing additional constraints through the LPConstraint
 *   template parameter, the corresponding linear constraint functions
 *   may change their values (since the coefficients they use for
 *   octagon types need not be related to the coefficients for the two
 *   corresponding quadrilateral columns).  Any such changes, if necessary,
 *   are encoded by the constant Constraint::octAdjustment.
 *
 * This class has been optimised to ensure that you only have one
 * octagon type declared at any given time (which is consistent with the
 * constraints of almost normal surface theory).
 *
 * All tableaux elements are of the integer class \a IntType, which is
 * supplied as a template argument.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  However, due to the unusual create-reserve-initialise
 * procedure, it does not support copying (either by copy construction or
 * copy assignment).  Because of the move semantics, this class avoids deep
 * copies, even when passing or returning objects by value.
 *
 * \headers Parts of this template class are implemented in a separate header
 * (treelp-impl.h), which is not included automatically by this file.
 * Most end users should not need this extra header, since Regina's calculation
 * engine already includes explicit instantiations for common combinations of
 * template arguments.
 *
 * \python This is a heavily templated class; nevertheless, many variants
 * are now made available to Python users.  Each class name is of the form
 * LPData_<i>Constraint</i>, where the suffix \a Constraint
 * is an abbreviated version of the \a Constraint template parameter;
 * this suffix is omitted entirely for the common case LPConstraintNone.
 * An example of such a Python class name is \c LPData_EulerPositive.
 * You are encouraged to look through the Regina namespace to see which
 * constraint classes are supported under Python.  In all cases, the IntType
 * parameter is taken to be regina::Integer.
 *
 * \apinotfinal
 *
 * \ingroup enumerate
 */
template <LPConstraint Constraint, ReginaInteger IntType>
class LPData : public Output<LPData<Constraint, IntType>> {
    private:
        const LPInitialTableaux<Constraint>* origTableaux_;
            /**< The original starting tableaux that holds the adjusted
                 matrix of matching equations, before the tree traversal
                 algorithm began.  This is stored by pointer, not reference,
                 to facilitate the create-reserve-initialise process. */
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
        size_t rank_;
            /**< The rank of the current tableaux, taking into account
                 any changes such as deactivation of variables.  This
                 will be at most (but quite possibly less than)
                 origTableaux_->rank().  We guarantee that the first
                 \a rank_ rows of the current tableaux are full rank (and so
                 any subsequent rows should simply be ignored from here on). */
        size_t* basis_;
            /**< An array of length origTableaux_->rank() that stores the
                 \a rank_ variables that form the current basis.  In
                 particular, for each i = 0,...,rank_-1, basis_[i] is the
                 basis variable whose defining row is row i.
                 If \a rank_ is smaller than origTableaux_->rank() then any
                 trailing entries in this array have undefined values,
                 and should simply be ignored. */
        ssize_t* basisRow_;
            /**< An array of length origTableaux_->columns() that indicates
                 which row of the current tableaux holds the defining equation
                 for each basis variable.  Specifically:
                 - if column \a i corresponds to a basic variable, then the
                   defining row for this basis variable is row basisRow_[i];
                 - if column \a i corresponds to an active non-basic variable,
                   then basisRow_[i] will be -1;
                 - if column \a i has been deactivated, then basisRow_[i]
                   will be zero.

                 For each i = 0,...,rank_-1, basisRow_[basis_[i]] == i. */
        bool feasible_;
            /**< Indicates whether or not the current system of
                 constraints is feasible. */
        ssize_t octPrimary_;
            /**< If we have declared an octagon type, this stores the
                 column that we use to count the octagons.
                 This will be one of the two quadrilateral columns that
                 together "represent" the octagon type, as described in
                 the class notes.
                 If we have not declared an octagon type (or if we are
                 working with angle structures), this is -1. */
        size_t octSecondary_;
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
         * Constructs a new tableaux.  You _must_ call reserve() before
         * doing anything else with this tableaux.
         */
        inline LPData();

        /**
         * Moves the contents of the given tableaux into this new tableaux.
         * This is a fast (constant time) operation.
         *
         * If the given tableaux is uninitialised, then this new tableaux
         * will be uninitialised also.
         *
         * The tableaux that is passed (\a src) will no longer be usable.
         *
         * \param src the tableaux to move.
         */
        inline LPData(LPData&& src) noexcept;

        /**
         * Destroys this tableaux.  This is safe even if reserve() was
         * never called.
         */
        inline ~LPData();

        /**
         * Moves the contents of the given tableaux into this tableaux.
         * This is a fast (constant time) operation.
         *
         * If the given tableaux is uninitialised, then this tableaux
         * will become uninitialised also.
         *
         * The tableaux that is passed (\a src) will no longer be usable.
         *
         * \param src the tableaux to move.
         * \return a reference to this tableaux.
         */
        inline LPData& operator = (LPData&& src) noexcept;

        /**
         * Swaps the contents of this and the given tableaux.
         *
         * It does not matter if the two tableaux have different sizes,
         * or if one or both is unintialised; if so then these properties
         * will be swapped also.
         *
         * \param other the tableaux whose contents should be swapped with this.
         */
        inline void swap(LPData& other) noexcept;

        /**
         * Reserves enough memory for this tableaux to work with.
         * You _must_ call this routine before doing anything else with
         * this tableaux.
         *
         * The data in this tableaux will not be initialised, and the
         * contents and behaviour of this tableaux will remain undefined
         * until you call one of the initialisation routines initStart()
         * or initClone().
         *
         * \param origTableaux the original starting tableaux that holds the
         * adjusted matrix of matching equations, before the tree traversal
         * algorithm began.
         */
        void reserve(const LPInitialTableaux<Constraint>& origTableaux);

        /**
         * Initialises this tableaux by beginning at the original
         * starting tableaux and working our way to any feasible basis.
         *
         * This routine also explicitly enforces the additional constraints
         * from the LPConstraint template parameter  (i.e., this routine
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
         * \param parent the tableaux to clone.
         */
        void initClone(const LPData& parent);

        /**
         * Returns the number of columns in this tableaux.
         *
         * Note that, if we are imposing extra constraints through the
         * LPConstraint template parameter, then there will be extra variables
         * to enforce these, and so the number of columns will be larger than
         * in the original matching equation matrix.
         *
         * \return the number of columns.
         */
        inline size_t columns() const;

        /**
         * Returns the number of columns in this tableaux that correspond to
         * normal coordinates or angle structure coordinates.  This is
         * precisely the number of columns in the original matrix of
         * matching equations.
         *
         * \return the number of normal or angle structure coordinate columns.
         */
        inline size_t coordinateColumns() const;

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
         * \return \c true if this system is feasible, or \c false if it
         * is infeasible.
         */
        inline bool isFeasible() const;

        /**
         * Determines whether the given variable is currently active.
         * See the LPData class notes for details.
         *
         * \param pos the index of the variable to query.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         * The index should be with respect to this tableaux (i.e., it must
         * take into account any permutation of columns from the original
         * matching equations).
         */
        inline bool isActive(size_t pos) const;

        /**
         * Returns the sign of the given variable under the current
         * basis.  This does _not_ attempt to "undo" any changes of variable
         * caused by prior calls to constrainPositive() or constrainOct();
         * it simply tests the sign of the variable in the given column
         * of the tableaux in its current form.
         *
         * Specifically: if the given variable is inactive or non-basic,
         * this routine returns zero.  If the given variable is in the
         * basis, this routine returns the sign of the corresponding
         * integer on the right-hand side of the tableaux.
         *
         * \param pos the index of the variable to query.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         * The index should be with respect to this tableaux (i.e., it must
         * take into account any permutation of columns from the original
         * matching equations).
         * \return the sign of the variable as described above;
         * this will be either 1, 0 or -1.
         */
        inline int sign(size_t pos) const;

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
         * _new_ variable, not the original, and so might not have the
         * intended effect.
         *
         * \param pos the index of the variable that is to be set to zero.
         * This must be between 0 and origTableaux_->columns()-1 inclusive.
         * The index should be with respect to this tableaux (i.e., it must
         * take into account any permutation of columns from the original
         * matching equations).
         */
        void constrainZero(size_t pos);

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
         * _new_ variable, not the original, and so might not have the
         * intended effect.
         *
         * \param pos the index of the variable that is to be constrained as
         * positive.  This must be between 0 and origTableaux_->columns()-1
         * inclusive.  The index should be with respect to this tableaux
         * (i.e., it must take into account any permutation of columns from
         * the original matching equations).
         */
        void constrainPositive(size_t pos);

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
         * _new_ variable, not the original, and so might not have the
         * intended effect.
         *
         * \param quad1 one of the two quadrilateral types that we
         * combine to form the new octagon type.  This should be a column index
         * with respect to this tableaux (i.e., it must take into account any
         * permutation of columns from the original matching equations).
         * \param quad2 the other of the two quadrilateral types that we
         * combine to form the new octagon type.  Again this should be a
         * column index with respect to this tableaux.
         */
        void constrainOct(size_t quad1, size_t quad2);

        /**
         * Extracts the values of the individual variables from the
         * current basis, with some modifications (as described below).
         * The values of the variables will be returned in vector form.
         *
         * The modifications are as follows:
         *
         * - We extract variables that correspond to the original
         *   matching equations obtained from the underlying
         *   triangulation, _not_ the current tableaux and _not_ even
         *   the original starting tableaux stored in origTableaux_.
         *   In other words, when we fill the resulting vector, we undo the
         *   column permutation described by LPInitialTableaux::columnPerm(),
         *   and we undo any changes of variable that were caused by
         *   calls to constrainPositive() and/or constrainOct().
         *
         * - To ensure that the variables are all integers, we scale the
         *   resulting vector by the smallest positive rational multiple
         *   for which all elements of the vector are integers.
         *
         * This routine is not used as an internal part of the tree traversal
         * algorithm; instead it is offered as a helper routine for
         * reconstructing the normal surfaces or angle structures that result.
         *
         * \pre No individual coordinate column has had more than one call
         * to either of constrainPositive() or constrainOct() (otherwise
         * the coordinate will not be correctly reconstructed).  Any
         * additional columns arising from the LPConstraint template parameter
         * are exempt from this requirement.
         *
         * \pre The precision of integers in \a Ray is at least as
         * large as the precision of \a IntType (as used by LPData).
         *
         * \python The type vector should be passed as a Python list of
         * integers (for example, in the enumeration of normal surfaces, there
         * would be one integer per tetrahedron, each equal to 0, 1, 2 or 3).
         * The template parameter \a Ray is taken to be Vector<Integer>.
         *
         * \param type the type vector corresponding to the current state of
         * this tableaux, indicating which variables were previously fixed as
         * positive via calls to constrainPositive().  This is necessary
         * because LPData does not keep such historical data on its own.
         * The order of these types should be with respect to the permuted
         * columns (i.e., it should reflect the columns as they are stored
         * in this tableaux, not the original matching equations).
         * As a special case, when extracting a strict angle structure
         * one may pass \a type = \c null, in which case this routine will
         * assume that _every_ coordinate was constrained as positive.
         * \return a vector containing the values of all the variables.
         * This vector will have length origTableaux_->coordinateColumns().
         */
        template <IntegerVector Ray>
        Ray extractSolution(const char* type) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
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

        // Mark this class as non-copyable:
        LPData(const LPData&) = delete;
        LPData& operator = (const LPData&) = delete;

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
         * \param row the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * \param col the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * \return the requested entry in this tableaux.
         */
        inline IntType entry(size_t row, size_t col) const;

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
         * \param row the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * \param col the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * \param ans an integer that will be set to the requested entry
         * in this tableaux.
         */
        inline void entry(size_t row, size_t col, IntType& ans) const;

        /**
         * Determines the sign of the given entry in this tableaux.
         *
         * Since we do not store the full tableaux, the entry is
         * computed on the fly.  However, this computation is fast
         * because the computations use sparse vector multiplication.
         *
         * \param row the row of the requested entry; this must be between 0
         * and rank_-1 inclusive.
         * \param col the column of the requested entry; this must be between 0
         * and origTableaux_->columns()-1 inclusive.
         * \return +1, -1 or 0 according to whether the requested entry
         * is positive, negative or zero.
         */
        inline int entrySign(size_t row, size_t col) const;

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
         * \param outCol the index of the variable to pivot out of the basis.
         * \param inCol the index of the variable to pivot into the basis.
         */
        void pivot(size_t outCol, size_t inCol);

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

/**
 * Swaps the contents of the given tableaux.
 *
 * This global routine simply calls LPData<Constraint, IntType>::swap();
 * it is provided so that LPData<Constraint, IntType> meets the C++ Swappable
 * requirements.
 *
 * \param a the first tableaux whose contents should be swapped.
 * \param b the second tableaux whose contents should be swapped.
 *
 * \ingroup enumerate
 */
template <LPConstraint Constraint, ReginaInteger IntType>
inline void swap(LPData<Constraint, IntType>& a,
        LPData<Constraint, IntType>& b) noexcept;

} // namespace regina

#include "enumerate/treeconstraint.h"

namespace regina {

// Inline functions for LPMatrix

template <ReginaInteger IntType>
inline LPMatrix<IntType>::LPMatrix() : rows_(0), cols_(0), dat_(nullptr) {
}

template <ReginaInteger IntType>
inline LPMatrix<IntType>::LPMatrix(size_t rows, size_t cols) :
        dat_(new IntType[rows * cols]),
        rows_(rows), cols_(cols) {
}

template <ReginaInteger IntType>
inline LPMatrix<IntType>::LPMatrix(LPMatrix&& src) noexcept :
        dat_(src.dat_), rows_(src.rows_), cols_(src.cols_) {
    src.dat_ = nullptr;
}

template <ReginaInteger IntType>
inline LPMatrix<IntType>::~LPMatrix() {
    delete[] dat_;
}

template <ReginaInteger IntType>
inline LPMatrix<IntType>& LPMatrix<IntType>::operator = (LPMatrix&& src)
        noexcept {
    std::swap(dat_, src.dat_);
    rows_ = src.rows_;
    cols_ = src.cols_;
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::swap(LPMatrix& other) noexcept {
    std::swap(dat_, other.dat_);
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::reserve(size_t maxRows, size_t maxCols) {
    dat_ = new IntType[maxRows * maxCols];
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::initClone(const LPMatrix& clone) {
    rows_ = clone.rows_;
    cols_ = clone.cols_;
    std::copy(clone.dat_, clone.dat_ + clone.rows_ * clone.cols_, dat_);
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::initIdentity(size_t size) {
    // Don't fuss about optimising this, since we only call it once
    // in the entire tree traversal algorithm.
    rows_ = cols_ = size;

    size_t r, c;
    for (r = 0; r < rows_; ++r)
        for (c = 0; c < cols_; ++c)
            entry(r, c) = (r == c ? 1 : long(0));
}

template <ReginaInteger IntType>
inline IntType& LPMatrix<IntType>::entry(size_t row, size_t col) {
    return dat_[row * cols_ + col];
}

template <ReginaInteger IntType>
inline const IntType& LPMatrix<IntType>::entry(size_t row, size_t col)
        const {
    return dat_[row * cols_ + col];
}

template <ReginaInteger IntType>
inline size_t LPMatrix<IntType>::rows() const {
    return rows_;
}

template <ReginaInteger IntType>
inline size_t LPMatrix<IntType>::columns() const {
    return cols_;
}

template <ReginaInteger IntType>
inline bool LPMatrix<IntType>::operator == (const LPMatrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_)
        return false;
    if (rows_ && cols_)
        return std::equal(dat_, dat_ + rows_ * cols_, other.dat_);
    else
        return true;
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::swapRows(size_t r1, size_t r2) {
    if (r1 != r2)
        std::swap_ranges(dat_ + r1 * cols_, dat_ + r1 * cols_ + cols_,
            dat_ + r2 * cols_);
}

template <ReginaInteger IntType>
inline void LPMatrix<IntType>::negateRow(size_t row) {
    IntType *p = dat_ + row * cols_;
    for (size_t i = 0; i < cols_; ++p, ++i)
        p->negate();
}

template <ReginaInteger IntType>
inline void swap(LPMatrix<IntType>& a, LPMatrix<IntType>& b) noexcept {
    a.swap(b);
}

// Inline functions for LPInitialTableaux

template <LPConstraint Constraint>
inline void LPInitialTableaux<Constraint>::swap(LPInitialTableaux& other)
        noexcept {
    std::swap(tri_, other.tri_);
    std::swap(system_, other.system_);
    eqns_.swap(other.eqns_);
    std::swap(rank_, other.rank_);
    std::swap(cols_, other.cols_);
    std::swap(scaling_, other.scaling_);
    col_.swap(other.col_);
    columnPerm_.swap(other.columnPerm_);
}

template <LPConstraint Constraint>
inline const Triangulation<3>& LPInitialTableaux<Constraint>::tri() const {
    return *tri_;
}

template <LPConstraint Constraint>
inline LPSystem LPInitialTableaux<Constraint>::system() const {
    return system_;
}

template <LPConstraint Constraint>
inline size_t LPInitialTableaux<Constraint>::rank() const {
    return rank_;
}

template <LPConstraint Constraint>
inline size_t LPInitialTableaux<Constraint>::columns() const {
    return cols_;
}

template <LPConstraint Constraint>
inline size_t LPInitialTableaux<Constraint>::coordinateColumns() const {
    return eqns_.columns();
}

template <LPConstraint Constraint>
inline const size_t* LPInitialTableaux<Constraint>::columnPerm() const {
    return columnPerm_.begin();
}

template <LPConstraint Constraint>
template <ReginaInteger IntType>
inline IntType LPInitialTableaux<Constraint>::multColByRow(
        const LPMatrix<IntType>& m, size_t mRow, size_t thisCol) const {
    if (scaling_ && thisCol == coordinateColumns() - 1) {
        // Multiply the entire row by the scaling coefficient.
        IntType ans; // Initialised to 0.
        for (size_t i = 0; i < rank_; ++i)
            ans += m.entry(mRow, i);
        ans *= scaling_;
        return ans;
    } else {
        // Just pick out individual coefficients using the sparse
        // representation of the column.
        IntType ans; // Initialised to 0.
        for (int i = 0; i < col_[thisCol].nPlus; ++i)
            ans += m.entry(mRow, col_[thisCol].plus[i]);
        for (int i = 0; i < col_[thisCol].nMinus; ++i)
            ans -= m.entry(mRow, col_[thisCol].minus[i]);
        for (size_t i = 0; i < Constraint::constraints.size(); ++i)
            ans += m.entry(mRow, m.rows() - Constraint::constraints.size() + i)
                * col_[thisCol].extra[i];
        return ans;
    }
}

template <LPConstraint Constraint>
template <ReginaInteger IntType>
inline IntType LPInitialTableaux<Constraint>::multColByRowOct(
        const LPMatrix<IntType>& m, size_t mRow, size_t thisCol) const {
    // By the preconditions of this routine, we must be working in some normal
    // or almost normal coordinate system, and so there is no scaling
    // coordinate to worry about.
    IntType ans; // Initialised to 0.
    for (int i = 0; i < col_[thisCol].nPlus; ++i)
        ans += m.entry(mRow, col_[thisCol].plus[i]);
    for (int i = 0; i < col_[thisCol].nMinus; ++i)
        ans -= m.entry(mRow, col_[thisCol].minus[i]);
    for (size_t i = 0; i < Constraint::constraints.size(); ++i)
        ans += m.entry(mRow, m.rows() - Constraint::constraints.size() + i) *
            (col_[thisCol].extra[i] + Constraint::octAdjustment);
    return ans;
}

template <LPConstraint Constraint>
template <ReginaInteger IntType>
inline void LPInitialTableaux<Constraint>::fillInitialTableaux(
        LPMatrix<IntType>& m) const {
    for (size_t c = 0; c < cols_; ++c) {
        for (int i = 0; i < col_[c].nPlus; ++i)
            ++m.entry(col_[c].plus[i], c);
        for (int i = 0; i < col_[c].nMinus; ++i)
            --m.entry(col_[c].minus[i], c);

        // Don't forget any additional constraints that we added
        // as final rows to the matrix.
        for (size_t i = 0; i < Constraint::constraints.size(); ++i)
            m.entry(m.rows() - Constraint::constraints.size() + i, c) =
                col_[c].extra[i];
    }

    if (scaling_)
        for (size_t i = 0; i < rank_; ++i)
            m.entry(i, coordinateColumns() - 1) = scaling_;
}

template <LPConstraint Constraint>
inline void swap(LPInitialTableaux<Constraint>& a,
        LPInitialTableaux<Constraint>& b) noexcept {
    a.swap(b);
}

// Template functions for LPData

template <LPConstraint Constraint, ReginaInteger IntType>
inline LPData<Constraint, IntType>::LPData() :
        rhs_(nullptr), rank_(0), basis_(nullptr), basisRow_(nullptr) {
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline LPData<Constraint, IntType>::LPData(LPData&& src) noexcept :
        origTableaux_(src.origTableaux_),
        rhs_(src.rhs_),
        rowOps_(std::move(src.rowOps_)),
        rank_(src.rank_),
        basis_(src.basis_),
        basisRow_(src.basisRow_),
        feasible_(src.feasible_),
        octPrimary_(src.octPrimary_),
        octSecondary_(src.octSecondary_) {
    src.rhs_ = nullptr;
    src.basis_ = nullptr;
    src.basisRow_ = nullptr;
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline LPData<Constraint, IntType>::~LPData() {
    delete[] rhs_;
    delete[] basis_;
    delete[] basisRow_;
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline LPData<Constraint, IntType>&
        LPData<Constraint, IntType>::operator = (LPData&& src) noexcept {
    origTableaux_ = src.origTableaux_;
    rowOps_ = std::move(src.rowOps_);
    rank_ = src.rank_;
    feasible_ = src.feasible_;
    octPrimary_ = src.octPrimary_;
    octSecondary_ = src.octSecondary_;

    std::swap(rhs_, src.rhs_);
    std::swap(basis_, src.basis_);
    std::swap(basisRow_, src.basisRow_);
    // Let src dispose of the original contents of these arrays in its
    // own destructor.
    return *this;
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline void LPData<Constraint, IntType>::swap(LPData& other) noexcept {
    std::swap(origTableaux_, other.origTableaux_);
    std::swap(rhs_, other.rhs_);
    rowOps_.swap(other.rowOps_);
    std::swap(rank_, other.rank_);
    std::swap(basis_, other.basis_);
    std::swap(basisRow_, other.basisRow_);
    std::swap(feasible_, other.feasible_);
    std::swap(octPrimary_, other.octPrimary_);
    std::swap(octSecondary_, other.octSecondary_);
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline void LPData<Constraint, IntType>::reserve(
        const LPInitialTableaux<Constraint>& origTableaux) {
    origTableaux_ = std::addressof(origTableaux);
    rhs_ = new IntType[origTableaux.rank()];
    rowOps_.reserve(origTableaux.rank(), origTableaux.rank());
    basis_ = new size_t[origTableaux.rank()];
    basisRow_ = new ssize_t[origTableaux.columns()];
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline size_t LPData<Constraint, IntType>::columns() const {
    return origTableaux_->columns();
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline size_t LPData<Constraint, IntType>::coordinateColumns() const {
    return origTableaux_->coordinateColumns();
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline bool LPData<Constraint, IntType>::isFeasible() const {
    return feasible_;
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline bool LPData<Constraint, IntType>::isActive(size_t pos) const {
    // If basisRow_[pos] < 0, the variable is active and non-basic.
    // If basisRow_[pos] > 0, the variable is active and basic.
    // If basisRow_[pos] == 0, then:
    //   - if rank_ > 0 and basis_[0] == pos, then the variable
    //     is active and basic;
    //   - otherwise the variable is not active.
    return ! (basisRow_[pos] == 0 &&
        (rank_ == 0 || basis_[0] != pos));
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline int LPData<Constraint, IntType>::sign(size_t pos) const {
    // If basisRow_[pos] < 0, the variable is active and non-basic.
    // If basisRow_[pos] > 0, the variable is active and basic.
    // If basisRow_[pos] == 0, then:
    //   - if rank_ > 0 and basis_[0] == pos, then the variable
    //     is active and basic;
    //   - otherwise the variable is not active.
    return ((basisRow_[pos] > 0 || (rank_ > 0 && basis_[0] == pos)) ?
        rhs_[basisRow_[pos]].sign() : 0);
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline IntType LPData<Constraint, IntType>::entry(size_t row, size_t col)
        const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != static_cast<ssize_t>(col))
        return origTableaux_->multColByRow(rowOps_, row, col);
    else {
        IntType ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
        return ans;
    }
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline void LPData<Constraint, IntType>::entry(size_t row, size_t col,
        IntType& ans) const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != static_cast<ssize_t>(col))
        ans = origTableaux_->multColByRow(rowOps_, row, col);
    else {
        ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
    }
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline int LPData<Constraint, IntType>::entrySign(size_t row, size_t col)
        const {
    // Remember to take into account any changes of variable due
    // to previous calls to constrainOct().
    if (octPrimary_ != static_cast<ssize_t>(col))
        return origTableaux_->multColByRow(rowOps_, row, col).sign();
    else {
        IntType ans = origTableaux_->multColByRowOct(rowOps_, row, col);
        ans += origTableaux_->multColByRowOct(rowOps_, row, octSecondary_);
        return ans.sign();
    }
}

template <LPConstraint Constraint, ReginaInteger IntType>
inline void swap(LPData<Constraint, IntType>& a,
        LPData<Constraint, IntType>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

