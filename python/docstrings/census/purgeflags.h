/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::CensusPurge
static const char *CensusPurge =
R"doc(Represents different classes of triangulations that may be ignored by
census generation algorithms. This enumeration type is used with
census generation routines, such as those found in
GluingPermSearcher<3>.

Excluding a triangulation from a census is not a promise: for example,
a census that uses the flag *NonMinimal* might include some non-
minimal triangulations and exclude others.

However, _including_ a triangulation _is_ a promise: for example, a
census that uses the flag *NonMinimal* will promise to include every
_minimal_ triangulation.

These values can be combined using the bitwise OR operator (resulting
in an object of type ``Flags<CensusPurge>``). In particular, if a
census generation function takes an argument of type
``Flags<CensusPurge>``, then you can pass a single CensusPurge
constant, or a bitwise combination of such constants ``(flag1 |
flag2)``, or empty braces ``{}`` to indicate no flags at all (which is
equivalent to passing ``CensusPurge::None``).)doc";

// Docstring regina::python::doc::__bor
static const char *__bor =
R"doc(Returns the bitwise OR of the two given flags.

Parameter ``lhs``:
    the first flag to combine.

Parameter ``rhs``:
    the second flag to combine.

Returns:
    the combination of both flags.)doc";

namespace CensusPurge_ {

// Docstring regina::python::doc::CensusPurge_::NonMinimal
static const char *NonMinimal = R"doc(Indicates that non-minimal triangulations may be ignored.)doc";

// Docstring regina::python::doc::CensusPurge_::NonMinimalHyp
static const char *NonMinimalHyp =
R"doc(Indicates that any triangulation that is not a minimal ideal
triangulation of a cusped finite-volume hyperbolic 3-manifold may be
ignored.)doc";

// Docstring regina::python::doc::CensusPurge_::NonMinimalPrime
static const char *NonMinimalPrime =
R"doc(Indicates that any triangulation that is not prime (i.e., can be
written as a non-trivial connected sum), any bounded triangulation
that is reducible over a disc and any triangulation that is non-
minimal may be ignored. Note that this is simply a combination of the
constants *NonMinimal* and *NonPrime*.)doc";

// Docstring regina::python::doc::CensusPurge_::NonPrime
static const char *NonPrime =
R"doc(Indicates that any triangulation that is not prime (i.e., can be
written as a non-trivial connected sum) and any bounded triangulation
that is reducible over a disc may be ignored.)doc";

// Docstring regina::python::doc::CensusPurge_::None
static const char *None = R"doc(Indicates that no triangulations should be ignored.)doc";

// Docstring regina::python::doc::CensusPurge_::P2Reducible
static const char *P2Reducible =
R"doc(Indicates that any triangulation containing an embedded two-sided
projective plane may be ignored.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

