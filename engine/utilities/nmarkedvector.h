
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file nmarkedvector.h
 *  \brief Provides space-efficient arrays with fast object-to-index lookup.
 */

#ifndef __NMARKEDVECTOR_H
#ifndef __DOXYGEN
#define __NMARKEDVECTOR_H
#endif

#include <vector>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A base class for elements of NMarkedVector.
 *
 * NMarkedVector is a vector class that provides fast, space-efficient
 * reverse lookup of array indices (i.e., given an object, find the
 * index at which it is stored).  The way it does this is to store the
 * array indices in the object themselves.
 *
 * As a result, any type to be stored in an NMarkedVector must be derived
 * from this class.  This class provides a space to store the
 * corresponding array index, and looks after access control so that
 * only NMarkedVector can edit this index.
 *
 * Since indices are stored with the objects, it is possible to perform
 * this reverse lookup by querying the object alone, without any
 * reference to the actual vector.  This can be done through the public
 * routine markedIndex().
 *
 * The trade-off of course is that any object may not belong to more
 * than one NMarkedVector at a time.  Any attempt to do this will result
 * in undefined behaviour.
 *
 * See NMarkedVector for further information.
 */
class NMarkedElement {
    private:
        long marking;
            /**< The index in the NMarkedVector at which this object is
                 stored.  If the object does not belong to an NMarkedVector,
                 the value of this field is undefined. */

    public:
        /**
         * Returns the index at which this object is stored in an
         * NMarkedVector.  If this object does not belong to an
         * NMarkedVector, the return value is undefined.
         *
         * @return the index at which this object is stored.
         */
        inline long markedIndex() const;

    template <typename T>
    friend class NMarkedVector;
        /**< Allow only NMarkedVector to edit the array index. */
};

/**
 * A vector of objects with fast, space-efficient reverse lookup of
 * array indices.
 *
 * This class derives from std::vector, and so provides fast forward
 * lookups from array indices to objects.  What NMarkedVector provides
 * in addition to this is fast reverse lookups from objects back to
 * array indices.
 *
 * The way this class is able to provide fast constant-time reverse
 * lookups without consuming a great deal of space (unlike NIndexedArray
 * and its space-hungry hash tables) is by storing array indices
 * inside the objects themselves.  As a result, there are two
 * significant constraints:
 *
 * - This class can only store objects derived from NMarkedElement
 *   (which provides space for storing the array indices and handles
 *   their access control).  In particular, it cannot store native types
 *   such as \c int or predefined types such as \c std::string.
 *
 * - An object can only belong to one NMarkedVector at a time.  Any
 *   attempt to insert an object into more than one NMarkedVector at the
 *   same time results in undefined behaviour.
 *
 * Using this class is fairly simple.  The class provides a restricted
 * subset of the std::vector functionality, including \a iterator,
 * \a const_iterator, \a begin, \a end, \a size, \a empty, \a front,
 * \a back, operator [], and \a clear (this subset may grow over
 * time if required).  In addition, any const method of std::vector can
 * be accessed through an explicit cast to const std::vector&.  To
 * perform a reverse lookup (find the index at which an array is stored),
 * simply call the object's inherited method NMarkedElement::markedIndex().
 *
 * Note that, like its parent std::vector, this class performs no memory
 * management.  In particular, elements (which are pointers to real objects)
 * are not destroyed when they are removed from a vector or when the vector
 * is eventually destroyed.
 *
 * \pre The type \a T is a class derived from NMarkedElement.
 */
template <typename T>
class NMarkedVector : private std::vector<T*> {
    public:
        using std::vector<T*>::iterator;
        using std::vector<T*>::const_iterator;

        using std::vector<T*>::size_type;

        using std::vector<T*>::begin;
        using std::vector<T*>::end;
        using std::vector<T*>::size;
        using std::vector<T*>::empty;
        using std::vector<T*>::operator [];
        using std::vector<T*>::front;
        using std::vector<T*>::back;

        using std::vector<T*>::clear;

        /**
         * Constructs a new empty vector.
         */
        inline NMarkedVector() {}

        /**
         * Casts this vector to a const std::vector, thus providing
         * access to the entire const functionality of std::vector.
         *
         * @return a reference to this vector, cast as a const std::vector.
         */
        inline const std::vector<T*>& operator ()() const {
            return *this;
        }

        /**
         * Pushes the given item onto the end of this vector.  The array
         * index stored inside \a item will be modified accordingly.
         *
         * The caller retains reponsibility for the ownership of \a item.
         * This class will make no attempt to deallocate \a item when it
         * is removed or when this vector is eventually destroyed.
         *
         * \pre The given item does not already belong to some other
         * NMarkedVector.
         *
         * @param item the item to add to this vector.
         */
        inline void push_back(T* item) {
            item->marking = size();
            std::vector<T*>::push_back(item);
        }

        /**
         * Erases the item at the given position in this vector.
         *
         * The item will not be destroyed, and the (now irrelevant)
         * index stored inside it will not be modified.
         *
         * \pre The given iterator points to an element of this vector.
         *
         * @param pos an iterator pointing to the element to erase.
         * @return an iterator pointing to the element immediately
         * after the element that was erased.
         */
        inline typename std::vector<T*>::iterator erase(
                typename std::vector<T*>::iterator pos) {
            typename std::vector<T*>::iterator it = pos;
            for (++it; it != end(); ++it)
                --((*it)->marking);
            return std::vector<T*>::erase(pos);
        }

        /**
         * Erases all items in the given range in this vector.
         *
         * The items will not be destroyed, and the (now irrelevant)
         * indices stored inside them will not be modified.
         *
         * \pre The given iterators describe a valid range in this vector.
         *
         * @param first an iterator pointing to the first element to erase.
         * @param last an iterator pointing just beyond the last element
         * to erase.
         * @return an iterator pointing to the element immediately
         * after the elements that were erased.
         */
        inline typename std::vector<T*>::iterator erase(
                typename std::vector<T*>::iterator first,
                typename std::vector<T*>::iterator last) {
            for (typename std::vector<T*>::iterator it = last;
                    it != end(); ++it)
                (*it)->marking -= (first - last);
            return std::vector<T*>::erase(first, last);
        }
};

/*@}*/

// Inline functions for NMarkedElement

inline long NMarkedElement::markedIndex() const {
    return marking;
}

} // namespace regina

#endif
