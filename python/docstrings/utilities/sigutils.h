/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Base64BitDecoder {

// Docstring regina::python::doc::Base64BitDecoder::__class
static constexpr const char __class[] =
R"doc(A helper class for reading signatures that use base64 encodings, but
that pack information as tightly as possible into bits whilst ignoring
boundaries between different base64 characters. See Base64BitEncoder
for details on how this works.

To use this class: create a new Base64BitDecoder by passing details of
the encoded string to its constructor, and then call its
``decode...()`` member functions to read values sequentially from the
encoding.

This class will keep track of a current position in the encoded bit
sequence (this position may be in the middle of a base64 character,
where some bits have been read from the character and some have not).
Each call to a ``decode...()`` member function will advance this
position accordingly (but never beyond the end of the string).

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

These decoders are single-use objects: they cannot be copied, moved or
swapped.

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.

Python:
    The type *Iterator* is an implementation detail, and is hidden
    from Python users. Just use the unadorned type name
    ``Base64BitDecoder``.)doc";

// Docstring regina::python::doc::Base64BitDecoder::__init
static constexpr const char __init[] =
R"doc(Creates a new decoder for the given encoded string.

The string itself should be passed as an iterator range. This iterator
range must remain valid for the entire lifespan of this decoder.

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

// Docstring regina::python::doc::Base64BitDecoder::decodeBit
static constexpr const char decodeBit[] =
R"doc(Returns the next bit in the encoded sequence.

Exception ``InvalidInput``:
    There are no more bits remaining in the encoded sequence.

Returns:
    ``True`` if the bit that was read is 1, or ``False`` if the bit
    that was read is 0.)doc";

// Docstring regina::python::doc::Base64BitDecoder::decodeBitmask
static constexpr const char decodeBitmask[] =
R"doc(Decodes a sequence of bits, and returns them in the form of a bitmask.

Exception ``InvalidInput``:
    There are fewer than *nBits* bits available in the encoded
    sequence.

Parameter ``nBits``:
    the number of bits to decode.

Returns:
    a bitmask holding the bits that were decoded. The bits will be
    stored in the bitmask in order from bit 0.)doc";

// Docstring regina::python::doc::Base64BitDecoder::decodeInt
static constexpr const char decodeInt[] =
R"doc(Decodes a sequence of bits, and returns them in the form of a native
unsigned integer.

Python:
    The template argument *IntType* is taken to be ``unsigned long``.

Exception ``InvalidInput``:
    There are fewer than *nBits* bits available in the encoded string.

Template parameter ``IntType``:
    the unsigned integer type to return; this must be at least *nBits*
    bits in size.

Parameter ``nBits``:
    the number of bits to decode.

Parameter ``bits``:
    an integer holding the bits that were decoded. The bits will be
    stored in order from the least significant bit.)doc";

// Docstring regina::python::doc::Base64BitDecoder::decodeSize
static constexpr const char decodeSize[] =
R"doc(Decodes a non-negative integer value that has been stored in some
number of whole base64 characters, without knowing in advance how many
base64 characters were used to encode it. This integer value must have
been encoded using Base64BitEncoder::encodeSize() (or the equivalent
Base64Encoder::encodeSize()).

Like the inverse routine Base64BitEncoder::encodeSize(), this is
intended to be called at the beginning of an encoding. It is possible
to call it at other positions; however, if the current reading
position is in the middle of a base64 character (i.e., some but not
all of the six bits for that character have been read), then this
routine will throw an exception.

This routine will read the same characters and return the same decoded
value as ``Base64Decoder::decodeSize()``. However, it only returns the
decoded integer, and not an extra integer byte width, since subsequent
data would typically be decoded on a bit-by-bit basis, not a
character-by-character basis.

Precondition:
    This decoder is currently positioned at a character boundary. That
    is, it is _not_ in a state where some but not all of the six bits
    have been read from the last base64 character that was extracted.

Exception ``FailedPrecondition``:
    This decoder is not positioned at a character boundary, as
    described above.

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string,
    or a character was encountered that was not a valid base64
    character.

Returns:
    the integer that was decoded.)doc";

// Docstring regina::python::doc::Base64BitDecoder::flushChar
static constexpr const char flushChar[] =
R"doc(Skips past unread bits until we reach the next base64 character
boundary.

This routine will test that all bits that are skipped are off;
otherwise it will throw an exception. The number of bits skipped will
be between 0 and 5 inclusive.

Exception ``InvalidInput``:
    At least one of the bits that was skipped was set.)doc";

// Docstring regina::python::doc::Base64BitDecoder::maybeDone
static constexpr const char maybeDone[] =
R"doc(Determines if the current position _could_ have reached the end of the
encoded bit sequence. The word "maybe" acknowledges that the precise
end of the bit sequence is often unclear (since the sequence is
presented in base64 characters, without knowing how many bits of the
final character were actually used).

This will return ``True`` if:

* there are no remaining base64 _characters_ that we have not read
  from at all; and,

* of the last character that we did read from (if any), all of the
  _bits_ that have not yet been read are set to zero.

Returns:
    ``True`` if and only if we could be at the end of the encoded bit
    sequence, as described above.)doc";

// Docstring regina::python::doc::Base64BitDecoder::noMoreBits
static constexpr const char noMoreBits[] =
R"doc(Determines if there are no more available bits to read.

This will return ``True`` when we have already read all six bits from
every base64 character of the input string.

Returns:
    ``True`` if and only if there are no more available bits.)doc";

// Docstring regina::python::doc::Base64BitDecoder::peek
static constexpr const char peek[] =
R"doc(Returns the base64 character at the current position in the encoded
string, assuming that this position is at a character boundary. The
current position will not move (i.e., the character that is returned
will remain available to be read from again later).

Precondition:
    This decoder is currently positioned at a character boundary. That
    is, it is _not_ in a state where some but not all of the six bits
    have been read from the last base64 character that was extracted.

Exception ``FailedPrecondition``:
    This decoder is not positioned at a character boundary, as
    described above.

Returns:
    the character at the current position, or 0 if there are no more
    characters available.)doc";

// Docstring regina::python::doc::Base64BitDecoder::remainingBits
static constexpr const char remainingBits[] =
R"doc(Returns the number of bits that can still be read from the encoded
string, counting from the current position onwards.

The routine ``noMoreBits()`` will return ``True`` if and only if
``remainingBits()`` returns zero.

Returns:
    the number of bits remaining.)doc";

// Docstring regina::python::doc::Base64BitDecoder::skip
static constexpr const char skip[] =
R"doc(Advances to the next position in the encoded base64 string, assuming
that the current position is at a character boundary.

Precondition:
    This decoder is currently positioned at a character boundary. That
    is, it is _not_ in a state where some but not all of the six bits
    have been read from the last base64 character that was extracted.

Exception ``FailedPrecondition``:
    This decoder is not positioned at a character boundary, as
    described above.

Precondition:
    The current position has not yet reached the end of the string.)doc";

}; // struct Base64BitDecoder

struct Base64BitEncoder {

// Docstring regina::python::doc::Base64BitEncoder::__class
static constexpr const char __class[] =
R"doc(A helper class for writing signatures that pack information as tightly
as possible into bits whilst ignoring byte/character boundaries, but
then writes its actual output as a printable base64 string.

This class is a hybrid between Base64Encoder and BitEncoder: it
attempts to combine the readability of the former with the efficiency
of the latter. (Of course it cannot be as efficient as BitEncoder,
which is able to use all eight bits in each byte.)

To use this class: create a new Base64BitEncoder, call one or more of
its member functions to write values to the encoding, and then call
str() to extract the resulting encoded string. Like
BitEncoder::bytes(), this call to str() will invalidate the encoder,
which means that after calling str() you cannot encode more data
and/or call str() again.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

These encoders are single-use objects: they cannot be copied, moved or
swapped.

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

// Docstring regina::python::doc::Base64BitEncoder::__default
static constexpr const char __default[] = R"doc(Creates a new encoder, with an empty base64 string.)doc";

// Docstring regina::python::doc::Base64BitEncoder::encodeBit
static constexpr const char encodeBit[] =
R"doc(Encodes the given boolean as a single bit.

Parameter ``bit``:
    ``True`` if we should encode the bit 1, or ``False`` if we should
    encode the bit 0.)doc";

// Docstring regina::python::doc::Base64BitEncoder::encodeBitmask
static constexpr const char encodeBitmask[] =
R"doc(Encodes a sequence of bits, taken from the given bitmask.

Parameter ``bits``:
    a bitmask holding the bits to encode; this bitmask must be capable
    of holding at least *nBits* bits. The bits will be encoded in
    order from bit 0 of the given bitmask.

Parameter ``nBits``:
    the total number of bits to encode.)doc";

// Docstring regina::python::doc::Base64BitEncoder::encodeInt
static constexpr const char encodeInt[] =
R"doc(Encodes a sequence of bits, all taken from a single native unsigned
integer.

Python:
    The template argument *IntType* is taken to be ``unsigned long``.

Exception ``InvalidArgument``:
    The given integer has some bit set beyond bits
    ``0,...,(nBits-1)``.

Parameter ``bits``:
    an integer holding the bits to encode; these will be encoded in
    order from the least significant bit of the argument *bits*.

Parameter ``nBits``:
    the total number of bits to encode; this must be strictly
    positive.)doc";

// Docstring regina::python::doc::Base64BitEncoder::encodeSize
static constexpr const char encodeSize[] =
R"doc(Encodes the given non-negative integer across some number of whole
base64 characters, without knowing in advance how many characters will
be required.

This is intended to be called at the beginning of an encoding. It is
possible to call it at other positions; however, if the current
writing position is in the middle of a base64 character (i.e., some
but not all of the six bits for that character have already been
supplied), then this routine will throw an exception.

This routine will write exactly the same base64 characters as
``Base64Encoder::encodeSize(size)``. It does not return an integer
byte width, however, since subsequent data would typically be encoded
on a bit-by-bit basis, not a character-by-character basis.

When decoding the resulting string, you would typically need to use
Base64BitEncoder::decodeSize().

Precondition:
    This encoder is currently positioned at a character boundary. That
    is, it is _not_ in a state where some but not all of the six bits
    have been supplied for the next base64 character that will be
    written.

Exception ``FailedPrecondition``:
    This encoder is not positioned at a character boundary, as
    described above.

Parameter ``size``:
    the non-negative integer to encode.)doc";

// Docstring regina::python::doc::Base64BitEncoder::flushAndAppend
static constexpr const char flushAndAppend[] =
R"doc(Advances the position of the encoder to the next character boundary if
necessary, and then appends the given character verbatim to the
encoded string.

If this encoder is already positioned at a character boundary, this
routine will simply append the character *c* to the encoded string.
Otherwise - if some but not all of the six bits have been supplied for
the next pending base64 character to be written - it will write that
pending character immediately (as though the remaining bits were all
zero) and _then_ append *c* as a separate character to the string.

Parameter ``c``:
    the character to append. This need not be one of the 64 characters
    used in this base64 encoding; however, ideally it should be
    printable.)doc";

// Docstring regina::python::doc::Base64BitEncoder::flushChar
static constexpr const char flushChar[] =
R"doc(Advances the position of the encoder to the next character boundary,
if it is not at one already.

If this encoder is already positioned at a character boundary, this
routine will do nothing. Otherwise - if some but not all of the six
bits have been supplied for the next pending base64 character to be
written - it will write that pending character immediately (as though
the remaining bits were all zero).)doc";

// Docstring regina::python::doc::Base64BitEncoder::reserveBits
static constexpr const char reserveBits[] =
R"doc(Pre-allocates the given amount of space for the entire encoding, as
measured in bits.

Internally, this calls ``std::string::reserve(...)``. The intent is to
avoid unnecessary reallocations as the encoding is constructed, and
also to avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final bit length of the encoding; however, this routine will of course
be more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected total number of bits in the _entire_ encoding (not
    just the portion that is not yet encoded).)doc";

// Docstring regina::python::doc::Base64BitEncoder::reserveChars
static constexpr const char reserveChars[] =
R"doc(Pre-allocates the given amount of space for the entire encoding, as
measured in characters.

Internally, this calls ``std::string::reserve(capacity)``. The intent
is to avoid unnecessary reallocations as the encoding is constructed,
and also to avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final byte length of the encoding; however, this routine will of
course be more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected string length of the _entire_ encoding (not just the
    portion that is not yet encoded).)doc";

// Docstring regina::python::doc::Base64BitEncoder::str
static constexpr const char str[] =
R"doc(Moves the final encoded base64 string out of this encoder.

After calling this function, this encoder object will be unusable.

Returns:
    the final base64 encoding.)doc";

}; // struct Base64BitEncoder

struct Base64Decoder {

// Docstring regina::python::doc::Base64Decoder::__class
static constexpr const char __class[] =
R"doc(A helper class for reading signatures that use base64 encodings. These
are (in particular) used in the encodings for Regina's first-
generation isomorphism signatures and knot signatures.

To use this class: create a new Base64Decoder by passing details of
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
    ``Base64Decoder``.)doc";

// Docstring regina::python::doc::Base64Decoder::__init
static constexpr const char __init[] =
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

// Docstring regina::python::doc::Base64Decoder::decode
static constexpr const char decode[] =
R"doc(Converts a single base64 character into its corresponding 6-bit
integer.

Exception ``InvalidArgument``:
    The given character is not one of the 64 printable base64
    characters recognised by this class.

Parameter ``c``:
    a single base64 character.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64Decoder::decodeBitmask
static constexpr const char decodeBitmask[] =
R"doc(Decodes a sequence of bits, and returns these in the form of a
bitmask. The bits would typically have been encoded using
Base64Encoder::encodeBitmask() with the same *nBits* argument.

Specifically, it will be assumed that the bits have been packed six at
a time into base64 characters, and that for each underlying 6-bit
integer, the bits are stored in order from lowest to highest
significance.

The inverse to this routine is Base64Encoder::encodeBitmask().

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string to
    hold the requested number of bits, and/or a character was
    encountered that was not a valid base64 character.

Parameter ``nBits``:
    the number of bits to decode.

Returns:
    a bitmask holding the bits that were decoded.)doc";

// Docstring regina::python::doc::Base64Decoder::decodeInt
static constexpr const char decodeInt[] =
R"doc(Decodes the next non-negative integer value, assuming this uses a
fixed number of base64 characters. This integer value would typically
have been encoded using Base64Encoder::encodeInt(), using the same
*nChars* argument.

Specifically, it will be assumed that the integer has been broken into
*nChars* 6-bit blocks, with each block encoded as a single base64
character, and with the blocks presented in order from lowest to
highest significance.

The result will be assembled using the integer type *IntType*, via
bitwise OR and bitwise shift lefts. It is assumed that the programmer
has chosen an integer type large enough to contain whatever values
they expect to read.

The inverse to this routine is Base64Encoder::encodeInt().

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

// Docstring regina::python::doc::Base64Decoder::decodeInts
static constexpr const char decodeInts[] =
R"doc(Decodes a sequence of non-negative integer values, assuming that each
individual value uses a fixed number of base64 characters, and returns
these as an array of native C++ integers. Each integer to be decoded
would typically have been encoded using Base64Encoder::encodeInts() or
Base64Encoder::encodeInt(), using the same *charsPerInt* argument.

Specifically, it will be assumed that each integer has been broken
into *charsPerInt* 6-bit blocks, with each block encoded as a single
base64 character, and with the blocks presented in order from lowest
to highest significance.

Each resulting integer will be assembled using the integer type
*IntType*, via bitwise OR and bitwise shift lefts. It is assumed that
the programmer has chosen an integer type large enough to contain
whatever values they expect to read.

The inverse to this routine is Base64Encoder::encodeInts().

Exception ``InvalidInput``:
    There are fewer than ``count × charsPerInt`` characters available
    in the encoded string, or a character was encountered that was not
    a valid base64 character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``. This routine returns a Python list of integers.

Parameter ``count``:
    the number of integers to decode.

Parameter ``charsPerInt``:
    the number of base64 characters to read for each integer.

Returns:
    the sequence of integers that were decoded.)doc";

// Docstring regina::python::doc::Base64Decoder::decodeSingle
static constexpr const char decodeSingle[] =
R"doc(Decodes the 6-bit integer value represented by the next single base64
character.

The inverse to this routine is Base64Encoder::encodeSingle().

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64Decoder::decodeSize
static constexpr const char decodeSize[] =
R"doc(Decodes the next non-negative integer value (typically representing
the size of some object), without knowing in advance how many base64
characters were used to encode it. This integer value must have been
encoded using Base64Encoder::encodeSize().

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also returns the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* base64 characters. This *b* is the same integer that was
returned when *size* was encoded using encodeSize(), and typically you
would pass *b* to subsequent calls to decodeInt().

The inverse to this routine is Base64Encoder::encodeSize().

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string,
    or a character was encountered that was not a valid base64
    character.

Returns:
    a pair (*size*, *b*), where *size* is the integer that was
    decoded, and *b* is the number of base64 characters described
    above.)doc";

// Docstring regina::python::doc::Base64Decoder::decodeTrits
static constexpr const char decodeTrits[] =
R"doc(Decodes three trits from a single base64 character, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is Base64Encoder::encodeTrits(); see that
routine for details of the encoding.

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Returns:
    an array containing the three trits that were decoded.)doc";

// Docstring regina::python::doc::Base64Decoder::done
static constexpr const char done[] =
R"doc(Determines whether the current position has reached the end of the
string.

.. warning::
    As of Regina 8.0, this behaviour has changed. Previously, calling
    ``done()`` with no arguments would ignore any final whitespace at
    the end of the string. Now it simply tests whether we have reached
    the end of the string. However, combined with the changes to the
    constructor, this yields the same default behaviour as before,
    since ``Base64Decoder(beginEncoding, endEncoding)`` will now by
    default move the endpoints of the string to ignore whitespace at
    both ends of the string (not just the start). Nevertheless, if you
    passed an extra boolean argument to the constructor then beware:
    the behaviour of ``done()`` might have changed.

Returns:
    ``True`` if and only if the current position is the end of the
    string.)doc";

// Docstring regina::python::doc::Base64Decoder::done_2
static constexpr const char done_2[] =
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

// Docstring regina::python::doc::Base64Decoder::isValid
static constexpr const char isValid[] =
R"doc(Is the given character one of the printable base64 characters
recognised by this class?

Parameter ``c``:
    the character to examine.

Returns:
    ``True`` if and only if *c* is one of the 64 printable characters
    described in the class notes.)doc";

// Docstring regina::python::doc::Base64Decoder::peek
static constexpr const char peek[] =
R"doc(Returns the character at the current position in the encoded string.
The current position will not move (i.e., the character that is
returned will remain available to be read again later).

Returns:
    the character at the current position, or 0 if there are no more
    characters available.)doc";

// Docstring regina::python::doc::Base64Decoder::remaining
static constexpr const char remaining[] =
R"doc(Returns the number of characters remaining in the encoded string,
counting from the current position onwards.

The routine ``done()`` will return ``True`` if and only if
``remaining()`` returns zero.

Returns:
    the number of characters remaining.)doc";

// Docstring regina::python::doc::Base64Decoder::skip
static constexpr const char skip[] =
R"doc(Advances to the next position in the encoded string.

Precondition:
    The current position has not yet reached the end of the string.)doc";

// Docstring regina::python::doc::Base64Decoder::skipWhitespace
static constexpr const char skipWhitespace[] =
R"doc(Moves the current position past any whitespace.

The movement will stop upon reaching either a non-whitespace character
or the end of the string.)doc";

// Docstring regina::python::doc::Base64Decoder::unreadBitDecoder
static constexpr const char unreadBitDecoder[] =
R"doc(Returns a bitwise decoder for the range of base64 characters that this
decoder has not yet read.

This may be useful if, as a result of some runtime decision, you need
to switch from character-by-character decoding to bit-by-bit decoding.

The decoder that is returned will _only_ see those base64 characters
that this decoder has not yet read (here a call to peek() is not
considered as having read the character).

This base64 decoder will remain valid, and its own reading position
will not change.

Returns:
    a bitwise decoder for the characters not yet read.)doc";

}; // struct Base64Decoder

struct Base64Encoder {

// Docstring regina::python::doc::Base64Encoder::__class
static constexpr const char __class[] =
R"doc(A helper class for writing signatures that use base64 encodings. These
are (in particular) used in the encodings for Regina's first-
generation isomorphism signatures and knot signatures.

To use this class: create a new Base64Encoder, call one or more of its
member functions to write values to the encoding, and then call str()
to extract the resulting base64 string.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

Base64 encoders are single-use objects: they cannot be copied, moved
or swapped.

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

// Docstring regina::python::doc::Base64Encoder::__default
static constexpr const char __default[] = R"doc(Creates a new encoder, with an empty base64 string.)doc";

// Docstring regina::python::doc::Base64Encoder::append
static constexpr const char append[] =
R"doc(Appends the given character verbatim to this encoding.

Parameter ``c``:
    the character to append. This need not be one of the 64 characters
    used in this base64 encoding; however, ideally it should be
    printable.)doc";

// Docstring regina::python::doc::Base64Encoder::encodeBitmask
static constexpr const char encodeBitmask[] =
R"doc(Encodes a sequence of bits.

The bits will be packed into base64 characters, six at a time. For
each individual base64 character, the six bits will use bits of the
underlying 6-bit integer in order from lowest to highest significance.
(The last base64 character might of course encode fewer than six bits
instead.)

The inverse to this routine is Base64Decoder::decodeBitmask().

Parameter ``bits``:
    a bitmask holding the bits to encode; this must be capable of
    holding at least *nBits* bits.

Parameter ``nBits``:
    the number of bits to encode.)doc";

// Docstring regina::python::doc::Base64Encoder::encodeInt
static constexpr const char encodeInt[] =
R"doc(Encodes the given non-negative native C++ integer using a fixed number
of base64 characters.

Specifically, the integer *val* will be broken into *nChars* distinct
6-bit blocks, which will be encoded in order from lowest to highest
significance.

The inverse to this routine is Base64Decoder::decodeInt().

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

// Docstring regina::python::doc::Base64Encoder::encodeInts
static constexpr const char encodeInts[] =
R"doc(Encodes a sequence of non-negative native C++ integers (given by an
input range), each using a fixed number of base64 characters.

Each integer in the sequence will be encoded using encodeInt(). That
is, each integer will be broken into *charsPerInt* distinct 6-bit
blocks, which will be encoded in order from lowest to highest
significance.

The inverse to this routine is Base64Decoder::decodeInts().

Exception ``InvalidArgument``:
    Some integer in the sequence is negative, or requires more than
    ``6 × charsPerInt`` bits.

Python:
    The argument *sequence* should be a Python list of integers, each
    of which will be read as a native C++ ``long``.

Parameter ``sequence``:
    the sequence of integers to encode.

Parameter ``charsPerInt``:
    the number of base64 characters to use for each integer; typically
    this would be obtained through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::Base64Encoder::encodeSingle
static constexpr const char encodeSingle[] =
R"doc(Encodes the given 6-bit integer using a single base64 character.

The inverse to this routine is Base64Decoder::decodeSingle().

Exception ``InvalidArgument``:
    The given integer is not between 0 and 63 inclusive.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Parameter ``c``:
    an integer between 0 and 63 inclusive.)doc";

// Docstring regina::python::doc::Base64Encoder::encodeSize
static constexpr const char encodeSize[] =
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

The inverse to this routine is Base64Decoder::decodeSize().

Parameter ``size``:
    the non-negative integer to encode.

Returns:
    the number of base64 characters required to write any integer
    between 0 and *size* inclusive.)doc";

// Docstring regina::python::doc::Base64Encoder::encodeTrits
static constexpr const char encodeTrits[] =
R"doc(Encodes a sequence of trits (given by an input range). A _trit_ is
either 0, 1 or 2.

The trits will be packed into base64 characters, three at a time. For
each individual base64 character, the three trits will use bits of the
underlying 6-bit integer in order from lowest to highest significance.
(The last base64 character might of course encode just one or two
trits instead.)

The inverse to this routine is Base64Decoder::decodeTrits(), though
that function only decodes three trits at a time.

Python:
    The argument *trits* should be a Python list.

Parameter ``trits``:
    the sequence of trits to encode. Each element of this sequence
    must be 0, 1 or 2.)doc";

// Docstring regina::python::doc::Base64Encoder::integerWidth
static constexpr const char integerWidth[] =
R"doc(Returns the smallest number of base64 characters required to encode
any integer between 0 and *size* inclusive.

For example, ``integerWidth(63) == 1``, and ``integerWidth(64) == 2``.
In the special case ``size = 0``, this function will return 1.

Returns:
    the number of base64 characters required.)doc";

// Docstring regina::python::doc::Base64Encoder::reserve
static constexpr const char reserve[] =
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

// Docstring regina::python::doc::Base64Encoder::str
static constexpr const char str[] =
R"doc(Returns the base64 encoding that has been constructed thus far.

Returns:
    the current base64 encoding.)doc";

}; // struct Base64Encoder

struct Base64SigEncoding {

// Docstring regina::python::doc::Base64SigEncoding::__class
static constexpr const char __class[] =
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
    This is now deprecated in favour of the new classes Base64Encoder
    and Base64Decoder, which carry state and have better error
    handling.)doc";

// Docstring regina::python::doc::Base64SigEncoding::decodeInt
static constexpr const char decodeInt[] =
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

// Docstring regina::python::doc::Base64SigEncoding::decodeSingle
static constexpr const char decodeSingle[] =
R"doc(Determine the integer value represented by the given base64 character.

The inverse to this routine is encodeSingle().

Parameter ``c``:
    a base64 character, which must be one of the 64 printable
    characters described in the class notes.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoding::decodeTrits
static constexpr const char decodeTrits[] =
R"doc(Decodes a single base64 character into three trits, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is encodeTrits(); see that routine for
details of the encoding.

Parameter ``c``:
    the base64 character to decode.

Returns:
    an array containing the three trits that had been encoded in the
    given base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding::encodeInt
static constexpr const char encodeInt[] =
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

// Docstring regina::python::doc::Base64SigEncoding::encodeSingle
static constexpr const char encodeSingle[] =
R"doc(Determine the base64 character that represents the given integer
value.

The inverse to this routine is decodeSingle().

Parameter ``c``:
    an integer between 0 and 63 inclusive.

Returns:
    the corresponding printable base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding::encodeTrits
static constexpr const char encodeTrits[] =
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

// Docstring regina::python::doc::Base64SigEncoding::isValid
static constexpr const char isValid[] =
R"doc(Is the given character a valid base64 character?

Returns:
    ``True`` if and only if the given character is one of the 64
    printable characters described in the class notes.)doc";

}; // struct Base64SigEncoding

struct BitDecoder {

// Docstring regina::python::doc::BitDecoder::__class
static constexpr const char __class[] =
R"doc(A helper class for reading signatures that pack information as tightly
as possible into bits, with no regard for boundaries between bytes in
the final signature.

To use this class: create a new BitDecoder by passing details of the
encoded byte sequence to its constructor, and then call its
``decode...()`` member functions to read values sequentially from the
encoding.

This class will keep track of a current position in the encoded bit
sequence (this position may be in the middle of a byte, where some
bits of the byte have been read and some have not). Each call to a
``decode...()`` member function will advance this position accordingly
(but never beyond the end of the sequence).

Bit decoders are single-use objects: they cannot be copied, moved or
swapped.

Python:
    The type *Iterator* is an implementation detail, and is hidden
    from Python users. Just use the unadorned type name
    ``BitDecoder``.)doc";

// Docstring regina::python::doc::BitDecoder::__init
static constexpr const char __init[] =
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

// Docstring regina::python::doc::BitDecoder::decodeBit
static constexpr const char decodeBit[] =
R"doc(Returns the next bit in the encoded sequence.

Exception ``InvalidInput``:
    There are no more bits remaining in the encoded sequence.

Returns:
    ``True`` if the bit that was read is 1, or ``False`` if the bit
    that was read is 0.)doc";

// Docstring regina::python::doc::BitDecoder::decodeBitmask
static constexpr const char decodeBitmask[] =
R"doc(Decodes a sequence of bits, and returns them in the form of a bitmask.

Exception ``InvalidInput``:
    There are fewer than *nBits* bits available in the encoded
    sequence.

Parameter ``nBits``:
    the number of bits to decode.

Returns:
    a bitmask holding the bits that were decoded. The bits will be
    stored in the bitmask in order from bit 0.)doc";

// Docstring regina::python::doc::BitDecoder::decodeInt
static constexpr const char decodeInt[] =
R"doc(Decodes a sequence of bits, and returns them in the form of a native
unsigned integer.

Python:
    The template argument *IntType* is taken to be ``unsigned long``.

Exception ``InvalidInput``:
    There are fewer than *nBits* bits available in the encoded
    sequence.

Template parameter ``IntType``:
    the unsigned integer type to return; this must be at least *nBits*
    bits in size.

Parameter ``nBits``:
    the number of bits to decode.

Parameter ``bits``:
    an integer holding the bits that were decoded. The bits will be
    stored in order from the least significant bit.)doc";

// Docstring regina::python::doc::BitDecoder::flushByte
static constexpr const char flushByte[] =
R"doc(Skips past unread bits until we reach the next byte boundary.

This routine will test that all bits that are skipped are off;
otherwise it will throw an exception. The number of bits skipped will
be between 0 and 7 inclusive.

Exception ``InvalidInput``:
    At least one of the bits that was skipped was set.)doc";

// Docstring regina::python::doc::BitDecoder::maybeDone
static constexpr const char maybeDone[] =
R"doc(Determines if the current position _could_ have reached the end of the
encoded bit sequence. The word "maybe" acknowledges that the precise
end of the bit sequence is often unclear (since the sequence is
presented in bytes, without knowing how many bits of the final byte
were actually used).

This will return ``True`` if:

* there are no remaining _bytes_ that we have not read from at all;
  and,

* of the last byte that we did read from (if any), all of the _bits_
  that have not yet been read are set to zero.

Returns:
    ``True`` if and only if we could be at the end of the encoded bit
    sequence, as described above.)doc";

// Docstring regina::python::doc::BitDecoder::noMoreBits
static constexpr const char noMoreBits[] =
R"doc(Determines if there are no more available bits to read.

This will return ``True`` when we have already read all eight bits
from every byte of the input sequence.

Returns:
    ``True`` if and only if there are no more available bits.)doc";

// Docstring regina::python::doc::BitDecoder::remainingBits
static constexpr const char remainingBits[] =
R"doc(Returns the number of bits that can still be read from the encoded
sequence, counting from the current position onwards.

The routine ``noMoreBits()`` will return ``True`` if and only if
``remainingBits()`` returns zero.

Returns:
    the number of bits remaining.)doc";

}; // struct BitDecoder

struct BitEncoder {

// Docstring regina::python::doc::BitEncoder::__class
static constexpr const char __class[] =
R"doc(A helper class for writing signatures that pack information as tightly
as possible into bits, with no regard for boundaries between bytes in
the final signature.

Note that, unlike Base64Encoder::str(), the "extraction" method
bytes() is an rvalue member function: you can only call it once (after
you have encoded everything that you need), and after this the encoder
will be unusable.

To use this class: create a new BitEncoder, call one or more of its
member functions to write values to the encoding, and then call
bytes() to extract the resulting byte sequence. As noted above, this
call to bytes() must be the last thing that you do with the encoder:
you cannot encode more bits and/or call bytes() again.

Bit encoders are single-use objects: they cannot be copied, moved or
swapped.)doc";

// Docstring regina::python::doc::BitEncoder::__default
static constexpr const char __default[] = R"doc(Creates a new encoder, with an empty byte sequence.)doc";

// Docstring regina::python::doc::BitEncoder::bytes
static constexpr const char bytes[] =
R"doc(Moves the final encoded byte sequence out of this encoder.

After calling this function, this encoder object will be unusable.

Returns:
    the final encoded byte sequence.)doc";

// Docstring regina::python::doc::BitEncoder::encodeBit
static constexpr const char encodeBit[] =
R"doc(Encodes the given boolean as a single bit.

Parameter ``bit``:
    ``True`` if we should encode the bit 1, or ``False`` if we should
    encode the bit 0.)doc";

// Docstring regina::python::doc::BitEncoder::encodeBitmask
static constexpr const char encodeBitmask[] =
R"doc(Encodes a sequence of bits, taken from the given bitmask.

Parameter ``bits``:
    a bitmask holding the bits to encode; this bitmask must be capable
    of holding at least *nBits* bits. The bits will be encoded in
    order from bit 0 of the given bitmask.

Parameter ``nBits``:
    the total number of bits to encode.)doc";

// Docstring regina::python::doc::BitEncoder::encodeInt
static constexpr const char encodeInt[] =
R"doc(Encodes a sequence of bits, all taken from a single native unsigned
integer.

Python:
    The template argument *IntType* is taken to be ``unsigned long``.

Exception ``InvalidArgument``:
    The given integer has some bit set beyond bits
    ``0,...,(nBits-1)``.

Parameter ``bits``:
    an integer holding the bits to encode; these will be encoded in
    order from the least significant bit of the argument *bits*.

Parameter ``nBits``:
    the total number of bits to encode; this must be strictly
    positive.)doc";

// Docstring regina::python::doc::BitEncoder::reserveBits
static constexpr const char reserveBits[] =
R"doc(Pre-allocates the given amount of space for the entire encoding, as
measured in bits.

Internally, this calls ``ByteSequence::reserve(...)``. The intent is
to avoid unnecessary reallocations as the encoding is constructed, and
also to avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final bit length of the encoding; however, this routine will of course
be more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected total number of bits in the _entire_ encoding (not
    just the portion that is not yet encoded).)doc";

// Docstring regina::python::doc::BitEncoder::reserveBytes
static constexpr const char reserveBytes[] =
R"doc(Pre-allocates the given amount of space for the entire encoding, as
measured in bytes.

Internally, this calls ``ByteSequence::reserve(capacity)``. The intent
is to avoid unnecessary reallocations as the encoding is constructed,
and also to avoid allocating more memory than is required.

It is harmless if *capacity* ends up being smaller or larger than the
final byte length of the encoding; however, this routine will of
course be more effective if *capacity* is accurate.

Parameter ``capacity``:
    the expected total number of bytes in the _entire_ encoding (not
    just the portion that is not yet encoded).)doc";

}; // struct BitEncoder

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

