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
static const char *bitsRequired =
R"doc(Returns the number of bits required to store integers in the range
0,...,*n*-1. This is simply the number of bits in the binary expansion
of *n*-1.

If *n* is non-positive then this function will return 0.

Python:
    In Python, this routine fixes the integer type *IntType* to be
    ``long``.

Template parameter ``IntType``:
    any integer type, such as ``int``, ``long``, and so on.

Parameter ``n``:
    any integer.

Returns:
    the number of bits required to store 0,...,*n*-1.)doc";

// Docstring regina::python::doc::nextPowerOfTwo
static const char *nextPowerOfTwo =
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

Template parameter ``IntType``:
    any integer type, such as ``int``, ``long``, and so on.

Parameter ``n``:
    any integer.

Returns:
    the smallest integer power of two that is ≥ *n*.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

