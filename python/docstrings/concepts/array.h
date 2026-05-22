/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct ArrayOf {

// Docstring regina::python::doc::ArrayOf::__concept
static constexpr const char __concept[] = R"doc(A type of the form ``std::array<Element, n>`` for some integer *n*.)doc";

}; // struct ArrayOf

struct ConstRefArrayOf {

// Docstring regina::python::doc::ConstRefArrayOf::__concept
static constexpr const char __concept[] =
R"doc(A type of the form ``const std::array<Element, n>&`` for some integer
*n*.

This concept could (for example) be used to constrain the type of an
array class constant.)doc";

}; // struct ConstRefArrayOf

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

