
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nindexedarray.h
 *  \brief Deals with arrays of objects with fast object-to-index lookup.
 */

#ifndef __NINDEXEDARRAY_H
#ifndef __DOXYGEN
#define __NINDEXEDARRAY_H
#endif

#include <iostream>
#include <vector>
#include "utilities/hashmap.h"
#include "utilities/hashutils.h"

#ifdef DEBUG_NINDEXEDARRAY
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY(where) \
        if (! validate()) \
            std::cerr << "Error noticed in: " << where << std::endl;
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY_TOP bool __valid = validate(true);
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY_BOTTOM(where) \
        if (__valid) \
            if (! validate()) \
                std::cerr << "Error created in: " << where << std::endl;
#else
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY(where)
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY_TOP
    /**
     * \hideinitializer
     * An internal macro to assist with debugging.
     */
    #define VALIDATE_NINDEXEDARRAY_BOTTOM(where)
#endif

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

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
 * Additional routines beyond the C++ standard requirements include index(),
 * erase(const_reference) and validate().
 *
 * Template parameter <tt>HashFcn</tt> will be used to generate hash
 * values for array elements.
 * Template parameter <tt>EqualTo</tt> will be used to compare array
 * elements when looking up the corresponding array index.
 *
 * \pre Template parameter <tt>HashFcn</tt> satisfies the requirements of
 * a hash function (with argument type <tt>Data</tt>) according to the
 * Standard Template Library.
 *
 * \testfull
 *
 * \ifacespython Not present.
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
            VALIDATE_NINDEXEDARRAY("NIndexedArray(size_type)")
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray(size_type n, const Data& t) : objects(n, t) {
            insertIndices(objects.begin(), objects.end());
            VALIDATE_NINDEXEDARRAY("NIndexedArray(size_type, const Data&)")
        }
        /**
         * See the C++ standard.
         */
        NIndexedArray(const NIndexedArray<Data, HashFcn, EqualTo>& array) :
                objects(array.objects), indices(array.indices) {
            VALIDATE_NINDEXEDARRAY("NIndexedArray(const NIndexedArray&)")
        }
        /**
         * See the C++ standard.
         */
        template <class InputIterator>
        NIndexedArray(InputIterator first, InputIterator last) :
                objects(first, last) {
            insertIndices(objects.begin(), objects.end());
            VALIDATE_NINDEXEDARRAY(
                "NIndexedArray(InputIterator, InputIterator)")
        }
        /**
         * See the C++ standard.
         */
        ~NIndexedArray() {
            VALIDATE_NINDEXEDARRAY("~NIndexedArray")
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
            VALIDATE_NINDEXEDARRAY_TOP
            objects = array.objects;
            indices = array.indices;
            VALIDATE_NINDEXEDARRAY_BOTTOM("operator =")
            return *this;
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
            VALIDATE_NINDEXEDARRAY_TOP
            indices.insert(std::make_pair(item, objects.size()));
            objects.push_back(item);
            VALIDATE_NINDEXEDARRAY_BOTTOM("push_back")
        }
        /**
         * See the C++ standard.
         */
        void pop_back() {
            VALIDATE_NINDEXEDARRAY_TOP
            eraseIndex(objects.size() - 1);
            objects.pop_back();
            VALIDATE_NINDEXEDARRAY_BOTTOM("pop_back")
        }
        /**
         * See the C++ standard.
         */
        void swap(NIndexedArray<Data, HashFcn>& array) {
            VALIDATE_NINDEXEDARRAY_TOP
            objects.swap(array.objects);
            indices.swap(array.indices);
            VALIDATE_NINDEXEDARRAY_BOTTOM("swap")
        }
        /**
         * See the C++ standard.
         */
        iterator insert(iterator pos, const Data& x) {
            VALIDATE_NINDEXEDARRAY_TOP
            incrementIndices(pos, objects.end(), 1);
            pos = objects.insert(convertIterator(pos), x);
            insertIndices(pos, pos + 1);
            VALIDATE_NINDEXEDARRAY_BOTTOM("insert(iterator, const Data&)")
            return pos;
        }
        /**
         * See the C++ standard.
         */
        template <class InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last) {
            VALIDATE_NINDEXEDARRAY_TOP
            difference_type posIndex = pos - objects.begin();
            difference_type newElts = last - first;

            incrementIndices(pos, objects.end(), newElts);
            objects.insert(convertIterator(pos), first, last);
            pos = objects.begin() + posIndex;
            insertIndices(pos, pos + newElts);
            VALIDATE_NINDEXEDARRAY_BOTTOM(
                "insert(iterator, InputIterator, InputIterator)")
        }
        /**
         * See the C++ standard.
         */
        void insert(iterator pos, size_type n, const Data& x) {
            VALIDATE_NINDEXEDARRAY_TOP
            difference_type posIndex = pos - objects.begin();

            incrementIndices(pos, objects.end(), n);
            objects.insert(convertIterator(pos), n, x);
            pos = objects.begin() + posIndex;
            insertIndices(pos, pos + n);
            VALIDATE_NINDEXEDARRAY_BOTTOM(
                "insert(iterator, size_type, const Data&)")
        }
        /**
         * See the C++ standard.
         */
        iterator erase(iterator pos) {
            VALIDATE_NINDEXEDARRAY_TOP
            incrementIndices(pos + 1, objects.end(), -1);
            eraseIndex(pos - objects.begin());
            iterator retVal = objects.erase(convertIterator(pos));
            VALIDATE_NINDEXEDARRAY_BOTTOM("erase(iterator)")
            return retVal;
        }
        /**
         * See the C++ standard.
         */
        iterator erase(iterator first, iterator last) {
            VALIDATE_NINDEXEDARRAY_TOP
            difference_type lostElts = last - first;
            incrementIndices(last, objects.end(), -lostElts);

            size_type index = first - objects.begin();
            for (iterator it = first; it != last; it++)
                eraseIndex(index++);

            iterator retVal =
                objects.erase(convertIterator(first), convertIterator(last));
            VALIDATE_NINDEXEDARRAY_BOTTOM("erase(iterator, iterator)")
            return retVal;
        }
        /**
         * See the C++ standard.
         */
        void clear() {
            VALIDATE_NINDEXEDARRAY_TOP
            objects.clear();
            indices.clear();
            VALIDATE_NINDEXEDARRAY_BOTTOM("clear")
        }
        /**
         * See the C++ standard.
         */
        void resize(size_type n) {
            if (n == objects.size())
                return;
            VALIDATE_NINDEXEDARRAY_TOP
            if (n > objects.size()) {
                // Increase the size.
                insert(objects.end(), n - objects.size(), Data());
            } else {
                // Decrease the size.
                erase(objects.begin() + n, objects.end());
            }
            VALIDATE_NINDEXEDARRAY_BOTTOM("resize(size_type)")
        }
        /**
         * See the C++ standard.
         */
        void resize(size_type n, const Data& t) {
            if (n == objects.size())
                return;
            VALIDATE_NINDEXEDARRAY_TOP
            if (n > objects.size()) {
                // Increase the size.
                insert(objects.end(), n - objects.size(), t);
            } else {
                // Decrease the size.
                erase(objects.begin() + n, objects.end());
            }
            VALIDATE_NINDEXEDARRAY_BOTTOM("resize(size_type, const Data&)")
        }

        /**
         * Finds the index of the given value in the array.
         *
         * This routine is made quite fast through use of the internal
         * hashed dictionary.
         *
         * If the given value is stored more than once in the array,
         * one of its indices will be returned but there is no guarantee
         * as to which of these indices it will be.
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
            VALIDATE_NINDEXEDARRAY_TOP
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
            VALIDATE_NINDEXEDARRAY_BOTTOM("erase(const_reference)")
        }

        /**
         * Checks the structural integrity of this array.
         *
         * The internal hashed dictionary is compared with the internal
         * array to ensure they are consistent with one another.
         *
         * Any inconsistencies are written to standard error (unless
         * parameter \a silent is passed as \c true).
         *
         * @param silent \c true if error reporting should be suppressed, or
         * \c false (the default) if errors should be reported to standard
         * error as described above.  Either way, the return value can
         * still be used to determine if any inconsistencies were
         * discovered.
         * @return \c true if no problems were found, or \c false if
         * any inconsistencies were discovered.
         */
        bool validate(bool silent = false) const {
            bool ok = true;
            difference_type index;

            // Check the container sizes.
            if (objects.size() != indices.size()) {
                if (! silent) {
                    std::cerr <<
                        "ERR: Internal containers have different sizes.\n";
                    std::cerr << "Array size: " << objects.size() << '\n';
                    std::cerr << "Dictionary size: " << indices.size()
                        << std::endl;
                }
                ok = false;
            }

            // Check that each element of the hashed dictionary appears
            // in the vector.
            typename IndexMap::const_iterator hashIt;
            for (hashIt = indices.begin(); hashIt != indices.end(); hashIt++) {
                index = (*hashIt).second;
                if (index < 0 || index >=
                        static_cast<difference_type>(objects.size())) {
                    if (! silent) {
                        std::cerr << "ERR: Invalid value in dictionary.\n";
                        std::cerr <<
                            "Dictionary value (should be array index): "
                            << index << '\n';
                        std::cerr << "Array size: " << objects.size()
                            << std::endl;
                    }
                    ok = false;
                } else if (! (objects[index] == (*hashIt).first)) {
                    if (! silent) {
                        std::cerr << "ERR: Dictionary key != array value.\n";
                        std::cerr << "Dictionary value / array index: "
                            << index << std::endl;
                    }
                    ok = false;
                }
            }

            // Check that each element of the vector appears in the
            // hashed dictionary.
            typename ObjectArray::const_iterator arrIt;
            std::pair<typename IndexMap::const_iterator,
                typename IndexMap::const_iterator> range;
            unsigned foundCount;

            index = 0;
            for (arrIt = objects.begin(); arrIt != objects.end(); arrIt++) {
                // Count the number of matching elements in the hashed
                // dictionary.
                range = indices.equal_range(*arrIt);
                foundCount = 0;
                for ( ; range.first != range.second; range.first++)
                    if ((*range.first).second == index)
                        foundCount++;

                if (foundCount == 0) {
                    if (! silent) {
                        std::cerr << "ERR: Array element not in dictionary.\n";
                        std::cerr << "Array index: " << index << std::endl;
                    }
                    ok = false;
                } else if (foundCount > 1) {
                    if (! silent) {
                        std::cerr << "ERR: Duplicate entries in dictionary.\n";
                        std::cerr << "Array index: " << index << '\n';
                        std::cerr << "Number of duplicates: " << foundCount
                            << std::endl;
                    }
                    ok = false;
                }

                index++;
            }

            // Finally check the reported size.
            if (index != static_cast<difference_type>(size())) {
                if (! silent) {
                    std::cerr << "ERR: Reported array size is incorrect.\n";
                    std::cerr << "Reported size: " << size() << '\n';
                    std::cerr << "Number of elements found: " << index << '\n';
                }
                ok = false;
            }

            // All done.
            return ok;
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
    return lexicographical_compare(array1.begin(), array1.end(),
        array2.begin(), array2.end());
}

/*@}*/

} // namespace regina

#endif
