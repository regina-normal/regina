/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LensSpace
static const char *LensSpace =
R"doc(Represents a general lens space.

The lens space L(*p*,*q*) is the 3-manifold you get by *p*/q Dehn
surgery on the unknot. For instance, L(1,0) and L(1,1) are the
3-sphere, L(0,1) is the product S^1 x S^2, and L(*p*,1) is the circle
bundle over S^2 with Euler class *p*. In L(*p*,*q*) if you take a
generator *g* of H_1 and evaluate the torsion linking form on it, then
<*g*,*g*> = [+/- *r*^2 *q*/*p*] in Q/Z where *r* is an integer.

All optional Manifold routines are implemented for this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
manifold classes.)doc";

namespace LensSpace_ {

// Docstring regina::python::doc::LensSpace_::__copy
static const char *__copy = R"doc(Creates a new copy of the given lens space.)doc";

// Docstring regina::python::doc::LensSpace_::__eq
static const char *__eq =
R"doc(Determines whether this and the given lens space have the same
presentation.

Since the presentation is made canonical by the class constructor,
this routine also identifies whether this and the given lens space are
homeomorphic as 3-manifolds. This is in contrast to the comparison
operators for other manifold classes (such as Seifert fibred spaces
and graph manifolds), where the same manifold could have different
presentations that compare as not equal.

Parameter ``compare``:
    the lens space with which this should be compared.

Returns:
    ``True`` if and only if this and the given lens space have the
    same presentation (i.e., are homeomorphic).)doc";

// Docstring regina::python::doc::LensSpace_::__init
static const char *__init =
R"doc(Creates a new lens space with the given parameters. See the class
notes for details.

Precondition:
    The two given parameters are coprime (have a gcd of 1).

Parameter ``p``:
    the first parameter *p* of the lens space L(p,q).

Parameter ``q``:
    the second parameter *q* of the lens space L(p,q). Note that there
    are no range restrictions whatsoever on this parameter.)doc";

// Docstring regina::python::doc::LensSpace_::__ne
static const char *__ne =
R"doc(Determines whether this and the given lens space have different
presentations.

Since the presentation is made canonical by the class constructor,
this routine also identifies whether this and the given lens space are
homeomorphic as 3-manifolds. This is in contrast to the comparison
operators for other manifold classes (such as Seifert fibred spaces
and graph manifolds), where the same manifold could have different
presentations that compare as not equal.

Parameter ``compare``:
    the lens space with which this should be compared.

Returns:
    ``True`` if and only if this and the given lens space have
    different presentations (i.e., are non-homeomorphic).)doc";

// Docstring regina::python::doc::LensSpace_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given lens spaces.

This global routine simply calls LensSpace::swap(); it is provided so
that LensSpace meets the C++ Swappable requirements.

Parameter ``a``:
    the first lens space whose contents should be swapped.

Parameter ``b``:
    the second lens space whose contents should be swapped.)doc";

// Docstring regina::python::doc::LensSpace_::p
static const char *p =
R"doc(Returns the first parameter *p* of this lens space L(p,q). See the
class notes for details.

Returns:
    the first parameter.)doc";

// Docstring regina::python::doc::LensSpace_::q
static const char *q =
R"doc(Returns the second parameter *q* of this lens space L(p,q). See the
class notes for details.

The value of *q* returned will be the smallest *q* between 0 and *p*-1
inclusive that produces the same 3-manifold as this lens space. This
means it might not be the value of *q* that was used to initialise
this lens space.)doc";

// Docstring regina::python::doc::LensSpace_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given lens space.

Parameter ``other``:
    the lens space whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

