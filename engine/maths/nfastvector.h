
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file nfastvector.h
 *  \brief Provides a fast but inflexible vector class for heavy computation.
 */

#ifndef __NFASTVECTOR_H
#ifndef __DOXYGEN
#define __NFASTVECTOR_H
#endif

#include <algorithm>
#include <iostream>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

template <class T>
class NFastVector;

/**
 * A fast but inflexible vector of elements from a given ring T.
 * This class is intended for heavy computation; as a result it has a
 * streamlined implementation with no virtual methods, but it cannot talk to
 * any vector class other than itself.  For a slower but more flexible vector
 * class, see the NVector hierarchy instead.
 *
 * Like NVector, this class is written with bulky types in mind (such as
 * arbitrary precision integers), so that creations and operations are kept
 * to a minimum.
 *
 * \pre Type T has a copy constructor.  That is,
 * if \c a and \c b are of type T, then \c a can be initialised to the value
 * of \c b using <tt>a(b)</tt>.
 * \pre Type T has a default constructor.  That is,
 * an object of type T can be declared with no arguments.  No specific
 * default value is required.
 * \pre Type T allows for operators <tt>=</tt>, <tt>==</tt>, <tt>+=</tt>,
 * <tt>-=</tt> and <tt>*=</tt>.
 * \pre Type T has a long integer constructor.  That is, if \c a is of type T,
 * then \c a can be initialised to a long integer \c l using <tt>a(l)</tt>.
 * \pre An element \c t of type T can be written to an output stream
 * \c out using the standard expression <tt>out << t</tt>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NFastVector {
    public:
        static T zero;
            /**< Zero in the underlying number system.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */
        static T one;
            /**< One in the underlying number system.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */
        static T minusOne;
            /**< Negative one in the underlying number system.
             *   This would be \c const if it weren't for the fact that
             *   some compilers don't like this.  It should never be
             *   modified! */

    protected:
        T* elements;
            /**< The internal array containing all vector elements. */
        unsigned vectorSize;
            /**< The size of the vector, possibly including zero
             *   elements. */

    public:
        /**
         * Creates a new vector.
         * Its elements will not be initialised.
         *
         * @param newVectorSize the number of elements in the new
         * vector; this must be strictly positive.
         */
        inline NFastVector(unsigned newVectorSize) :
                elements(new T[newVectorSize]), vectorSize(newVectorSize) {
        }
        /**
         * Creates a new vector and initialises every element to the
         * given value.
         *
         * @param newVectorSize the number of elements in the new
         * vector; this must be strictly positive.
         * @param initValue the value to assign to every element of the
         * vector.
         */
        inline NFastVector(unsigned newVectorSize, const T& initValue) :
                elements(new T[newVectorSize]), vectorSize(newVectorSize) {
            std::fill(elements, elements + vectorSize, initValue);
        }
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        inline NFastVector(const NFastVector<T>& cloneMe) :
                elements(new T[cloneMe.vectorSize]),
                vectorSize(cloneMe.vectorSize) {
            std::copy(cloneMe.elements, cloneMe.elements + vectorSize,
                elements);
        }
        /**
         * Destroys this vector.
         */
        inline ~NFastVector() {
            delete[] elements;
        }
        /**
         * Returns the number of elements in the vector.
         *
         * @return the vector size.
         */
        inline unsigned size() const {
            return vectorSize;
        }
        /**
         * Returns the element at the given index in the vector.
         * A constant reference to the element is returned; the element
         * may not be altered.
         *
         * \pre \c index is between 0 and size()-1 inclusive.
         *
         * @param index the vector index to examine.
         * @return the vector element at the given index.
         */
        inline const T& operator[](unsigned index) const {
            return elements[index];
        }
        /**
         * Sets the element at the given index in the vector to the
         * given value.
         *
         * \pre \c index is between 0 and size()-1 inclusive.
         *
         * @param index the vector index to examine.
         * @param value the new value to assign to the element.
         * @return the vector element at the given index.
         */
        inline void setElement(unsigned index, const T& value) {
            elements[index] = value;
        }

        /**
         * Determines if this vector is equal to the given vector.
         *
         * \pre This and the given vector have the same size.
         * 
         * @param compare the vector with which this will be compared.
         * @return \c true if and only if the this and the given vector
         * are equal.
         */
        inline bool operator == (const NFastVector<T>& compare) const {
            return std::equal(elements, elements + vectorSize,
                compare.elements);
        }
        /**
         * Sets this vector equal to the given vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param cloneMe the vector whose value shall be assigned to this
         * vector.
         */
        inline void operator = (const NFastVector<T>& cloneMe) {
            std::copy(cloneMe.elements, cloneMe.elements + vectorSize,
                elements);
        }
        /**
         * Adds the given vector to this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to add to this vector.
         */
        inline void operator += (const NFastVector<T>& other) {
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < elements + vectorSize; ++e, ++o)
                *e += *o;
        }
        /**
         * Subtracts the given vector from this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to subtract from this vector.
         */
        inline void operator -= (const NFastVector<T>& other) {
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < elements + vectorSize; ++e, ++o)
                *e -= *o;
        }
        /**
         * Multiplies this vector by the given scalar.
         *
         * @param factor the scalar with which this will be multiplied.
         */
        inline void operator *= (const T& factor) {
            if (factor == NFastVector<T>::one)
                return;
            for (T* e = elements; e < elements + vectorSize; ++e)
                *e *= factor;
        }
        /**
         * Calculates the dot product of this vector and the given vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector with which this will be multiplied.
         * @return the dot product of this and the given vector.
         */
        inline T operator * (const NFastVector<T>& other) const {
            T ans(zero);

            const T* e = elements;
            const T* o = other.elements;
            for ( ; e < elements + vectorSize; ++e, ++o)
                ans += (*e) * (*o);

            return ans;
        }
        /**
         * Negates every element of this vector.
         */
        inline void negate() {
            for (T* e = elements; e < elements + vectorSize; ++e)
                *e = -*e;
        }
        /**
         * Returns the norm of this vector.
         * This is the dot product of the vector with itself.
         *
         * @return the norm of this vector.
         */
        inline T norm() const {
            T ans(zero);
            for (const T* e = elements; e < elements + vectorSize; ++e)
                ans += (*e) * (*e);
            return ans;
        }
        /**
         * Returns the sum of all elements of this vector.
         *
         * @return the sum of the elements of this vector.
         */
        inline T elementSum() const {
            T ans(zero);
            for (const T* e = elements; e < elements + vectorSize; ++e)
                ans += *e;
            return ans;
        }
        /**
         * Adds the given multiple of the given vector to this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be added to
         * this vector.
         * @param multiple the multiple of \a other to be added to this
         * vector.
         */
        void addCopies(const NFastVector<T>& other, const T& multiple) {
            if (multiple == NFastVector<T>::zero)
                return;
            if (multiple == NFastVector<T>::one) {
                (*this) += other;
                return;
            }
            if (multiple == NFastVector<T>::minusOne) {
                (*this) -= other;
                return;
            }
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < elements + vectorSize; ++e, ++o)
                *e += *o * multiple;
        }
        /**
         * Subtracts the given multiple of the given vector to this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be
         * subtracted from this vector.
         * @param multiple the multiple of \a other to be subtracted
         * from this vector.
         */
        void subtractCopies(const NFastVector<T>& other, const T& multiple) {
            if (multiple == NFastVector<T>::zero)
                return;
            if (multiple == NFastVector<T>::one) {
                (*this) -= other;
                return;
            }
            if (multiple == NFastVector<T>::minusOne) {
                (*this) += other;
                return;
            }
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < elements + vectorSize; ++e, ++o)
                *e -= *o * multiple;
        }
};

/**
 * Writes the given vector to the given output stream.
 * The vector will be written on a single line with elements separated
 * by a single space.  No newline will be written.
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param vector the vector to write.
 * @return a reference to \a out.
 */
template <class T>
std::ostream& operator << (std::ostream& out, const NFastVector<T>& vector) {
    unsigned size = vector.size();
    if (size == 0)
        return out;
    out << vector[0];
    for (unsigned i=1; i<size; i++)
        out << ' ' << vector[i];
    return out;
}

template <class T>
T NFastVector<T>::zero(0L);
    /**< Zero in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NFastVector<T>::one(1L);
    /**< One in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NFastVector<T>::minusOne(-1L);
    /**< Negative one in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */

/*@}*/

} // namespace regina

#endif

