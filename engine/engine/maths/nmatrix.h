
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nmatrix.h
 *  \brief Deals with matrices of elements of various types.
 */

#ifndef __NMATRIX_H
#ifndef __DOXYGEN
#define __NMATRIX_H
#endif

namespace regina {

/**
 * Represents a matrix of elements of the given type T.
 *
 * \pre Type T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NMatrix {
    protected:
        unsigned long nRows;
            /**< The number of rows in the matrix. */
        unsigned long nCols;
            /**< The number of columns in the matrix. */
        T** data;
            /**< The actual entries in the matrix.
             *   <tt>data[r][c]</tt> is the element in row \a r,
             *   column \a c. */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default
         * constructors.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        NMatrix(unsigned long rows, unsigned long cols) :
                nRows(rows), nCols(cols), data(new (T*)[rows]){
            for (unsigned long i = 0; i < rows; i++)
                data[i] = new T[cols];
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        NMatrix(const NMatrix& cloneMe) : nRows(cloneMe.nRows),
                nCols(cloneMe.nCols), data(new (T*)[cloneMe.nRows]) {
            unsigned long r, c;
            for (r = 0; r < nRows; r++) {
                data[r] = new T[nCols];
                for (c = 0; c < nCols; c++)
                    data[r][c] = cloneMe.data[r][c];
            }
        }
        /**
         * Destroys this matrix.
         */
        virtual ~NMatrix() {
            for (unsigned long i = 0; i < nRows; i++)
                delete[] data[i];
            delete[] data;
        }

        /**
         * Sets every entry in the matrix to the given value.
         *
         * @param value the value to assign to each entry.
         */
        void initialise(const T& value) {
            unsigned long r, c;
            for (r = 0; r < nRows; r++)
                for (c = 0; c < nCols; c++)
                    data[r][c] = value;
        }

        /**
         * Returns the number of rows in this matrix.
         *
         * @return the number of rows.
         */
        unsigned long rows() const {
            return nRows;
        }
        /**
         * Returns the number of columns in this matrix.
         *
         * @return the number of columns.
         */
        unsigned long columns() const {
            return nCols;
        }

        /**
         * Returns the entry at the given row and column.
         * Rows and columns are numbered beginning at zero.
         *
         * \pre \a row is between 0 and rows()-1 inclusive.
         * \pre \a column is between 0 and columns()-1 inclusive.
         *
         * \ifaces Not present, even if a subclass of NMatrix
         * is mirrored and its inherited routines are mirrored also.
         * In such a case, the entry() routines will be replaced by the two
         * routines <i>getEntry(row, column)</i> and <i>setEntry(row,
         * column, value)</i> that retrieve and set entries in the
         * matrix respectively.  Routine \a setEntry returns void and
         * takes an extra (final) value parameter of type
         * <tt>(const T&)</tt>.  Routine
         * \a getEntry returns an object of type T that is a snapshot of
         * the current matrix entry and will not remain correct if the
         * matrix later changes.
         *
         * @param row the row of the desired entry.
         * @param column the column of the desired entry.
         * @return a reference to the entry in the given row and column.
         */
        T& entry(unsigned long row, unsigned long column) {
            return data[row][column];
        }
        /**
         * Returns the entry at the given row and column.
         * Rows and columns are numbered beginning at zero.
         *
         * \pre \a row is between 0 and rows()-1 inclusive.
         * \pre \a column is between 0 and columns()-1 inclusive.
         *
         * \ifaces Not present, even if a subclass of NMatrix
         * is mirrored and its inherited routines are mirrored also.
         * In such a case, the entry() routines will be replaced by the two
         * routines <i>getEntry(row, column)</i> and <i>setEntry(row,
         * column, value)</i> that retrieve and set entries in the
         * matrix respectively.  Routine \a setEntry returns void and
         * takes an extra (final) value parameter of type
         * <tt>(const T&)</tt>.  Routine
         * \a getEntry returns an object of type T that is a snapshot of
         * the current matrix entry and will not remain correct if the
         * matrix later changes.
         *
         * @param row the row of the desired entry.
         * @param column the column of the desired entry.
         * @return a reference to the entry in the given row and column.
         */
        const T& entry(unsigned long row, unsigned long column) const {
            return data[row][column];
        }

        /**
         * Writes a complete representation of the matrix to the given
         * output stream.
         * Each row will be written on a separate line with elements in
         * each row separated by single spaces.
         *
         * \ifaces Not present, even if a subclass of NMatrix
         * is mirrored and its inherited routines are mirrored also.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeMatrix(std::ostream& out) const {
            unsigned long r, c;
            for (r = 0; r < nRows; r++) {
                for (c = 0; c < nCols; c++) {
                    if (c > 0) out << ' ';
                    out << data[r][c];
                }
                out << '\n';
            }
        }

        /**
         * Swaps the elements of the two given rows in the matrix.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param first the first row to swap.
         * @param second the second row to swap.
         */
        void swapRows(unsigned long first, unsigned long second) {
            T tmp;
            for (unsigned long i = 0; i < nCols; i++) {
                tmp = data[first][i];
                data[first][i] = data[second][i];
                data[second][i] = tmp;
            }
        }
        /**
         * Swaps the elements of the two given columns in the matrix.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param first the first column to swap.
         * @param second the second column to swap.
         */
        void swapColumns(unsigned long first, unsigned long second) {
            T tmp;
            for (unsigned long i = 0; i < nRows; i++) {
                tmp = data[i][first];
                data[i][first] = data[i][second];
                data[i][second] = tmp;
            }
        }
};

/**
 * Represents a matrix of elements from a given ring T.
 *
 * \pre Type T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre An element <i>t</i> of type T can be written to an output stream
 * <i>out</i> using the standard expression <tt>out << t</tt>.
 * \pre Type T provides binary operators <tt>+</tt> and <tt>*</tt> and
 * unary operators <tt>+=</tt> and <tt>*=</tt>.
 * \pre Type T has a long integer constructor.  That is, if \c a is of type T,
 * then \c a can be initialised to a long integer \c l using <tt>a(l)</tt>.
 * Here the value 1 refers to the multiplicative identity in the ring T.
 *
 * \ifaces Not present.
 */
template <class T>
class NMatrixRing : public NMatrix<T> {
    public:
        static T zero;
            /**< Zero in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */
        static T one;
            /**< One (the multiplicative identity) in the underlying ring.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */

    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised using their default
         * constructors.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        NMatrixRing(unsigned long rows, unsigned long cols) :
                NMatrix<T>(rows, cols) {
        }
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        NMatrixRing(const NMatrix<T>& cloneMe) :
                NMatrix<T>(cloneMe) {
        }

        /**
         * Turns this matrix into an identity matrix.
         * This matrix need not be square; after this routine it will
         * have <tt>entry(r,c)</tt> equal to <tt>one</tt> if
         * <tt>r == c</tt> and <tt>zero</tt> otherwise.
         */
        void makeIdentity() {
            initialise(zero);
            for (unsigned long i = 0; i < nRows && i < nCols; i++)
                data[i][i] = one;
        }

        /**
         * Adds the given source row to the given destination row.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         */
        void addRow(unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < nCols; i++)
                data[dest][i] += data[source][i];
        }
        /**
         * Adds the given number of copies of the given source row to
         * the given destination row.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The two given rows are distinct and between 0 and
         * rows()-1 inclusive.
         *
         * @param source the row to add.
         * @param dest the row that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        void addRow(unsigned long source, unsigned long dest,
                T copies) {
            for (unsigned long i = 0; i < nCols; i++)
                data[dest][i] += copies * data[source][i];
        }
        /**
         * Adds the given source column to the given destination column.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         */
        void addCol(unsigned long source, unsigned long dest) {
            for (unsigned long i = 0; i < nRows; i++)
                data[i][dest] += data[i][source];
        }
        /**
         * Adds the given number of copies of the given source column to
         * the given destination column.
         *
         * Note that \a copies is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The two given columns are distinct and between 0 and
         * columns()-1 inclusive.
         *
         * @param source the columns to add.
         * @param dest the column that will be added to.
         * @param copies the number of copies of \a source to add to
         * \a dest.
         */
        void addCol(unsigned long source, unsigned long dest,
                T copies) {
            for (unsigned long i = 0; i < nRows; i++)
                data[i][dest] += copies * data[i][source];
        }
        /**
         * Multiplies the given row by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The given row is between 0 and rows()-1 inclusive.
         *
         * @param row the row to work with.
         * @param factor the factor by which to multiply the given row.
         */
        void multRow(unsigned long row, T factor) {
            for (unsigned long i = 0; i < nCols; i++)
                data[row][i] *= factor;
        }
        /**
         * Multiplies the given column by the given factor.
         *
         * Note that \a factor is passed by value in case it is an
         * element of the row to be changed.
         *
         * \pre The given column is between 0 and columns()-1 inclusive.
         *
         * @param column the column to work with.
         * @param factor the factor by which to multiply the given column.
         */
        void multCol(unsigned long column, T factor) {
            for (unsigned long i = 0; i < nRows; i++)
                data[i][column] *= factor;
        }

        /**
         * Multiplies this by the given matrix.
         *
         * \pre The number of columns in this matrix equals the number
         * of rows in the given matrix.
         *
         * \ifaces Not present, even if a subclass of NMatrix
         * is mirrored and its inherited routines are mirrored also.
         *
         * @param other the matrix by which to multiply this matrix.
         * @return a newly allocated matrix representing
         * <tt>this * other</tt>.
         */
        NMatrixRing<T>* operator * (const NMatrixRing<T>& other) const {
            NMatrixRing<T>* ans = new NMatrixRing<T>(nRows, other.nCols);
            unsigned long row, col, k;
            for (row = 0; row < nRows; row++)
                for (col = 0; col < other.nCols; col++) {
                    ans->data[row][col] = zero;
                    for (k = 0; k < nCols; k++)
                        ans->data[row][col] +=
                            (data[row][k] * other.data[k][col]);
                }
            return ans;
        }
};

template <class T>
T NMatrixRing<T>::zero((long)0);
    /**< Zero in the underlying ring.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NMatrixRing<T>::one((long)1);
    /**< One (the multiplicative identity) in the underlying ring.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */

} // namespace regina

#endif

