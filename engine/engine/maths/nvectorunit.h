
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nvectorunit.h
 *  \brief Deals with unit vectors of various types.
 */

#ifndef __NVECTORUNIT_H
#ifndef __DOXYGEN
#define __NVECTORUNIT_H
#endif

#include "maths/nvector.h"
#include "maths/nvectordense.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * A unit vector of type T.  A unit vector has every coordinate
 * set to 0 except for a single coordinate which is 1.
 *
 * A unit vector takes almost no storage space and provides very fast
 * operations.  It should <b>never</b> be modified!  The modification
 * routines (such as <tt>=</tt>, <tt>+=</tt> and so on)
 * do <b>nothing</b>, since a modified
 * unit vector will probably no longer be a unit vector.  It is
 * recommended to declare any unit vector as \c const for a safeguard.
 *
 * Since a cloned vector might be modified, clone() will
 * return a new NVectorDense instead of a new NVectorUnit.
 *
 * The primary reason for having this class is for its dot product,
 * which is exceptionally fast since all it need do is look up the
 * corresponding coordinate of the other vector.
 *
 * \pre See the preconditions for NVector<T>.
 *
 * \ifaces Not present.
 */
template <class T>
class NVectorUnit : public NVector<T> {
    protected:
        unsigned vectorSize;
            /**< The size of the vector, possibly including zero
             *   elements. */
        unsigned direction;
            /**< The coordinate that is 1 instead of 0. */

    public:
        /**
         * Creates a new unit vector with 1 in the given coordinate
         * position.  All other positions will contain 0.
         *
         * \pre The given coordinate is between 0 and size()-1 inclusive.
         *
         * @param newVectorSize the number of elements in the new
         * vector.
         * @param coordinate the coordinate in which the 1 will appear.
         */
        NVectorUnit(unsigned newVectorSize, unsigned coordinate) :
                vectorSize(newVectorSize), direction(coordinate) {
        }
        virtual NVector<T>* clone() const {
            NVector<T>* ans = new NVectorDense<T>(vectorSize, zero);
            ans->setElement(direction, one);
            return ans;
        }
        virtual unsigned size() const {
            return vectorSize;
        }
        virtual const T& operator[](unsigned index) const {
            if (index == direction)
                return one;
            else
                return zero;
        }
        virtual void setElement(unsigned index, const T& value) {
        }
    
        virtual void operator = (const NVector<T>& cloneMe) {
        }
        virtual void operator += (const NVector<T>& other) {
        }
        virtual void operator -= (const NVector<T>& other) {
        }
        virtual void operator *= (const T& factor) {
        }
        virtual T operator * (const NVector<T>& other) const {
            return other[direction];
        }
        virtual void negate() {
        }
        virtual T norm() const {
            return one;
        }
        virtual T elementSum() const {
            return one;
        }
        virtual void addCopies(const NVector<T>& other,
                const T& multiple) {
        }
        virtual void subtractCopies(const NVector<T>& other,
                const T& multiple) {
        }
};

/*@}*/

} // namespace regina

#endif

