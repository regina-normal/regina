/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct RichStringifiable {

// Docstring regina::python::doc::RichStringifiable::__concept
static constexpr const char __concept[] =
R"doc(A class or struct type that supports Regina's rich string output
interface, via member functions ``str()``, ``detail()``, and
``utf8()``.

Such types often (but are not required to) derive from
``Output<...>``.

Concepts:
    RichStringifiable is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct RichStringifiable

struct Stringifiable {

// Docstring regina::python::doc::Stringifiable::__concept
static constexpr const char __concept[] =
R"doc(A class or struct type that supports string output through a member
function ``str()``.

Many of Regina's own classes provide such a function.

Concepts:
    Stringifiable is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct Stringifiable

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

