/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace xml {

// Docstring regina::python::doc::xml::xmlEncodeComment
static const char *xmlEncodeComment =
R"doc(Returns the given string encoded so it is suitable for use inside an
XML comment. As well as converting special characters to XML entities,
this routine will replace dashes with underscores to avoid double-
hyphens (which are illegal in XML comments).

Parameter ``comment``:
    the string to convert; this string will not be changed.

Returns:
    the string converted to be usable inside an XML comment.)doc";

// Docstring regina::python::doc::xml::xmlEncodeSpecialChars
static const char *xmlEncodeSpecialChars =
R"doc(Returns the given string with special characters converted to XML
entities. For instance, the string ``"a < b"`` would be converted to
``"a &lt; b"``.

Parameter ``original``:
    the string to convert; this string will not be changed.

Returns:
    the converted string with special characters replaced by XML
    entities.)doc";

// Docstring regina::python::doc::xml::xmlValueTag
static const char *xmlValueTag =
R"doc(Returns an XML tag with a single property containing the given value.
The tag will be of the form ``<tagName value="..."/>``.

The value itself will be written to the tag string using the standard
output stream operator <<.

Precondition:
    The property value when written to an output stream does not
    contain any special characters (such as ``<`` or ``&``) that need
    to be encoded as XML entities.

Python:
    Not present.

Parameter ``tagName``:
    the name of the XML tag to create.

Parameter ``value``:
    the value to assign to the <i>value</i> property of the tag.

Returns:
    the corresponding XML tag.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

