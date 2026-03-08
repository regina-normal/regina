
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/globals.h
 *  \brief Assists with safely wrapping heavily overloaded functions.
 */

#include <cstddef> // for nullptr_t

namespace regina {

// Declare a dummy swap function, so that regina::swap always resolves.
// This avoids a compile error in scenarios where we are including this header
// but not calling add_global_swap(), and where there is in fact no variant of
// regina::swap() currently visible.
//
void swap(std::nullptr_t, std::nullptr_t) noexcept;

namespace python {

/**
 * Adds a Python binding for the overloaded global regina::swap function
 * for objects of type T, using the given docstring.
 *
 * This routine exists purely for safety: it uses static_cast (not C-style
 * casts) to resolve the overload, so that there is no risk of accidentally
 * binding the wrong variant of regina::swap().
 */
template <typename T>
inline void add_global_swap(pybind11::module_& m, const char* doc) {
    m.def("swap", static_cast<void(&)(T&, T&)>(regina::swap), doc);
}

} } // namespace regina::python
