
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

/*! \file python/helpers/equality.h
 *  \brief Assists with wrapping Regina's common string output routines.
 */

#include <type_traits>
#include <sstream>
#include "core/output.h"

namespace regina::python {

/**
 * Adds rich string output functions to the python bindings for a C++ class.
 *
 * This will add str(), utf8() and detail() to the python class, as provided by
 * the regina::Output (templated) C++ base class.  It will also add \a __str__
 * to provide "native" Python string output, which calls the C++ str() member
 * function.  If the optional second argument \a reprAlso is passed as \c true,
 * then an identical \a __repr__ function will be added also.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_output(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::Output, or
 * should provide str(), utf8() and detail() functions and an ostream output
 * operator in a way that is consistent with the regina::Output interface.
 */
template <class C, typename... options>
void add_output(pybind11::class_<C, options...>& c, bool reprAlso = false) {
    using BaseType = typename regina::OutputBase<C>::type;
    using OutputFunctionType = std::string (BaseType::*)() const;

    c.def("str", OutputFunctionType(&BaseType::str));
    c.def("utf8", OutputFunctionType(&BaseType::utf8));
    c.def("detail", OutputFunctionType(&BaseType::detail));

    c.def("__str__", OutputFunctionType(&BaseType::str));
    if (reprAlso)
        c.def("__repr__", OutputFunctionType(&BaseType::str));
}

/**
 * Adds rich string output functions to the python bindings for a C++ class.
 *
 * This will add str(), utf8() and detail() to the python class, as provided by
 * the regina::Output (templated) C++ base class.  It will also add \a __str__
 * to provide "native" Python string output, which calls the C++ str() member
 * function.
 *
 * This will also add a \a __repr__ function.  If the argument \a reprClassName
 * is non-null, then the \a __repr__ output will incorporate this class name
 * (which should not include the \c regina module prefix) and the output
 * provided by str().  If \a reprClassName is \c null, then \a __repr__
 * will return the same output as str().
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_output(c, reprClassName)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::Output, or
 * should provide str(), utf8() and detail() functions and an ostream output
 * operator in a way that is consistent with the regina::Output interface.
 */
template <class C, typename... options>
void add_output(pybind11::class_<C, options...>& c, const char* reprClassName) {
    using BaseType = typename regina::OutputBase<C>::type;
    using OutputFunctionType = std::string (BaseType::*)() const;

    c.def("str", OutputFunctionType(&BaseType::str));
    c.def("utf8", OutputFunctionType(&BaseType::utf8));
    c.def("detail", OutputFunctionType(&BaseType::detail));

    c.def("__str__", OutputFunctionType(&BaseType::str));
    if (reprClassName) {
        c.def("__repr__", [reprClassName](const C& c) {
            std::ostringstream s;
            s << "<regina." << reprClassName << ": ";
            c.writeTextShort(s);
            s << '>';
            return s.str();
        });
    } else {
        c.def("__repr__", OutputFunctionType(&BaseType::str));
    }
}

/**
 * Adds basic string output functions to the python bindings for a C++ class.
 *
 * This will add a str() function to the python class, and will also add
 * \a __str__ as an alias for this function to provide "native" Python string
 * output.  If the optional second argument \a reprAlso is passed as \c true,
 * then an identical \a __repr__ function will be added also.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_output_basic(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * It is assumed that the wrapped class \a T does not derive from
 * regina::Output (otherwise you should use add_output, not add_output_basic).
 * Instead we simply assume that \a T provides a function of the form
 * <tt>std::string T::str() const</tt>.
 */
template <class C, typename... options>
void add_output_basic(pybind11::class_<C, options...>& c,
        bool reprAlso = false) {
    using BaseType = typename regina::OutputBase<C>::type;
    using OutputFunctionType = std::string (BaseType::*)() const;

    c.def("str", OutputFunctionType(&BaseType::str));
    c.def("__str__", OutputFunctionType(&BaseType::str));
    if (reprAlso)
        c.def("__repr__", OutputFunctionType(&BaseType::str));
}

/**
 * Adds output stream functionality to the python bindings for a C++ class.
 *
 * This will add a function \a __str__ to the python class to provide "native"
 * Python string output.  The implementation just writes the underlying C++
 * object to an output stream and collects the result.  If the optional
 * second argument \a reprAlso is passed as \c true, then an identical
 * \a __repr__ function will be added to the python class also.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_output_basic(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * It is assumed that the wrapped class \a T does not derive from regina::Output
 * and does not provide a str() function (otherwise you should use add_output
 * or add_output_basic respectively).  Instead we simply assume that there is
 * a C++ operator for writing an object of type \a T to a C++ output stream.
 */
template <class C, typename... options>
void add_output_ostream(pybind11::class_<C, options...>& c,
        bool reprAlso = false) {
    auto func = [](const C& x) {
        std::ostringstream s;
        s << x;
        return s.str();
    };

    c.def("__str__", func);
    if (reprAlso)
        c.def("__repr__", func);
}

/**
 * Adds output stream functionality to the python bindings for a C++ class.
 *
 * This will add a function \a __str__ to the python class to provide "native"
 * Python string output.  The implementation just writes the underlying C++
 * object to an output stream and collects the result.
 *
 * This will also add a \a __repr__ function.  If the argument \a reprClassName
 * is non-null, then the \a __repr__ output will incorporate this class name
 * (which should not include the \c regina module prefix) and the output
 * that the class writes to an output stream.  If \a reprClassName is \c null,
 * then \a __repr__ will return the same output as \a __str__.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_output_basic(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * It is assumed that the wrapped class \a T does not derive from regina::Output
 * and does not provide a str() function (otherwise you should use add_output
 * or add_output_basic respectively).  Instead we simply assume that there is
 * a C++ operator for writing an object of type \a T to a C++ output stream.
 */
template <class C, typename... options>
void add_output_ostream(pybind11::class_<C, options...>& c,
        const char* reprClassName) {
    auto func = [](const C& x) {
        std::ostringstream s;
        s << x;
        return s.str();
    };

    c.def("__str__", func);
    if (reprClassName) {
        c.def("__repr__", [reprClassName](const C& c) {
            std::ostringstream s;
            s << "<regina." << reprClassName << ": " << c << '>';
            return s.str();
        });
    } else {
        c.def("__repr__", func);
    }
}

} // namespace regina::python
