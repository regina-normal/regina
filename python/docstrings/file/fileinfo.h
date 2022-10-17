/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FileInfo
static const char *FileInfo =
R"doc(Stores information about a Regina data file, including file format and
version.

Routine identify() can be used to determine this information for a
given file.

As of Regina 4.94, the ancient first-generation binary files
(REGINA_BINARY_GEN_1) are no longer supported, and this class cannot
recognise them at all. These have not been in use since mid-2002. The
only file formats that this class now recognises are Regina's newer
XML-based (compressed or uncompressed) data files.

This class implements C++ move semantics and adheres to the C++
Swappable requirement, though it does not implement (or need) its own
custom swap() function. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace FileInfo_ {

// Docstring regina::python::doc::FileInfo_::__copy
static const char *__copy = R"doc(Creates a new copy of the given file information.)doc";

// Docstring regina::python::doc::FileInfo_::__eq
static const char *__eq =
R"doc(Determines whether this and the given file information describe the
same format and version.

For two FileInfo objects to compare as equal, they must have the same
file formats, use the same version of the calculation engine, and use
the same compression type. The pathnames of the files being described
are ignored.

It is safe to compare FileInfo objects even if one or both is invalid.
Two invalid FileInfo objects will compare as equal.

Parameter ``other``:
    the file information to compare with this.

Returns:
    ``True`` if and only if this and the given file information
    describe the same format and version, as described above.)doc";

// Docstring regina::python::doc::FileInfo_::__ne
static const char *__ne =
R"doc(Determines whether this and the given file information do not describe
the same format and version.

For two FileInfo objects to compare as equal, they must have the same
file formats, use the same version of the calculation engine, and use
the same compression type. The pathnames of the files being described
are ignored.

It is safe to compare FileInfo objects even if one or both is invalid.
Two invalid FileInfo objects will compare as equal.

Parameter ``other``:
    the file information to compare with this.

Returns:
    ``True`` if and only if this and the given file information do not
    describe the same format and version, as described above.)doc";

// Docstring regina::python::doc::FileInfo_::engine
static const char *engine =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

// Docstring regina::python::doc::FileInfo_::format
static const char *format =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

// Docstring regina::python::doc::FileInfo_::formatDescription
static const char *formatDescription =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

// Docstring regina::python::doc::FileInfo_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

// Docstring regina::python::doc::FileInfo_::identify
static const char *identify =
R"doc(Return information about the given Regina data file.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given path *name*, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. If a FileInfo
    structure is returned, its pathname() routine will use the same
    encoding that is passed here.

Parameter ``idPathname``:
    the pathname of the data file to be examined.

Returns:
    a FileInfo structure containing information about the given file,
    or no value if the file type could not be identified.)doc";

// Docstring regina::python::doc::FileInfo_::isCompressed
static const char *isCompressed =
R"doc(Returns whether this file is stored in compressed format. Currently
this option only applies to XML data files.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

// Docstring regina::python::doc::FileInfo_::isInvalid
static const char *isInvalid =
R"doc(Returns whether the file metadata could not be read.

Returns:
    ``True`` if the metadata could not be read, ``False`` otherwise.)doc";

// Docstring regina::python::doc::FileInfo_::pathname
static const char *pathname =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The character encoding used in the pathname will be whatever was
    originally passed to identify(). This might or might not be UTF-8,
    since it needs to be understood by the low-level C/C++ file I/O
    routines.

Returns:
    the pathname.)doc";

// Docstring regina::python::doc::FileInfo_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

