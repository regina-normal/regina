/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::IsoSigClassic
static const char *IsoSigClassic =
R"doc(The default signature type to use for isomorphism signatures of
triangulations.

This signature type is slower to use than others; its main benefit is
that it is consistent with the original isomorphism signatures that
were implemented in Regina 4.90.

See the IsoSigType concept documentation for a discussion of how
isomorphism signature types act as "iterators" through candidate pairs
``(s, p)``, where *s* is a "starting simplex" within some
triangulation component *c*, and *p* describes a "starting labelling"
of its vertices.

This classic signature type is trivial: it considers _all_ possible
pairs ``(s, p)``, without any pruning.

This class is designed to be used as a template parameter for
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``. Typical users would have no
need to create objects of this class or call any of its functions
directly.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g.,

Python:
    Python does not support C++ templates. To _use_ this signature
    type in Python, just call the relevant signature function with no
    extra suffix (e.g., ``Triangulation::isoSig()`` or
    ``Triangulation::isoSigDetail()``), since this signature type is
    the default. To access this type _class_ (which you would
    typically not need to do), append the dimension as a suffix (e.g.,
    IsoSigClassic2 and IsoSigClassic3 for dimensions 2 and 3).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::IsoSigData
static const char *IsoSigData =
R"doc(Holds all of the data required to reconstruct a single non-empty
connected component of a *dim*-dimensional triangulation, up to
relabelling. This is a halfway point between isomorphism signatures
and triangulations: the data is small and uses simple
numerical/combinatorial types, and is easy to encode or decode in an
appropriate signature form (e.g., a base64 string).

Specifically, this data encodes a "compressed" gluings table for a
"canonical" labelling of a triangulation component. Both the notions
of "compressed" and "canonical" involve stepping through the facets of
top-dimensional simplices in the usual order (simplex 0, facets
``0,...,dim``; then simplex 1, facets ``0,...,dim``; and so on). Some
of these facets will be boundary (i.e., unglued), some will be glued
to "new" facets that we have not yet stepped through, and some will be
glued to "old" facets (i.e., we have already seen the same
`(dim-1)`-face from the other side).

To be _canonical_:

_ We require that each time we see a new facet whose gluing partner
comes from a previously-unseen simplex, then that partner simplex uses
the next available simplex label and the gluing uses the identity
permutation. So, for example, the first facet of simplex 0 that is
glued to some _other_ simplex must be glued to simplex 1 using the
identity permutation.

* The condition above ensures that, once we have chosen which top-
  dimensional simplex will be simplex 0 and how its vertices
  ``0,...,dim`` will be labelled, the labelling of the remaining
  simplices and their vertices will be uniquely determined (and can be
  computed in linear time).

* Your chosen signature _type_ may impose further requirements on the
  choice of simplex 0 and its vertex labels; see the IsoSigType
  concept documentation for details.

Given a canonical labelling, a _compressed_ gluings table holds the
following information:

* For each facet *f* that we step through in order, _ignoring_ those
  glued to "old" facets, we record whether *f* is: boundary (type 0);
  glued to a "new" facet of a previously-unseen simplex using the
  identity permutation (type 1); or glued to a "new" facet of a
  simplex that has been seen before (type 2). This is stored in array
  whose length is the total number of `(dim-1)`-faces in the
  triangulation component.

* For each facet of type 2, we record the top-dimensional simplex
  number that it is glued to. This is stored in an array whose length
  is the total number of facets of type 2.

* For each facet of type 2, we record the ordered ``S_n`` index of the
  specific gluing permutation, again using an array whose length is
  the total number of facets of type 2.

* Finally, if (and only if) there are any simplex or facet locks in
  this triangulation component, we also record the lock masks of all
  top-dimensional simplices in order (simplex ``0,1,2,...``).

An isomorphism signature _encoding_ is responsible for encoding this
data in its final signature form (e.g., a base64 string), and the
final isomorphism signature will use the labelling that minimises this
encoding (under the inherent ordering of the resulting signature
type).

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::IsoSigDegrees
static const char *IsoSigDegrees =
R"doc(Defines an alternate type of isomorphism signature based on degree
sequences of *subdim*-faces.

This is a "proof of concept" type that shows how you might speed up
isomorphism signature computations. It requires that the signature
choose a starting simplex whose set of *subdim*-face degrees is
lexicographically minimal amongst all top-dimensional simplices. The
hope is that this eliminates a large number of potential starting
simplices without adding an enormous amount of computational overhead.

See the IsoSigType concept documentation for a discussion of how
isomorphism signature types act as "iterators" through candidate pairs
``(s, p)``, where *s* is a "starting simplex" within some
triangulation component *c*, and *p* describes a "starting labelling"
of its vertices.

This class is designed to be used as a template parameter for
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``. Typical users would have no
need to create objects of this class or call any of its functions
directly.

Python:
    Python does not support C++ templates, and there are far too many
    of these classes to wrap. Currently Python supports only the cases
    where *subdim* is 1 or *dim*-2, using the type aliases
    IsoSigEdgeDegrees and IsoSigRidgeDegrees respectively (these cover
    all the variants that Regina currently uses internally). To _use_
    the corresponding signature type, call the relevant signature
    function with an extra ``_EdgeDegrees`` or ``_RidgeDegrees``
    suffix (e.g., ``Triangulation::isoSig_EdgeDegrees()`` or
    ``Triangulation::isoSigDetail_EdgeDegrees()``). To access the
    corresponding signature type _class_ (which you would typically
    not need to do), append the dimension as a suffix to the type
    alias (e.g., ``IsoSigEdgeDegrees3``).)doc";

// Docstring regina::python::doc::IsoSigPrintable
static const char *IsoSigPrintable =
R"doc(The default encoding to use for isomorphism signatures.

This printable encoding is consistent with the original isomorphism
signatures that were implemented in Regina 4.90. It represents an
isomorphism signature as a ``std::string``, using only printable
characters from the 7-bit ASCII range.

As of Regina 7.4, if *supportLocks* is ``True`` (the default), then
this encoding will also encode any simplex and/or facet locks into the
isomorphism signature. If *supportLocks* is ``False``, and/or if the
triangulation in question does not actually have any simplex and/or
facet locks, then the resulting signature will be the same as produced
by earlier versions of Regina, before locks were implemented.

See the IsoSigEncoding concept documentation for general details on
encodings for isomorphism signatures.

This class is designed to be used as a template parameter for
``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``. Typical users would have no
need to call any of its functions directly.

Python:
    Python does not support C++ templates. To _use_ this encoding in
    Python: if *supportLocks* is ``True`` (the default), call the
    relevant signature function with no extra suffix (e.g.,
    ``Triangulation::isoSig()`` or ``Triangulation::isoSigDetail()``);
    if *supportLocks* is ``False``, use an extra ``_LockFree`` suffix
    (e.g., ``Triangulation::isoSig_LockFree()`` or
    ``Triangulation::isoSigDetail_LockFree()``). To access this
    encoding _class_ (which you would typically not need to do): if
    *supportLocks* is ``True``, append the dimension as a suffix
    (e.g., ``IsoSigPrintable2`` and ``IsoSigPrintable3`` for
    dimensions 2 and 3); if *supportLocks* is ``False``, use the type
    aliases ``IsoSigPrintableLockFree2``,
    ``IsoSigPrintableLockFree3``, and so on.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace IsoSigClassic_ {

// Docstring regina::python::doc::IsoSigClassic_::__init
static const char *__init =
R"doc(Initialises this "iterator" to the first candidate pair ``(s, p)`` for
the given triangulation component.

Parameter ``comp``:
    the triangulation component that we are examining.)doc";

// Docstring regina::python::doc::IsoSigClassic_::next
static const char *next =
R"doc(Advances this "iterator" to the next candidate pair ``(s, p)``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    ``True`` if this was successful, or ``False`` if there are no more
    candidate pairs.)doc";

// Docstring regina::python::doc::IsoSigClassic_::perm
static const char *perm =
R"doc(Returns the current starting labelling *p* of the vertices of the
current starting simplex *s*. This maps the vertices of *s* to the
"canonical" labels ``0,1,...,dim``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the starting labelling *p*.)doc";

// Docstring regina::python::doc::IsoSigClassic_::simplex
static const char *simplex =
R"doc(Returns the index of the current starting simplex *s* within the
relevant triangulation component.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the index within the component of the starting simplex *s*.)doc";

}

namespace IsoSigData_ {

// Docstring regina::python::doc::IsoSigData_::__copy
static const char *__copy = R"doc(Makes a new deep copy of the given data set.)doc";

// Docstring regina::python::doc::IsoSigData_::__eq
static const char *__eq =
R"doc(Determines whether this and the given triangulation component data
hold identical information.

Returns:
    ``True`` if and only if this and the given data are identical.)doc";

// Docstring regina::python::doc::IsoSigData_::__init
static const char *__init =
R"doc(Allocates space for a data set for the given triangulation component.

The arrays that make up this data set will be initialised to the
correct sizes, but the individual array elements will be left
uninitialised. You will need to call fillFrom() before this data set
can be used (i.e., before any other methods are called).

Parameter ``comp``:
    the triangulation component that we intend to encode.)doc";

// Docstring regina::python::doc::IsoSigData_::adjacentGluings
static const char *adjacentGluings =
R"doc(Gives read-only access to the array of gluing permutations, as
described in the class notes.

The length of this array will be the number of facets of top-
dimensional simplices of type 2 (as described in the class notes, and
as indicated by the facetTypes() array).

Each element of this array holds the ordered ``S_n`` index of the
gluing permutation for the relevant facet.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing permutations.)doc";

// Docstring regina::python::doc::IsoSigData_::adjacentSimplices
static const char *adjacentSimplices =
R"doc(Gives read-only access to the array of gluing destinations, as
described in the class notes.

The length of this array will be the number of facets of top-
dimensional simplices of type 2 (as described in the class notes, and
as indicated by the facetTypes() array).

Each element of this array indicates the index of the top-dimensional
simplex to which the relevant facet is glued.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing destinations.)doc";

// Docstring regina::python::doc::IsoSigData_::facetTypes
static const char *facetTypes =
R"doc(Gives read-only access to the array of facet types, as described in
the class notes.

This will be an array of integers, each taking the value 0, 1 or 2,
and each indicating whether a particular facet of a top-dimensional
simplex is boundary (0), glued to a "new" facet of a previously-unseen
simplex (1), or glued to a "new" facet of a simplex that has been seen
before (2). Facets that are glued to "old" facets (i.e., facets that
represent internal `(dim-1)`-faces of the triangulation and have
already been seen from the other side) are not represented in this
array.

The length of the array will be the total number of `(dim-1)`-faces in
the triangulation component being encoded.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of facet types.)doc";

// Docstring regina::python::doc::IsoSigData_::fillFrom
static const char *fillFrom =
R"doc(Determines the (unique) canonical labelling of the triangulation
component for a particular choice of simplex 0 and vertices
``0,...,dim``, and fills this structure with the corresponding
compressed gluings table.

See the class notes for full details on canonical labellings. In the
gluings table that we build here, *simplex* will be labelled as
simplex 0, and its vertices ``vertices[0],...,vertices[dim]`` will be
labelled as vertices ``0,...,dim``.

This routine may be (and typically will be) called many times for
different choices of *simplex* and *vertices*; this is perfectly safe,
and each subsequent call to fillFrom() will overwrite the previously-
stored gluings table.

Precondition:
    The given simplex belongs to the triangulation component that was
    originally passed to the IsoSigData constructor.

Precondition:
    If the isomorphism *relabelling* is non-null, then it must have
    been constructed for *size* top-dimensional simplices, where
    *size* is the size of the entire _triangulation_, not just the
    component being encoded.

Parameter ``simplex``:
    any top-dimensional simplex in the triangulation component being
    encoded.

Parameter ``vertices``:
    any ordering of the vertices of the given simplex.

Parameter ``relabelling``:
    if non-null, this isomorphism will be filled with the isomorphism
    from the original triangulation to the canonical labelling. If the
    underlying triangulation has multiple components, then only the
    information relating to this component will be changed; other
    simplex/vertex images in *relabelling* will be left untouched.)doc";

// Docstring regina::python::doc::IsoSigData_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given triangulation component data sets.

This global routine simply calls ``IsoSigData<dim>::swap()``; it is
provided so that IsoSigData meets the C++ Swappable requirements.

Parameter ``a``:
    the first component data set whose contents should be swapped.

Parameter ``b``:
    the second component data set whose contents should be swapped.)doc";

// Docstring regina::python::doc::IsoSigData_::hasLocks
static const char *hasLocks =
R"doc(Indicates whether the triangulation component being encoded has any
simplex or facet locks.

This will be ``True`` if and only if the array returned by locks() is
non-empty.

Returns:
    a reference to the (possibly empty) array of lock masks.)doc";

// Docstring regina::python::doc::IsoSigData_::locks
static const char *locks =
R"doc(Gives read-only access to the array of lock masks, as described in the
class notes.

If the triangulation component being encoded has no simplex or facet
locks, then this will be an empty array. Otherwise it will be an array
whose length is the total number of top-dimensional simplices in the
component, and whose elements are the lock masks for the individual
simplices (as indicated by `Simplex<dim>`::lockMask()`).

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of lock masks.)doc";

// Docstring regina::python::doc::IsoSigData_::size
static const char *size =
R"doc(Returns the total number of top-dimensional simplices in the connected
triangulation component that this data set describes.

Returns:
    the total number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::IsoSigData_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given triangulation component data.

This routine will behave correctly if *other* is in fact this data
set.

Parameter ``other``:
    the component data whose contents should be swapped with this.)doc";

}

namespace IsoSigDegrees_ {

// Docstring regina::python::doc::IsoSigDegrees_::__init
static const char *__init =
R"doc(Initialises this "iterator" to the first candidate pair ``(s, p)`` for
the given triangulation component.

Parameter ``comp``:
    the triangulation component that we are examining.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::next
static const char *next =
R"doc(Advances this "iterator" to the next candidate pair ``(s, p)``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    ``True`` if this was successful, or ``False`` if there are no more
    candidate pairs.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::perm
static const char *perm =
R"doc(Returns the current starting labelling *p* of the vertices of the
current starting simplex *s*. This maps the vertices of *s* to the
"canonical" labels ``0,1,...,dim``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the starting labelling *p*.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::simplex
static const char *simplex =
R"doc(Returns the index of the current starting simplex *s* within the
relevant triangulation component.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the index within the component of the starting simplex *s*.)doc";

}

namespace IsoSigPrintable_ {

// Docstring regina::python::doc::IsoSigPrintable_::encode
static const char *encode =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as an isomorphism signature.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::encodeEmpty
static const char *encodeEmpty =
R"doc(Encodes the isomorphism signature of the empty *dim*-dimensional
triangulation.

Note that IsoSigPrintable does _not_ return an empty signature for
this; instead it returns the non-empty string ``a``.

Returns:
    the isomorphism signature of the empty triangulation.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

