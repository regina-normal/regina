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
R"doc(Represents a triangle within a 2-manifold triangulation.

This class is typically referred to by the aliases Simplex<2> or
Triangle<2> (or, for Python users, ``Simplex2`` and ``Triangle2``).

The class template Face<dim, dim> specialises the generic Face class
template in order to work with top-dimensional simplices in a
triangulation. This class Face<2, 2> specialises it further to add
dimension-specific aliases for dimension 2.

See the documentation for the partial specialisation Face<dim, dim>
for an overview of how a top-dimensional simplex class works.

Triangles do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

\headerfile triangulation/dim2.h)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::adjacentEdge
static const char *adjacentEdge =
R"doc(A dimension-specific alias for adjacentFacet().

See adjacentFacet() for further information.)doc";

// Docstring regina::python::doc::Face_::adjacentTriangle
static const char *adjacentTriangle =
R"doc(A dimension-specific alias for adjacentSimplex().

See adjacentSimplex() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

