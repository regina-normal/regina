/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HomologicalData
static const char *HomologicalData =
R"doc(A specialised class that computes a large amount of detailed
homological information for a 3-manifold triangulation.

This class is both heavyweight and specialised. If you just wish to
compute homology or Euler characteristic, or even map faces of the
skeleton to classes in the homology groups, the Triangulation class
has simpler and slicker member functions that can do this for you. See
for example the Triangulation functions homology(), eulerCharTri(),
eulerCharManifold(), and markedHomology().

The most important thing that HomologicalData adds is the ability to
work with the torsion linking form. Be aware however that the code
that computes the torsion linking form includes some floating-point
arithmetic, and this could be subject to round-off error. Such errors
are not expected, since the floating-point code is simply
distinguishing different multiples of a known irrational, but
nevertheless these results should be considered non-rigorous.

Currently, the information computed by HomologicalData includes:

* the manifold's homology;

* the boundary's homology;

* the map from boundary -> manifold;

* the dual cellular homology;

* the isomorphism on H1 from the dual cellular homology to the regular
  cellular homology;

* the H1 torsion form;

* the Kawauchi-Kojima invariants of torsion linking forms.

This class takes a "least effort" approach to all computations. It
only computes what is neccessary for your requests. It also keeps a
record of all previous computations you've made. If a computation can
be sped up by not recomputing some data, it takes that short-cut.

All these algorithms use two transverse CW decompositions of the
manifold. They correspond to the (possibly ideal) triangulation native
to Regina, and the dual polyhedral (CW) decomposition which appears in
Seifert and Threlfall's textbook.

In the following lists we describe the canonical ordering of both the
cells and the dual cells of the given triangulation.

First we list the cell orderings for the _standard CW decomposition_,
which most closely resembles the ideal triangulation.

* **0**-cells: The non-ideal vertices given in the order
  vertices.begin() to vertices.end(), followed by the ideal endpoints
  of the edges edges.begin() to edges.end() with endpoints for each
  edge taken in the order 0,1.

* **1**-cells: edges.begin() to edges.end(), followed by the ideal
  edges of faces.begin() to faces.end() in order 0,1,2.

* **2**-cells: faces.begin() to faces.end(), followed by the ideal
  faces of tetrahedra.begin() through tetrahedra.end() in order
  0,1,2,3.

* **3**-cells: tetrahedra.begin() through tetrahedra.end().

Next we list the cell orderings for the _dual CW decomposition_: if
the standard CW decomposition came from a morse function *f*, this
would be the one for -*f*.

* **0**-cells: tetrahedra.begin() through tetrahedra.end().

* **1**-cells: the non-boundary faces.begin() through faces.end().

* **2**-cells: the non-boundary edges.begin() through edges.end().

* **3**-cells: the non-boundary, non-ideal vertices.begin() through
  vertices.end().

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Be aware,
however, that it contains an enormous amount of internal data, and
even moves will still be relatively expensive - you should try to use
just the one HomologicalData object and not copy or move it at all, if
possible.

This class will eventually be removed in a future release of Regina.

Author:
    Ryan Budney)doc";

namespace HomologicalData_ {

// Docstring regina::python::doc::HomologicalData_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given object.

This constructor induces a deep copy of the given object; moreover,
this is expensive since HomologicalData objects are _very_ large.)doc";

// Docstring regina::python::doc::HomologicalData_::__init
static const char *__init =
R"doc(Takes as input a triangulation.

This class will maintain a fixed snapshot of the given triangulation
as it is at this point in time. You are free to edit or even destroy
the input triangulation later on; if you do then this HomologicalData
object will automatically take its own deep copy of the original and
continue to use that for its own internal reference.

Parameter ``input``:
    the triangulation to use.)doc";

// Docstring regina::python::doc::HomologicalData_::bdryHomology
static const char *bdryHomology =
R"doc(This routine gives access to the homology of the boundary of the
manifold, computed with the regular CW-decomposition.

Parameter ``q``:
    the dimension of the homology group: can be 0, 1 or 2.

Returns:
    the q-th boundary homology group, in standard cellular homology
    coordinates)doc";

// Docstring regina::python::doc::HomologicalData_::bdryHomologyMap
static const char *bdryHomologyMap =
R"doc(This routine gives access to the homomorphism from the homology of the
boundary to the homology of the manifold.

Parameter ``q``:
    the dimension of the map: can be 0, 1 or 2.

Returns:
    the map from H_q of the boundary to H_q of the manifold, computed
    in standard coordinates.)doc";

// Docstring regina::python::doc::HomologicalData_::countBdryCells
static const char *countBdryCells =
R"doc(Returns the number of cells of the given dimension in the standard CW-
decomposition of the boundary of the manifold. This is a subcomplex of
the complex used in countStandardCells().

Parameter ``dimension``:
    the dimension of the cells in question; this must be 0, 1 or 2.

Returns:
    the number of cells of the given dimension in the standard CW-
    decomposition of the boundary.)doc";

// Docstring regina::python::doc::HomologicalData_::countDualCells
static const char *countDualCells =
R"doc(Returns the number of cells of the given dimension in the dual CW-
decomposition of the manifold. This is typically much smaller than
countStandardCells().

Parameter ``dimension``:
    the dimension of the cells in question; this must be 0, 1, 2 or 3.

Returns:
    the number of cells of the given dimension in the dual CW-
    decomposition to the triangulation.)doc";

// Docstring regina::python::doc::HomologicalData_::countStandardCells
static const char *countStandardCells =
R"doc(Returns the number of cells of the given dimension in the standard
genuine CW-decomposition of the manifold.

In the case that the triangulation is a proper triangulation of a
manifold (or delta-complex decomposition) it simply returns the same
information as in the Triangulation<3> vertex, edge, face and
tetrahedron lists.

In the case that this is an ideal triangulation, this algorithm
returns the details of the corresponding compact manifold with
boundary a union of closed surfaces.

Parameter ``dimension``:
    the dimension of the cells in question; this must be 0, 1, 2 or 3.

Returns:
    the number of cells of the given dimension in the standard CW-
    decomposition of the closed manifold.)doc";

// Docstring regina::python::doc::HomologicalData_::dualHomology
static const char *dualHomology =
R"doc(This routine gives access to the manifold's homology computed with the
dual CW-decomposition.

This routine is typically faster than homology() since the dual CW-
decomposition generally has far fewer cells.

Note that the groups returned by homology() and dualHomology() are
isomorphic, though they are generally described by different
presentations.

Parameter ``q``:
    the dimension of the homology group: can be 0, 1, 2 or 3.

Returns:
    the q-th homology group, computed in the dual CW-decomposition.)doc";

// Docstring regina::python::doc::HomologicalData_::embeddabilityComment
static const char *embeddabilityComment =
R"doc(Returns a comment on whether the manifold might embed in a homology
3-sphere or 4-sphere. Basically, this routine runs through all the
Kawauchi-Kojima conditions, plus a few other `elementary' conditions.

Each comment will be formatted as one or more English sentences (i.e.,
with capitalisation and punctuation). The comments themselves are
subject to change between releases of Regina, since later releases may
have more detailed tests at their disposal.

This routine is available for both orientable and non-orientable
triangulations. In the non-orientable case it may return additional
information regarding the orientable double cover.

Precondition:
    The triangulation is of a connected 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    a string giving a one-line description of what is known about
    where this manifold embeds, based solely on the manifold's
    homological data.)doc";

// Docstring regina::python::doc::HomologicalData_::eulerChar
static const char *eulerChar =
R"doc(The proper Euler characteristic of the manifold, computed from the
dual CW-decomposition.

This routine calculates the Euler characteristic of the corresponding
compact triangulated 3-manifold, with each ideal vertex treated as a
surface boundary component.

This routine returns the same value as
Triangulation<3>::eulerCharManifold(), though it computes it in a
different way.

On the other hand, this routine differs from
Triangulation<3>::eulerCharTri(), which handles ideal triangulations
in a non-standard way (treating each ideal vertex as just a single
vertex).

Returns:
    the Euler characteristic of the corresponding compact triangulated
    3-manifold.)doc";

// Docstring regina::python::doc::HomologicalData_::formIsHyperbolic
static const char *formIsHyperbolic =
R"doc(Returns true iff torsion linking form is `hyperbolic' in the linking-
form sense of the word.

To be a little more precise, Poincare-duality in a compact orientable
boundaryless manifold gives an isomorphism between the torsion
subgroup of H_1(M) denoted tH_1(M) and Hom(tH_1(M),Q/Z), where Q is
the rationals and Z the integers. The associated bilinear form (with
values in Q/Z) is said to be `hyperbolic' if tH_1(M) splits as a
direct sum A+B such that Poincare duality sends A to Hom(B,Q/Z) and B
to Hom(A,Q/Z).

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    ``True`` iff the torsion linking form is hyperbolic.)doc";

// Docstring regina::python::doc::HomologicalData_::formIsSplit
static const char *formIsSplit =
R"doc(Returns true iff the torsion linking form is split.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    ``True`` iff the linking form is split.)doc";

// Docstring regina::python::doc::HomologicalData_::formSatKK
static const char *formSatKK =
R"doc(Returns true iff the torsion linking form satisfies the Kawauchi-
Kojima 2-torsion condition. This condition states that on all elements
*x* of order 2^k, 2^{k-1}form(x,x) = 0.

This is a neccessary condition for an orientable 3-manifold perhaps
with boundary to embed in a homology 4-sphere.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    ``True`` iff the form satisfies the 2-torsion condition of
    Kawauchi-Kojima.)doc";

// Docstring regina::python::doc::HomologicalData_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given HomologicalData objects.

This global routine simply calls HomologicalData::swap(); it is
provided so that HomologicalData meets the C++ Swappable requirements.

.. warning::
    Although this operation is constant time, the HomologicalData
    class contains an enormous amount of data spread across many
    different member variables, and so this should really be
    considered "expensive constant time". You should still work to
    avoid swapping (or moving, and certainly copying) HomologicalData
    objects where possible.

Parameter ``a``:
    the first object whose contents should be swapped.

Parameter ``b``:
    the second object whose contents should be swapped.)doc";

// Docstring regina::python::doc::HomologicalData_::h1CellAp
static const char *h1CellAp =
R"doc(Returns the isomorphism from dualHomology(1) to homology(1) given by a
cellular approximation to the identity map on the manifold.

Returns:
    The isomorphism from dualHomology(1) to homology(1) computed via a
    cellular approximation of the identity map from the first
    1-skeleton to the second.)doc";

// Docstring regina::python::doc::HomologicalData_::homology
static const char *homology =
R"doc(This routine gives access to the manifold's homology computed with the
regular CW-decomposition.

This routine is typically slower than dualHomology(), since
dualHomology() uses the dual CW-decomposition which typically has an
order of magnitude fewer cells.

Note that the groups returned by homology() and dualHomology() are
isomorphic, though they are generally described by different
presentations.

Parameter ``q``:
    the dimension of the homology group: can be 0, 1, 2 or 3.

Returns:
    the q-th homology group, computed in the standard CW-
    decomposition.)doc";

// Docstring regina::python::doc::HomologicalData_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given object.

.. warning::
    Although this operation is constant time, the HomologicalData
    class contains an enormous amount of data spread across many
    different member variables, and so this should really be
    considered "expensive constant time". You should still work to
    avoid swapping (or moving, and certainly copying) HomologicalData
    objects where possible.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionLegendreSymbolVector
static const char *torsionLegendreSymbolVector =
R"doc(Returns the odd p-torsion Legendre symbol vector. This is the last of
the three Kawauchi-Kojima invariants.

For details, see "Algebraic classification of linking pairings on
3-manifolds", Akio Kawauchi and Sadayoshi Kojima, Math. Ann. 253
(1980), 29--42.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    the Legendre symbol vector associated to the torsion linking form.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionLegendreSymbolVectorString
static const char *torsionLegendreSymbolVectorString =
R"doc(Same as torsionLegendreSymbolVector() but returns as a human-readable
string.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    the Legendre symbol vector in human-readable form.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionRankVector
static const char *torsionRankVector =
R"doc(Returns the torsion form rank vector. This is the first of the three
Kawauchi-Kojima complete invariants of the torsion linking form.

This vector describes the rank of the torsion subgroup of H1, given in
prime power form. It is a vector of pairs (*p*, *x*), where *p* is a
prime and *x* is its exponent.

For details, see "Algebraic classification of linking pairings on
3-manifolds", Akio Kawauchi and Sadayoshi Kojima, Math. Ann. 253
(1980), 29--42.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    the torsion form rank vector.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionRankVectorString
static const char *torsionRankVectorString =
R"doc(Same as torsionRankVector() but returns as a human-readable string.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    human-readable prime power factorization of the order of the
    torsion subgroup of H1.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionSigmaVector
static const char *torsionSigmaVector =
R"doc(Returns the 2-torsion sigma vector. This is the second of the three
Kawauchi-Kojima invariants. It is orientation-sensitive.

For details, see "Algebraic classification of linking pairings on
3-manifolds", Akio Kawauchi and Sadayoshi Kojima, Math. Ann. 253
(1980), 29--42.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    the Kawauchi-Kojima sigma-vector.)doc";

// Docstring regina::python::doc::HomologicalData_::torsionSigmaVectorString
static const char *torsionSigmaVectorString =
R"doc(Same as torsionSigmaVector() but returns as a human-readable string.
This is an orientation-sensitive invariant.

Precondition:
    The triangulation is of a connected orientable 3-manifold.

.. warning::
    The code that computes the torsion linking form includes some
    floating-point arithmetic that could be subject to round-off
    error. Such errors are not expected, since this code is simply
    distinguishing different multiples of a known irrational;
    nevertheless, these results should at present be considered non-
    rigorous.

Exception ``UnsolvedCase``:
    The torsion linking form could not be computed. This should be
    rare: the only way it can occur is during an internal rational-to-
    double conversion if the rational is out of range.

Returns:
    the Kawauchi-Kojima sigma-vector in human readable form.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

