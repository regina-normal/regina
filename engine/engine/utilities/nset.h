
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

#include "utilities/ndoublelist.h"
#include "utilities/nstring.h"

/**
 * Recasts a void pointer as an unsigned integer, even if the compiler does
 * not allow this.  Note that \a p must be a variable name so that we can
 * find its memory address.
 */
#define PVOID_TO_UNSIGNED(p) (*((unsigned*)((void*)&(p))))

/**
 * Computes a hash value for the given pointer.
 *
 * @param obj the object whose hash value should be computed.
 * @return the corresponding hash value.
 */
template <class T>
unsigned hashMap(const T* obj) {
    return PVOID_TO_UNSIGNED(obj);
}

/**
 * Computes a hash value for the given string.
 *
 * @param str the string whose hash value should be computed.
 * @return the corresponding hash value.
 */
unsigned hashMap(const NString& str);

template <class T>
class NHashSetIterator;

/**
 * A set of objects of type \c T designed for speed.
 * The set is implemented using a hash table, so insertion, removal and
 * lookup are all fast.
 *
 * \pre Calling <tt>hashMap(t)</tt> will return a hash value for an
 * object <tt>t</tt> of type <tt>T</tt>.
 * \pre Type \c T has a default constructor and overloads the assignment
 * (<tt>=</tt>) operator.
 * \pre Type \c T supports the test <tt>a==b</tt>, where \c a and \c b
 * are of type <tt>T</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NHashSet {
    public:
        typedef NHashSetIterator<T> Iterator;
            /**< Iterator type used for running through all set elements. */

    protected:
        NDoubleList<T>* hashTable;
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
        NHashSet(unsigned newHashTableSize = 53) :
                hashTable(new NDoubleList<T>[newHashTableSize]),
                hashTableSize(newHashTableSize), nElements(0) {
        }

        /**
         * Destroys the set.  If individual elements are pointers,
         * the objects to which they point will not be deallocated.
         */
        virtual ~NHashSet() {
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
         * Determines if the given object is contained in the set.
         * The hash table implementation ensures that this operation is
         * reasonably fast.
         *
         * @param obj the object to search for.
         * @return \c true if and only if the given object was found.
         */
        bool contains(const T& obj) const {
            unsigned hashValue = hashMap(obj) % hashTableSize;
            return (hashTable[hashValue].position(obj) >= 0);
        }

        /**
         * Adds the given object to the set.
         * If the given object is already present in the set, this
         * routine does nothing.
         *
         * @param obj the object to add to the set.
         * @return \c true if the given object was added to the set, or
         * \c false if the given object was already present in the set.
         */
        bool add(const T& obj) {
            unsigned hashValue = hashMap(obj) % hashTableSize;
            if (hashTable[hashValue].position(obj) < 0) {
                hashTable[hashValue].addLast(obj);
                nElements++;
                return true;
            } else
                return false;
        }

        /**
         * Removes the given object from the set.
         * If the given object is a pointer, the object to which it
         * points will not be deallocated.
         *
         * \pre The given object is already present in the set.
         *
         * @param obj the object to remove from the set.
         * @return the object that was removed.
         */
        T remove(const T& obj) {
            nElements--;
            unsigned hashValue = hashMap(obj) % hashTableSize;
            return hashTable[hashValue].remove(obj);
        }

        /**
         * Removes all elements from the set.  If individual elements are
         * pointers, the objects to which they point will not be deallocated.
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
 * An iterator used for running through elements of an NHashSet.
 *
 * \ifaces Not present.
 */
template <class T>
class NHashSetIterator {
    public:
        typedef NHashSet<T> Set;
            /**< The set class whose elements we can iterate through. */

    protected:
        typedef NDoubleList<T> List;
            /**< List used by the set class to store objects for an
                 individual hash value. */
        typedef NDoubleListIterator<T> ListIterator;
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
        NHashSetIterator() : whichSet(0) {
        }

        /**
         * Creates a new iterator pointing at the first element of
         * the given set.
         *
         * @param basis the set whose elements we will iterate through.
         */
        NHashSetIterator(const Set& basis) {
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
        T& operator *() const {
            return *it;
        }

        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c false if this iterator points to a valid element
         * of the collection, or \c true if this iterator is past-the-end.
         */
        bool done() const {
            if (whichSet == 0)
                return true;
            return (whichList >= whichSet->hashTableSize);
        }
        
    friend class List;
        /**< Allow access to private members. */
};

/**
 * A set of pointers to objects of type \c T implemented using a hash table.
 *
 * \ifaces Not present.
 */
template <class T>
class NPointerSet : public NHashSet<T*> {
    public:
        /**
         * Creates a new empty set.
         *
         * @param newHashTableSize the number of distinct hash values to
         * allow.  This will default to a reasonable value.  A prime
         * number is advisable.
         */
        NPointerSet(unsigned newHashTableSize = 53) :
                NHashSet<T*>(newHashTableSize) {
        }
};

/**
 * An iterator used for running through elements of an NPointerSet.
 *
 * \ifaces Not present.
 */
template <class T>
class NPointerSetIterator : public NHashSetIterator<T*> {
    public:
        /**
         * Creates a new past-the-end iterator.
         */
        NPointerSetIterator() : NHashSetIterator<T*>() {
        }

        /**
         * Creates a new iterator pointing at the first element of
         * the given set.
         *
         * @param basis the set whose elements we will iterate through.
         */
        NPointerSetIterator(const Set& basis) : NHashSetIterator<T*>(basis) {
        }
};

/**
 * A set of strings implemented using a hash table.
 *
 * \ifaces Not present.
 */
typedef NHashSet<NString> NStringSet;

/**
 * An iterator used for running through elements of an NStringSet.
 *
 * \ifaces Not present.
 */
typedef NHashSetIterator<NString> NStringSetIterator;

#endif

