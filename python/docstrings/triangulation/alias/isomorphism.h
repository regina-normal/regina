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

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

