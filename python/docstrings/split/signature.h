/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Signature
static const char *Signature =
R"doc(Represents a signature of a splitting surface in a closed 3-manifold
triangulation.

A _splitting surface_ is (for these purposes) a compact normal surface
consisting of precisely one quad per tetrahedron and no other normal
(or almost normal) discs.

A _signature_ of order *n* is a string consisting of 2*n* letters
arranged into cycles, where *n* is the number of quads in the
splitting surface. From a signature, the corresponding splitting
surface and then the entire 3-manifold triangulation can be recreated.

A signature of order *n* uses the first *n* letters of the alphabet,
each precisely twice. Case is important; the meaning of a letter
changes according to whether it appears in upper-case or lower-case.

Each letter represents an individual quadrilateral (the two
occurrences of the letter representing the quadrilateral's two sides).
Each cycle represents a chain of quadrilaterals joined together in the
splitting surface. The case of a letter represents in which direction
a quadrilateral is traversed within a cycle.

Cycles are arranged into _cycle groups_, where a cycle group consists
of a series of consecutive cycles all of the same length.

An example of a signature is ``(abc)(a)(b)(c)``. This signature is of
order 3 and contains two cycle groups, the first being ``(abc)`` and
the second being ``(a)(b)(c)``.

A signature cannot represent a splitting surface with more than 26
quadrilaterals.

For further details on splitting surfaces and their signatures,
consult _Minimal triangulations and normal surfaces_, Burton, PhD
thesis, available from the Regina website.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace Signature_ {

// Docstring regina::python::doc::Signature_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given signature.

Parameter ``sig``:
    the signature to copy.)doc";

// Docstring regina::python::doc::Signature_::__eq
static const char *__eq =
R"doc(Determines whether this and the given signature are identical.

To be considered identical, it is not enough for two signatures to be
isomorphic: their cycles and cycle groups must be presented in the
same order, using the same symbols which must likewise be presented in
the same order.

If either signature was parsed from a string, any choice of formatting
(e.g., punctuation and/or whitespace) is irrelevant; only the
mathematical content of the signatures is important here.

Parameter ``other``:
    the signature to compare with this.

Returns:
    ``True`` if and only if this and *other* are identical.)doc";

// Docstring regina::python::doc::Signature_::__init
static const char *__init =
R"doc(Creates a new signature by parsing the given signature string.

Punctuation characters in the given string will be interpreted as
separating cycles. All whitespace will be ignored.

Examples of valid signatures are ``"(ab)(bC)(Ca)"`` and ``"AAb-
bc-C"``. See the class notes for further details on what constitutes a
valid signature.

Exception ``InvalidArgument``:
    The given string was not a valid signature with a positive number
    of letters.

Parameter ``str``:
    a string representation of a splitting surface signature.)doc";

// Docstring regina::python::doc::Signature_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given signatures.

This global routine simply calls Signature::swap(); it is provided so
that Signature meets the C++ Swappable requirements.

Parameter ``a``:
    the first signature whose contents should be swapped.

Parameter ``b``:
    the second signature whose contents should be swapped.)doc";

// Docstring regina::python::doc::Signature_::order
static const char *order =
R"doc(Returns the order of this signature. The order is the number of quads
in the corresponding splitting surface.

Returns:
    the order of this signature.)doc";

// Docstring regina::python::doc::Signature_::str
static const char *str =
R"doc(Returns a customised string representation of this signature.

Note that there is also a zero-argument version of str(), inherited
through the ShortOutput base class. This zero-argument str() make
sensible default choices for the three arguments required here.

Parameter ``cycleOpen``:
    the text to write at the beginning of each cycle (such as
    ``"("``).

Parameter ``cycleClose``:
    the text to write at the end of each cycle (such as ``")"``).

Parameter ``cycleJoin``:
    the text to write between each pair of consecutive cycles.)doc";

// Docstring regina::python::doc::Signature_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given signature.

It does not matter if this and the given signature use different
number of symbols, cycles and/or cycle groups; if so then both
signatures will be adjusted accordingly.

Parameter ``other``:
    the signature whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::Signature_::triangulate
static const char *triangulate =
R"doc(Returns the 3-manifold triangulation corresponding to this splitting
surface signature.

Returns:
    the corresponding triangulation.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

