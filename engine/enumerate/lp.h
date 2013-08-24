/**
 * Optimisation flags:
 * Define any combination of the following flags to switch \e off
 * various optimisations.
 * This is for diagnostic purposes only.
 */
// #define REGINA_NOOPT_REORDER_COLUMNS

/**
 * Define REGINA_VERIFY_LPDATA to check invariants as the algorithm runs.
 * This checking is slow and can increase the running time significantly.
 */
// #define REGINA_VERIFY_LPDATA

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
 * NInteger.
 *
 * \ifacespython Not present.
 */
class LPMatrix {
    private:
        NInteger* dat_;
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
                dat_(new NInteger[rows * cols]),
                rows_(rows), cols_(cols) {
            // The NInteger default constructor sets all elements to zero.
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
            dat_ = new NInteger[maxRows * maxCols];
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
        inline NInteger& entry(unsigned row, unsigned col) {
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
        inline const NInteger& entry(unsigned row, unsigned col) const {
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
        inline void combRow(const NInteger& destCoeff, unsigned dest,
                const NInteger& srcCoeff, unsigned src,
                const NInteger& div) {
            NInteger* ps = dat_ + src * cols_;
            NInteger* pd = dat_ + dest * cols_;
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
        inline NInteger combRowAndNorm(const NInteger& destCoeff,
                unsigned dest, const NInteger& srcCoeff, unsigned src) {
            NInteger gcdRow; // Initialised to zero.
            NInteger* ps = dat_ + src * cols_;
            NInteger* pd = dat_ + dest * cols_;
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
            NInteger *p = dat_ + row * cols_;
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
        inline NInteger multColByRow(const LPMatrix& m, unsigned mRow,
                unsigned thisCol) const {
            NInteger ans = col_[thisCol].innerProduct(m, mRow);

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
        inline NInteger multColByRowOct(const LPMatrix& m,
                unsigned mRow, unsigned thisCol) const {
            NInteger ans = col_[thisCol].innerProductOct(m, mRow);

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
        NInteger* rhs_;
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
            rhs_ = new NInteger[origTableaux->rank()];
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
                NInteger e1 = entry(row1, quad1);
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

                    NInteger coeff;
                    NInteger gcdRow;
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
            NInteger lcm(1);
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
            NInteger coord;
            for (i = 0; i < rank_; ++i) {
                if (basis_[i] >= v.size())
                    continue;
                coord = lcm;
                coord *= rhs_[i];
                coord /= entry(i, basis_[i]);
                // Here we convert from the faster NInteger back to the
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
            NLargeInteger lcmAsLarge(lcm); // NInteger -> NLargeInteger

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
        inline NInteger entry(unsigned row, unsigned col) const {
            // Remember to take into account any changes of variable due
            // to previous calls to constrainOct().
            if (octPrimary_ != col)
                return origTableaux_->multColByRow(rowOps_, row, col);
            else {
                NInteger ans = origTableaux_->multColByRowOct(rowOps_,
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
            NInteger base = entry(defRow, inCol);
            if (base < 0) {
                base.negate();
                rhs_[defRow].negate();
                rowOps_.negateRow(defRow);
            }

            // Walk through the entire tableaux and perform row operations
            // to ensure that the only non-zero entry in column \a inCol
            // is the entry base in row defRow (as extracted above).
            NInteger coeff;
            NInteger gcdRow;
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
            NInteger base, coeff;
            NInteger gcdRow;
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
            NInteger outEntry, tmp;

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
                NInteger g; // Initialised to zero.
                for (c = 0; c < rowOps_.columns(); ++c)
                    g = g.gcd(rowOps_.entry(r, c));
                if (g != 1) {
                    std::cerr << "VERIFY: GCD error" << std::endl;
                    ::exit(1);
                }
            }
        }
};

