/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::readSigList
static const char *readSigList =
R"doc(Reads a list of signatures (e.g., isomorphism signatures for
triangulations, or knot/link signatures) from the given text file, and
reconstructs the corresponding mathematical objects in a new packet
tree. The file should contain one signature per line.

All signatures should be for the same type of object, which is passed
as the template parameter *ObjectType*. For example, they could all be
knot/link signatures (if ObjectType is Link), or they could all be
isomorphism signatures for triangulations of some fixed dimension
*dim* (if ObjectType is ``Triangulation<dim>``).

A new container will be returned, and the reconstructed objects (e.g.,
triangulations or links) will be inserted as children of this
container. The container will not be assigned a label. The individual
reconstructed objects will be assigned labels according to the
parameter *colLabels*.

Regarding errors:

* Empty lines in the file are harmless: they will simply be ignored.

* If any signatures are invalid, these will be recorded in an
  additional text packet that will be the last child of the container.

* If an I/O error occurs while trying to read the given file, this
  routine will return ``None``.

In its simplest form, the text file can simply contain one signature
per line and nothing else. However, more complex formats are allowed.
In particular, by passing appropriate values for the arguments
*colSigs* and *colLabels*, the signatures and packet labels can be
taken from arbitrary columns of the text file. Columns are considered
to be separated by whitespace and are numbered beginning at 0.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. It assumes however
    that the _contents_ of the file are in UTF-8.

Template parameter ``ObjectType``:
    indicates what type of object is reconstructed from each signature
    in the file, as described above.

Python:
    Since Python does not support templates, the object type is
    specified by passing an integer *dimension* as the first parameter
    (before the filename). This should be *dim* to specify the object
    type ``Triangulation<dim>``, or 0 to specify the object type Link.
    Currenty no other object type is supported in Python.

Parameter ``filename``:
    the name of the text file from which to read.

Parameter ``colSigs``:
    the column of the text file containing the signatures.

Parameter ``colLabels``:
    the column of the text file that provides packet labels for the
    reconstructed objects. If this is negative then the signatures
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

