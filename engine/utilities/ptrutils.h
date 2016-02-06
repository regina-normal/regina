
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

/*! \file utilities/ptrutils.h
 *  \brief Provides function objects for use in the Standard Template
 *  Library that take pointers as arguments but work with the pointees
 *  instead.
 */

#ifndef __PTRUTILS_H
#ifndef __DOXYGEN
#define __PTRUTILS_H
#endif

#include <memory>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A simple routine for cloning an object if and only if it exists.
 *
 * If the given pointer is non-null, this routine returns a new clone of the
 * object, created using the copy constructor for type \a T.  Otherwise
 * this routine simply returns a null pointer.
 *
 * The caller of this routine is responsible for deleting the new clone
 * when it is no longer required.
 *
 * This routine can be useful when implementing copy constructors for
 * classes that only initialise internal data members on demand.
 *
 * \ifacespython Not present.
 *
 * @param cloneMe a pointer to the object to clone; this may be null.
 * @return a newly allocated copy of the given object, or the null
 * pointer if \a cloneMe is null.
 */
template <typename T>
inline T* clonePtr(T* cloneMe) {
    return (cloneMe ? new T(*cloneMe) : 0);
}

/**
 * A simple routine for cloning an object if and only if it exists.
 *
 * If the given pointer is non-null, this routine returns a new clone of the
 * object, created using the copy constructor for type \a T.  Otherwise
 * this routine simply returns a null pointer.
 *
 * Note that, even though this routine takes a std::unique_ptr, it returns
 * a raw pointer.  The caller of this routine is responsible for
 * deleting the new clone when it is no longer required.
 *
 * This routine can be useful when implementing copy constructors for
 * classes that only initialise internal data members on demand.
 *
 * \ifacespython Not present.
 *
 * @param cloneMe a pointer to the object to clone; this may be null.
 * @return a newly allocated copy of the given object, or the null
 * pointer if \a cloneMe is null.
 */
template <typename T>
inline T* clonePtr(const std::unique_ptr<T>& cloneMe) {
    return (cloneMe.get() ? new T(*cloneMe) : 0);
}

/**
 * An adaptable binary function used to compare the objects to which
 * pointers are pointing.  This class is for use with the Standard Template
 * Library.
 *
 * The first template argument \a T will generally not be a pointer class.
 * Instead, this function will accept two const \e pointers to \a T.  It
 * will then dereference these pointers and compare these dereferenced
 * objects using the given comparison function (which defaults to std::less,
 * but which can be changed by passing a different second template argument).
 *
 * \ifacespython Not present.
 */
template <typename T, typename Comp = std::less<T> >
class LessDeref {
    public:
        typedef const T* first_argument_type;
            /**< The first argument type for this binary function. */
        typedef const T* second_argument_type;
            /**< The second argument type for this binary function. */
        typedef bool result_type;
            /**< The result type for this binary comparison function. */

    private:
        Comp comp_;
            /**< A function object for performing comparisons between
                 dereferenced objects. */

    public:
        /**
         * Compares the objects to which the given pointers are pointing.
         * The two pointers are dereferenced, and then a function of
         * type \a Comp (the second template argument) is used to
         * compare the dereferenced objects.
         *
         * @param ptr1 a pointer to the first object under examination.
         * @param ptr2 a pointer to the second object under examination.
         * @return \c true if the first dereferenced object is less than
         * the second, or \c false otherwise.
         */
        bool operator() (const T* ptr1, const T* ptr2) const {
            return comp_(*ptr1, *ptr2);
        }
};

/*@}*/

} // namespace regina

#endif

