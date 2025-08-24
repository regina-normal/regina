/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::IsoSigPrintable
constexpr const char *IsoSigPrintable =
R"doc(The default encoding to use for isomorphism signatures.

This printable encoding is consistent with the original isomorphism
signatures that were implemented in Regina 4.90. It represents an
isomorphism signature as a std::string, using only printable
characters from the 7-bit ASCII range.

As of Regina 7.4, if *supportLocks* is ``True`` (the default), then
this encoding will also encode any simplex and/or facet locks into the
isomorphism signature. If *supportLocks* is ``False``, and/or if the
triangulation in question does not actually have any simplex and/or
facet locks, then the resulting signature will be the same as produced
by earlier versions of Regina, before locks were implemented.

This class is designed to be used as a template parameter for
Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
Typical users would have no need to create objects of this class or
call any of its functions directly.

Python:
    Python does not support templates. For encodings that do support
    locks (the default), Python users can just append the dimension as
    a suffix (e.g., IsoSigPrintable2 and IsoSigPrintable3 for
    dimensions 2 and 3). For encodings that do not support locks,
    Python users should use the type aliases IsoSigPrintableLockFree2,
    IsoSigPrintableLockFree3, and so on.)doc";

namespace IsoSigPrintable_ {

// Docstring regina::python::doc::IsoSigPrintable_::emptySig
constexpr const char *emptySig =
R"doc(Returns the isomorphism signature of an empty *dim*-dimensional
triangulation.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::encode
constexpr const char *encode =
R"doc(Encodes data for a single connected component of a *dim*-dimensional
triangulation.

The description consists of several arrays, describing facets of the
top-dimensional simplices, as well as the ways that these facets are
glued together and any simplex and/or facet locks. Which array
elements represent which facets/gluings is an implementation detail;
the purpose of this routine is simply to encode the given information.
See the isoSig() implementation for further details.

Python:
    The arrays *facetAction*, *joinDest*, *joinGluing* and *lockMasks*
    should each be passed as Python lists of integers; the argument
    *lockMasks* may be ``None``. The arguments *nFacetActions* and
    *nJoins* are not present, since Python lists already know their
    own sizes.

Parameter ``size``:
    the number of top-dimensional simplices in the component.

Parameter ``nFacetActions``:
    the size of the array *facetAction*.

Parameter ``facetAction``:
    an array of size *nFacetActions*, where each element is either 0,
    1 or 2, respectively representing a boundary facet, a facet joined
    to a new simplex, or a facet joined to a simplex that has already
    been seen.

Parameter ``nJoins``:
    the size of the arrays *joinDest* and *joinGluing*.

Parameter ``joinDest``:
    an array whose elements are indices of top-dimensional simplices
    to which gluings are being made.

Parameter ``joinGluing``:
    an array of gluing permutations.

Parameter ``lockMasks``:
    an array that holds the lock masks for all top-dimensional
    simplices in the component, as returned by
    ``Simplex<dim>::lockMask()``. This array should have length
    *size*. If no simplices have any locks at all, then this argument
    must be ``None``.

Returns:
    the encoding of the component being described.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

