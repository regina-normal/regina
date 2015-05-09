
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/*! \file python/equality.h
 *  \brief Allows the comparison of Python objects which refer to C++ objects.
 *  Two Python pointer objects are equal if and only if they both point to the
 *  same C++ object.
 */

#ifndef __EQUALITY_H
#ifndef __DOXYGEN
#define __EQUALITY_H
#endif

namespace regina {

/**
 * Utility functions to compare python objects.
 */
namespace python {

/**
 * Compares two objects which are both pointers to a class T. If the two
 * pointers point to the same object, they are considered equal.
 */
template<class T>
bool EqualPtr(const T * a, const T * b) {
    return (a == b);
}

/**
 * Compares two objects which are both pointers to a class T. If the two
 * pointers point to the different objects, they are considered not equal.
 */
template<class T>
bool NotEqualPtr(const T * a, const T * b) {
    return (a != b);
}

} } // namespace regina::python

#endif
