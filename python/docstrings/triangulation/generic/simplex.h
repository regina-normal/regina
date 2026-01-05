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
constexpr const char *Face =
R"doc(Represents a top-dimensional simplex in a *dim*-manifold
triangulation.

For example, for 3-manifolds this class represents a tetrahedron, and
for 2-manifolds this class represents a triangle.

Although this is a specialisation of the Face class template, this
class is typically referred to using the alias Simplex<dim>. For
Regina's standard dimensions, you can also use the aliases
Triangle<2>, Tetrahedron<3> and Pentachoron<4>.

Top-dimensional simplices cannot exist in isolation (without a
triangulation object), and they cannot be created or destroyed
directly. Instead, you create and destroy them via the underlying
triangulation, by calling routines such as
Triangulation<dim>::newSimplex() or
Triangulation<dim>::removeSimplex().

Amongst other things, this class is used to view and change the
gluings between top-dimensional simplices. For this we number the
facets and vertices of each simplex 0,...,*dim*, so that facet *i* is
opposite vertex *i*.

Each simplex may have an optional description. This is typically a
human-readable piece of text. Descriptions are not required, and do
not need to be unique.

For Regina's standard dimensions, this template is specialised and
offers significant extra functionality. In order to use these
specialised classes, you will need to include the corresponding
triangulation headers (e.g., triangulation/dim2.h for *dim* = 2, or
triangulation/dim3.h for *dim* = 3).

Simplices do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Simplex2 and
    Simplex3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

