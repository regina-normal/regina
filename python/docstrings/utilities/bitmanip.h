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
R"doc(An optimised class for bitwise analysis and manipulation of native C++
integer types.

Whilst BitManipulator has a generic implementation, all or most native
types *T* have template specialisations that are carefully optimised
(precisely what gets specialised depends upon properties of the
compiler).

Python:
    For Python users, the class BitManipulator represents the C++ type
    ``BitManipulator<unsigned long>``. In particular, you should be
    aware that BitManipulator is designed specifically to work with
    native C++ integer types, and _cannot_ handle Python's arbitrary-
    precision integers. It is up to you to ensure that any Python
    integers that you pass into the BitManipulator routines are small
    enough to fit inside a C++ ``unsigned long``.

Template parameter ``T``:
    the native unsigned C++ integer type to work with. The number of
    bits in *T* must be a power of two (which is true in practice for
    all native integer types on all typical modern hardware).)doc";

namespace BitManipulator_ {

// Docstring regina::python::doc::BitManipulator_::bits
static const char *bits =
R"doc(Returns the number of bits that are set to 1 in the given integer.

The implementation uses ``std::popcount()`` where possible, and a
hand-rolled implementation where ``std::popcount()`` might be
unavailable (e.g., for 128-bit integers).

Parameter ``x``:
    the integer of type *T* to examine.

Returns:
    the number of bits that are set.)doc";

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

// Docstring regina::python::doc::BitManipulator_::swapBits
static const char *swapBits =
R"doc(Returns a copy of the given integer with two bits swapped. Bits are
indexed from 0 upwards, starting at the least significant bit.

The two indices *index0* and *index1* may be the same (in which case
the given bitmask will be returned unchanged).

Parameter ``x``:
    the bitmask to examine.

Parameter ``index0``:
    the index of the first bit to swap.

Parameter ``index1``:
    the index of the second bit to swap.

Returns:
    a copy of *x* with bits *index0* and *index1* swapped.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

