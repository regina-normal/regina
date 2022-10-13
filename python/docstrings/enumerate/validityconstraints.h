/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ValidityConstraints
static const char *ValidityConstraints =
R"doc(Represents a set of combinatorial validity constraints for use with
polytope vertex enumeration.

Vertex enumeration routines such as DoubleDescription::enumerate()
take a cone (specifically the non-negative orthant), form the
intersection of that cone with a given linear subspace, and return the
extremal rays of the new cone that results.

In some cases we are only interested in *valid* rays of the new cone.
The ValidityConstraints class stores a number of "validity
constraints"; a ray is then "valid" if it satisfies all of these
constraints.

Each individual constraint encodes a subset *S* of coordinate
positions, and requires that a ray can only be non-zero on at most
*one* of those coordinate positions. Equivalently, if we were to
assume that the linear subspace is in general position with respect to
the coordinate axes (which it is often not), then the *i*th facet of
the cone would come from the hyperplane perpendicular to *i*th
coordinate; such a constraint would then require that a ray can only
lie outside at most one of the facets corresponding to the coordinate
positions in *S*.

This class is tailored specifically to constraints that come from
normal surfaces and angle structures:

* We assume that the coordinate positions are grouped into consecutive
  *blocks*, each of the same size. For instance, in standard
  coordinates for normal surfaces, there is one block per tetrahedron,
  each of size seven (since each tetrahedron provides four triangle
  coordinates and three quadrilateral coordinates).

* It is okay if there are additional coordinates that appear beyond
  these blocks (e.g., the final scaling coordinate in angle structure
  coordinates). However, these additional coordinates cannot be used
  in any validity constraints.

It is assumed that all constraints are either *local* or *global:*

* A *local* constraint involves coordinates within a single block
  only. It is assumed that, if local constraints are used, then
  *every* block will use analogous local constraints (where
  "analagous" means they use the same coordinate positions relative to
  the start of each block). An example of a local constraint is the
  quadrilateral constraints from normal surface theory, which require
  that each tetrahedron has at most one non-zero quadrilateral
  coordinate.

* A *global* constraint involves coordinates in every block; moreover,
  each block must constraint the same coordinates relative to the
  start of the block. An example of a global constraint is with almost
  normal surfaces, where we require that the entire surface has at
  most one non-zero octagonal coordinate.

You can add a full set of local constraints (one for each block) using
a single call to addLocal(), and you can add a single global
constraint by calling addGlobal().

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the two given constraint sets.

This global routine simply calls ValidityConstraints::swap(); it is
provided so that ValidityConstraints meets the C++ Swappable
requirements.

Parameter ``a``:
    the first constraint set whose contents should be swapped.

Parameter ``b``:
    the second constraint set whose contents should be swapped.)doc";

namespace ValidityConstraints_ {

// Docstring regina::python::doc::ValidityConstraints_::ValidityConstraints
static const char *ValidityConstraints =
R"doc(Creates an empty set of validity constraints for vectors with the
given block structure.

See the class notes for details on how vector coordinates are assumed
to be grouped into *nBlocks* consecutive blocks, each containing
*blockSize* coordinates (possibly with some additional coordinates
beyond this block structure that are not used in any validity
constraints).

Parameter ``blockSize``:
    the number of coordinates in each block. For example, for vectors
    describing normal surfaces in standard coordinates, this block
    size would be 7 (representing the three triangle and four
    quadrilateral coordinates for each tetrahedron).

Parameter ``nBlocks``:
    the number of consecutive blocks of size *blockSize*. For example,
    for vectors describing normal surfaces in standard coordinates,
    this number of blocks would be the number of tetrahedra in the
    underlying triangulation.

Parameter ``reserveLocal``:
    indicates that we should reserve space for *reserveLocal* calls to
    addLocal(). This is purely for optimisation; it is safe to leave
    this as 0 (the default).

Parameter ``reserveGlobal``:
    indicates that we should reserve space for *reserveGlobal* calls
    to addGlobal(). This is purely for optimisation; it is safe to
    leave this as 0 (the default).)doc";

// Docstring regina::python::doc::ValidityConstraints_::ValidityConstraints_2
static const char *ValidityConstraints_2 = R"doc(Creates a clone of the given constraint set.)doc";

// Docstring regina::python::doc::ValidityConstraints_::__eq
static const char *__eq =
R"doc(Determines whether this and the given set contain the same
constraints.

This test compares the number of coordinates in each block, the total
number of blocks, the set of local constraints, and the set of global
constraints. The local and global constraints may appear in any order,
and their individual coordinates may likewise appear in any order;
such reorderings will not affect the outcome of this test.

.. warning::
    Because this test allows for reordering, the comparison is not
    very efficient. It is assumed that this will not be a problem,
    because typical constraint sets are extremely small.

Parameter ``other``:
    the constraint set to compare against this.

Returns:
    ``True`` if and only if this and the given set contain the same
    constraints.)doc";

// Docstring regina::python::doc::ValidityConstraints_::__ne
static const char *__ne =
R"doc(Determines whether this and the given set do not contain the same
constraints.

This test compares the number of coordinates in each block, the total
number of blocks, the set of local constraints, and the set of global
constraints. The local and global constraints may appear in any order,
and their individual coordinates may likewise appear in any order;
such reorderings will not affect the outcome of this test.

.. warning::
    Because this test allows for reordering, the comparison is not
    very efficient. It is assumed that this will not be a problem,
    because typical constraint sets are extremely small.

Parameter ``other``:
    the constraint set to compare against this.

Returns:
    ``True`` if and only if this and the given set do not contain the
    same constraints.)doc";

// Docstring regina::python::doc::ValidityConstraints_::addGlobal
static const char *addGlobal =
R"doc(Adds one new global constraint to this set.

A call to addGlobal() will add a single global constraint, using
coordinates from every block. Within each block, the coordinates that
are constrained will be those listed in the given iterator range,
where these coordinate positions are given relative to the start of
each block.

As an example, for almost normal surfaces in standard coordinates, you
can encode the constraint that there is at most one octagon in the
entire surface by passing an iterator range that encodes the three
integers 7, 8, 9.

Precondition:
    The iterator type *iterator*, when dereferenced, can be assigned
    to a native C++ ``int``.

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of integers.

Parameter ``begin``:
    the beginning of the list of coordinates to constraint within each
    block, relative to the start of the block, as outlined above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the list of
    coordinates to constraint within each block.)doc";

// Docstring regina::python::doc::ValidityConstraints_::addLocal
static const char *addLocal =
R"doc(Adds a new family of local constraints to this set.

A single call to addLocal() will effectively add one local constraint
for every block. Each local constraint will constraint the coordinates
in the given iterator range, where these coordinate positions are
given relative to the start of each block.

For example, to encode the quadrilateral constraints for normal
surfaces in standard coordinates, you can pass an iterator range that
encodes the three integers 4, 5, 6.

Precondition:
    The iterator type *iterator*, when dereferenced, can be assigned
    to a native C++ ``int``.

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of integers.

Parameter ``begin``:
    the beginning of the list of coordinates to constraint within each
    block, relative to the start of the block, as outlined above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the list of
    coordinates to constraint within each block.)doc";

// Docstring regina::python::doc::ValidityConstraints_::bitmasks
static const char *bitmasks =
R"doc(Returns the list of all individual validity constraints, each
expressed as a bitmask of the given length.

Each local constraint "pattern" that was added using addLocal() will
produce many bitmasks, since there will be one local constraint for
every block. Each global constraint that was added using addGlobal()
will produce just one bitmask.

The bits corresponding to coordinate positions that are constrained
will be set to ``True``; all other bits will be set to ``False``.

Precondition:
    The given length *len* is at least the number of blocks multiplied
    by the block size.

Precondition:
    A bitmask of type *BitmaskType* is large enough to store *len*
    bits.

Python:
    This routine uses the bitmask type regina::Bitmask.

Template parameter ``BitmaskType``:
    the bitmask type used to encode each constraint; this must be one
    of Regina's own bitmask types, such as Bitmask, Bitmask1 or
    Bitmask2.

Parameter ``len``:
    the total number of coordinates in the vectors being constrained.
    Each bitmask will be created with this length.

Returns:
    the list of bitmasks describing the full set of validity
    constraints.)doc";

// Docstring regina::python::doc::ValidityConstraints_::bitmasks_2
static const char *bitmasks_2 =
R"doc(Returns the list of all individual validity constraints, each
expressed as a bitmask of the smallest possible length.

Calling ``bitmasks()`` is equivalent to calling ``bitmasks(len)``,
where *len* is the block size multiplied by the number of blocks.

As an example, this is appropriate for normal surface coordinate
systems, where the normal coordinates incorporate precisely one block
for each tetrahedron and nothing more. However, it is not appropriate
for angle structure coordinates, where there is an additional "scaling
coordinate" that appears after all the blocks.

Each local constraint "pattern" that was added using addLocal() will
produce many bitmasks, since there will be one local constraint for
every block. Each global constraint that was added using addGlobal()
will produce just one bitmask.

The bits corresponding to coordinate positions that are constrained
will be set to ``True``; all other bits will be set to ``False``.

Precondition:
    A bitmask of type *BitmaskType* is large enough to store *len*
    bits, where *len* is the block size multiplied by the number of
    blocks. Each bitmask that is returned will be created with this
    length.

Python:
    This routine uses the bitmask type regina::Bitmask.

Template parameter ``BitmaskType``:
    the bitmask type used to encode each constraint; this must be one
    of Regina's own bitmask types, such as Bitmask, Bitmask1 or
    Bitmask2.

Returns:
    the list of bitmasks describing the full set of validity
    constraints.)doc";

// Docstring regina::python::doc::ValidityConstraints_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given constraint set.

Parameter ``other``:
    the constraint set whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

