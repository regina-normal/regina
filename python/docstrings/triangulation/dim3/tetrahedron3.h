/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Face {

// Docstring regina::python::doc::Face::__class
static constexpr const char __class[] =
R"doc(Represents a tetrahedron within a 3-manifold triangulation.

This class is typically referred to by the aliases Simplex<3> or
Tetrahedron<3> (or, for Python users, ``Simplex3`` and
``Tetrahedron3``).

The class template Face<dim, dim> specialises the generic Face class
template in order to work with top-dimensional simplices in a
triangulation. This class Face<3, 3> specialises it further to add
dimension-specific aliases for dimension 3.

See the generic documentation for Face<dim, dim> for an overview of
how the top-dimensional simplex classes work. In Python, you can read
this generic documentation by looking at a higher dimension: try
``help(Simplex5)``.

Tetrahedra do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.)doc";

// Docstring regina::python::doc::Face::adjacentFace
static constexpr const char adjacentFace[] =
R"doc(A dimension-specific alias for adjacentFacet().

See adjacentFacet() for further information.)doc";

// Docstring regina::python::doc::Face::adjacentTetrahedron
static constexpr const char adjacentTetrahedron[] =
R"doc(A dimension-specific alias for adjacentSimplex().

See adjacentSimplex() for further information.)doc";

}; // struct Face

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

