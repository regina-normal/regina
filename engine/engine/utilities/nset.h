
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

/*! \file nset.h
 *  \brief Deals with sets of objects.
 */

#ifndef __NSET_H
#ifndef __DOXYGEN
#define __NSET_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ndoublelist.h"
#else
    #include "engine/utilities/ndoublelist.h"
#endif

/**
 * Recasts a void pointer as an unsigned integer, even if the compiler does
 * not allow this.  Note that \a p must be a variable name so that we can
 * find its memory address.
 */
#define PVOID_TO_UNSIGNED(p) (*((unsigned*)((void*)&(p))))

template <class T>
class NPointerSetIterator;

/**
 * A set of pointers to objects of type \c T designed for speed.
 * The set is implemented using a hash table, so insertion, removal and
 * lookup are all fast.
 *
 * \ifaces Not present.
 */
template <class T>
class NPointerSet {
    public:
        typedef NPointerSetIterator<T> Iterator;
            /**< Iterator type used for running through all set elements. */

    protected:
        NDoubleList<T*>* hashTable;
            /**< Hash table containing a list for each hash value. */
        unsigned hashTableSize;
            /**< Number of distinct hash values. */
        unsigned long nElements;
            /**< Number of elements stored in the entire set. */

    public:
        /**
         * Creates a new empty set.
         *
         * @param newHashTableSize the number of distinct hash values to
         * allow.  This will default to a reasonable value.  A prime
         * number is advisable.
         */
        NPointerSet(unsigned newHashTableSize = 53) :
                hashTable(new NDoubleList<T*>[newHashTableSize]),
                hashTableSize(newHashTableSize), nElements(0) {
        }

        /**
         * Destroys the set.  The objects to which individual set
         * elements point will not be deallocated.
         */
        virtual ~NPointerSet() {
            delete[] hashTable;
        }

        /**
         * Returns the number of elements contained in the set.
         *
         * @return the set size.
         */
        unsigned long size() const {
            return nElements;
        }

        /**
         * Determines if the given pointer is contained in the set.
         * The hash table implementation ensures that this operation is
         * reasonably fast.
         *
         * @param ptr the pointer to search for.
         * @return \c true if and only if the given pointer was found.
         */
        bool contains(T* ptr) const {
            unsigned hashValue = PVOID_TO_UNSIGNED(ptr) % hashTableSize;
            return (hashTable[hashValue].position(ptr) >= 0);
        }

        /**
         * Adds the given pointer to the set.
         * If the given pointer is already present in the set, this
         * routine does nothing.
         *
         * @param ptr the pointer to add to the set.
         */
        void add(T* ptr) {
            unsigned hashValue = PVOID_TO_UNSIGNED(ptr) % hashTableSize;
            if (hashTable[hashValue].position(ptr) < 0) {
                hashTable[hashValue].addLast(ptr);
                nElements++;
            }
        }

        /**
         * Removes the given pointer from the set.
         * Note that the object pointed to is not deallocated.
         *
         * \pre The given pointer is already present in the set.
         *
         * @param ptr the pointer to remove from the set.
         * @return the pointer that was removed.
         */
        T* remove(T* ptr) {
            nElements--;
            unsigned hashValue = PVOID_TO_UNSIGNED(ptr) % hashTableSize;
            return hashTable[hashValue].remove(ptr);
        }

        /**
         * Removes all elements from the set.
         * Note that the objects pointed to by the removed pointers
         * are not deallocated.
         */
        void flush() {
            for (unsigned i=0; i<hashTableSize; i++)
                hashTable[i].flush();
            nElements = 0;
        }
    
    friend class Iterator;
        /**< Allow access to private members. */
};

/**
 * An iterator used for running through elements of an NPointerSet.
 *
 * \ifaces Not present.
 */
template <class T>
class NPointerSetIterator {
    public:
        typedef NPointerSet<T> Set;
            /**< The set class whose elements we can iterate through. */

    protected:
        typedef NDoubleList<T*> List;
            /**< List used by the set class to store pointers for an
                 individual hash value. */
        typedef NDoubleListIterator<T*> ListIterator;
            /**< Iterator for running through elements of a List. */

    protected:
        const Set* whichSet;
            /**< Indicates which set we are looking at. */
        unsigned whichList;
            /**< Indicates which hash value we are currently looking at. */
        ListIterator it;
            /**< Indicates which element of the particular hash value
                 list we are looking at. */

    public:
        /**
         * Creates a new past-the-end iterator.
         */
        NPointerSetIterator() : whichSet(0) {
        }

        /**
         * Creates a new iterator pointing at the first element of
         * the given set.
         *
         * @param basis the set whose elements we will iterate through.
         */
        NPointerSetIterator(const Set& basis) {
            init(basis);
        }

        /**
         * Points this iterator at the first element of the given set.
         *
         * @param basis the set whose elements we will iterate through.
         */
        void init(const Set& basis) {
            whichSet = &basis;
            whichList = 0;
            while (whichList < whichSet->hashTableSize)
                if (whichSet->hashTable[whichList].size() == 0)
                    whichList++;
                else
                    break;
            if (whichList < whichSet->hashTableSize)
                it.init(whichSet->hashTable[whichList]);
        }

        /**
         * Moves the iterator to point to the next member of the
         * collection, or makes the iterator past-the-end if no
         * next member exists.  Note that the integer argument is merely
         * to distinguish between <tt>++it</tt> and <tt>it++</tt>, and
         * no actual argument need be passed.
         *
         * \pre The iterator is currently pointing at a
         * valid member of the collection.
         */
        void operator++(int) {
            it++;
            if (it.done()) {
                whichList++;
                while (whichList < whichSet->hashTableSize)
                    if (whichSet->hashTable[whichList].size() == 0)
                        whichList++;
                    else
                        break;
                if (whichList < whichSet->hashTableSize)
                    it.init(whichSet->hashTable[whichList]);
            }
        }

        /**
         * Returns the current set element pointed to by the iterator.
         *
         * \pre The iterator is currently pointing at a
         * valid member of the collection.
         *
         * @return the element currently pointed to.
         */
        T* operator *() {
            return *it;
        }

        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c false if this iterator points to a valid element
         * of the collection, or \c true if this iterator is past-the-end.
         */
        bool done() {
            if (whichSet == 0)
                return true;
            return (whichList >= whichSet->hashTableSize);
        }
        
    friend class List;
        /**< Allow access to private members. */
};

#endif

