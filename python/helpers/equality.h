
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

/*! \file python/helpers/equality.h
 *  \brief Assists with wrapping equality and comparison operators in Python.
 */

#include <concepts>
#include <sstream>
#include <type_traits>
#include "pybind11/operators.h"

namespace regina {

class Packet;

namespace python {

/**
 * Indicates the different ways in which the equality (==) and inequality (!=)
 * operators can behave under Python.  This behaviour differs according
 * to the underlying class of the objects being compared.
 *
 * Specifically, suppose we have two python objects \a x and \a y that
 * wrap instances of some underlying C++ class \a C.  Then, if we
 * test `x == y` or `x != y` under python, the possible
 * behaviours are:
 */
enum class EqualityType {
    /**
     * The objects are compared by value.  This means that the underlying
     * C++ operators == and != for the class \a C are used.
     */
    BY_VALUE = 1,
    /**
     * The objects are compared by reference.  This means that the
     * python operators == and != simply test whether \a x and \a y refer to
     * the same instance of the C++ class \a C.  In other words, they test
     * whether the underlying C++ _pointers_ to \a x and \a y are the same.
     */
    BY_REFERENCE = 2,
    /**
     * No objects of the class \a C are ever instantiated.  This means that
     * no comparisons are ever made.  An example of such a class is
     * Example<dim>, which consists entirely of static functions.
     */
    NEVER_INSTANTIATED = 3,
    /**
     * Objects of the class \a C cannot be compared by value (because
     * the comparison operators are not implemented), and they should not
     * be compared by reference (because they are passed around by value).
     */
    DISABLED = 4
};

#ifndef __DOCSTRINGS

#ifndef __DOXYGEN
/**
 * A fallback Python equality/inequality test to use when comparing packets of
 * different types.  This routine ignores its arguments, and instead throws an
 * exception with an informative message.
 */
inline bool invalidPacketComparison(const regina::Packet&,
        const regina::Packet&) {
    throw std::runtime_error("The comparison operators == and != "
        "now compare packet contents by value, and can only be used with two "
        "packets of the same type.  To test whether two Python objects refer "
        "to the same underlying packet, use Packet.samePacket() instead.");
}
#endif // __DOXYGEN

/**
 * Adds `==` and `!=` operators that compare by value to the python bindings
 * for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_eq_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be to add
 * python operators `==` and `!=` that compare objects by value.  The \a docEq
 * argument will be used to generate the respective python docstrings.
 *
 * Furthermore, this will add an attribute \a equalityType to the python
 * wrapper class, which will be set to `EqualityType::BY_VALUE`.
 *
 * If \a T is a packet type (e.g., regina::Text) or is held by a wrapped
 * packet type (e.g., regina::Link), then you should use packet_eq_operators()
 * instead.
 */
template <typename T, typename... options>
requires (std::equality_comparable<T>)
void add_eq_operators(pybind11::class_<T, options...>& c, const char* docEq) {
    c.def("__eq__", [](const T& a, const T& b) { return a == b; }, docEq);
    c.def("__ne__", [](const T& a, const T& b) { return a != b; },
        doc::common::neq_value);

    c.def("__eq__", [](const T&, std::nullptr_t) { return false; },
        doc::common::eq_None);
    c.def("__ne__", [](const T&, std::nullptr_t) { return true; },
        doc::common::neq_None);

    c.attr("equalityType") = EqualityType::BY_VALUE;
}

/**
 * Adds `==` and `!=` operators that compare by reference to the python bindings
 * for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_eq_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be to add
 * python operators `==` and `!=` that compare objects by reference.
 * Sensible docstrings for comparison-by-reference will be provided.
 *
 * Furthermore, this will add an attribute \a equalityType to the python
 * wrapper class, which will be set to `EqualityType::BY_REFERENCE`.
 *
 * This should _not_ be used for packet types (e.g., regina::Text) or classes
 * held by wrapped packet types (e.g., regina::Link), since packets should
 * always be compared by value, not by reference.
 */
template <typename T, typename... options>
requires (! std::equality_comparable<T>)
void add_eq_operators(pybind11::class_<T, options...>& c) {
    c.def("__eq__", [](const T& a, const T& b) { return &a == &b; },
        python::doc::common::eq_reference);
    c.def("__ne__", [](const T& a, const T& b) { return &a != &b; },
        python::doc::common::neq_reference);

    c.def("__eq__", [](const T&, std::nullptr_t) { return false; },
        doc::common::eq_None);
    c.def("__ne__", [](const T&, std::nullptr_t) { return true; },
        doc::common::neq_None);

    c.attr("equalityType") = EqualityType::BY_REFERENCE;
}

/**
 * Adds `==` and `!=` operators that compare by value to the python bindings
 * for a C++ class that is either itself a packet type (e.g., regina::Text),
 * or a mathematical class held by a wrapped packet type (e.g., regina::Link).
 *
 * This routine performs the same task as `add_eq_operators()`, and in addition
 * it adds fallback `==` and `!=` operators that throw exceptions if an object
 * of type \a T is compared against a packet of some different type.
 * The intent is for these exceptions to be informative, so that users are
 * aware that they should use `samePacket()` and not the comparison operators
 * to test whether two Python objects wrap the same packet.
 */
template <typename T, typename... options>
requires (std::equality_comparable<T> &&
    (regina::PacketClass<T> || regina::PacketHeldType<T>))
void packet_eq_operators(pybind11::class_<T, options...>& c,
        const char* docEq) {
    add_eq_operators(c, docEq);
    c.def("__eq__", &invalidPacketComparison, doc::common::eq_packet_invalid);
    c.def("__ne__", &invalidPacketComparison, doc::common::eq_packet_invalid);
}

/**
 * Adds `==` and `!=` operators that throw exceptions to the python bindings
 * for a C++ class that is either itself a packet type (e.g., regina::Text),
 * or a mathematical class held by a wrapped packet type (e.g., regina::Link).
 *
 * Specifically, these operators throw exceptions if an object of type \a T
 * is compared against any type of packet.  This should only be used with
 * packet types that do not provide their own comparison operators (such as
 * Container, which does not contain any internal data of its own).
 */
template <typename T, typename... options>
requires ((! std::equality_comparable<T>) &&
    (regina::PacketClass<T> || regina::PacketHeldType<T>))
void packet_disable_eq_operators(pybind11::class_<T, options...>& c) {
    auto func = [](const regina::Packet&, const regina::Packet&) {
        std::ostringstream s;
        s << "The comparison operators == and != now compare packet contents "
            "by value, and are not available for the class "
            << pybind11::str(pybind11::type::handle_of<T>().attr(
                "__name__")).cast<std::string_view>()
            << ".  To test whether two Python objects refer to the same "
                "underlying packet, use Packet.samePacket() instead.";
        throw std::runtime_error(s.str());
    };

    c.def("__eq__", func, doc::common::eq_packet_disabled);
    c.def("__ne__", func, doc::common::eq_packet_disabled);

    c.def("__eq__", [](const T&, std::nullptr_t) { return false; },
        doc::common::eq_None);
    c.def("__ne__", [](const T&, std::nullptr_t) { return true; },
        doc::common::neq_None);

    c.attr("equalityType") = EqualityType::DISABLED;
}

/**
 * Indicates that a C++ class is never instantiated, and that its python
 * wrapper class should not support the operators `==` or `!=`.
 *
 * This should only be used with C++ classes that are never instantiated,
 * and (at least from the user's point of view) are only ever accessed
 * through static methods (e.g., `Example<dim>` or `Primes`).  As such, it
 * should be impossible to even call the `==` and `!=` operators under python.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::no_eq_static(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators `==` and `!=` will be added to the python wrapper
 *   class (thus overriding any default provided by pybind11).  These operators
 *   will throw python exceptions if they are ever called.  Sensible docstrings
 *   for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a NEVER_INSTANTIATED.
 *
 * This is similar in effect to no_eq_abstract(); the main difference here
 * is that different docstrings will be supplied.
 */
template <typename T, typename... options>
requires ((! std::default_initializable<T>) && (! std::equality_comparable<T>))
void no_eq_static(pybind11::class_<T, options...>& c) {
    auto func = [](const T&, const T&) {
        throw std::runtime_error(
            "It should be impossible to create objects of this class, and so "
            "there are no operators == or !=.");
    };

    c.def("__eq__", func, doc::common::eq_none_static);
    c.def("__ne__", func, doc::common::eq_none_static);
    c.attr("equalityType") = EqualityType::NEVER_INSTANTIATED;
}

/**
 * Indicates that a C++ class is an abstract base class, and that its
 * subclasses are responsible for providing operators `==` and `!=` in Python.
 *
 * This should only be used with C++ abstract base classes whose subclasses
 * provide their own equality tests (e.g., Manifold or StandardTriangulation).
 * Under normal circumstances (i.e., unless the user is deliberately trying to
 * do this), it would be impossible for Python users to call the `==` or `!=`
 * operators from the base class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::no_eq_abstract(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators `==` and `!=` will be added to the python wrapper
 *   class (thus overriding any default provided by pybind11).  These operators
 *   will throw python exceptions if they are ever called.  Sensible docstrings
 *   for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a NEVER_INSTANTIATED.
 *
 * This is similar in effect to no_eq_static(); the main difference here
 * is that different docstrings will be supplied.
 */
template <typename T, typename... options>
requires (std::is_abstract_v<T> && ! std::equality_comparable<T>)
void no_eq_abstract(pybind11::class_<T, options...>& c) {
    auto func = [](const T&, const T&) {
        throw std::runtime_error(
            "It should be impossible to create objects of this class, and so "
            "there are no operators == or !=.");
    };

    c.def("__eq__", func, doc::common::eq_none_abstract);
    c.def("__ne__", func, doc::common::eq_none_abstract);
    c.attr("equalityType") = EqualityType::NEVER_INSTANTIATED;
}

/**
 * Explicitly disables the `==` and `!=` operators for a C++ class, even
 * though objects of that type can be created.
 *
 * This should be used with classes that use value semantics (which means you
 * should not compare by reference), but which have no comparison operators
 * implemented (which means you cannot compare by value).
 *
 * If the user tries to test for equality or inequality, an exception
 * will be thrown that contains useful information.
 *
 * To use this for some C++ class \a T in Regina, call
 * `regina::python::disable_eq_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be as follows:
 *
 * - Operators `==` and `!=` will be added to the python wrapper class (thus
 *   overriding any default provided by pybind11), and these operators will
 *   throw python exceptions that contain useful explanations.  Sensible
 *   docstrings for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a DISABLED.
 */
template <typename T, typename... options>
requires (! std::equality_comparable<T>)
void disable_eq_operators(pybind11::class_<T, options...>& c) {
    auto func = [](const T&, const T&) {
        std::ostringstream s;
        s << "You cannot compare two objects of type "
            << pybind11::str(pybind11::type::handle_of<T>().attr(
                "__name__")).cast<std::string_view>()
            << ".  These objects use value semantics (they are designed to be "
            "moved and/or copied), and so you probably mean to test whether "
            "they have the same contents, not whether they reference the same "
            "native Regina object (i.e., the same location in memory).  "
            "However, Regina does not yet implement such a test.";
        throw std::runtime_error(s.str());
    };

    c.def("__eq__", func, doc::common::eq_disabled);
    c.def("__ne__", func, doc::common::eq_disabled);

    c.def("__eq__", [](const T&, std::nullptr_t) { return false; },
        doc::common::eq_None);
    c.def("__ne__", [](const T&, std::nullptr_t) { return true; },
        doc::common::neq_None);

    c.attr("equalityType") = EqualityType::DISABLED;
}

/**
 * Adds comparison operators '<', '<=', '>' and '>=' that compare by value to
 * the python bindings for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_cmp_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.
 *
 * The effect will be to add Python operators `<`, `<=`, `>` and `>=`, all of
 * which compare by value.  The \a doc argument will be used for all four
 * Python docstrings.
 *
 * This does _not_ add operators '==' or '!='; you will typically need a
 * separate call to `add_eq_operators()` for that.
 */
template <typename T, typename... options>
requires (std::three_way_comparable<T>)
void add_cmp_operators(pybind11::class_<T, options...>& c, const char* doc) {
    c.def(pybind11::self < pybind11::self, doc);
    c.def(pybind11::self <= pybind11::self, doc);
    c.def(pybind11::self > pybind11::self, doc);
    c.def(pybind11::self >= pybind11::self, doc);
}

#endif // __DOCSTRINGS

} } // namespace regina::python
