/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LinkSigCompact
static const char *LinkSigCompact =
R"doc(A compact string-based encoding for use with knot/link signatures.

Like LinkSigPrintable, this encodes a signature as a ``std::string``
using only printable characters from the 7-bit ASCII range. However,
the strings it produces are significantly shorter (a little over half
the length in general). These signatures are not compatible with the
knot/link signatures that were used in Regina ≤ 7.x; they are designed
for scenarios where memory usage needs to be kept as small as
possible.

See the LinkSigEncoding concept documentation for general details on
encodings for knot/link signatures.

This class is designed to be used as a template parameter for
Link::sig(). Typical users would have no need to call any of its
functions directly.

Python:
    Python does not support C++ templates. To use this encoding in
    Python, you can call ``Link::sig_Compact()``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LinkSigData
static const char *LinkSigData =
R"doc(Holds all the data required to reconstruct a single connected
component of a link diagram. This is a halfway point between link
diagrams and knot/link signatures: the data is purely numerical,
making it easy to minimise amongst all possible labellings, and also
easy to encode or decode in an appropriate signature form (e.g., a
base64 string).

Specifically, this data consists of a sequence of integer triples
``(crossing_index, strand, crossing_sign)``, describing traversals
along each of the _topological_ link components within this diagram
component, with sentinels ``(n, 0, 0)`` placed between topological
link components (where *n* is the total number of crossings in the
diagram component). There is no sentinel at the beginning or end of
the overall sequence. Crossings indices are in the range
``0,...,n-1``, and strands are either 0 or 1 for lower or upper
strands respectively.

A knot/link signature will encode a labelling that minimises this
sequence lexicographically. The ordering used for individual triples
places lower crossing indices first, then upper strands first, then
positive signs first.

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

// Docstring regina::python::doc::LinkSigPacked
static const char *LinkSigPacked =
R"doc(A small-memory byte-based encoding for use with knot/link signatures.

This uses a similar "compression" of the combinatorial link data as
LinkSigCompact; however, it encodes this data in a byte sequence using
all eight bits per byte (as opposed to LinkSigCompact, which only
encodes six bits per byte but creates a printable string as a result).

This encoding is intended for scenarios where memory use needs to be
kept as small as possible.

See the LinkSigEncoding concept documentation for general details on
encodings for knot/link signatures.

This class is designed to be used as a template parameter for
Link::sig(). Typical users would have no need to call any of its
functions directly.

Python:
    Python does not support C++ templates. To use this encoding in
    Python, you can call ``Link::sig_Packed()``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LinkSigPrintable
static const char *LinkSigPrintable =
R"doc(The default encoding to use for knot/link signatures.

This printable encoding is the one that was used for knot/link
signatures in Regina ≤ 7.x. It represents a signature as a
``std::string``, using only printable characters from the 7-bit ASCII
range.

See the LinkSigEncoding concept documentation for general details on
encodings for knot/link signatures.

This class is designed to be used as a template parameter for
Link::sig(). Typical users would have no need to call any of its
functions directly.

Python:
    Python does not support C++ templates. To use this encoding in
    Python, you can simply call ``Link::sig()`` (since this encoding
    is the default).

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

namespace LinkSigCompact_ {

// Docstring regina::python::doc::LinkSigCompact_::encode
static const char *encode =
R"doc(Encodes a single connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the given data encoded as a knot/link signature.)doc";

// Docstring regina::python::doc::LinkSigCompact_::encodeEmpty
static const char *encodeEmpty =
R"doc(Encodes the signature of the empty link.

Note that LinkSigPrintable and LinkSigCompact do _not_ return an empty
signature for this; instead they both return the special string ``_``.

Returns:
    the signature of the empty link.)doc";

// Docstring regina::python::doc::LinkSigCompact_::encodeUnknot
static const char *encodeUnknot =
R"doc(Encodes the signature of the zero-crossing unknot diagram.

Both LinkSigPrintable and LinkSigCompact return the same signature
``a`` in this case.

Returns:
    the signature of the zero-crossing unknot.)doc";

// Docstring regina::python::doc::LinkSigCompact_::length
static const char *length =
R"doc(Precomputes the length of the signature that encodes the given
connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the length of the knot/link signature that encodes *data*.)doc";

}

namespace LinkSigData_ {

// Docstring regina::python::doc::LinkSigData_::Term
static const char *Term =
R"doc(An integer triple ``(crossing_index, strand, crossing_sign)``,
describing a single step in the traversal along some topological link
component.

See the LinkSigData class notes for further details.

This is a very simple struct, and its constructors and assignment
operators are provided automatically by the compiler. These objects
are small enough to pass by value and swap with ``std::swap()``, with
no need for any specialised move operations or swap functions.

Python:
    This is a read-only struct: its data members cannot be modified.
    Moreover, objects of this type cannot be explicitly constructed;
    instead you would typically retrieve them from a link diagram data
    set by calling ``LinkSigData::traversal()``.)doc";

// Docstring regina::python::doc::LinkSigData_::__cmp
static const char *__cmp =
R"doc(Compares two sets of diagram component data. Such comparisons are
useful when creating a signature for a disconnected link diagram.

Broadly speaking, the intent is for the most significant components of
the link diagram to be sorted first. Specifically, a data set will be
sorted first if it involves more crossings, and then if it involves
more topological link components; after this we fall back to a
lexicographical comparison of sequences of triples as described in the
LinkSigData class notes.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Parameter ``rhs``:
    the component data to compare with this.

Returns:
    the result of the comparison between this and the given data.)doc";

// Docstring regina::python::doc::LinkSigData_::__copy
static const char *__copy = R"doc(Makes a new deep copy of the given data set.)doc";

// Docstring regina::python::doc::LinkSigData_::__eq
static const char *__eq =
R"doc(Determines whether this and the given diagram component data hold
identical information.

Returns:
    ``True`` if and only if this and the given data are identical.)doc";

// Docstring regina::python::doc::LinkSigData_::__init
static const char *__init =
R"doc(Creates a new data set for the given connected link diagram, using a
labelling that minimises the sequence of triples. See the LinkSigData
class notes for details on how this ordering works.

Precondition:
    The given link diagram has exactly one connected component. In
    particular, ``link.isEmpty()`` must return ``False``, and
    ``link.isConnected()`` must return ``True``.

Parameter ``link``:
    the connected link diagram that this data set should represent.

Parameter ``reflectionOptions``:
    indicates whether we may reflect the entire link diagram when
    minimising the sequence of triples. This is a BoolSet, which means
    reflection may be mandatory, forbidden, or optional. This BoolSet
    cannot be empty.

Parameter ``allowReversal``:
    ``True`` if and only if reversing some or all link components is
    allowed when minimising the sequence of triples.

Parameter ``rotationOptions``:
    indicates whether we may rotate the entire link diagram when
    minimising the sequence of triples. This is a BoolSet, which means
    rotation may be mandatory, forbidden, or optional. This BoolSet
    cannot be empty.)doc";

// Docstring regina::python::doc::LinkSigData_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given diagram component data sets.

This global routine simply calls LinkSigData::swap(); it is provided
so that LinkSigData meets the C++ Swappable requirements.

Parameter ``a``:
    the first component data set whose contents should be swapped.

Parameter ``b``:
    the second component data set whose contents should be swapped.)doc";

// Docstring regina::python::doc::LinkSigData_::sequence
static const char *sequence =
R"doc(Gives read-only access to the full sequence of triples that describe
this diagram component.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the full sequence of triples.)doc";

// Docstring regina::python::doc::LinkSigData_::size
static const char *size =
R"doc(Returns the total number of crossings in the connected diagram
component that this data set describes.

Returns:
    the total number of crossings.)doc";

// Docstring regina::python::doc::LinkSigData_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given diagram component data.

This routine will behave correctly if *other* is in fact this data
set.

Parameter ``other``:
    the component data whose contents should be swapped with this.)doc";

}

namespace LinkSigData_::Term_ {

// Docstring regina::python::doc::LinkSigData_::Term_::__cmp
static const char *__cmp =
R"doc(Compares two triples, using the same ordering as described in the
LinkSigData class notes.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Parameter ``rhs``:
    the triple to compare with this.

Returns:
    the result of the comparison between this and the given triple.)doc";

// Docstring regina::python::doc::LinkSigData_::Term_::__eq
static const char *__eq =
R"doc(Determines whether this and the given triple are identical.

Returns:
    ``True`` if and only if this and the given triple are identical.)doc";

// Docstring regina::python::doc::LinkSigData_::Term_::crossing
static const char *crossing = R"doc(The zero-based crossing index.)doc";

// Docstring regina::python::doc::LinkSigData_::Term_::makeSentinel
static const char *makeSentinel =
R"doc(Sets this to be a sentinel triple ``(n, 0, 0)``, as described in the
LinkSigData class notes, where *n* is the total number of crossings in
the diagram component being described.

Parameter ``size``:
    the total number of crossings in the diagram component (i.e., the
    integer *n* that is used above).)doc";

// Docstring regina::python::doc::LinkSigData_::Term_::sign
static const char *sign = R"doc(±1 indicating the sign of the crossing.)doc";

// Docstring regina::python::doc::LinkSigData_::Term_::strand
static const char *strand = R"doc(0 or 1 for the lower or upper strand respectively.)doc";

}

namespace LinkSigPacked_ {

// Docstring regina::python::doc::LinkSigPacked_::encode
static const char *encode =
R"doc(Encodes a single connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the given data encoded as a knot/link signature.)doc";

// Docstring regina::python::doc::LinkSigPacked_::encodeEmpty
static const char *encodeEmpty =
R"doc(Encodes the signature of the empty link.

For LinkSigPacked (unlike Regina's string-based encodings), this will
simply be an empty sequence.

Returns:
    the signature of the empty link.)doc";

// Docstring regina::python::doc::LinkSigPacked_::encodeUnknot
static const char *encodeUnknot =
R"doc(Encodes the signature of the zero-crossing unknot diagram.

Returns:
    the signature of the zero-crossing unknot.)doc";

// Docstring regina::python::doc::LinkSigPacked_::length
static const char *length =
R"doc(Precomputes the length of the signature that encodes the given
connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the length of the knot/link signature that encodes *data*.)doc";

}

namespace LinkSigPrintable_ {

// Docstring regina::python::doc::LinkSigPrintable_::encode
static const char *encode =
R"doc(Encodes a single connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the given data encoded as a knot/link signature.)doc";

// Docstring regina::python::doc::LinkSigPrintable_::encodeEmpty
static const char *encodeEmpty =
R"doc(Encodes the signature of the empty link.

Note that LinkSigPrintable and LinkSigCompact do _not_ return an empty
signature for this; instead they both return the special string ``_``.

Returns:
    the signature of the empty link.)doc";

// Docstring regina::python::doc::LinkSigPrintable_::encodeUnknot
static const char *encodeUnknot =
R"doc(Encodes the signature of the zero-crossing unknot diagram.

Both LinkSigPrintable and LinkSigCompact return the same signature
``a`` in this case.

Returns:
    the signature of the zero-crossing unknot.)doc";

// Docstring regina::python::doc::LinkSigPrintable_::length
static const char *length =
R"doc(Precomputes the length of the signature that encodes the given
connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the length of the knot/link signature that encodes *data*.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

