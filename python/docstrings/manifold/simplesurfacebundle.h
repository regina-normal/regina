/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SimpleSurfaceBundle
static const char *SimpleSurfaceBundle =
R"doc(Represents a particularly simple closed surface bundle over the
circle. Only 2-sphere bundles, twisted 2-sphere bundles and projective
plane bundles are considered.

All optional Manifold routines are implemented for this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
manifold classes.)doc";

namespace SimpleSurfaceBundle_ {

// Docstring regina::python::doc::SimpleSurfaceBundle_::__copy
static const char *__copy = R"doc(Creates a new copy of the given surface bundle.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::__eq
static const char *__eq =
R"doc(Determines whether this and the given surface bundle are of the same
type (i.e., they represent the same 3-manifold).

Parameter ``compare``:
    the surface bundle with which this will be compared.

Returns:
    ``True`` if and only if this and the given surface bundle are of
    the same type.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::__init
static const char *__init =
R"doc(Creates a new surface bundle of the given type.

Parameter ``bundleType``:
    the specific type of surface bundle to represent. This must be one
    of the 3-manifold constants defined in this class.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::__ne
static const char *__ne =
R"doc(Determines whether this and the given surface bundle are of different
types (i.e., they represent different 3-manifolds).

Parameter ``compare``:
    the surface bundle with which this will be compared.

Returns:
    ``True`` if and only if this and the given surface bundle are of
    different types.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given surface bundles.

This global routine simply calls SimpleSurfaceBundle::swap(); it is
provided so that SimpleSurfaceBundle meets the C++ Swappable
requirements.

Parameter ``a``:
    the first surface bundle whose contents should be swapped.

Parameter ``b``:
    the second surface bundle whose contents should be swapped.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given surface bundle.

Parameter ``other``:
    the surface bundle whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SimpleSurfaceBundle_::type
static const char *type =
R"doc(Returns the specific type of surface bundle being represented.

Returns:
    the type of surface bundle. This will be one of the 3-manifold
    constants defined in this class.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

