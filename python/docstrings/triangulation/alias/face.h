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

struct SimplexVoid {

// Docstring regina::python::doc::alias::SimplexVoid::__class
static constexpr const char __class[] =
R"doc(Helper class that provides a dimension-specific alias for simplex(),
where reasonable.

This is inherited by the class *Derived*, which must provide a routine
of the form ``Simplex<dim>* simplex() const``.

The name of the alias is determined by the dimension *dim*, and the
alias is only provided for those dimensions *dim* that are
sufficiently small.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::__class_2
static constexpr const char __class_2[] =
R"doc(Helper class that provides a dimension-specific alias for simplex().

This is inherited by the class *Derived*, which must provide a routine
of the form ``Simplex<dim>* simplex() const``.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::__class_3
static constexpr const char __class_3[] =
R"doc(Helper class that provides a dimension-specific alias for simplex().

This is inherited by the class *Derived*, which must provide a routine
of the form ``Simplex<dim>* simplex() const``.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::__class_4
static constexpr const char __class_4[] =
R"doc(Helper class that provides a dimension-specific alias for simplex().

This is inherited by the class *Derived*, which must provide a routine
of the form ``Simplex<dim>* simplex() const``.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::pentachoron
static constexpr const char pentachoron[] =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::tetrahedron
static constexpr const char tetrahedron[] =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

// Docstring regina::python::doc::alias::SimplexVoid::triangle
static constexpr const char triangle[] =
R"doc(A dimension-specific alias for simplex().

See simplex() for further information.)doc";

}; // struct SimplexVoid

} // namespace alias

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

