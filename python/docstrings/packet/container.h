/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Container
static const char *Container =
R"doc(A packet that simply contains other packets. Such a packet contains no
real data.

Like all packet types, this class does not support C++ move semantics
since this would interfere with the structure of the packet tree. It
does support copy construction, copy assignment and swaps, but only
for consistency with the other packet types. Such copy/swap operations
are pointless for container packets since they do not copy/swap the
packet infrastructure (e.g., they do not touch packet labels, or the
packet tree, or event listeners), and containers have no "real"
content of their own.)doc";

namespace Container_ {

// Docstring regina::python::doc::Container_::__copy
static const char *__copy =
R"doc(Copy constructor that does nothing.

This is only here for consistency with the other packet types. Like
all packet types, this copy constructor does not copy any of the
packet infrastructure (e.g., it will not copy the packet label, it
will not clone the given packet's children, and it will not insert the
new packet into any packet tree).)doc";

// Docstring regina::python::doc::Container_::__init
static const char *__init = R"doc(Default constructor.)doc";

// Docstring regina::python::doc::Container_::__init_2
static const char *__init_2 =
R"doc(Constructs a new container with the given packet label.

This constructor is (for example) helpful when you are building a
complex packet tree to save to a Regina data file, and you are using
containers to organise the data in this tree.

Parameter ``label``:
    the packet label for this new container.)doc";

// Docstring regina::python::doc::Container_::global_swap
static const char *global_swap =
R"doc(Swap function for container packets that does nothing.

This is only here for consistency with the other packet types. For
container packets, the swap operation does nothing since containers
have no "real" content of their own. See the member function
Container::swap() for further explanation.)doc";

// Docstring regina::python::doc::Container_::swap
static const char *swap =
R"doc(Swap function that does nothing.

This is only here for consistency with the other packet types. Like
all packet types, this operation does not swap any of the packet
infrastructure (e.g., it will not swap packet labels, or change either
packet's location in any packet tree).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

