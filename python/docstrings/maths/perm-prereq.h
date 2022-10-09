/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PermCodeType
static const char *PermCodeType =
R"doc(Represents the different kinds of internal permutation codes that are
used in Regina's various Perm<n> template classes. See the Perm<n>
class notes for more information on exactly how these codes are
constructed. The class constant Perm<n>::codeType indicates which type
of code is used for which *n*.)doc";

namespace PermCodeType_ {

// Docstring regina::python::doc::PermCodeType_::PERM_CODE_IMAGES
static const char *PERM_CODE_IMAGES =
R"doc(This is a permutation code that packs the images of 0,...,*n*-1 into a
single native integer using a handful of bits per image. Such codes
are easier to manipulate on an element-by-element basis.

Codes of this type can always be queried using Perm<n>::permCode(),
and permutations can be recreated from them using
Perm<n>::fromPermCode().)doc";

// Docstring regina::python::doc::PermCodeType_::PERM_CODE_INDEX
static const char *PERM_CODE_INDEX =
R"doc(This is a permutation code that stores the index into the full
permutation group *S_n*. Such codes typically require fewer bytes and
are packed together, making them ideal for working with lookup tables.

Codes of this type can be queried using Perm<n>::SnIndex(), and
permutations can be recreated from them by indexing into Perm<n>::Sn.

.. warning::
    The routines Perm<n>::permCode() and Perm<n>::fromPermCode() will
    still be present, but in some classes (e.g., Perm<4> and Perm<5>),
    these are legacy routines that refer to different types of codes.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

