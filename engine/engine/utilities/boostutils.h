
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

/*! \file boostutils.h
 *  \brief Miscellaneous utility classes taken or modified from the
 *  Boost C++ libraries.
 *
 *  The Boost copyright notices are as follows.
 *
 *  <b>Type Traits:</b>
 *
 *  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
 *
 *  Permission to copy, use, modify, sell and distribute this software
 *  is granted provided this copyright notice appears in all copies.
 *  This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 *
 *  <b>Reference Wrappers:</b>
 *
 *  Copyright (C) 1999, 2000 Jaakko Järvi (jaakko.jarvi@cs.utu.fi)<br>
 *  Copyright (C) 2001, 2002 Peter Dimov<br>
 *  Copyright (C) 2002 David Abrahams
 *
 *  Permission to copy, use, modify, sell and distribute this software
 *  is granted provided this copyright notice appears in all copies.
 *  This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 *
 *  <b>Next, Prior and Non-Copyable:</b>
 *
 *  Contributed by Dave Abrahams
 *
 *  (C) Copyright Boost.org 1999-2003.
 *  Permission to copy, use, modify, sell and distribute this software
 *  is granted provided this copyright notice appears in all copies.
 *  This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 */

#ifndef __BOOSTUTILS_H
#ifndef __DOXYGEN
#define __BOOSTUTILS_H
#endif

namespace regina {

/**
 * Miscellaneous utility classes taken or modified from the Boost C++
 * libraries.
 *
 * See the boostutils.h file documentation for Boost license details.
 */
namespace boost {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A template class used to remove the indirection from a pointer type.
 *
 * If <tt>T</tt> is a pointer type, then
 * \code remove_pointer<T>::type \endcode
 * removes the top-level indirection from <tt>T</tt>; otherwise
 * <tt>T</tt> remains unchanged.  For example <tt>int*</tt> becomes
 * <tt>int</tt>, but <tt>int&</tt> remains unchanged.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <typename T>
struct remove_pointer {
    typedef T type;
        /**< The template argument with the top-level indirection
         *   removed if it is a pointer type. */
};

#ifndef __DOXYGEN
    /**
     * Specialisations of remove_pointer:
     */
    template <typename T>
    struct remove_pointer<T*> {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* const> {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* volatile> {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* const volatile> {
        typedef T type;
    };
#endif

/**
 * A wrapper allowing references to be passed through generic functions.
 * This class is for use with the Standard Template Library.
 *
 * The primary advantage of this class is its implicit conversion to
 * type <tt>T&</tt>.  Thus it can be passed to routines expecting
 * references to <tt>T</tt> but can also be passed by reference itself.
 *
 * See global routines ::ref() and ::cref() for simple creation of these
 * wrappers.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <class T>
class reference_wrapper {
    public:
        typedef T type;
            /**< The data type being referenced by this wrapper. */

    private:
        T* t_;
            /**< A pointer to the object being referenced. */

    public:
        /**
         * Creates a new wrapper to reference the given object.
         *
         * @param t the object to be referenced.
         */
        explicit reference_wrapper(type& t) : t_(&t) {
        }
        /**
         * Returns a reference to the object being referenced.
         * This routine provides an implicit conversion to type
         * <tt>T&</tt>.
         *
         * @return the corresonding reference.
         */
        operator type& () const {
            return *t_;
        }
        /**
         * Returns a reference to the object being referenced.
         *
         * @return the corresonding reference.
         */
        type& get() const {
            return *t_;
        }
        /**
         * Returns a pointer to the object being referenced.
         *
         * @return the corresponding pointer.
         */
        type* get_pointer() const {
            return t_;
        }
};

/**
 * Returns a wrapper for the given reference.  See reference_wrapper for
 * further details.
 *
 * \ifacespython Not present.
 *
 * @param t the reference to wrap.
 * @return the corresponding wrapper.
 *
 * @author This routine was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <class T>
inline reference_wrapper<T> const ref(T& t) {
    return reference_wrapper<T>(t);
}

/**
 * Returns a wrapper for the given const reference.  See reference_wrapper for
 * further details.
 *
 * \ifacespython Not present.
 *
 * @param t the reference to wrap.
 * @return the corresponding wrapper.
 *
 * @author This routine was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <class T>
inline reference_wrapper<T const> const cref(T const& t) {
    return reference_wrapper<T const>(t);
}

/**
 * Returns the iterator prior to the given iterator.
 * This function avoids having to explicitly create a temporary to
 * decrement.
 *
 * Only the decrement operator <tt>--it</tt> needs to be defined.
 *
 * \ifacespython Not present.
 *
 * @param it the iterator to examine.
 * @return the iterator prior to the given iterator.
 *
 * @author This routine was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <class T>
inline T prior(T it) {
    return --it;
}

/**
 * Returns the iterator following the given iterator.
 * This function avoids having to explicitly create a temporary to
 * increment.
 *
 * Only the increment operator <tt>++it</tt> needs to be defined.
 *
 * \ifacespython Not present.
 *
 * @param it the iterator to examine.
 * @return the iterator following the given iterator.
 *
 * @author This routine was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
template <class T>
inline T next(T it) {
    return ++it;
}

/**
 * A base class that guarantees that derived classes cannot be copied.
 * This is done by defining a private copy constructor and a private
 * copy assignment operator.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Boost C++ libraries
 * (<tt>http://www.boost.org/</tt>).
 */
class noncopyable {
    protected:
        /**
         * A constructor which does nothing.
         */
        noncopyable() {}
        /**
         * A destructor which does nothing.
         */
        ~noncopyable() {}

    private:
        /**
         * An inaccessable copy constructor.
         */
        noncopyable(const noncopyable&) {}
        /**
         * An inaccessible copy assignment operator.
         */
        const noncopyable& operator = (const noncopyable&) { return *this; }
};

/*@}*/

} } // namespace regina::boost

#endif

