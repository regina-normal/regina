/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::readDehydrationList
static const char *readDehydrationList =
R"doc(Reads a list of dehydrated triangulations from the given text file.
The file should contain one dehydration string per line. These strings
will be rehydrated as described in Triangulation<3>::rehydrate().

A new container will be returned; the imported triangulations will be
inserted as children of this container. The container will not be
assigned a label. The individual triangulations will be assigned
labels according to the parameter *colLabels*.

If any dehydrations strings are invalid, these will be recorded in an
additional text packet that will be the last child of the returned
container.

If an I/O error occurred while trying to read the given file, 0 will
be returned.

In its simplest form, the text file can simply contain one dehydration
string per line and nothing else. However, more complex formats are
allowed. In particular, by passing appropriate values for the
arguments *colDehydrations* and *colLabels*, the dehydration strings
and triangulation packet labels can be taken from arbitrary columns of
the text file. Columns are considered to be separated by whitespace
and are numbered beginning at 0.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file *name*, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. It assumes however
    that the *contents* of the file are in UTF-8.

Parameter ``filename``:
    the name of the text file from which to read.

Parameter ``colDehydrations``:
    the column of the text file containing the dehydration strings.

Parameter ``colLabels``:
    the column of the text file containing the triangulation packet
    labels. If this is negative then the dehydration strings
    themselves will be used as packet labels.

Parameter ``ignoreLines``:
    the number of lines at the beginning of the text file that should
    be ignored completely.

Returns:
    a new container as described above, or ``None`` if an I/O error
    occurred whilst reading the given file.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

