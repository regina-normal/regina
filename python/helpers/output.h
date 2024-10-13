
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

/*! \file python/helpers/equality.h
 *  \brief Assists with wrapping Regina's common string output routines.
 */

#include <type_traits>
#include <sstream>
#include "core/output.h"
#include "../docstrings/core/output.h"

namespace regina::python {

/**
 * Indicates the style of output to use for the Python \a __repr__ function
 * when wrapping a C++ class.
 */
enum class ReprStyle {
    /**
     * Indicates a more detailed output style.
     *
     * The output will incorporate both the class name and the C++ short
     * output (as returned by the C++ functions writeTextShort() or str(),
     * or writing the C++ object directly to an output stream).
     * It will be wrapped with angle brackets as suggested by Python's
     * own documentation for what \a __repr__ should do.
     *
     * Most classes should use this output style.
     */
    Detailed = 1,
    /**
     * Indicates a slimmed-down output style.
     *
     * The output will be exactly the same as the C++ short output
     * (as returned by the C++ functions writeTextShort() or str(),
     * or writing the C++ object directly to an output stream).
     *
     * This style should be used sparingly, since it does not indicate the
     * underlying C++ type, and it does not conform to what the Python
     * documentation says \a __repr__ should do.  Ideally this would
     * only be used for simple numeric types (e.g., regina::Integer).
     */
    Slim,
    /**
     * Indicates that there should be no custom \a __repr__ function at all.
     *
     * This will fall back to the (fairly uninformative) default provided by
     * pybind11 and/or python.
     */
    None
};

/**
 * Adds rich string output functions to the python bindings for a C++ class.
 *
 * This will add str(), utf8() and detail() to the python class, as provided by
 * the regina::Output (templated) C++ base class.  It will also add \a __str__
 * to provide "native" Python string output, which calls the C++ str() member
 * function.  This will also add a \a __repr__ function, using the given
 * output style.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_output(c, style)`, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::Output, or
 * should provide str(), utf8() and detail() functions and an ostream output
 * operator in a way that is consistent with the regina::Output interface.
 */
template <class C, typename... options>
void add_output(pybind11::class_<C, options...>& c,
        ReprStyle style = ReprStyle::Detailed) {
    // The messy std::conditional below is to resolve packets of type
    // PacketOf<...>, which inherit from Output<...> via both Packet and Held.
    using BaseType = std::conditional_t<std::is_base_of_v<regina::Packet, C>,
        Output<regina::Packet> /* choose the Output that comes via Packet */,
        typename regina::OutputBase<C>::type>;
    using OutputFunctionType = std::string (BaseType::*)() const;

    c.def("str", static_cast<OutputFunctionType>(&BaseType::str),
        doc::Output_::str);
    c.def("utf8", static_cast<OutputFunctionType>(&BaseType::utf8),
        doc::Output_::utf8);
    c.def("detail", static_cast<OutputFunctionType>(&BaseType::detail),
        doc::Output_::detail);

    c.def("__str__", static_cast<OutputFunctionType>(&BaseType::str));

    switch (style) {
        case ReprStyle::Detailed:
            c.def("__repr__", [](const C& c) {
                std::ostringstream s;
                s << "<regina.";
                s << pybind11::str(pybind11::type::handle_of<C>().attr(
                        "__qualname__")).cast<std::string_view>() << ": ";
                c.writeTextShort(s);
                s << '>';
                return s.str();
            });
            break;

        case ReprStyle::Slim:
            c.def("__repr__", static_cast<OutputFunctionType>(&BaseType::str));
            break;

        case ReprStyle::None:
            break;
    }
}

/**
 * Adds basic string output functions to the python bindings for a C++ class.
 *
 * This will add a str() function to the python class, and will also add
 * \a __str__ as an alias for this function to provide "native" Python string
 * output.  This will also add a \a __repr__ function, using the given
 * output style.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_output_basic(c, doc, style)`, where \a c is the
 * pybind11::class_ object that wraps \a T and \a doc is the Python docstring
 * for str().
 *
 * It is assumed that the wrapped class \a T does not derive from
 * regina::Output (otherwise you should use add_output, not add_output_basic).
 * Instead we simply assume that \a T provides a function of the form
 * `std::string T::str() const`.
 */
template <class C, typename... options>
void add_output_basic(pybind11::class_<C, options...>& c,
        const char* doc, ReprStyle style = ReprStyle::Detailed) {
    using BaseType = typename regina::OutputBase<C>::type;
    using OutputFunctionType = std::string (BaseType::*)() const;

    c.def("str", static_cast<OutputFunctionType>(&BaseType::str), doc);
    c.def("__str__", static_cast<OutputFunctionType>(&BaseType::str));

    switch (style) {
        case ReprStyle::Detailed:
            c.def("__repr__", [](const C& c) {
                std::ostringstream s;
                s << "<regina."
                    << pybind11::str(pybind11::type::handle_of<C>().attr(
                        "__qualname__")).cast<std::string_view>()
                    << ": " << c.str() << '>';
                return s.str();
            });
            break;

        case ReprStyle::Slim:
            c.def("__repr__", static_cast<OutputFunctionType>(&BaseType::str));
            break;

        case ReprStyle::None:
            break;
    }
}

/**
 * Adds output stream functionality to the python bindings for a C++ class.
 *
 * This will add a function \a __str__ to the python class to provide "native"
 * Python string output.  The implementation just writes the underlying C++
 * object to an output stream and collects the result.  This will also add
 * a \a __repr__ function, using the given output style.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_output_ostream(c, style)`, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * It is assumed that the wrapped class \a T does not derive from regina::Output
 * and does not provide a str() function (otherwise you should use add_output
 * or add_output_basic respectively).  Instead we simply assume that there is
 * a C++ operator for writing an object of type \a T to a C++ output stream.
 */
template <class C, typename... options>
void add_output_ostream(pybind11::class_<C, options...>& c,
        ReprStyle style = ReprStyle::Detailed) {
    auto func = [](const C& x) {
        std::ostringstream s;
        s << x;
        return s.str();
    };

    c.def("__str__", func);

    switch (style) {
        case ReprStyle::Detailed:
            c.def("__repr__", [](const C& c) {
                std::ostringstream s;
                s << "<regina."
                    << pybind11::str(pybind11::type::handle_of<C>().attr(
                        "__qualname__")).cast<std::string_view>()
                    << ": " << c << '>';
                return s.str();
            });
            break;

        case ReprStyle::Slim:
            c.def("__repr__", func);
            break;

        case ReprStyle::None:
            break;
    }
}

/**
 * Adds custom string output functions to the python bindings for a C++ class.
 *
 * This will add a function \a __str__ to the python class to provide "native"
 * Python string output.  The implementation will call \a outputFunction,
 * which must be a callable object (typically a lambda) that can be called
 * with arguments of the form `outputFunction(const C&, std::ostream&)`.
 *
 * This will also add a \a __repr__ function.  There is no choice of output
 * style: if you use add_output_custom() then the output style will always be
 * ReprStyle::Detailed.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_output_custom(c, style)`, where \a c is the
 * pybind11::class_ object that wraps \a T.
 */
template <class C, typename Function, typename... options>
void add_output_custom(pybind11::class_<C, options...>& c,
        Function&& outputFunction) {
    // We make local copies of outputFunction, since this may have been
    // passed as a temporary.

    c.def("__str__", [outputFunction](const C& x) {
        std::ostringstream s;
        outputFunction(x, s);
        return s.str();
    });

    c.def("__repr__", [outputFunction](const C& c) {
        std::ostringstream s;
        s << "<regina."
            << pybind11::str(pybind11::type::handle_of<C>().attr(
                "__qualname__")).cast<std::string_view>() << ": ";
        outputFunction(c, s);
        s << '>';
        return s.str();
    });
}

/**
 * Casts the given C++ object to Python and writes its __repr__ to
 * the given C++ output stream.
 *
 * It is assumed that this process will succeed.  That is, we assume that
 * type \a T either maps to one of the standard Python types (e.g., is
 * \c int or std::string), or else is wrapped (or will be wrapped) in Python,
 * with an appropriate __repr__ function.  If this assumption fails,
 * then this routine will almost certainly throw an exception.
 */
template <typename T>
void writeRepr(std::ostream& out, const T& obj) {
    out << static_cast<std::string>(pybind11::str(
        pybind11::cast(obj).attr("__repr__")()));
}

/**
 * Casts the given C++ object to Python and writes its __str__ to
 * the given C++ output stream.
 *
 * It is assumed that this process will succeed.  That is, we assume that
 * type \a T either maps to one of the standard Python types (e.g., is
 * \c int or std::string), or else is wrapped (or will be wrapped) in Python,
 * with an appropriate __str__ function.  If this assumption fails,
 * then this routine will almost certainly throw an exception.
 */
template <typename T>
void writeStr(std::ostream& out, const T& obj) {
    out << static_cast<std::string>(pybind11::str(pybind11::cast(obj)));
}

} // namespace regina::python
