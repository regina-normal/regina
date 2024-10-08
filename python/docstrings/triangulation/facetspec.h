/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FacetSpec
static const char *FacetSpec =
R"doc(A lightweight class used to refer to a particular facet of a
particular top-dimensional simplex in a *dim*-dimensional
triangulation. Only the simplex index and the facet number are stored.

Facilities are provided for iterating through simplex facets. With
this in mind, it is also possible to represent the overall boundary, a
past-the-end value and a before-the-start value.

When iterating through the simplex facets, the facets will be ordered
first by simplex index and then by facet number. The overall boundary
appears after all other simplex facets.

If there are *n* simplices, the simplices will be numbered from 0 to
*n*-1 inclusive. The boundary will be represented as simplex *n*,
facet 0. The past-the-end value will be represented as simplex *n*,
facet 1, and the before-the-start value will be represented as simplex
-1, facet *dim*.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., FacetSpec2 and
    FacetSpec3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.)doc";

namespace FacetSpec_ {

// Docstring regina::python::doc::FacetSpec_::__cmp
static const char *__cmp =
R"doc(Compares two specifiers, following the same ordering that is used for
iteration.

Specifically: specifiers are compared first by simplex number and then
by facet number. The overall boundary is considered greater than any
"real" simplex facet, the before-the-start value is considered smaller
than anything else, and the past-the-end value is considered greater
than anything else.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the specifier to compare with this.

Returns:
    The result of the comparison between this and the given specifier.)doc";

// Docstring regina::python::doc::FacetSpec_::__copy
static const char *__copy =
R"doc(Creates a new specifier referring to the same simplex facet as the
given specifier.

Parameter ``cloneMe``:
    the specifier to clone.)doc";

// Docstring regina::python::doc::FacetSpec_::__dec
static const char *__dec =
R"doc(Decrements this specifier. It will be changed to point to the previous
simplex facet.

Facets are ordered first by simplex index and then by facet number.
The overall boundary appears after all other facets.

Precondition:
    This specifier is not before-the-start.

Python:
    This routine is named dec() since python does not support the
    decrement operator.

Returns:
    A copy of this specifier before it was decremented.)doc";

// Docstring regina::python::doc::FacetSpec_::__default
static const char *__default =
R"doc(Creates a new specifier with no initialisation. This specifier must be
initialised before it is used.)doc";

// Docstring regina::python::doc::FacetSpec_::__eq
static const char *__eq =
R"doc(Determines if this and the given specifier are identical.

Parameter ``rhs``:
    the specifier to compare with this.

Returns:
    ``True`` if and only if this and the given specifier are equal.)doc";

// Docstring regina::python::doc::FacetSpec_::__inc
static const char *__inc =
R"doc(Increments this specifier. It will be changed to point to the next
simplex facet.

Facets are ordered first by simplex index and then by facet number.
The overall boundary appears after all other facets.

Precondition:
    This specifier is not past-the-end.

Python:
    This routine is named inc() since python does not support the
    increment operator.

Returns:
    A copy of this specifier before it was incremented.)doc";

// Docstring regina::python::doc::FacetSpec_::__init
static const char *__init =
R"doc(Creates a new specifier referring to the given facet of the given
simplex.

Parameter ``newSimp``:
    the given simplex; see the class notes for allowable values of
    this parameter.

Parameter ``newFacet``:
    the given facet; this should be between 0 and *dim* inclusive.)doc";

// Docstring regina::python::doc::FacetSpec_::isBeforeStart
static const char *isBeforeStart =
R"doc(Determines if this specifier represents a before-the-start value.

Returns:
    ``True`` if and only if this specifier is before-the-start.)doc";

// Docstring regina::python::doc::FacetSpec_::isBoundary
static const char *isBoundary =
R"doc(Determines if this specifier represents the overall boundary.

Parameter ``nSimplices``:
    the number of simplices under consideration. Note that the
    boundary is represented in this specifier as simplex *nSimplices*,
    facet 0.

Returns:
    ``True`` if and only if this specifier represents the overall
    boundary.)doc";

// Docstring regina::python::doc::FacetSpec_::isPastEnd
static const char *isPastEnd =
R"doc(Determines if this specifier represents a past-the-end value. You can
optionally declare the overall boundary to be past-the-end as well as
the already predefined past-the-end value.

Parameter ``nSimplices``:
    the number of simplices under consideration. Note that past-the-
    end is represented in this specifier as simplex *nSimplices*,
    facet 1.

Parameter ``boundaryAlso``:
    ``True`` if the overall boundary should be considered past-the-end
    in addition to the predefined past-the-end value.

Returns:
    ``True`` if and only if this specifier is past-the-end.)doc";

// Docstring regina::python::doc::FacetSpec_::setBeforeStart
static const char *setBeforeStart = R"doc(Sets this specifier to before-the-start.)doc";

// Docstring regina::python::doc::FacetSpec_::setBoundary
static const char *setBoundary =
R"doc(Sets this specifier to the overall boundary.

Parameter ``nSimplices``:
    the number of simplices under consideration. Note that the
    boundary is represented in this specifier as simplex *nSimplices*,
    facet 0.)doc";

// Docstring regina::python::doc::FacetSpec_::setFirst
static const char *setFirst = R"doc(Sets this specifier to the first facet of the first simplex.)doc";

// Docstring regina::python::doc::FacetSpec_::setPastEnd
static const char *setPastEnd =
R"doc(Sets this specifier to past-the-end.

Parameter ``nSimplices``:
    the number of simplices under consideration. Note that past-the-
    end is represented in this specifier as simplex *nSimplices*,
    facet 1.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

