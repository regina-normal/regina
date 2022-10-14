/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Text
static const char *Text =
R"doc(A packet representing a text string.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps; however,
these operations only copy/swap the text content, not the packet
infrastructure (e.g., they do not touch packet labels, or the packet
tree, or event listeners).)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the given text packets.

This global routine simply calls Text::swap(); it is provided so that
Text meets the C++ Swappable requirements.

Parameter ``a``:
    the first text packet whose contents should be swapped.

Parameter ``b``:
    the second text packet whose contents should be swapped.)doc";

namespace Text_ {

// Docstring regina::python::doc::Text_::__eq
static const char *__eq =
R"doc(Determines if this and the given packet contain the same text.

Parameter ``other``:
    the text packet to compare with this.

Returns:
    ``True`` if and only if this and the given packet contain the same
    text.)doc";

// Docstring regina::python::doc::Text_::__init
static const char *__init = R"doc(Initialises the packet to the empty string.)doc";

// Docstring regina::python::doc::Text_::__init_2
static const char *__init_2 =
R"doc(Initialises the packet to the given string.

Parameter ``text``:
    the new value for the packet.)doc";

// Docstring regina::python::doc::Text_::__init_3
static const char *__init_3 =
R"doc(Creates a new copy of the given text packet.

Like all packet types, this only copies the text content, not the
packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).)doc";

// Docstring regina::python::doc::Text_::__ne
static const char *__ne =
R"doc(Determines if this and the given packet do not contain the same text.

Parameter ``other``:
    the text packet to compare with this.

Returns:
    ``True`` if and only if this and the given packet do not contain
    the same text.)doc";

// Docstring regina::python::doc::Text_::setText
static const char *setText =
R"doc(Sets the packet data to the given string.

Parameter ``text``:
    the new value for the packet.)doc";

// Docstring regina::python::doc::Text_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given text packet.

Like all packet types, this only swaps the text content, not the
packet infrastructure (e.g., it will not swap packet labels, or change
either packet's location in any packet tree).

Parameter ``other``:
    the text packet whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Text_::text
static const char *text =
R"doc(Returns the string stored in the packet.

Returns:
    the stored string.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

