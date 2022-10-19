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
R"doc(Represents a vertex in the skeleton of a 4-manifold triangulation.

This is a specialisation of the generic Face class template; see the
documentation for Face for a general overview of how this class works.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Vertices do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::buildLink
static const char *buildLink =
R"doc(Returns a full 3-manifold triangulation describing the link of this
vertex.

This routine is fast (it uses a pre-computed triangulation). The
downside is that the triangulation is read-only (though you can always
clone it).

Regarding the labelling of tetrahedra in the vertex link:

* The tetrahedra of the vertex link are numbered as follows. Let *i*
  lie between 0 and degree()-1 inclusive, let *pent* represent
  ``embedding(i).pentachoron()``, and let *v* represent
  ``embedding(i).vertex()``. Then ``buildLink()->tetrahedron(i)`` is
  the tetrahedron in the vertex link that "slices off" vertex *v* from
  pentachoron *pent*. In other words, ``buildLink()->tetrahedron(i)``
  in the vertex link is parallel to tetrahedron
  ``pent->tetrahedron(v)`` in the surrounding 4-manifold
  triangulation.

* The vertices of each tetrahedron in the vertex link are numbered as
  follows. Following the discussion above, suppose that
  ``buildLink()->tetrahedron(i)`` sits within ``pent`` and is parallel
  to ``pent->tetrahedron(v)``. Then vertices 0,1,2,3 of the
  tetrahedron in the link will be parallel to vertices 0,1,2,3 of the
  corresponding Tetrahedron<4>. The permutation
  ``pent->tetrahedronMapping(v)`` will map vertices 0,1,2,3 of the
  tetrahedron in the link to the corresponding vertices of ``pent``
  (those opposite ``v``), and will map 4 to ``v`` itself.

* If you need this labelling data in a format that is easy to compute
  with, you can call buildLinkInclusion() to retrieve this information
  as an isomorphism.

Python:
    Since Python does not distinguish between const and non-const,
    this routine will return by value (thus making a deep copy of the
    vertex link. You are free to modify the triangulation that is
    returned.

Returns:
    the read-only triangulated link of this vertex.)doc";

// Docstring regina::python::doc::Face_::buildLinkInclusion
static const char *buildLinkInclusion =
R"doc(Returns details of how the tetrahedra are labelled in the link of this
vertex. This is a companion function to buildLink(), which returns a
full 3-manifold triangulation of the vertex link.

The documentation for buildLink() describes in plain English exactly
how the vertex link will be triangulated. This function essentially
returns the same information in a machine-readable form.

Specifically, this function returns an Isomorphism<4> that describes
how the individual tetrahedra of the link sit within the pentachora of
the original triangulation. If *p* is the isomorphism returned, then
``p.pentImage(i)`` will indicate which pentachoron *pent* of the
4-manifold triangulation contains the *i*th tetrahedron of the link.
Moreover, ``p.facetPerm(i)`` will indicate exactly where the *i*th
tetrahedron sits within *pent:* it will send 4 to the vertex of *pent*
that the tetrahedron links, and it will send 0,1,2,3 to the vertices
of *pent* that are parallel to vertices 0,1,2,3 of this tetrahedron.

Strictly speaking, this is an abuse of the Isomorphism<4> class (the
domain is a triangulation of the wrong dimension, and the map is not
1-to-1 into the range pentachora). We use it anyway, but you should
not attempt to call any high-level routines (such as
Isomorphism<4>::apply).

This is the same isomorphism that was accessible through the old
buildLinkDetail() function in Regina 6.0.1 and earlier.

Returns:
    details of how buildLink() labels the tetrahedra of the vertex
    link.)doc";

// Docstring regina::python::doc::Face_::isIdeal
static const char *isIdeal =
R"doc(Determines if this vertex is an ideal vertex. To be an ideal, a vertex
must (i) be valid, and (ii) have a closed vertex link that is not a
3-sphere.

Returns:
    ``True`` if and only if this is an ideal vertex.)doc";

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this vertex as a normal hypersurface.

Note that vertex linking hypersurfaces only ever contain tetrahedra
(not prisms). Moreover, vertex links are always thin (i.e., after
constructing the frontier of a regular neighbourhood of the vertex, no
further normalisation steps are required).

Returns:
    the corresponding vertex linking normal hypersurface.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

