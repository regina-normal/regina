
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

/*! \file nindexedarray.h
 *  \brief Deals with arrays of objects with fast object-to-index lookup.
 */

#ifndef __NINDEXEDARRAY_H
#ifndef __DOXYGEN
#define __NINDEXEDARRAY_H
#endif

#include <vector>
#include <hash_map>
#include "utilities/hashutils.h"

namespace regina {

/**
 * A dynamically resizable array of objects of type T with fast random
 * access and fast object-to-index lookup.  The fast object-to-index
 * lookup is achieved by using a hashed dictionary mapping objects to
 * array indices.  See routine index() for further details.
 *
 * This class satisfies all of the requirements of a random access
 * container and a back insertion sequence in the C++ standard, with the
 * exception that once an object has been inserted into the container it
 * cannot be modified.  Thus all routines returning a <tt>reference</tt>
 * instead of a <tt>const_reference</tt> have been removed, and type
 * <tt>iterator</tt> is the same as type <tt>const_iterator</tt> (and
 * similarly for reverse iterators).
 *
 * Additional routines beyond the C++ standard requirements include index()
 * and erase(const_reference).
 *
 * Template parameter <tt>HashFcn</tt> will be used to generate hash
 * values for array elements.
 * Template parameter <tt>EqualTo</tt> will be used to compare array
 * elements when looking up the corresponding array index.
 *
 * \pre Template parameter <tt>HashFcn</tt> satisfies the requirements of
 * a hash function (with argument type <tt>Data</tt>) according to the
 * Standard Template Library.
 */
template <class Data, class HashFcn = stdhash::hash<Data>,
    class EqualTo = std::equal_to<Data> >
class NIndexedArray {
    private:
        typedef std::vector<Data> ObjectArray;
            /**< The type used for the internal array of objects. */
        typedef stdhash::hash_multimap<Data,
                typename ObjectArray::difference_type,
                HashFcn, EqualTo> IndexMap;
            /**< The type used for the internal object-to-index dictionary. */
        ObjectArray objects;
            /**< The internal array of objects. */
        IndexMap indices;
            /**< The dictionary mapping objects to array indices. */

    public:
        typedef typename ObjectArray::value_type value_type;
            /**< See the C++ standard. */
        typedef typename ObjectArray::pointer pointer;
            /**< See the C++ standard. */
        typedef typename ObjectArray::const_reference const_reference;
            /**< See the C++ standard. */
        typedef typename ObjectArray::size_type size_type;
            /**< See the C++ standard. */
        typedef typename ObjectArray::difference_type difference_type;
            /**< See the C++ standard. */
        typedef typename ObjectArray::const_iterator iterator;
            /**< See the C++ standard. */
        typedef typename ObjectArray::const_iterator const_iterator;
            /**< See the C++ standard. */
        typedef typename ObjectArray::const_reverse_iterator reverse_iterator;
            /**< See the C++ standard. */
        typedef typename ObjectArray::const_reverse_iterator
                const_reverse_iterator;
            /**< See the C++ standard. */

    public:
        /**
         * See the C++ standard.
         */
        NIndexedArray() {
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray(size_type n) : objects(n) {
            insertIndices(objects.begin(), objects.end());
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray(size_type n, const Data& t) : objects(n, t) {
            insertIndices(objects.begin(), objects.end());
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray(const NIndexedArray<Data, HashFcn, EqualTo>& array) :
                objects(array.objects), indices(array.indices) {
        }
        /**
         * See the C++ standard.
         */
        template <class InputIterator>
        NIndexedArray(InputIterator first, InputIterator last) :
                objects(first, last) {
            insertIndices(objects.begin(), objects.end());
        }
        /**
         * See the C++ standard.
         */
        ~NIndexedArray() {
        }

        /**
         * See the C++ standard.
         */
        iterator begin() {
            return objects.begin();
        }
        /**
         * See the C++ standard.
         */
        iterator end() {
            return objects.end();
        }
        /**
         * See the C++ standard.
         */
        const_iterator begin() const {
            return objects.begin();
        }
        /**
         * See the C++ standard.
         */
        const_iterator end() const {
            return objects.end();
        }
        /**
         * See the C++ standard.
         */
        reverse_iterator rbegin() {
            return objects.rbegin();
        }
        /**
         * See the C++ standard.
         */
        reverse_iterator rend() {
            return objects.rend();
        }
        /**
         * See the C++ standard.
         */
        const_reverse_iterator rbegin() const {
            return objects.rbegin();
        }
        /**
         * See the C++ standard.
         */
        const_reverse_iterator rend() const {
            return objects.rend();
        }
        /**
         * See the C++ standard.
         */
        size_type size() const {
            return objects.size();
        }
        /**
         * See the C++ standard.
         */
        size_type max_size() const {
            size_type maxObj = objects.max_size();
            size_type maxInd = indices.max_size();
            return (maxObj < maxInd ? maxObj : maxInd);
        }
        /**
         * See the C++ standard.
         */
        bool empty() const {
            return objects.empty();
        }
        /**
         * See the C++ standard.
         */
        const_reference operator [] (size_type n) const {
            return objects[n];
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray<Data, HashFcn>& operator =
                (const NIndexedArray<Data, HashFcn, EqualTo>& array) {
            objects = array.objects;
            indices = array.indices;
        }
        /**
         * See the C++ standard.
         */
        const_reference front() const {
            return objects.front();
        }
        /**
         * See the C++ standard.
         */
        const_reference back() const {
            return objects.back();
        }
        /**
         * See the C++ standard.
         */
        void push_back(const Data& item) {
            indices.insert(std::make_pair(item, objects.size()));
            objects.push_back(item);
        }
        /**
         * See the C++ standard.
         */
        void pop_back() {
            eraseIndex(objects.size() - 1);
            objects.pop_back();
        }
        /**
         * See the C++ standard.
         */
        void swap(NIndexedArray<Data, HashFcn>& array) {
            objects.swap(array.objects);
            indices.swap(array.indices);
        }
        /**
         * See the C++ standard.
         */
        iterator insert(iterator pos, const Data& x) {
            incrementIndices(pos, objects.end(), 1);
            pos = objects.insert(convertIterator(pos), x);
            insertIndices(pos, pos + 1);
            return pos;
        }
        /**
         * See the C++ standard.
         */
        template <class InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last) {
            difference_type posIndex = pos - objects.begin();
            difference_type newElts = last - first;

            incrementIndices(pos, objects.end(), newElts);
            objects.insert(convertIterator(pos), first, last);
            pos = objects.begin() + posIndex;
            insertIndices(pos, pos + newElts);
        }
        /**
         * See the C++ standard.
         */
        void insert(iterator pos, size_type n, const Data& x) {
            difference_type posIndex = pos - objects.begin();

            incrementIndices(pos, objects.end(), n);
            objects.insert(convertIterator(pos), n, x);
            pos = objects.begin() + posIndex;
            insertIndices(pos, pos + n);
        }
        /**
         * See the C++ standard.
         */
        iterator erase(iterator pos) {
            incrementIndices(pos + 1, objects.end(), -1);
            eraseIndex(pos - objects.begin());
            return objects.erase(convertIterator(pos));
        }
        /**
         * See the C++ standard.
         */
        iterator erase(iterator first, iterator last) {
            difference_type lostElts = last - first;
            incrementIndices(last, objects.end(), -lostElts);

            size_type index = first - objects.begin();
            for (iterator it = first; it != last; it++)
                eraseIndex(index++);

            return objects.erase(convertIterator(first), convertIterator(last));
        }
        /**
         * See the C++ standard.
         */
        void clear() {
            objects.clear();
            indices.clear();
        }
        /**
         * See the C++ standard.
         */
        void resize(size_type n) {
            if (n == objects.size())
                return;
            if (n > objects.size()) {
                // Increase the size.
                insert(objects.end(), n - objects.size(), Data());
            } else {
                // Decrease the size.
                erase(objects.begin() + n, objects.end());
            }
        }
        /**
         * See the C++ standard.
         */
        void resize(size_type n, const Data& t) {
            if (n == objects.size())
                return;
            if (n > objects.size()) {
                // Increase the size.
                insert(objects.end(), n - objects.size(), t);
            } else {
                // Decrease the size.
                erase(objects.begin() + n, objects.end());
            }
        }

        /**
         * Finds the index in the array of the first occurrence of the
         * given value.
         *
         * This routine is made quite fast through use of the internal
         * hashed dictionary.
         *
         * @param value the object to search for in the array.
         * @return the corresponding array index, or -1 if the given
         * object does not exist in the array.
         */
        difference_type index(const_reference value) const {
            typename IndexMap::const_iterator pos = indices.find(value);
            if (pos == indices.end())
                return -1;
            return (*pos).second;
        }
        /**
         * Erases all copies of the given object from the array.
         *
         * This routine is made quite fast through use of the internal
         * hashed dictionary.
         *
         * @param value the object to remove from the array.
         */
        void erase(const_reference value) {
            std::pair<typename IndexMap::iterator,
                typename IndexMap::iterator> range =
                indices.equal_range(value);
            for (typename IndexMap::iterator it = range.first;
                    it != range.second; it++) {
                incrementIndices(objects.begin() + (*it).second + 1,
                    objects.end(), -1);
                objects.erase(objects.begin() + (*it).second);
            }
            indices.erase(range.first, range.second);
        }

    private:
        /**
         * Converts an iterator for this class to an iterator for the
         * internal object array.  This is necessary because an iterator
         * for this class is in fact a const iterator for the internal
         * array.
         *
         * @param it the iterator for this class.
         * @return the corresponding iterator for the internal object array.
         */
        typename ObjectArray::iterator convertIterator(iterator it) {
            return objects.begin() + (it - objects.begin());
        }
        /**
         * Add the objects and their corresponding indices from the
         * given range to the internal hash map.
         *
         * @param first the first object to add.
         * @param last one step beyond the last object to add.
         */
        void insertIndices(const_iterator first, const_iterator last) {
            difference_type index = first - begin();
            for ( ; first != last; first++)
                indices.insert(std::make_pair(*first, index++));
        }
        /**
         * Increment the indices stored in the internal hash map for the
         * objects in the given range by the given (possibly negative) amount.
         *
         * @param first the first object whose index should be adjusted.
         * @param last one step beyond the last object whose index
         * should be adjusted.
         * @param delta the amount by which the indices should be
         * incremented; this may be negative.
         */
        void incrementIndices(const_iterator first, const_iterator last,
                difference_type delta) {
            // The order in which we do this depends upon whether delta
            // is positive or negative.
            if (delta == 0)
                return;
            else if (delta > 0) {
                // Delta is positive - work backwards.
                std::pair<typename IndexMap::iterator,
                    typename IndexMap::iterator> range;
                difference_type index = last - begin();
                while (last != first) {
                    last--;
                    index--;
                    range = indices.equal_range(*last);
                    for ( ; range.first != range.second; range.first++)
                        if ((*range.first).second == index) {
                            (*range.first).second += delta;
                            break;
                        }
                }
            } else {
                // Delta is negative - work forwards.
                std::pair<typename IndexMap::iterator,
                    typename IndexMap::iterator> range;
                difference_type index = first - begin();
                for ( ; first != last; first++) {
                    range = indices.equal_range(*first);
                    for ( ; range.first != range.second; range.first++)
                        if ((*range.first).second == index) {
                            (*range.first).second += delta;
                            break;
                        }
                    index++;
                }
            }
        }
        /**
         * Erase the entry for the given index from the internal hash
         * map.  This routine uses a hash table lookup on the object at
         * the given index for efficiency.
         *
         * @param index the array index whose corresponding entry
         * will be removed.
         */
        void eraseIndex(difference_type index) {
            std::pair<typename IndexMap::iterator,
                typename IndexMap::iterator> range =
                indices.equal_range(objects[index]);
            for ( ; range.first != range.second; range.first++)
                if ((*range.first).second == index) {
                    indices.erase(range.first);
                    return;
                }
        }
};

/**
 * See the C++ standard.
 */
template <class Data, class HashFcn, class EqualTo>
inline bool operator == (const NIndexedArray<Data, HashFcn, EqualTo>& array1,
        const NIndexedArray<Data, HashFcn, EqualTo>& array2) {
    return (array1.size() == array2.size()) &&
        equal(array1.begin(), array1.end(), array2.begin(), EqualTo());
}

/**
 * See the C++ standard.
 */
template <class Data, class HashFcn, class EqualTo>
inline bool operator < (const NIndexedArray<Data, HashFcn, EqualTo>& array1,
        const NIndexedArray<Data, HashFcn, EqualTo>& array2) {
    return lexicograhical_compare(array1.begin(), array1,end(),
        array2.begin(), array2.end());
}

} // namespace regina

#endif
