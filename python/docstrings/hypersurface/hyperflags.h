/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HyperAlgFlags
static const char *HyperAlgFlags =
R"doc(Represents options and variants of algorithms for enumerating various
types of normal hypersurfaces in 4-manifold triangulations.

These options can be combined using the bitwise OR operator, and then
passed to enumeration routines such as the NormalHypersurfaces class
constructor.)doc";

// Docstring regina::python::doc::HyperListFlags
static const char *HyperListFlags =
R"doc(Represents different lists of normal hypersurfaces that might be
constructed for a given 4-manifold triangulation.

The HyperList enumeration refers to the _contents_ of the list,
whereas the HyperAlgFlags enumeration refers to the _algorithm_ used
to build it.

These flags can be combined using the bitwise OR operator, and then
passed to enumeration routines such as the NormalHypersurfaces class
constructor.)doc";

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

namespace HyperAlgFlags_ {

// Docstring regina::python::doc::HyperAlgFlags_::HS_ALG_CUSTOM
static const char *HS_ALG_CUSTOM =
R"doc(Indicates that a normal hypersurface list was built using a customised
algorithm. In such cases, no further details on the algorithm are
available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::HyperAlgFlags_::HS_ALG_DEFAULT
static const char *HS_ALG_DEFAULT =
R"doc(An empty flag, indicating to an enumeration routine that it should use
its default behaviour. The numeric value of this flag is zero (i.e.,
it has no effect when combined with other flags using bitwise OR).)doc";

// Docstring regina::python::doc::HyperAlgFlags_::HS_ALG_LEGACY
static const char *HS_ALG_LEGACY =
R"doc(Indicates that a normal hypersurface list was enumerated using an
older prerelease version of the 4-manifolds code (prior to Regina
5.0).

These prerelease versions did not retain details of the algorithm used
to build each list, and so in such cases no further algorithmic
information is available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::HyperAlgFlags_::HS_HILBERT_DUAL
static const char *HS_HILBERT_DUAL =
R"doc(When enumerating fundamental normal hypersurfaces, this flag indicates
that the dual method should be used for enumerating a Hilbert basis.

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

This flag is incompatible with HS_HILBERT_PRIMAL.)doc";

// Docstring regina::python::doc::HyperAlgFlags_::HS_HILBERT_PRIMAL
static const char *HS_HILBERT_PRIMAL =
R"doc(When enumerating fundamental normal hypersurfaces, this flag indicates
that the primal method should be used for enumerating a Hilbert basis.

The primal method is recommended, and enumeration algorithms will use
it if possible unless a different method is explicitly requested. This
method uses code from Normaliz for parts of its processing.

For details and comparisons of the various options for enumerating
fundamental normal surfaces, see B. A. Burton, "Enumerating
fundamental normal surfaces: Algorithms, experiments and invariants",
ALENEX 2014: Proceedings of the Meeting on Algorithm Engineering &
Experiments, SIAM, 2014, pp. 112-124.

This flag is incompatible with HS_HILBERT_DUAL.)doc";

// Docstring regina::python::doc::HyperAlgFlags_::HS_VERTEX_DD
static const char *HS_VERTEX_DD =
R"doc(When enumerating vertex normal hypersurfaces, this flag indicates that
a modified double description method should be used.

This algorithm can suffer from a combinatorial explosion with larger
problems, leading to extremely large time and memory footprints. Users
should only request this if they have some specialised need.

Having said that: this is the only algorithm that Regina currently
supports for 4-dimensional vertex enumeration (unlike the
3-dimensional case, where several alternative algorithms are
available). Therefore Regina will use the double description method
regardless. Alternative algorithms will be implemented in future
versions of Regina.

For details on the modified double description method, see B. A.
Burton, "Optimizing the double description method for normal surface
enumeration", Mathematics of Computation 79 (2010), pp. 453-484.)doc";

}

namespace HyperListFlags_ {

// Docstring regina::python::doc::HyperListFlags_::HS_CUSTOM
static const char *HS_CUSTOM =
R"doc(Indicates some other type of list, typically hand-crafted by the user
or built by some customised algorithm.

If this flag is passed to an enumeration routine, it will be ignored.)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_EMBEDDED_ONLY
static const char *HS_EMBEDDED_ONLY =
R"doc(Indicates that this list is restricted to properly embedded
hypersurfaces only.

This flag is incompatible with HS_IMMERSED_SINGULAR.)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_FUNDAMENTAL
static const char *HS_FUNDAMENTAL =
R"doc(Indicates a list of all fundamental normal hypersurfaces, with respect
to the particular normal coordinate system used by the list.

This flag is incompatible with HS_VERTEX.)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_IMMERSED_SINGULAR
static const char *HS_IMMERSED_SINGULAR =
R"doc(Indicates that the scope of this list includes not just properly
embedded hypersurfaces, but also immersed and/or branched
hypersurfaces.

This is no guarantee that the list _contains_ immersed and/or branched
hypersurfaces; it merely states that such hypersurfaces have not been
explicitly excluded (in particular, the prism constraints have not
been enforced).

This flag is incompatible with HS_EMBEDDED_ONLY.)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_LEGACY
static const char *HS_LEGACY =
R"doc(Indicates a list that was constructed using an older prerelease
version of the 4-manifolds code (prior to Regina 5.0).

These prerelease versions did not retain details of how each list was
constructed, beyond whether immersed and/or singular hypersurfaces
were included. Therefore no information is available for such lists,
other than the presence or absence of the HS_EMBEDDED_ONLY and/or
HS_IMMERSED_SINGULAR flags.

If this flag is passed to an enumeration routine, it will be ignored.)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_LIST_DEFAULT
static const char *HS_LIST_DEFAULT =
R"doc(An empty flag, indicating to an enumeration routine that it should use
its default behaviour. The numeric value of this flag is zero (i.e.,
it has no effect when combined with other flags using bitwise OR).)doc";

// Docstring regina::python::doc::HyperListFlags_::HS_VERTEX
static const char *HS_VERTEX =
R"doc(Indicates a list of all vertex normal hypersurfaces, with respect to
the particular normal coordinate system used by the list.

This flag is incompatible with HS_FUNDAMENTAL.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

