
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file stlutils.h
 *  \brief Miscellaneous utility classes and routines taken or modified
 *  from the Standard Template Library.
 *
 *  These classes and routines are not yet part of the C++ standard,
 *  hence their inclusion here.
 *
 *  The Standard Template Library copyright notices are as follows.
 *
 *  <b>Function Classes:</b>
 *
 *  Copyright (c) 1994
 *  Hewlett-Packard Company<br>
 *  Copyright (c) 1996-1998
 *  Silicon Graphics Computer Systems, Inc.<br>
 *  Copyright (c) 1997
 *  Moscow Center for SPARC Technology<br>
 *  Copyright (c) 1999
 *  Boris Fomitchev
 *
 *  This material is provided "as is", with absolutely no warranty expressed
 *  or implied. Any use is at your own risk.
 *  
 *  Permission to use or copy this software for any purpose is hereby granted
 *  without fee, provided the above notices are retained on all copies.
 *  Permission to modify the code and to distribute modified code is granted,
 *  provided the above notices are retained, and a notice that the code was
 *  modified is included with the above copyright notice.
 */

#ifndef __STLUTILS_H
#ifndef __DOXYGEN
#define __STLUTILS_H
#endif

#include <functional>

namespace regina {

/**
 * Miscellaneous utility classes and routines taken or modified from
 * the Standard Template Library.
 *
 * See the stlutils.h file documentation for Standard Template Library
 * license details.
 */
namespace stl {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An adaptable unary function used to select the first element of a pair.
 * This class is for use with the Standard Template Library.
 *
 * \pre The template argument <tt>Pair</tt> has a data member
 * <tt>Pair::first</tt>.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Standard Template
 * Library (<tt>http://www.stlport.org/</tt>).
 */
template <class Pair>
struct select1st : public std::unary_function<Pair, typename Pair::first_type> {
    /**
     * Returns the first element of the given pair.
     *
     * @param x the pair to examine.
     * @return the first element of the given pair.
     */
    const typename Pair::first_type& operator() (const Pair& x) const {
        return x.first;
    }
};

/**
 * An adaptable unary function used to select the second element of a pair.
 * This class is for use with the Standard Template Library.
 *
 * \pre The template argument <tt>Pair</tt> has a data member
 * <tt>Pair::second</tt>.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Standard Template
 * Library (<tt>http://www.stlport.org/</tt>).
 */
template <class Pair>
struct select2nd : public std::unary_function<Pair,
        typename Pair::second_type> {
    /**
     * Returns the second element of the given pair.
     *
     * @param x the pair to examine.
     * @return the second element of the given pair.
     */
    const typename Pair::second_type& operator() (const Pair& x) const {
        return x.second;
    }
};

/**
 * An adaptable unary function used to compose two unary functions.
 * This class is for use with the Standard Template Library.
 *
 * The composition of functions <tt>func1</tt> and <tt>func2</tt> is the
 * function <tt>comp</tt> for which <tt>comp(x) == func1(func2(x))</tt>.
 *
 * \pre Both template arguments are themselves adaptable unary
 * functions.
 * \pre Type <tt>Operation1::argument_type</tt> is the same as (or can
 * be constructed from) type <tt>Operation2::result_type</tt>.
 *
 * \ifacespython Not present.
 *
 * @author This class was taken and modified from the Standard Template
 * Library (<tt>http://www.stlport.org/</tt>).
 */
template <class Operation1, class Operation2>
class unary_compose : public std::unary_function<
        typename Operation2::argument_type, typename Operation1::result_type> {
    protected:
        Operation1 func1;
            /**< The first function to use in the composition. */
        Operation2 func2;
            /**< The second function to use in the composition. */

    public:
        /**
         * Creates a new composition function.
         *
         * The new function <tt>comp</tt> will satisfy
         * <tt>comp(arg) == x(y(arg))</tt>.
         *
         * @param x the first function to use in the composition.
         * @param y the second function to use in the composition.
         */
        unary_compose(const Operation1& x, const Operation2& y) :
                func1(x), func2(y) {
        }
        /**
         * Returns the result of this composition function when applied
         * to the given argument.
         *
         * @param x the argument to pass to this composition function.
         * @return the corresponding result of this function.
         */
        typename Operation1::result_type operator() (
                const typename Operation2::argument_type& x) const {
            return func1(func2(x));
        }
};

/**
 * Returns an adaptable unary function that is the composition of the
 * two given functions.  This class is for use with the Standard
 * Template Library.
 *
 * The composition of functions <tt>func1</tt> and <tt>func2</tt> is the
 * function <tt>comp</tt> for which <tt>comp(x) == func1(func2(x))</tt>.
 *
 * \pre Both arguments are themselves adaptable unary functions.
 * \pre Type <tt>Operation1::argument_type</tt> is the same as (or can
 * be constructed from) type <tt>Operation2::result_type</tt>.
 *
 * \ifacespython Not present.
 *
 * @param func1 the first function to use in the composition.
 * @param func2 the second function to use in the composition.
 * @return the composition of the two given functions.
 *
 * @author This routine was taken and modified from the Standard Template
 * Library (<tt>http://www.stlport.org/</tt>).
 */
template <class Operation1, class Operation2>
inline unary_compose<Operation1, Operation2> compose1(
        const Operation1& func1, const Operation2& func2) {
    return unary_compose<Operation1, Operation2>(func1, func2);
}

/*@}*/

} } // namespace regina::stl

#endif

