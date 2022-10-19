/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Example
static const char *Example =
R"doc(This class offers static routines for constructing a variety of sample
*dim*-dimensional triangulations. These triangulations may be useful
for testing new code, or for simply getting a feel for how Regina
works.

In higher dimensions, only a handful of triangulations are available
(in contrast to the 3-dimensional class Example<3>, which offers many
interesting examples). The authors hope to expand this list in future
releases of Regina.

The sample triangulations offered here may prove especially useful in
Regina's scripting interface, where working with pre-existing files is
more complicated than in the GUI.

For Regina's standard dimensions, this template is specialised and
offers many more example triangulations. In order to use these
specialised classes, you will need to include the corresponding
headers (e.g., triangulation/example2.h for *dim* = 2, or
triangulation/example3.h for *dim* = 3).

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Example2 and
    Example3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the example triangulations to construct. This
    must be between 2 and 15 inclusive.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

