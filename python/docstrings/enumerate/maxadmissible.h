/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct MaxAdmissible {

// Docstring regina::python::doc::MaxAdmissible::__class
static constexpr const char __class[] =
R"doc(Used to enumerate all maximal admissible faces of a polyhedral cone
under a given set of admissibility constraints. See the routine
enumerate() for details.

All routines of interest within this class are static; no object of
this class should ever be created.)doc";

// Docstring regina::python::doc::MaxAdmissible::enumerate
static constexpr const char enumerate[] =
R"doc(Enumerates all maximal admissible faces of the given polyhedral cone.
The cone must be the intersection of the non-negative orthant in some
Euclidean space ``R^n`` with a linear subspace.

Admissibility is defined by the given set of constraints. Each
constraint requires that at most one of a given set of coordinates can
be non-zero; see the ValidityConstraints class for details. In
particular, the quadrilateral constraints from normal surface theory
are of this type.

It is simple to show that the admissible region of the cone is a union
of faces, which we call _admissible faces_. Those admissible faces
that do not appear as a strict subface of some other admissible face
are called _maximal admissible faces_. The admissible region can
therefore be expressed as the union of all maximal admissible faces.

The input for this routine is the list of all admissible extremal rays
of the cone. These should be computed beforehand; for instance, using
the routine DoubleDescription::enumerate().

The return value is a vector containing all maximal admissible faces.
Each face *F* is described by a bitmask. Specifically: if we are
working in ``R^n``, then each face is described by a bitmask *b* of
length *n*, where ``b[i]`` is ``False`` if every point *x* in *F* has
``x[i]=0``, and ``b[i]`` is ``True`` if every point *x* in the
relative interior of *F* has ``x[i] > 0``.

Template parameter ``BitmaskType``:
    the type to use for the bitmasks that are returned as output. This
    type must be able to hold *n* bits, where *n* is the dimension of
    the underlying space (i.e., the size of each extreme ray given as
    input). This is always true of ``regina::Bitmask``, but you must
    pay attention to this requirement when using one of the fast but
    size-limited types ``regina::Bitmask1`` or ``regina::Bitmask2``.

Template parameter ``RayIterator``:
    the iterator type used to supply the list of admissible extremal
    rays given as input. Typically these extremal rays would be
    integer vectors.

Python:
    The extremal rays should be passed as a Python list of VectorInt
    objects, not a pair of iterators. The bitmasks that are returned
    will be of type regina::Bitmask.

Parameter ``beginExtremalRays``:
    an iterator that begins the set of admissible extremal rays, as
    described above.

Parameter ``endExtremalRays``:
    an iterator that is past-the-end of the set of admissible extremal
    rays.

Parameter ``constraints``:
    a set of validity constraints as described above. This may be
    ValidityConstraints::none to indicate no constraints (in which
    case there will be just one maximal admissible face).

Returns:
    a vector containing one bitmask representing each maximal
    admissible face, as described above.)doc";

}; // struct MaxAdmissible

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

