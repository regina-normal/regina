/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Attachment
static const char *Attachment =
R"doc(A packet that can hold an arbitrary file attachment.

The attachment is stored as a block of data in memory. If the
attachment was originally read from the filesystem, this link is not
maintained (i.e., if the source file subsequently changes, the
attachment data stored in this packet will *not* change to reflect
this).

This packet may or may not contain an attachment at any given time.
This can be tested by calling isNull(), and can be changed by calling
reset().

This class does not distinguish between an empty file attachment
versus no attachment at all. In both cases, isNull() will return
``True``.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the mathematical content, not the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners).

Until Regina 6.0.1, this class was called PDF. It was renamed and
expanded in Regina 7.0 to allow for any kind of file to be attached.)doc";

namespace Attachment_ {

// Docstring regina::python::doc::Attachment_::__copy
static const char *__copy =
R"doc(Creates a new deep copy of the given attachment.

Like all packet types, this only copies the content of the attachment,
not the packet infrastructure (e.g., it will not copy the packet
label, it will not clone the given packet's children, and it will not
insert the new packet into any packet tree).

This is safe to call even if *src* does not contain a non-empty
attachment (i.e., if ``src.isNull()`` returns ``True``).

Parameter ``src``:
    the attachment packet whose contents should be copied.)doc";

// Docstring regina::python::doc::Attachment_::__default
static const char *__default =
R"doc(Creates a packet with no attachment stored.

Until some content is assigned (e.g., by the assignment operator, or
by a non-trivial call to reset()), isNull() will return ``True``.)doc";

// Docstring regina::python::doc::Attachment_::__eq
static const char *__eq =
R"doc(Determines if this and the given attachment hold identical data.

The filenames will not be compared.

It is safe to call this operator if one or both attachments is empty
(i.e., isNull() returns ``True``), in which case an empty attachment
will compare as equal to any other empty attachment.

Parameter ``other``:
    the attachment to compare with this.

Returns:
    ``True`` if and only if this and the given attachment contain
    identical data.)doc";

// Docstring regina::python::doc::Attachment_::__init
static const char *__init =
R"doc(Creates a new attachment containing the contents of the given file.

The file will be read immediately, but no ongoing link will be
maintained. That is, if the given file should later change its
contents on the filesystem, the change will *not* be reflected in this
attachment packet.

If the file could not be read or is empty, then no attachment will be
stored. You can test this by calling isNull().

The filename that is stored with this attachment (i.e., the string
that will be returned by filename()) will be the argument *pathname*
with any directory prefixes removed (i.e., just the final filename
component). If an error occurs when attempting to remove directory
prefixes, the filename will be the empty string.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file *name*, and simply passes it through
    unchanged to low-level C/C++ file I/O routines.

Parameter ``pathname``:
    the full pathname of the attachment to read.)doc";

// Docstring regina::python::doc::Attachment_::__init_2
static const char *__init_2 =
R"doc(Creates a new attachment containing the given binary data.

The *data* array must contain a block of binary data, of *size* bytes.
There is no notion of null termination: the data block may contain
``null`` bytes within it, and does not need to be terminated by a
``null`` byte at the end.

The *alloc* argument shows if/how this packet claims ownership of the
data. In particular, unless *alloc* is *DEEP_COPY*, this packet will
claim ownership of the given data block and will deallocate it when
the packet is destroyed. If *alloc* is *DEEP_COPY* then the given
block of data will not be modified in any way.

It is possible to pass a null pointer as the data array, in which case
the new packet will have no attachment stored (so isNull() will return
``True``).

Python:
    The argument *data* should be a Python ``bytes`` object. The
    *size* and *alloc* arguments are not present, since a ``bytes``
    object knows its size, and in Python the only available ownership
    policy is to make a deep copy.

Parameter ``data``:
    the block of binary data that forms the attachment, or ``null`` if
    no attachment is to be stored.

Parameter ``size``:
    the number of bytes in this block of binary data; if actual data
    is passed (i.e., *data* is not ``null``) then this must be
    strictly positive.

Parameter ``alloc``:
    describes if/how this packet should claim ownership of the given
    block of data; see the notes above for details.

Parameter ``filename``:
    the filename to associated with this attachment; typically this
    would be a filename only, with no directory prefixes. See
    filename() for details on how this string will be used.)doc";

// Docstring regina::python::doc::Attachment_::__ne
static const char *__ne =
R"doc(Determines if this and the given attachment hold different data.

The filenames will not be compared.

It is safe to call this operator if one or both attachments is empty
(i.e., isNull() returns ``True``), in which case an empty attachment
will compare as equal to any other empty attachment.

Parameter ``other``:
    the attachment to compare with this.

Returns:
    ``True`` if and only if this and the given attachment contain
    different data.)doc";

// Docstring regina::python::doc::Attachment_::data
static const char *data =
R"doc(Returns a pointer to the block of raw data that forms this attachment.
The number of bytes in this block can be found by calling size().

If this packet does not currently hold a non-empty attachment (i.e.,
if isNull() returns ``True``), then this routine will return ``null``.

Python:
    This routine returns a Python ``bytes`` object. Unlike the C++
    version of this routine, the Python version returns a deep copy of
    the raw data (so this routine becomes slower, but the resulting
    byte sequence can be freely modified).

Returns:
    the raw attachment data.)doc";

// Docstring regina::python::doc::Attachment_::extension
static const char *extension =
R"doc(Returns the extension of the filename associated with this attachment.

The user interface may use the result of filename() and/or extension()
to determine which external viewer to launch when the user asks to
view the attachment.

The extension will include the leading period. So, for example, if
filename() returns "file.pdf", then extension() will return ".pdf".

If the associated filename has no extension, or if an error occurred
when attempting to deduce it, then this routine will return the empty
string.

Returns:
    the extension of the filename for this attachment.)doc";

// Docstring regina::python::doc::Attachment_::filename
static const char *filename =
R"doc(Returns the filename associated with this attachment. Typically this
would be a filename only, with no path information.

The user interface may use the result of filename() and/or extension()
to determine which external viewer to launch when the user asks to
view the attachment.

There are no guarantees on the contents of this string, and user
interfaces should treat it as a hint only (i.e., they may modify or
even completely override it when saving the attachment as a local
file). It is possible in some scenarios that this could be the empty
string, or could be some other string that is not a valid filename on
the current platform.

Returns:
    the filename associated with this attachment.)doc";

// Docstring regina::python::doc::Attachment_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given attachment packets.

This global routine simply calls Attachment::swap(); it is provided so
that Attachment meets the C++ Swappable requirements.

Parameter ``a``:
    the first attachment packet whose contents should be swapped.

Parameter ``b``:
    the second attachment packet whose contents should be swapped.)doc";

// Docstring regina::python::doc::Attachment_::isNull
static const char *isNull =
R"doc(Determines whether this packet is currently holding a non-empty
attachment.

Returns:
    ``True`` if and only if this packet is holding a non-empty
    attachment.)doc";

// Docstring regina::python::doc::Attachment_::reset
static const char *reset =
R"doc(Empties this packet so that no attachment is stored. After calling
this routine, isNull() will return ``True``.

The old data will be deallocated if required.)doc";

// Docstring regina::python::doc::Attachment_::reset_2
static const char *reset_2 =
R"doc(Refills this attachment packet with the given binary data. The old
data will be deallocated if required.

This routine behaves like the class constructor; see the constructor
documentation for details.

.. warning::
    This cannot be used for self-assignment. If the argument *data* is
    in fact data() then this routine will most likely crash.

Python:
    The argument *data* should be a Python ``bytes`` object. The
    *size* and *alloc* arguments are not present, since a ``bytes``
    object knows its size, and in Python the only available ownership
    policy is to make a deep copy.

Parameter ``data``:
    the block of binary data that forms the new attachment, or
    ``null`` if no attachment is to be stored.

Parameter ``size``:
    the number of bytes in this new block of binary data; if actual
    data is passed (i.e., *data* is not ``null``) then this must be
    strictly positive.

Parameter ``alloc``:
    describes if/how this packet should claim ownership of the given
    block of data; see the notes above for details.

Parameter ``filename``:
    the new filename to associated with this attachment; this will
    override the previously stored filename. Typically this would be a
    filename only, with no directory prefixes. See filename() for
    details on how this string will be used.)doc";

// Docstring regina::python::doc::Attachment_::save
static const char *save =
R"doc(Saves the contents of this attachment to the given file.

If this packet does not currently hold a non-empty attachment (i.e.,
if isNull() returns ``True``), then this routine will do nothing and
simply return ``False``.

Internationalisation:
    This routine makes no assumptions about the character encoding
    used in the given file *name*, and simply passes it unchanged to
    low-level C/C++ file I/O routines.

Parameter ``pathname``:
    the full pathname of the file to write.

Returns:
    ``True`` if the file was successfully written, or ``False``
    otherwise.)doc";

// Docstring regina::python::doc::Attachment_::size
static const char *size =
R"doc(Returns the size of this attachment in bytes.

If this packet does not currently hold a non-empty attachment (i.e.,
if isNull() returns ``True``), then this routine will return zero.

Returns:
    the number of bytes.)doc";

// Docstring regina::python::doc::Attachment_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given attachment.

Like all packet types, this only swaps the content of the attachments,
not the packet infrastructure (e.g., it will not swap packet labels,
or change either packet's location in any packet tree).

This is safe to call even if this packet and/or *other* does not
contain a non-empty attachment (i.e., if isNull() returns ``True``).

Parameter ``other``:
    the attachment packet whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

