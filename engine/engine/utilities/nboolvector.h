
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

/*! \file nboolvector.h
 *  \brief Deals with vectors of trues and falses, compacted for space
 *  efficiency.
 */

#ifndef __NBOOLVECTOR_H
#ifndef __DOXYGEN
#define __NBOOLVECTOR_H
#endif

class ostream;

/**
 * A fixed-size vector of trues and falses.
 * Internally the vector is stored as an array of unsigned ints to give
 * maximum space efficiency.
 * Various boolean operations are available, as well as a partial
 * ordering by inclusion and a total lexiographical ordering.
 *
 * Note that this class is also ideal for representing subsets of a
 * given fixed-size set.
 *
 * \ifaces Not present.
 */
class NBoolVector {
    private:
        unsigned *array;
            /**< Stores the actual bits, one bit representing one
             *   boolean in the array.  The first bit stored is the high
             *   bit of the first element of the array.  The bits in the
             *   final int that do not represent bits of the vector can have
             *   arbitrary values and these might change over time. */
        unsigned long nBits;
            /**< Number of bits stored in this vector. */
        unsigned long nInts;
            /**< Number of unsigned ints used to store all the bits. */
    
        const static unsigned bitsPerInt = 8*sizeof(unsigned);
            /**< Number of bits stored in each unsigned integer. */
        const unsigned unusedBits;
            /**< The portion of the final int that does not represent bits
             *   of the vector.  For instance, if the final 3 bits are not
             *   used, this will be equal to 00..00111 in binary. */

    public:
        /**
         * Creates a new boolean vector.
         * The booleans are not initialised to any particular values.
         *
         * \pre The requested vector size is strictly positive.
         *
         * @param size the number of booleans to store in the new vector.
         */
        NBoolVector(unsigned long size);
        /**
         * Creates a new boolean vector with all booleans initialised to the
         * given value.
         *
         * \pre The requested vector size is strictly positive.
         *
         * @param size the number of booleans to store in the new vector.
         * @param value the value with which to initialise each boolean.
         */
        NBoolVector(unsigned long size, bool value);
        /**
         * Creates a new clone of the given boolean vector.
         * The new vector will have the same size and contents as the given
         * vector.
         *
         * @param cloneMe the boolean vector that is to be cloned.
         */
        NBoolVector(const NBoolVector& cloneMe);
        /**
         * Destroys the boolean vector.
         */
        virtual ~NBoolVector();
    
        /**
         * Returns the number of booleans stored in the vector.
         *
         * @return the vector size.
         */
        unsigned long size() const;
        /**
         * Sets all the booleans to false.
         */
        void clear();
    
        /**
         * Returns the value of the requested boolean.
         *
         * @param index specifies which boolean to return; this should be
         * between 0 and size()-1 inclusive.
         * @return the value of the requested boolean.
         */
        bool operator[](unsigned long index) const;
        /**
         * Sets the value of the requested boolean.
         *
         * @param index specifies which boolean to set; this should be
         * between 0 and size()-1 inclusive.
         * @param value the new value to be assigned to the requested
         * boolean.
         */
        void setElement(unsigned long index, bool value);

        /**
         * Performs a logical AND with the given vector.
         *
         * \pre This and the given vector store the same number of
         * booleans.
         *
         * @param other the vector with which to AND this vector.
         */
        void operator &= (const NBoolVector& other);
        /**
         * Performs a logical OR with the given vector.
         *
         * \pre This and the given vector store the same number of
         * booleans.
         *
         * @param other the vector with which to OR this vector.
         */
        void operator |= (const NBoolVector& other);
        /**
         * Negates every boolean in this vector.
         */
        void negate();

        /**
         * Determines if this vector contains the given vector as a
         * sub-vector.
         * This means that wherever the sub-vector contains a true
         * boolean, this vector must also contain a true boolean.
         *
         * \pre This and the given vector store the same number of
         * booleans.
         *
         * @param subvector the vector to test for inclusion.
         * @return \c true if and only if this vector contains \a subvector
         * as a sub-vector.
         */
        bool contains(const NBoolVector& subvector) const;

        /**
         * Compares this and the given vector as if they were binary
         * numbers, the first boolean being the most significant.
         * True is considered larger than false.
         *
         * \pre This and the given vector store the same number of
         * booleans.
         *
         * @param other the boolean vector to compare with this.
         * @return \c true if and only if this boolean vector is strictly less
         * than the given boolean vector.
         */
        bool operator < (const NBoolVector& other) const;
        /**
         * Determines if this and the given vector are identical.
         *
         * \pre This and the given vector store the same number of
         * booleans.
         *
         * @param other the boolean vector to compare with this.
         * @return \c true if and only if both vectors are identical.
         */
        bool operator == (const NBoolVector& other) const;
        /**
         * Determines if this vector contains only false booleans.
         *
         * @return \c true if and only if this vector contains only false
         * booleans.
         */
        bool isAllFalse() const;
        /**
         * Determines if this vector contains only true booleans.
         *
         * @return \c true if and only if this vector contains only true
         * booleans.
         */
        bool isAllTrue() const;

    friend ostream& operator << (ostream& out, const NBoolVector& vector);
};

/**
 * Writes the given vector to the given output stream.
 * The vector will be written on a single line.
 * True and false elements will be written as 1 and 0 respectively, and
 * they will not be separated by spaces.  No newline will be written.
 *
 * @param out the output stream to which to write.
 * @param vector the vector to write.
 * @return a reference to \a out.
 */
ostream& operator << (ostream& out, const NBoolVector& vector);

// Inline functions for NBoolVector

inline NBoolVector::NBoolVector(unsigned long size) :
        nBits(size),
        nInts(((size-1)/bitsPerInt) + 1),
        unusedBits( (unsigned(1) <<
            ((bitsPerInt - 1) - ((size-1) % bitsPerInt))) - 1) {
    array = new unsigned[nInts];
}

inline NBoolVector::~NBoolVector() {
    delete[] array;
}

inline unsigned long NBoolVector::size() const {
    return nBits;
}

inline bool NBoolVector::operator[](unsigned long index) const {
    return array[index / bitsPerInt] &
        (unsigned(1) << (bitsPerInt - 1 - (index % bitsPerInt)));
}

#endif
