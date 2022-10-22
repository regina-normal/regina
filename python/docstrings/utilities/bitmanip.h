/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BitManipulator
static const char *BitManipulator =
R"doc(An optimised class for bitwise analysis and manipulation of native
data types.

The class BitManipulator<T> is used to manipulate an integer of type
*T* as a sequence of bits. Here *T* must be an unsigned native integer
type such as unsigned char, unsigned int, or unsigned long long.

Whilst BitManipulator has a generic implementation, all or most native
types *T* have template specialisations that are carefully optimised
(precisely what gets specialised depends upon properties of the
compiler).

Precondition:
    Type *T* is an unsigned integral numeric type whose size in bits
    is a power of two.

Python:
    For Python users, the class BitManipulator represents the C++ type
    BitManipulator<unsigned long>. In particular, you should be aware
    that BitManipulator is designed specifically to work with native
    C++ integer types, and _cannot_ handle Python's arbitrary-
    precision integers. It is up to you to ensure that any Python
    integers that you pass into the BitManipulator routines are small
    enough to fit inside a C++ unsigned long.)doc";

namespace BitManipulator_ {

// Docstring regina::python::doc::BitManipulator_::firstBit
static const char *firstBit =
R"doc(Returns the index of the first ``True`` bit in the given integer, or
-1 if the given integer is zero. Bits are indexed from 0 upwards,
starting at the least significant bit.

Parameter ``x``:
    the integer of type *T* to examine.

Returns:
    the position of the first ``True`` bit, or -1 if there are no
    ``True`` bits.)doc";

// Docstring regina::python::doc::BitManipulator_::lastBit
static const char *lastBit =
R"doc(Returns the index of the last ``True`` bit in the given integer, or -1
if the given integer is zero. Bits are indexed from 0 upwards,
starting at the least significant bit.

Parameter ``x``:
    the integer of type *T* to examine.

Returns:
    the position of the last ``True`` bit, or -1 if there are no
    ``True`` bits.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

