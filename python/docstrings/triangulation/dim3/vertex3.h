/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Face
static const char *Face =
R"doc(Represents a vertex in the skeleton of a 3-manifold triangulation.

This is a specialisation of the generic Face class template; see the
documentation for Face for a general overview of how this class works.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Vertices do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

\headerfile triangulation/dim3.h)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::LinkType
static const char *LinkType =
R"doc(Categorises the possible links of a vertex into a small number of
common types. Here a vertex link is considered only up to its topology
(not the combinatorics of its triangulation).

See also:
    link)doc";

// Docstring regina::python::doc::Face_::buildLink
static const char *buildLink =
R"doc(Returns a full 2-manifold triangulation describing the link of this
vertex.

This routine is fast (it uses a pre-computed triangulation if
possible). The downside is that the triangulation is read-only (though
you can always clone it).

Regarding the labelling of triangles in the vertex link:

* The triangles of the vertex link are numbered as follows. Let *i*
  lie between 0 and degree()-1 inclusive, let *tet* represent
  ``embedding(i).tetrahedron()``, and let *v* represent
  ``embedding(i).vertex()``. Then ``buildLink()->triangle(i)`` is the
  triangle in the vertex link that "slices off" vertex *v* from
  tetrahedron *tet*. In other words, ``buildLink()->triangle(i)`` in
  the vertex link is parallel to triangle ``tet->triangle(v)`` in the
  surrounding 3-manifold triangulation.

* The vertices of each triangle in the vertex link are numbered as
  follows. Following the discussion above, suppose that
  ``buildLink()->triangle(i)`` sits within ``tet`` and is parallel to
  ``tet->triangle(v)``. Then vertices 0,1,2 of the triangle in the
  link will be parallel to vertices 0,1,2 of the corresponding
  Triangle<3>. The permutation ``tet->triangleMapping(v)`` will map
  vertices 0,1,2 of the triangle in the link to the corresponding
  vertices of ``tet`` (those opposite ``v``), and will map 3 to ``v``
  itself.

* If you need this labelling data in a format that is easy to compute
  with, you can call buildLinkInclusion() to retrieve this information
  as an isomorphism.

Python:
    Since Python does not distinguish between const and non-const,
    this routine will return by value (thus making a deep copy of the
    vertex link). You are free to modify the triangulation that is
    returned.

Returns:
    the read-only triangulated link of the vertex.)doc";

// Docstring regina::python::doc::Face_::buildLinkInclusion
static const char *buildLinkInclusion =
R"doc(Returns details of how the triangles are labelled in the link of this
vertex. This is a companion function to buildLink(), which returns a
full 2-manifold triangulation of the vertex link.

The documentation for buildLink() describes in plain English exactly
how the vertex link will be triangulated. This function essentially
returns the same information in a machine-readable form.

Specifically, this function returns an Isomorphism<3> that describes
how the individual triangles of the link sit within the tetrahedra of
the original triangulation. If *p* is the isomorphism returned, then
``p.tetImage(i)`` will indicate which tetrahedron *tet* of the
3-manifold triangulation contains the *i*th triangle of the link.
Moreover, ``p.facePerm(i)`` will indicate exactly where the *i*th
triangle sits within *tet:* it will send 3 to the vertex of *t* that
the triangle links, and it will send 0,1,2 to the vertices of *tet*
that are parallel to vertices 0,1,2 of this triangle.

Strictly speaking, this is an abuse of the Isomorphism<3> class (the
domain is a triangulation of the wrong dimension, and the map is not
1-to-1 into the range tetrahedra). We use it anyway, but you should
not attempt to call any high-level routines (such as
Isomorphism<3>::apply).

This is the same isomorphism that was accessible through the old
buildLinkDetail() function in Regina 6.0.1 and earlier.

Returns:
    details of how buildLink() labels the triangles of the vertex
    link.)doc";

// Docstring regina::python::doc::Face_::isIdeal
static const char *isIdeal =
R"doc(Determines if this vertex is an ideal vertex. This requires the vertex
link to be closed and not a 2-sphere.

Returns:
    ``True`` if and only if this is an ideal vertex.)doc";

// Docstring regina::python::doc::Face_::isLinkClosed
static const char *isLinkClosed =
R"doc(Determines if the link of this vertex is closed.

Returns:
    ``True`` if and only if the link of this vertex is closed.)doc";

// Docstring regina::python::doc::Face_::isStandard
static const char *isStandard =
R"doc(Determines if this vertex is standard. This requires the vertex link
to be a sphere, disc, torus or Klein bottle.

Returns:
    ``True`` if and only if this vertex is standard.)doc";

// Docstring regina::python::doc::Face_::linkEulerChar
static const char *linkEulerChar =
R"doc(Returns the Euler characteristic of the vertex link.

This routine does not require a full triangulation of the vertex link,
and so can be much faster than calling buildLink().eulerChar().

Returns:
    the Euler characteristic of the vertex link.)doc";

// Docstring regina::python::doc::Face_::linkType
static const char *linkType =
R"doc(Returns a broad categorisation of the link of the vertex. This
considers topological information only, not the combinatorics of how
the link is triangulated.

This routine does not require a full triangulation of the vertex link,
and so can be much faster than analysing the result of buildLink().

This is the routine that used to be called link() in Regina 6.0.1 and
earlier. It was renamed to linkType() in Regina 7.0, to clear the way
for a different routine to be called link() in the future.

Returns:
    a broad categorisation of the vertex link.)doc";

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this vertex as a normal surface.

Note that vertex linking surfaces only ever contain triangles (not
quadrilaterals). Moreover, vertex links are always thin (i.e., after
constructing the frontier of a regular neighbourhood of the vertex, no
further normalisation steps are required).

Returns:
    the corresponding vertex linking normal surface.)doc";

}

namespace Face_::LinkType_ {

// Docstring regina::python::doc::Face_::LinkType_::DISC
static const char *DISC =
R"doc(Specifies a vertex link that is a disc. In other words, the vertex
lies on a real boundary component.)doc";

// Docstring regina::python::doc::Face_::LinkType_::INVALID
static const char *INVALID =
R"doc(Specifies a vertex link that has boundary and is not a disc. In other
words, this vertex makes the triangulation invalid.)doc";

// Docstring regina::python::doc::Face_::LinkType_::KLEIN_BOTTLE
static const char *KLEIN_BOTTLE =
R"doc(Specifies a vertex link that is a Klein bottle. In other words, this
is an ideal vertex representing a Klein bottle cusp.)doc";

// Docstring regina::python::doc::Face_::LinkType_::NON_STANDARD_CUSP
static const char *NON_STANDARD_CUSP =
R"doc(Specifies a vertex link that is closed and is not a sphere, torus or
Klein bottle. In other words, this is an ideal vertex but not one of
the standard ideal vertex types.)doc";

// Docstring regina::python::doc::Face_::LinkType_::SPHERE
static const char *SPHERE =
R"doc(Specifies a vertex link that is a sphere. In other words, the vertex
is internal.)doc";

// Docstring regina::python::doc::Face_::LinkType_::TORUS
static const char *TORUS =
R"doc(Specifies a vertex link that is a torus. In other words, this is an
ideal vertex representing a torus cusp.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

