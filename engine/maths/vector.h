
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file maths/vector.h
 *  \brief Provides a fast and generic vector class.
 */

#ifndef __VECTOR_H
#ifndef __DOXYGEN
#define __VECTOR_H
#endif

#include <algorithm>
#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * An optimised vector class of elements from a given ring T.
 * Various mathematical vector operations are available.
 *
 * This class is intended for serious computation, and as a result it has a
 * streamlined implementation with no virtual methods.  It can be subclassed,
 * but since there are no virtual methods, type information must generally
 * be known at compile time.  Nevertheless, in many respects, different
 * subclasses of Vector<T> can happily interact with one another.
 *
 * This class is written with bulky types in mind (such as
 * arbitrary precision integers), and so creations and operations are kept
 * to a minimum.
 *
 * \warning As of Regina 4.90, this class merges the old functionality of
 * NFastVector and the NVector hierarchy from Regina 4.6.  As a side-effect,
 * the hierarchy has been compressed into just one class (NVectorUnit,
 * NVectorMatrix and NVectorDense are gone), elements are always stored as
 * dense vectors, and functions are no longer virtual (since the storage
 * model is now fixed).  The virtual clone() method is gone completely
 * (since there are no longer virtual functions you should use the copy
 * constructor instead), and the old makeLinComb() method is also gone
 * (just use operator *= and addCopies()).
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
class Vector {
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
        T* end;
            /**< A pointer just beyond the end of the internal array.
                 The size of the vector can be computed as (end - elements). */

    public:
        /**
         * Creates a new vector.
         * Its elements will not be initialised.
         *
         * @param newVectorSize the number of elements in the new
         * vector; this must be strictly positive.
         */
        inline Vector(size_t newVectorSize) :
                elements(new T[newVectorSize]), end(elements + newVectorSize) {
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
        inline Vector(size_t newVectorSize, const T& initValue) :
                elements(new T[newVectorSize]), end(elements + newVectorSize) {
            std::fill(elements, end, initValue);
        }
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        inline Vector(const Vector<T>& cloneMe) :
                elements(new T[cloneMe.end - cloneMe.elements]),
                end(elements + (cloneMe.end - cloneMe.elements)) {
            std::copy(cloneMe.elements, cloneMe.end, elements);
        }
        /**
         * Destroys this vector.
         */
        inline ~Vector() {
            delete[] elements;
        }
        /**
         * Returns the number of elements in the vector.
         *
         * @return the vector size.
         */
        inline size_t size() const {
            return end - elements;
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
        inline const T& operator[](size_t index) const {
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
         */
        inline void setElement(size_t index, const T& value) {
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
        inline bool operator == (const Vector<T>& compare) const {
            return std::equal(elements, end, compare.elements);
        }
        /**
         * Sets this vector equal to the given vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param cloneMe the vector whose value shall be assigned to this
         * vector.
         */
        inline Vector<T>& operator = (const Vector<T>& cloneMe) {
            std::copy(cloneMe.elements, cloneMe.end, elements);
            return *this;
        }
        /**
         * Adds the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to add to this vector.
         */
        inline void operator += (const Vector<T>& other) {
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < end; ++e, ++o)
                *e += *o;
        }
        /**
         * Subtracts the given vector from this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to subtract from this vector.
         */
        inline void operator -= (const Vector<T>& other) {
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < end; ++e, ++o)
                *e -= *o;
        }
        /**
         * Multiplies this vector by the given scalar.
         *
         * @param factor the scalar with which this will be multiplied.
         */
        inline void operator *= (const T& factor) {
            if (factor == Vector<T>::one)
                return;
            for (T* e = elements; e < end; ++e)
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
        inline T operator * (const Vector<T>& other) const {
            T ans(zero);

            const T* e = elements;
            const T* o = other.elements;
            for ( ; e < end; ++e, ++o)
                ans += (*e) * (*o);

            return ans;
        }
        /**
         * Negates every element of this vector.
         */
        inline void negate() {
            for (T* e = elements; e < end; ++e)
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
            for (const T* e = elements; e < end; ++e)
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
            for (const T* e = elements; e < end; ++e)
                ans += *e;
            return ans;
        }
        /**
         * Adds the given multiple of the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be added to
         * this vector.
         * @param multiple the multiple of \a other to be added to this
         * vector.
         */
        void addCopies(const Vector<T>& other, const T& multiple) {
            if (multiple == Vector<T>::zero)
                return;
            if (multiple == Vector<T>::one) {
                (*this) += other;
                return;
            }
            if (multiple == Vector<T>::minusOne) {
                (*this) -= other;
                return;
            }
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < end; ++e, ++o)
                *e += *o * multiple;
        }
        /**
         * Subtracts the given multiple of the given vector to this vector.
         * This behaves correctly in the case where \a other is \c this.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector a multiple of which will be
         * subtracted from this vector.
         * @param multiple the multiple of \a other to be subtracted
         * from this vector.
         */
        void subtractCopies(const Vector<T>& other, const T& multiple) {
            if (multiple == Vector<T>::zero)
                return;
            if (multiple == Vector<T>::one) {
                (*this) -= other;
                return;
            }
            if (multiple == Vector<T>::minusOne) {
                (*this) += other;
                return;
            }
            T* e = elements;
            const T* o = other.elements;
            for ( ; e < end; ++e, ++o)
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
std::ostream& operator << (std::ostream& out, const Vector<T>& vector) {
    size_t size = vector.size();
    if (size == 0)
        return out;
    out << vector[0];
    for (size_t i=1; i<size; i++)
        out << ' ' << vector[i];
    return out;
}

template <class T>
T Vector<T>::zero(0L);
    /**< Zero in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T Vector<T>::one(1L);
    /**< One in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T Vector<T>::minusOne(-1L);
    /**< Negative one in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NVector has now been renamed to Vector.
 */
template <class T>
using NVector REGINA_DEPRECATED = Vector<T>;

/*@}*/

} // namespace regina

#endif

