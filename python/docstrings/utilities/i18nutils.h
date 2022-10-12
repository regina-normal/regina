/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace i18n {

// Docstring regina::python::doc::i18n::Locale
static const char *Locale =
R"doc(A simple class with static routines for querying information about the
current locale.

These routines use ::setlocale() to determine the current locale,
which means they respect environment variables such as LANG and
LC_ALL.)doc";

// Docstring regina::python::doc::i18n::utf8ValidTo
static const char *utf8ValidTo =
R"doc(Identifies the longest prefix of the given string that is valid UTF-8.

The substring from ``s.begin()`` to the iterator that is returned is
guaranteed to be valid UTF-8. If the entire string is valid UTF-8,
then this routine will return ``s.end()``.

Python:
    This routine returns the *length* of the longest valid UTF-8
    prefix. The length is measured in raw bytes (not unicode
    characters).

Returns:
    an iterator marking the end of the longest valid UTF-8 prefix.)doc";

// Docstring regina::python::doc::i18n::utf8ValidTo_2
static const char *utf8ValidTo_2 =
R"doc(Identifies the longest prefix of the given string that is valid UTF-8.

The substring from *s* until just before the pointer that is returned
is guaranteed to be valid UTF-8. If the entire string is valid UTF-8,
then this routine will return a pointer to the null terminator of *s*.

Python:
    This routine returns the *length* of the longest valid UTF-8
    prefix. The length is measured in raw bytes (not unicode
    characters).

Returns:
    a pointer marking the end of the longest valid UTF-8 prefix.)doc";

}

namespace i18n::Locale_ {

// Docstring regina::python::doc::i18n::Locale_::codeset
static const char *codeset =
R"doc(Returns the character encoding used in the current locale. This is a
plain string, such as "UTF-8" or "ISO-8859-1".

Returns:
    the character encoding for the current locale.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

