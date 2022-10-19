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
R"doc(Represents a tetrahedron within a 3-manifold triangulation.

This class is typically referred to by the aliases Simplex<3> or
Tetrahedron<3> (or, for Python users, ``Simplex3`` and
``Tetrahedron3``).

The class template Face<dim, dim> specialises the generic Face class
template in order to work with top-dimensional simplices in a
triangulation. This class Face<3, 3> specialises it further to add
dimension-specific aliases for dimension 3.

See the documentation for the partial specialisation Face<dim, dim>
for an overview of how a top-dimensional simplex class works.

Tetrahedra do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::adjacentFace
static const char *adjacentFace =
R"doc(A dimension-specific alias for adjacentFacet().

See adjacentFacet() for further information.)doc";

// Docstring regina::python::doc::Face_::adjacentTetrahedron
static const char *adjacentTetrahedron =
R"doc(A dimension-specific alias for adjacentSimplex().

See adjacentSimplex() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

