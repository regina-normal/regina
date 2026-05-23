/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct LinkSigBinary {

// Docstring regina::python::doc::LinkSigBinary::__class
static constexpr const char __class[] =
R"doc(Encodes a second-generation knot/link signature using a small-memory
byte-based encoding.

This uses a similar "compression" of the combinatorial link data as
the second-generation encoding via LinkSigPrintable; however, it
encodes this data in a byte sequence using all eight bits per byte (as
opposed to LinkSigPrintable, which only encodes six bits per byte but
creates a printable string as a result).

This encoding is intended for scenarios where memory use needs to be
kept as small as possible.

See the LinkSigEncoding concept documentation for general details on
encodings for knot/link signatures.

This class is designed to be used as a template parameter for
Link::neoSig() or Link::sig(). Typical users would not need to call
any of its functions directly.

Python:
    Python does not support C++ templates. To use this encoding in
    Python, you can call ``Link::neoSig_binary()``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LinkSigBinary::asString
static constexpr const char asString[] =
R"doc(Re-encodes the given binary signature as a string-based second-
generation signature (using the LinkSigPrintable encoding), which uses
only printable characters from the 7-bit ASCII range.

Calling ``printable(sig)`` is significantly more efficient than
calling ``Link::fromSig(sig).neoSig()``, and should give the same
result.

Precondition:
    The argument *sig* is indeed a second-generation knot/link
    signature, encoded via LinkSigBinary. This will _not_ be checked
    thoroughly (though some minimal checks will be done).

Exception ``InvalidArgument``:
    It was detected that *sig* was not a valid second-generation
    knot/link signature encoded via LinkSigBinary. Again, this will
    not be checked thoroughly; this exception will only be thrown if
    the violation is sufficiently obvious that it is picked up during
    the re-encoding process.

Parameter ``sig``:
    the second-generation signature of some link diagram, encoded as a
    byte sequence using the LinkSigBinary encoding.

Returns:
    the second-generation signature of the same link diagram, encoded
    as a printable string using the LinkSigPrintable encoding.)doc";

// Docstring regina::python::doc::LinkSigBinary::encode
static constexpr const char encode[] =
R"doc(Encodes a single connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Python:
    Python does not support C++ templates. Instead, you should pass
    the generation at runtime, using the argument order
    ``encode(generation, data)``.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the given data encoded as a knot/link signature.)doc";

// Docstring regina::python::doc::LinkSigBinary::encodeEmpty
static constexpr const char encodeEmpty[] =
R"doc(Encodes the signature of the empty link.

For LinkSigBinary (unlike Regina's string-based encodings), this will
simply be an empty sequence.

Returns:
    the signature of the empty link.)doc";

// Docstring regina::python::doc::LinkSigBinary::encodeUnknot
static constexpr const char encodeUnknot[] =
R"doc(Encodes the signature of the zero-crossing unknot diagram.

Returns:
    the signature of the zero-crossing unknot.)doc";

// Docstring regina::python::doc::LinkSigBinary::length
static constexpr const char length[] =
R"doc(Precomputes the length of the signature that encodes the given
connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Python:
    Python does not support C++ templates. Instead, you should pass
    the generation at runtime, using the argument order
    ``length(generation, data)``.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the length of the knot/link signature that encodes *data*.)doc";

}; // struct LinkSigBinary

struct LinkSigData {

// Docstring regina::python::doc::LinkSigData::__class
static constexpr const char __class[] =
R"doc(Holds all the data required to reconstruct a single connected
component of a link diagram. This is a halfway point between link
diagrams and knot/link signatures: the data is purely numerical,
making it easy to minimise amongst all possible labellings, and also
easy to encode or decode in an appropriate signature form (e.g., a
base64 string or a byte sequence).

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

// Docstring regina::python::doc::LinkSigData::__cmp
static constexpr const char __cmp[] =
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

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the component data to compare with this.

Returns:
    the result of the comparison between this and the given data.)doc";

// Docstring regina::python::doc::LinkSigData::__copy
static constexpr const char __copy[] = R"doc(Makes a new deep copy of the given data set.)doc";

// Docstring regina::python::doc::LinkSigData::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given diagram component data hold
identical information.

Returns:
    ``True`` if and only if this and the given data are identical.)doc";

// Docstring regina::python::doc::LinkSigData::__init
static constexpr const char __init[] =
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

// Docstring regina::python::doc::LinkSigData::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given diagram component data sets.

This global routine simply calls LinkSigData::swap(); it is provided
so that LinkSigData meets the C++ Swappable requirements.

Parameter ``a``:
    the first component data set whose contents should be swapped.

Parameter ``b``:
    the second component data set whose contents should be swapped.)doc";

// Docstring regina::python::doc::LinkSigData::sequence
static constexpr const char sequence[] =
R"doc(Gives read-only access to the full sequence of triples that describe
this diagram component.

Python:
    This routine returns a deep copy (not a reference), in the form of
    a Python list.

Returns:
    a reference to the full sequence of triples.)doc";

// Docstring regina::python::doc::LinkSigData::size
static constexpr const char size[] =
R"doc(Returns the total number of crossings in the connected diagram
component that this data set describes.

Returns:
    the total number of crossings.)doc";

// Docstring regina::python::doc::LinkSigData::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given diagram component data.

This routine will behave correctly if *other* is in fact this data
set.

Parameter ``other``:
    the component data whose contents should be swapped with this.)doc";

struct Term {

// Docstring regina::python::doc::LinkSigData::Term::__class
static constexpr const char __class[] =
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

// Docstring regina::python::doc::LinkSigData::Term::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two triples, using the same ordering as described in the
LinkSigData class notes.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the triple to compare with this.

Returns:
    the result of the comparison between this and the given triple.)doc";

// Docstring regina::python::doc::LinkSigData::Term::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given triple are identical.

Returns:
    ``True`` if and only if this and the given triple are identical.)doc";

// Docstring regina::python::doc::LinkSigData::Term::crossing
static constexpr const char crossing[] = R"doc(The zero-based crossing index.)doc";

// Docstring regina::python::doc::LinkSigData::Term::makeSentinel
static constexpr const char makeSentinel[] =
R"doc(Sets this to be a sentinel triple ``(n, 0, 0)``, as described in the
LinkSigData class notes, where *n* is the total number of crossings in
the diagram component being described.

Parameter ``size``:
    the total number of crossings in the diagram component (i.e., the
    integer *n* that is used above).)doc";

// Docstring regina::python::doc::LinkSigData::Term::sign
static constexpr const char sign[] = R"doc(±1 indicating the sign of the crossing.)doc";

// Docstring regina::python::doc::LinkSigData::Term::strand
static constexpr const char strand[] = R"doc(0 or 1 for the lower or upper strand respectively.)doc";

}; // struct Term

}; // struct LinkSigData

struct LinkSigEncoding {

// Docstring regina::python::doc::LinkSigEncoding::__concept
static constexpr const char __concept[] =
R"doc(Represents an encoding that can be used for the given generation of
knot/link signatures. Essentially, the job of an encoding algorithm is
to pack the information describing a single connected diagram
component into a small piece of data (such as a string) that is easily
transported.

An encoding should provide a type alias ``Signature``, indicating the
type that holds the final knot/link signature (e.g., ``std::string``).
In addition, it should provide the following static routines:

* ``encodeEmpty()``, which encodes the empty link;

* ``encodeUnknot()``, which encodes the zero-crossing unknot diagram;

* ``encode<generation>(const LinkSigData&)``, which encodes the
  information describing a single connected diagram component;

* ``length<generation>(const LinkSigData&)``, which pre-computes the
  length of the signature that encodes a single diagram component.

All three encoding routines should return the type ``Signature``.

Both encode() and length() require the generation as a template
argument, since the same encoding type may be used for multiple
generations of signature (e.g., LinkSigPrintable can be used in this
way).

Both ``encode()`` and ``length()`` may assume that the given data set
has at least one crossing, and is minimal amongst all allowed
relabellings of the underlying connected link diagram. (Here "allowed"
accounts for the fact that reflection, reversal and/or rotation may or
may not be permitted depending upon context.)

Template parameter ``generation``:
    the generation of signature to encode; this must be either 1 or 2.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Concepts:
    LinkSigEncoding is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct LinkSigEncoding

struct LinkSigPrintable {

// Docstring regina::python::doc::LinkSigPrintable::__class
static constexpr const char __class[] =
R"doc(Encodes both first-generation and second-generation knot/link
signatures as printable strings.

This printable encoding represents a knot/link signature as a
``std::string``, using only printable characters from the 7-bit ASCII
range. For first-generation signatures, this is the same encoding that
was used in Regina ≤ 7.x.

See the LinkSigEncoding concept documentation for general details on
encodings for knot/link signatures.

This class is designed to be used as a template parameter for
Link::sig() or Link::neoSig(). Typical users would not need to call
any of its functions directly.

Python:
    To use this encoding in Python, you can just call the relevant
    signature function (e.g., ``Link::knotSig()`` or
    ``Link::neoSig()``) with no extra encoding argument, since this
    encoding is the default.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.)doc";

// Docstring regina::python::doc::LinkSigPrintable::encode
static constexpr const char encode[] =
R"doc(Encodes a single connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Python:
    Python does not support C++ templates. Instead, you should pass
    the generation at runtime, using the argument order
    ``encode(generation, data)``.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the given data encoded as the requested generation of knot/link
    signature.)doc";

// Docstring regina::python::doc::LinkSigPrintable::encodeEmpty
static constexpr const char encodeEmpty[] =
R"doc(Encodes the signature of the empty link.

Note that LinkSigPrintable does _not_ return an empty signature for
this; instead it returns the special string ``_``.

Returns:
    the signature of the empty link.)doc";

// Docstring regina::python::doc::LinkSigPrintable::encodeUnknot
static constexpr const char encodeUnknot[] =
R"doc(Encodes the signature of the zero-crossing unknot diagram.

LinkSigPrintable will return the signature ``a`` in this case.

Returns:
    the signature of the zero-crossing unknot.)doc";

// Docstring regina::python::doc::LinkSigPrintable::generation
static constexpr const char generation[] =
R"doc(Identifies whether the given signature is a first-generation or
second-generation signature, as encoded by LinkSigPrintable.

This routine aims to be fast, and does not verify the entire
signature; instead it reads just enough of the initial characters to
make its decision. What this means is:

* If the given signature _is_ a first-generation or second-generation
  signature as encoded by LinkSigPrintable, this routine guarantees to
  return 1 or 2 respectively.

* Otherwise, there are no guarantees: this output _could_ return 0
  (indicating that it identified *sig* as being neither of these), or
  it could still return 1 or 2 (indicating that, whilst invalid, *sig*
  nevertheless has a prefix that _looks_ like a first-generation or
  second-generation signature).

As a special case, for the empty link and zero-crossing unlinks, the
first-generation and second-generation signatures are identical (``_``
for the empty link, or ``aa…a`` for a zero-crossing unlink). In these
scenarios, generation() will return 2.

If you need to verify the _validity_ of a signature, this is not the
correct routine to use - instead you should test whether
``Link::fromSig(sig)`` throws an exception.

Parameter ``sig``:
    a printable knot/link signature of some generation.

Returns:
    1 or 2 if *sig* is a first-generation or second-generation
    signature respectively as encoded via LinkSigPrintable, or 0 if
    *sig* was explicitly discovered to be neither of these. As
    described above, if \s sig is _not_ a printable knot/link
    signature of any generation, this routine could return any of the
    values 0, 1 or 2.)doc";

// Docstring regina::python::doc::LinkSigPrintable::length
static constexpr const char length[] =
R"doc(Precomputes the length of the signature that encodes the given
connected diagram component.

Precondition:
    The given diagram component has at least one crossing, and is
    minimal amongst all allowed relabellings of the underlying
    connected link diagram.

Python:
    Python does not support C++ templates. Instead, you should pass
    the generation at runtime, using the argument order
    ``length(generation, data)``.

Parameter ``data``:
    the data describing a connected diagram component.

Returns:
    the length of the given generation of knot/link signature that
    encodes *data*.)doc";

}; // struct LinkSigPrintable

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

