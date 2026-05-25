/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::readOrb
inline constexpr const char readOrb[] =
R"doc(Reads a triangulation from the given Orb / Casson file. A new
triangulation packet will be returned.

The packet label of the new triangulation will be the manifold name
read from the second line of the Orb / Casson file. The first line of
the Orb / Casson file must simply be: ``% orb``

If the file cannot be read or if it is not in the correct format, this
routine will throw an exception. This is a change in behaviour as of
Regina 8.0: older versions of Regina (≤ 7.x) returned ``None``
instead.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. It assumes however
    that the _contents_ of the file are in UTF-8.

Parameter ``filename``:
    the name of the Orb / Casson file from which to read.

Returns:
    a new triangulation packet containing the data read from the Orb /
    Casson file, or ``None`` on error.

Author:
    Ryan Budney, also with code from Damien Heard

Exception ``FileError``:
    An error occurred whilst reading the given file.

Exception ``InvalidInput``:
    The given file does not appear to be a valid Orb / Casson file.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

