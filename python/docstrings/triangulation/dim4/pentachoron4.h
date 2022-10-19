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
R"doc(Represents a pentachoron (a 4-dimensional simplex) within a 4-manifold
triangulation.

This class is typically referred to by the aliases Simplex<4> or
Pentachoron<4> (or, for Python users, ``Simplex4`` and
``Pentachoron4``).

The class template Face<dim, dim> specialises the generic Face class
template in order to work with top-dimensional simplices in a
triangulation. This class Face<4, 4> specialises it further to add
dimension-specific aliases for dimension 4.

See the documentation for the partial specialisation Face<dim, dim>
for an overview of how a top-dimensional simplex class works.

Pentachora do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

\headerfile triangulation/dim4.h)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::adjacentPentachoron
static const char *adjacentPentachoron =
R"doc(A dimension-specific alias for adjacentSimplex().

See adjacentSimplex() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

