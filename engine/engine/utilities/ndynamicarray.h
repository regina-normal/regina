
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

/*! \file ndynamicarray.h
 *  \brief Deals with dynamically resizable arrays of objects.
 */

#ifndef __NDYNAMICARRAY_H
#ifndef __DOXYGEN
#define __NDYNAMICARRAY_H
#endif

/**
 * For internal use with NDynamicArray.
 * Represents a single allocated chunk of array elements.
 *
 * \ifaces Not present.
 */
template <class T>
struct NChunk
{
    T* array;
        /**< Data elements stored in this chunk. */
    NChunk* nextChunk;
        /**< The next chunk in the dynamic array, or null if this
         *   is the last. */
    NChunk* prevChunk;
        /**< The previous chunk in the dynamic array, or null if this is
         *   the first. */
        
    /**
     * Creates a new chunk of the given size.
     * Previous and next chunks will be set to null, and all new data
     * elements will be initialised using their default constructors.
     *
     * @param size the number of data elements to store in this chunk.
     */
    NChunk(unsigned size)
    {
        array = new T[size];
        nextChunk = 0;
        prevChunk = 0;
    }
    /**
     * Destroys the chunk and the data contained therein.
     * If T is a pointer type, the data pointed to by each data element
     * will not be deallocated.
     */
    virtual ~NChunk()
    {
        delete []array;
    }
};

template <class T>
class NDynamicArrayIterator;

/**
 * A dynamically resizable array of objects of type T.
 * This class allows flexibility in array size whilst maintaining
 * reasonably fast random access.  The data is stored as a doubly linked
 * list of chunks, where each chunk is a raw array of \a chunkSize
 * elements.  An iterator class is also provided to allow constant time
 * sequential access.
 *
 * Note that insertions and deletions are fast at the end of the array
 * and become progressivly slower as the position approaches the
 * beginning, since elements after the insertion/deletion point will
 * have to be shuffled back and forth.
 *
 * Array indices are numbered from 0 to size()-1 inclusive.
 *
 * \pre Type T has a default constructor and overloads
 * the assignment (<tt>=</tt>) operator.
 *
 * \ifaces Not present.
 */
template <class T>
class NDynamicArray
{
    public:
        typedef NDynamicArrayIterator<T> Iterator;
            /**< Iterator type used to run through all elements. */

    protected:
        unsigned chunkSize;
            /**< Maximum number of data elements stored in each chunk. */
        unsigned long numChunks;
            /**< Number of completely filled chunks.  Note that this
             *   does not count the final chunk, which might be empty.
             *   The final chunk will never be full. */
        unsigned lastChunkSize;
            /**< Number of data elements stored in the final chunk.
             *   This will always be strictly less than \a chunkSize. */
            
        NChunk<T>* firstChunk;
            /**< The first chunk in the array.  This always exists. */
        NChunk<T>* lastChunk;
            /**< The last chunk in the array.  This always exists. */
            
    public:
        /**
         * Creates an empty dynamic array with the given chunk size.
         * The chunk size cannot be modified after creation.
         *
         * \pre The given chunk size must be strictly positive.
         *
         * @param newChunkSize the maximum chunk size for the new array.
         */
        NDynamicArray(unsigned newChunkSize = 10) : chunkSize(newChunkSize),
                numChunks(0), lastChunkSize(0) {
            firstChunk = new NChunk<T>(chunkSize);
            lastChunk = firstChunk;
        }
        /**
         * Destroys the array.
         * All elements are removed.  However, if the elements are
         * pointers, the corresponding objects pointed to will not be
         * deleted.  See flushAndDelete() for how to do this.
         */
        virtual ~NDynamicArray() {
            flush();
            delete firstChunk;
        }
        /**
         * Returns the numberof objects contained in the dynamic array.
         *
         * @return the size of the array.
         */
        unsigned long size() const {
            return (numChunks * (unsigned long)chunkSize) +
                (unsigned long)lastChunkSize;
        }
        /**
         * Adds the given object to the beginning of the array.
         * Note that this is slow since the existing array elements will
         * need to be shuffled forwards.
         *
         * @param obj the object to add.
         */
        void addFirst(const T& obj) {
            insert(obj, firstChunk, 0);
        }
        /**
         * Adds the given object to the end of the array.
         * This can be done in constant time.
         *
         * @param obj the object to add.
         */
        void addLast(const T& obj) {
            // Place obj at the end of lastChunk.
            // FACT: lastChunkSize < chunkSize.
            lastChunk->array[lastChunkSize] = obj;
            lastChunkSize++;
            
            if (lastChunkSize == chunkSize)
            {
                // We must create a new chunk
                lastChunk->nextChunk = new NChunk<T>(chunkSize);
                lastChunk->nextChunk->prevChunk = lastChunk;
                lastChunk = lastChunk->nextChunk;
                numChunks++;
                lastChunkSize = 0;
            }        
        }
        /**
         * Inserts the given object into the array immediately after the
         * object pointed to by the given iterator.  After insertion the
         * iterator will still be valid and will remain pointing to the
         * same element as it did before.
         *
         * \pre The given iterator is not past-the-end
         * and in fact points to an element of this array.
         *
         * @param obj the object to add.
         * @param it the position after which to add the object.
         */
        void addAfter(const T& obj, const NDynamicArrayIterator<T>& it) {
            if (it.currIndex == chunkSize - 1)
                insert(obj, it.currChunk->nextChunk, 0);
            else
                insert(obj, it.currChunk, it.currIndex + 1);
        }
        /**
         * Inserts the given object into the array immediately before the
         * object pointed to by the given iterator.  After insertion the
         * iterator will still be valid and will remain pointing to the
         * same element as it did before.  Note that this will mean the
         * iterator has to change its internal values.
         *
         * \pre The given iterator is not past-the-end
         * and in fact points to an element of this array.
         *
         * @param obj the object to add.
         * @param it the position before which to add the object.
         */
        void addBefore(const T& obj, NDynamicArrayIterator<T>& it) {
            insert(obj, it.currChunk, it.currIndex);
            it++;
        }
        /**
         * Inserts the given object into the array at the given index.
         * All subsequent objects in the array will be pushed towards
         * the end to make room.
         *
         * \pre The given index is between 0 and size() inclusive.
         *
         * @param obj the object to add.
         * @param index the index at which to insert the new object.
         */
        void addAtPosition(const T& obj, unsigned long index) {
            NChunk<T>* chunk = firstChunk;
            while (index >= (unsigned long)chunkSize) {
                chunk = chunk->nextChunk;
                index -= (unsigned long)chunkSize;
            }
            insert(obj, chunk, index);
        }
        /**
         * Inserts the given object into a sorted array in the correct
         * position to ensure that the array remains sorted.
         * The new element will be placed immediately after the last
         * element less than it.
         *
         * Ordering will be determined using the test <tt>a\<b</tt>,
         * where \c a and \c b are of type T.
         *
         * \pre Type T allows for the test
         * <tt>a\<b</tt> where \c a and \c b are of type T.
         * \pre This array is sorted according to the above ordering.
         *
         * @param obj the object to add.
         */
        void addSort(const T& obj) {
            NDynamicArrayIterator<T> it(*this);

            while (! it.done()) {
                if (! (*it < obj))
                    break;
                it++;
            }

            if (it.done())
                addLast(obj);
            else
                addBefore(obj,it);
        }
        /**
         * Inserts the given object into an array of pointers sorted by
         * the data pointed to.  Use this instead of addSort() when
         * using an array of pointers to avoid sorting by position in
         * memory.
         *
         * As with addSort(), the given object will be inserted in the
         * correct position to ensure that the array remains sorted.
         * The new element will be placed immediately after the last
         * element less than it.
         *
         * Ordering will be determined using the test <tt>*a\<*b</tt>,
         * where \c a and \c b are of type T.
         *
         * \pre Type T allows for the test
         * <tt>*a\<*b</tt> where \c a and \c b are of type T.
         * \pre This array is sorted according to the above ordering.
         *
         * @param obj the object to add.
         */
        void addSortDereference(const T& obj) {
            NDynamicArrayIterator<T> it(*this);

            while (! it.done()) {
                if (! (**it < *obj))
                    break;
                it++;
            }

            if (it.done())
                addLast(obj);
            else
                addBefore(obj,it);
        }

        /**
         * Removes from the array the first object that is equal to the
         * given object.  If no such object can be found, this routine
         * will do nothing.
         *
         * Objects in the array will be compared with the given object
         * using the test <tt>a==b</tt>, where \c a and \c b are of type
         * T.  Note that if T is a pointer type, the pointers themselves
         * will be compared (according to location in memory), as
         * opposed to the data that they point to.
         *
         * \pre Type T supports the test <tt>a==b</tt>,
         * where \c a and \c b are of type T.
         *
         * @param obj the object to search for and remove.
         * @return the removed object, or the given object if nothing
         * was removed.
         */
        T remove(const T& obj) {
            NDynamicArrayIterator<T> it(*this);
            while (!it.done()) {
                if ((*it) == obj)
                    return remove(it.currChunk, it.currIndex);
                it++;
            }
            return obj;
        }
        /**
         * Removes from the array the object pointed to by the given
         * iterator.  After removal the iterator will point to the next
         * item in the array.  If the removed object was the last in the
         * array, the iterator will now be past-the-end.
         *
         * \pre The given iterator is not past-the-end
         * and is in fact pointing to a member of this array.
         *
         * @param it an iterator pointing to the object to remove.
         * @return the removed object.
         */
        T remove(NDynamicArrayIterator<T>& it) {
            NDynamicArrayIterator<T> tmpIt(it);
            if (it.currChunk == lastChunk && it.currIndex+1 == lastChunkSize)
                it.currChunk = 0;
            return remove(tmpIt.currChunk, tmpIt.currIndex);
        }

        /**
         * Removes from the array the object at the given index.
         *
         * \pre The given index is between 0 and size()-1 inclusive.
         *
         * @param pos the index from which to remove an object.
         * @return the removed object.
         */
        T removeFromPosition(unsigned long pos) {
            NChunk<T> *currChunk = firstChunk;
            while (pos >= (unsigned long)chunkSize) {
                currChunk = currChunk->nextChunk;
                pos -= (unsigned long)chunkSize;
            }
            return remove(currChunk, pos);
        }

        /**
         * Removes all elements from the array.
         * Note that if T is a pointer type, the objects pointed to will
         * not be deallocated; see flushAndDelete() for more details.
         */
        void flush() {
            // After the flush is complete, there will be exactly one
            // chunk which will be empty.
            
            NChunk<T> *chunk = firstChunk->nextChunk;
            NChunk<T> *tmp;
            while (chunk) {
                tmp = chunk->nextChunk;
                delete chunk;
                chunk = tmp;
            }

            lastChunkSize = 0;
            numChunks = 0;
            lastChunk = firstChunk;
            firstChunk->nextChunk = 0;
        }
        /**
         * Removes all elements from an array of pointers and
         * deallocates the corresponding objects.
         * For each object \c t of type T in the array, <tt>delete t</tt>
         * will be called.
         *
         * \pre T is a pointer type.
         */
        void flushAndDelete() {
            unsigned i;
            if (firstChunk == lastChunk) {
                for (i=0; i<lastChunkSize; i++)
                    delete lastChunk->array[i];
                lastChunkSize = 0;
                return;
            }

            // Clean up and delete the second to second-last chunks.
            NChunk<T> *chunk = firstChunk->nextChunk;
            NChunk<T> *tmp;
            while (chunk != lastChunk) {
                for (i=0; i<chunkSize; i++)
                    delete chunk->array[i];
                tmp = chunk->nextChunk;
                delete chunk;
                chunk = tmp;
            }
            
            // Clean up and delete the last chunk.
            for (i=0; i<lastChunkSize; i++)
                delete lastChunk->array[i];
            delete lastChunk;

            // Clean up the first chunk and finish off.
            for (i=0; i<chunkSize; i++)
                delete firstChunk->array[i];

            lastChunkSize = 0;
            numChunks = 0;
            lastChunk = firstChunk;
            firstChunk->nextChunk = 0;
        }
        /**
         * Finds the index in the array of the first object equal to the
         * given object.
         *
         * Objects in the array will be compared with the given object
         * using the test <tt>a==b</tt>, where \c a and \c b are of type
         * T.  Note that if T is a pointer type, the pointers themselves
         * will be compared (according to location in memory), as
         * opposed to the data that they point to.
         *
         * \pre Type T supports the test <tt>a==b</tt>,
         * where \c a and \c b are of type T.
         *
         * @param obj the object whose equivalent we will search for.
         * @return the index of the first such object in the array
         * (between 0 and size()-1 inclusive), or -1 if no such object
         * is found.
         */
        long position(const T& obj) const {                       
            NDynamicArrayIterator<T> it(*this);
            while (!it.done()) {
                if ((*it) == obj)
                    return it.getArrayIndex();
                it++;
            }
            return -1;
        }
        /**
         * Returns the object at the given index in the array.
         *
         * \pre The given index is between 0 and size()-1 inclusive.
         *
         * @param index the index of the requested object.
         * @return a reference to the object at the given position.
         */
        T& operator[](unsigned long index) const {
            NChunk<T> *chunk = firstChunk;
            
            // Move to the correct chunk
            while (index >= (unsigned long)chunkSize) {
                index -= (unsigned long)chunkSize;
                chunk = chunk->nextChunk;
            }
            
            // Return the requested element from this chunk.
            return chunk->array[index];
        }

    friend class NDynamicArrayIterator<T>;
        /**< Allow access to private members. */

    protected:
        /**
         * Insert the given element into the given index of the given
         * chunk.  All subsequent elements of the array will be shuffled
         * towards the end and a new last chunk will be created if
         * necessary.
         *
         * \pre The given chunk is one of the chunks in this array.
         * \pre If the given chunk is \a lastChunk, the
         * given index is between 0 and \a lastChunkSize inclusive.
         * Otherwise the given index is between 0 and <i>chunkSize</i>-1
         * inclusive.
         *
         * @param obj the element to insert.
         * @param chunk the chunk in which to insert the new element.
         * @param chunkIndex the index within \a chunk at which to insert
         * the new element.
         */
        void insert(const T& obj, NChunk<T>* chunk, unsigned chunkIndex) {
            // Shuffle everything towards the end.
            NChunk<T>* ch = lastChunk;
            unsigned i = lastChunkSize;
            while ((ch != chunk) || (i > chunkIndex)) {
                if (i == 0) {
                    ch->array[i] = ch->prevChunk->array[chunkSize-1];
                    ch = ch->prevChunk;
                    i = chunkSize - 1;
                } else {
                    ch->array[i] = ch->array[i-1];
                    i--;
                }
            }

            // Insert the new element.
            chunk->array[chunkIndex] = obj;

            // Sort out details with the last chunk.
            if (lastChunkSize == chunkSize-1) {
                lastChunk->nextChunk = new NChunk<T>(chunkSize);
                lastChunk->nextChunk->prevChunk = lastChunk;
                lastChunk = lastChunk->nextChunk;
                numChunks++;
                lastChunkSize = 0;
            } else
                lastChunkSize++;
        }
        /**
         * Removes the element from the given index within the given
         * chunk.  All later elements will be shuffled towards the front 
         * and the last chunk will be deleted if necessary.
         *
         * \pre Such an element exists in the array.
         *
         * @param chunk the chunk from which to remove an element.
         * @param chunkIndex the index of the element within the chunk
         * to remove; this should be between 0 and <i>chunkSize</i>-1
         * inclusive.  If \a chunk is the last chunk, this should in
         * fact be less than \a lastChunkSize.
         * @return the removed element.
         */
        T remove(NChunk<T>* chunk, unsigned chunkIndex) {
            // Remember the value that was removed.
            T old = chunk->array[chunkIndex];

            // Shift everything back.
            while ((chunk != lastChunk) || (chunkIndex+1 < lastChunkSize)) {
                if (chunkIndex < chunkSize-1) {
                    chunk->array[chunkIndex] = chunk->array[chunkIndex+1];
                    chunkIndex++;
                } else if (chunk->nextChunk == lastChunk && lastChunkSize == 0)
                    break;
                else {
                    chunk->array[chunkIndex] = chunk->nextChunk->array[0];
                    chunk = chunk->nextChunk;
                    chunkIndex = 0;
                }
            }

            // Sort out the last chunk details.
            if (lastChunkSize == 0) {
                lastChunk = lastChunk->prevChunk;
                delete lastChunk->nextChunk;
                lastChunk->nextChunk = 0;
                lastChunkSize = chunkSize - 1;
                numChunks--;
            } else
                lastChunkSize--;

            // Return.
            return old;
        }
};

/**
 * An iterator used for running through elements of an NDynamicArray.
 * By using initEnd() and operator --,
 * you can also iterate backwards through an array.
 *
 * \ifaces Not present.
 */
template <class T>
class NDynamicArrayIterator
{
    protected:
        const NDynamicArray<T> *array;
            /**< The dynamic array to iterate through. */
        NChunk<T> *currChunk;
            /**< The chunk containing the current element, or null if we
             *   are past-the-end. */
        unsigned currIndex;
            /**< Index of the current element in \a currChunk.  This is
             *   between 0 and <tt>array->chunkSize</tt> inclusive. */
        unsigned long currArrayIndex;
            /**< Index of the current element in the entire array. */
    
    public:
        /**
         * Creates a new past-the-end iterator.
         */
        NDynamicArrayIterator() : currChunk(0) {
        }
        /**
         * Creates a new iterator pointing at the first element of the
         * given array.
         *
         * @param basis the array used to initialise the iterator.
         */
        NDynamicArrayIterator(const NDynamicArray<T>& basis) {
            init(basis);
        }
        /**
         * Creates a new iterator pointing at the same element as the
         * given iterator.
         *
         * @param it the iterator to clone.
         */
        NDynamicArrayIterator(const NDynamicArrayIterator<T> &it) :
                array(it.array), currChunk(it.currChunk),
                currIndex(it.currIndex), currArrayIndex(it.currArrayIndex) {
        }
        /**
         * Points this iterator to the first element of the given array.
         *
         * @param basis the array used to reinitialise this iterator.
         */
        void init(const NDynamicArray<T>& basis) {
            if (basis.numChunks == 0 && basis.lastChunkSize == 0)
                currChunk = 0;    // The array is empty
            else {
                array = &basis;
                currChunk = basis.firstChunk;
                currIndex = 0;
                currArrayIndex = 0;
            }
        }
        /**
         * Points this iterator to the last element of the given array.
         * This is used in conjunction with operator -- to iterate
         * through an array backwards.
         *
         * @param basis the array used to reinitialise this iterator.
         */
        void initEnd(const NDynamicArray<T>& basis) {
            if (basis.lastChunkSize == 0) {
                if (basis.numChunks == 0) {
                    currChunk = 0;   // Empty array
                    return;
                } else {
                    currChunk = basis.lastChunk->prevChunk;
                    currIndex = basis.chunkSize - 1;
                }
                // Go to the last element of the second last chunk.
            } else {
                currChunk = basis.lastChunk;
                currIndex = basis.lastChunkSize - 1;
            }
            array = &basis;
            currArrayIndex = basis.size() - 1;
        }
        /**
         * Points this iterator to whatever the given iterator is
         * pointing to.
         *
         * @param cloneMe the iterator whose value will be assigned to
         * this iterator.
         */
        NDynamicArrayIterator<T>& operator=(
                const NDynamicArrayIterator<T>& cloneMe) {
            array = cloneMe.array;
            currChunk = cloneMe.currChunk;
            currIndex = cloneMe.currIndex;
            currArrayIndex = cloneMe.currArrayIndex;
            return *this;
        }
        /**
         * Points this iterator at the next array element, or makes it
         * past-the-end if there is no next element.
         *
         * \pre This iterator is not past-the-end.
         */
        void operator++(int) {
            currArrayIndex++;
            if (currChunk != array->lastChunk) {
                if (currIndex == array->chunkSize - 1) {
                    currIndex = 0;
                    currChunk = currChunk->nextChunk;
                    if (currChunk != array->lastChunk)
                        return;
                } else {
                    currIndex++;
                    return;
                }
            } else
                currIndex++;
                
            // We are currently in the last chunk pointing at the
            // correct next element.
            // Check to make sure we haven't gone past the end.
            if (currIndex == array->lastChunkSize)
                currChunk = 0;
        }
        /**
         * Points this iterator at the previous array element, or makes
         * it past-the-end if there is no previous element.
         *
         * \pre This iterator is not past-the-end.
         */
        void operator--(int) {
            currArrayIndex--;
                // If unsigned currArrayIndex is 0, that is okay because
                // the iterator will become past-the-end.
            if (currIndex == 0) {
                if (currChunk->prevChunk == 0)
                    currChunk = 0;
                else {
                    currChunk = currChunk->prevChunk;
                    currIndex = array->chunkSize - 1;
                }
            } else
                currIndex--;
        }
        /**
         * Returns a reference to the data pointed to by this iterator.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return a reference to the data pointed to by this iterator.
         */
        T& operator *() const {
            return currChunk->array[currIndex];
        }
        /**
         * Returns the index in the array of the data pointed to by this
         * iterator.  The index will be between 0 and the array size
         * minus one inclusive.  This routine is much faster than
         * dereferencing the iterator and calling NDynamicArray::position().
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the array index of the data pointed to by this
         * iterator.
         */
        unsigned long getArrayIndex() const {
            return currArrayIndex;
        }
        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c true if and only if this iterator is past-the-end.
         */
        bool done() const {
            return (currChunk == 0);
        }

    friend class NDynamicArray<T>;
        /**< Allow access to private members. */
};

#endif
