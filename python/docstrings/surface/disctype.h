/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::DiscType
static const char *DiscType =
R"doc(Identifies a single normal or almost normal disc type within a
triangulation.

A disc type is identified by a tetrahedron index (the data member
*tetIndex*), and a disc type within that tetrahedron (the data member
*type*). The latter could mean any number of things according to the
application at hand. For instance, if we are tracking quad types then
*type* might be an integer between 0 and 2 inclusive, or if we are
tracking all normal discs in standard coordinates then *type* might be
an integer between 0 and 6 inclusive. Ultimately, the specific meaning
of *type* is left to the user.

It is however assumed that *type* will always be non-negative for
"meaningful" disc types.

This class can also store a *null* disc type; this is obtained by
calling the default constructor, and it will have a type of -1 and a
tetrahedron index of 0. You can test for a null disc type by casting
to ``bool``.

Note that this class tracks disc *types*, not discs themselves. To
track individual normal discs, see the DiscSpec class instead.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

namespace DiscType_ {

// Docstring regina::python::doc::DiscType_::DiscType
static const char *DiscType = R"doc(Creates a new null disc type, as described in the class notes.)doc";

// Docstring regina::python::doc::DiscType_::DiscType_2
static const char *DiscType_2 =
R"doc(Creates a new disc type initialised with the given values.

Parameter ``newTetIndex``:
    the index within the triangulation of the tetrahedron containing
    this disc type.

Parameter ``newType``:
    the specific disc type within the given tetrahedron; see the class
    notes for the meaning of this field.)doc";

// Docstring regina::python::doc::DiscType_::DiscType_3
static const char *DiscType_3 =
R"doc(Creates a copy of the given disc type.

Parameter ``cloneMe``:
    the disc type to clone.)doc";

// Docstring regina::python::doc::DiscType_::__eq
static const char *__eq =
R"doc(Determines if this and the given disc type are identical.

Regarding null disc types: two null DiscType objects that were both
created using the default constructor will be considered equal to each
other, and will not be equal to any "meaningful" disc type (where
*type* is non-negative).

Returns:
    ``True`` if this and the given disc type are identical, or
    ``False`` if they are different.)doc";

// Docstring regina::python::doc::DiscType_::__lt
static const char *__lt =
R"doc(Provides an ordering of disc types. Types are ordered first by
*tetrahedron* and then by *type*. The null disc type is considered
less than all "meaningful" disc types.

Returns:
    ``True`` if this disc type appears before the given disc type in
    the ordering, or ``False`` if not.)doc";

// Docstring regina::python::doc::DiscType_::__ne
static const char *__ne =
R"doc(Determines if this and the given disc type are different.

This is the negation of the equality test; see operator == for further
details.

Returns:
    ``True`` if this and the given disc type are different, or
    ``False`` if they are identical.)doc";

// Docstring regina::python::doc::DiscType_::operator_bool
static const char *operator_bool =
R"doc(Returns ``True`` if this disc type is non-null.

The implementation will assume that a non-null disc type has non-
negative *type* (as explained in the class notes).

Returns:
    ``True`` if and only if this is not a null disc type.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

