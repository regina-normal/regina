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
constexpr const char *FaceNumber =
R"doc(Helper class that provides a dimension-specific alias for face(),
where reasonable.

This is inherited by the class *Derived*, which must provide a routine
of the form ``int face() const``.

The name of the alias is determined by the dimension *subdim*, and the
alias is only provided for those dimensions *subdim* that are
sufficiently small.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

