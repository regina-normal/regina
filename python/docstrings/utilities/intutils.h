/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::bitsRequired
inline constexpr const char bitsRequired[] =
R"doc(Returns the number of bits required to store integers in the range
``0,...,n-1``. This is simply the number of bits in the binary
expansion of ``n-1``.

If *n* is non-positive then this function will return 0.

Python:
    In Python, this routine fixes the integer type *IntType* to be
    ``long``.

Parameter ``n``:
    any integer.

Returns:
    the number of bits required to store ``0,...,n-1``.)doc";

// Docstring regina::python::doc::nextPowerOfTwo
inline constexpr const char nextPowerOfTwo[] =
R"doc(Returns the smallest integer power of two that is greater than or
equal to the given argument *n*.

If *n* is non-positive then this function will return 1.

Python:
    In Python, this routine fixes the integer type *IntType* to be
    ``long``.

.. warning::
    Even though the return value is the same type as the argument *n*,
    this routine may still overflow. For example, if *IntType* is a
    signed char then nextPowerOfTwo(127) will return -128, and if
    *IntType* is an unsigned char then nextPowerOfTwo(255) will return
    0. Be sure that *IntType* is large enough for your requirements.

Parameter ``n``:
    any integer.

Returns:
    the smallest integer power of two that is ``≥ n``.)doc";

// Docstring regina::python::doc::supportsNativeIntegerSize
inline constexpr const char supportsNativeIntegerSize[] =
R"doc(Determines whether Regina is able to access native C++ integers of the
given size.

At present, this _will_ return ``True`` if *bytes* is a power of two
with ``bytes ≤ 8``; this _might_ return ``True`` if ``bytes = 16``
(depending on whether the platform supports native 128-bit
arithmetic); and this will _not_ return ``True`` for any other value
of ``bytes``.

Parameter ``bytes``:
    the number of bytes in the native integers being queried.

Returns:
    ``True`` if and only if Regina is able to access native C++
    integers with exactly this number of bytes.)doc";

struct AnyInteger {

// Docstring regina::python::doc::AnyInteger::__concept
static constexpr const char __concept[] =
R"doc(Either any standard non-boolean C++ integer type or any of Regina's
own integer types.

This concept excludes ``bool``, and does not make any special
accommodations for 128-bit integer compiler extensions (which are not
standard C++). This means that 128-bit integers might or might not
pass this test, depending on your compiler.

Concepts:
    AnyInteger is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct AnyInteger

struct ArbitraryPrecisionInteger {

// Docstring regina::python::doc::ArbitraryPrecisionInteger::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's arbitrary precision integer types (Integer or
LargeInteger).

Concepts:
    ArbitraryPrecisionInteger is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct ArbitraryPrecisionInteger

struct CppInteger {

// Docstring regina::python::doc::CppInteger::__concept
static constexpr const char __concept[] =
R"doc(A native non-boolean C++ integer type, allowing for 128-bit integers
also if these are supported by the compiler.

Except for booleans, this concept is satisfied precisely when either
``std::is_integral_v<T>`` is true and/or *T* is a native 128-bit
integer.

Regina treats booleans differently: ``CppInteger<bool>`` is false,
even though C++ ``std::is_integral_v<bool>`` is true, since Regina's
functions aim to identify native types that _behave_ like integers
arithmetically.

The main reason for using this concept (as opposed to
``std::is_integral_v<T>``) is because the C++ standard type traits
treat 128-bit integers differently under different compilers.

Concepts:
    CppInteger is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct CppInteger

struct IntegerComparable {

// Docstring regina::python::doc::IntegerComparable::__concept
static constexpr const char __concept[] =
R"doc(A type that supports interoperability with native C++ integer values
via construction, assignment, equality/inequality testing, and
comparisons. The comparisons must yield a total order.

Concepts:
    IntegerComparable is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct IntegerComparable

struct IntegerCompatible {

// Docstring regina::python::doc::IntegerCompatible::__concept
static constexpr const char __concept[] =
R"doc(A type that supports very basic interoperability with native C++
integer values, via construction, assignment, and equality/inequality
testing.

Concepts:
    IntegerCompatible is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct IntegerCompatible

struct ReginaInteger {

// Docstring regina::python::doc::ReginaInteger::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's own integer types (Integer, LargeInteger, or
NativeInteger).

An important feature of all of Regina's integer types is that their
default constructors initialise the integers to zero.

Concepts:
    ReginaInteger is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct ReginaInteger

struct SignedCppInteger {

// Docstring regina::python::doc::SignedCppInteger::__concept
static constexpr const char __concept[] =
R"doc(A signed native non-boolean C++ integer type, allowing for 128-bit
integers also if these are supported by the compiler.

Except for booleans, this concept is satisfied precisely when (i)
either ``std::is_integral_v<T>`` is true and/or *T* is a native
128-bit integer, and (ii) *T* is a signed type.

Regina treats booleans differently: ``SignedCppInteger<bool>`` is
false, even though C++ ``std::is_integral_v<bool>`` is true, since
Regina's functions aim to identify native types that _behave_ like
integers arithmetically.

The main reason for using this constant (as opposed to
``std::is_integral_v<T>`` and ``std::is_signed_v<T>``) is because the
C++ standard type traits treat 128-bit integers differently under
different compilers.

Concepts:
    SignedCppInteger is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct SignedCppInteger

struct StandardCppInteger {

// Docstring regina::python::doc::StandardCppInteger::__concept
static constexpr const char __concept[] =
R"doc(One of the standard non-boolean C++ integer types, without making any
special accommodations for 128-bit integer compiler extensions.

This concept is exactly like ``std::integral`` but with ``bool``
excluded.

Note that 128-bit integers (which are not standard C++) might or might
not pass this test, depending on your compiler.

Concepts:
    StandardCppInteger is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct StandardCppInteger

struct UnsignedCppInteger {

// Docstring regina::python::doc::UnsignedCppInteger::__concept
static constexpr const char __concept[] =
R"doc(An unsigned native non-boolean C++ integer type, allowing for 128-bit
integers also if these are supported by the compiler.

Except for booleans, this concept is satisfied precisely when (i)
either ``std::is_integral_v<T>`` is true and/or *T* is a native
128-bit integer, and (ii) *T* is an unsigned type.

Regina treats booleans differently: ``UnsignedCppInteger<bool>`` is
false, even though C++ ``std::is_integral_v<bool>`` and
``std::is_unsigned_v<bool>`` are both true, since Regina's functions
aim to identify native types that _behave_ like integers
arithmetically.

The main reason for using this constant (as opposed to
``std::is_integral_v<T>`` and ``std::is_unsigned_v<T>``) is because
the C++ standard type traits treat 128-bit integers differently under
different compilers.

Concepts:
    UnsignedCppInteger is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct UnsignedCppInteger

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

