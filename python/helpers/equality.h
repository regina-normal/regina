
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
 *  \brief Assists with wrapping == and != operators in Python.
 */

#include <type_traits>

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
 * test <tt>x == y</tt> or <tt>x != y</tt> under python, the possible
 * behaviours are:
 */
enum EqualityType {
    /**
     * The objects are compared by value.  This means that the underlying
     * C++ operators == and != for the class \a C are used.
     */
    BY_VALUE = 1,
    /**
     * The objects are compared by reference.  This means that the
     * python operators == and != simply test whether \a x and \a y refer to
     * the same instance of the C++ class \a C.  In other words, they test
     * whether the underlying C++ \e pointers to \a x and \a y are the same.
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

/**
 * Adds appropriate == and != operators to the python bindings for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_eq_operators(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.  The effect will be as follows:
 *
 * - If \a T provides both == and != operators (either as member
 *   functions or as global functions), then the python operators == and !=
 *   will compare by value.
 *
 * - If \a T provides neither == nor != operators, then the python
 *   operators == and != will compare by reference.
 *
 * - If \a T provides one of == or != but not the other, then this will
 *   generate a compile error.
 *
 * Furthermore, this will add an attribute \a equalityType to the python
 * wrapper class, which will be the corresponding constant from the
 * EqualityType enum (either \a BY_VALUE or \a BY_REFERENCE).
 *
 * If \a C is a packet type (such as regina::Text) or is inherited by a packet
 * type (such as regina::Link), then you should use packet_eq_operators()
 * instead.
 */
template <class C, typename... options>
void add_eq_operators(pybind11::class_<C, options...>& c);

/**
 * Adds appropriate == and != operators to the python bindings for a C++ class
 * that is either equal to a packet type (such as regina::Text), or inherited
 * by a packet type (such as regina::Link).
 *
 * This routine performs the same task as add_eq_operators, and in addition
 * it adds fallback == and != operators that throw exceptions if an object of
 * the given type is compared against a packet of some different type.
 * The intent is for these exceptions to be informative, so that users are
 * aware that they should use samePacket() and not the comparison operators
 * to test whether two Python objects wrap the same packet.
 */
template <class C, typename... options>
void packet_eq_operators(pybind11::class_<C, options...>& c);

/**
 * Indicates that a C++ class is never instantiated, and that its python
 * wrapper class should not support the operators == or !=.
 *
 * This should only be used with C++ classes that are never instantiated (such
 * as Example<dim>, which consists entirely of static methods, or Manifold,
 * which is an abstract base class).  As such, it should be impossible to
 * even call the == and != operators under python.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::no_eq_operators(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators == and != will be added to the python wrapper class
 *   (thus overriding any default provided by pybind11).  These operators will
 *   throw python exceptions if they are ever called.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a NEVER_INSTANTIATED.
 */
template <class C, typename... options>
void no_eq_operators(pybind11::class_<C, options...>& c);

/**
 * Explicitly disables the == and != operators for a C++ class.
 *
 * This should be used with classes that use value semantics (which
 * means you should not compare by reference), but which have no
 * comparison operators implemented (which means you cannot compare by value).
 *
 * If the user tries to test for equality or inequality, an exception
 * will be thrown that contains useful information.
 *
 * To use this for some C++ class \a T in Regina, call
 * <t>regina::python::disable_eq_operators(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.  The effect will be as follows:
 *
 * - Operators == and != will be added to the python wrapper class (thus
 *   overriding any default provided by pybind11), and these operators will
 *   throw python exceptions that contain useful explanations.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a DISABLED.
 */
template <class C, typename... options>
void disable_eq_operators(pybind11::class_<C, options...>& c);

#ifndef __DOXYGEN
namespace add_eq_operators_detail {
    template <typename T>
    static void no_equality_operators(const T&, const T&) {
        PyErr_SetString(PyExc_RuntimeError,
            "It should be impossible to create objects of this class, and so "
            "there are no operators == or !=.");
    }

    template <typename T>
    static void disable_equality_operators(const T&, const T&) {
        PyErr_SetString(PyExc_RuntimeError,
            "You cannot compare two objects of this class.  These objects "
            "use value semantics (they are designed to be moved and/or "
            "copied), and so you probably do not mean to test whether "
            "two Python wrappers reference the same internal object "
            "(i.e., the same location in memory).  However, Regina does "
            "not yet implement a test that compares the contents "
            "of two objects of this class.");
    }

    /**
     * Provide default == and != operators that return void (a type that no
     * sensible == or != operator should return).
     *
     * We follow the way that boost does this: we provide our default operators
     * via the helper class Any, so that the implicit conversion from our type
     * to Any makes these default == / != operators less preferred than any
     * of regina's own operators.
     *
     * This is indeed necessary: if we just offer default == / != operators
     * for an arbitrary type T, then these default operators are chosen for
     * MatrixInt *ahead* of the operators that MatrixInt inherits from
     * Matrix<Integer>.  If we use the Any helper class (as seen below),
     * then the inherited == / != operators are (correctly) chosen intsead.
     */
    struct Any {
        template <typename T>
        Any(const T&);
    };

    void operator == (const Any&, const Any&);
    void operator != (const Any&, const Any&);

    /**
     * A helper struct that determines at compile time whether or not
     * Regina provides == and/or != operators for type T.
     */
    template<class T>
    struct EqOperatorTraits {
        static const T& makeRef();

        using EqType = decltype(makeRef() == makeRef());
        using IneqType = decltype(makeRef() != makeRef());

        static constexpr bool hasEqOperator =
            ! std::is_same<void, EqType>::value;
        static constexpr bool hasIneqOperator =
            ! std::is_same<void, IneqType>::value;
    };

    /**
     * The template EqualityOperators<T> provides the implementation
     * that we use in python for == and != when wrapping the C++ class T.
     */
    template <class T,
              bool hasEqualityOperator = EqOperatorTraits<T>::hasEqOperator,
              bool hasInequalityOperator = EqOperatorTraits<T>::hasIneqOperator>
    struct EqualityOperators {
        // This default template is instantiated precisely when T offers
        // one of the operators == or !=, but not both.
        //
        // In Regina, we insist on an all-or-nothing approach.
        // Force a compile-time error, and tell the developers which
        // operator is missing.
        static_assert(hasEqualityOperator,
                      "Wrapped C++ type implements != but not ==.");
        static_assert(hasInequalityOperator,
                      "Wrapped C++ type implements == but not !=.");
    };

    template <class T>
    struct EqualityOperators<T, true, true> {
        // Instantion of this template means we know that T offers both
        // an operator == and an operator !=.
        static constexpr EqualityType equalityType() {
            return BY_VALUE;
        }

        static bool are_equal(const T& a, const T& b) {
            return (a == b);
        }

        static bool are_not_equal(const T& a, const T& b) {
            return (a != b);
        }
    };

    template <class T>
    struct EqualityOperators<T, false, false> {
        // Instantion of this template means we know that T offers neither
        // an operator == nor an operator !=.
        static constexpr EqualityType equalityType() {
            return BY_REFERENCE;
        }

        static bool are_equal(const T& a, const T& b) {
            return (&a == &b);
        }

        static bool are_not_equal(const T& a, const T& b) {
            return (&a != &b);
        }
    };
} // namespace add_eq_operators_detail

// Implementation of the main ..._eq_operators() functions.
// See the top of this header for their documentation.

template <class C, typename... options>
inline void add_eq_operators(pybind11::class_<C, options...>& c) {
    c.def("__eq__",
        &add_eq_operators_detail::EqualityOperators<C>::are_equal);
    c.def("__eq__", [](const C&, std::nullptr_t) { return false; });
    c.def("__ne__",
        &add_eq_operators_detail::EqualityOperators<C>::are_not_equal);
    c.def("__ne__", [](const C&, std::nullptr_t) { return true; });
    c.attr("equalityType") =
        add_eq_operators_detail::EqualityOperators<C>::equalityType();
}

template <class C, typename... options>
inline void no_eq_operators(pybind11::class_<C, options...>& c) {
    c.def("__eq__", &add_eq_operators_detail::no_equality_operators<C>);
    c.def("__ne__", &add_eq_operators_detail::no_equality_operators<C>);
    c.attr("equalityType") = EqualityType::NEVER_INSTANTIATED;
}

template <class C, typename... options>
inline void disable_eq_operators(pybind11::class_<C, options...>& c) {
    c.def("__eq__", &add_eq_operators_detail::disable_equality_operators<C>);
    c.def("__ne__", &add_eq_operators_detail::disable_equality_operators<C>);
    c.attr("equalityType") = EqualityType::DISABLED;
}

inline bool invalidPacketComparison(const regina::Packet&,
        const regina::Packet&) {
    throw std::runtime_error("The comparison operators == and != "
        "now compare packet contents by value, and can only be used with "
        "two packets of the same type.  To test whether two Python objects "
        "wrap the same underlying packet, use Packet.samePacket() instead.");
}

template <class C, typename... options>
inline void packet_eq_operators(pybind11::class_<C, options...>& c) {
    add_eq_operators(c);
    c.def("__eq__", &invalidPacketComparison);
    c.def("__ne__", &invalidPacketComparison);
}

#endif // __DOXYGEN

} } // namespace regina::python
