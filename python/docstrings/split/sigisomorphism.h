/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SigPartialIsomorphism
static const char *SigPartialIsomorphism =
R"doc(Represents a partial isomorphism between two splitting surface
signatures. See class Signature for details on splitting surface
signatures.

The two signatures related by this partial isomorphism must have the
same cycle structure, i.e., the same number of cycle groups and the
same cycle length and number of cycles within each cycle group.

The partial isomorphism maps symbols to symbols and cycles to cycles,
with the option of rotating some cycles and/or reversing all cycles in
the process. Cycles within the *k*th cycle group of the source
signature must map to cycles within the *k*th cycle group of the
destination signature.

A *partial* isomorphism is only required to map the cycles and symbols
found in the first *g* cycle groups of the source isomorphism (for
some *g*). If only a subset of symbols are mapped, that subset must be
symbols 0,1,...,*k* for some *k*.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the given partial isomorphisms.

This global routine simply calls SigPartialIsomorphism::swap(); it is
provided so that SigPartialIsomorphism meets the C++ Swappable
requirements.

Parameter ``a``:
    the first partial isomorphism whose contents should be swapped.

Parameter ``b``:
    the second partial isomorphism whose contents should be swapped.)doc";

namespace SigPartialIsomorphism_ {

// Docstring regina::python::doc::SigPartialIsomorphism_::SigPartialIsomorphism
static const char *SigPartialIsomorphism =
R"doc(Creates a new partial isomorphism that maps no cycles or symbols. This
empty isomorphism is designed to be extended at some later point.

Parameter ``newDir``:
    positive if this isomorphism specifies that all cycles are
    reversed, or negative if this isomorphism specifies that all
    cycles keep their original direction.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::SigPartialIsomorphism_2
static const char *SigPartialIsomorphism_2 =
R"doc(Creates a copy of the given partial isomorphism.

Parameter ``iso``:
    the partial isomorphism to copy.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::__eq
static const char *__eq =
R"doc(Determines whether this and the given partial isomorphism are
identical.

Parameter ``other``:
    the partial isomorphism to compare with this.

Returns:
    ``True`` if and only if this and *other* are identical.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::__ne
static const char *__ne =
R"doc(Determines whether this and the given partial isomorphism are not
identical.

Parameter ``other``:
    the partial isomorphism to compare with this.

Returns:
    ``True`` if and only if this and *other* are not identical.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the results of applying this and the given
isomorphism to the given signature.

Comparisons are done on a cycle-by-cycle basis; comparisons within a
cycle are done as described by Signature::cycleCmp(). Comparison will
not proceed beyond the cycles mapped by this partial isomorphism.

Precondition:
    the given partial isomorphism maps at least as many cycles and
    symbols as this partial isomorphism.

Parameter ``sig``:
    the signature to which both this and the given isomorphism will be
    applied.

Parameter ``other``:
    the isomorphism to compare with this isomorphism.

Parameter ``fromCycleGroup``:
    the first cycle group whose images should be examined. If it is
    already known that the cycle images for the first *k* cycle groups
    are identical under both isomorphisms, *k* should be passed in
    this parameter. This parameter should not exceed the number of
    cycle groups whose cycles are mapped by this partial isomorphism.

Returns:
    -1, 1 or 0 if the image of the given signature under this
    isomorphism is lexicographically less than, greater than or equal
    to its image under the given isomorphism respectively.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::compareWithIdentity
static const char *compareWithIdentity =
R"doc(Lexicographically compares the results of applying this and the
identity isomorphism to the given signature.

This routine behaves identically to compareWith(), except that it does
not take a second isomorphism to compare against. See compareWith()
for further details.

Parameter ``sig``:
    the signature to which this isomorphism will be applied.

Parameter ``fromCycleGroup``:
    the first cycle group whose images should be examined. If it is
    already known that the cycle images for the first *k* cycle groups
    are identical under both this and the identity isomorphism, *k*
    should be passed in this parameter. This parameter should not
    exceed the number of cycle groups whose cycles are mapped by this
    partial isomorphism.

Returns:
    -1, 1 or 0 if the image of the given signature under this
    isomorphism is lexicographically less than, greater than or equal
    to its image under the identity isomorphism respectively.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::makeCanonical
static const char *makeCanonical =
R"doc(Rearranges the cycle images so that this isomorphism when applied to
the given signature produces a new signature that is in canonical
form.

The result of this routine is dependent upon the symbol map defined by
this isomorphism (this symbol map will not be changed).

Parameter ``sig``:
    the signature to which this isomorphism will be applied.

Parameter ``fromCycleGroup``:
    the first cycle group whose images may be rearranged. If it is
    already known that the cycle images for the first *k* cycle groups
    are correct, *k* should be passed in this parameter. This
    parameter should not exceed the number of cycle groups whose
    cycles are mapped by this partial isomorphism.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given partial isomorphism.

It does not matter if this and the given partial isomorphism work with
different numbers of symbols or different cycle structures; if they do
then they will both be adjusted accordingly.

Parameter ``other``:
    the partial isomorphism whose contents are to be swapped with
    this.)doc";

}

namespace SigPartialIsomorphism_::ShorterCycle_ {

// Docstring regina::python::doc::SigPartialIsomorphism_::ShorterCycle_::ShorterCycle
static const char *ShorterCycle =
R"doc(Creates a new comparison function.

Parameter ``newSig``:
    the signature containing the cycles that this function will
    examine.

Parameter ``newIso``:
    the partial isomorphism to apply to the cycles before they are
    compared.)doc";

// Docstring regina::python::doc::SigPartialIsomorphism_::ShorterCycle_::__call
static const char *__call =
R"doc(Determines whether the image of one cycle is lexicographically less
than the image of another. See the class notes for further details on
how this comparison is done.

Parameter ``cycle1``:
    the index of the first cycle to examine; this must be less than
    the total number of cycles mapped by the isomorphism concerned and
    less than the total number of cycles in the signature concerned.

Parameter ``cycle2``:
    the index of the second cycle to examine; this must be less than
    the total number of cycles mapped by the isomorphism concerned and
    less than the total number of cycles in the signature concerned.

Returns:
    ``True`` if and only if the image of the first cycle is less than
    the image of the second cycle.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

