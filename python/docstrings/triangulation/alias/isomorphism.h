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

// Docstring regina::python::doc::alias::IsomorphismImage
constexpr const char *IsomorphismImage =
R"doc(Helper class that provides dimension-specific aliases for both const
and non-const versions of simpImage(size_t) and facetPerm(size_t),
where reasonable, for isomorphisms in dimension *dim*.

This is inherited by the class *Derived*, which must provide functions
of the form ``int& simpImage(size_t)``, ``int simpImage(size_t)
const``, ``Perm<dim+1>& facetPerm(size_t)`` and ``Perm<dim+1>
facetPerm(size_t) const``.

The names of the aliases are determined by the dimension *dim*, and
these aliases are only provided for sufficiently small *dim*.)doc";

}

namespace alias::IsomorphismImage_ {

// Docstring regina::python::doc::alias::IsomorphismImage_::edgePerm
constexpr const char *edgePerm =
R"doc(A dimension-specific alias for facetPerm().

See facetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::facePerm
constexpr const char *facePerm =
R"doc(A dimension-specific alias for facetPerm().

See facetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::pentImage
constexpr const char *pentImage =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::setEdgePerm
constexpr const char *setEdgePerm =
R"doc(A dimension-specific alias for setFacetPerm().

See setFacetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::setFacePerm
constexpr const char *setFacePerm =
R"doc(A dimension-specific alias for setFacetPerm().

See setFacetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::setPentImage
constexpr const char *setPentImage =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::setTetImage
constexpr const char *setTetImage =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::setTriImage
constexpr const char *setTriImage =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::tetImage
constexpr const char *tetImage =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage_::triImage
constexpr const char *triImage =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

