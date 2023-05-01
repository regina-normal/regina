/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::basicTokenise
static const char *basicTokenise =
R"doc(Decomposes the given string into tokens.

This is an extremely simple tokeniser; tokens are defined to be
separated by arbitrary blocks of whitespace. Any leading or trailing
whitespace will be ignored.

.. warning::
    This routine treats all strings as plain ASCII. In particular,
    characters are examined one at a time, and the C routine isspace()
    is used to identify whitespace. Use it on strings with
    international characters at your own peril.

Parameter ``str``:
    the string to decompose.

Returns:
    the resulting list of tokens.)doc";

// Docstring regina::python::doc::startsWith
static const char *startsWith =
R"doc(Determines whether the given C++ string begins with the given prefix.

Parameter ``str``:
    the full C++ string to examine.

Parameter ``prefix``:
    the prefix whose presence we are testing for.

Returns:
    ``True`` if and only if *str* begins with *prefix*.)doc";

// Docstring regina::python::doc::stringToToken
static const char *stringToToken =
R"doc(Returns a token derived from the given string. All whitespace
characters in the given string will be replaced with an underscore.

Parameter ``str``:
    the string on which to base the token.

Returns:
    the corresponding token.)doc";

// Docstring regina::python::doc::stripWhitespace
static const char *stripWhitespace =
R"doc(Strips all whitespace from the beginning and end of the given C++
string. The new stripped string is returned; the original string is
not altered.

.. warning::
    This routine treats all strings as plain ASCII. In particular,
    characters are examined one at a time, and the C routine isspace()
    is used to identify whitespace. Use it on strings with
    international characters at your own peril.

Parameter ``str``:
    the string to be stripped.

Returns:
    the resulting stripped string.)doc";

// Docstring regina::python::doc::subscript
static const char *subscript =
R"doc(Converts the given C++ integer into a unicode subscript string.

The resulting string will be encoded using UTF-8.

Precondition:
    The template argument *T* is either (i) a native C++ integer type,
    for which the standard C++11 library routine std::to_string(T) is
    defined; or (ii) a const reference to Integer or LargeInteger.

Python:
    This template function is instantiated in Python for types *T* =
    ``long``, as well as const references to Integer and LargeInteger.)doc";

// Docstring regina::python::doc::superscript
static const char *superscript =
R"doc(Converts the given C++ integer into a unicode superscript string.

The resulting string will be encoded using UTF-8.

Precondition:
    The template argument *T* is either (i) a native C++ integer type,
    for which the standard C++11 library routine std::to_string(T) is
    defined; or (ii) a const reference to Integer or LargeInteger.

Python:
    This template function is instantiated in Python for types *T* =
    ``long``, as well as const references to Integer and LargeInteger.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

