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
R"doc(Represents a 3-dimensional triangulation, typically of a 3-manifold.

This is a specialisation of the generic Triangulation class template;
see the generic Triangulation documentation for a general overview of
how the triangulation classes work. In Python, you can read this
generic documentation by looking at a higher dimension: try
``help(Triangulation5)``.

This 3-dimensional specialisation offers significant extra
functionality, including many functions specific to 3-manifolds.

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

If you want a "clean" copy that resets all properties to unknown and
leaves the skeleton uncomputed, you can use the two-argument copy
constructor instead.

Parameter ``src``:
    the triangulation to copy.)doc";

// Docstring regina::python::doc::Triangulation_::__default
static const char *__default =
R"doc(Default constructor.

Creates an empty triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::__init
static const char *__init =
R"doc(Creates a new copy of the given triangulation, with the option of
whether or not to clone its computed properties also.

If *cloneProps* is ``True``, then this constructor will also clone any
computed properties (such as homology, fundamental group, and so on),
as well as the skeleton (vertices, edges, components, etc.). In
particular, the same numbering and labelling will be used for all
skeletal objects in both triangulations.

If *cloneProps* is ``False``, then these properties and skeletal
objects will be marked as unknown in the new triangulation, and will
be recomputed on demand if/when they are required. Note in particular
that, when the skeleton is recomputed, there is no guarantee that the
numbering and labelling for skeletal objects will be the same as in
the source triangulation.

If *src* has any locks on top-dimensional simplices and/or their
facets, these locks will be copied across _only_ if *cloneProps* is
``True``. If *cloneProps* is ``False`` then the new triangulation will
have no locks at all.

Parameter ``src``:
    the triangulation to copy.

Parameter ``cloneProps``:
    ``True`` if this should also clone any computed properties as well
    as the skeleton of the given triangulation, or ``False`` if the
    new triangulation should have such properties and skeletal data
    marked as unknown.)doc";

// Docstring regina::python::doc::Triangulation_::__init_2
static const char *__init_2 =
R"doc(Creates a new ideal triangulation representing the complement of the
given link in the 3-sphere.

The triangulation will have one ideal vertex for each link component.
Assuming you pass *simplify* as ``True`` (the default), there will
typically be no internal vertices; however, this is not guaranteed.

Initially, each tetrahedron will be oriented according to a right-hand
rule: the thumb of the right hand points from vertices 0 to 1, and the
fingers curl around to point from vertices 2 to 3. If you pass
*simplify* as ``True``, then Regina will attempt to simplify the
triangulation to as few tetrahedra as possible: this may relabel the
tetrahedra, though their orientations will be preserved.

This is the same triangulation that is produced by Link::complement().

Parameter ``link``:
    the link whose complement we should build.

Parameter ``simplify``:
    ``True`` if and only if the triangulation should be simplified to
    use as few tetrahedra as possible.)doc";

// Docstring regina::python::doc::Triangulation_::__init_3
static const char *__init_3 =
R"doc("Magic" constructor that tries to find some way to interpret the given
string as a triangulation.

At present, Regina understands the following types of strings (and
attempts to parse them in the following order):

* isomorphism signatures (see fromIsoSig());

* dehydration strings (see rehydrate());

* the contents of a SnapPea data file (see fromSnapPea()).

This list may grow in future versions of Regina.

.. warning::
    If you pass the contents of a SnapPea data file, then only the
    tetrahedron gluings will be read; all other SnapPea-specific
    information (such as peripheral curves) will be lost. See
    fromSnapPea() for details, and for other alternatives that
    preserve SnapPea-specific data.

Exception ``InvalidArgument``:
    Regina could not interpret the given string as representing a
    triangulation using any of the supported string types.

Parameter ``description``:
    a string that describes a 3-manifold triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::__init_4
static const char *__init_4 =
R"doc(Python-only constructor that copies the given SnapPy manifold.

Although the argument *m* would typically be a ``SnapPy.Manifold``, it
could in fact be anything with a ``_to_string()`` method (so you could
instead pass a ``SnapPy.Triangulation``, for example). Regina will
then call ``m._to_string()`` and pass the result to the "magic" string
constructor for Regina's Triangulation3 class. Typically, if *m* is a
SnapPy object, this means that ``m._to_string()`` would need to return
the contents of a SnapPy/SnapPea data file.

.. warning::
    Only the tetrahedron gluings will be copied; all other SnapPy-
    specific information (such as peripheral curves) will be lost. See
    fromSnapPea() for details, and for other alternatives that
    preserve SnapPy-specific data.

Parameter ``m``:
    a SnapPy object of type snappy.Manifold.)doc";

// Docstring regina::python::doc::Triangulation_::allCalculatedTuraevViro
static const char *allCalculatedTuraevViro =
R"doc(Returns the cache of all Turaev-Viro state sum invariants that have
been calculated for this 3-manifold. This cache is updated every time
turaevViro() is called, and is emptied whenever the triangulation is
modified.

Turaev-Viro invariants are identified by an (*r*, *parity*) pair as
described in the turaevViro() documentation. The cache is just a set
that maps (*r*, *parity*) pairs to the corresponding invariant values.

For even values of *r*, the parity is ignored when calling
turaevViro() (since the even and odd versions of the invariant contain
essentially the same information). Therefore, in this cache, all even
values of *r* will have the corresponding parities set to ``False``.

.. note::
    All invariants in this cache are now computed using exact
    arithmetic, as elements of a cyclotomic field. This is a change
    from Regina 4.96 and earlier, which computed floating-point
    approximations instead.

Python:
    This routine returns a Python dictionary. It also returns by
    value, not by reference (i.e., if more Turaev-Viro invariants are
    computed later on, the dictionary that was originally returned
    will not change as a result).

Returns:
    the cache of all Turaev-Viro invariants that have already been
    calculated.

See also:
    turaevViro)doc";

// Docstring regina::python::doc::Triangulation_::closeBook
static const char *closeBook =
R"doc(Checks the eligibility of and/or performs a book closing move about
the given boundary edge. This involves taking a boundary edge of the
triangulation and folding together the two boundary triangles on
either side. This move is the inverse of the openBook() move, and is
used to simplify the boundary of the triangulation. This move can be
done if:

* the edge *e* is a boundary edge;

* the two vertices opposite *e* in the boundary triangles that contain
  *e* are valid and distinct;

* the boundary component containing *e* contains more than two
  triangles.

There are in fact several other distinctness conditions on the nearby
edges and triangles, but they follow automatically from the conditions
above.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
facet locks (see Simplex<3>::lockFacet() for further details on
locks).

If this triangulation is currently oriented, then this operation will
(trivially) preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *f*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any facet locks.

Precondition:
    The given edge is an edge of this triangulation.

Exception ``LockViolation``:
    This move would violate a facet lock, and *check* was passed as
    ``False``. This exception will be thrown before any changes are
    made. See Simplex<3>::lockFacet() for details on how facet locks
    work and what their implications are.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::collapseEdge
static const char *collapseEdge =
R"doc(Checks the eligibility of and/or performs a collapse of an edge
between two distinct vertices. This operation (when it is allowed)
does not change the topology of the manifold, decreases the number of
vertices by one, and also decreases the number of tetrahedra.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<3>::lock() and
Simplex<3>::lockFacet() for further details on locks).

If you are trying to reduce the number of vertices without changing
the topology, and if *e* is an edge connecting an internal vertex with
some different vertex, then either collapseEdge() or pinchEdge() may
be more appropriate for your situation (though you may find it easier
just to call minimiseVertices() instead).

* The advantage of collapseEdge() is that it decreases the number of
  tetrahedra, whereas pinchEdge() increases this number (but only by
  two).

* The disadvantages of collapseEdge() are that it cannot always be
  performed, and its validity tests are expensive; pinchEdge() on the
  other hand can always be used for edges *e* of the type described
  above.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

The eligibility requirements for this move are somewhat involved, and
are discussed in detail in the collapseEdge() source code for those
who are interested.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given edge is an edge of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how locks work and
    what their implications are.

Parameter ``e``:
    the edge to collapse.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the given edge may be collapsed without changing the topology
    of the manifold or violating any locks. If *check* is ``False``,
    the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::connectedSumWith
static const char *connectedSumWith =
R"doc(Forms the connected sum of this triangulation with the given
triangulation. This triangulation will be altered directly.

If this and the given triangulation are both oriented, then the result
will be oriented also, and the connected sum will respect these
orientations.

If one or both triangulations contains multiple connected components,
this routine will connect the components containing tetrahedron 0 of
each triangulation, and will copy any additional components across
with no modification.

If either triangulation is empty, then the result will simply be a
clone of the other triangulation.

This and/or the given triangulation may be bounded or ideal, or even
invalid; in all cases the connected sum will be formed correctly.
Note, however, that the result might possibly contain internal
vertices (even if the original triangulations do not).

Tetrahedron and/or facet locks will not prevent the connected sum from
taking place. The operation essentially involves prying open two
triangles (one from each triangulation) and joining them with a
connector gadget; if some original triangle *t* is locked then the
lock will be pushed across to one of two triangles that results when
*t* is pried open. In particular, if *t* is a boundary triangle then
the lock will be kept on the boundary (as expected).

It is allowed to pass this triangulation as *other*.

Parameter ``other``:
    the triangulation to sum with this.)doc";

// Docstring regina::python::doc::Triangulation_::countBoundaryTriangles
static const char *countBoundaryTriangles =
R"doc(A dimension-specific alias for countBoundaryFacets().

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::dehydrate
static const char *dehydrate =
R"doc(Dehydrates this triangulation into an alphabetical string.

A _dehydration string_ is a compact text representation of a
triangulation, introduced by Callahan, Hildebrand and Weeks for their
cusped hyperbolic census (see below). The dehydration string of an
*n*-tetrahedron triangulation consists of approximately (but not
precisely) 5*n*/2 lower-case letters.

Dehydration strings come with some restrictions:

* They rely on the triangulation being "canonical" in some
  combinatorial sense. This is not enforced here; instead a
  combinatorial isomorphism is applied to make the triangulation
  canonical, and this isomorphic triangulation is dehydrated instead.
  Note that the original triangulation is not changed.

* They require the triangulation to be connected.

* They require the triangulation to have no boundary triangles (though
  ideal triangulations are fine).

* They can only support triangulations with at most 25 tetrahedra.

The routine rehydrate() can be used to recover a triangulation from a
dehydration string. Note that the triangulation recovered might not be
identical to the original, but it is guaranteed to be an isomorphic
copy.

For a full description of the dehydrated triangulation format, see _A
Census of Cusped Hyperbolic 3-Manifolds_, Callahan, Hildebrand and
Weeks, Mathematics of Computation 68/225, 1999.

Exception ``NotImplemented``:
    Either this triangulation is disconnected, it has boundary
    triangles, or it contains more than 25 tetrahedra.

Returns:
    a dehydrated representation of this triangulation (or an
    isomorphic variant of this triangulation).)doc";

// Docstring regina::python::doc::Triangulation_::eulerCharManifold
static const char *eulerCharManifold =
R"doc(Returns the Euler characteristic of the corresponding compact
3-manifold.

Instead of simply calculating ``V-E+F-T``, this routine also:

* treats ideal vertices as surface boundary components (i.e.,
  effectively truncates them);

* truncates invalid boundary vertices (i.e., boundary vertices whose
  links are not discs);

* truncates the projective plane cusps at the midpoints of invalid
  edges (edges identified with themselves in reverse).

For ideal triangulations, this routine therefore computes the proper
Euler characteristic of the manifold (unlike eulerCharTri(), which
does not).

For triangulations whose vertex links are all spheres or discs, this
routine and eulerCharTri() give identical results.

Returns:
    the Euler characteristic of the corresponding compact manifold.)doc";

// Docstring regina::python::doc::Triangulation_::fillTorus
static const char *fillTorus =
R"doc(Fills a two-triangle torus boundary component by attaching a solid
torus along a given curve.

The boundary component to be filled should be passed as the argument
*bc*; if the triangulation has exactly one boundary component then you
may omit *bc* (i.e., pass ``None``), and the (unique) boundary
component will be inferred.

If the boundary component cannot be inferred, and/or if the selected
boundary component is not a two-triangle torus, then this routine will
do nothing and return ``False``.

Otherwise the given boundary component will be filled with a solid
torus whose meridional curve cuts the edges ``bc->edge(0)``,
``bc->edge(1)`` and ``bc->edge(2)`` a total of *cuts0*, *cuts1* and
*cuts2* times respectively.

For the filling to be performed successfully, the integers *cuts0*,
*cuts1* and *cuts2* must be coprime, and two of them must add to give
the third. Otherwise, as above, this routine will do nothing and
return ``False``.

The triangulation will be simplified before returning.

There are two versions of fillTorus(); the other takes three explicit
edges instead of a boundary component. You should use the other
version if you know how the filling curve cuts each boundary edge but
you do not know how these edges are indexed in the boundary component.

Parameter ``cuts0``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge ``bc->edge(0)``.

Parameter ``cuts1``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge ``bc->edge(1)``.

Parameter ``cuts2``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge ``bc->edge(2)``.

Parameter ``bc``:
    the boundary component to fill. If the triangulation has precisely
    one boundary component then this may be ``None``.

Returns:
    ``True`` if the boundary component was filled successfully, or
    ``False`` if one of the required conditions as described above is
    not satisfied.)doc";

// Docstring regina::python::doc::Triangulation_::fillTorus_2
static const char *fillTorus_2 =
R"doc(Fills a two-triangle torus boundary component by attaching a solid
torus along a given curve.

The three edges of the boundary component should be passed as the
arguments *e0*, *e1* and *e2*. The boundary component will then be
filled with a solid torus whose meridional curve cuts these three
edges *cuts0*, *cuts1* and *cuts2* times respectively.

For the filling to be performed successfully, the three given edges
must belong to the same boundary component, and this boundary
component must be a two-triangle torus. Moreover, the integers
*cuts0*, *cuts1* and *cuts2* must be coprime, and two of them must add
to give the third. If any of these conditions are not met, then this
routine will do nothing and return ``False``.

The triangulation will be simplified before returning.

There are two versions of fillTorus(); the other takes a boundary
component, and sets *e0*, *e1* and *e2* to its three edges according
to Regina's own edge numbering. This version of fillTorus() should be
used when you know how the filling curve cuts each boundary edge but
you do not know how these edges are indexed in the corresponding
boundary component.

Parameter ``e0``:
    one of the three edges of the boundary component to fill.

Parameter ``e1``:
    the second of the three edges of the boundary component to fill.

Parameter ``e2``:
    the second of the three edges of the boundary component to fill.

Parameter ``cuts0``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge *e0*.

Parameter ``cuts1``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge *e1*.

Parameter ``cuts2``:
    the number of times that the meridional curve of the new solid
    torus should cut the edge *e2*.

Returns:
    ``True`` if the boundary component was filled successfully, or
    ``False`` if one of the required conditions as described above is
    not satisfied.)doc";

// Docstring regina::python::doc::Triangulation_::fourFourMove
static const char *fourFourMove =
R"doc(Checks the eligibility of and/or performs a 4-4 move about the given
edge. This involves replacing the four tetrahedra joined at that edge
with four tetrahedra joined along a different edge. Consider the
octahedron made up of the four original tetrahedra; this has three
internal axes. The initial four tetrahedra meet along the given edge
which forms one of these axes; the new tetrahedra will meet along a
different axis. This move can be done iff (i) the edge is valid and
non-boundary, and (ii) the four tetrahedra are distinct.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<3>::lock() and
Simplex<3>::lockFacet() for further details on locks).

If this triangulation is currently oriented, then this 4-4 move will
label the new tetrahedra in a way that preserves the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given edge is an edge of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how locks work and
    what their implications are.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``newAxis``:
    Specifies which axis of the octahedron the new tetrahedra should
    meet along; this should be 0 or 1. Consider the four original
    tetrahedra in the order described by Edge<3>::embedding(0,...,3);
    call these tetrahedra 0, 1, 2 and 3. If *newAxis* is 0, the new
    axis will separate tetrahedra 0 and 1 from 2 and 3. If *newAxis*
    is 1, the new axis will separate tetrahedra 1 and 2 from 3 and 0.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::fromSnapPea
static const char *fromSnapPea =
R"doc(Extracts the tetrahedron gluings from a string that contains the full
contents of a SnapPea data file. All other SnapPea-specific
information (such as peripheral curves, and the manifold name) will be
ignored, since Regina's Triangulation<3> class does not track such
information itself.

If you wish to preserve all SnapPea-specific information from the data
file, you should work with the SnapPeaTriangulation class instead
(which uses the SnapPea kernel directly, and can therefore store
anything that SnapPea can).

If you wish to read a triangulation from a SnapPea _file_, you should
likewise call the SnapPeaTriangulation constructor, giving the
filename as argument. This will read all SnapPea-specific information
(as described above), and also avoids constructing an enormous
intermediate string.

.. warning::
    This routine is "lossy", in that drops SnapPea-specific
    information (as described above). Unless you specifically need an
    Triangulation<3> (not an SnapPeaTriangulation) or you need to
    avoid calling routines from the SnapPea kernel, it is highly
    recommended that you create a SnapPeaTriangulation from the given
    file contents instead. See the string-based SnapPeaTriangulation
    constructor for how to do this.

Exception ``InvalidArgument``:
    The given SnapPea data was not in the correct format.

Parameter ``snapPeaData``:
    a string containing the full contents of a SnapPea data file.

Returns:
    a native Regina triangulation extracted from the given SnapPea
    data.)doc";

// Docstring regina::python::doc::Triangulation_::generalAngleStructure
static const char *generalAngleStructure =
R"doc(Returns a generalised angle structure on this triangulation, if one
exists. A _generalised_ angle structure must satisfy the same matching
equations as all angle structures do, but there is no constraint on
the signs of the angles; in particular, negative angles are allowed.
If a generalised angle structure does exist, then this routine is
guaranteed to return one.

This routine is designed for scenarios where you already know that a
generalised angle structure exists. This means:

* If no generalised angle structure exists, this routine will throw an
  exception, which will incur a significant overhead.

* It should be rare that you do not know in advance whether a
  generalised angle structure exists (see the simple conditions in the
  note below). However, if you don't yet know, you should call
  hasGeneralAngleStructure() first. If the answer is no, this will
  avoid the overhead of throwing and catching exceptions. If the
  answer is yes, this will have the side-effect of caching the angle
  structure, which means your subsequent call to
  generalAngleStructure() will be essentially instantaneous.

The underlying algorithm simply solves a system of linear equations,
and so should be fast even for large triangulations.

The result of this routine is cached internally: as long as the
triangulation does not change, multiple calls to
generalAngleStructure() will return identical angle structures, and
every call after the first be essentially instantaneous.

If the triangulation does change, however, then the cached angle
structure will be deleted, and any reference that was returned before
will become invalid.

.. note::
    For a valid triangulation with no boundary faces, a generalised
    angle structure exists if and only if every vertex link is a torus
    or Klein bottle. The "only if" direction is a simple Euler
    characteristic calculation; for the "if" direction see "Angle
    structures and normal surfaces", Feng Luo and Stephan Tillmann,
    Trans. Amer. Math. Soc. 360:6 (2008), pp. 2849-2866).

Exception ``NoSolution``:
    No generalised angle structure exists on this triangulation.

Returns:
    a generalised angle structure on this triangulation, if one
    exists.)doc";

// Docstring regina::python::doc::Triangulation_::hasBoundaryTriangles
static const char *hasBoundaryTriangles =
R"doc(A dimension-specific alias for hasBoundaryFacets().

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::hasCompressingDisc
static const char *hasCompressingDisc =
R"doc(Searches for a compressing disc within the underlying 3-manifold.

Let *M* be the underlying 3-manifold and let *B* be its boundary. By a
_compressing disc_, we mean a disc *D* properly embedded in *M*, where
the boundary of *D* lies in *B* but does not bound a disc in *B*.

This routine will first call the heuristic routine
hasSimpleCompressingDisc() in the hope of obtaining a fast answer. If
this fails, it will do one of two things:

* If the triangulation is orientable and 1-vertex, it will use the
  linear programming and crushing machinery outlined in "Computing
  closed essential surfaces in knot complements", Burton, Coward and
  Tillmann, SCG '13, p405-414, 2013. This is often extremely fast,
  even for triangulations with many tetrahedra.

* If the triangulation is non-orientable or has multiple vertices then
  it will run a full enumeration of vertex normal surfaces, as
  described in "Algorithms for the complete decomposition of a closed
  3-manifold", Jaco and Tollefson, Illinois J. Math. 39 (1995),
  358-406. As the number of tetrahedra grows, this can become
  extremely slow.

This routine will work on a copy of this triangulation, not the
original. In particular, the copy will be simplified, which means that
there is no harm in calling this routine on an unsimplified
triangulation.

If this triangulation has no boundary components, this routine will
simply return ``False``.

Precondition:
    This triangulation is valid and is not ideal.

Precondition:
    The underlying 3-manifold is irreducible.

.. warning::
    This routine can be infeasibly slow for large triangulations
    (particularly those that are non-orientable or have multiple
    vertices), since it may need to perform a full enumeration of
    vertex normal surfaces, and since it might perform "large"
    operations on these surfaces such as cutting along them. See
    hasSimpleCompressingDisc() for a "heuristic shortcut" that is
    faster but might not give a definitive answer.

Returns:
    ``True`` if the underlying 3-manifold contains a compressing disc,
    or ``False`` if it does not.)doc";

// Docstring regina::python::doc::Triangulation_::hasGeneralAngleStructure
static const char *hasGeneralAngleStructure =
R"doc(Determines whether this triangulation supports a generalised angle
structure. A _generalised_ angle structure must satisfy the same
matching equations as all angle structures do, but there is no
constraint on the signs of the angles; in particular, negative angles
are allowed.

This routine returns ``False`` if and only if generalAngleStructure()
throws an exception. However, if you do not _know_ whether a
generalised angle structure exists, then this routine is faster:

* If there is _no_ generalised angle structure, this routine will
  avoid the overhead of throwing and catching exceptions.

* If there _is_ a generalised angle structure, this routine will find
  and cache this angle structure, which means that any subsequent call
  to generalAngleStructure() to retrieve its details will be
  essentially instantaneous.

The underlying algorithm simply solves a system of linear equations,
and so should be fast even for large triangulations.

.. note::
    For a valid triangulation with no boundary faces, a generalised
    angle structure exists if and only if every vertex link is a torus
    or Klein bottle. The "only if" direction is a simple Euler
    characteristic calculation; for the "if" direction see "Angle
    structures and normal surfaces", Feng Luo and Stephan Tillmann,
    Trans. Amer. Math. Soc. 360:6 (2008), pp. 2849-2866).

.. note::
    Even if the condition above is true and it is clear that a
    generalised angle structure should exist, this routine will still
    do the extra work to compute an explicit solution (in order to
    fulfil the promise made in the generalAngleStructure()
    documentation).

Returns:
    ``True`` if and only if a generalised angle structure exists on
    this triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::hasMinimalBoundary
static const char *hasMinimalBoundary =
R"doc(Determines whether the boundary of this triangulation contains the
smallest possible number of triangles.

This is true if and only if, amongst all real boundary components,
every sphere or projective plane boundary component has precisely two
triangles, and every other boundary component has precisely one
vertex.

For the purposes of this routine, ideal boundary components are
ignored.

If this routine returns ``False``, you can call minimiseBoundary() to
make the number of boundary triangles minimal.

Precondition:
    This triangulation is valid.

Returns:
    ``True`` if and only if the boundary contains the smallest
    possible number of triangles.)doc";

// Docstring regina::python::doc::Triangulation_::hasMinimalVertices
static const char *hasMinimalVertices =
R"doc(Determines whether this triangulation contains the smallest possible
number of vertices for the 3-manifold that it represents.

This is true if and only if:

* amongst all real boundary components, every sphere or projective
  plane boundary component has precisely two triangles, and every
  other boundary component has precisely one vertex;

* amongst all connected components, every closed component has
  precisely one vertex, and every component with real or ideal
  boundary has no internal vertices at all.

If this routine returns ``False``, you can call minimiseVertices() to
make the number of vertices minimal.

Precondition:
    This triangulation is valid.

Returns:
    ``True`` if and only if this triangulation contains the smallest
    possible number of vertices.)doc";

// Docstring regina::python::doc::Triangulation_::hasNegativeIdealBoundaryComponents
static const char *hasNegativeIdealBoundaryComponents =
R"doc(Determines if this triangulation contains any ideal boundary
components with negative Euler characteristic.

Returns:
    ``True`` if and only if there is at least one such boundary
    component.)doc";

// Docstring regina::python::doc::Triangulation_::hasSimpleCompressingDisc
static const char *hasSimpleCompressingDisc =
R"doc(Searches for a "simple" compressing disc inside this triangulation.

Let *M* be the underlying 3-manifold and let *B* be its boundary. By a
_compressing disc_, we mean a disc *D* properly embedded in *M*, where
the boundary of *D* lies in *B* but does not bound a disc in *B*.

By a *simple* compressing disc, we mean a compressing disc that has a
very simple combinatorial structure (here "simple" is subject to
change; see the warning below). Examples include the compressing disc
inside a 1-tetrahedron solid torus, or a compressing disc formed from
a single internal triangle surrounded by three boundary edges.

The purpose of this routine is to avoid working with normal surfaces
within a large triangulation where possible. This routine is
relatively fast, and if it returns ``True`` then this 3-manifold
definitely contains a compressing disc. If this routine returns
``False`` then there might or might not be a compressing disc; the
user will need to perform a full normal surface enumeration using
hasCompressingDisc() to be sure.

This routine will work on a copy of this triangulation, not the
original. In particular, the copy will be simplified, which means that
there is no harm in calling this routine on an unsimplified
triangulation.

If this triangulation has no boundary components, this routine will
simply return ``False``.

For further information on this test, see "The Weber-Seifert
dodecahedral space is non-Haken", Benjamin A. Burton, J. Hyam
Rubinstein and Stephan Tillmann, Trans. Amer. Math. Soc. 364:2 (2012),
pp. 911-932.

.. warning::
    The definition of "simple" is subject to change in future releases
    of Regina. That is, this routine may be expanded over time to
    identify more types of compressing discs (thus making it more
    useful as a "heuristic shortcut").

Precondition:
    This triangulation is valid and is not ideal.

Returns:
    ``True`` if a simple compressing disc was found, or ``False`` if
    not. Note that even with a return value of ``False``, there might
    still be a compressing disc (just not one with a simple
    combinatorial structure).)doc";

// Docstring regina::python::doc::Triangulation_::hasSplittingSurface
static const char *hasSplittingSurface =
R"doc(Determines whether this triangulation has a normal splitting surface.
See NormalSurface::isSplitting() for details regarding normal
splitting surfaces.

In the special case where this is the empty triangulation, this
routine returns ``False``.

As of Regina 6.0, this routine is now fast (small polynomial time),
and works even for triangulations with more than one connected
component. Thanks to Robert Haraway.

Returns:
    ``True`` if and only if this triangulation has a normal splitting
    surface.)doc";

// Docstring regina::python::doc::Triangulation_::hasStrictAngleStructure
static const char *hasStrictAngleStructure =
R"doc(Determines whether this triangulation supports a strict angle
structure. Recall that a _strict_ angle structure is one in which
every angle is strictly between 0 and π.

This routine returns ``False`` if and only if strictAngleStructure()
throws an exception. However, if you do not _know_ whether a strict
angle structure exists, then this routine is faster:

* If there is _no_ strict angle structure, this routine will avoid the
  overhead of throwing and catching exceptions.

* If there _is_ a strict angle structure, this routine will find and
  cache this angle structure, which means that any subsequent call to
  strictAngleStructure() to retrieve its details will be essentially
  instantaneous.

The underlying algorithm runs a single linear program (it does _not_
enumerate all vertex angle structures). This means that it is likely
to be fast even for large triangulations.

Returns:
    ``True`` if and only if a strict angle structure exists on this
    triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::hasTwoSphereBoundaryComponents
static const char *hasTwoSphereBoundaryComponents =
R"doc(Determines if this triangulation contains any two-sphere boundary
components.

Returns:
    ``True`` if and only if there is at least one two-sphere boundary
    component.)doc";

// Docstring regina::python::doc::Triangulation_::homologyBdry
static const char *homologyBdry =
R"doc(Returns the first homology group of the boundary for this
triangulation. Note that ideal vertices are considered part of the
boundary.

Bear in mind that each time the triangulation changes, the homology
groups will be deleted. Thus the reference that is returned from this
routine should not be kept for later use. Instead, homologyBdry()
should be called again; this will be instantaneous if the group has
already been calculated.

This routine is fairly fast, since it deduces the homology of each
boundary component through knowing what kind of surface it is.

Precondition:
    This triangulation is valid.

Exception ``FailedPrecondition``:
    This triangulation is invalid.

Returns:
    the first homology group of the boundary.)doc";

// Docstring regina::python::doc::Triangulation_::homologyH2Z2
static const char *homologyH2Z2 =
R"doc(Returns the second homology group with coefficients in Z_2 for this
triangulation. If this triangulation contains any ideal vertices, the
homology group will be calculated as if each such vertex had been
truncated. The algorithm used calculates the relative first homology
group with respect to the boundary and uses homology and cohomology
theorems to deduce the second homology group.

This group will simply be the direct sum of several copies of Z_2, so
the number of Z_2 terms is returned.

Precondition:
    This triangulation is valid.

Exception ``FailedPrecondition``:
    This triangulation is invalid.

Returns:
    the number of Z_2 terms in the second homology group with
    coefficients in Z_2.)doc";

// Docstring regina::python::doc::Triangulation_::homologyRel
static const char *homologyRel =
R"doc(Returns the relative first homology group with respect to the boundary
for this triangulation. Note that ideal vertices are considered part
of the boundary.

Bear in mind that each time the triangulation changes, the homology
groups will be deleted. Thus the reference that is returned from this
routine should not be kept for later use. Instead, homologyRel()
should be called again; this will be instantaneous if the group has
already been calculated.

Precondition:
    This triangulation is valid.

Exception ``FailedPrecondition``:
    This triangulation is invalid.

Returns:
    the relative first homology group with respect to the boundary.)doc";

// Docstring regina::python::doc::Triangulation_::idealToFinite
static const char *idealToFinite =
R"doc(Converts an ideal triangulation into a finite triangulation. All ideal
or invalid vertices are truncated and thus converted into real
boundary components made from unglued faces of tetrahedra.

Note that this operation is a loose converse of finiteToIdeal().

.. warning::
    Currently, this routine subdivides all tetrahedra as if *all*
    vertices (not just some) were ideal. This may lead to more
    tetrahedra than are necessary.

.. warning::
    Currently, the presence of an invalid edge will force the
    triangulation to be subdivided even if there are no ideal
    vertices. The final triangulation will still have the projective
    plane cusp caused by the invalid edge.

Exception ``LockViolation``:
    This triangulation contains at least one locked top-dimensional
    simplex and/or facet. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how such locks work
    and what their implications are.

Returns:
    ``True`` if and only if the triangulation was changed.

Author:
    David Letscher)doc";

// Docstring regina::python::doc::Triangulation_::inAnyPacket
static const char *inAnyPacket =
R"doc(Returns the packet that holds this data, even if it is held indirectly
via a SnapPea triangulation.

This routine is similar to PacketOf<Triangulation<3>>::packet(). In
particular, if this triangulation is held directly by a 3-dimensional
triangulation packet *p*, then this routine will return *p*.

The difference is when this triangulation is held "indirectly" by a
SnapPea triangulation packet *q* (i.e., this is the inherited
Triangulation<3> data belonging to the SnapPea triangulation). In such
a scenario, Triangulation<3>::packet() will return ``None`` (since
there is no "direct" 3-dimensional triangulation packet), but
inAnyPacket() will return *q* (since the triangulation is still
"indirectly" held by a different type of packet).

Returns:
    the packet that holds this data (directly or indirectly), or
    ``None`` if this data is not held by either a 3-dimensional
    triangulation packet or a SnapPea triangulation packet.)doc";

// Docstring regina::python::doc::Triangulation_::inAnyPacket_2
static const char *inAnyPacket_2 =
R"doc(Returns the packet that holds this data, even if it is held indirectly
via a SnapPea triangulation.

This routine is similar to PacketOf<Triangulation<3>>::packet(). In
particular, if this triangulation is held directly by a 3-dimensional
triangulation packet *p*, then this routine will return *p*.

The difference is when this triangulation is held "indirectly" by a
SnapPea triangulation packet *q* (i.e., this is the inherited
Triangulation<3> data belonging to the SnapPea triangulation). In such
a scenario, Triangulation<3>::packet() will return ``None`` (since
there is no "direct" 3-dimensional triangulation packet), but
inAnyPacket() will return *q* (since the triangulation is still
"indirectly" held by a different type of packet).

Returns:
    the packet that holds this data (directly or indirectly), or
    ``None`` if this data is not held by either a 3-dimensional
    triangulation packet or a SnapPea triangulation packet.)doc";

// Docstring regina::python::doc::Triangulation_::insertLayeredSolidTorus
static const char *insertLayeredSolidTorus =
R"doc(Inserts a new layered solid torus into the triangulation. The
meridinal disc of the layered solid torus will intersect the three
edges of the boundary torus in *cuts0*, *cuts1* and (*cuts0* +
*cuts1*) points respectively.

The boundary torus will always consist of faces 012 and 013 of the
tetrahedron containing this boundary torus (this tetrahedron will be
returned). In face 012, edges 12, 02 and 01 will meet the meridinal
disc *cuts0*, *cuts1* and (*cuts0* + *cuts1*) times respectively. The
only exceptions are if these three intersection numbers are (1,1,2) or
(0,1,1), in which case edges 12, 02 and 01 will meet the meridinal
disc (1, 2 and 1) or (1, 1 and 0) times respectively.

The new tetrahedra will be inserted at the end of the list of
tetrahedra in the triangulation.

Precondition:
    0 ≤ *cuts0* ≤ *cuts1*;

Precondition:
    gcd(*cuts0*, *cuts1*) = 1.

Exception ``InvalidArgument``:
    The preconditions above do not hold; that is, either *cuts0* >
    *cuts1*, and/or *cuts0* and *cuts1* are not coprime.

Parameter ``cuts0``:
    the smallest of the three desired intersection numbers.

Parameter ``cuts1``:
    the second smallest of the three desired intersection numbers.

Returns:
    the tetrahedron containing the boundary torus.

See also:
    LayeredSolidTorus)doc";

// Docstring regina::python::doc::Triangulation_::intelligentSimplify
static const char *intelligentSimplify =
R"doc(Attempts to simplify the triangulation using fast and greedy
heuristics. This routine will attempt to reduce the number of
tetrahedra, the number of vertices and the number of boundary
triangles (with the number of tetrahedra as its priority).

Currently this routine uses simplifyToLocalMinimum() and
minimiseVertices() in combination with random 4-4 moves, book opening
moves and book closing moves.

Although intelligentSimplify() works very well most of the time, it
can occasionally get stuck; in such cases you may wish to try the more
powerful but (much) slower simplifyExhaustive() instead.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

.. warning::
    Running this routine multiple times upon the same triangulation
    may return different results, since the implementation makes
    random decisions. More broadly, the implementation of this routine
    (and therefore its results) may change between different releases
    of Regina.

Returns:
    ``True`` if and only if the triangulation was successfully
    simplified. Otherwise this triangulation will not be changed.)doc";

// Docstring regina::python::doc::Triangulation_::isBall
static const char *isBall =
R"doc(Determines whether this is a triangulation of a 3-dimensional ball.

This routine is based on isSphere(), which in turn combines
Rubinstein's 3-sphere recognition algorithm with Jaco and Rubinstein's
0-efficiency prime decomposition algorithm.

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and so can be very slow for larger triangulations (although faster
    tests are used where possible). The routine knowsBall() can be
    called to see if this property is already known or if it happens
    to be very fast to calculate for this triangulation.

Returns:
    ``True`` if and only if this is a triangulation of a 3-dimensional
    ball.)doc";

// Docstring regina::python::doc::Triangulation_::isClosed
static const char *isClosed =
R"doc(Determines if this triangulation is closed. This is the case if and
only if it has no boundary. Note that ideal triangulations are not
closed.

Returns:
    ``True`` if and only if this triangulation is closed.)doc";

// Docstring regina::python::doc::Triangulation_::isHaken
static const char *isHaken =
R"doc(Determines whether the underlying 3-manifold (which must be closed and
orientable) is Haken. In other words, this routine determines whether
the underlying 3-manifold contains an embedded closed two-sided
incompressible surface.

Currently Hakenness testing is available only for irreducible
manifolds. This routine will first test whether the manifold is
irreducible and, if it is not, will return ``False`` immediately.

Precondition:
    This triangulation is valid, closed, orientable and connected.

.. warning::
    This routine could be very slow for larger triangulations.

Returns:
    ``True`` if and only if the underlying 3-manifold is irreducible
    and Haken.)doc";

// Docstring regina::python::doc::Triangulation_::isIdeal
static const char *isIdeal =
R"doc(Determines if this triangulation is ideal. This is the case if and
only if one of the vertex links is closed and not a 2-sphere. Note
that the triangulation is not required to be valid.

Returns:
    ``True`` if and only if this triangulation is ideal.)doc";

// Docstring regina::python::doc::Triangulation_::isIrreducible
static const char *isIrreducible =
R"doc(Determines whether the underlying 3-manifold (which must be closed) is
irreducible. In other words, this routine determines whether every
embedded sphere in the underlying 3-manifold bounds a ball.

If the underlying 3-manifold is orientable, this routine will use fast
crushing and branch-and-bound methods. If the underlying 3-manifold is
non-orientable, it will use a (much slower) full enumeration of vertex
normal surfaces.

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and might be slow for larger triangulations.

Precondition:
    This triangulation is valid, closed, orientable and connected.

Returns:
    ``True`` if and only if the underlying 3-manifold is irreducible.)doc";

// Docstring regina::python::doc::Triangulation_::isOrdered
static const char *isOrdered =
R"doc(Determines if this triangulation is ordered; that is, if tetrahedron
vertices are labelled so that all gluing permutations are order-
preserving on the tetrahedron faces. Equivalently, this tests whether
the edges of the triangulation can all be oriented such that they
induce a consistent ordering on the vertices of each tetrahedron.

Triangulations are not ordered by default, and indeed some cannot be
ordered at all. The routine order() will attempt to relabel
tetrahedron vertices to give an ordered triangulation.

Returns:
    ``True`` if and only if all gluing permutations are order
    preserving on the tetrahedron faces.

Author:
    Matthias Goerner)doc";

// Docstring regina::python::doc::Triangulation_::isSnapPea
static const char *isSnapPea =
R"doc(Returns the SnapPea triangulation that holds this data, if there is
one.

This routine essentially replaces a dynamic_cast, since the class
Triangulation<3> is not polymorphic.

If this object in fact belongs to a SnapPeaTriangulation *t* (through
its inherited Triangulation<3> interface), then this routine will
return *t*. Otherwise it will return ``None``.

Returns:
    the SnapPea triangulation that holds this data, or ``None`` if
    this data is not part of a SnapPea triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::isSnapPea_2
static const char *isSnapPea_2 =
R"doc(Returns the SnapPea triangulation that holds this data, if there is
one.

This routine essentially replaces a dynamic_cast, since the class
Triangulation<3> is not polymorphic.

If this object in fact belongs to a SnapPeaTriangulation *t* (through
its inherited Triangulation<3> interface), then this routine will
return *t*. Otherwise it will return ``None``.

Returns:
    the SnapPea triangulation that holds this data, or ``None`` if
    this data is not part of a SnapPea triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::isSolidTorus
static const char *isSolidTorus =
R"doc(Determines whether this is a triangulation of the solid torus; that
is, the unknot complement. This routine can be used on a triangulation
with real boundary triangles, or on an ideal triangulation (in which
case all ideal vertices will be assumed to be truncated).

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and so might be very slow for larger triangulations (although
    faster tests are used where possible). The routine
    knowsSolidTorus() can be called to see if this property is already
    known or if it happens to be very fast to calculate for this
    triangulation.

Returns:
    ``True`` if and only if this is either a real (compact) or ideal
    (non-compact) triangulation of the solid torus.)doc";

// Docstring regina::python::doc::Triangulation_::isSphere
static const char *isSphere =
R"doc(Determines whether this is a triangulation of a 3-sphere.

This routine relies upon a combination of Rubinstein's 3-sphere
recognition algorithm and Jaco and Rubinstein's 0-efficiency prime
decomposition algorithm.

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and so can be very slow for larger triangulations (although faster
    tests are used where possible). The routine knowsSphere() can be
    called to see if this property is already known or if it happens
    to be very fast to calculate for this triangulation.

Returns:
    ``True`` if and only if this is a 3-sphere triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::isStandard
static const char *isStandard =
R"doc(Determines if this triangulation is standard. This is the case if and
only if every vertex is standard. See Vertex<3>::isStandard() for
further details.

Returns:
    ``True`` if and only if this triangulation is standard.)doc";

// Docstring regina::python::doc::Triangulation_::isTxI
static const char *isTxI =
R"doc(Determines whether or not the underlying 3-manifold is the product of
a torus with an interval.

This routine can be used on a triangulation with real boundary
triangles, or ideal boundary components, or a mix of both. If there
are any ideal vertices, they will be treated as though they were
truncated.

The underlying algorithm is due to Robert C. Haraway, III; see
https://journals.carleton.ca/jocg/index.php/jocg/article/view/433 for
details.

.. warning::
    This algorithm ultimately relies on isSolidTorus(), which might
    run slowly for large triangulations.

Returns:
    ``True`` if and only if this is a triangulation (either real,
    ideal or a combination) of the product of the torus with an
    interval.)doc";

// Docstring regina::python::doc::Triangulation_::isZeroEfficient
static const char *isZeroEfficient =
R"doc(Determines if this triangulation is 0-efficient. A triangulation is
0-efficient if its only normal spheres and discs are vertex linking,
and if it has no 2-sphere boundary components.

Returns:
    ``True`` if and only if this triangulation is 0-efficient.)doc";

// Docstring regina::python::doc::Triangulation_::knowsBall
static const char *knowsBall =
R"doc(Is it already known (or trivial to determine) whether or not this is a
triangulation of a 3-dimensional ball? See isBall() for further
details.

If this property is indeed already known, future calls to isBall()
will be very fast (simply returning the precalculated value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
isBall() and this routine will return ``True``.

Otherwise a call to isBall() may potentially require more significant
work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation forms a ball; it merely tells you whether the answer
    has already been computed (or is very easily computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsCompressingDisc
static const char *knowsCompressingDisc =
R"doc(Is it already known (or trivial to determine) whether or not the
underlying 3-manifold contains a compressing disc? See
hasCompressingDisc() for further details.

If this property is indeed already known, future calls to
hasCompressingDisc() will be very fast (simply returning the
precalculated value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
hasCompressingDisc() and this routine will return ``True``.

Otherwise a call to hasCompressingDisc() may potentially require more
significant work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ the underlying
    3-manifold has a compressing disc; it merely tells you whether the
    answer has already been computed (or is very easily computed).

Precondition:
    This triangulation is valid and is not ideal.

Precondition:
    The underlying 3-manifold is irreducible.

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsHaken
static const char *knowsHaken =
R"doc(Is it already known (or trivial to determine) whether or not the
underlying 3-manifold is Haken? See isHaken() for further details.

If this property is indeed already known, future calls to isHaken()
will be very fast (simply returning the precalculated value).

.. warning::
    This routine does not actually tell you _whether_ the underlying
    3-manifold is Haken; it merely tells you whether the answer has
    already been computed (or is very easily computed).

Precondition:
    This triangulation is valid, closed, orientable and connected.

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsHandlebody
static const char *knowsHandlebody =
R"doc(Is it already known (or trivial to determine) whether or not this is a
triangulation of an orientable handlebody? See recogniseHandlebody()
for further details.

If this property is indeed already known, future calls to
recogniseHandlebody() will be very fast (simply returning the
precalculated value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
recogniseHandlebody() and this routine will return ``True``.

Otherwise a call to recogniseHandlebody() may potentially require more
significant work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation forms an orientable handlebody; it merely tells you
    whether the answer has already been computed (or is very easily
    computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::knowsIrreducible
static const char *knowsIrreducible =
R"doc(Is it already known (or trivial to determine) whether or not the
underlying 3-manifold is irreducible? See isIrreducible() for further
details.

If this property is indeed already known, future calls to
isIrreducible() will be very fast (simply returning the precalculated
value).

.. warning::
    This routine does not actually tell you _whether_ the underlying
    3-manifold is irreducible; it merely tells you whether the answer
    has already been computed (or is very easily computed).

Precondition:
    This triangulation is valid, closed, orientable and connected.

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsSolidTorus
static const char *knowsSolidTorus =
R"doc(Is it already known (or trivial to determine) whether or not this is a
triangulation of a solid torus (that is, the unknot complement)? See
isSolidTorus() for further details.

If this property is indeed already known, future calls to
isSolidTorus() will be very fast (simply returning the precalculated
value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
isSolidTorus() and this routine will return ``True``.

Otherwise a call to isSolidTorus() may potentially require more
significant work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation forms a solid torus; it merely tells you whether the
    answer has already been computed (or is very easily computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsSphere
static const char *knowsSphere =
R"doc(Is it already known (or trivial to determine) whether or not this is a
triangulation of a 3-sphere? See isSphere() for further details.

If this property is indeed already known, future calls to isSphere()
will be very fast (simply returning the precalculated value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
isSphere() and this routine will return ``True``.

Otherwise a call to isSphere() may potentially require more
significant work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation forms a 3-sphere; it merely tells you whether the
    answer has already been computed (or is very easily computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsStrictAngleStructure
static const char *knowsStrictAngleStructure =
R"doc(Is it already known (or trivial to determine) whether or not this
triangulation supports a strict angle structure? See
hasStrictAngleStructure() for further details.

If this property is indeed already known, future calls to
strictAngleStructure() and hasStrictAngleStructure() will be very fast
(simply returning the precalculated solution).

.. warning::
    This routine does not actually tell you _whether_ the
    triangulation supports a strict angle structure; it merely tells
    you whether the answer has already been computed (or is very
    easily computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsTxI
static const char *knowsTxI =
R"doc(Is it already known (or trivial to determine) whether or not this is a
triangulation of a the product of a torus with an interval? See
isTxI() for further details.

If this property is indeed already known, future calls to isTxI() will
be very fast (simply returning the precalculated value).

If this property is not already known, this routine will nevertheless
run some very fast preliminary tests to see if the answer is obviously
no. If so, it will store ``False`` as the precalculated value for
isTxI() and this routine will return ``True``.

Otherwise a call to isTxI() may potentially require more significant
work, and so this routine will return ``False``.

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation forms the product of the torus with an interval; it
    merely tells you whether the answer has already been computed (or
    is very easily computed).

Returns:
    ``True`` if and only if this property is already known or trivial
    to calculate.)doc";

// Docstring regina::python::doc::Triangulation_::knowsZeroEfficient
static const char *knowsZeroEfficient =
R"doc(Is it already known whether or not this triangulation is 0-efficient?
See isZeroEfficient() for further details.

If this property is already known, future calls to isZeroEfficient()
will be very fast (simply returning the precalculated value).

.. warning::
    This routine does not actually tell you _whether_ this
    triangulation is 0-efficient; it merely tells you whether the
    answer has already been computed.

Returns:
    ``True`` if and only if this property is already known.)doc";

// Docstring regina::python::doc::Triangulation_::layerOn
static const char *layerOn =
R"doc(Performs a layering upon the given boundary edge of the triangulation.
See the Layering class notes for further details on what a layering
entails.

The new tetrahedron will be returned, and the new boundary edge that
it creates will be edge 5 (i.e., the edge joining vertices 2 and 3) of
this tetrahedron.

Precondition:
    The given edge is a boundary edge of this triangulation, and the
    two boundary triangles on either side of it are distinct.

Exception ``InvalidArgument``:
    The preconditions above do not hold; that is, either the given
    edge is non-boundary, or the same boundary triangles lie on both
    sides of it.

Exception ``LockViolation``:
    At least one of the two boundary triangles on either side of the
    given edge is currently locked. This exception will be thrown
    before any changes are made. See Simplex<3>::lockFacet() for
    further details on how such locks work and what their implications
    are.

Parameter ``edge``:
    the boundary edge upon which to layer.

Returns:
    the new tetrahedron provided by the layering.)doc";

// Docstring regina::python::doc::Triangulation_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of the given face as a normal surface.

Constructing the link of a face begins with building the frontier of a
regular neighbourhood of the face. If this is already a normal
surface, then then link is called _thin_. Otherwise the usual
normalisation steps are performed until the surface becomes normal;
note that these normalisation steps could change the topology of the
surface, and in some pathological cases could even reduce it to the
empty surface.

Template parameter ``subdim``:
    the dimension of the face to link; this must be between 0 and 2
    inclusive.

Precondition:
    The given face is a face of this triangulation.

Returns:
    a pair (*s*, *thin*), where *s* is the face linking normal
    surface, and *thin* is ``True`` if and only if this link is thin
    (i.e., no additional normalisation steps were required).)doc";

// Docstring regina::python::doc::Triangulation_::longitude
static const char *longitude =
R"doc(Modifies a triangulated knot complement so that the algebraic
longitude follows a single boundary edge, and returns this edge.

Assuming that this triangulation represents the complement of a knot
in the 3-sphere, this routine:

* identifies the algebraic longitude of the knot complement; that is,
  identifies the non-trivial simple closed curve on the boundary whose
  homology in the 3-manifold is trivial;

* layers additional tetrahedra on the boundary if necessary so that
  this curve is represented by a single boundary edge;

* returns that (possibly new) boundary edge.

Whilst this routine returns less information than meridianLongitude(),
it (1) runs much faster since it is based on fast algebraic
calculations, and (2) guarantees to terminate. In contrast,
meridianLongitude() must repeatedly try to test for 3-spheres, and (as
a result of only using fast 3-sphere recognition heuristics) does not
guarantee to terminate.

At present this routine is fairly restrictive in what triangulations
it can work with: it requires the triangulation to be one-vertex and
have real (not ideal) boundary. These restrictions may be eased in
future versions of Regina.

If the algebraic longitude is already represented by a single boundary
edge, then it is guaranteed that this routine will _not_ modify the
triangulation, and will simply return this boundary edge.

Precondition:
    The underlying 3-manifold is known to be the complement of a knot
    in the 3-sphere.

Precondition:
    This triangulation has precisely one vertex, and its (unique)
    boundary component is formed from two triangles.

.. warning::
    This routine may modify the triangluation, as explained above,
    which will have the side-effect of invalidating any existing
    Vertex, Edge or Triangle references.

.. warning::
    If you have an _ideal_ triangulation of a knot complement, you
    _must_ first run idealToFinite() and then simplify the resulting
    triangulation to have two boundary triangles.

Exception ``FailedPrecondition``:
    This triangulation is not a valid one-vertex orientable
    triangulation with homology *Z*, and with a two-triangle torus as
    its one and only boundary component. Note that this does not
    capture all of the preconditions for this routine, but it does
    capture those that are easy to test.

Exception ``UnsolvedCase``:
    An integer overflow occurred during the computation.

Exception ``LockViolation``:
    At least one of the two boundary triangles is currently locked.
    This exception will be thrown before any changes are made. See
    Simplex<3>::lockFacet() for further details on how such locks work
    and what their implications are.

Returns:
    the boundary edge representing the algebraic longitude of the knot
    (after this triangulation has been modified if necessary).)doc";

// Docstring regina::python::doc::Triangulation_::longitudeCuts
static const char *longitudeCuts =
R"doc(Identifies the algebraic longitude as a curve on the boundary of a
triangulated knot complement.

Specifically, assuming that this triangulation represents the
complement of a knot in the 3-sphere, this routine identifies the non-
trivial simple closed curve on the boundary whose homology in the
3-manifold is trivial.

The curve will be returned as a triple of integers, indicating how
many times the longitude intersects each of the three boundary edges.
It is always true that the largest of these three integers will be the
sum of the other two.

At present this routine is fairly restrictive in what triangulations
it can work with: it requires the triangulation to be one-vertex and
have real (not ideal) boundary. These restrictions may be eased in
future versions of Regina.

Precondition:
    The underlying 3-manifold is known to be the complement of a knot
    in the 3-sphere.

Precondition:
    This triangulation has precisely one vertex, and its (unique)
    boundary component is formed from two triangles.

.. warning::
    If you have an _ideal_ triangulation of a knot complement, you
    _must_ first run idealToFinite() and then simplify the resulting
    triangulation to have two boundary triangles.

Exception ``FailedPrecondition``:
    This triangulation is not a valid one-vertex orientable
    triangulation with homology *Z*, and with a two-triangle torus as
    its one and only boundary component. Note that this does not
    capture all of the preconditions for this routine, but it does
    capture those that are easy to test.

Exception ``UnsolvedCase``:
    An integer overflow occurred during the computation.

Returns:
    a triple of non-negative integers indicating how many times the
    longitude intersects each of the three boundary edges.
    Specifically, if the returned tuple is *t* and the unique boundary
    component is *bc*, then for each *k* = 0,1,2, the element ``t[k]``
    indicates the (absolute) number of times that the longitude
    intersects the edge ``bc->edge(k)``.)doc";

// Docstring regina::python::doc::Triangulation_::maximalForestInBoundary
static const char *maximalForestInBoundary =
R"doc(Produces a maximal forest in the 1-skeleton of the triangulation
boundary.

Note that the edge pointers returned will become invalid once the
triangulation has changed.

Returns:
    a set containing the edges of the maximal forest.)doc";

// Docstring regina::python::doc::Triangulation_::maximalForestInSkeleton
static const char *maximalForestInSkeleton =
R"doc(Produces a maximal forest in the triangulation's 1-skeleton. It can be
specified whether or not different boundary components may be joined
by the maximal forest.

An edge leading to an ideal vertex is still a candidate for inclusion
in the maximal forest. For the purposes of this algorithm, any ideal
vertex will be treated as any other vertex (and will still be
considered part of its own boundary component).

Note that the edge pointers returned will become invalid once the
triangulation has changed.

Parameter ``canJoinBoundaries``:
    ``True`` if and only if different boundary components are allowed
    to be joined by the maximal forest.

Returns:
    a set containing the edges of the maximal forest.)doc";

// Docstring regina::python::doc::Triangulation_::meridian
static const char *meridian =
R"doc(Modifies a triangulated knot complement so that the meridian follows a
single boundary edge, and returns this edge.

Assuming that this triangulation represents the complement of a knot
in the 3-sphere, this routine:

* identifies the meridian of the knot complement;

* layers additional tetrahedra on the boundary if necessary so that
  this curve is represented by a single boundary edge;

* returns that (possibly new) boundary edge.

This routine uses fast heuristics to locate the meridian; as a result,
**it does not guarantee to terminate** (but if you find a case where
it does not, please let the Regina developers know!). If it does
return then it guarantees that the result is correct.

This routine uses a similar algorithm to meridianLongitude(), with the
same problem that it could be slow and might not terminate. However,
meridian() has the advantage that it might produce a smaller
triangulation, since there is no need to arrange for the longitude to
be a boundary edge also.

At present this routine is fairly restrictive in what triangulations
it can work with: it requires the triangulation to be one-vertex and
have real (not ideal) boundary. These restrictions may be eased in
future versions of Regina.

If the meridian is already represented by a single boundary edge, then
it is guaranteed that, if this routine does terminate, it will _not_
modify the triangulation, and will simply return this boundary edge.

Precondition:
    The underlying 3-manifold is known to be the complement of a knot
    in the 3-sphere.

Precondition:
    This triangulation has precisely one vertex, and its (unique)
    boundary component is formed from two triangles.

.. warning::
    This routine may modify the triangluation, as explained above,
    which will have the side-effect of invalidating any existing
    Vertex, Edge or Triangle references.

.. warning::
    If you have an _ideal_ triangulation of a knot complement, you
    _must_ first run idealToFinite() and then simplify the resulting
    triangulation to have two boundary triangles.

Exception ``FailedPrecondition``:
    This triangulation is not a valid one-vertex orientable
    triangulation with homology *Z*, and with a two-triangle torus as
    its one and only boundary component. Note that this does not
    capture all of the preconditions for this routine, but it does
    capture those that are easy to test.

Exception ``UnsolvedCase``:
    An integer overflow occurred during the computation.

Exception ``LockViolation``:
    At least one of the two boundary triangles is currently locked.
    This exception will be thrown before any changes are made. See
    Simplex<3>::lockFacet() for further details on how such locks work
    and what their implications are.

Returns:
    the boundary edge representing the meridian (after this
    triangulation has been modified if necessary).)doc";

// Docstring regina::python::doc::Triangulation_::meridianLongitude
static const char *meridianLongitude =
R"doc(Modifies a triangulated knot complement so that the meridian and
algebraic longitude each follow a single boundary edge, and returns
these two edges.

Assuming that this triangulation represents the complement of a knot
in the 3-sphere, this routine:

* identifies the meridian of the knot complement, and also the
  algebraic longitude (i.e., the non-trivial simple closed curve on
  the boundary whose homology in the 3-manifold is trivial);

* layers additional tetrahedra on the boundary if necessary so that
  each of these curves is represented by a single boundary edge;

* returns these two (possibly new) boundary edges.

This routine uses fast heuristics to locate the meridian; as a result,
**it does not guarantee to terminate** (but if you find a case where
it does not, please let the Regina developers know!). If it does
return then it guarantees that the result is correct.

Whilst this routine returns more information than longitude(), note
that longitude() (1) runs much faster since it is based on fast
algebraic calculations, and (2) guarantees to terminate.

At present this routine is fairly restrictive in what triangulations
it can work with: it requires the triangulation to be one-vertex and
have real (not ideal) boundary. These restrictions may be eased in
future versions of Regina.

If the meridian and algebraic longitude are already both represented
by single boundary edges, then it is guaranteed that, if this routine
does terminate, it will _not_ modify the triangulation, and will
simply return these two boundary edges.

Precondition:
    The underlying 3-manifold is known to be the complement of a knot
    in the 3-sphere.

Precondition:
    This triangulation has precisely one vertex, and its (unique)
    boundary component is formed from two triangles.

.. warning::
    This routine may modify the triangluation, as explained above,
    which will have the side-effect of invalidating any existing
    Vertex, Edge or Triangle references.

.. warning::
    If you have an _ideal_ triangulation of a knot complement, you
    _must_ first run idealToFinite() and then simplify the resulting
    triangulation to have two boundary triangles.

Exception ``FailedPrecondition``:
    This triangulation is not a valid one-vertex orientable
    triangulation with homology *Z*, and with a two-triangle torus as
    its one and only boundary component. Note that this does not
    capture all of the preconditions for this routine, but it does
    capture those that are easy to test.

Exception ``UnsolvedCase``:
    An integer overflow occurred during the computation.

Exception ``LockViolation``:
    At least one of the two boundary triangles is currently locked.
    This exception will be thrown before any changes are made. See
    Simplex<3>::lockFacet() for further details on how such locks work
    and what their implications are.

Returns:
    a pair (*m*, *l*), where *m* is the boundary edge representing the
    meridian and *l* is the boundary edge representing the algebraic
    longitude of the knot complement (after this triangulation has
    been modified if necessary).)doc";

// Docstring regina::python::doc::Triangulation_::minimiseBoundary
static const char *minimiseBoundary =
R"doc(Ensures that the boundary contains the smallest possible number of
triangles, potentially adding tetrahedra to do this.

This routine is for use with algorithms that require minimal
boundaries (e.g., torus boundaries must contain exactly two
triangles). As noted above, it may in fact increase the total number
of tetrahedra in the triangulation (though the implementation does
make efforts not to do this).

Once this routine is finished, every boundary component will have
exactly one vertex, except for sphere and projective plane boundaries
which will have exactly two triangles (but three and two vertices
respectively).

The changes that this routine performs can always be expressed using
only close book moves and/or layerings. In particular, this routine
never creates new vertices, and it never creates a non-vertex-linking
normal disc or sphere if there was not one before.

Although this routine only modifies real boundary components, it is
fine if the triangulation also contains ideal boundary components (and
these simply will be left alone). If the triangulation contains
internal vertices, these will likewise be left untouched. If you wish
to remove internal vertices also, then you should call
minimiseVertices() instead.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Precondition:
    This triangulation is valid.

Exception ``FailedPrecondition``:
    This triangulation is not valid.

Returns:
    ``True`` if the triangulation was changed, or ``False`` if every
    boundary component was already minimal to begin with.)doc";

// Docstring regina::python::doc::Triangulation_::minimiseVertices
static const char *minimiseVertices =
R"doc(Ensures that this triangulation contains the smallest possible number
of vertices for the 3-manifold that it represents, potentially adding
tetrahedra to do this.

This routine is for use with algorithms that require a minimal number
of vertices (e.g., one-vertex triangulations of closed manifolds, or
*k*-vertex triangulations of the complements of *k*-component links).
As noted above, this routine may in fact increase the total number of
tetrahedra in the triangulation (though the implementation does make
efforts not to do this).

Once this routine is finished:

* every real boundary component will have exactly one vertex, except
  for sphere and projective plane boundaries which will have three and
  two vertices respectively (i.e., the minimum possible);

* for each component of the triangulation that contains one or more
  boundary components (either real and/or ideal), there will be no
  internal vertices at all;

* for each component of the triangulation that has no boundary
  components (i.e., that represents a closed 3-manifold), there will
  be precisely one vertex.

The changes that this routine performs can always be expressed using
only close book moves, layerings, collapse edge moves, and/or pinch
edge moves. In particular, this routine never creates new vertices.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Precondition:
    This triangulation is valid.

Exception ``FailedPrecondition``:
    This triangulation is not valid.

Returns:
    ``True`` if the triangulation was changed, or ``False`` if the
    number of vertices was already minimal to begin with.)doc";

// Docstring regina::python::doc::Triangulation_::minimizeBoundary
static const char *minimizeBoundary =
R"doc(A deprecated synonym for minimiseBoundary(). This ensures that the
boundary contains the smallest possible number of triangles,
potentially adding tetrahedra to do this.

See minimiseBoundary() for further details.

.. deprecated::
    Regina uses British English throughout its API. This synonym was a
    special case where Regina used to offer both British and American
    alternatives, but this will be removed in a future release. See
    the page on spelling throughout Regina for further details.

Precondition:
    This triangulation is valid.

Returns:
    ``True`` if the triangulation was changed, or ``False`` if every
    boundary component was already minimal to begin with.)doc";

// Docstring regina::python::doc::Triangulation_::minimizeVertices
static const char *minimizeVertices =
R"doc(A deprecated synonym for minimiseVertices(). This ensures that the
triangulation contains the smallest possible number of vertices,
potentially adding tetrahedra to do this.

See minimiseVertices() for further details.

.. deprecated::
    Regina uses British English throughout its API. This synonym was a
    special case where Regina used to offer both British and American
    alternatives, but this will be removed in a future release. See
    the page on spelling throughout Regina for further details.

Precondition:
    This triangulation is valid.

Returns:
    ``True`` if the triangulation was changed, or ``False`` if the
    number of vertices was already minimal to begin with.)doc";

// Docstring regina::python::doc::Triangulation_::newTetrahedra
static const char *newTetrahedra =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTetrahedra_2
static const char *newTetrahedra_2 =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTetrahedron
static const char *newTetrahedron =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTetrahedron_2
static const char *newTetrahedron_2 =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::niceTreeDecomposition
static const char *niceTreeDecomposition =
R"doc(Returns a nice tree decomposition of the face pairing graph of this
triangulation. This can (for example) be used in implementing
algorithms that are fixed-parameter tractable in the treewidth of the
face pairing graph.

See TreeDecomposition for further details on tree decompositions, and
see TreeDecomposition::makeNice() for details on what it means to be a
_nice_ tree decomposition.

This routine is fast: it will use a greedy algorithm to find a tree
decomposition with (hopefully) small width, but with no guarantees
that the width of this tree decomposition is the smallest possible.

The tree decomposition will be cached, so that if this routine is
called a second time (and the underlying triangulation has not been
changed) then the same tree decomposition will be returned
immediately.

Returns:
    a nice tree decomposition of the face pairing graph of this
    triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::nonTrivialSphereOrDisc
static const char *nonTrivialSphereOrDisc =
R"doc(Searches for a non-vertex-linking normal sphere or disc within this
triangulation. If such a surface exists within this triangulation,
this routine is guaranteed to find one.

Returns:
    a non-vertex-linking normal sphere or disc, or ``None`` if none
    exists.)doc";

// Docstring regina::python::doc::Triangulation_::octagonalAlmostNormalSphere
static const char *octagonalAlmostNormalSphere =
R"doc(Searches for an octagonal almost normal 2-sphere within this
triangulation. If such a surface exists, this routine is guaranteed to
find one.

Precondition:
    This triangulation is valid, closed, orientable, connected, and
    0-efficient. These preconditions are almost certainly more
    restrictive than they need to be, but we stay safe for now.

Returns:
    an octagonal almost normal 2-sphere, or ``None`` if none exists.)doc";

// Docstring regina::python::doc::Triangulation_::openBook
static const char *openBook =
R"doc(Checks the eligibility of and/or performs a book opening move about
the given triangle. This involves taking a triangle meeting the
boundary along two edges, and ungluing it to create two new boundary
triangles (thus exposing the tetrahedra it initially joined). This
move is the inverse of the closeBook() move, and is used to open the
way for new shellBoundary() moves.

This move can be done if:

* the triangle meets the boundary in precisely two edges (and thus
  also joins two tetrahedra);

* the vertex between these two edges is a standard boundary vertex
  (its link is a disc);

* the remaining edge of the triangle (which is internal to the
  triangulation) is valid.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
facet locks (see Simplex<3>::lockFacet() for further details on
locks).

If this triangulation is currently oriented, then this operation will
(trivially) preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *f*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any facet locks.

Precondition:
    The given triangle is a triangle of this triangulation.

Exception ``LockViolation``:
    This move would violate a facet lock, and *check* was passed as
    ``False``. This exception will be thrown before any changes are
    made. See Simplex<3>::lockFacet() for details on how facet locks
    work and what their implications are.

Parameter ``t``:
    the triangle about which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::order
static const char *order =
R"doc(Relabels tetrahedron vertices in this triangulation to give an ordered
triangulation, if possible.

To be an ordered triangulation, all face gluings (when restricted to
the tetrahedron face) must be order preserving. In other words, it
must be possible to orient all edges of the triangulation in such a
fashion that they are consistent with the ordering of the vertices in
each tetrahedron.

If it is possible to order this triangulation, the vertices of each
tetrahedron will be relabelled accordingly and this routine will
return ``True``. Otherwise, this routine will return ``False`` and the
triangulation will not be changed.

If this triangulation has locks on any tetrahedra and/or their facets,
these will not prevent the ordering from taking place. Instead, any
locks will be transformed accordingly (i.e., the facets of each
tetrahedron will exchange their lock states according to how the
vertices of that tetrahedron have been relabelled).

.. warning::
    This routine may be slow, since it backtracks through all possible
    edge orientations until a consistent one has been found.

Parameter ``forceOriented``:
    ``True`` if the triangulation must be both ordered and _oriented_,
    in which case this routine will return ``False`` if the
    triangulation cannot be oriented and ordered at the same time. See
    orient() for further details.

Returns:
    ``True`` if the triangulation has been successfully ordered as
    described above, or ``False`` if not.

Author:
    Matthias Goerner)doc";

// Docstring regina::python::doc::Triangulation_::pinchEdge
static const char *pinchEdge =
R"doc(Pinches an internal edge to a point. Topologically, this collapses the
edge to a point with no further side-effects, and it increases the
number of tetrahedra by two.

This operation can be performed on _any_ internal edge, without
further constraints. Two particularly useful settings are:

* If the edge joins an internal vertex with some different vertex
  (which may be internal, boundary, ideal or invalid), then this move
  does not change the topology of the manifold at all, and it reduces
  the total number of vertices by one. In this sense, it acts as an
  alternative to collapseEdge(), and unlike collapseEdge() it can
  _always_ be performed.

* If the edge runs from an internal vertex back to itself, then this
  move effectively drills out the edge, leaving an ideal torus or
  Klein bottle boundary component.

We do not allow *e* to lie entirely on the triangulation boundary,
because the implementation actually collapses an internal curve
_parallel_ to *e*, not the edge *e* itself (and so if *e* is a
boundary edge then the topological effect would not be as intended).
We do allow *e* to be an internal edge with both endpoints on the
boundary, but note that in this case the resulting topological
operation would render the triangulation invalid.

If you are trying to reduce the number of vertices without changing
the topology, and if *e* is an edge connecting an internal vertex with
some different vertex, then either collapseEdge() or pinchEdge() may
be more appropriate for your situation (though you may find it easier
just to call minimiseVertices() instead).

* The advantage of collapseEdge() is that it decreases the number of
  tetrahedra, whereas pinchEdge() increases this number (but only by
  two).

* The disadvantages of collapseEdge() are that it cannot always be
  performed, and its validity tests are expensive; pinchEdge() on the
  other hand can always be used for edges *e* of the type described
  above.

This operation works by prying open a triangle *t* and inserting a
two-tetrahedron gadget *g* within the resulting triangular pillow. In
particular, this means that simplex and/or facet locks will never
prevent this operation from taking place: if the triangle *t* happens
to be locked, then this lock will simply move across to one of the two
triangles bounding the gadget *g*.

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    The given edge is an internal edge of this triangulation (that is,
    *e* does not lie entirely within the boundary).

Exception ``InvalidArgument``:
    The given edge lies entirely within the boundary of the
    triangulation.

Parameter ``e``:
    the edge to collapse.)doc";

// Docstring regina::python::doc::Triangulation_::puncture
static const char *puncture =
R"doc(Punctures this manifold by thickening the given triangle into a
triangular pillow and then removing a 3-ball from its interior. If no
triangle is specified (i.e., the triangle pointer is ``None``), then
the triangle used will be facet 0 of tetrahedron 0.

The puncture will not meet the boundary of the pillow, and so nothing
will go wrong if the given triangle is boundary or has ideal vertices.
A side-effect of this, however, is that the resulting triangulation
will contain additional vertices, and will almost certainly be far
from minimal. It is highly recommended that you run
intelligentSimplify() if you do not need to preserve the combinatorial
structure of the new triangulation.

If this triangulation was originally oriented, then it will also be
oriented after this routine has been called. See isOriented() for
further details on oriented triangulations.

The new sphere boundary will be formed from two triangles;
specifically, face 0 of the last and second-last tetrahedra of the
triangulation. These two triangles will be joined so that vertex 1 of
each tetrahedron coincides, and vertices 2,3 of one map to vertices
3,2 of the other.

Tetrahedron and/or facet locks will not prevent the puncture from
taking place. If the given triangle was locked, then this lock will be
moved to one of the two triangles surrounding the triangular pillow.
In particular, if the given triangle is boundary, then the lock will
be moved to the corresponding boundary triangle.

Precondition:
    This triangulation is non-empty, and if ``location`` is non-null
    then it is in fact a triangle belonging to this triangulation.

Exception ``InvalidArgument``:
    The given triangle is non-null but not a triangle of this
    triangulation, or the given triangle is null but this
    triangulation is empty.

Parameter ``location``:
    the triangle indicating where the puncture should be taken. This
    may be ``None`` (the default), in which case facet 0 of
    tetrahedron 0 will be used.)doc";

// Docstring regina::python::doc::Triangulation_::puncture_2
static const char *puncture_2 =
R"doc(Deprecated routine that punctures this manifold by removing a 3-ball
from the interior of the given tetrahedron.

.. deprecated::
    Since the operation in fact involves prying open a triangle,
    puncture() now takes a triangle instead of a tetrahedron to
    indicate the location for the operation. If *tet* is null, then
    this function is equivalent to calling ``puncture()``; otherwise
    it is equivalent to calling ``puncture(tet->triangle(0))``. See
    puncture(Triangle<3>*) for further details.

Precondition:
    This triangulation is non-empty, and if ``tet`` is non-null then
    it is in fact a tetrahedron of this triangulation.

Exception ``InvalidArgument``:
    The given tetrahedron is non-null but not a tetrahedron of this
    triangulation, or the given tetrahedron is null but this
    triangulation is empty.

Parameter ``tet``:
    the tetrahedron indicating where the puncture will be taken. This
    may be ``None``, in which case tetrahedron 0 will be used.)doc";

// Docstring regina::python::doc::Triangulation_::recogniseHandlebody
static const char *recogniseHandlebody =
R"doc(Determines whether this is a triangulation of an orientable
handlebody, and if so, which genus. Specifically, this routine returns
the genus if this is indeed a handlebody, and returns -1 otherwise.
This routine can be used on a triangulation with real boundary
triangles, or on an ideal triangulation (in which case all ideal
vertices will be assumed to be truncated).

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and so might be very slow for larger triangulations (although
    faster tests are used where possible). The routine
    knowsHandlebody() can be called to see if this property is already
    known or if it happens to be very fast to calculate for this
    triangulation.

Returns:
    the genus if this is a triangulation of an orientable handlebody,
    or -1 otherwise.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::recogniser
static const char *recogniser =
R"doc(Returns a string that expresses this triangulation in Matveev's
3-manifold recogniser format.

Recogniser exports are currently not available for triangulations that
are invalid or contain boundary triangles. If either of these
conditions is true then this routine will throw an exception.

Exception ``NotImplemented``:
    This triangulation is either invalid or has boundary triangles.

Returns:
    a string containing the 3-manifold recogniser data.)doc";

// Docstring regina::python::doc::Triangulation_::recognizer
static const char *recognizer =
R"doc(A synonym for recogniser(). This returns a string that expresses this
triangulation in Matveev's 3-manifold recogniser format.

Recogniser exports are currently not available for triangulations that
are invalid or contain boundary triangles. If either of these
conditions is true then this routine will throw an exception.

Exception ``NotImplemented``:
    This triangulation is either invalid or has boundary triangles.

Returns:
    a string containing the 3-manifold recogniser data.)doc";

// Docstring regina::python::doc::Triangulation_::rehydrate
static const char *rehydrate =
R"doc(Rehydrates the given alphabetical string into a 3-dimensional
triangulation.

For a full description of the dehydrated triangulation format, see _A
Census of Cusped Hyperbolic 3-Manifolds_, Callahan, Hildebrand and
Weeks, Mathematics of Computation 68/225, 1999.

The converse routine dehydrate() can be used to extract a dehydration
string from an existing triangulation. Dehydration followed by
rehydration might not produce a triangulation identical to the
original, but it is guaranteed to produce an isomorphic copy. See
dehydrate() for the reasons behind this.

Exception ``InvalidArgument``:
    The given string could not be rehydrated.

Parameter ``dehydration``:
    a dehydrated representation of the triangulation to construct.
    Case is irrelevant; all letters will be treated as if they were
    lower case.

Returns:
    the rehydrated triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::removeAllTetrahedra
static const char *removeAllTetrahedra =
R"doc(A dimension-specific alias for removeAllSimplices().

See removeAllSimplices() for further information.

Exception ``LockViolation``:
    This triangulation contains at least one locked tetrahedron and/or
    facet. This exception will be thrown before any changes are made.
    See Simplex<3>::lock() and Simplex<3>::lockFacet() for further
    details on how such locks work and what their implications are.)doc";

// Docstring regina::python::doc::Triangulation_::removeTetrahedron
static const char *removeTetrahedron =
R"doc(A dimension-specific alias for removeSimplex().

See removeSimplex() for further information.

Exception ``LockViolation``:
    The given tetrahedron and/or one of its facets is currently
    locked. This exception will be thrown before any changes are made.
    See Simplex<3>::lock() and Simplex<3>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``tet``:
    the tetrahedron to remove.)doc";

// Docstring regina::python::doc::Triangulation_::removeTetrahedronAt
static const char *removeTetrahedronAt =
R"doc(A dimension-specific alias for removeSimplexAt().

See removeSimplexAt() for further information.

Exception ``LockViolation``:
    The requested tetrahedron and/or one of its facets is currently
    locked. This exception will be thrown before any changes are made.
    See Simplex<3>::lock() and Simplex<3>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``index``:
    specifies which tetrahedron to remove; this must be between 0 and
    size()-1 inclusive.)doc";

// Docstring regina::python::doc::Triangulation_::reorderTetrahedraBFS
static const char *reorderTetrahedraBFS =
R"doc(Deprecated alias for reorderBFS(), which reorders the tetrahedra of
this triangulation using a breadth-first search.

.. deprecated::
    This routine has been renamed reorderBFS() (and is now available
    for triangulations in all dimension). See reorderBFS() for further
    details.

Parameter ``reverse``:
    ``True`` if the new tetrahedron numbers should be assigned in
    reverse order, as described above.)doc";

// Docstring regina::python::doc::Triangulation_::retriangulate
static const char *retriangulate =
R"doc(Explores all triangulations that can be reached from this via Pachner
moves, without exceeding a given number of additional tetrahedra.

Specifically, this routine will iterate through all triangulations
that can be reached from this triangulation via 2-3 and 3-2 Pachner
moves, without ever exceeding *height* additional tetrahedra beyond
the original number.

For every such triangulation (including this starting triangulation),
this routine will call *action* (which must be a function or some
other callable object).

* *action* must take the following initial argument(s). Either (a) the
  first argument must be a triangulation (the precise type is
  discussed below), representing the triangluation that has been
  found; or else (b) the first two arguments must be of types const
  std::string& followed by a triangulation, representing both the
  triangulation and _an_ isomorphism signature. The second form is
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
of additional tetrahedra. This means that the routine will _never
terminate_, unless *action* returns ``True`` for some triangulation
that is passed to it.

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
    Triangulation<3>&&) representing a signature and the
    triangulation, as described in option (b) above.

Parameter ``height``:
    the maximum number of _additional_ tetrahedra to allow beyond the
    number of tetrahedra originally present in the triangulation, or a
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

// Docstring regina::python::doc::Triangulation_::saveRecogniser
static const char *saveRecogniser =
R"doc(Writes this triangulation to the given file in Matveev's 3-manifold
recogniser format.

Recogniser exports are currently not available for triangulations that
are invalid or contain boundary triangles. If either of these
conditions is true then the file will not be written, and this routine
will return ``False``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. The _contents_ of
    the file will be written using UTF-8.

Parameter ``filename``:
    the name of the Recogniser file to which to write.

Returns:
    ``True`` if and only if the file was successfully written.)doc";

// Docstring regina::python::doc::Triangulation_::saveRecognizer
static const char *saveRecognizer =
R"doc(A synonym for saveRecogniser(). This writes this triangulation to the
given file in Matveev's 3-manifold recogniser format.

Precondition:
    This triangulation is not invalid, and does not contain any
    boundary triangles.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. The _contents_ of
    the file will be written using UTF-8.

Parameter ``filename``:
    the name of the Recogniser file to which to write.

Returns:
    ``True`` if and only if the file was successfully written.)doc";

// Docstring regina::python::doc::Triangulation_::saveSnapPea
static const char *saveSnapPea =
R"doc(Writes this triangulation to the given file using SnapPea's native
file format.

Regarding what gets stored in the SnapPea data file:

* Since this function is defined by Regina's own Triangulation<3>
  class, only the tetrahedron gluings will be included in the SnapPea
  data file. All other SnapPea-specific information (such as
  peripheral curves) will be marked as unknown, since Regina does not
  track such information itself, and of course Regina-specific
  information (such as the Turaev-Viro invariants) will not be
  included in the SnapPea file either.

* The subclass SnapPeaTriangulation implements its own version of this
  function, which writes all additional SnapPea-specific information
  to the file (in fact it uses the SnapPea kernel itself to produce
  the file contents). However, to access that function you must
  explicitly call SnapPeaTriangulation::saveSnapPea() (since
  Triangulation<3> is not a polymorphic class, and in particular this
  function is not virtual).

SnapPea cannot represent triangulations that are empty, invalid, or
contain boundary triangles. If any of these conditions is true then
the file will not be written and this routine will return ``False``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. The _contents_ of
    the file will be written using UTF-8.

Parameter ``filename``:
    the name of the SnapPea file to which to write.

Returns:
    ``True`` if and only if the file was successfully written.)doc";

// Docstring regina::python::doc::Triangulation_::shellBoundary
static const char *shellBoundary =
R"doc(Checks the eligibility of and/or performs a boundary shelling move on
the given tetrahedron. This involves simply popping off a tetrahedron
that touches the boundary. This can be done if:

* all edges of the tetrahedron are valid;

* precisely one, two or three faces of the tetrahedron lie in the
  boundary;

* if one face lies in the boundary, then the opposite vertex does not
  lie in the boundary, and no two of the remaining three edges are
  identified;

* if two faces lie in the boundary, then the remaining edge does not
  lie in the boundary, and the remaining two faces of the tetrahedron
  are not identified.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<3>::lock() and
Simplex<3>::lockFacet() for further details on locks).

If this triangulation is currently oriented, then this operation will
(trivially) preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given tetrahedron is a tetrahedron of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how locks work and
    what their implications are.

Parameter ``t``:
    the tetrahedron upon which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::simplifyExhaustive
static const char *simplifyExhaustive =
R"doc(Attempts to simplify this triangulation using a slow but exhaustive
search through the Pachner graph. This routine is more powerful but
much slower than intelligentSimplify().

Specifically, this routine will iterate through all triangulations
that can be reached from this triangulation via 2-3 and 3-2 Pachner
moves, without ever exceeding *height* additional tetrahedra beyond
the original number.

If at any stage it finds a triangulation with _fewer_ tetrahedra than
the original, then this routine will call intelligentSimplify() to
shrink the triangulation further if possible and will then return
``True``. If it cannot find a triangulation with fewer tetrahedra then
it will leave this triangulation unchanged and return ``False``.

This routine can be very slow and very memory-intensive: the number of
triangulations it visits may be superexponential in the number of
tetrahedra, and it records every triangulation that it visits (so as
to avoid revisiting the same triangulation again). It is highly
recommended that you begin with *height* = 1, and if this fails then
try increasing *height* one at a time until either you find a
simplification or the routine becomes too expensive to run.

If *height* is negative, then there will be _no_ bound on the number
of additional tetrahedra. This means that the routine will not
terminate until a simpler triangulation is found. If no simpler
diagram exists then the only way to terminate this function is to
cancel the operation via a progress tracker (read on for details).

If you want a _fast_ simplification routine, you should call
intelligentSimplify() instead. The benefit of simplifyExhaustive() is
that, for very stubborn triangulations where intelligentSimplify()
finds itself stuck at a local minimum, simplifyExhaustive() is able to
"climb out" of such wells.

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
triangulation was simplified or the search was exhausted).

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
    the maximum number of _additional_ tetrahedra to allow beyond the
    number of tetrahedra originally present in the triangulation, or a
    negative number if this should not be bounded.

Parameter ``threads``:
    the number of threads to use. If this is 1 or smaller then the
    routine will run single-threaded.

Parameter ``tracker``:
    a progress tracker through which progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    ``True`` if and only if the triangulation was successfully
    simplified to fewer tetrahedra.)doc";

// Docstring regina::python::doc::Triangulation_::simplifyToLocalMinimum
static const char *simplifyToLocalMinimum =
R"doc(Uses all known simplification moves to reduce the triangulation
monotonically to some local minimum number of tetrahedra.

End users will probably not want to call this routine. You should call
intelligentSimplify() if you want a fast (and usually effective) means
of simplifying a triangulation, or you should call
simplifyExhaustive() if you are still stuck and you want to try a
slower but more powerful method instead.

The moves used by this routine include 3-2, 2-0 (edge and vertex), 2-1
and boundary shelling moves.

Moves that do not reduce the number of tetrahedra (such as 4-4 moves
or book opening moves) are not used in this routine. Such moves do
however feature in intelligentSimplify().

If this triangulation is currently oriented, then this operation will
preserve the orientation.

.. warning::
    The implementation of this routine (and therefore its results) may
    change between different releases of Regina.

Parameter ``perform``:
    ``True`` if we are to perform the simplifications, or ``False`` if
    we are only to investigate whether simplifications are possible
    (defaults to ``True``).

Returns:
    if *perform* is ``True``, this routine returns ``True`` if and
    only if the triangulation was changed to reduce the number of
    tetrahedra; if *perform* is ``False``, this routine returns
    ``True`` if and only if it determines that it is capable of
    performing such a change.)doc";

// Docstring regina::python::doc::Triangulation_::snapPea
static const char *snapPea =
R"doc(Returns a string containing the full contents of a SnapPea data file
that describes this triangulation. In particular, this string can be
used in a Python session to pass the triangulation directly to SnapPy
(without writing to the filesystem).

Regarding what gets stored in the SnapPea data file:

* Since this function is defined by Regina's own Triangulation<3>
  class, only the tetrahedron gluings will be included in the SnapPea
  data file. All other SnapPea-specific information (such as
  peripheral curves) will be marked as unknown, since Regina does not
  track such information itself, and of course Regina-specific
  information (such as the Turaev-Viro invariants) will not be
  included in the SnapPea file either.

* The subclass SnapPeaTriangulation implements its own version of this
  function, which writes all additional SnapPea-specific information
  to the file (in fact it uses the SnapPea kernel itself to produce
  the file contents). However, to access that function you must
  explicitly call SnapPeaTriangulation::snapPea() (since
  Triangulation<3> is not a polymorphic class, and in particular this
  function is not virtual).

If you wish to export a triangulation to a SnapPea _file_, you should
call saveSnapPea() instead (which has better performance, and does not
require you to construct an enormous intermediate string).

SnapPea cannot represent triangulations that are empty, invalid, or
contain boundary triangles. If any of these conditions is true then
this routine will throw an exception.

Exception ``NotImplemented``:
    This triangulation is either empty, invalid, or has boundary
    triangles.

Returns:
    a string containing the contents of the corresponding SnapPea data
    file.)doc";

// Docstring regina::python::doc::Triangulation_::strictAngleStructure
static const char *strictAngleStructure =
R"doc(Returns a strict angle structure on this triangulation, if one exists.
Recall that a _strict_ angle structure is one in which every angle is
strictly between 0 and π. If a strict angle structure does exist, then
this routine is guaranteed to return one.

This routine is designed for scenarios where you already know that a
strict angle structure exists. This means:

* If no strict angle structure exists, this routine will throw an
  exception, which will incur a significant overhead.

* If you do _not_ know in advance whether a strict angle structure
  exists, you should call hasStrictAngleStructure() first. If the
  answer is no, this will avoid the overhead of throwing and catching
  exceptions. If the answer is yes, this will have the side-effect of
  caching the strict angle structure, which means your subsequent call
  to strictAngleStructure() will be essentially instantaneous.

The underlying algorithm runs a single linear program (it does _not_
enumerate all vertex angle structures). This means that it is likely
to be fast even for large triangulations.

The result of this routine is cached internally: as long as the
triangulation does not change, multiple calls to
strictAngleStructure() will return identical angle structures, and
every call after the first be essentially instantaneous.

If the triangulation does change, however, then the cached angle
structure will be deleted, and any reference that was returned before
will become invalid.

Exception ``NoSolution``:
    No strict angle structure exists on this triangulation.

Returns:
    a strict angle structure on this triangulation, if one exists.)doc";

// Docstring regina::python::doc::Triangulation_::summands
static const char *summands =
R"doc(Computes the connected sum decomposition of this triangulation.

The prime summands will be returned as a vector of triangulations;
this triangulation will not be modified.

As far as possible, the summands will be represented using 0-efficient
triangulations (i.e., triangulations that contain no non-vertex-
linking normal spheres). Specifically, for every summand, either:

* the triangulation of the summand that is produced will be
  0-efficient; or

* the summand is one of RP3, the product S2xS1, or the twisted product
  S2x~S1. In each of these cases there is no possible 0-efficient
  triangulation of the summand, and so the triangulation that is
  produced will just be minimal (i.e., two tetrahedra).

For non-orientable triangulations, this routine is only guaranteed to
succeed if the original manifold contains no embedded two-sided
projective planes. If the manifold _does_ contain embedded two-sided
projective planes, then this routine might still succeed but it might
fail; however, such a failure will always be detected, and in such a
case this routine will throw an exception (as detailed below).

Note that this routine is currently only available for closed
triangulations; see the list of preconditions for full details. If
this triangulation is a 3-sphere then this routine will return an
empty list.

This function is new to Regina 7.0, and it has some important changes
of behaviour from the old connectedSumDecomposition() from Regina
6.0.1 and earlier:

* This function does not insert the resulting components into the
  packet tree.

* If this routine fails because of an embedded two-sided projective
  plane, then it throws an exception instead of returning -1.

* This function does not assign labels to the new summands.

The underlying algorithm appears in "A new approach to crushing
3-manifold triangulations", Discrete and Computational Geometry 52:1
(2014), pp. 116-139. This algorithm is based on the Jaco-Rubinstein
0-efficiency algorithm, and works in both orientable and non-
orientable settings.

.. warning::
    Users are strongly advised to check for exceptions if embedded
    two-sided projective planes are a possibility, since in such a
    case this routine might fail (as explained above). Note however
    that this routine might still succeed, and so success is not a
    proof that no embedded two-sided projective planes exist.

.. warning::
    The algorithms used in this routine rely on normal surface theory
    and so can be very slow for larger triangulations. For 3-sphere
    testing, see the routine isSphere() which uses faster methods
    where possible.

Precondition:
    This triangulation is valid, closed and connected.

Exception ``UnsolvedCase``:
    The original manifold is non-orientable and contains one or more
    embedded two-sided projective planes, and this routine was not
    able to recover from this situation.

Returns:
    a list of triangulations of prime summands.)doc";

// Docstring regina::python::doc::Triangulation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given triangulation.

All tetrahedra that belong to this triangulation will be moved to
*other*, and all tetrahedra that belong to *other* will be moved to
this triangulation. Likewise, all skeletal objects (such as lower-
dimensional faces, components, and boundary components) and all cached
properties (such as homology and fundamental group) will be swapped.

In particular, any pointers or references to Tetrahedron<3> and/or
Face<3, subdim> objects will remain valid.

This routine will behave correctly if *other* is in fact this
triangulation.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both triangulations which may in turn call
    arbitrary code via any registered packet listeners.

Parameter ``other``:
    the triangulation whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Triangulation_::turaevViro
static const char *turaevViro =
R"doc(Computes the given Turaev-Viro state sum invariant of this 3-manifold
using exact arithmetic.

The initial data for the Turaev-Viro invariant is as described in the
paper of Turaev and Viro, "State sum invariants of 3-manifolds and
quantum 6j-symbols", Topology, vol. 31, no. 4, 1992, pp 865-902. In
particular, Section 7 of this paper describes the initial data as
determined by an integer r ≥ 3, and a root of unity q₀ of degree 2r
for which q₀² is a primitive root of unity of degree r. There are
several cases to consider:

* *r* may be even. In this case *q₀* must be a primitive (*2r*)th root
  of unity, and the invariant is computed as an element of the
  cyclotomic field of order *2r*. There is no need to specify _which_
  root of unity is used, since switching between different roots of
  unity corresponds to an automorphism of the underlying cyclotomic
  field (i.e., it does not yield any new information). Therefore, if
  *r* is even, the additional argument *parity* is ignored.

* *r* may be odd, and *q₀* may be a primitive (2*r*)th root of unity.
  This case corresponds to passing the argument *parity* as ``True``.
  Here the invariant is again computed as an element of the cyclotomic
  field of order *2r*. As before, there is no need to give further
  information as to which root of unity is used, since switching
  between roots of unity does not yield new information.

* *r* may be odd, and *q₀* may be a primitive (*r*)th root of unity.
  This case corresponds to passing the argument *parity* as ``False``.
  In this case the invariant is computed as an element of the
  cyclotomic field of order *r*. Again, there is no need to give
  further information as to which root of unity is used.

This routine works entirely within the relevant cyclotomic field,
which yields exact results but adds a significant overhead to the
running time. If you want a fast floating-point approximation, you can
call turaevViroApprox() instead.

Unlike this routine, turaevViroApprox() requires a precise
specification of which root of unity is used (since it returns a
numerical real value). The numerical value obtained by calling
``turaevViroApprox(r, whichRoot)`` should be the same as
``turaevViro(r, parity).evaluate(whichRoot)``, where *parity* is
``True`` or ``False`` according to whether *whichRoot* is odd or even
respectively. Of course in practice the numerical values might be very
different, since turaevViroApprox() performs significantly more
floating point operations, and so is subject to a much larger
potential numerical error.

If the requested Turaev-Viro invariant has already been computed, then
the result will be cached and so this routine will be very fast (since
it just returns the previously computed result). Otherwise the
computation could be quite slow, particularly for larger
triangulations and/or larger values of *r*.

Since Regina 7.0, this routine will not return until the Turaev-Viro
computation is complete, regardless of whether a progress tracker was
passed. If you need the old behaviour (where passing a progress
tracker caused the computation to start in the background), simply
call this routine in a new detached thread.

Precondition:
    This triangulation is valid, closed and non-empty.

Python:
    The global interpreter lock will be released while this function
    runs, so you can use it with Python-based multithreading.

Parameter ``r``:
    the integer *r* as described above; this must be at least 3.

Parameter ``parity``:
    determines for odd *r* whether *q₀* is a primitive *2r*th or *r*th
    root of unity, as described above.

Parameter ``alg``:
    the algorithm with which to compute the invariant. If you are not
    sure, the default value (ALG_DEFAULT) is a safe choice. This
    should be treated as a hint only: if the algorithm you choose is
    not supported for the given parameters (*r* and *parity*), then
    Regina will use another algorithm instead.

Parameter ``tracker``:
    a progress tracker through will progress will be reported, or
    ``None`` if no progress reporting is required.

Returns:
    the requested Turaev-Viro invariant, or an uninitialised field
    element if the calculation was cancelled via the given progress
    tracker.

See also:
    allCalculatedTuraevViro)doc";

// Docstring regina::python::doc::Triangulation_::turaevViroApprox
static const char *turaevViroApprox =
R"doc(Computes the given Turaev-Viro state sum invariant of this 3-manifold
using a fast but inexact floating-point approximation.

The initial data for the Turaev-Viro invariant is as described in the
paper of Turaev and Viro, "State sum invariants of 3-manifolds and
quantum 6j-symbols", Topology, vol. 31, no. 4, 1992, pp 865-902. In
particular, Section 7 describes the initial data as determined by an
integer *r* ≥ 3 and a root of unity *q₀* of degree *2r* for which
*q₀²* is a primitive root of unity of degree *r*.

The argument *whichRoot* specifies which root of unity is used for
*q₀*. Specifically, *q₀* will be the root of unity ``e^(2πi *
whichRoot / 2r)``. There are additional preconditions on *whichRoot*
to ensure that *q₀²* is a _primitive_ root of unity of degree *r*; see
below for details.

This same invariant can be computed by calling ``turaevViro(r,
parity).evaluate(whichRoot)``, where *parity* is ``True`` or ``False``
according to whether *whichRoot* is odd or even respectively. Calling
turaevViroApprox() is significantly faster (since it avoids the
overhead of working in cyclotomic fields), but may also lead to a much
larger numerical error (since this routine might perform an
exponential number of floating point operations, whereas the
alternative only uses floating point for the final call to
Cyclotomic::evaluate()).

These invariants, although computed in the complex field, should all
be reals. Thus the return type is an ordinary double.

Precondition:
    This triangulation is valid, closed and non-empty.

Precondition:
    The argument *whichRoot* is strictly between 0 and *2r*, and has
    no common factors with *r*.

Parameter ``r``:
    the integer *r* as described above; this must be at least 3.

Parameter ``whichRoot``:
    specifies which root of unity is used for *q₀*, as described
    above.

Parameter ``alg``:
    the algorithm with which to compute the invariant. If you are not
    sure, the default value (ALG_DEFAULT) is a safe choice. This
    should be treated as a hint only: if the algorithm you choose is
    not supported for the given parameters (*r* and *whichRoot*), then
    Regina will use another algorithm instead.

Returns:
    the requested Turaev-Viro invariant.

See also:
    allCalculatedTuraevViro)doc";

// Docstring regina::python::doc::Triangulation_::twoOneMove
static const char *twoOneMove =
R"doc(Checks the eligibility of and/or performs a 2-1 move about the given
edge. This involves taking an edge meeting only one tetrahedron just
once and merging that tetrahedron with one of the tetrahedra joining
it.

This can be done assuming the following conditions:

* The edge must be valid and non-boundary.

* The two remaining faces of the tetrahedron are not joined, and the
  tetrahedron face opposite the given endpoint of the edge is not
  boundary.

* Consider the second tetrahedron to be merged (the one joined along
  the face opposite the given endpoint of the edge). Moreover,
  consider the two edges of this second tetrahedron that run from the
  (identical) vertices of the original tetrahedron not touching *e* to
  the vertex of the second tetrahedron not touching the original
  tetrahedron. These edges must be distinct and may not both be in the
  boundary.

There are additional "distinct and not both boundary" conditions on
faces of the second tetrahedron, but those follow automatically from
the final condition above.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal.

If this triangulation is currently oriented, then this 2-1 move will
label the new tetrahedra in a way that preserves the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal.

Precondition:
    The given edge is an edge of this triangulation.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``edgeEnd``:
    the end of the edge _opposite_ that at which the second
    tetrahedron (to be merged) is joined. The end is 0 or 1,
    corresponding to the labelling (0,1) of the vertices of the edge
    as described in EdgeEmbedding<3>::vertices().

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold. If *check* is ``False``, the function
    simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::twoZeroMove
static const char *twoZeroMove =
R"doc(Checks the eligibility of and/or performs a 2-0 move about the given
edge of degree 2. This involves taking the two tetrahedra joined at
that edge and squashing them flat. This can be done if:

* the edge is valid and non-boundary;

* the two tetrahedra are distinct;

* the edges opposite *e* in each tetrahedron are distinct and not both
  boundary;

* if triangles *f1* and *f2* from one tetrahedron are to be flattened
  onto triangles *g1* and *g2* of the other respectively, then (a)
  *f1* and *g1* are distinct, (b) *f2* and *g2* are distinct, (c) we
  do not have both *f1* = *g2* and *g1* = *f2*, (d) we do not have
  both *f1* = *f2* and *g1* = *g2*, and (e) we do not have two of the
  triangles boundary and the other two identified.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<3>::lock() and
Simplex<3>::lockFacet() for further details on locks).

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given edge is an edge of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how locks work and
    what their implications are.

Parameter ``e``:
    the edge about which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::twoZeroMove_2
static const char *twoZeroMove_2 =
R"doc(Checks the eligibility of and/or performs a 2-0 move about the given
vertex of degree 2. This involves taking the two tetrahedra joined at
that vertex and squashing them flat. This can be done if:

* the vertex is non-boundary and has a 2-sphere vertex link;

* the two tetrahedra are distinct;

* the triangles opposite *v* in each tetrahedron are distinct and not
  both boundary;

* the two tetrahedra meet each other on all three faces touching the
  vertex (as opposed to meeting each other on one face and being glued
  to themselves along the other two).

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<3>::lock() and
Simplex<3>::lockFacet() for further details on locks).

If this triangulation is currently oriented, then this operation will
preserve the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *v*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given vertex is a vertex of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<3>::lock() and
    Simplex<3>::lockFacet() for further details on how locks work and
    what their implications are.

Parameter ``v``:
    the vertex about which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.)doc";

// Docstring regina::python::doc::Triangulation_::zeroTwoMove
static const char *zeroTwoMove =
R"doc(Checks the eligibility of and/or performs a 0-2 move about the (not
necessarily distinct) triangles
``e0.tetrahedron()->triangle(e0.vertices()[t0])`` and
``e1.tetrahedron()->triangle(e1.vertices()[t1])``.

This involves fattening up these two triangles into a new pair of
tetrahedra around a new degree-two edge *d*; this is the inverse of
performing a 2-0 move about the edge *d*. This can be done if and only
if the following conditions are satisfied:

* *e0* and *e1* are both embeddings of the same edge *e*.

* *t0* and *t1* are both either 2 or 3; this ensures that the
  triangles about which we perform the move are triangles that are
  incident with *e*.

* The edge *e* is valid.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal.

If this triangulation is currently oriented, then this 0-2 move will
label the new tetrahedra in a way that preserves the orientation.

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the arguments *e0* and *e1*) can no
longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    by known in advance that the move is legal.

Precondition:
    The edge *e* is an edge of this triangulation.

Parameter ``e0``:
    an embedding of the common edge *e* of the two triangles about
    which to perform the move.

Parameter ``t0``:
    one of the two triangles about which to perform the move
    (associated to the edge embedding *e0*).

Parameter ``e1``:
    another embedding of the edge *e*.

Parameter ``t1``:
    the other triangle about which to perform move (associated to the
    edge embedding *e1*).

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold. If *check* is false, the function simply
    returns ``True``.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::zeroTwoMove_2
static const char *zeroTwoMove_2 =
R"doc(Checks the eligibility of and/or performs a 0-2 move about the (not
necessarily distinct) triangles incident to *e* that are numbered *t0*
and *t1*.

This involves fattening up these two triangles into a new pair of
tetrahedra around a new degree-two edge *d*; this is the inverse of
performing a 2-0 move about the edge *d*. This can be done if and only
if the following conditions are satisfied:

* The edge *e* is valid.

* The numbers *t0* and *t1* are both less than or equal to
  ``e->degree()``, and strictly less than ``e->degree()`` if *e* is
  non-boundary. This ensures that *t0* and *t1* correspond to sensible
  triangle numbers (as described below).

The triangles incident to *e* are numbered as follows:

* For each *i* from 0 up to ``e->degree()``, we assign the number *i*
  to the triangle ``emb.tetrahedron()->triangle(emb.vertices()[3])``,
  where *emb* denotes ``e->embedding(i)``.

* If *e* is a boundary edge, then we additionally assign the number
  ``e->degree()`` to the boundary triangle
  ``emb.tetrahedron()->triangle(emb.vertices()[2])``, where this time
  *emb* denotes ``e->back()``.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal.

If this triangulation is currently oriented, then this 0-2 move will
label the new tetrahedra in a way that preserves the orientation.

The implementation of this routine simply translates the given
arguments to call the variant of zeroTwoMove() that takes a pair of
edge embeddings (and other associated arguments).

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *e*) can no longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    by known in advance that the move is legal.

Precondition:
    The given edge *e* is an edge of this triangulation.

Parameter ``e``:
    the common edge of the two triangles about which to perform the
    move.

Parameter ``t0``:
    the number assigned to one of two triangles about which to perform
    the move.

Parameter ``t1``:
    the number assigned to the other triangle about which to perform
    the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold. If *check* is false, the function simply
    returns ``True``.

Author:
    Alex He)doc";

// Docstring regina::python::doc::Triangulation_::zeroTwoMove_3
static const char *zeroTwoMove_3 =
R"doc(Checks the eligibility of and/or performs a 0-2 move about the (not
necessarily distinct) triangles *t0* and *t1*.

This involves fattening up these two triangles into a new pair of
tetrahedra around a new degree-two edge *d*; this is the inverse of
performing a 2-0 move about the edge *d*. This can be done if and only
if the following conditions are satisfied:

* The edges ``t0->edge(e0)`` and ``t1->edge(e1)`` are the same edge
  *e* of this triangulation.

* The edge *e* is valid.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal.

If this triangulation is currently oriented, then this 0-2 move will
label the new tetrahedra in a way that preserves the orientation.

The implementation of this routine simply translates the given
arguments to call the variant of zeroTwoMove() that takes a pair of
edge embeddings (and other associated arguments).

Note that after performing this move, all skeletal objects (triangles,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the arguments *t0* and *t1*) can no
longer be used.

Precondition:
    If the move is being performed and no check is being run, it must
    by known in advance that the move is legal.

Precondition:
    The given triangles *t0* and *t1* are triangles of this
    triangulation.

Precondition:
    The numbers *e0* and *e1* are both 0, 1 or 2.

Parameter ``t0``:
    one of the two triangles about which to perform the move.

Parameter ``e0``:
    the edge at which *t0* meets the other triangle about which to
    perform the move.

Parameter ``t1``:
    the other triangle about which to perform the move.

Parameter ``e1``:
    the edge at which *t1* meets *t0*.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold. If *check* is false, the function simply
    returns ``True``.

Author:
    Alex He)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

