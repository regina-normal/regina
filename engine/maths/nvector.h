
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

/*! \file maths/nvector.h
 *  \brief Provides a slow but flexible mathematical vector template.
 */

#ifndef __NVECTOR_H
#ifndef __DOXYGEN
#define __NVECTOR_H
#endif

#include <iostream>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

template <class T>
class NVector;

/**
 * A slow but flexible vector class of elements from a given ring T.
 * Various mathematical vector operations are available.
 *
 * This is a virtual base class for a variety of concrete
 * implementations, allowing for memory-efficient representations of
 * sparse, dense and other specialty vectors.  Different vector
 * subclasses based upon the same ring T can happily
 * interact with each other.
 *
 * The side-effect of this flexibility is that this vector class is slow
 * (in particular, many functions are virtual).  For a fast vector class
 * better suited to heavy computation, see NFastVector instead.
 *
 * This class and its subclasses are written with bulky types
 * in mind (such as arbitrary precision integers), so creations and
 * operations are kept to a minimum.
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
class NVector {
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

    public:
        /**
         * Destroys the vector.
         */
        virtual ~NVector() {
        }
        /**
         * Makes a newly allocated clone of this vector.
         * The clone will be of the same subclass of NVector as
         * this vector.
         *
         * @return a clone of this vector.
         */
        virtual NVector<T>* clone() const = 0;
        /**
         * Returns the number of elements in the vector.
         *
         * @return the vector size.
         */
        virtual unsigned size() const = 0;
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
        virtual const T& operator[](unsigned index) const = 0;
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
        virtual void setElement(unsigned index, const T& value) = 0;

        /**
         * Determines if this vector is equal to the given vector.
         * The default implementation simply compares elements one at a
         * time.
         *
         * \pre This and the given vector have the same size.
         * 
         * @param compare the vector with which this will be compared.
         * @return \c true if and only if the this and the given vector
         * are equal.
         */
        virtual bool operator == (const NVector<T>& compare) const {
            unsigned tot = size();
            for (unsigned i=0; i<tot; i++)
                if (! ((*this)[i] == compare[i]))
                    return false;
            return true;
        }
        /**
         * Sets this vector equal to the given vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param cloneMe the vector whose value shall be assigned to this
         * vector.
         */
        virtual void operator = (const NVector<T>& cloneMe) = 0;
        /**
         * Adds the given vector to this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to add to this vector.
         */
        virtual void operator += (const NVector<T>& other) = 0;
        /**
         * Subtracts the given vector from this vector.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector to subtract from this vector.
         */
        virtual void operator -= (const NVector<T>& other) = 0;
        /**
         * Multiplies this vector by the given scalar.
         *
         * @param factor the scalar with which this will be multiplied.
         */
        virtual void operator *= (const T& factor) = 0;
        /**
         * Calculates the dot product of this vector and the given vector.
         * The default implementation simply runs through the two
         * vectors multiplying elements in pairs.
         *
         * \pre This and the given vector have the same size.
         *
         * @param other the vector with which this will be multiplied.
         * @return the dot product of this and the given vector.
         */
        virtual T operator * (const NVector<T>& other) const {
            T ans(0L);
            unsigned tot = size();
            T term;
            for (unsigned i=0; i<tot; i++) {
                term = (*this)[i];
                term *= other[i];
                ans += term;
            }
            return ans;
        }
        /**
         * Negates every element of this vector.
         */
        virtual void negate() = 0;
        /**
         * Returns the norm of this vector.
         * This is the dot product of the vector with itself.
         * The default implementation simply runs through the elements
         * squaring each one in turn.
         *
         * @return the norm of this vector.
         */
        virtual T norm() const {
            T ans(0L);
            unsigned tot = size();
            T term;
            for (unsigned i=0; i<tot; i++) {
                term = (*this)[i];
                term *= (*this)[i];
                ans += term;
            }
            return ans;
        }
        /**
         * Returns the sum of all elements of this vector.
         * The default implementation simply runs through the elements
         * adding each one in turn.
         *
         * @return the sum of the elements of this vector.
         */
        virtual T elementSum() const {
            T ans(0L);
            unsigned tot = size();
            for (unsigned i=0; i<tot; i++)
                ans += (*this)[i];
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
        virtual void addCopies(const NVector<T>& other,
                const T& multiple) = 0;
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
        virtual void subtractCopies(const NVector<T>& other,
                const T& multiple) = 0;
        /**
         * Returns a newly created vector that is a linear combination
         * of this vector and another given vector.
         * The vector returned will be
         * <tt>myCoeff * this + yourCoeff * you</tt>.
         *
         * The new vector will initially be created by cloning this
         * vector, which will thus determine its specific NVector
         * subclass.
         *
         * @param myCoeff the coefficient of this vector in the linear
         * combination.
         * @param you the other vector to combine with this.
         * @param yourCoeff the coefficient of \a you in the linear
         * combination.
         */
        NVector<T>* makeLinComb(const T& myCoeff,
                const NVector<T>& you, const T& yourCoeff) const {
            NVector<T>* ans = clone();
            ans *= myCoeff;
            ans->addCopies(you, yourCoeff);
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
std::ostream& operator << (std::ostream& out, const NVector<T>& vector) {
    unsigned size = vector.size();
    if (size == 0)
        return out;
    out << vector[0];
    for (unsigned i=1; i<size; i++)
        out << ' ' << vector[i];
    return out;
}

template <class T>
T NVector<T>::zero(0L);
    /**< Zero in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NVector<T>::one(1L);
    /**< One in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */
template <class T>
T NVector<T>::minusOne(-1L);
    /**< Negative one in the underlying number system.
     *   This would be \c const if it weren't for the fact that
     *   some compilers don't like this.  It should never be
     *   modified! */

/*@}*/

} // namespace regina

#endif

