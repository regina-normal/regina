/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct HilbertPrimal {

// Docstring regina::python::doc::HilbertPrimal::__class
static constexpr const char __class[] =
R"doc(Implements a modified primal algorithm for enumerating Hilbert bases.
This incorporates the primal algorithm described in "Normaliz:
Algorithms for affine monoids and rational cones", Winfried Bruns and
Bogdan Ichim, J. Algebra 324 (2010), 1098-1113, and has been modified
to allow for additional constraints (such as the quadrilateral
constraints from normal surface theory).

To summarise: the algorithm first enumerates extremal rays of the
rational cone, and then decomposes the admissible region of the cone
(where the extra constraints are satisfied) into maximal admissible
faces. It calls Normaliz directly to enumerate the Hilbert basis for
each maximal admissible faces, and finally combines these into a basis
for the entire space.

All routines of interest within this class are static; no object of
this class should ever be created.)doc";

// Docstring regina::python::doc::HilbertPrimal::enumerate
static constexpr const char enumerate[] =
R"doc(Determines the Hilbert basis that generates all integer points in the
intersection of the *n*-dimensional non-negative orthant with some
linear subspace. The resulting basis vectors will be of the class
*Ray*, and will be passed into the given action function one at a
time.

The intersection of the non-negative orthant with this linear subspace
is a pointed polyhedral cone with apex at the origin, and this routine
requires the extremal rays of this cone to be provided as input. The
extremal rays can be computed using ``DoubleDescription::enumerate()``
in general cases, though sometimes (such as with normal surface
theory) more efficient methods are available.

This routine computes the Hilbert basis of all integer points in this
cone. The resulting list of basis vectors will contain no duplicates
or redundancies.

The parameter *constraints* may contain a set of validity constraints,
in which case this routine will only return _valid_ basis vectors.
Each validity constraint is of the form "at most one of these
coordinates may be non-zero"; see the ValidityConstraints class for
details. These contraints have the important property that, although
validity is not preserved under addition, _invalidity_ is.

An optional progress tracker may be passed. If so, this routine will
update the percentage progress and poll for cancellation requests. It
will be assumed that an appropriate stage has already been declared
via ProgressTracker::newStage() before this routine is called, and
that ProgressTracker::setFinished() will be called after this routine
returns.

For each of the resulting basis vectors, this routine will call
*action* (which must be a function or some other callable type). This
action must take exactly one argument: the basis vector stored using
type *Ray*. The argument will be passed as an rvalue; a typical
*action* would take it as an rvalue reference (``Ray&&``) and move its
contents into some other more permanent storage. The return value of
*action* will be ignored (a typical *action* would return ``void``).

Precondition:
    If *constraints* is passed, then the given list of extremal rays
    contains _only_ those extremal rays that satisfy all of the given
    constraints.

.. warning::
    If a progress tracker is passed, be aware that the present
    implementation updates percentage progress very infrequently, and
    may take a very long time to honour cancellation requests.

Exception ``UnsolvedCase``:
    Normaliz was unable to compute the Hilbert basis for one or more
    maximal admissible faces.

Python:
    There are two versions of this function available in Python:

    * ``enumerate(action, rays, constraints, tracker)``, which mirrors
      the C++ function; here *action* may be a pure Python function.

    * ``enumerate(rays, constraints, tracker)``, where the *action*
      argument is missing, and instead the function returns a Python
      list containing all Hilbert basis vectors.

    In both versions, the extremal rays must be passed as either:

    * a Python list of VectorInt objects, which means the output type
      *Ray* will be VectorInt; or

    * a NormalSurfaces or NormalHypersurfaces object, which meams the
      output type *Ray* will be VectorLarge, and the input list of
      extremal rays will be the range from ``rays.beginVectors()`` to
      ``rays.endVectors()``.

    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Template parameter ``Ray``:
    the type to use for the basis vectors that are returned as output.

Template parameter ``RayIterator``:
    the iterator type used to supply the list of extremal rays given
    as input. Each extremal ray should be a vector of arbitrary-
    precision integers; however, the vector interface required of
    these extremal rays is minimal, and in particular both
    ``std::vector`` and ``regina::Vector`` are suitable.

Parameter ``action``:
    a function (or other callable type) that will be called for each
    basis vector. This function must take a single argument, which
    will be passed as an rvalue of type Ray.

Parameter ``beginExtremalRays``:
    an iterator pointing to the beginning of the list of extremal
    rays.

Parameter ``endExtremalRays``:
    an iterator pointing past the end of the list of extremal rays.

Parameter ``constraints``:
    a set of validity constraints as described above, or
    ValidityConstraints::none if none should be imposed.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.)doc";

}; // struct HilbertPrimal

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

