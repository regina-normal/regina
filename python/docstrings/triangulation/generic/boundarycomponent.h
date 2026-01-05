/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BoundaryComponent
constexpr const char *BoundaryComponent =
R"doc(A component of the boundary of a *dim*-manifold triangulation.

Regina recognises three types of boundary components:

* A _real boundary component_ is formed from unglued facets of top-
  dimensional simplices of the triangulation. For example, a real
  boundary component of a 3-manifold triangulation is formed from
  boundary triangles in the 2-skeleton.

* An _ideal boundary component_ consists of a single ideal vertex;
  that is, a vertex whose link is a closed (*dim*-1)-manifold but not
  a sphere. This is only possible for dimensions *dim* ≥ 3.

* An _invalid vertex boundary component_ consists of a single invalid
  invalid vertex that does not belong to a real boundary component.
  This means the vertex link must be an ideal (*dim*-1)-manifold, or
  an invalid (*dim*-1)-manifold with no real boundary. Invalid vertex
  boundary components are only possible for dimensions *dim* ≥ 4.

Ideal and invalid vertex boundary components are _only_ recognised
when *dim* is one of Regina's standard dimensions. This is because, in
higher dimensions, the relevant conditions rely on undecidable
problems.

Regina stores different skeletal information for different types of
boundary components:

* For real boundary components in standard dimensions, Regina stores
  all boundary faces of all dimensions 0,1,...,*dim*-1.

* For real boundary components in higher (non-standard) dimensions,
  Regina only stores the boundary (*dim*-1)-faces (i.e., the unglued
  facets of top-dimensional simplices that make up the boundary
  component). This is simply to avoid excessive memory usage.

* For ideal and invalid vertex boundary components, Regina stores the
  relevant vertex. Such boundary components do not contain any faces
  of any dimension greater than zero.

We can encounter some interesting cases with invalid triangulations.
Consider some face whose link has more than one boundary component
(which makes the face invalid). This means that different parts of the
(*dim*)-manifold boundary are effectively "pinched" together. If this
happens, the different parts of the boundary that are pinched might or
might not be listed as part of the same boundary component; if not
then the offending face will be included in _all_ of these boundary
components. Nevertheless, only one of these can be considered the
"official" boundary component of the face as returned by
Face::boundaryComponent(). This is all a bit of a mess, but then again
the entire triangulation is invalid and so you almost certainly have
bigger problems to deal with.

Boundary components are highly temporary: whenever a triangulation
changes, all of its boundary component objects will be deleted and new
ones will be created in their place.

Boundary components do not support value semantics: they cannot be
copied, swapped, or manually constructed. Their location in memory
defines them, and they are often passed and compared by pointer. End
users are never responsible for their memory management; this is all
taken care of by the Triangulation to which they belong.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., BoundaryComponent2
    and BoundaryComponent3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

