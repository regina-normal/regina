
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 2023, Ben Burton                                        *
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

#ifndef __TESTHELPER_H
#define __TESTHELPER_H

#include "regina-config.h"
#include "gtest/gtest.h"

/**
 * Returns the (possibly mangled) name of type \a T.
 *
 * The reason for using this function instead of `typeid(T).name()` is that
 * typeid() causes linker errors on some compilers if \a T is a native 128-bit
 * integer type.
 */
template <typename T>
inline const char* safeTypeName() {
#if defined(INTERNAL___INT128_FOUND)
    if constexpr (std::is_same_v<T, __int128>)
        return "__int128";
    else if constexpr (std::is_same_v<T, __uint128>)
        return "__uint128";
#elif defined(INTERNAL___INT128_T_FOUND)
    if constexpr (std::is_same_v<T, __int128_t>)
        return "__int128_t";
    else if constexpr (std::is_same_v<T, __uint128_t>)
        return "__uint128_t";
#elif defined(INTERNAL_INT128_T_FOUND)
    if constexpr (std::is_same_v<T, int128_t>)
        return "int128_t";
    else if constexpr (std::is_same_v<T, uint128_t>)
        return "uint128_t";
#endif
    return typeid(T).name();
}

/**
 * A SCOPED_TRACE() macro for use with GoogleTest that writes the value of
 * the given variable in the trace.
 *
 * Here \a var should be a native numeric C++ type that can be converted to a
 * std::string via std::to_string().
 */
#define SCOPED_TRACE_NUMERIC(var) SCOPED_TRACE(#var " = " + std::to_string(var))

/**
 * A SCOPED_TRACE() macro for use with GoogleTest that writes the value of
 * the given variable in the trace, using the given variable name.
 *
 * Here \a name should be a C-style string, and \a var should be a native
 * numeric C++ type that can be converted to a std::string via std::to_string().
 */
#define SCOPED_TRACE_NAMED_NUMERIC(name, var) SCOPED_TRACE(name " = " + \
    std::to_string(var))

/**
 * A SCOPED_TRACE() macro for use with GoogleTest that writes the value of
 * the given variable in the trace.
 *
 * Here \a var should be a pointer to a native C-style string; that is, \a var
 * should be of type (char*) or (const char*).
 */
#define SCOPED_TRACE_CSTRING(var) SCOPED_TRACE(std::string(#var " = ") + var)

/**
 * A SCOPED_TRACE() macro for use with GoogleTest that writes the value of
 * the given variable in the trace.
 *
 * Here \a var should be one of Regina's types that provides a str() member
 * function.
 */
#define SCOPED_TRACE_REGINA(var) SCOPED_TRACE(#var " = " + var.str())

/**
 * A SCOPED_TRACE() macro for use with GoogleTest that writes the (possibly
 * mangled) name of the given type \a T in the trace.
 */
#define SCOPED_TRACE_TYPE(T) SCOPED_TRACE(std::string("type = ") + \
    safeTypeName<T>())

#endif

