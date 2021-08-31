
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file python/helpers/pointers.h
 *  \brief Assists with converting C++ smart pointers to Regina's SafePtr.
 */

#ifndef __REGINA_PYTHON_POINTERS_H
#ifndef __DOXYGEN
#define __REGINA_PYTHON_POINTERS_H
#endif

/**
 * Tell pybind11 how to convert a C++ std::unique_ptr into Regina's SafePtr.
 *
 * The macro CONVERT_FROM_UNIQUE_PTR(T) needs to appear at the start of every
 * source file where we bind a function that returns a std::unique_ptr<T>,
 * but where pybind11 holds T using regina::SafePtr.  The macro should
 * appear in the global namespace.
 *
 * It appears that without this, the returned object is never destroyed
 * (and also there is no warning from the compiler that something is wrong).
 * I'm not sure if pybind11 is taking a deep copy of the object, or if it's
 * just releasing the std::unique_ptr without claiming ownership itself;
 * the answer is buried deep within pybind11/cast.h.
 *
 * But regardless: here is a fix.
 *
 * It would be better if we could find a fix where the compiler enforces
 * its use.  At the moment we have to be vigilant about remembering to use
 * this macro, with the possible penalty of a memory leak if we do not.
 *
 * It would also be better if the macro could enforce that T is actually held
 * by regina::SafePtr, so that we do not accidentally use this macro with the
 * wrong type(s) T.  Probably this just requires the right static_assert.
 */
#define CONVERT_FROM_UNIQUE_PTR(T) \
namespace pybind11::detail { \
template <> \
struct type_caster<std::unique_ptr<T>> { \
    public: \
        PYBIND11_TYPE_CASTER(T*, _("unique_ptr")); \
        bool load(handle, bool) { \
            return false; \
        } \
        static handle cast(std::unique_ptr<T> src, return_value_policy policy, \
                handle parent) { \
            return pybind11::detail::type_caster_base<T>::cast(src.release(), \
                policy, parent); \
        } \
}; \
}

#endif

