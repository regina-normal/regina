/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace Triangulation_ {

// Docstring regina::python::doc::Triangulation_::global_swap
constexpr const char *global_swap =
R"doc(Swaps the contents of the two given triangulations.

This global routine simply calls Triangulation<dim>::swap(); it is
provided so that Triangulation<dim> meets the C++ Swappable
requirements.

See Triangulation<dim>::swap() for more details.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    packet change events which may in turn call arbitrary code via any
    registered packet listeners.

Parameter ``lhs``:
    the triangulation whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the triangulation whose contents should be swapped with *lhs*.)doc";

}

namespace detail {

// Docstring regina::python::doc::detail::TriangulationBase
constexpr const char *TriangulationBase =
R"doc(Provides core functionality for *dim*-dimensional triangulations.

Such a triangulation is represented by the class Triangulation<dim>,
which uses this as a base class. End users should not need to refer to
TriangulationBase directly.

See the Triangulation class notes for further information.

Note that this class does not derive from Output. This is to avoid
clashes with the output code inherited from Packet. Specifically:

* for those dimensions where Triangulation<dim> derives from Packet,
  the output routines are inherited from Packet (which derives from
  Output).

* for other dimensions, Triangulation<dim> derives from Output
  directly.

Python:
    This base class is not present, but the "end user" class
    Triangulation<dim> is.

Template parameter ``dim``:
    the dimension of the triangulation. This must be between 2 and 15
    inclusive.)doc";

}

namespace detail::TriangulationBase_ {

// Docstring regina::python::doc::detail::TriangulationBase_::__copy
constexpr const char *__copy =
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

// Docstring regina::python::doc::detail::TriangulationBase_::__default
constexpr const char *__default =
R"doc(Default constructor.

Creates an empty triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::__eq
constexpr const char *__eq =
R"doc(Determines if this triangulation is combinatorially identical to the
given triangulation.

Here "identical" means that the triangulations have the same number of
top-dimensional simplices, with gluings between the same pairs of
numbered simplices using the same gluing permutations. In other words,
"identical" means that the triangulations are isomorphic via the
identity isomorphism.

For the less strict notion of _isomorphic_ triangulations, which
allows relabelling of the top-dimensional simplices and their
vertices, see isIsomorphicTo() instead.

This test does _not_ examine the textual simplex descriptions or
simplex/facet locks, as seen in Simplex<dim>::description() and
Simplex<dim>::lockMask(); these may still differ. It also does not
test whether lower-dimensional faces are numbered identically
(vertices, edges and so on); this routine is only concerned with top-
dimensional simplices.

(At the time of writing, two identical triangulations will always
number their lower-dimensional faces in the same way. However, it is
conceivable that in future versions of Regina there may be situations
in which identical triangulations can acquire different numberings for
vertices, edges, and so on.)

In Regina 6.0.1 and earlier, this comparison was called
isIdenticalTo().

Parameter ``other``:
    the triangulation to compare with this.

Returns:
    ``True`` if and only if the two triangulations are combinatorially
    identical.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::__init
constexpr const char *__init =
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

If *cloneLocks* is ``True`` then any locks on the top-dimensional
simplices and/or facets of *src* will be copied across. If
*cloneLocks* is ``False`` then the new triangulation will have no
locks at all.

Parameter ``src``:
    the triangulation to copy.

Parameter ``cloneProps``:
    ``True`` if this should also clone any computed properties as well
    as the skeleton of the given triangulation, or ``False`` if the
    new triangulation should have such properties and skeletal data
    marked as unknown.

Parameter ``cloneLocks``:
    ``True`` if this should also clone any simplex and/or facet locks
    from the given triangulation, or ``False`` if the new
    triangulation should have no locks at all.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::__ne
constexpr const char *__ne =
R"doc(Determines if this triangulation is not combinatorially identical to
the given triangulation.

Here "identical" means that the triangulations have the same number of
top-dimensional simplices, with gluings between the same pairs of
numbered simplices using the same gluing permutations. In other words,
"identical" means that the triangulations are isomorphic via the
identity isomorphism.

For the less strict notion of _isomorphic_ triangulations, which
allows relabelling of the top-dimensional simplices and their
vertices, see isIsomorphicTo() instead.

This test does _not_ examine the textual simplex descriptions or
simplex/facet locks, as seen in Simplex<dim>::description() and
Simplex<dim>::lockMask(); these may still differ. It also does not
test whether lower-dimensional faces are numbered identically
(vertices, edges and so on); this routine is only concerned with top-
dimensional simplices.

(At the time of writing, two identical triangulations will always
number their lower-dimensional faces in the same way. However, it is
conceivable that in future versions of Regina there may be situations
in which identical triangulations can acquire different numberings for
vertices, edges, and so on.)

Parameter ``other``:
    the triangulation to compare with this.

Returns:
    ``True`` if and only if the two triangulations are not
    combinatorially identical.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::barycentricSubdivision
constexpr const char *barycentricSubdivision =
R"doc(Deprecated routine that performs a barycentric subdivision of the
triangulation.

.. deprecated::
    This routine has been renamed to subdivide(), both to shorten the
    name but also to make it clearer that this triangulation will be
    modified directly.

Precondition:
    *dim* is one of Regina's standard dimensions.

Exception ``LockViolation``:
    This triangulation contains at least one locked top-dimensional
    simplex and/or facet. This exception will be thrown before any
    changes are made. See Simplex<dim>::lock() and
    Simplex<dim>::lockFacet() for further details on how such locks
    work and what their implications are.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::boundaryComponent
constexpr const char *boundaryComponent =
R"doc(Returns the requested boundary component of this triangulation.

Note that each time the triangulation changes, all boundary components
will be deleted and replaced with new ones. Therefore this object
should be considered temporary only.

Parameter ``index``:
    the index of the desired boundary component; this must be between
    0 and countBoundaryComponents()-1 inclusive.

Returns:
    the requested boundary component.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::boundaryComponents
constexpr const char *boundaryComponents =
R"doc(Returns an object that allows iteration through and random access to
all boundary components of this triangulation.

Note that, in Regina's standard dimensions, each ideal vertex forms
its own boundary component, and some invalid vertices do also. See the
BoundaryComponent class notes for full details on what constitutes a
boundary component in standard and non-standard dimensions.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (BoundaryComponent<dim>* b : tri.boundaryComponents()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the triangulation exists. In contrast, however, remember that
the individual boundary components _within_ this list will be deleted
and replaced each time the triangulation changes. Therefore it is best
to treat this object as temporary only, and to call
boundaryComponents() again each time you need it.

Returns:
    access to the list of all boundary components.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::boundaryMap
constexpr const char *boundaryMap =
R"doc(Returns the boundary map from *subdim*-faces to (*subdim*-1)-faces of
the triangulation, where the face dimension does not need to be known
until runtime.

For C++ programmers who know *subdim* at compile time, you are better
off using the template function boundaryMap<subdim>() instead, which
is slightly faster.

This is the boundary map that you would use if you were building the
homology groups manually from a chain complex.

Unlike homology(), this code does _not_ use the dual skeleton: instead
it uses the primal (i.e., ordinary) skeleton.

* The main advantage of this is that you can easily match rows and
  columns of the returned matrix to faces of this triangulation.

* The main disadvantage is that ideal vertices are _not_ treated as
  though they were truncated; instead they are just treated as 0-faces
  that appear as part of the chain complex.

The matrix that is returned should be thought of as acting on column
vectors. Specifically, the *c*th column of the matrix corresponds to
the *c*th *subdim*-face of this triangulation, and the *r*th row
corresponds to the *r*th (*subdim*-1)-face of this triangulation.

For the boundary map, we fix orientations as follows. In simplicial
homology, for any *k*, the orientation of a *k*-simplex is determined
by assigning labels 0,...,*k* to its vertices. For this routine, since
every *k*-face *f* is already a *k*-simplex, these labels will just be
the inherent vertex labels 0,...,*k* of the corresponding Face<k>
object. If you need to convert these labels into vertex numbers of a
top-dimensional simplex containing *f*, you can use either
Simplex<dim>::faceMapping<k>(), or the equivalent routine
FaceEmbedding<k>::vertices().

If you wish to convert these boundary maps to homology groups
yourself, either the AbelianGroup class (if you do not need to track
which face is which) or the MarkedAbelianGroup class (if you do need
to track individual faces) can help you do this.

Note that, unlike many of the templated face-related routines, this
routine explicitly supports the case *subdim* = *dim*.

Precondition:
    This triangulation is valid and non-empty.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    less than 1 or greater than *dim*).

Parameter ``subdim``:
    the face dimension; this must be between 1 and *dim* inclusive.

Returns:
    the boundary map from *subdim*-faces to (*subdim*-1)-faces.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::component
constexpr const char *component =
R"doc(Returns the requested connected component of this triangulation.

Note that each time the triangulation changes, all component objects
will be deleted and replaced with new ones. Therefore this component
object should be considered temporary only.

Parameter ``index``:
    the index of the desired component; this must be between 0 and
    countComponents()-1 inclusive.

Returns:
    the requested component.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::components
constexpr const char *components =
R"doc(Returns an object that allows iteration through and random access to
all components of this triangulation.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Component<dim>* c : tri.components()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the triangulation exists. In contrast, however, remember that
the individual component objects _within_ this list will be deleted
and replaced each time the triangulation changes. Therefore it is best
to treat this object as temporary only, and to call components() again
each time you need it.

Returns:
    access to the list of all components.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countBoundaryComponents
constexpr const char *countBoundaryComponents =
R"doc(Returns the number of boundary components in this triangulation.

Note that, in Regina's standard dimensions, each ideal vertex forms
its own boundary component, and some invalid vertices do also. See the
BoundaryComponent class notes for full details on what constitutes a
boundary component in standard and non-standard dimensions.

Returns:
    the number of boundary components.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countBoundaryFaces
constexpr const char *countBoundaryFaces =
R"doc(Returns the number of boundary *subdim*-faces in this triangulation,
where the face dimension does not need to be known until runtime.

This routine takes linear time in the dimension *dim*. For C++
programmers who know *subdim* at compile time, you are better off
using the template function countBoundaryFaces<subdim>() instead,
which is fast constant time.

Specifically, this counts the number of *subdim*-faces for which
isBoundary() returns ``True``. This may lead to some unexpected
results in non-standard scenarios; for example:

* In non-standard dimensions, ideal vertices are not recognised and so
  will not be counted as boundary;

* In an invalid triangulation, the number of boundary faces reported
  here may be smaller than the number of faces obtained when you
  triangulate the boundary using BoundaryComponent::build(). This is
  because "pinched" faces (where separate parts of the boundary are
  identified together) will only be counted once here, but will
  "spring apart" into multiple faces when the boundary is
  triangulated.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative or greater than *dim*-1).

Parameter ``subdim``:
    the face dimension; this must be between 0 and *dim*-1 inclusive.

Returns:
    the number of boundary *subdim*-faces.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countBoundaryFacets
constexpr const char *countBoundaryFacets =
R"doc(Returns the total number of boundary facets in this triangulation.

This routine counts facets of top-dimensional simplices that are not
glued to some adjacent top-dimensional simplex.

This is equivalent to calling countBoundaryFaces<dim-1>().

Returns:
    the total number of boundary facets.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countComponents
constexpr const char *countComponents =
R"doc(Returns the number of connected components in this triangulation.

Returns:
    the number of connected components.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countEdges
constexpr const char *countEdges =
R"doc(A dimension-specific alias for countFaces<1>().

This alias is available for all dimensions *dim*.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countFaces
constexpr const char *countFaces =
R"doc(Returns the number of *subdim*-faces in this triangulation, where the
face dimension does not need to be known until runtime.

This routine takes linear time in the dimension *dim*. For C++
programmers who know *subdim* at compile time, you are better off
using the template function countFaces<subdim>() instead, which is
fast constant time.

For convenience, this routine explicitly supports the case *subdim* =
*dim*. This is _not_ the case for the routines face() and faces(),
which give access to individual faces (the reason relates to the fact
that top-dimensional simplices are built manually, whereas lower-
dimensional faces are deduced properties).

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative or greater than *dim*).

Parameter ``subdim``:
    the face dimension; this must be between 0 and *dim* inclusive.

Returns:
    the number of *subdim*-faces.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countPentachora
constexpr const char *countPentachora =
R"doc(A dimension-specific alias for countFaces<4>().

This alias is available for dimensions *dim* ≥ 4.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countTetrahedra
constexpr const char *countTetrahedra =
R"doc(A dimension-specific alias for countFaces<3>().

This alias is available for dimensions *dim* ≥ 3.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countTriangles
constexpr const char *countTriangles =
R"doc(A dimension-specific alias for countFaces<2>().

This alias is available for all dimensions *dim*.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::countVertices
constexpr const char *countVertices =
R"doc(A dimension-specific alias for countFaces<0>().

This alias is available for all dimensions *dim*.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::dot
constexpr const char *dot =
R"doc(Returns a Graphviz DOT representation of the dual graph of this
triangulation.

Every vertex of this graph represents a top-dimensional simplex, and
every edge represents a pair of simplex facets that are joined
together. Note that for a closed triangulation this graph will be
entirely (*dim* + 1)-valent; for triangulations with boundary facets,
some graph vertices will have degree *dim* or less.

The output from this routine can be used as a standalone DOT file,
ready for use with Graphviz. This DOT file will describe an
_undirected_ graph, and should be used with either the *neato* or
*fdp* programs shipped with Graphviz.

The functions Triangulation<dim>::dot() and FacetPairing<dim>::dot()
differ in a few ways:

* Triangulation<dim>::dot() does not support _subgraph_ output (where
  you construct a large DOT file containing the dual graphs of many
  independent triangulations). If you need subgraph output, you can
  always call ``pairing().dot()`` instead.

* Triangulation<dim>::dot() makes more use of colour, in particular to
  indicate locked top-dimensional simplices and/or facets.

If you are simply writing this string to an output stream then you
should call writeDot() instead, which is more efficient.

Parameter ``labels``:
    indicates whether graph vertices will be labelled with the
    corresponding simplex numbers.

Returns:
    the output of writeDot(), as outlined above.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::doubleCover
constexpr const char *doubleCover =
R"doc(Returns the orientable double cover of this triangulation.

Each orientable component will be duplicated, and each non-orientable
component will be converted into its orientable double cover.

If this triangulation has locks on any top-dimensional simplices
and/or their facets, then these locks will be duplicated alongside
their corresponding simplices and/or facets (i.e., they will appear in
both sheets of the double cover).)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::dualBoundaryMap
constexpr const char *dualBoundaryMap =
R"doc(Returns the boundary map from dual *subdim*-faces to dual
(*subdim*-1)-faces of the triangulation, where the face dimension does
not need to be known until runtime.

For C++ programmers who know *subdim* at compile time, you are better
off using the template function dualBoundaryMap<subdim>() instead,
which is slightly faster.

This function is analogous to boundaryMap(), but is designed to work
with dual faces instead of ordinary (primal) faces. In particular,
this is used in the implementation of homology(), which works with the
dual skeleton in order to effectively truncate ideal vertices.

The matrix that is returned should be thought of as acting on column
vectors. Specifically, the *c*th column of the matrix corresponds to
the *c*th dual *subdim*-face of this triangulation, and the *r*th row
corresponds to the *r*th dual (*subdim*-1)-face of this triangulation.
Here we index dual faces in the same order as the (primal) faces of
the triangulation that they are dual to, except that we omit primal
_boundary_ faces (i.e., primal faces for which Face::isBoundary()
returns ``True``). Therefore, for triangulations with boundary, the
dual face indices and the corresponding primal face indices might not
be equal.

For this dual boundary map, for positive dual face dimensions *k*, we
fix the orientations of the dual *k*-faces as follows:

* In simplicial homology, the orientation of a *k*-simplex is
  determined by assigning labels 0,...,*k* to its vertices.

* Consider a dual *k*-face *d*, and let this be dual to the primal
  (*dim*-*k*)-face *f*. In general, *d* will _not_ be a simplex. Let
  *B* denote the barycentre of *f* (which also appears as the "centre"
  point of *d*).

* Let *emb* be an arbitrary FaceEmbedding<dim-k> for *f* (i.e., chosen
  from ``f.embeddings()``), and let *s* be the corresponding top-
  dimensional simplex containing *f* (i.e., ``emb.simplex()``). For
  the special case of dual edges (*k* = 1), this choice matters; here
  we choose *emb* to be the first embedding (that is, ``f.front()``).
  For larger *k* this choice does not matter; see below for the
  reasons why.

* Now consider how *d* intersects the top-dimensional simplex *s*.
  This intersection is a *k*-polytope with *B* as one of its vertices.
  We can extend this polytope away from *B*, pushing it all the way
  through the simplex *s*, until it becomes a *k*-simplex *g* whose
  vertices are *B* along with the *k* "unused" vertices of *s* that do
  _not_ appear in *f*.

* We can now define the orientation of the dual *k*-face *d* to be the
  orientation of this *k*-simplex *g* that contains it. All that
  remains now is to orient *g* by choosing a labelling 0,...,*k* for
  its vertices.

* To orient *g*, we assign the label 0 to *B*, and we assign the
  labels 1,...,*k* to the "unused" vertices
  ``v[dim-k+1]``,...,``v[dim]`` of *s* respectively, where *v* is the
  permutation ``emb.vertices()``.

* Finally, we note that for *k* > 1, the orientation for *d* does not
  depend on the particular choice of *s* and *emb:* by the
  preconditions and the fact that this routine only considers duals of
  non-boundary faces, the link of *f* must be a sphere, and therefore
  the images of those "other" vertices are fixed in a way that
  preserves orientation as you walk around the link. See the
  documentation for Simplex<dim>::faceMapping() for details.

* For the special case of dual edges (*k* = 1), the conditions above
  can be described more simply: the two endpoints of the dual edge *d*
  correspond to the two top-dimensional simplices on either side of
  the (*dim*-1)-face *f*, and we orient *d* by labelling these
  endpoints (0, 1) in the order (``f.back()``, ``f.front()``).

If you wish to convert these boundary maps to homology groups
yourself, either the AbelianGroup class (if you do not need to track
which dual face is which) or the MarkedAbelianGroup class (if you do
need to track individual dual faces) can help you do this.

Precondition:
    This triangulation is valid and non-empty.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (as
    documented for the *subdim* argument below).

Parameter ``subdim``:
    the dual face dimension; this must be between 1 and *dim*
    inclusive if *dim* is one of Regina's standard dimensions, or
    between 1 and (*dim* - 1) inclusive otherwise.

Returns:
    the boundary map from dual *subdim*-faces to dual
    (*subdim*-1)-faces.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::dualToPrimal
constexpr const char *dualToPrimal =
R"doc(Returns a map from dual chains to primal chains that preserves
homology classes, where the chain dimension does not need to be known
until runtime.

For C++ programmers who know *subdim* at compile time, you are better
off using the template function dualToPrimal<subdim>() instead, which
is slightly faster.

The matrix that is returned should be thought of as acting on column
vectors. Specifically, the *c*th column of the matrix corresponds to
the *c*th dual *subdim*-face of this triangulation, and the *r*th row
corresponds to the *r*th primal *subdim*-face of this triangulation.

We index and orient these dual and primal faces in the same manner as
dualBoundaryMap() and boundaryMap() respectively. In particular, dual
faces are indexed in the same order as the primal
(*dim*-*subdim*)-faces of the triangulation that they are dual to,
except that we omit primal boundary faces. See dualBoundaryMap() and
boundaryMap() for further details.

The key feature of this map is that, if a column vector *v* represents
a cycle *c* in the dual chain complex (i.e., it is a chain with zero
boundary), and if this map is represented by the matrix *M*, then the
vector ``M×v`` represents a cycle in the primal chain complex that
belongs to the same *subdim*th homology class as *c*.

Regarding implementation: the map is constructed by (i) subdividing
each dual face into smaller *subdim*-simplices whose vertices are
barycentres of primal faces of different dimensions, (ii) moving each
barycentre to vertex 0 of the corresponding face, and then (iii)
discarding any resulting simplices with repeated vertices (which
become "flattened" to a dimension less than *subdim*).

Precondition:
    This trianguation is valid, non-empty, and non-ideal. Note that
    Regina can only detect ideal triangulations in standard
    dimensions; for higher dimensions it is the user's reponsibility
    to confirm this some other way.

Exception ``InvalidArgument``:
    The chain dimension *subdim* is outside the supported range (as
    documented for the *subdim* argument below).

Parameter ``subdim``:
    the chain dimension; this must be between 0 and (*dim* - 1)
    inclusive.

Returns:
    the map from dual *subdim*-chains to primal *subdim*-chains.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::dumpConstruction
constexpr const char *dumpConstruction =
R"doc(Deprecated routine that returns C++ code to reconstruct this
triangulation.

.. deprecated::
    This is equivalent to calling ``source(Language::Cxx)``, for
    compatibility with older versions of Regina. In particular, it is
    _not_ equivalent to calling ``source()`` (which defaults to the
    programming language currently being used). See source() for
    further details.

Returns:
    the C++ code that was generated.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::edge
constexpr const char *edge =
R"doc(A dimension-specific alias for face<1>().

This alias is available for all dimensions *dim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::edges
constexpr const char *edges =
R"doc(A dimension-specific alias for faces<1>().

This alias is available for all dimensions *dim*.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::eulerCharTri
constexpr const char *eulerCharTri =
R"doc(Returns the Euler characteristic of this triangulation. This will be
evaluated strictly as the alternating sum of the number of *i*-faces
(that is, ``countVertices() - countEdges() + countTriangles() -
...``).

Note that this routine handles ideal triangulations in a non-standard
way. Since it computes the Euler characteristic of the triangulation
(and not the underlying manifold), this routine will treat each ideal
boundary component as a single vertex, and _not_ as an entire
(*dim*-1)-dimensional boundary component.

In Regina's standard dimensions, for a routine that handles ideal
boundary components properly (by treating them as
(*dim*-1)-dimensional boundary components when computing Euler
characteristic), you can use the routine eulerCharManifold() instead.

Returns:
    the Euler characteristic of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::fVector
constexpr const char *fVector =
R"doc(Returns the f-vector of this triangulation, which counts the number of
faces of all dimensions.

The vector that is returned will have length *dim*+1. If this vector
is *f*, then *f*[*k*] will be the number of *k*-faces for each 0 ≤ *k*
≤ *dim*.

This routine is significantly more heavyweight than countFaces(). Its
advantage is that, unlike the templatised countFaces(), it allows you
to count faces whose dimensions are not known until runtime.

Returns:
    the f-vector of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::face
constexpr const char *face =
R"doc(Returns the requested *subdim*-face of this triangulation, in a way
that is optimised for Python programmers.

For C++ users, this routine is not very useful: since precise types
must be know at compile time, this routine returns a std::variant *v*
that could store a pointer to any class Face<dim, ...>. This means you
cannot access the face directly: you will still need some kind of
compile-time knowledge of *subdim* before you can extract and use an
appropriate Face<dim, subdim> object from *v*. However, once you know
*subdim* at compile time, you are better off using the (simpler and
faster) routine face<subdim>() instead.

For Python users, this routine is much more useful: the return type
can be chosen at runtime, and so this routine simply returns a
Face<dim, subdim> object of the appropriate face dimension that you
can use immediately.

The specific return type for C++ programmers will be
std::variant<Face<dim, 0>*, ..., Face<dim, dim-1>*>.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative, or greater than or equal to *dim*).

Parameter ``subdim``:
    the face dimension; this must be between 0 and *dim*-1 inclusive.

Parameter ``index``:
    the index of the desired face, ranging from 0 to
    countFaces<subdim>()-1 inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::faces
constexpr const char *faces =
R"doc(Returns an object that allows iteration through and random access to
all *subdim*-faces of this triangulation, in a way that is optimised
for Python programmers.

C++ users should not use this routine. The return type must be fixed
at compile time, and so it is a std::variant that can hold any of the
lightweight return types from the templated faces<subdim>() function.
This means that the return value will still need compile-time
knowledge of *subdim* to extract and use the appropriate face objects.
However, once you know *subdim* at compile time, you are much better
off using the (simpler and faster) routine faces<subdim>() instead.

For Python users, this routine is much more useful: the return type
can be chosen at runtime, and so this routine returns a Python list of
Face<dim, subdim> objects (holding all the *subdim*-faces of the
triangulation), which you can use immediately.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative, or greater than or equal to *dim*).

Parameter ``subdim``:
    the face dimension; this must be between 0 and *dim*-1 inclusive.

Returns:
    access to the list of all *subdim*-faces.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::findAllIsomorphisms
constexpr const char *findAllIsomorphisms =
R"doc(Finds all ways in which this triangulation is combinatorially
isomorphic to the given triangulation.

This routine behaves identically to isIsomorphicTo(), except that
instead of returning just one isomorphism, all such isomorphisms will
be found and processed. See the isIsomorphicTo() notes for details on
this.

For each isomorphism that is found, this routine will call *action*
(which must be a function or some other callable object).

* The first argument to *action* must be of type ``(const
  Isomorphism<dim>&)``; this will be a reference to the isomorphism
  that was found. If *action* wishes to keep the isomorphism, it
  should take a deep copy (not a reference), since the isomorphism may
  be changed and reused after *action* returns.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return a ``bool``. A return value of ``False``
  indicates that the search for isomorphisms should continue, and a
  return value of ``True`` indicates that the search should terminate
  immediately.

* This triangulation _must_ remain constant while the search runs
  (i.e., *action* must not modify the triangulation).

.. warning::
    For large dimensions, this routine can become extremely slow: its
    running time includes a factor of (*dim*+1)!.

Python:
    There are two versions of this function available in Python. The
    first form is ``findAllIsomorphisms(other, action)``, which
    mirrors the C++ function: it takes *action* which may be a pure
    Python function, the return value indicates whether *action* ever
    terminated the search, but it does _not_ take an additonal
    argument list (*args*). The second form is
    ``findAllIsomorphisms(other)``, which returns a Python list
    containing all of the isomorphisms that were found.

Parameter ``other``:
    the triangulation to compare with this one.

Parameter ``action``:
    a function (or other callable object) to call for each isomorphism
    that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial isomorphism argument.

Returns:
    ``True`` if *action* ever terminated the search by returning
    ``True``, or ``False`` if the search was allowed to run to
    completion.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::findAllSubcomplexesIn
constexpr const char *findAllSubcomplexesIn =
R"doc(Finds all ways in which an isomorphic copy of this triangulation is
contained within the given triangulation, possibly as a subcomplex of
some larger component (or components).

This routine behaves identically to isContainedIn(), except that
instead of returning just one isomorphism (which may be boundary
incomplete and need not be onto), all such isomorphisms will be found
and processed. See the isContainedIn() notes for details on this.

For each isomorphism that is found, this routine will call *action*
(which must be a function or some other callable object).

* The first argument to *action* must be of type ``(const
  Isomorphism<dim>&)``; this will be a reference to the isomorphism
  that was found. If *action* wishes to keep the isomorphism, it
  should take a deep copy (not a reference), since the isomorphism may
  be changed and reused after *action* returns.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return a ``bool``. A return value of ``False``
  indicates that the search for isomorphisms should continue, and a
  return value of ``True`` indicates that the search should terminate
  immediately.

* This triangulation _must_ remain constant while the search runs
  (i.e., *action* must not modify the triangulation).

.. warning::
    For large dimensions, this routine can become extremely slow: its
    running time includes a factor of (*dim*+1)!.

Python:
    There are two versions of this function available in Python. The
    first form is ``findAllSubcomplexesIn(other, action)``, which
    mirrors the C++ function: it takes *action* which may be a pure
    Python function, the return value indicates whether *action* ever
    terminated the search, but it does _not_ take an additonal
    argument list (*args*). The second form is
    ``findAllSubcomplexesIn(other)``, which returns a Python list
    containing all of the isomorphisms that were found.

Parameter ``other``:
    the triangulation in which to search for isomorphic copies of this
    triangulation.

Parameter ``action``:
    a function (or other callable object) to call for each isomorphism
    that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial isomorphism argument.

Returns:
    ``True`` if *action* ever terminated the search by returning
    ``True``, or ``False`` if the search was allowed to run to
    completion.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::finiteToIdeal
constexpr const char *finiteToIdeal =
R"doc(Converts each real boundary component into a cusp (i.e., an ideal
vertex). Only boundary components formed from real (*dim*-1)-faces
will be affected; ideal boundary components are already cusps and so
will not be changed.

One side-effect of this operation is that all spherical boundary
components will be filled in with balls.

This operation is performed by attaching a new *dim*-simplex to each
boundary (*dim*-1)-face, and then gluing these new simplices together
in a way that mirrors the adjacencies of the underlying boundary
facets. Each boundary component will thereby be pushed up through the
new simplices and converted into a cusp formed using vertices of these
new simplices.

In Regina's standard dimensions, where triangulations also support an
idealToFinite() operation, this routine is a loose converse of that
operation.

In dimension 2, every boundary component is spherical and so this
routine simply fills all the punctures in the underlying surface. (In
dimension 2, triangulations cannot have cusps).

.. warning::
    If a real boundary component contains vertices whose links are not
    discs, this operation may have unexpected results.

Exception ``LockViolation``:
    This triangulation contains at least one locked boundary facet.
    This exception will be thrown before any changes are made. See
    Simplex<dim>::lockFacet() for further details on how such locks
    work and what their implications are.

Returns:
    ``True`` if changes were made, or ``False`` if the original
    triangulation contained no real boundary components.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::fromGluings
constexpr const char *fromGluings =
R"doc(Creates a triangulation from a list of gluings.

This routine is an analogue to the variant of fromGluings() that takes
a C++ initialiser list; however, here the input data may be
constructed at runtime (which makes it accessible to Python, amongst
other things).

The iterator range (*beginGluings*, *endGluings*) should encode the
list of gluings for the triangulation. Each iterator in this range
must dereference to a tuple of the form (*simp*, *facet*, *adj*,
*gluing*); here *simp*, *facet* and *adj* are all integers, and
*gluing* is of type Perm<dim+1>. Each such tuple indicates that facet
*facet* of top-dimensional simplex number *simp* should be glued to
top-dimensional simplex number *adj* using the permutation *gluing*.
In other words, such a tuple encodes the same information as calling
``simplex(simp).join(facet, simplex(adj), gluing)`` upon the
triangulation being constructed.

Every gluing should be encoded from _one direction only_. This means,
for example, that to build a closed 3-manifold triangulation with *n*
tetrahedra, you would pass a list of 2*n* such tuples. If you attempt
to make the same gluing twice (e.g., once from each direction), then
this routine will throw an exception.

Any facet of a simplex that does not feature in the given list of
gluings (either as a source or a destination) will be left as a
boundary facet.

Note that, as usual, the top-dimensional simplices are numbered
0,...,(*size*-1), and the facets of each simplex are numbered
0,...,*dim*.

As an example, Python users can construct the figure eight knot
complement as follows:

```
tri = Triangulation3.fromGluings(2, [
    ( 0, 0, 1, Perm4(1,3,0,2) ), ( 0, 1, 1, Perm4(2,0,3,1) ),
    ( 0, 2, 1, Perm4(0,3,2,1) ), ( 0, 3, 1, Perm4(2,1,0,3) )])
```

.. note::
    The assumption is that the iterators dereference to a
    std::tuple<size_t, int, size_t, Perm<dim+1>>. However, this is not
    strictly necessary - the dereferenced type may be any type that
    supports std::get (and for which std::get<0..3>() yields suitable
    integer/permutation types).

Exception ``InvalidArgument``:
    The given list of gluings does not correctly describe a
    triangulation with *size* top-dimensional simplices.

Python:
    The gluings should be passed as a single Python list of tuples
    (not an iterator pair).

Parameter ``size``:
    the number of top-dimensional simplices in the triangulation to
    construct.

Parameter ``beginGluings``:
    the beginning of the list of gluings, as described above.

Parameter ``endGluings``:
    a past-the-end iterator indicating the end of the list of gluings.

Returns:
    the reconstructed triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::fromIsoSig
constexpr const char *fromIsoSig =
R"doc(Recovers a full triangulation from an isomorphism signature.

See isoSig() for more information on isomorphism signatures. It will
be assumed that the signature describes a triangulation of dimension
*dim*.

Currently this routine only supports isomorphism signatures that were
created with the default encoding (i.e., there was no *Encoding*
template parameter passed to isoSig()).

Calling isoSig() followed by fromIsoSig() is not guaranteed to produce
an _identical_ triangulation to the original, but it is guaranteed to
produce a combinatorially _isomorphic_ triangulation. In other words,
fromIsoSig() may reconstruct the triangulation with its simplices
and/or vertices relabelled. The optional argument to isoSig() allows
you to determine the precise relabelling that will be used, if you
need to know it.

For a full and precise description of the isomorphism signature format
for 3-manifold triangulations, see _Simplification paths in the
Pachner graphs of closed orientable 3-manifold triangulations_,
Burton, 2011, ``arXiv:1110.6080``. The format for other dimensions is
essentially the same, but with minor dimension-specific adjustments.

.. warning::
    Do not mix isomorphism signatures between dimensions! It is
    possible that the same string could corresponding to both a
    *p*-dimensional triangulation and a *q*-dimensional triangulation
    for different dimensions *p* and *q*.

Exception ``InvalidArgument``:
    The given string was not a valid *dim*-dimensional isomorphism
    signature created using the default encoding.

Parameter ``sig``:
    the isomorphism signature of the triangulation to construct. Note
    that isomorphism signatures are case-sensitive (unlike, for
    example, dehydration strings for 3-manifolds).

Returns:
    the reconstructed triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::fromSig
constexpr const char *fromSig =
R"doc(Alias for fromIsoSig(), to recover a full triangulation from an
isomorphism signature.

This alias fromSig() is provided to assist with generic code that can
work with both triangulations and links.

See fromIsoSig() for further details.

Exception ``InvalidArgument``:
    The given string was not a valid *dim*-dimensional isomorphism
    signature created using the default encoding.

Parameter ``sig``:
    the isomorphism signature of the triangulation to construct. Note
    that isomorphism signatures are case-sensitive (unlike, for
    example, dehydration strings for 3-manifolds).

Returns:
    the reconstructed triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::fundamentalGroup
constexpr const char *fundamentalGroup =
R"doc(An alias for group(), which returns the fundamental group of this
triangulation.

See group() for further details, including how ideal vertices and
invalid faces are managed.

.. note::
    In Regina 7.2, the routine fundamentalGroup() was renamed to
    group() for brevity and for consistency with Link::group(). This
    more expressive name fundamentalGroup() will be kept as a long-
    term alias, and you are welcome to continue using it if you
    prefer.

Precondition:
    This triangulation has at most one component.

.. warning::
    In dimension 3, if you are calling this from the subclass
    SnapPeaTriangulation then **any fillings on the cusps will be
    ignored**. (This is the same as for every routine implemented by
    Regina's Triangulation<3> class.) If you wish to compute the
    fundamental group with fillings, call
    SnapPeaTriangulation::fundamentalGroupFilled() instead.

Returns:
    the fundamental group.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::group
constexpr const char *group =
R"doc(Returns the fundamental group of this triangulation.

The fundamental group is computed in the dual 2-skeleton. This means:

* If the triangulation contains any ideal vertices, the fundamental
  group will be calculated as if each such vertex had been truncated.

* Likewise, if the triangulation contains any invalid faces of
  dimension 0,1,...,(*dim*-3), these will effectively be truncated
  also.

* In contrast, if the triangulation contains any invalid
  (*dim*-2)-faces (i.e., codimension-2-faces that are identified with
  themselves under a non-trivial map), the fundamental group will be
  computed _without_ truncating the centroid of the face. For
  instance, if a 3-manifold triangulation has an edge identified with
  itself in reverse, then the fundamental group will be computed
  without truncating the resulting projective plane cusp. This means
  that, if a barycentric subdivision is performed on a such a
  triangulation, the result of group() might change.

Bear in mind that each time the triangulation changes, the fundamental
group will be deleted. Thus the reference that is returned from this
routine should not be kept for later use. Instead, group() should be
called again; this will be instantaneous if the group has already been
calculated.

Before Regina 7.2, this routine was called fundamentalGroup(). It has
since been renamed to group() for brevity and for consistency with
Link::group(). The more expressive name fundamentalGroup() will be
kept, and you are welcome to use that instead if you prefer.

Precondition:
    This triangulation has at most one component.

.. warning::
    In dimension 3, if you are calling this from the subclass
    SnapPeaTriangulation then **any fillings on the cusps will be
    ignored**. (This is the same as for every routine implemented by
    Regina's Triangulation<3> class.) If you wish to compute the
    fundamental group with fillings, call
    SnapPeaTriangulation::fundamentalGroupFilled() instead.

Returns:
    the fundamental group.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::hasBoundaryFacets
constexpr const char *hasBoundaryFacets =
R"doc(Determines if this triangulation has any boundary facets.

This routine returns ``True`` if and only if the triangulation
contains some top-dimension simplex with at least one facet that is
not glued to an adjacent simplex.

Returns:
    ``True`` if and only if there are boundary facets.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::hasLocks
constexpr const char *hasLocks =
R"doc(Identifies whether any top-dimensional simplices and/or any of their
facets are locked.

In short, locking a top-dimensional simplex and/or some of its facets
means that that the simplex and/or facets must not be changed. See
Simplex<dim>::lock() and Simplex<dim>::lockFacet() for full details on
how locks work and what their implications are.

Returns:
    ``True`` if and only if there is at least one locked top-
    dimensional simplex or at least one locked facet of a top-
    dimensional simplex within this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::homology
constexpr const char *homology =
R"doc(Returns the *k*th homology group of this triangulation, treating any
ideal vertices as though they had been truncated, where the parameter
*k* does not need to be known until runtime.

For C++ programmers who know *k* at compile time, you are better off
using the template function homology<k>() instead, which is slightly
faster.

A problem with computing homology is that, if *dim* is not one of
Regina's standard dimensions, then Regina cannot actually _detect_
ideal vertices (since in general this requires solving undecidable
problems). Currently we resolve this by insisting that, in higher
dimensions, the homology dimension *k* is at most (*dim*-2); the
underlying algorithm will then effectively truncate _all_ vertices
(since truncating "ordinary" vertices whose links are spheres or balls
does not affect the *k*th homology in such cases).

In general, this routine insists on working with a valid triangulation
(see isValid() for what this means). However, for historical reasons,
if you are computing first homology (*k* = 1) then your triangulation
is allowed to be invalid, though the results might or might not be
useful to you. The homology will be computed using the dual skeleton:
what this means is that any invalid faces of dimension
0,1,...,(*dim*-3) will be treated as though their centroids had been
truncated, but any invalid (*dim*-2)-faces will be treated _without_
such truncation. A side-effect is that, after performing a barycentric
on an invalid triangulation, the group returned by homology<1>() might
change.

.. warning::
    In dimension 3, if you are calling this from the subclass
    SnapPeaTriangulation then **any fillings on the cusps will be
    ignored**. (This is the same as for every routine implemented by
    Regina's Triangulation<3> class.) If you wish to compute homology
    with fillings, call SnapPeaTriangulation::homologyFilled()
    instead.

Precondition:
    Unless you are computing first homology (*k* = 1), this
    triangulation must be valid, and every face that is not a vertex
    must have a ball or sphere link. The link condition already forms
    part of the validity test if *dim* is one of Regina's standard
    dimensions, but in higher dimensions it is the user's own
    responsibility to ensure this. See isValid() for details.

Exception ``FailedPrecondition``:
    This triangulation is invalid, and the homology dimension *k* is
    not 1.

Exception ``InvalidArgument``:
    The homology dimension *k* is outside the supported range. This
    range depends upon the triangulation dimension *dim*; for details
    see the documentation below for the argument *k*.

Python:
    Like the C++ template function homology<k>(), you can omit the
    homology dimension *k*; this will default to 1.

Parameter ``k``:
    the dimension of the homology group to return; this must be
    between 1 and (*dim* - 1) inclusive if *dim* is one of Regina's
    standard dimensions, or between 1 and (*dim* - 2) inclusive if
    not.

Returns:
    the *k*th homology group.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::insertTriangulation
constexpr const char *insertTriangulation =
R"doc(Inserts a copy of the given triangulation into this triangulation.

The top-dimensional simplices of *source* will be copied into this
triangulation in the same order in which they appear in *source*. That
is, if the original size of this triangulation was *S*, then the
simplex at index *i* in *source* will be copied into this
triangulation as a new simplex at index *S*+*i*.

The copies will use the same vertex numbering and descriptions as the
original simplices from *source*, and any gluings between the
simplices of *source* will likewise be copied across as gluings
between their copies in this triangulation.

If *source* has locks on any top-dimensional simplices and/or their
facets, these locks will also be copied over to this triangulation.

This routine behaves correctly when *source* is this triangulation.

Parameter ``source``:
    the triangulation whose copy will be inserted.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isConnected
constexpr const char *isConnected =
R"doc(Determines if this triangulation is connected.

This routine returns ``False`` only if there is more than one
connected component. In particular, it returns ``True`` for the empty
triangulation.

Returns:
    ``True`` if and only if this triangulation is connected.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isContainedIn
constexpr const char *isContainedIn =
R"doc(Determines if an isomorphic copy of this triangulation is contained
within the given triangulation, possibly as a subcomplex of some
larger component (or components).

Specifically, this routine determines if there is a boundary
incomplete combinatorial isomorphism from this triangulation to
*other*. Boundary incomplete isomorphisms are described in detail in
the Isomorphism class notes.

In particular, note that facets of top-dimensional simplices that lie
on the boundary of this triangulation need not correspond to boundary
facets of *other*, and that *other* may contain more top-dimensional
simplices than this triangulation.

If a boundary incomplete isomorphism is found, the details of this
isomorphism are returned. Thus, to test whether an isomorphism exists,
you can just call ``if (isContainedIn(other))``.

If more than one such isomorphism exists, only one will be returned.
For a routine that returns all such isomorphisms, see
findAllSubcomplexesIn().

.. warning::
    For large dimensions, this routine can become extremely slow: its
    running time includes a factor of (*dim*+1)!.

Parameter ``other``:
    the triangulation in which to search for an isomorphic copy of
    this triangulation.

Returns:
    details of the isomorphism if such a copy is found, or ``None``
    otherwise.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isEmpty
constexpr const char *isEmpty =
R"doc(Determines whether this triangulation is empty. An empty triangulation
is one with no simplices at all.

Returns:
    ``True`` if and only if this triangulation is empty.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isIsomorphicTo
constexpr const char *isIsomorphicTo =
R"doc(Determines if this triangulation is combinatorially isomorphic to the
given triangulation.

Two triangulations are _isomorphic_ if and only it is possible to
relabel their top-dimensional simplices and the (*dim*+1) vertices of
each simplex in a way that makes the two triangulations
combinatorially identical, as returned by isIdenticalTo().

Equivalently, two triangulations are isomorphic if and only if there
is a one-to-one and onto boundary complete combinatorial isomorphism
from this triangulation to *other*, as described in the Isomorphism
class notes.

In particular, note that this triangulation and *other* must contain
the same number of top-dimensional simplices for such an isomorphism
to exist.

If the triangulations are isomorphic, then this routine returns one
such boundary complete isomorphism (i.e., one such relabelling).
Otherwise it returns ``None``. Thus, to test whether an isomorphism
exists, you can just call ``if (isIsomorphicTo(other))``.

There may be many such isomorphisms between the two triangulations. If
you need to find _all_ such isomorphisms, you may call
findAllIsomorphisms() instead.

If you need to ensure that top-dimensional simplices are labelled the
same in both triangulations (i.e., that the triangulations are related
by the _identity_ isomorphism), you should call the stricter test
isIdenticalTo() instead.

.. warning::
    For large dimensions, this routine can become extremely slow: its
    running time includes a factor of (*dim*+1)!.

Parameter ``other``:
    the triangulation to compare with this one.

Returns:
    details of the isomorphism if the two triangulations are
    combinatorially isomorphic, or ``None`` otherwise.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isOrientable
constexpr const char *isOrientable =
R"doc(Determines if this triangulation is orientable.

Returns:
    ``True`` if and only if this triangulation is orientable.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isOriented
constexpr const char *isOriented =
R"doc(Determines if this triangulation is oriented; that is, if the vertices
of its top-dimensional simplices are labelled in a way that preserves
orientation across adjacent facets. Specifically, this routine returns
``True`` if and only if every gluing permutation has negative sign.

Note that _orientable_ triangulations are not always _oriented_ by
default. You can call orient() if you need the top-dimensional
simplices to be oriented consistently as described above.

A non-orientable triangulation can never be oriented.

Returns:
    ``True`` if and only if all top-dimensional simplices are oriented
    consistently.

Author:
    Matthias Goerner)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isValid
constexpr const char *isValid =
R"doc(Determines if this triangulation is valid.

There are several conditions that might make a *dim*-dimensional
triangulation invalid:

1. if some face is identified with itself under a non-identity
   permutation (e.g., an edge is identified with itself in reverse, or
   a triangle is identified with itself under a rotation);

2. if some *subdim*-face does not have an appropriate link. Here the
   meaning of "appropriate" depends upon the type of face:

   * for a face that belongs to some boundary facet(s) of this
     triangulation, its link must be a topological ball;

   * for a vertex that does not belong to any boundary facets, its
     link must be a closed (*dim* - 1)-manifold;

   * for a (*subdim* ≥ 1)-face that does not belong to any boundary
     facets, its link must be a topological sphere.

Condition (1) is tested for all dimensions *dim*. Condition (2) is
more difficult, since it relies on undecidable problems. As a result,
(2) is _only_ tested when *dim* is one of Regina's standard
dimensions.

If a triangulation is invalid then you can call Face<dim,
subdim>::isValid() to discover exactly which face(s) are responsible,
and you can call Face<dim, subdim>::hasBadIdentification() and/or
Face<dim, subdim>::hasBadLink() to discover exactly which conditions
fail.

Note that all invalid vertices are considered to be on the boundary;
see isBoundary() for details.

Returns:
    ``True`` if and only if this triangulation is valid.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isoSig
constexpr const char *isoSig =
R"doc(Constructs the isomorphism signature of the given type for this
triangulation. Support for different _types_ of signature is new to
Regina 7.0 (see below for details); all isomorphism signatures created
in Regina 6.0.1 or earlier are of the default type IsoSigClassic.

An _isomorphism signature_ is a compact representation of a
triangulation that uniquely determines the triangulation up to
combinatorial isomorphism. That is, for any fixed signature type *T*,
two triangulations of dimension *dim* are combinatorially isomorphic
if and only if their isomorphism signatures of type *T* are the same.

The length of an isomorphism signature is proportional to ``n log n``,
where *n* is the number of top-dimenisonal simplices. The time
required to construct it is worst-case ``O((dim!) n² log² n)``. Whilst
this is fine for large triangulations, it becomes very slow for large
_dimensions_; the main reason for introducing different signature
types is that some alternative types can be much faster to compute in
practice.

Whilst the format of an isomorphism signature bears some similarity to
dehydration strings for 3-manifolds, they are more general:
isomorphism signatures can be used with any triangulations, including
closed, bounded and/or disconnected triangulations, as well as
triangulations with many simplices. Note also that 3-manifold
dehydration strings are not unique up to isomorphism (they depend on
the particular labelling of tetrahedra).

The routine fromIsoSig() can be used to recover a triangulation from
an isomorphism signature (only if the default encoding has been used,
but it does not matter which signature type was used). The
triangulation recovered might not be identical to the original, but it
_will_ be combinatorially isomorphic. If you need the precise
relabelling, you can call isoSigDetail() instead.

Regina supports several different variants of isomorphism signatures,
which are tailored to different computational needs; these are
currently determined by the template parameters *Type* and *Encoding:*

* The *Type* parameter identifies which signature type is to be
  constructed. Essentially, different signature types use different
  rules to determine which labelling of a triangulation is
  "canonical". The default type IsoSigClassic is slow (it never does
  better than the worst-case time described above); its main advantage
  is that it is consistent with the original implementation of
  isomorphism signatures in Regina 4.90.

* The *Encoding* parameter controls how Regina encodes a canonical
  labelling into a final signature. The default encoding
  IsoSigPrintable returns a std::string consisting entirely of
  printable characters in the 7-bit ASCII range. Importantly, this
  default encoding is currently the only encoding from which Regina
  can _reconstruct_ a triangulation from its isomorphism signature.

You may instead pass your own type and/or encoding parameters as
template arguments. Currently this facility is for internal use only,
and the requirements for type and encoding parameters may change in
future versions of Regina. At present:

* The *Type* parameter should be a class that is constructible from a
  componenent reference, and that offers the member functions
  simplex(), perm() and next(); see the implementation of
  IsoSigClassic for details.

* The *Encoding* parameter should be a class that offers a *Signature*
  type alias, and static functions emptySig() and encode(). See the
  implementation of IsoSigPrintable for details.

For a full and precise description of the classic isomorphism
signature format for 3-manifold triangulations, see _Simplification
paths in the Pachner graphs of closed orientable 3-manifold
triangulations_, Burton, 2011, ``arXiv:1110.6080``. The format for
other dimensions is essentially the same, but with minor dimension-
specific adjustments.

Python:
    Although this is a templated function, all of the variants
    supplied with Regina are available to Python users. For the
    default type and encoding, you can simply call isoSig(). For other
    signature types, you should call the function as isoSig_*Type*,
    where the suffix *Type* is an abbreviated version of the *Type*
    template parameter; one such example is ``isoSig_EdgeDegrees``
    (for the case where *Type* is the class IsoSigEdgeDegrees).
    Currently Regina only offers one encoding (the default), and so
    there are no suffixes for encodings.

.. warning::
    Do not mix isomorphism signatures between dimensions! It is
    possible that the same string could corresponding to both a
    *p*-dimensional triangulation and a *q*-dimensional triangulation
    for different dimensions *p* and *q*.

Returns:
    the isomorphism signature of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isoSigComponentSize
constexpr const char *isoSigComponentSize =
R"doc(Deduces the number of top-dimensional simplices in a connected
triangulation from its isomorphism signature.

See isoSig() for more information on isomorphism signatures. It will
be assumed that the signature describes a triangulation of dimension
*dim*.

Currently this routine only supports isomorphism signatures that were
created with the default encoding (i.e., there was no *Encoding*
template parameter passed to isoSig()).

If the signature describes a connected triangulation, this routine
will simply return the size of that triangulation (e.g., the number of
tetrahedra in the case *dim* = 3). You can also pass an isomorphism
signature that describes a disconnected triangulation; however, this
routine will only return the number of top-dimensional simplices in
the first connected component. If you need the total size of a
disconnected triangulation, you will need to reconstruct the full
triangulation by calling fromIsoSig() instead.

This routine is very fast, since it only examines the first few
characters of the isomorphism signature (in which the size of the
first component is encoded). However, a side-effect of this is that it
is possible to pass an _invalid_ isomorphism signature and still
receive a positive result. If you need to test whether a signature is
valid or not, you must call fromIsoSig() instead, which will examine
the entire signature in full.

.. warning::
    Do not mix isomorphism signatures between dimensions! It is
    possible that the same string could corresponding to both a
    *p*-dimensional triangulation and a *q*-dimensional triangulation
    for different dimensions *p* and *q*.

Parameter ``sig``:
    the isomorphism signature of a *dim*-dimensional triangulation.
    Note that isomorphism signature are case-sensitive (unlike, for
    example, dehydration strings for 3-manifolds).

Returns:
    the number of top-dimensional simplices in the first connected
    component, or 0 if this could not be determined because the given
    string was not a valid isomorphism signature created using the
    default encoding.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::isoSigDetail
constexpr const char *isoSigDetail =
R"doc(Constructs the isomorphism signature for this triangulation, along
with the relabelling that will occur when the triangulation is
reconstructed from it.

Essentially, an isomorphism signature is a compact representation of a
triangulation that uniquely determines the triangulation up to
combinatorial isomorphism. See isoSig() for much more detail on
isomorphism signatures as well as the support for different signature
types and encodings.

As described in the isoSig() notes, you can call fromIsoSig() to
recover a triangulation from an isomorphism signature (assuming the
default encoding was used). Whilst the triangulation that is recovered
will be combinatorially isomorphic to the original, it might not be
identical. This routine returns not only the isomorphism signature,
but also an isomorphism that describes the precise relationship
between this triangulation and the reconstruction from fromIsoSig().

Specifically, if this routine returns the pair (*sig*, *relabelling*),
this means that the triangulation reconstructed from
``fromIsoSig(sig)`` will be identical to ``relabelling(this)``.

Python:
    Although this is a templated function, all of the variants
    supplied with Regina are available to Python users. For the
    default type and encoding, you can simply call isoSigDetail(). For
    other signature types, you should call the function as
    isoSigDetail_*Type*, where the suffix *Type* is an abbreviated
    version of the *Type* template parameter; one such example is
    ``isoSigDetail_EdgeDegrees`` (for the case where *Type* is the
    class IsoSigEdgeDegrees). Currently Regina only offers one
    encoding (the default), and so there are no suffixes for
    encodings.

Precondition:
    This triangulation must be non-empty and connected. The facility
    to return a relabelling for disconnected triangulations may be
    added to Regina in a later release.

Exception ``FailedPrecondition``:
    This triangulation is either empty or disconnected.

Returns:
    a pair containing (i) the isomorphism signature of this
    triangulation, and (ii) the isomorphism between this triangulation
    and the triangulation that would be reconstructed from
    fromIsoSig().)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::lockBoundary
constexpr const char *lockBoundary =
R"doc(Locks all boundary facets of this triangulation.

In short, this means that the boundary facets must not be changed. See
Simplex<dim>::lockFacet() for full details on how locks work and what
their implications are.

If there are any other locks on top-dimensional simplices and/or their
facets, these other locks will be left intact.

Note that this only locks the facets of real boundary components.
Ideal boundary components are not affected (since they have no facets
to lock).)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::makeCanonical
constexpr const char *makeCanonical =
R"doc(Relabel the top-dimensional simplices and their vertices so that this
triangulation is in canonical form. This is essentially the
lexicographically smallest labelling when the facet gluings are
written out in order.

Two triangulations are isomorphic if and only if their canonical forms
are identical.

The lexicographic ordering assumes that the facet gluings are written
in order of simplex index and then facet number. Each gluing is
written as the destination simplex index followed by the gluing
permutation (which in turn is written as the images of 0,1,...,*dim*
in order).

Precondition:
    This routine currently works only when the triangulation is
    connected. It may be extended to work with disconnected
    triangulations in later versions of Regina.

Returns:
    ``True`` if the triangulation was changed, or ``False`` if the
    triangulation was in canonical form to begin with.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::makeDoubleCover
constexpr const char *makeDoubleCover =
R"doc(Deprecated routine that converts this triangulation into its
orientable double cover. This triangulation wll be modified directly.

.. deprecated::
    This routine has been replaced by doubleCover(), which returns the
    result as a new triangulation and leaves the original
    triangulation untouched.

See doubleCover() for further details.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::markedHomology
constexpr const char *markedHomology =
R"doc(Returns the *k*th homology group of this triangulation, without
truncating ideal vertices, but with explicit coordinates that track
the individual *k*-faces of this triangulation, where the parameter
*k* does not need to be known until runtime.

For C++ programmers who know *k* at compile time, you are better off
using the template function markedHomology<k>() instead, which is
slightly faster.

This is a specialised homology routine; you should only use it if you
need to understand how individual *k*-faces (or chains of *k*-faces)
appear within the homology group.

* The major disadvantage of this routine is that it does not truncate
  ideal vertices. Instead it computes the homology of the union of all
  top-dimensional simplices, working directly with the boundary maps
  between (*k*+1)-faces, *k*-faces and (*k*-1)-faces of the
  triangulation. If your triangulation is ideal, then this routine
  will almost certainly _not_ give the correct homology group for the
  underlying manifold. If, however, all of your vertex links are
  spheres or balls (i.e., the triangulation is closed or all of its
  boundary components are built from unglued (*dim*-1)-faces), then
  the homology of the manifold will be computed correctly.

* The major advantage is that, instead of returning a simpler
  AbelianGroup, this routine returns a MarkedAbelianGroup. This allows
  you to track chains of individual *k*-faces of the triangulation as
  they appear within the homology group. Specifically, the chain
  complex cordinates with this MarkedAbelianGroup represent precisely
  the *k*-faces of the triangulation in the same order as they appear
  in the list faces<k>(), using the inherent orientation provided by
  Face<dim, k>.

Precondition:
    This triangulation is valid and non-empty.

Exception ``FailedPrecondition``:
    This triangulation is empty or invalid.

Exception ``InvalidArgument``:
    The homology dimension *k* is outside the supported range (i.e.,
    less than 1 or greater than or equal to *dim*).

Python:
    Like the C++ template function markedHomology<k>(), you can omit
    the homology dimension *k*; this will default to 1.

Parameter ``k``:
    the dimension of the homology group to compute; this must be
    between 1 and (*dim*-1) inclusive.

Returns:
    the *k*th homology group of the union of all simplices in this
    triangulation, as described above.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::moveContentsTo
constexpr const char *moveContentsTo =
R"doc(Moves the contents of this triangulation into the given destination
triangulation, without destroying any pre-existing contents.

All top-dimensional simplices that currently belong to *dest* will
remain there (and will keep the same indices in *dest*). All top-
dimensional simplices that belong to this triangulation will be moved
into *dest* also (but in general their indices will change).

This triangulation will become empty as a result.

Any pointers or references to Simplex<dim> objects will remain valid,
and any locks on top-dimensional simplices and/or their facets will be
preserved.

If your intention is to _replace_ the simplices in *dest* (i.e., you
do not need to preserve the original contents), then consider using
the move assignment operator instead (which is more streamlined and
also moves across any cached properties from the source
triangulation).

Precondition:
    *dest* is not this triangulation.

Parameter ``dest``:
    the triangulation into which simplices should be moved.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::newSimplex
constexpr const char *newSimplex =
R"doc(Creates a new top-dimensional simplex and adds it to this
triangulation.

The new simplex will have an empty description. All (*dim*+1) facets
of the new simplex will be boundary facets.

The new simplex will become the last simplex in this triangulation;
that is, it will have index size()-1.

Returns:
    the new simplex.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::newSimplex_2
constexpr const char *newSimplex_2 =
R"doc(Creates a new top-dimensional simplex with the given description and
adds it to this triangulation.

All (*dim*+1) facets of the new simplex will be boundary facets.

Descriptions are optional, may have any format, and may be empty. How
descriptions are used is entirely up to the user.

The new simplex will become the last simplex in this triangulation;
that is, it will have index size()-1.

Parameter ``desc``:
    the description to give to the new simplex.

Returns:
    the new simplex.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::newSimplices
constexpr const char *newSimplices =
R"doc(Creates *k* new top-dimensional simplices, adds them to this
triangulation, and returns them in a std::array.

The main purpose of this routine is to support structured binding; for
example:

```
auto [r, s, t] = ans.newSimplices<3>();
r->join(0, s, {1, 2, 3, 0});
...
```

All new simplices will have empty descriptions, and all facets of each
new simplex will be boundary facets.

The new simplices will become the last *k* simplices in this
triangulation. Specifically, if the return value is the array *ret*,
then each simplex ``ret[i]`` will have index ``size()-k+i`` in the
overall triangulation.

Python:
    For Python users, the two variants of newSimplices() are
    essentially merged: the argument *k* is passed as an ordinary
    runtime argument, and the new top-dimensional simplices will be
    returned in a Python tuple of size *k*.

Template parameter ``k``:
    the number of new top-dimensional simplices to add; this must be
    non-negative.

Returns:
    an array containing all of the new simplices, in the order in
    which they were added.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::newSimplices_2
constexpr const char *newSimplices_2 =
R"doc(Creates *k* new top-dimensional simplices and adds them to this
triangulation.

This is similar to the templated routine ``newSimplices<k>()``, but
with two key differences:

* This routine has the disadvantage that it does not return the new
  top-dimensional simplices, which means you cannot use it with
  structured binding.

* This routine has the advantage that *k* does not need to be known
  until runtime, which means this routine is accessible to Python
  users.

All new simplices will have empty descriptions, and all facets of each
new simplex will be boundary facets.

The new simplices will become the last *k* simplices in this
triangulation.

Python:
    For Python users, the two variants of newSimplices() are
    essentially merged: the argument *k* is passed as an ordinary
    runtime argument, and the new top-dimensional simplices will be
    returned in a Python tuple of size *k*.

Parameter ``k``:
    the number of new top-dimensional simplices to add; this must be
    non-negative.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::orient
constexpr const char *orient =
R"doc(Relabels the vertices of top-dimensional simplices in this
triangulation so that all simplices are oriented consistently, if
possible.

This routine works by flipping vertices (*dim* - 1) and *dim* of each
top-dimensional simplex that has negative orientation. The result will
be a triangulation where the top-dimensional simplices have their
vertices labelled in a way that preserves orientation across adjacent
facets. In particular, every gluing permutation will have negative
sign.

If this triangulation includes both orientable and non-orientable
components, the orientable components will be oriented as described
above and the non-orientable components will be left untouched.

If this triangulation has locks on any top-dimensional simplices
and/or their facets, these will not prevent the orientation from
taking place. Instead, any locks will be transformed accordingly
(i.e., facets (*dim* - 1) and *dim* will exchange their lock states
for those simplices that originally had negative orientation).)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::pachner
constexpr const char *pachner =
R"doc(Checks the eligibility of and/or performs a (*dim* + 1 - *k*)-(*k* +
1) Pachner move about the given *k*-face. This involves replacing the
(*dim* + 1 - *k*) top-dimensional simplices meeting that *k*-face with
(*k* + 1) new top-dimensional simplices joined along a new internal
(*dim* - *k*)-face. This can be done iff (i) the given *k*-face is
valid and non-boundary; (ii) the (*dim* + 1 - *k*) top-dimensional
simplices that contain it are distinct; and (iii) these simplices are
joined in such a way that the link of the given *k*-face is the
standard triangulation of the (*dim* - 1 - *k*)-sphere as the boundary
of a (*dim* - *k*)-simplex.

If the routine is asked to both check and perform, the move will only
be performed if the check shows it is legal and will not violate any
simplex and/or facet locks (see Simplex<dim>::lock() and
Simplex<dim>::lockFacet() for further details on locks).

Note that after performing this move, all skeletal objects (facets,
components, etc.) will be reconstructed, which means any pointers to
old skeletal objects (such as the argument *v*) can no longer be used.

If this triangulation is currently oriented, then this Pachner move
will label the new top-dimensional simplices in a way that preserves
the orientation.

See the page on Pachner moves on triangulations for definitions and
terminology relating to Pachner moves. After the move, the new belt
face will be formed from vertices 0,1,...,(*dim* - *k*) of
``simplices().back()``.

.. warning::
    For the case *k* = *dim* in Regina's standard dimensions, the
    labelling of the belt face has changed as of Regina 5.96 (the
    first prerelease for Regina 6.0). In versions 5.1 and earlier, the
    belt face was ``simplices().back()->vertex(dim)``, and as of
    version 5.96 it is now ``simplices().back()->vertex(0)``.

Precondition:
    If the move is being performed and no check is being run, it must
    be known in advance that the move is legal and will not violate
    any simplex and/or facet locks.

Precondition:
    The given *k*-face is a *k*-face of this triangulation.

Exception ``LockViolation``:
    This move would violate a simplex or facet lock, and *check* was
    passed as ``False``. This exception will be thrown before any
    changes are made. See Simplex<dim>::lock() and
    Simplex<dim>::lockFacet() for further details on how locks work
    and what their implications are.

Parameter ``f``:
    the *k*-face about which to perform the move.

Parameter ``check``:
    ``True`` if we are to check whether the move is allowed (defaults
    to ``True``).

Parameter ``perform``:
    ``True`` if we are to perform the move (defaults to ``True``).

Returns:
    If *check* is ``True``, the function returns ``True`` if and only
    if the requested move may be performed without changing the
    topology of the manifold or violating any locks. If *check* is
    ``False``, the function simply returns ``True``.

Template parameter ``k``:
    the dimension of the given face. This must be between 0 and
    (*dim*) inclusive.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::pairing
constexpr const char *pairing =
R"doc(Returns the dual graph of this triangulation, expressed as a facet
pairing.

Calling ``tri.pairing()`` is equivalent to calling
``FacetPairing<dim>(tri)``.

Precondition:
    This triangulation is not empty.

Returns:
    the dual graph of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::pentachora
constexpr const char *pentachora =
R"doc(A dimension-specific alias for faces<4>(), or an alias for simplices()
in dimension *dim* = 4.

This alias is available for dimensions *dim* ≥ 4.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::pentachoron
constexpr const char *pentachoron =
R"doc(A dimension-specific alias for face<4>(), or an alias for simplex() in
dimension *dim* = 4.

This alias is available for dimensions *dim* ≥ 4. It returns a non-
const pentachoron pointer.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::pentachoron_2
constexpr const char *pentachoron_2 =
R"doc(A dimension-specific alias for face<4>(), or an alias for simplex() in
dimension *dim* = 4.

This alias is available for dimensions *dim* ≥ 4. It returns a const
pentachoron pointer in dimension *dim* = 4, and a non-const
pentachoron pointer in all higher dimensions.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::randomiseLabelling
constexpr const char *randomiseLabelling =
R"doc(Randomly relabels the top-dimensional simplices and their vertices.

Essentially, this routine creates a random isomorphism of the correct
size and applies it in-place to this triangulation.

The advantage of using this routine instead of working directly
through the Isomorphism class is that this routine preserves any
computed topological properties of the triangulation (as opposed to
the isomorphism bracket operator, which at the time of writing does
not).

Parameter ``preserveOrientation``:
    if ``True``, then every top-dimensional simplex will have its
    vertices permuted with an even permutation. This means that, if
    this triangulation is oriented, then randomiseLabelling() will
    preserve the orientation.

Returns:
    the random isomorphism that was applied; that is, the isomorphism
    from the original triangulation to the final triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::reflect
constexpr const char *reflect =
R"doc(Relabels the vertices of top-dimensional simplices in this
triangulation so that all simplices reflect their orientation. In
particular, if this triangulation is oriented, then it will be
converted into an isomorphic triangulation with the opposite
orientation.

This routine works by flipping vertices (*dim* - 1) and *dim* of every
top-dimensional simplex.

If this triangulation has locks on any top-dimensional simplices
and/or their facets, these will not prevent the reflection from taking
place. Instead, any locks will be transformed accordingly (i.e.,
facets (*dim* - 1) and *dim* will exchange their lock states in every
top-dimensional simplex).)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::removeAllSimplices
constexpr const char *removeAllSimplices =
R"doc(Removes all simplices from the triangulation. As a result, this
triangulation will become empty.

All of the simplices that belong to this triangulation will be
destroyed immediately.

Exception ``LockViolation``:
    This triangulation contains at least one locked top-dimensional
    simplex and/or facet. This exception will be thrown before any
    changes are made. See Simplex<dim>::lock() and
    Simplex<dim>::lockFacet() for further details on how such locks
    work and what their implications are.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::removeSimplex
constexpr const char *removeSimplex =
R"doc(Removes the given top-dimensional simplex from this triangulation.

The given simplex will be unglued from any adjacent simplices (if
any), and will be destroyed immediately.

Precondition:
    The given simplex is a top-dimensional simplex in this
    triangulation.

Exception ``LockViolation``:
    The given simplex and/or one of its facets is currently locked.
    This exception will be thrown before any changes are made. See
    Simplex<dim>::lock() and Simplex<dim>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``simplex``:
    the simplex to remove.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::removeSimplexAt
constexpr const char *removeSimplexAt =
R"doc(Removes the top-dimensional simplex at the given index in this
triangulation.

This is equivalent to calling ``removeSimplex(simplex(index))``.

The given simplex will be unglued from any adjacent simplices (if
any), and will be destroyed immediately.

Exception ``LockViolation``:
    The requested simplex and/or one of its facets is currently
    locked. This exception will be thrown before any changes are made.
    See Simplex<dim>::lock() and Simplex<dim>::lockFacet() for further
    details on how such locks work and what their implications are.

Parameter ``index``:
    specifies which top-dimensional simplex to remove; this must be
    between 0 and size()-1 inclusive.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::reorderBFS
constexpr const char *reorderBFS =
R"doc(Reorders the top-dimensional simplices of this triangulation using a
breadth-first search, so that small-numbered simplices are adjacent to
other small-numbered simplices.

Specifically, the reordering will operate as follows. Simplex 0 will
remain simplex 0. Its immediate neighbours will be numbered
1,2,...,(*dim*+1) (though if these neighbours are not distinct then of
course fewer labels will be required). Their immediate neighbours will
in turn be numbered (*dim*+2), (*dim*+3) and so on, ultimately
following a breadth-first search throughout the entire triangulation.

If the optional argument *reverse* is ``True``, then simplex numbers
will be assigned in reverse order. That is, simplex 0 will become
simplex *n*-1, its immediate neighbours will become simplices *n*-2,
*n*-3, etc., and so on.

If this triangulation has locks on any top-dimensional simplices
and/or their facets, these will not prevent the reordering from taking
place. Instead, any locks will be transformed accordingly; that is,
all top-dimensional simplices will carry their own locks and their
facets' locks around with them as they are reordered.

Parameter ``reverse``:
    ``True`` if the new simplex numbers should be assigned in reverse
    order, as described above.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::setGroupPresentation
constexpr const char *setGroupPresentation =
R"doc(Allows the specific presentation of the fundamental group to be
changed by some other (external) means.

Specifically, this routine assumes that you have changed (and
presumably simplified) the presentation of the fundamental group using
some external tool (such as GAP or Magma), and it replaces the current
presentation with the new presentation *pres* that is passed.

Regina does _not_ verify that the new presentation is isomorphic to
the old, since this is an extremely hard problem.

If the fundamental group has not yet been calculated for this
triangulation, then this routine will cache *pres* as the fundamental
group, under the assumption that you have worked out the group through
some other clever means without ever having needed to call group() at
all.

Note that this routine will not fire a packet change event.

Precondition:
    The given presentation *pres* is indeed a presentation of the
    fundamental group of this triangulation, as described by group().

Parameter ``pres``:
    a new presentation of the fundamental group of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::sig
constexpr const char *sig =
R"doc(Alias for isoSig(), which constructs the isomorphism signature of the
given type for this triangulation.

This alias sig() is provided to assist with generic code that can work
with both triangulations and links.

See isoSig() for further details.

Python:
    This alias is only available for the default signature type and
    encoding (i.e., the default C++ template arguments). If you wish
    to use a different signature type and/or encoding, you can instead
    use the variants provided with isoSig(); that is, you can call a
    function of the form isoSig_*Type*. See the isoSig() documentation
    for further details.

Returns:
    the isomorphism signature of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::simplex
constexpr const char *simplex =
R"doc(Returns the top-dimensional simplex at the given index in the
triangulation.

Note that indexing may change when a simplex is added to or removed
from the triangulation.

Parameter ``index``:
    specifies which simplex to return; this value should be between 0
    and size()-1 inclusive.

Returns:
    the *index*th top-dimensional simplex.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::simplex_2
constexpr const char *simplex_2 =
R"doc(Returns the top-dimensional simplex at the given index in the
triangulation.

Note that indexing may change when a simplex is added to or removed
from the triangulation.

Parameter ``index``:
    specifies which simplex to return; this value should be between 0
    and size()-1 inclusive.

Returns:
    the *index*th top-dimensional simplex.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::simplices
constexpr const char *simplices =
R"doc(Returns an object that allows iteration through and random access to
all top-dimensional simplices in this triangulation.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Simplex<dim>* s : tri.simplices()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the triangulation exists: even as simplices are added and/or
removed, it will always reflect the simplices that are currently in
the triangulation. Nevertheless, it is recommended to treat this
object as temporary only, and to call simplices() again each time you
need it.

Returns:
    access to the list of all top-dimensional simplices.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::simplifiedFundamentalGroup
constexpr const char *simplifiedFundamentalGroup =
R"doc(Deprecated alias for setGroupPresentation(), which allows the specific
presentation of the fundamental group to be changed by some other
(external) means.

.. deprecated::
    This routine has been renamed to setGroupPresentation().

Precondition:
    The given presentation *pres* is indeed a presentation of the
    fundamental group of this triangulation, as described by group().

Parameter ``pres``:
    a new presentation of the fundamental group of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::size
constexpr const char *size =
R"doc(Returns the number of top-dimensional simplices in the triangulation.

Returns:
    The number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::source
constexpr const char *source =
R"doc(Returns C++ or Python source code that can be used to reconstruct this
triangulation.

This code will call Triangulation<dim>::fromGluings(), passing a hard-
coded C++ initialiser list or Python list (depending on the requested
language).

The main purpose of this routine is to generate this hard-coded list,
which can be tedious and error-prone to write by hand.

Note that the number of lines of code produced grows linearly with the
number of simplices. If this triangulation is very large, the returned
string will be very large as well.

Parameter ``language``:
    the language in which the source code should be written.

Returns:
    the source code that was generated.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::subdivide
constexpr const char *subdivide =
R"doc(Does a barycentric subdivision of the triangulation. This is done in-
place, i.e., the triangulation will be modified directly.

Each top-dimensional simplex *s* is divided into (*dim* + 1) factorial
sub-simplices by placing an extra vertex at the centroid of every face
of every dimension. Each of these sub-simplices *t* is described by a
permutation *p* of (0, ..., *dim*). The vertices of such a sub-simplex
*t* are:

* vertex *p*[0] of *s*;

* the centre of edge (*p*[0], *p*[1]) of *s*;

* the centroid of triangle (*p*[0], *p*[1], *p*[2]) of *s*;

* ...

* the centroid of face (*p*[0], *p*[1], *p*[2], *p*[*dim*]) of *s*,
  which is the entire simplex *s* itself.

The sub-simplices have their vertices numbered in a way that mirrors
the original simplex *s:*

* vertex *p*[0] of *s* will be labelled *p*[0] in *t*;

* the centre of edge (*p*[0], *p*[1]) of *s* will be labelled *p*[1]
  in *t*;

* the centroid of triangle (*p*[0], *p*[1], *p*[2]) of *s* will be
  labelled *p*[2] in *t*;

* ...

* the centroid of *s* itself will be labelled *p*[*dim*] in *t*.

In particular, if this triangulation is currently oriented, then this
barycentric subdivision will preserve the orientation.

If simplex *s* has index *i* in the original triangulation, then its
sub-simplex corresponding to permutation *p* will have index ``((dim +
1)! * i + p.orderedSnIndex())`` in the resulting triangulation. In
other words: sub-simplices are ordered first according to the original
simplex that contains them, and then according to the lexicographical
ordering of the corresponding permutations *p*.

Precondition:
    *dim* is one of Regina's standard dimensions. This precondition is
    a safety net, since in higher dimensions the triangulation would
    explode too quickly in size (and for the highest dimensions,
    possibly beyond the limits of ``size_t``).

.. warning::
    In dimensions 3 and 4, both the labelling and ordering of sub-
    simplices in the subdivided triangulation has changed as of Regina
    5.1. (Earlier versions of Regina made no guarantee about the
    labelling and ordering; these guarantees are also new to Regina
    5.1).

Exception ``LockViolation``:
    This triangulation contains at least one locked top-dimensional
    simplex and/or facet. This exception will be thrown before any
    changes are made. See Simplex<dim>::lock() and
    Simplex<dim>::lockFacet() for further details on how such locks
    work and what their implications are.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::tetrahedra
constexpr const char *tetrahedra =
R"doc(A dimension-specific alias for faces<3>(), or an alias for simplices()
in dimension *dim* = 3.

This alias is available for dimensions *dim* ≥ 3.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::tetrahedron
constexpr const char *tetrahedron =
R"doc(A dimension-specific alias for face<3>(), or an alias for simplex() in
dimension *dim* = 3.

This alias is available for dimensions *dim* ≥ 3. It returns a non-
const tetrahedron pointer.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::tetrahedron_2
constexpr const char *tetrahedron_2 =
R"doc(A dimension-specific alias for face<3>(), or an alias for simplex() in
dimension *dim* = 3.

This alias is available for dimensions *dim* ≥ 3. It returns a const
tetrahedron pointer in dimension *dim* = 3, and a non-const
tetrahedron pointer in all higher dimensions.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::translate
constexpr const char *translate =
R"doc(Translates a face of some other triangulation into the corresponding
face of this triangulation, using simplex numbers for the translation.

Typically this routine would be used when the given face comes from a
triangulation that is combinatorially identical to this, and you wish
to obtain the corresponding face of this triangulation.

Specifically: if *other* refers to face *i* of top-dimensional simplex
number *k* of some other triangulation, then this routine will return
face *i* of top-dimensional simplex number *k* of this triangulation.
Note that this routine does _not_ use the face indices within each
triangulation (which is outside the user's control), but rather the
simplex numbering (which the user has full control over).

This routine behaves correctly even if *other* is a null pointer.

Precondition:
    This triangulation contains at least as many top-dimensional
    simplices as the triangulation containing *other* (though, as
    noted above, in typical scenarios both triangulations would
    actually be combinatorially identical).

Template parameter ``subdim``:
    the face dimension; this must be between 0 and *dim*-1 inclusive.

Parameter ``other``:
    the face to translate.

Returns:
    the corresponding face of this triangulation.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::triangle
constexpr const char *triangle =
R"doc(A dimension-specific alias for face<2>(), or an alias for simplex() in
dimension *dim* = 2.

This alias is available for all dimensions *dim*. It returns a non-
const triangle pointer.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::triangle_2
constexpr const char *triangle_2 =
R"doc(A dimension-specific alias for face<2>(), or an alias for simplex() in
dimension *dim* = 2.

This alias is available for all dimensions *dim*. It returns a const
triangle pointer in dimension *dim* = 2, and a non-const triangle
pointer in all higher dimensions.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::triangles
constexpr const char *triangles =
R"doc(A dimension-specific alias for faces<2>(), or an alias for simplices()
in dimension *dim* = 2.

This alias is available for all dimensions.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::triangulateComponents
constexpr const char *triangulateComponents =
R"doc(Returns the individual connected components of this triangulation.
This triangulation will not be modified.

This function is new to Regina 7.0, and it has two important changes
of behaviour from the old splitIntoComponents() from Regina 6.0.1 and
earlier:

* This function does not insert the resulting components into the
  packet tree.

* This function does not assign labels to the new components.

If this triangulation has locks on any top-dimensional simplices
and/or their facets, these locks will also be copied over to the
newly-triangulated components.

Returns:
    a list of individual component triangulations.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::unlockAll
constexpr const char *unlockAll =
R"doc(Unlocks all top-dimensional simplices and their facets.

In short, locking a top-dimensional simplex and/or some of its facets
means that that the simplex and/or facets must not be changed. See
Simplex<dim>::lock() and Simplex<dim>::lockFacet() for full details on
how locks work and what their implications are.

After this is routine called, hasLocks() will return ``False``.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::vertex
constexpr const char *vertex =
R"doc(A dimension-specific alias for face<0>().

This alias is available for all dimensions *dim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::TriangulationBase_::vertices
constexpr const char *vertices =
R"doc(A dimension-specific alias for faces<0>().

This alias is available for all dimensions *dim*.

See faces() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

