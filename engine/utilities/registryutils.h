
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

/*! \file utilities/registryutils.h
 *  \brief Miscellaneous utility classes for working with registries of
 *  packet types, normal coordinate systems, and so on.
 */

#ifndef __REGISTRYUTILS_H
#ifndef __DOXYGEN
#define __REGISTRYUTILS_H
#endif

#include <cstddef>
#include <type_traits>
#include <utility>

namespace regina {

/**
 * A convenience base class for a non-void function object, which does nothing
 * beyond provide a \a ReturnType typedef.
 *
 * Specifically, a function object \a F that returns type \a T may inherit
 * from the base class \a Returns<T>.  This will ensure that \a F includes a
 * typedef \a F::ReturnType representing type \a T.
 *
 * See the non-void variant of the forCoords() routine for an example of where
 * such a function object might be used.
 *
 * \ifacespython Not present.
 */
template <typename ReturnType_>
struct Returns {
    /**
     * Indicates the return type for a function object.
     */
    typedef ReturnType_ ReturnType;
};

/**
 * Internal to ReturnsTraits.
 *
 * This class is used to detect at compile time whether the type
 * <tt>T::ReturnType</tt> exists.
 *
 * If T::ReturnType exists, the enum constant <tt>HasReturnType<T>::value</tt>
 * will be non-zero (i.e., true).  Otherwise <tt>HasReturnType<T>::value</tt>
 * will be zero (i.e., false).
 */
template <typename T>
class HasReturnType {
    private:
        typedef char yes;
        typedef long no;

        template <typename U> static yes test(typename U::ReturnType *);
        template <typename U> static no test(...);

    public:
        enum {
            /**
             * Evaluates to non-zero (true) or zero (false) according to
             * whether or not the type <tt>T::ReturnType</tt> exists.
             */
            value = (sizeof(test<T>(0)) == sizeof(yes))
        };
};

/**
 * Provides the implementation of ReturnsTraits.
 * See the ReturnsTraits documentation for details.
 */
template <typename T, bool hasReturnType>
struct ReturnsTraitsImplementation;

#ifndef __DOXYGEN
template <typename T>
struct ReturnsTraitsImplementation<T, true> {
    typedef typename std::remove_reference<T>::type Deref;
    typedef typename Deref::ReturnType ReturnType;
};

template <typename T>
struct ReturnsTraitsImplementation<T, false> {
    typedef typename std::remove_reference<T>::type Deref;
    typedef void Void;
};
#endif

/**
 * Helper class to assist with compile-time analysis of Regina's
 * function objects.
 *
 * For the purposes of this documentation:
 *
 * - a \e void function object is any callable object with a \c void
 *   return type, such as a class with a bracket operator or simply a
 *   function reference;
 *
 * - a \e non-void function object is a callable object that defines a
 *   \a ReturnType typedef, typically of a class derived from Returns<...>.
 *
 * This helper class can be used to distinguish between void and
 * non-void function objects, and/or (thanks to the C++11 SFINAE mechanism)
 * define some other template function differently according to whether
 * its template argument is a void or non-void function object.  See the
 * forCoords() functions in surfaces/coordregistry.h for an example of
 * ReturnsTraits in action.
 *
 * The template parameter \a T should either be a function object, or a
 * reference to a function object (both lvalue and rvalue references are
 * recognised).  This class provides the following typedefs:
 *
 * - In all cases, this class provides the typedef \a Deref, which simply
 *   strips any references from \a T and returns the type of the underlying
 *   function object.
 *
 * - If \a T represents a non-void function object (i.e., the type
 *   <tt>Deref::ReturnType</tt> exists), then this class provides the
 *   typedef \a ReturnType, which is identical to <tt>Deref::ReturnType</tt>.
 *   Otherwise \a ReturnType is not defined.
 *
 * - If \a T represents a void function object (i.e., the type
 *   <tt>Deref::ReturnType</tt> does not exist), then this class provides
 *   the typedef \a Void, which is simply \c void.
 *   Otherwise \a Void is not defined.
 */
template <typename T>
struct ReturnsTraits : public ReturnsTraitsImplementation<T,
        HasReturnType<typename std::remove_reference<T>::type>::value> {
};

/**
 * A function object that creates a new object subclassed from \a Base,
 * where the particular subclass is chosen according to the template argument
 * to operator().  The template argument to the bracket operator would
 * typically be one of the registry helper classes, such as PacketInfo or
 * NormalInfo.
 *
 * This routine is intended for use with registry routines, such as the
 * non-void variants of forPacket() and forCoords().
 *
 * \ifacespython Not present.
 */
template <class Base>
struct NewFunction : public Returns<Base*> {
    /**
     * Creates a new object of the subclass Info::Class.
     *
     * \pre It is known in advance that Info::Class will be a subclass of
     * \a Base.
     *
     * @param args any additional arguments to pass to the Info::Class
     * constructor.  These will be copied/moved, so if you wish to pass
     * a reference then you should wrap it in std::ref or std::cref.
     * @return a new object of the subclass Info::Class.
     */
    template <typename Info, typename... Args>
    inline Base* operator() (Args&&... args) const {
        return new typename Info::Class(std::forward<Args>(args)...);
    }
};

} // namespace regina

#endif

