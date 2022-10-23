/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FaceEmbedding
constexpr const char *FaceEmbedding =
R"doc(Details how a *subdim*-face of a *dim*-dimensional triangulation
appears within each top-dimensional simplex.

For small-dimensional faces, this class is typically described using
dimension-specific type aliases: VertexEmbedding<dim>,
EdgeEmbedding<dim>, TriangleEmbedding<dim>, TetrahedronEmbedding<dim>
and PentachoronEmbedding<dim> refer to the cases *subdim* = 0, 1, 2, 3
and 4 respectively.

For a *dim*-dimensional triangulation *T*, each *subdim*-face *F*
typically belongs to many top-dimensional simplices of *T*, and
therefore has many associated FaceEmbedding objects. These individual
FaceEmbedding objects correspond to the top-dimensional simplices of
the link of *F* (which is a (*dim* - *subdim* - 1)-dimensional
triangulation).

As of Regina 7.0, a FaceEmbedding can happily outlive its face - even
if the underlying Face object is destroyed (e.g., because the
triangulation changed), if you made a local copy of a FaceEmbedding
beforehand then its simplex(), face() and vertices() routines will
continue to return the same values as they did before, back when the
underlying Face still existed. A FaceEmbedding cannot, however,
outlive its top-dimensional simplex, because internally a
FaceEmbedding references the Simplex object in which it lives (i.e.,
it does not just store an integer simplex index).

If *dim* is one of Regina's standard dimensions, then this template is
specialised to offer additional dimension-specific aliases. In order
to use these specialised classes, you will need to include the
corresponding triangulation headers (e.g., triangulation/dim2.h for
*dim* = 2, or triangulation/dim3.h for *dim* = 3).

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Python does not support templates. Instead this class can be used
    by appending dimensions *dim* and *subdim* as suffices (e.g.,
    FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples above).

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces of the underlying triangulation. This
    must be between 0 and *dim*-1 inclusive.)doc";

namespace FaceEmbedding_ {

// Docstring regina::python::doc::FaceEmbedding_::__copy
constexpr const char *__copy =
R"doc(Creates a new copy of the given object.

Parameter ``cloneMe``:
    the object to copy.)doc";

// Docstring regina::python::doc::FaceEmbedding_::__init
constexpr const char *__init =
R"doc(Creates a new object containing the given data.

Parameter ``simplex``:
    the top-dimensional simplex in which the underlying *subdim*-face
    of the triangulation is contained.

Parameter ``vertices``:
    a mapping from the vertices of the underlying *subdim*-face of the
    triangulation to the corresponding vertex numbers of *simplex*.
    See FaceEmbeddingBase::vertices() for details of how this
    permutation should be structured.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

