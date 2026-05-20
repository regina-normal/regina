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

struct IsomorphismImage {

// Docstring regina::python::doc::alias::IsomorphismImage::__class
static constexpr const char __class[] =
R"doc(Helper class that provides dimension-specific aliases for both const
and non-const versions of ``simpImage(size_t)`` and
``facetPerm(size_t)``, where reasonable, for isomorphisms in dimension
*dim*.

This is inherited by the class *Derived*, which must provide functions
of the form ``int& simpImage(size_t)``, ``int simpImage(size_t)
const``, ``Perm<dim+1>& facetPerm(size_t)`` and ``Perm<dim+1>
facetPerm(size_t) const``.

The names of the aliases are determined by the dimension *dim*, and
these aliases are only provided for sufficiently small *dim*.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::__class_2
static constexpr const char __class_2[] =
R"doc(Helper class that provides dimension-specific aliases for both const
and non-const versions of ``simpImage(size_t)`` and
``facetPerm(size_t)``.

This is inherited by the class *Derived*, which must provide functions
of the form ``int& simpImage(size_t)``, ``int simpImage(size_t)
const``, ``Perm<dim+1>& facetPerm(size_t)`` and ``Perm<dim+1>
facetPerm(size_t) const``.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::__class_3
static constexpr const char __class_3[] =
R"doc(Helper class that provides dimension-specific aliases for both const
and non-const versions of ``simpImage(size_t)`` and
``facetPerm(size_t)``.

This is inherited by the class *Derived*, which must provide functions
of the form ``int& simpImage(size_t)``, ``int simpImage(size_t)
const``, ``Perm<dim+1>& facetPerm(size_t)`` and ``Perm<dim+1>
facetPerm(size_t) const``.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::__class_4
static constexpr const char __class_4[] =
R"doc(Helper class that provides dimension-specific aliases for both const
and non-const versions of ``simpImage(size_t)``.

This is inherited by the class *Derived*, which must provide functions
of the form ``int& simpImage(size_t)`` and ``int simpImage(size_t)
const``.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::edgePerm
static constexpr const char edgePerm[] =
R"doc(A dimension-specific alias for facetPerm().

See facetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::facePerm
static constexpr const char facePerm[] =
R"doc(A dimension-specific alias for facetPerm().

See facetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::pentImage
static constexpr const char pentImage[] =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::setEdgePerm
static constexpr const char setEdgePerm[] =
R"doc(A dimension-specific alias for setFacetPerm().

See setFacetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::setFacePerm
static constexpr const char setFacePerm[] =
R"doc(A dimension-specific alias for setFacetPerm().

See setFacetPerm() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::setPentImage
static constexpr const char setPentImage[] =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::setTetImage
static constexpr const char setTetImage[] =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::setTriImage
static constexpr const char setTriImage[] =
R"doc(A dimension-specific alias for setSimpImage().

See setSimpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::tetImage
static constexpr const char tetImage[] =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

// Docstring regina::python::doc::alias::IsomorphismImage::triImage
static constexpr const char triImage[] =
R"doc(A dimension-specific alias for simpImage().

See simpImage() for further information.)doc";

}; // struct IsomorphismImage

} // namespace alias

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

