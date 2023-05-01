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

// Docstring regina::python::doc::python::EqualityType
static const char *EqualityType =
R"doc(Indicates the different ways in which the equality (==) and inequality
(!=) operators can behave under Python. This behaviour differs
according to the underlying class of the objects being compared.

Specifically, suppose we have two python objects *x* and *y* that wrap
instances of some underlying C++ class *C*. Then, if we test ``x ==
y`` or ``x != y`` under python, the possible behaviours are:)doc";

}

namespace python::EqualityType_ {

// Docstring regina::python::doc::python::EqualityType_::BY_REFERENCE
static const char *BY_REFERENCE =
R"doc(The objects are compared by reference. This means that the python
operators == and != simply test whether *x* and *y* refer to the same
instance of the C++ class *C*. In other words, they test whether the
underlying C++ _pointers_ to *x* and *y* are the same.)doc";

// Docstring regina::python::doc::python::EqualityType_::BY_VALUE
static const char *BY_VALUE =
R"doc(The objects are compared by value. This means that the underlying C++
operators == and != for the class *C* are used.)doc";

// Docstring regina::python::doc::python::EqualityType_::DISABLED
static const char *DISABLED =
R"doc(Objects of the class *C* cannot be compared by value (because the
comparison operators are not implemented), and they should not be
compared by reference (because they are passed around by value).)doc";

// Docstring regina::python::doc::python::EqualityType_::NEVER_INSTANTIATED
static const char *NEVER_INSTANTIATED =
R"doc(No objects of the class *C* are ever instantiated. This means that no
comparisons are ever made. An example of such a class is Example<dim>,
which consists entirely of static functions.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

