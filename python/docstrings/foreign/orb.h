/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::readOrb
static const char *readOrb =
R"doc(Reads a triangulation from the given Orb / Casson file. A new
triangulation packet will be returned.

The packet label of the new triangulation will be the manifold name
read from the second line of the Orb / Casson file. The first line of
the Orb / Casson file must simply be: ``% orb``

If the file could not be read or if the data was not in the correct
format, ``null`` will be returned.

Internationalisation:
    This routine makes no assumptions about the i18n "character
    encoding" used in the given file *name*, and simply passes it
    through unchanged to low-level C/C++ file I/O routines. It assumes
    however that the *contents* of the file are in UTF-8.

Parameter ``filename``:
    the name of the Orb / Casson file from which to read.

Returns:
    a new triangulation packet containing the data read from the Orb /
    Casson file, or ``null`` on error.

Author:
    Ryan Budney, also with code from Damien Heard)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

