
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

/*! \file nstack.h
 *  \brief Deals with stacks of objects.
 */

#ifndef __NSTACK_H
#ifndef __DOXYGEN
#define __NSTACK_H
#endif

/**
 * For internal use with NStack.
 * Wrapper class for a single stack element containing an object of type \c T.
 *
 * \ifaces Not present.
 */
template <class T>
struct NStackElement {
    T data;
        /**< The object stored at this position in the stack. */
    NStackElement* next;
        /**< The NStackElement below this one in the stack, or
             0 if this element is at the bottom. */
    
    /**
     * Creates a new stack element containing the given data.
     * 
     * @param newData the data to be stored by this stack element.
     */
    NStackElement(const T& newData) : data(newData) {
    }
};

/**
 * A simple first-in-last-out stack of objects of type \c T.
 * 
 * Note that no assumptions are made about \c T; thus if \c T is
 * a pointer type, \e NStack itself will not deallocate popped objects.
 *
 * \pre Type T has a copy constructor.  That is, if \c a
 * and \c b are of type T, then \c a can be initialised to the value of
 * \c b using <tt>a(b)</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
class NStack {
    public:
        typedef NStackElement<T> Element;
            /**< Wrapper class storing a single stack element. */

    protected:
        Element* top;
            /**< Element at the top of the stack, or 0 if the stack
                 is empty. */

    public:
        /**
         * Creates a new empty stack.
         */
        NStack() : top(0) {
        }
        /**
         * Destroys the stack.  All internal structures will be
         * deallocated, but if the data stored (type \c T) is
         * pointers, the objects to which they point will not be
         * deallocated.
         */
        virtual ~NStack() {
            Element* tmp;
            while (top) {
                tmp = top;
                top = top->next;
                delete tmp;
            }
        }

        /**
         * Determines whether the stack is empty.
         *
         * @return \c true if and only if the stack is empty.
         */
        bool empty() const {
            return (top == 0);
        }
        /**
         * Pushes a new element onto the top of the stack.
         *
         * @param obj the new object to be pushed.
         */
        void push(const T& obj) {
            Element* tmp = new Element(obj);
            tmp->next = top;
            top = tmp;
        }
        /**
         * Pops the top element off the stack.
         * Associated internal structures are deallocated, but if the
         * element popped (type \c T) is a pointer, the object to which
         * it points is not deallocated.
         *
         * \pre The stack is not empty.
         *
         * @return the element popped off the stack.
         */
        T pop() {
            T ans = top->data;
            Element* tmp = top;
            top = top->next;
            delete tmp;
            return ans;
        }
        /**
         * Returns the element currently at the top of the stack.
         * Note that this element is \e not popped from the stack.
         *
         * \pre The stack is not empty.
         *
         * @return the element at the top of the stack.
         */
        T& peek() const {
            return top->data;
        }
};

#endif

