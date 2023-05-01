/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::CensusPurgeFlags
static const char *CensusPurgeFlags =
R"doc(Represents different classes of triangulations that may be ignored by
census generation algorithms.

Excluding a triangulation from a census is not a promise: for example,
a census that uses the flag PURGE_NON_MINIMAL might include some non-
minimal triangulations and exclude others.

However, _including_ a triangulation _is_ a promise: for example, a
census that uses the flag PURGE_NON_MINIMAL will promise to include
every _minimal_ triangulations.

These flags can be combined using the bitwise OR operator. See (for
example) the GluingPermSearcher<3> constructor documentation for
further details on how these flags are used.)doc";

// Docstring regina::python::doc::__bor
static const char *__bor =
R"doc(Returns the bitwise OR of the two given flags.

Parameter ``lhs``:
    the first flag to combine.

Parameter ``rhs``:
    the second flag to combine.

Returns:
    the combination of both flags.)doc";

namespace CensusPurgeFlags_ {

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_NONE
static const char *PURGE_NONE = R"doc(Indicates that no triangulations should be ignored.)doc";

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_NON_MINIMAL
static const char *PURGE_NON_MINIMAL = R"doc(Indicates that non-minimal triangulations may be ignored.)doc";

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_NON_MINIMAL_HYP
static const char *PURGE_NON_MINIMAL_HYP =
R"doc(Indicates that any triangulation that is not a minimal ideal
triangulation of a cusped finite-volume hyperbolic 3-manifold may be
ignored.)doc";

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_NON_MINIMAL_PRIME
static const char *PURGE_NON_MINIMAL_PRIME =
R"doc(Indicates that any triangulation that is not prime (i.e., can be
written as a non-trivial connected sum), any bounded triangulation
that is reducible over a disc and any triangulation that is non-
minimal may be ignored. Note that this is simply a combination of the
constants *PURGE_NON_MINIMAL* and *PURGE_NON_PRIME*.)doc";

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_NON_PRIME
static const char *PURGE_NON_PRIME =
R"doc(Indicates that any triangulation that is not prime (i.e., can be
written as a non-trivial connected sum) and any bounded triangulation
that is reducible over a disc may be ignored.)doc";

// Docstring regina::python::doc::CensusPurgeFlags_::PURGE_P2_REDUCIBLE
static const char *PURGE_P2_REDUCIBLE =
R"doc(Indicates that any triangulation containing an embedded two-sided
projective plane may be ignored.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

