/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::binomMedium
static const char *binomMedium =
R"doc(Returns the binomial coefficient *n* choose *k* in linear time for
medium-sized arguments (*n* ≤ 29).

This routine computes the binomial coefficient using the standard
formula. It works entirely with native long integers; the constraint
*n* ≤ 29 is designed to avoid overflow (since all intermediate results
are guaranteed to stay below 2^31).

If *n* ≤ 16 then this routine will use the same constant-time lookup
as binomSmall() (i.e., there is no penalty for calling this routine
with very small arguments).

Parameter ``n``:
    the parameter *n* in (*n* choose *k*); this must be between 0 and
    29 inclusive.

Parameter ``k``:
    the parameter *k* in (*n* choose *k*); this must be between 0 and
    *n* inclusive.

Returns:
    the binomial coefficient *n* choose *k*.)doc";

// Docstring regina::python::doc::binomSmall
static const char *binomSmall =
R"doc(Returns the binomial coefficient *n* choose *k* in constant time for
small arguments (*n* ≤ 16).

This routine is very fast, since it uses a constant-time lookup. The
trade-off is that it can only be used for *n* ≤ 16.

.. note::
    The constraint *n* ≤ 16 is large enough for working with
    triangulations in Regina, since Regina restricts its
    triangulations to dimension ≤ 15 (where each simplex has 16
    vertices).

Parameter ``n``:
    the parameter *n* in (*n* choose *k*); this must be between 0 and
    16 inclusive.

Parameter ``k``:
    the parameter *k* in (*n* choose *k*); this must be between 0 and
    *n* inclusive.

Returns:
    the binomial coefficient *n* choose *k*.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

