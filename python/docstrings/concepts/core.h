/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct AssignableTo {

// Docstring regina::python::doc::AssignableTo::__concept
static constexpr const char __concept[] =
R"doc(Indicates that a variable of type *Source* can be assigned to a
variable of type *Target*. This is identical to
``std::assignable_from``, but with the arguments in the opposite
order.

Typically *Target* would be an lvalue reference.

Concepts:
    AssignableTo is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct AssignableTo

struct CanConstruct {

// Docstring regina::python::doc::CanConstruct::__concept
static constexpr const char __concept[] =
R"doc(Indicates that a variable of type *Source* can be used to construct a
variable of type *Target*. This is identical to
``std::constructible_from``, but with the arguments in the opposite
order.

Concepts:
    CanConstruct is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct CanConstruct

struct CoefficientDomain {

// Docstring regina::python::doc::CoefficientDomain::__concept
static constexpr const char __concept[] =
R"doc(A type suitable to use for coefficients in Regina's polynomial-like
classes.

This concept is tailored to Regina's own requirements, and so is
stricter than the mathematical requirements for polynomial
coefficients. For example, we insist here on no zero divisors (to
support division-related algorithms), and we insist on default
constructors that initialise to zero (to simplify algorithm
implementations).

Concepts:
    CoefficientDomain is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct CoefficientDomain

struct Domain {

// Docstring regina::python::doc::Domain::__concept
static constexpr const char __concept[] =
R"doc(A ring with no zero divisors.

The property of having no zero divisors is self-identified through the
specialisation ``RingTraits<T>``.

Concepts:
    Domain is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct Domain

struct Field {

// Docstring regina::python::doc::Field::__concept
static constexpr const char __concept[] =
R"doc(A mathematical field.

The property of being a field is self-identified through the various
constants in the specialisation ``RingTraits<T>``.

Concepts:
    Field is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct Field

struct IntegralDomain {

// Docstring regina::python::doc::IntegralDomain::__concept
static constexpr const char __concept[] =
R"doc(A commutative ring with no zero divisors.

Commutativity and the property of having no zero divisors are both
self-identified through the specialisation ``RingTraits<T>``.

Concepts:
    IntegralDomain is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct IntegralDomain

struct ReginaBitmask {

// Docstring regina::python::doc::ReginaBitmask::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's own bitmask types.

Concepts:
    ReginaBitmask is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct ReginaBitmask

struct ReginaQitmask {

// Docstring regina::python::doc::ReginaQitmask::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's own qitmask types.

Concepts:
    ReginaQitmask is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct ReginaQitmask

struct Ring {

// Docstring regina::python::doc::Ring::__concept
static constexpr const char __concept[] =
R"doc(A type that behaves like a mathematical ring, and for which the
specialisation ``RingTraits<T>`` is available.

Concepts:
    Ring is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct Ring

struct RingLike {

// Docstring regina::python::doc::RingLike::__concept
static constexpr const char __concept[] =
R"doc(A type that has the necessary operations to behave like a mathematical
ring.

Concepts:
    RingLike is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct RingLike

struct SameModCVRef {

// Docstring regina::python::doc::SameModCVRef::__concept
static constexpr const char __concept[] =
R"doc(Indicates that types *T* and *U* are identical, after removing
references and const/volatile qualifiers.

So, for example, ``std::same_as<const int&, int>`` is ``False``, but
``SameModCVRef<const int&, int>`` is ``True``.

Concepts:
    SameModCVRef is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct SameModCVRef

struct StrictWeakOrder {

// Docstring regina::python::doc::StrictWeakOrder::__concept
static constexpr const char __concept[] =
R"doc(A callable type that acts as a strict weak order on the given argument
type. Such an object could (for example) be used for comparisons
during a sorting operation.

This concept is identical to the standard C++ concept
``std::strict_weak_order<T, Arg, Arg>``. It is provided here for
convenience so that the argument type does not need to be repeated.

Concepts:
    StrictWeakOrder is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct StrictWeakOrder

struct TerminatingCallback {

// Docstring regina::python::doc::TerminatingCallback::__concept
static constexpr const char __concept[] =
R"doc(A callable type that takes the given argument types, with a boolean
return value indicating whether the current operation should
terminate. Such objects are often passed into Regina's enumeration
and/or exploration routines (e.g., for enumerating normal surfaces, or
exploring nearby retriangulations).

The return type for such a callback must be convertible to ``bool``. A
return value of ``False`` (no, do not terminate) would typically
indicate that the current operation should continue, and ``True``
(yes, terminate) would typically indicate that it the operation should
stop.

Concepts:
    TerminatingCallback is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct TerminatingCallback

struct VoidCallback {

// Docstring regina::python::doc::VoidCallback::__concept
static constexpr const char __concept[] =
R"doc(A callable type that takes the given argument types, and whose return
value is ignored. Such objects are often passed into Regina's
enumeration routines (e.g., the various triangulation and link census
generation routines).

Typically the return type for such a callback would be ``void`` (since
Regina will ignore it), though this is not enforced.

Concepts:
    VoidCallback is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct VoidCallback

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

