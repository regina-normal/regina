
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

/*! \file nintmap.h
 *  \brief For internal use with hash tables.
 */

#ifndef __NINTMAP_H
#ifndef __DOXYGEN
#define __NINTMAP_H
#endif

/**
 * An (index, value) pair used internally in
 * hash tables, where the index is a non-negative integer.
 *
 * \pre Type T has a copy constructor.  That is, if \c a
 * and \c b are of type T, then \c a can be initialised to the value of
 * \c b using <tt>a(b)</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
struct NIntMap {
    unsigned long index;
        /**< Array index at which this value is stored. */
    T value;
        /**< Value stored at this array index. */
    
    /**
     * Creates a new (index, value) pair containing the given information.
     *
     * @param newIndex the array index at which this value is to be stored.
     * @param newValue the new value to be stored.
     */
    NIntMap(unsigned long newIndex, const T& newValue) :
            index(newIndex), value(newValue) {
    }
    
    /**
     * Creates a new (index, value) pair that is a clone of the given pair.
     *
     * @param cloneMe the pair to clone.
     */
    NIntMap(const NIntMap& cloneMe) :
            index(cloneMe.index), value(cloneMe.value) {
    }

    /**
     * Determines if this pair's index is less than that of the given
     * pair.
     *
     * @param compare the pair to be compared with this.
     * @return \c true if and only if the index of this pair is strictly
     * smaller than that of \c compare.
     */
    bool operator < (const NIntMap& compare) const {
        return (index < compare.index);
    }

    /**
     * Determines if this pair's index and value are equal to those of
     * the given pair.
     *
     * \pre Type T allows for equality testing.
     *
     * @param compare the pair to be compared with this.
     * @return \c true if and only if the index and value of this pair are
     * both identical to those of \c compare.
     */
    bool operator == (const NIntMap& compare) const {
        return (index == compare.index && value == compare.value);
    }
};

#endif

