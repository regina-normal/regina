/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::IsoSigBinary
inline constexpr const char IsoSigBinary[] =
R"doc(Encodes isomorphism signatures using small-memory byte-based
encodings. This encoding method is _only_ available for second-
generation signatures, not first-generation signatures.

This encoding algorithm is intended for scenarios where memory needs
to be kept as small as possible. It uses all eight bits per byte (as
opposed to IsoSigPrintable, which only encodes six bits per byte but
creates a printable string as a result).

Any simplex and/or facet locks will be encoded as part of the
signature.

See the IsoSigEncoding concept documentation for general details on
encodings for isomorphism signatures.

This class is designed to be used as a template parameter for second-
generation signature routines in the class ``Triangulation<dim>``,
including ``neoSig()`` and ``neoSigDetail()``. Typical users would
have no need to call any functions from this encoding class directly.

Python:
    To use this encoding in Python, pass IsoSigBinary as a runtime
    argument to the relevant ``Triangulation<dim>`` signature function
    (e.g., ``neoSig()`` or ``neoSigDetail()``).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::IsoSigClassic
inline constexpr const char IsoSigClassic[] =
R"doc(A slow isomorphism signature type that is consistent with the original
isomorphism signatures that were first introduced in Regina 4.90.

This signature type is slow to use and is not recommended for use in
new projects. It is still offered because it reproduces the first-
generation signatures from Regina ≤ 7.x. For backward compatibility,
it remains the default signature type for the first-generation
signature routines ``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::isoSigDetail()``.

The IsoSigType concept documentation explains how an isomorphism
signature type acts as an "iterator" through candidate pairs ``(s,
p)``, where *s* is a "starting simplex" within some triangulation
component *c*, and *p* describes a "starting labelling" of its
vertices. This signature type is trivial: it considers _all_ possible
pairs ``(s, p)``, without any pruning.

This class is designed to be used as a template parameter for
signature routines in the class ``Triangulation<dim>``, including
``isoSig()``, ``neoSig()``, ``isoSigDetail()``, and
``neoSigDetail()``. Typical users would have no need to create objects
of this class or call any of its functions directly.

Python:
    Python does not support C++ templates. To access this class,
    append the dimension as a suffix (e.g., IsoSigClassic3 for
    dimension 3). To _use_ this signature type in Python: for first-
    generation signatures this type is the default, so just call the
    ``Triangulation<dim>`` signature function with no extra arguments
    (e.g., ``isoSig()`` or ``isoSigDetail()``); for second-generation
    signatures, pass this class as a runtime argument (e.g.,
    ``neoSig(False, IsoSigPrintable, IsoSigClassic3)``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::IsoSigData1
inline constexpr const char IsoSigData1[] =
R"doc(Holds the combinatorial data required to reconstruct a single non-
empty connected component of a *dim*-dimensional triangulation, up to
relabelling, for use with first-generation isomorphism signatures as
returned by ``Triangulation<dim>::isoSig()``. This is a halfway point
between signatures and triangulations: the data is small and uses
simple numerical types, and is easy to encode or decode in an
appropriate signature form (e.g., a base64 string).

Specifically, this data encodes a "compressed" gluings table for a
"canonical" labelling of a triangulation component. Both the notions
of "compressed" and "canonical" involve stepping through the facets of
top-dimensional simplices in the usual order (simplex 0, facets
``0,...,dim``; then simplex 1, facets ``0,...,dim``; and so on). Some
of these facets will be boundary (i.e., unglued), some will be glued
to "new" facets that we have not yet stepped through, and some will be
glued to "old" facets (i.e., representing a `(dim-1)`-face of the
triangulation that we have already seen from the other side).

The notion of "canonical" differs between first-generation signatures
(``isoSig()``) and second-generation signatures (``neoSig()``). To be
_canonical_ for a first-generation signature:

* As we step through the facets, if we reach a facet ``f`` that is
  glued to a partner ``g`` belonging to a top-dimensional simplex that
  we have not yet seen before, we require that the simplex containing
  ``g`` uses the next available simplex label, and that the gluing
  between ``f`` and ``g`` uses the identity permutation. So, for
  example, the first facet of simplex 0 that is glued to some _other_
  simplex must be glued to simplex 1 using the identity permutation.
  Note that, in constrast to second-generation signatures, if the
  component has more than one top-dimensional simplex then this will
  never result in an oriented labelling.

* Your chosen signature _type_ may impose further requirements on the
  choice of simplex 0 and its vertex labels; see the IsoSigType
  concept documentation for details.

The first condition above ensures that, once we have chosen which top-
dimensional simplex will be simplex 0 and how its vertices
``0,...,dim`` will be labelled, the labelling of the remaining
simplices and their vertices will be uniquely determined (and can be
computed in linear time).

Given a canonical labelling, a _compressed gluings table_ for a first-
generation signature holds the following information:

* For each facet *f* that we step through in order, _ignoring_ those
  glued to "old" facets, we record whether *f* is: boundary (type 0);
  glued to a "new" facet of a previously-unseen simplex using the
  identity permutation (type 1); or glued to a "new" facet of a
  simplex that has already been seen (type 2). This is stored in an
  array whose length is the total number of `(dim-1)`-faces in the
  triangulation component.

* For each facet *f* of type 2, we record the top-dimensional simplex
  number of the facet that *f* is glued to. This is stored in an array
  whose length is the total number of facets of type 2.

* For each facet *f* of type 2, we record the ordered ``S_n`` index of
  the specific gluing permutation between *f* and its partner, again
  using an array whose length is the total number of facets of type 2.
  Note that this is different from second-generation signatures, which
  use the plain (not ordered) ``S_n`` index.

* Finally, if (and only if) there are any simplex or facet locks in
  this triangulation component, we record the lock masks of all top-
  dimensional simplices in order (simplex ``0,1,2,...``).

A first-generation signature _encoding_ is responsible for encoding
this data in its final signature form (e.g., a base64 string); see the
IsoSigEncoding concept documentation for details. The final signature
will use the labelling that minimises this encoding, under the
inherent ordering of the resulting signature type (e.g., the inherent
ordering on strings). Note that this works differently from second-
generation signatures, which minimise the compressed gluings table
_before_ it is encoded.

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

// Docstring regina::python::doc::IsoSigData2
inline constexpr const char IsoSigData2[] =
R"doc(Holds the combinatorial data required to reconstruct a single non-
empty connected component of a *dim*-dimensional triangulation, up to
relabelling, for use with second-generation isomorphism signatures as
returned by ``Triangulation<dim>::neoSig()``. This is a halfway point
between signatures and triangulations: the data is small and uses
simple numerical types, and is easy to encode or decode in an
appropriate signature form (e.g., a base64 string).

Specifically, this data encodes a "compressed" gluings table for a
"canonical" labelling of a triangulation component. Both the notions
of "compressed" and "canonical" involve stepping through the facets of
top-dimensional simplices in the usual order (simplex 0, facets
``0,...,dim``; then simplex 1, facets ``0,...,dim``; and so on). Some
of these facets will be boundary (i.e., unglued), some will be glued
to "new" facets that we have not yet stepped through, and some will be
glued to "old" facets (i.e., representing a `(dim-1)`-face of the
triangulation that we have already seen from the other side).

The notion of "canonical" differs between first-generation signatures
(``isoSig()``) and second-generation signatures (``neoSig()``). To be
_canonical_ for a second-generation signature:

* As we step through the facets, if we reach a facet ``f`` that is
  glued to a partner ``g`` belonging to a top-dimensional simplex that
  we have not yet seen before, we require that the simplex containing
  ``g`` uses the next available simplex label, and that the gluing
  between ``f`` and ``g`` uses the permutation that swaps 0 with 1.
  So, for example, the first facet of simplex 0 that is glued to some
  _other_ simplex must be glued to simplex 1 using the permutation
  ``Perm<dim+1>(0,1)``. Note that, in contrast to first-generation
  signatures, if the component is orientable then this will always
  result in an oriented labelling.

* Your chosen signature _type_ may impose further requirements on the
  choice of simplex 0 and its vertex labels; see the IsoSigType
  concept documentation for details.

The first condition above ensures that, once we have chosen which top-
dimensional simplex will be simplex 0 and how its vertices
``0,...,dim`` will be labelled, the labelling of the remaining
simplices and their vertices will be uniquely determined (and can be
computed in linear time).

Given a canonical labelling, a _compressed gluings table_ for a
second-generation signature holds the following information:

* For each facet *f* that we step through in order, _ignoring_ those
  glued to "old" facets, we record whether *f* is: glued to a "new"
  facet of a previously-unseen simplex using the permutation swapping
  0 and 1 (type A); or either boundary or glued to a "new" facet of a
  simplex that has already been seen (type B). This is stored in a
  bitmask whose length is the total number of `(dim-1)`-faces in the
  triangulation component. Comparing these types to the types used in
  first-generation signatures: type A corresponds to the old type 1,
  and type B combines the old types 0 and 2.

* For each facet *f* of type B, we record the top-dimensional simplex
  number of the facet that *f* is glued to; if *f* is boundary then
  this number will be the total number of top-dimensional simplices in
  the component. This is stored in an array whose length is the total
  number of facets of type B.

* For each non-boundary facet *f* of type B, we record the ``S_n``
  index of the specific gluing permutation between *f* and its
  partner, using an array whose length is the total number of non-
  boundary facets of type B. Note that this is different from first-
  generation signatures, which use the _ordered_ ``S_n`` index.

* Finally, if (and only if) there are any simplex or facet locks in
  this triangulation component, we record the lock masks of all top-
  dimensional simplices in order (simplex ``0,1,2,...``).

A second-generation signature _encoding_ is responsible for encoding
this data in its final signature form (e.g., a base64 string); see the
IsoSigEncoding concept documentation for details. The final signature
will use the labelling that minimises the compressed gluings table,
under the inherent ordering of ``IsoSigData<2, dim>``. Note that this
works differently from first-generation signatures, which minimise the
_encoding_ (e.g., the base64 string).

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
inline constexpr const char IsoSigDegrees[] =
R"doc(A faster isomorphism signature type based on degree sequences of
*subdim*-faces.

The signature type ``IsoSigDegrees<dim, dim-2>`` (also available under
the alias ``IsoSigRidgeDegrees<dim>``) is recommended for new
projects, and is the default signature type for the second-generation
signature routines ``Triangulation<dim>::neoSig()`` and
``Triangulation<dim>::neoSigDetail()``.

The IsoSigType concept documentation explains how an isomorphism
signature type acts as an "iterator" through candidate pairs ``(s,
p)``, where *s* is a "starting simplex" within some triangulation
component *c*, and *p* describes a "starting labelling" of its
vertices. This signature type only considers starting simplices *s*
with a lexicographically minimal *subdim*-face degree sequence. The
hope is that this eliminates a large number of potential starting
simplices without adding an enormous amount of computational overhead,
thereby speeding up the overall isomorphism signature computation.

This class is designed to be used as a template parameter for
signature routines in the class ``Triangulation<dim>``, including
``isoSig()``, ``neoSig()``, ``isoSigDetail()``, and
``neoSigDetail()``. Typical users would have no need to create objects
of this class or call any of its functions directly.

Python:
    Python does not support C++ templates, and there are far too many
    of these classes to wrap. Currently Python supports only the cases
    where *subdim* is 1 or ``dim-2``, using the type aliases
    IsoSigEdgeDegrees and IsoSigRidgeDegrees respectively. To access
    one of these classes, append the dimension as a suffix to the
    relevant type alias (e.g., IsoSigEdgeDegrees3, or
    IsoSigRidgeDegrees4). To _use_ this signature type in Python, pass
    the type alias as a runtime argument to the ``Triangulation<dim>``
    signature function (e.g., ``neoSig(False, IsoSigPrintable,
    IsoSigEdgeDegrees4)``. As an exception, if you are building a
    second-generation signature and you wish to use IsoSigRidgeDegrees
    (or IsoSigEdgeDegrees in dimension three, which is the same type),
    you do not need any extra arguments since this signature type is
    the default.)doc";

// Docstring regina::python::doc::IsoSigPrintable
inline constexpr const char IsoSigPrintable[] =
R"doc(Encodes both first-generation and second-generation isomorphism
signatures as printable strings. This is the default encoding used by
both ``Triangulation<dim>::isoSig()`` and
``Triangulation<dim>::neoSig()``.

This printable encoding represents an isomorphism signature as a
``std::string``, using only printable characters from the 7-bit ASCII
range. For first-generation signatures of triangulations with no
simplex or facet locks, this is the same encoding that was used for
the original isomorphism signatures first introduced in Regina 4.90.

If the triangulation does have one or more simplex and/or facet locks,
then these locks will be encoded as part of the isomorphism signature
(they will appear at the end, after the encoded gluings table). This
will break compatibility with signatures from Regina ≤ 7.3.1 (before
locks were implemented). If the triangulation does not have any locks
then no lock data will encoded, and so the encoding will be remain
compatible with all older versions of Regina. If you need a signature
where locks are not encoded (e.g., for backward compatibility), then
you can use the encoding IsoSigPrintableLockFree.

See the IsoSigEncoding concept documentation for general details on
encodings for isomorphism signatures.

This class is designed to be used as a template parameter for
signature routines in the class ``Triangulation<dim>``, including
``isoSig()``, ``neoSig()``, ``isoSigDetail()``, and
``neoSigDetail()``. Typical users would have no need to call any
functions from this encoding class directly.

Python:
    To use this encoding in Python, you can just call the relevant
    ``Triangulation<dim>`` signature function (e.g., ``isoSig()`` or
    ``neoSig()``) with no extra encoding argument, since this encoding
    is the default.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree
inline constexpr const char IsoSigPrintableLockFree[] =
R"doc(Encodes both first-generation and second-generation isomorphism
signatures as printable strings, ignoring any simplex and/or facet
locks.

Like IsoSigPrintable, this encoding represents an isomorphism
signature as a ``std::string`` using only printable characters from
the 7-bit ASCII range. For first-generation signatures, this is
exactly the same as the printable encoding that was used with Regina ≤
7.3.x, before locks were implemented.

See the IsoSigEncoding concept documentation for general details on
encodings for isomorphism signatures.

This class is designed to be used as a template parameter for
signature routines in the class ``Triangulation<dim>``, including
``isoSig()``, ``neoSig()``, ``isoSigDetail()``, and
``neoSigDetail()``. Typical users would have no need to call any
functions from this encoding class directly.

Python:
    To use this encoding in Python, pass IsoSigPrintableLockFree as a
    runtime argument to the relevant ``Triangulation<dim>`` signature
    function (e.g., ``isoSig()`` or ``neoSig()``).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace IsoSigBinary_ {

// Docstring regina::python::doc::IsoSigBinary_::asString
inline constexpr const char asString[] =
R"doc(Re-encodes the given binary signature as a string-based second-
generation signature (using the IsoSigPrintable encoding), which uses
only printable characters from the 7-bit ASCII range.

Calling ``printable(sig)`` is significantly more efficient than
calling ``Triangulation<dim>::fromSig(sig).neoSig()`` (with an
appropriate orientation argument if necessary), and should give the
same result.

If *sig* is an oriented signature, then this re-encoding will preserve
the orientation.

Precondition:
    The argument *sig* is indeed a second-generation isomorphism
    signature of a <i>dim</i/>-dimensional triangulation, encoded via
    IsoSigBinary. This will _not_ be checked thoroughly (though some
    minimal checks will be done).

Python:
    Python does not support C++ templates. Instead, the dimension
    *dim* should be passed as a second runtime argument; for example,
    ``IsoSigBinary.asString(sig, 3)``.

Exception ``InvalidArgument``:
    It was detected that *sig* was not a valid second-generation
    isomorphism signature of a <i>dim</i/>-dimensional triangulation
    encoded via IsoSigBinary. Again, this will not be checked
    thoroughly; this exception will only be thrown if the violation is
    sufficiently obvious that it is picked up during the re-encoding
    process.

Parameter ``sig``:
    the second-generation signature of some <i>dim</i/>-dimensional
    triangulation, encoded as a byte sequence using the IsoSigBinary
    encoding.

Returns:
    the second-generation signature of the same triangulation, encoded
    as a printable string using the IsoSigPrintable encoding.)doc";

// Docstring regina::python::doc::IsoSigBinary_::encode
inline constexpr const char encode[] =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation as a second-generation isomorphism signature.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as a second-generation signature.)doc";

// Docstring regina::python::doc::IsoSigBinary_::encodeEmpty
inline constexpr const char encodeEmpty[] =
R"doc(Encodes the isomorphism signature of the empty triangulation.

For IsoSigBinary (unlike Regina's string-based encodings), this will
simply be an empty sequence.

Returns:
    the isomorphism signature of the empty triangulation.)doc";

// Docstring regina::python::doc::IsoSigBinary_::length
inline constexpr const char length[] =
R"doc(Precomputes the length of the second-generation isomorphism signature
that encodes the given connected component.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the length of the second-generation signature that encodes *data*.)doc";

}

namespace IsoSigClassic_ {

// Docstring regina::python::doc::IsoSigClassic_::__init
inline constexpr const char __init[] =
R"doc(Initialises this "iterator" to the first candidate pair ``(s, p)`` for
the given triangulation component.

Parameter ``component``:
    the triangulation component that we are examining.

Parameter ``oriented``:
    ``True`` if you are creating an oriented signature, or ``False``
    if you are creating an unoriented signature.)doc";

// Docstring regina::python::doc::IsoSigClassic_::next
inline constexpr const char next[] =
R"doc(Advances this "iterator" to the next candidate pair ``(s, p)``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    ``True`` if this was successful, or ``False`` if there are no more
    candidate pairs.)doc";

// Docstring regina::python::doc::IsoSigClassic_::perm
inline constexpr const char perm[] =
R"doc(Returns the current starting labelling *p* of the vertices of the
current starting simplex *s*. This maps the vertices of *s* to the
"canonical" labels ``0,1,...,dim``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the starting labelling *p*.)doc";

// Docstring regina::python::doc::IsoSigClassic_::simplex
inline constexpr const char simplex[] =
R"doc(Returns the index of the current starting simplex *s* within the
relevant triangulation component.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the index within the component of the starting simplex *s*.)doc";

}

namespace IsoSigData1_ {

// Docstring regina::python::doc::IsoSigData1_::__copy
inline constexpr const char __copy[] = R"doc(Makes a new deep copy of the given data set.)doc";

// Docstring regina::python::doc::IsoSigData1_::__eq
inline constexpr const char __eq[] =
R"doc(Determines whether this and the given triangulation component data
hold identical information.

Returns:
    ``True`` if and only if this and the given data are identical.)doc";

// Docstring regina::python::doc::IsoSigData1_::__init
inline constexpr const char __init[] =
R"doc(Allocates space for a data set for the given triangulation component.

The arrays that make up this data set will be initialised to the
correct sizes, but the individual array elements will be left
uninitialised. You will need to call fillFrom() before this data set
can be used (i.e., before any other methods are called).

Parameter ``component``:
    the triangulation component that we intend to encode.)doc";

// Docstring regina::python::doc::IsoSigData1_::adjacentGluings
inline constexpr const char adjacentGluings[] =
R"doc(Gives read-only access to the array of gluing permutations, as
described in the class notes.

The length of this array will be the number of facets of top-
dimensional simplices of type 2 (as described in the class notes).

Each element of this array holds the ordered ``S_n`` index of the
gluing permutation for the relevant facet. Note that, in constrast to
second-generation signatures, here we used the ordered ``S_n`` index
and not the plain ``S_n`` index.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing permutations.)doc";

// Docstring regina::python::doc::IsoSigData1_::adjacentSimplices
inline constexpr const char adjacentSimplices[] =
R"doc(Gives read-only access to the array of gluing destinations, as
described in the class notes.

The length of this array will be the number of facets of top-
dimensional simplices of type 2 (as described in the class notes).

Each element of this array corresponds to some facet *f* of type 2,
and indicates the index of the top-dimensional simplex containing the
partner facet to which *f* is glued.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing destinations.)doc";

// Docstring regina::python::doc::IsoSigData1_::facetTypes
inline constexpr const char facetTypes[] =
R"doc(Gives read-only access to the array of facet types, as described in
the class notes.

This will be an array of integers 0, 1 or 2, corresponding to the
facets of top-dimensional simplices that we step through in order as
described in the class notes. Facets that are glued to "old" partner
facets (i.e., facets that represent internal `(dim-1)`-faces of the
triangulation and have already been seen from the other side) are not
represented at all in this array.

Each integer in the array indicates whether a particular facet of a
top-dimensional simplex is boundary (0), glued to a "new" partner
facet of a previously-unseen simplex (1), or glued to a "new" partner
facet belonging to a simplex that has already been seen before (2).

The length of the array will be the total number of `(dim-1)`-faces in
the triangulation component being encoded.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of facet types.)doc";

// Docstring regina::python::doc::IsoSigData1_::fillFrom
inline constexpr const char fillFrom[] =
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

// Docstring regina::python::doc::IsoSigData1_::hasLocks
inline constexpr const char hasLocks[] =
R"doc(Indicates whether the triangulation component being encoded has any
simplex or facet locks.

This will be ``True`` if and only if the array returned by locks() is
non-empty.

Returns:
    a reference to the (possibly empty) array of lock masks.)doc";

// Docstring regina::python::doc::IsoSigData1_::locks
inline constexpr const char locks[] =
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

// Docstring regina::python::doc::IsoSigData1_::size
inline constexpr const char size[] =
R"doc(Returns the total number of top-dimensional simplices in the connected
triangulation component that this data set describes.

Returns:
    the total number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::IsoSigData1_::swap
inline constexpr const char swap[] =
R"doc(Swaps the contents of this and the given triangulation component data.

This routine will behave correctly if *other* is in fact this data
set.

Parameter ``other``:
    the component data whose contents should be swapped with this.)doc";

}

namespace IsoSigData2_ {

// Docstring regina::python::doc::IsoSigData2_::__cmp
inline constexpr const char __cmp[] =
R"doc(Compares two sets of triangulation component data. Such comparisons
are used by the signature algorithm when choosing a minimal canonical
labelling, and also when choosing an ordering of components in
disconnected triangulations.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the component data to compare with this.

Returns:
    the result of the comparison between this and the given data.)doc";

// Docstring regina::python::doc::IsoSigData2_::__copy
inline constexpr const char __copy[] = R"doc(Makes a new deep copy of the given data set.)doc";

// Docstring regina::python::doc::IsoSigData2_::__eq
inline constexpr const char __eq[] =
R"doc(Determines whether this and the given triangulation component data
hold identical information.

Returns:
    ``True`` if and only if this and the given data are identical.)doc";

// Docstring regina::python::doc::IsoSigData2_::adjacentGluings
inline constexpr const char adjacentGluings[] =
R"doc(Gives read-only access to the array of gluing permutations, as
described in the class notes.

The length of this array will be the number of non-boundary facets of
top-dimensional simplices of type B (as described in the class notes).

Each element of this array holds the ``S_n`` index of the gluing
permutation for the relevant facet. Note that, in contrast to first-
generation signatures, here we use the plain ``S_n`` index and not the
ordered ``S_n`` index.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing permutations.)doc";

// Docstring regina::python::doc::IsoSigData2_::adjacentSimplices
inline constexpr const char adjacentSimplices[] =
R"doc(Gives read-only access to the array of gluing destinations, as
described in the class notes.

The length of this array will be the number of facets of top-
dimensional simplices of type B (as described in the class notes).

Each element of this array corresponds to some facet *f* of type B,
and indicates the index of the top-dimensional simplex containing the
partner facet to which *f* is glued, or ``size()`` if *f* is a
boundary facet.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the array of gluing destinations.)doc";

// Docstring regina::python::doc::IsoSigData2_::countFacetBits
inline constexpr const char countFacetBits[] =
R"doc(Returns the precise length of the bitmask of facet types, as returned
by facetTypes(). This will be the total number of `(dim-1)`-faces in
the underlying triangulation component, as described in the class
notes.

This routine is provided because IsoSigData does not keep a reference
to the component itself, and because Bitmask does not encode its
precise length.

Returns:
    the length of the bitmask of facet types.)doc";

// Docstring regina::python::doc::IsoSigData2_::facetTypes
inline constexpr const char facetTypes[] =
R"doc(Gives read-only access to the bitmask of facet types, as described in
the class notes.

The bits correspond to the facets of top-dimensional simplices that we
step through as described in the class notes, but are stored in
_reverse_ order; this ensures that ``Bitmask::numericalComp()`` treats
the first facet (not the last) as most significant. Facets that are
glued to "old" partner facets (i.e., facets that represent internal
`(dim-1)`-faces of the triangulation and have already been seen from
the other side) are not represented at all in this bitmask.

Each bit is set if and only if the corresponding facet is of type B
(i.e., either boundary, or glued to a "new" partner facet belonging to
a simplex that has already been seen before).

The length of the bitmask will be the total number of `(dim-1)`-faces
in the triangulation component being described.

Python:
    This routine returns a deep copy (not a reference), for
    consistency with the array-based query routines in this class.

Returns:
    a reference to the bitmask of facet types.)doc";

// Docstring regina::python::doc::IsoSigData2_::hasLocks
inline constexpr const char hasLocks[] =
R"doc(Indicates whether the triangulation component being encoded has any
simplex or facet locks.

This will be ``True`` if and only if the array returned by locks() is
non-empty.

Returns:
    ``True`` if and only if there are simplex and/or facet locks.)doc";

// Docstring regina::python::doc::IsoSigData2_::isOriented
inline constexpr const char isOriented[] =
R"doc(Determines whether the labelling that this data set encodes is
oriented.

This will be ``True`` if and only if all permutations in
adjacentGluings() are odd.

Returns:
    ``True`` if and only if this labelling is oriented.)doc";

// Docstring regina::python::doc::IsoSigData2_::locks
inline constexpr const char locks[] =
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

// Docstring regina::python::doc::IsoSigData2_::minimal
inline constexpr const char minimal[] =
R"doc(Chooses a canonical labelling for the given triangulation component
that minimises the compressed gluings table.

The minimisation will use the inherent ordering of IsoSigData.

Python:
    Python does not support C++ templates. Instead, you should pass
    *Type* as the first runtime argument; that is, ``minimal(type,
    component, oriented, relabelling)``.

Template parameter ``Type``:
    an isomorphism signature type, which will be used to determine
    which labellings are considered "canonical".

Parameter ``component``:
    the triangulation component that this data set should represent.

Parameter ``oriented``:
    indicates whether we are building an oriented isomorphism
    signature (``True``), or an unoriented signature (``False``). If
    this is ``True`` and if *comp* is oriented, then the relabelling
    that is chosen will be orientation-preserving.

Parameter ``relabelling``:
    if non-null, this isomorphism will be filled with the isomorphism
    from the original triangulation to the chosen labelling. If the
    underlying triangulation has multiple components, then only the
    information relating to this component will be changed; other
    simplex/vertex images in *relabelling* will be left untouched.

Returns:
    the data set that describes the minimal labelling, as described
    above.)doc";

// Docstring regina::python::doc::IsoSigData2_::size
inline constexpr const char size[] =
R"doc(Returns the total number of top-dimensional simplices in the connected
triangulation component that this data set describes.

Returns:
    the total number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::IsoSigData2_::swap
inline constexpr const char swap[] =
R"doc(Swaps the contents of this and the given triangulation component data.

This routine will behave correctly if *other* is in fact this data
set.

Parameter ``other``:
    the component data whose contents should be swapped with this.)doc";

}

namespace IsoSigData_ {

// Docstring regina::python::doc::IsoSigData_::global_swap
inline constexpr const char global_swap[] =
R"doc(Swaps the contents of the given triangulation component data sets.

This global routine simply calls ``IsoSigData<generation,
dim>::swap()``; it is provided so that IsoSigData meets the C++
Swappable requirements.

Parameter ``a``:
    the first component data set whose contents should be swapped.

Parameter ``b``:
    the second component data set whose contents should be swapped.)doc";

}

namespace IsoSigDegrees_ {

// Docstring regina::python::doc::IsoSigDegrees_::__init
inline constexpr const char __init[] =
R"doc(Initialises this "iterator" to the first candidate pair ``(s, p)`` for
the given triangulation component.

Parameter ``component``:
    the triangulation component that we are examining.

Parameter ``oriented``:
    ``True`` if you are creating an oriented signature, or ``False``
    if you are creating an unoriented signature.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::next
inline constexpr const char next[] =
R"doc(Advances this "iterator" to the next candidate pair ``(s, p)``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    ``True`` if this was successful, or ``False`` if there are no more
    candidate pairs.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::perm
inline constexpr const char perm[] =
R"doc(Returns the current starting labelling *p* of the vertices of the
current starting simplex *s*. This maps the vertices of *s* to the
"canonical" labels ``0,1,...,dim``.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the starting labelling *p*.)doc";

// Docstring regina::python::doc::IsoSigDegrees_::simplex
inline constexpr const char simplex[] =
R"doc(Returns the index of the current starting simplex *s* within the
relevant triangulation component.

Precondition:
    This "iterator" is holding a valid candidate pair ``(s, p)``; that
    is, next() has not yet returned ``False``.

Returns:
    the index within the component of the starting simplex *s*.)doc";

}

namespace IsoSigPrintableLockFree_ {

// Docstring regina::python::doc::IsoSigPrintableLockFree_::charsPerPerm
inline constexpr const char charsPerPerm[] =
R"doc(The number of characters that we use in our encoding to represent a
single gluing permutation. This must be large enough to encode an
index into Perm<dim+1>::Sn.

Python:
    Since Python does not support C++ templates, this constant is
    accessible via a function
    ``IsoSigPrintableLockFree.charsPerPerm(dim)``.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree_::encode
inline constexpr const char encode[] =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation as a first-generation isomorphism signature.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as a first-generation signature.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree_::encode_2
inline constexpr const char encode_2[] =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation as a second-generation isomorphism signature.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as a second-generation signature.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree_::encodeEmpty
inline constexpr const char encodeEmpty[] =
R"doc(Encodes the isomorphism signature of the empty *dim*-dimensional
triangulation.

Like IsoSigPrintable, this does _not_ return an empty signature for
this; instead it returns the non-empty string ``a``.

Returns:
    the isomorphism signature of the empty triangulation.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree_::length
inline constexpr const char length[] =
R"doc(Precomputes the length of the first-generation isomorphism signature
that encodes the given connected component.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the length of the first-generation signature that encodes *data*.)doc";

// Docstring regina::python::doc::IsoSigPrintableLockFree_::length_2
inline constexpr const char length_2[] =
R"doc(Precomputes the length of the second-generation isomorphism signature
that encodes the given connected component.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the length of the second-generation signature that encodes *data*.)doc";

}

namespace IsoSigPrintable_ {

// Docstring regina::python::doc::IsoSigPrintable_::charsPerPerm
inline constexpr const char charsPerPerm[] =
R"doc(The number of characters that we use in our encoding to represent a
single gluing permutation. This must be large enough to encode an
index into Perm<dim+1>::Sn.

Python:
    Since Python does not support C++ templates, this constant is
    accessible via a function ``IsoSigPrintable.charsPerPerm(dim)``.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::encode
inline constexpr const char encode[] =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation as a first-generation isomorphism signature.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as a first-generation signature.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::encode_2
inline constexpr const char encode_2[] =
R"doc(Encodes a single connected component of a *dim*-dimensional
triangulation as a second-generation isomorphism signature.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the given gluings table encoded as a second-generation signature.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::encodeEmpty
inline constexpr const char encodeEmpty[] =
R"doc(Encodes the isomorphism signature of the empty *dim*-dimensional
triangulation.

Note that IsoSigPrintable does _not_ return an empty signature for
this; instead it returns the non-empty string ``a``.

Returns:
    the isomorphism signature of the empty triangulation.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::generation
inline constexpr const char generation[] =
R"doc(Identifies whether the given signature is a first-generation or
second-generation signature, as encoded by IsoSigPrintable.

This routine aims to be fast, and does not verify the entire
signature; instead it reads just enough of the initial characters to
make its decision. What this means is:

* If the given signature _is_ a first-generation or second-generation
  signature as encoded by IsoSigPrintable, this routine guarantees to
  return 1 or 2 respectively.

* Otherwise, there are no guarantees: this output _could_ return 0
  (indicating that it identified *sig* as being neither of these), or
  it could still return 1 or 2 (indicating that, whilst invalid, *sig*
  nevertheless has a prefix that _looks_ like a first-generation or
  second-generation signature).

As a special case, for the empty triangulation, the first-generation
and second-generation signatures are identical (both are the single
letter ``a``). In this case, generation() will return 2.

If you need to verify the _validity_ of a signature, this is not the
correct routine to use - instead you should test whether
``Triangulation<dim>::fromSig(sig)`` throws an exception.

This routine will also recognise signatures encoded by
IsoSigPrintableLockFree (since those are identical to signatures
encoded by IsoSigPrintable for triangulations with no simplex/facet
locks).

This routine does not require knowledge of the triangulation
dimension.

Parameter ``sig``:
    a printable isomorphism signature of some generation.

Returns:
    1 or 2 if *sig* is a first-generation or second-generation
    signature respectively as encoded via IsoSigPrintable, or 0 if
    *sig* was explicitly discovered to be neither of these. As
    described above, if \s sig is _not_ a printable isomorphism
    signature of any generation, this routine could return any of the
    values 0, 1 or 2.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::length
inline constexpr const char length[] =
R"doc(Precomputes the length of the first-generation isomorphism signature
that encodes the given connected component.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the length of the first-generation signature that encodes *data*.)doc";

// Docstring regina::python::doc::IsoSigPrintable_::length_2
inline constexpr const char length_2[] =
R"doc(Precomputes the length of the second-generation isomorphism signature
that encodes the given connected component.

Precondition:
    The given component is non-empty, and uses a canonical labelling
    in the sense described in the IsoSigData class notes.

Parameter ``data``:
    the compressed gluings table for the component to encode.

Returns:
    the length of the second-generation signature that encodes *data*.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

