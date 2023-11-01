/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Algorithm
static const char *Algorithm =
R"doc(Represents various classes of algorithms that Regina can use for
computations. A function that takes an Algorithm argument need not
support all types of algorithm - if an unsupported algorithm is passed
then Regina will fall back to Algorithm::Default.

This enumeration type does _not_ allow constants to be combined using
the OR operator.

For some computations where the user can exert more fine-grained
control over the underlying algorithm, a more specialised enumeration
type is used instead. For example, see the NormalSurfaces class
constructor, which uses the specialised algorithm types NormalAlg and
NormalAlgFlags.)doc";

// Docstring regina::python::doc::Language
static const char *Language = R"doc(Represents different programming languages that Regina supports.)doc";

// Docstring regina::python::doc::maxDim
static const char *maxDim =
R"doc(Indicates that largest dimension of triangulation that Regina can work
with.

If Regina was built with the ``REGINA_HIGHDIM`` option, then this will
be 15; otherwise it will be 8 (the default for ordinary builds).

Returns:
    Regina's maximum dimension of triangulation.)doc";

// Docstring regina::python::doc::standardDim
static const char *standardDim =
R"doc(Indicates whether the given dimension is one of Regina's standard
dimensions. Standard dimensions offer significantly richer
functionality for triangulations than generic dimensions.

Parameter ``dim``:
    the dimension in question.

Returns:
    ``True`` if and only if *dim* is one of Regina's standard
    dimensions.)doc";

namespace Algorithm_ {

// Docstring regina::python::doc::Algorithm_::Backtrack
static const char *Backtrack =
R"doc(An optimised backtracking algorithm. This typically works over some
search tree (often of exponential size or worse), but include
significant optimisations to prune the search tree and/or cache
computations where possible.)doc";

// Docstring regina::python::doc::Algorithm_::Default
static const char *Default =
R"doc(The default algorithm. Here Regina will choose whichever algorithm it
thinks (rightly or wrongly) is most appropriate.)doc";

// Docstring regina::python::doc::Algorithm_::Naive
static const char *Naive =
R"doc(A naive algorithm. This typically works directly with the underlying
definitions (e.g., computing Turaev-Viro as a state sum), without
further optimisations.

.. warning::
    Naive algorithms should only be used for comparison and
    experimentation. Due to their slow performance, they are not
    suitable for "real" applications.)doc";

// Docstring regina::python::doc::Algorithm_::Treewidth
static const char *Treewidth =
R"doc(A treewidth-based algorithm. Typically this uses dynamic programming
over a tree decomposition of some underlying graph. Such algorithms
are often fast for triangulations or links with small treewidth, but
may require large amounts of memory.)doc";

}

namespace Language_ {

// Docstring regina::python::doc::Language_::Current
static const char *Current =
R"doc(The programming language currently being used.

When writing C++ code, this is equivalent to Language::Cxx. When using
the Python bindings, this is equivalent to Language::Python.

.. warning::
    For developers who are _writing_ the Python bindings, remember
    that your binding code (which is written in C++) will treat this
    as Language::Cxx, whereas your Python users will expect
    Language::Python. See the code for binding for Link::source() as
    an example of where this issue arises and how it is resolved.)doc";

// Docstring regina::python::doc::Language_::Cxx
static const char *Cxx = R"doc(The C++ programming language.)doc";

// Docstring regina::python::doc::Language_::Python
static const char *Python = R"doc(The Python programming language.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

