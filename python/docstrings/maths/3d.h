/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Vector3D
static const char *Vector3D =
R"doc(Represents a vector in real three-dimensional space. This class is
also used to represent a single 3-D point.

See Regina's notes on 3-D geometry for importing information,
including the inexact floating-point nature of the Vector3D class, and
the right-handedness of Regina's coordinate system.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    The template parameter *Real* is ``double``.

Template parameter ``Real``:
    the floating-point type to use for all storage and computation.)doc";

namespace Vector3D_ {

// Docstring regina::python::doc::Vector3D_::__add
static const char *__add =
R"doc(Returns the sum of this and the given vector.

Parameter ``rhs``:
    the vector to add to this vector.

Returns:
    the sum of this and the given vector.)doc";

// Docstring regina::python::doc::Vector3D_::__copy
static const char *__copy = R"doc(Creates a new copy of the given vector.)doc";

// Docstring regina::python::doc::Vector3D_::__default
static const char *__default = R"doc(Creates a new vector whose coordinates are uninitialised.)doc";

// Docstring regina::python::doc::Vector3D_::__div
static const char *__div =
R"doc(Returns a copy of this vector scaled down by the given factor.
Specifically, the coordinates of the vector that is returned will be
the coordinates of this vector divided by *scale*.

Parameter ``scale``:
    the scaling factor to apply; this must be non-zero.

Returns:
    a rescaled copy of this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__eq
static const char *__eq =
R"doc(Determines if this and the given vector have the same coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the vector to compare with this.

Returns:
    ``True`` if and only if the two vectors are equal.)doc";

// Docstring regina::python::doc::Vector3D_::__iadd
static const char *__iadd =
R"doc(Adds the given vector to this vector.

Parameter ``rhs``:
    the vector to add to this vector.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__idiv
static const char *__idiv =
R"doc(Scales this vector down by the given factor. Specifically, all
coordinates of this vector will be divided by *scale*.

Parameter ``scale``:
    the scaling factor to apply; this must be non-zero.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__imul
static const char *__imul =
R"doc(Scales this vector up by the given factor. Specifically, all
coordinates of this vector will be multiplied by *scale*.

Parameter ``scale``:
    the scaling factor to apply.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__init
static const char *__init =
R"doc(Creates a new vector with the given coordinates.

Parameter ``x``:
    the first (x) coordinate.

Parameter ``y``:
    the second (y) coordinate.

Parameter ``z``:
    the third (z) coordinate.)doc";

// Docstring regina::python::doc::Vector3D_::__init_2
static const char *__init_2 =
R"doc(Creates a new vector with the given coordinates.

Parameter ``coordinates``:
    array whose three elements are the \z x, *y* and *z* coordinate
    respectively.)doc";

// Docstring regina::python::doc::Vector3D_::__isub
static const char *__isub =
R"doc(Subtracts the given vector from this vector.

Parameter ``rhs``:
    the vector to subtract from this vector.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__mul
static const char *__mul =
R"doc(Returns a copy of this vector scaled up by the given factor.
Specifically, the coordinates of the vector that is returned will be
the coordinates of this vector multiplied by *scale*.

Parameter ``scale``:
    the scaling factor to apply.

Returns:
    a rescaled copy of this vector.)doc";

// Docstring regina::python::doc::Vector3D_::__ne
static const char *__ne =
R"doc(Determines if this and the given vector have different coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the vector to compare with this.

Returns:
    ``True`` if and only if the two vectors are different.)doc";

// Docstring regina::python::doc::Vector3D_::__sub
static const char *__sub =
R"doc(Returns the difference obtained from this vector minus the given
vector.

If both this and *rhs* represent 3-D points, then the result is the
vector pointing from *rhs* to this point.

Parameter ``rhs``:
    the vector to subtract from this vector.

Returns:
    the difference obtained from this minus the given vector.)doc";

// Docstring regina::python::doc::Vector3D_::distance
static const char *distance =
R"doc(Returns the distance between this and the given point in 3-D space.

The naming and description of this function assume that both this and
*other* represent individual points in 3-D space.

Computing ``a.distance(b)`` is equivalent to computing
``(a-b).length()``.

Returns:
    the distance between this and the given point.)doc";

// Docstring regina::python::doc::Vector3D_::length
static const char *length =
R"doc(Returns the length of this vector.

If this object represents a single 3-D point, then this function
returns the distance of this point from the origin.

Returns:
    the length of this vector.)doc";

// Docstring regina::python::doc::Vector3D_::midpoint
static const char *midpoint =
R"doc(Returns the midpoint between this and the given point.

The naming and description of this function assume that both this and
*other* represent individual points in 3-D space.

Computing ``a.midpoint(b)`` is equivalent to computing ``(a+b)/2``.

Returns:
    the midpoint between this and the given point.)doc";

// Docstring regina::python::doc::Vector3D_::x
static const char *x = R"doc(The first (x) coordinate of the vector.)doc";

// Docstring regina::python::doc::Vector3D_::y
static const char *y = R"doc(The second (y) coordinate of the vector.)doc";

// Docstring regina::python::doc::Vector3D_::z
static const char *z = R"doc(The third (z) coordinate of the vector.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

