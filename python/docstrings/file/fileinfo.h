/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


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

static const char *FileInfo_FileInfo = R"doc(Creates a new copy of the given file information.)doc";

static const char *FileInfo_FileInfo_2 =
R"doc(Moves the contents of the given file information to this new object.

The object that was passed will no longer be usable.)doc";

static const char *FileInfo_engine =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

static const char *FileInfo_format =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

static const char *FileInfo_formatDescription =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

static const char *FileInfo_identify =
R"doc(Return information about the given Regina data file.

Internationalisation:
    This routine makes no assumptions about the i18n "character
    encoding" used in the given path *name*, and simply passes it
    through unchanged to low-level C/C++ file I/O routines. If a
    FileInfo structure is returned, its pathname() routine will use
    the same encoding that is passed here.

Parameter ``idPathname``:
    the pathname of the data file to be examined.

Returns:
    a FileInfo structure containing information about the given file,
    or no value if the file type could not be identified.)doc";

static const char *FileInfo_isCompressed =
R"doc(Returns whether this file is stored in compressed format. Currently
this option only applies to XML data files.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

static const char *FileInfo_isInvalid =
R"doc(Returns whether the file metadata could not be read.

Returns:
    ``True`` if the metadata could not be read, ``False`` otherwise.)doc";

static const char *FileInfo_operator_assign = R"doc(Sets this to be a copy of the given file information.)doc";

static const char *FileInfo_operator_assign_2 =
R"doc(Moves the contents of the given file information to this new object.

The object that was passed will no longer be usable.

Returns:
    a reference to this object.)doc";

static const char *FileInfo_operator_eq =
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

static const char *FileInfo_operator_ne =
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

static const char *FileInfo_pathname =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The i18n "character encoding" used in the pathname will be
    whatever was originally passed to identify(). This might or might
    not be UTF-8, since it needs to be understood by the low-level
    C/C++ file I/O routines.

Returns:
    the pathname.)doc";

static const char *FileInfo_swap =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

static const char *FileInfo_writeTextLong =
R"doc(Writes a detailed text representation of this object to the given
output stream.

Python:
    Not present; use detail() instead.

Parameter ``out``:
    the output stream to which to write.)doc";

static const char *FileInfo_writeTextShort =
R"doc(Writes a short text representation of this object to the given output
stream.

Python:
    Not present; use str() instead.

Parameter ``out``:
    the output stream to which to write.)doc";

static const char *engine =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

static const char *format =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

static const char *formatDescription =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

static const char *isCompressed =
R"doc(Returns whether this file is stored in compressed format. Currently
this option only applies to XML data files.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

static const char *isInvalid =
R"doc(Returns whether the file metadata could not be read.

Returns:
    ``True`` if the metadata could not be read, ``False`` otherwise.)doc";

static const char *operator_eq =
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

static const char *operator_ne =
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

static const char *pathname =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The i18n "character encoding" used in the pathname will be
    whatever was originally passed to identify(). This might or might
    not be UTF-8, since it needs to be understood by the low-level
    C/C++ file I/O routines.

Returns:
    the pathname.)doc";

static const char *swap =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

static const char *swap_2 =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

static const char *swap_3 =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

