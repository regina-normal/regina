/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Base64SigEncoding
static const char *Base64SigEncoding =
R"doc(General helper tools for signatures that use base64 encodings. These
are (in particular) used in the default encodings for Regina's own
isomorphism signatures and knot signatures.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

namespace Base64SigEncoding_ {

// Docstring regina::python::doc::Base64SigEncoding_::decodeInt
static const char *decodeInt =
R"doc(Read the integer that is encoded at the beginning of the given base64
string.

It will be assumed that the integer is encoded using *nChars* base64
characters, each containing 6 bits of the integer, with the lowest-
significance bits encoded in the first characters.

The inverse to this routine is encodeInt().

Precondition:
    The given string contains at least *nChars* characters.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type, such as ``uint8_t``, or ``unsigned``,
    or ``long long``. The result will be assembled using bitwise OR
    and bitwise shift lefts, and it is assumed that the programmer has
    chosen an integer type large enough to contain whatever values
    they expect to read.

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
R"doc(Append a base64 encoding of the given integer to the given string. The
integer will be broken into *nChars* distinct 6-bit blocks, and the
lowest-significance blocks will be written first.

The inverse to this routine is decodeInt().

Precondition:
    The given integer *val* is non-negative, and fits within 6*nChars*
    bits.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type, such as ``uint8_t``, or ``unsigned``,
    or ``long long``.

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

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

