/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HilbertDual
static const char *HilbertDual =
R"doc(Implements a modified dual algorithm for enumerating Hilbert bases.
This is based on the dual algorithm as described in "Normaliz:
Algorithms for affine monoids and rational cones", Winfried Bruns and
Bogdan Ichim, J. Algebra 324 (2010), 1098-1113, and has been modified
to allow for additional constraints (such as the quadrilateral
constraints from normal surface theory).

All routines of interest within this class are static; no object of
this class should ever be created.)doc";

namespace HilbertDual_ {

// Docstring regina::python::doc::HilbertDual_::enumerate
static const char *enumerate =
R"doc(Determines the Hilbert basis that generates all integer points in the
intersection of the *n*-dimensional non-negative orthant with the
given linear subspace. The resulting basis elements will be of the
class *RayClass*, and will be passed into the given action function
one at a time.

The non-negative orthant is an *n*-dimensional cone with its vertex at
the origin. The extremal rays of this cone are the *n* non-negative
coordinate axes. This cone also has *n* facets, where the *i*th facet
is the non-negative orthant of the plane perpendicular to the *i*th
coordinate axis.

This routine takes a linear subspace, defined by the intersection of a
set of hyperplanes through the origin (this subspace is described as a
matrix, with each row giving the equation for one hyperplane).

The purpose of this routine is to compute the Hilbert basis of the set
of all integer points in the intersection of the original cone with
this linear subspace. The resulting list of basis vectors will contain
no duplicates or redundancies.

Parameter *constraints* may contain a set of validity constraints, in
which case this routine will only return *valid* basis elements. Each
validity constraint is of the form "a basis element may only lie
outside at most one of these facets of the original cone"; see the
ValidityConstraints class for details. These contraints have the
important property that, although validity is not preserved under
addition, *invalidity* is.

An optional progress tracker may be passed. If so, this routine will
update the percentage progress and poll for cancellation requests. It
will be assumed that an appropriate stage has already been declared
via ProgressTracker::newStage() before this routine is called, and
that ProgressTracker::setFinished() will be called after this routine
returns.

For each of the resulting basis elements, this routine will call
*action* (which must be a function or some other callable object).
This action should return ``void``, and must take exactly one
argument, which will be the basis element stored using *RayClass*. The
argument will be passed as an rvalue; a typical *action* would take it
as an rvalue reference (RayClass&&) and move its contents into some
other more permanent storage.

Precondition:
    The template argument RayClass is derived from (or equal to)
    Vector<T>, where *T* is one of Regina's arbitrary-precision
    integer classes (Integer or LargeInteger).

Python:
    There are two versions of this function available in Python. The
    first version is the same as the C++ function; here you must pass
    *action*, which may be a pure Python function. The second form
    does not have an *action* argument; instead you call
    ``enumerate(subspace, constraints, tracker, initialRows)``, and it
    returns a Python list containing all Hilbert basis elements. In
    both versions, the argument *RayClass* is fixed as VectorInt. The
    global interpreter lock will be released while this function runs,
    so you can use it with Python-based multithreading.

Parameter ``action``:
    a function (or other callable object) that will be called for each
    basis element. This function must take a single argument, which
    will be passed as an rvalue of type RayClass.

Parameter ``subspace``:
    a matrix defining the linear subspace to intersect with the given
    cone. Each row of this matrix is the equation for one of the
    hyperplanes whose intersection forms this linear subspace. The
    number of columns in this matrix must be the dimension of the
    overall space in which we are working.

Parameter ``constraints``:
    a set of validity constraints as described above, or
    ValidityConstraints::none if none should be imposed.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Parameter ``initialRows``:
    specifies how many initial rows of *subspace* are to be processed
    in the precise order in which they appear. The remaining rows will
    be sorted using the PosOrder class before they are processed.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

