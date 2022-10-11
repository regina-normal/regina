/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalAlgFlags
static const char *NormalAlgFlags =
R"doc(Represents options and variants of algorithms for enumerating various
types of normal surfaces in 3-manifold triangulations.

These options can be combined using the bitwise OR operator, and then
passed to enumeration routines such as the NormalSurfaces class
constructor.)doc";

// Docstring regina::python::doc::NormalListFlags
static const char *NormalListFlags =
R"doc(Represents different lists of normal surfaces that might be
constructed for a given 3-manifold triangulation.

The NormalList enumeration refers to the *contents* of the list,
whereas the NormalAlgFlags enumeration refers to the *algorithm* used
to build it.

These flags can be combined using the bitwise OR operator, and then
passed to enumeration routines such as the NormalSurfaces class
constructor.)doc";

// Docstring regina::python::doc::NormalTransform
static const char *NormalTransform =
R"doc(Represents different ways in which Regina can transform one normal
surface list into another.

Each type of transformation comes with its own preconditions on the
original normal surface list and/or its underlying triangulation;
these preconditions are documented alongside the individual
enumeration values.)doc";

// Docstring regina::python::doc::__bor
static const char *__bor =
R"doc(Returns the bitwise OR of the two given flags.

Parameter ``lhs``:
    the first flag to combine.

Parameter ``rhs``:
    the second flag to combine.

Returns:
    the combination of both flags.)doc";

// Docstring regina::python::doc::__bor_2
static const char *__bor_2 =
R"doc(Returns the bitwise OR of the two given flags.

Parameter ``lhs``:
    the first flag to combine.

Parameter ``rhs``:
    the second flag to combine.

Returns:
    the combination of both flags.)doc";

namespace NormalAlgFlags_ {

// Docstring regina::python::doc::NormalAlgFlags_::NS_ALG_CUSTOM
static const char *NS_ALG_CUSTOM =
R"doc(Indicates that a normal surface list was built using a customised
algorithm. In such cases, no further details on the algorithm are
available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_ALG_DEFAULT
static const char *NS_ALG_DEFAULT =
R"doc(An empty flag, indicating to an enumeration routine that it should use
its default behaviour. The numeric value of this flag is zero (i.e.,
it has no effect when combined with other flags using bitwise OR).)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_ALG_LEGACY
static const char *NS_ALG_LEGACY =
R"doc(Indicates that a normal surface list was enumerated using an older
version of Regina (4.93 or earlier).

These older versions did not retain details of the algorithm used to
build each list, and so in such cases no further algorithmic
information is available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_HILBERT_CD
static const char *NS_HILBERT_CD =
R"doc(When enumerating fundamental normal surfaces, this flag indicates that
a modified Contejean-Devie procedure should be used for enumerating a
Hilbert basis.

The Contejean-Devie procedure is typically *much* slower than either
the primal or dual method, and users should only request it if they
have some specialised need.

For details and comparisons of the various options for enumerating
fundamental normal surfaces, see B. A. Burton, "Enumerating
fundamental normal surfaces: Algorithms, experiments and invariants",
ALENEX 2014: Proceedings of the Meeting on Algorithm Engineering &
Experiments, SIAM, 2014, pp. 112-124.

This flag is incompatible with NS_HILBERT_PRIMAL, NS_HILBERT_DUAL and
NS_HILBERT_FULLCONE.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_HILBERT_DUAL
static const char *NS_HILBERT_DUAL =
R"doc(When enumerating fundamental normal surfaces, this flag indicates that
the dual method should be used for enumerating a Hilbert basis.

The dual method is fast (like the primal method), but its performance
is highly variable; for this reason the primal method is recommended
instead. This method does not make use of Normaliz, and is the
recommended method for situations in which Normaliz is not available
for some reason.

For details and comparisons of the various options for enumerating
fundamental normal surfaces, see B. A. Burton, "Enumerating
fundamental normal surfaces: Algorithms, experiments and invariants",
ALENEX 2014: Proceedings of the Meeting on Algorithm Engineering &
Experiments, SIAM, 2014, pp. 112-124.

This flag is incompatible with NS_HILBERT_PRIMAL, NS_HILBERT_CD and
NS_HILBERT_FULLCONE.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_HILBERT_FULLCONE
static const char *NS_HILBERT_FULLCONE =
R"doc(When enumerating fundamental normal surfaces, this flag indicates that
a Hilbert basis for the full solution cone should be constructed, and
additional combinatorial constraints (such as the quadrilateral
constraints) should only be enforced as the final step.

If you are only enumerating properly embedded surfaces then this
procedure *extremely* slow, and users should only request it if they
have some specialised need.

For details and comparisons of the various options for enumerating
fundamental normal surfaces, see B. A. Burton, "Enumerating
fundamental normal surfaces: Algorithms, experiments and invariants",
ALENEX 2014: Proceedings of the Meeting on Algorithm Engineering &
Experiments, SIAM, 2014, pp. 112-124.

This flag is incompatible with NS_HILBERT_PRIMAL, NS_HILBERT_DUAL and
NS_HILBERT_CD.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_HILBERT_PRIMAL
static const char *NS_HILBERT_PRIMAL =
R"doc(When enumerating fundamental normal surfaces, this flag indicates that
the primal method should be used for enumerating a Hilbert basis.

The primal method is recommended, and enumeration algorithms will use
it if possible unless a different method is explicitly requested. This
method uses code from Normaliz for parts of its processing.

For details and comparisons of the various options for enumerating
fundamental normal surfaces, see B. A. Burton, "Enumerating
fundamental normal surfaces: Algorithms, experiments and invariants",
ALENEX 2014: Proceedings of the Meeting on Algorithm Engineering &
Experiments, SIAM, 2014, pp. 112-124.

This flag is incompatible with NS_HILBERT_DUAL, NS_HILBERT_CD and
NS_HILBERT_FULLCONE.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_VERTEX_DD
static const char *NS_VERTEX_DD =
R"doc(When enumerating vertex normal surfaces, this flag indicates that a
modified double description method should be used.

This algorithm can suffer from a combinatorial explosion with larger
problems, leading to extremely large time and memory footprints. Users
should only request this if they have some specialised need.

For details on the modified double description method, see B. A.
Burton, "Optimizing the double description method for normal surface
enumeration", Mathematics of Computation 79 (2010), pp. 453-484.

This flag is incompatible with NS_VERTEX_TREE.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_VERTEX_STD_DIRECT
static const char *NS_VERTEX_STD_DIRECT =
R"doc(When enumerating in standard normal or almost normal coordinates, this
flag indicates that the algorithm should work directly in that
coordinate system, and should not go via the "reduced" (quadrilateral
or quadrilateral-octagon) coordinate system.

This is typically *much* slower than going via the reduced system, and
users should only request this if they have a specialised need. See
NS_VERTEX_VIA_REDUCED for further information.

This flag is incompatible with NS_VERTEX_VIA_REDUCED.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_VERTEX_TREE
static const char *NS_VERTEX_TREE =
R"doc(When enumerating vertex normal surfaces, this flag indicates that the
tree traversal algorithm should be used.

This algorithm is based on linear and integer programming techniques,
and has many desirable properties including a relatively low overhead.
Enumeration algorithms will use it if possible unless a different
method is explicitly requested.

For details on the tree traversal algorithm, see B. A. Burton and M.
Ozlen, "A tree traversal algorithm for decision problems in knot
theory and 3-manifold topology", Algorithmica 65 (2013), pp. 772-801.

This flag is incompatible with NS_VERTEX_DD.)doc";

// Docstring regina::python::doc::NormalAlgFlags_::NS_VERTEX_VIA_REDUCED
static const char *NS_VERTEX_VIA_REDUCED =
R"doc(When enumerating in standard normal or almost normal coordinates, this
flag indicates that the algorithm should first enumerate in
quadrilateral or quadrilateral-octagon coordinates, and then expand
this "reduced" solution set to the (typically larger) "standard"
solution set.

This is typically much faster than a direct enumeration in standard
normal or almost normal coordinates, and enumeration routines will use
this option where possible unless explicitly requested not to (via the
flag NS_VERTEX_STD_DIRECT).

For an explanation of this procedure, see B. A. Burton, "Converting
between quadrilateral and standard solution sets in normal surface
theory", Algebr. Geom. Topol. 9 (2009), 2121-2174.

This flag is incompatible with NS_VERTEX_STD_DIRECT.)doc";

}

namespace NormalListFlags_ {

// Docstring regina::python::doc::NormalListFlags_::NS_CUSTOM
static const char *NS_CUSTOM =
R"doc(Indicates some other type of list, typically hand-crafted by the user
or built by some customised algorithm.

If this flag is passed to an enumeration routine, it will be ignored.)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_EMBEDDED_ONLY
static const char *NS_EMBEDDED_ONLY =
R"doc(Indicates that this list is restricted to properly embedded surfaces
only.

This flag is incompatible with NS_IMMERSED_SINGULAR.)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_FUNDAMENTAL
static const char *NS_FUNDAMENTAL =
R"doc(Indicates a list of all fundamental normal surfaces, with respect to
the particular normal coordinate system used by the list.

This flag is incompatible with NS_VERTEX.)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_IMMERSED_SINGULAR
static const char *NS_IMMERSED_SINGULAR =
R"doc(Indicates that the scope of this list includes not just properly
embedded surfaces, but also immersed and/or branched surfaces.

This is no guarantee that the list *contains* immersed and/or branched
surfaces; it merely states that such surfaces have not been explicitly
excluded (in particular, the quadrilateral constraints have not been
enforced).

This flag is incompatible with NS_EMBEDDED_ONLY.)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_LEGACY
static const char *NS_LEGACY =
R"doc(Indicates a list that was constructed using an old version of Regina
(4.93 or earlier).

These older versions did not retain details of how each list was
constructed, beyond whether immersed and/or singular surfaces were
included. Therefore no information is available for such lists, other
than the presence or absence of the NS_EMBEDDED_ONLY and/or
NS_IMMERSED_SINGULAR flags.

If this flag is passed to an enumeration routine, it will be ignored.)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_LIST_DEFAULT
static const char *NS_LIST_DEFAULT =
R"doc(An empty flag, indicating to an enumeration routine that it should use
its default behaviour. The numeric value of this flag is zero (i.e.,
it has no effect when combined with other flags using bitwise OR).)doc";

// Docstring regina::python::doc::NormalListFlags_::NS_VERTEX
static const char *NS_VERTEX =
R"doc(Indicates a list of all vertex normal surfaces, with respect to the
particular normal coordinate system used by the list.

This flag is incompatible with NS_FUNDAMENTAL.)doc";

}

namespace NormalTransform_ {

// Docstring regina::python::doc::NormalTransform_::NS_CONV_REDUCED_TO_STD
static const char *NS_CONV_REDUCED_TO_STD =
R"doc(Converts the set of all embedded vertex normal surfaces in
quadrilateral or quadrilateral-octagon coordinates to the set of all
embedded vertex normal surfaces in standard normal or standard almost
normal coordinates respectively.

It should be emphasised that this routine does *not* simply convert
vectors from one coordinate system to another; instead it converts a
full set of vertex surfaces in quad or quad-oct coordinates into a
full set of vertex surfaces in standard normal or almost normal
coordinates. Typically there are many more vertex surfaces in standard
coordinates (all of which this routine will find).

This conversion process is typically *much* faster than enumerating
surfaces directly in standard coordinates. However, normally you would
not need to invoke this transformation yourself, since the standard
enumeration process will use it automatically when possible. That is,
when asked to build a list of standard vertex surfaces, the
NormalSurfaces constructor will (if possible) first find all quad or
quad-oct vertex surfaces and then use this procedure to convert the
solution set.

Nevertheless, this standalone transformation is provided as a
convenience for users who already have a set of quad or quad-oct
vertex surfaces, and who simply wish to convert them to a set of
standard vertex surfaces without the implicit cost of enumerating the
quad or quad-oct vertex surfaces again.

The conversion algorithm is described in detail in "Converting between
quadrilateral and standard solution sets in normal surface theory",
Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.

The preconditions for using this transformation:

* The underlying triangulation is valid, and has no ideal vertices.

* The input to this transformation is exactly the set of all embedded
  vertex surfaces in quadrilateral or quadrilateral-octagon
  coordinates. This will be checked by examining
  NormalSurface::coords() and NormalSurface::which().)doc";

// Docstring regina::python::doc::NormalTransform_::NS_CONV_STD_TO_REDUCED
static const char *NS_CONV_STD_TO_REDUCED =
R"doc(Converts the set of all embedded vertex normal surfaces in standard
normal or standard almost normal coordinates to the set of all
embedded vertex normal surfaces in quadrilateral or quadrilateral-
octagon coordinates respectively.

It should be emphasised that this routine does *not* simply convert
vectors from one coordinate system to another; instead it converts a
full set of vertex surfaces in standard normal or almost normal
coordinates into a full set of vertex surfaces in quad or quad-oct
coordinates. Typically there are far fewer vertex surfaces in quad or
quad-oct coordinates (all of which this routine will find).

The conversion algorithm is described in detail in "Converting between
quadrilateral and standard solution sets in normal surface theory",
Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.

The preconditions for using this transformation:

* The underlying triangulation is valid, and has no ideal vertices.

* The input to this transformation is exactly the set of all embedded
  vertex surfaces in standard normal or almost normal coordinates.
  This will be checked by examining NormalSurface::coords() and
  NormalSurface::which().)doc";

// Docstring regina::python::doc::NormalTransform_::NS_FILTER_COMPATIBLE
static const char *NS_FILTER_COMPATIBLE =
R"doc(Selects only the surfaces in the input list that have at least one
locally compatible partner. That is, a surface *S* from the input list
will be included in the output list if and only if there is some other
surface *T* in the input list for which *S* and *T* are locally
compatible. See NormalSurface::locallyCompatible() for further details
on compatibility testing.

Be aware that, since vertex links are compatible with everything, if
the input list contains a vertex link plus at least one other surface,
then the output list will be identical to the input.

For the output list, which() will include the NS_CUSTOM flag, and
algorithm() will be precisely NS_ALG_CUSTOM.

The preconditions for using this transformation:

* The input list contains only embedded normal or almost normal
  surfaces. This will be checked by examining NormalSurface::which().)doc";

// Docstring regina::python::doc::NormalTransform_::NS_FILTER_DISJOINT
static const char *NS_FILTER_DISJOINT =
R"doc(Selects only the surfaces in the input list that have at least one
disjoint partner. That is, a surface *S* from the input list will be
included in the output list if and only if there is some other surface
*T* in the input list for which *S* and *T* can be made to intersect
nowhere at all, without changing either normal isotopy class. See
NormalSurface::disjoint() for further details on disjointness testing.

This transformation comes with some caveats:

* It cannot deal with empty, disconnected or non-compact surfaces.
  Such surfaces will be silently ignored, and will not be used in any
  disjointness tests (in particular, they will never be considered as
  a "disjoint partner" for any other surface).

* Since vertex links can always be made disjoint from other surfaces,
  if the input list contains a vertex link plus at least one other
  surface, then the output list will be identical to the input.

For the output list, which() will include the NS_CUSTOM flag, and
algorithm() will be precisely NS_ALG_CUSTOM.

The preconditions for using this transformation:

* The input list contains only embedded normal or almost normal
  surfaces. This will be checked by examining NormalSurface::which().)doc";

// Docstring regina::python::doc::NormalTransform_::NS_FILTER_INCOMPRESSIBLE
static const char *NS_FILTER_INCOMPRESSIBLE =
R"doc(Selects only the surfaces in the input list that "might" represent
two-sided incompressible surfaces.

More precisely, this transformation considers all two-sided surfaces
in the input list, as well as the two-sided double covers of all one-
sided surfaces in the input list (see below for details on how one-
sided surfaces are handled). Each of these surfaces is examined using
relatively fast heuristic tests for incompressibility. Any surface
that is definitely *not* incompressible is ignored, and all other
surfaces are placed in the output list.

Therefore, it is guaranteed that every incompressible surface from the
input list will be included in the output list. However, each
individual output surface might or might not be incompressible.

See NormalSurface::isIncompressible() for the definition of
incompressibility that is used here. Note in particular that spheres
are *never* considered incompressible.

As indicated above, this filter works exclusively with two-sided
surfaces. If a surface in the input list is one-sided, the heuristic
incompressibility tests will be run on its two-sided double cover.
Nevertheless, if the tests pass, the original one-sided surface (not
the double cover) will be added to the output list.

Currently the heuristic tests include (i) throwing away all vertex
links and thin edge links, and then (ii) cutting along the remaining
surfaces and running Triangulation<3>::hasSimpleCompressingDisc() on
the resulting bounded triangulations. For more details on these tests
see "The Weber-Seifert dodecahedral space is non-Haken", Benjamin A.
Burton, J. Hyam Rubinstein and Stephan Tillmann, Trans. Amer. Math.
Soc. 364:2 (2012), pp. 911-932.

For the output list, which() will include the NS_CUSTOM flag, and
algorithm() will be precisely NS_ALG_CUSTOM.

The preconditions for using this transformation:

* The underlying 3-manifold triangulation is valid and closed.

* The input list contains only compact, connected, embedded normal
  surfaces. In particular, almost normal surfaces are not supported.

.. warning::
    The behaviour of this transformation is subject to change in
    future versions of Regina, since additional tests may be added to
    improve the power of this filtering.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

