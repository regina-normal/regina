/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Triangulation
static const char *Triangulation =
R"doc(Represents a 4-dimensional triangulation, typically of a 4-manifold.

This is a specialisation of the generic Triangulation class template;
see the generic Triangulation documentation for a general overview of
how the triangulation classes work. In Python, you can read this
generic documentation by looking at a higher dimension: try
``help(Triangulation5)``.

This 4-dimensional specialisation offers significant extra
functionality, including many functions specific to 4-manifolds.

A 4-manifold triangulation is built from pentachora: a _pentachoron_
is a 4-dimensional simplex, with five vertices.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace Triangulation_ {

// Docstring regina::python::doc::Triangulation_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given triangulation.

This will also clone any computed properties (such as homology,
fundamental group, and so on), as well as the skeleton (vertices,
edges, components, etc.). In particular, the same numbering and
labelling will be used for all skeletal objects.

If *src* has any locks on top-dimensional simplices and/or their
facets, these locks will also be copied across.

If you want a "clean" copy that resets all properties to unknown, you
can use the two-argument copy constructor instead.

Parameter ``src``:
    the triangulation to copy.)doc";

// Docstring regina::python::doc::Triangulation_::__default
static const char *__default =
R"doc(Default constructor.

Creates an empty triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::__init
static const char *__init =
R"doc(Creates a new copy of the given triangulation, with the option of
whether or not to clone its computed properties and/or locks also.

If *cloneProps* is ``True``, then this constructor will also clone any
computed properties (such as homology, fundamental group, and so on).
If *cloneProps* is ``False``, then these properties will be marked as
unknown in the new triangulation, and will be recomputed on demand
if/when they are required.

Regardless of *cloneProps*, the skeleton (vertices, edges, components,
etc.) will _always_ be cloned. This is to ensure that the same
numbering and labelling will be used for all skeletal objects in both
triangulations.

Likewise, regardless of *cloneProps*, if it is known that all vertex
links of *copy* are 3-sphere or 3-balls, this knowledge will be copied
over to the new triangulation.

If *cloneLocks* is ``True`` then any locks on the top-dimensional
simplices and/or facets of *src* will be copied across. If
*cloneLocks* is ``False`` then the new triangulation will have no
locks at all.

Parameter ``src``:
    the triangulation to copy.

Parameter ``cloneProps``:
    ``True`` if this should also clone any computed properties of the
    given triangulation, or ``False`` if the new triangulation should
    have such properties marked as unknown.

Parameter ``cloneLocks``:
    ``True`` if this should also clone any simplex and/or facet locks
    from the given triangulation, or ``False`` if the new
    triangulation should have no locks at all.)doc";

// Docstring regina::python::doc::Triangulation_::__init_2
static const char *__init_2 =
R"doc("Magic" constructor that tries to find some way to interpret the given
string as a triangulation.

At present, Regina understands the following types of strings (and
attempts to parse them in the following order):

* isomorphism signatures (see fromIsoSig()).

This list may grow in future versions of Regina.

Exception ``InvalidArgument``:
    Regina could not interpret the given string as representing a
    triangulation using any of the supported string types.

Parameter ``description``:
    a string that describes a 4-manifold triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::collapseEdge
static const char *collapseEdge =
R"doc(If possible, performs an edge collapse move upon the given edge. This
involves collapsing the edge to a point, and flattening all of the
pentachora that contain it. The resulting effect is to reduce the
number of vertices in this triangulation by one.

This triangulation will be changed directly.

This move will only be performed if it will not change the topology of
the manifold (as discussed below), _and_ it will not violate any
simplex and/or facet locks. See Simplex<4>::lock() and
Simplex<4>::lockFacet() for further details on locks.

The requirements for this move to not change the topology are complex,
and are discussed in detail in the collapseEdge() source code for
those who are interested. The most important thing to note here is
that checking these requirements is expensive (amongst other things,
we need to build two union-find structures to implement the test).

If you are trying to reduce the number of vertices without changing
the topology, and if *e* is an edge connecting an internal vertex with
some different vertex, then either collapseEdge() or snapEdge() may be
more appropriate for your situation.

* The advantage of collapseEdge() is that it decreases the number of
  tetrahedra, whereas snapEdge() increases this number (but only by
  four).

* The disadvantages of collapseEdge() are that it cannot always be
  performed, and its validity tests are expensive; snapEdge() on the
  other hand can always be used for edges *e* of the type described
  above.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (faces,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge to collapse.

Returns:
    ``True`` if and only if the requested move was able to be
    performed.)doc";

// Docstring regina::python::doc::Triangulation_::collapseEdge_2
static const char *collapseEdge_2 =
R"doc(Deprecated routine that tests for and optionally performs an edge
collapse move upon the given edge of this triangulation.

For more details on edge collapse moves and when they can be
performed, as well as the difference between edge collapse and edge
snap moves, see the variant of collapseEdge() without the extra
boolean arguments.

This routine will always _check_ whether the requested move is legal
and will not violate any simplex and/or facet locks (see
Simplex<4>::lock() and Simplex<4>::lockFacet() for further details on
locks). If the move _is_ allowed, and if the argument *perform* is
``True``, this routine will also _perform_ the move.

.. deprecated::
    If you just wish to test whether such a move is possible, call
    hasCollapseEdge(). If you wish to both check and perform the move,
    call collapseEdge() without the two extra boolean arguments.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge to collapse. :

Parameter ``ignored``:
    an argument that is ignored. In earlier versions of Regina this
    argument controlled whether we check if the move can be performed;
    however, now this check is done always.

Parameter ``perform``:
    ``True`` if we should actually perform the move, assuming the move
    is allowed.

Returns:
    ``True`` if and only if the requested move could be performed.)doc";

// Docstring regina::python::doc::Triangulation_::countBoundaryTetrahedra
static const char *countBoundaryTetrahedra =
R"doc(A dimension-specific alias for countBoundaryFacets().

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::eulerCharManifold
static const char *eulerCharManifold =
R"doc(Returns the Euler characteristic of the corresponding compact
manifold.

Instead of simply calculating ``V-E+F-T+P``, this routine also treats
ideal vertices as 3-manifold boundary components (i.e., effectively
truncates them).

For ideal triangulations, this routine therefore computes the proper
Euler characteristic of the manifold (unlike eulerCharTri(), which
does not).

For triangulations whose vertex links are all 3-spheres or 3-balls,
this routine and eulerCharTri() give identical results.

This routine does _not_ yet handle invalid triangulations correctly.
For this reason, this routine currently insists on a valid
triangulation as a precondition.

Precondition:
    This triangulation is valid.

Returns:
    the Euler characteristic of the corresponding compact manifold.)doc";

// Docstring regina::python::doc::Triangulation_::fourFourMove
static const char *fourFourMove =
R"doc(Deprecated routine that tests for and optionally performes a 4-4 move
about the given edge of this triangulation.

For more details on 4-4 moves and when they can be performed, see
move44().

This routine will always _check_ whether the requested move is legal
and will not violate any simplex and/or facet locks (see
Simplex<4>::lock() and Simplex<4>::lockFacet() for further details on
locks). If the move _is_ allowed, and if the argument *perform* is
``True``, this routine will also _perform_ the move.

.. deprecated::
    If you just wish to test whether such a move is possible, call
    has44(). If you wish to both check and perform the move, call
    move44().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``ignored``:
    an argument that is ignored. In earlier versions of Regina this
    argument controlled whether we check if the move can be performed;
    however, now this check is done always.

Parameter ``perform``:
    ``True`` if we should actually perform the move, assuming the move
    is allowed.

Returns:
    ``True`` if and only if the requested move could be performed.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::has44
static const char *has44 =
R"doc(Determines whether it is possible to perform a 4-4 move about the
given edge of this triangulation, without violating any simplex and/or
facet locks.

For more detail on 4-4 moves and when they can be performed, see
fourFourMove().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the candidate edge about which to perform the move.

Returns:
    ``True`` if and only if the requested move can be performed.)doc";

// Docstring regina::python::doc::Triangulation_::hasBoundaryTetrahedra
static const char *hasBoundaryTetrahedra =
R"doc(A dimension-specific alias for hasBoundaryFacets().

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::hasCollapseEdge
static const char *hasCollapseEdge =
R"doc(Determines whether it is possible to collapse the given edge of this
triangulation, without violating any simplex and/or facet locks.

For more detail on edge collapse moves and when they can be performed,
see collapseEdge().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the candidate edge to collapse.

Returns:
    ``True`` if and only if the requested move can be performed.)doc";

// Docstring regina::python::doc::Triangulation_::hasOpenBook
static const char *hasOpenBook =
R"doc(Determines whether it is possible to perform a book opening move about
the given tetrahedron of this triangulation, without violating any
facet locks.

For more detail on book opening moves and when they can be performed,
see openBook().

Precondition:
    The given tetrahedron is a tetrahedron of this triangulation.

Parameter ``t``:
    the candidate tetrahedron about which to perform the move.

Returns:
    ``True`` if and only if the requested move can be performed.)doc";

// Docstring regina::python::doc::Triangulation_::hasSnapEdge
static const char *hasSnapEdge =
R"doc(Determines whether it is possible to snap together the endpoints of
the given edge of this triangulation.

For more detail on snapping moves on edges and when they can be
performed, see snapEdge().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the candidate edge whose endpoints would be snapped together.

Returns:
    ``True`` if and only if the requested move can be performed.)doc";

// Docstring regina::python::doc::Triangulation_::idealToFinite
static const char *idealToFinite =
R"doc(Converts an ideal triangulation into a finite triangulation. All ideal
or invalid vertices are truncated and thus converted into real
boundary components made from unglued facets of pentachora.

Exception ``LockViolation``:
    This triangulation contains at least one locked top-dimensional
    simplex and/or facet. (This 4-dimensional algorithm does not
    necessarily subdivide _every_ pentachoron, and so this test is
    stronger than necessary; however, it will be enforced.) This
    exception will be thrown before any changes are made. See
    Simplex<4>::lock() and Simplex<4>::lockFacet() for further details
    on how such locks work and what their implications are.

Returns:
    ``True`` if and only if the triangulation was changed.)doc";

// Docstring regina::python::doc::Triangulation_::intelligentSimplify
static const char *intelligentSimplify =
R"doc(Deprecated alias for simplify(), which attempts to simplify this
triangulation as intelligently as possible using fast and greedy
heuristics.

.. deprecated::
    This routine has been renamed to simplify(). See simplify() for
    further details.

Returns:
    ``True`` if and only if the triangulation was changed.)doc";

// Docstring regina::python::doc::Triangulation_::intersectionForm
static const char *intersectionForm =
R"doc(Returns the intersection form of this 4-manifold.

The intersection form is stored as a square matrix with respect to an
arbitrary basis of the second homology group; you can access this
matrix via IntersectionForm::matrix(). This matrix is guaranteed to be
symmetric and unimodular, but will not be normalised in any way. You
can, however, query invariants of the intersection form via routines
such as IntersectionForm::signature() and IntersectionForm::even(),
which in the simply connected case are enough to determine the
topology of the underlying 4-manifold.

(As an implementation detail, the basis is currently chosen to be the
one produced by constructing a MarkedAbelianGroup using the boundary
maps on the dual faces. This specific choice of basis is subject to
change in future releases of Regina.)

The sign convention for counting intersections is as follows: in a
pentachoron with positive orientation and vertices (0,1,2,3,4), the
triangles with ordered vertices (0,1,2) and ordered vertices (2,3,4)
have positive intersection.

Precondition:
    This triangulation is valid, non-empty, orientable and closed.

Exception ``FailedPrecondition``:
    This triangulation is invalid, empty, non-orientable, or not
    closed.

Returns:
    the intersection form of this 4-manifold.)doc";

// Docstring regina::python::doc::Triangulation_::isClosed
static const char *isClosed =
R"doc(Determines if this triangulation is closed. This is the case if and
only if it has no boundary components.

Note that ideal triangulations are not closed. Triangulations with
invalid vertices are also considered not closed; see
Vertex<4>::isBoundary() for details.

Returns:
    ``True`` if and only if this triangulation is closed.)doc";

// Docstring regina::python::doc::Triangulation_::isIdeal
static const char *isIdeal =
R"doc(Determines if this triangulation is ideal.

A triangulation is ideal if and only if (i) the triangulation is
valid, and (ii) one of the vertex links is closed but not a 3-sphere.

Note that for 4-manifolds, isIdeal() will only return ``True`` if the
triangulation is valid (unlike 3-manifolds, where invalid ideal
triangulations are allowed). This is to avoid situations like
4-manifold vertices whose links are cusped 3-manifolds (a situation
that has no analogue in lower dimensions).

Returns:
    ``True`` if and only if this triangulation is ideal.)doc";

// Docstring regina::python::doc::Triangulation_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of the given face as a normal hypersurface.

Constructing the link of a face begins with building the frontier of a
regular neighbourhood of the face. If this is already a normal
hypersurface, then then link is called _thin_. Otherwise some basic
normalisation steps are performed until the hypersurface becomes
normal; note that these normalisation steps could change the topology
of the hypersurface, and in some pathological cases could even reduce
it to the empty hypersurface.

Although normalisation of arbitrary embedded 3-manifolds is messy, for
face links the process is thankfully simpler. Essentially, any changes
will be limited to operations analagous to compressions and boundary
compressions along discs and 3-balls, as well as removing trivial
4-sphere components.

Template parameter ``subdim``:
    the dimension of the face to link; this must be between 0 and 3
    inclusive.

Precondition:
    The given face is a face of this triangulation.

Returns:
    a pair (*s*, *thin*), where *s* is the face linking normal
    hypersurface, and *thin* is ``True`` if and only if this link is
    thin (i.e., no additional normalisation steps were required).)doc";

// Docstring regina::python::doc::Triangulation_::move44
static const char *move44 =
R"doc(If possible, performs a 4-4 move about the given edge of this
triangulation. This involves replacing the four pentachora joined
along that edge with four new pentachora joined along a different (and
new) edge instead.

This triangulation will be changed directly.

This move will only be performed if it will not change the topology of
the manifold (as discussed below), _and_ it will not violate any
simplex and/or facet locks. See Simplex<4>::lock() and
Simplex<4>::lockFacet() for further details on locks.

For this move to make sense, the initial configuration of four
pentachora around the given edge *e* should be equivalent to the join
of a double edge and a square with diagonal given by the edge *e*. The
4-4 move then essentially performs a 2-2 move on this square, with the
new diagonal of the square corresponding precisely to the new edge of
this triangulation that will join the four new pentachora.

Therefore, to be able to perform this move (and to do so without
changing the topology), we require that:

* the link of the given edge is a 2-2 move away from being
  combinatorially isomorphic to the boundary of a tetrahedron; and

* the four pentachora joined along the given edge are distinct.

If this triangulation is currently oriented, then this 4-4 move will
label the new pentachora in a way that preserves the orientation.

Note that after performing this move, all skeletal objects (faces,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge about which to perform the move.

Returns:
    ``True`` if and only if the requested move was able to be
    performed.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::newPentachora
static const char *newPentachora =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newPentachora_2
static const char *newPentachora_2 =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newPentachoron
static const char *newPentachoron =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newPentachoron_2
static const char *newPentachoron_2 =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::openBook
static const char *openBook =
R"doc(If possible, performs a book opening move about the given tetrahedron.
This involves taking a tetrahedron that meets the boundary along
precisely one, two or three triangles, and ungluing it to create two
new boundary facets. The resulting effect is to expose the pentachora
it initially joined, and hopefully open the way for subsequent
boundary shelling moves.

This triangulation will be changed directly.

This move will only be performed if it will not change the topology of
the manifold (as discussed below), _and_ it will not violate any facet
locks. See Simplex<4>::lockFacet() for further details on facet locks.
Note that simplex locks are never a concern for this type of move.

In order not to change the topology, we impose the following
requirements:

* all vertices, edges and triangles of the given tetrahedron are
  valid;

* the given tetrahedron meets the boundary in precisely one, two or
  three triangles (and therefore also joins two pentachora);

* if the given tetrahedron meets the boundary in precisely one
  triangle, then the remaining vertex of the tetrahedron is non-
  boundary, and no two of the remaining three edges of the tetrahedron
  are identified;

* if the given tetrahedron meets the boundary in precisely two
  triangles, then the remaining edge of the tetrahedron is non-
  boundary, and the remaining two triangles of the tetrahedron are not
  identified.

These requirements are stronger than they need to be, but the
resulting "lost opportunities" only affect invalid triangulations.

If this triangulation is currently oriented, then this operation will
(trivially) preserve the orientation.

Note that after performing this move, all skeletal objects (faces,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *t*) can no longer be used.

Precondition:
    The given tetrahedron is a tetrahedron of this triangulation.

Parameter ``t``:
    the tetrahedron about which to perform the move.

Returns:
    ``True`` if and only if the requested move was able to be
    performed.)doc";

// Docstring regina::python::doc::Triangulation_::openBook_2
static const char *openBook_2 =
R"doc(Deprecated routine that tests for and optionally performs a book
opening move about the given tetrahedron of this triangulation.

For more details on book opening moves and when they can be performed,
see the variant of openBook() without the extra boolean arguments.

This routine will always _check_ whether the requested move is legal
and will not violate any facet locks (see Simplex<4>::lockFacet() for
further details on facet locks). Note that this type of move can never
violate a simplex lock, and so there is no need to check for those at
all. If the move _is_ allowed, and if the argument *perform* is
``True``, this routine will also _perform_ the move.

.. deprecated::
    If you just wish to test whether such a move is possible, call
    hasOpenBook(). If you wish to both check and perform the move,
    call openBook() without the two extra boolean arguments.

Precondition:
    The given tetrahedron is a tetrahedron of this triangulation.

Parameter ``t``:
    the tetrahedron about which to perform the move.

Parameter ``ignored``:
    an argument that is ignored. In earlier versions of Regina this
    argument controlled whether we check if the move can be performed;
    however, now this check is done always.

Parameter ``perform``:
    ``True`` if we should actually perform the move, assuming the move
    is allowed.

Returns:
    ``True`` if and only if the requested move could be performed.)doc";

// Docstring regina::python::doc::Triangulation_::removeAllPentachora
static const char *removeAllPentachora =
R"doc(A dimension-specific alias for removeAllSimplices().

See removeAllSimplices() for further information.

Exception ``LockViolation``:
    This triangulation contains at least one locked pentachoron and/or
    facet. This exception will be thrown before any changes are made.
    See Simplex<4>::lock() and Simplex<4>::lockFacet() for further
    details on how such locks work and what their implications are.)doc";

// Docstring regina::python::doc::Triangulation_::removePentachoron
static const char *removePentachoron =
R"doc(A dimension-specific alias for removeSimplex().

See removeSimplex() for further information.

Exception ``LockViolation``:
    The given pentachoron and/or one of its facets is currently
    locked. This exception will be thrown before any changes are made.
    See Simplex<4>::lock() and Simplex<4>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``pent``:
    the pentachoron to remove.)doc";

// Docstring regina::python::doc::Triangulation_::removePentachoronAt
static const char *removePentachoronAt =
R"doc(A dimension-specific alias for removeSimplexAt().

See removeSimplexAt() for further information.

Exception ``LockViolation``:
    The requested pentachoron and/or one of its facets is currently
    locked. This exception will be thrown before any changes are made.
    See Simplex<4>::lock() and Simplex<4>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``index``:
    specifies which pentachoron to remove; this must be between 0 and
    size()-1 inclusive.)doc";

// Docstring regina::python::doc::Triangulation_::retriangulate
static const char *retriangulate =
R"doc(Explores all triangulations that can be reached from this via Pachner
moves, without exceeding a given number of additional pentachora.

Specifically, this routine will iterate through all triangulations
that can be reached from this triangulation via 1-5, 2-4, 3-3 and 4-2
Pachner moves, without ever exceeding *height* additional pentachora
beyond the original number. Note that 5-1 moves are currently not
supported, though this may be added in a future verson of Regina.

For every such triangulation (including this starting triangulation),
this routine will call *action* (which must be a function or some
other callable object).

* *action* must take the following initial argument(s). Either (a) the
  first argument must be a triangulation (the precise type is
  discussed below), representing the triangluation that has been
  found; or else (b) the first two arguments must be of types const
  std::string& followed by a triangulation, representing both the
  triangulation and its isomorphism signature. The second form is
  offered in order to avoid unnecessary recomputation within the
  *action* function; however, note that the signature might not be of
  the IsoSigClassic type (i.e., it might not match the output from the
  default version of isoSig()). If there are any additional arguments
  supplied in the list *args*, then these will be passed as subsequent
  arguments to *action*.

* The triangulation argument will be passed as an rvalue; a typical
  action could (for example) take it by const reference and query it,
  or take it by value and modify it, or take it by rvalue reference
  and move it into more permanent storage.

* *action* must return a boolean. If *action* ever returns ``True``,
  then this indicates that processing should stop immediately (i.e.,
  no more triangulations will be processed).

* *action* may, if it chooses, make changes to this triangulation
  (i.e., the original triangulation upon which retriangulate() was
  called). This will not affect the search: all triangulations that
  this routine visits will be obtained via Pachner moves from the
  original form of this triangulation, before any subsequent changes
  (if any) were made.

* *action* will only be called once for each triangulation (including
  this starting triangulation). In other words, no triangulation will
  be revisited a second time in a single call to retriangulate().

This routine can be very slow and very memory-intensive, since the
number of triangulations it visits may be superexponential in the
number of tetrahedra, and it records every triangulation that it
visits (so as to avoid revisiting the same triangulation again). It is
highly recommended that you begin with *height* = 1, and if necessary
try increasing *height* one at a time until this routine becomes too
expensive to run.

If *height* is negative, then there will be _no_ bound on the number
of additional pentachora. This means that the routine will _never
terminate_, unless *action* returns ``True`` for some triangulation
that is passed to it (or unless there are so many locks that the
number of reachable triangulations becomes finite).

If any pentachora and/or tetrahedra are locked, these locks will be
respected: that is, the retriangulation will avoid any moves that
would violate these locks (and in particular, no LockViolation
exceptions should be thrown). Of course, however, having locks may
reduce the number of distinct triangulations that can be reached.

Since Regina 7.0, this routine will not return until the exploration
of triangulations is complete, regardless of whether a progress
tracker was passed. If you need the old behaviour (where passing a
progress tracker caused the enumeration to start in the background),
simply call this routine in a new detached thread.

To assist with performance, this routine can run in parallel
(multithreaded) mode; simply pass the number of parallel threads in
the argument *threads*. Even in multithreaded mode, this routine will
not return until processing has finished (i.e., either *action*
returned ``True``, or the search was exhausted). All calls to *action*
will be protected by a mutex (i.e., different threads will never be
calling *action* at the same time); as a corollary, the action should
avoid expensive operations where possible (otherwise it will become a
serialisation bottleneck in the multithreading).

If this triangulation is not connected, then this routine will do
nothing: it will immediately return ``False``, and if a progress
tracker was passed then it will be immediately marked as finished.

Precondition:
    This triangulation is connected.

Exception ``FailedPrecondition``:
    This triangulation has more than one connected component. If a
    progress tracker was passed, it will be marked as finished before
    the exception is thrown.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. However, its form is more
    restricted: the arguments *tracker* and *args* are removed, so you
    call it as retriangulate(height, threads, action). Moreover,
    *action* must take exactly two arguments (const std::string&,
    Triangulation<4>&&) representing a signature and the
    triangulation, as described in option (b) above.

Parameter ``height``:
    the maximum number of _additional_ pentachora to allow beyond the
    number of pentachora originally present in the triangulation, or a
    negative number if this should not be bounded.

Parameter ``threads``:
    the number of threads to use. If this is 1 or smaller then the
    routine will run single-threaded.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Parameter ``action``:
    a function (or other callable object) to call for each
    triangulation that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial triangulation argument(s).

Returns:
    ``True`` if some call to *action* returned ``True`` (thereby
    terminating the search early), or ``False`` if the search ran to
    completion.)doc";

// Docstring regina::python::doc::Triangulation_::simplify
static const char *simplify =
R"doc(Attempts to simplify this triangulation as intelligently as possible
using fast and greedy heuristics. Specifically, this routine will
attempt to reduce the number of pentachora in the triangulation.

Currently this routine uses simplifyToLocalMinimum() in combination
with random 3-3 moves and book opening moves.

If simplify() fails to improve the triangulation (which in four
dimensions is likely), you may instead wish to try the well-climbing
routine simplifyUpDown(), or the powerful but *much* slower
simplifyExhaustive().

If this triangulation is currently oriented, then this operation will
preserve the orientation.

If any pentachora and/or tetrahedra are locked, these locks will be
respected: that is, the simplification operation will avoid any moves
that would violate these locks (and in particular, no LockViolation
exceptions should be thrown). Of course, however, having locks may
make the simplification less effective in reducing the number of
pentachora.

.. warning::
    The specific behaviour of this routine will almost certainly
    change between releases. At present, simplification for 4-manifold
    triangulations is extremely weak (as opposed to 3-manifolds, where
    a rich library of simplification techinques is available to call
    upon).

.. note::
    For long-term users of Regina: this is the routine that was for a
    long time called intelligentSimplify(). It was renamed to
    simplify() in Regina 7.4.

Returns:
    ``True`` if and only if the triangulation was changed.)doc";

// Docstring regina::python::doc::Triangulation_::simplifyExhaustive
static const char *simplifyExhaustive =
R"doc(Attempts to simplify this triangulation using a slow but exhaustive
search through the Pachner graph. This routine is more powerful but
much slower than simplify().

Specifically, this routine will iterate through all triangulations
that can be reached from this triangulation via 1-5, 2-4, 3-3 and 4-2
Pachner moves, without ever exceeding *height* additional pentachora
beyond the original number. Note that 5-1 moves are currently not
supported, though this may be added in a future verson of Regina.

If at any stage it finds a triangulation with _fewer_ pentachora than
the original, then this routine will call simplify() to shrink the
triangulation further if possible and will then return ``True``. If it
cannot find a triangulation with fewer pentachora then it will leave
this triangulation unchanged and return ``False``.

This routine can be very slow and very memory-intensive: the number of
triangulations it visits may be superexponential in the number of
pentachora, and it records every triangulation that it visits (so as
to avoid revisiting the same triangulation again). It is highly
recommended that you begin with *height* = 1, and if this fails then
try increasing *height* one at a time until either you find a
simplification or the routine becomes too expensive to run.

If *height* is negative, then there will be _no_ bound on the number
of additional pentachora. This means that the routine will not
terminate until a simpler triangulation is found (unless there are so
many locks that the number of reachable triangulations is finite).
This means that, if no simpler triangulation exists, the only way to
terminate this function is to cancel the operation via a progress
tracker (read on for details).

If any pentachora and/or tetrahedra are locked, these locks will be
respected: that is, the retriangulation will avoid any moves that
would violate these locks (and in particular, no LockViolation
exceptions should be thrown). Of course, however, having locks may
reduce the number of distinct triangulations that can be reached.

If you want a _fast_ simplification routine, you should call
simplify() instead. The benefit of simplifyExhaustive() is that, for
very stubborn triangulations where simplify() finds itself stuck at a
local minimum, simplifyExhaustive() is able to "climb out" of such
wells.

Since Regina 7.0, this routine will not return until either the
triangulation is simplified or the exhaustive search is complete,
regardless of whether a progress tracker was passed. If you need the
old behaviour (where passing a progress tracker caused the exhaustive
search to start in the background), simply call this routine in a new
detached thread.

To assist with performance, this routine can run in parallel
(multithreaded) mode; simply pass the number of parallel threads in
the argument *threads*. Even in multithreaded mode, this routine will
not return until processing has finished (i.e., either the
triangulation was simplified or the search was exhausted), and any
change to this triangulation will happen in the calling thread.

If this routine is unable to simplify the triangulation, then the
triangulation will not be changed.

Precondition:
    This triangulation is connected.

Exception ``FailedPrecondition``:
    This triangulation has more than one connected component. If a
    progress tracker was passed, it will be marked as finished before
    the exception is thrown.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``height``:
    the maximum number of _additional_ pentachora to allow beyond the
    number of pentachora originally present in the triangulation, or a
    negative number if this should not be bounded.

Parameter ``threads``:
    the number of threads to use. If this is 1 or smaller then the
    routine will run single-threaded.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    ``True`` if and only if the triangulation was successfully
    simplified to fewer pentachora.)doc";

// Docstring regina::python::doc::Triangulation_::simplifyToLocalMinimum
static const char *simplifyToLocalMinimum =
R"doc(Uses all known simplification moves to reduce the triangulation
monotonically to some local minimum number of pentachora.

End users will probably not want to call this routine. You should call
simplify() if you want a fast method of simplifying a triangulation.

The moves used by this routine include collapsing edges, 4-2 moves,
and boundary shelling moves.

Moves that do not reduce the number of pentachora (such as 3-3 moves
or book opening moves) are not used in this routine. Such moves do
however feature in simplify().

If this triangulation is currently oriented, then this operation will
preserve the orientation.

If any pentachora and/or tetrahedra are locked, these locks will be
respected: that is, the simplification operation will avoid any moves
that would violate these locks (and in particular, no LockViolation
exceptions should be thrown). Of course, however, having locks may
make the simplification less effective in reducing the number of
pentachora.

.. warning::
    The specific behaviour of this routine will almost certainly
    change between releases. At present, simplification for 4-manifold
    triangulations is extremely weak (as opposed to 3-manifolds, where
    a rich library of simplification techinques is available to call
    upon).

Parameter ``perform``:
    ``True`` if we are to perform the simplifications, or ``False`` if
    we are only to investigate whether simplifications are possible
    (defaults to ``True``).

Returns:
    if *perform* is ``True``, this routine returns ``True`` if and
    only if the triangulation was changed to reduce the number of
    pentachora; if *perform* is ``False``, this routine returns
    ``True`` if and only if it determines that it is capable of
    performing such a change.)doc";

// Docstring regina::python::doc::Triangulation_::simplifyUpDown
static const char *simplifyUpDown =
R"doc(Attempts to simplify this triangulation by making increasingly long
sequences of 2-4 moves and then attempting to simplify back down.

This routine will _only_ perform 2-4 moves, 2-0 edge moves, 2-0
triangle moves, and 3-3 moves.

The main purpose of this routine is to offer a "well-climbing"
technique that explores more widely than simplify(), but that is not
nearly as slow as simplifyExhaustive().

If this triangulation is currently oriented, then this operation will
preserve the orientation.

If any pentachora and/or tetrahedra are locked, these locks will be
respected: that is, this routine will avoid any moves that would
violate these locks (and in particular, no LockViolation exceptions
should be thrown). Of course, however, having locks may make the
simplification less effective in reducing the number of pentachora.

.. warning::
    The specific behaviour of this routine will almost certainly
    change between releases.

Parameter ``max24``:
    the maximum number of consecutive 2-4 moves to perform in a single
    "up" sequence. Note that this routine will attempt several "up"
    sequences of differing lengths, and in particular may eventually
    pass through triangulations with more than ``size() + max24``
    pentachora. If this is -1, then a sensible default will be chosen.

Parameter ``max33``:
    the maximum number of consecutive 3-3 moves to perform immediately
    after each "up" sequence. If this is -1, then a sensible default
    will be chosen.

Parameter ``alwaysModify``:
    ``True`` if this triangulation should be modified after this
    operation, even if the final endpoint has _more_ pentachora than
    the triangulation began with, or ``False`` if this triangulation
    should only be modified if the total number of pentachora was
    strictly reduced.

Returns:
    ``True`` if and only if the number of pentachora was strictly
    reduced.

Author:
    Rhuaidi Burke)doc";

// Docstring regina::python::doc::Triangulation_::snapEdge
static const char *snapEdge =
R"doc(If possible, performs an edge snap move about the given edge. This
involves snapping together the endpoints of the edge, and thereby
reducing the number of vertices in this triangulation by one (but at
the cost of adding a few extra pentachora). See below for more details
on the mechanics of how this move actually operates.

This triangulation will be changed directly.

This move will only be performed if it will not change the topology of
the manifold (as outlined below). Simplex and/or facet locks are never
a concern for this type of move; again see below for details.

In order to not change the toplogy, we require that:

* the given edge joins two distinct vertices of the triangulation;

* at least one of these two vertices is internal.

Depending on your situation, collapseEdge() may be a more appropriate
method for reducing the number of vertices without changing the
topology.

* The advantage of collapseEdge() is that it decreases the number of
  tetrahedra, whereas snapEdge() increases this number (but only by
  four).

* The disadvantages of collapseEdge() are that it cannot always be
  performed, and its validity tests are expensive; snapEdge() on the
  other hand can always be used for edges *e* of the type described
  above.

This operation essentially works by taking a triangle *f* that meets
the given edge *e*, and folding the other two edges of *f* together
about their common vertex. To _implement_ this, we pry open a
tetrahedron *t* and insert a four-pentachoron gadget *g* within the
resulting tetrahedral pillow. In particular, this means that simplex
and/or facet locks will never prevent this operation from taking
place: if the tetrahedron *t* happens to be locked, then this lock
will simply move across to one of the two tetrahedra bounding the
gadget *g*.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (faces,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge whose endpoints are to be snapped together.

Returns:
    ``True`` if and only if the requested move was able to be
    performed.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::snapEdge_2
static const char *snapEdge_2 =
R"doc(Deprecated routine that tests for and optionally performs an edge snap
move upon the given edge of this triangulation.

For more details on edge snap moves and when they can be performed, as
well as the difference between edge snap and edge collapse moves, see
the variant of snapEdge() without the extra boolean arguments.

This routine will always _check_ whether the requested move is legal.
Note that this type of move can never violate a simplex or facet lock,
and so there is no need to check for those at all. If the move _is_
allowed, and if the argument *perform* is ``True``, this routine will
also _perform_ the move.

.. deprecated::
    If you just wish to test whether such a move is possible, call
    hasSnapEdge(). If you wish to both check and perform the move,
    call snapEdge() without the two extra boolean arguments.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``ignored``:
    an argument that is ignored. In earlier versions of Regina this
    argument controlled whether we check if the move can be performed;
    however, now this check is done always.

Parameter ``perform``:
    ``True`` if we should actually perform the move, assuming the move
    is allowed.

Returns:
    ``True`` if and only if the requested move could be performed.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given triangulation.

All pentachora that belong to this triangulation will be moved to
*other*, and all pentachora that belong to *other* will be moved to
this triangulation. Likewise, all skeletal objects (such as lower-
dimensional faces, components, and boundary components) and all cached
properties (such as homology and fundamental group) will be swapped.

In particular, any pointers or references to Pentachoron<4> and/or
Face<4, subdim> objects will remain valid.

This routine will behave correctly if *other* is in fact this
triangulation.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both triangulations which may in turn call
    arbitrary code via any registered packet listeners.

Parameter ``other``:
    the triangulation whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Triangulation_::with44
static const char *with44 =
R"doc(If possible, returns the triangulation obtained by performing a 4-4
move about the given edge of this triangulation. If such a move is not
allowed, or if such a move would violate any simplex and/or facet
locks, then this routine returns no value.

This triangulation will not be changed.

For more detail on 4-4 moves and when they can be performed, see
fourFourMove().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge about which to perform the move.

Returns:
    The new triangulation obtained by performing the requested move,
    or no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Triangulation_::withCollapseEdge
static const char *withCollapseEdge =
R"doc(If possible, returns the triangulation obtained by collapsing the
given edge of this triangulation. If such a move is not allowed, or if
such a move would violate any simplex and/or facet locks, then this
routine returns no value.

This triangulation will not be changed.

For more detail on edge collapse moves and when they can be performed,
see collapseEdge().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge to collapse.

Returns:
    The new triangulation obtained by performing the requested move,
    or no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Triangulation_::withOpenBook
static const char *withOpenBook =
R"doc(If possible, returns the triangulation obtained by performing a book
opening move about the given tetrahedron of this triangulation. If
such a move is not allowed, or if such a move would violate any facet
locks, then this routine returns no value.

This triangulation will not be changed.

For more detail on book opening moves and when they can be performed,
see openBook().

Precondition:
    The given tetrahedron is a tetrahedron of this triangulation.

Parameter ``t``:
    the tetrahedron about which to perform the move.

Returns:
    The new triangulation obtained by performing the requested move,
    or no value if the requested move cannot be performed.)doc";

// Docstring regina::python::doc::Triangulation_::withSnapEdge
static const char *withSnapEdge =
R"doc(If possible, returns the triangulation obtained by snapping together
the endpoints of the given edge of this triangulation. If such a move
is not allowed, then this routine returns no value.

This triangulation will not be changed.

For more detail on snapping moves on edges and when they can be
performed, see snapEdge().

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge whose endpoints are to be snapped together.

Returns:
    The new triangulation obtained by performing the requested move,
    or no value if the requested move cannot be performed.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

