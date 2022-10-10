/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TightEncodable
static const char *TightEncodable =
R"doc(A base class that assists with support for tight encodings and
corresponding decodings.

If a class *T* supports tight encodings, then it *may* derive from
TightEncodable<T>. If it does, then your derived class must provide
the following two functions, which implement tight encodings and
decodings via input/output streams:

* ``void tightEncode(std::ostream&) const``, which writes a tight
  encoding of the object to the given output stream. This is allowed
  to (but not required to) throw a FailedPrecondition if the object is
  in an invalid state; if so then the exception should be documented
  in this member function T::tightEncode().

* ``static T tightDecode(std::istream&)``, which reconstructs an
  object of type *T* from a tight encoding that is read from the given
  input stream. This routine must not skip leading whitespace, and
  must leave the input stream positioned immediately after the
  encoding (without consuming any trailing whitespace or other
  characters). This should throw an InvalidInput exception if the
  input stream does not begin with a valid tight encoding of an object
  of type *T*.

In return, this base class will provide the following two functions,
both of which work with strings (and which are documented in full
below):

* ``std::string tightEncoding() const``; and

* ``static T tightDecoding(const std::string&)``.

A class *T* that supports tight encodings does not *need* to derive
from TightEncodable. However, if it does not then it should implement
all four of the above functions itself. Examples of this include the
permutation classes (which have optimised implementations due to their
very small space requirements), and the arbitrary-precision integer
classes (which use the global integer encoding/decoding routines).

Template parameter ``T``:
    the type of object being encoded/decoded; this must derive from
    TightEncodable<T>.

.. note::
    Every object of this class that is ever instantiated *must* be
    derived from the class *T*. In other words, end users cannot
    construct objects of the parent class TightEncodable<T>.

Python:
    Not present, but the routines tightEncoding() and tightDecoding()
    will be provided directly through the various subclasses.)doc";

// Docstring regina::python::doc::tightDecoding
static const char *tightDecoding =
R"doc(Reconstructs an integer or boolean from its given tight encoding. See
the page on tight "tight encodings" for details.

The tight encoding should be given as a string. If this string
contains leading whitespace or any trailing characters at all
(including trailing whitespace), then it will be treated as an invalid
encoding (i.e., this routine will throw an exception).

This routine does recognise infinity in the case where *Int* is the
type regina::LargeInteger.

If *Int* is one of Regina's arbitrary precision integer types, then
this routine is identical to calling Int::tightDecoding().

Exception ``InvalidArgument``:
    The given string is not a tight encoding of an integer/boolean of
    type *Int*. This includes the case where the encoding *is* a valid
    integer encoding but the integer itself is outside the allowed
    range for the *Int* type.

Python:
    Since Python does not support templates, the interface for this
    routine is a little different. The global routine
    regina::tightDecoding() will return a Python integer; since these
    are arbitrary precision, the decoding will never encounter an out-
    of-range exceptions as it might with a native C++ integer type. If
    you are trying to reconstruct a boolean, then the integer you
    receive will be either 1 or 0 to represent ``True`` or ``False``
    respectively. If you are trying to reconstruct one of Regina's
    arbitrary precision integer types, you should instead call
    Integer::tightDecoding() or LargeInteger::tightDecoding(), which
    will return a Regina integer instead of a Python integer.

Template parameter ``Int``:
    The type of integer/boolean to reconstruct; this must be either
    (i) a native C++ integer type or ``bool``, or (ii) one of Regina's
    arbitrary precision integer types (i.e., regina::Integer or
    regina::LargeInteger).

Parameter ``enc``:
    the tight encoding for an integer or boolean.

Returns:
    the integer or boolean represented by the given tight encoding.)doc";

// Docstring regina::python::doc::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of the given signed integer. See the page
on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_2
static const char *tightEncoding_2 =
R"doc(Returns the tight encoding of the given signed long integer. See the
page on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_3
static const char *tightEncoding_3 =
R"doc(Returns the tight encoding of the given signed long long integer. See
the page on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_4
static const char *tightEncoding_4 =
R"doc(Returns the tight encoding of the given unsigned integer. See the page
on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_5
static const char *tightEncoding_5 =
R"doc(Returns the tight encoding of the given unsigned long integer. See the
page on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_6
static const char *tightEncoding_6 =
R"doc(Returns the tight encoding of the given unsigned long long integer.
See the page on tight "tight encodings" for details.

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::tightEncoding_7
static const char *tightEncoding_7 =
R"doc(Returns the tight encoding of the given boolean. See the page on tight
"tight encodings" for details.

The booleans ``True`` and ``False`` are guaranteed to have the same
tight encodings as the integers 1 and 0 respectively.

Parameter ``value``:
    the boolean to encode.

Returns:
    the resulting encoded string.)doc";

namespace TightEncodable_ {

// Docstring regina::python::doc::TightEncodable_::tightDecoding
static const char *tightDecoding =
R"doc(Reconstructs an object of type *T* from its given tight encoding. See
the page on tight "tight encodings" for details.

The tight encoding should be given as a string. If this string
contains leading whitespace or any trailing characters at all
(including trailing whitespace), then it will be treated as an invalid
encoding (i.e., this routine will throw an exception).

Exception ``InvalidArgument``:
    The given string is not a tight encoding of an object of type *T*.

Parameter ``enc``:
    the tight encoding for an object of type *T*.

Returns:
    the object represented by the given tight encoding.)doc";

// Docstring regina::python::doc::TightEncodable_::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of this object. See the page on tight
"tight encodings" for details.

Exception ``FailedPrecondition``:
    This may be thrown for some classes *T* if the object is in an
    invalid state. If this is possible, then a more detailed
    explanation of "invalid" can be found in the class documentation
    for *T*, under the member function T::tightEncode(). See
    FacetPairing::tightEncode() for an example of this.

Returns:
    the resulting encoded string.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

