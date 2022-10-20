/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::base64Decode
static const char *base64Decode =
R"doc(Decodes the given sequence of base64 characters, and writes the
resulting raw bytes into a preallocated output buffer.

The given base64 sequence should not contain any unexpected
characters; even whitespace will cause the decoding procedure to
abort.

The length of the output buffer is passed as the argument *outlen*. If
an unexpected or invalid character is found, or the output buffer is
exhausted, this routine will write as many output bytes as it can and
then return ``False``. Otherwise (on success) it will return ``True``.
Either way, it will reset *outlen* to the total number of bytes that
were written.

The total number of output bytes is important to know, since the
output array is not terminated in any special way.

Python:
    These base64 decoding routines are made available to Python in the
    form ``base64Decode(input_string)``, where *input_string* is a
    base64 string, and the return value is a Python ``bytes`` object.
    You do not need to supply any input or output buffer lengths. If
    the decoding is unsuccessful, this routine will return ``None``.

Parameter ``in``:
    the input sequence of base64 characters; this does not need to be
    terminated in any special way.

Parameter ``inlen``:
    the length of the input sequence.

Parameter ``out``:
    the output buffer into which the resulting raw bytes will be
    written.

Parameter ``outlen``:
    must contain the length of the output buffer on entry, and on exit
    contains the number of output bytes that were successfully
    written.

Returns:
    ``True`` if decoding was successful, or ``False`` if the output
    buffer was exhausted or an unexpected input character was found.

Author:
    This routine is based on the *Base64* project at
    base64.sourceforge.net. The original was written by Bob Trower,
    and is licensed under the MIT license. See the base64.h notes for
    details.)doc";

// Docstring regina::python::doc::base64Decode_2
static const char *base64Decode_2 =
R"doc(Decodes the given sequence of base64 characters, and passes back a
newly allocated array containing the results. The *out* pointer will
be set to this new array, and *outlen* will be set to the number of
raw bytes in this output array. This array will be allocated using
``new``[], and the caller is responsible for destroying it using
``delete``[].

The given base64 sequence should not contain any unexpected
characters; even whitespace will cause the decoding procedure to
abort.

The length of the output buffer is passed as the argument *outlen*. If
an unexpected or invalid character is found or the output buffer is
exhausted, this routine will return ``False``, set *out* to ``None``,
and leave *outlen* undefined. Otherwise (on success) it will return
``True`` and set *outlen* to the total number of output bytes.

If the user is not interested in the length of the output array, a
null pointer may be passed in the *outlen* argument. Note however that
the output array is not terminated in any special way.

Python:
    These base64 decoding routines are made available to Python in the
    form ``base64Decode(input_string)``, where *input_string* is a
    base64 string, and the return value is a Python ``bytes`` object.
    You do not need to supply any input or output buffer lengths. If
    the decoding is unsuccessful, this routine will return ``None``.

Parameter ``in``:
    the input sequence of base64 characters; this does not need to be
    terminated in any special way.

Parameter ``inlen``:
    the length of the input sequence.

Parameter ``out``:
    the address of a pointer which will be set to the output array of
    raw bytes (or which will be set to ``None`` on failure).

Parameter ``outlen``:
    the address of an integer which will be set to the length of the
    output array (or which will be left undefined on failure).

Returns:
    ``True`` if decoding was successful, or ``False`` if an unexpected
    input character was found or some other error occurred.

Author:
    This routine is based on the *Base64* project at
    base64.sourceforge.net. The original was written by Bob Trower,
    and is licensed under the MIT license. See the base64.h notes for
    details.)doc";

// Docstring regina::python::doc::base64Encode
static const char *base64Encode =
R"doc(Encodes the given sequence of raw bytes in base64, and writes the
results into a preallocated output buffer.

The length of the output buffer is passed as the argument *outlen*. If
the number of base64 characters required is less than *outlen*, a
terminating null will be written to the end of the output sequence. If
the number of base64 characters is *outlen* or greater, this routine
will output as many base64 characters as possible, up to a maximum of
*outlen*.

The routine base64Length() can be used to precalculate precisely how
many output characters will be required.

Python:
    These base64 encoding routines are made available to Python in the
    form ``base64Encode(input_bytes)``, where *input_bytes* is a a
    Python ``bytes`` object, and the return value is a Python string.
    You do not need to supply any input or output buffer lengths.

Parameter ``in``:
    the sequence of input bytes; this does not need to be terminated
    in any special way.

Parameter ``inlen``:
    the length of the input sequence.

Parameter ``out``:
    the output buffer into which the resulting base64 characters will
    be written.

Parameter ``outlen``:
    the length of the output buffer.

Author:
    This routine is based on the *Base64* project at
    base64.sourceforge.net. The original was written by Bob Trower,
    and is licensed under the MIT license. See the base64.h notes for
    details.)doc";

// Docstring regina::python::doc::base64Encode_2
static const char *base64Encode_2 =
R"doc(Encodes the given sequence of raw bytes in base64, and passes back a
newly allocated array containing the results. The *out* pointer will
be set to this new array, which will be null-terminated. This array
will be allocated using ``new``[], and the caller is responsible for
destroying it using ``delete``[].

If the output array is too large (in particular, the expected size
will overflow a ``size_t``), the *out* pointer will be set to
``None``.

Python:
    These base64 encoding routines are made available to Python in the
    form ``base64Encode(input_bytes)``, where *input_bytes* is a a
    Python ``bytes`` object, and the return value is a Python string.
    You do not need to supply any input or output buffer lengths.

Parameter ``in``:
    the sequence of input bytes; this does not need to be terminated
    in any special way.

Parameter ``inlen``:
    the length of the input sequence.

Parameter ``out``:
    the address of a pointer which will be set to the output array of
    base64 characters.

Returns:
    the length of the output array, not counting the terminating null.

Author:
    This routine is based on the *Base64* project at
    base64.sourceforge.net. The original was written by Bob Trower,
    and is licensed under the MIT license. See the base64.h notes for
    details.)doc";

// Docstring regina::python::doc::base64Length
static const char *base64Length =
R"doc(Returns the number of base64 characters required to encode the given
number of bytes. This is the number of characters used (excluding the
null terminator) by the routine base64Encode(const char*, size_t,
char**).

Parameter ``bytes``:
    the number of raw input bytes.

Returns:
    the corresponding number of base64 printable output characters.)doc";

// Docstring regina::python::doc::isBase64
static const char *isBase64 =
R"doc(Determines whether the given character is a base64 printable character
as used by the base64 routines in Regina. The base64 printable
characters are the letters (both upper-case and lower-case), digits,
plus (+), and forward slash (/).

Note that the equals sign (=) is padding, and is not considered by
this routine to be a base64 printable character.

Parameter ``ch``:
    any character.

Returns:
    ``True`` if the given character is one of the base64 printable
    characters used in Regina, or ``False`` if it is not.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

