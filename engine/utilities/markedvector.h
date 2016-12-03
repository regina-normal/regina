
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

/*! \file utilities/markedvector.h
 *  \brief Provides space-efficient arrays with fast object-to-index lookup.
 */

#ifndef __MARKEDVECTOR_H
#ifndef __DOXYGEN
#define __MARKEDVECTOR_H
#endif

#include <vector>
#include "regina-core.h"

namespace regina {

/**
 * \addtogroup utilities Miscellaneous Utilities
 * Miscellaneous helper classes and functions
 * @{
 */

/**
 * A base class for elements of MarkedVector.
 *
 * MarkedVector is a vector class that provides fast, space-efficient
 * reverse lookup of array indices (i.e., given an object, find the
 * index at which it is stored).  The way it does this is to store the
 * array indices in the object themselves.
 *
 * As a result, any type to be stored in a MarkedVector must be derived
 * from this class.  This class provides a space to store the
 * corresponding array index, and looks after access control so that
 * only MarkedVector can edit this index.
 *
 * Since indices are stored with the objects, it is possible to perform
 * this reverse lookup by querying the object alone, without any
 * reference to the actual vector.  This can be done through the public
 * routine markedIndex().
 *
 * The trade-off of course is that any object may not belong to more
 * than one MarkedVector at a time.  Any attempt to do this will result
 * in undefined behaviour.
 *
 * See MarkedVector for further information.
 *
 * \ifacespython Not present.
 */
class REGINA_API MarkedElement {
    private:
        size_t marking_;
            /**< The index in the MarkedVector at which this object is
                 stored.  If the object does not belong to a MarkedVector,
                 the value of this field is undefined. */

    public:
        /**
         * Returns the index at which this object is stored in an
         * MarkedVector.  If this object does not belong to an
         * MarkedVector, the return value is undefined.
         *
         * @return the index at which this object is stored.
         */
        inline size_t markedIndex() const;

    template <typename T>
    friend class MarkedVector;
        /**< Allow only MarkedVector to edit the array index. */
};

/**
 * A vector of objects with fast, space-efficient reverse lookup of
 * array indices.
 *
 * This class derives from std::vector, and so provides fast forward
 * lookups from array indices to objects.  What MarkedVector provides
 * in addition to this is fast reverse lookups from objects back to
 * array indices.
 *
 * The way this class is able to provide fast constant-time reverse
 * lookups without consuming a great deal of space is by storing array indices
 * inside the objects themselves.  As a result, there are two
 * significant constraints:
 *
 * - This class can only store objects derived from MarkedElement
 *   (which provides space for storing the array indices and handles
 *   their access control).  In particular, it cannot store native types
 *   such as \c int or predefined types such as \c std::string.
 *
 * - An object can only belong to one MarkedVector at a time.  Any
 *   attempt to insert an object into more than one MarkedVector at the
 *   same time results in undefined behaviour.
 *
 * Using this class is fairly simple.  The class provides a restricted
 * subset of the std::vector functionality, including \a iterator,
 * \a const_iterator, \a begin, \a end, \a size, \a empty, \a front,
 * \a back, operator [], and \a clear (this subset may grow over
 * time if required).  In addition, any const method of std::vector can
 * be accessed through an explicit cast to const std::vector&.  To
 * perform a reverse lookup (find the index at which an array is stored),
 * simply call the object's inherited method MarkedElement::markedIndex().
 *
 * Note that, like its parent std::vector, this class performs no memory
 * management.  In particular, elements (which are pointers to real objects)
 * are not destroyed when they are removed from a vector or when the vector
 * is eventually destroyed.
 *
 * \pre The type \a T is a class derived from MarkedElement.
 *
 * \ifacespython Not present.
 */
template <typename T>
class MarkedVector : private std::vector<T*> {
    public:
        using typename std::vector<T*>::iterator;
        using typename std::vector<T*>::const_iterator;

        using typename std::vector<T*>::size_type;

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
        inline MarkedVector() {}

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
         * MarkedVector.
         *
         * @param item the item to add to this vector.
         */
        inline void push_back(T* item) {
            item->marking_ = size();
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
                --((*it)->marking_);
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
                (*it)->marking_ -= (first - last);
            return std::vector<T*>::erase(first, last);
        }

        /**
         * Swaps the contents of this and the given vector.
         *
         * @param other the vector whose contents are to be swapped with this.
         */
        inline void swap(MarkedVector<T>& other) {
            std::vector<T*>::swap(other);
        }

        /**
         * Empties this vector and refills it with the given range of items.
         *
         * Calling this routine is equivalent to calling clear() followed by
         * push_back() for each item in the range from \a begin to \a end.
         * Its implementation, however, is a little more efficient.
         *
         * The algorithm only makes a single pass through the given
         * range of iterators.
         *
         * \tparam Iterator an input iterator type, whose dereference
         * operator returns a pointer of type <tt>T*</tt>.
         *
         * @param begin an iterator that points to the beginning of the range
         * of items with which to refill this vector.
         * @param end an iterator that points past the end of the range of
         * items with which to refill this vector.
         */
        template <typename Iterator>
        void refill(Iterator begin, Iterator end) {
            Iterator it = begin;
            typename std::vector<T*>::iterator local = std::vector<T*>::begin();
            while (it != end && local != std::vector<T*>::end())
                *local++ = *it++;

            if (local != std::vector<T*>::end())
                std::vector<T*>::erase(local, std::vector<T*>::end());
            else
                while (it != end)
                    std::vector<T*>::push_back(*it++);

            size_t i = 0;
            for (local = std::vector<T*>::begin();
                    local != std::vector<T*>::end(); ++local)
                (*local)->marking_ = i++;
        }
};

/*@}*/

// Inline functions for MarkedElement

inline size_t MarkedElement::markedIndex() const {
    return marking_;
}

} // namespace regina

#endif
