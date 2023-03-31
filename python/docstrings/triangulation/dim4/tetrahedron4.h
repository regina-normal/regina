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
R"doc(Represents a tetrahedron in the skeleton of a 4-manifold
triangulation.

This is a specialisation of the generic Face class template; see the
generic documentation for Face for a general overview of how the face
classes work. In Python, you can read this generic documentation by
looking at faces in a higher dimension: try ``help(Tetrahedron5)``.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Tetrahedra do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this tetrahedron as a normal hypersurface.

Constructing the link of a tetrahedron begins with building the
frontier of a regular neighbourhood of the tetrahedron. If this is
already a normal hypersurface, then then link is called _thin_.
Otherwise some basic normalisation steps are performed until the
hypersurface becomes normal; note that these normalisation steps could
change the topology of the hypersurface, and in some pathological
cases could even reduce it to the empty hypersurface.

Although normalisation of arbitrary embedded 3-manifolds is messy, for
tetrahedron links the process is thankfully simpler. Essentially, any
changes will be limited to operations analagous to compressions and
boundary compressions along discs and 3-balls, as well as removing
trivial 4-sphere components.

Returns:
    a pair (*s*, *thin*), where *s* is the tetrahedron linking normal
    hypersurface, and *thin* is ``True`` if and only if this link is
    thin (i.e., no additional normalisation steps were required).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

