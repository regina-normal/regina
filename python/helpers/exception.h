
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/exception.h
 *  \brief Assists with wrapping Regina's exception classes.
 *
 *  This header is _not_ included automatically by python/helpers.h.
 *  If you need it, you will need to include it yourself.
 */

#include <pybind11/pybind11.h>
#include <pybind11/subinterpreter.h>
#include "utilities/exception.h"
#include "../helpers/docstrings.h"

namespace regina::python {

/**
 * Adds Python bindings for one of Regina's C++ exception types.
 *
 * This routine will create a corresponding Python exception type (using
 * PyExc_RuntimeError as its base), and will register a translator between
 * the two.
 *
 * Conceptually, this does a similar job to `pybind11::register_exception()`;
 * however, it is also safe to use with subinterpreters (which, as of
 * pybind11 3.0.0rc3, `pybind11::register_exception()` is not).
 */
template <typename ReginaExceptionType>
void registerReginaException(pybind11::module_& m, const char* className,
        const char* docstring) {
#if REGINA_PYBIND11_VERSION == 3
    // In pybind11 v3 (at least in 3.0.0rc3), the implementation of
    // register_exception() uses a global singleton Python exception object,
    // which breaks things when we load the module multiple times in different
    // subinterpreters.  We will need to implement things manually ourselves,
    // with a different Python exception object for each subinterpreter.

    // TODO: Check what happens when we load the module multiple times within
    // the same subinterpreter.
    // TODO: Lock access to cached.
    // TODO: Find a way to clean up these exceptions when the subinterpreter
    // is destroyed.
    static std::map<int64_t, PyObject*> cached;
    int64_t id = pybind11::subinterpreter::current().id();
    cached.emplace(id, (new pybind11::exception<ReginaExceptionType>(
        m, className, docstring, PyExc_RuntimeError))->ptr());

    pybind11::register_exception_translator([](std::exception_ptr p) {
        if (!p) {
            return;
        }
        try {
            std::rethrow_exception(p);
        } catch (const ReginaExceptionType &e) {
            int64_t id = pybind11::subinterpreter::current().id();
            auto it = cached.find(id);
            if (it != cached.end())
                PyErr_SetString(it->second, e.what());
            else {
                // This should never happen.  But.. just in case, translate
                // this to a standard python RuntimeError instead.
                PyErr_SetString(PyExc_RuntimeError, e.what());
            }
        }
    });
#elif REGINA_PYBIND11_VERSION == 2
    // pybind11 v2 does not support subinterpreters properly, and
    // register_exception() appears to work perfectly fine with the
    // subinterpreter hacks that we are using.
    pybind11::register_exception<ReginaExceptionType>(m, className,
        PyExc_RuntimeError).doc() = docstring;
#else
    #error "Unsupported pybind11 version"
#endif
}

} // namespace regina::python

