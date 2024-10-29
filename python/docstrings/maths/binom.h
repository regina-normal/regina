/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::binomMedium
static const char *binomMedium =
R"doc(Returns the binomial coefficient *n* choose *k* in linear time for
medium-sized arguments (*n* ≤ 61).

This routine computes the binomial coefficient using the standard
formula. It works entirely with native integers of a large enough
size; the constraint *n* ≤ 61 is designed to avoid overflow (since all
intermediate results are guaranteed to stay below 2^63).

If *n* ≤ 16 then this routine will use the same constant-time lookup
as binomSmall() (i.e., there is no penalty for calling this routine
with very small arguments).

If *k* is outside the usual range (i.e., *k* is negative or greater
than *n*), then this routine will return 0.

Exception ``InvalidArgument``:
    The argument *n* is negative or greater than 61.

Parameter ``n``:
    the parameter *n* in (*n* choose *k*); this must be between 0 and
    61 inclusive.

Parameter ``k``:
    the parameter *k* in (*n* choose *k*).

Returns:
    the binomial coefficient *n* choose *k*.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

