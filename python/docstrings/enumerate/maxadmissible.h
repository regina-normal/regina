/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::MaxAdmissible
static const char *MaxAdmissible =
R"doc(Used to enumerate all maximal admissible faces of a polyhedral cone
under a given set of admissibility constraints. See the routine
enumerate() for details.

All routines of interest within this class are static; no object of
this class should ever be created.)doc";

namespace MaxAdmissible_ {

// Docstring regina::python::doc::MaxAdmissible_::enumerate
static const char *enumerate =
R"doc(Enumerates all maximal admissible faces of the given polyhedral cone.
The cone must be the intersection of the non-negative orthant in some
Euclidean space R^n with a linear subspace.

Admissibility is defined by the given set of constraints. Each
constraint requires that at most one of a given set of coordinates can
be non-zero; see the ValidityConstraints class for details. In
particular, the quadrilateral constraints from normal surface theory
are of this type.

It is simple to show that the admissible region of the cone is a union
of faces, which we call "admissible faces". Those admissible faces
that do not appear as a strict subface of some other admissible face
are called "maximal admissible faces". The admissible region can
therefore be expressed as the union of all maximal admissible faces.

The input for this routine is the set of all admissible extremal rays
of the cone. These should be computed beforehand; for instance, using
the routine DoubleDescription::enumerate().

The return value is a vector containing all maximal admissible faces.
Each face *F* is described by a bitmask. Specifically: if we are
working in R^n, then each face is described by a bitmask *b* of length
n, where ``b[i]`` is ``False`` if every point *x* in *F* has
``x[i]=0``, and ``b[i]`` is ``True`` if every point *x* in the
relative interior of *F* has ``x[i] > 0``.

Precondition:
    The template argument RayIterator should be an iterator type that,
    when dereferenced, can be treated as a vector of integers. The
    exact vector and integer types are unimportant; all that matters
    is that, for any such iterator *it* and any integer index *i*, we
    can test ``(*it)[i] == 0`` and ``(*it)[i] != 0``.

Precondition:
    The template argument BitmaskType is one of the bitmask types
    Bitmask, Bitmask1 or Bitmask2.

Precondition:
    Bitmasks of type BitmaskType can hold *n* bits, where *n* is the
    dimension of the underlying space (i.e., the size of the input
    vectors described by *beginExtremalRays* and *endExtremalRays*).
    This is always true of Bitmask, but you must be careful when using
    one of the fast but size-limited types Bitmask1 or Bitmask2.

Python:
    The extremal rays should be passed as a Python list of VectorInt
    objects, not a pair of iterators. The bitmasks that are returned
    will be of type regina::Bitmask.

Parameter ``beginExtremalRays``:
    an iterator that begins the set of admissible extremal rays, as
    described above. Typically this would be rays.begin() if *rays* is
    a standard container type.

Parameter ``endExtremalRays``:
    an iterator that is past-the-end of the set of admissible extremal
    rays. Typically this would be rays.end() if *rays* is a standard
    container type.

Parameter ``constraints``:
    a set of validity constraints as described above. This may be
    ValidityConstraints::none to indicate no constraints (in which
    case there will be just one maximal admissible face).

Returns:
    a vector containing one bitmask representing each maximal
    admissible face, as described above.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

