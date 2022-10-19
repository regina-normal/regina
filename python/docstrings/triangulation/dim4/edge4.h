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
R"doc(Represents an edge in the skeleton of a 4-manifold triangulation.

This is a specialisation of the generic Face class template; see the
documentation for Face for a general overview of how this class works.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Edges do not support value semantics: they cannot be copied, swapped,
or manually constructed. Their location in memory defines them, and
they are often passed and compared by pointer. End users are never
responsible for their memory management; this is all taken care of by
the Triangulation to which they belong.

\headerfile triangulation/dim4.h)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::buildLink
static const char *buildLink =
R"doc(Returns a full 2-manifold triangulation describing the link of this
edge.

This routine is fast (it uses a pre-computed triangulation where
possible). The downside is that the triangulation is read-only (though
you can always clone it).

Regarding the labelling of triangles in the edge link:

* The triangles of the edge link are numbered as follows. Let *i* lie
  between 0 and degree()-1 inclusive, let *pent* represent
  ``embedding(i).pentachoron()``, and let *e* represent
  ``embedding(i).edge()``. Then ``buildLink()->triangle(i)`` is the
  triangle in the edge link that links edge *e* of pentachoron *pent*.
  In other words, ``buildLink()->triangle(i)`` in the edge link is
  parallel to triangle ``pent->triangle(e)`` in the surrounding
  4-manifold triangulation.

* The vertices of each triangle in the edge link are numbered as
  follows. Following the discussion above, suppose that
  ``buildLink()->triangle(i)`` sits within ``pent`` and is parallel to
  ``pent->triangle(e)``. Then vertices 0,1,2 of the triangle in the
  link will be parallel to vertices 0,1,2 of the corresponding
  Triangle<4>. The permutation ``pent->triangleMapping(e)`` will map
  vertices 0,1,2 of the triangle in the link to the corresponding
  vertices of ``pent`` (those opposite ``e``), and will map 3 and 4 to
  the vertices of ``e`` itself.

* If you need this labelling data in a format that is easy to compute
  with, you can call buildLinkInclusion() to retrieve this information
  as an isomorphism.

Python:
    Since Python does not distinguish between const and non-const,
    this routine will return by value (thus making a deep copy of the
    edge link). You are free to modify the triangulation that is
    returned.

Returns:
    the read-only triangulated link of this edge.)doc";

// Docstring regina::python::doc::Face_::buildLinkInclusion
static const char *buildLinkInclusion =
R"doc(Returns details of how the triangles are labelled in the link of this
edge. This is a companion function to buildLink(), which returns a
full 2-manifold triangulation of the edge link.

The documentation for buildLink() describes in plain English exactly
how the edge link will be triangulated. This function essentially
returns the same information in a machine-readable form.

Specifically, this function returns an Isomorphism<4> that describes
how the individual triangles of the link sit within the pentachora of
the original triangulation. If *p* is the isomorphism returned, then
``p.pentImage(i)`` will indicate which pentachoron *pent* of the
4-manifold triangulation contains the *i*th triangle of the link.
Moreover, ``p.facetPerm(i)`` will indicate exactly where the *i*th
triangle sits within *pent:* (i) it will send 3,4 to the vertices of
*pent* that lie on the edge that the triangle links, with 3 and 4
mapping to vertices 0 and 1 respectively of the corresponding Edge<4>;
and (ii) it will send 0,1,2 to the vertices of *pent* that are
parallel to vertices 0,1,2 of this triangle.

Strictly speaking, this is an abuse of the Isomorphism<4> class (the
domain is a triangulation of the wrong dimension, and the map is not
1-to-1 into the range pentachora). We use it anyway, but you should
not attempt to call any high-level routines (such as
Isomorphism<4>::apply).

This is the same isomorphism that was accessible through the old
buildLinkDetail() function in Regina 6.0.1 and earlier.

Returns:
    details of how buildLink() labels the triangles of the edge link.)doc";

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this edge as a normal hypersurface.

Constructing the link of a edge begins with building the frontier of a
regular neighbourhood of the edge. If this is already a normal
hypersurface, then then link is called *thin*. Otherwise some basic
normalisation steps are performed until the hypersurface becomes
normal; note that these normalisation steps could change the topology
of the hypersurface, and in some pathological cases could even reduce
it to the empty hypersurface.

Although normalisation of arbitrary embedded 3-manifolds is messy, for
edge links the process is thankfully simpler. Essentially, any changes
will be limited to operations analagous to compressions and boundary
compressions along discs and 3-balls, as well as removing trivial
4-sphere components.

Returns:
    a pair (*s*, *thin*), where *s* is the edge linking normal
    hypersurface, and *thin* is ``True`` if and only if this link is
    thin (i.e., no additional normalisation steps were required).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

