
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

/*! \file python/helpers/equality.h
 *  \brief Assists with wrapping Regina's common string output routines.
 */

#include <type_traits>
#include <boost/python/def_visitor.hpp>
#include <boost/python/operators.hpp>
#include "../engine/output.h"

namespace regina {
namespace python {

/**
 * Adds rich string output functions to the python bindings for a C++ class.
 *
 * This will add str(), utf8() and detail(), as provided by the regina::Output
 * (templated) base class.  It will also add \a __str__ to provide "native"
 * Python string output, using the C++ ostream output operator.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>c.def(regina::python::add_output())</t>, where \a c is the
 * boost::python::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::Output, or
 * should provide str(), utf8() and detail() functions and an ostream output
 * operator in a way that is consistent with the regina::Output interface.
 */
struct add_output : boost::python::def_visitor<add_output> {
    friend class boost::python::def_visitor_access;

    template <typename Class>
    void visit(Class& c) const {
        typedef typename Class::wrapped_type Type;
        typedef typename regina::OutputBase<Type>::type BaseType;
        typedef std::string (BaseType::*OutputFunctionType)() const;

        c.def("str", OutputFunctionType(&BaseType::str));
        c.def("utf8", OutputFunctionType(&BaseType::utf8));
        c.def("detail", OutputFunctionType(&BaseType::detail));
        c.def(boost::python::self_ns::str(boost::python::self));
    }
};

/**
 * Adds basic string output functions to the python bindings for a C++ class.
 *
 * This will add a str() function, and will also add \a __str__ as an alias for
 * this function to provide "native" Python string output.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>c.def(regina::python::add_output_basic())</t>, where \a c is the
 * boost::python::class_ object that wraps \a T.
 *
 * It is assumed that the wrapped class \a T does not derive from
 * regina::Output (otherwise you should use add_output, not add_output_basic).
 * Instead we simply assume that \a T provides a function of the form
 * <tt>std::string T::str() const</tt>.
 */
struct add_output_basic : boost::python::def_visitor<add_output_basic> {
    friend class boost::python::def_visitor_access;

    template <typename Class>
    void visit(Class& c) const {
        typedef typename Class::wrapped_type Type;
        typedef typename regina::OutputBase<Type>::type BaseType;
        typedef std::string (BaseType::*OutputFunctionType)() const;

        c.def("str", OutputFunctionType(&BaseType::str));
        c.def("__str__", OutputFunctionType(&BaseType::str));
    }
};

} } // namespace regina::python
