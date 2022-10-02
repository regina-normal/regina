/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_regina_FileInfo =
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

static const char *__doc_regina_FileInfo_FileInfo = R"doc(Creates a new copy of the given file information.)doc";

static const char *__doc_regina_FileInfo_FileInfo_2 =
R"doc(Moves the contents of the given file information to this new object.

The object that was passed will no longer be usable.)doc";

static const char *__doc_regina_FileInfo_FileInfo_3 = R"doc(Create a new uninitialised structure.)doc";

static const char *__doc_regina_FileInfo_compressed = R"doc()doc";

static const char *__doc_regina_FileInfo_engine = R"doc()doc";

static const char *__doc_regina_FileInfo_engine_2 =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

static const char *__doc_regina_FileInfo_format = R"doc()doc";

static const char *__doc_regina_FileInfo_format_2 =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

static const char *__doc_regina_FileInfo_formatDescription =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

static const char *__doc_regina_FileInfo_identify =
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

static const char *__doc_regina_FileInfo_invalid = R"doc()doc";

static const char *__doc_regina_FileInfo_isCompressed =
R"doc(Returns whether this file is stored in compressed format. Currently
this option only applies to XML data files.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

static const char *__doc_regina_FileInfo_isInvalid =
R"doc(Returns whether the file metadata could not be read.

Returns:
    ``True`` if the metadata could not be read, ``False`` otherwise.)doc";

static const char *__doc_regina_FileInfo_operator_assign = R"doc(Sets this to be a copy of the given file information.)doc";

static const char *__doc_regina_FileInfo_operator_assign_2 =
R"doc(Moves the contents of the given file information to this new object.

The object that was passed will no longer be usable.

Returns:
    a reference to this object.)doc";

static const char *__doc_regina_FileInfo_operator_eq =
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

static const char *__doc_regina_FileInfo_operator_ne =
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

static const char *__doc_regina_FileInfo_pathname = R"doc()doc";

static const char *__doc_regina_FileInfo_pathname_2 =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The i18n "character encoding" used in the pathname will be
    whatever was originally passed to identify(). This might or might
    not be UTF-8, since it needs to be understood by the low-level
    C/C++ file I/O routines.

Returns:
    the pathname.)doc";

static const char *__doc_regina_FileInfo_swap =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

static const char *__doc_regina_FileInfo_writeTextLong =
R"doc(Writes a detailed text representation of this object to the given
output stream.

Python:
    Not present; use detail() instead.

Parameter ``out``:
    the output stream to which to write.)doc";

static const char *__doc_regina_FileInfo_writeTextShort =
R"doc(Writes a short text representation of this object to the given output
stream.

Python:
    Not present; use str() instead.

Parameter ``out``:
    the output stream to which to write.)doc";

static const char *__doc_regina_engine =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

static const char *__doc_regina_format =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

static const char *__doc_regina_formatDescription =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

static const char *__doc_regina_isCompressed =
R"doc(Returns whether this file is stored in compressed format. Currently
this option only applies to XML data files.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

static const char *__doc_regina_isInvalid =
R"doc(Returns whether the file metadata could not be read.

Returns:
    ``True`` if the metadata could not be read, ``False`` otherwise.)doc";

static const char *__doc_regina_operator_eq =
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

static const char *__doc_regina_operator_ne =
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

static const char *__doc_regina_pathname =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The i18n "character encoding" used in the pathname will be
    whatever was originally passed to identify(). This might or might
    not be UTF-8, since it needs to be understood by the low-level
    C/C++ file I/O routines.

Returns:
    the pathname.)doc";

static const char *__doc_regina_swap =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

static const char *__doc_regina_swap_2 =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

static const char *__doc_regina_swap_3 =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

