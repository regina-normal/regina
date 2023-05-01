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

// Docstring regina::python::doc::alias::SimplexVoid
constexpr const char *SimplexVoid =
R"doc(Helper class that provides a dimension-specific alias for simplex(),
where reasonable.

This is inherited by the class *Derived*, which must provide a routine
of the form ``Simplex<dim>* simplex() const``.

The name of the alias is determined by the dimension *dim*, and the
alias is only provided for those dimensions *dim* that are
sufficiently small.)doc";

}

namespace alias::SimplexVoid_ {

// Docstring regina::python::doc::alias::SimplexVoid_::pentachoron
constexpr const char *pentachoron =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

// Docstring regina::python::doc::alias::SimplexVoid_::tetrahedron
constexpr const char *tetrahedron =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

// Docstring regina::python::doc::alias::SimplexVoid_::triangle
constexpr const char *triangle =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

