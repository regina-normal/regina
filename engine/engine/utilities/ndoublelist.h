
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

/*! \file ndoublelist.h
 *  \brief Deals with doubly linked lists of objects.
 */

#ifndef __NDOUBLELIST_H
#ifndef __DOXYGEN
#define __NDOUBLELIST_H
#endif

/**
 * For internal use with NDoubleList.
 * Represents a single list element.
 *
 * \ifaces Not present.
 */
template <class T>
struct NDoubleListElement {
    T data;
        /**< The data stored in this list element. */
    NDoubleListElement* prev;
        /**< The previous list element or null if none exists. */
    NDoubleListElement* next;
        /**< The next list element or null if none exists. */
    
    /**
     * Creates a new list element containing the given data.
     *
     * @param newData the data to store in this list element.
     */
    NDoubleListElement(const T& newData) : data(newData) {}
};

template <class T>
class NDoubleListIterator;

/**
 * A doubly linked list of objects of type T.
 * Note that the size of the list is stored as a data member and thus
 * can be calculated in constant time.
 *
 * In general no assumptions will be made about T; thus removing an
 * element from or flushing a list of pointers will not by default
 * cause the object pointed to to be deallocated.  Note however
 * the function flushAndDelete() which will perform this task.
 *
 * \pre Type T has a copy constructor.  That is, if \c a
 * and \c b are of type T, then \c a can be initialised to the value of
 * \c b using <tt>a(b)</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NDoubleList {
    private:
        typedef NDoubleListElement<T> Element;
            /**< Internal type used to store list elements. */
    public:
        typedef NDoubleListIterator<T> Iterator;
            /**< Iterator type used to run through all elements. */

    protected:
        Element* firstElement;
            /**< First element in the list. */
        Element* lastElement;
            /**< Last element in the list. */
        unsigned long nElements;
            /**< Number of elements stored in the list. */

    public:
        /**
         * Creates a new empty list.
         */
        NDoubleList() : firstElement(0), lastElement(0), nElements(0) {
        }
        /**
         * Destroys the list.
         * All elements are removed.  However, if the elements are
         * pointers, the corresponding objects pointed to will not be
         * deleted.  See flushAndDelete() for how to do this.
         */
        virtual ~NDoubleList() {
            flush();
        }
        /**
         * Returns the number of objects contained in the list.
         *
         * @return the size of the list.
         */
        unsigned long size() const {
            return nElements;
        }
        /**
         * Adds the given object to the beginning of the list.
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use addAfter() or addBefore()
         * instead.
         *
         * @param obj the object to add.
         */
        void addFirst(const T& obj) {
            // Create the new list element.
            Element* e = new Element(obj);
            
            // Insert in into the list.
            if (firstElement == 0) {
                firstElement = e;
                lastElement = e;
                e->next = 0;
                e->prev = 0;
            }
            else {
                firstElement->prev = e;
                e->next = firstElement;
                firstElement = e;
                e->prev = 0;
            }
            
            // Increment the list size.
            nElements++;
        }
        /**
         * Adds the given object to the end of the list.
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use addAfter() or addBefore()
         * instead.
         *
         * @param obj the object to add.
         */
        void addLast(const T& obj) {
            // Create the new list element.
            Element* e = new Element(obj);

            // Insert in into the list.
            if (firstElement == 0) {
                firstElement = e;
                lastElement = e;
                e->next = 0;
                e->prev = 0;
            }
            else {
                lastElement->next = e;
                e->prev = lastElement;
                lastElement = e;
                e->next = 0;
            }

            // Increment the list size.
            nElements++;
        }
        /**
         * Inserts the given object into the list immediately after the
         * object pointed to by the given iterator.  After insertion the
         * iterator will still be valid and will remain pointing to the
         * same element as it did before.
         *
         * \pre The given iterator is not past-the-end
         * and in fact points to an element of this list.
         *
         * @param obj the object to add.
         * @param it the position after which to add the object.
         */
        void addAfter(const T& obj, const Iterator& it) {
            Element* e = new Element(obj);
            
            e->prev = it.current;
            e->next = it.current->next;
            it.current->next = e;
            if (e->next)
                e->next->prev = e;
            else
                lastElement = e;

            // Increment the list size.
            nElements++;
        }
        /**
         * Inserts the given object into the list immediately before the
         * object pointed to by the given iterator.  After insertion the
         * iterator will still be valid and will remain pointing to the
         * same element as it did before.
         *
         * \pre The given iterator is not past-the-end
         * and in fact points to an element of this list.
         *
         * @param obj the object to add.
         * @param it the position before which to add the object.
         */
        void addBefore(const T& obj, const Iterator& it) {
            Element* e = new Element(obj);
            
            e->next = it.current;
            e->prev = it.current->prev;
            it.current->prev = e;
            if (e->prev)
                e->prev->next = e;
            else
                firstElement = e;

            // Cast it as non-const so we can change the stored list
            // index.  Note however that we are still pointing at the
            // same object.
            ((Iterator&)it).listPosition++;

            // Increment the list size.
            nElements++;
        }
        /**
         * Inserts the given object into a sorted list in the correct
         * position to ensure that the list remains sorted.
         * The new element will be placed immediately after the last
         * element less than it.
         *
         * Ordering will be determined using the test <tt>a\<b</tt>,
         * where \c a and \c b are of type T.
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use addAfter() or addBefore()
         * instead.
         *
         * \pre Type T allows for the test
         * <tt>a\<b</tt> where \c a and \c b are of type T.
         * \pre This list is sorted according to the
         * above ordering.
         *
         * @param obj the object to add.
         */
        void addSort(const T& obj) {
            Iterator it(*this);

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
         * Inserts the given object into a list of pointers sorted by
         * the data pointed to.  Use this instead of addSort() when
         * using a list of pointers to avoid sorting by position in
         * memory.
         *
         * As with addSort(), the given object will be inserted in the
         * correct position to ensure that the list remains sorted.
         * The new element will be placed immediately after the last
         * element less than it.
         *
         * Ordering will be determined using the test <tt>*a\<*b</tt>,
         * where \c a and \c b are of type T.
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use addAfter() or addBefore()
         * instead.
         *
         * \pre Type T allows for the test
         * <tt>*a\<*b</tt> where \c a and \c b are of type T.
         * \pre This list is sorted according to the above ordering.
         *
         * @param obj the object to add.
         */
        void addSortDereference(const T& obj) {
            Iterator it(*this);
            
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
         * Removes from the list the first object that is equal to the
         * given object.  If no such object can be found, this routine
         * will do nothing.
         *
         * Objects in the list will be compared with the given object
         * using the test <tt>a==b</tt>, where \c a and \c b are of type
         * T.  Note that if T is a pointer type, the pointers themselves
         * will be compared (according to location in memory), as
         * opposed to the data that they point to.
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use remove(Iterator&) instead.
         *
         * \pre Type T supports the test <tt>a==b</tt>,
         * where \c a and \c b are of type T.
         *
         * @param obj the object to search for and remove.
         * @return the removed object, or the given object if nothing was
         * removed.
         */
        T remove(const T& obj) {
            Element* e = firstElement;
            while (e) {
                if (e->data == obj)
                    return remove(e);
                e = e->next;
            }
            return obj;
        }
        /**
         * Removes from the list the object pointed to by the given
         * iterator.  After removal the iterator will point to the next
         * item in the list.  If the removed object was the last in the
         * list, the iterator will now be past-the-end.
         *
         * \pre The given iterator is not past-the-end
         * and is in fact pointing to a member of this list.
         *
         * @param it an iterator pointing to the object to remove.
         * @return the removed object.
         */
        T remove(Iterator& it) {
            Element* e = it.current;
            it.current = it.current->next;
            return remove(e);
        }
        /**
         * Removes from the list the object in the given position.
         * Position <i>p</i> represents the <i>(p+1)</i>th object in the
         * list (so position 0 represents the first object, position 1
         * represents the second and so on).
         *
         * \warning Any iterator currently moving through this
         * list might have its internal list position invalidated.
         * If an iterator is being used, use remove(Iterator&) instead.
         *
         * \pre There is in fact an object at the given
         * position, i.e. \a pos is between 0 and size()-1 inclusive.
         *
         * @param pos the position from which to remove an object.
         * @return the removed object.
         */
        T removeFromPosition(unsigned long pos) {
            Element* e = firstElement;
            for (unsigned long i=0; i<pos; i++)
                e = e->next;
            return remove(e);
        }
        /**
         * Removes all elements from the list.
         * Note that if T is a pointer type, the objects pointed to will
         * not be deallocated; see flushAndDelete() for more details.
         *
         * Any iterator currently moving through this list should be
         * reinitialised before being used again.
         */
        void flush() {
            Element* e;
            while (firstElement) {
                e = firstElement;
                firstElement = firstElement->next;
                delete e;
            }
            // Reset all variables.
            firstElement = 0;
            lastElement = 0;
            nElements = 0;
        }
        /**
         * Removes all elements from a list of pointers and deallocates
         * the corresponding objects.
         * For each object \c t of type T in the list, <tt>delete t</tt>
         * will be called.
         *
         * Any iterator currently moving through this list should be
         * reinitialised before being used again.
         *
         * \pre T is a pointer type.
         */
        void flushAndDelete() {
            Element* e;
            while (firstElement) {
                e = firstElement;
                firstElement = firstElement->next;
                delete e->data;
                delete e;
            }
            // Reset all variables.
            firstElement = 0;
            lastElement = 0;
            nElements = 0;
        }
        /**
         * Finds the position in the list of the first object equal to
         * the given object.
         * Position <i>p</i> represents the <i>(p+1)</i>th object in the
         * list (so position 0 represents the first object, position 1
         * represents the second and so on).
         *
         * Objects in the list will be compared with the given object
         * using the test <tt>a==b</tt>, where \c a and \c b are of type
         * T.  Note that if T is a pointer type, the pointers themselves
         * will be compared (according to location in memory), as
         * opposed to the data that they point to.
         *
         * If you have an iterator pointing to \a obj, use
         * NDoubleListIterator::getListPosition() instead, since this
         * will avoid having to search through the entire list.
         *
         * \pre Type T supports the test <tt>a==b</tt>,
         * where \c a and \c b are of type T.
         *
         * @param obj the object whose equivalent we will search for.
         * @return the position of the first such object in the list, or
         * -1 if no such object is found.
         */
        long position(const T& obj) const {
            Element* e = firstElement;
            long pos = 0;
            while (e) {
                if (e->data == obj)
                    return pos;
                e = e->next;
                pos++;
            }
            return -1;
        }
        /**
         * Returns a reference to the first object in the list.
         *
         * \pre The list is not empty.
         *
         * @return the first object in the list.
         */
        T& getFirst() const {
            return firstElement->data;
        }
        /**
         * Returns a reference to the last object in the list.
         *
         * \pre The list is not empty.
         *
         * @return the last object in the list.
         */
        T& getLast() const {
            return lastElement->data;
        }
        /**
         * Returns the object at the given position in the list.
         * Position <i>p</i> represents the <i>(p+1)</i>th object in the
         * list (so position 0 represents the first object, position 1
         * represents the second and so on).
         *
         * \pre There is in fact an object at the given
         * position, i.e. \a pos is between 0 and size()-1 inclusive.
         *
         * @param pos the position of the requested object.
         * @return a reference to the object at the given position.
         */
        T& operator[](unsigned long pos) const {
            Element* e = firstElement;
            for (unsigned long i=0; i<pos; i++)
                e = e->next;
            return e->data;
        }

    friend class Iterator;

    protected:
        /**
         * Removes the given element from the list.
         *
         * \warning The stored list position of any iterator currently
         * working through this list will not be updated, and
         * might become invalid.
         *
         * \pre The given element actually belongs to this list.
         *
         * @param e the element to remove.
         * @return the removed data.
         */
        T remove(Element* e) {
            T ans = e->data;
            if (e->prev == 0)
                firstElement = e->next;
            else
                e->prev->next = e->next;
            if (e->next == 0)
                lastElement = e->prev;
            else
                e->next->prev = e->prev;
            nElements--;
            delete e;
            return ans;
        }
};

/**
 * An iterator used for running through elements of an NDoubleList.
 * By using initEnd() and operator --, 
 * you can also iterate backwards through a list.
 *
 * \ifaces Not present.
 */
template <class T>
class NDoubleListIterator {
    public:
        typedef NDoubleList<T> List;
            /**< The list class whose elements we can iterate through. */
        typedef NDoubleListElement<T> Element;
            /**< The internal element class used in NDoubleList. */

    protected:
        Element* current;
            /**< The list element currently pointed to, or null if this
             *   iterator is past-the-end. */
        unsigned long listPosition;
            /**< The position in the list of the currently pointed to
             *   element. */

    public:
        /**
         * Creates a new past-the-end iterator.
         */
        NDoubleListIterator() : current(0) {
        }
        /**
         * Creates a new iterator pointing to the first element of the
         * given list.
         *
         * @param basis the list used to initialise this iterator.
         */
        NDoubleListIterator(const List& basis) : current(basis.firstElement),
                listPosition(0) {
        }
        /**
         * Points this iterator to the first element of the given list.
         *
         * @param basis the list used to reinitialise this iterator.
         */
        void init(const List& basis) {
            current = basis.firstElement;
            listPosition = 0;
        }
        /**
         * Points this iterator to the last element of the given list.
         * This is used in conjunction with operator -- to iterate
         * through a list backwards.
         *
         * @param basis the list used to initialise this iterator.
         */
        void initEnd(const List& basis) {
            current = basis.lastElement;
            listPosition = basis.size() - 1;
                // If the unsigned listPosition is trying to become -1,
                // this is okay because the iterator will be past-the-end.
        }
        /**
         * Points this iterator to whatever the given iterator is
         * pointing to.
         *
         * @param cloneMe the iterator whose value will be assigned to
         * this iterator.
         */
        NDoubleListIterator<T>& operator=(
                const NDoubleListIterator<T>& cloneMe) {
            current = cloneMe.current;
            listPosition = cloneMe.listPosition;
            return *this;
        }
        /**
         * Points this iterator at the next list element, or makes it
         * past-the-end if there is no next element.
         *
         * \pre This iterator is not past-the-end.
         */
        void operator++(int) {
            current = current->next;
            listPosition++;
        }
        /**
         * Points this iterator at the previous list element, or makes
         * it past-the-end if there is no previous element.
         *
         * \pre This iterator is not past-the-end.
         */
        void operator--(int) {
            current = current->prev;
            listPosition--;
                // If the unsigned listPosition is 0 this is okay,
                // because the iterator will become past-the-end.
        }
        /**
         * Returns a reference to the data pointed to by this iterator.
         *
         * \pre This iterator is not past-the-end.
         *
         * @return a reference to the data pointed to by this iterator.
         */
        T& operator *() const {
            return current->data;
        }
        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c true if and only if this iterator is past-the-end.
         */
        bool done() const {
            return (current == 0);
        }
        /**
         * Determines the position in the list of the list element
         * currently pointed to.
         * Positions are measured from 0 to NDoubleList::size()-1
         * inclusive.
         * This routine is constant time and thus
         * much faster than NDoubleList::position().
         *
         * \pre This iterator is not past-the-end.
         *
         * @return the position in the list of the element currently
         * pointed to.
         */
        unsigned long getListPosition() const {
            return listPosition;
        }
        
    friend class List;
};

#endif

