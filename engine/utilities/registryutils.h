
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file utilities/registryutils.h
 *  \brief Miscellaneous utility classes for working with registries of
 *  packet types, normal coordinate systems, and so on.
 */

#ifndef __REGISTRYUTILS_H
#ifndef __DOXYGEN
#define __REGISTRYUTILS_H
#endif

#include <cstddef>

namespace regina {

/**
 * A convenience base class for a function object, which does nothing
 * beyond provide a \a ReturnType typedef.
 *
 * Specifically, a function object \a F that returns type \a T could inherit
 * from the base class \a Returns<T>.  This will ensure that \a F includes a
 * typedef \a F::ReturnType representing type \a T.
 *
 * See the three-argument forCoords() routine for an example of where
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
 * Detects at compile time whether the type <tt>T::ReturnType</tt> exists.
 * This can be used in conjunction with constructs such as std::enable_if.
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
 * A function object that creates a new object subclassed from \a Base,
 * where the particular subclass is chosen according to the template argument
 * to operator().  The template argument to the bracket operator would
 * typically be one of the registry helper classes, such as PacketInfo or
 * NormalInfo.
 *
 * The new object will be created using a single-argument constructor,
 * where the argument is of type \a Arg.
 *
 * This routine is intended for use with registry routines, such as the
 * three-argument variants of forPacket() and forCoords().
 *
 * \ifacespython Not present.
 */
template <class Base, typename Arg>
struct NewFunction1 : public Returns<Base*> {
    size_t arg_;
        /**< The length of the new vector to create. */

    /**
     * Creates a new function object, whose bracket operator will create a
     * new object by passing \a arg to its constructor.
     *
     * @param arg the argument to pass to the new object's class constructor.
     */
    inline NewFunction1(Arg arg) : arg_(arg) {
    }

    /**
     * Creates a new object of the subclass Info::Class.
     * The object will be created using a single-argument constructor
     * for Info::Class, and the argument to this constructor will be
     * the same argument \a arg that was passed to this function
     * object's NewFunction1 constructor.
     *
     * \pre It is known in advance that Info::Class will be a subclass of
     * \a Base.
     *
     * @return a new object of the subclass Info::Class.
     */
    template <typename Info>
    inline Base* operator() (Info) {
        return new typename Info::Class(arg_);
    }
};

} // namespace regina

#endif

