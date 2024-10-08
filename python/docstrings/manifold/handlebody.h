/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Handlebody
static const char *Handlebody =
R"doc(Represents an orientable handlebody.

All optional Manifold routines are implemented for this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
manifold classes.)doc";

namespace Handlebody_ {

// Docstring regina::python::doc::Handlebody_::__copy
static const char *__copy = R"doc(Creates a new copy of the given handlebody.)doc";

// Docstring regina::python::doc::Handlebody_::__eq
static const char *__eq =
R"doc(Determines whether this and the given handlebody have the same genus.

Handlebodies with the same genus are homeomorphic as 3-manifolds.

Parameter ``compare``:
    the handlebody with which this will be compared.

Returns:
    ``True`` if and only if this and the given handlebody have the
    same genus.)doc";

// Docstring regina::python::doc::Handlebody_::__init
static const char *__init =
R"doc(Creates a new orientable handlebody of the given genus.

Parameter ``genus``:
    the number of handles.)doc";

// Docstring regina::python::doc::Handlebody_::genus
static const char *genus =
R"doc(Returns the genus of this handlebody.

Returns:
    the genus; that is, number of handles.)doc";

// Docstring regina::python::doc::Handlebody_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given handlebodies.

This global routine simply calls Handlebody::swap(); it is provided so
that LensSpace meets the C++ Handlebody requirements.

Parameter ``a``:
    the first handlebody whose contents should be swapped.

Parameter ``b``:
    the second handlebody whose contents should be swapped.)doc";

// Docstring regina::python::doc::Handlebody_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given handlebody.

Parameter ``other``:
    the handlebody whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

