
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

/*! \file nqueue.h
 *  \brief Deals with queues of objects.
 */

#ifndef __NQUEUE_H
#ifndef __DOXYGEN
#define __NQUEUE_H
#endif

/**
 * For internal use with NQueue.
 * Wrapper class for a single queue element containing an object of type \c T.
 *
 * \ifaces Not present.
 */
template <class T>
struct NQueueElement {
    T data;
        /**< The object stored at this position in the queue. */
    NQueueElement* prev;
        /**< The NQueueElement behind this one in the queue, or
             0 if this element is at the tail. */
    
    /**
     * Creates a new queue element containing the given data.
     * 
     * @param newData the data to be stored by this queue element.
     */
    NQueueElement(const T& newData) : data(newData) {
    }
};

/**
 * A simple first-in-first-out queue of objects of type \c T.
 * 
 * Note that no assumptions are made about \c T; thus if \c T is
 * a pointer type, \e NQueue itself will not deallocate removed objects.
 *
 * \pre Type T has a copy constructor.  That is, if \c a
 * and \c b are of type T, then \c a can be initialised to the value of
 * \c b using <tt>a(b)</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NQueue {
    public:
        typedef NQueueElement<T> Element;
            /**< Wrapper class storing a single queue element. */

    protected:
        Element* head;
            /**< Element at the head of the queue, or 0 if the queue
                 is empty. */
        Element* tail;
            /**< Element at the tail of the queue, or 0 if the queue
                 is empty. */

    public:
        /**
         * Creates a new empty queue.
         */
        NQueue() : head(0), tail(0) {
        }
        /**
         * Destroys the queue.  All internal structures will be
         * deallocated, but if the data stored (type \c T) is
         * pointers, the objects to which they point will not be
         * deallocated.
         */
        virtual ~NQueue() {
            Element* tmp;
            while (head) {
                tmp = head;
                head = head->prev;
                delete tmp;
            }
        }

        /**
         * Determines whether the queue is empty.
         *
         * @return \c true if and only if the queue is empty.
         */
        bool empty() const {
            return (head == 0);
        }
        /**
         * Inserts a new element at the tail of the queue.
         *
         * @param obj the new object to be inserted.
         */
        void insert(const T& obj) {
            Element* tmp = new Element(obj);
            tmp->prev = 0;
            if (tail)
                tail->prev = tmp;
            else
                head = tmp;
            tail = tmp;
        }
        /**
         * Removes the element at the head of the queue.
         * Associated internal structures are deallocated, but if the
         * element removed (type \c T) is a pointer, the object to which
         * it points is not deallocated.
         *
         * \pre The queue is not empty.
         *
         * @return the element removed from the queue.
         */
        T remove() {
            T ans = head->data;
            Element* tmp = head;
            head = head->prev;
            delete tmp;
            if (! head)
                tail = 0; // We just removed the tail also!
            return ans;
        }
        /**
         * Removes all elements from this queue.
         * Associated internal structures are deallocated, but if the
         * elements removed (of type \c T) are pointers, the objects to which
         * they point are not deallocated.
         */
        void flush() {
            Element* tmp;
            while (head) {
                tmp = head;
                head = head->prev;
                delete tmp;
            }
            head = 0;
            tail = 0;
        }
        /**
         * Returns the element currently at the head of the queue.
         * Note that this element is \e not removed from the queue.
         *
         * \pre The queue is not empty.
         *
         * @return the element at the head of the queue.
         */
        T& peek() const {
            return head->data;
        }
};

#endif

