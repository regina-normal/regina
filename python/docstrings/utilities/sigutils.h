/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Base64SigDecoder
static const char *Base64SigDecoder =
R"doc(A helper class for reading signatures that use base64 encodings. These
are (in particular) used in the default encodings for Regina's own
isomorphism signatures and knot signatures.

To use this class: create a new Base64SigDecoder by passing details of
the encoded string to its constructor, and then call its
``decode...()`` member functions to read values sequentially from the
encoding.

This class will keep track of a current position in the encoded
string. Each call to a ``decode...()`` member function will advance
this position accordingly (but never beyond the end of the string).

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

Base64 decoders are single-use objects: they cannot be copied, moved
or swapped.

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.

Python:
    The type *Iterator* is an implementation detail, and is hidden
    from Python users. Just use the unadorned type name
    ``Base64SigDecoder``.)doc";

// Docstring regina::python::doc::Base64SigEncoder
static const char *Base64SigEncoder =
R"doc(A helper class for writing signatures that use base64 encodings. These
are (in particular) used in the default encodings for Regina's own
isomorphism signatures and knot signatures.

To use this class: create a new Base64SigEncoder, call one or more of
its member functions to write values to the encoding, and then call
str() to extract the resulting base64 string.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

Base64 encoders are single-use objects: they cannot be copied, moved
or swapped.

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

// Docstring regina::python::doc::Base64SigEncoding
static const char *Base64SigEncoding =
R"doc(A deprecated set of helper tools for signatures that use base64
encodings. These are (in particular) used in the default encodings for
Regina's own isomorphism signatures and knot signatures.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.

.. deprecated::
    This is now deprecated in favour of the new classes
    Base64SigEncoder and Base64SigDecoder, which carry state and have
    better error handling.)doc";

// Docstring regina::python::doc::PackedSigDecoder
static const char *PackedSigDecoder =
R"doc(A helper class for reading signatures that are encoded as packed byte
sequences.

To use this class: create a new PackedSigDecoder by passing details of
the encoded byte sequence to its constructor, and then call its
``decode...()`` member functions to read values sequentially from the
encoding.

This class will keep track of a current position in the encoded byte
sequence. Each call to a ``decode...()`` member function will advance
this position accordingly (but never beyond the end of the sequence).

Packed decoders are single-use objects: they cannot be copied, moved
or swapped.

Python:
    The type *Iterator* is an implementation detail, and is hidden
    from Python users. Just use the unadorned type name
    ``PackedSigDecoder``.)doc";

// Docstring regina::python::doc::PackedSigEncoder
static const char *PackedSigEncoder =
R"doc(A helper class for writing signatures that pack information as tightly
as possible into byte sequences. These signatures use ``std::string``
but are typically _not_ printable, and indeed they may even contain
null characters (which means they are not convertible to C-style
strings).

To use this class: create a new PackedSigEncoder, call one or more of
its member functions to write values to the encoding, and then call
bytes() to extract the resulting byte sequence.

Packed encoders are single-use objects: they cannot be copied, moved
or swapped.)doc";

namespace Base64SigDecoder_ {

// Docstring regina::python::doc::Base64SigDecoder_::__init
static const char *__init =
R"doc(Creates a new decoder for the given encoded string.

The string itself should be passed as an iterator range. This iterator
range must remain valid for the entire lifespan of this decoder.

.. warning::
    As of Regina 8.0, the meaning of the *stripWhitespace* argument
    has changed. Previously, it only skipped past _initial_
    whitespace, and calls to ``done()`` would by default read ahead to
    ignore _final_ whitespace. Now passing *stripWhitespace* as
    ``True`` will ignore whitespace at _both_ ends of the string at
    the point of construction, and calls to ``done()`` will simply
    test whether we have reached the pre-computed endpoint. So: the
    _default_ behaviour remains the same, but if you are passing a
    custom constructor argument for *stripWhitespace* and/or a custom
    boolean argument to ``done()``, then beware: the behaviour might
    have changed.

Python:
    Instead of an iterator range, this constructor takes a Python
    string. In Python (but not C++), the decoder will also keep a deep
    copy of the string, to ensure the lifespan requirements.

Parameter ``beginEncoding``:
    an iterator pointing to the beginning of the encoded string.

Parameter ``endEncoding``:
    a past-the-end iterator that marks the end of the encoded string.

Parameter ``stripWhitespace``:
    ``True`` if the given bounds should be squeezed inwards to ignore
    whitespace at both the beginning and the end of the encoded
    string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeBits
static const char *decodeBits =
R"doc(Decodes a sequence of bits, and returns these in the form of a
bitmask. The bits would typically have been encoded using
Base64SigEncoder::encodeBits() with the same *count* argument.

Specifically, it will be assumed that the bits have been packed six at
a time into base64 characters, and that for each underlying 6-bit
integer, the bits are stored in order from lowest to highest
significance.

The inverse to this routine is Base64SigEncoder::encodeBits().

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string to
    hold the requested number of bits, and/or a character was
    encountered that was not a valid base64 character.

Python:
    The template argument *BitmaskType* is taken to be Bitmask.

Template parameter ``BitmaskType``:
    the bitmask type to return; this must be capable of holding at
    least *count* bits.

Parameter ``count``:
    the number of bits to decode.

Returns:
    a bitmask holding the bits that were decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeInt
static const char *decodeInt =
R"doc(Decodes the next non-negative integer value, assuming this uses a
fixed number of base64 characters. This integer value would typically
have been encoded using Base64SigEncoder::encodeInt(), using the same
*nChars* argument.

Specifically, it will be assumed that the integer has been broken into
*nChars* 6-bit blocks, with each block encoded as a single base64
character, and with the blocks presented in order from lowest to
highest significance.

The result will be assembled using the integer type *IntType*, via
bitwise OR and bitwise shift lefts. It is assumed that the programmer
has chosen an integer type large enough to contain whatever values
they expect to read.

The inverse to this routine is Base64SigEncoder::encodeInt().

Exception ``InvalidInput``:
    There are fewer than *nChars* characters available in the encoded
    string, or a character was encountered that was not a valid base64
    character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``nChars``:
    the number of base64 characters to read.

Returns:
    the integer that was decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeInts
static const char *decodeInts =
R"doc(Decodes a sequence of non-negative integer values, assuming that each
individual value uses a fixed number of base64 characters, and returns
these as an array of native C++ integers. Each integer to be decoded
would typically have been encoded using Base64SigEncoder::encodeInt()
or Base64SigEncoder::encodeInts(), using the same *nChars* argument.

Specifically, it will be assumed that each integer has been broken
into *nChars* 6-bit blocks, with each block encoded as a single base64
character, and with the blocks presented in order from lowest to
highest significance.

Each resulting integer will be assembled using the integer type
*IntType*, via bitwise OR and bitwise shift lefts. It is assumed that
the programmer has chosen an integer type large enough to contain
whatever values they expect to read.

The inverse to this routine is Base64SigEncoder::encodeInts().

Exception ``InvalidInput``:
    There are fewer than ``count × nChars`` characters available in
    the encoded string, or a character was encountered that was not a
    valid base64 character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``. This routine returns a Python list of integers.

Parameter ``count``:
    the number of integers to decode.

Parameter ``nChars``:
    the number of base64 characters to read for each integer.

Returns:
    the sequence of integers that were decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeSingle
static const char *decodeSingle =
R"doc(Decodes the 6-bit integer value represented by the next single base64
character.

The inverse to this routine is Base64SigEncoder::encodeSingle().

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeSize
static const char *decodeSize =
R"doc(Decodes the next non-negative integer value (typically representing
the size of some object), without knowing in advance how many base64
characters were used to encode it. This integer value must have been
encoded using Base64SigEncoder::encodeSize().

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also returns the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* base64 characters. Typically these *x* would be _indices_
into an object (e.g., top-dimensional simplex numbers, or crossing
numbers). More precisely, *b* is the same integer that was returned
when *size* was encoded using encodeSize(). Typically you would pass
*b* to subsequent calls to decodeInt().

The inverse to this routine is Base64SigEncoder::encodeSize().

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string,
    or a character was encountered that was not a valid base64
    character.

Returns:
    a pair (*size*, *b*), where *size* is the integer that was
    decoded, and *b* is the number of base64 characters described
    above.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeTrits
static const char *decodeTrits =
R"doc(Decodes three trits from a single base64 character, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is Base64SigEncoder::encodeTrits(); see
that routine for details of the encoding.

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Returns:
    an array containing the three trits that were decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::done
static const char *done =
R"doc(Determines whether the current position has reached the end of the
string.

.. warning::
    As of Regina 8.0, this behaviour has changed. Previously, calling
    ``done()`` with no arguments would ignore any final whitespace at
    the end of the string. Now it simply tests whether we have reached
    the end of the string. However, combined with the changes to the
    constructor, this yields the same default behaviour as before,
    since ``Base64SigDecoder(beginEncoding, endEncoding)`` will now by
    default move the endpoints of the string to ignore whitespace at
    both ends of the string (not just the start). Nevertheless, if you
    passed an extra boolean argument to the constructor then beware:
    the behaviour of ``done()`` might have changed.

Returns:
    ``True`` if and only if the current position is the end of the
    string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::done_2
static const char *done_2 =
R"doc(Deprecated routine that determines whether the current position has
reached the end of the string, optionally ignoring any final
whitespace.

.. deprecated::
    As of Regina 8.0, you should control whitespace handling by
    passing an extra boolean argument to the class constructor, not to
    done(). If you use the default behaviour for both the constructor
    and done() (i.e., without extra boolean arguments), then you will
    get the same behaviour as in previous versions of Regina (i.e.,
    whitespace will be ignored at both ends of the encoded string).
    However, if you explicitly pass boolean arguments to the
    constructor and/or done() then the behaviour might have changed;
    for details see the documentation for these individual routines.

Parameter ``ignoreWhitespace``:
    ``True`` if we should ignore any trailing whitespace. If there is
    whitespace at the current position, the current position will not
    be changed; this will merely make the test succeed if _only_
    whitespace characters remain.

Returns:
    ``True`` if and only if the current position is the end of the
    string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::isValid
static const char *isValid =
R"doc(Is the given character one of the printable base64 characters
recognised by this class?

Parameter ``c``:
    the character to examine.

Returns:
    ``True`` if and only if *c* is one of the 64 printable characters
    described in the class notes.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::peek
static const char *peek =
R"doc(Returns the character at the current position in the encoded string.
The current position will not move.

Returns:
    the character at the current position, or 0 if there are no more
    characters available.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::remaining
static const char *remaining =
R"doc(Returns the number of characters remaining in the encoded string,
counting from the current position onwards.

The routine ``done()`` will return ``True`` if and only if
``remaining()`` returns zero.

Returns:
    the number of characters remaining.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::skip
static const char *skip =
R"doc(Advances to the next position in the encoded string.

Precondition:
    The current position has not yet reached the end of the string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::skipWhitespace
static const char *skipWhitespace =
R"doc(Moves the current position past any whitespace.

The movement will stop upon reaching either a non-whitespace character
or the end of the string.)doc";

}

namespace Base64SigEncoder_ {

// Docstring regina::python::doc::Base64SigEncoder_::__default
static const char *__default = R"doc(Creates a new encoder, with an empty base64 string.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::append
static const char *append =
R"doc(Appends the given character verbatim to this encoding.

Parameter ``c``:
    the character to append. This need not be one of the 64 characters
    used in this base64 encoding; however, ideally it should be
    printable.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeBits
static const char *encodeBits =
R"doc(Encodes a sequence of bits.

The bits will be packed into base64 characters, six at a time. For
each individual base64 character, the six bits will use bits of the
underlying 6-bit integer in order from lowest to highest significance.
(The last base64 character might of course encode fewer than six bits
instead.)

The inverse to this routine is Base64SigDecoder::decodeBits().

Python:
    The template argument *BitmaskType* is taken to be Bitmask.

Parameter ``count``:
    the number of bits to encode.

Parameter ``bits``:
    a bitmask holding the bits to encode; this must be capable of
    holding at least *count* bits.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeInt
static const char *encodeInt =
R"doc(Encodes the given non-negative native C++ integer using a fixed number
of base64 characters.

Specifically, the integer *val* will be broken into *nChars* distinct
6-bit blocks, which will be encoded in order from lowest to highest
significance.

The inverse to this routine is Base64SigDecoder::decodeInt().

Exception ``InvalidArgument``:
    The given integer *val* is negative, or requires more than ``6 ×
    nChars`` bits.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``val``:
    the non-negative integer to encode.

Parameter ``nChars``:
    the number of base64 characters to use; typically this would be
    obtained through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeInts
static const char *encodeInts =
R"doc(Encodes a sequence of non-negative native C++ integers (given by an
input range), each using a fixed number of base64 characters.

Each integer in the sequence will be encoded using encodeInt(). That
is, each integer will be broken into *nChars* distinct 6-bit blocks,
which will be encoded in order from lowest to highest significance.

The inverse to this routine is Base64SigDecoder::decodeInts().

Exception ``InvalidArgument``:
    Some integer in the sequence is negative, or requires more than
    ``6 × nChars`` bits.

Python:
    The argument *sequence* should be a Python list of integers, each
    of which will be read as a native C++ ``long``.

Parameter ``sequence``:
    the sequence of integers to encode.

Parameter ``nChars``:
    the number of base64 characters to use for each integer; typically
    this would be obtained through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeSingle
static const char *encodeSingle =
R"doc(Encodes the given 6-bit integer using a single base64 character.

The inverse to this routine is Base64SigDecoder::decodeSingle().

Exception ``InvalidArgument``:
    The given integer is not between 0 and 63 inclusive.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``c``:
    an integer between 0 and 63 inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeSize
static const char *encodeSize =
R"doc(Encodes the given non-negative integer (typically representing the
size of some object), without knowing in advance how many base64
characters will be required.

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also computes (and returns) the smallest number of base64
characters required to encode any integer *x* between 0 and *size*
inclusive. In other words, it returns the smallest *b* for which any
such *x* can be encoded by calling ``encodeInt(x, b)``. Typically such
an *x* would be an _index_ into an object (e.g., a top-dimensional
simplex number, or a crossing index). Note that encodeSize() itself
might write more than *b* characters.

The inverse to this routine is Base64SigDecoder::decodeSize().

Parameter ``size``:
    the non-negative integer to encode.

Returns:
    the number of base64 characters required to write any integer
    between 0 and *size* inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeTrits
static const char *encodeTrits =
R"doc(Encodes a sequence of trits (given by an input range). A _trit_ is
either 0, 1 or 2.

The trits will be packed into base64 characters, three at a time. For
each individual base64 character, the three trits will use bits of the
underlying 6-bit integer in order from lowest to highest significance.
(The last base64 character might of course encode just one or two
trits instead.)

The inverse to this routine is Base64SigDecoder::decodeTrits(), though
that function only decodes three trits at a time.

Python:
    The argument *trits* should be a Python list.

Parameter ``trits``:
    the sequence of trits to encode. Each element of this sequence
    must be 0, 1 or 2.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::integerWidth
static const char *integerWidth =
R"doc(Returns the smallest number of base64 characters required to encode
any integer between 0 and *size* inclusive.

For example, ``integerWidth(63) == 1``, and ``integerWidth(64) == 2``.
In the special case ``size = 0``, this function will return 1.

Returns:
    the number of base64 characters required.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::reserve
static const char *reserve =
R"doc(Pre-allocates the given amount of space for the entire base64
encoding.

This calls ``std::string::reserve(capacity)``. The intent is to avoid
unnecessary reallocations as the encoding is constructed, and also to
avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final length of the encoding; however, this routine will of course be
more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected length of the _entire_ base64 encoding (not just the
    portion that is not yet encoded).)doc";

// Docstring regina::python::doc::Base64SigEncoder_::str
static const char *str =
R"doc(Returns the base64 encoding that has been constructed thus far.

Returns:
    The current base64 encoding.)doc";

}

namespace Base64SigEncoding_ {

// Docstring regina::python::doc::Base64SigEncoding_::decodeInt
static const char *decodeInt =
R"doc(Read the integer that is encoded at the beginning of the given base64
string.

It will be assumed that the integer is encoded using *nChars* base64
characters, each containing 6 bits of the integer, with the lowest-
significance bits encoded in the first characters.

The result will be assembled using the integer type *IntType*, via
bitwise OR and bitwise shift lefts. It is assumed that the programmer
has chosen an integer type large enough to contain whatever values
they expect to read.

The inverse to this routine is encodeInt().

Precondition:
    The given string contains at least *nChars* characters.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``s``:
    the string from which the encoded base64 characters should be
    read.

Parameter ``nChars``:
    the number of base64 characters to read.

Returns:
    the native integer that was encoded.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::decodeSingle
static const char *decodeSingle =
R"doc(Determine the integer value represented by the given base64 character.

The inverse to this routine is encodeSingle().

Parameter ``c``:
    a base64 character, which must be one of the 64 printable
    characters described in the class notes.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::decodeTrits
static const char *decodeTrits =
R"doc(Decodes a single base64 character into three trits, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is encodeTrits(); see that routine for
details of the encoding.

Parameter ``c``:
    the base64 character to decode.

Returns:
    an array containing the three trits that had been encoded in the
    given base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeInt
static const char *encodeInt =
R"doc(Append a base64 encoding of the given native C++ integer to the given
string. The integer will be broken into *nChars* distinct 6-bit
blocks, and the lowest-significance blocks will be written first.

The inverse to this routine is decodeInt().

Precondition:
    The given integer *val* is non-negative, and fits within 6*nChars*
    bits.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``s``:
    the string that resulting characters should be appended to.

Parameter ``val``:
    the integer to encode.

Parameter ``nChars``:
    the number of base64 characters to use.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeSingle
static const char *encodeSingle =
R"doc(Determine the base64 character that represents the given integer
value.

The inverse to this routine is decodeSingle().

Parameter ``c``:
    an integer between 0 and 63 inclusive.

Returns:
    the corresponding printable base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeTrits
static const char *encodeTrits =
R"doc(Returns a single base64 character that encodes up to three trits,
given using a fixed-size array. A _trit_ is either 0, 1 or 2.

The given trits will be packed into a single base64 character, with
the first trit representing the lowest-significance bits of the
underlying integer and so on.

The inverse to this routine is decodeTrits().

Template parameter ``nTrits``:
    the number of trits to encode; this must be between 0 and 3
    inclusive.

Parameter ``trits``:
    the array of trits to encode. Each trit must take the value 0, 1
    or 2.

Returns:
    the resulting printable base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::isValid
static const char *isValid =
R"doc(Is the given character a valid base64 character?

Returns:
    ``True`` if and only if the given character is one of the 64
    printable characters described in the class notes.)doc";

}

namespace PackedSigDecoder_ {

// Docstring regina::python::doc::PackedSigDecoder_::__init
static const char *__init =
R"doc(Creates a new decoder for the given encoded byte sequence.

The byte sequence should be passed as an iterator range. This iterator
range must remain valid for the entire lifespan of this decoder.

Python:
    Instead of an iterator range, this constructor takes a Python
    ``bytes`` object. In Python (but not C++), the decoder will also
    keep a deep copy of the byte sequence, to ensure the lifespan
    requirements.

Parameter ``beginEncoding``:
    an iterator pointing to the beginning of the encoded byte
    sequence.

Parameter ``endEncoding``:
    a past-the-end iterator that marks the end of the encoded byte
    sequence.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::decodeBits
static const char *decodeBits =
R"doc(Decodes a sequence of bits, and returns these in the form of a
bitmask. The bits would typically have been encoded using
PackedSigEncoder::encodeBits() with the same *count* argument.

Specifically, it will be assumed that the bits have been packed eight
at a time into bytes, and that within each byte the bits are stored in
order from lowest to highest significance.

The inverse to this routine is PackedSigEncoder::encodeBits().

Exception ``InvalidInput``:
    There are not enough bytes available in the encoded byte sequence
    to hold the requested number of bits.

Python:
    The template argument *BitmaskType* is taken to be Bitmask.

Template parameter ``BitmaskType``:
    the bitmask type to return; this must be capable of holding at
    least *count* bits.

Parameter ``count``:
    the number of bits to decode.

Returns:
    a bitmask holding the bits that were decoded.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::decodeInt
static const char *decodeInt =
R"doc(Decodes the next non-negative integer value, assuming this uses a
fixed number of bytes. This integer value would typically have been
encoded using PackedSigEncoder::encodeInt(), using the same *nBytes*
argument.

Specifically, it will be assumed that the integer has been broken into
*nBytes* bytes, in order from lowest to highest significance.

The result will be assembled using the integer type *IntType*, via
bitwise OR and bitwise shift lefts. It is assumed that the programmer
has chosen an integer type of size at least *nBytes* bytes.

The inverse to this routine is PackedSigEncoder::encodeInt().

Exception ``InvalidInput``:
    There are fewer than *nBytes* bytes available in the encoded byte
    sequence.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``nBytes``:
    the number of bytes to read.

Returns:
    the integer that was decoded.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::decodeInts
static const char *decodeInts =
R"doc(Decodes a sequence of non-negative integer values, assuming that each
individual value uses a fixed number of bytes, and returns these as an
array of native C++ integers. Each integer to be decoded would
typically have been encoded using PackedSigEncoder::encodeInt() or
PackedSigEncoder::encodeInts(), using the same *nBytes* argument.

Specifically, it will be assumed that each integer has been broken
into *nBytes* bytes, in order from lowest to highest significance.

Each resulting integer will be assembled using the integer type
*IntType*, via bitwise OR and bitwise shift lefts. It is assumed that
the programmer has chosen an integer type of size at least *nBytes*
bytes.

The inverse to this routine is PackedSigEncoder::encodeInts().

Exception ``InvalidInput``:
    There are fewer than ``count × nBytes`` bytes available in the
    encoded byte sequence.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``. This routine returns a Python list of integers.

Parameter ``count``:
    the number of integers to decode.

Parameter ``nBytes``:
    the number of bytes to read for each integer.

Returns:
    the sequence of integers that were decoded.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::decodeSize
static const char *decodeSize =
R"doc(Decodes the next non-negative integer value (typically representing
the size of some object), without knowing in advance how many bytes
were used to encode it. This integer value must have been encoded
using PackedSigEncoder::encodeSize().

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also returns the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* bytes. Typically these *x* would be _indices_ into an object
(e.g., top-dimensional simplex numbers, or crossing numbers). More
precisely, *b* is the same integer that was returned when *size* was
encoded using encodeSize(). Typically you would pass *b* to subsequent
calls to decodeInt().

The inverse to this routine is PackedSigEncoder::encodeSize().

Exception ``InvalidInput``:
    There are not enough bytes available in the encoded byte sequence.

Returns:
    a pair (*size*, *b*), where *size* is the integer that was
    decoded, and *b* is the number of bytes described above.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::decodeTrits
static const char *decodeTrits =
R"doc(Decodes four trits from a single byte, and returns these as a fixed-
size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is PackedSigEncoder::encodeTrits(); see
that routine for details of the encoding.

Exception ``InvalidInput``:
    There are no more bytes remaining in the encoded byte sequence.

Returns:
    an array containing the four trits that were decoded.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::done
static const char *done =
R"doc(Determines whether the current position has reached the end of the
byte sequence.

Returns:
    ``True`` if and only if the current position is the end of the
    byte sequence.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::next
static const char *next =
R"doc(Returns the next byte in the encoded byte sequence.

The byte will be treated as an unsigned integer (regardless of whether
the native ``char`` type is signed or unsigned).

Exception ``InvalidInput``:
    There are no more bytes remaining in the encoded byte sequence.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Returns:
    the corresponding integer, which will be between 0 and 255
    inclusive.)doc";

// Docstring regina::python::doc::PackedSigDecoder_::remaining
static const char *remaining =
R"doc(Returns the number of bytes remaining in the encoded byte sequence,
counting from the current position onwards.

The routine ``done()`` will return ``True`` if and only if
``remaining()`` returns zero.

Returns:
    the number of bytes remaining.)doc";

}

namespace PackedSigEncoder_ {

// Docstring regina::python::doc::PackedSigEncoder_::__default
static const char *__default = R"doc(Creates a new encoder, with an empty byte sequence.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::bytes
static const char *bytes =
R"doc(Returns the byte sequence that has been constructed thus far.

Python:
    This routine returns a Python ``bytes`` object.

Returns:
    The current byte sequence.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::encodeBits
static const char *encodeBits =
R"doc(Encodes a sequence of bits.

The bits will be packed into bytes, eight at a time. Within each
individual byte, the eight bits will be stored in order from lowest to
highest significance. (The last byte might of course hold fewer than
eight bits.)

The inverse to this routine is PackedSigDecoder::decodeBits().

Python:
    The template argument *BitmaskType* is taken to be Bitmask.

Parameter ``count``:
    the number of bits to encode.

Parameter ``bits``:
    a bitmask holding the bits to encode; this must be capable of
    holding at least *count* bits.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::encodeInt
static const char *encodeInt =
R"doc(Encodes the given non-negative native C++ integer using a fixed number
of bytes.

The bytes will be encoded in order from lowest to highest
significance.

The inverse to this routine is PackedSigDecoder::decodeInt().

Exception ``InvalidArgument``:
    The given integer *val* is negative, or requires more than the
    given number of bytes.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``val``:
    the non-negative integer to encode.

Parameter ``nBytes``:
    the number of bytes to use; typically this would be obtained
    through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::PackedSigEncoder_::encodeInts
static const char *encodeInts =
R"doc(Encodes a sequence of non-negative native C++ integers (given by an
input range), each using a fixed number of bytes.

Each integer in the sequence will be encoded using encodeInt(). That
is, each integer will be broken into *nBytes* distinct bytes, which
will be encoded in order from lowest to highest significance.

The inverse to this routine is PackedSigDecoder::decodeInts().

Exception ``InvalidArgument``:
    Some integer in the sequence is negative, or requires more than
    the given number of bytes.

Python:
    The argument *sequence* should be a Python list of integers, each
    of which will be read as a native C++ ``long``.

Parameter ``sequence``:
    the sequence of integers to encode.

Parameter ``nBytes``:
    the number of bytes to use for each integer; typically this would
    be obtained through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::PackedSigEncoder_::encodeSize
static const char *encodeSize =
R"doc(Encodes the given non-negative integer (typically representing the
size of some object), without knowing in advance how many bytes will
be required.

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also computes the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* bytes. In other words, any such *x* can be encoded by
calling ``encodeInt(x, b)``. Typically these *x* would be _indices_
into an object (e.g., top-dimensional simplex numbers, or crossing
numbers). Note that encodeSize() itself might write more than *b*
bytes.

The inverse to this routine is PackedSigDecoder::decodeSize().

Parameter ``size``:
    the non-negative integer to encode.

Returns:
    the number of bytes required to write any integer between 0 and
    *size* inclusive.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::encodeTrits
static const char *encodeTrits =
R"doc(Encodes a sequence of trits (given by an input range). A _trit_ is
either 0, 1 or 2.

The trits will be packed into bytes, four at a time. Within each
individual byte, the four trits will use bits in order from lowest to
highest significance. (The last byte might of course hold fewer than
four trits.)

The inverse to this routine is PackedSigDecoder::decodeTrits(), though
that function only decodes four trits at a time.

Python:
    The argument *trits* should be a Python list.

Parameter ``trits``:
    the sequence of trits to encode. Each element of this sequence
    must be 0, 1 or 2.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::integerWidth
static const char *integerWidth =
R"doc(Returns the smallest number of bytes required to encode any integer
between 0 and *size* inclusive.

For example, ``integerWidth(255) == 1``, and ``integerWidth(256) ==
2``. In the special case ``size = 0``, this function will return 1.

Returns:
    the number of bytes required.)doc";

// Docstring regina::python::doc::PackedSigEncoder_::reserve
static const char *reserve =
R"doc(Pre-allocates the given amount of space for the entire encoding.

This calls ``std::string::reserve(capacity)``. The intent is to avoid
unnecessary reallocations as the encoding is constructed, and also to
avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final byte length of the encoding; however, this routine will of
course be more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected byte length of the _entire_ encoding (not just the
    portion that is not yet encoded).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

