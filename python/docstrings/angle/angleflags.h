/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::AngleAlgFlags
static const char *AngleAlgFlags =
R"doc(Represents options and variants of algorithms for enumerating various
types of angle structures on 3-manifold triangulations.

These options can be combined using the bitwise OR operator, and then
passed to enumeration routines such as the AngleStructures class
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

namespace AngleAlgFlags_ {

// Docstring regina::python::doc::AngleAlgFlags_::AS_ALG_CUSTOM
static const char *AS_ALG_CUSTOM =
R"doc(Indicates that an angle structure list was built using a customised
algorithm. In such cases, no further details on the algorithm are
available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::AngleAlgFlags_::AS_ALG_DD
static const char *AS_ALG_DD =
R"doc(When enumerating vertex or taut angle structures, this flag indicates
that a modified double description method should be used.

This is currently the only supported algorithm for enumerating all
vertex angle structures (not just taut structures).

This flag is incompatible with AS_ALG_TREE.)doc";

// Docstring regina::python::doc::AngleAlgFlags_::AS_ALG_DEFAULT
static const char *AS_ALG_DEFAULT =
R"doc(An empty flag, indicating to an enumeration routine that it should use
its default behaviour. The numeric value of this flag is zero (i.e.,
it has no effect when combined with other flags using bitwise OR).)doc";

// Docstring regina::python::doc::AngleAlgFlags_::AS_ALG_LEGACY
static const char *AS_ALG_LEGACY =
R"doc(Indicates that an angle structure list was enumerated using an older
version of Regina (6.0.1 or earlier).

These older versions did not retain details of the algorithm used to
build each list, and so in such cases no further algorithmic
information is available.

If this flag is passed to an enumeration algorithm, it will be
ignored.)doc";

// Docstring regina::python::doc::AngleAlgFlags_::AS_ALG_TREE
static const char *AS_ALG_TREE =
R"doc(When enumerating taut angle structures, this flag indicates that the
tree traversal algorithm should be used.

This algorithm is based on linear and integer programming techniques,
and has many desirable properties including a relatively low overhead.
Enumeration algorithms will use it if possible unless a different
method is explicitly requested.

This is a variant of the tree traversal algorithm from B. A. Burton
and M. Ozlen, "A tree traversal algorithm for decision problems in
knot theory and 3-manifold topology", Algorithmica 65 (2013), pp.
772-801.

This flag is incompatible with AS_ALG_DD.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

