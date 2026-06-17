
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#ifndef __HELPERS_EQUALITY_H
#ifndef __DOXYGEN
#define __HELPERS_EQUALITY_H
#endif

#include <concepts>
#include <sstream>
#include <type_traits>
#include "pybind11/operators.h"

namespace regina {

class Packet;

namespace python {

namespace doc::common {
    // Note: docstrings should be wrapped at 70 characters per line;
    // the hard maximum is 72.

    inline constexpr const char neq_value[] =
R"doc(Determines whether this and the given object have different values.
This operator ``x != y`` is generated automatically, as the negation
of ``x == y``.

This test compares the _contents_ of the two objects (i.e., it
compares by value, not by reference). See the documentation for the
corresponding equality test (i.e., the member function ``__eq__``)
for full details on how objects of this type will be compared.)doc";

    inline constexpr const char eq_reference[] =
R"doc(Determines whether this and the given Python wrapper refer to the same
underlying object in Regina's calculation engine.

Note that most of Regina's classes do **not** test equality in this
way; instead they use value semantics (i.e., the == and != operators
compare the *contents* of the two objects). This class is one of the
few exceptions that uses reference semantics, as explained below.

Regina's calculation engine is written in C++, not Python. It is
therefore possible to have several different Python objects that are
all thin wrappers around the same underlying C++ object (so changes to
any one of these objects will be reflected in all of them). The
operators == and != for this class will test for exactly this scenario.

Essentially, these tests are similar in spirit to the Python test
``x is y``, but instead of looking at the Python wrappers they look at
the underlying C++ objects in the calculation engine.  In particular,
as noted above, it is possible to have two different Python wrappers
(so ``x is y`` is false) that refer to the same underlying C++ object
(so ``x == y`` is true).)doc";

    inline constexpr const char neq_reference[] =
R"doc(Determines whether this and the given Python wrapper refer to different
underlying objects in Regina's calculation engine.

Note that most of Regina's classes do **not** test equality in this
way; instead they use value semantics (i.e., the == and != operators
compare the *contents* of the two objects). This class is one of the
few exceptions that uses reference semantics, as explained below.

Regina's calculation engine is written in C++, not Python. It is
therefore possible to have several different Python objects that are
all thin wrappers around the same underlying C++ object (so changes to
any one of these objects will be reflected in all of them). The
operators == and != for this class will test for exactly this scenario.

Essentially, these tests are similar in spirit to the Python test
``x is y``, but instead of looking at the Python wrappers they look at
the underlying C++ objects in the calculation engine.  In particular,
as noted above, it is possible to have two different Python wrappers
(so ``x is y`` is false) that refer to the same underlying C++ object
(so ``x == y`` is true).)doc";

    inline constexpr const char eq_None[] =
R"doc(Always returns ``False``, since an object of this type is never equal
to ``None``.)doc";

    inline constexpr const char neq_None[] =
R"doc(Always returns ``True``, since an object of this type is never equal
to ``None``.)doc";

    inline constexpr const char eq_disabled[] =
R"doc(Disabled for objects of this type.

Objects of this type use value semantics, which means that the
operators == and != should compare by value (i.e., they test whether
two objects have the same contents). However, Regina does not
currently implement such a test for objects of this type.)doc";

    inline constexpr const char eq_packet_disabled[] =
R"doc(Disabled for packets of this type.

The operators == and != compare packet contents by value (i.e., they
test whether two packets have the same contents). However, Regina does
not currently implement such a test for packets of this type.

To test whether two Python objects refer to the same underlying packet,
use Packet.samePacket() instead.)doc";

    inline constexpr const char eq_packet_invalid[] =
R"doc(Disabled for packets of different types.

The operators == and != compare packet contents by value, and therefore
can only be used to compare two packets of the same type.

To test whether two Python objects refer to the same underlying packet,
use Packet.samePacket() instead.)doc";

    inline constexpr const char eq_none_static[] =
R"doc(Disabled in Regina.

Objects of this type cannot be created, and so cannot be compared.)doc";

    inline constexpr const char eq_none_abstract[] =
R"doc(Disabled in Regina.

This is an abstract base class, and so objects of this base class
cannot be created directly. Instead its various subclasses are
responsible for providing their own comparison operators == and !=.)doc";

} // namespace doc::common

/**
 * Indicates the different ways in which the equality (==) and inequality (!=)
 * operators can behave under Python.  This behaviour differs according
 * to the underlying class of the objects being compared.
 *
 * Specifically, suppose we have two Python objects \a x and \a y that
 * wrap instances of some underlying C++ class \a C.  Then, if we
 * test `x == y` or `x != y` under Python, the possible
 * behaviours are:
 */
enum class EqualityType {
    /**
     * The objects are compared by value.  This means that the underlying
     * C++ operators == and != for the class \a C are used.
     */
    ByValue = 1,
    /**
     * The objects are compared by reference.  This means that the
     * Python operators == and != simply test whether \a x and \a y refer to
     * the same instance of the C++ class \a C.  In other words, they test
     * whether the underlying C++ _pointers_ to \a x and \a y are the same.
     */
    ByReference = 2,
    /**
     * No objects of the class \a C are ever instantiated.  This means that
     * no comparisons are ever made.  An example of such a class is
     * Example<dim>, which consists entirely of static functions.
     */
    NeverInstantiated = 3,
    /**
     * Objects of the class \a C cannot be compared by value (because
     * the comparison operators are not implemented), and they should not
     * be compared by reference (because they are passed around by value).
     */
    Disabled = 4,

    /**
     * Deprecated constant indicating that objects are compared by value.
     *
     * \deprecated This has been renamed to `ByValue`.
     */
    BY_VALUE [[deprecated]] = 1,
    /**
     * Deprecated constant indicating that objects are compared by reference.
     *
     * \deprecated This has been renamed to `ByReference`.
     */
    BY_REFERENCE [[deprecated]] = 2,
    /**
     * Deprecated constant indicating that no objects of the class \a C are
     * ever instantiated.
     *
     * \deprecated This has been renamed to `NeverInstantiated`.
     */
    NEVER_INSTANTIATED [[deprecated]] = 3,
    /**
     * Deprecated constant indicating that objects cannot be compared by value
     * but also should not be compared by reference.
     *
     * \deprecated This has been renamed to `Disabled`.
     */
    DISABLED [[deprecated]] = 4
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
 * Adds `==` and `!=` operators that compare by value to the Python bindings
 * for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_eq_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be to add
 * Python operators `==` and `!=` that compare objects by value.  The \a docEq
 * argument will be used to generate the respective Python docstrings.
 *
 * Furthermore, this will add an attribute \a equalityType to the Python
 * wrapper class, which will be set to `EqualityType::ByValue`.
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

    c.attr("equalityType") = EqualityType::ByValue;
}

/**
 * Adds `==` and `!=` operators that compare by reference to the Python bindings
 * for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::add_eq_operators(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be to add
 * Python operators `==` and `!=` that compare objects by reference.
 * Sensible docstrings for comparison-by-reference will be provided.
 *
 * Furthermore, this will add an attribute \a equalityType to the Python
 * wrapper class, which will be set to `EqualityType::ByReference`.
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

    c.attr("equalityType") = EqualityType::ByReference;
}

/**
 * Adds `==` and `!=` operators that compare by value to the Python bindings
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
 * Adds `==` and `!=` operators that throw exceptions to the Python bindings
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

    c.attr("equalityType") = EqualityType::Disabled;
}

/**
 * Indicates that a C++ class is never instantiated, and that its Python
 * wrapper class should not support the operators `==` or `!=`.
 *
 * This should only be used with C++ classes that are never instantiated,
 * and (at least from the user's point of view) are only ever accessed
 * through static methods (e.g., `Example<dim>` or `Primes`).  As such, it
 * should be impossible to even call the `==` and `!=` operators under Python.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::no_eq_static(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators `==` and `!=` will be added to the Python wrapper
 *   class (thus overriding any default provided by pybind11).  These operators
 *   will throw Python exceptions if they are ever called.  Sensible docstrings
 *   for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the Python wrapper class.
 *   Its value will be the EqualityType enum constant \a NeverInstantiated.
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
    c.attr("equalityType") = EqualityType::NeverInstantiated;
}

/**
 * Indicates that a C++ class is an abstract base class, and that its
 * subclasses are responsible for providing operators `==` and `!=` in Python.
 *
 * This should only be used with C++ abstract base classes whose subclasses
 * provide their own equality tests (e.g., StandardSubcomplex<dim>).
 * Under normal circumstances (i.e., unless the user is deliberately trying to
 * do this), it would be impossible for Python users to call the `==` or `!=`
 * operators from the base class.
 *
 * Here by "abstract" we mean that classes that are "philosophically abstract",
 * not technically abstract in the C++ sense.  That is: they should be
 * non-constructible base classes with virtual functions that subclasses
 * override; however, we do not actually require at least one virtual function
 * to be pure.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * `regina::python::no_eq_abstract(c)`, where \a c is the
 * `pybind11::class_` object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators `==` and `!=` will be added to the Python wrapper
 *   class (thus overriding any default provided by pybind11).  These operators
 *   will throw Python exceptions if they are ever called.  Sensible docstrings
 *   for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the Python wrapper class.
 *   Its value will be the EqualityType enum constant \a NeverInstantiated.
 *
 * This is similar in effect to no_eq_static(); the main difference here
 * is that different docstrings will be supplied.
 */
template <typename T, typename... options>
requires ((! std::equality_comparable<T>) &&
    (! std::is_default_constructible_v<T>) &&
    (! std::is_copy_constructible_v<T>))
void no_eq_abstract(pybind11::class_<T, options...>& c) {
    // Note: the requires clause does not enforce std::is_polymorphic_v<T>,
    // because that would disallow the trivial StandardSubcomplexOptions<4>
    // (which _would_ have virtual functions if there were any optional
    // subcomplex features in dimension 4, except there are not).
    auto func = [](const T&, const T&) {
        throw std::runtime_error(
            "It should be impossible to create objects of this class, and so "
            "there are no operators == or !=.");
    };

    c.def("__eq__", func, doc::common::eq_none_abstract);
    c.def("__ne__", func, doc::common::eq_none_abstract);
    c.attr("equalityType") = EqualityType::NeverInstantiated;
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
 * - Operators `==` and `!=` will be added to the Python wrapper class (thus
 *   overriding any default provided by pybind11), and these operators will
 *   throw Python exceptions that contain useful explanations.  Sensible
 *   docstrings for these operators will be provided.
 *
 * - The attribute \a equalityType will be added to the Python wrapper class.
 *   Its value will be the EqualityType enum constant \a Disabled.
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

    c.attr("equalityType") = EqualityType::Disabled;
}

/**
 * Adds comparison operators '<', '<=', '>' and '>=' that compare by value to
 * the Python bindings for a C++ class.
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

#endif
