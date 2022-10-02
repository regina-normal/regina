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


static const char *__doc_regina_Output = R"doc()doc";

static const char *__doc_regina_OutputBase =
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

static const char *__doc_regina_OutputBase_test = R"doc()doc";

static const char *__doc_regina_OutputBase_test_2 = R"doc()doc";

static const char *__doc_regina_OutputBase_test_3 = R"doc()doc";

static const char *__doc_regina_ShortOutput =
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

static const char *__doc_regina_ShortOutput_writeTextLong =
R"doc(A default implementation for detailed output. This routine simply
calls *T::writeTextShort*() and appends a final newline.

Python:
    Not present; instead you can call detail() from the subclass *T*,
    which returns this output as a string.

Parameter ``out``:
    the output stream to which to write.)doc";

static const char *__doc_regina_operator_lshift = R"doc()doc";

static const char *__doc_regina_operator_lshift_2 = R"doc()doc";

static const char *__doc_regina_writeTextLong =
R"doc(A default implementation for detailed output. This routine simply
calls *T::writeTextShort*() and appends a final newline.

Python:
    Not present; instead you can call detail() from the subclass *T*,
    which returns this output as a string.

Parameter ``out``:
    the output stream to which to write.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

