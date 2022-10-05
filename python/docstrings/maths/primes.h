/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Primes
static const char *Primes =
R"doc(A helper class for finding primes and factorising integers.

This class has two functions: (i) to maintain a list of known primes,
and (ii) to use this list to factorise integers into prime factors.

The primes stored by this class will always be the smallest *k*
suspected primes, where *k* may grow dynamically as the program runs.
Specifically:

* An initial hard-coded list of seed primes is loaded into the class
  on startup. This list contains precisely the smallest 10,000 primes
  (the size of this list is subject to change in future versions of
  Regina).

* Whenever a prime beyond the known list is requested (e.g., when a
  number greater than the largest stored prime is to be factorised),
  the list is extended on the fly. The extension uses the
  probabilistic algorithm shipped with GMP (hence the phrase
  "suspected primes" above); regarding this algorithm, the GMP
  documentation states that "for practical purposes it's adequate, the
  chance of a composite passing will be extremely small."

This list is used by the high-level factorisation routines in this
class, such as primeDecomp() and primePowerDecomp(). For users only
interested in these high-level routines, there is no need to worry
about the size of the list; the high-level routines will extend it if
necessary.

Although this class makes use of global data in its implementation,
all of its methods are thread-safe.

Author:
    Ryan Budney, B.B.)doc";

namespace Primes_ {

// Docstring regina::python::doc::Primes_::prime
static const char *prime =
R"doc(Returns the requested prime (or suspected prime). More specifically,
this routine returns the (*which* + 1)th smallest prime. Thus prime(0)
returns 2, prime(1) returns 3, prime(2) returns 5, and so on.

If *which* is smaller than the number of initial seed primes, the
result is guaranteed to be the (*which* + 1)th smallest prime (see the
Primes class notes for the size of the initial seed list). If *which*
is larger, a probabilistic algorithm is used and so there is a
possibility that non-primes are included in the list.

If *which* < size() then this routine is essentially instantaneous,
since the (*which* + 1)th smallest (suspected) prime is already
stored. Otherwise the behaviour depends on the argument *autoGrow*. If
*autoGrow* is ``True`` (the default) then this routine calculates the
requested prime, which might take some time. If *autoGrow* is
``False`` then this routine returns zero.

Parameter ``which``:
    indicates which prime is requested.

Parameter ``autoGrow``:
    specifies what to do if the requested prime lies beyond the list
    currently stored (see above).

Returns:
    the requested prime (or suspected prime), or zero if *which* was
    too large and *autoGrow* was ``False``.)doc";

// Docstring regina::python::doc::Primes_::primeDecomp
static const char *primeDecomp =
R"doc(Returns the prime factorisation of the given integer as a list of
individual primes (or suspected primes).

Prime factors are returned in increasing order. Where a prime power
appears in the factorisation, the relevant prime will appear several
times in the list.

For very large integers, the factorisation becomes probabilistic: (i)
this routine examines suspected primes instead of primes (see the
class notes), and (ii) if the routine is having trouble finding
factors then it will run a probabilistic prime test on whatever
portion of *n* still remains (and will assume that portion to be prime
if the test passes).

The given integer may be negative, in which case -1 will be listed as
the first factor (even though -1 is not prime). If 0 is passed then a
single factor of 0 will be returned; if 1 is passed then an empty list
will be returned. In all cases, the given integer *n* will be the
product of all elements of the final list (where an empty product is
assumed to be 1).

As an example, the prime factors of 54 will be listed as (2, 3, 3, 3),
and the prime factors of -90 will be listed as (-1, 2, 3, 3, 5).

Note that the internal list of known primes and suspected primes will
be expanded as necessary; there is no need for the caller to manage
this list manually.

Python:
    In addition to this routine, the routine primeDecompInt() is also
    available. The routine primeDecompInt() behaves identically to
    this routine except that the (i) return values are of ordinary
    integer type, not Integer; (ii) the input value *n* must lie
    within the C++ long integer range (otherwise the behaviour is
    undefined).

Parameter ``n``:
    the integer to factorise.

Returns:
    the list of prime factors as described above.)doc";

// Docstring regina::python::doc::Primes_::primePowerDecomp
static const char *primePowerDecomp =
R"doc(Returns the prime factorisation of the given integer as a list of
prime powers (or suspected prime powers).

Factors are returned as (prime, exponent) pairs. Different pairs
describe different primes, and the pairs are sorted in order from
smallest prime to largest. All exponents are strictly positive.

For very large integers, the factorisation becomes probabilistic: (i)
this routine examines suspected primes instead of primes (see the
class notes), and (ii) if the routine is having trouble finding
factors then it will run a probabilistic prime test on whatever
portion of *n* still remains (and will assume that portion to be prime
if the test passes).

The given integer may be negative, in which case (-1,1) will be listed
as the first prime power (even though -1 is not prime). If 0 is passed
then a single pair (0,1) will be returned; if 1 is passed then an
empty list will be returned. In all cases, the given integer *n* will
be the product of all powers described by the final list (where an
empty product is assumed to be 1).

As an example, the factorisation of 54 will be reported as [(2,1)
(3,3)], and the factorisation of -90 will be reported as [(-1,1) (2,1)
(3,2) (5,1)].

Note that the internal list of known primes and suspected primes will
be expanded as necessary; there is no need for the caller to manage
this list manually.

The current implementation of this routine merely calls primeDecomp()
and rewrites the list of factors by grouping primes.

Python:
    In addition to this routine, the routine primePowerDecompInt() is
    also available. The routine primePowerDecompInt() behaves
    identically to this routine except that the (i) return values are
    of ordinary integer type, not Integer; (ii) the input value *n*
    must lie within the C++ long integer range (otherwise the
    behaviour is undefined).

Parameter ``n``:
    the integer to factorise.

Returns:
    the list of prime power factors as described above.)doc";

// Docstring regina::python::doc::Primes_::size
static const char *size =
R"doc(Returns the number of primes (or suspected primes) currently stored.

Primes that are already stored can be accessed instantly; primes
larger than those currently stored must be generated on the fly (which
takes time).

This number may increase as the program runs (according to whether
larger primes are requested), but it will never decrease.

Returns:
    the number of primes or suspected primes currently stored.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

