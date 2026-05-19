/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace alias {

// Docstring regina::python::doc::alias::FaceNumber
inline constexpr const char FaceNumber[] =
R"doc(Helper class that provides a dimension-specific alias for face(),
where reasonable.

This is inherited by the class *Derived*, which must provide a routine
of the form ``int face() const``.

The name of the alias is determined by the dimension *subdim*, and the
alias is only provided for those dimensions *subdim* that are
sufficiently small.)doc";

}

namespace alias::FaceNumber_ {

// Docstring regina::python::doc::alias::FaceNumber_::edge
inline constexpr const char edge[] =
R"doc(A dimension-specific alias for face().

See face() for further information.)doc";

// Docstring regina::python::doc::alias::FaceNumber_::pentachoron
inline constexpr const char pentachoron[] =
R"doc(A dimension-specific alias for face().

See face() for further information.)doc";

// Docstring regina::python::doc::alias::FaceNumber_::tetrahedron
inline constexpr const char tetrahedron[] =
R"doc(A dimension-specific alias for face().

See face() for further information.)doc";

// Docstring regina::python::doc::alias::FaceNumber_::triangle
inline constexpr const char triangle[] =
R"doc(A dimension-specific alias for face().

See face() for further information.)doc";

// Docstring regina::python::doc::alias::FaceNumber_::vertex
inline constexpr const char vertex[] =
R"doc(A dimension-specific alias for face().

See face() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

