
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file utilities/memutils.h
 *  \brief Provides object creation and deletion functions for use with
 *  the Standard Template Library.
 */

#ifndef __MEMUTILS_H
#ifndef __DOXYGEN
#define __MEMUTILS_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An adaptable generator used to create objects using default constructors.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this generator will return a
 * \e pointer to a newly created object of type <tt>T</tt>.
 *
 * \ifacespython Not present.
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
 * \ifacespython Not present.
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
 * \ifacespython Not present.
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
 * An adaptable unary function used to create objects using the
 * <tt>clone()</tt> method.
 * This class is for use with the Standard Template Library.
 *
 * Note that the template argument need not be a pointer class.  If the
 * template argument is <tt>T</tt>, this unary function will accept
 * a \e pointer to <tt>T</tt> and call <tt>clone()</tt> upon the
 * corresponding object, returning a pointer to the newly created clone
 * of type <tt>T</tt>.
 *
 * \pre Type <tt>T</tt> has method <tt>T* clone() const</tt>.  The
 * declared return type may be different, but the result \e must
 * be castable to <tt>T*</tt>.
 *
 * \ifacespython Not present.
 */
template <class T>
struct FuncNewClonePtr {
    typedef const T* argument_type;
        /**< The argument type for this unary function. */
    typedef T* result_type;
        /**< The return type for this unary function. */

    /**
     * Creates a new object using the <tt>clone()</tt> method.
     *
     * @param ptr the pointer whose corresponding object should be cloned.
     * @return the newly created clone.
     */
    T* operator() (const T* ptr) const {
        return dynamic_cast<T*>(ptr->clone());
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
 * \ifacespython Not present.
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

/*@}*/

} // namespace regina

#endif

