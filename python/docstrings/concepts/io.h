/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct InherentlyTightEncodable {

// Docstring regina::python::doc::InherentlyTightEncodable::__concept
static constexpr const char __concept[] =
R"doc(A type that has member functions for tight encoding and decoding (both
via strings and via input/output streams) and also hashing.)doc";

}; // struct InherentlyTightEncodable

struct Readable {

// Docstring regina::python::doc::Readable::__concept
static constexpr const char __concept[] = R"doc(A type that can be read from an input stream.)doc";

}; // struct Readable

struct RichStringifiable {

// Docstring regina::python::doc::RichStringifiable::__concept
static constexpr const char __concept[] =
R"doc(A class or struct type that supports Regina's rich string output
interface, via member functions ``str()``, ``detail()``, and
``utf8()``.

Such types often (but are not required to) derive from
``Output<...>``.)doc";

}; // struct RichStringifiable

struct StandardStringifiable {

// Docstring regina::python::doc::StandardStringifiable::__concept
static constexpr const char __concept[] =
R"doc(A type that supports string conversion via ``std::to_string()``.

For example, the standard C++ native integer types support string
conversion in this way.)doc";

}; // struct StandardStringifiable

struct Stringifiable {

// Docstring regina::python::doc::Stringifiable::__concept
static constexpr const char __concept[] =
R"doc(A class or struct type that supports string output through a member
function ``str()``.

Many of Regina's own classes provide such a function.)doc";

}; // struct Stringifiable

struct Writeable {

// Docstring regina::python::doc::Writeable::__concept
static constexpr const char __concept[] = R"doc(A type that can be written to an output stream.)doc";

}; // struct Writeable

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

