
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

/*! \file nmiscutils.h
 *  \brief Provides miscellaneous utility classes including helper
 *  classes for use with the Standard Template Library.
 */

#ifndef __NMISCUTILS_H
#ifndef __DOXYGEN
#define __NMISCUTILS_H
#endif

#include <stdlib.h>

/**
 * An adaptable generator used to create objects using default constructors.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this generator will return a
 * \e pointer to a newly created object of type <tt>T</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
struct FuncNew {
    typedef T* result_type;
        /**< The return type for this generator. */

    /**
     * Creates a new object using the default constructor.
     *
     * @return the newly created object.
     */
    T* operator() () const {
        return new T;
    }
};

/**
 * An adaptable unary function used to create objects using copy constructors.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this unary function will accept
 * a \e pointer to <tt>T</tt>, dereference this pointer and pass it to the
 * copy constructor to return a pointer to a newly created object of
 * type <tt>T</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
struct FuncNewCopyPtr {
    typedef const T* argument_type;
        /**< The argument type for this unary function. */
    typedef T* result_type;
        /**< The return type for this unary function. */

    /**
     * Creates a new object using the copy constructor.
     *
     * @param ptr the pointer whose data should (after dereferencing) be
     * passed to the copy constructor.
     * @return the newly created object.
     */
    T* operator() (const T* ptr) const {
        return new T(*ptr);
    }
};

/**
 * An adaptable unary function used to create objects using copy constructors.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this unary function will accept
 * a \e reference to <tt>T</tt> and pass it directly to the
 * copy constructor to return a pointer to a newly created object of
 * type <tt>T</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
struct FuncNewCopyRef {
    typedef const T& argument_type;
        /**< The argument type for this unary function. */
    typedef T* result_type;
        /**< The return type for this unary function. */

    /**
     * Creates a new object using the copy constructor.
     *
     * @param obj the object whose data should be passed to the copy
     * constructor.
     * @return the newly created object.
     */
    T* operator() (const T& obj) const {
        return new T(obj);
    }
};

/**
 * An adaptable unary function used to deallocate objects.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this unary function will accept (and
 * call \c delete upon) \e pointers to <tt>T</tt>.
 *
 * \ifaces Not present.
 */
template <class T>
struct FuncDelete {
    typedef T* argument_type;
        /**< The argument type for this unary function. */
    typedef void result_type;
        /**< The return type for this unary function. */

    /**
     * Calls \c delete upon the given pointer.
     *
     * @param ptr the pointer whose data should be deleted.
     */
    void operator() (T* ptr) const {
        delete ptr;
    }
};

/**
 * A hash function used to calculate hash values for arbitrary pointers.
 * This class is for use with the Standard Template Library.
 *
 * The only guarantee provided by this hash function is that two
 * pointers representing the same memory location will return the same
 * hash value.  Two pointers pointing to identical data in two different
 * memory locations might very well return two different hash values.
 *
 * \ifaces Not present.
 */
struct HashPointer {
    /**
     * Returns a hash value for the given pointer.  See the general
     * class notes for further details.
     *
     * @param p the pointer whose hash value should be calculated.
     * @return the corresponding hash value.
     */
    size_t operator() (const void* p) const {
        // Cast the pointer directly to a size_t.
        return *((size_t*)((void*)&p));
    }
};

#endif

