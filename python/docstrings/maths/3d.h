/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Matrix3D
static const char *Matrix3D =
R"doc(Represents a linear transformation in three-dimensional space, as
represented by a real 3-by-3 matrix.

If you are interested specifically in rotations, then you should use
the Rotation3D class instead, which uses a more compact and
numerically stable representation (quaternions).

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

// Docstring regina::python::doc::Rotation3D
static const char *Rotation3D =
R"doc(Represents a rotation about the origin in real three-dimensional
space.

Regina stores a rotation using a _quaternion_, which consists of four
real numbers. We refer to these four numbers as the _quaternion
coordinates_.

Specifically, suppose we rotate by an angle of θ around the axis
pointing from the origin to the unit vector ``(x,y,z)``, and this
rotation follows a right-hand rule (the thumb of the right hand points
from the origin out towards ``(x,y,z)``, and the fingers follow the
direction of the rotation). Then the four real numbers that make up
the quaternion are ``(cos θ/2, x sin θ/2, y sin θ/2, z sin θ/2)``.
Since the axis vector

Regarding normalisation:

* In theory, a quaternion that describes a rotation must be
  _normalised_; that is, its four real quaternion coordinates must
  form a unit vector in 4-D. This follows immediately from the
  discussion above, using the fact that the axis vector ``(x,y,z)`` is
  a unit vector in 3-D.

* In this class, however, we do _not_ require the quaternion
  coordinates to be normalised, since this may allow quaternions to be
  constructed more easily. Instead we allow the quaternion coordinates
  ``(λa, λb, λc, λd)`` to represent the same rotation as ``(a,b,c,d)``
  for any positive λ (and, if we ignore the _direction_ of rotation
  and just consider its endpoint, then for any negative λ also).

* If you wish (though this is not actually necessary), you can
  normalise the coordinates yourself by calling normalise().

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

namespace Matrix3D_ {

// Docstring regina::python::doc::Matrix3D_::__array
static const char *__array =
R"doc(Gives read-write access to a single row of this matrix.

This means that the entry in row *r*, column *c* can be accessed as
``matrix[r][c]`` (where *r* and *c* are each 0, 1 or 2).

Parameter ``row``:
    the index of the requested row; this must be 0, 1 or 2.

Returns:
    a reference to the three-element array containing the elements of
    the requested row.)doc";

// Docstring regina::python::doc::Matrix3D_::__array_2
static const char *__array_2 =
R"doc(Gives read-only access to a single row of this matrix.

This means that the entry in row *r*, column *c* can be accessed as
``matrix[r][c]`` (where *r* and *c* are each 0, 1 or 2).

Parameter ``row``:
    the index of the requested row; this must be 0, 1 or 2.

Returns:
    a three-element array containing the elements of the requested
    row.)doc";

// Docstring regina::python::doc::Matrix3D_::__copy
static const char *__copy = R"doc(Creates a new copy of the given matrix.)doc";

// Docstring regina::python::doc::Matrix3D_::__default
static const char *__default = R"doc(Creates the identity matrix.)doc";

// Docstring regina::python::doc::Matrix3D_::__eq
static const char *__eq =
R"doc(Determines if this and the given matrix are equal.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if and only if the two matrices are equal.)doc";

// Docstring regina::python::doc::Matrix3D_::__init
static const char *__init =
R"doc(Creates a new matrix containin the given entries.

Parameter ``m00``:
    the entry in row 0, column 0.

Parameter ``m01``:
    the entry in row 0, column 1.

Parameter ``m02``:
    the entry in row 0, column 2.

Parameter ``m10``:
    the entry in row 1, column 0.

Parameter ``m11``:
    the entry in row 1, column 1.

Parameter ``m12``:
    the entry in row 1, column 2.

Parameter ``m20``:
    the entry in row 2, column 0.

Parameter ``m21``:
    the entry in row 2, column 1.

Parameter ``m22``:
    the entry in row 2, column 2.)doc";

// Docstring regina::python::doc::Matrix3D_::__ne
static const char *__ne =
R"doc(Determines if this and the given matrix are different.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if and only if the two matrices are not equal.)doc";

}

namespace Rotation3D_ {

// Docstring regina::python::doc::Rotation3D_::__array
static const char *__array =
R"doc(Returns the given quaternion coordinate for this rotation.

Parameter ``index``:
    indicates which coordinate to return; this must be between 0 and 3
    inclusive.

Returns:
    the corresponding quaternion coordinate.)doc";

// Docstring regina::python::doc::Rotation3D_::__copy
static const char *__copy = R"doc(Creates a new copy of the given rotation.)doc";

// Docstring regina::python::doc::Rotation3D_::__default
static const char *__default =
R"doc(Creates the identity rotation. This is the operation that does not
rotate at all.)doc";

// Docstring regina::python::doc::Rotation3D_::__eq
static const char *__eq =
R"doc(Determines if this and the given rotation have the same quaternion
coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the rotation to compare with this.

Returns:
    ``True`` if and only if the two rotations have the same quaternion
    coordinates.)doc";

// Docstring regina::python::doc::Rotation3D_::__init
static const char *__init =
R"doc(Creates a new rotation from the given quaternion coordinates.

As described in the class notes, these coordinates do not need to be
normalised.

Precondition:
    The given coordinates are not all zero.

Parameter ``a``:
    the first quaternion coordinate; that is, the coordinate
    corresponding to ``cos θ/2`` from the discussion in the class
    notes.

Parameter ``b``:
    the second quaternion coordinate; that is, the coordinate
    corresponding to ``x sin θ/2`` from the discussion in the class
    notes.

Parameter ``c``:
    the third quaternion coordinate; that is, the coordinate
    corresponding to ``y sin θ/2`` from the discussion in the class
    notes.

Parameter ``d``:
    the fourth quaternion coordinate; that is, the coordinate
    corresponding to ``z sin θ/2`` from the discussion in the class
    notes.)doc";

// Docstring regina::python::doc::Rotation3D_::__mul
static const char *__mul =
R"doc(Returns the composition of this and the given rotation.

Composition of 3-D rotations is _not_ commutative. Here we follow the
same convention as used elsewhere in Regina (e.g., by Regina's
permutation classes): the product ``q * r`` indicates that we should
apply rotation ``r`` first, followed by rotation ``q``. This is also
consistent with the matrix() function, which produces matrices that
act on column vectors (``matrix * vector``), and which therefore
compose using the same convention.

This function does not require the quaternion coordinates of either
this or *rhs* to be normalised. If the two rotations have coordinates
scaled by λ and μ respectively, then the resulting rotation will have
its coordinates scaled by λμ.

Returns:
    the composition of this and the given rotation.)doc";

// Docstring regina::python::doc::Rotation3D_::__ne
static const char *__ne =
R"doc(Determines if this and the given rotation have different quaternion
coordinates.

.. warning::
    Equality and inequailty testing, while supported, is extremely
    fragile, since it relies on floating-point comparisons.

Parameter ``other``:
    the rotation to compare with this.

Returns:
    ``True`` if and only if the two rotations have different
    quaternion coordinates.)doc";

// Docstring regina::python::doc::Rotation3D_::inverse
static const char *inverse =
R"doc(Returns the inverse to this rotation.

This rotates around the same axis by negative the original angle.

This function does not require the quaternion coordinates to be
normalised. If this rotation has its coordinates scaled by λ, then the
resulting rotation will have its coordinates scaled by λ also.

Returns:
    the inverse rotation.)doc";

// Docstring regina::python::doc::Rotation3D_::matrix
static const char *matrix =
R"doc(Returns the 3-dimensional transformation matrix for this rotation.

The result will be a 3-by-3 matrix ``M``, which can be used to rotate
column vectors by matrix multiplication. Specifically, this rotation
transforms the column vector ``v`` into the vector ``M * v``.

This function does not require the quaternion coordinates to be
normalised. The result will be the same as though normalise() had been
called beforehand.

Returns:
    the corresponding 3-dimensional rotation matrix.)doc";

// Docstring regina::python::doc::Rotation3D_::normalise
static const char *normalise =
R"doc(Rescales all four quaternion coordinates by the same positive constant
so that the quaternion coordinates become normalised.

Specifically, after this operation:

* each quaternion coordinate will have the same sign as it did before
  this operation;

* the four quaternion coordinates ``(a,b,c,d)`` will satisfy ``a^2 +
  b^2 + c^2 + d^2 = 1``.)doc";

}

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

