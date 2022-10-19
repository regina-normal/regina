/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Layering
static const char *Layering =
R"doc(Represents a layering of zero or more tetrahedra upon a torus
boundary.

A *layering* involves laying a new tetrahedron flat upon two adjacent
boundary triangles in order to change the boundary curves. Many
tetrahedra may be layered upon a boundary in succession in order to
change the boundary curves more dramatically.

A torus boundary is specified by two tetrahedra (which may be the
same) and two permutations. Each permutation maps (0,1,2) in the
diagram below to the corresponding vertex numbers in each tetrahedron
(and therefore maps 3 to the corresponding face number).

```
    *--->>--*
    |0  2 / |
    |    / 1|
    v   /   v
    |1 /    |
    | / 2  0|
    *--->>--*
```

In particular, if the two tetrahedra are *t0* and *t1* and the two
corresponding permutations are *p0* and *p1*, then:

* the torus boundary is formed from faces *p0*[3] and *p1*[3] of
  tetrahedra *t0* and *t1* respectively;

* edges *p0*[0]-*p0*[1] and *p1*[1]-*p1*[0] of tetrahedra *t0* and
  *t1* respectively are identified;

* edges *p0*[1]-*p0*[2] and *p1*[2]-*p1*[1] of tetrahedra *t0* and
  *t1* respectively are identified;

* edges *p0*[2]-*p0*[0] and *p1*[0]-*p1*[2] of tetrahedra *t0* and
  *t1* respectively are identified.

Note that we do not actually require these triangular faces to form a
torus, and this is never verifed by any of the routines in this class.
What these routines do is use the diagram above to define the rules of
what forms a valid layering (and in fact the layering itself will
often be the cause of these edge identifications). This allows the
Layering class a little more versatility in degenerate and boundary
cases.

This class keeps track of an *old* boundary, which is the original
pair of triangles upon which the first tetrahedron is layered, and a
*new* boundary, which is formed by the last layered tetrahedron and
contains the modified boundary curves. If no tetrahedra are layered at
all then the old and new boundaries will be identical.

This class is used to search for layerings as follows. The constructor
is called with a particular pair of triangles that will form the old
boundary (note that these are generally *not* boundary triangles in
the triangulation, since we are searching for layerings that have been
placed upon them). This forms a trivial (zero-tetrahedron) layering.
The routines extend() or extendOne() are then called to see how many
additional tetrahedra have been layered upon this pair of triangles
according to the rules above.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

namespace Layering_ {

// Docstring regina::python::doc::Layering_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given layering structure.

The new structure will describe the same layering within the same
underlying triangulation.)doc";

// Docstring regina::python::doc::Layering_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
layering.

Specifically, for two layerings to compare as equal, they must:

* use the same numbered tetrahedra in the old boundary, presented in
  the same order and using the same two permutations;

* likewise use the same numbered tetrahedra in the new boundary,
  presented in the same order and using the same two permutations;

* contain the same number of layered tetrahedra, and use the same
  matrix relating the old and new boundary curves.

Since this test looks at tetrahedron numbers and not the specific
Tetrahedron objects, it is meaningful to compare layerings within
different triangulations.

Parameter ``other``:
    the layering to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same layering, as described above.)doc";

// Docstring regina::python::doc::Layering_::__init
static const char *__init =
R"doc(Creates a new trivial (zero-tetrahedron) layering upon the given
boundary.

The boundary is described by two tetrahedra and two permutations as
explained in the class notes. Note that the given tetrahedra need not
be boundary triangles in the triangulation (and if search routines
such as extend() are called then they almost certainly should not be).

Parameter ``bdry0``:
    the tetrahedron providing the first triangle of the boundary.

Parameter ``roles0``:
    the permutation describing how this first triangle is formed from
    three vertices of tetrahedron *bdry0*, as described in the class
    notes.

Parameter ``bdry1``:
    the tetrahedron providing the second triangle of the boundary.

Parameter ``roles1``:
    the permutation describing how this second triangle is formed from
    three vertices of tetrahedron *bdry1*.)doc";

// Docstring regina::python::doc::Layering_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object represent different
layerings.

Specifically, for two layerings to compare as equal, they must:

* use the same numbered tetrahedra in the old boundary, presented in
  the same order and using the same two permutations;

* likewise use the same numbered tetrahedra in the new boundary,
  presented in the same order and using the same two permutations;

* contain the same number of layered tetrahedra, and use the same
  matrix relating the old and new boundary curves.

Since this test looks at tetrahedron numbers and not the specific
Tetrahedron objects, it is meaningful to compare layerings within
different triangulations.

Parameter ``other``:
    the layering to compare with this.

Returns:
    ``True`` if and only if this and the given object represent
    different layerings, as described above.)doc";

// Docstring regina::python::doc::Layering_::boundaryReln
static const char *boundaryReln =
R"doc(Returns a 2-by-2 matrix describing the relationship between curves on
the old and new boundary tori. Note that this relationship will often
be non-trivial, since one of the key reasons for layering is to modify
boundary curves.

Let *t* and *p* be the first tetrahedron and permutation of the old
boundary (as returned by oldBoundaryTet(0) and oldBoundaryRoles(0)),
and let *old_x* and *old_y* be the directed edges *p*[0]-*p*[1] and
*p*[0]-*p*[2] respectively of tetrahedron *t* (these are the leftmost
and uppermost edges of the diagram below). Likewise, let *s* and *q*
be the first tetrahedron and permutation of the new boundary (as
returned by newBoundaryTet(0) and newBoundaryRoles(0)), and let
*new_x* and *new_y* be the directed edges *q*[0]-*q*[1] and
*q*[0]-*q*[2] respectively of tetrahedron *s*.

```
    *--->>--*
    |0  2 / |
    |    / 1|
    v   /   v
    |1 /    |
    | / 2  0|
    *--->>--*
```

Assuming both boundaries are tori, edges *old_x* and *old_y* are
generators of the old boundary torus and edges *new_x* and *new_y* are
generators of the new boundary torus. Suppose that this routine
returns the matrix *M*. This signifies that, using additive notation:

```
    [new_x]         [old_x]
    [     ]  =  M * [     ] .
    [new_y]         [old_y]
```

In other words, the matrix that is returned expresses the generator
curves of the new boundary in terms of the generator curves of the old
boundary.

Note that the determinant of this matrix will always be 1.

Returns:
    the matrix relating the old and new boundary curves.)doc";

// Docstring regina::python::doc::Layering_::extend
static const char *extend =
R"doc(Examines whether one or more additional tetrahedra have been layered
upon the current new boundary.

Specifically, this routine calls extendOne() as many times as
possible. If *k* additional layerings are discovered as a result, the
size of this structure will have grown by *k* and the new boundary
will be changed to describe the remaining two faces of the *kth*
layered tetrahedron.

It is guaranteed that, once this routine is finished, the new boundary
will not have any additional tetrahedron layered upon it. That is, if
extendOne() were called again then it would return ``False``.

Returns:
    the number of additional layered tetrahedra that were discovered.)doc";

// Docstring regina::python::doc::Layering_::extendOne
static const char *extendOne =
R"doc(Examines whether a single additional tetrahedron has been layered upon
the current new boundary.

The new boundary triangles are assumed to form a torus as described in
the class notes (this is not verified, and there are degenerate cases
where this will likely be false). This defines three possible ways in
which an additional tetrahedron may be layered (over the three
boundary edges respectively).

If it is found that an additional tetrahedron does exist and has been
joined to the new boundary in one of these three possible ways, this
structure is extended to incorporate the additional tetrahedron. The
size will grow by one, and the new boundary will become the remaining
two faces of this additional tetrahedron.

Returns:
    ``True`` if a tetrahedron was found as described above and this
    structure was extended accordingly, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Layering_::matchesTop
static const char *matchesTop =
R"doc(Determines whether the new torus boundary of this structure is
identified with the given torus boundary. In other words, this routine
determines whether the new torus boundary of this structure and the
given torus boundary represent opposite sides of the same two
triangles.

The two boundaries must be identified according to some homeomorphism
of the torus. Note that there are 12 different ways in which this can
be done (two choices for which tetrahedron face joins with which, and
then six possible rotations and reflections).

As with the other routines in this class, this routine does not verify
that either boundary in fact forms a torus. Instead, it uses this
assumption to define the rules of what identifications are allowable.

If there is a match, the given matrix *upperReln* will be modified to
describe how the edges of the given boundary relate to the edges of
the old boundary torus. Note that this relationship depends on how the
intermediate tetrahedra are layered (and in fact the purpose of a
layering is often to produce such a non-trivial relationship).

Specifically, let *t0* and *p0* be the first tetrahedron and
permutation of the old boundary (as returned by oldBoundaryTet(0) and
oldBoundaryRoles(0)), and let *x* and *y* be the directed edges
*p0*[0]-*p0*[1] and *p0*[0]-*p0*[2] of tetrahedron *t0* respectively
(these are the leftmost and uppermost edges of the diagram below).
Likewise, let *u* and *q* be the first tetrahedron and permutation of
the given boundary (as passed by parameters *upperBdry0* and
*upperRoles0*), and let *a* and *b* be the directed edges
*q*[0]-*q*[1] and *q*[0]-*q*[2] of tetrahedron *u* respectively.

```
    *--->>--*
    |0  2 / |
    |    / 1|
    v   /   v
    |1 /    |
    | / 2  0|
    *--->>--*
```

Assuming both boundaries are tori, edges *x* and *y* are generators of
the original torus boundary and edges *a* and *b* are generators of
the given torus boundary. Using additive notation, the matrix
*upperReln* is modified so that

```
    [a]                 [x]
    [ ]  =  upperReln * [ ] .
    [b]                 [y]
```

In other words, the modified *upperReln* matrix expresses the
generator curves of the given boundary in terms of the generator
curves of the old boundary.

If no match is found, the matrix *upperReln* is not touched.

Parameter ``upperBdry0``:
    the tetrahedron providing the first triangle of the given
    boundary.

Parameter ``upperRoles0``:
    the permutation describing how this first triangle is formed from
    three vertices of tetrahedron upperBdry0, as described in the
    class notes.

Parameter ``upperBdry1``:
    the tetrahedron providing the second triangle of the given
    boundary.

Parameter ``upperRoles1``:
    the permutation describing how this second triangle is formed from
    three vertices of tetrahedron upperBdry1.

Parameter ``upperReln``:
    the matrix that is changed to reflect the relationship between the
    old boundary of this structure and the given boundary.

Returns:
    ``True`` if the given boundary is found to matche the new boundary
    of this structure, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Layering_::newBoundaryRoles
static const char *newBoundaryRoles =
R"doc(Returns the permutations that describe the new boundary triangles.
These refer to the final boundary after layerings have been performed.

See the Layering class notes for details on how a torus boundary is
formed from two tetrahedra and two permutations.

Parameter ``which``:
    specifies which permutation to return; this must be either 0 or 1.

Returns:
    the requested permutation describing the new boundary.)doc";

// Docstring regina::python::doc::Layering_::newBoundaryTet
static const char *newBoundaryTet =
R"doc(Returns the tetrahedra that provide the new boundary triangles. These
belong to the final boundary after layerings have been performed.

See the Layering class notes for details on how a torus boundary is
formed from two tetrahedra and two permutations.

Parameter ``which``:
    specifies which tetrahedron to return; this must be either 0 or 1.

Returns:
    the requested tetrahedron of the new boundary.)doc";

// Docstring regina::python::doc::Layering_::oldBoundaryRoles
static const char *oldBoundaryRoles =
R"doc(Returns the permutations that describe the old boundary triangles.
These refer to the original boundary before any layerings take place.

See the Layering class notes for details on how a torus boundary is
formed from two tetrahedra and two permutations.

Parameter ``which``:
    specifies which permutation to return; this must be either 0 or 1.

Returns:
    the requested permutation describing the old boundary.)doc";

// Docstring regina::python::doc::Layering_::oldBoundaryTet
static const char *oldBoundaryTet =
R"doc(Returns the tetrahedra that provide the old boundary triangles. These
belong to the original boundary before any layerings take place.

See the Layering class notes for details on how a torus boundary is
formed from two tetrahedra and two permutations.

Parameter ``which``:
    specifies which tetrahedron to return; this must be either 0 or 1.

Returns:
    the requested tetrahedron of the old boundary.)doc";

// Docstring regina::python::doc::Layering_::size
static const char *size =
R"doc(Returns the number of individual tetrahedra that have been layered
onto the original boundary, according to the data stored in this
structure.

This begins at zero when the class constructor is called, and it
increases if the routines extend() or extendOne() find that additional
layerings have taken place.

Returns:
    the number of layered tetrahedra.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

