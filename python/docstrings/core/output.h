/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Output
static const char *Output =
R"doc(A common base class for objects that write human-readable text output.
This class ensures that text output routines have consistent names and
behaviours across Regina's entire API.

Three types of output are supported:

* *short* output, which fits on a single line and uses plain ASCII
  characters wherever possible;

* *utf8* output, which is like short output but supports the much
  richer unicode character set; and

* *detailed* output, which may be arbitrarily long.

Any class that provides text output should ultimately inherit from
this base class. Your derived class must provide two functions:

* ``writeTextShort(std::ostream& out, bool utf8 = false)``, which
  writes either the short output or the utf8 output to the given
  output stream, according to whether *utf8* is ``False`` or ``True``
  respectively;

* ``writeTextLong(std::ostream& out)``, which writes the detailed
  output to the given output stream.

The boolean *utf8* argument to writeTextShort() must be optional.
Moreover, if your class does not benefit from unicode characters
(i.e., the short and utf8 outputs are identical), then you may omit
the *utf8* argument entirely; in this case, you must set the template
argument *supportsUtf8* as ``False``. Both *writeTextShort*() and
*writeTextLong*() may take additional arguments, as long as they are
optional.

The documentation for str(), utf8() and detail() gives guidelines as
to how the various types of output should be formatted.

In return, this class will provide the functions str(), utf8() and
detail(), which return the short, utf8 and detailed outputs
respectively in std::string format. It will also provide a global
operator << that allows you to write objects of type *T* to an
arbitrary output stream.

If your class is simple and has no need for detailed output then it
may derive from ShortOutput instead, which provides a default
implementation for *writeTextLong*().

Template parameter ``T``:
    the class that provides the implementations of *writeTextShort*()
    and *writeTextLong*(). Typically this will be your own class
    (i.e., your class *C* derives from Output<C>). However, this may
    be deeper in the class hierarchy.

Template parameter ``supportsUtf8``:
    ``True`` if the class *T* can make use of the richer unicode
    character set, or ``False`` if the short and utf8 outputs are
    identical. If this is ``False`` then T::writeTextShort() will only
    ever be called in the form ``writeTextShort(std::ostream&)``, and
    you may for simplicity omit the second boolean *utf8* argument.
    This Output base class will still provide a utf8() function, but
    it will return the same output as short().

.. note::
    Every object of this class that is ever instantiated *must* be
    derived from the class *T*. In other words, end users can
    construct objects of type *T* (which derives from Output<T>), but
    they cannot construct objects of the parent class Output<T>
    itself.

Python:
    Not present, but the output routines str(), utf8() and detail()
    will be provided directly through the various subclasses.)doc";

// Docstring regina::python::doc::OutputBase
static const char *OutputBase =
R"doc(Provides a type alias to help identify where in the class hierarchy
the output functions T::str() and T::detail() are implemented.

If *T* is a class derived (directly or indirectly) from some class
Output<...>, then ``OutputBase<T>::type`` is defined to be this parent
class Output<...>. If *T* is derived from multiple Output<...> classes
(like SnapPeaTriangulation is), then this ambiguity will be resolved
if possible by prioritising Output<T, ...>.

If *T* is not derived from any class Output<...>, then
``OutputBase<T>::type`` is defined to be *T* itself.

This helper class can be useful when trying to disambiguate between
the implementation of str() that is inherited from Output, versus an
extended implementation of str() (perhaps with more arguments) that is
implemented in the class *T* itself.

Precondition:
    *T* is a class or struct type.)doc";

// Docstring regina::python::doc::ShortOutput
static const char *ShortOutput =
R"doc(A common base class for objects that provide short text output only.

All classes that provide human-readable text output should ultimately
inherit from Output, which provides support for short output (both
with and without unicode support) as well as detailed output. However,
if your class is simple and has no need for detailed output, then you
may inherit from ShortOutput instead.

If you inherit from this class, then you only need to provide a single
function *writeTextShort*(), as described in the Output class notes.
You do not need to provide *writeTextLong*(); this class will
implement it for you.

As with Output, this class will provide three routines str(), utf8()
and detail(). The function detail() will return the same output as
str(), but with a final newline added.

Template parameter ``T``:
    the class that provides the implementation of *writeTextShort*().
    Typically this will be your own class (i.e., your class *C*
    derives from ShortOutput<C>). However, this may be deeper in the
    class hierarchy.

Template parameter ``supportsUtf8``:
    ``True`` if the class *T* can make use of the richer unicode
    character set, or ``False`` if the short and utf8 outputs are
    identical. If this is ``False`` then T::writeTextShort() will only
    ever be called in the form ``writeTextShort(std::ostream&)``, and
    you may for simplicity omit the second boolean *utf8* argument.
    This ShortOutput base class will still provide a utf8() function,
    but it will return the same output as short().

Python:
    Not present, but the output routines str(), utf8() and detail()
    will be provided directly through the various subclasses.)doc";

namespace Output_ {

// Docstring regina::python::doc::Output_::detail
static const char *detail =
R"doc(Returns a detailed text representation of this object. This text may
span many lines, and should provide the user with all the information
they could want. It should be human-readable, should not contain
extremely long lines (which cause problems for users reading the
output in a terminal), and should end with a final newline. There are
no restrictions on the underlying character set.

Returns:
    a detailed text representation of this object.)doc";

// Docstring regina::python::doc::Output_::str
static const char *str =
R"doc(Returns a short text representation of this object. This text should
be human-readable, should use plain ASCII characters where possible,
and should not contain any newlines.

Within these limits, this short text ouptut should be as information-
rich as possible, since in most cases this forms the basis for the
Python ``__str__()`` and ``__repr__()`` functions.

Python:
    The Python "stringification" function ``__str__()`` will use
    precisely this function, and for most classes the Python
    ``__repr__()`` function will incorporate this into its output.

Returns:
    a short text representation of this object.)doc";

// Docstring regina::python::doc::Output_::utf8
static const char *utf8 =
R"doc(Returns a short text representation of this object using unicode
characters. Like str(), this text should be human-readable, should not
contain any newlines, and (within these constraints) should be as
information-rich as is reasonable.

Unlike str(), this function may use unicode characters to make the
output more pleasant to read. The string that is returned will be
encoded in UTF-8.

Returns:
    a short text representation of this object.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

