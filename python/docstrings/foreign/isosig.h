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
R"doc(Reads a list of isomorphism signatures or knot signatures from the
given text file. The file should contain one signature per line.
Signatures for knots or triangulations of any dimension are all
accepted, though the type of object must be known in advance and fixed
for the entire function call using the template parameter
*ObjectType*. These signatures will be converted into knots and/or
triangulations using Link::fromKnotSig() and
Triangulation<dim>::fromIsoSig() respectively.

A new container will be returned; the imported knots or triangulations
will be inserted as children of this container. The container will not
be assigned a label. The individual knots or triangulations will be
assigned labels according to the parameter *colLabels*.

If any signatures are invalid, these will be recorded in an additional
text packet that will be the last child of the returned container.

If an I/O error occurred while trying to read the given file, ``null``
will be returned.

In its simplest form, the text file can simply contain one signature
per line and nothing else. However, more complex formats are allowed.
In particular, by passing appropriate values for the arguments
*colSigs* and *colLabels*, the signatures and packet labels can be
taken from arbitrary columns of the text file. Columns are considered
to be separated by whitespace and are numbered beginning at 0.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file *name*, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. It assumes however
    that the *contents* of the file are in UTF-8.

Template parameter ``ObjectType``:
    Indicates which types of signatures the file contains. This must
    be either Link (indicating that the file contains knot
    signatures), or one of the Triangulation<dim> classes (indicating
    that the file contains isomorphism signatures for
    *dim*-dimensional triangulations).

Python:
    Since Python does not support templates, the Python version of
    this function takes an extra first parameter *dimension*. This
    should be the dimension of the underlying triangulation type, or 0
    to indicate that we are working with knot signatures. Moreover,
    the Python version currently only supports Regina's standard
    dimensions (i.e., you cannot use the Python version of this
    function with triangulations of dimension 5 or higher).

Parameter ``filename``:
    the name of the text file from which to read.

Parameter ``colSigs``:
    the column of the text file containing the signatures.

Parameter ``colLabels``:
    the column of the text file containing the packet labels for the
    resulting knots or triangulations. If this is negative then the
    signatures themselves will be used as packet labels.

Parameter ``ignoreLines``:
    the number of lines at the beginning of the text file that should
    be ignored completely.

Returns:
    a new container as described above, or ``null`` if an I/O error
    occurred whilst reading the given file.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

