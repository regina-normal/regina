/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace python {

struct EqualityType {

// Docstring regina::python::doc::python::EqualityType::BY_REFERENCE
static constexpr const char BY_REFERENCE[] =
R"doc(Deprecated constant indicating that objects are compared by reference.

.. deprecated::
    This has been renamed to ``ByReference``.)doc";

// Docstring regina::python::doc::python::EqualityType::BY_VALUE
static constexpr const char BY_VALUE[] =
R"doc(Deprecated constant indicating that objects are compared by value.

.. deprecated::
    This has been renamed to ``ByValue``.)doc";

// Docstring regina::python::doc::python::EqualityType::ByReference
static constexpr const char ByReference[] =
R"doc(The objects are compared by reference. This means that the python
operators == and != simply test whether *x* and *y* refer to the same
instance of the C++ class *C*. In other words, they test whether the
underlying C++ _pointers_ to *x* and *y* are the same.)doc";

// Docstring regina::python::doc::python::EqualityType::ByValue
static constexpr const char ByValue[] =
R"doc(The objects are compared by value. This means that the underlying C++
operators == and != for the class *C* are used.)doc";

// Docstring regina::python::doc::python::EqualityType::DISABLED
static constexpr const char DISABLED[] =
R"doc(Deprecated constant indicating that objects cannot be compared by
value but also should not be compared by reference.

.. deprecated::
    This has been renamed to ``Disabled``.)doc";

// Docstring regina::python::doc::python::EqualityType::Disabled
static constexpr const char Disabled[] =
R"doc(Objects of the class *C* cannot be compared by value (because the
comparison operators are not implemented), and they should not be
compared by reference (because they are passed around by value).)doc";

// Docstring regina::python::doc::python::EqualityType::NEVER_INSTANTIATED
static constexpr const char NEVER_INSTANTIATED[] =
R"doc(Deprecated constant indicating that no objects of the class *C* are
ever instantiated.

.. deprecated::
    This has been renamed to ``NeverInstantiated``.)doc";

// Docstring regina::python::doc::python::EqualityType::NeverInstantiated
static constexpr const char NeverInstantiated[] =
R"doc(No objects of the class *C* are ever instantiated. This means that no
comparisons are ever made. An example of such a class is Example<dim>,
which consists entirely of static functions.)doc";

// Docstring regina::python::doc::python::EqualityType::__class
static constexpr const char __class[] =
R"doc(Indicates the different ways in which the equality (==) and inequality
(!=) operators can behave under Python. This behaviour differs
according to the underlying class of the objects being compared.

Specifically, suppose we have two python objects *x* and *y* that wrap
instances of some underlying C++ class *C*. Then, if we test ``x ==
y`` or ``x != y`` under python, the possible behaviours are:)doc";

}; // struct EqualityType

} // namespace python

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

