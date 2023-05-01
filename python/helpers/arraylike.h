
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file python/helpers/arraylike.h
 *  \brief Allows lightweight array-like objects to be wrapped neatly in Python.
 *
 *  This header is _not_ included automatically by python/helpers.h.
 *  If you need it, you will need to include it yourself.
 */

#ifndef __ARRAYLIKE_H
#ifndef __DOXYGEN
#define __ARRAYLIKE_H
#endif

#include "../helpers.h"

/**
 * Utility classes and routines to assist with Python bindings for
 * the calculation engine.
 */
namespace regina::python {

/**
 * Adds python bindings for the lightweight constant-array-like class \a Array.
 * An example of such a class is Perm<n>::SnLookup.
 *
 * It is assumed that \a Array contains a size() function and a square
 * bracket operator (which is assumed to be read-only - the Python
 * bindings will include \a __getitem__ but not \a __setitem).
 * For now, we assume that size() is static and \c constexpr (and thus the
 * entire array is a compile-time constant, as seen for example with
 * Perm<n>::SnLookup); this assumption might be relaxed if necessary in
 * future versions of Regina.
 *
 * The corresponding Python class will be placed within the scope \a scope
 * (which will typically be a pybind11::module_ or a pybind11::class_),
 * and will be given the class name \a name and Python docstring \a doc.
 * The access function \a __getitem__ will range-check its argument, and
 * will throw an exception if the given argument is out-of-range.
 *
 * It is fine if the C++ type \a Array is const (which may happen for
 * example if you are extracting the C++ class using decltype(...)).
 */
template <typename Array, typename Scope>
void add_lightweight_array(Scope& scope, const char* name, const char* doc) {
    using Index = decltype(Array::size());

    auto c = pybind11::class_<std::remove_cv_t<Array>>(scope, name, doc)
        .def("__getitem__", [](const Array& a, Index index) {
            if (index >= Array::size())
                throw pybind11::index_error("Array index out of range");
            return a[index];
        })
        .def("__len__", [](const Array&) {
            return Array::size();
        })
    ;
    regina::python::add_output_custom(c, [](const Array& a, std::ostream& out) {
        out << "[ ";
        // For small arrays, output the entire array.
        // For large arrays, do not output everything.
        if constexpr (Array::size() <= 24) {
            for (Index i = 0; i < Array::size(); ++i)
                out << a[i] << ' ';
        } else {
            for (Index i = 0; i < 4; ++i)
                out << a[i] << ' ';
            out << "... " << a[Array::size() - 1] << ' ';
        }
        out << "]";
    });
    regina::python::disable_eq_operators(c);
}

} // namespace regina::python

#endif
