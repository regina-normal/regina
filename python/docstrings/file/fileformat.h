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


static const char *__doc_regina_FileFormat =
R"doc(Represents each generation of Regina's file formats.

These constants are intended to capture "generational changes" to the
file format, where backward compatibility is broken (i.e., where older
versions of Regina that do not know about the new generation format
cannot even attempt to read its data files at all).

In constrast, they do not capture incremental changes (e.g., new
elements in data files that correspond to new features of Regina,
which are simply ignored when an older version of Regina sees them).

Put differently: within each generation, the file formats should be
considered "interoperable", in that they maintain both forward and
backward compatibility as far as possible. In contrast, between
generations the file formats are not compatible at all, and require
some form of explicit conversion.

Of course, Regina can still read and write some older generation
formats; these abilities are indicated alongside the constants below.)doc";

static const char *__doc_regina_FileFormat_REGINA_BINARY_GEN_1 =
R"doc(Indicates the old first-generation binary format used by Regina 2.4
and earlier. This format was discontinued in mid-2002, when Regina
adopted an XML-based format instead.

Regina no longer supports this (ancient) binary format at all: since
version 4.94 in late 2013, Regina cannot read or write this format.)doc";

static const char *__doc_regina_FileFormat_REGINA_CURRENT_FILE_FORMAT =
R"doc(An alias for whichever file format is current. The numerical value of
this constant may change in future releases of Regina, if/when new
generational changes to the file format occur.)doc";

static const char *__doc_regina_FileFormat_REGINA_XML_GEN_2 =
R"doc(Indicates the second-generation file format used from Regina 3.0
through to Regina 6.0.1 inclusive. This was the first file format
based on XML. The root XML element is ``reginadata``.

Regina can still transparently read second-generation data files, and
can write them when explicitly asked.)doc";

static const char *__doc_regina_FileFormat_REGINA_XML_GEN_3 =
R"doc(Indicates the third-generation file format used from Regina 7.0
onwards. This format is both more streamlined and more human-readable.
The root XML element is ``regina``.

This is the current file format, and is used by default when saving
data files.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

