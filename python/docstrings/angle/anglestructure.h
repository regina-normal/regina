/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::AngleStructure
static const char *AngleStructure =
R"doc(Represents an angle structure on a triangulation.

Since Regina 7.0, you can modify or even destroy the original
triangulation that was used to create this angle structure. If you do,
then this angle structure will automatically make a private copy of
the original triangulation as an ongoing reference. Different angle
structures (and normal surfaces) can all share the same private copy,
so this is not an expensive process.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace AngleStructure_ {

// Docstring regina::python::doc::AngleStructure_::__copy
static const char *__copy = R"doc(Creates a new copy of the given angle structure.)doc";

// Docstring regina::python::doc::AngleStructure_::__eq
static const char *__eq =
R"doc(Determines whether this and the given angle structure are identical.

This routine is safe to call even if this and the given angle
structure do not belong to the same triangulation:

* If the two triangulations have the same size, then this routine will
  test whether this angle structure, if transplanted into the other
  triangulation using the same tetrahedron numbering, would be the
  same as *other*.

* If the two triangulations have different sizes, then this routine
  will return ``False``.

Parameter ``other``:
    the angle structure to be compared with this structure.

Returns:
    ``True`` if and only if this and the given structure are
    identical.)doc";

// Docstring regina::python::doc::AngleStructure_::__init
static const char *__init =
R"doc(Creates a new copy of the given angle structure, but relocated to the
given triangulation.

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this angle structure will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given triangulation is either the same as, or is
    combinatorially identical to, the triangulation on which *src* is
    placed.

Parameter ``src``:
    the angle structure to copy.

Parameter ``triangulation``:
    the triangulation on which this new angle structure will be
    placed.)doc";

// Docstring regina::python::doc::AngleStructure_::__init_2
static const char *__init_2 =
R"doc(Creates a new angle structure on the given triangulation with the
given coordinate vector.

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this angle structure will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given coordinate vector represents an angle structure on the
    given triangulation, according to the integer vector
    representation described in the notes for vector().

Python:
    The supported types for the template parameter *U* are
    regina::Integer and regina::LargeInteger. You may also, if you
    prefer, pass *vector* as a Python list of integers.

Template parameter ``U``:
    the type of object held by the given vector. It must be possible
    to assign an object of type *U* to a regina::Integer.

Parameter ``triang``:
    the triangulation on which this angle structure lies.

Parameter ``vector``:
    a vector containing the individual angles in the angle structure.)doc";

// Docstring regina::python::doc::AngleStructure_::__lt
static const char *__lt =
R"doc(Imposes a total order on all angle structures.

This order is not mathematically meaningful; it is merely provided for
scenarios where you need to be able to sort angle structures (e.g.,
when using them as keys in a map).

The order _is_ well-defined, and will be preserved across copy/move
operations, different program executions, and different platforms
(since it is defined purely in terms of the angle coordinates, and
does not use transient properties such as locations in memory).

This operation is consistent with the equality test. In particular, it
does not matter whether the two angle structures belong to different
triangulations. See the equality test operator==() for further
details.

Parameter ``other``:
    the angle structure to be compared with this structure.

Returns:
    ``True`` if and only if this appears before the given structure in
    the total order.)doc";

// Docstring regina::python::doc::AngleStructure_::__ne
static const char *__ne =
R"doc(Determines whether this and the given angle structure are different.

This routine is safe to call even if this and the given angle
structure do not belong to the same triangulation:

* If the two triangulations have the same size, then this routine will
  test whether this angle structure, if transplanted into the other
  triangulation using the same tetrahedron numbering, would be
  different from *other*.

* If the two triangulations have different sizes, then this routine
  will return ``True``.

Parameter ``other``:
    the angle structure to be compared with this structure.

Returns:
    ``True`` if and only if this and the given structure are
    different.)doc";

// Docstring regina::python::doc::AngleStructure_::angle
static const char *angle =
R"doc(Returns the requested angle in this angle structure. The angle
returned will be scaled down; the actual angle is the returned value
multiplied by π.

Within a tetrahedron, the three angles are indexed 0, 1 and 2. Angle
*i* appears on edge *i* of the tetrahedron as well as its opposite
edge (5-*i*).

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron in which the
    requested angle lives; this should be between 0 and
    Triangulation<3>::size()-1 inclusive.

Parameter ``edgePair``:
    the number representing the pair of edges holding the requested
    angle, as described above; this should be 0, 1 or 2.

Returns:
    the requested angle scaled down by π.)doc";

// Docstring regina::python::doc::AngleStructure_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given angle structures.

This global routine simply calls AngleStructure::swap(); it is
provided so that AngleStructure meets the C++ Swappable requirements.

Parameter ``a``:
    the first angle structure whose contents should be swapped.

Parameter ``b``:
    the second angle structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::AngleStructure_::isStrict
static const char *isStrict =
R"doc(Determines whether this is a strict angle structure. A strict angle
structure has all angles strictly between (not including) 0 and π.

Returns:
    ``True`` if and only if this is a strict angle structure.)doc";

// Docstring regina::python::doc::AngleStructure_::isTaut
static const char *isTaut =
R"doc(Determines whether this is a taut angle structure. A taut angle
structure contains only angles 0 and π.

Here we use the Kang-Rubinstein definition of a taut angle structure
[1], which is based on the angles alone. In his original paper [2],
Lackenby has an extra condition whereby 2-faces of the triangulation
must have consistent coorientations, which we do not enforce here.

[1] E. Kang and J. H. Rubinstein, "Ideal triangulations of 3-manifolds
II; Taut and angle structures", Algebr. Geom. Topol. 5 (2005), pp.
1505-1533.

[2] M. Lackenby, "Taut ideal triangulations of 3-manifolds", Geom.
Topol. 4 (2000), pp. 369-395.

Returns:
    ``True`` if and only if this is a taut structure.)doc";

// Docstring regina::python::doc::AngleStructure_::isVeering
static const char *isVeering =
R"doc(Determines whether this is a veering structure. A veering structure is
taut angle structure with additional strong combinatorial constraints,
which we do not outline here. For a definition, see C. D. Hodgson, J.
H. Rubinstein, H. Segerman, and S. Tillmann, "Veering triangulations
admit strict angle structures", Geom. Topol., 15 (2011), pp.
2073-2089.

Note that the Hodgson et al. definition is slightly more general than
Agol's veering taut triangulations from his original paper: I. Agol, "
Ideal triangulations of pseudo-Anosov mapping tori", in "Topology and
Geometry in Dimension Three", volume 560 of Contemp. Math., pp. 1-17,
Amer. Math. Soc., 2011. This mirrors the way in which the Kang-
Rubinstein definition of taut angle structure is slightly more general
than Lackenby's. See the Hodgson et al. paper for full details and
comparisons between the two settings.

If this angle structure is not taut, or if the underlying
triangulation is non-orientable, then this routine will return
``False``.

Returns:
    ``True`` if and only if this is a veering structure.)doc";

// Docstring regina::python::doc::AngleStructure_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given angle structure. This is a
fast (constant time) operation.

This and the given angle structure do not need to be on the same
underlying triangulation, and they do not need to have the same length
vectors (but of course if either property differs then these features
will be adjusted accordingly).

Parameter ``other``:
    the angle structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::AngleStructure_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation on which this angle structure lies.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the AngleStructure constructor.

This will return a correct result even if the original triangulation
has since been modified or destroyed. However, in order to ensure this
behaviour, it is possible that at different points in time this
function may return references to different C++ objects.

The rules for using the triangulation() reference are:

* Do not keep the resulting reference as a long-term reference or
  pointer of your own, since in time you may find yourself referring
  to the wrong object (see above). Just call this function again.

* You must respect the read-only nature of the result (i.e., you must
  not cast the constness away). The snapshotting process detects
  modifications, and modifying the frozen snapshot may result in an
  exception being thrown.

Returns:
    a reference to the underlying triangulation.)doc";

// Docstring regina::python::doc::AngleStructure_::vector
static const char *vector =
R"doc(Gives read-only access to the integer vector that Regina uses
internally to represent this angle structure.

This vector contains one member per angle plus a final scaling member;
to obtain the actual angle in the angle structure one should divide
the corresonding angle member by the scaling member and then multiply
by π.

If there are *t* tetrahedra in the underlying triangulation, there
will be precisely 3*t*+1 elements in this vector. The first three
elements will be the angle members for the first tetrahedron, the next
three for the second tetrahedron and so on. For each tetrahedron, the
three individual elements are the angle members corresponding to edges
0, 1 and 2 of the tetrahedron (and also their opposite edges 5, 4 and
3 respectively). The final element of the vector is the scaling member
as described above.

Returns:
    the underlying integer vector.)doc";

// Docstring regina::python::doc::AngleStructure_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this angle structure and all of its
properties. This routine will be called from within
AngleStructures::writeXMLPacketData().

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

