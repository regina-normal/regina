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
R"doc(Represents an edge in the skeleton of a 3-manifold triangulation.

This is a specialisation of the generic Face class template; see the
documentation for Face for a general overview of how this class works.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Edges do not support value semantics: they cannot be copied, swapped,
or manually constructed. Their location in memory defines them, and
they are often passed and compared by pointer. End users are never
responsible for their memory management; this is all taken care of by
the Triangulation to which they belong.

\headerfile triangulation/dim3.h)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this edge as a normal surface.

Constructing the link of a edge begins with building the frontier of a
regular neighbourhood of the edge. If this is already a normal
surface, then then link is called *thin*. Otherwise the usual
normalisation steps are performed until the surface becomes normal;
note that these normalisation steps could change the topology of the
surface, and in some pathological cases could even reduce it to the
empty surface.

Returns:
    a pair (*s*, *thin*), where *s* is the edge linking normal
    surface, and *thin* is ``True`` if and only if this link is thin
    (i.e., no additional normalisation steps were required).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

