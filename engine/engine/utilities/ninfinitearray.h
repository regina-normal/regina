
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file ninfinitearray.h
 *  \brief Deals with infinite arrays of objects.
 */

#ifndef __NINFINITEARRAY_H
#ifndef __DOXYGEN
#define __NINFINITEARRAY_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ndoublelist.h"
    #include "nintmap.h"
#else
    #include "engine/utilities/ndoublelist.h"
    #include "engine/utilities/nintmap.h"
#endif

template <class T>
class NInfiniteArrayIterator;

/**
 * An infinite array of objects of type T.
 * Array indices must be non-negative integers, and not all indices
 * need store a corresponding array element.
 * The array is implemented using a hash table, so insertion, removal
 * and lookup are all fast.
 * Note however that the corresponding iterator class does <b>not</b>
 * run through array elements in any predefined order.
 *
 * \pre Type T has a copy constructor.  That is,
 * if \c a and \c b are of type T, then \c a can be initialised to the value
 * of \c b using <tt>a(b)</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NInfiniteArray {
    public:
        typedef NInfiniteArrayIterator<T> Iterator;
            /**< Iterator type used for running through all array elements. */

    protected:
        NDoubleList<NIntMap<T>*>* hashTable;
            /**< Hash table containing a list for each hash value. */
        unsigned hashTableSize;
            /**< Number of distinct hash values. */
        unsigned long nElements;
            /**< Number of elements stored in the entire array. */

    public:
        /**
         * Creates a new empty array.
         *
         * @param newHashTableSize the number of distinct hash values to
         * allow.  This will default to a reasonable value.  A prime
         * number is advisable.
         */
        NInfiniteArray(unsigned newHashTableSize = 53) :
                hashTable(new NDoubleList<NIntMap<T>*>[newHashTableSize]),
                hashTableSize(newHashTableSize), nElements(0) {
        }

        /**
         * Destroys the array.
         */
        virtual ~NInfiniteArray() {
            for (unsigned i=0; i<hashTableSize; i++)
                hashTable[i].flushAndDelete();
            delete[] hashTable;
        }

        /**
         * Returns the number of elements contained in the array.
         *
         * @return the array size.
         */
        unsigned long size() const {
            return nElements;
        }

        /**
         * Determines if the given array index contains an element.
         * The hash table implementation ensures that this operation is
         * reasonably fast.
         *
         * @param index the index to examine.
         * @return \c true if and only if a value is stored at the given
         * array index.
         */
        bool contains(unsigned long index) const {
            return (findIndex(index) != 0);
        }

        /**
         * Returns the element at the given index in the array.
         * If there is no element at the given index, a new array
         * element will be created at that index and its value will
         * be returned.
         *
         * @param index the array index to examine.
         * @param defaultValue the value to assign to the new array element
         * if there is no element already at the given index.
         * @return the array element at the given index.  If a new element
         * was created, its value will be returned.
         */
        T& elementAt(unsigned long index, const T& defaultValue) {
            NIntMap<T>* map = findIndex(index);
            if (! map) {
                map = new NIntMap<T>(index, defaultValue);
                unsigned hashValue =
                    (unsigned)(index % (unsigned long)hashTableSize);
                hashTable[hashValue].addLast(map);
                nElements++;
            }
            return map->value;
        }

        /**
         * Returns the element at the given index in the array.
         *
         * \pre There is an element at the given index.
         *
         * @param index the array index to examine.
         * @return the array element at the given index.
         */
        T& elementAt(unsigned long index) {
            return findIndex(index)->value;
        }

        /**
         * Removes the array element at the given array index.
         *
         * \pre There is some array element at the given index.
         *
         * @param index the index from which to remove the element.
         * @return the array element that was removed.
         */
        T remove(unsigned long index) {
            nElements--;
            NIntMap<T>* map = findIndex(index, true);
            T ans(map->value);
            delete map;
            return ans;
        }

        /**
         * Removes all elements from the array.
         */
        void flush() {
            for (unsigned i=0; i<hashTableSize; i++)
                hashTable[i].flushAndDelete();
            nElements = 0;
        }

        /**
         * Determines if the given array contains exactly
         * the same information as this array.  This requires
         * each array index of one array to contain the same
         * value as the corresponding array index in the other array,
         * and vice versa.
         *
         * \pre Elements of type T can be tested for
         * equality using the syntax <tt>a == b</tt>.
         *
         * @param compare the array with which this array will be compared.
         * @return \c true if and only if the this and the given array
         * contain identical information.
         */
        bool isIdentical(NInfiniteArray<T>& compare) {
            if (nElements != compare.nElements)
                return false;
            Iterator it(compare);
            NIntMap<T>* map;
            while (! it.done()) {
                map = findIndex(it.index());
                if (! map)
                    return false;
                if (! (map->value == it.value()))
                    return false;
                it++;
            }
            return true;
        }

    private:
        /**
         * Locates and optionally removes the array element
         * at the given index.
         *
         * @param index the array index to examine.
         * @param remove \c true if and only if the element at
         * the given index is to be removed.  If so, the corresponding
         * (index, value) pair will be removed but not destroyed.
         * @return the (index, value) pair at that index, or
         * 0 if there is no element stored at the given index.
         */
        NIntMap<T>* findIndex(unsigned long index, bool remove = false) {
            unsigned hashValue =
                (unsigned)(index % (unsigned long)hashTableSize);
            NIntMap<T>* map;
            NDoubleListIterator<NIntMap<T>*> it(hashTable[hashValue]);
            while (! it.done()) {
                map = *it;
                if (map->index == index) {
                    if (remove)
                        hashTable[hashValue].remove(it);
                    return map;
                }
                it++;
            }
            return 0;
        }
    
    friend class Iterator;
};

/**
 * An iterator used for running through elements of an NInfiniteArray.
 * Note that array elements will not be iterated in any particular order.
 *
 * \ifaces Not present.
 */
template <class T>
class NInfiniteArrayIterator {
    public:
        typedef NInfiniteArray<T> Array;
            /**< The array class whose elements we can iterate through. */

    protected:
        typedef NDoubleList<NIntMap<T>*> List;
            /**< List used by the array class to store elements for an
                 individual hash value. */
        typedef NDoubleListIterator<NIntMap<T>*> ListIterator;
            /**< Iterator for running through elements of a List. */

    protected:
        const Array* whichArray;
            /**< Indicates which array we are looking at. */
        unsigned whichList;
            /**< Indicates which hash value we are currently looking at. */
        ListIterator it;
            /**< Indicates which element of the particular hash value
                 list we are looking at. */

    public:
        /**
         * Creates a new past-the-end iterator.
         */
        NInfiniteArrayIterator() : whichArray(0) {
        }

        /**
         * Creates a new iterator pointing at the "first" element of
         * the given array.  Note that this need not be the element of
         * smallest index.
         *
         * @param basis the array whose elements we will iterate through.
         */
        NInfiniteArrayIterator(const Array& basis) {
            init(basis);
        }

        /**
         * Points this iterator at the "first" element of the given array.
         * Note that this need not be the element of smallest index.
         *
         * @param basis the array whose elements we will iterate through.
         */
        void init(const Array& basis) {
            whichArray = &basis;
            whichList = 0;
            while (whichList < whichArray->hashTableSize)
                if (whichArray->hashTable[whichList].size() == 0)
                    whichList++;
                else
                    break;
            if (whichList < whichArray->hashTableSize)
                it.init(whichArray->hashTable[whichList]);
        }

        /**
         * Moves the iterator to point to the next member of the
         * collection, or makes the iterator past-the-end if no
         * next member exists.  Note that the integer argument is merely
         * to distinguish between <tt>++it</tt> and <tt>it++</tt>, and
         * no actual argument need be passed.
         * Note also that array elements will not be iterated in any
         * particular order; thus this operation need not move to the
         * element of next highest index.
         *
         * \pre The iterator is currently pointing at a
         * valid member of the collection.
         */
        void operator++(int) {
            it++;
            if (it.done()) {
                whichList++;
                while (whichList < whichArray->hashTableSize)
                    if (whichArray->hashTable[whichList].size() == 0)
                        whichList++;
                    else
                        break;
                if (whichList < whichArray->hashTableSize)
                    it.init(whichArray->hashTable[whichList]);
            }
        }

        /**
         * Returns the current array index pointed to by the iterator.
         *
         * \pre The iterator is currently pointing at a
         * valid member of the collection.
         *
         * @return the array index currently pointed to.
         */
        unsigned long index() {
            return (*it)->index;
        }

        /**
         * Returns the value of the current array element pointed to
         * by the iterator.
         *
         * \pre The iterator is currently pointing at a
         * valid member of the collection.
         *
         * @return the value of the array element currently pointed to.
         */
        T& value() {
            return (*it)->value;
        }

        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c false if this iterator points to a valid element
         * of the collection, or \c true if this iterator is past-the-end.
         */
        bool done() {
            if (whichArray == 0)
                return true;
            return (whichList >= whichArray->hashTableSize);
        }
        
    friend class List;
};

#endif

