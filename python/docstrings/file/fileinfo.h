/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct FileInfo {

// Docstring regina::python::doc::FileInfo::__class
static constexpr const char __class[] =
R"doc(Stores information about a Regina data file, including file format and
version.

The static routine identify() can be used to determine this
information for a given file.

As of Regina 4.94, the ancient first-generation binary files
(FileFormat::BinaryGen1) are no longer supported, and this class
cannot recognise them at all. These have not been in use since
mid-2002. The only file formats that this class now recognises are
Regina's newer XML-based (compressed or uncompressed) data files.

This class implements C++ move semantics and adheres to the C++
Swappable requirement, though it does not implement (or need) its own
custom swap() function. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::FileInfo::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given file information.)doc";

// Docstring regina::python::doc::FileInfo::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given file information describe the
same format and version.

For two FileInfo objects to compare as equal, they must have the same
file formats, use the same version of the calculation engine, and use
the same compression type. The pathnames of the files being described
are ignored.

Parameter ``other``:
    the file information to compare with this.

Returns:
    ``True`` if and only if this and the given file information
    describe the same format and version, as described above.)doc";

// Docstring regina::python::doc::FileInfo::engine
static constexpr const char engine[] =
R"doc(Returns the version of the calculation engine that wrote this file.

Returns:
    the engine version for this file.)doc";

// Docstring regina::python::doc::FileInfo::format
static constexpr const char format[] =
R"doc(Returns which of Regina's file formats the data file uses.

In particular, this encodes which generation of XML the file uses, but
does not encode whether the XML is compressed.

Returns:
    the file format.)doc";

// Docstring regina::python::doc::FileInfo::formatDescription
static constexpr const char formatDescription[] =
R"doc(Returns a human-readable description of the file format used by the
data file.

Like format(), this indicates which generation of XML the file uses,
but not whether the XML is compressed.

Returns:
    a description of the file format.)doc";

// Docstring regina::python::doc::FileInfo::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given file information objects.

This global routine simply calls FileInfo::swap(); it is provided so
that FileInfo meets the C++ Swappable requirements.

Parameter ``a``:
    the object whose contents should be swapped with *b*.

Parameter ``b``:
    the object whose contents should be swapped with *a*.)doc";

// Docstring regina::python::doc::FileInfo::identify
static constexpr const char identify[] =
R"doc(Returns information about the given Regina data file.

If the given file cannot be read, or if it cannot be identified as a
Regina data file, then this routine will throw an exception. This is a
change of behaviour as of Regina 8.0: older versions of Regina (≤ 7.x)
would return either ``None`` (``std::nullopt`` in C++, or ``None`` in
Python) or an "invalid" FileInfo object (depending upon the exact
error condition). Nowadays this function returns a FileInfo (not a
``std::optional<FileInfo>``), and there is no concept of an "invalid"
FileInfo object at all.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given path _name_, and simply passes it through
    unchanged to low-level C/C++ file I/O routines. If a FileInfo
    structure is returned, its pathname() routine will use the same
    encoding that is passed here.

Exception ``FileError``:
    The file could not be read.

Exception ``InvalidInput``:
    The file could be read, but it does not appear to be a Regina data
    file (in particular, Regina's file metadata could not be located
    and parsed).

Parameter ``idPathname``:
    the pathname of the data file to be examined.

Returns:
    a FileInfo structure containing information about the given Regina
    data file.)doc";

// Docstring regina::python::doc::FileInfo::isCompressed
static constexpr const char isCompressed[] =
R"doc(Returns whether this file is stored in compressed XML format.

Returns:
    ``True`` if this file is compressed or ``False`` otherwise.)doc";

// Docstring regina::python::doc::FileInfo::isInvalid
static constexpr const char isInvalid[] =
R"doc(Deprecated routine that now always returns ``False``.

.. deprecated::
    As of Regina 8.0 this function is irrelevant, since identify() now
    only returns a concrete FileInfo object if it can successfully
    locate and parse Regina's file metadata. That is, as of Regina
    8.0, every FileInfo object is now valid.

Returns:
    ``False``.)doc";

// Docstring regina::python::doc::FileInfo::pathname
static constexpr const char pathname[] =
R"doc(Returns the pathname of the data file being described.

Internationalisation:
    The character encoding used in the pathname will be whatever was
    originally passed to identify(). This might or might not be UTF-8,
    since it needs to be understood by the low-level C/C++ file I/O
    routines.

Returns:
    the pathname.)doc";

// Docstring regina::python::doc::FileInfo::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given file information.

Parameter ``other``:
    the object whose contents should be swapped with this.)doc";

}; // struct FileInfo

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

