
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
 *  \brief Assists with wrapping == and != operators in Python.
 */

#include <type_traits>
#include <boost/python/def_visitor.hpp>

namespace regina {
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
    NEVER_INSTANTIATED = 3
};

/**
 * Adds appropriate == and != operators to the python bindings for a C++ class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>c.def(regina::python::add_eq_operators())</t>, where \a c is the
 * boost::python::class_ object that wraps \a T.  The effect will be as follows:
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
 */
struct add_eq_operators;

/**
 * Indicates that a C++ class is never instantiated, and that its python
 * wrapper class should not support the operators == or !=.
 *
 * This should only be used with C++ classes that are never instantiated (such
 * as Example<dim>, which consists entirely of static methods).
 * As such, it should be impossible to even call the == and != operators
 * under python.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>c.def(regina::python::no_eq_operators())</t>, where \a c is the
 * boost::python::class_ object that wraps \a T.  The effect will be as follows:
 *
 * - Placeholder operators == and != will be added to the python wrapper class
 *   (thus overriding any boost-provided default).  These operators will
 *   throw python exceptions if they are ever called.
 *
 * - The attribute \a equalityType will be added to the python wrapper class.
 *   Its value will be the EqualityType enum constant \a NEVER_INSTANTIATED.
 */
struct no_eq_operators;

#ifndef __DOXYGEN
namespace add_eq_operators_detail {
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

        typedef decltype(makeRef() == makeRef()) EqType;
        typedef decltype(makeRef() != makeRef()) IneqType;

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

// Implementation of add_eq_operators and no_eq_operators.
// See the top of this header for their documentation.

struct add_eq_operators : boost::python::def_visitor<add_eq_operators> {
    friend class boost::python::def_visitor_access;

    template <typename Class>
    void visit(Class& c) const {
        typedef typename Class::wrapped_type Type;

        c.def("__eq__",
            &add_eq_operators_detail::EqualityOperators<Type>::are_equal);
        c.def("__ne__",
            &add_eq_operators_detail::EqualityOperators<Type>::are_not_equal);
        c.attr("equalityType") =
            add_eq_operators_detail::EqualityOperators<Type>::equalityType();
    }
};

struct no_eq_operators : boost::python::def_visitor<no_eq_operators> {
    friend class boost::python::def_visitor_access;

    template <typename T>
    static void no_equality_operators(const T&, const T&) {
        PyErr_SetString(PyExc_RuntimeError,
            "It should be impossible to create objects of this class, and so "
            "there are no operators == or !=.");
    }

    template <typename Class>
    void visit(Class& c) const {
        typedef typename Class::wrapped_type Type;

        c.def("__eq__", &no_equality_operators<Type>);
        c.def("__ne__", &no_equality_operators<Type>);
        c.attr("equalityType") = EqualityType::NEVER_INSTANTIATED;
    }
};

#endif // __DOXYGEN

} } // namespace regina::python
