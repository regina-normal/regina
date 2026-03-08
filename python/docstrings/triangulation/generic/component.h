/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Component
constexpr const char *Component =
R"doc(A connected component of a *dim*-manifold triangulation.

Components are highly temporary: whenever a triangulation changes, all
of its component objects will be deleted and new ones will be created
in their place.

For Regina's standard dimensions, this template is specialised and
offers more functionality. In order to use these specialised classes,
you will need to include the corresponding triangulation headers
(e.g., triangulation/dim2.h for *dim* = 2, or triangulation/dim3.h for
*dim* = 3).

Components do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Component2 and
    Component3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

